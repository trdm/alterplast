// ExecuteModule.cpp: implementation of the CExecuteModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ExecuteModule.h"
//#include "CodeDecodeFile.h"
#include "RuntimeException.h"

#include ".\\System\\Informer.h"

#include "BLModuleWrapper.h"
#include "MetaDataOfClasses.h"

IMPLEMENT_DYNCREATE(CExecuteModule, CBLContext);

class CObjID CExecuteModule::ObjID;

struct CExecuteModule::paramdefs CExecuteModule::defFnNames[] = {
	{{"Start", "Старт"},0,0},
	{{"AssignSource", "УстановитьМодуль"},0,1},
	{{"AssignContext", "НазначитьКонтекст"},0,1},
	{{"Compile", "КомпилироватьМодуль"},1,0},
	{{"Execute", "ВыполнитьМодуль"},1,0},
	{{"AssignFriendModule", "ПрисоединитьТекущийМодуль"},0,0},
	{{"HasRetVal", "ВозвращаетЗначение"},1,2},
	{{"GetDescr", "ПолучитьОписание"},0,2},
	{{"GetTextCurModules", "ПолучитьТекстТекМодуля"},1,1},
	{{"WorkWithMD", "РаботаМД"},0,0},
	{{"GetInfo", "ПолучитьИнформацию"},0,1},
	{{"GetMethOfContext", "GetMethOfContext"},0,3},
	{{"RaiseError", "СформироватьОшибку"},0,1},
	{{"ErasePostState","ОчиститьПозициюПроведения"},0,0},  
	{{"RestorePostState","ВосстановитьПозициюПроведения"},0,0},    
	{{"ExInfo","ExInfo"},0,0},      
	{{"InfoAboutCurExeMeth","InfoAboutCurExeMeth"},0,0},      
	{{"GetExeption","ПолучитьИсключение"},1,0},
	{{"Throw","ВыброситьИскл"},0,2},  
	{{"GetProcInfo","ПолучитьИнформациюОПроцедуре"},1,4},  
	{{"CompileAndExecute", "КомпилироватьИВыполнитьМодуль"},0,0},
	{{"EnableExceptions", "РазрешитьИсключения"},0,0},
	
	{{"CallContextProcedure", "ВыполнитьПроцедуруКонтекста"}, 1,3},
	{{"CallContextFunction", "ВыполнитьФункциюКонтекста"}, 1,4},

	{{"EnableInsideCalling", "РазрешитьВложенныйВызовМетодов"}, 0,0},
	{{"GetModuleText", "ПолучитьТекстМодуля"}, 1,0},

	{NULL,NULL,0,0}
};

CExecuteModule::CExecuteModule():CBLContext(0), 
	//pBLModule (new CBLModuleWrapper("", NULL, NULL, NULL)),
	pBLModule (CBLModuleWrapper::CreateSimpleModule()), // artbear kms TODO
	pPoint(NULL), m_bInsideMethodCall(false), m_bEnableInsideCalling(false)
{
}

CExecuteModule::~CExecuteModule()
{
}

int  CExecuteModule::CallAsFunc(int iMethNum,CValue & rValue,CValue * *ppValue)
{
	if (!m_bEnableInsideCalling && m_bInsideMethodCall && IDFuncGetExeption != iMethNum)
		RuntimeError("Для класса ВыполняемыйМодуль рекурсивные вызовы запрещены");

	CFlagGuard guard(this->m_bInsideMethodCall, true);
	
	if (IsUserMethod(iMethNum)) 
		return CallUsersMethod(true, iMethNum, &rValue, ppValue);

	switch(iMethNum)
	{
	case methCompile:
		return funcCompile(rValue);

	case methExecute:
		return funcExecute(rValue);

	case methHasRetVal:
			funcHasRetVal(*ppValue[0], *ppValue[1], rValue);
			return TRUE;

	case methGetTextModules:
		return TRUE;

	case IDFuncGetExeption:
		return funcGetExeption(rValue);

	case IDFuncGetProcInfo:
		funcGetProcInfo(ppValue, rValue);
		return TRUE;

	case iFuncCallContextProcedure:
	{
		bool bRes = CallContextProcedure(*ppValue[0], *ppValue[1], *ppValue[2]);
		rValue = bRes ? 1L : 0L; 
		return TRUE;
	}

	case iFuncCallContextFunction:
	{
		bool bRes = CallContextFunction(*ppValue[0], *ppValue[1], *ppValue[2], *ppValue[3]);
		rValue = bRes ? 1L : 0L; 
		return TRUE;
	}
	
	case iFuncGetModuleText:
	{
		CString strModuleText = GetModuleText();
		rValue = strModuleText;
		return TRUE;
	}

	default:
		return TRUE; //CallUsersMethod(true, iMethNum, &rValue, ppValue);
	};
	return TRUE;

  return TRUE;
}

