// IConfig.cpp: implementation of the CIConfig class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "dispimpl.h"
#include "metadataobj.h"
#include "metadata.h"
#include "IConfig.h"
#include "activedoc.h"
#include "pluginmanager.h"
#include "cfgwindows.h"
#include "scripting.h"
#include "scriptmaneger.h"
#include "iplugins.h"
#include "iscripts.h"
#include "icfgdocuments.h"
#include "ITimer.h"
#include "IdleHandler.h"
#include "IMDWindow.h"
#include "ICfgDoc.h"
#include "HotKeyDlg.h"

ITypeInfo* CIConfig::m_pTypeInfo=NULL;

CIConfigEvents CIConfigEvents::m_ConfigEvents;

CIConfig::CIConfig()
{
}
CIConfig::~CIConfig()
{
}

STDMETHODIMP CIConfig::Message(BSTR line,MsgLineMarkers icon)
{
	CString txt=(LPCTSTR)_bstr_t(line);
	txt.Replace("\r\n","\n");
	txt.Replace("\n","\r\n");
	pBkEndUI->DoMessageLine(txt,MessageMarker(icon));
	return S_OK;
}
STDMETHODIMP CIConfig::Status(BSTR text)
{
	CString txt=text;
	pBkEndUI->DoStatusLine(txt);
	return S_OK;
}
STDMETHODIMP CIConfig::get_CommandLine(BSTR* pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString txt=GetCommandLine();
	*pVal=txt.AllocSysString();
	return S_OK;
}
STDMETHODIMP CIConfig::get_IBDir(BSTR* pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CConfigSvcImpl::m_pTheService->m_IBPath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CIConfig::get_BinDir(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CConfigSvcImpl::m_pTheService->m_BinPath.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIConfig::get_IBName(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=pMainApp->GetProps()->GetStringProp(appIBName).AllocSysString();
	return S_OK;
}
STDMETHODIMP CIConfig::put_IBName(BSTR newVal)
{
	HRESULT hr=E_FAIL;
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\1C\\1Cv7\\7.7\\Titles",0,KEY_SET_VALUE,&hKey)==ERROR_SUCCESS)
	{
		CString txt=newVal;
		if(RegSetValueEx(hKey,CConfigSvcImpl::m_pTheService->m_IBPath,0,REG_SZ,
			(CONST BYTE*)(LPCTSTR)txt,txt.GetLength()+1)==ERROR_SUCCESS)
		{
			pMainApp->GetProps()->SetStringProp(appIBName,txt);
			hr=S_OK;
		}
		RegCloseKey(hKey);
	}
	return hr;
}
STDMETHODIMP CIConfig::OpenOLEForm(BSTR progID,BSTR title,IDispatch** ppDisp)
{
	if(IsBadWritePtr(ppDisp,sizeof(*ppDisp)))
		return E_POINTER;
	HRESULT hr=S_OK;
	*ppDisp=CActiveDoc::OpenForm(progID,title);
	return hr;
}

STDMETHODIMP CIConfig::get_MetaData(IMetaData **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CIMetaData::m_pOneMetaData;
	(*pVal)->AddRef();
	return S_OK;
}
STDMETHODIMP CIConfig::SendCommand(ConfigCommands cmd)
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND,(WPARAM)cmd,0);
	return S_OK;
}
STDMETHODIMP CIConfig::IsConfigWndOpen(VARIANT_BOOL *pResult)
{
	if(IsBadWritePtr(pResult,sizeof(*pResult)))
		return E_POINTER;
	POSITION pos=CIMetaData::m_pOneMetaData->m_pTemplateOfMetaData->GetFirstDocPosition();
	*pResult=pos?-1:0;
	return S_OK;
}

STDMETHODIMP CIConfig::get_Plugins(IPlugins **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	static CIPlugins* pPlugins=new CIPlugins;
	*pVal=pPlugins;
	pPlugins->AddRef();
	return S_OK;
}
STDMETHODIMP CIConfig::get_Scripts(IScripts **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	static CIScripts* pScript=new CIScripts;
	*pVal=pScript;
	pScript->AddRef();
	return S_OK;
}

