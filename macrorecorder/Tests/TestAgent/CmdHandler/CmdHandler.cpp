#include "../stdafx.h"
#include "CmdHandler.h"
#include "../../../mrCommonLib/proto/PackageHeader.h"
#include "../../../mrCommonLib/proto/Packages.h"
#include "../../../../../CommonLib/crypto/EmptyDataCipher.h"
#include "../SvcContext.h"
#include "RemoteDestopSession.h"

CCmdHandler::CCmdHandler(TSvcContextPtr pSvcContextPtr, CommonLib::network::AcceptedSocketPtr ptrSocket) : m_pSvcContextPtr(pSvcContextPtr), m_ptrSocket(ptrSocket)
{

}

CCmdHandler::~CCmdHandler()
{

}

void CCmdHandler::Process()
{
	try
	{

		CommonLib::network::CNetworkStreamIoDev networkStream(m_ptrSocket, CommonLib::crypto::IDataCipherPtr(new CommonLib::crypto::CEmptyDataCipher()), CommonLib::IAllocPtr());

		mrCommonLib::proto::CPackegHeader header;
		m_stream.Seek(0, CommonLib::soFromBegin);
		networkStream.Read(&m_stream);

		header.Read(&m_stream);

		switch (header.GetType())
		{
			case mrCommonLib::proto::eStartRemoteDestopSession:
			{
				CRemoteDestopSession session(m_pSvcContextPtr, m_ptrSocket);
				session.Process(m_stream);
			}
			break;
			default:
				throw CommonLib::CExcBase("Unknown type: %1", (uint32_t)header.GetType());
				break;
		}
	}
	catch (std::exception& exc)
	{
		CommonLib::CExcBase::RegenExc("Failed to process socket", exc);
	}
}