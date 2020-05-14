#include "pch.h"
#include "../../geometry.h"
#include "../../DataFrame.h"
#include "DxgiFrame.h"
#include "DxgiContext.h"
#include "DxgiDuplicatorController.h"


namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			const char* CDxgiDuplicatorController::ResultName(CDxgiDuplicatorController::Result result)
			{
				switch (result)
				{
				case Result::SUCCEEDED:
					return "Succeeded";

				case Result::UNSUPPORTED_SESSION:
					return "Unsupported session";

				case Result::FRAME_PREPARE_FAILED:
					return "Frame preparation failed";

				case Result::INITIALIZATION_FAILED:
					return "Initialization failed";

				case Result::DUPLICATION_FAILED:
					return "Duplication failed";

				case Result::INVALID_MONITOR_ID:
					return "Invalid monitor id";

				default:
					return "Unknown error";
				}
			}

			// static
			bool CDxgiDuplicatorController::IsCurrentSessionSupported()
			{
				DWORD session_id = 0;

				if (!ProcessIdToSessionId(GetCurrentProcessId(), &session_id))
				{
					Log.Warning("Failed to retrieve current session Id, current binary may not have required priviledge");
					return false;
				}

				return session_id != 0;
			}

			CDxgiDuplicatorController::CDxgiDuplicatorController() = default;

			CDxgiDuplicatorController::~CDxgiDuplicatorController()
			{
				Deinitialize();
			}

			bool CDxgiDuplicatorController::IsSupported()
			{
				return Initialize();
			}

			bool CDxgiDuplicatorController::RetrieveD3dInfo(D3dInfo* info)
			{
				bool result = false;
				{
					result = Initialize();
					*info = m_d3dInfo;
				}

				if (!result)
				{
					Log.Warning("Failed to initialize DXGI components, the D3dInfo retrieved may not accurate or out of date");
				}

				return result;
			}

			CDxgiDuplicatorController::Result CDxgiDuplicatorController::Duplicate(CDxgiFrame* frame)
			{
				return DoDuplicate(frame, -1);
			}

			CDxgiDuplicatorController::Result CDxgiDuplicatorController::DuplicateMonitor(
				CDxgiFrame* frame, int monitor_id)
			{
				return DoDuplicate(frame, monitor_id);
			}

			int CDxgiDuplicatorController::ScreenCount()
			{
				if (Initialize())
					return DoScreenCount();

				return 0;
			}

			bool CDxgiDuplicatorController::DeviceNames(std::vector<wstr>& output)
			{

				if (Initialize())
				{
					for (const auto& duplicator : m_duplicators)
					{
						for (int i = 0; i < duplicator.ScreenCount(); ++i)
							output.push_back(duplicator.DeviceName(i));
					}

					return true;
				}

				return false;
			}

			CDxgiDuplicatorController::Result CDxgiDuplicatorController::DoDuplicate(
				CDxgiFrame* frame, int monitor_id)
			{

				// The dxgi components and APIs do not update the screen resolution without a reinitialization.
				// So we use the GetDC() function to retrieve the screen resolution to decide whether dxgi
				// components need to be reinitialized.
				// If the screen resolution changed, it's very likely the next duplicate() function call will
				// fail because of a missing monitor or the frame size is not enough to store the output. So we
				// reinitialize dxgi components in-place to avoid a capture failure.
				// But there is no guarantee GetDC() function returns the same resolution as dxgi APIs, we
				// still rely on dxgi components to return the output frame size.
				// TODO(zijiehe): Confirm whether IDXGIOutput::GetDesc() and IDXGIOutputDuplication::GetDesc()
				// can detect the resolution change without reinitialization.
				if (m_displayConfigurationMonitor.IsChanged())
					Deinitialize();

				if (!Initialize())
				{
					if (m_succeededDuplications == 0 && !IsCurrentSessionSupported())
					{
						Log.Warning("Current binary is running in session 0. DXGI components cannot be initialized");
						return Result::UNSUPPORTED_SESSION;
					}

					// Cannot initialize COM components now, display mode may be changing.
					return Result::INITIALIZATION_FAILED;
				}

				///if (!frame->prepare(selectedDesktopSize(monitor_id), monitor_id))
				//	return Result::FRAME_PREPARE_FAILED;
				frame->Prepare(SelectedDesktopSize(monitor_id), monitor_id);

				frame->Frame()->UpdatedRegion()->Clear();

				Setup(frame->GetContext());

				if (EnsureFrameCaptured(frame->GetContext(), frame->Frame()))
				{
					bool result;

					if (monitor_id < 0)
					{
						// Capture entire screen.
						result = DoDuplicateAll(frame->GetContext(), frame->Frame());
					}
					else
					{
						result = DoDuplicateOne(frame->GetContext(), monitor_id, frame->Frame());
					}

					if (result)
					{
						++m_succeededDuplications;
						return Result::SUCCEEDED;
					}
				}

				if (monitor_id >= DoScreenCount())
				{
					// It's a user error to provide a |monitor_id| larger than screen count. We do not need to
					// deinitialize.
					return Result::INVALID_MONITOR_ID;
				}

				// If the |monitor_id| is valid, but doDuplicateAll() or doDuplicateOne failed, something must
				// be wrong from capturer APIs. We should deinitialize().
				Deinitialize();
				return Result::DUPLICATION_FAILED;
			}

			void CDxgiDuplicatorController::Unregister(const Context* const context)
			{
				if (ContextExpired(context))
				{
					// The Context has not been setup after a recent initialization, so it should not been
					// registered in duplicators.
					return;
				}

				for (size_t i = 0; i < m_duplicators.size(); ++i)
					m_duplicators[i].Unregister(&context->m_contexts[i]);
			}

			bool CDxgiDuplicatorController::Initialize()
			{
				if (!m_duplicators.empty())
					return true;

				if (DoInitialize())
					return true;

				Deinitialize();
				return false;
			}

			bool CDxgiDuplicatorController::DoInitialize()
			{

				m_d3dInfo.min_feature_level = static_cast<D3D_FEATURE_LEVEL>(0);
				m_d3dInfo.max_feature_level = static_cast<D3D_FEATURE_LEVEL>(0);

				std::vector<CD3dDevice> devices = CD3dDevice::EnumDevices();
				if (devices.empty())
				{
					Log.Warning("No D3dDevice found");
					return false;
				}

				for (size_t i = 0; i < devices.size(); ++i)
				{
					D3D_FEATURE_LEVEL feature_level = devices[i].D3dDevice()->GetFeatureLevel();

					if (m_d3dInfo.max_feature_level == 0 || feature_level > m_d3dInfo.max_feature_level)
						m_d3dInfo.max_feature_level = feature_level;

					if (m_d3dInfo.min_feature_level == 0 || feature_level < m_d3dInfo.min_feature_level)
						m_d3dInfo.min_feature_level = feature_level;

					CDxgiAdapterDuplicator duplicator(devices[i]);
					// There may be several video cards on the system, some of them may not support
					// IDXGOutputDuplication. But they should not impact others from taking effect, so we
					// should continually try other adapters. This usually happens when a non-official virtual
					// adapter is installed on the system.
					if (!duplicator.Initialize())
					{
						//LOG(LS_WARNING) << "Failed to initialize DxgiAdapterDuplicator on adapter " << i;
						continue;
					}

					//DCHECK(!duplicator.desktopRect().isEmpty());
					m_duplicators.push_back(std::move(duplicator));

					m_desktop_rect.UnionWith(m_duplicators.back().DesktopRect());
				}

				TranslateRect();

				++m_identity;

				if (m_duplicators.empty())
				{
					Log.Warning("Cannot initialize any DxgiAdapterDuplicator instance");
				}

				return !m_duplicators.empty();
			}

			void CDxgiDuplicatorController::Deinitialize()
			{
				m_desktop_rect = CRect();
				m_duplicators.clear();
				m_displayConfigurationMonitor.Reset();
			}

			bool CDxgiDuplicatorController::ContextExpired(const Context* const context) const
			{
				return context->m_controllerId != m_identity || context->m_contexts.size() != m_duplicators.size();
			}

			void CDxgiDuplicatorController::Setup(Context* context)
			{
				if (ContextExpired(context))
				{

					context->m_contexts.clear();
					context->m_contexts.resize(m_duplicators.size());

					for (size_t i = 0; i < m_duplicators.size(); ++i)
						m_duplicators[i].Setup(&context->m_contexts[i]);

					context->m_controllerId = m_identity;
				}
			}

			bool CDxgiDuplicatorController::DoDuplicateAll(Context* context, IFramePtr target)
			{
				for (size_t i = 0; i < m_duplicators.size(); ++i)
				{
					if (!m_duplicators[i].Duplicate(&context->m_contexts[i], target))
						return false;
				}

				return true;
			}

			bool CDxgiDuplicatorController::DoDuplicateOne(Context* context, int monitor_id, IFramePtr target)
			{

				for (size_t i = 0; i < m_duplicators.size() && i < context->m_contexts.size(); ++i)
				{
					if (monitor_id >= m_duplicators[i].ScreenCount())
					{
						monitor_id -= m_duplicators[i].ScreenCount();
					}
					else
					{
						return m_duplicators[i].DuplicateMonitor(&context->m_contexts[i], monitor_id, target);
					}
				}

				return false;
			}

			int64_t CDxgiDuplicatorController::NumFramesCaptured() const
			{
				int64_t min = std::numeric_limits<int64_t>::max();

				for (const auto& duplicator : m_duplicators)
					min = std::min(min, duplicator.NumFramesCaptured());

				return min;
			}

			CSize CDxgiDuplicatorController::DesktopSize() const
			{
				return m_desktop_rect.Size();
			}

			CRect CDxgiDuplicatorController::ScreenRect(int id) const
			{

				for (size_t i = 0; i < m_duplicators.size(); ++i)
				{
					if (id >= m_duplicators[i].ScreenCount())
						id -= m_duplicators[i].ScreenCount();
					else
						return m_duplicators[i].ScreenRect(id);
				}

				return CRect();
			}

			int CDxgiDuplicatorController::DoScreenCount() const
			{
				int result = 0;

				for (const auto& duplicator : m_duplicators)
					result += duplicator.ScreenCount();

				return result;
			}

			CSize CDxgiDuplicatorController::SelectedDesktopSize(int monitor_id) const
			{
				if (monitor_id < 0)
					return DesktopSize();

				return ScreenRect(monitor_id).Size();
			}

			bool CDxgiDuplicatorController::EnsureFrameCaptured(Context* context, IFramePtr target)
			{
				using Clock = std::chrono::high_resolution_clock;
				using TimePoint = std::chrono::time_point<Clock>;
				using Milliseconds = std::chrono::milliseconds;

				// On a modern system, the FPS / monitor refresh rate is usually larger than
				// or equal to 60. So 17 milliseconds is enough to capture at least one frame.
				const Milliseconds ms_per_frame(17);

				// Skips the first frame to ensure a full frame refresh has happened before
				// this function returns.
				const int64_t frames_to_skip = 1;

				// The total time out milliseconds for this function. If we cannot get enough
				// frames during this time interval, this function returns false, and cause
				// the DXGI components to be reinitialized. This usually should not happen
				// unless the system is switching display mode when this function is being
				// called. 500 milliseconds should be enough for ~30 frames.
				const Milliseconds timeout_ms(500);

				if (NumFramesCaptured() >= frames_to_skip)
					return true;

				IFramePtr fallback_frame;
				IFramePtr shared_frame ;

				if (target->Size().Width() >= DesktopSize().Width() &&
					target->Size().Height() >= DesktopSize().Height())
				{
					// |target| is large enough to cover entire screen, we do not need to use |fallback_frame|.
					shared_frame = target;
				}
				else
				{
					fallback_frame.reset(new CDataFrame(CPixelFormat::ARGB(), DesktopSize()));
					shared_frame = fallback_frame;
				}

				const TimePoint start_ms = Clock::now();
				TimePoint last_frame_start_ms;

				while (NumFramesCaptured() < frames_to_skip)
				{
					if (NumFramesCaptured() > 0)
					{
						// Sleep |ms_per_frame| before capturing next frame to ensure the screen has been
						// updated by the video adapter.
						std::this_thread::sleep_for(ms_per_frame - (Clock::now() - last_frame_start_ms));
					}

					last_frame_start_ms = Clock::now();

					if (!DoDuplicateAll(context, shared_frame))
						return false;

					if (Clock::now() - start_ms > timeout_ms)
					{
						Log.ErrorT("Failed to capture %1 frames within %2 milliseconds", frames_to_skip, timeout_ms.count());
						return false;
					}
				}

				return true;
			}

			void CDxgiDuplicatorController::TranslateRect()
			{
				const CPoint position = CPoint().Subtract(m_desktop_rect.TopLeft());

				m_desktop_rect.Translate(position);

				for (auto& duplicator : m_duplicators)
					duplicator.TranslateRect(position);
			}
		}
	}
}