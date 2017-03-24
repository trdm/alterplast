// CoClassInfo.h : Declaration of the CCoClassInfo

#ifndef __COCLASSINFO_H_
	#define __COCLASSINFO_H_

#include "resource.h"       // main symbols
#include <boost\weak_ptr.hpp>

#include "ClassInfo.h"

interface IClassInfo;
typedef CComPtr<IClassInfo> IClassInfoPtr;

class ATL_NO_VTABLE CCoClasses;
interface IClasses;
typedef CComPtr<IClasses> IClassesPtr;

/////////////////////////////////////////////////////////////////////////////
// CCoClassInfo
class ATL_NO_VTABLE CCoClassInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCoClassInfo, &CLSID_CoClassInfo>,
	public IDispatchImpl<IClassInfo, &IID_IClassInfo, &LIBID_VIS1CPPLib>
{
public:
	CCoClassInfo();
	~CCoClassInfo();

	void Init(const CClassInfo::CPtr& pInfo);
	static IClassInfoPtr CreateInstance(const CClassInfo::CPtr& pInfo);

//DECLARE_REGISTRY_RESOURCEID(IDR_COCLASSINFO)
DECLARE_NO_REGISTRY()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCoClassInfo)
	COM_INTERFACE_ENTRY(IClassInfo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IClassInfo
public:
	STDMETHOD(get_BaseClasses)(/*[out, retval]*/ IClasses* *pVal);
	STDMETHOD(get_SourceText)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_DefFilePath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(Open)();
	STDMETHOD(get_Path)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);

private:
	CComBSTR m_Name;
	CComBSTR m_Path;
	CComBSTR m_DefFilePath;

	boost::weak_ptr<CClassInfo> m_pElem;

	CComObject<CCoClasses>* m_pBaseCoClassesCollection;
	IClassesPtr m_pBaseIClassInfoCollection;

	static void Static_AddEachChildClass( const CClassInfo::CWeakPtr& pClassInfo );
	void InitChildClasses(const CClassInfo::CPtr& pInfo);
};

#endif //__COCLASSINFO_H_
