
#include "stdafx.h"
#include "..\\TurboBL\\gcboost.h"

#include "GroupContextWrap.h"
#include "DllHooker.h"

#include "..\\ExecuteModule.h"

static BOOL CallMethodOfContext(const CBLContext* pCont, bool bFuncFlag, int num,class CValue * pRetValue, class CValue * * ppValue)
{
	BOOL result = FALSE;
	CBLContext* pRunCont = const_cast<CBLContext*>(pCont);

	if (bFuncFlag){
		//return pRunCont->CallAsFunc(num, *pRetValue, ppValue);
		result = pRunCont->CallAsFunc(num, *pRetValue, ppValue);
		if (result)
			return result;

		pRetValue->Reset();
		return pRunCont->CallAsProc(num, ppValue);
	}
	else{
		//return pRunCont->CallAsProc(num, ppValue);
		BOOL result = pRunCont->CallAsProc(num, ppValue);
		if (result)
			return result;

		CValue LocalValue;
		return pRunCont->CallAsFunc(num, LocalValue, ppValue);
	}
	return result;
}

// =======================  ============================
//
//		class CEventsWrapperManager::CComponentClassWrapper
//
// =======================  ============================


// =======================  ============================
//
//		class CEventsWrapperManager::CModulesManager
//
// =======================  ============================

void CEventsWrapperManager::CModulesManager::Add(const CBLModule* pMod, CEventsWrapperManager::CConstComponentClassPtr pUserClassCont)
{
	stEventHandlerData* pModData = new stEventHandlerData(pUserClassCont);

	int nCount = m_ModulesMap.GetCount();
	m_ModulesMap.SetAt(pMod, pModData);
	nCount = m_ModulesMap.GetCount();
	// 	if (bIsHookOfGlobalModule)
	// 		LogErr("Установили перехват метода %s глобального модуля (%d)", "SetWrapper", pMod);
	// 	else
	// 		LogErr("Установили перехват метода %s pMod = %d", "SetWrapper", pMod);

	m_bHaveAddedModules = true;
}

bool CEventsWrapperManager::CModulesManager::FindEventHandlerDataWithoutVerifyUserWrapper(const CBLModule* pMod, stEventHandlerData*& pModData) const
{
	int nCount = m_ModulesMap.GetCount();
	if(!m_ModulesMap.Lookup(pMod, pModData))
	{
		//LogErr("Неудача Lookup, не нашли для CBLModule* %d", pMod);
		return false;
	}
	else
	{
		if (NULL == pModData)
		{
			//LogErr("Неудача Lookup, нашли, но вернули указатель stEventHandlerData* = NULL");
			return false;
		}

		//if (ppCont != NULL)
		{
			if (!pModData->GetHandler())
			{
				//LogErr("Неудача Lookup, нашли, но класс-перехватчик уже уничтожен");
				return false;
			}
			//*ppCont = pModData->GetHandler();
		}
		return true;
	}
	return false;
}

bool CEventsWrapperManager::CModulesManager::IsWrapperExists(const CBLModule* pMod) const
{
	stEventHandlerData* pModData = NULL;

	return FindEventHandlerDataWithoutVerifyUserWrapper(pMod, pModData);
}

bool CEventsWrapperManager::CModulesManager::FindEventHandlerDataAndUserWrapper(const CBLModule* pMod, stEventHandlerData*& pModData, CEventsWrapperManager::CConstComponentClassPtr& pCont) const
{
	bool bRes = FindEventHandlerDataWithoutVerifyUserWrapper(pMod, pModData);
	if (bRes)
		pCont = pModData->GetHandler();
	else
		pCont = NULL;
	return bRes;
}

CEventsWrapperManager::CConstComponentClassPtr CEventsWrapperManager::CModulesManager::FindWrapper(const CBLModule* pMod) const
{
	stEventHandlerData* pModData = NULL;
	CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;

	FindEventHandlerDataAndUserWrapper(pMod, pModData, pCont);
	return pCont;
}

bool CEventsWrapperManager::CModulesManager::LookupMethod(const CBLModule* pMod, stEventHandlerData*& pModData, CEventsWrapperManager::CConstComponentClassPtr& pCont, int iMethNumOfModule, int& iNumMethOfContext) const
{
	if(!FindEventHandlerDataAndUserWrapper(pMod, pModData, pCont))
		return false;

	if (!pModData->LookupMethodsNum(iMethNumOfModule, iNumMethOfContext))
	{
		//LogErr("Неудача LookupMethod,  не нашли метод контекста по номеру метода формы %d", iMethNumOfModule);
		return false;
	}

	return true;
}

