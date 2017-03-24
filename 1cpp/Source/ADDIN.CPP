// AddIn.cpp

#include "stdafx.h"
#include "mycontextbase.h"
#include "AddIn.h"
#include "ExecuteModule.h"
#include "WorkAsRegister.h"
#include "TemplateClass.h"
//#include "EventManager.h"
#include "ComponentClass.h"
#include "HashMap.h"
//#include "Delegate.h"
#include "Struct.h"
#include "DynaValue.h"
#include "DynaValueList.h"
#include "MySQL_Query.h"
#include "CreateMetaInfoClasses.h"

//{OAS
#include ".\\TurboBL\\gcboost.h"
//#include "loader.h"
//OAS}

#include ".\\SQL\\MetaDataWork.h"
#include ".\\ODBC\\1CPP_ODBCDatabase.h"
#include ".\\ODBC\\1CPP_ODBCRecordSet.h"
#include ".\\ODBC\\SQLLock.h"
#include "BinaryData.h"
#include "GUID.h"
//#include "istrmap.h"
//#include "Factory.h"
// #include ".\\OLEDB\\sp_oledb.h"// Kirill
// #include ".\\OLEDB\\sp_cooledb.h"// Kirill

//#include "MethodsWrapper.h" // artbear
#include "TableEx.h" // artbear
#include ".\\System\\Settings.h" // artbear
//#include ".\\System\\GroupContextWrap.h"
#include "System\DllHooker.h"

#include ".\\System\\MyAddInContext.h"

#include "System\WrapAboutDlg.h"

// This global interface is for timer implementation only

class CDataBase7 * pDataBase7 = NULL;
class CMetaDataCont * pMetaDataCont = NULL;
class CBkEndUI * pBkEndUI;
class CAddIn * pMainAddIn;
//CRuntimeClass* pCBLContextRuntimeClass;

IDispatch *pGlobalConnection = NULL;

//DMS:BEGIN
char buf[4096];
char **pst = 0;
//DMS:END

//+ kms
#include "System/compare.h"

// быстрые строчные компараторы
class CCompare FastCompare;
class CCompare FastCompareNoCase(0xFFFFFF00);
//- kms

/////////////////////////////////////////////////////////////////////////////
// CAddIn

BOOL CAddIn::LoadProperties()
{
	m_EnableOptimizate = TRUE; // по умолчанию оптимизация включена

	m_PropertyCollection.Init(m_iProfile);
	m_PropertyCollection.Load();

	return TRUE;
}

void CAddIn::SaveProperties()
{
	m_PropertyCollection.Save();
}

CCreateInstanceBoost& CCreateInstanceBoost::TheObject()
{
	static CCreateInstanceBoost object;
	return object;
}
static LPCSTR szFastCreationValueList =  "{\"VL\",{}}";
static LPCSTR szFastCreationTableList =  "{\"VT\",\"1\",{\"0\",{{\"\",\"0\",\"0\",\"0\",\"\",\"2\"}}}}";

inline void CCreateInstanceBoost::Init(void)
{
    m_pClassNameHash = sh_ptr<CIStringMapToInfo>(new CIStringMapToInfo());//m_pClassNameHash = new CIStringMapToInfo();

	m_wrapperCreateInstance.DoWrap(::GetModuleHandle("bkend.dll"), "?CreateInstance@CBLContext@@SAPAV1@PBD@Z",
		(PF_CreateInstance)CCreateInstanceBoost::CreateInstanceBoosted);

	// быстрое создание объектов
	AddFastCreationObject("СписокЗначений", szFastCreationValueList);
	AddFastCreationObject("ТаблицаЗначений", szFastCreationTableList);
};

inline void CCreateInstanceBoost::Destroy(void)
{
	if (m_pClassNameHash.isValid())
	{
		//delete m_pClassNameHash;
		m_wrapperCreateInstance.RestoreOrigAddress();
	}
}

