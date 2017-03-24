// Selection.h : Declaration of the CSelection

#ifndef __SELECTION_H_
#define __SELECTION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSelection
class ATL_NO_VTABLE CSelection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSelection, &CLSID_Selection>,
	public IConnectionPointContainerImpl<CSelection>,
	public IDispatchImpl<ISelection, &IID_ISelection, &LIBID_SELECTDIALOGLib>
{
public:
	CSelection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SELECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSelection)
	COM_INTERFACE_ENTRY(ISelection)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CSelection)
END_CONNECTION_POINT_MAP()


// ISelection
public:
	STDMETHOD(put_InitialText)(/*[in]*/ BSTR* newVal);
	STDMETHOD(Try)();
	STDMETHOD(get_WndDescrsSet)(long ConfDescr, long ModuleDescr, /*[out, retval]*/ long *pVal);
	STDMETHOD(get_DoSelection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ListToChoose)(/*[in]*/ BSTR newVal);

	CStringArray AllStrings;
	CString InitialText;
private:
	long ModuleDescription;
};

#endif //__SELECTION_H_
