// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "ComponentClass.h"

//#include "System\GroupContextWrap.h"
#include "System\DllHooker.h"

//extern CMetaDataCont * pMetaDataCont;

// ======================================================================
//
// begin of CDebug
//

CComponentClass* CComponentClass::CDebug::m_CurDebugObject = NULL;

void CComponentClass::CDebug::ClearDebugObject()
{
  m_CurDebugObject = NULL;
}

void CComponentClass::CDebug::SetDebugObject(CComponentClass *pComp)
{
  if (pMainAddIn->m_EnableDebugging)
    m_CurDebugObject = pComp;
}


void CComponentClass::CDebug::Release(void)
{
  if (IsDebug())
  {
    int n = pDebugContext->FindProp("Form");
    if (n != -1)
    {
	  CHookGroupContext::EnableToFormClose(pDebugContext);

      CValue cont1;
      pDebugContext->GetPropVal(n, cont1);
      CBLContext *pCont = cont1.GetContext();
      if (pCont)
      {
        n = pCont->FindMethod("Close");
        if (n != -1)
        {
          CValue par1(1);
          CValue *massPar[1];
          massPar[0] = &par1;
          pCont->CallAsProc(n, massPar);

        }
      }
    }
	if (pDebugContext)
	{
		pDebugContext->DecrRef();
	}
  }
}

void CComponentClass::CDebug::OpenDebugForm(CComponentClass* pComponent)
{
	if(!pMainAddIn->m_EnableDebugging)
		return;

	//CString m_NameOfInstance = pComponent->m_NameOfInstance;
	CString m_NameOfInstance = pComponent->GetTypeString();

	CBLContext* pContLoaded = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts("OpenForm", pContLoaded);
	if (-1 == nMethNum)
		return;

	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CString str = pHierarchy->GetNameOfFile(m_NameOfInstance);
	CSetOfHierarchy::MakeFullFileName(&str);

	CString strPar1;
	if (0 == str.Right(4).CompareNoCase(".ert"))
		strPar1 = "Отчет#";
	else if (0 == str.Right(3).CompareNoCase("@md"))
	{
		strPar1 = "Обработка.";
		strPar1 += str.Left(str.GetLength() - 3);
		strPar1 += "#";
	}
	else
		return;

	// artbear отладка возможна только для внешних/встроенных отчетов/обработок
	// иначе выдается дурацкое, неудобное предупреждение
	// artbear - при открытии формы класса в виде группового контекста директива _NOW_PREPARE_CLASS выключена
	// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2505

	const char* cstr_NOW_PREPARE_CLASS = "_NOW_PREPARE_CLASS";
	void *pV = NULL;

	CMapStringToPtr &m_RefMap = pHierarchy->m_SymbolOfPreprocessor;

	bool bEnable_NOW_PREPARE_CLASS_Prev = m_RefMap.Lookup(cstr_NOW_PREPARE_CLASS, (void*&)pV) > 0 ? true : false;

	m_RefMap[cstr_NOW_PREPARE_CLASS] = NULL;

	pHierarchy->SetIsOpenCompForm(true);

	CValue retval;
	CValue Par1 = strPar1;
	CValue Par2;
	Par2.SetValTypeID(100);
	Par2.AssignContext(pComponent);


	CValue Par3 = str;

	CValue* MasPar[6] = {&Par1, &Par2, &Par3, 0, 0, 0};

	try
	{
		pContLoaded->CallAsFunc(nMethNum, retval, &MasPar[0]);
		pDebugContext = Par2.GetContext(); // sedmin

		CHookGroupContext::DisableToFormClose(pDebugContext);
	}
	catch(...)
	{
		::ShowMsg("Не удалось открыть форму класса: %s", mmBlackErr, m_NameOfInstance);
	}

	pHierarchy->SetIsOpenCompForm(false);

	if (bEnable_NOW_PREPARE_CLASS_Prev)
		m_RefMap.SetAt(cstr_NOW_PREPARE_CLASS, pV);
	else
		m_RefMap.RemoveKey(cstr_NOW_PREPARE_CLASS);
}

