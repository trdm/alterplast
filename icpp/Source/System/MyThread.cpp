// MyThread.cpp: implementation of the CMyThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <Windows.h>
#include "MyThread.h"


//_____________________________________________________________________________
//
// CMyThread Maps
//_____________________________________________________________________________
//
/*BEGIN_BL_METH_MAP(CMyThread) 
    BL_METH("CreateThread",	"СоздатьПоток",		1,	procCreateThread,	NULL,		NULL)
    BL_METH("DeleteThread",	"УдалитьПоток",		0,	procDeleteThread,	NULL,		NULL)
    BL_METH("Sleep",		"Уснуть",			1,	procSleep,			NULL,		NULL)
    BL_METH("Sleep",		"Спать",			1,	procSleep,			NULL,		NULL)
END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CMyThread)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CMyThread, "Thread", "Поток", 1, NULL, NULL, -1);
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CBLModule * pThreadBLModule;

CMyThread::CMyThread()
{
	hThread=NULL;

	pThreadBLModule = NULL;
	CBLModule *pRunCBLModule = NULL;
}

CMyThread::CMyThread(int param)
{
	hThread=NULL;

	pThreadBLModule = NULL;
	CBLModule *pRunCBLModule = NULL;
}

CMyThread::~CMyThread()
{
	if(hThread)
		TerminateThread(hThread,0);
	hThread=NULL;

	if (pThreadBLModule)
	{
		delete pThreadBLModule;
	}
	pThreadBLModule = NULL;
	CBLModule *pRunCBLModule = NULL;
}

//Поддержка многопоточности
DWORD WINAPI TreadProc(LPVOID lpParam)
{
	try // чтобы в случае ошибок при исполнении 1С не падала
	{
		class CBLModule * pThreadBLModule = (class CBLModule *)lpParam;
		if (pThreadBLModule)
		{
			pThreadBLModule->Execute();
			//pThreadBLModule->~CBLModule();
			//delete pThreadBLModule;
			//pThreadBLModule = NULL;
		}
	}
	catch(...) {}
	return 0;
}

BOOL CMyThread::procCreateThread(CValue **params)
{
// 	CString str = params[0]->GetString();
// 
// 	DWORD trdID;
// 	if(hThread)
// 		TerminateThread(hThread,0);
// 	hThread=NULL;
// 
// 	if (pThreadBLModule)
// 	{
// 		delete pThreadBLModule;
// 	}
// 	pThreadBLModule = new CBLModuleWrapper("", NULL, NULL, NULL, true);
// 	pThreadBLModule->AssignSource(params[0]->GetString());
// 
// 	CBLModule *pRunCBLModule=CBLModule::GetExecutedModule();//контекст текущей обработки
// 	pThreadBLModule->AssignFriendModule(pRunCBLModule);
// 
// 	try{
// 		int nRes=pThreadBLModule->Compile();
// 
// 		if (0 == nRes)
// 			throw CString("Модуль не скомпилирован");
// 	}
// 	catch(CString &err)
// 	{
// 		if (pThreadBLModule)
// 		{
// 			delete pThreadBLModule;
// 			pThreadBLModule = NULL;
// 		}
// 		RuntimeError("%s", "Не удалось создать поток из-за ошибки синтаксиса \r\n" + err);
// 		return TRUE;
// 	}
// 	catch(...)
// 	{
// 		if (pThreadBLModule)
// 		{
// 			delete pThreadBLModule;
// 			pThreadBLModule = NULL;
// 		}
// 		RuntimeError("%s", "Не удалось создать поток из-за ошибки синтаксиса");
// 		return TRUE;
// 	};
// 
// 	//hThread=CreateThread(0,0,TreadProc,0,0,&trdID);
// 	hThread=CreateThread(0,0,TreadProc, pThreadBLModule,0,&trdID);

	return TRUE;
}

BOOL CMyThread::procDeleteThread(CValue **params)
{
	if(hThread)
		TerminateThread(hThread,0);
	hThread=NULL;

	if (pThreadBLModule)
	{
		delete pThreadBLModule;
	}

	pThreadBLModule = NULL;
	CBLModule *pRunCBLModule = NULL;

	return TRUE;
}

BOOL CMyThread::procSleep(CValue **params)
{
	Sleep(params[0]->GetNumeric());

	return TRUE;
}