void CEventsWrapperManager::CModulesManager::DestroyWrapper(const CBLModule* pMod)
{
	if (!IsAddedModules())
		return;

	//LogErr("Пытаюсь снять перехват %d", pMod);

	stEventHandlerData* pModData = NULL;
	if(FindEventHandlerDataWithoutVerifyUserWrapper(pMod, pModData))
	{
		//LogErr("Снимаю перехват");
		RemoveKey(pMod);
		delete pModData;
	}
}

// =======================  ============================
//
//		class CEventsWrapperManager
//
// =======================  ============================

void CEventsWrapperManager::CValidator::ValidateParamUserClass(const CValue& UserClassContValue) const
{
	CBLContext* pUserClassCont = UserClassContValue.GetContext();

	if (!IS_KINDOF_CComponentClass(pUserClassCont))
		RuntimeError("ПерехватитьСобытияГлобальногоМодуля: объект-перехватчик должен быть пользовательским классом 1С++");
}

void CEventsWrapperManager::CValidator::ValidateParamUserClass(CEventsWrapperManager::CConstComponentClassPtr pUserClassCont, bool bIsHookOfGlobalModule) const
{
	if (bIsHookOfGlobalModule)
		if (!pMainAddIn->Property["ПерехватитьСобытияГК"])
			RuntimeError("Использование метода 'ПерехватитьСобытияГлобальногоМодуля' запрещено в настройках");

	if (!pUserClassCont)
		if (!bIsHookOfGlobalModule)
			RuntimeError("ПерехватитьСобытияГК: не передан объект-перехватчик");
		else
			RuntimeError("ПерехватитьСобытияГлобальногоМодуля: не передан объект-перехватчик");
}

void CEventsWrapperManager::CValidator::ValidateParam(const CGroupContext* pGroupCont, CEventsWrapperManager::CConstComponentClassPtr pUserClassCont) const
{
	if (!pMainAddIn->Property["ПерехватитьСобытияГК"])
		RuntimeError("Использование метода 'ПерехватитьСобытияГК' запрещено в настройках");

	if (!pGroupCont)
		RuntimeError("ПерехватитьСобытияГК: не передан групповой контекст");

	ValidateParamUserClass(pUserClassCont);
}

void CEventsWrapperManager::CValidator::ValidateParam(const CValue& GroupContValue, const CValue& UserClassContValue) const
{
	ValidateParamGroupContext(GroupContValue, "ПерехватитьСобытияГК: должен быть передан групповой контекст");
	ValidateParamUserClass(UserClassContValue);
}

// =======================  ============================
//
//		class CModuleEventsManager
//
// =======================  ============================

int CModuleEventsManager::FindMethodOfContext(CEventsWrapperManager::CConstComponentClassPtr pCont, char const * sName, bool bIsGlobalModuleEvent) const
{
	if (!bIsGlobalModuleEvent)
	{
		int iNum = pCont->FindMethod(CString("Событие_")+sName);
		if (-1 == iNum)
			iNum = pCont->FindMethod(sName);
		return iNum;
	}
	else{
		int iNum = pCont->FindMethod(CString("СобытиеГМ_") + sName);
		return iNum;
	}
}

int  CModuleEventsManager::FindWrapMethod(const CBLModule* pMod, char const * sRusName, char const * sEngName, CEventsWrapperManager::CConstComponentClassPtr& pCont)const
{
	if (!IsWrapEnable())
		return -1;

	pCont = m_map->FindWrapper(pMod);
	if (pCont)
	{
		//LogErr("%s - Получили перехват метода %s", "FindWrapMethod", sRusName);
		int iNum = FindMethodOfContext(pCont, sRusName, ::GetGlobalModule() == pMod);
		if (-1 == iNum)
			iNum = FindMethodOfContext(pCont, sEngName, ::GetGlobalModule() == pMod);
		//LogErr("%s - Получили перехват метода %s, вернули номер %d", "FindWrapMethod", sRusName, iNum);

		return iNum;

	}

	return -1;
}

