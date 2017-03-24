#pragma once

#include "../mycontextbase.h"

class CInformer : public CMyContextBase
{
	DECLARE_DYNCREATE(CInformer);

public:

	BOOL funcGetMethodsTable(CValue &RetVal, CValue **ppValue);
	BOOL funcGetPropsTable(CValue &RetVal, CValue **ppValue);

	BOOL funcHaveContextMethod(CValue &RetVal, CValue **ppValue);
	BOOL funcHaveContextProp(CValue &RetVal, CValue **ppValue);

	// явл€етс€ќбъектом(ѕровер€емыйќбъект»лиЌаименование“ипа, Ќаименование“ипаЌа оторыйѕровер€ем)
	BOOL funcTheObjectIs(CValue &RetVal, CValue **ppValue);

	// явл€етс€‘ункцией(ѕровер€емыйќбъект, Ќаименованиећетода оторыйѕровер€ем)
	BOOL funcIsFunction(CValue &RetVal, CValue **ppValue);

	// получить командную строку 1— (вз€то из FormEx)
	BOOL  funcCommandLine(CValue& RetVal, CValue **params);
	// получить путь к приложению 1—
	BOOL  funcGetAppPath(CValue& RetVal, CValue **params);

	// получить внутренний тип значени€ дл€ контекста (из RuntimeClass)
	// например, дл€ объекта "–егистры" встроенный “ип«начени€—тр выдает "Ќеизвестныйќбъект"
	BOOL  funcGetInternalTypeName(CValue& RetVal, CValue **params);

	// получить врем€ просто€ компьютера
	BOOL funcGetIdleTime(CValue &RetVal, CValue **ppValue);

	BOOL funcGetAddInDllTable(CValue &RetVal, CValue **ppValue);

	static bool GetMethodsTable(const CValue& ContextValue, CValue& RetVal);
	static bool GetPropsTable(const CValue& ContextValue, CValue& RetVal);

	static void GetMethodOfContext(class CValue * * ppValue);

	BOOL funcGetCurrentModuleText(CValue &RetVal, CValue **ppValue);
	BOOL funcGetModuleText(CValue &RetVal, CValue **ppValue);
	
public:

	DECLARE_MY_CONTEXT();

private:
	static bool GetTableFromContext(bool bNeedMethods, const CValue& ContextValue, CValue& VTValue);
	static bool GetTableFromContext(bool bNeedMethods, CBLContext* pCont, CValueTable* pVT);

	BOOL HaveContextMethodOrProp(bool bNeedMethods, CValue &RetVal, CValue **ppValue);
};
