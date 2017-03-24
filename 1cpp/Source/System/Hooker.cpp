// Hooker.cpp: implementation of the CHooker class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Hooker.h"

#include "GroupContextWrap.h"
#include "DllHooker.h"

#include "WrapAboutDlg.h"

//_____________________________________________________________________________
//
// CHooker Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CHooker) 
    BL_METH_FUNC("HookEventsOfGroupContext", "ѕерехватить—обыти€√ ", 2, procHookEventsOfGroupContext)
	//BL_METH_FUNC("CallOriginalEventOfGroupContext", "¬ызватьќригинальное—обытие√ ", 3, funcCallOriginalEventOfGroupContext)

    BL_METH_FUNC("GetHookObjectForEventsOfGroupContext", "ѕолучитьѕерехватчик—обытий√ ", 1, funcGetHookObjectForEventsOfGroupContext)
	BL_METH_PROC("UnHookEventsOfGroupContext", "—н€тьѕерехват—обытий√K", 1, procUnHookEventsOfGroupContext)

    BL_METH_FUNC("HookEventsOfGlobalModule", "ѕерехватить—обыти€√лобальногоћодул€", 1, procHookEventsOfGlobalModule)
	//BL_METH_FUNC("CallOriginalEventOfGlobalModule", "¬ызватьќригинальное—обытие√лобальногоћодул€", 2, funcCallOriginalEventOfGlobalModule)

    BL_METH_FUNC("GetHookObjectForEventsOfGlobalModule", "ѕолучитьѕерехватчик—обытий√лобальногоћодул€", 0, funcGetHookObjectForEventsOfGlobalModule)
	BL_METH_PROC("UnHookEventsOfGlobalModule", "—н€тьѕерехват—обытий√лобальногоћодул€", 0, procUnHookOriginalEventOfGlobalModule)

	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunOriginalEventOfGroupContext", "¬ыполнитьќригинальное—обытие√ ", 2, funcRunOriginalEventOfGroupContext)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunOriginalEventOfGlobalModule", "¬ыполнитьќригинальное—обытие√лобальногоћодул€", 1, funcRunOriginalEventOfGlobalModule)

	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunEventOfGroupContext", "¬ыполнить—обытие√ ", 3, funcRunEventOfGroupContext)
	BL_METH_FUNC_WITH_UN_LIM_PARAMS("RunEventOfGlobalModule", "¬ыполнить—обытие√лобальногоћодул€", 2, funcRunEventOfGlobalModule)

	BL_METH_PROC("AddEvent_UnHandledError", "ѕодключить—обытие_Ќеобработанна€ќшибка", 0, procAddEvent_UnHandledError)
	BL_METH_PROC("AddEvent_ReportEventA", "ѕодключить—обытие_«апись—обыти€∆урнала–егистрации", 0, procAddEvent_ReportEventA)
	BL_METH_PROC("AddEvent_ReportUserEvent", "ѕодключить—обытие_«аписьѕользовательского—обыти€∆урнала–егистрации", 0, procAddEvent_ReportUserEvent)
	
	BL_METH_PROC("AddVeryLongStringRestructure", "¬ключить–азбиениеƒлинных—трокƒл€ћетода—ообщить", 0, procAddEvent_WriteMessageEvent_DoRestructureVeryLongString)

END_BL_METH_MAP() 

BEGIN_BL_PROP_MAP(CHooker)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CHooker, "Hooker", "ѕерехватчик");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CHooker::procHookEventsOfGroupContext(CValue &RetVal, CValue **ppValue)
{
	funcGetHookObjectForEventsOfGroupContext(RetVal, ppValue);

	CEventsWrapperManager::GetWrapper()->SetWrapper(*ppValue[0], *ppValue[1]);

	return TRUE;
}

BOOL CHooker::procHookEventsOfGlobalModule(CValue &RetVal, CValue **ppValue)
{
	funcGetHookObjectForEventsOfGlobalModule(RetVal, ppValue);

	CEventsWrapperManager::GetWrapper()->SetWrapperForGlobalModule(*ppValue[0]);

	return TRUE;
}

BOOL CHooker::funcGetHookObjectForEventsOfGroupContext(CValue &RetVal, CValue **ppValue)
{
	const CValue& GroupContValue = *ppValue[0];
	ValidateParamGroupContext(GroupContValue, "ѕерехватить—обыти€√ : должен быть передан групповой контекст");
	CGroupContext* pGroupContext = static_cast<CGroupContext*>(GroupContValue.GetContext());
	
	CComponentClass* pCont = const_cast<CComponentClass*>(CEventsWrapperManager::GetWrapper()->GetWrapper(pGroupContext));
	RetVal.AssignContext(pCont);
	
	return TRUE;
}

BOOL CHooker::funcGetHookObjectForEventsOfGlobalModule(CValue &RetVal, CValue **ppValue)
{
	CComponentClass* pCont = const_cast<CComponentClass*>(CEventsWrapperManager::GetWrapper()->GetWrapperForGlobalModule());
	RetVal.AssignContext(pCont);
	
	return TRUE;
}

