// CoClassInfo.cpp : Implementation of CCoClassInfo
#include "stdafx.h"
#include "Vis1cpp.h"
#include "CoClassInfo.h"

#include "CoClasses.h"

/////////////////////////////////////////////////////////////////////////////
// CCoClassInfo

CCoClassInfo::CCoClassInfo()
{
	m_Name = L"";
	m_Path = L"";
	m_DefFilePath = L"";
}

CCoClassInfo::~CCoClassInfo()
{
}

STDMETHODIMP CCoClassInfo::get_Name(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;

	*pVal = m_Name.Copy();

	return S_OK;
}

STDMETHODIMP CCoClassInfo::get_Path(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;

	*pVal = m_Path.Copy();

	return S_OK;
}

void CCoClassInfo::Init(const CClassInfo::CPtr& pInfo)
{
	USES_CONVERSION;
	assert(pInfo);

	m_pElem = pInfo;

	m_Name = A2BSTR(pInfo->GetDisplayName());
	m_Path = A2BSTR(pInfo->GetPath());
	m_DefFilePath = A2BSTR(pInfo->GetDefFilePath());

	InitChildClasses(pInfo);
}

IClassInfoPtr CCoClassInfo::CreateInstance( const CClassInfo::CPtr& pClassInfo )
{
	assert(pClassInfo);

	CComObject<CCoClassInfo>* pNewCoClassInfo = NULL;
	CComObject<CCoClassInfo>::CreateInstance(&pNewCoClassInfo);
	assert(pNewCoClassInfo);
	
	IClassInfoPtr pInfo = NULL;
	
	pNewCoClassInfo->AddRef();
	if (FAILED( pNewCoClassInfo->QueryInterface(IID_IClassInfo, reinterpret_cast<void**>(&pInfo)) )){
		MessageBox("%s - ошибка получения интерфейса %s", "CCoClassInfo::CreateInstance", "IID_IClassInfo");
	}
	assert(pInfo);
	pNewCoClassInfo->Release();
	
	pNewCoClassInfo->Init(pClassInfo);

	return pInfo;
}

// TODO метод должен возвращать объект ICfgDoc из Опенконфа
STDMETHODIMP CCoClassInfo::Open()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;

	CClassInfo::CPtr pElem( m_pElem.lock() );
	assert(pElem);
	pElem->OpenClass();

	return S_OK;
}

STDMETHODIMP CCoClassInfo::get_DefFilePath(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;
	
	*pVal = m_DefFilePath.Copy();

	return S_OK;
}

STDMETHODIMP CCoClassInfo::get_SourceText(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;

	CClassInfo::CPtr pElem( m_pElem.lock() );
	assert(pElem);
	CComBSTR SourceText = L"";
	SourceText = A2BSTR(pElem->GetSourceText());

	*pVal = SourceText.Copy();

	return S_OK;
}

static CInterfacesCollection<CClassInfo, IClassInfo, CCoClassInfo>* pStaticCoClassesCollection = NULL;

void CCoClassInfo::InitChildClasses( const CClassInfo::CPtr& pInfo )
{
	//Msg("%s - %s", "CCoDefFile::InitChildClasses", pElem->GetDisplayName().operator LPCTSTR());
	
	m_pBaseIClassInfoCollection.Release();
	assert(!m_pBaseIClassInfoCollection);
	//if (!m_pCoClassesCollection){
	CComObject<CCoClasses>::CreateInstance(&m_pBaseCoClassesCollection);
	assert(m_pBaseCoClassesCollection);
	
	m_pBaseCoClassesCollection->AddRef();
	if (FAILED( m_pBaseCoClassesCollection->QueryInterface(IID_IClasses, reinterpret_cast<void**>(&m_pBaseIClassInfoCollection)) )){
		MessageBox("%s - ошибка получения интерфейса %s", "CCoClasses::FillAllElements", "IID_IClasses");
	}
	assert(m_pBaseIClassInfoCollection);
	m_pBaseCoClassesCollection->Release();
	//}
	
	assert(NULL != m_pBaseCoClassesCollection);
	pStaticCoClassesCollection = m_pBaseCoClassesCollection;
	//pElem->ForEachChildClassInfoIntoDefFile( &CCoDefFile::Static_AddEachChildClassIntoDefFile );

	pInfo->ForEachChildClass( &CCoClassInfo::Static_AddEachChildClass );
}

// TODO отрефакторить лишний метод Static_AddEachChildClass - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CCoClassInfo::Static_AddEachChildClass( const CClassInfo::CWeakPtr& pClassInfo )
{
	assert(NULL != pStaticCoClassesCollection);
	assert(!pClassInfo.expired());
	
	pStaticCoClassesCollection->Add(pClassInfo.lock());
}

STDMETHODIMP CCoClassInfo::get_BaseClasses(IClasses **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	m_pBaseIClassInfoCollection.CopyTo(pVal);

	return S_OK;
}
