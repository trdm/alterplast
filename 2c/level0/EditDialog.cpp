// EditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "EditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDialog

IMPLEMENT_DYNCREATE(CEditDialog, CFormView)

CEditDialog::CEditDialog()
	: CFormView(CEditDialog::IDD)
{
	//{{AFX_DATA_INIT(CEditDialog)
	//}}AFX_DATA_INIT
}

CEditDialog::~CEditDialog()
{
}

void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDialog)
	DDX_Control(pDX, IDC_COMMENT, m_Edit);
	DDX_Control(pDX, IDC_STATIC2, m_Group);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditDialog, CFormView)
	//{{AFX_MSG_MAP(CEditDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_STATIC2, OnStatic2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDialog diagnostics

#ifdef _DEBUG
void CEditDialog::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditDialog::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditDialog message handlers

void CEditDialog::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	CToolBar* pToolBox=new CToolBar();
	pToolBox->Create(GetParentFrame(),WS_CHILD | WS_VISIBLE | CBRS_BOTTOM );
	pToolBox->LoadToolBar(SECAH_IDR_LAYOUT);
	GetParentFrame()->ShowControlBar(pToolBox,  1, FALSE);
	
//	MoveWindow(CRect(0,0,100,100));
}

void CEditDialog::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
//	MoveWindow(CRect(0,0,100,100));
	
}

void CEditDialog::OnStatic2() 
{
	// TODO: Add your control notification handler code here
	
}