void CExecuteModule::GetMethodOfContext(CValue * * ppValue) const
{
	CInformer::GetMethodOfContext(ppValue);
}

int  CExecuteModule::CallAsProc(int iMethNum,CValue * * ppValue)
{
	if (!m_bEnableInsideCalling && m_bInsideMethodCall)
		RuntimeError("Для класса ВыполняемыйМодуль рекурсивные вызовы запрещены");

	CFlagGuard guard(this->m_bInsideMethodCall, true);

	if (IsUserMethod(iMethNum)) 
		return CallUsersMethod(false, iMethNum, NULL, ppValue);

	switch(iMethNum)
	{
	case methStart:
		procStart();
		return TRUE;

	case methAssignSource:
		procAssignSource(ppValue[0]->GetString());
		return TRUE;

	case methAssignContext:
		procAssignContext(ppValue[0]->GetContext());
		return TRUE;

	case methAssignFriendModule:
		procAssignFriendModule();
		return TRUE;

	case methGetContextDescr:
		procGetContextDescr(ppValue);
		return TRUE;

	case methWorkWithMD:
		procWorkWithMD();
		return TRUE;

	case methGetInfo:
		procGetInfo(ppValue);
		return TRUE;

	case procGetContextMeth:
		procGetMethodOfContext(ppValue);
		return TRUE;

	case procRaiseError:
		RuntimeError(ppValue[0]->GetString());
		return TRUE;

	case IDProcErasePostState:
		procErasePostState();
		return TRUE;

	case IDProcRestorePostState:
		procRestorePostState();
		return TRUE;

	case IDProcExInfo:
		procExInfo();
		return TRUE;

	case IDProcInfoAboutCurExeMeth:
		procInfoAboutCurExeMeth();
		return TRUE;

	case IDProcThrow:
		procThrow(ppValue);
		return TRUE;

	case IDProcCompileAndExecute:
		procCompileAndExecute();
		return TRUE;

	case IDProcEnableExceptions:
		procEnableExceptions();
		return TRUE;

	case iProcEnableInsideCalling:
		procEnableInsideCalling();
		return TRUE;

	default:
		return TRUE; //	return CallUsersMethod(false, iMethNum, NULL, ppValue);
	};

	return TRUE;
}

int  CExecuteModule::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	//i		= pBLModule->FindFunc(lpMethodName);
	i = pBLModule->FindFunc(lpMethodName);
	if (i>=0) 
		return i+lastMethod;
	
	i = pBLModule->FindProc(lpMethodName,0);
	if(i>=0)
		return i+lastMethod;
	
	return -1;
}

char const *  CExecuteModule::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (IsUserMethod(iMethodNum)) 
	{
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(GetMethodNumberInUserModule(iMethodNum), ProcInfo); //artbear
		return ProcInfo.GetName();
	}

	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CExecuteModule::GetNMethods(void)const
{
	return pBLModule->GetNProcs() + lastMethod;
}

int  CExecuteModule::HasRetVal(int iMethodNum)const
{
	if (IsUserMethod(iMethodNum)) 
	{
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(GetMethodNumberInUserModule(iMethodNum),ProcInfo);
		return ProcInfo.HasRetVal();
	}

	return defFnNames[iMethodNum].HasReturnValue ;
}

