#pragma once

#include "stdafx.h"
#include "SvcContext.h"

typedef std::shared_ptr<class CClientThread> TCClientThreadPtr;
typedef CommonLib::synch::TSyncQueue<CommonLib::network::AcceptedSocketPtr> TSocketQueue;
typedef std::shared_ptr<TSocketQueue> TSocketQueuePtr;
 

class CClientThread
{
public:
	CClientThread(TSocketQueuePtr pSocketQueue, TSvcContextPtr pSvcContextPtr);
	~CClientThread();


	void ThreadFunc();


private:

	TSocketQueuePtr m_queue;
	CommonLib::synch::ThreadPtr m_thread;
	std::vector<byte_t> m_headerBuff;
	TSvcContextPtr m_pSvcContextPtr;

};