// FormViewConfig.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormViewConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewConfig

IMPLEMENT_DYNCREATE(CFormViewConfig, CFormView)

CFormViewConfig::CFormViewConfig()
	: CFormView(CFormViewConfig::IDD)
{
	//{{AFX_DATA_INIT(CFormViewConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormViewConfig::~CFormViewConfig()
{
}

void CFormViewConfig::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormViewConfig, CFormView)
	//{{AFX_MSG_MAP(CFormViewConfig)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewConfig diagnostics

#ifdef _DEBUG
void CFormViewConfig::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewConfig::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewConfig message handlers

void CFormViewConfig::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	

	CToolBar* pToolBox=new CToolBar();
	pToolBox->Create(GetParentFrame(),WS_CHILD | WS_VISIBLE | CBRS_BOTTOM );
	pToolBox->LoadToolBar(SECAH_IDR_LAYOUT);
	GetParentFrame()->ShowControlBar(pToolBox,  1, FALSE);
	
}