void CCreateInstanceBoost::RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration /*= TRUE*/)
{
    if (m_pClassNameHash->GetCount() * 1.2 > m_pClassNameHash->GetHashTableSize())
    {
        //CIStringMapToOb* pNewHash = new CIStringMapToOb();
        sh_ptr<CIStringMapToInfo> pNewHash(new CIStringMapToInfo()); //CIStringMapToInfo* pNewHash = new CIStringMapToInfo();
        pNewHash->InitHashTable((m_pClassNameHash->GetCount() + 11) * 1.2);
        POSITION pos = m_pClassNameHash->GetStartPosition();
        CString strCurName;
		CInfoPtr pObject;
        while (pos)
        {
            m_pClassNameHash->GetNextAssoc(pos, strCurName, pObject);
            pNewHash->operator[](strCurName) = pObject;
        }
        //delete m_pClassNameHash;
        m_pClassNameHash = pNewHash;

    }
    if (bNativeRegistration)
        CBLContext::RegisterContextClass(pRuntimeClass, szClassName, CType(100));
	CInfoPtr pInfo = new CInfo(pRuntimeClass);
    m_pClassNameHash->operator[](szClassName) = pInfo;
}

// для быстрого создания системных объектов
void CCreateInstanceBoost::AddFastCreationObject(LPCSTR szClassName, LPCSTR szClassFastCreationString)
{
	CInfoPtr pInfo = new CInfo(szClassFastCreationString);
	m_pClassNameHash->operator[](szClassName) = pInfo;
}

// без ускорения
CBLContext* CCreateInstanceBoost::CreateInstanceWithoutBoosted(char const * szClassName)
{
	CDllMethodWrapperGuard<PF_CreateInstance> wrapper(TheObject().m_wrapperCreateInstance);

	return CBLContext::CreateInstance(szClassName);
}

// перехват создания объекта - т.е. когда пользователь пишет СоздатьОбъект(ТипОбъекта)
// здесь самое начало ООП - возможность создавать собственные типы !
//	также выполнено ускорение создания любых объектов в 1С
//
CBLContext* CCreateInstanceBoost::CreateInstanceBoosted(char const * szClassName)
{
	sh_ptr<CIStringMapToInfo>& pClassNameHash = TheObject().m_pClassNameHash; //CIStringMapToInfo*& pClassNameHash = TheObject().m_pClassNameHash;

    CBLContext* pBLContext;
	CInfoPtr pInfo;

    if (pClassNameHash->Lookup(szClassName, pInfo))
		if (pInfo->IsBLContext) // ускоренное создание объектов
		{
			pBLContext = (CBLContext*)pInfo->pRuntimeClass->CreateObject();
			pBLContext->InitObject(szClassName);
		}
		else // быстрое создание системных объектов
		{
			LoadValueFromString(&TheObject().m_ValueForFastCreation, pInfo->szFastCreationString);
			pBLContext = TheObject().m_ValueForFastCreation.GetContext();

			// чтобы контекст не был удален после сброса на след.строке
			if (pBLContext)
				pBLContext->IncrRef();

			TheObject().m_ValueForFastCreation.Reset(); // сбросить временный объект

		}

    else
    {
		pBLContext = CreateInstanceWithoutBoosted(szClassName);

        if (pBLContext)
        {
 			const int iCAddInContextLen = strlen("Addin.");
			const LPCTSTR szCAddIn = "Addin.";

			// выполняю подмену объектов ВК, созданных по штатной технологии создания ВК, на свой прокси-объект
			// потери скорости быть не должно, т.к. этот код вызывается только один раз при создании объекта одного типа
			// далее используется InitObejct - см. выше
			// нужно это для возможности нормального наследования собственных классов от подобных объектов
 			if (!strnicmp(szClassName, szCAddIn, iCAddInContextLen))
				pBLContext = CAddInContext_MyProxy::CreateWithNewObject(szClassName, static_cast<CAddInContext*>(pBLContext));
			else
				if (!pBLContext->IsOleContext())
					TheObject().RegisterCBLContext(pBLContext->GetRuntimeClass(), szClassName, FALSE);
        }
    }
    return pBLContext;
}

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration)
{
	CCreateInstanceBoost::TheObject().RegisterCBLContext(pRuntimeClass, szClassName, bNativeRegistration);
}

