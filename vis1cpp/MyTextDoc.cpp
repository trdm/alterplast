// MyTextDoc.cpp : implementation file
//

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "MyTextDoc.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIStringMap<DWORD*, DWORD*> CDocumentInterceptor::m_pVtableMap; //CMapStringToPtr CDocumentInterceptor::m_pVtableMap;
//CTypedPtrMap<CMapStringToPtr, CString, void*> CDocumentInterceptor::m_OnCloseMap; 
CMapStringToPtr CDocumentInterceptor::m_OnCloseMap;
CMapStringToPtr CDocumentInterceptor::m_DoSaveMap;
// CMapStringToPtr CDocumentInterceptor::m_OnOpenMap;
// CMapStringToPtr CDocumentInterceptor::m_OnNewMap;

CIStringMap<CMyTextDoc*, CMyTextDoc*> CDocumentInterceptor::m_Hooked; //CMapStringToOb  CDocumentInterceptor::m_Hooked;

/////////////////////////////////////////////////////////////////////////////
// CMyTextDoc

IMPLEMENT_DYNCREATE(CMyTextDoc, CDocument)

CMyTextDoc::CMyTextDoc()
{
}

CMyTextDoc::~CMyTextDoc()
{
}


BEGIN_MESSAGE_MAP(CMyTextDoc, CDocument)
	//{{AFX_MSG_MAP(CMyTextDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyTextDoc::OnCloseDocumentMy() 
{
	CDocumentInterceptor::RunOrig_OnCloseDocument(this);
}