int CExecuteModule::GetNParams(int iMethodNum)const
{
	if (IsUserMethod(iMethodNum)) 
	{
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(GetMethodNumberInUserModule(iMethodNum),ProcInfo);
		return ProcInfo.GetNParams();
	} 

	return defFnNames[iMethodNum].NumberOfParams;
}

int  CExecuteModule::GetParamDefValue(int iMethodNum,int iParamNum,CValue * pDefValue)const
{
	if (IsUserMethod(iMethodNum))
		return TRUE;

	if (IDProcThrow == iMethodNum && iParamNum == 1)
    {
        *pDefValue = "";
        return TRUE;
    }
    else if (IDFuncGetProcInfo == iMethodNum && (iParamNum == 2 || iParamNum == 3))
    {
        pDefValue->Reset();
        return TRUE;
    }
	else if (procGetContextMeth == iMethodNum && (iParamNum == 1 || iParamNum == 2))
	{
		*pDefValue = 0L;
		return TRUE;
	}
    
	return FALSE;
}

char const *  CExecuteModule::GetCode(void)const
{
	return 0;
}

int  CExecuteModule::GetDestroyUnRefd(void)const
{
	return 1;
}

class CObjID   CExecuteModule::GetID(void)const
{
	return ObjID;
}

long  CExecuteModule::GetTypeID(void)const
{
	return 100;
}

char const *  CExecuteModule::GetTypeString(void)const
{
	return "ВыполняемыйМодуль";
}

CType   CExecuteModule::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}

int  CExecuteModule::IsExactValue(void)const
{
	return 0;
}

int  CExecuteModule::IsOleContext(void)const
{
	return 0;
}

int  CExecuteModule::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CExecuteModule::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CExecuteModule::IsSerializable(void)
{
	return 0;
}

int  CExecuteModule::SaveToString(CString & csStr)
{
	csStr = "DJK";
	return 1;
}

int  CExecuteModule::GetNProps(void)const
{
	return pBLModule->GetNStaticVars();
}

char const *  CExecuteModule::GetPropName(int A,int B)const
{
	CBLVarInfo VarInfo;
	pBLModule->GetStaticVarDescr(A,VarInfo);
	return VarInfo.GetName();
}

int  CExecuteModule::GetPropVal(int iPropNum,CValue & rValue)const
{
	return pBLModule->GetStaticVarValue(iPropNum,rValue,0);
}

int  CExecuteModule::SetPropVal(int iPropNum,CValue const & vValue)
{
	return pBLModule->SetStaticVarValue(iPropNum,vValue,0);
}

int  CExecuteModule::FindProp(char const * Name)const
{
	return pBLModule->FindStaticVar(Name);
}

CString CExecuteModule::GetExecuteMethInfo(CBLModule *pModule)
{
	int res = 0;        
	int threePar = 0;
	CString strRet;
	
	if (pModule)
	{
		CBLProcInfo procInfo;
		
		res = pModule->GetCallLevelProcInfo(pModule->GetCallStackDepth()-1, procInfo, threePar);
		if (res != 0)
		{    
			CString strInfoPars;      
			for (int i = 0; i < procInfo.GetNParams(); ++i)
			{
				CBLVarInfo VarInfo;
				CString strInfoPar;
				procInfo.GetVarDescr(i, VarInfo);
				CValue ValRes;
				CValue *masVal[1] = {0};
				pModule->EvalExpr(VarInfo.GetName(),ValRes, masVal);            
				CString strType(ValRes.GetTypeString());
				
				if (ValRes.GetContext() == NULL)
					if (strType.Find("Справочник") != -1   || strType.Find("Документ") != -1 || strType.Find("Перечисление") != -1 )
					{              
						long lll = 0;                
						strType = pMetaDataCont->GetFullName(ValRes.GetRealTypeID(), &lll, 1);                 
					}
					else if (strType.Find("Referens") != -1 || strType.Find("Document") != -1 || strType.Find("Enum") != -1)
					{
						long lll = 0;                
						strType = pMetaDataCont->GetFullName(ValRes.GetRealTypeID(), &lll, 0);
					} 
					
					strInfoPar.Format("%s %s=%s",strType.operator LPCTSTR(), VarInfo.GetName(), ValRes.GetString().operator LPCTSTR());                        
					if (procInfo.GetNParams() > i + 1)
						strInfoPar += ", ";              
					strInfoPars += strInfoPar;
			}    
			strRet.Format("%s(%s)", procInfo.GetName(), strInfoPars.operator LPCTSTR());
		}
	}
	return strRet;
}

