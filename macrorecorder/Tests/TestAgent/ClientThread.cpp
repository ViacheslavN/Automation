#include "stdafx.h"
#include "ClientThread.h"


CClientThread::CClientThread(CommonLib::network::AcceptedSocketPtr pSocket): m_socket(pSocket)
{
	m_headerBuff.resize(100);
	astr threadName = m_socket->GetClientAddr();
	m_thread.reset(new CommonLib::synch::CThread(

	[this]()
	{
		ThreadFunc();
	}, std::move(threadName)));

}

CClientThread::~CClientThread()
{

}

void CClientThread::ThreadFunc()
{
	Log.Info("Start threading");

	while (true)
	{
		m_socket->Recv(m_headerBuff.data(), m_headerBuff.size());
	}
}