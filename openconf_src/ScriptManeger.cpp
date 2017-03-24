// ScriptManeger.cpp: implementation of the CScriptManager class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ScriptManeger.h"
#include "iconfig.h"
#include "IdleHandler.h"
#include "OCView.h"
#include "iselfscript.h"

CScriptManager* CScriptManager::m_pOneManager=NULL;

CScriptManager::CScriptManager()
{
	m_pProcDebugManager=NULL;
	m_pDebugApplication=NULL;
	CScripting::m_mapEngineNames[".vbs"]="VBScript";
	CScripting::m_mapEngineNames[".js"] ="JScript";
	//CScripting::m_mapEngineNames[".pys"]="Python";
	//CScripting::m_mapEngineNames[".pls"]="PerlScript";
}
CScriptManager::~CScriptManager()
{
	for(int i=m_ScriptsArray.GetSize()-1;i>=0;i--)
	{
		CScripting* pScript=(CScripting*)m_ScriptsArray[i];
		pScript->Unload();
	}
	if(m_pOneManager->m_pDebugApplication)
	{
		m_pOneManager->m_pProcDebugManager->RemoveApplication(m_pOneManager->m_dwDebugCookie);
		m_pOneManager->m_pDebugApplication->Release();
		m_pOneManager->m_pDebugApplication=NULL;
	}
	if(m_pOneManager->m_pProcDebugManager)
	{
		m_pOneManager->m_pProcDebugManager->Release();
		m_pOneManager->m_pProcDebugManager=NULL;
	}
}

HRESULT CScriptManager::GetItemInfo(CScripting* pScript,LPCOLESTR pstrName,DWORD dwReturnMask,
									IUnknown **ppiunkItem, ITypeInfo **ppti)
{
	if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		if(IsBadWritePtr(ppti,sizeof(ITypeInfo*)))
			return E_POINTER;
		*ppti=NULL;
	}
	if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		if(IsBadWritePtr(ppiunkItem,sizeof(IUnknown*)))
			return E_POINTER;
		*ppiunkItem= NULL;
	}
	bool found=false;
	CString name=(LPCTSTR)_bstr_t(pstrName);
	name.MakeLower();
	if(name=="configurator")
	{
		found=true;
		if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
			CConfigSvcImpl::m_pTheService->m_pTypeLib->GetTypeInfoOfGuid(CLSID_Configurator,ppti);
		if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
			*ppiunkItem=(IDispatch*)CConfigSvcImpl::m_pTheService->m_Configurator;
	}
	else if(name=="selfscript")
	{
		found=true;
		if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
			pScript->m_pSelf->GetTypeInfo(0,0,ppti);
		if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
			*ppiunkItem=(IDispatch*)pScript->m_pSelf;
	}
	else
	{
		IDispatch* pDisp;
		if(pScript->m_pSelf->m_Objects.Lookup(name,(void*&)pDisp))
		{
			found=true;
			if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
			{
				IProvideClassInfoPtr pci=pDisp;
				if(pci!=NULL)
					pci->GetClassInfo(ppti);
				else
					pDisp->GetTypeInfo(0,0,ppti);
			}
			if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
				*ppiunkItem=pDisp;
		}
	}
	if(!found)
		return TYPE_E_ELEMENTNOTFOUND;
	if((dwReturnMask & SCRIPTINFO_IUNKNOWN) && *ppiunkItem)
		(*ppiunkItem)->AddRef();
	return S_OK;
}

void CScriptManager::InitScriptManeger()
{
	if(m_pOneManager)
		return;
	m_pOneManager=new CScriptManager;
	if(!m_pOneManager->Prepare())
	{
		delete m_pOneManager;
		m_pOneManager=NULL;
	}
}