// без дополнительной проверки параметров
bool CExecuteModule::CallContextMethod(bool bIsFunction, CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont, CValue* pRetValue) const
{
	// TODO добавить кеширование последнего вызванного контекста и имени функции для ускорения

	int iMethodNum = pCont->FindMethod(strMethodName);
	if(-1 == iMethodNum)
		return false;

	if (!bIsFunction && pCont->HasRetVal(iMethodNum))
		RuntimeError("Метод %s существует, но является функцией, а пытались вызвать процедуру", strMethodName.operator LPCTSTR());
	if (bIsFunction && !pCont->HasRetVal(iMethodNum))
		RuntimeError("Метод %s существует, но является функцией, а пытались вызвать процедуру", strMethodName.operator LPCTSTR());

	int iMethodParams = pCont->GetNParams(iMethodNum);
	CCallWithValueListGuard guard(iMethodParams, pValueListCont);

	int iRes = 0;
	if (bIsFunction)
	{
		ASSERT(pRetValue);
		iRes = pCont->CallAsFunc(iMethodNum, *pRetValue, guard.GetData());
	}
	else
		iRes = pCont->CallAsProc(iMethodNum, guard.GetData());

	return iRes ? true : false;

}

bool CExecuteModule::CallContextProcedure(CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont) const
{
	return CallContextMethod(false, pCont, strMethodName, pValueListCont);
}

bool CExecuteModule::CallContextFunction(CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont, CValue& RetValue) const
{
	return CallContextMethod(true, pCont, strMethodName, pValueListCont, &RetValue);
}

// с дополнительной проверкой параметров
bool CExecuteModule::CallContextMethod(bool bIsFunction, const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue* pRetValue) const
{
	ValidateParamContext(ValueContext, CString("Класс <")+GetTypeString()+"> - первым параметром должен быть передан контекст");
	ValidateParamNotEmptyString(ValueMethodName, CString("Класс <")+GetTypeString()+"> - второй параметр должен быть строкой");
	ValidateParamValueListContext(ValueArgList, CString("Класс <")+GetTypeString()+"> - третий параметр должен быть списком значений");

	CValueListContext* pValueListContext = static_cast<CValueListContext*>(ValueArgList.GetContext());
	if (bIsFunction)
		return CallContextMethod(true, ValueContext.GetContext(), ValueMethodName.GetString(), pValueListContext, pRetValue);
	else
		return CallContextMethod(false, ValueContext.GetContext(), ValueMethodName.GetString(), pValueListContext);
}

bool CExecuteModule::CallContextProcedure(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList) const
{
	return CallContextMethod(false, ValueContext, ValueMethodName, ValueArgList);
}

bool CExecuteModule::CallContextFunction(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue& RetValue) const
{
	return CallContextMethod(true, ValueContext, ValueMethodName, ValueArgList, &RetValue);
}

CCallWithValueListGuard::CCallWithValueListGuard(int iMethodParamsA, CValueListContext* pValListContA) :
	m_iMethodParams(iMethodParamsA), m_pValListCont(pValListContA), 
	m_iNumArgs(0)
{
	if(m_iMethodParams > 0)
	{
		CValueListContextData* pVLCD = static_cast<CValueListContextData*>(m_pValListCont->GetInternalData());
		PValueItemArrayT *pValueItemArray = reinterpret_cast<PValueItemArrayT*>(pVLCD->GetValueList());
	
		if(m_iMethodParams >= pValueItemArray->GetSize()) 
			m_iNumArgs = pValueItemArray->GetSize();
		else 
			m_iNumArgs = m_iMethodParams;
		
		ppValues.resize(max(m_iMethodParams, m_iNumArgs));

		for(int i=0; i<m_iNumArgs; ++i) ppValues[i] = pValueItemArray->GetAt(i);

		for(int i=m_iNumArgs; i<m_iMethodParams; ++i)
		{
			sh_ptr<CValue> pNewValue(new CValue);
			AddedValuesVector.push_back(pNewValue);
 			ppValues[i] = pNewValue.get();
		}
	}

}
CCallWithValueListGuard::~CCallWithValueListGuard()
{
}

