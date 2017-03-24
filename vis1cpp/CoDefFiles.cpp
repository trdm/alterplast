// CoDefFiles.cpp : Implementation of CCoDefFiles
#include "stdafx.h"
#include <functional>

#include "Vis1cpp.h"
#include "Vis1cpp_my.h"


#include "CoDefFiles.h"

/////////////////////////////////////////////////////////////////////////////
// CCoDefFiles

CComImplPair<CCoDefFiles, IDefFiles, &IID_IDefFiles> CCoDefFiles::static_ComPair;

CCoDefFiles::CCoDefFiles()
{
	
}

CCoDefFiles::~CCoDefFiles()
{
}

void CCoDefFiles::Init()
{
	if (!static_ComPair){
		if ( !static_ComPair.CreateInstance() )
			MessageBox("%s - ошибка получения интерфейса %s", "CCoDefFiles::Init", "IID_IDefFiles");
	}
}

void CCoDefFiles::Destroy()
{
	Get()->RemoveAll();

	static_ComPair.Release();
}

STDMETHODIMP CCoDefFiles::get_Count(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	// TODO: Add your implementation code here
 	*pVal = Count();
	
	return S_OK;
}

STDMETHODIMP CCoDefFiles::get_Item(VARIANT varSrcIndexOrDefPath, IDefFile **pVal)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	// TODO: Add your implementation code here
	return GetItem(&varSrcIndexOrDefPath, pVal);
}

// умеем работать с FOR EACH в Visual Basic
STDMETHODIMP CCoDefFiles::get__NewEnum(LPUNKNOWN *pVal)
{
	return GetNewEnum(pVal);
}

void CCoDefFiles::IfNeeded_FillAllElements()
{
	if (IsEmpty())
		FillAllElements();
}

void CCoDefFiles::FillAllElements()
{
	RemoveAll();

	CDefFileInfo::ForEach( &CCoDefFiles::Static_AddEachElement );
	//CDefFileInfo::ForEach( std::mem_fun_ref<void, CCoDefFiles, >( &CCoDefFiles::Add );
}

// TODO отрефакторить лишний метод Static_AddEachElement - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CCoDefFiles::Static_AddEachElement( const CDefFileInfo::CPtr& pClassInfo )
{
	Get()->Add(pClassInfo);
}

void CCoDefFiles::OnAdd( const CDefFileInfo::CPtr& pElem )
{
}

void CCoDefFiles::OnRemoveAll()
{
}

STDMETHODIMP CCoDefFiles::FindByPath(VARIANT varSrcPath, IDefFile **pElem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	return FindItemByPath(varSrcPath, pElem);
}

CString CCoDefFiles::GetItemName( const CDefFileInfo::CPtr& pClassInfo )
{
	return pClassInfo->GetPath();
}