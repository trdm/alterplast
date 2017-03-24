// Informer.cpp: implementation of the CInformer class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\\SetOfHierarchy.h"
#include "Informer.h"

//_____________________________________________________________________________
//
// CInformer Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CInformer)
    BL_METH_FUNC("GetMethodsTable", "ѕолучить“аблицућетодов", 1, funcGetMethodsTable)
    BL_METH_FUNC("GetPropsTable", "ѕолучить“аблицу—войств", 1, funcGetPropsTable)

    BL_METH_FUNC("MethodExists", "ћетод—уществует", 2, funcHaveContextMethod)
    BL_METH_FUNC("PropExists", "—войство—уществует", 2, funcHaveContextProp)

	// явл€етс€ќбъектом(ѕровер€емыйќбъект»лиЌаименование“ипа, Ќаименование“ипаЌа оторыйѕровер€ем)
    BL_METH_FUNC("TheObjectIs", "явл€етс€ќбъектом", 2, funcTheObjectIs)

	// явл€етс€‘ункцией(ѕровер€емыйќбъект, Ќаименованиећетода оторыйѕровер€ем)
    BL_METH_FUNC("IsFunction", "явл€етс€‘ункцией", 2, funcIsFunction)

    BL_METH_FUNC("CommandLine",		" омандна€—трока",	0, funcCommandLine)
    BL_METH_FUNC("GetAppPath",		"ѕутьѕриложени€1—",	0, funcGetAppPath)

    BL_METH_FUNC("GetInternalTypeName",		"“ип«начени€—тр¬нутр",	1, funcGetInternalTypeName)

    BL_METH_FUNC("GetIdleTimeOfSystem",		"ѕолучить¬рем€ѕросто€—истемы",	0, funcGetIdleTime)

    BL_METH_FUNC("GetAddInDllTable",		"ѕолучить“аблицу«агруженных¬ ",	0, funcGetAddInDllTable)

	BL_METH_FUNC("GetCurrentModuleText",		"ѕолучить“екст“екущегоћодул€",	0, funcGetCurrentModuleText)
	BL_METH_FUNC("GetModuleText",		"ѕолучить“екстћодул€",	1, funcGetModuleText)

END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CInformer)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CInformer, "Informer", "»нформатор");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CInformer::funcGetMethodsTable(CValue &RetVal, CValue **ppValue)
{
	GetMethodsTable(*ppValue[0], RetVal);

	return TRUE;
}

BOOL CInformer::funcGetPropsTable(CValue &RetVal, CValue **ppValue)
{
	GetPropsTable(*ppValue[0], RetVal);

	return TRUE;
}

bool CInformer::GetMethodsTable(const CValue& ContextValue, CValue& RetVal)
{
	return GetTableFromContext(true, ContextValue, RetVal);
}

bool CInformer::GetPropsTable(const CValue& ContextValue, CValue& RetVal)
{
	return GetTableFromContext(false, ContextValue, RetVal);
}

bool CInformer::GetTableFromContext(bool bNeedMethods, const CValue& ContextValue, CValue& VTValue)
{
	CBLContext* pCont = ContextValue.GetContext();

	CValueTable* pVT = NULL;
	if (VTValue.IsEmpty())
	{
		VTValue.CreateObject("“аблица«начений");
		pVT = ((CValueTableContextData*)(VTValue.GetContext()->GetInternalData()))->GetValueTable();
	}
	else
		pVT = CValue2VT(VTValue);

	if (pCont)
		return GetTableFromContext(bNeedMethods, pCont, pVT);

	return false;
}

