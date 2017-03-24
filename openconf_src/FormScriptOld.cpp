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
#include "ICfgDocuments.h"
#include "FormHolder.h"
/////////////////////////////////////////////////////////////////////////////
// CFormScript

IMPLEMENT_DYNCREATE(CFormScript, COleDocument)

CFormScript::CFormScript()
{
	m_pForm=new CFormHolder;
}

BOOL CFormScript::OnNewDocument()
{
	if (!COleDocument::OnNewDocument())
		return FALSE;
	COleClientItemHook* pItem=(COleClientItemHook*)new COleClientItemHook(this);
	CLSID cls;
	CLSIDFromProgID(L"Forms.Form.1",&cls);
	if(!pItem->CreateNewItem(cls))
		return FALSE;
	m_pUserForm=pItem->GetObject();
	return TRUE;
}

CFormScript::~CFormScript()
{
	delete m_pForm;
}

BEGIN_MESSAGE_MAP(CFormScript, COleDocument)
	//ON_COMMAND(ID_FILE_SAVE,OnSave)
	//{{AFX_MSG_MAP(CFormScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormScript commands
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


	IStreamPtr pStream=NULL;
	path="FORM_MODULE";
	hr=m_lpRootStg->OpenStream(path,NULL,STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStream);
	if(FAILED(hr))
		return FALSE;
	STATSTG stat;
	pStream->Stat(&stat,0);
	ULONG readed;
	pStream->Read(m_module.GetBufferSetLength(stat.cbSize.QuadPart),stat.cbSize.QuadPart,&readed);
	m_module.ReleaseBuffer();

	m_lpRootStg->Release();
	m_lpRootStg=NULL;

	return TRUE;
}

BOOL CFormScript::OnSaveDocument(LPCTSTR lpszPathName)
{
	IStoragePtr pStorage=NULL;
	HRESULT hr;
	_bstr_t path=lpszPathName;

	hr=StgOpenStorage(path,NULL,STGM_TRANSACTED|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,NULL,0,&pStorage);
	if(FAILED(hr))
		hr=StgCreateDocfile(path,STGM_CREATE|STGM_TRANSACTED|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStorage);
	if(FAILED(hr))
		return FALSE;

	IPersistStoragePtr pPersist=m_pUserForm;
	if(pPersist!=NULL)
	{
		pPersist->SaveCompleted(pStorage);
		hr=OleSave(pPersist,pStorage,FALSE);
		if(FAILED(hr))
			return FALSE;
		pPersist->HandsOffStorage();
	}
	else
		return FALSE;

	IStreamPtr pStream=NULL;
	path="FORM_MODULE";
	hr=pStorage->OpenStream(path,NULL,STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&pStream);
	if(FAILED(hr))
		hr=pStorage->CreateStream(path,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,0,&pStream);
	if(FAILED(hr))
		return FALSE;
	m_module="Sub Hello()\r\n\tMsgBox \"Hello World\"\r\nEnd Sub";
	ULONG writed;
	pStream->Write((LPCTSTR)m_module,m_module.GetLength(),&writed);
	pStream->Commit(1);
	pStorage->Commit(1);
	return TRUE;
}

CMultiDocTemplate* CFormScript::m_pFormTemplate=NULL;

CFormScript* CFormScript::CreateForm(CString path)
{
	if(!m_pFormTemplate)
	{
		AfxSetResourceHandle(g_hInst);
		m_pFormTemplate=new CMultiDocTemplate(IDR_MDI,
			RUNTIME_CLASS(CFormScript),
			CICfgDocuments::m_pUsefulRTC[5],
			RUNTIME_CLASS(CFormCnt));
		pMainApp->AddDocTemplate(m_pFormTemplate);
		AfxSetResourceHandle(h1CResource);
	}
	LPCTSTR lpPath=NULL;
	if(!path.IsEmpty())
		lpPath=path;
	CFormScript* pForm=(CFormScript*)m_pFormTemplate->OpenDocumentFile(lpPath,TRUE);
	return pForm;
}

bool CFormScript::OpenForm()
{
	return true;
}

BOOL CFormScript::IsModified()
{
	IPersistStoragePtr stg=m_pForm->m_pForm;
	if(stg!=NULL)
	{
		return S_OK==stg->IsDirty();
	}
	return FALSE;
}
