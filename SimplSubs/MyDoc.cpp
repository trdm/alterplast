// MyDoc.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDoc

IMPLEMENT_DYNCREATE(CMyDoc, CDocument)

CMyDoc::CMyDoc()
{
}

BOOL CMyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	m_MDChildNames.SetAt("Константа", "Константы");
	m_MDChildNames.SetAt("Справочник","Справочники");
	m_MDChildNames.SetAt("Документ", "Документы");

	m_MDChildNames.SetAt("","");
	return TRUE;
}

CMyDoc::~CMyDoc()
{
}


BEGIN_MESSAGE_MAP(CMyDoc, CCmdTarget)
	//{{AFX_MSG_MAP(CMyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDoc diagnostics

#ifdef _DEBUG
void CMyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDoc serialization

void CMyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyDoc commands

void CMyDoc::UpdateTreeMD(CTreeCtrl *nTreCtrl)
{

}