bool CInformer::GetTableFromContext(bool bNeedMethods, CBLContext* pCont, CValueTable* pVT)
{
	if (!pVT)
		return false;

	CString str;
	const CType ctNum(NUMBER_TYPE_1C);
	const CType ctStr(STRING_TYPE_1C);

	const int iColCount = 5;
	vector<CValue> pv(iColCount);
	vector<CValue*> ppv(iColCount);
	for (int i = 0; i < iColCount; ++i)
		ppv[i] = &pv[i];

	pVT->Clear(TRUE);
	pVT->AddColumn(str = "Number",		ctNum, str, 0, "", 0);
	pVT->AddColumn(str = "NameEng",	ctStr, str, 0, "", 0);
	pVT->AddColumn(str = "NameRus",	ctStr, str, 0, "", 0);
	if (bNeedMethods)
	{
		pVT->AddColumn(str = "HasRetVal",	ctNum, str, 0, "", 0);
		pVT->AddColumn(str = "NParams",	ctNum, str, 0, "", 0);
	}
	else
	{
		pVT->AddColumn(str = "IsReadable",	ctNum, str, 0, "", 0);
		pVT->AddColumn(str = "IsWritable",	ctNum, str, 0, "", 0);
	}

	if (bNeedMethods)
		for (i = 0; i < pCont->GetNMethods(); ++i)
		{
			pv[0] = static_cast<long>(i);
			pv[1] = pCont->GetMethodName(i, 0);
			pv[2] = pCont->GetMethodName(i, 1);
			pv[3] = static_cast<long>(pCont->HasRetVal(i));
			pv[4] = pCont->GetNParams(i);
			pVT->AddRow(&ppv[0], -1);
		}
	else
		for (i = 0; i < pCont->GetNProps(); ++i)
		{
			pv[0] = static_cast<long>(i);
			pv[1] = pCont->GetPropName(i, 0);
			pv[2] = pCont->GetPropName(i, 1);
			pv[3] = static_cast<long>(pCont->IsPropReadable(i));
			pv[4] = static_cast<long>(pCont->IsPropWritable(i));
			pVT->AddRow(&ppv[0], -1);
		}
	//}

	return true;
}

void CInformer::GetMethodOfContext(class CValue * * ppValue)
{
	CBLContext* pCont = ppValue[0]->GetContext();

	if (pCont)
	{
		CValueTable *pVTm = CValue2VT(*ppValue[1]);
		CValueTable *pVTp = CValue2VT(*ppValue[2]);
		if (pVTm || pVTp)
		{
			if (pVTm)
				GetMethodsTable(*ppValue[0], *ppValue[1]);

			if (pVTp)
				GetPropsTable(*ppValue[0], *ppValue[2]);
		}
		else
		{
			// original section by Deb
			pBkEndUI->DoMessageLine(pCont->GetRuntimeClass()->m_lpszClassName, mmInformation);

			int nMeth = pCont->GetNMethods();
			if (nMeth > 0)
				pBkEndUI->DoMessageLine("==============================\r\nMethods\r\n==============================", mmInformation);
			for (int i = 0; i < nMeth; i++)
			{
				char buf[1000] = {0};
				char whatIs[100] = {0};
				if (pCont->HasRetVal(i)==1)
					strcpy(whatIs, "Function ");
				else
					strcpy(whatIs, "Procedure ");

				int nParams = pCont->GetNParams(i);

				sprintf(buf, "%d. %s [%s]  [%s] nParams = %d",i,whatIs,pCont->GetMethodName(i, 0), pCont->GetMethodName(i, 1), nParams);
				pBkEndUI->DoMessageLine(buf, mmInformation);
			}
			int nProps = pCont->GetNProps();
			if (nProps > 0)
				pBkEndUI->DoMessageLine("==============================\r\nProperties\r\n==============================", mmInformation);

			for (i = 0; i < nProps; i++)
			{
				char buf[1000] = {0};
				char whatIs[100] = {0};

				if (pCont->IsPropReadable(i)==0)
					strcpy(whatIs, "Write only ");
				if (pCont->IsPropWritable(i)==0)
					strcpy(whatIs, "Read only ");

				//int nParams = pCont->GetNParams(i);

				sprintf(buf, "%d. [%s]  [%s]  %s",i,pCont->GetPropName(i, 0), pCont->GetPropName(i, 1), whatIs);
				pBkEndUI->DoMessageLine(buf, mmInformation);
			}
		}
	}
}

BOOL CInformer::HaveContextMethodOrProp(bool bNeedMethods, CValue &RetVal, CValue **ppValue)
{
	CBLContext* pCont = ppValue[0]->GetContext();
	if (!pCont)
		if (bNeedMethods)
			RuntimeError("»нформатор::ћетод онтекста—уществует - первым параметром нужно передать контекст");
		else
			RuntimeError("»нформатор::—войство онтекста—уществует - первым параметром нужно передать контекст");

	CValue& ValueParam2 = *ppValue[1];

	if (2 != ValueParam2.GetTypeCode())
		if (bNeedMethods)
			RuntimeError("»нформатор::ћетод онтекста—уществует - вторым параметром нужно передать название метода (—трока)");
		else
			RuntimeError("»нформатор::—войство онтекста—уществует - вторым параметром нужно передать название метода (—трока)");

	int iMethodNum = -1;
	if (bNeedMethods)
		iMethodNum = pCont->FindMethod(ValueParam2.GetString());
	else
		iMethodNum = pCont->FindProp(ValueParam2.GetString());

	RetVal = CNumeric(-1 != iMethodNum? 1 : 0);

	return TRUE;
}

