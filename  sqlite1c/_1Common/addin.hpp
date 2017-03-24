// addin.hpp
// (c) Александр Орефков
// Заголовки интерфейсов 1С и реализция IInitDone.
// Загрузка ВК без COM и реестра.

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

struct __declspec(uuid("ab634001-f13d-11d0-a459-004095e1daea"))
IInitDone : IUnknown
{
	STDMETHOD(Init)(IDispatch *pConnection) = 0;
	STDMETHOD(Done)() = 0;
	STDMETHOD(GetInfo)(SAFEARRAY **pInfo) = 0;
};

struct __declspec(uuid("ab634002-f13d-11d0-a459-004095e1daea"))
IPropertyProfile : IPropertyBag
{
	STDMETHOD(RegisterProfileAs)(BSTR bstrProfileName) = 0;
};

struct __declspec(uuid("ab634004-f13d-11d0-a459-004095e1daea"))
IAsyncEvent : IUnknown
{
	STDMETHOD(SetEventBufferDepth)(long lDepth) = 0;
	STDMETHOD(GetEventBufferDepth)(long *plDepth) = 0;
	STDMETHOD(ExternalEvent)(BSTR bstrSource, BSTR bstrMessage, BSTR bstrData) = 0;
	STDMETHOD(CleanBuffer)() = 0;
};

struct __declspec(uuid("ab634003-f13d-11d0-a459-004095e1daea"))
ILanguageExtender : IUnknown
{
	STDMETHOD(RegisterExtensionAs)(BSTR *bstrExtensionName) = 0;
	STDMETHOD(GetNProps)(long *plProps) = 0;
	STDMETHOD(FindProp)(BSTR bstrPropName, long *plPropNum) = 0;
	STDMETHOD(GetPropName)(long lPropNum, long lPropAlias, BSTR *pbstrPropName) = 0;
	STDMETHOD(GetPropVal)(long lPropNum, VARIANT *pvarPropVal) = 0;
	STDMETHOD(SetPropVal)(long lPropNum, VARIANT *varPropVal) = 0;
	STDMETHOD(IsPropReadable)(long lPropNum, long *pboolPropRead) = 0;
	STDMETHOD(IsPropWritable)(long lPropNum, long *pboolPropWrite) = 0;
	STDMETHOD(GetNMethods)(long *plMethods ) = 0;
	STDMETHOD(FindMethod)(BSTR bstrMethodName, long *plMethodNum) = 0;
	STDMETHOD(GetMethodName)(long lMethodNum, long lMethodAlias, BSTR *pbstrMethodName) = 0;
	STDMETHOD(GetNParams)(long lMethodNum, long *plParams) = 0;
	STDMETHOD(GetParamDefValue)(long lMethodNum, long lParamNum, VARIANT *pvarParamDefValue) = 0;
	STDMETHOD(HasRetVal)(long lMethodNum, long *pboolRetValue) = 0;
	STDMETHOD(CallAsProc)(long lMethodNum, SAFEARRAY **paParams) = 0;
	STDMETHOD(CallAsFunc)(long lMethodNum, VARIANT *pvarRetValue, SAFEARRAY **paParams) = 0;
};

struct __declspec(uuid("ab634005-f13d-11d0-a459-004095e1daea"))
IStatusLine : IUnknown
{
	STDMETHOD(SetStatusLine)(BSTR bstrStatusLine) = 0;
	STDMETHOD(ResetStatusLine)() = 0;
};

struct __declspec(uuid("efe19ea0-09e4-11d2-a601-008048da00de"))
IExtWndsSupport : IUnknown
{
	STDMETHOD(GetAppMainFrame)(wireHWND *hwnd) = 0;
	STDMETHOD(GetAppMDIFrame)(wireHWND *hwnd) = 0;
	STDMETHOD(CreateAddInWindow)(
		BSTR bstrProgID,
		BSTR bstrWindowName,
		long dwStyles,
		long dwExStyles,
		struct tagRECT *rctl,
		long Flags,
		wireHWND *pHwnd,
		IDispatch **pDisp) = 0;
};

_COM_SMARTPTR_TYPEDEF(IInitDone, __uuidof(IInitDone));
_COM_SMARTPTR_TYPEDEF(IPropertyProfile, __uuidof(IPropertyProfile));
_COM_SMARTPTR_TYPEDEF(IAsyncEvent, __uuidof(IAsyncEvent));
_COM_SMARTPTR_TYPEDEF(ILanguageExtender, __uuidof(ILanguageExtender));
_COM_SMARTPTR_TYPEDEF(IStatusLine, __uuidof(IStatusLine));
_COM_SMARTPTR_TYPEDEF(IExtWndsSupport, __uuidof(IExtWndsSupport));


