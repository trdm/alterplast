// RootProp.cpp : implementation file
//

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "RootProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRootProp property page

IMPLEMENT_DYNCREATE(CRootProp, CPropertyPage)

CRootProp::CRootProp() : CPropertyPage(CRootProp::IDD)
{
	//{{AFX_DATA_INIT(CRootProp)
	m_path = pSvc->IBDir();
	m_bin = pSvc->BinDir();
	//}}AFX_DATA_INIT
}

CRootProp::CRootProp(const CString& strPath) : CPropertyPage(CRootProp::IDD)
{
	//{{AFX_DATA_INIT(CRootProp)
	m_path = strPath;
	m_bin = pSvc->BinDir();
	//}}AFX_DATA_INIT
}

CRootProp::~CRootProp()
{
}

void CRootProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRootProp)
	DDX_Text(pDX, IDC_EDIT1, m_path);
	DDX_Text(pDX, IDC_EDIT2, m_bin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRootProp, CPropertyPage)
	//{{AFX_MSG_MAP(CRootProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRootProp message handlers
