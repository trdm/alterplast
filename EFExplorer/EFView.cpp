// EFView.cpp : implementation file
//

#include "stdafx.h"
#include "efexplorer.h"
#include "EFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEFView

IMPLEMENT_DYNCREATE(CEFView, CView)

CEFView::CEFView()
{
}

CEFView::~CEFView()
{
}


BEGIN_MESSAGE_MAP(CEFView, CView)
	//{{AFX_MSG_MAP(CEFView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEFView drawing

void CEFView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CEFView diagnostics

#ifdef _DEBUG
void CEFView::AssertValid() const
{
	CView::AssertValid();
}

void CEFView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEFView message handlers

BOOL CEFView::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
/*
  if (m_wndSplitter.CreateStatic(this, 1, 2))
  {
    m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CEFTreeView), CSize(250, 0), pContext);
    m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CEFEditView), CSize(0, 0), pContext);
    SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
  }
*/	
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	return true;
}