int  CModuleEventsManager::FindMethod(const CBLModule* pMod, bool bFuncFlag, char const * pName, int flag)const
{
	if (!IsWrapEnable())
		return -1;
	//LogErr("%s: перед проверкой перехвата метода %s %d", "FindMethod", pName, flag);

	stEventHandlerData* pModData = NULL;
	CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;

	if(m_map->FindEventHandlerDataAndUserWrapper(pMod, pModData, pCont))
	{
		//LogErr("%s: Получили перехват метода %s %d", "FindMethod", pName, flag);
		int iNumInContext = FindMethodOfContext(pCont, pName, ::GetGlobalModule() == pMod); // TODO здесь падает, если сделать в КОП-е заменить базу на контекст открытой формы:(
		if (-1 == iNumInContext)
		{
			//LogErr("%s: не нашли в перехватчике метод %s", "FindMethod", pName);
			return -1;
		}

		int iNumInMod = -1;

		// для исключения багов нужно отключить свой перехват
		{
			CDllMethodWrapperGuard<PF_FF> wrapper1(wrapperFindFunc);
			CDllMethodWrapperGuard<PF_FP> wrapper2(wrapperFindProc);

			// вместо номера метода в модуле подсовываем номер метода из контекста-перехватчика
			if (bFuncFlag)
				iNumInMod = pMod->FindFunc(pName);
			else{
				iNumInMod = pMod->FindProc(pName, flag);
				if (-1 == iNumInMod)
					iNumInMod = pMod->FindFunc(pName);
			}
		}

		if (-1 == iNumInMod)
		{
			//LogErr("%s: не нашли в модуле формы метод %s", "FindMethod", pName);
			iNumInMod = pMod->GetNProcs() + iNumInContext;
		}

		pModData->AddMethodsNum(iNumInMod, iNumInContext);

		// получу и запомню количество параметров метода в обработчике
		int nParamsCount = pCont->GetNParams(iNumInContext);
		pModData->AddMethodsParams(iNumInContext, nParamsCount);

		//LogErr("Вернули метод модуля <%s> с номером <%d>, подменив его на номер перехватчика <%d>", "FindMethod", iNumInMod, iNum);
		return iNumInMod;
	}
	return -1;
}

int CModuleEventsManager::CallMethod(const CBLModule7* pMod, bool bFuncFlag, int num,class CValue * pRetValue,int argNum,class CValue * * ppValue) const
{
	if (!IsWrapEnable())
		return FALSE;

	stEventHandlerData* pModData = NULL;
	CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;
	int iNumMethOfCont;

	//LogErr("Попробую выполнить метод формы <%s> с номером <%d>", "CallMethod", num);

	if (!m_map->LookupMethod(pMod, pModData, pCont, num, iNumMethOfCont))
		return FALSE;

	//LogErr("Выполняем метод перехватчика <%s> с номером <%d>", "CallMethod", iNumMethOfCont);

	// для процедур/функций, число параметров в которых не совпадает с переданным количеством параметров, вызов не делаю
	int nParamsCountOfClassContext = -1;
	if (pModData->LookupMethodsParams(iNumMethOfCont, nParamsCountOfClassContext))
 		//if (nParamsCountOfClassContext != argNum)
 		if (nParamsCountOfClassContext != argNum && argNum != -1)
 			return TRUE;


	BOOL ret = CallMethodOfContext(pCont, bFuncFlag, iNumMethOfCont, pRetValue, ppValue);

	// для обработчиков-функций событий FormEx и других ВК - если вернули 0, значит, статус возврата также 0
	if (bFuncFlag && !pRetValue->GetNumeric().operator long())
		const_cast<CBLModule7*>(pMod)->SetStatusCode(0);

	return ret;
}

BOOL GetVerifyedParams(CBLContext* pCont, int iMethodNum, int nParamsCount, int iNotUsedParams, CValue** ppSourceValueArrayPtr, CValue**& ppResultValueArrayPtr)
{
	// для процедур/функций, число параметров в которых не совпадает с переданным количеством параметров, вызов не делаю
 	if (pCont->GetNParams(iMethodNum) > (nParamsCount-iNotUsedParams))
 		return FALSE;

	if (nParamsCount >= iNotUsedParams)
		ppResultValueArrayPtr = &ppSourceValueArrayPtr[iNotUsedParams];
	else
		ppResultValueArrayPtr = NULL;

	return TRUE;
};

