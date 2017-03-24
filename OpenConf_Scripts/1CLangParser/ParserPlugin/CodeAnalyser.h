// CodeAnalyser.h : Declaration of the CCodeAnalyser

#ifndef __CODEANALYSER_H_
#define __CODEANALYSER_H_

#include <locale.h>
#include <locale.h>
#include "resource.h"       // main symbols
#import "C:\Program Files\1Cv77\BIN\config.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids 
#include "..\Analyser.h"
#include "..\Types1C.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeAnalyser
class ATL_NO_VTABLE CCodeAnalyser : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCodeAnalyser, &CLSID_CodeAnalyser>,
	public ISupportErrorInfo,
	public IDispatchImpl<ICodeAnalyser, &IID_ICodeAnalyser, &LIBID_PARSERPLUGINLib>,
	public IConfigPlugin
{
public:
	CCodeAnalyser();
	~CCodeAnalyser();

DECLARE_REGISTRY_RESOURCEID(IDR_CODEANALYSER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCodeAnalyser)
	COM_INTERFACE_ENTRY(ICodeAnalyser)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConfigPlugin)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IConfigPlugin
public:
	STDMETHOD(Init)(IConfigurator * pConf, BSTR * PluginName);
	STDMETHOD(Done)();

	STDMETHOD(GetPluginCaps)(PluginCaps capNum, VARIANT * pResult);
	STDMETHOD(DoAction)(PluginAction Action, VARIANT Param, VARIANT * pResult);

// ICodeAnalyser
public:
	STDMETHOD(AnalyseGlobalModule)();
	STDMETHOD(GetTypeInPos)();
	STDMETHOD(AnalyseModule)(/*[in]*/ BSTR bstrModuleBody, /*[in]*/ long line, /*[in]*/ long col);

	STDMETHOD(FindVar)(BSTR bstrVarName, /*[out, retval]*/BOOL* pFound);
	STDMETHOD(FindVarProp)(/*[in]*/ BSTR bstrPropName, /*[out, retval]*/ BOOL* pFound);
	STDMETHOD(StartVarPropsIteration)();
	STDMETHOD(NextVarProp)(/*[out, retval]*/ BSTR* pPropName);
	STDMETHOD(get_VarPropName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_VarPropName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_VarPropType)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_VarPropType)(/*[in]*/ BSTR newVal);

	STDMETHOD(FindType)(BSTR TypeName, BOOL* pFound);
	STDMETHOD(StartPropsIteration)();
	STDMETHOD(NextProperty)(/*[out, retval]*/ BSTR* pSuccess);

	STDMETHOD(AddType)(BSTR TypeName);
	STDMETHOD(AddBaseType)(/*[in]*/ BSTR bstrBaseType);
	STDMETHOD(AddTypeProperty)(BSTR PropName, BSTR PropType);
	STDMETHOD(FindTypeProp)(BSTR bstrPropName, /*[out, retval]*/ BOOL* pFound);
	STDMETHOD(RemoveType)(/*[in]*/ BSTR bstrTypeName);
	STDMETHOD(RemoveAllTypes)();

	//Методы для инициализации локального контекста
	STDMETHOD(ResetLocalContext)();
	STDMETHOD(LC_AddFunction)(/*[in]*/ BSTR bstrFuncName, /*[in]*/ BSTR bstrFuncType);
	STDMETHOD(LC_AddVar)(/*[in]*/ BSTR bstrVarName, /*[in]*/ BSTR bstrVarType);
	STDMETHOD(LC_AddVarProperty)(/*[in]*/ BSTR bstrVarName, /*[in]*/ BSTR bstrPropName, /*[in]*/ BSTR bstrPropType);

	STDMETHOD(AddExemplarModifyingMethod)(/*[in]*/ BSTR bstrMethodName, /*[in]*/ long nParam_PropName, /*[in]*/ long nParam_PropType);

	STDMETHOD(get_Type)(/*[out, retval]*/ BSTR *pVal);

	STDMETHOD(get_TypeName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TypeName)(/*[in]*/ BSTR newVal);
	
	STDMETHOD(get_TypePropertyName)(/*[out]*/ BSTR* pVal);
	STDMETHOD(put_TypePropertyName)(/*[in]*/ BSTR newVal);
	
	STDMETHOD(get_TypePropertyType)(/*[out]*/ BSTR* pVal);
	STDMETHOD(put_TypePropertyType)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_TimeSpent)(/*[out, retval]*/ long *pVal);
	
	STDMETHOD(get_Column)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Column)(/*[in]*/ long newVal);
	STDMETHOD(get_Line)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Line)(/*[in]*/ long newVal);
	
	static IConfigurator* pConfigurator;

private:
	CExecutionContext* pLocalContext; //контролы формы, атрибуты и методы текущего контекста
	CExecutionContext GlobalContext; //предопределенные константы и методы
	CModule* pGlobalModule;
	CCode* pGlobalModuleCode;

	CModule* pLastModule;
	CType1C* pLastType;
	CVariable* pLastVar;

	long nLine;
	long nColumn;
	CString strMethod;
	CString strType;
	long TimeSpent;

	CString TypeName;
	CString TypePropertyName, TypePropertyType;
	CString VarPropName, VarPropType;

private:
	CString* GetModuleText();
	ITextDoc* GetTextDoc();
	ITextDoc* GetGlobalModuleDoc();

	CCode* ParseModule(CString& strModule);
	void AnalyseModule(CString& strModule, long line, long col);
	void AnalyseGlobalModule(CString& strModule);
};

#endif //__CODEANALYSER_H_
