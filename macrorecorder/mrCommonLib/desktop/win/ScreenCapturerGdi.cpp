#include "pch.h"
#include "ScreenCapturerGdi.h"
#include "ScreenUtils.h"
#include "FrameDib.h"
#include "GdiObject.h"
#pragma warning(disable:4995)

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			CScreenCapturerGdi::CScreenCapturerGdi()
			{

			}

			CScreenCapturerGdi::~CScreenCapturerGdi()
			{
				if (m_composition_changed)
					DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
			}


			IFramePtr CScreenCapturerGdi::CaptureFrame()
			{
				try
				{
					m_queue.MoveToNextFrame();
					PrepareCaptureResources();

					CRect screen_rect = CScreenUtils::ScreenRect(m_current_screen_id, m_current_device_key);
					if (screen_rect.IsEmpty())
						throw CommonLib::CWinExc("Failed to get screen rect");


					if (!m_queue.currentFrame().get() || m_queue.currentFrame()->Size() != screen_rect.Size())
					{

						IFramePtr frame = CFrameDib::Create(screen_rect.Size(), m_PixelFormat, m_memoryDC);
						if (!frame.get())
							throw CommonLib::CExcBase("Failed to create frame buffer");					

						m_queue.ReplaceCurrentFrame(frame);
					}

					IFramePtr current = m_queue.currentFrame();
					IFramePtr previous = m_queue.previousFrame();

					{
						CScopedSelectObject select_object(m_memoryDC, static_cast<CFrameDib*>(current.get())->Bitmap());

						BitBlt(m_memoryDC,
							0, 0,
							screen_rect.Width(), screen_rect.Height(),
							m_winDesktopDC,
							screen_rect.Left(), screen_rect.Top(),
							CAPTUREBLT | SRCCOPY);
					}

					if (!previous || previous->Size() != current->Size())
					{
						m_differ = std::make_unique<CDiffer>(screen_rect.Size(), m_PixelFormat);
						current->UpdatedRegion().AddRect(CRect::MakeSize(screen_rect.Size()));
					}
					else
					{
						m_differ->CalcDirtyRegion(previous->Data(),
							current->Data(),
							&current->UpdatedRegion());
					}

					return current;
				}
				catch (std::exception& excSrc)
				{
					CommonLib::CExcBase::RegenExc("Failed to capture frame", excSrc);
					throw;
				}
			
			}

			CPixelFormat CScreenCapturerGdi::GetPixelFormat() const
			{
				return m_PixelFormat;
			}

			int CScreenCapturerGdi::GetScreenCount()
			{
				return CScreenUtils::ScreenCount();
			}

			void CScreenCapturerGdi::GetScreenList(ScreenList& screens)
			{
				return CScreenUtils::GetScreenList(screens);
			}

			void CScreenCapturerGdi::SelectScreen(ScreenId screen_id)
			{
				if (!CScreenUtils::IsScreenValid(screen_id, m_current_device_key))
					throw CommonLib::CExcBase("Invalid screen id %1", screen_id);

				// At next screen capture, the resources are recreated.
				m_desktop_dc_rect = CRect();
				m_current_screen_id = screen_id;
			 
			}

			void CScreenCapturerGdi::PrepareCaptureResources()
			{
				CRect desktop_rect = CScreenUtils::FullScreenRect();

				// If the display bounds have changed then recreate GDI resources.
				if (desktop_rect != m_desktop_dc_rect)
				{
					m_winDesktopDC.Close();
					m_memoryDC.Reset();

					m_desktop_dc_rect = CRect();
				}

				if (!m_winDesktopDC)
				{

					BOOL enabled;
					HRESULT hr = DwmIsCompositionEnabled(&enabled);
					if (SUCCEEDED(hr) && enabled)
					{
						// Vote to disable Aero composited desktop effects while capturing.
						// Windows will restore Aero automatically if the process exits.
						// This has no effect under Windows 8 or higher.
						DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
						m_composition_changed = true;
					}

					// Create GDI device contexts to capture from the desktop into memory.
					m_winDesktopDC.GetDC(nullptr);
					m_memoryDC.Reset(CreateCompatibleDC(m_winDesktopDC));
					if (!m_memoryDC)
						throw CommonLib::CWinExc("CreateCompatibleDC failed");


					m_PixelFormat = CScreenUtils::DetectPixelFormat();
					m_desktop_dc_rect = desktop_rect;

					// Make sure the frame buffers will be reallocated.
					m_queue.Reset();
				}

			}

		}
	}
}