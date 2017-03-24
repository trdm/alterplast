// AddIn.cpp

#include "stdafx.h"
#include "mycontextbase.h"
#include "AddIn.h"
#include "ExecuteModule.h"
#include "WorkAsRegister.h"
#include "EventManager.h"
#include "ComponentClass.h"
#include "HashMap.h"
#include "Struct.h"
#include "DynaValue.h"
#include "DynaValueList.h"
#include "MySQL_Query.h"
#include "CreateMetaInfoClasses.h"

#include ".\\TurboBL\\gcboost.h"

#include ".\\SQL\\MetaDataWork.h"
#include ".\\ODBC\\1CPP_ODBCDatabase.h"
#include ".\\ODBC\\1CPP_ODBCRecordSet.h"
#include ".\\ODBC\\SQLLock.h"
#include "BinaryData.h"
#include "GUID.h"

#include "TableEx.h"
#include ".\\System\\Settings.h"
#include "System\DllHooker.h"
#include "System\Hooker.h"

#include ".\\System\\MyAddInContext.h"
#include "RuntimeException.h"
#include "MetaDataOfClasses.h"

#include "System\WrapAboutDlg.h"
#include "System\CreationBoost.h"

#include "DynaCrypt.h"
#include "dml.h"

//+ kms
#include "System/compare.h"

// быстрые строчные компараторы
class CCompare FastCompare;
class CCompare FastCompareNoCase(0xFFFFFF00);
//- kms

//////////////////////////////////////////////////////////////////////////
// class CAddInPropertyCollection
//////////////////////////////////////////////////////////////////////////
// массив свойств-настроек плагина, к которым обращаются по имени
// класс по работе с настройками ВК, 
// упрощается работа со множеством настроек + единая схема обращения, загрузки и сохранения.

typedef void (*CVoidFuncPtr)(void);
class CAddInPropertyCollection
{
private:
	struct CPropertyFields
	{
		BOOL defaultValue;
		BOOL trueValue;
		BOOL falseValue;
		BOOL* pRealVariable;
		typedef boost::shared_ptr<CPropertyFields> pointer;
	};

	struct CPropertiesFunc
	{
		CVoidFuncPtr pInit;
		CVoidFuncPtr pDestroy;
		typedef boost::shared_ptr<CPropertiesFunc> pointer;
	};

	CIStringMap<BOOL, BOOL> m_PropertyMap;
	CIStringMap<CPropertyFields::pointer, CPropertyFields::pointer const&> m_PropertyFieldsMap;
	CMap<int, int, CString, CString const&> m_PropertyKeyMapOnDialogID;

	// карта наименований (русских и английских) для удобного использования
	CMapStringToString m_PropertyNameMap;

	// карта функций, которые вызываются при установке значения
	CIStringMap<CPropertiesFunc::pointer, CPropertiesFunc::pointer const&> m_PropertyFuncMap;
	boost::shared_ptr<IPropertyProvider> m_pProfile;

	void LoadOne(LPCSTR key, BOOL defaultValue, BOOL trueValue);
	BOOL SaveOne(LPCSTR key, BOOL trueValue, BOOL falseValue);
	void FireFunctor(CPropertiesFunc::pointer const& ptrFunc);
public: 
	DECLARE_PROPERTY_ARRAY(Item, BOOL, LPCSTR)

	CAddInPropertyCollection(){}
	~CAddInPropertyCollection(){}

	void SetProvider(IPropertyProvider *pProfile) { m_pProfile.reset(pProfile); }

	void Add(LPCSTR key, int DialogID, BOOL defaultValue, BOOL trueValue, BOOL falseValue, BOOL* pRealVariable = NULL);
	void AddAlias(LPCSTR sRealName, LPCSTR sAddName);
	void AddFunctor(LPCSTR key, CVoidFuncPtr pInit1, CVoidFuncPtr pDestroy1);

	void FireAllFunctors(void);
	void LoadAll();
	void SaveAll();

	CString GetKeyByDialogID(int dialogID);
	CString GetInfo() const;
};

void CAddInPropertyCollection::FireFunctor(CPropertiesFunc::pointer const& ptrFunc)
{
	if (!ptrFunc)
		return;

	CVoidFuncPtr ptr = ptrFunc->pInit;
	if (ptr)
		(*ptr)();

	// чтобы исключить возможность повторного запуска
	// т.к. в некоторых случаях при повторных перехватах 1С может падать
	// TODO добавить в класс возможность выбора такой фичи 
	ptrFunc->pInit = NULL;
}

