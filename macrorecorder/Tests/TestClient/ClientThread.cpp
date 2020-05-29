#include "stdafx.h"
#include "ClientThread.h"

CClientThread::CClientThread()
{
	m_thread.reset(new CommonLib::synch::CThread(

		[this]()
	{
		ThreadFunc();
	}, "connectionThread"));

}

CClientThread::~CClientThread()
{

}

void CClientThread::Stop()
{
	
}

void CClientThread::ThreadFunc()
{

}