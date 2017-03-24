// CoDefFile.h : Declaration of the CCoDefFile

#ifndef __CODEFFILE_H_
#define __CODEFFILE_H_

#include "resource.h"       // main symbols
#include <boost\weak_ptr.hpp>

#include "DefFileInfo.h"
#include "CoClasses.h"

interface IDefFile;
typedef CComPtr<IDefFile> IDefFilePtr;

// class ATL_NO_VTABLE CCoClasses;
// interface IClasses;
// typedef CComPtr<IClasses> IClassesPtr;

/////////////////////////////////////////////////////////////////////////////
// CCoDefFile
class ATL_NO_VTABLE CCoDefFile : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCoDefFile, &CLSID_CoDefFile>,
	public IDispatchImpl<IDefFile, &IID_IDefFile, &LIBID_VIS1CPPLib>
{
public:
	CCoDefFile();
	~CCoDefFile();

 	void Init(const CDefFileInfo::CPtr& pInfo);
 	static IDefFilePtr CreateInstance(const CDefFileInfo::CPtr& pInfo);

//DECLARE_REGISTRY_RESOURCEID(IDR_CODEFFILE)
DECLARE_NO_REGISTRY()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCoDefFile)
	COM_INTERFACE_ENTRY(IDefFile)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDefFile
public:
	STDMETHOD(get_Classes)(/*[out, retval]*/ IClasses* *pVal);
	STDMETHOD(Open)();
	STDMETHOD(get_Path)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	
private:
	CComBSTR m_Name;
	CComBSTR m_Path;
	
	boost::weak_ptr<CDefFileInfo> m_pElem;
	
	CComObject<CCoClasses>* m_pCoClassesCollection;
	IClassesPtr m_pIClassInfoCollection;

	static void Static_AddEachChildClassIntoDefFile(const CClassInfo::CWeakPtr& pClassInfo);

	void InitChildClasses(const CDefFileInfo::CPtr& pInfo);
};

#endif //__CODEFFILE_H_
