#pragma once
#include "../SvcContext.h"

class CCmdHandler
{
public:
	CCmdHandler(TSvcContextPtr m_pSvcContextPtr, CommonLib::network::AcceptedSocketPtr ptrSocket);
	~CCmdHandler();

	void Process();
private:
	void StartWork();

private:
	TSvcContextPtr m_pSvcContextPtr;
	CommonLib::CReadMemoryStream  m_stream;
	CommonLib::network::AcceptedSocketPtr m_ptrSocket;
};

typedef std::shared_ptr<CCmdHandler> CCmdHandlerPtr;