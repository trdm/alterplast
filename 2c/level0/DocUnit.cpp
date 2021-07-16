//Авторские права - VTOOLS.RU (info@vtools.ru)
// DocUnit.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "DocUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocUnit

IMPLEMENT_DYNCREATE(CMetaDocument, CDocument)
IMPLEMENT_DYNCREATE(CDocUnit, CMetaDocument)

CDocUnit::CDocUnit()
{
}

BOOL CDocUnit::OnNewDocument()
{
	return TRUE;
//	if (!CMetaDocument::OnNewDocument())
//		return FALSE;
//	return TRUE;
}

CDocUnit::~CDocUnit()
{
}


BEGIN_MESSAGE_MAP(CDocUnit, CMetaDocument)
	//{{AFX_MSG_MAP(CDocUnit)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocUnit diagnostics

#ifdef _DEBUG
void CDocUnit::AssertValid() const
{
	CMetaDocument::AssertValid();
}

void CDocUnit::Dump(CDumpContext& dc) const
{
	CMetaDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDocUnit serialization

void CDocUnit::Serialize(CArchive& ar)
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
// CDocUnit commands

BOOL CDocUnit::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CMetaDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

BOOL CDocUnit::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	return FALSE;
//	return CMetaDocument::OnSaveDocument(lpszPathName);
}

void CDocUnit::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(0);
}

void CDocUnit::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(0);
}

