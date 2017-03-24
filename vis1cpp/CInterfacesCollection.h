// CoClasses.h : Declaration of the CCoClasses

#ifndef __CINTERFACES_COLLECTION_H_
	#define __CINTERFACES_COLLECTION_H_

#include "resource.h"       // main symbols

#include <boost\weak_ptr.hpp>

/////////////////////////////////////////////////////////////////////////////
// CInterfacesCollection
template <class TMainClass, class TInterfaceClass, class TCoClass>
class CInterfacesCollection
{
public:
	typedef CComPtr<TInterfaceClass> TInterfaceClassPtr;
	typedef boost::shared_ptr<TMainClass> TMainClassPtr;

	bool Add(const TMainClassPtr& pClassInfo);
	void RemoveAll();
	
	HRESULT GetItem(int index, TInterfaceClass **& pVal);
	HRESULT GetItem(LPCSTR szName, TInterfaceClass **&pVal);
	HRESULT GetItem(VARIANT* pVarSrcName, TInterfaceClass **&pVal);

	HRESULT FindItemByPath(LPCSTR szPath, TInterfaceClass**& pElem);
	
	bool IsEmpty() const { return m_IVector.empty(); }
	int Count() const { return m_IVector.size(); }
	
	STDMETHOD(FindItemByPath)(/*[in]*/ VARIANT varPath, /*[out, retval]*/ TInterfaceClass** pElem);
	STDMETHOD(GetNewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal); // умеем работать с FOR EACH в Visual Basic
	//STDMETHOD(GetItem)(VARIANT Name, /*[out, retval]*/ TInterfaceClass* *pVal);

protected:
	// события
	virtual void OnAdd(const TMainClassPtr& pClassInfo) = 0;
	virtual void OnRemoveAll() = 0;

	virtual CString GetItemName(const TMainClassPtr& pClassInfo);

private:
	std::vector<TInterfaceClassPtr> m_IVector;
	CIStringMap<int, int> m_IndexMap;
	CIStringMap<int, int> m_Path2IndexMap;
	
	typedef boost::weak_ptr<TMainClass> TMainClassWeakPtr;
	std::vector<TMainClassWeakPtr> m_Vector;
};

//template <class T, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
template <typename TCoClass, typename TInterfaceClass, const IID* piid>
class CComImplPair
{
public:
	typedef CComPtr<TInterfaceClass> CInterfacePtr;

	CComImplPair() :  pCoClass(NULL) {}
	bool CreateInstance();
	void Release() { pInterface.Release(); pCoClass = NULL; }

	CComObject<TCoClass>* GetCoClass() const { return pCoClass; }
	CInterfacePtr GetInterface() const { return pInterface; }

	operator bool() const { return ! pInterface.operator !(); }
	bool operator !() const { return ! operator bool(); }
private:
	CComObject<TCoClass>* pCoClass;
	CInterfacePtr pInterface;
};

