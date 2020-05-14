#include "pch.h"

#include "../ScreenUtils.h"
#include "DxgiFrame.h"
#include "DxgiDuplicatorController.h"
#include "ScreenCapturerDxgi.h"

namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			namespace {
			bool ScreenListFromDeviceNames(const std::vector<wstr>& device_names,
				ScreenList& screens)
			{
				//DCHECK(screens->empty());

				ScreenList gdi_screens;
				CScreenUtils::GetScreenList(gdi_screens);

				if (gdi_screens.empty())
					return false;

				ScreenId max_screen_id = -1;

				for (const auto& screen : gdi_screens)
					max_screen_id = std::max(max_screen_id, screen.id);

				for (const auto& device_name : device_names)
				{
					bool device_found = false;

					for (const auto& gdi_screen : gdi_screens)
					{
						std::string device_name_utf8 = CommonLib::StringEncoding::str_w2utf8_safe(device_name);

						if (gdi_screen.title == device_name_utf8)
						{
							screens.push_back(gdi_screen);
							device_found = true;

							break;
						}
					}

					if (!device_found)
					{
						// devices_names[i] has not been found in gdi_names, so use max_screen_id.
						++max_screen_id;
						screens.push_back({ max_screen_id, std::string() });
					}
				}

				return true;
			}

			int indexFromScreenId(ScreenId id, const std::vector<wstr>& device_names)
			{
				ScreenList screens;

				if (!ScreenListFromDeviceNames(device_names, screens))
					return -1;

				//DCHECK_EQ(device_names.size(), screens.size());

				for (int i = 0; i < screens.size(); ++i)
				{
					if (screens[i].id == id)
						return i;
				}

				return -1;
			}

		} // namespace

		CScreenCapturerDxgi::CScreenCapturerDxgi()
			: m_controller(std::make_shared<CDxgiDuplicatorController>())
		{
			// Nothing
		}

		CScreenCapturerDxgi::~CScreenCapturerDxgi() = default;

		bool CScreenCapturerDxgi::IsSupported()
		{
			// Forwards isSupported() function call to DxgiDuplicatorController.
			return m_controller->IsSupported();
		}

		// static
		bool CScreenCapturerDxgi::IsCurrentSessionSupported()
		{
			return CDxgiDuplicatorController::IsCurrentSessionSupported();
		}

		int CScreenCapturerDxgi::GetScreenCount()
		{
			return m_controller->ScreenCount();
		}

		void  CScreenCapturerDxgi::GetScreenList(ScreenList& screens)
		{
			std::vector<wstr> device_names;

			if (!m_controller->DeviceNames(device_names))
				throw CommonLib::CExcBase("Failed to get device name");

			ScreenListFromDeviceNames(device_names, screens);
		}

		void CScreenCapturerDxgi::SelectScreen(ScreenId screen_id)
		{
			if (screen_id == kFullDesktopScreenId)
			{
				current_screen_id_ = screen_id;
			}

			std::vector<wstr> device_names;

			if (!m_controller->DeviceNames(device_names))
				throw CommonLib::CExcBase("Failed to select screen %1", screen_id);

			int index = indexFromScreenId(screen_id, device_names);
			if (index == -1)
				throw CommonLib::CExcBase("Failed to select screen %1, invalid index", screen_id);

			current_screen_id_ = index;
		}

		IFramePtr CScreenCapturerDxgi::CaptureFrame()
		{
			try
			{
				m_queue.MoveToNextFrame();

				if (!m_queue.currentFrame().get())
				{
					m_queue.ReplaceCurrentFrame(DxgiFramePtr(new CDxgiFrame(m_controller)));
				}

				CDxgiDuplicatorController::Result result;

				if (current_screen_id_ == kFullDesktopScreenId)
					result = m_controller->Duplicate(m_queue.currentFrame().get());
				else
					result = m_controller->DuplicateMonitor(m_queue.currentFrame().get(), current_screen_id_);

				using DuplicateResult = CDxgiDuplicatorController::Result;

				if (result != DuplicateResult::SUCCEEDED)
				{
					Log.ErrorT("DxgiDuplicatorController failed to capture desktop, error code %1",	CDxgiDuplicatorController::ResultName(result));
				}

				switch (result)
				{
				case DuplicateResult::SUCCEEDED:
				{
					//	*error = Error::SUCCEEDED;
					return m_queue.currentFrame()->Frame();
				}
				case DuplicateResult::UNSUPPORTED_SESSION:
				{
					//	LOG(LS_ERROR) << "Current binary is running on a session not supported "
					//		"by DirectX screen capturer";
					//	*error = Error::PERMANENT;
					//	return nullptr;
					throw	CommonLib::CExcBase("Current binary is running on a session not supported");
				}
				case DuplicateResult::FRAME_PREPARE_FAILED:
				{
					/*LOG(LS_ERROR) << "Failed to allocate a new Frame";
					// This usually means we do not have enough memory or SharedMemoryFactory cannot work
					// correctly.
					*error = Error::PERMANENT;
					return nullptr;*/
					throw	CommonLib::CExcBase("Failed to allocate a new Frame");
				}
				case DuplicateResult::INVALID_MONITOR_ID:
				{
					//LOG(LS_ERROR) << "Invalid monitor id " << current_screen_id_;
					//*error = Error::PERMANENT;
					throw	CommonLib::CExcBase("Invalid monitor id");

				}
				case DuplicateResult::INITIALIZATION_FAILED:
				case DuplicateResult::DUPLICATION_FAILED:
				default:
				{
					throw	CommonLib::CExcBase("initialization failed");
				}
				}
			}
			catch (std::exception& excSrc)
			{
				CommonLib::CExcBase::RegenExc("Failed to capture frame", excSrc);
				throw;
			}
		}

		void CScreenCapturerDxgi::Reset()
		{
			m_queue.Reset();
		}

		CPixelFormat CScreenCapturerDxgi::GetPixelFormat() const
		{
			return CPixelFormat();
		}
		}
	}
}