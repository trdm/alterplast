// CoDefFile.cpp : Implementation of CCoDefFile
#include "stdafx.h"
#include "Vis1cpp.h"
#include "CoDefFile.h"

// #include "CoClasses.h"

/////////////////////////////////////////////////////////////////////////////
// CCoDefFile

CCoDefFile::CCoDefFile()
{
	m_Name = L"";
	m_Path = L"";
}

CCoDefFile::~CCoDefFile()
{
	//Msg("CCoDefFile::~CCoDefFile %s");
}

STDMETHODIMP CCoDefFile::get_Name(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;
	
	*pVal = m_Name.Copy();
	
	return S_OK;
}

STDMETHODIMP CCoDefFile::get_Path(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;
	
	*pVal = m_Path.Copy();
	
	return S_OK;
}

void CCoDefFile::Init(const CDefFileInfo::CPtr& pElem)
{
	USES_CONVERSION;
	assert(pElem);
	
	m_pElem = pElem;
	
	m_Name = A2BSTR(pElem->GetDisplayName());
	m_Path = A2BSTR(pElem->GetPath()); // TODO разница
	
	InitChildClasses(pElem);
}

IDefFilePtr CCoDefFile::CreateInstance( const CDefFileInfo::CPtr& pElemInfo )
{
	assert(pElemInfo);

	CComObject<CCoDefFile>* pNewCoElem = NULL;
	CComObject<CCoDefFile>::CreateInstance(&pNewCoElem);
	assert(pNewCoElem);
	
	IDefFilePtr pInfo = NULL;
	
	pNewCoElem->AddRef();
	if (FAILED( pNewCoElem->QueryInterface(IID_IDefFile, reinterpret_cast<void**>(&pInfo)) )){
		MessageBox("%s - ошибка получения интерфейса %s", "CCoDefFile::CreateInstance", "IID_IDefFile");
	}
	assert(pInfo);
	pNewCoElem->Release();
	
	pNewCoElem->Init(pElemInfo);
	
	return pInfo;
}

// TODO метод должен возвращать объект ICfgDoc из Опенконфа
STDMETHODIMP CCoDefFile::Open()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	// TODO: Add your implementation code here
	if (m_pElem.expired())
		return S_FALSE;
	
 	CDefFileInfo::CPtr pClassInfo( m_pElem.lock() );
	assert(pClassInfo);
 	pClassInfo->Open();
	
	return S_OK;
}

static CInterfacesCollection<CClassInfo, IClassInfo, CCoClassInfo>* pStaticCoClassesCollection = NULL;

void CCoDefFile::InitChildClasses( const CDefFileInfo::CPtr& pElem )
{
	//Msg("%s - %s", "CCoDefFile::InitChildClasses", pElem->GetDisplayName().operator LPCTSTR());

	m_pIClassInfoCollection.Release();
	assert(!m_pIClassInfoCollection);
	//if (!m_pCoClassesCollection){
	CComObject<CCoClasses>::CreateInstance(&m_pCoClassesCollection);
	assert(m_pCoClassesCollection);
	
	m_pCoClassesCollection->AddRef();
	if (FAILED( m_pCoClassesCollection->QueryInterface(IID_IClasses, reinterpret_cast<void**>(&m_pIClassInfoCollection)) )){
		MessageBox("%s - ошибка получения интерфейса %s", "CCoClasses::FillAllElements", "IID_IClasses");
	}
	assert(m_pIClassInfoCollection);
	m_pCoClassesCollection->Release();
	//}
	
	assert(NULL != m_pCoClassesCollection);
	pStaticCoClassesCollection = m_pCoClassesCollection;
	pElem->ForEachChildClassInfoIntoDefFile( &CCoDefFile::Static_AddEachChildClassIntoDefFile );
}

// TODO отрефакторить лишний метод ForEachChildClassInfoIntoDefFile - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CCoDefFile::Static_AddEachChildClassIntoDefFile( const CClassInfo::CWeakPtr& pClassInfo )
{
	assert(NULL != pStaticCoClassesCollection);
	assert(!pClassInfo.expired());
	
	pStaticCoClassesCollection->Add(pClassInfo.lock());
}

STDMETHODIMP CCoDefFile::get_Classes(IClasses **pVal)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	m_pIClassInfoCollection.CopyTo(pVal);

	return S_OK;
}
