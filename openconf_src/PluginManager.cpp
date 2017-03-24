// PluginManager.cpp: implementation of the CPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "PluginManager.h"
#include "ocview.h"
#include "lefttree.h"

CPluginManager* CPluginManager::m_pPM=NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPluginManager::CPluginManager()
{
	m_pPM=this;
	// Инициализация плагинов
	CString src=CConfigSvcImpl::m_pTheService->m_BinPath;
	/*if(!SetCurrentDirectory(src))
	{
		AfxMessageBox("No set");
		return;
	}
	*/
	src+="config\\*.dll";
	WIN32_FIND_DATA fnd;
	HANDLE hFnd=FindFirstFile(src,&fnd);
	if(hFnd!=(HANDLE)-1)
	{
		src=CConfigSvcImpl::m_pTheService->m_BinPath;
		src+="config\\";
		do{
			if(!lstrcmpi(fnd.cFileName,"config.dll"))
				continue;
			CString path=src+fnd.cFileName;
			LoadPlugin(path);
		}while(FindNextFile(hFnd,&fnd));
		FindClose(hFnd);
	}
}

CPluginManager::~CPluginManager()
{
	Info* pInfo;
	CString key;
	for(POSITION pos=m_Plugins.GetStartPosition();pos;)
	{
		m_Plugins.GetNextAssoc(pos,key,(void*&)pInfo);
		if(pInfo->pPlugin)
			UnloadPlugin(pInfo->pPlugin);
		FreeLibrary((HINSTANCE)pInfo->handle);
		delete pInfo;
	}
}

CPluginManager* CPluginManager::GetPM()
{
	if(!m_pPM)
		Init();
	return m_pPM;
}

void CPluginManager::Init()
{
	if(m_pPM)
		return;
	new CPluginManager;
}

void CPluginManager::Done()
{
	if(m_pPM)
	{
		delete m_pPM;
		m_pPM=NULL;
	}
}



CPluginManager::Info* CPluginManager::RegisterPlugin(LPCTSTR path,LPCTSTR name,HANDLE h)
{
	CString PluginName=name;
	HRESULT hr;
	IDispatchPtr ptr;
	try
	{
		hr=ptr.CreateInstance(name);
		if(ptr==NULL)
		{
			CString errMsg;
			errMsg.Format("При загрузке плагина \"%s\" не удалось создать объект \"%s\"\n",path,name);
			errMsg+=GetErrorDescription(hr);
			AfxMessageBox(errMsg);
			return NULL;
		}
		IConfigPluginPtr ptrConfig=ptr;
		if(ptrConfig!=NULL)
		{
			BSTR retName=NULL;
			hr=ptrConfig->Init(CConfigSvcImpl::m_pTheService->m_Configurator,&retName);
			if(hr==S_OK && retName)
			{
				_bstr_t bstrPluginName=retName;
				PluginName=(LPCTSTR)bstrPluginName;
				SysFreeString(retName);
			}
		}
	}
	catch(_com_error err)
	{
		DoMsgLine("Ошибка COM при загрузке плагина %s: %s",mmRedErr,PluginName,err.ErrorMessage());
		return NULL;
	}
	catch(...)
	{
		DoMsgLine("Ошибка при загрузке плагина %s",mmRedErr,PluginName);
		return NULL;
	}
	if(!CheckName(PluginName))
	{
		UnloadPlugin(ptr);
		return NULL;
	}
	Info* pInfo=new Info(path,PluginName,ptr,h);
	pInfo->progID=name;
	IConfigPluginPtr ptrConfig=ptr;
	if(ptrConfig!=NULL)
	{
		VARIANT varResult;
		varResult.vt=VT_BOOL;
		varResult.boolVal=-1;
		try{
		HRESULT hr=ptrConfig->GetPluginCaps(capShowMacros,&varResult);
		if(hr==S_OK)
		{
			if(!(varResult.vt==VT_EMPTY || varResult.vt==VT_ERROR))
			{
				if(varResult.vt!=VT_BOOL)
					VariantChangeType(&varResult,&varResult,0,VT_BOOL);
				if(varResult.vt==VT_BOOL)
					pInfo->ShowMacros=varResult.boolVal!=0;
			}
		}
		VariantClear(&varResult);
		hr=ptrConfig->GetPluginCaps(capIconNum,&varResult);
		if(hr==S_OK)
		{
			if(!(varResult.vt==VT_EMPTY || varResult.vt==VT_ERROR))
			{
				if(varResult.vt!=VT_I2)
					VariantChangeType(&varResult,&varResult,0,VT_I2);
				if(varResult.vt==VT_I2)
					pInfo->IconNum=varResult.intVal;
			}
		}
		VariantClear(&varResult);
		IOleControlPtr pOleCtrl=ptrConfig;
		if(pOleCtrl!=NULL)
		{
			hr=ptrConfig->GetPluginCaps(capOpenIn,&varResult);
			if(hr==S_OK)
			{
				if(!(varResult.vt==VT_EMPTY || varResult.vt==VT_ERROR))
				{
					if(varResult.vt!=VT_I2)
						VariantChangeType(&varResult,&varResult,0,VT_I2);
					if(varResult.vt==VT_I2)
						pInfo->OpenIn=varResult.iVal;
				}
			}
		}
		}catch(...)
		{
			DoMsgLine("При загрузке плагина %s произошла ошибка",mmRedErr,pInfo->path);	
		}
		VariantClear(&varResult);
	}
	return pInfo;
}

