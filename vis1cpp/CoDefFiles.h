// CoDefFiles.h : Declaration of the CCoDefFiles

#ifndef __CODEFFILES_H_
#define __CODEFFILES_H_

#include "resource.h"       // main symbols

#include <boost\weak_ptr.hpp>

#include "DefFileInfo.h"
#include "CoDefFile.h"

#include "CInterfacesCollection.h"

class ATL_NO_VTABLE CCoDefFiles;
interface IDefFiles;
typedef CComPtr<IDefFiles> IDefFilesPtr;

/////////////////////////////////////////////////////////////////////////////
// CCoDefFiles
class ATL_NO_VTABLE CCoDefFiles : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCoDefFiles, &CLSID_CoDefFiles>,
	public IDispatchImpl<IDefFiles, &IID_IDefFiles, &LIBID_VIS1CPPLib>
	, protected CInterfacesCollection<CDefFileInfo, IDefFile, CCoDefFile>
{
public:
	CCoDefFiles();
	~CCoDefFiles();

	static void Init();
	static void Destroy();
	
	static CCoDefFiles* Get() { assert(static_ComPair); return static_ComPair.GetCoClass(); }
	static IDefFilesPtr GetInterfacePtr() { assert(static_ComPair); return static_ComPair.GetInterface(); }
	
	void IfNeeded_FillAllElements();
	void FillAllElements();
	
//DECLARE_REGISTRY_RESOURCEID(IDR_CODEFFILES)
DECLARE_NO_REGISTRY()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCoDefFiles)
	COM_INTERFACE_ENTRY(IDefFiles)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDefFiles
public:
	STDMETHOD(FindByPath)(/*[in]*/ VARIANT varSrcPath, /*[out, retval]*/ IDefFile** pElem);
	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal); // умеем работать с FOR EACH в Visual Basic
	STDMETHOD(get_Item)(VARIANT IndexOrDefPath, /*[out, retval]*/ IDefFile* *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);

private:

	static CComImplPair<CCoDefFiles, IDefFiles, &IID_IDefFiles> static_ComPair;
	
	static void Static_AddEachElement(const CDefFileInfo::CPtr& pClassInfo);

	virtual void OnAdd(const CDefFileInfo::CPtr& pElem);
	virtual void OnRemoveAll();

	virtual CString GetItemName(const CDefFileInfo::CPtr& pClassInfo);
};

#endif //__CODEFFILES_H_
