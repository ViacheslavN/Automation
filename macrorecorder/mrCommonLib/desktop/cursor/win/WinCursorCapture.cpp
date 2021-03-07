#include "pch.h"
#include "WinCursorCapture.h"
#include "CursorUtils.h"

namespace mrCommonLib
{
	namespace desktop
	{

			namespace win
			{
				bool IsSameCursorShape(const CURSORINFO& left, const CURSORINFO& right)
				{
					// If the cursors are not showing, we do not care the hCursor handle.
					return left.flags == right.flags && (left.flags != CURSOR_SHOWING ||
						left.hCursor == right.hCursor);
				}


				CCursorCapturerWin::CCursorCapturerWin()
				{
					memset(&m_PrevCursorInfo, 0, sizeof(m_PrevCursorInfo));
				}

				CCursorCapturerWin::~CCursorCapturerWin() = default;

				const std::shared_ptr<CMouseCursor> CCursorCapturerWin::CaptureCursor()
				{
					if (!m_DesktopDC.IsValid())
						m_DesktopDC.GetDC(nullptr);

					CURSORINFO cursor_info = { 0 };

					// Note: cursor_info.hCursor does not need to be freed.
					cursor_info.cbSize = sizeof(cursor_info);
					if (GetCursorInfo(&cursor_info))
					{
						if (!IsSameCursorShape(cursor_info, m_PrevCursorInfo))
						{
							if (cursor_info.flags == 0)
							{
								// Host machine does not have a hardware mouse attached, we will send a default one
								// instead. Note, Windows automatically caches cursor resource, so we do not need
								// to cache the result of LoadCursor.
								cursor_info.hCursor = LoadCursorW(nullptr, IDC_ARROW);
							}

							m_MouseCursor = CCursorUtil::GetMouseCursorFromHCursor(m_DesktopDC, cursor_info.hCursor);
							if (m_MouseCursor)
							{
								m_PrevCursorInfo = cursor_info;
								return m_MouseCursor;
							}
						}
					}

					throw CommonLib::CWinExc("GetCursorInfo failed");
				}

				void CCursorCapturerWin::Reset()
				{
					m_DesktopDC.Close();
				}
			}
	}
}
