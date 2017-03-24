// FormScript.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "FormScript.h"
#include "Formcnt.h"
//#include "1cheaders\userdef.h"
#include "scripting.h"
#include "scriptmaneger.h"
#include "oleclientiemhook.h"
/////////////////////////////////////////////////////////////////////////////
// CFormScript

IMPLEMENT_DYNCREATE(CFormScript, COleDocument)

CFormScript::CFormScript()
{
	EnableAutomation();
	EnableCompoundFile();
	m_pScript=NULL;
	m_pUserForm=NULL;
}

BOOL CFormScript::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;
	COleClientItemHook* pItem=(COleClientItemHook*)new COleClientItemHook(this);
	CLSID cls;
	CLSIDFromProgID(L"Forms.Form.1",&cls);
	pItem->CreateNewItem(cls);
	pItem->GetObject()->QueryInterface(__uuidof(_UserForm),(void**)&m_pUserForm);
	POSITION pos=GetFirstViewPosition();
	if(pos)
	{
		SEC3DTabWnd* pWnd=(SEC3DTabWnd*)GetNextView(pos);
		CCreateContext ctx;
		ctx.m_pCurrentDoc=this;
		ctx.m_pNewViewClass=RUNTIME_CLASS(CFormCnt);
		pWnd->AddTab(ctx.m_pNewViewClass,"Form",&ctx,0);
	}
	return TRUE;
}

CFormScript::~CFormScript()
{
	if(m_pUserForm)
		m_pUserForm->Release();
}

