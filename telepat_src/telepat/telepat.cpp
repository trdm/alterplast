// telepat.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f telepatps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "telepat.h"

//#include "telepat_i.c"
#include "Plugin.h"
#include "txttrap.h"
#include "mycmdtarget.h"
#include "telelist.h"
#include "paramwnd.h"
#include "keywords.h"
#include "methcombobox.h"


CComModule _Module;
CBkEndUI* pBkEndUI=NULL;
CMetaDataCont *pMetaDataCont;
CConfigService *pConfSvc=NULL;
HINSTANCE hMyInst=NULL;
CMDIFrameWnd* pMainFrame=NULL;
CApp7* pMainApp=NULL;
HINSTANCE h1CResource=NULL;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Plugin, CPlugin)
END_OBJECT_MAP()

class CTelepatApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelepatApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTelepatApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTelepatApp, CWinApp)
	//{{AFX_MSG_MAP(CTelepatApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTelepatApp theApp;

BOOL CTelepatApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_TELEPATLib);
	hMyInst=m_hInstance;
    return CWinApp::InitInstance();
}

int CTelepatApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
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
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

static AFX_EXTENSION_MODULE TelepatDLL={NULL,NULL};

void OnCreateMainWnd(CWnd* pWnd)
{
	pMainFrame=(CMDIFrameWnd*)pWnd;
	new CTeleList;
	CParamWnd::GetParamWnd()->Init();
}

extern "C" _declspec(dllexport) void SetConvigService(void* p)
{
	pConfSvc=(CConfigService*)p;
	pBkEndUI=GetBkEndUI();
	pMetaDataCont=GetMetaData();
	pMainApp=(CApp7*)AfxGetApp();
	h1CResource=AfxGetResourceHandle();
	CTxtTrap::Init();
	AfxInitExtensionModule(TelepatDLL, hMyInst);
	pConfSvc->AddCmdTarget(new CMyCmdTarget);
	pConfSvc->AddDynLink(TelepatDLL);
	pConfSvc->LoadToolBar(hMyInst,IDR_TOOLBAR1,"Телепат",8);
	pConfSvc->AddConfigEventHandler(CFG_ON_CREATE_MAIN_WND,OnCreateMainWnd);
	CMyToolBarMgr::Init();
}
void DoMsgLine(const char* format,MessageMarker marker,...)
{
	va_list arg;
	va_start(arg,marker);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoMessageLine(txt,marker);
	va_end(arg);
}

void* GetZipResource(UINT id)
{
	void* pRet=NULL;
	HRSRC hRsrc=FindResource(hMyInst, (LPCTSTR) id, (LPCTSTR)333);
	if(hRsrc)
	{
		HGLOBAL hGlobal=LoadResource(hMyInst,hRsrc);
		if(hGlobal)
		{
			void* pData=LockResource(hGlobal);
			if(pData)
			{
				IzlibEnginePtr pZlib;
				if(S_OK==pZlib.CreateInstance(L"V75.zlibEngine"))
				{
					IStream *pIn=NULL,*pOut=NULL;
					CreateStreamOnHGlobal(NULL,TRUE,&pIn);
					CreateStreamOnHGlobal(NULL,TRUE,&pOut);
					if(pIn && pOut)
					{
						UINT size=SizeofResource(hMyInst,hRsrc);
						ULONG writed;
						pIn->Write(pData,size,&writed);
						LARGE_INTEGER lNull;
						lNull.QuadPart=0;
						pIn->Seek(lNull,0,NULL);
						if(pZlib->pkDecompress(pIn,pOut)==S_OK)
						{
							HGLOBAL hOut=NULL;
							LPVOID  lpMemStream = NULL;
							STATSTG St;
							pOut->Stat(&St,STATFLAG_NONAME);
							ULONG cbStreamSizeOut=St.cbSize.LowPart;
							GetHGlobalFromStream(pOut, &hOut);
							if(hOut)
							{
								lpMemStream=GlobalLock(hOut);
								pRet=new char[cbStreamSizeOut];
								memcpy(pRet, lpMemStream, cbStreamSizeOut);            
								GlobalUnlock(hOut);
								GlobalFree(hOut);
							}
						}
					}
					if(pOut)
						pOut->Release();
					if(pIn)
						pIn->Release();
				}
				UnlockResource(hGlobal);
			}
			FreeResource(hGlobal);
		}
	}
	return pRet;
}