// BOOL CHooker::funcCallOriginalEventOfGlobalModule(CValue &RetVal, CValue **ppValue)
// {
// 	int iRes = CEventsWrapperManager::GetWrapper()->CallOriginalEventOfGlobalModule(*ppValue[0], *ppValue[1]);
// 	RetVal = CNumeric(iRes);
// 
// 	return TRUE;
// }
// 
// BOOL CHooker::funcCallOriginalEventOfGroupContext(CValue &RetVal, CValue **ppValue)
// {
// 	int iRes = CEventsWrapperManager::GetWrapper()->CallOriginalEventOfGroupContext(*ppValue[0], *ppValue[1], *ppValue[2]);
// 	RetVal = CNumeric(iRes);
// 
// 	return TRUE;
// }

BOOL CHooker::procUnHookEventsOfGroupContext(CValue **ppValue)
{
	CValue& GroupContValue = *ppValue[0];
	ValidateParamGroupContext(GroupContValue, "—н€тьѕерехват—обытий√K: необходимо передать групповой контекст");
	CGroupContext* pGroupCont = static_cast<CGroupContext*>(GroupContValue.GetContext());

	CEventsWrapperManager::GetWrapper()->DestroyWrapper(pGroupCont);

	return TRUE;
}

BOOL CHooker::procUnHookOriginalEventOfGlobalModule(CValue **ppValue)
{
	CEventsWrapperManager::GetWrapper()->DestroyWrapper(::GetGlobalModule());

	return TRUE;
}

BOOL CHooker::funcRunOriginalEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamGroupContext = *ppValue[0];
	CValue& ParamEventName = *ppValue[1];

	ValidateParamGroupContext(ParamGroupContext, "Ќеобходимо передать групповой контекст");
 	ValidateParamNotEmptyString(ParamEventName, "Ќеобходимо передать им€ событи€ группового контекста");

	CGroupContext* pGroupContext = static_cast<CGroupContext*>(ParamGroupContext.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunOriginalEventOfGroupContext(pGroupContext, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunOriginalEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventName = *ppValue[0];
	ValidateParamNotEmptyString(ParamEventName, "Ќеобходимо передать им€ событи€ группового контекста");
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunOriginalEventOfGlobalModule(strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventHandler = *ppValue[0];
	CValue& ParamGroupContext = *ppValue[1];
	CValue& ParamEventName = *ppValue[2];

	ValidateParamGroupContext(ParamGroupContext, "Ќеобходимо передать групповой контекст");
	ValidateParamNotEmptyString(ParamEventName, "Ќеобходимо передать им€ событи€ группового контекста");

 	if (!ParamEventHandler.IsEmpty()) // вызываем обработчик из формы
		ValidateParamUserClass(ParamEventHandler, "Ќеобходимо передать объект класса-обработчика");

	CComponentClass* pEventHandlerCont = static_cast<CComponentClass*>(ParamEventHandler.GetContext());
	CGroupContext* pGroupContext = static_cast<CGroupContext*>(ParamGroupContext.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunEventOfGroupContext(pEventHandlerCont, pGroupContext, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::funcRunEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue)
{
	CValue& ParamEventHandler = *ppValue[0];
	CValue& ParamEventName = *ppValue[1];

	ValidateParamNotEmptyString(ParamEventName, "Ќеобходимо передать им€ событи€ группового контекста");

 	if (!ParamEventHandler.IsEmpty()) // вызываем обработчик из формы
		ValidateParamUserClass(ParamEventHandler, "Ќеобходимо передать объект класса-обработчика");

	CComponentClass* pEventHandlerCont = static_cast<CComponentClass*>(ParamEventHandler.GetContext());
	CString strEventName = ParamEventName.GetString();

	int iRes = CEventsWrapperManager::GetWrapper()->RunEventOfGlobalModule(pEventHandlerCont, strEventName, nParamsCount, RetVal, ppValue);
	RetVal = CNumeric(iRes);

	return TRUE;
}

BOOL CHooker::procAddEvent_UnHandledError(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_UnHandledError();

	return TRUE;
}

BOOL CHooker::procAddEvent_ReportEventA(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_ReportEventA();

	return TRUE;
}

BOOL CHooker::procAddEvent_ReportUserEvent(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_ReportUserEvent();

	return TRUE;
}

BOOL CHooker::procAddEvent_WriteMessageEvent_DoRestructureVeryLongString(CValue **ppValue)
{
	CHookGroupContext::GetHooker()->AddEvent_WriteMessageEvent();
	
	return TRUE;
}

// интерфейс IAddOwnAboutWindowManager
void CHooker::Add(IOwnAboutWindowManager* pOwnAboutWindowManager)
{
	CWrapAboutDlg::AddMyAboutWindowManager(pOwnAboutWindowManager);
}
