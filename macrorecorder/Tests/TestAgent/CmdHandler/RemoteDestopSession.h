#pragma once

#include "../../../mrCommonLib/proto/VideoCodecSettingsPackage.h"

class CRemoteDestopSession
{
public:
	CRemoteDestopSession(TSvcContextPtr pSvcContextPtr, CommonLib::network::AcceptedSocketPtr ptrSocket);
	~CRemoteDestopSession();

	void Process(CommonLib::CReadMemoryStream& stream);
private:
	void CreateVideoCodec(const mrCommonLib::proto::CVideoCodecSettingsPackage& videoCodecSettings);
	void CreateCapture();

private:
	TSvcContextPtr m_pSvcContextPtr;
	CommonLib::CReadMemoryStream  m_stream;
	CommonLib::network::AcceptedSocketPtr m_ptrSocket;

	mrCommonLib::desktop::TScreenCapturerPtr m_ScreenCapturer;
	mrCommonLib::desktop::TCursorCapturerPtr m_CursorCapture;
	mrCommonLib::video::IVideoEncoderPtr m_VideoEncoder;
};

typedef std::shared_ptr< CRemoteDestopSession> CRemoteDestopSessionPtr;