void CScriptManager::LoadScripts(const CString dir)
{
	CString src=dir+"*.*";
	WIN32_FIND_DATA fnd;
	HANDLE hFnd=FindFirstFile(src,&fnd);
	if(hFnd!=(HANDLE)-1)
	{
		do{
			try{
				if(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(fnd.cFileName[0]=='.')
					{
						if(!fnd.cFileName[1] || (fnd.cFileName[1]=='.' && !fnd.cFileName[2]))
							continue;
					}
					LoadScripts(dir+fnd.cFileName+"\\");
					continue;
				}
				CString path=dir+fnd.cFileName;
				if(!GetScriptByPath(path))
					LoadScript(path);
			}catch(...){}
		}while(FindNextFile(hFnd,&fnd));
		FindClose(hFnd);
	}
}

bool CScriptManager::Prepare()
{
	bool IsDebugInit=false;
	if(!m_pProcDebugManager)
	{
		HRESULT hr = CoCreateInstance(CLSID_ProcessDebugManager, NULL, 
		CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, 
		IID_IProcessDebugManager, (void **)&m_pProcDebugManager);
		//If that failed, the facilities aren't available for smart debugging
		if(hr==S_OK)
		{
			//Next, create a debugger application to associate with the script we're 
			//running.  An "application" is the smallest unit that can be debugged.
			hr = m_pProcDebugManager->CreateApplication(&m_pDebugApplication);
			if(hr==S_OK)
			{
				//Set the name that will appear in the debugger heirarchy for this app.
				_bstr_t AppName=L"OpenConf - ";
				BSTR ibName;
				CConfigSvcImpl::m_pTheService->m_Configurator->get_IBName(&ibName);
				AppName+=ibName;
				hr = m_pDebugApplication->SetName(AppName);
				if(hr==S_OK)
				{
					//Add the debugger application to the debug heirarchy of the debug manager
					//so it will appear in the debug window.
					hr = m_pProcDebugManager->AddApplication(m_pDebugApplication, 
						&m_dwDebugCookie);
					if(hr==S_OK)
					{
						IsDebugInit=((m_pDebugApplication->FIsAutoJitDebugEnabled()==TRUE) &&
							m_pDebugApplication->FCanJitDebug()==TRUE);
					}
				}
			}
		}
	}
	
	if(!IsDebugInit)
	{
		if(m_pDebugApplication)
		{
			m_pProcDebugManager->RemoveApplication(m_dwDebugCookie);
			m_pDebugApplication->Release();
			m_pDebugApplication=NULL;
		}
		if(m_pProcDebugManager)
		{
			m_pProcDebugManager->Release();
			m_pProcDebugManager=NULL;
		}
	}


	DoStsLine("Загрузка скриптов...");
	try{
		//CConfigSvcImpl::m_pTheService->m_Configurator->AddRef();
		LoadScripts(CConfigSvcImpl::m_pTheService->m_BinPath+"config\\scripts\\");
	}
	catch(_com_error err)
	{
		DoMsgLine(err.ErrorMessage(),mmRedErr);
		return false;
	}
	catch(...)
	{
		DoMsgLine("Ошибка при загрузке скриптов",mmRedErr);
		return false;
	}
	return true;
}

CScripting* CScriptManager::GetScriptByName(CString name)
{
	name.MakeLower();
	CScripting* pScript=NULL;
	if(m_ScriptsByName.Lookup(name,(void*&)pScript))
		return pScript;
	return NULL;
}

void CScriptManager::Done()
{
	if(m_pOneManager)
	{
		delete m_pOneManager;
		m_pOneManager=NULL;
	}
}

// Должна вызываться скриптом после определения имени скрипта
// и создания движка до запуска движка.
bool CScriptManager::RegisterScript(CScripting *pScript)
{
	if(GetScriptByName(pScript->m_ScriptName))
	{
		DoMsgLine("Скрипт с именем \"%s\" уже загружен",mmRedErr,pScript->m_ScriptName);
		return false;
	}
	m_ScriptsArray.Add(pScript);
	CString lo=pScript->m_ScriptName;
	lo.MakeLower();
	m_ScriptsByName[lo]=pScript;
	lo=pScript->m_FileName;
	lo.MakeLower();
	m_ScriptsByPath[lo]=pScript;
	return true;
}

