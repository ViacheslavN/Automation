#include "stdafx.h"
#include "ClientThread.h"
#include "View.h"

CClientThread::CClientThread(CView *pView) : m_pView(pView)
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
	try
	{
	}
	catch (std::exception& ex)
	{

	}
}