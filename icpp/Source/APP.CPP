// App.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "AddIn.h"

CComModule _Module;
HINSTANCE hDllInstance;

BEGIN_OBJECT_MAP(ObjectMap)
// OBJECT_ENTRY(CLSID_ExtraC, CAddIn)
END_OBJECT_MAP()

class CAddInApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CAddInApp theApp;

BOOL CAddInApp::InitInstance()
{
	hDllInstance = m_hInstance;
	_Module.Init(ObjectMap, m_hInstance);  
	BOOL ret = CWinApp::InitInstance();
	return ret;
}

int CAddInApp::ExitInstance()
{
	_Module.Term();  
	BOOL ret = CWinApp::ExitInstance();
	pMainAddIn->Done();
	pMainAddIn.reset();
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	if (!pMainAddIn)
	{
		pMainAddIn.reset(new CAddIn);
		pMainAddIn->Init();
	}
 	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	return _Module.UnregisterServer();
}
