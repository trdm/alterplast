// ActiveDoc.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "ActiveDoc.h"
#include "scriptmaneger.h"
#include "formscript.h"
#include "oleformsview.h"
#include "oleclientiemhook.h"
#include "resource.h"


IMPLEMENT_DYNCREATE(CActiveDoc, COleDocument)

CMultiDocTemplate* CActiveDoc::m_pTemplate=NULL;

void CActiveDoc::Init()
{
	HINSTANCE hFrame=GetModuleHandle("frame.dll");
	CRuntimeClass* pChild7=(CRuntimeClass*)GetProcAddress(hFrame,"?classCChildFrame7@CChildFrame7@@2UCRuntimeClass@@B");
	AfxSetResourceHandle(g_hInst);
	m_pTemplate=new CMultiDocTemplate(IDR_MDI1,
			RUNTIME_CLASS(CActiveDoc),
			pChild7,
			RUNTIME_CLASS(COleFormsView));
	pMainApp->AddDocTemplate(m_pTemplate);
	AfxSetResourceHandle(h1CResource);
	m_pTemplate->SetContainerInfo(IDR_MDI1);
}

CActiveDoc::CActiveDoc()
{
}

BOOL CActiveDoc::OnNewDocument()
{
	return TRUE;
}

CActiveDoc::~CActiveDoc()
{
}


BEGIN_MESSAGE_MAP(CActiveDoc, CCmdTarget)
	//{{AFX_MSG_MAP(CActiveDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CActiveDoc::Serialize(CArchive& ar)
{
}

/////////////////////////////////////////////////////////////////////////////
// CActiveDoc commands

IDispatch* CActiveDoc::OpenForm(BSTR progID,BSTR title)
{
	_bstr_t txt=progID;
	if(NULL==(LPCTSTR)txt)
		return NULL;
	try{
		CActiveDoc* pDoc=(CActiveDoc*)m_pTemplate->OpenDocumentFile(txt);
		if(pDoc)
		{
			txt=title;
			pDoc->SetTitle(txt);
			POSITION pos=pDoc->GetStartPosition();
			if(pos)
			{
				COleClientItemHook* pItem=(COleClientItemHook*)pDoc->GetNextItem(pos);
				LPOLEOBJECT pObj=pItem->GetObject();
				IDispatch* pDisp=NULL;
				pObj->QueryInterface(IID_IDispatch,(void**)&pDisp);
				return pDisp;
			}
		}
	}catch(...)
	{
		DoMsgLine("При открытии формы произошла неизвестная ошибка.",mmRedErr);
	}
	return NULL;
}

BOOL CActiveDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	_bstr_t progID=lpszPathName;
	CLSID cls;
	HRESULT hr=CLSIDFromProgID(progID,&cls);
	if(FAILED(hr))
		hr=CLSIDFromString(progID,&cls);
	if(FAILED(hr))
		return FALSE;

	COleClientItemHook* pItem=new COleClientItemHook(this);
	try
	{
		if(!pItem->CreateNewItem(cls))
		{
			delete pItem;
			return FALSE;
		}
	}
	catch(...)
	{
		delete pItem;
		return FALSE;
	}
	return TRUE;
}

BOOL CActiveDoc::SaveModified() 
{
	return TRUE;
}

void CActiveDoc::OpenPlugin(IDispatchPtr &pPlugin)
{
	CActiveDoc* pDoc=(CActiveDoc*)m_pTemplate->OpenDocumentFile(NULL);
	if(pDoc)
		pDoc->OnOpenPlugin(pPlugin);
}

void CActiveDoc::OnOpenPlugin(IDispatchPtr &pPlugin)
{
	COleClientItemHook* pItem=new COleClientItemHook(this);
	try
	{
		pItem->OpenPlugin(pPlugin);
		POSITION pos=GetFirstViewPosition();
		CView* pView=GetNextView(pos);
		pView->OnInitialUpdate();
	}
	catch(...)
	{
		delete pItem;
		return;
	}
}