extern "C" const GUID __declspec(selectany) IID_IInitDone = 
    {0xab634001,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}};
extern "C" const GUID __declspec(selectany) IID_IPropertyProfile =
    {0xab634002,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}};
extern "C" const GUID __declspec(selectany) IID_IAsyncEvent =
    {0xab634004,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}};
extern "C" const GUID __declspec(selectany) IID_ILanguageExtender =
    {0xab634003,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}};
extern "C" const GUID __declspec(selectany) IID_IStatusLine =
    {0xab634005,0xf13d,0x11d0,{0xa4,0x59,0x00,0x40,0x95,0xe1,0xda,0xea}};
extern "C" const GUID __declspec(selectany) IID_IExtWndsSupport =
    {0xefe19ea0,0x09e4,0x11d2,{0xa6,0x01,0x00,0x80,0x48,0xda,0x00,0xde}};
extern "C" const GUID __declspec(selectany) IID_IPropertyLink =
    {0x52512a61,0x2a9d,0x11d1,{0xa4,0xd6,0x00,0x40,0x95,0xe1,0xda,0xea}};

#pragma pack(pop)

namespace addin{
// Детали реализации перехвата
namespace impl_trap{

// Функция возвращает адрес переходника в таблице импорта указанной длл для
// функции strFunc, импортированной из strExporter длл.
// ВНИМАНИЕ: никаких проверок не делается. Вызывайте метод, если точно знаете,
// что hDll импортирует strFunc из strExporter
static inline DWORD* GetThunkAddres(HINSTANCE hDll, LPCSTR strExporter, LPCSTR strFunc)
{
	BYTE *pimage = reinterpret_cast<BYTE*>(hDll);
	PIMAGE_OPTIONAL_HEADER ioh=reinterpret_cast<PIMAGE_OPTIONAL_HEADER>(pimage +
		reinterpret_cast<PIMAGE_DOS_HEADER>(hDll)->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));
	
	PIMAGE_IMPORT_DESCRIPTOR iid=reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(pimage +
		(ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
	
	// В таблице импорта ищем соответствующий элемент для библиотеки
	while(iid->Name)  //до тех пор пока поле структуры не содержит 0
	{
		if(_strcmpi(reinterpret_cast<char*>(pimage + iid->Name), strExporter) ==0 )
			break;
		iid++;
	}

	PIMAGE_THUNK_DATA isd = reinterpret_cast<PIMAGE_THUNK_DATA>(pimage + iid->OriginalFirstThunk);
	DWORD* pReal=reinterpret_cast<DWORD*>(pimage + iid->FirstThunk);
	for(int i=0;;isd++, i++)
	{
		LPCSTR name=(LPCSTR)(pimage + (DWORD)((PIMAGE_IMPORT_BY_NAME)isd->u1.AddressOfData)->Name);
		if(!_stricmp(name, strFunc))
			return pReal+i;
	}
}

static inline DWORD* GetThunkAddres(LPCSTR strImporter, LPCSTR strExporter, LPCSTR strFunc)
{
	return GetThunkAddres(GetModuleHandle(strImporter), strExporter, strFunc);
}

struct trap_load_vk
{
	static trap_load_vk*& GetTrap()
	{
		static trap_load_vk* pTrap;
		return pTrap;
	}

	const IInitDone* pAddin;
	const CLSID& pClsID;
	DWORD  dwOldCLSIDFromProgID;
	DWORD* pOldCLSIDFromProgID;
	DWORD  dwOldCoCreateInstance;
	DWORD* pOldCoCreateInstance;

	static HRESULT STDAPICALLTYPE myCLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID pclsid)
	{
		*pclsid=GetTrap()->pClsID;
		return S_OK;
	}

	static HRESULT STDAPICALLTYPE myCoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
						  DWORD dwClsContext, REFIID riid, LPVOID *ppv)
	{
		trap_load_vk* pTrap=GetTrap();
		*ppv=const_cast<IInitDone*>(pTrap->pAddin);
		delete pTrap;
		return S_OK;
	}

