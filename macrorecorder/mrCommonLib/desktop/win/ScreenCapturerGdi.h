#pragma once
#include "../ScreenCapturer.h"
#include "HDC.h"
#include "../differ/differ.h"

namespace mrCommonLib
{
	namespace desktop
	{
		namespace win
		{
			class CScreenCapturerGdi : public IScreenCapturer
			{
			public:

				CScreenCapturerGdi();
				~CScreenCapturerGdi();


				virtual IFramePtr CaptureFrame();
				virtual CPixelFormat GetPixelFormat() const;
				virtual int GetScreenCount();
				virtual void GetScreenList(ScreenList& screens);
				virtual void SelectScreen(ScreenId screen_id);

			private:
				void PrepareCaptureResources();

			private:


				CWinHDC m_winDesktopDC;
				CHDC m_memoryDC;

				bool m_composition_changed = false;

				ScreenId m_current_screen_id = kFullDesktopScreenId;
				wstr m_current_device_key;

				CRect m_desktop_dc_rect;
				CPixelFormat m_PixelFormat;
				CFrameQueue<IFramePtr> m_queue;
				std::unique_ptr<CDiffer> m_differ;
			};
		}
	}
}