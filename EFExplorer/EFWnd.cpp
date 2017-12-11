// EFWnd.cpp : implementation file
//

#include "stdafx.h"
#include "efexplorer.h"
#include "EFWnd.h"
#include "EFTreeView.h"
#include "EFEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CEFWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CEFWnd

CEFWnd::CEFWnd()
{
}

CEFWnd::~CEFWnd()
{
}


BEGIN_MESSAGE_MAP(CEFWnd, CWnd)
	//{{AFX_MSG_MAP(CEFWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEFWnd message handlers

BOOL CEFWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  if (m_wndSplitter.CreateStatic(this, 1, 2))
  {
    m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CEFTreeView), CSize(260, 0), pContext);
    m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CEFEditView), CSize(0, 0), pContext);
//    SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
  }
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
