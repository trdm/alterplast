#pragma once

#include "../mycontextbase.h"
#include "OwnAboutWindowManager.h"

class CInterceptor : public CMyContextBase, public IAddOwnAboutWindowManager
{
	DECLARE_DYNCREATE(CInterceptor);

public:

	BOOL procHookEventsOfGroupContext(CValue &RetVal, CValue **ppValue);
	//BOOL funcCallOriginalEventOfGroupContext(CValue &RetVal, CValue **ppValue);
	BOOL funcGetHookObjectForEventsOfGroupContext(CValue &RetVal, CValue **ppValue);
	BOOL procUnHookEventsOfGroupContext(CValue **ppValue);

	BOOL procHookEventsOfGlobalModule(CValue &RetVal, CValue **ppValue);
	//BOOL funcCallOriginalEventOfGlobalModule(CValue &RetVal, CValue **ppValue);
	BOOL funcGetHookObjectForEventsOfGlobalModule(CValue &RetVal, CValue **ppValue);
	BOOL procUnHookOriginalEventOfGlobalModule(CValue **ppValue);

	BOOL funcRunOriginalEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue);
	BOOL funcRunOriginalEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue);

	BOOL funcRunEventOfGroupContext(int nParamsCount, CValue &RetVal, CValue **ppValue);
	BOOL funcRunEventOfGlobalModule(int nParamsCount, CValue &RetVal, CValue **ppValue);

	BOOL procAddEvent_UnHandledError(CValue **ppValue);
	BOOL procAddEvent_ReportEventA(CValue **ppValue);
	BOOL procAddEvent_ReportUserEvent(CValue **ppValue);

	BOOL procAddEvent_WriteMessageEvent_DoRestructureVeryLongString(CValue **ppValue);

	BOOL procHookEventsOfDocumentTransaction(CValue &RetVal, CValue **ppValue);
	BOOL funcGetHookObjectForEventsOfDocumentTransaction(CValue &RetVal, CValue **ppValue);
	BOOL procUnHookOriginalEventOfDocumentTransaction(CValue **ppValue);

	// интерфейс IAddOwnAboutWindowManager
	virtual void Add(IOwnAboutWindowManager* pOwnAboutWindowManager);

public:

	DECLARE_MY_CONTEXT();
};