	trap_load_vk(const IInitDone* pA, const CLSID& pCls):
		pAddin(pA), pClsID(pCls), pOldCLSIDFromProgID(NULL), pOldCoCreateInstance(NULL)
	{
		GetTrap()=this;
		HINSTANCE hSeven=GetModuleHandle("seven.dll");

		pOldCLSIDFromProgID =GetThunkAddres(hSeven, "ole32.dll", "CLSIDFromProgID");
		dwOldCLSIDFromProgID=*pOldCLSIDFromProgID;
		
		pOldCoCreateInstance=GetThunkAddres(hSeven, "ole32.dll", "CoCreateInstance");
		dwOldCoCreateInstance=*pOldCoCreateInstance;

		// Заменяем адреса на свои функции
		DWORD op;
		VirtualProtect(pOldCLSIDFromProgID, 4, PAGE_EXECUTE_READWRITE, &op);
		VirtualProtect(pOldCoCreateInstance, 4, PAGE_EXECUTE_READWRITE, &op);

		*pOldCLSIDFromProgID=reinterpret_cast<DWORD>(&myCLSIDFromProgID);
		*pOldCoCreateInstance=reinterpret_cast<DWORD>(&myCoCreateInstance);
	}
	~trap_load_vk()
	{
		*pOldCoCreateInstance=dwOldCoCreateInstance;
		*pOldCLSIDFromProgID=dwOldCLSIDFromProgID;
	}
};

} // namespace impl_trap

/*
Реализация основного объекта ВК.
Данная реализация предполагает наличие ОДНОГО-ЕДИНСТВЕННОГО глобального
объекта ВК. Обеспечивает корректную загрузку ВК 1Ской, без использования
подсистемы COM и обращений к реестру. Доступ к единственному объекту ВК
осуществляется с помощью статической функции GetAddin();

Использование:

Вам необходимо определить свой класс, реализующий два метода:
BOOL InitAddin(IDispatch* pConnection);
void DoneAddin();
Для класса должен быть назначен уникальный CLSID, с помощью __declspec(uuid(""))

Класс COneAddinImpl<ВашКласс> и будет представлять объект ВК
Для загрузки ВК необходимо в DllMain создать первый и единственный объект,
методом OnProcessAttach, и там же уничтожать OnProcessDetach:

Пример:
class _declspec(uuid("00cd947e-1f68-4ba6-a69f-990a8affd815")) CAddin_;

class CAddin_
{
protected:
	BOOL InitAddin(IDispatch* pConn);
	void DoneAddin();
public:
	... остальные методы
};

typedef addin::COneAddinImpl<CAddin_> CAddin;

extern "C"
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
		CAddin::OnProcessAttach();
	else if(dwReason == DLL_PROCESS_DETACH)
		CAddin::OnProcessDetach();
	return 1;   // ok
}

  Либо так:

class CAddin
{
protected:
	BOOL InitAddin(IDispatch* pConn);
	void DoneAddin();
public:
	CAddin& GetAddin(){return addin::COneAddinImpl<CAddin>::GetAddin();}
	... остальные методы
};
class _declspec(uuid("00cd947e-1f68-4ba6-a69f-990a8affd815")) CAddin;
....
extern "C"
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
		addin::COneAddinImpl<CAddin>::OnProcessAttach();
	else if(dwReason == DLL_PROCESS_DETACH)
		addin::COneAddinImpl<CAddin>::OnProcessDetach();
	return 1;   // ok
}

Данная реализация не позволяет иметь страницу свойств для ВК,
что есть небольшая плата за загрузку ВК без регистрации и админских прав.

*/

template<typename T>
class COneAddinImpl : public T, public IInitDone
{
	// Закроем конструктор, это синглтон
	COneAddinImpl(){new impl_trap::trap_load_vk(this, __uuidof(T));}
protected:
	// Реализация IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject)
	{
		if(riid==IID_IInitDone)
			*ppvObject=static_cast<IInitDone*>(this);
		else if(riid==IID_IUnknown)
			*ppvObject=static_cast<IUnknown*>(this);
		else
			return E_NOINTERFACE;
		return S_OK;
	}
	STDMETHOD_(ULONG, AddRef)(){return 1;}
	STDMETHOD_(ULONG, Release)(){return 1;}

	// Реализация IInitDone
    STDMETHOD(Init)(IDispatch *pConnection)
	{
		return T::InitAddin(pConnection) ? S_OK : E_FAIL;
	}
    STDMETHOD(Done)()
	{
		T::DoneAddin();
		return S_OK;
	}
    STDMETHOD(GetInfo)(SAFEARRAY **pInfo)
	{
		long lInd = 0;
		VARIANT varVersion;
		V_VT(&varVersion) = VT_I4;
		V_I4(&varVersion) = 2000;
		SafeArrayPutElement(*pInfo,&lInd,&varVersion);
		return S_OK;
	}
	static COneAddinImpl<T>*& getptr(){static COneAddinImpl<T>* ptr;return ptr;}
public:
	static void OnProcessAttach(){getptr()=new COneAddinImpl<T>;}
	static void OnProcessDetach(){delete getptr();}
	// Получение ссылки на единственный экземпляр аддина
	static COneAddinImpl<T>& GetAddin(){return *getptr();}
};

}// namespace addin
