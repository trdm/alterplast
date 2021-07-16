// Connect.h : Declaration of the CConnect

#ifndef __CONNECT_H_
#define __CONNECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CConnect
class ATL_NO_VTABLE CConnect : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConnect, &CLSID_Connect>,
	public IDispatchImpl<IConnect, &IID_IConnect, &LIBID_SERVERCOMLib>
{
public:
	CConnect()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONNECT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CConnect)
	COM_INTERFACE_ENTRY(IConnect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IConnect
public:
	STDMETHOD(get_Array)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Array)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Test11)(/*[out, retval]*/ VARIANT* *pVal);
	STDMETHOD(get_Test10)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Test10)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Test9)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Test9)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Test8)(/*[out, retval]*/ BSTR* *pVal);
	STDMETHOD(put_Test8)(/*[in]*/ BSTR* newVal);
	STDMETHOD(EvalExpr2)(/*[in]*/BSTR Str,/*[out,retval]*/ VARIANT *Ret);
	STDMETHOD(EvalExpr)(BSTR Str);
	STDMETHOD(Test7)(BSTR A);
	STDMETHOD(Test6)();
	STDMETHOD(put_Test5)(BSTR A, /*[in]*/ BSTR newVal);
	STDMETHOD(put_Test4)(/*[in]*/ BSTR newVal);
	STDMETHOD(Test3)(VARIANT A);
	STDMETHOD(Test2)();
	STDMETHOD(Test)(int a);
};

#endif //__CONNECT_H_
