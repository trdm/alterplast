#include "StdAfx.h"
#include "CreationBoost.h"
#include "MyAddinContext.h"

#include "../Interfaces/ICloneCreated.h"
using Utils1cpp::ICloneCreated;

struct ICreatedStrategy
{
	virtual ~ICreatedStrategy() {};
	virtual CBLContext* CreateObject(LPCSTR szClassName) = 0;
};

typedef ICreatedStrategy* ICreatedStrategyPtr;
typedef CIStringMap< ICreatedStrategyPtr, ICreatedStrategyPtr > CIStringMapToInfo;
typedef boost::shared_ptr<CIStringMapToInfo> CIStringMapToInfoPtr;

class CCreatedStrategyFromRuntimeClass : public ICreatedStrategy
{
private:
	CCreatedStrategyFromRuntimeClass(CRuntimeClass* pRuntimeClass): m_pRuntimeClass(pRuntimeClass)
	{ };
	CRuntimeClass* m_pRuntimeClass; // для ускорения создания контекстов

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	bool m_bDontUseInitObject;
//	CCreatedStrategyFromRuntimeClass(CRuntimeClass* pRuntimeClass, bool bDontUseInitObject): m_pRuntimeClass(pRuntimeClass),
//		m_bDontUseInitObject(bDontUseInitObject)
//	{ };
//#endif

public:
	static ICreatedStrategyPtr Create(CRuntimeClass* pRuntimeClass)
	{ return ICreatedStrategyPtr(new CCreatedStrategyFromRuntimeClass(pRuntimeClass)); };

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	static ICreatedStrategyPtr Create(CRuntimeClass* pRuntimeClass, bool bDontUseInitObject)
//	{ return ICreatedStrategyPtr(new CCreatedStrategyFromRuntimeClass(pRuntimeClass, bDontUseInitObject)); };
//#endif

	// ICreatedStrategy
	virtual CBLContext* CreateObject(LPCSTR szClassName) {
		CBLContext* pCont = static_cast<CBLContext*>(m_pRuntimeClass->CreateObject());
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//		if (!m_bDontUseInitObject)
//#endif
			pCont->InitObject(szClassName);
		return pCont;
	}
};

//// быстрое создание системных объектов
//class CCreatedStrategyFromString : public ICreatedStrategy
//{
//private:
//	CCreatedStrategyFromString(LPCSTR szFastCreationString): m_szFastCreationString(szFastCreationString)
//	{};
//
//	// TODO для произвольных строк нужно использовать CString !
//	// для фиксированных строк (например, статичных в модуле) можно использовать LPCSTR для ускорения
//	//CString m_szFastCreationString; // если LPCSTR - тогда ссылки не сохраняются, и 1С падает
//	LPCSTR m_szFastCreationString; // если LPCSTR - тогда ссылки не сохраняются, и 1С падает
//
//	static CValue m_ValueForFastCreation;
//
//public:
//	static ICreatedStrategyPtr Create(LPCSTR szFastCreationString)
//	{ return ICreatedStrategyPtr(new CCreatedStrategyFromString(szFastCreationString)); };
//
//	// ICreatedStrategy
//	virtual CBLContext* CreateObject(LPCSTR szClassName)
//	{
//		LoadValueFromString(&m_ValueForFastCreation, m_szFastCreationString);
//		CBLContext* pCont = m_ValueForFastCreation.GetContext();
//
//		// чтобы контекст не был удален после сброса на след.строке
//		if (pCont)
//			pCont->IncrRef();
//
//		m_ValueForFastCreation.Reset(); // сбросить временный объект
//		return pCont;
//	}
//
//};

class CCreatedStrategyFromCloneCreated : public ICreatedStrategy
{
private:
	CCreatedStrategyFromCloneCreated(ICloneCreated* pCloneCreatedContextObject, CBLContext* pContForCloneCreated): 
	   m_pCloneCreatedContextObject(pCloneCreatedContextObject), m_pContForCloneCreated(pContForCloneCreated)
	{ };

	CBLContext* m_pContForCloneCreated;
	ICloneCreated* m_pCloneCreatedContextObject;

public:
	static ICreatedStrategyPtr Create(ICloneCreated* pCloneCreatedContextObject, CBLContext* pContForCloneCreated)
	{ return ICreatedStrategyPtr(new CCreatedStrategyFromCloneCreated(pCloneCreatedContextObject, pContForCloneCreated)); };