int CModuleEventsManager::VerifyParamsAndRunMethodOfUserClassObject( CComponentClass* pEventHandlerCont, int iMethodNum, int nParamsCount, int iNotUsedParams, CValue &RetVal, CValue ** ppValue, const CString& ErrorMessage )
{
	CValue** ppValueForCallMethod = NULL;
	if (!GetVerifyedParams(pEventHandlerCont, iMethodNum, nParamsCount, iNotUsedParams, ppValue, ppValueForCallMethod))
	{
		// если выбрасывать здесь исключение, 1С может падать :(
 		Msg(ErrorMessage);
 		return TRUE;
	}

	bool bIsFunction = !pEventHandlerCont->HasRetVal(iMethodNum) ? false : true;

	CallMethodOfContext(pEventHandlerCont, bIsFunction, iMethodNum, &RetVal, ppValueForCallMethod);
	if (bIsFunction)
		return RetVal.GetNumeric().operator long();

	return TRUE;
}

// =======================  ============================
//
//		class CEventsWrapperManager
//
// =======================  ============================

CEventsWrapperManager::PGroupContextWrap CEventsWrapperManager::wrapperForGroupContext(NULL);
bool CEventsWrapperManager::bLoadedOptimizedFormEx = false;

CEventsWrapperManager::CConstComponentClassPtr CEventsWrapperManager::GetWrapper(const CGroupContext* pGroupCont) const
{
	CBLModule* pMod = GetModuleByContext(pGroupCont);
	if (!pMod)
		pMod = CBLModule::GetExecutedModule(); // TODO уточнить, верно это или нет?

	return m_map->FindWrapper(pMod);
}

CEventsWrapperManager::CConstComponentClassPtr CEventsWrapperManager::GetWrapperForGlobalModule() const
{
	return m_map->FindWrapper(::GetGlobalModule());
}

void CEventsWrapperManager::SetWrapperForGlobalModule(const CValue& UserClassContValue)
{
	m_Validator.ValidateParamUserClass(UserClassContValue);
	SetWrapperForGlobalModule(static_cast<CComponentClass*>(UserClassContValue.GetContext()));
}

void CEventsWrapperManager::SetWrapperForModule(const CBLModule* pMod, CEventsWrapperManager::CConstComponentClassPtr pUserClassCont, bool bIsHookOfGlobalModule)
{
	stEventHandlerData* pModData = NULL;

	if(!m_map->IsWrapperExists(pMod))
	{
		m_map->Add(pMod, pUserClassCont);

		RefreshAllEventsDataOfGlobalModule_From_FormEx();

	}
	else
		if (bIsHookOfGlobalModule)
			RuntimeError("На глобальный модуль уже установлен перехват!"); // TODO нужны тесты
		else
		//	RuntimeError("На данную форму уже установлен перехват!"); // TODO нужны тесты
		{
			m_map->DestroyWrapper(pMod);
			m_map->Add(pMod, pUserClassCont);

			RefreshAllEventsDataOfGlobalModule_From_FormEx();
		}
}

void CEventsWrapperManager::SetWrapperForGlobalModule(CEventsWrapperManager::CConstComponentClassPtr pUserClassCont)
{
	m_Validator.ValidateParamUserClass(pUserClassCont);

	CBLModule* pMod = ::GetGlobalModule();
	SetWrapperForModule(pMod, pUserClassCont, true);
}

void CEventsWrapperManager::SetWrapper(const CValue& GroupContValue, const CValue& UserClassContValue)
{
	m_Validator.ValidateParam(GroupContValue, UserClassContValue);
	SetWrapper(static_cast<CGroupContext*>(GroupContValue.GetContext()), static_cast<CComponentClass*>(UserClassContValue.GetContext()));
}

void CEventsWrapperManager::SetWrapper(const CGroupContext* pGroupCont, CEventsWrapperManager::CConstComponentClassPtr pUserClassCont)
{
	m_Validator.ValidateParam(pGroupCont, pUserClassCont);

	CBLModule* pMod = GetModuleByContext(pGroupCont);
	//if (!pMod)
	//	pMod = CBLModule::GetExecutedModule(); // TODO уточнить, верно это или нет?

	SetWrapperForModule(pMod, pUserClassCont, false);

	RefreshAllEventsDataOfGroupContext_From_FormEx(pGroupCont);
};

