#include "../stdafx.h"
#include "../../../mrCommonLib/proto/PackageHeader.h"

#include "../../../../../CommonLib/crypto/EmptyDataCipher.h"
#include "../SvcContext.h"
#include "../../../mrCommonLib/desktop/cursor/win/WinCursorCapture.h"
#include "RemoteDestopSession.h"
 


CRemoteDestopSession::CRemoteDestopSession(TSvcContextPtr pSvcContextPtr, CommonLib::network::AcceptedSocketPtr ptrSocket) : m_pSvcContextPtr(pSvcContextPtr), m_ptrSocket(ptrSocket)
{

}

CRemoteDestopSession::~CRemoteDestopSession()
{

}

void CRemoteDestopSession::Process(CommonLib::CReadMemoryStream& stream)
{
	try
	{
		mrCommonLib::proto::CVideoCodecSettingsPackage videoCodecSettings;
		videoCodecSettings.Read(&stream);

		CreateVideoCodec(videoCodecSettings);
		CreateCapture();
		CommonLib::CWriteMemoryStream writeStream;
		mrCommonLib::video::CVideoPackage encodePackage;
		while (true)
		{
			
			mrCommonLib::desktop::IFramePtr pFrame = m_ScreenCapturer->CaptureFrame();
			bool bSkip = false;
			encodePackage.Clear();

			m_VideoEncoder->Encode(pFrame.get(), &encodePackage, bSkip);
			if (bSkip)
				continue;

			encodePackage.Save(&writeStream);


		}

	}
	catch (std::exception& exc)
	{
		CommonLib::CExcBase::RegenExc("Failed to process remote destop session", exc);
	}
}

void CRemoteDestopSession::CreateVideoCodec(const mrCommonLib::proto::CVideoCodecSettingsPackage& videoCodecSettings)
{
	m_VideoEncoder = mrCommonLib::video::IVideoEncoder::CreateVideoEncoder(videoCodecSettings.GetEncodeId());
}

void CRemoteDestopSession::CreateCapture()
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