BOOL CInformer::funcHaveContextMethod(CValue &RetVal, CValue **ppValue)
{
	return HaveContextMethodOrProp(true, RetVal, ppValue);
}

BOOL CInformer::funcHaveContextProp(CValue &RetVal, CValue **ppValue)
{
	return HaveContextMethodOrProp(false, RetVal, ppValue);
}

// явл€етс€ќбъектом(ѕровер€емыйќбъект»лиЌаименование“ипа, Ќаименование“ипаЌа оторыйѕровер€ем)
BOOL CInformer::funcTheObjectIs(CValue &RetVal, CValue **ppValue)
{
	RetVal = CNumeric(0);

	CValue& valParam1 = *ppValue[0];
	CValue& valParam2 = *ppValue[1];

	if (2 != valParam2.GetTypeCode())
	{
		return true; // TODO возможно, нужно исключение
	}
	CString strPar2 = valParam2.GetString();
	if (strPar2.IsEmpty())
	{
		//CBLModule::RaiseExtRuntimeError("¬ метод необходимо передавать им€ класса",0);
		return true; // TODO возможно, нужно исключение
	}

	CString strPar1;
	int iTypeCodeParam1 = valParam1.GetTypeCode();

	bool bIsString = 2 == iTypeCodeParam1;
	if (bIsString)
	{

		strPar1 = valParam1.GetString();
		if (strPar1.IsEmpty())
		{
			//CBLModule::RaiseExtRuntimeError("¬ метод необходимо передавать им€ класса",0);
			return true; // TODO возможно, нужно исключение
		}
	}
	else // if (bIsString)
	{
		if (100 != iTypeCodeParam1)
			RuntimeError("ѕервым параметром необходимо передавать агрегатный объект или строку-наименование типа");

		CBLContext* pCont = valParam1.GetContext();
		if (!pCont)
			return true;

		strPar1 = pCont->GetTypeString();
	}

	if (!strPar1.CompareNoCase(strPar2) || CSetOfHierarchy::GetHierarchy()->TheClassIsDerive(strPar1, strPar2))
		RetVal = CNumeric(1);

	return true;
}

BOOL CInformer::funcIsFunction(CValue &RetVal, CValue **ppValue)
{
	RetVal = CNumeric(0); // невозможно определить

	CBLContext* pContext = ppValue[0]->GetContext();

	if (!pContext)
	{
		return TRUE;
	}
	CValue& valParam2 = *ppValue[1];

	if (2 != valParam2.GetTypeCode())
	{
		RuntimeError("¬торым параметром необходимо передавать строку-наименование метода");
	}

	CString strPar2 = valParam2.GetString();
	if (strPar2.IsEmpty())
	{
		RuntimeError("¬торым параметром необходимо передавать строку-наименование метода");
	}

	int iNumMethod = pContext->FindMethod(strPar2);
	if (iNumMethod != -1)
	{
 		RetVal = CNumeric(pContext->HasRetVal(iNumMethod));
// 		int iRes = pContext->HasRetVal(iNumMethod);
// 		RetVal = CNumeric(iRes);
	}

	return TRUE;
}