CSafeContextPtrDef GetServiceObjectFromFormEx(void)
{
	CSafeContextPtrDef pCont;
	try
	{
		pCont.Create("Сервис");
	}
	catch(...)
	{
		return pCont; // FormEx не загружена
	}

	return pCont;
}

static bool FindAndCallMethodOfServiceFromFormEx(const char* szMethodsName, CValue** ppParams)
{
	CSafeContextPtrDef pCont = GetServiceObjectFromFormEx();
	if (!pCont)
		return false;

	int nMethod = pCont->FindMethod(szMethodsName);
	if (-1 == nMethod)
		return false; // старая версия ФормЕкс, без оптимизатора

	int res = pCont->CallAsProc(nMethod, ppParams);

	return true;
}

// если ФормЕкс загружена, заставим оптимизатор событий FormEx заново перечитать данные методов модуля формы
void CEventsWrapperManager::RefreshAllEventsDataOfGroupContext_From_FormEx(const CGroupContext* pGroupCont) const
{
	CValue val; val.AssignContext(const_cast<CGroupContext*>(pGroupCont));
	CValue* pValue = &val;

	bLoadedOptimizedFormEx = FindAndCallMethodOfServiceFromFormEx("ОбновитьДанныеМетодовФормы", &pValue);
}

void CEventsWrapperManager::RefreshAllEventsDataOfGlobalModule_From_FormEx(void) const
{
	bLoadedOptimizedFormEx = FindAndCallMethodOfServiceFromFormEx("ОбновитьДанныеМетодовГлобальногоМодуля", NULL);
}

void CEventsWrapperManager::DestroyWrapper(const CBLModule* pMod)
{
	m_map->DestroyWrapper(pMod);
}

void CEventsWrapperManager::DestroyWrapper(CGroupContext* pGroupCont)
{
	CBLModule* pMod = GetModuleByContext(pGroupCont);
	//if (!pMod)
	//	pMod = CBLModule::GetExecutedModule(); // TODO уточнить, верно это или нет?

	DestroyWrapper(pMod);
}

// исключительно для правильного перехвата событий FormEx и других ВК
//
// 2) TODO_ВАЖНО учитываю тот факт, что в при вызове методов через IDispatch-интерфейс 1с два метода ВСЕГДА вызываются последовательно
//
//	CBLModule::GetProcInfo, далее CBLProcInfo::IsExported
//
//	если перестает работать перехват событий глобального модуля, например, для ВК RikInet, проверить в коде 1C.exe вызов этих методов

int  CEventsWrapperManager::GetProcInfo(const CBLModule* pMod, int nProc, class CBLProcInfo & cProcInfo) //const
{
	// эти две строки нужны для проверки - установлен перехват или нет
	if (!IsWrapEnable())
		return FALSE;

	stEventHandlerData* pModData = NULL;
	CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;
	int iNum;

	if (!m_map->LookupMethod(pMod, pModData, pCont, nProc, iNum))
	{
		// FormEx может вызывать данный метод без вызова FindFunc или FindProc
		// поэтому нужна доп. обработка в этом
		class CBLProcInfo ProcInfo;
		if(!pMod->GetProcInfo(nProc, ProcInfo))
			return FALSE;

		if (-1 == FindMethod(pMod, ProcInfo.HasRetVal() ? true : false, ProcInfo.GetName(), 0))
			return FALSE;
	}

	//LogErr("Возвращаю %s для модуля %d номер метода %d", "GetProcInfo", pMod, nProc);

	cProcInfo.m_Module = const_cast<CBLModule*>(pMod);
	cProcInfo.m_ProcID = nProc; //iNum; // так как будто данная процедура в модуле есть !

	//int t = cProcInfo.GetType();
	m_pCBLProcInfo = &cProcInfo;

	wrapperCBLProcInfo_IsExported.RestoreWrapAddress();

	return TRUE;

}