// счетчик числа инициализаций компоненты
int CAddIn::m_nInitDoneCount = 0;

STDMETHODIMP CAddIn::Init(IDispatch *pConnection)
{
	// предотвращение повторной загрузки
	if (m_nInitDoneCount++)
		return(E_FAIL);

	pBkEndUI = GetBkEndUI();

	pConnection->QueryInterface(IID_IErrorLog,(void **)&m_iErrorLog);
	pConnection->QueryInterface(IID_IAsyncEvent,(void **)&m_iAsyncEvent);
	pConnection->QueryInterface(IID_IStatusLine,(void **)&m_iStatusLine);
	pConnection->QueryInterface(IID_IPropertyProfile,(void **)&m_iProfile);
	pConnection->QueryInterface(IID_IExtWndsSupport,(void **)&m_iExtWndsSupport);

	InitProperties(); // настройка свойств

	if (m_iProfile)
	{
		bstr_t strProfile("1C++ Components & Class\\");
		strProfile += bstr_t(((CApp7*)AfxGetApp())->GetProps()->GetStringProp(0x12));
		HRESULT hr = m_iProfile->RegisterProfileAs(strProfile);

		if (!LoadProperties())
			return E_FAIL;
	}

	pGlobalConnection = pConnection;

	pDataBase7			= CDataBase7::GetDefault();
	pMetaDataCont		= pDataBase7->GetMD();

	CCreateInstanceBoost::TheObject().Init();
	CSafeModulePtr::InitHook();

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CExecuteModule) ,"ExecuteModule",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CExecuteModule) ,"ВыполняемыйМодуль",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CWorkAsRegister), "WorkAsRegisterWin", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CWorkAsRegister), "РаботаСРегистромWin", CType(100));

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CWorkAsRegister), "RegistryWin", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CWorkAsRegister), "РеестрWin", CType(100));

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CComponentClass), "CComponentClass", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CHashMap), "HashMap", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CStruct), "Struct", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CStruct), "Структура", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CDynaValueList), "DynaValueList", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CDynaValue), "DynaValue", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CDynaCrypt), "DynaCrypt", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CMySQL_Query), "SQL_Query", CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CCreateMetaInfoClasses), "MetaInfoClasses", CType(100));

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CMetaDataWork) ,"MetaDataWork",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCRecordSet) ,"ODBCRecordSet",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCDatabase) ,"ODBCDatabase",CType(100));

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CSQLLock) ,"SQLLock",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CBinaryData) ,"BinaryData",CType(100));
	CBLContext::RegisterContextClass(RUNTIME_CLASS(CGUID) ,"GUID",CType(100));

	CBLContext::RegisterContextClass(RUNTIME_CLASS(CAddInContext_MyProxy) ,"CAddInContext_MyProxy", CType(100));

//	CBLContext::RegisterContextClass(RUNTIME_CLASS(COLEDBData), "OLEDBData", CType(100));// Kirill

//	CBLContext::RegisterContextClass(RUNTIME_CLASS(CTableEx), "ТаблицаРасширенная", CType(100));// artbear

    CMyContextBase::InitAllContextClasses();

/*	данные методы вызываются через свойства ВК - artbear
	InitAllBoost();
	//OAS}
	CHookGroupContext::InitHooks();
//	if(!InitTableWrap.Init()) return E_FAIL; */
	m_PropertyCollection.SetupPropertyFunction(); // вызываю все ассоциированные со свойствами функции

	hLibBasic = GetModuleHandle("Basic.dll");
	if (hLibBasic == NULL)
		pBkEndUI->DoMessageLine("Неудачная попытка получить хендл модуля Basic.dll",mmInformation);

	CSetOfHierarchy *pHierarchy = CSetOfHierarchy::GetHierarchy();

	pHierarchy->Init();
	pHierarchy->ProcessDefines();

	pHierarchy->RegistersContextClasses();

	pMainAddIn = this;

	CRuntimeExceptionClass::Init();

	CWrapAboutDlg::Init();

	return S_OK;
}

