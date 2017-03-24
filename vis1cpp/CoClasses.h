// CoClasses.h : Declaration of the CCoClasses

#ifndef __COCLASSES_H_
#define __COCLASSES_H_

#include "resource.h"       // main symbols

#include <boost\weak_ptr.hpp>

#include "ClassInfo.h"
#include "CoClassInfo.h"

#include "CInterfacesCollection.h"

class ATL_NO_VTABLE CCoClasses;

interface IClasses;
typedef CComPtr<IClasses> IClassesPtr;

/////////////////////////////////////////////////////////////////////////////
// CCoClasses
class ATL_NO_VTABLE CCoClasses : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCoClasses, &CLSID_CoClasses>,
	public IDispatchImpl<IClasses, &IID_IClasses, &LIBID_VIS1CPPLib>
	//, protected CInterfacesCollection<CClassInfo, IClassInfo, CCoClassInfo>
	, public CInterfacesCollection<CClassInfo, IClassInfo, CCoClassInfo>
{
public:
	CCoClasses();
	~CCoClasses();

	static void Init();
	static void Destroy();

// 	static CCoClasses* Get() { assert(static_pCoClass); return static_pCoClass; }
// 	static IClassesPtr GetInterfacePtr() { assert(static_pCoClass); assert(static_pInterface); return static_pInterface; }
	static CCoClasses* Get() { assert(static_ComPair); return static_ComPair.GetCoClass(); }
	static IClassesPtr GetInterfacePtr() { assert(static_ComPair); return static_ComPair.GetInterface(); }

	void IfNeeded_FillAllElements();
	void FillAllElements();

//DECLARE_REGISTRY_RESOURCEID(IDR_COCLASSES)
DECLARE_NO_REGISTRY()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCoClasses)
	COM_INTERFACE_ENTRY(IClasses)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IClasses
public:
	STDMETHOD(FindByPath)(/*[in]*/ VARIANT varPath, /*[out, retval]*/ IClassInfo** pElem);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal); // умеем работать с FOR EACH в Visual Basic
	STDMETHOD(get_Item)(VARIANT Name, /*[out, retval]*/ IClassInfo* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);

private:
// 	static CComObject<CCoClasses>* static_pCoClass;
// 	static IClassesPtr static_pInterface;
	static CComImplPair<CCoClasses, IClasses, &IID_IClasses> static_ComPair;
	
	static void Static_AddEachElement(const CClassInfo::CPtr& pClassInfo);

	virtual void OnAdd(const CClassInfo::CPtr& pElem);
	virtual void OnRemoveAll();
};

#endif //__COCLASSES_H_
