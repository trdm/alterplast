// MyHelpDoc.cpp : implementation of the CMyHelpDoc class
//

#include "stdafx.h"
//#include "TestForMyHelpDoc.h"

#include "MyHelpDoc.h"
#include "MyHelpView.h"
#include "CntrItem.h"
#include "../MetadataTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyHelpDoc

IMPLEMENT_DYNCREATE(CMyHelpDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CMyHelpDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CMyHelpDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHelpDoc construction/destruction

CMyHelpDoc::CMyHelpDoc()
{
	// TODO: add one-time construction code here

}

CMyHelpDoc::~CMyHelpDoc()
{
}

BOOL CMyHelpDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CMyHelpDoc::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CTestForMyHelpDocCntrItem(preo, (CMyHelpDoc*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CMyHelpDoc serialization

void CMyHelpDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CMyHelpDoc diagnostics

#ifdef _DEBUG
void CMyHelpDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CMyHelpDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyHelpDoc commands

BOOL CMyHelpDoc::SaveModified() 
{
//	return CRichEditDoc::SaveModified();
	if(GetTree())//это режим в составе конфигурации
	{
		if(!GetObj()->bCanSave)
			return TRUE;

		CRichEditDoc::SetModifiedFlag(0);
		CString Str;
		((CMyHelpView*)GetView())->GetText(Str);
		GetObj()->csFile=Str;
		return TRUE;
	}
	else//это самостоятельный документ
	{
		return CRichEditDoc::SaveModified();
	}
	return TRUE;
}

BOOL CMyHelpDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	return TRUE;
}

