// config.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "config.h"
#include "configsvcimpl.h"
#include <afxdllx.h>
#include "formscript.h"
#include "formcnt.h"
#include "initializer.h"
#include "scriptmaneger.h"
#include "pluginmanager.h"
#include "resource.h"
#include "pluginpagedoc.h"
#include "pluginpageview.h"

HINSTANCE g_hInst;
HINSTANCE h1CResource=NULL;

static AFX_EXTENSION_MODULE ConfigDLL = { NULL, NULL };
static HINSTANCE hInst;

#ifdef LOG_MSG
static HANDLE hLog=HANDLE(-1);
void LogMsg(const char*format,...)
{
	if(hLog==HANDLE(-1))
		return;
	va_list arg;
	va_start(arg,format);
	CString msg;
	msg.FormatV(format,arg);
	msg+="\r\n";
	DWORD w;
	WriteFile(hLog,(LPCTSTR)msg,msg.GetLength(),&w,NULL);
	FlushFileBuffers(hLog);
	va_end(arg);
}
#else
void LogMsg(const char*format,...){}
#endif

void DoMsgLine(const char* format,MessageMarker marker,...)
{
	va_list arg;
	va_start(arg,marker);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoMessageLine(txt,marker);
	va_end(arg);
}

void DoStsLine(const char* format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoStatusLine(txt);
	va_end(arg);
}

CString GetErrorDescription(DWORD err/*=0*/)
{
	if(!err)
		err=GetLastError();
	CString errMsg;
	errMsg.Format("Код ошибки: 0x%X",err);
	LPTSTR lpMsgBuf;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,err,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,0,NULL))
	{
		errMsg=errMsg+"\r\n"+lpMsgBuf;
		LocalFree(lpMsgBuf);
	}
	return errMsg;
}

void RunMacros(CString modul,CString macros,bool IsScript)
{
	IDispatch* pDisp=NULL;
	if(IsScript)
	{
		CScripting* pScript=CScriptManager::GetScriptManager()->GetScriptByName(modul);
		if(pScript)
			pScript->m_pEngine->GetScriptDispatch(NULL,&pDisp);
	}
	else
	{
		CPluginManager::Info* pInfo=CPluginManager::GetPM()->GetPluginByName(modul);
		if(pInfo && pInfo->pPlugin!=NULL)
		{
			pDisp=pInfo->pPlugin;
			pDisp->AddRef();
		}
	}
	if(pDisp)
	{
		DISPID dispID;
		BSTR bName=macros.AllocSysString();
		if(pDisp->GetIDsOfNames(IID_NULL,&bName,1,0,&dispID)==S_OK)
		{
			DISPPARAMS none={0,0,0,0};
			bool success=false;
			try{
				success=(pDisp->Invoke(dispID,IID_NULL,0,DISPATCH_METHOD,&none,NULL,NULL,NULL)==S_OK);
			}catch(...){}
			if(!success)
				DoMsgLine("Не удалось выполнить макрос \"%s\".",mmRedErr,macros);
		}
		else
			DoMsgLine("Макрос с именем \"%s\" не найден.",mmRedErr,macros);
		SysFreeString(bName);
		pDisp->Release();
	}
	else
		DoMsgLine("%s с именем \"%s\" не найден.",mmRedErr,(IsScript?"Скрипт":"Плагин"),modul);
}
CApp7* pMainApp=NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		pMainApp=(CApp7*)AfxGetApp();
		pBkEndUI=GetBkEndUI();
		g_hInst=hInstance;
		if (!AfxInitExtensionModule(ConfigDLL, hInstance))
			return 0;
		h1CResource=AfxGetResourceHandle();
		new CDynLinkLibrary(ConfigDLL);
		new CConfigSvcImpl;
		if(!CConfigSvcImpl::m_pTheService->InitConfigSvc())
			return FALSE;
		CString buf=CConfigSvcImpl::m_pTheService->BinDir();
		hInst=LoadLibrary(buf+"config\\config.dll");
		CInitializer::InitDLLMain();
		/*
		if(CTraceInterface::IsOpen())
		{
			CTraceInterface::Close();
		}
		*/


	///////////// LOG //////////////////
#ifdef LOG_MSG
		hLog=CreateFile(buf+"config.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,0,NULL);
		if(hLog==HANDLE(-1))
			AfxMessageBox("Не удалось создать лог-файл");
		else
			SetFilePointer(hLog,0,NULL,FILE_END);
		SYSTEMTIME time;
		GetLocalTime(&time);
		LogMsg("Start %02i.%02i.%02i at %02i:%02i:%02i",time.wDay,time.wMonth,time.wYear%100,
			time.wHour,time.wMinute,time.wSecond);
		LogMsg("Cmd Line:  %s\r\nBinDir: %s\r\nIBDir: %s",GetCommandLine(),
			CConfigSvcImpl::m_pTheService->m_BinPath,
			CConfigSvcImpl::m_pTheService->m_IBPath);
#endif
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxGetModuleState()->m_nObjectCount=0;
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ConfigDLL);
		CInitializer::Done();
		delete CConfigSvcImpl::m_pTheService;
		FreeLibrary(hInst);
#ifdef LOG_MSG
		CloseHandle(hLog);
#endif
	}
	return 1;   // ok
}
