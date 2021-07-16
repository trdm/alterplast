// MetedataDocument.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "MetedataDocument.h"
#include "MetadataTree.h"
#include "ModuleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CMetedataDocument

IMPLEMENT_DYNCREATE(CMetedataDocument, CDocument)
 
CMetedataDocument::CMetedataDocument()
{
}

BOOL CMetedataDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	GetView()->Load();
	return TRUE;
}

CMetedataDocument::~CMetedataDocument()
{
}


BEGIN_MESSAGE_MAP(CMetedataDocument, CDocument)
	//{{AFX_MSG_MAP(CMetedataDocument)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMetedataDocument diagnostics

#ifdef _DEBUG
void CMetedataDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CMetedataDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMetedataDocument serialization

void CMetedataDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//CZipArchive m_zip;
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMetedataDocument commands

void CMetedataDocument::SetModifiedFlag(BOOL bFlag)
{
	CString Str=GetTitle();
	Str.TrimRight('*');
	if(bFlag)
		Str+="*";
	SetTitle(Str);
	CDocument::SetModifiedFlag(bFlag);
	UpdateAllViews(0);
}

BOOL CMetedataDocument::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return TRUE;
}

BOOL CMetedataDocument::OnOpenDocument(LPCTSTR lpszPathName) 
{
	GetView()->Load();
	return TRUE;
}




void CMetedataDocument::OnFileSave() 
{
	CString Str=GetTitle();
	Str.TrimRight('*');
	//DoFileSave();
	if(GetView()->Save())
	{
		SetModifiedFlag(0);
		SetTitle(Str);
	}
}

extern BOOL afxExitMode;
BOOL CMetedataDocument::CanCloseFrame(CFrameWnd* pFrame) 
{
	if(!afxExitMode)
	GetView()->SaveOpenModulesState(0);

	BOOL bRes=CDocument::CanCloseFrame(pFrame);

	if(bRes)
	{
		GetView()->Clear();
	}
	return bRes;
}


void CMetedataDocument::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	
}

BOOL CMetedataDocument::SaveModified()
{
	GetView()->CheckModified();
	if (!IsModified())
		return TRUE;        // ok to continue

	CString prompt;
	prompt="Конфигурация не сохранена!\nСохранить?";
	HWND hWnd=0;
	if(AfxGetMainWnd())
		hWnd=AfxGetMainWnd()->m_hWnd;
	switch (MessageBox(hWnd,prompt, "Внимание",MB_YESNOCANCEL | MB_ICONQUESTION|MB_APPLMODAL))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		OnFileSave();
		break;
	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}
