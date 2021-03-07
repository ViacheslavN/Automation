#pragma once

typedef std::shared_ptr<class CSvcContext> TSvcContextPtr;

class CSvcContext
{
public:
	CSvcContext();
	~CSvcContext();
private:

	mrCommonLib::desktop::TScreenCapturerPtr m_ScreenCapturer;
	mrCommonLib::desktop::TCursorCapturerPtr m_CursorCapture;
};