STDMETHODIMP CAddIn::Done()
{
	// предотвращение повторной загрузки
	if (--m_nInitDoneCount)
		return S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SaveProperties();

	if (m_iStatusLine) m_iStatusLine->Release();
	if (m_iProfile) m_iProfile->Release();
	if (m_iAsyncEvent) m_iAsyncEvent->Release();
	if (m_iErrorLog) m_iErrorLog->Release();
	if (m_iExtWndsSupport) m_iExtWndsSupport->Release();

	CCreateInstanceBoost::TheObject().Destroy();

	CSafeModulePtr::DestroyHook();
	CHookGroupContext::DestroyHooks();

	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CExecuteModule));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CExecuteModule));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CWorkAsRegister));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CComponentClass));
// 	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CEventManager));
// 	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CEventManager));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CStruct));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CStruct));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CDynaValueList));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CDynaValue));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CDynaCrypt));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CMySQL_Query));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CCreateMetaInfoClasses));

	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CMetaDataWork));

	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCRecordSet));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(C1CPP_ODBCDatabase));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CSQLLock));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CBinaryData));
	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CGUID));

//	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(COLEDBData));// Kirill

//	CBLContext::UnRegisterContextClass(RUNTIME_CLASS(CTableEx));// artbear

	if (hLibBasic != NULL)
		FreeLibrary(hLibBasic);

	CSetOfHierarchy::DeInitHierarchy();

	CRuntimeExceptionClass::Destroy();

	CComponentClass::ClearOptimizationData();

	pMainAddIn = NULL;

	return S_OK;
}

