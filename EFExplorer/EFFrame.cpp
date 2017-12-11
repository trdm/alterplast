// EFFrame.cpp : implementation file
//

#include "stdafx.h"
#include "efexplorer.h"
#include "EFFrame.h"
#include "EFTreeView.h"
#include "EFEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEFFrame

IMPLEMENT_DYNCREATE(CEFFrame, CFrameWnd)

CEFFrame::CEFFrame()
{
}

CEFFrame::~CEFFrame()
{
}


BEGIN_MESSAGE_MAP(CEFFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CEFFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEFFrame message handlers

BOOL CEFFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

  if (m_wndSplitter.CreateStatic(this, 1, 2))
  {
    m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CEFTreeView), CSize(250, 0), pContext);
    m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CEFEditView), CSize(0, 0), pContext);
    SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
  }
	
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	return true;
}