bool CScriptManager::UnRegisterScript(CScripting *pScript)
{
	for(int i=0;i<m_ScriptsArray.GetSize();i++)
	{
		if(m_ScriptsArray[i]==pScript)
		{
			m_ScriptsArray.RemoveAt(i);
			CString lo=pScript->m_ScriptName;
			lo.MakeLower();
			m_ScriptsByName.RemoveKey(lo);
			lo=pScript->m_FileName;
			lo.MakeLower();
			m_ScriptsByPath.RemoveKey(lo);
			return true;
		}
	}
	return false;
}

CScripting* CScriptManager::LoadScript(CString strFileName)
{
	if(strFileName.IsEmpty())
	{
		CString filter,allExt;
		for(POSITION pos=CScripting::m_mapEngineNames.GetStartPosition();pos;)
		{
			CString name,ext;
			CScripting::m_mapEngineNames.GetNextAssoc(pos,ext,name);
			filter=filter+"|"+name+" (*"+ext+")|*"+ext;
			if(!allExt.IsEmpty())
				allExt+=";";
			allExt=allExt+"*"+ext;
		}
		filter="Скрипты ("+allExt+")|"+allExt+filter+"|Все файлы|*.*||";
		CFileDialog fdlg(TRUE,NULL,NULL,0,filter);
		if(fdlg.DoModal()!=IDOK)
			return NULL;
		strFileName=fdlg.GetPathName();
	}
	if(GetScriptByPath(strFileName))
	{
		DoMsgLine("Файл %s уже загружен.",mmRedErr,strFileName);
		return NULL;
	}
	CScripting* pScript=new CScripting;
	DoStsLine("Загрузка скрипта %s ...",strFileName);
	bool res=pScript->LoadScript(strFileName);
	DoStsLine("");
	if(!res)
		goto err;
	if(!RegisterScript(pScript))
		goto err;
	if(!ConnectScript(pScript))
		goto err;
	if(COCView::m_pView)
		COCView::m_pView->m_tree.InsertScript(pScript);
	return pScript;
err:
	delete pScript;
	return NULL;
}

bool CScriptManager::ConnectScript(CScripting* pScript)
{
	if(pScript->m_pEngine==NULL)
		return false;
	pScript->m_pEngine->AddNamedItem(L"Configurator",SCRIPTITEM_GLOBALMEMBERS|SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE);
	pScript->m_pEngine->AddNamedItem(L"SelfScript",SCRIPTITEM_ISVISIBLE);
	// Запускаем на выполнение
	pScript->m_pEngine->SetScriptState(SCRIPTSTATE_CONNECTED);
	// Проверим, что все нормально
	SCRIPTSTATE ss=SCRIPTSTATE_UNINITIALIZED;
	pScript->m_pEngine->GetScriptState(&ss);
	if(ss!=SCRIPTSTATE_CONNECTED)
		return false;
	return true;
}

CScripting* CScriptManager::GetScriptByPath(CString path)
{
	path.MakeLower();
	CScripting* pScript=NULL;
	if(m_ScriptsByPath.Lookup(path,(void*&)pScript))
		return pScript;
	return NULL;
}

void CScriptManager::UnLoadScript(CScripting *pScript)
{
	UnRegisterScript(pScript);
	if(COCView::m_pView)
		COCView::m_pView->m_tree.RemoveScript(pScript);
	if(pScript->Unload())
		pScript->Release();
	else
		AddDelayed(pScript);
}

CScripting* CScriptManager::GetScriptByNum(long num)
{
	if(num<0 || num>=m_ScriptsArray.GetSize())
		return NULL;
	return (CScripting*)m_ScriptsArray[num];
}

void CScriptManager::AddDelayed(CScripting *pScript)
{
	m_Delayed.Add(pScript);
}