	// ICreatedStrategy
	virtual CBLContext* CreateObject(LPCSTR szClassName)
	{
		return m_pCloneCreatedContextObject->Clone(szClassName);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CCreateInstanceBoost
//

// класс-одиночка с функционалом - 
// перехват создания объекта - т.е. когда пользователь пишет СоздатьОбъект(ТипОбъекта)
// здесь самое начало ООП - возможность создавать собственные типы !
//	также выполнено ускорение создания любых объектов в 1С
//
class CCreateInstanceBoost::CCreateInstanceBoostImpl : public boost::noncopyable
{
private:
	typedef CBLContext* (*PF_CreateInstance)(char const *);

	CDllMethodWrapper<PF_CreateInstance> m_wrapperCreateInstance;

	CIStringMapToInfoPtr m_pClassNameHash;

	CCreateInstanceBoostImpl(CCreateInstanceBoostImpl&){};

	static CBLContext* CreateInstanceBoosted(char const * szClassName);
	CBLContext* Create(char const * szClassName);
	void AddCloneCreatedContext(ICloneCreated* pCloneCreatedContext, LPCSTR szClassName);

	void AutoGrowIfNeeded();

public:	
	
	CCreateInstanceBoostImpl() 
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	: m_iTestDisable(0)
//#endif
	{};

	CBLContext* CreateInstanceWithoutBoosted(char const * szClassName);

	static CCreateInstanceBoostImpl& TheObject();

	void Init(void);
	void Destroy(void);

	void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration);

	//void AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString);

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	void TestDisableFastCreationAllContexts(int param);
//
//private:
//	int m_iTestDisable;
//
//#endif
};

CCreateInstanceBoost& CCreateInstanceBoost::TheObject()
{
	static CCreateInstanceBoost object;
	return object;
}

CCreateInstanceBoost::CCreateInstanceBoostImpl& CCreateInstanceBoost::CCreateInstanceBoostImpl::TheObject()
{
	static CCreateInstanceBoostImpl object;
	return object;
}

CCreateInstanceBoost::CCreateInstanceBoost() : m_pImpl(&CCreateInstanceBoostImpl::TheObject())
{}

CCreateInstanceBoost::~CCreateInstanceBoost()
{}

//static LPCSTR szFastCreationValueList =  "{\"VL\",{}}";
//static LPCSTR szFastCreationTableList =  "{\"VT\",\"1\",{\"0\",{{\"\",\"0\",\"0\",\"0\",\"\",\"2\"}}}}";

//CValue CCreatedStrategyFromString::m_ValueForFastCreation;

void CCreateInstanceBoost::Init(void)
{
	m_pImpl->Init();
};

void CCreateInstanceBoost::CCreateInstanceBoostImpl::Init(void)
{
	m_pClassNameHash = CIStringMapToInfoPtr(new CIStringMapToInfo());

	m_wrapperCreateInstance.DoWrap(::GetModuleHandle("bkend.dll"), "?CreateInstance@CBLContext@@SAPAV1@PBD@Z", 
		(PF_CreateInstance)CCreateInstanceBoostImpl::CreateInstanceBoosted);

	// быстрое создание этих объектов теперь реализовано через перехват CBLContext::InitObject
	//AddFastCreationObject("СписокЗначений", szFastCreationValueList);
	//AddFastCreationObject("ТаблицаЗначений", szFastCreationTableList);
};

void CCreateInstanceBoost::Destroy(void)
{
	m_pImpl->Destroy();
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::Destroy(void)
{
	if (m_pClassNameHash)
	{
		m_pClassNameHash.reset();
		m_wrapperCreateInstance.RestoreOrigAddress();
	}
}

void CCreateInstanceBoost::RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration /*= TRUE*/)
{
	m_pImpl->RegisterCBLContext(pRuntimeClass, szClassName, bNativeRegistration);
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::AutoGrowIfNeeded()
{
	if (m_pClassNameHash->GetCount() * 1.2 > m_pClassNameHash->GetHashTableSize())
	{
		CIStringMapToInfoPtr pNewHash(new CIStringMapToInfo());
		pNewHash->InitHashTable((m_pClassNameHash->GetCount() + 11) * 1.2);
		POSITION pos = m_pClassNameHash->GetStartPosition();
		CString strCurName;
		ICreatedStrategyPtr pObject;
		while (pos)
		{
			m_pClassNameHash->GetNextAssoc(pos, strCurName, pObject);
			pNewHash->operator[](strCurName) = pObject;
		}

		m_pClassNameHash = pNewHash;
	}
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration /*= TRUE*/)
{
	AutoGrowIfNeeded();
	
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	bool bDontUseInitObject = m_iTestDisable = 3;
//#endif

	if (bNativeRegistration)
		CBLContext::RegisterContextClass(pRuntimeClass, szClassName, CType(100));

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	ICreatedStrategyPtr pInfo(CCreatedStrategyFromRuntimeClass::Create(pRuntimeClass, bDontUseInitObject)); // TODO объект никогда не удаляется - неявная работа с памятью, лучше юзать смарт-пойнтер
//#else
	ICreatedStrategyPtr pInfo(CCreatedStrategyFromRuntimeClass::Create(pRuntimeClass)); // TODO объект никогда не удаляется - неявная работа с памятью, лучше юзать смарт-пойнтер
//#endif

	m_pClassNameHash->operator[](szClassName) = pInfo;
}

void CCreateInstanceBoost::CCreateInstanceBoostImpl::AddCloneCreatedContext(ICloneCreated* pCloneCreatedContextObject, LPCSTR szClassName)
{
	AutoGrowIfNeeded();

	CBLContext* pContForCloneCreated = pCloneCreatedContextObject->Clone(szClassName);
	assert(1 == pContForCloneCreated->m_RefCount);

	ICloneCreated* pNewCloneCreatedContextObject = dynamic_cast<ICloneCreated*>(pContForCloneCreated);
	assert(NULL != pNewCloneCreatedContextObject);

	ICreatedStrategyPtr pInfo(CCreatedStrategyFromCloneCreated::Create(pNewCloneCreatedContextObject, pContForCloneCreated)); // TODO объект никогда не удаляется - неявная работа с памятью, лучше юзать смарт-пойнтер
	m_pClassNameHash->operator[](szClassName) = pInfo;
}

//void CCreateInstanceBoost::AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString)
//{
//	m_pImpl->AddFastCreationObject(szClassName, szClassFastCreationString);
//}
//
//// для быстрого создания системных объектов
//void CCreateInstanceBoost::CCreateInstanceBoostImpl::AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString)
//{
//	ICreatedStrategyPtr pInfo(CCreatedStrategyFromString::Create(szClassFastCreationString)); // TODO объект никогда не удаляется - неявная работа с памятью, лучше юзать смарт-пойнтер
//	m_pClassNameHash->operator[](szClassName) = pInfo;
//}

CBLContext* CCreateInstanceBoost::CreateInstanceWithoutBoosted(char const * szClassName)
{
	return m_pImpl->CreateInstanceWithoutBoosted(szClassName);
}

CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::CreateInstanceWithoutBoosted(char const * szClassName)
{
	CDllMethodWrapperGuard<PF_CreateInstance> wrapper(TheObject().m_wrapperCreateInstance);

	return CBLContext::CreateInstance(szClassName);
}

// перехват создания объекта - т.е. когда пользователь пишет СоздатьОбъект(ТипОбъекта)
// здесь самое начало ООП - возможность создавать собственные типы !
//	также выполнено ускорение создания любых объектов в 1С
//
CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::CreateInstanceBoosted(char const * szClassName)
{
	return TheObject().Create(szClassName);
}

CBLContext* CCreateInstanceBoost::CCreateInstanceBoostImpl::Create(char const * szClassName)
{
//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	if (1 == m_iTestDisable)
//		return CreateInstanceWithoutBoosted(szClassName);
//#endif

	ICreatedStrategyPtr pInfo;

	if (m_pClassNameHash->Lookup(szClassName, pInfo))
		return pInfo->CreateObject(szClassName);

	CBLContext* pBLContext = CreateInstanceWithoutBoosted(szClassName);

	if (!pBLContext)
		return NULL;

	int iIsOleContext = pBLContext->IsOleContext();

// TODO работа с интерфейсом клонирования ICloneCreated временно отключена до реализации подобного поведения в классах,
// например, для классов КОП
//	if (!iIsOleContext)
////#ifdef TEST_FOR_COMPARE_CREATION_SPEED
////	if (0 == m_iTestDisable)
////#endif
//	{
//		ICloneCreated* pCloneCreatedContext = safe_dynamic_cast<ICloneCreated*>(pBLContext);
//		if (NULL != pCloneCreatedContext)
//		{
//			AddCloneCreatedContext(pCloneCreatedContext, szClassName);
//			return pBLContext;
//		}
//	}

	const int iCAddInContextLen = strlen("Addin.");
	const LPCTSTR szCAddIn = "Addin.";

	// выполняю подмену объектов ВК, созданных по штатной технологии создания ВК, на свой прокси-объект
	// потери скорости быть не должно, т.к. этот код вызывается только один раз при создании объекта одного типа
	// далее используется InitObject - см. выше
	// нужно это для возможности нормального наследования собственных классов от подобных объектов
	if (!strnicmp(szClassName, szCAddIn, iCAddInContextLen))
		return CAddInContext_MyProxy::CreateWithNewObject(szClassName, static_cast<CAddInContext*>(pBLContext));

	if (!iIsOleContext)
		RegisterCBLContext(pBLContext->GetRuntimeClass(), szClassName, FALSE);

	return pBLContext;
}

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//
//void CCreateInstanceBoost::TestDisableFastCreationAllContexts(int param)
//{
//	m_pImpl->TestDisableFastCreationAllContexts(param);
//}
//
//void CCreateInstanceBoost::CCreateInstanceBoostImpl::TestDisableFastCreationAllContexts(int param)
//{
//	m_iTestDisable = param;
//	m_pClassNameHash->RemoveKey("ИндексированнаяТаблица");
//	m_pClassNameHash->RemoveKey("Структура");
//}
//
//#endif