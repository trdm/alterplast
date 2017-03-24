// VLResultLoader.cpp : реализация универсального загрузчика IResultLoader/IResultPublisher для СписокЗначений / ТаблицаЗначений.
//
//	Автор - Артур Аюханов aka artbear, 2008
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Interfaces/VLResultLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using boost::shared_ptr;

CDocContext* CValue2Doc(CValue const& Value);

void CVLResultLoader::setContext(CValueListContext* pCont)
{
	assert(pCont);

	m_pCont = static_cast<VLC_TYPE*>(pCont);
	m_pList = m_pCont->GetPValueList();
}

CBLPtr<CValueListContext> CVLResultLoader::createAndSetContext()
{
	VLC_TYPE* pCont = new VLC_TYPE; //CValueListContext* pCont = new CValueListContext;
	assert(pCont);

	setContext(pCont);
	return CBLPtr<CValueListContext>(pCont); //m_pCont;
}

// artbear TODO при использовании штатных методов, например, CValueListContextFriend::P_Add(ppValues)
// пропускаются пустые значения, и в приемник-СЗ перестает соответствовать источнику, что плохо :(
// но без штатных методов есть баг с выгрузкой в список значений на форме - нужно думать :(
// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=4016

// сейчас же пустые значения копируются также, как и заполненные.
// и есть точное соответствие между источником и СЗ

//////////////////////////////////////////////////////////////////////////
//	IResultLoader interface
//////////////////////////////////////////////////////////////////////////

HRESULT CVLResultLoader::ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(m_pCont.operator bool());

	//// TODO нужно очищать или нет перед загрузкой ? сейчас очищается в любом случае
	//if (m_pList->GetSize()) // if (m_pCont->GetSize())
	//{
	//	CValueItem** ppItems = (CValueItem**)m_pList->GetData();
	//	for(DWORD c = m_pList->GetSize(); c--;)
	//		delete *ppItems++;
	//	m_pList->RemoveAll();
	//	//m_pCont->P_RemoveAll(NULL);
	//}
	nColCount = min(2, nColCount);

	m_colCount = nColCount;
	return NO_ERROR;
}

HRESULT CVLResultLoader::ldrDone() throw()
{
	assert(m_pCont.operator bool());

	m_pCont->RefreshCtrl();
	return NO_ERROR;
}

HRESULT CVLResultLoader::ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
{ 
	assert(m_pCont.operator bool());

	return NO_ERROR; 
}

HRESULT CVLResultLoader::ldrAddRow(CValue const* const* ppValues) throw()
{
	assert(m_pCont.operator bool());
	assert(NULL != ppValues);

	CString strColumnName;
	if (2 == m_colCount){
		//CValue val = **ppValues++;
		//strColumnName = val.GetString();
		CValue valColumnName = **(ppValues+1);
		strColumnName = valColumnName.GetString();
	}
	CValueItem* pItem = new CValueItem(**ppValues, strColumnName);
	m_pList->Add(pItem);
	//if (1 != m_colCount)
	//	m_pCont->P_Add(ppValues);
	//else{
	//	CValue valColumName;
	//	CValue* pVal[2] = {ppValues[0], &valColumName};
	//	m_pCont->P_Add(pVal);
	//}
	return NO_ERROR;
}

LPCSTR CVLResultLoader::GetLastError() throw()
{ return ""; }

void CVTResultLoader::setContext(CValueTableContext* pCont)
{
	assert(pCont);

	m_pCont = pCont;
	m_pTable = static_cast<CValueTableContextData*>(pCont->GetInternalData())->GetValueTable();

	CDerivedVT* pDVT = static_cast<CDerivedVT*>(m_pTable);
	m_bPrevEnableRefresh = pDVT->m_bEnableRefresh;
	pDVT->m_bEnableRefresh = FALSE;
}
// TODO решить вопрос с выгрузкой в таблицу значений на форме - по аналогии с CVTExtended::UnloadToVT

//////////////////////////////////////////////////////////////////////////
//	IResultLoader interface
//////////////////////////////////////////////////////////////////////////

HRESULT CVTResultLoader::ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(m_pCont.operator bool());

	m_nAddedCount = 0;
	m_nColCount = nColCount;
	m_vp.resize(m_nColCount, &m_vEmpty);
	return NO_ERROR;
}

HRESULT CVTResultLoader::ldrDone() throw()
{
	assert(m_pCont.operator bool());

	// restore refresh flag and refresh
	CDerivedVT* pDVT = static_cast<CDerivedVT*>(m_pTable);
	pDVT->m_bEnableRefresh = m_bPrevEnableRefresh;
	pDVT->DeleteRow(m_pTable->NewRow(-1));

	return NO_ERROR;
}