BOOL CMyTextDoc::DoFileSaveMy()
{
 	CString strSavedTitle = m_strTitle;
	
	BOOL ret = CDocumentInterceptor::RunOrig_DoFileSave(this);

	m_strTitle = strSavedTitle;
 	
 	POSITION pos=GetFirstViewPosition();
 	GetNextView(pos)->GetParentFrame()->SetWindowText(m_strTitle);
 	CCPPView::GetView()->UpdateClass(this);
 	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CDocumentInterceptor

void CDocumentInterceptor::HookDoc( CDocument* pDoc1, LPCSTR title, HICON hIcon/*=NULL*/ )
{
	CMyTextDoc* pDoc = static_cast<CMyTextDoc*>(pDoc1);

	CString path;
	long id=CConfigCont::DocumentToID(pDoc, path);
	if(id>0)
		CConfigCont::DocumentToPath(pDoc, path);
	else
		path=pDoc->GetPathName();

	if(LookupDoc(path))
		return;

	pDoc->ST(title);

	POSITION pos=pDoc->GetFirstViewPosition();
	CFrameWnd* pFrame=pDoc->GetNextView(pos)->GetParentFrame();
	pFrame->SetWindowText(title);
	if(hIcon)
		pFrame->SetIcon(hIcon,FALSE);
	else
	{
		pFrame->SetIcon(LoadIcon(hMyInst,(LPCTSTR)IDI_CLASS),FALSE);
	}

	CString strDocumentClassName = pDoc->GetRuntimeClass()->m_lpszClassName;
	DWORD* pVtable;
	if(!m_pVtableMap.Lookup(strDocumentClassName, pVtable))
	{
		DWORD *pVtableDoc=*(DWORD**)pDoc,*ptr=pVtableDoc;
		while(*++ptr);
		pVtable = new DWORD[ptr-pVtableDoc];
		m_pVtableMap[strDocumentClassName] = pVtable;
		int i=0;
		while(pVtableDoc!=ptr)
			pVtable[i++]=*pVtableDoc++;
		
		const char nCDocument_DoFileSave_Number = 0x29;
		const char nCDocument_OnCloseDocument_Number = 0x21;
		const char nCDocument_OnOpenDocument_Number = 0x1F;
		const char nCDocument_OnNewDocument_Number = 0x1E;

		PtrConv c;

		c.pV=(void*)*(pVtable + nCDocument_DoFileSave_Number);
		m_DoSaveMap[strDocumentClassName]=c.pV;
		c.pDoSave=(PFDOFSAVE)&CMyTextDoc::DoFileSaveMy;
		*(pVtable + nCDocument_DoFileSave_Number)=(DWORD)c.pV;

		c.pV=(void*)*(pVtable + nCDocument_OnCloseDocument_Number);
		m_OnCloseMap[strDocumentClassName]=c.pV;
		c.pOnClose=(PFONCLOSE)&CMyTextDoc::OnCloseDocumentMy;
		*(pVtable + nCDocument_OnCloseDocument_Number)=(DWORD)c.pV;
		
// 		c.pV=(void*)*(pVtable + nCDocument_OnOpenDocument_Number);
// 		m_OnOpenMap[strDocumentClassName]=c.pV;
// 		c.pOnOpen = (PFOnOpenDocument)&CMyTextDoc::OnOpenDocumentMy;
// 		*(pVtable + nCDocument_OnOpenDocument_Number)=(DWORD)c.pV;
// 		
// 		c.pV=(void*)*(pVtable + nCDocument_OnNewDocument_Number);
// 		m_OnNewMap[strDocumentClassName]=c.pV;
// 		c.pOnNew = (PFOnNewDocument)&CMyTextDoc::OnNewDocumentMy;
// 		*(pVtable + nCDocument_OnNewDocument_Number)=(DWORD)c.pV;
	}
	*((DWORD**)pDoc)=pVtable;
	m_Hooked[pDoc->GetPathName()]=pDoc;
}

CMyTextDoc* CDocumentInterceptor::LookupDoc( LPCSTR strDocPath )
{
	CMyTextDoc* pDoc = NULL;
	if(! m_Hooked.Lookup(strDocPath, pDoc) )
		return NULL;

	return pDoc;
}

bool CDocumentInterceptor::HaveUnsavedDocuments()
{
	CMyTextDoc* pDoc;
	CString path;
	for(POSITION pos = m_Hooked.GetStartPosition();pos;)
	{
		m_Hooked.GetNextAssoc(pos, path, pDoc);
		if(pDoc->IsModified() == TRUE)
			return true;
	}
	return false;
}

void CDocumentInterceptor::SendSaveCommandToAllDocuments()
{
	CMyTextDoc* pDoc;
	CString path;
	for(POSITION pos = m_Hooked.GetStartPosition();pos;)
	{
		m_Hooked.GetNextAssoc(pos,path, pDoc);
		
		pDoc->OnCmdMsg(ID_FILE_SAVE,(WM_COMMAND<<16)|CN_COMMAND,NULL,NULL);
	}
}

void CDocumentInterceptor::RunOrig_OnCloseDocument( CMyTextDoc* pDoc )
{
	m_Hooked.RemoveKey(pDoc->m_strPathName);
	PtrConv c;
	c.pV=m_OnCloseMap[pDoc->GetRuntimeClass()->m_lpszClassName];
	(pDoc->*c.pOnClose)();
}

BOOL CDocumentInterceptor::RunOrig_DoFileSave( CMyTextDoc* pDoc )
{
	PtrConv c;
	c.pV=m_DoSaveMap[pDoc->GetRuntimeClass()->m_lpszClassName];
	
	BOOL ret=(pDoc->*c.pDoSave)();
	
	return ret;
}

// BOOL CMyTextDoc::OnOpenDocumentMy( LPCTSTR lpszPathName )
// {
// 	Msg("lpszPathName = <%s>", lpszPathName);
// 
// 	PtrConv c;
// 	c.pV = m_OnOpenMap[GetRuntimeClass()->m_lpszClassName];
// 	BOOL ret = (this->*c.pOnOpen)(lpszPathName);
// 
// 	return ret;
// }
// 
// BOOL CMyTextDoc::OnNewDocumentMy()
// {
// 	Msg("Title = <%s>", m_strTitle);
// 
// 	PtrConv c;
// 	c.pV = m_OnNewMap[GetRuntimeClass()->m_lpszClassName];
// 	BOOL ret = (this->*c.pOnNew)();
// 	
// 	return ret;
// }