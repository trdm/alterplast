//Авторские права - VTOOLS.RU (info@vtools.ru)
// FormFrame.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormFrame.h"
#include "FormUnit.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormFrame

IMPLEMENT_DYNCREATE(CFormFrame, CMDIChildWnd)

CFormFrame::CFormFrame()
{
	pForm=0;
}

CFormFrame::~CFormFrame()
{ 
}


BEGIN_MESSAGE_MAP(CFormFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFormFrame)
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormFrame message handlers

BOOL CFormFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

 

void CFormFrame::ActivateFrame(int nCmdShow) 
{
	if(pForm)
		if(!pForm->bCanOpen)
			return;
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

void CFormFrame::OnClose() 
{
	if(pForm)
		if(pForm->bCanOpen)
		if(!pForm->IsCanDestroy())
			return;
	
	CMDIChildWnd::OnClose();
}


/*
//из-за этого тормозит при движениях мышки...
BOOL CFormFrame::PreTranslateMessage(MSG* pMsg) 
{
//	return CMDIChildWnd::PreTranslateMessage(pMsg);
	CMainFrame * pMainFrame =
		DYNAMIC_DOWNCAST(
		CMainFrame,
		::AfxGetMainWnd()
		);
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	
	return CMDIChildWnd::PreTranslateMessage(pMsg);

}
*/
