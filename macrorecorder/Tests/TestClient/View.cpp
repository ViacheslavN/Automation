// View.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "View.h"
#include "ClientThread.h"

BOOL CView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here

	return 0;
}


void CView::ErrorFromThread(std::exception& ex)
{

}

void CView::UpdateFrame()
{

}

void CView::OnStart()
{
	if (m_clientThread.get() != nullptr)
	{
		m_clientThread.reset(new CClientThread(this));
	}






}