int CExecuteModule::CallUsersMethod( bool bIsFunc, int iMethNum, CValue* rValue, CValue ** ppValue )
{
	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	CBLProcInfo ProcInfo;
	pBLModule->GetProcInfo(GetMethodNumberInUserModule(iMethNum),ProcInfo);
	int j = ProcInfo.GetNParams();

	BOOL iRes = FALSE;

	if (!bIsFunc)
		iRes = pBLModule->CallAsProc(GetMethodNumberInUserModule(iMethNum), j, ppValue);
	else
		iRes = pBLModule->CallAsFunc(GetMethodNumberInUserModule(iMethNum), *rValue, j, ppValue);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	return iRes;
}

BOOL CExecuteModule::funcCompile( CValue &rValue )
{
	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	rValue = pBLModule->Compile();

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return TRUE;
}

BOOL CExecuteModule::funcExecute( CValue &rValue )
{
	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	rValue = pBLModule->Execute();

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	
	return TRUE;
}

void CExecuteModule::funcHasRetVal( const CValue& ObjectContextValue, const CValue& MethodNameValue, CValue &rValue )
{
	CBLContext* pContext = ObjectContextValue.GetContext();

	if (!pContext)
	{
		int iAnswerNotFound = -1;
		rValue = CNumeric(iAnswerNotFound); // невозможно определить
		return;
	}

	int iNumMethod = pContext->FindMethod(MethodNameValue.GetString());
	if (iNumMethod != -1)
	{
		int iAnswer = pContext->HasRetVal(iNumMethod);
		rValue = CNumeric(iAnswer);
	}
}

int CExecuteModule::funcGetExeption( CValue & rValue )
{
	CRuntimeExceptionClass::GetException(rValue);
	return TRUE;
}

void CExecuteModule::funcGetProcInfo( CValue ** ppValue, CValue & rValue )
{
	int nIndex;
	if (ppValue[0]->GetNumeric().operator long())
		nIndex = pBLModule->FindProc(ppValue[1]->GetString(), 0);
	else
		nIndex = pBLModule->FindFunc(ppValue[1]->GetString());
	if (nIndex == -1)
		rValue = 0L;
	else
	{
		rValue = 1;
		CBLProcInfo ProcInfo;
		if (pBLModule->GetProcInfo(nIndex, ProcInfo))
		{
			*ppValue[2] = ProcInfo.GetNParams();
			*ppValue[3] = ProcInfo.IsExported();
		}
	}
}

