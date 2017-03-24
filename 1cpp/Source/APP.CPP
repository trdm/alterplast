// App.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#ifdef __REG__
#include "resource_reg.h"
#else
#include "resource.h"
#endif
#include "initguid.h"

#include "AddInTlb.h"
#include "AddIn_i.c"
#include "AddIn.h"

CComModule _Module;
HINSTANCE hDllInstance;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_ExtraC, CAddIn)
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
	return CWinApp::InitInstance();
}

int CAddInApp::ExitInstance()
{
	_Module.Term();  
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  
  /*if (_Module.GetLockCount()==0)
    ::MessageBox(NULL, "DllCanUnloadNow", "1C++ DLL", MB_OK);*/    
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

//extern BOOL bHKLM_IsOK;

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib

/*	Не работает. На Vista без прав админа перерегистрация не проходит,
	но RegisterServer() возвращает 0.
	HRESULT hr = _Module.RegisterServer(FALSE);
	bHKLM_IsOK = (hr == 0);

	if (!bHKLM_IsOK) {
		RedirectHKCR(TRUE);
		hr = _Module.RegisterServer(FALSE);
		RedirectHKCR(FALSE);
	}*/

	HRESULT hr = _Module.RegisterServer(TRUE);
	RedirectHKCR(TRUE);
	hr = _Module.RegisterServer(TRUE);
	RedirectHKCR(FALSE);

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}


