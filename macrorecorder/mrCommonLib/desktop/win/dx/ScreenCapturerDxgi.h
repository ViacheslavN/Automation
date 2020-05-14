#pragma once

#include "../../ScreenCapturer.h"



namespace mrCommonLib
{
	namespace desktop {

		namespace win {

			class CDxgiDuplicatorController;
			class CDxgiFrame;

			class CScreenCapturerDxgi : public IScreenCapturer
			{
			public:
				CScreenCapturerDxgi();
				~CScreenCapturerDxgi();

				// Whether the system supports DXGI based capturing.
				bool IsSupported();

				// Whether current process is running in a Windows session which is supported by
				// ScreenCapturerDxgi.
				// Usually using ScreenCapturerDxgi in unsupported sessions will fail.
				// But this behavior may vary on different Windows version. So consumers can
				// always try isSupported() function.
				static bool IsCurrentSessionSupported();

				// ScreenCapturer implementation.
				virtual IFramePtr CaptureFrame();
				virtual CPixelFormat GetPixelFormat() const;
				virtual int GetScreenCount();
				virtual void GetScreenList(ScreenList& screens);
				virtual void SelectScreen(ScreenId screen_id);


			protected:
				void Reset();

			private:
				std::shared_ptr<CDxgiDuplicatorController> m_controller;

				ScreenId current_screen_id_ = kFullDesktopScreenId;
				CFrameQueue<std::shared_ptr<CDxgiFrame> > m_queue;

			};
		}
	}
}