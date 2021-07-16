// profuisdll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE ProfuisdllDLL = { NULL, NULL };

static HINSTANCE g_hResourceInstOld = NULL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE3(
			"   * * * INITIALIZING DYNAMIC LIBRARY: ProfUIS version %d.%d%d * * * \n",
			((__PROF_UIS_VERSION_DWORD >> 24) & 0XFF),
			((__PROF_UIS_VERSION_DWORD >> 16) & 0XFF),
			((__PROF_UIS_VERSION_DWORD >>  8) & 0XFF)
			);
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ProfuisdllDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(ProfuisdllDLL);
		//g_hResourceInstOld = AfxGetResourceHandle();
		ASSERT( g_hResourceInstOld == NULL );
//		AfxSetResourceHandle(
//			GetModuleHandle( __PROF_UIS_MODULE_NAME ) /*ProfuisdllDLL.hResource*/
//			);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE3(
			"   * * * TERMINATING DYNAMIC LIBRARY: ProfUIS version %d.%d%d * * * \n",
			((__PROF_UIS_VERSION_DWORD >> 24) & 0XFF),
			((__PROF_UIS_VERSION_DWORD >> 16) & 0XFF),
			((__PROF_UIS_VERSION_DWORD >>  8) & 0XFF)
			);
		if( g_hResourceInstOld != NULL )
			AfxSetResourceHandle( g_hResourceInstOld );
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ProfuisdllDLL);
	}
	return 1;   // ok
}
