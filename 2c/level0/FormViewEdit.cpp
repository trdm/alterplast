// FormViewEdit.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormViewEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewEdit

IMPLEMENT_DYNCREATE(CFormViewEdit, CFormView)

CFormViewEdit::CFormViewEdit()
	: CFormView(CFormViewEdit::IDD)
{
	//{{AFX_DATA_INIT(CFormViewEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormViewEdit::~CFormViewEdit()
{
}

void CFormViewEdit::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormViewEdit, CFormView)
	//{{AFX_MSG_MAP(CFormViewEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewEdit diagnostics

#ifdef _DEBUG
void CFormViewEdit::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewEdit::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewEdit message handlers