// получить командную строку 1— (вз€то из FormEx)
BOOL  CInformer::funcCommandLine(CValue& RetVal, CValue **params)
{
	CApp7* pApp7 = (CApp7*) AfxGetApp();
	RetVal = pApp7->m_lpCmdLine;

			/*
			CApp7* pApp7 = (CApp7*) AfxGetApp();
			CProfile7* pProfile7 = pApp7->GetProps();

			CString Prop0 = pProfile7->GetStringProp(0x00); // каталог, в который установлен движок
			CString Prop1 = pProfile7->GetStringProp(0x01); // каталог базы
			CString Prop2 = pProfile7->GetStringProp(0x02); // каталог пользовател€
			CString Prop3 = pProfile7->GetStringProp(0x03); // временный каталог
			CString Prop4 = pProfile7->GetStringProp(0x04); // полный путь к файлу 1Cv7.CFG (с именем файла)
			CString Prop5 = pProfile7->GetStringProp(0x05); // полный путь к файлу 1Cv7.DD (с именем файла)
			CString Prop6 = pProfile7->GetStringProp(0x06); // полный путь к файлу 1Cv7.DDS (с именем файла)
			CString Prop7 = pProfile7->GetStringProp(0x07); // полный путь к файлу 1Cv7.DBA (с именем файла)
			CString Prop8 = pProfile7->GetStringProp(0x08); // полный путь к файлу 1Cv7.MD (с именем файла)
			CString Prop9 = pProfile7->GetStringProp(0x09); // полный путь к файлу 1Cv7.LCK (с именем файла)
			CString PropA = pProfile7->GetStringProp(0x0A); // ???
			CString PropB = pProfile7->GetStringProp(0x0B); // ???
			CString PropC = pProfile7->GetStringProp(0x0C); // им€ текущей »Ѕ в списке баз
			CString PropD = pProfile7->GetStringProp(0x0D); // им€ текущего пользовател€
			CString PropE = pProfile7->GetStringProp(0x0E); // полное им€ текущего пользовател€
			CString PropF = pProfile7->GetStringProp(0xF); // ???
			CString Prop10 = pProfile7->GetStringProp(0x10); // название набора прав текущего пользовател€
			CString Prop11 = pProfile7->GetStringProp(0x11); // название интерфейса текущего пользовател€
			CString Prop12 = pProfile7->GetStringProp(0x12); // путь в реестре к установкам текущего пользовател€ (относительно HKEY_CURRENT_USER\Software\1C\1Cv7\7.7)
			*/
/*

		LPCTSTR m_pszAppName = pApp7->m_pszAppName;  // human readable name
									//  (from constructor or AFX_IDS_APP_TITLE)
		LPCTSTR m_pszExeName = pApp7->m_pszExeName;       // executable name (no spaces)
		LPCTSTR m_pszHelpFilePath = pApp7->m_pszHelpFilePath;  // default based on module path
		LPCTSTR m_pszProfileName = pApp7->m_pszProfileName;   // default based on app name

		m_pszAppName <1—:ѕредпри€тие>
		m_pszExeName <1CV7>
		m_pszHelpFilePath <C:\Program Files\1Cv77\Bin25\BIN\1CV7.HLP>
		m_pszProfileName <1Cv7\7.7\Testing 1C++\V7\Default>
		sAppPath <C:\Program Files\1Cv77\Bin25\BIN\1CV7.exe>
*/

	return true;
}

BOOL  CInformer::funcGetAppPath(CValue& RetVal, CValue **params)
{
	CApp7* pApp7 = (CApp7*) AfxGetApp();
	CProfile7* pProfile7 = pApp7->GetProps();
	CString sAppPath = pProfile7->GetStringProp(0x00) + pApp7->m_pszExeName + ".exe";

	RetVal = sAppPath;
	return true;
}

// получить внутренний тип значени€ дл€ контекста (из RuntimeClass)
// например, дл€ объекта "–егистры" встроенный “ип«начени€—тр выдает "Ќеизвестныйќбъект"
BOOL  CInformer::funcGetInternalTypeName(CValue& RetVal, CValue **params)
{
	ValidateParamContext(*params[0], "ƒанный метод примен€етс€ только дл€ агрегатных типов");

	CBLContext* pCont = params[0]->GetContext();
	CString strTypeName = pCont->GetRuntimeClass()->m_lpszClassName;

	RetVal = strTypeName;

	return TRUE;
}

// получить врем€ просто€ компьютера
BOOL CInformer::funcGetIdleTime(CValue &RetVal, CValue **ppValue)
{
	if (!IsSystem_Windows2000AndHigher())
		RuntimeError("ћетод <»нформатор::ѕолучить¬рем€ѕросто€—истемы> можно использовать только в ќ— >= Windows 2000");

	DWORD allTime = GetTickCount();

	#if(_WIN32_WINNT < 0x0500)
		typedef struct tagLASTINPUTINFO {
			UINT cbSize;
			DWORD dwTime;
		} LASTINPUTINFO, * PLASTINPUTINFO;

		typedef WINUSERAPI BOOL (APIENTRY CFUNCTION_GetLastInputInfo)(PLASTINPUTINFO plii);
	#endif

	LASTINPUTINFO Info;
	Info.cbSize = sizeof(LASTINPUTINFO);

	#if(_WIN32_WINNT < 0x0500)
		static CFUNCTION_GetLastInputInfo* pGetLastInputInfo = NULL;
		HMODULE hm = GetModuleHandle("USER32.DLL");
		if (hm)
			pGetLastInputInfo = reinterpret_cast<CFUNCTION_GetLastInputInfo*>(GetProcAddress(hm, "GetLastInputInfo"));

		if (!pGetLastInputInfo)
			RuntimeError("ћетод <»нформатор::ѕолучить¬рем€ѕросто€—истемы> не удалось выполнить GetProcAddress");

		if (!SUCCEEDED((*pGetLastInputInfo)(&Info)))
	#else
		if (!SUCCEEDED(GetLastInputInfo(&Info)))
	#endif
			RuntimeError("ћетод <»нформатор::ѕолучить¬рем€ѕросто€—истемы> - системна€ ошибка %s", (LPCSTR)strLastError());
		
	RetVal = allTime - Info.dwTime;

	return TRUE;
}