// исключительно для правильного перехвата событий FormEx
// ---------------------------------  ---------------------------------  ---------------------------------
//
// 1) TODO_ВАЖНО учитываю тот факт, что в FormEx три метода ВСЕГДА вызываются последовательно
//
//	конструктор CBLProcInfo, далее CBLProcInfo::GetType, далее CBLProcInfo::GetNParams
//
//	если перестает работать перехват событий ФормЕкс, проверить в коде ФормЕкс вызов этих методов
//
// 2) TODO_ВАЖНО учитываю тот факт, что в при вызове методов через IDispatch-интерфейс 1с два метода ВСЕГДА вызываются последовательно
//
//	CBLModule::GetProcInfo, далее CBLProcInfo::IsExported
//
//	если перестает работать перехват событий глобального модуля, например, для ВК RikInet, проверить в коде 1C.exe вызов этих методов
//
// ---------------------------------  ---------------------------------  ---------------------------------
void CEventsWrapperManager::CBLProcInfo_CBLProcInfo(void)
{
	// эти две строки нужны для проверки - установлен перехват или нет
	if (!IsWrapEnable())
		return;

	m_pCBLProcInfo = NULL;

}

// исключительно для правильного перехвата событий FormEx
int  CEventsWrapperManager::CBLProcInfo_GetType(CBLProcInfo* pProcInfo)
{
	if (IsWrapEnable() && m_pCBLProcInfo && m_pCBLProcInfo == pProcInfo)
	{
		stEventHandlerData* pModData = NULL;
		CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;
		int iNum;
		//LogErr("%s для модуля %d номер метода %d", "GetProcInfo", pMod, nProc);

		if (m_map->LookupMethod(pProcInfo->m_Module, pModData, pCont, pProcInfo->m_ProcID, iNum))
			return !pCont->HasRetVal(iNum) ? 1 : 0 ;
	}

	// 	m_pCBLProcInfo = NULL;

 	// TODO для правильного перехвата глобальных событий FormEx
 	wrapperCallAsFunc.RestoreWrapAddress();
 	wrapperCallAsProc.RestoreWrapAddress();

 	wrapperCBLProcInfo_GetNParams.RestoreWrapAddress();

	return pProcInfo->GetType();

}

int  CEventsWrapperManager::CBLProcInfo_GetNParams(CBLProcInfo* pProcInfo)
{
	if (IsWrapEnable() && m_pCBLProcInfo && m_pCBLProcInfo == pProcInfo)
	{
		stEventHandlerData* pModData = NULL;
		CEventsWrapperManager::CConstComponentClassPtr pCont = NULL;
		int iNum;

		// TODO для правильного перехвата глобальных событий FormEx
 		wrapperCallAsFunc.RestoreWrapAddress();
 		wrapperCallAsProc.RestoreWrapAddress();

		if (m_map->LookupMethod(pProcInfo->m_Module, pModData, pCont, pProcInfo->m_ProcID, iNum))
			return pCont->GetNParams(iNum);
	}

	m_pCBLProcInfo = NULL;

	return pProcInfo->GetNParams();

}

int  CEventsWrapperManager::CBLProcInfo_IsExported(CBLProcInfo* pProcInfo)
{
	if (IsWrapEnable() && m_pCBLProcInfo && m_pCBLProcInfo == pProcInfo)
	{
		// TODO для правильного перехвата глобальных событий FormEx
 		wrapperCallAsFunc.RestoreWrapAddress();
 		wrapperCallAsProc.RestoreWrapAddress();

		m_pCBLProcInfo = NULL;

		return TRUE;
	}

	m_pCBLProcInfo = NULL;

	return pProcInfo->IsExported();

}

BOOL GetVerifyedParams(CBLModule* pMod, int iMethodNum, int nParamsCount, int iNotUsedParams, CValue** ppSourceValueArrayPtr, CValue**& ppResultValueArrayPtr)
{
	// для процедур/функций, число параметров в которых не совпадает с переданным количеством параметров, вызов не делаю
	CBLProcInfo procInfo;
	pMod->GetProcInfo(iMethodNum, procInfo);
	if (procInfo.GetNParams() > (nParamsCount-iNotUsedParams))
 		return FALSE;

	if (nParamsCount >= iNotUsedParams)
		ppResultValueArrayPtr = &ppSourceValueArrayPtr[iNotUsedParams];
	else
		ppResultValueArrayPtr = NULL;

	return TRUE;
}

