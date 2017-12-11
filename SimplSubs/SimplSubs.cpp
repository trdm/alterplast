// SimplSubs.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "plugin.h"
#include "MyDoc.h"
#include "MyView.h"
#include "BaseView.h"
#include "MyADO.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConfigService* pSvc=NULL;
HINSTANCE hMyInst;
static AFX_EXTENSION_MODULE SimplSubsDLL = { NULL, NULL };
CBkEndUI *pBkEndUI;
CMyADO* pAdo = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SIMPLSUBS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SimplSubsDLL, hInstance))
			return 0;
		new CDynLinkLibrary(SimplSubsDLL);
		hMyInst=hInstance;
		pBkEndUI=GetBkEndUI();
		//CMetaDataCont* m_MD = GetMetaData();

		CoInitialize(NULL);
		//AfxMessageBox("Bad open connection");

		

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SIMPLSUBS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SimplSubsDLL);
		CoUninitialize();   

	}
	return 1;   // ok
}

void OnCreateCfgWnd(CWnd* pWnd)
{
	// не надо слов....
	/*
	pSvc->AddConfigTab("",LoadIcon(hMyInst,(LPCTSTR)IDI_ICN_SUBS),
		RUNTIME_CLASS(CMyDoc),RUNTIME_CLASS(CBaseView));		
	*/
	int w=0;
	
}
extern "C" _declspec(dllexport) void SetConvigService(void* p)
{
	 //GetMetaData
	pSvc=(CConfigService*)p;
	pSvc->AddConfigEventHandler(CFG_ON_CREATE_CFG_WND,OnCreateCfgWnd);
	pSvc->RegisterPanelClass("SimpSub",RUNTIME_CLASS(CBaseView),(HICON)LoadImage(hMyInst,(LPCTSTR)IDI_ICN_SUBS,IMAGE_ICON,16,16,0));

}


void Log(const char* format,MessageMarker mark,bool MsgBox,...)
{
	va_list arg;
	va_start(arg,MsgBox);
	CString txt;
	txt.FormatV(format,arg);
	va_end(arg);
	if(MsgBox)
		AfxMessageBox(txt);
	else
		pBkEndUI->DoMessageLine(txt,mark);
}