STDMETHODIMP CIConfig::get_Windows(ICfgWindows **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CICfgWindows::m_pCfgWindows;
	(*pVal)->AddRef();
	return S_OK;
}
static void CopyMap(CMapPtrToPtr& map1,CMapPtrToPtr& map2)
{
	void* key;
	void* val;
	for(POSITION pos=map1.GetStartPosition();pos;)
	{
		map1.GetNextAssoc(pos,key,val);
		map2[key]=val;
	}
}
void CIConfigEvents::FireEventNonParam(DISPID idEvent)
{
	void* key;
	IDispatch* pDisp;
	DISPPARAMS dNone={NULL,NULL,0,0};
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(idEvent,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dNone,NULL,NULL,NULL);
			}
			catch(...)
			{
			}
		}
	}
}
void CIConfigEvents::FireOpenConfigWnd()
{
	FireEventNonParam(0);
}
void CIConfigEvents::FireAllPluginsInit()
{
	FireEventNonParam(1);
}
void CIConfigEvents::FireMDSaved(CString fName)
{
	void* key;
	IDispatch* pDisp;
	VARIANT var;
	var.vt=VT_BSTR;
	var.bstrVal=fName.AllocSysString();
	DISPPARAMS dArg={&var,NULL,1,0};
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(2,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch(...){}
		}
	}
	VariantClear(&var);
}

int CIConfigEvents::FireDoModal(CDialog* pDlg)
{
	HWND hWnd=pDlg->m_hWnd;
	CString caption;
	pDlg->GetWindowText(caption);
	VARIANT answer;
	answer.vt=VT_I4;
	answer.lVal=0;
	long LastAnswer=0;
	VARIANTARG vars[3];
	vars[0].vt=VT_VARIANT|VT_BYREF;
	vars[0].pvarVal=&answer;
	vars[1].vt=VT_BSTR;
	vars[1].bstrVal=caption.AllocSysString();
	vars[2].vt=VT_I4;
	vars[2].lVal=(long)hWnd;
	DISPPARAMS dArg={vars,NULL,3,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(5,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
				if(hr==S_OK)
				{
					if(answer.vt!=VT_I4)
						VariantChangeType(&answer,&answer,0,VT_I4);
					if(answer.vt==VT_I4)
						LastAnswer=answer.lVal;
				}
			}catch(...)
			{
			}
		}
	}
	SysFreeString(vars[1].bstrVal);
	return LastAnswer;
}
int CIConfigEvents::FireFileDialog(bool bSaved,CString lpTitle,CString lpFilter,CString& fName)
{
	/*[id(4), helpstring("Вызывается в \"тихом\" режиме вместо запроса имени файла")]
		HRESULT OnFileDialog([in] VARIANT_BOOL Saved,
		[in] BSTR Caption,[in] BSTR Filter,[in,out] BSTR* FileName, [in,out] VARIANT* Answer);
		*/
	VARIANT ans;
	VARIANT vname;
	ans.vt=VT_I4;
	ans.lVal=0;
	vname.vt=VT_BSTR;
	vname.bstrVal=fName.AllocSysString();
	
	VARIANTARG var[5];
	var[0].vt=VT_VARIANT|VT_BYREF;
	var[0].pvarVal=&ans;
	var[1].vt=VT_VARIANT|VT_BYREF;
	var[1].pvarVal=&vname;
	var[2].vt=VT_BSTR;
	var[2].bstrVal=lpFilter.AllocSysString();
	var[3].vt=VT_BSTR;
	var[3].bstrVal=lpTitle.AllocSysString();
	var[4].vt=VT_BOOL;
	var[4].boolVal=bSaved?-1:0;

	DISPPARAMS dArg={var,NULL,5,0};
	IDispatch* pDisp;
	void* key;
	VARIANT_BOOL LastAnswer=0;
	_bstr_t LastFName=fName;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(4,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
				if(hr==S_OK)
				{
					if(ans.vt!=VT_I4)
						VariantChangeType(&ans,&ans,0,VT_I4);
					if(ans.vt==VT_I4)
					{

						LastAnswer=ans.lVal;
						LastFName=vname;
					}
				}
			}catch (...) {
			}
		}
	}
	VariantClear(&vname);
	fName=(LPCTSTR)LastFName;
	return LastAnswer;
}
int CIConfigEvents::FireMsgBox(CString text,UINT styles)
{
	long style=styles & 0xF;
	long answer;
	int AllowAnswers[8]={1,0,0,0,0,0,0,0};
	switch(style)
	{
	case MB_OKCANCEL:
		AllowAnswers[IDCANCEL]=1;
	case MB_OK:
		AllowAnswers[IDOK]=1;
		answer=IDOK;
		break;
	case MB_YESNOCANCEL:
		AllowAnswers[IDCANCEL]=1;
	case MB_YESNO:
		AllowAnswers[IDYES]=1;
		AllowAnswers[IDNO]=1;
		answer=IDYES;
		break;
	case MB_ABORTRETRYIGNORE:
		AllowAnswers[IDABORT]=1;
		AllowAnswers[IDRETRY]=1;
		AllowAnswers[IDIGNORE]=1;
		answer=IDABORT;
		break;
	case MB_RETRYCANCEL:
		AllowAnswers[IDRETRY]=1;
		AllowAnswers[IDCANCEL]=1;
		answer=IDRETRY;
	};
	if(styles & MB_DEFBUTTON2)
	{
		switch(style)
		{
		case MB_OKCANCEL:
		case MB_RETRYCANCEL:
			answer=IDCANCEL;
			break;
		case MB_YESNO:
		case MB_YESNOCANCEL:
			answer=IDNO;
			break;
		case MB_ABORTRETRYIGNORE:
			answer=IDRETRY;
		};
	}
	else if(styles & MB_DEFBUTTON3)
	{
		switch(style)
		{
		case MB_YESNOCANCEL:
			answer=IDCANCEL;
			break;
		case MB_ABORTRETRYIGNORE:
			answer=IDIGNORE;
		};
	}
	VARIANT ans;
	ans.vt=VT_I4;
	ans.lVal=0;

	VARIANTARG vars[4];
	vars[0].vt=VT_VARIANT|VT_BYREF;
	vars[0].pvarVal=&ans;
	vars[1].vt=VT_I4;
	vars[1].lVal=answer;
	vars[2].vt=VT_I4;
	vars[2].lVal=style;
	vars[3].vt=VT_BSTR;
	vars[3].bstrVal=text.AllocSysString();
	DISPPARAMS dArg={vars,NULL,4,0};
	IDispatch* pDisp;
	void* key;
	long LastAnswer=0;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(3,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
				if(hr==S_OK)
				{
					if(ans.vt!=VT_I4)
						VariantChangeType(&ans,&ans,0,VT_I4);
					if(ans.vt==VT_I4 && ans.lVal>=0 && ans.lVal<8 && AllowAnswers[ans.lVal])
						LastAnswer=ans.lVal;
				}
			}catch (...) {
			}
		}
	}
	SysFreeString(vars[3].bstrVal);
	return LastAnswer;
}

