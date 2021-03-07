#pragma once
class CSvcContex;


class CTCServer
{
public:
	CTCServer( std::shared_ptr< CSvcContext> pSvcContextPtr);
	~CTCServer();

	void Start();

private:
	CommonLib::network::CServerSocket serverSocket;
	std::shared_ptr< CSvcContext> m_pSvcContextPtr;
};

 