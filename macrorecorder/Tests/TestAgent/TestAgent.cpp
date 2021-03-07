// TestAgent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "stdafx.h"
#include "ClientThread.h"

int main()
{
	std::shared_ptr<CommonLib::IlogLineFormat> plogLineFormat;
	std::shared_ptr<CommonLib::IlogRetention> plogRetention;
	std::shared_ptr<CommonLib::IlogWriter> fileLogger = CommonLib::CFileAsyncLogger::Create("C:\\testAgent.log", 1024*1024*20, plogRetention);

	std::shared_ptr<CommonLib::IlogWriter> pLogWriter(fileLogger);
	Log.InnitLog(std::shared_ptr<CommonLib::IlogLineFormat>(), pLogWriter);

	WSADATA wsaData = { 0 };
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);

	std::vector<TCClientThreadPtr> vecClient;

	try
	{

		CommonLib::network::CServerSocket serverSocket;
		serverSocket.OpenSocket(CommonLib::network::CBaseSocketIP::SOCKSTREAM);
		serverSocket.Bind(10000, 40000);
		serverSocket.Listen();

		Log.InfoT("Port connection %1", serverSocket.GetPort());


		while (true)
		{			  
			
			auto acceptSocket = serverSocket.Accept();

			Log.InfoT("accept connection from %1", acceptSocket->GetClientAddr());

		//	vecClient.push_back(TClientPtr(new CClientThread(acceptSocket)));



		}

	}
	catch (std::exception& exc)
	{

		Log.Exc(exc);

		std::cout << "Failed: " << "\n";

		astrvec msgChain = CommonLib::CExcBase::GetChainFromExc(exc);


		for (size_t i = 0, sz = msgChain.size(); i < sz; ++i)
		{
			if (i != 0)
				std::cout << "\r\n";

			std::cout << msgChain[i];
		}
	}


	return 0;

 
}
