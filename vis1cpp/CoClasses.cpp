// CoClasses.cpp : Implementation of CCoClasses
#include "stdafx.h"

#include "Vis1cpp.h"
#include "Vis1cpp_my.h"

#include <functional>

#include "CoClasses.h"

// CComObject<CCoClasses>* CCoClasses::static_pCoClass = NULL;
// IClassesPtr CCoClasses::static_pInterface;
CComImplPair<CCoClasses, IClasses, &IID_IClasses> CCoClasses::static_ComPair;

/////////////////////////////////////////////////////////////////////////////
// CCoClasses

CCoClasses::CCoClasses()
{
}

CCoClasses::~CCoClasses()
{
}

STDMETHODIMP CCoClasses::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	*pVal = Count();

	return S_OK;
}


STDMETHODIMP CCoClasses::get_Item(VARIANT varSrcName, IClassInfo **pVal)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	// TODO: Add your implementation code here
	return GetItem(&varSrcName, pVal);
}

void CCoClasses::Init()
{
// 	if (!static_pCoClass){
// 		CComObject<CCoClasses>::CreateInstance(&static_pCoClass);
// 		assert(static_pCoClass);
// 		
// 		static_pCoClass->AddRef();
// 		if (FAILED( static_pCoClass->QueryInterface(IID_IClasses, reinterpret_cast<void**>(&static_pInterface)) )){
// 			MessageBox("%s - ошибка получения интерфейса %s", "CCoClasses::Init", "IID_IClasses");
// 		}
// 		assert(static_pInterface);
// 		static_pCoClass->Release();
// 	}
	if (!static_ComPair){
		if ( !static_ComPair.CreateInstance() )
			MessageBox("%s - ошибка получения интерфейса %s", "CCoClasses::Init", "IID_IClasses");
	}
}

void CCoClasses::Destroy()
{
	Get()->RemoveAll();
	
	//static_pInterface.Release();	
	static_ComPair.Release();
}

void CCoClasses::IfNeeded_FillAllElements()
{
	if (IsEmpty())
		FillAllElements();
}

void CCoClasses::FillAllElements()
{
	RemoveAll();
	CClassInfo::ForEachClassInfo( &CCoClasses::Static_AddEachElement );
}

// TODO отрефакторить лишний метод Static_AddEachElement - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CCoClasses::Static_AddEachElement( const CClassInfo::CPtr& pClassInfo )
{
	Get()->Add(pClassInfo);
}

STDMETHODIMP CCoClasses::get__NewEnum(LPUNKNOWN *pVal)
{
	return GetNewEnum(pVal);
}

STDMETHODIMP CCoClasses::FindByPath(VARIANT varSrcPath, IClassInfo **pElem)
{
	return FindItemByPath(varSrcPath, pElem);
}

void CCoClasses::OnAdd( const CClassInfo::CPtr& pElem )
{
}

void CCoClasses::OnRemoveAll()
{
}