BOOL CInformer::funcGetAddInDllTable(CValue &RetVal, CValue **ppValue)
{
	RetVal.CreateObject("“аблица«начений");
	CValueTable* pVT = ((CValueTableContextData*)(RetVal.GetContext()->GetInternalData()))->GetValueTable();
	
	const int iColCount = 5;

	CString str;
	const CType ctNum(NUMBER_TYPE_1C);
	const CType ctStr(STRING_TYPE_1C);

	pVT->Clear(TRUE);
	pVT->AddColumn(str = "Number",		ctNum, str, 0, "", 0);
	pVT->AddColumn(str = "»м€‘айла",	ctStr, str, 0, "", 0);
	pVT->AddColumn(str = "ѕуть‘айла",	ctStr, str, 0, "", 0);
	pVT->AddColumn(str = "»м€ќбъекта",	ctStr, str, 0, "", 0);
	pVT->AddColumn(str = "ѕуть–еестра",	ctStr, str, 0, "", 0);

	vector<CValue> pv(iColCount);
	vector<CValue*> ppv(iColCount);
	for (int i = 0; i < iColCount; ++i)
		ppv[i] = &pv[i];
	
	CLoadedAddInDllInfo::CPtrArray& PtrArray = CLoadedAddInDllInfo::GetLoadedAddInDllInfoArray();
	int sz = PtrArray.GetSize();
	for(i = 0; i < sz; i++)
	{
		CLoadedAddInDllInfo* pAddInContext = PtrArray.GetAt(i);

		pv[0] = static_cast<long>(i);
		CString FilePath = pAddInContext->m_strPath;
		pv[2] = FilePath;

		FilePath.Replace(ExtractFileDir(FilePath), "");
		pv[1] = FilePath;
		
		if (!pAddInContext->m_PtrArray.GetSize()){
			pv[3] = "";
			pv[4] = "";
		}
		else{
			CAddInDll_SEvent* pAddInDll_SEvent = pAddInContext->m_PtrArray.GetAt(0);
			CAddInDll_SSubEvent& SubEvent = pAddInDll_SEvent->m_SubEvent;

			pv[3] = SubEvent.m_Str1;
			pv[4] = SubEvent.m_Str2;
		};
		pVT->AddRow(&ppv[0], -1);
	}

	return TRUE;
}

BOOL CInformer::funcGetCurrentModuleText(CValue &RetVal, CValue **ppValue)
{
	CBLModule* pMod = CBLModule::GetExecutedModule();
	RetVal = pMod->GetInternalData()->mSource;
	return TRUE;
}

// к сожалению, пр€мой dynamic_cast<CGroupContext*>(pCont); не работает, выбрасывает исключение
CGroupContext* safe_dynamic_cast_CGroupContextPtr(CBLContext* pCont)
{
	if (!IS_KINDOF_CGroupContext(pCont))
		return NULL;
	
	return static_cast<CGroupContext*>(pCont);
}

BOOL CInformer::funcGetModuleText(CValue &RetVal, CValue **ppValue)
{
	CBLContext* pCont = (*ppValue)->GetContext();
	if (!pCont)
		RuntimeError("Ќужно передавать агрегатное значение");
	
	CGroupContext* pGroupContext = safe_dynamic_cast_CGroupContextPtr(pCont);
	if (NULL != pGroupContext)
	{
		CBLModule& mod = *GetModuleByContext(pGroupContext);
		RetVal = mod.GetInternalData()->mSource;
		return TRUE;
	}
	
	CComponentClass* pUserClass = safe_dynamic_cast<CComponentClass*>(pCont);
	if (NULL != pUserClass)
	{
		CBLModule& mod = *pUserClass->GetModule();
		RetVal = mod.GetInternalData()->mSource;
		return TRUE;
	}
	
	RuntimeError("»нформатор::ѕолучить“екстћодул€ работает только с классами или групповыми контекстами");
	return TRUE;
}