void CExecuteModule::procStart()
{
	ULONG first = CBLContext::GetFirstLoadedContextID();

	for (ULONG ind = first; ind != 0; ind = CBLContext::GetNextLoadedContextID(ind))
	{
		CBLContext* pCont = CBLContext::GetLoadedContext(ind);
		if (pCont)
		{
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

			for (int i = 0; i < nProps; i++)
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

void CExecuteModule::procAssignSource( const CString& strModuleText )
{
	pBLModule->AssignSource(strModuleText);
}

void CExecuteModule::procAssignContext( CBLContext* pCont )
{
	pBLModule->AssignContext(pCont);
}

void CExecuteModule::procGetContextDescr( CValue * * ppValue )
{
	CValue* pValue = ppValue[0];
	CBLContext* pContext = pValue->GetContext();
	int iNumMethod = pContext->FindMethod("AddValue");
	if (iNumMethod != -1)
	{
		int i;
		for (i = 0;i<lastMethod;i++)
		{
			pValue = new CValue(defFnNames[i].Names[ppValue[1]->GetNumeric()]);
			pContext->CallAsProc(iNumMethod, &pValue);
		}
	}
	ppValue[0]->AssignContext(pContext);

	long lId = CBLModule::GetFirstLoadedModuleID();

	char buff[200];
	sprintf(buff,"Id loaded module = %d",lId);
	pBkEndUI->DoMessageLine(buff,mmInformation);
	CBLModule* pModule = CBLModule::GetLoadedModule(lId);
	//pBLModule = CBLModule::GetExecutedModule();
	//pBLModule->ResetExecuted();
	while(lId = CBLModule::GetNextLoadedModuleID(lId))
	{
		sprintf(buff,"Id loaded module = %d",lId);
		pBkEndUI->DoMessageLine(buff,mmInformation);
	}
}

void CExecuteModule::procAssignFriendModule()
{
	pBLModule->AssignFriendModule(CBLModule::GetExecutedModule());
}

void CExecuteModule::procWorkWithMD()
{
	//CMetaDataCont - класс для работы с объектом MD
	//pMetaDataCont - указатель на текущий MD сеанса
	//CDocDef - класс описания документа

	CMetaDataObjArray *pMetaDataObjArray = pMetaDataCont->GetDocDefs();

	if (pMetaDataObjArray == NULL) {
		pBkEndUI->DoMessageLine("Неудачная попытка получить описание документов", mmExclamation3);	
		return;
	}

	int countDefDoc = pMetaDataObjArray->GetNItems();
	for (int ind = 0; ind < countDefDoc; ind++) {
		CMetaDataObj *pMetaDataObj = pMetaDataObjArray->operator[](ind);
		if (pMetaDataObj == NULL) continue;

		// char const* GetCode() - получить строку с идентификатором объекта
		// char const* GetDescr() - получить строку с комментарием объекта
		// char const* GetPresent() - получить строку с синонимом объекта

		pBkEndUI->DoMessageLine(pMetaDataObj->GetCode(), mmInformation);
		pBkEndUI->DoMessageLine(pMetaDataObj->GetDescr(), mmInformation);
		long lIDObject = pMetaDataObj->GetID();
		char buf[20];
		sprintf(buf, "Id = %d", lIDObject);
		pBkEndUI->DoMessageLine(buf, mmInformation);
		pBkEndUI->DoMessageLine(pMetaDataObj->GetPresent(), mmInformation);

		//_itoa(objDocDef.GetDocNumDefID(), buf, 10);
		CDocDef *pDocDef = pMetaDataCont->GetDocDef(pMetaDataObj->GetCode());
		if (pDocDef == NULL) {
			pBkEndUI->DoMessageLine("Неудачная попытка получить описание документа", mmExclamation3);	
			break;
		}

		_itoa(pDocDef->GetBookKeepingEnable(), buf, 10);
		pBkEndUI->DoMessageLine(buf, mmInformation);
		pBkEndUI->DoMessageLine("======================", mmInformation);
	}
	//CBLContext* pValueList = CBLContext::CreateInstance("ValueList");
	CBLContext* pValueList = CBLContext::CreateInstance("Справочник.Новый1");
	pBkEndUI->DoMessageLine(pValueList->GetRuntimeClass()->m_lpszClassName, mmInformation);
}

void CExecuteModule::procGetInfo( CValue * * ppValue )
{
	const CString& c_str = ppValue[0]->GetString();
	pBkEndUI->DoMessageLine(pMetaDataCont->GetRegDef(c_str)->GetRegActTableName(), mmInformation);
	pBkEndUI->DoMessageLine(pMetaDataCont->GetRegDef(c_str)->GetRegTableName(), mmInformation);
	//class CBLContext *pBL = ppValue[0]->GetContext();
	//if (pBL != NULL) {		      
	//}
	CBLModule * pBLM = CBLModule::GetExecutedModule();	//53
}

void CExecuteModule::procGetMethodOfContext( CValue * * ppValue )
{
	try{
		GetMethodOfContext(ppValue);
		return;
	}
	catch(...){
		RuntimeError("Ошибка получения методов и свойств контекста");
	}
}

void CExecuteModule::procErasePostState()
{
	try
	{
		unsigned char **ppPoint = reinterpret_cast<unsigned char **>(0X2A133620);
		pPoint = *ppPoint;
		if (pPoint)
		{
			*ppPoint = NULL;
		}
	}
	catch (...)
	{
		RuntimeError("В ErasePostState() не удалось записать в память по адресу 0X2A133620");	
	}
}

void CExecuteModule::procRestorePostState()
{
	if (pPoint)
	{
		try
		{
			unsigned char **ppPoint = reinterpret_cast<unsigned char **>(0X2A133620);
			*ppPoint = pPoint;
			pPoint   = NULL;
		}
		catch (...)
		{
			RuntimeError("В RestorePostState() не удалось записать в память по адресу 0X2A133620");	
		}
	}      
}

void CExecuteModule::procExInfo()
{
	// artbear метод ExInfo недокументирован, но используется как внешний метод при работе препроцессора 1С++ //#exinfo

	CComponentClass* pCompClass = CComponentClass::GetComponentClassCurExModule();
	if (pCompClass)
	{
		CBLModule *pMod = pCompClass->GetHostModule();
		// Узнаем принадлежит ли этот модуль классу или это просто модуль 1С
		pCompClass = NULL;
		pCompClass = CComponentClass::GetComponentClassCurExModule(pMod);

		CString strCode("",500);
		int nLineNum = pMod->GetExecutedLineNum();
		pMod->GetCurSourceLine(nLineNum, strCode);
		strCode.TrimLeft(); strCode.TrimRight();

		CString strInfo;
		CString strFullName;
		if (pCompClass)
		{
			// Модуль принадлежит классу          
			strFullName = pCompClass->GetTypeString();
			strFullName += " = ";
			strFullName += ISetOfHierarchy::GetMetaDataOfClasses().GetNameOfFile(pCompClass->GetTypeString());
			nLineNum++;
		}
		else
		{
			// Это 1С модуль
#ifndef _DEBUG
			strFullName = static_cast<CBLModule7*>(pMod)->GetFullName();          
#else
			strFullName = "Путь к модулю 1С в DEBUG mode определять нельзя";
#endif
		} 
		strInfo.Format("{%s(%d)}",strFullName.operator LPCTSTR(),nLineNum);
		pBkEndUI->DoMessageLine(strCode, mmInformation);	        
		pBkEndUI->DoMessageLine(strInfo, mmInformation);	        
	}
}

void CExecuteModule::procInfoAboutCurExeMeth()
{
	// artbear метод InfoAboutCurExeMeth недокументирован, но используется как внешний метод при работе препроцессора 1С++ //#curmeth

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();      
	CComponentClass* pCompClass = CComponentClass::GetComponentClassCurExModule();
	if (pCompClass)
	{        
		CString strRes = pCompClass->GetTypeString();
		strRes += "::";
		strRes += GetExecuteMethInfo(pCompClass->GetModule());
		pBkEndUI->DoMessageLine(strRes, mmInformation);	                          
	}
	/*CArray<CBLModule7*,CBLModule7*> *rModArr = (CArray<CBLModule7*,CBLModule7*>*)0x22132C98;
	for (int i = 0; i < rModArr->GetSize(); i++)
	{
	pBkEndUI->DoMessageLine(rModArr->GetAt(i)->GetFullName(), mmInformation);	                
	}*/
}

void CExecuteModule::procThrow( CValue * * ppValue )
{
	CRuntimeExceptionClass::RuntimeError(*ppValue[0], ppValue[1]->GetString());
}

void CExecuteModule::procCompileAndExecute()
{
	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	const int res = pBLModule->Compile();

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	if (!res)
		//throw CString("Модуль не скомпилирован");
		return; // TODO скорее всего, не нужно.
	
	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	const int res1 = pBLModule->Execute();

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	//if (!res1)
	//	throw CString("Модуль не выполнен");
		return; // TODO скорее всего, не нужно.
}

void CExecuteModule::procEnableExceptions() // только для совместимости
{
}

void CExecuteModule::procEnableInsideCalling()
{
	m_bEnableInsideCalling = true;
}

CString CExecuteModule::GetModuleText() const
{
	return pBLModule->GetInternalData()->mSource;
}
