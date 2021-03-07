#pragma once

class CView;

class CClientThread
{
public:
	CClientThread(CView *pView);
	~CClientThread();

	void Stop();

private:
	void ThreadFunc();

private:
	CommonLib::synch::ThreadPtr m_thread;
	CommonLib::network::CClientSocketPtr m_socket;
	CView *m_pView;
};