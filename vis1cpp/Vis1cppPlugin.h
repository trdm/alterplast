// Vis1cppPlugin.h : Declaration of the CVis1cppPlugin

#ifndef __VIS1CPPPLUGIN_H_
#define __VIS1CPPPLUGIN_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "Vis1cppCP.h"
#include "CoClasses.h"

//#import "D:\1C\BIN\config.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids 
#import "config.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids 

/////////////////////////////////////////////////////////////////////////////
// CVis1cppPlugin

class ATL_NO_VTABLE CVis1cppPlugin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IVis1cppPlugin, &IID_IVis1cppPlugin, &LIBID_VIS1CPPLib>,
	public CComCoClass<CVis1cppPlugin, &CLSID_Vis1cppPlugin>,
	public ISupportErrorInfo,
	public IConfigPlugin,
	public CProxy_IVis1cppPluginEvents< CVis1cppPlugin >,
	public IConnectionPointContainerImpl<CVis1cppPlugin>,
		//public IProvideClassInfo // 1 - так чуть более сложнее - без этого не ловятся события плагина в скриптах Опенконфа - artbear
		public IProvideClassInfo2Impl<&CLSID_Vis1cppPlugin, &DIID__IVis1cppPluginEvents> // 2 - без этого не ловятся события плагина в скриптах Опенконфа - artbear
{
private:
	static CVis1cppPlugin* pPlugin;
	//IClassesPtr m_pClasses;

public:
	CVis1cppPlugin();
	~CVis1cppPlugin();
	static CVis1cppPlugin* Get() { assert(pPlugin); return pPlugin; };

DECLARE_REGISTRY_RESOURCEID(IDR_VIS1CPPPLUGIN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVis1cppPlugin)
	COM_INTERFACE_ENTRY(IVis1cppPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConfigPlugin)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IProvideClassInfo)  // 1 и 2 - без этого не ловятся события плагина в скриптах Опенконфа - artbear
		COM_INTERFACE_ENTRY(IProvideClassInfo2)  // 2 - без этого не ловятся события плагина в скриптах Опенконфа - artbear
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CVis1cppPlugin)
CONNECTION_POINT_ENTRY(DIID__IVis1cppPluginEvents)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
 	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// //	IProvideClassInfo
// 	STDMETHOD(GetClassInfo)(ITypeInfo** ppTI)  // 1 - без этого не ловятся события плагина в скриптах Опенконфа - artbear
// 	{
// 		if(!ppTI)
// 			return E_POINTER;
// 		HRESULT hr=E_FAIL;
// 		ITypeLib* pLib=NULL;
// 		LoadRegTypeLib(LIBID_VIS1CPPLib,1,0,NULL,&pLib);
// 		if(pLib)
// 		{
// 			hr=pLib->GetTypeInfoOfGuid(CLSID_Vis1cppPlugin,ppTI);
// 			pLib->Release();
// 		}
// 		return hr;		
// 	}

// IVis1cppPlugin
public:
	STDMETHOD(GetDefFiles)(/*[out, retval]*/ IDefFiles** DefFiles);
	STDMETHOD(GetClasses)(IClasses** Classes);
	STDMETHOD(RefreshAll)();
	STDMETHOD(get_FlagGroupByType)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_FlagGroupByType)(/*[in]*/ BOOL newVal);
	STDMETHOD(Method1)();

// IConfigPlugin
	STDMETHOD(Init)(IConfigurator * pConfigurator, BSTR * PluginName);
	STDMETHOD(Done)();
	STDMETHOD(GetPluginCaps)(PluginCaps capNum, VARIANT * pResult)
	{
		if (pResult == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
	STDMETHOD(DoAction)(PluginAction Action, VARIANT Param, VARIANT * pResult)
	{
		if (pResult == NULL)
			return E_POINTER;
			
		return E_NOTIMPL;
	}
};

#endif //__VIS1CPPPLUGIN_H_
