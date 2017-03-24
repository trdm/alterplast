// Vis1cpp.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Vis1cppps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "Vis1cpp.h"
//#include "vis1cpp_my.h"

#include "Vis1cpp_i.c"
#include "Vis1cppPlugin.h"

#include "config.h"

#include "VIS1CPP_my.h"

#include "cppdoc.h"
// #include "cppview.h"
#include "moduleparser.h"
// #include "mytextdoc.h"
// #include "resource.h"
#include "txtget.h"
// #include "syntax.h"
// 
#include "WrapperForSyntaxCheck.h"
#include "CoClassInfo.h"
#include "CoClasses.h"
#include "CoDefFiles.h"
#include "CoDefFile.h"

CComModule _Module;

// TODO перенести в другой модуль
CConfigService* pSvc;
HINSTANCE hMyInst;
CApp7 * pMainApp;

class CMetaDataCont * pMetaDataCont = NULL;
CBkEndUI *pBkEndUI;

// =====================================================

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_Vis1cppPlugin, CVis1cppPlugin)
	OBJECT_ENTRY_NON_CREATEABLE(CCoClassInfo)
	OBJECT_ENTRY_NON_CREATEABLE(CCoClasses)
	OBJECT_ENTRY_NON_CREATEABLE(CCoDefFiles)
	OBJECT_ENTRY_NON_CREATEABLE(CCoDefFile)
END_OBJECT_MAP()

class CVis1cppApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVis1cppApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CVis1cppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void Init();
	void Done();

	void InitMainAppInstead_AfxGetApp();
};

BEGIN_MESSAGE_MAP(CVis1cppApp, CWinApp)
	//{{AFX_MSG_MAP(CVis1cppApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CVis1cppApp theApp;

BOOL CVis1cppApp::InitInstance()
{
	//CoInitialize(NULL);

    _Module.Init(ObjectMap, m_hInstance, &LIBID_VIS1CPPLib);

	Init();
	
    return CWinApp::InitInstance();
}

int CVis1cppApp::ExitInstance()
{
	//CoUninitialize();
	Done();

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

//CConfigService* pSvc;
//HINSTANCE hMyInst;

void OnCreateCfgWindow(CWnd* pWnd)
{
	if(CCPPView::IsInit())
		return;

	pSvc->AddConfigTab("1C++",LoadIcon(hMyInst,(LPCTSTR)IDI_WRCCPACE),
		RUNTIME_CLASS(CCPPDoc),RUNTIME_CLASS(CCPPView));
}

extern "C" _declspec(dllexport) void SetConvigService(void* p)
{
	pSvc = reinterpret_cast<CConfigService*>( p );
	assert(pSvc);

	pSvc->AddConfigEventHandler(CFG_ON_CREATE_CFG_WND, OnCreateCfgWindow);
	pSvc->RegisterPanelClass("1C++", RUNTIME_CLASS(CCPPView), (HICON)LoadImage(hMyInst, (LPCTSTR)IDI_WRCCPACE, IMAGE_ICON, 16, 16, 0));
	
}

void CVis1cppApp::Init()
{
	hMyInst = m_hInstance;
	pBkEndUI = GetBkEndUI();
	assert(pBkEndUI);
	
	InitMainAppInstead_AfxGetApp();

	CModuleParser::InitMaps();
	
	//_AFXDLL
	//assert( 1 == 0);
	
	// TODO сделать данный список опциональным
	// TODO некоторые ключевые слова должны работать только в файле определения классов
	//
	static const char* keys[]={"class","класс","void","Справочник","Документ","ТаблицаЗначений",
		"СписокЗначений","this","Неопределенный", "Этот", "Сам", "вирт", "я"
	};
	for(int i=0;i<sizeof(keys)/sizeof(keys[0]);i++)
		CBLModule::AddToKeywordColorList(keys[i]);
	CTxtGet::Init();
	
	CWrapperForSyntaxCheck::SetHooks();
	
	/*
	CRuntimeClass* pFrameClass=(CRuntimeClass*)GetProcAddress(
	GetModuleHandle("frame.dll"),
	"?classCPopupFrame@CPopupFrame@@2UCRuntimeClass@@B");
	//Временно меняем ресурсы по умолчанию на свои
	HINSTANCE h1CRes=AfxGetResourceHandle();
	AfxSetResourceHandle(hInstance);
	//Регистрируем шаблон
	pMainApp->AddDocTemplate(new CTemplate7(
	IDR_MAIN,
	RUNTIME_CLASS(CCPPDoc),
	pFrameClass,
	RUNTIME_CLASS(CCPPView)));
	AfxSetResourceHandle(h1CRes);
	
	  //Добавляем описание своей панели управления через ToolBarManager
	  //это избавляет нас от хлопот по выводу и сохранению/восстановлению состояния панели
	  GetToolBarManager()->FUNC3(IDR_MAIN, "Visual 1C++", IDR_MAIN, IDR_MAIN, NULL, 0xE81D, 0, 1, 0x3F800001, 0x96);
	*/
}

void CVis1cppApp::InitMainAppInstead_AfxGetApp()
{
	pMainApp = NULL;
	{
		// получение AfxGetApp() в COM-модуле обрамлять скобками и AFX_MANAGE_STATE(AfxGetAppModuleState())
		AFX_MANAGE_STATE(AfxGetAppModuleState())
		pMainApp = static_cast<CApp7*>( AfxGetApp() );
	}
	assert(pMainApp);
}

void CVis1cppApp::Done()
{
	CWrapperForSyntaxCheck::DestroyHooks();
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
