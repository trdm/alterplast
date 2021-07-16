//Авторские права - VTOOLS.RU (info@vtools.ru)
// EditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "EditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditWnd

CEditWnd::CEditWnd()
{
}

CEditWnd::~CEditWnd()
{
}


BEGIN_MESSAGE_MAP(CEditWnd, CWnd)
	//{{AFX_MSG_MAP(CEditWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditWnd message handlers

BOOL CEditWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create("CButton", "CButton", dwStyle, rect, pParentWnd, nID, pContext);
}

void CEditWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}
