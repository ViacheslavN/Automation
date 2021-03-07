#pragma once

#include "ClientThread.h"
#include "SvcContext.h"

class CThreadPool
{
public:
	CThreadPool(uint32_t nThreads, TSvcContextPtr svcContext);
	~CThreadPool();

	void AddAcceptedSocket(CommonLib::network::AcceptedSocketPtr ptrSocket);
private:
	std::vector<TCClientThreadPtr> m_threads;
	TSocketQueuePtr m_queue;
	TSvcContextPtr m_svcContext;
};