BOOL CAddInPropertyCollection::get_Item(LPCSTR key)
{
	BOOL value = FALSE;
	CString sRealKey;
	if (!m_PropertyNameMap.Lookup(key, sRealKey) || !m_PropertyMap.Lookup(sRealKey, value))
		RuntimeError("Не существует свойства с именем %s", key);
	return value;
}

void CAddInPropertyCollection::put_Item(LPCSTR key, BOOL newValue)
{
	BOOL value;
	CString sRealKey;

	if (m_PropertyNameMap.Lookup(key, sRealKey))
		if (m_PropertyMap.Lookup(sRealKey, value))
		{
			BOOL oldValue = value;
			m_PropertyMap[sRealKey] = newValue;

			CPropertyFields::pointer fields = m_PropertyFieldsMap[sRealKey];
			*(fields->pRealVariable) = newValue;
			if (newValue != oldValue)
			{
				SaveOne(sRealKey, fields->trueValue, fields->falseValue);

				CPropertiesFunc::pointer ptrFunc = m_PropertyFuncMap[sRealKey];
				if(ptrFunc)
					if (newValue)
					{
						FireFunctor(ptrFunc);
					}
					else 
					{
						CVoidFuncPtr ptr = ptrFunc->pDestroy;
						if (ptr)
							(*ptr)();
					}
			}
			return;
		};
	RuntimeError("Не существует свойства с именем %s", key);
}

CString CAddInPropertyCollection::GetKeyByDialogID(int dialogID)
{
	return m_PropertyKeyMapOnDialogID[dialogID];
}

void CAddInPropertyCollection::Add(LPCSTR key, int DialogID, BOOL defaultValue, BOOL trueValue, BOOL falseValue, BOOL* pRealVariable)
{
	m_PropertyKeyMapOnDialogID.SetAt(DialogID, key);

	m_PropertyMap[key] = defaultValue;

	CPropertyFields::pointer fields(new CPropertyFields);
	fields->defaultValue = defaultValue;
	fields->trueValue = trueValue;
	fields->falseValue = falseValue;

	if (pRealVariable) // удобно использовать, если есть глобальная переменная или переменная, которая хранится в другом классе
		fields->pRealVariable = pRealVariable;
	else
		fields->pRealVariable = &(m_PropertyMap[key]);

	m_PropertyFieldsMap[key] = fields;

	AddAlias(key, key); // добавляю символическое имя
}

void CAddInPropertyCollection::AddAlias(LPCSTR sRealName, LPCSTR sAddName)
{
	m_PropertyNameMap[sAddName] = sRealName;
}

void CAddInPropertyCollection::LoadOne(LPCSTR key, BOOL defaultValue, BOOL trueValue)
{
	if (m_pProfile)
	{
		int nVal = m_pProfile->ReadInt(key, defaultValue);
		Item[key] = nVal == trueValue ? TRUE : FALSE;
	}
}

void CAddInPropertyCollection::LoadAll()
{
	POSITION pos;
	CString key;
	BOOL val;
	for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
	{
		m_PropertyMap.GetNextAssoc( pos, key, val );
		CPropertyFields::pointer fields = m_PropertyFieldsMap[key];
		LoadOne(key, fields->defaultValue, fields->trueValue);
	}
}

BOOL CAddInPropertyCollection::SaveOne(LPCSTR key, BOOL trueValue, BOOL falseValue)
{
	BOOL bResult = FALSE;
	if (m_pProfile)
		bResult = m_pProfile->WriteInt(key, Item[key] ? trueValue : falseValue);
	return bResult;
}

void CAddInPropertyCollection::SaveAll()
{
	POSITION pos;
	CString key;
	BOOL val;
	for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
	{
		m_PropertyMap.GetNextAssoc( pos, key, val );
		CPropertyFields::pointer fields = m_PropertyFieldsMap[key];
		SaveOne(key, fields->trueValue, fields->falseValue);
	}
}

void CAddInPropertyCollection::AddFunctor(LPCSTR key, CVoidFuncPtr pInit1, CVoidFuncPtr pDestroy1)
{
	CPropertiesFunc::pointer ptr(new CPropertiesFunc);
	ptr->pInit = pInit1;
	ptr->pDestroy = pDestroy1;
	m_PropertyFuncMap[key] = ptr;
}