// чтобы избавиться от перехватов FormEx, иначе будут падения из-за некорректного поведения FormEx
class COriginalMethodsGuard
{
private:
	CDllMethodWrapperGuard<PF_FF> m_wrapper1;
	CDllMethodWrapperGuard<PF_FP> m_wrapper2;
	CDllMethodWrapperGuard<PF_GetNParams> m_wrapper3;
	CDllMethodWrapperGuard<PF_GPI> m_wrapper4;
	CDllMethodWrapperGuard<PF_CAP> m_wrapper5;
	CDllMethodWrapperGuard<PF_CAF> m_wrapper6;
public:
	COriginalMethodsGuard() :
		m_wrapper1(wrapperFindFunc),
		m_wrapper2(wrapperFindProc),
		m_wrapper3(wrapperCBLProcInfo_GetNParams),
		m_wrapper4(wrapperGetProcInfo),
		m_wrapper5(wrapperCallAsProc),
		m_wrapper6(wrapperCallAsFunc)
	{};
};

// такая же проблема и решение, как с в схеме с перехватом CBLModule7::ExecProc и CBLModule::IsCompiled
//
// Выход из ситуации найден такой - из кода 1С видно, что 1С при вызове CBLModule::CallAsProc/CallAsFunc
// ВСЕГДА вызывает метод CValue::CValue(const CValue&), который, в свою очередь,
// ВСЕГДА вызывает метод CNumeric::CNumeric(void), который, в свою очередь,
//
// к сожалению, к конструктору копирования CValue привязаться не удалось, компилятор дает ошибку приведения типов в методе DoWrap
// а к конструктору по умолчанию CNumeric удалось привязаться без проблем
//

int CEventsWrapperManager::RunOriginalEventOfGroupContext( CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue )
{
	COriginalMethodsGuard MethodsWrapper;

	int iMethodNum = pGroupContext->FindMethod(strEventName);
	if(-1 == iMethodNum)
		return TRUE; // если не нашли, тогда статус возврата в любом случае TRUE

	const int iNotUsedParams = 2;

	CValue** ppValueForCallMethod = NULL;
	if (!GetVerifyedParams(pGroupContext, iMethodNum, nParamsCount, iNotUsedParams, ppValue, ppValueForCallMethod))
	{
		// если выбрасывать здесь исключение, 1С может падать :(
		Msg("Перехватчик::ВыполнитьОригинальноеСобытиеГК - Неверное число параметров у обработчика события <%s>", strEventName);
 		return TRUE;
	}

	bool bIsFunction = 0 == pGroupContext->HasRetVal(iMethodNum) ? false : true;
	//LogErr("strEventName <%s>, iMethodNum <%d>, bIsFunction <%d>", strEventName, iMethodNum, bIsFunction);

	{
		//CWrapper_CBLModule_CallAs_Guard MethodsGuard;

		if (bIsFunction)
			pGroupContext->CallAsFunc(iMethodNum, RetVal, ppValueForCallMethod);
		else
			pGroupContext->CallAsProc(iMethodNum, ppValueForCallMethod);
	}

	CBLModule7* pMod = static_cast<CBLModule7*>(::GetModuleByContext(pGroupContext));
	return pMod->GetStatusCode();
}

int CEventsWrapperManager::RunOriginalEventOfGlobalModule(const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue)
{
	COriginalMethodsGuard MethodsWrapper;

	int iMethodNum = CHookGroupContext::FindOriginalEventOfGlobalModule(strEventName);
	if(-1 == iMethodNum)
		return TRUE; // если не нашли, тогда статус возврата в любом случае TRUE

	CBLModule7* pMod = GetGlobalModule();

	const int iNotUsedParams = 1;

	CValue** ppValueForCallMethod = NULL;
	if (!GetVerifyedParams(pMod, iMethodNum, nParamsCount, iNotUsedParams, ppValue, ppValueForCallMethod))
	{
		// если выбрасывать здесь исключение, 1С может падать :(
		Msg("Перехватчик::ВыполнитьОригинальноеСобытиеГлобальногоМодуля - Неверное число параметров у обработчика события <%s>", strEventName);
 		return TRUE;
	}

	{
		//CWrapper_CBLModule_CallAs_Guard MethodsGuard;

		int iRes = pMod->CallAsProc(iMethodNum, nParamsCount-iNotUsedParams, ppValueForCallMethod);
	}

	return pMod->GetStatusCode();
}

