// EFEditView.cpp : implementation file
//

#include "stdafx.h"
#include "efexplorer.h"
#include "EFEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEFEditView

IMPLEMENT_DYNCREATE(CEFEditView, CEditView)

CEFEditView::CEFEditView()
{
}

CEFEditView::~CEFEditView()
{
}


BEGIN_MESSAGE_MAP(CEFEditView, CEditView)
	//{{AFX_MSG_MAP(CEFEditView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEFEditView drawing

void CEFEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CEFEditView diagnostics

#ifdef _DEBUG
void CEFEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CEFEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEFEditView message handlers