void CPluginManager::UnloadPlugin(IDispatchPtr& ptr)
{
	IConfigPluginPtr pPlugin=ptr;
	if(pPlugin!=NULL)
	{
		try{
			pPlugin->Done();
		}
		catch(...){}
	}
}

void CPluginManager::LoadPlugin(CString path)
{
	if(path.IsEmpty())
	{
		CFileDialog fdlg(TRUE,NULL,NULL,0,"Файлы плагинов (*.dll;*.ocx)|*.dll;*.ocx|Все файлы|*.*||");
		if(fdlg.DoModal()!=IDOK)
			return;
		path=fdlg.GetPathName();
	}
	HINSTANCE hMod=LoadLibrary(path);
	Info* pInfo=NULL;
	if(hMod)
	{
		// Загрузили длл
		char buf[MAX_PATH];
		PFUNCINIT pFunc=(PFUNCINIT)GetProcAddress(hMod,"SetConvigService");
		if(pFunc)
		{
			try{
				(*pFunc)(CConfigSvcImpl::m_pTheService);
				}catch(...){}
		}
		if(LoadString(hMod,100,buf,MAX_PATH))
		{
			PREGFUNC pRegFunc=(PREGFUNC)GetProcAddress(hMod,"DllRegisterServer");
			if(pRegFunc)
				pRegFunc();
			pInfo=RegisterPlugin(path,buf,hMod);
		}
		else
		{
			pInfo=RegisterDLL(path,path.Mid(path.ReverseFind('\\')+1),hMod);
		}
	}
	if(pInfo)
	{
		if(COCView::m_pView)
		{
			COCView::m_pView->m_tree.InsertPlugin(pInfo);
		}
	}
}

CPluginManager::Info* CPluginManager::RegisterDLL(LPCTSTR path,LPCTSTR name,HINSTANCE h)
{
	if(!CheckName(name))
	{
		FreeLibrary(h);
		return NULL;
	}
	Info* pInfo=new Info(path,name,h);
	return pInfo;
}

CPluginManager::Info* CPluginManager::GetPluginByName(CString name)
{
	name.MakeLower();
	Info* pInfo=NULL;
	if(m_Plugins.Lookup(name,(void*&)pInfo))
		return pInfo;
	return NULL;
}

CPluginManager::Info* CPluginManager::GetPluginByNum(long num)
{
	if(num<0 || num>=m_Plugins.GetCount())
		return NULL;
	POSITION pos=m_Plugins.GetStartPosition();
	Info* pInfo;
	CString name;
	for(int i=-1;i<num;i++)
		m_Plugins.GetNextAssoc(pos,name,(void*&)pInfo);
	return pInfo;
}
