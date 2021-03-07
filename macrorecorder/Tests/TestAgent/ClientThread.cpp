#include "stdafx.h"
#include "ClientThread.h"
#include "../../mrCommonLib/proto/PackageHeader.h"
#include "../../../../CommonLib/crypto/EmptyDataCipher.h"
#include "SvcContext.h"
#include "CmdHandler/CmdHandler.h"

CClientThread::CClientThread(TSocketQueuePtr pSocketQueue, std::shared_ptr< CSvcContext> pSvcContextPtr): m_queue(pSocketQueue),  m_pSvcContextPtr(pSvcContextPtr)
{
	m_headerBuff.resize(100);

	m_thread.reset(new CommonLib::synch::CThread(

	[this]()
	{
		ThreadFunc();
	}, "Client Thread"));

}

CClientThread::~CClientThread()
{

}

void CClientThread::ThreadFunc()
{
	Log.Info("Start threading");
	try
	{
		while (true)
		{
			CommonLib::network::AcceptedSocketPtr ptrSocket = m_queue->Pop();
			if (!ptrSocket.get())
				break;

			CCmdHandlerPtr handler(new CCmdHandler(m_pSvcContextPtr, ptrSocket));
			handler->Process();
		}
	}
	catch (std::exception& exc)
	{
		Log.Exc(exc);
	}


	Log.Info("Stop threading");
}