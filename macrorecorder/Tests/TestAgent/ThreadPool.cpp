#include "stdafx.h"
#include "ThreadPool.h"

CThreadPool::CThreadPool(uint32_t nThreads, TSvcContextPtr svcContext)
{

	m_queue.reset(new TSocketQueue());

	for (uint32_t i = 0; i < nThreads; ++i)
	{
		m_threads.push_back(TCClientThreadPtr(new CClientThread(m_queue, svcContext)));
	}
}

CThreadPool::~CThreadPool()
{

}

void CThreadPool::AddAcceptedSocket(CommonLib::network::AcceptedSocketPtr ptrSocket)
{
	m_queue->Push(ptrSocket);
}