template <typename TCoClass, typename TInterfaceClass, const IID* piid>
bool CComImplPair<TCoClass, TInterfaceClass, piid>::CreateInstance()
{
	Release();

	CComObject<TCoClass>::CreateInstance(&pCoClass);
	assert(pCoClass);
	
	pCoClass->AddRef();
	if (FAILED( pCoClass->QueryInterface(*piid, reinterpret_cast<void**>(&pInterface)) ))
		return false;

	assert(! pInterface.operator !());
	pCoClass->Release();

	return true;
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
HRESULT CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::GetItem(int index, TInterfaceClass **& pVal)
{
	if (index >= Count() || index < 0)
		return S_FALSE;

	//IClassInfoPtr pClassInfo = m_IVector[index];
	TInterfaceClassPtr pClassInfo = m_IVector[index];
	assert(pClassInfo);
	pClassInfo.CopyTo(pVal);
	
	return S_OK;
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
HRESULT CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::GetItem(LPCSTR szName, TInterfaceClass **&pVal)
{
	if (!szName)
		return S_FALSE;

	int index;
	if (!m_IndexMap.Lookup(szName, index))
		return S_FALSE;
	
	return GetItem(index, pVal);
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
HRESULT CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::GetItem(VARIANT* pVarSrcName, TInterfaceClass **&pVal)
{
	USES_CONVERSION;
	
	_variant_t varName(*pVarSrcName);
	if (VT_I2 == varName.vt){
		return GetItem(varName.operator short(), pVal);
	}
	if (VT_I4 == varName.vt){
		return GetItem(varName.operator long(), pVal);
	}
	if (VT_BSTR == varName.vt){
		_bstr_t bstrName = varName.operator _bstr_t();
		LPCSTR szName = W2A(bstrName);
		
		return GetItem(szName, pVal);
	}
	
	if (VT_VARIANT & varName.vt){
		VARIANT* pVar = varName.pvarVal;
		return GetItem(pVar, pVal);
	}
	return S_FALSE;
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
bool CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::Add( const TMainClassPtr& pClassInfo )
{
	m_Vector.push_back(pClassInfo);

	//IClassInfoPtr pInfo = CCoClassInfo::CreateInstance(pClassInfo);
	TInterfaceClassPtr pInfo = TCoClass::CreateInstance(pClassInfo);
	assert(pInfo);

	m_IVector.push_back(pInfo);

	CString strItemName = GetItemName(pClassInfo);
 	m_IndexMap.SetAt(strItemName, Count()-1);
// Msg("CInterfacesCollection::Add() %s", pClassInfo->GetDisplayName());
	m_Path2IndexMap.SetAt(pClassInfo->GetPath(), Count()-1);
	
	OnAdd(pClassInfo);

	return true;
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
CString CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::GetItemName(const TMainClassPtr& pClassInfo)
{
	return pClassInfo->GetDisplayName();
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
void CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::RemoveAll()
{
	if (IsEmpty())
		return;

	m_Vector.resize(0);
	m_IVector.resize(0);
	m_IndexMap.RemoveAll();
	m_Path2IndexMap.RemoveAll();

	OnRemoveAll();
}

// умеем работать с FOR EACH в Visual Basic
template <class TMainClass, class TInterfaceClass, class TCoClass>
STDMETHODIMP CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::GetNewEnum(LPUNKNOWN *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	
	// This block of code allows VB-like iteration.
	//
	const int count = Count();
	std::vector<VARIANT> pVar(count);
	
	for(int i = 0; i < count; i++)
	{
		pVar[i].vt = VT_DISPATCH;
		pVar[i].pdispVal = m_IVector[i];
	}
	
	// Now make the enum.
	typedef CComObject< CComEnum< IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy< VARIANT > > > enumVar;
	enumVar* pEnumVar = new enumVar;
	pEnumVar->Init(pVar.begin(), pVar.end(), NULL, AtlFlagCopy);
	
	// return the enum.
	return pEnumVar->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(pVal));
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
HRESULT CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::FindItemByPath(LPCSTR szPath, TInterfaceClass**& pElem)
{
	int index;
	if (!m_Path2IndexMap.Lookup(szPath, index))
		return S_FALSE;
	
	return GetItem(index, pElem);
}

template <class TMainClass, class TInterfaceClass, class TCoClass>
STDMETHODIMP CInterfacesCollection<TMainClass, TInterfaceClass, TCoClass>::FindItemByPath(VARIANT varSrcPath, TInterfaceClass **pElem)
{
	USES_CONVERSION;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	_variant_t varPath(varSrcPath);
	if (!(VT_BSTR == varPath.vt) && !(VT_VARIANT & varPath.vt) )
		return S_FALSE;

	_bstr_t bstrPath = varPath.operator _bstr_t();
	LPCSTR szPath = W2A(bstrPath);
	
	return FindItemByPath(szPath, pElem);
}

#endif //__CINTERFACES_COLLECTION_H_