//void CComponentClass::CDebug::AssignDebugModule(CBLContext* pComponent, CBLModule*& pMod)
void CComponentClass::CDebug::AssignDebugModule(CBLContext* pComponent, CSafeModulePtr& pMod)
{
	if (IsDebug())
	{
//		if (pMod != NULL)
//			delete pMod;

		pMod = NULL;
		pMod = GetModuleByContext((CGroupContext*)(CBLContext*)pDebugContext);

		if (!pMod)
			return;

		pDebugContext->IncrRef();
		pMod->AssignContext(m_CurDebugObject);

		pComponent->DecrRef();
	}
}

//
// ======================================================================

// ======================================================================
//
// begin of CRuntimeExceptionClass
//

bool CRuntimeExceptionClass::isExceptionStatus = false;
CString CRuntimeExceptionClass::strTextException;
CValue* CRuntimeExceptionClass::ExceptionValue = NULL;

//CBLModule7* CRuntimeExceptionClass::pMod = NULL;
CSafeModulePtr CRuntimeExceptionClass::pMod;

int CRuntimeExceptionClass::iRuntimeErrLineNum = 0;
CString CRuntimeExceptionClass::m_strModulePath;
DWORD CRuntimeExceptionClass::m_nID = 0;
bool CRuntimeExceptionClass::m_bStatusOfTryingBlock = false; // флаг, находимся ли в блоке Попытка-Исключение

void CRuntimeExceptionClass::Init(void)
{
	ExceptionValue = new CValue;
	Empty();
}

void CRuntimeExceptionClass::Destroy(void)
{
  delete ExceptionValue;
}

void CRuntimeExceptionClass::Empty(void)
{
	isExceptionStatus = false;
	strTextException.Empty();

	pMod = NULL;
	iRuntimeErrLineNum = 0;
	m_strModulePath.Empty();
	m_nID = 0;

}

// запомнить исключение и данные о нем
void CRuntimeExceptionClass::SetException(const CString& strExceptionA)
{
  Empty(); // TODO

	isExceptionStatus = true;
	strTextException = strExceptionA;
}

