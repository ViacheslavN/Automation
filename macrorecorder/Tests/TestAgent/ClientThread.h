#pragma once

#include "stdafx.h"

class CClientThread
{
public:
	CClientThread(CommonLib::network::AcceptedSocketPtr pSocket);
	~CClientThread();


	void ThreadFunc();


private:
	CommonLib::network::AcceptedSocketPtr m_socket;
	CommonLib::synch::ThreadPtr m_thread;
	std::vector<byte_t> m_headerBuff;
};