void CFormScript::OnFinalRelease()
{
	CDocument::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CFormScript, COleDocument)
	//ON_COMMAND(ID_FILE_SAVE,OnSave)
	//{{AFX_MSG_MAP(CFormScript)
	ON_COMMAND(ID_DEBUG_CONSTRUCT, OnDebugConstruct)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_CONSTRUCT, OnUpdateDebugConstruct)
	ON_COMMAND(ID_SHOW_TEXT, OnShowText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFormScript, COleDocument)
	//{{AFX_DISPATCH_MAP(CFormScript)
	DISP_PROPERTY_EX(CFormScript, "Caption", GetCaption, SetCaption, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFormScript to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {930A3096-F138-11D7-8D5E-B340FDE9FB02}
static const IID IID_IFormScript =
{ 0x930a3096, 0xf138, 0x11d7, { 0x8d, 0x5e, 0xb3, 0x40, 0xfd, 0xe9, 0xfb, 0x2 } };
BEGIN_INTERFACE_MAP(CFormScript, CDocument)
	INTERFACE_PART(CFormScript, IID_IFormScript, Dispatch)
END_INTERFACE_MAP()

void CFormScript::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
	try{
	COleDocument::Serialize(ar);
	}catch(COleException* pEx)
	{
		long err=pEx->m_sc;
		pEx->Delete();
	}
	catch(CException* pE)
	{
		pE->Delete();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFormScript commands

BSTR CFormScript::GetCaption() 
{
	CString strResult;
	m_pWnd->GetWindowText(strResult);
	return strResult.AllocSysString();
}

void CFormScript::SetCaption(LPCTSTR lpszNewValue) 
{
	m_pWnd->SetWindowText(lpszNewValue);
}

BOOL CFormScript::OnOpenDocument(LPCTSTR lpszPathName)
{
	HRESULT hr;
	_bstr_t path=lpszPathName;
	hr=StgOpenStorage(path,NULL,STGM_TRANSACTED|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,NULL,0,&m_lpRootStg);
	if(FAILED(hr))
		return FALSE;
	COleClientItemHook* pItem=(COleClientItemHook*)new COleClientItemHook(this);
	pItem->Load(m_lpRootStg);
	pItem->GetObject()->QueryInterface(__uuidof(_UserForm),(void**)&m_pUserForm);
	IPersistStoragePtr pers=m_pUserForm;
	pers->HandsOffStorage();
	m_lpRootStg->Release();
	m_lpRootStg=NULL;
	return TRUE;
}
BOOL CFormScript::OnSaveDocument(LPCTSTR lpszPathName)
{
	BOOL bSameAsLoad=FALSE;//m_strPathName.CompareNoCase(lpszPathName)==0;
	IStoragePtr pStorage=NULL;
	HRESULT hr;
	_bstr_t path=lpszPathName;
	if(bSameAsLoad)
		pStorage=m_lpRootStg;
	else
	{
		hr=StgOpenStorage(path,NULL,STGM_TRANSACTED|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,NULL,0,&pStorage);
		if(FAILED(hr))
			hr=StgCreateDocfile(path,STGM_CREATE|STGM_TRANSACTED|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStorage);
		if(FAILED(hr))
			return FALSE;
	}
	IPersistStoragePtr pPersist=m_pUserForm;
	if(pPersist!=NULL)
	{
		pPersist->SaveCompleted(pStorage);
		hr=OleSave(pPersist,pStorage,bSameAsLoad);
		if(FAILED(hr))
			return FALSE;
		pPersist->HandsOffStorage();
	}
	else
		return FALSE;
	IStreamPtr pStream=NULL;
	path="FORM_MODULE";
	if(bSameAsLoad)
		hr=pStorage->OpenStream(path,NULL,STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStream);
	else
		hr=pStorage->CreateStream(path,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,0,&pStream);
	if(FAILED(hr))
		return FALSE;
	ULONG writed;
	pStream->Write("hello",5,&writed);
	pStream->Commit(1);
	pStorage->Commit(1);
	return TRUE;
}

LPDISPATCH CFormScript::GetControl(LPCTSTR name)
{
	ControlsPtr ctrls=m_pUserForm->GetControls();
	if(ctrls!=NULL)
	{
		try{
		IControlPtr ctrl=ctrls->_GetItemByName(name);
		if(ctrl!=NULL)
		{
			IDispatchPtr disp=ctrl;
			return disp;
		}
		}catch(...)
		{
		}
	}
	return NULL;
}

void CFormScript::OnCreateForm(CWnd *pWnd)
{
	return;
}

void CFormScript::OnDebugConstruct() 
{
	if(m_pUserForm!=NULL)
	{
		fmMode mode;
		m_pUserForm->get_DesignMode(&mode);
		if(mode==fmModeOn)
			RunScript();
		else
			StopScript();
	}
}

void CFormScript::OnUpdateDebugConstruct(CCmdUI* pCmdUI) 
{
	POSITION pos=GetStartPosition();
	COleClientItemHook* pItem=(COleClientItemHook*)GetNextItem(pos);
	if(pItem)
	{
		_UserFormPtr pUF=pItem->GetObject();
		if(pUF!=NULL)
		{
			fmMode mode;
			pUF->get_DesignMode(&mode);
			if(mode==fmModeOn)
				pCmdUI->SetText("Выполнить");
			else
				pCmdUI->SetText("Конструирование");
		}
	}
}

void CFormScript::OnShowText() 
{
	m_ModuleText="Hello, World!!!";
	CWinApp* pApp=AfxGetApp();
	POSITION pos=pApp->GetFirstDocTemplatePosition();
	CString txt="CBodyTextDoc";
	while(pos)
	{
		CMyDocTemp* pTemp=(CMyDocTemp*)pApp->GetNextDocTemplate(pos);
		if(txt==pTemp->GetDocClass()->m_lpszClassName)
		{
			CBodyTextDoc* pDoc=(CBodyTextDoc*)pTemp->GetDocClass()->CreateObject();
			pDoc->OnNewDocument();
			CCreateContext ctx;
			ctx.m_pCurrentDoc=pDoc;
			ctx.m_pNewViewClass=pTemp->GetViewClass();
			POSITION posView=GetFirstViewPosition();
			CView* pView=GetNextView(posView);
			CFrameWnd* pFrame=pView->GetParentFrame();
			SetWindowLong(pView->m_hWnd,GWL_ID,AFX_IDW_PANE_FIRST+1);
			CView* pNewView=(CView*)pFrame->CreateView(&ctx);
			SetWindowLong(pNewView->m_hWnd,GWL_ID,AFX_IDW_PANE_FIRST);
			pFrame->SetActiveView(pNewView);
			pView->ShowWindow(SW_HIDE);
			pNewView->OnInitialUpdate();
			pNewView->ShowWindow(SW_SHOW);
			pFrame->RecalcLayout();
			pDoc->SetText(m_ModuleText);
		}
	}
}

void CFormScript::RunScript()
{
	m_pScript=new CScripting;
	m_pScript->LoadScript("c:\\temp\\script.vbs");
	long cnt=m_pUserForm->Controls->Count;
	for(long i=0;i<cnt;i++)
		m_pScript->m_pEngine->AddNamedItem(m_pUserForm->Controls->_GetItemByIndex(i)->Name,SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE);
	m_pScript->m_pEngine->AddNamedItem(L"Config",SCRIPTITEM_GLOBALMEMBERS|SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE);
	m_pUserForm->DesignMode=fmModeOff;
	m_pUserForm->ShowGridDots=fmModeOff;
	m_pScript->m_pEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
}

void CFormScript::StopScript()
{
	if(m_pScript)
	{
		m_pScript->m_pEngine->SetScriptState(SCRIPTSTATE_CLOSED);
		delete m_pScript;
		m_pScript=NULL;
	}
	m_pUserForm->DesignMode=fmModeOn;
	m_pUserForm->ShowGridDots=fmModeOn;
}