STDMETHODIMP CAddIn::GetInfo(SAFEARRAY **pInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

		// Component should put supported component technology version
		// in VARIANT at index 0
		long lInd = 0;
    VARIANT varVersion;
    V_VT(&varVersion) = VT_I4;
	// This component supports 1.0 version
    V_I4(&varVersion) = 2000;
    SafeArrayPutElement(*pInfo,&lInd,&varVersion);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// ILanguageExtender

STDMETHODIMP CAddIn::RegisterExtensionAs(BSTR *bstrExtensionName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

		// You may delete next lines and add your own implementation code here

		// Name of extension should be changed avoiding conflicts
		CString csExtenderName = "1C++";
    *bstrExtensionName = csExtenderName.AllocSysString();

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// IPropertyPage

/*STDMETHODIMP CAddIn::SetPageSite(IPropertyPageSite *pPageSite)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState())
m_pPageSite = pPageSite;
// TODO: Add your implementation code here

  return S_OK;
}*/

/*STDMETHODIMP CAddIn::Activate(HWND hWndParent,LPCRECT prc,BOOL bModal)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState())

  // TODO: Add your implementation code here

    return IPropertyPageImpl<CAddIn>::Activate(hWndParent,prc,bModal);
}*/

STDMETHODIMP CAddIn::Deactivate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

		IPropertyPageImpl<CAddIn>::Deactivate();

    // Don't forget to detach controls!
	m_boxEnabledCheckType.Detach();
	m_boxOptimizate.Detach();
	m_boxEnableDebugging.Detach();
	m_boxEnabledColor.Detach();
	m_boxLocalColor.Detach();
	m_boxShowKL.Detach();

	return S_OK;
}

/*STDMETHODIMP CAddIn::GetPageInfo(PROPPAGEINFO *pPageInfo)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState())

  // TODO: Add your implementation code here

	return S_OK;
	}

	  STDMETHODIMP CAddIn::SetObjects(ULONG cObjects,IUnknown **ppUnk)
	  {
	  AFX_MANAGE_STATE(AfxGetStaticModuleState())

		// TODO: Add your implementation code here

		  return IPropertyPageImpl<CAddIn>::SetObjects(cObjects,ppUnk);
		  }

			/*STDMETHODIMP CAddIn::Show(UINT nCmdShow)
			{
			AFX_MANAGE_STATE(AfxGetStaticModuleState())

			  // TODO: Add your implementation code here

				return S_OK;
				}

				  STDMETHODIMP CAddIn::Move(LPCRECT prc)
				  {
				  AFX_MANAGE_STATE(AfxGetStaticModuleState())

					// TODO: Add your implementation code here

					  return S_OK;
					  }

						STDMETHODIMP CAddIn::IsPageDirty()
						{
						AFX_MANAGE_STATE(AfxGetStaticModuleState())

						  // TODO: Add your implementation code here

							return S_OK;
}*/

STDMETHODIMP CAddIn::Apply()
{
	{  // без фигурных скобок 1С-как падает почему-то
		AFX_MANAGE_STATE(AfxGetStaticModuleState())
		BOOL boolEnabled;
		IPropertyLink *pLink = NULL;
		m_ppUnk[0]->QueryInterface(IID_IPropertyLink,(LPVOID *)&pLink);
		if (pLink != NULL)
		{
			boolEnabled = m_boxEnabledCheckType.GetCheck()?TRUE:FALSE;
			pLink->put_Enabled((unsigned char *)&boolEnabled, IDC_CHECK_TYPE);
			boolEnabled = m_boxOptimizate.GetCheck()?TRUE:FALSE;
			pLink->put_Enabled((unsigned char *)&boolEnabled, IDC_OPTIMIZATE);

			boolEnabled = m_boxEnableDebugging.GetCheck()?TRUE:FALSE;
			pLink->put_Enabled((unsigned char *)&boolEnabled, IDC_ENABLE_DEBUGGING);

			pLink->Release();
		}
		SetDirty(FALSE);
	}

	return S_OK;
}

/*STDMETHODIMP CAddIn::Help(LPCOLESTR pszHelpDir)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState())

  // TODO: Add your implementation code here

	return S_OK;
	}

	  STDMETHODIMP CAddIn::TranslateAccelerator(LPMSG pMsg)
	  {
	  AFX_MANAGE_STATE(AfxGetStaticModuleState())

		// TODO: Add your implementation code here

		  return S_OK;
}*/


LRESULT CAddIn::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		IPropertyLink *pLink = NULL;
	m_ppUnk[0]->QueryInterface(IID_IPropertyLink,(LPVOID *)&pLink);
	m_boxEnabledCheckType.Attach(GetDlgItem(IDC_CHECK_TYPE));
	m_boxOptimizate.Attach(GetDlgItem(IDC_OPTIMIZATE));
	m_boxEnableDebugging.Attach(GetDlgItem(IDC_ENABLE_DEBUGGING));

	if (pLink != NULL)
	{
		pLink->get_Enabled((unsigned char *)&m_EnabledCheckType, IDC_CHECK_TYPE);
		pLink->get_Enabled((unsigned char *)&m_EnableOptimizate, IDC_OPTIMIZATE);
		pLink->get_Enabled((unsigned char *)&m_EnableDebugging, IDC_ENABLE_DEBUGGING);
		pLink->Release();

	}


	m_boxEnabledCheckType.SetCheck(m_EnabledCheckType?1:0);
	m_boxOptimizate.SetCheck(m_EnableOptimizate?1:0);
	m_boxEnableDebugging.SetCheck(m_EnableDebugging?1:0);

	m_boxEnabledCheckType.EnableWindow(m_AccessCheckType);
	m_boxOptimizate.EnableWindow(m_AccessOptimizate);
	m_boxEnableDebugging.EnableWindow(m_AccessEnableDebugging);


	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CString text;
	if (pHierarchy)
		text.Format("Всего зарегистрировано классов: %d\r\nВсего объявлено методов: %d\r\nОпределенные директивы препроцессора:\r\n%s\r\nВложенные файлы:\r\n%s\r\nИмена зарегистрированных классов:\r\n%s\r\nИмена псевдонимов и их пути:\r\n%s",pHierarchy->GetCountClasses(), pHierarchy->GetCountMethods(), pHierarchy->GetDefSymbolOfPreproc(),pHierarchy->GetLoadIncludeFiles(),pHierarchy->GetAllClassesNames(), pHierarchy->GetAllAliasAndPath());

	SetDlgItemText(IDC_EDIT1,text);

	SetDirty(FALSE);
	m_pPageSite->OnStatusChange(0);
	bHandled = FALSE;

	GetDlgItemText(IDC_ABOUT_STATIC, text.GetBufferSetLength(250), 250);
    text.ReleaseBuffer();
    CString sVersion = ::GetVersionField("FileVersion");
    sVersion.Replace(" ", "");
    sVersion.Replace(',', '.');
    CString sPrivateBuild = ::GetVersionField("PrivateBuild");
    if (!sPrivateBuild.IsEmpty())
    {
        sVersion += ' ';
        sVersion += sPrivateBuild;
    }
    CString sSpecialBuild = ::GetVersionField("SpecialBuild");
    if (!sSpecialBuild.IsEmpty())
    {
        sVersion += ' ';
        sVersion += sSpecialBuild;
    }
    text.Replace("_VersionInfo_", sVersion);
    SetDlgItemText(IDC_ABOUT_STATIC, text);

	char buffer[MAX_PATH];
	extern HINSTANCE hDllInstance;
	::GetModuleFileName(hDllInstance,buffer,MAX_PATH);
	SetDlgItemText(IDC_DLLPATH, buffer);

	return 0;
}

