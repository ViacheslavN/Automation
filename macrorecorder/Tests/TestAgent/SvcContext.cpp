#include "stdafx.h"
#include "SvcContext.h"
#include "../../mrCommonLib/desktop/cursor/win/WinCursorCapture.h"

CSvcContext::CSvcContext()
{
	mrCommonLib::desktop::win::CScreenCapturerDxgi* pDxgi = new mrCommonLib::desktop::win::CScreenCapturerDxgi();
	if (!pDxgi->IsSupported())
	{
		delete pDxgi;
		m_ScreenCapturer.reset(new mrCommonLib::desktop::win::CScreenCapturerGdi());
	}
	else
	{
		m_ScreenCapturer.reset(pDxgi);
	}

	m_CursorCapture.reset(new mrCommonLib::desktop::win::CCursorCapturerWin());
}

CSvcContext::~CSvcContext()
{

}