HRESULT CVTResultLoader::ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
{
	assert(m_pCont.operator bool());

	if (++m_nAddedCount > m_pTable->GetColumnCount())
	{
		if (!pType)
			m_pTable->AddColumn(szColName, CType(0), szColName, 0, CString(), 0);
		else
			m_pTable->AddColumn(szColName, *pType, szColName, 0, CString(), 0);
	}
	return NO_ERROR;
}

HRESULT CVTResultLoader::ldrAddRow(CValue const* const* ppValues) throw()
{
	assert(m_pCont.operator bool());
	assert(NULL != ppValues);

	for (int i = 0; i < m_nColCount; ++i)
		m_vp[i] = const_cast<CValue*>(ppValues[i]);

	m_pTable->AddRow(&m_vp[0], -1);
	return NO_ERROR;
}

LPCSTR CVTResultLoader::GetLastError() throw()
{ return ""; }

void CTableOfDocResultLoader::setContext(CDocContext* pDoc)
{
	assert(pDoc);
	m_pDoc = pDoc;

	m_vVT.CreateObject("ТаблицаЗначений");
	m_pVTLoader = CValue2ResultLoader(m_vVT);
}

//////////////////////////////////////////////////////////////////////////
//	IResultLoader interface
//////////////////////////////////////////////////////////////////////////

HRESULT CTableOfDocResultLoader::ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(m_pDoc);
	if (!m_pDoc->m_dwDocIsSelect)
	{
		setError("Контекст документа не определен!");
		return E_FAIL;
	}

	HRESULT hr = m_pVTLoader->ldrInit(nColCount, nRowCount);
	if (FAILED(hr))
		setError(m_pVTLoader->GetLastError());
	return hr;
}

HRESULT CTableOfDocResultLoader::ldrDone() throw()
{
	if (!m_pDoc->m_dwDocIsSelect)
		return S_OK;

	HRESULT hr = m_pVTLoader->ldrDone();
	if (FAILED(hr))
		setError(m_pVTLoader->GetLastError());
	else
	{
		CValue* params[1] = { &m_vVT };
		::CallAsProc(m_pDoc, "ЗагрузитьТабличнуюЧасть", params);
	}
	return hr;
}

HRESULT CTableOfDocResultLoader::ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
{
	HRESULT hr = m_pVTLoader->ldrAddColumn(szColName, pType);
	if (FAILED(hr))
		setError(m_pVTLoader->GetLastError());
	return hr;
}

HRESULT CTableOfDocResultLoader::ldrAddRow(CValue const* const* ppValues) throw()
{
	assert(NULL != ppValues);
	HRESULT hr = m_pVTLoader->ldrAddRow(ppValues);
	if (FAILED(hr))
		setError(m_pVTLoader->GetLastError());
	return hr;
}

LPCSTR CTableOfDocResultLoader::GetLastError() throw()
{ 
	return m_csLastError;
}

void CVLResultPublisher::setContext(CValueListContext* pCont)
{
	assert(pCont);

	m_pCont = static_cast<VLC_TYPE*>(pCont);
	m_pList = m_pCont->GetPValueList();
}

//CBLPtr<CValueListContext> CVLResultPublisher::createAndSetContext()
//{
//	VLC_TYPE* pCont = new VLC_TYPE; //CValueListContext* pCont = new CValueListContext;
//	assert(pCont);

//	setContext(pCont);
//	return CBLPtr<CValueListContext>(pCont); //m_pCont;
//}

// artbear TODO при использовании штатных методов, например, CValueListContextFriend::P_Add(ppValues)
// пропускаются пустые значения, и в приемник-СЗ перестает соответствовать источнику, что плохо :(
// но без штатных методов есть баг с выгрузкой в список значений на форме - нужно думать :(
// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=4016

// сейчас же пустые значения копируются также, как и заполненные.
// и есть точное соответствие между источником и СЗ

//////////////////////////////////////////////////////////////////////////
//	IResultPublisher interface
//////////////////////////////////////////////////////////////////////////

HRESULT CVLResultPublisher::pubInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(m_pCont.operator bool());

	nColCount = 2;
	if (m_ValueCollection.empty())
	{
		m_ValueCollection.resize(nColCount);
		m_Value.reset(new CValue);
		m_TextValue.reset(new CValue);
		m_ValueCollection[0] = m_Value.get();
		assert(2 == nColCount);
		m_ValueCollection[1] = m_TextValue.get();
	}
	m_nRowIndex = 0;

	nRowCount = m_pList->GetSize();

	return NO_ERROR;
}