static bool gPromptSave;
static void QProc()
{
	if(gPromptSave)
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
	else
		TerminateProcess(GetCurrentProcess(),0);
	CIdleHandler::RemoveIdleHandler(QProc);
}

STDMETHODIMP CIConfig::Quit(VARIANT_BOOL PromptSave)
{
	gPromptSave=PromptSave?true:false;
	CIdleHandler::SetIdleHandler(QProc);
	return S_OK;
}

STDMETHODIMP CIConfig::get_Version(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	long version=1001;
	CString fname=CConfigSvcImpl::m_pTheService->m_BinPath+"config.dll";
	DWORD z,size;
	size=GetFileVersionInfoSize((LPTSTR)(LPCTSTR)fname,&z);
	if(size)
	{
		char* pVersion=new char[size];
		GetFileVersionInfo((LPTSTR)(LPCTSTR)fname,0,size,pVersion);
		DWORD* translate;
		UINT len;
		VerQueryValue(pVersion,"\\VarFileInfo\\Translation",(void**)&translate,&len);
		CString blockName;
		blockName.Format("\\StringFileInfo\\%04x%04x\\",LOWORD(translate[0]),HIWORD(translate[0]));
		LPCTSTR valuebuf;
		CString fdName=blockName+"FileVersion";
		VerQueryValue(pVersion,(char*)(LPCTSTR)fdName,(void**)&valuebuf,&len);
		if(len)
		{
			CString vers=valuebuf;
			vers.Replace(".","");
			version=atol(vers);
		}
		delete pVersion;
	}

	*pVal=version;
	return S_OK;
}

STDMETHODIMP CIConfig::LogMessageWrite(EventCategories EventCategory,BSTR Message,
						BSTR strEventType,BSTR Event)
{
	CAdminService* pLog=pMainApp->GetLog();
	if(!pLog)
		return SetError(E_FAIL,"Внутренняя ошибка");

	CString msg=Message;
	CString bstrEventType=strEventType;
	CString bstrEvent=Event;
	pLog->ReportUserEvent(bstrEventType,bstrEvent,(EventType)EventCategory,msg,NULL,"");
	return S_OK;
}

STDMETHODIMP CIConfig::Run1CApp(RunAppModes mode)
{
	if(!(mode==-1 || mode==2 || mode==4))
		return S_OK;
	pMainApp->StartNewProcess(CAppRunMode(mode));
	return S_OK;
}