LRESULT CAddIn::OnCommand(WORD wNotifyCode, WORD nID, HWND hWndCtl, BOOL& bHandled)
{
	SetDirty(TRUE);

	/*switch(nID)
	{
    case IDC_OPTIMIZATE:
	{
	EnableWindow(IDC_ENABLE_DEBUGGING,m_EnableDebugging);
	}
	break;
	default:
	break;
}*/
	bHandled = FALSE;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// IPropertyLink

STDMETHODIMP CAddIn::get_Enabled(unsigned char* pData, int Id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString key = m_PropertyCollection.GetKeyByDialogID(Id);
	BOOL* boolEnabled = (BOOL*)pData;
	*boolEnabled = m_PropertyCollection.Item[key];

	return S_OK;
}

STDMETHODIMP CAddIn::put_Enabled(unsigned char* pData, int Id)
{
	{  // без фигурных скобок 1С-как падает почему-то
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString key = m_PropertyCollection.GetKeyByDialogID(Id);
	BOOL* boolEnabled = (BOOL*)pData;
	m_PropertyCollection.Item[key] = *boolEnabled;

	SaveProperties(); //TODO
	return S_OK;
	}
}


STDMETHODIMP CAddIn::CallAsFunc(long,struct tagVARIANT *,struct tagSAFEARRAY * *)
{
	return S_OK;
}

STDMETHODIMP CAddIn::CallAsProc(long,struct tagSAFEARRAY * *)
{
	return S_OK;
}

STDMETHODIMP CAddIn::HasRetVal(long,int * ipRes)
{
	*ipRes	= 0;

	return S_OK;
}

STDMETHODIMP CAddIn::GetParamDefValue(long,long,struct tagVARIANT *)
{
	return S_OK;
}

STDMETHODIMP CAddIn::GetNParams(long,long * lpParams)
{
	*lpParams	= 0;
	return S_OK;
}

STDMETHODIMP CAddIn::GetMethodName(long,long,unsigned short * *)
{
	return S_OK;
}

STDMETHODIMP CAddIn::FindMethod(unsigned short *,long *)
{
	return S_OK;
}

STDMETHODIMP CAddIn::GetNMethods(long * lpRes)
{
	*lpRes = 0;
	return S_OK;
}

STDMETHODIMP CAddIn::IsPropWritable(long,int *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::IsPropReadable(long,int *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::SetPropVal(long,struct tagVARIANT *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::GetPropVal(long,struct tagVARIANT *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::GetPropName(long,long,unsigned short * *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::FindProp(unsigned short *,long *)
{
	return S_OK;
}
STDMETHODIMP CAddIn::GetNProps(long * lpNum)
{
	*lpNum = 0;
	return S_OK;
}

//custom funcs
int IsDMSContext(const char *name)
{
	return cFeatureList.FindName(name) ? 1 : 0;
}

BOOL CAddIn::get_Property(CString key)
{
	return m_PropertyCollection.Item[key];
}

void CAddIn::put_Property(CString key, BOOL newValue)
{
	m_PropertyCollection.Item[key] = newValue;
}

// только настраивает свойства, из реестра ничего не читается !!
void CAddIn::InitProperties()
{
	CString key; int dialogID;

	key = "EnabledCheckType";
    dialogID = IDC_CHECK_TYPE;
	m_PropertyCollection.Add(key, dialogID, 1, 2, 1, &(this->m_EnabledCheckType));

	key = "EnableOptimizate";
    dialogID = IDC_OPTIMIZATE;
	m_PropertyCollection.Add(key, dialogID, 2, 2, 1, &(this->m_EnableOptimizate));

	key = "EnableDebugging";
    dialogID = IDC_ENABLE_DEBUGGING;
	m_PropertyCollection.Add(key, dialogID, 1, 2, 1, &(this->m_EnableDebugging));

	key = "EnableTableSourceTable";
	m_PropertyCollection.Add(key, 0, 1, 1);

	key = "EnableTurboBL";
	m_PropertyCollection.Add(key, 0, 1, 1);

	key = "EnableHookEventsGroupContext";
	m_PropertyCollection.Add(key, 0, 0, 1);

	m_PropertyCollection.AddName("EnabledCheckType", "Проверка типов");
	m_PropertyCollection.AddName("EnableOptimizate", "Оптимизация");
	m_PropertyCollection.AddName("EnableDebugging", "Отладчик");
	m_PropertyCollection.AddName("EnableDebugging", "Отладка");

	m_PropertyCollection.AddName("EnableTableSourceTable", "ТаблицаИсходнаяТаблица");
	m_PropertyCollection.AddName("EnableTableSourceTable", "Таблица.ИсходнаяТаблица");

	m_PropertyCollection.AddName("EnableTurboBL", "TurboBL");
	m_PropertyCollection.AddName("EnableHookEventsGroupContext", "ПерехватитьСобытияГК");

	m_PropertyCollection.AddFunction("EnableTurboBL", ::InitAllBoost, NULL);
	m_PropertyCollection.AddFunction("EnableTableSourceTable", (CVoidFuncPtr)CTableWrapEx::InitTableWrap, NULL);
	m_PropertyCollection.AddFunction("EnableHookEventsGroupContext", (CVoidFuncPtr)CHookGroupContext::InitHooks, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// class CAddInPropertyCollection

BOOL CAddInPropertyCollection::get_Item(CString key)
{
	BOOL value;
	CString sRealKey;
	//if (!m_PropertyMap.Lookup(key, value))
	if (m_PropertyNameMap.Lookup(key, sRealKey))
		if (m_PropertyMap.Lookup(sRealKey, value))
			return value;
	RuntimeError("Не существует свойства с именем %s", key);

	return 0; // чтобы компилятор не ругался
}

void CAddInPropertyCollection::CallInitFunction(CPropertiesFunc* ptrFunc)
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

void CAddInPropertyCollection::put_Item(CString key, BOOL newValue)
{
	BOOL value;
	CString sRealKey;

	if (m_PropertyNameMap.Lookup(key, sRealKey))
		if (m_PropertyMap.Lookup(sRealKey, value))
		{
			BOOL oldValue = value; //m_PropertyMap[sRealKey];
			m_PropertyMap[sRealKey] = newValue;

			CPropertyFields* fields = m_PropertyFieldsMap[sRealKey];
			*(fields->pRealVariable) = newValue;
			if (m_bFlagEnableFunction)
				if (newValue != oldValue)
				{
					SaveOneProperty(sRealKey, fields->trueValue, fields->falseValue);

					CPropertiesFunc* ptrFunc = m_PropertyFuncMap[sRealKey];
					if(ptrFunc)
						if (newValue)
						{
							CallInitFunction(ptrFunc);
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

void CAddInPropertyCollection::Add(CString key, int DialogID, BOOL defaultValue, BOOL trueValue, BOOL falseValue, BOOL* pRealVariable)
{
	m_PropertyKeyMapOnDialogID.SetAt(DialogID, key);

	m_PropertyMap[key] = defaultValue;

	CPropertyFields* fields = new CPropertyFields; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
	fields->defaultValue = defaultValue;
	fields->trueValue = trueValue;
	fields->falseValue = falseValue;

	if (pRealVariable) // удобно использовать, если есть глобальная переменная или переменная, которая хранится в другом классе
		fields->pRealVariable = pRealVariable;
	else
		fields->pRealVariable = &(m_PropertyMap[key]);

	m_PropertyFieldsMap[key] = fields;

	AddName(key, key); // добавляю символическое имя
}

void CAddInPropertyCollection::AddName(CString sRealName, CString sAddName)
{
	m_PropertyNameMap[sAddName] = sRealName;
}

CAddInPropertyCollection::~CAddInPropertyCollection()
{
	// удалить CPropertyFields
    POSITION pos;
    CString key;
	CPropertyFields* fields;
    for (pos = m_PropertyFieldsMap.GetStartPosition(); pos != NULL;)
    {
		m_PropertyFieldsMap.GetNextAssoc( pos, key, fields );
		delete fields; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
    };

	// удалить CPropertiesFunc
	CPropertiesFunc* ptrFunc;
    for (pos = m_PropertyFuncMap.GetStartPosition(); pos != NULL;)
    {
		m_PropertyFuncMap.GetNextAssoc( pos, key, ptrFunc );
		delete ptrFunc; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
    }
}

bool CAddInPropertyCollection::LoadOneProperty(CString key, BOOL defaultValue, BOOL trueValue)
{
	variant_t var;
	V_VT(&var) = VT_I4;

	CWChar wStorName(key+":0");
	bool ret = S_OK==m_iProfile->Read(wStorName, &var, NULL);

	if (ret)
		Item[key] = V_I4(&var)==trueValue;
	else
		Item[key] = defaultValue == trueValue ? TRUE : FALSE;

	return ret;
}

void CAddInPropertyCollection::Load()
{

	if (!m_iProfile)
		return;

    POSITION pos;
    CString key;
    BOOL val;
    for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
    {
		m_PropertyMap.GetNextAssoc( pos, key, val );
		CPropertyFields* fields = m_PropertyFieldsMap[key];

		LoadOneProperty(key, fields->defaultValue, fields->trueValue);
    }

}

bool CAddInPropertyCollection::SaveOneProperty(CString key, BOOL trueValue, BOOL falseValue)
{
    variant_t var;
    V_VT(&var) = VT_I4;

	V_I4(&var) = Item[key] ? trueValue : falseValue;

	CWChar wStorName(key);
    m_iProfile->Write(wStorName, &var);

	return true;
}

void CAddInPropertyCollection::Save()
{
	if (!m_iProfile)
		return;

    POSITION pos;
    CString key;
    BOOL val;
    for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
    {
		m_PropertyMap.GetNextAssoc( pos, key, val );
		CPropertyFields* fields = m_PropertyFieldsMap[key];

		SaveOneProperty(key, fields->trueValue, fields->falseValue);
    }
}

void CAddInPropertyCollection::AddFunction(CString sName, CVoidFuncPtr pInit1, CVoidFuncPtr pDestroy1)
{
	CPropertiesFunc* ptr = new CPropertiesFunc; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
	ptr->pInit = pInit1;
	ptr->pDestroy = pDestroy1;
	m_PropertyFuncMap[sName] = ptr;
}

// запустить ассоциированные функции
void CAddInPropertyCollection::SetupPropertyFunction(void)
{
    POSITION pos;
    CString key;
	BOOL value;
    for (pos = m_PropertyMap.GetStartPosition(); pos != NULL;)
    {
		m_PropertyMap.GetNextAssoc( pos, key, value );
		if (value)
		{
			CPropertiesFunc* ptrFunc = NULL;
			if (m_PropertyFuncMap.Lookup(key, ptrFunc))
				CallInitFunction(ptrFunc);
		}
    }

	m_bFlagEnableFunction = true;
}