HRESULT CVLResultPublisher::pubDone() throw()
{
	return NO_ERROR;
}

HRESULT CVLResultPublisher::pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw()
{ 
	szColName = NULL;
	pType = NULL;
	return NO_ERROR; 
}

HRESULT CVLResultPublisher::pubGetRow(CValue const* const*& ppValues) throw()
{
	assert(m_pCont.operator bool());

	if (m_pList->GetSize() == m_nRowIndex)
		return S_FALSE;
	*m_Value = m_pList->GetAt(m_nRowIndex)->m_value;
	*m_TextValue = m_pList->GetAt(m_nRowIndex)->m_text;
	++m_nRowIndex;

	ppValues = &m_ValueCollection[0];

	return NO_ERROR;
}

LPCSTR CVLResultPublisher::GetLastError() throw()
{ return ""; }

void CVTResultPublisher::setContext(CValueTableContext* pCont)
{
	assert(pCont);

	m_pCont = pCont;
	m_pTable = static_cast<CValueTableContextData*>(pCont->GetInternalData())->GetValueTable();
}
// TODO решить вопрос с выгрузкой в таблицу значений на форме - по аналогии с CVTExtended::UnloadToVT

//////////////////////////////////////////////////////////////////////////
//	IResultLoader interface
//////////////////////////////////////////////////////////////////////////

HRESULT CVTResultPublisher::pubInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(m_pCont.operator bool());

	nColCount = m_pTable->GetColumnCount();
	nRowCount = m_pTable->GetRowCount();

	m_vector.resize(nColCount);
	m_pVector.resize(nColCount);
	for (int i = 0; i < nColCount; ++i)
		m_pVector[i] = &m_vector[i];

	iRowIndex = 0;

	return NO_ERROR;
}

HRESULT CVTResultPublisher::pubDone() throw()
{
	// TODO проверить на тестах для КОП правильность удаления !!
	//m_vector.erase(m_vector.begin(), m_vector.end());
	//m_pVector.erase(m_pVector.begin(), m_pVector.end());

	return NO_ERROR;
}

HRESULT CVTResultPublisher::pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw()
{
	assert(m_pCont.operator bool());

	szColName = m_pTable->GetColumnCode(nIndex);
	CVTColumn* pVTColumn = m_pTable->GetColumn(nIndex);
	pType = &const_cast<CType&>(pVTColumn->GetType()); // TODO возможно, что в интерфейсе должно быть CType *const & pType

	return NO_ERROR;
}

HRESULT CVTResultPublisher::pubGetRow(CValue const* const*& ppValues) throw()
{
	assert(m_pCont.operator bool());

	if (m_pTable->GetRowCount() == iRowIndex)
		return S_FALSE;

	int nColCount = m_pTable->GetColumnCount();
	for (int i = 0; i < nColCount; ++i){
		m_vector[i] = m_pTable->GetValue(i, iRowIndex);
	}

	++iRowIndex;
	CValue* * ppResultValues = &m_pVector[0];
	ppValues = ppResultValues;

	return NO_ERROR;
}

LPCSTR CVTResultPublisher::GetLastError() throw()
{ return ""; }

shared_ptr<IResultLoader> CValue2ResultLoader(CValue const& vDest)
{
	CBLContext* pCont = vDest.GetContext();
	if(!pCont)
		return shared_ptr<IResultLoader>();

	return CBLContext2ResultLoader(pCont);
}

class IResultLoader_functor : public CastContext::by_empty
{
public:
	BOOL CheckContext(CBLContext* pCont) {
		return IsGeneralResultLoader(pCont); 
	}
	shared_ptr<IResultLoader> GetResultLoader() const { return m_pLoader; };
private:
	bool IsGeneralResultLoader(CBLContext* pCont);

	shared_ptr<IResultLoader> m_pLoader;
};

shared_ptr<IResultLoader> CBLContext2ResultLoader(CBLContext* pSrcCont)
{
	IResultLoader_functor find_functor;

	CBLContext* pFindCont = CastContext::DoCast(pSrcCont, find_functor);

	if (!pFindCont)
		return shared_ptr<IResultLoader>();
	return find_functor.GetResultLoader();
}