// запустить ассоциированные функции
void CAddInPropertyCollection::FireAllFunctors(void)
{
	POSITION pos;
	CString key;
	BOOL value;
	for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
	{
		m_PropertyMap.GetNextAssoc(pos, key, value);
		if (value)
		{
			CPropertiesFunc::pointer ptrFunc;
			if (m_PropertyFuncMap.Lookup(key, ptrFunc))
				FireFunctor(ptrFunc);
		}
	}
}

CString CAddInPropertyCollection::GetInfo(void) const
{
	CString text;

	POSITION pos;
	LPCSTR key;
	BOOL val;
	for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
	{
		m_PropertyMap.GetNextAssoc(pos, key, val);
		CString tmp;
		tmp.Format("%s: %i\r\n", key, val);
		text += tmp;
	}
	return text;
}

//////////////////////////////////////////////////////////////////////////
// CAddIn
//////////////////////////////////////////////////////////////////////////

CAddIn::CAddIn()
{     
	m_pPropertyCollection.reset(new CAddInPropertyCollection);
}

CAddIn::~CAddIn() 
{
}

BOOL CAddIn::get_Property(LPCSTR key)
{
	return m_pPropertyCollection->Item[key];
}

void CAddIn::put_Property(LPCSTR key, BOOL newValue)
{
	m_pPropertyCollection->Item[key] = newValue;
}

// только настраивает свойства, из реестра ничего не читается !!
void CAddIn::InitProperties()
{
	// main
	m_pPropertyCollection->Add("EnableCheckType", 0, 0, 1, 0, &(this->m_EnabledCheckType));
	m_pPropertyCollection->Add("EnableOptimization", 0, 1, 1, 0, &(this->m_EnableOptimizate));
	m_pPropertyCollection->Add("EnableDebugging", 0, 0, 1, 0, &(this->m_EnableDebugging));
	m_pPropertyCollection->Add("EnableTableSourceTable", 0, 1, 1, 0);
	m_pPropertyCollection->Add("EnableTurboBL", 0, 1, 1, 0);
	m_pPropertyCollection->Add("EnableHookEventsGroupContext", 0, 0, 1, 0);

	// useful
	m_pPropertyCollection->AddAlias("EnableCheckType", "Проверка типов");
	m_pPropertyCollection->AddAlias("EnableOptimization", "Оптимизация");
	m_pPropertyCollection->AddAlias("EnableDebugging", "Отладка");
	m_pPropertyCollection->AddAlias("EnableTableSourceTable", "ТаблицаИсходнаяТаблица");
	m_pPropertyCollection->AddAlias("EnableTurboBL", "TurboBL");
	m_pPropertyCollection->AddAlias("EnableTurboBL", "ТурбоБЛ");
	m_pPropertyCollection->AddAlias("EnableHookEventsGroupContext", "ПерехватитьСобытияГК");

	// obsolete
	m_pPropertyCollection->AddAlias("EnableCheckType", "EnabledCheckType");
	m_pPropertyCollection->AddAlias("EnableOptimization", "EnableOptimizate");
	m_pPropertyCollection->AddAlias("EnableDebugging", "Отладчик");
	m_pPropertyCollection->AddAlias("EnableTableSourceTable", "Таблица.ИсходнаяТаблица");

	m_pPropertyCollection->AddFunctor("EnableTurboBL", ::InitAllBoost, NULL);
	m_pPropertyCollection->AddFunctor("EnableTableSourceTable", (CVoidFuncPtr)CTableWrapEx::InitTableWrap, NULL);
	m_pPropertyCollection->AddFunctor("EnableHookEventsGroupContext", (CVoidFuncPtr)CHookGroupContext::InitHooks, NULL);
}

void CAddIn::LoadProperties(IPropertyProvider* pProvider)
{
	m_pPropertyCollection->SetProvider(pProvider);
	m_pPropertyCollection->LoadAll();
	m_pPropertyCollection->FireAllFunctors();
}

void CAddIn::SaveProperties()
{
	m_pPropertyCollection->SaveAll();
}

CString CAddIn::GetPropertyInfo() const
{
	return m_pPropertyCollection->GetInfo();
}

