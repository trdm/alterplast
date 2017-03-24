// FormDoc.cpp : implementation file
//

#include "stdafx.h"
#include "config.h"
#include "FormDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CFormDoc

IMPLEMENT_DYNCREATE(CFormDoc, CDocument)

CFormDoc::CFormDoc()
{
}

BOOL CFormDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFormDoc::~CFormDoc()
{
}


BEGIN_MESSAGE_MAP(CFormDoc, CDocument)
	//{{AFX_MSG_MAP(CFormDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDoc serialization

void CFormDoc::Serialize(CArchive& ar)
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
// CFormDoc commands
