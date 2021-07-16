//Авторские права - VTOOLS.RU (info@vtools.ru)
// FormObjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormObject.h"
#include "FormObjectDoc.h"
#include "MetadataTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormObjectDoc

IMPLEMENT_DYNCREATE(CFormObjectDoc, CMetaDocument)

CFormObjectDoc::CFormObjectDoc()
{
	SetObj(afxMetaObj);
	SetTree(afxMetaTree);
}

BOOL CFormObjectDoc::OnNewDocument()
{
	if (!CMetaDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFormObjectDoc::~CFormObjectDoc()
{
}


BEGIN_MESSAGE_MAP(CFormObjectDoc, CMetaDocument)
	//{{AFX_MSG_MAP(CFormObjectDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormObjectDoc diagnostics

#ifdef _DEBUG
void CFormObjectDoc::AssertValid() const
{
	CMetaDocument::AssertValid();
}

void CFormObjectDoc::Dump(CDumpContext& dc) const
{
	CMetaDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormObjectDoc serialization

void CFormObjectDoc::Serialize(CArchive& ar)
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
// CFormObjectDoc commands

BOOL CFormObjectDoc::SaveModified() 
{
	CFormObject* pView = (CFormObject*)GetView();
	if(GetTree())
		GetTree()->GetDocument()->SetModifiedFlag(1);
	return TRUE;
}



void CFormObjectDoc::OnCloseDocument() 
{
	if(GetTree())
		GetTree()->RemoveFromList(GetObj(),GetObjName());
	CMetaDocument::OnCloseDocument();
}

void CFormObjectDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	
}
void CFormObjectDoc::OnChangedViewList()
{
	GetView()->OnUpdate();
}

