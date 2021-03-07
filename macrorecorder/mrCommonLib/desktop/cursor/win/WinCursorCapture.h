#pragma once
#include "../CursorCapturer.h"
#include "../../../desktop/win/HDC.h"

namespace mrCommonLib
{
	namespace desktop
	{

		namespace win
		{
			class CCursorCapturerWin : public ICursorCapturer
			{
			public:
				CCursorCapturerWin();
				~CCursorCapturerWin();

				const std::shared_ptr<CMouseCursor> CaptureCursor() override;
				void Reset() override;

			private:
				CWinHDC m_DesktopDC;
				std::shared_ptr<CMouseCursor> m_MouseCursor;
				CURSORINFO m_PrevCursorInfo;

			};
		}

	}
}