STDMETHODIMP CIConfig::get_Documents(ICfgDocuments **pVal)
{
	static CICfgDocuments* pDocs=new CICfgDocuments;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CICfgDocuments::m_pDocuments;
	(*pVal)->AddRef();
	return S_OK;
}

STDMETHODIMP CIConfig::get_CfgTimer(ICfgTimer **pVal)
{
	static CITimer* pTimer=new CITimer;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=pTimer;
	pTimer->AddRef();
	return S_OK;
}

void CIConfigEvents::FireTimer(long id)
{
	VARIANTARG vars;
	vars.vt=VT_I4;
	vars.lVal=id;
	DISPPARAMS dArg={&vars,NULL,1,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				pDisp->Invoke(6,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch (...) {
			}
		}
	}
}

void CIConfigEvents::FireOnIdle()
{
	FireEventNonParam(7);
}

void CIConfigEvents::FireActivateMDIWnd(ICfgWindow* pWnd,bool bActive)
{
	VARIANTARG vars[2];
	vars[0].vt=VT_BOOL;
	vars[0].boolVal=bActive?-1:0;
	vars[1].vt=VT_DISPATCH;
	vars[1].pdispVal=(IDispatch*)pWnd;
	DISPPARAMS dArg={vars,NULL,2,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(8,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch(...)
			{
			}
		}
	}
}

void CIConfigEvents::FireActivateMainWnd(bool bActive)
{
	VARIANTARG vars[1];
	vars[0].vt=VT_BOOL;
	vars[0].boolVal=bActive?-1:0;
	DISPPARAMS dArg={vars,NULL,1,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(9,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch(...)
			{
			}
		}
	}
}

STDMETHODIMP CIConfig::get_AppProps(AppProperties idx, BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CProfile7* pProf=pMainApp->GetProps();
	CString ret;
	if(pProf)
		ret=pProf->GetStringProp(idx);
	*pVal=ret.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIConfig::get_MDWnd(IMDWindow **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CIMDWindow::m_pMDWindow;
	(*pVal)->AddRef();
	return S_OK;
}

void CIConfigEvents::FireFileSave(CDocument *pDoc)
{
	VARIANTARG vars[1];
	vars[0].vt=VT_DISPATCH;
	vars[0].pdispVal=CICfgDoc::CreateDoc(pDoc);

	DISPPARAMS dArg={vars,NULL,1,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(10,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch(...)
			{
			}
		}
	}
}

bool CIConfigEvents::OnQueryQuit()
{
	DISPPARAMS dArg={NULL,NULL,0,0};
	VARIANT varResult;
	bool retVal=false;

	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				varResult.vt=VT_BOOL;
				HRESULT hr=pDisp->Invoke(11,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,&varResult,NULL,NULL);
				if(S_OK==hr && varResult.vt==VT_BOOL)
					retVal=varResult.boolVal==VARIANT_TRUE;
				VariantClear(&varResult);
				if(retVal)
					return true;
			}catch(...)
			{
			}
		}
	}
	return false;
}

void CIConfigEvents::OnQuit()
{
	FireEventNonParam(12);
}

void CIConfigEvents::Done()
{
	m_ConfigEvents.OnQuit();
}

STDMETHODIMP CIConfig::get_CancelHotKey(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CHotKeyDlg::m_CancelHotkey?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CIConfig::put_CancelHotKey(VARIANT_BOOL newVal)
{
	CHotKeyDlg::m_CancelHotkey=newVal==VARIANT_TRUE;
	return S_OK;
}

void CIConfigEvents::FireDialogChange(CDocument *pDoc)
{
	VARIANTARG vars[1];
	vars[0].vt=VT_DISPATCH;
	vars[0].pdispVal=CICfgDoc::CreateDoc(pDoc);

	DISPPARAMS dArg={vars,NULL,1,0};
	IDispatch* pDisp;
	void* key;
	CMapPtrToPtr mapAdvised;
	CopyMap(m_Advised,mapAdvised);
	for(POSITION pos=mapAdvised.GetStartPosition();pos;)
	{
		mapAdvised.GetNextAssoc(pos,key,(void*&)pDisp);
		if(m_Advised.Lookup(key,(void*&)pDisp))
		{
			try{
				HRESULT hr=pDisp->Invoke(13,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&dArg,NULL,NULL,NULL);
			}catch(...)
			{
			}
		}
	}
}