bool IResultLoader_functor::IsGeneralResultLoader(CBLContext* pCont)
{
	m_pLoader = shared_ptr<IResultLoader>();

	if(!pCont)
		return false;

	LPCSTR lpszClassName = pCont->GetRuntimeClass()->m_lpszClassName;

	if(!strcmp(lpszClassName, "CValueTableContext"))
	{
		shared_ptr<CVTResultLoader> pVTLoader(new CVTResultLoader);
		pVTLoader->setContext(static_cast<CValueTableContext*>(pCont));

		m_pLoader = pVTLoader;
		return true;
	}
	else if(!strcmp(lpszClassName, "CValueListContext"))
	{
		shared_ptr<CVLResultLoader> pVLLoader(new CVLResultLoader);
		pVLLoader->setContext(static_cast<CValueListContext*>(pCont));

		m_pLoader = pVLLoader;
		return true;
	}
	else{
		using Utils1cpp::IQueryInterface_Loader_Publisher;
		IQueryInterface_Loader_Publisher* pQueryInterface_Loader_Unloader = safe_dynamic_cast<IQueryInterface_Loader_Publisher*>(pCont);
		if (pQueryInterface_Loader_Unloader)
			if (!pQueryInterface_Loader_Unloader->IsLoader())
				return false;

		IResultLoader* pLoader = safe_dynamic_cast<IResultLoader*>(pCont);

		if (pLoader){
			shared_ptr<IResultLoader> pDestLoader ( pLoader, null_deleter<IResultLoader>() );

			m_pLoader = pDestLoader;
			return true;
		}
	}

	CValue vDoc; vDoc.AssignContext(pCont);
	CDocContext* pDoc = CValue2Doc(vDoc);
	if (pDoc){
		shared_ptr<CTableOfDocResultLoader> pTableOfDocLoader(new CTableOfDocResultLoader);
		pTableOfDocLoader->setContext(pDoc);

		m_pLoader = pTableOfDocLoader;
		return true;
	}

	return false;
}

shared_ptr<IResultPublisher> CValue2ResultPublisher(CValue const& vDest)
{
	CBLContext* pCont = vDest.GetContext();
	if(!pCont)
		return shared_ptr<IResultPublisher>();

	return CBLContext2ResultPublisher(pCont);
}

class IResultPublisher_functor : public CastContext::by_empty
{
public:
	BOOL CheckContext(CBLContext* pCont) {
		return IsResultPublisher(pCont); 
	}
	shared_ptr<IResultPublisher> GetResultPublisher() const { return m_pLoader; };
private:
	bool IsResultPublisher(CBLContext* pCont);

	shared_ptr<IResultPublisher> m_pLoader;
};

shared_ptr<IResultPublisher> CBLContext2ResultPublisher(CBLContext* pSrcCont)
{
	IResultPublisher_functor find_functor;

	CBLContext* pFindCont = CastContext::DoCast(pSrcCont, find_functor);

	if (!pFindCont)
		return shared_ptr<IResultPublisher>();
	return find_functor.GetResultPublisher();
}

bool IResultPublisher_functor::IsResultPublisher(CBLContext* pCont)
{
	m_pLoader = shared_ptr<IResultPublisher>();

	if(!pCont)
		return false;

	LPCSTR lpszClassName = pCont->GetRuntimeClass()->m_lpszClassName;

	if(!strcmp(lpszClassName, "CValueTableContext"))
	{
		shared_ptr<CVTResultPublisher> pVTLoader(new CVTResultPublisher);
		pVTLoader->setContext(static_cast<CValueTableContext*>(pCont));

		m_pLoader = pVTLoader;
		return true;
	}
	else if(!strcmp(lpszClassName, "CValueListContext"))
	{
		shared_ptr<CVLResultPublisher> pVLLoader(new CVLResultPublisher);
		pVLLoader->setContext(static_cast<CValueListContext*>(pCont));

		m_pLoader = pVLLoader;
		return true;
	}
	else{
		using Utils1cpp::IQueryInterface_Loader_Publisher;
		IQueryInterface_Loader_Publisher* pQueryInterface_Loader_Unloader = safe_dynamic_cast<IQueryInterface_Loader_Publisher*>(pCont);
		if (pQueryInterface_Loader_Unloader)
			if (!pQueryInterface_Loader_Unloader->IsPublisher())
				return false;

		IResultPublisher* pLoader = safe_dynamic_cast<IResultPublisher*>(pCont);

		if (pLoader){
			shared_ptr<IResultPublisher> pDestLoader ( pLoader, null_deleter<IResultPublisher>() );

			m_pLoader = pDestLoader;
			return true;
		}
	}

	//CValue vDoc; vDoc.AssignContext(pCont);
	//CDocContext* pDoc = CValue2Doc(vDoc);
	//if (pDoc){
	//	shared_ptr<CTableOfDocResultLoader> pTableOfDocLoader(new CTableOfDocResultLoader);
	//	pTableOfDocLoader->setContext(pDoc);

	//	m_pLoader = pTableOfDocLoader;
	//	return true;
	//}

	return false;
}