void CRuntimeExceptionClass::SetException(const CString& strExceptionA, CBLModule7* pModA, DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA)
{
  // для вызова CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());

  SetException(strExceptionA);

  pMod = pModA;
  m_nID = m_nIDA;
  m_strModulePath = m_strModulePathA;
  iRuntimeErrLineNum = iRuntimeErrLineNumA;
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::ThrowException(bool isThrow)
{
	CString strTextException_copy(strTextException);

	CBLModule7* pMod_copy = (CBLModule7*)(CBLModule*)pMod; // TODO
	DWORD m_nID_copy = m_nID;

	CString m_strModulePath_copy( m_strModulePath );
	int iRuntimeErrLineNum_copy = iRuntimeErrLineNum;

  Empty();

  if (pMod_copy)
  {
	  // для каждого класса согласно иерархии создания будет показано отдельное сообщение об ошибке
	  // т.е. если открыты форма Форма1, в ней создан класс КОП1, в нем КОП2, будет показано 3 сообщения
	  // сначала для КОП2, далее КОП1, далее Форма1 - и двойной щелчок на каждом из сообщений при открытом Конфигураторе вызовет переход на соответствующий файл в Конфигураторе
	  //
	if (!GetStatusOfTryingBlock() && !IsInsideTryingBlock())
	{

		pMod_copy->OnErrorMessageEx(strTextException_copy, m_nID_copy, m_strModulePath_copy, iRuntimeErrLineNum_copy);

	}
	::RuntimeError(strTextException_copy) ;
  }
  else
  {
	if (isThrow)
		::RuntimeError(strTextException_copy) ;
	else
		::ShowMsg(strTextException_copy, mmExclamation);
  }
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::IfNeedExceptionThenThrowException(void)
{
  if (isExceptionStatus) ThrowException();
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::ThrowException(const CString& strException)
{
	SetException(strException);
	ThrowException();
}

// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет пустое
void CRuntimeExceptionClass::RuntimeError(const CValue& param)
{
	Empty();
  *ExceptionValue = param;
  ::RuntimeError("");
}

// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет равно указанному
void CRuntimeExceptionClass::RuntimeError(const CValue& param, const CString& strNewTextException)
{
	Empty();
  *ExceptionValue = param;
  ::RuntimeError(strNewTextException);
}

// получить ранее сохраненный объект-исключение
void CRuntimeExceptionClass::GetException(class CValue & rValue)
{
	rValue = *ExceptionValue;
	ExceptionValue->Reset();
}

// получить флаг, находимся ли в блоке Попытка-Исключение
void CRuntimeExceptionClass::SaveStatusOfTryingBlock(void)
{
	m_bStatusOfTryingBlock = IsInsideTryingBlock(); // флаг, находимся ли в блоке Попытка-Исключение
}

bool CRuntimeExceptionClass::GetStatusOfTryingBlock(void)
{
	return m_bStatusOfTryingBlock;
}

// проверить, не находимся ли мы в блоке Попытка-Исключение
bool CRuntimeExceptionClass::IsInsideTryingBlock(void)
{

	CBLModule* pCurModule = CBLModule::GetExecutedModule();
	if (!pCurModule)
		return false;
	if (!pCurModule->pIntInfo)
		return false;

	CExecutedModule* pExecutedModule = pCurModule->pIntInfo->pExecutedModule;
	if (!pExecutedModule)
		return false;

	// это разведал АльФ - огромное ему спасибо
	bool bRetInsideTryingBlock = (pExecutedModule->m_DWordArray2.GetSize() > pExecutedModule->val_23);

	// для классов - особая обработка, т.к. у них использованные выше параметры всегда нулевые
	CBLContext* pCont = CBLModuleWrapper::GetContextFromModule(pCurModule);

	if (pCont && !bRetInsideTryingBlock && IS_KINDOF_CComponentClass(pCont))
	{
		bRetInsideTryingBlock = m_bStatusOfTryingBlock;
	}

	return bRetInsideTryingBlock;
}

//
// end of CRuntimeExceptionClass
// ======================================================================

// ---------------------------------------------------------------------------------
//
// CComponentClass::CImplicitParams - работа с неопределенным числом параметров
//
// ---------------------------------------------------------------------------------
CComponentClass::CImplicitParams* CComponentClass::pImplicitParams = NULL;

CComponentClass::CImplicitParams::CImplicitParams()
{
  m_CountParametersInCallMethod = -1;
}

BOOL CComponentClass::CImplicitParams::LookupInBindInstanceValueList(const CComponentClass* pComp, const CString& m_strClassAndMethCurrent, InfoUnlimitsParams *&pInfo) const
{
  return pComp->GetBindInstanceValueList().Lookup(m_strClassAndMethCurrent, (void*&)pInfo);
}

int CComponentClass::CImplicitParams::GetNParams(const CComponentClass *pComp, int nNumMeth, int nNParams)
{
  InfoUnlimitsParams *pInfo = NULL;
  int nRes = m_CountParametersInCallMethod < 0 ? 0 : m_CountParametersInCallMethod;

//+ kms: deprecated code cut нах
/*
  int nRes = 0;

  if (m_CountParametersInCallMethod >= 0 && m_CountParametersInCallMethod < 30)
  {
    nRes = m_CountParametersInCallMethod;
  }
  else
  {
    if (LookupInBindInstanceValueList(pComp, m_strClassAndMethCurrent, pInfo))
      return pInfo->nNumAllParams;

    CString str("", 200);
    CBLModule *pCurMod = CBLModule::GetExecutedModule();
    CString strMethodName;
    int nPosColon = m_strClassAndMethCurrent.Find(':');
    if (nPosColon != -1)
    {
      int nLen = m_strClassAndMethCurrent.GetLength();
      strMethodName = m_strClassAndMethCurrent.Right(nLen - nPosColon - 1);
    }


    int nCountPairBrackets  = 0;

    int nNSourceLines =  pCurMod->GetNSourceLines();
    bool bNotCommas = true;
    bool isSearchOpenBracket = true;
    bool bFindMethName = false;

    CString strLexem;

    for (int i = pCurMod->GetExecutedLineNum(); i < nNSourceLines; i++)
    {
      pCurMod->GetCurSourceLine(i, str);
      str.TrimRight(); str.TrimLeft();
      str.Replace(" ", "");

      int pos = 0;
      if (!bFindMethName)
      {
        int nPosMethodName = str.Find(strMethodName);
        if (nPosMethodName != -1)
        {
          bFindMethName = true;
          pos = nPosMethodName + strMethodName.GetLength() - 1;
        }
        else
          continue;
      }

      int nLenStr = str.GetLength();
      for (; pos < nLenStr; pos++)
      {
        if (str[pos] == '\"' && bNotCommas)
          bNotCommas = false; // Символы в кавычках, необходим их пропустить
        else if (str[pos] == '\"' && !bNotCommas)
          bNotCommas = true;

        if (str[pos] == '/' && str[pos+1] == '/')
          break; // Дальше до конца строки идет комментарий, можно пропустить

        if (bNotCommas)
        {
          if (str[pos] == '(')
          {
            nCountPairBrackets++;
            isSearchOpenBracket = false;
          }
          else if (str[pos] == ')')
          {
            nCountPairBrackets--;
            if (nCountPairBrackets == 0)
              break;
          }
          else if (str[pos] == ',' && nCountPairBrackets < 2 && nCountPairBrackets > 0 && !strLexem.IsEmpty())
          {
            strLexem.Empty();
            nRes++;
          }
          else if (str[pos] != '\r' && str[pos] != '\n' && nCountPairBrackets != 0)
            strLexem += str[pos];

        }
      }
      if (nCountPairBrackets == 0 && !isSearchOpenBracket)
        break;
    }

    if (!strLexem.IsEmpty())
      ++nRes;

  }
*/

  if (nRes < nNParams)
    return nNParams;

  if (LookupInBindInstanceValueList(pComp, m_strClassAndMethCurrent, pInfo))
  {
    static const int nNMeth = pInfo->cont_ValueList->FindMethod("RemoveAll");
    CValue *Params[1] = {0};
    CValue par;
    Params[0] = &par;
    pInfo->cont_ValueList->CallAsProc(nNMeth, Params);
  }
  else
  {
	  pInfo = new InfoUnlimitsParams;
    pInfo->cont_ValueList     = CBLContext::CreateInstance("ValueList");
    pComp->GetBindInstanceValueList()[m_strClassAndMethCurrent] = pInfo;
  }
  pInfo->nNumAllParams      = nRes;
  pInfo->nNumExplicitParams = nNParams;

  return nRes;
}

bool CComponentClass::CImplicitParams::GetImplicitParams(CComponentClass *pComp, CValue **ppPar, int nNumMeth)
{
	InfoUnlimitsParams *pInfo = NULL;

  if (LookupInBindInstanceValueList(pComp, m_strClassAndMethCurrent, pInfo))
  {
    m_strClassAndMethImplicit = m_strClassAndMethCurrent;
    pInfo->vecPointOfPars.clear();

    for (int i = 0; i < pInfo->nNumAllParams - pInfo->nNumExplicitParams; i++)
    {
      pInfo->vecPointOfPars.push_back(ppPar[i+pInfo->nNumExplicitParams]);
    }

    CStringArray ArrString;
    for (int ind = 0; ind < pInfo->nNumExplicitParams; ind++)
    {
      CString strType = ppPar[ind]->GetTypeString();
      if (strType.CompareNoCase("string") != 0)
      {
        CString strSourse = ppPar[ind]->GetString();
        if (!strSourse.IsEmpty())
        {
          if (strSourse[0] == '{' && strSourse[strSourse.GetLength()-1] == '}')
          {
            strSourse.Replace("{", "");
            strSourse.Replace("}", "");
            strSourse.Replace(" ", "");
            strSourse.Replace("\r", "");
            strSourse.Replace("\n", "");
            CString token;
            for (int n = 0; n < strSourse.GetLength(); n++)
            {
              if (strSourse[n] == ',' && !token.IsEmpty())
              {
                ArrString.Add(token);
                token.Empty();
              }
              else
                token += strSourse[n];
            }
            if (!token.IsEmpty())
              ArrString.Add(token);
            break;
          }
        }
      }
    }
    CBLContext *pCont = pInfo->cont_ValueList;
    ASSERT(pCont != NULL);

    const int nNMeth = pCont->FindMethod("AddValue");
    if (nNMeth != -1)
    {
      int nSizeArr = ArrString.GetSize();
      for (int i = pInfo->nNumExplicitParams; i < pInfo->nNumAllParams; i++)
      {
        CValue *Params[2] = {0};
        CValue par2;
        int nInd = i - pInfo->nNumExplicitParams;
        if (nInd < nSizeArr)
          par2 = ArrString[nInd];
        if ((ppPar[i]->IsEmpty())&&(ppPar[i]->GetTypeCode() == UNDEFINE_TYPE_1C))
        {
          CValue emptyVal("");
          ppPar[i]->MakeExactValueFrom(&emptyVal);
        }
        Params[0] = ppPar[i];
        Params[1] = &par2;
        pCont->CallAsProc(nNMeth, Params);
      }
      return true;
    }
  }
  return false;
}

//CComponentClass::InfoUnlimitsParams *
void* CComponentClass::CImplicitParams::GetImplicitParamsInfo(CComponentClass *pComp, const char* lpcsNameMeth) const
{
  if (!lpcsNameMeth)
    return 0;

  CComponentClass* pCompReal = CComponentClass::GetComponentClassCurExModule();
  if (!pCompReal)
    RuntimeError("Нельзя вызывать функцию GetParamsList в модуле не принадлежащему текущему классу!");

  if (pCompReal != pComp)
    pComp = pCompReal;

  CString str = pComp->GetTypeString();
  str += ":";
  str += lpcsNameMeth;

  InfoUnlimitsParams *pInfo = NULL;
  if (LookupInBindInstanceValueList(pComp, str, pInfo))
    return pInfo;
  else
    return NULL;

}


CBLContext* CComponentClass::CImplicitParams::GetImplicitParamsList(CComponentClass *pComp, const char* lpcsNameMeth)
{
	InfoUnlimitsParams *pInfo = (InfoUnlimitsParams *)GetImplicitParamsInfo(pComp, lpcsNameMeth);
	if (pInfo)
		return pInfo->cont_ValueList;
	else
		return NULL;
}

int CComponentClass::CImplicitParams::SetImplicitParamOfInd(CComponentClass *pComp, const char *lpcsNameMeth, int nIndex, CValue* pVal)
{
	InfoUnlimitsParams *pInfo = (InfoUnlimitsParams *)GetImplicitParamsInfo(pComp, lpcsNameMeth);
	if (pInfo)
	  {
		if (nIndex >  (pInfo->nNumAllParams - pInfo->nNumExplicitParams) || (nIndex < 1))
		  RuntimeError("Указан неверный индекс в параметре 2 метода УстановитьПараметрПоИндексу!");

		*pInfo->vecPointOfPars[nIndex-1] = *pVal;
		return 1;
	  }
	  else
		return 0;
}

// class CParamByValueArrayGuard
// {
// public:
// 	CParamByValueArrayGuard(CValue **ppParam, ARRAY_VAL& arrValOfMeth, int nParams) :
// 		m_ppParam(ppParam), m_arrValOfMeth(arrValOfMeth), m_nParams(nParams)
// 		{ Save(); }
//
// 	~CParamByValueArrayGuard() { Restore(); };
// 	void SetFlagDoNotRestore() { m_bDoNotRestore = false; };
//
// private:
// 	void Save();
// 	void Restore();
//
// 	bool m_bDoNotRestore;
// 	CValue **m_ppParam;
// 	ARRAY_VAL& m_arrValOfMeth;
// 	int m_nParams;
// };
//
// void CParamByValueArrayGuard::Save()
// {
//     nSize =  min(num,nSize);
//     int nNumParam = 0;
//
//     for (int i = 0; i < nSize; i++, nNumParam++)
//     {
//       CDataOfParamForUserMethods::Ptr NameBind = param->operator[](i+nPlus);
//       if (NameBind->bIsVal)
//       {
//         arrValOfMeth->Add(CValue_SharedPtr(new CValue(*(ppParam[i])))); // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
//       }
//       else
//         arrValOfMeth->Add(CValue_SharedPtr()); //NULL);
//     }
// }
//
// void CParamByValueArrayGuard::Restore()
// {
//   int nSize   = m_arrValOfMeth->GetSize();
//   int nS = min(m_nParams, nSize);
//
//   for(int i = 0; i < nS; i++)
//   {
//     //CValue* pVal = NULL;
//     CValue_SharedPtr pVal = m_arrValOfMeth.GetAt(i);
//     if (pVal)
//     {
//       *(m_ppParam[i]) = *pVal;
//       //delete pVal; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
//     }
//   }
//
// }

bool CComponentClass::CImplicitParams::SaveValParam(CComponentClass *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
{

  // m_strClassAndMethCurrent - устанавливается ранее GetNParams

  CParamDefs::CArrayOfMethodsParamData_PTR param;
  CMetaDataOfClasses& m_Meta = CSetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();

  if (m_Meta.LookupMethHaveParVal(m_strClassAndMethCurrent, param))
  {
    int num = pComponentClass->GetNParams(nNumMeth);
    int nSize = param->GetSize();
    int nPlus = 0;

    if (pComponentClass->HasRetVal(nNumMeth) == 1)
    {
      nPlus = 1;
      nSize--;
    }

    nSize =  min(num,nSize);
    int nNumParam = 0;

    for (int i = 0; i < nSize; i++, nNumParam++)
    {
      CMethodsParamData::Ptr NameBind = param->operator[](i+nPlus);
      if (NameBind->IsByVal())
      {
        arrValOfMeth->Add(CValue_SharedPtr(new CValue(*(ppParam[i])))); // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
      }
      else
        arrValOfMeth->Add(CValue_SharedPtr()); //NULL);
    }
    return true;
  }
  else
    return false;
}

void CComponentClass::CImplicitParams::RestoreValParam(CComponentClass *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
{

  int nParams = pComponentClass->GetNParams(nNumMeth);
  int nSize   = arrValOfMeth->GetSize();
  int nS = min(nParams, nSize);

  for(int i = 0; i < nS; i++)
  {
    //CValue* pVal = NULL;
    CValue_SharedPtr pVal = arrValOfMeth->GetAt(i);
    if (pVal)
    {
      *(ppParam[i]) = *pVal;
      //delete pVal; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
    }
  }
}

// класс с подсчетом ссылок
// НО после обнуления счетчика ссылок хранимый объект не удаляется, а хранится !!
// объекты с нулевым счетчиком ссылок удаляются спец.статическим методом
//
//	Удобно использовать для компиляции объектов
//
//template<class CType, class CKeyType>
// template<class CType>
// class CShared_Ptr_Owner
// {
//
// public:
// 	typedef CType* CPtr;
// 	//typedef const CType* CConstPtr;
//
// 	CShared_Ptr_Owner()//;
// 		: m_pSharedRes(NULL)
// 	{}
// 	CShared_Ptr_Owner(const CShared_Ptr_Owner& lp) //;
// 		: m_pSharedRes(lp)
// 	{
// 		++m_pSharedRes->m_RefCount;
// 	};
// 	CShared_Ptr_Owner(CPtr newPtr)//; // TODO
// 		: m_pSharedRes(new CSharedRes(newPtr))
// 	{
// 		++m_pSharedRes->m_RefCount;
// 	};
//
// 	~CShared_Ptr_Owner()//;
// 	{
// 		if (!--m_pSharedRes->m_RefCount)
// 			; // нужно сохранить
// 	}
//
// 	CShared_Ptr_Owner& operator=(const CShared_Ptr_Owner& rp)//;
// 	{
// 		if (!--m_pSharedRes->m_RefCount)
// 			; // нужно сохранить
// 		m_pSharedRes = rp;
// 		++m_pSharedRes->m_RefCount;
// 	}
// private:
// 	CShared_Ptr_Owner& operator=(CPtr rp); // запрещаю напрямую использовать !
// public:
// // 	operator bool() const;
// // 	operator !() const;
// // 	bool operator==(const CShared_Ptr_Owner& ) const;
// // 	bool operator!=(const CShared_Ptr_Owner& ) const;
//
// 	CPtr operator->() const//;
// 	{
// 		return m_pSharedRes->m_pointer;
// 	}
// 	CType& operator*() const//;
// 	{
// 		return *m_pSharedRes->m_pointer;
// 	}
//
// 	//operator CType*() const
// 	CPtr Get() const//;
// 	{
// 		return m_pSharedRes->m_pointer;
// 	}
//
// 	// удалить все объекты, даже те, на которые есть ссылки
// 	void RemoveAll(void);
//
// 	// удалить объекты, на которые нет ссылок
// 	void RemoveAllUnreferenced(void);
//
// private:
// 	struct CSharedRes
// 	{
// 		CPtr m_pointer;
// 		size_t m_RefCount;
// 		CSharedRes(CPtr newPtr) : m_pointer(newPtr), ++m_RefCount
// 		{}
// 		//~CSharedRes()
// 	};
// 	typedef CSharedRes* CSharedResPtr;
//
// 	CSharedRes* m_pSharedRes;
//};