STDMETHODIMP CAddIn::Init()
{
	pBkEndUI = GetBkEndUI();
	pDataBase7			= CDataBase7::GetDefault();
	pMetaDataCont		= pDataBase7->GetMD();

	InitProperties();
	IPropertyProvider* pProfile = new CRegistryPropertyProvider();
	LoadProperties(pProfile);

	CCreateInstanceBoost::TheObject().Init();
	CSafeModulePtr::InitHook();

	RegisterAllContextClasses();

	ISetOfHierarchy *pHierarchy = ISetOfHierarchy::GetHierarchy();
	ISetOfHierarchy::GetHierarchy()->Init();

	CRuntimeExceptionClass::Init();
	CWrapAboutDlg::Init();

	return S_OK;
}

STDMETHODIMP CAddIn::Done() throw()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SaveProperties();

	CCreateInstanceBoost::TheObject().Destroy();

	CSafeModulePtr::DestroyHook();
	CHookGroupContext::DestroyHooks(); 

	RemoveAllOfRegisterContextClasses();

	CComponentClass::ClearOptimizationData();
	ISetOfHierarchy::DeInitHierarchy();

	CRuntimeExceptionClass::Destroy();
 	CGCBoost::Done();

	return S_OK;
}

void CAddIn::SafeRegisterContextClass(CRuntimeClass* pRuntimeClass , LPCSTR szContextName)
{
	CBLContext::RegisterContextClass(pRuntimeClass, szContextName, CType(100));
	m_vRegisteredRTC.push_back(pRuntimeClass);
}

void CAddIn::RegisterAllContextClasses()
{
	SafeRegisterContextClass(RUNTIME_CLASS(CExecuteModule) ,"ExecuteModule");
	SafeRegisterContextClass(RUNTIME_CLASS(CExecuteModule) ,"ВыполняемыйМодуль");

	SafeRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister) ,"WorkAsRegisterWin");
	SafeRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister) ,"РаботаСРегистромWin");
	SafeRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister) ,"RegistryWin");
	SafeRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister) ,"РеестрWin");

	SafeRegisterContextClass(RUNTIME_CLASS(CComponentClass) ,"CComponentClass");
	
	SafeRegisterContextClass(RUNTIME_CLASS(CHashMap) ,"HashMap");
	SafeRegisterContextClass(RUNTIME_CLASS(CStruct) ,"Struct");
	SafeRegisterContextClass(RUNTIME_CLASS(CStruct) ,"Структура");

	SafeRegisterContextClass(RUNTIME_CLASS(CDynaValueList) ,"DynaValueList");
	SafeRegisterContextClass(RUNTIME_CLASS(CDynaValue) ,"DynaValue");
	SafeRegisterContextClass(RUNTIME_CLASS(CDynaCrypt) ,"DynaCrypt");

	SafeRegisterContextClass(RUNTIME_CLASS(CMySQL_Query) ,"SQL_Query");
	SafeRegisterContextClass(RUNTIME_CLASS(CCreateMetaInfoClasses) ,"MetaInfoClasses");
	SafeRegisterContextClass(RUNTIME_CLASS(CMetaDataWork) ,"MetaDataWork");
	SafeRegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCRecordSet) ,"ODBCRecordSet");
	SafeRegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCDatabase) ,"ODBCDatabase");

	SafeRegisterContextClass(RUNTIME_CLASS(CSQLLock) ,"SQLLock");
	SafeRegisterContextClass(RUNTIME_CLASS(CBinaryData) ,"BinaryData");
	SafeRegisterContextClass(RUNTIME_CLASS(CGUID) ,"GUID");

	SafeRegisterContextClass(RUNTIME_CLASS(CAddInContext_MyProxy) ,"CAddInContext_MyProxy");
	SafeRegisterContextClass(RUNTIME_CLASS(CInterceptor) ,"Hooker");

	CMyContextBase::InitAllContextClasses();
}

void CAddIn::RemoveAllOfRegisterContextClasses()
{
	std::vector<CRuntimeClass*>::const_iterator iter = m_vRegisteredRTC.begin();
	for (; iter != m_vRegisteredRTC.end(); iter++)
	{
		CRuntimeClass* pRuntimeClass = *iter;
		CBLContext::UnRegisterContextClass(pRuntimeClass);
	}
	
	CMyContextBase::DoneAllContextClasses();
}
