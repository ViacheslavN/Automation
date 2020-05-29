#pragma once

class CClientThread
{
public:
	CClientThread();
	~CClientThread();

	void Stop();

private:
	void ThreadFunc();

private:
	CommonLib::synch::ThreadPtr m_thread;
	CommonLib::network::CClientSocketPtr m_socket;
};