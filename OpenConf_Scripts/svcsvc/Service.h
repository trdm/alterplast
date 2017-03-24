// Service.h : Declaration of the CService

#ifndef __SERVICE_H_
#define __SERVICE_H_

#include "resource.h"       // main symbols
/////////////////////////////////////////////////////////////////////////////
// CService
class ATL_NO_VTABLE CService : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CService, &CLSID_Service>,
	public IDispatchImpl<IService, &IID_IService, &LIBID_SVCSVCLib>
{
public:
	CService()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SERVICE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CService)
	COM_INTERFACE_ENTRY(IService)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IService
public:
	STDMETHOD(get_Version)(/*[out, retval]*/ long *pVal);
	STDMETHOD(SetWindowText)(long WH,BSTR Text);
	STDMETHOD(GetWindowText)(long h,VARIANT_BOOL IsList, BSTR* Result);
	STDMETHOD(FilterValue)(BSTR Values, short Flags, BSTR Caption,long X,long Y, long W, long H, BSTR* Result);
	STDMETHOD(PopupMenu)(BSTR Values, short ShowIn, long X, long Y, BSTR* Select);
	STDMETHOD(SelectInTree)(BSTR Values, BSTR Caption, VARIANT_BOOL Mark, VARIANT_BOOL OnlyItems, BSTR* Select);
	STDMETHOD(SelectValue)(BSTR Values, BSTR Caption, VARIANT_BOOL Mark, BSTR* Select);
	STDMETHOD(FileO2A)(BSTR Path);
	STDMETHOD(FileA2O)(BSTR Path);
	STDMETHOD(SelectFile)(VARIANT_BOOL ForSave, BSTR FileName, BSTR Filter, VARIANT_BOOL MultiSelect, BSTR* FilePath);
	STDMETHOD(SelectFolder)(BSTR Caption, BSTR StartFolder, long Flags,BSTR Root, BSTR* FolderPath);
	STDMETHOD(OEMToAnsi)(BSTR OEM, BSTR* Ansi);
	STDMETHOD(AnsiToOEM)(BSTR Ansi, BSTR* OEM);
	STDMETHOD(SetImageList)(BSTR Values, BSTR *Result);
};

#endif //__SERVICE_H_
