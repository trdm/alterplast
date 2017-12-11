// EFExplorer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "EFExplorer.h"
#include "EFFrame.h"
#include "EFTreeView.h"
#include "EFView.h"
#include "EFDoc.h"
#include "EFWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConfigService* pSvc=NULL;
HINSTANCE hMyInst;
static AFX_EXTENSION_MODULE EFExplorerDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("EFExplorer.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(EFExplorerDLL, hInstance))
			return 0;
		new CDynLinkLibrary(EFExplorerDLL);
		hMyInst=hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("EFExplorer.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(EFExplorerDLL);
	}
	return 1;   // ok
}

void OnCreateMainWnd(CWnd* pWnd)
{
}

void OnCreateCfgWnd(CWnd* pWnd)
{
  pSvc->AddConfigTab("Внешние формы",
                     LoadIcon(hMyInst,(LPCTSTR)IDI_ICON),
                     RUNTIME_CLASS(CEFDoc),
                     RUNTIME_CLASS(CEFFrame)
                     //RUNTIME_CLASS(CEFTreeView)
                     //RUNTIME_CLASS(CEFView)
                     //RUNTIME_CLASS(CEFDlg)
                     //RUNTIME_CLASS(CEFWnd)
                    );
}

extern "C" _declspec(dllexport) void SetConvigService(void* p)
{
  pSvc=(CConfigService*)p;
  pSvc->AddConfigEventHandler(CFG_ON_CREATE_MAIN_WND,OnCreateMainWnd);
  pSvc->AddConfigEventHandler(CFG_ON_CREATE_CFG_WND,OnCreateCfgWnd);
}
