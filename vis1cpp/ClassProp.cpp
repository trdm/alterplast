// ClassProp.cpp : implementation file
//

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "ClassProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassProp property page

IMPLEMENT_DYNCREATE(CClassProp, CPropertyPage)

CClassProp::CClassProp() : CPropertyPage(CClassProp::IDD)
{
	//{{AFX_DATA_INIT(CClassProp)
	m_def = _T("");
	m_mod = _T("");
	//}}AFX_DATA_INIT
}

CClassProp::CClassProp(const CString& strDefinePath, const CString& strImplementPath) : CPropertyPage(CClassProp::IDD)
{
	//{{AFX_DATA_INIT(CClassProp)
// 	m_def = _T("");
// 	m_mod = _T("");
	m_def = strDefinePath;
	m_mod = strImplementPath;
	//}}AFX_DATA_INIT
}

CClassProp::~CClassProp()
{
}

void CClassProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassProp)
	DDX_Text(pDX, IDC_EDIT1, m_def);
	DDX_Text(pDX, IDC_EDIT2, m_mod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassProp, CPropertyPage)
	//{{AFX_MSG_MAP(CClassProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassProp message handlers

