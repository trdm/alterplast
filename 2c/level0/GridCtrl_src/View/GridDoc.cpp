// GridDoc.cpp : implementation of the CGridDoc class
//

#include "stdafx.h"
#include "GridView.h"

#include "GridDoc.h"
#include "../../MetadataTree.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString LoadFromFile(CString csFileName);
void WritoToFile(CString csCFile,CString Str);

/////////////////////////////////////////////////////////////////////////////
// CGridDoc

IMPLEMENT_DYNCREATE(CGridDoc, CMetaDocument)

BEGIN_MESSAGE_MAP(CGridDoc, CMetaDocument)
	//{{AFX_MSG_MAP(CGridDoc)
//	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDoc construction/destruction
CGridCtrlExt *afxGrid=0;
CGridCtrlExt *AfxNewGridDoc()
{
	ASSERT(AfxGetMainWnd());
	CGridCtrlExt *p=new CGridCtrlExt();
	p->Create(CRect(0,0,0,0),AfxGetMainWnd(),100,WS_CHILD | WS_BORDER | WS_TABSTOP);
	return p;
}
void AfxSetGridDoc(CGridCtrlExt *p)
{
	afxGrid=p;
}
CGridDoc::CGridDoc()
{
	if(afxGrid)
	{
		m_pGridCtrl=afxGrid;
		//bDelete=0;
	}
	else
	{
		m_pGridCtrl=new CGridCtrlExt();
		//bDelete=1;
	}

	m_pGridCtrl->pDoc=this;
	afxGrid=0;
}

CGridDoc::~CGridDoc()
{
	if(m_pGridCtrl)
		delete m_pGridCtrl;
	m_pGridCtrl=0;
}

BOOL CGridDoc::OnNewDocument()
{
	if (!CMetaDocument::OnNewDocument())
		return FALSE;

    CheckPoint();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGridDoc serialization

void CGridDoc::Serialize(CArchive& ar)
{
	//AfxMessageBox("!!");
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
// CGridDoc diagnostics

#ifdef _DEBUG
void CGridDoc::AssertValid() const
{
	CMetaDocument::AssertValid();
}

void CGridDoc::Dump(CDumpContext& dc) const
{
	CMetaDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridDoc commands

BOOL CGridDoc::SaveModified() 
{
	if(AfxGetProperty())
		AfxGetProperty()->EndEdit();

	if(GetTree())//это режим в составе конфигурации
	{
		CMetaDocument::SetModifiedFlag(0);
		m_pGridCtrl->SaveMXL(Buffer);
		GetObj()->csFile=Buffer;
		return TRUE;
	}
	
	//это самостоятельный документ
	BOOL bRes=CMetaDocument::SaveModified();
	if(bRes)
		CMetaDocument::SetModifiedFlag(0);
	return bRes;
}

BOOL CGridDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CMetaDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
    CheckPoint();

	//CString LoadFromFile(CString csFileName)

	Buffer=LoadFromFile(lpszPathName);
	

//	m_pGridCtrl->Load(lpszPathName);
	
	return TRUE;
}

BOOL CGridDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	m_pGridCtrl->SaveMXL(Buffer);
	WritoToFile(lpszPathName,Buffer);
	return 1;
	//return CMetaDocument::OnSaveDocument(lpszPathName);
}

void CGridDoc::DeleteContents()
{
	//m_pGridCtrl->DeleteAllItems();
}