int CEventsWrapperManager::RunEventOfGroupContext( CComponentClass* pEventHandlerCont, CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue )
{
	if (!pEventHandlerCont)
 		return RunOriginalEventOfGroupContext(pGroupContext, strEventName, nParamsCount-1, RetVal, ppValue+1);

	// теперь работаем с классом-обработчиком

	int iMethodNum = FindMethodOfContext(pEventHandlerCont, strEventName, false);

	// если метода нет, это нормально, тогда тупо вызываем обработчик из формы
	if (-1 == iMethodNum)
		return RunOriginalEventOfGroupContext(pGroupContext, strEventName, nParamsCount-1, RetVal, ppValue+1);

	const int iNotUsedParams = 3;

	CString strErrorMessage;
	strErrorMessage.Format("Перехватчик::ВыполнитьСобытиеКонтекста - Неверное число параметров у обработчика события <%s>", strEventName);

	return VerifyParamsAndRunMethodOfUserClassObject(pEventHandlerCont, iMethodNum, nParamsCount, iNotUsedParams, RetVal, ppValue, strErrorMessage);
}

int CEventsWrapperManager::RunEventOfGlobalModule( CComponentClass* pEventHandlerCont, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue )
{
	if (!pEventHandlerCont)
		return RunOriginalEventOfGlobalModule(strEventName, nParamsCount-1, RetVal, ppValue+1);

	// теперь работаем с классом-обработчиком

	int iMethodNum = FindMethodOfContext(pEventHandlerCont, strEventName, true);

	// если метода нет, это нормально, тогда тупо вызываем обработчик из формы
	if (-1 == iMethodNum)
		return RunOriginalEventOfGlobalModule(strEventName, nParamsCount-1, RetVal, ppValue+1);

	const int iNotUsedParams = 2;

	CString strErrorMessage;
	strErrorMessage.Format("Перехватчик::ВыполнитьСобытиеГлобальногоМодуля - Неверное число параметров у обработчика события <%s>", strEventName);

	return VerifyParamsAndRunMethodOfUserClassObject(pEventHandlerCont, iMethodNum, nParamsCount, iNotUsedParams, RetVal, ppValue, strErrorMessage);
}

int CEventsWrapperManager::RunModuleHandler(bool bFSO, CBLModule7* pThisMod, CComponentClass* pCont, int iNum, class CValue * * params )
{
	//LogErr("вызываем перехват");
	bool bFuncFlag = !pCont->HasRetVal(iNum) ? false : true;
	CValue locVal;

	CallMethodOfContext(pCont, bFuncFlag, iNum, &locVal, params);

	// если обработчик события - это функция, ее результат - это статус возврата
	if (bFuncFlag)
	{
		int i = locVal.GetNumeric().operator long();
		//LogErr("iStatusCode %d", i);
		if (!bFSO || (bFSO && !i)){
			pThisMod->SetStatusCode(i);
			pThisMod->m_bProcNotFound = 0; // иначе, например, не вызывается ВводНаОсновании
		}

		if (bFSO && !i)
			return TRUE;
	}

	return !bFSO ? TRUE : FALSE;
}

int CEventsWrapperManager::FindAndRunGlobalModuleEventHandler( char* sRusName, char* sEngName, CBLModule7* pThisMod, int iParamsCount, class CValue * * params )
{
	CComponentClass* pCont = NULL;
	int iNum = FindWrapMethod(::GetGlobalModule(), sRusName, sEngName, pCont);
	if (-1 == iNum)
		return FALSE;

	CValue param1;
	param1.AssignContext(pThisMod->GetInternalData()->pGeneralContext);

	std::vector<CValue*> paramsVector;//(iParamsCount+1);
	paramsVector.push_back(&param1);
	for(int i=0;i<iParamsCount;i++)
		paramsVector.push_back(*params++);

	if (!stricmp("ВводНаОсновании", sRusName))
		return RunModuleHandler(false, pThisMod, pCont, iNum, &paramsVector[0]);
	else
		return RunModuleHandler(true, pThisMod, pCont, iNum, &paramsVector[0]);
}

int CEventsWrapperManager::FindAndRunModuleEventHandler( CBLModule7* pThisMod, char* sRusName, char* sEngName, class CValue * * params )
{
	CComponentClass* pCont = NULL;
	int iNum = FindWrapMethod(pThisMod, sRusName, sEngName, pCont);

	if (-1 == iNum)
		return FALSE;

	pThisMod->SetStatusCode(1);
	return RunModuleHandler(false, pThisMod, pCont, iNum, params);
}
