// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ComponentClass.h"

#include "System\GroupContextWrap.h"

extern CMetaDataCont * pMetaDataCont;

class CObjID CComponentClass::ObjID;

class CMapDefMethod CComponentClass::MapDef;

CComponentClass* CComponentClass::pCreatedCompModule = NULL;
bool CComponentClass::bFlagCreateWithoutConstructor = false;

  // Карта с указателями на модули с их реальными контекстами
  // Ключ - указатель на модуль, Занчение - указателя на контекст CComponentClass
CMapPtrToPtr CComponentClass::m_MapOfModulesAndComp;

IMPLEMENT_DYNCREATE(CComponentClass, CBLContext)

class CDynamicPropertyManager
{
public:
// 	CDynamicPropertyManager();
// 	~CDynamicPropertyManager();

	BOOL IsEmpty() const { return Size() == 0;};
	int Size() const { return m_MapPropertyNumbers.GetCount(); };

	void Add(LPCTSTR sNewProperty, int iNewPropertyNumber);

	int FindNumberByName(LPCTSTR sProperty) const;
	LPCTSTR FindNameByNumber(const int iPropertyNumber) const;

	BOOL IsExist(const int iPropertyNumber) { return !FindNameByNumber(iPropertyNumber); };
protected:

private:
	CIStringMapToInt m_MapPropertyNumbers;
	vector<LPCTSTR> m_MapPropertyNames;
};

void CDynamicPropertyManager::Add(LPCTSTR sNewProperty, int iNewPropertyNumber)
{
	int iPropNumber = -1;
	if (m_MapPropertyNumbers.Lookup(sNewProperty, iPropNumber))
		RuntimeError("Динамическое свойство %s уже задано ранее, второй раз добавить нельзя");

	//Инвариант
	const int size = m_MapPropertyNames.size();
	if (iNewPropertyNumber < size)
		RuntimeError("КОП::ДобавитьДинамическоеСвойство: Ошибка логики");

	m_MapPropertyNumbers.SetAt(sNewProperty, iNewPropertyNumber);
	LPCTSTR key;
	m_MapPropertyNumbers.LookupKey(sNewProperty, key);
	m_MapPropertyNames.resize(iNewPropertyNumber+1);
	m_MapPropertyNames[iNewPropertyNumber] =  key;
}
int CDynamicPropertyManager::FindNumberByName(LPCTSTR sProperty) const
{
	int iPropNumber = -1;
	m_MapPropertyNumbers.Lookup(sProperty, iPropNumber);
	return iPropNumber;
}

LPCTSTR CDynamicPropertyManager::FindNameByNumber(const int iPropertyNumber) const
{
	const int size = m_MapPropertyNames.size();
	if (size <= iPropertyNumber) return NULL;
	return m_MapPropertyNames[iPropertyNumber];
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CComponentClass::CComponentClass() : CBLContext(1), m_pMod(NULL), m_pCurModule(),
CComponentClass::CComponentClass() : m_pMod(NULL), m_pCurModule(),
									defFnNames(NULL), m_bDonNotCallDestructor(false), m_pDynamicPropertyManager(new CDynamicPropertyManager)
{
  //AFX_MANAGE_STATE(AfxGetStaticModuleState());

  //m_pMod = NULL;
  //defFnNames = NULL;
  m_pCurModule = CBLModule::GetExecutedModule();
  //CSafeModulePtr pSafeMod(pCurModule);

  m_MethParamsDef.nNumberOfMeth = -1;
  m_MethParamsDef.nRealNumberOfMeth = -1;
  m_MethParamsDef.pContext = NULL;
  m_isCreateOptimizeMode = pMainAddIn->m_EnableOptimizate==0 ? false : true;

  if (!pImplicitParams)
	pImplicitParams = new CImplicitParams; // TODO скорее всего, нужен всего один статический объект

}

CComponentClass::~CComponentClass()
{
	m_DebugInfo.Release();
//LogErr("~CComponentClass %s pCont = %d", GetTypeString(), this);
  POSITION pos;
  CString pkey;
  InfoUnlimitsParams *pInfo = NULL;

  for( pos = m_BindInstaceValueList.GetStartPosition(); pos != NULL; )
  {
    m_BindInstaceValueList.GetNextAssoc( pos, pkey, (void*&)pInfo );
    pInfo->cont_ValueList->DecrRef(); // TODO проще закинуть в деструктор ~InfoUnlimitsParams :)
    delete pInfo; // TODO проще исправить на смарт-пойнтер :)
  }


  if (m_pMod)
  {
	//artbear рефакторинг
	m_MapOfModulesAndComp.RemoveKey(m_pMod);

    if (!m_DebugInfo)
    {
      if (!pMainAddIn->m_EnableOptimizate && MapDef.GetModule(m_NameOfInstance).isValid())
        MapDef.DelModule(m_NameOfInstance);

	  //UnlinkContext();
//      delete m_pMod;
	  m_pMod = NULL;

    }

  }

  if(!pMainAddIn->m_EnableOptimizate && defFnNames.isValid())
  {
    if (MapDef.GetDefs(m_NameOfInstance).isValid())
    {
      MapDef.DelDefParam(m_NameOfInstance);
    }
    else
      if (!m_isCreateOptimizeMode)
        ; //delete defFnNames; // artbear
  }

  //delete pImplicitParams; // TODO решить, когда же нужно удалить ?
}

CSafeContextPtr<CBLContext> CComponentClass::FindBaseContextByMethodNumber(const int iMethodNum, int& iMethNumInSearchedContext) const
{
	iMethNumInSearchedContext = -1;
 	if (!IsMethodFromBaseContexts(iMethodNum)) // TODO доп. проверка, скорее всего, нужна !
 		return CSafeContextPtr<CBLContext>();

	int iMet = defFnNames->Size();

    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CSafeContextPtr<CBLContext> result = *iter;

      int nNMethods = result->GetNMethods();

      if (iMethodNum < iMet+nNMethods)
      {
		iMethNumInSearchedContext = iMethodNum - iMet;
        return result;
      }
      iMet += nNMethods;
    }

	return CSafeContextPtr<CBLContext>();
}

int CComponentClass::CallAsMethod(int iMethodNum, CValue **ppValue, bool bHasRetVal, CValue *pRetValue)
{
  int res = 0;
//  const char* szMethodName = GetMethodName(iMethNum, 0);
//  const char* szMethodName1 = GetMethodName(iMethNum, 1);
//LogErr("%s::CallAsMethod (%d) <%s, %s>", GetTypeString(), iMethNum, szMethodName, szMethodName1);

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  if (IsMethodFromBaseContexts(iMethodNum))
  {
        int nMeth;
	  	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		if (pCont)
			if (bHasRetVal)
			  res = pCont->CallAsFunc(nMeth, *pRetValue, ppValue);
			else
			  res = pCont->CallAsProc(nMeth, ppValue);
		return res;
  }

    if (!m_pMod)
		return 0;

	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
      int iProc = MethodParam.nProcPos;

      CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
      assert(pHierarchy!=NULL);

      if (pMainAddIn->m_EnabledCheckType)
      {
        // Проверим сначала параметры метода
        CBLProcInfo ProcInfo;
        m_pMod->GetProcInfo(iProc, ProcInfo);
        CString strTemp = pImplicitParams->m_strClassAndMethCurrent;
        pHierarchy->CheckTypeOfParam(this, iMethodNum, ppValue, &ProcInfo);
        pImplicitParams->m_strClassAndMethCurrent = strTemp;
      }


      bool bResOfImpParam = false;

      // Массив с параметрами переданными по значению в метод для последующего восстановления
      ARRAY_VAL arrParamValOfMeth; //CArray<CValue*, CValue*> arrParamValOfMeth;

      CString strClassAndMeth;
      bool isUnLimParMethod = MethodParam.m_bIsMethodHaveUnlimitedCountOfParams;

      if (iMethodNum >= lastMethod)
      {
        if (isUnLimParMethod)
        {
          bResOfImpParam = pImplicitParams->GetImplicitParams(this, ppValue, iMethodNum);
          if (bResOfImpParam)
            strClassAndMeth = pImplicitParams->m_strClassAndMethImplicit;
        }
		// TODO при возникновении исключений в CallAsFunc или CallAsProc не будет восстановления

        if (MethodParam.m_bIsMethodHaveParamsReceivedByValue)
          pImplicitParams->SaveValParam(this, iMethodNum, ppValue, &arrParamValOfMeth);
      }
      int nCountParams = pImplicitParams->m_CountParametersInCallMethod;

      if (bHasRetVal)
      {
        CValue locVal;
		res = m_pMod->CallAsFunc(iProc, locVal, MethodParam.NumberOfParams, ppValue);
        *pRetValue = locVal;
      }
      else
      {
        res = m_pMod->CallAsProc(iProc, MethodParam.NumberOfParams, ppValue);
      }

      pImplicitParams->m_CountParametersInCallMethod = nCountParams;

      if (bResOfImpParam)
        pImplicitParams->m_strClassAndMethImplicit = strClassAndMeth;

      if (MethodParam.m_bIsMethodHaveParamsReceivedByValue)
        pImplicitParams->RestoreValParam(this, iMethodNum, ppValue, &arrParamValOfMeth);

	  CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      if (bHasRetVal && pMainAddIn && pMainAddIn->m_EnabledCheckType && res == 1)
      {
        // Затем проверим возвращенный тип метода
        CBLProcInfo ProcInfo;
        m_pMod->GetProcInfo(iProc, ProcInfo);
        pHierarchy->CheckTypeOfParam(this, iMethodNum, ppValue, &ProcInfo, pRetValue);
      }

	return res;
}

// artbear
void CComponentClass::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng,
		CString& rusKind, bool bAddKind)
{

  const CString strClassName(pCont->GetRuntimeClass()->m_lpszClassName);
  rus.Empty();
  eng.Empty();
  rusKind.Empty();
//LogErr("на что меняем strClassName %s", strClassName);

  rus = pCont->GetTypeString();
//LogErr("на что меняем rus %s", rus);

  //if (rus.IsEmpty() || strClassName == "CRegActContext" || strClassName == "CAllRegsContext")
  if (rus.IsEmpty() || !rus.CompareNoCase("ГрупповойКонтекст") || strClassName == "CRegActContext" || strClassName == "CAllRegsContext")
  {
    // Здесь агрегатные объекты 1С только такие как "Справочник" и "Документ"
    const int meth = pCont->FindMethod("Kind");
    if (meth != -1) // Нашли номер метода
    {
      //Вызываем данный метод
      CValue value;
      CValue ValueParam;
      CValue *pMassVal[1] = {&ValueParam};

      pCont->CallAsFunc(meth, value, pMassVal);

	  int meth = pCont->FindMethod("ТекущийДокумент");
	  bool bIsDoc = -1 != meth;

      //if (strClassName == "CDocContext")
      if (strClassName == "CDocContext" || bIsDoc)
      {
        rus = "Документ";
        eng = "Document";
      }
      else if (strClassName == "CSubcContext")
      {
        rus = "Справочник";
        eng = "Reference";
      }
      else if (strClassName == "CRegActContext")
      {
        rus = "Регистр";
        eng = "Register";
      }

      CString sKind = value.GetString();
      if (!sKind.IsEmpty() && bAddKind)
		  rusKind = sKind;
    }
    else
    {
      if (strClassName == "CAllRegsContext")
      {
        rus = "Регистры";
        eng = "Registers";
      }
    }
  }
  else
  {
    eng = rus;
  }
  // проверю для группового контекста
	{
		CGetDoc7* pDoc = ::GetDoc7(pCont);
		if (pDoc)
		{
			CTypedCont *pContainer = pDoc->m_pTypedCont; // artbear

			CString strObjectType = "";
			CString strKind = "";
			if (pContainer) {
				CString csType;
				long lMetaID = CConfigCont::DocumentToID (pContainer, csType);

				CString str = "НеизвестныйОбъект";
				if (lMetaID > 0)
				{
					str = GetModuleFullName (lMetaID, csType, 0, pMetaDataCont);

					int n = str.Find(".");
					if(n >= 0)
					{
						strKind = str.Mid(n+1);
						str = str.Left(n);
						int n = strKind.Find(".");
						if(n >= 0)
						{
							strKind = strKind.Left(n);
						}
					}
				}
				strObjectType = str;
			} else
				strObjectType = "ВнешнийОтчет";

			rus = strObjectType;
			eng = "";
			// rusKind = strKind;
		    if (!strKind.IsEmpty() && bAddKind)
				rusKind = strKind;

		}
	}
  if (rus.IsEmpty())
    RuntimeError("Имя класса: %s для которого не удалось сформировать имя в методе CComponentClass::GetFullTypeName! Обратитесь за консультацией к разработчику.", strClassName);
}

void CComponentClass::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng, bool bAddKind)
{
	CString rusKind;
	GetFullTypeName(pCont, rus, eng, rusKind,bAddKind);
	if (!rusKind.IsEmpty())
	{
		rus += "."+rusKind;
		eng += "."+rusKind;
	}
}

int CComponentClass::GetBaseClass(const CValue &NameClass, CValue &rValue) const
{
  rValue = CNumeric(0);

  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
  assert(pHierarchy!=NULL);

  if (NameClass.IsEmpty())
  {
	CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
    if (pCompClass)
      return pCompClass->GetBaseClass(NameClass, rValue);

	if (!vecOfBaseCont.empty())
    {
        CBLContext *p = *vecOfBaseCont.begin();
        rValue.AssignContext(p);
    }
    else
        rValue = CNumeric(0);
    return 1;
  }

    if (!m_NameOfInstance.CompareNoCase(NameClass.GetString()))
    {
      rValue.AssignContext(const_cast<CComponentClass*>(this));
      return 1;
    }

	  CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
      if (pCompClass)
      {
        if (pCompClass->GetBaseClass(NameClass, rValue) == 1)
          return 1;
      }
      else
      {
        for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
        {
          CBLContext *pCBLContext = *iter;
          CString strRus;
          CString strEng;
          GetFullTypeName(pCBLContext, strRus, strEng);

          if (!strRus.CompareNoCase(NameClass.GetString()) || !strEng.CompareNoCase(NameClass.GetString()))
          {
            rValue.AssignContext(pCBLContext);
            return 1;
          }

		  // artbear если у базы тип Справочник.ВидСправочника, а запрашиваем Справочник
// 		  CString strRus;
// 		  CString strEng;
		  GetFullTypeName(pCBLContext, strRus, strEng, false);
		  if (!strRus.CompareNoCase(NameClass.GetString()) || (!strEng.IsEmpty() && !strEng.CompareNoCase(NameClass.GetString())))
		  {
			rValue.AssignContext(pCBLContext);
			return 1;
		  }
		  else if (IS_KINDOF_CGroupContext(pCBLContext) && !NameClass.GetString().CompareNoCase("групповойконтекст"))
		  {
			rValue.AssignContext(pCBLContext);
			return 1;
		  }

		  else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
		  {
			CValue* params[1] = {const_cast<CValue*>(&NameClass)};

			if (pCBLContext->CallAsFunc(funcGetBaseClass, rValue, params) == 1)
			  return 1;
		  }
        }
      }

	return 0;
}

int CComponentClass::ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue)
{
  int res = 0;
  rValue = CNumeric(0);
  if (!Instance.GetContext())
    RuntimeError("Заменить экземпляр базового класса можно только объектом созданным функцией СоздатьОбъект или контекстом модуля!",0);

  if (!NameClass.GetString().GetLength())
    RuntimeError("Первым параметром должна быть не пустая строка!");

  {
    CString strNameClass(NameClass.GetString());
    CString strRusInstance;
    CString strEngInstance;
    CString strRusInstanceKind;
//LogErr("что меняем strNameClass %s", strNameClass);
	// последний параметр true - чтобы правильно обрабатывал ситуацию наследования
	// от универсального "Справочник"
	// описание бага http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2513
    GetFullTypeName(Instance.GetContext(), strRusInstance, strEngInstance,
		strRusInstanceKind);
	bool bInstanceIsGroupContext = IS_KINDOF_CGroupContext(Instance.GetContext());

    CComponentClass* pCompClass = VerifyCurrentExecuteContextIsBaseClassForThisClass(); // TODO

    if (!m_NameOfInstance.CompareNoCase(NameClass.GetString()))
	{
    // artbear Сами себя мы можем заменить, если только передаваемый объект того же типа
    // иначе здесь будет ошибка для кода Потомок.ЗаменитьБазовыйОбъект("Предок", СовсемДругойОбъект);
	// тест "Тест_ЗаменитьБазовыйОбъект_НаНеверныйКласс" - здесь очень-очень старая ошибка с первых релизов 1С++ :( !!
		if (!pCompClass)
			return 0;
        CString strRus;
        CString strEng;
        CString strRusKind;

        GetFullTypeName(this, strRus, strEng, strRusKind);
		bool bNeedReplace = false;
		if (strRusKind.IsEmpty())
		{
			bNeedReplace = !strRus.CompareNoCase(strRusInstance) ||
				!strEng.CompareNoCase(strEngInstance);
		}
		else
		{
			bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
				(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
		}

		if (bNeedReplace)
			return 1;
		else
			return 0;
	}
	else
    if (pCompClass)
    {
      if (pCompClass->ReplaceInstBaseClasses(NameClass, Instance, rValue) == 1)
      {
        res = 1;
		return res;
      }
    }
    else
    {
      for (ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
      {
        CBLContext* pCBLContext = *iter;
        CString strRus;
        CString strEng;
        CString strRusKind;
//LogErr("смотрим базу");

        GetFullTypeName(pCBLContext, strRus, strEng, strRusKind);

		// описание бага http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2513

		bool bNeedReplace = false;
		if (strRusKind.IsEmpty())
		{
			bNeedReplace = !strRus.CompareNoCase(strNameClass) ||
				!strEng.CompareNoCase(strNameClass); // TODO
			if (bNeedReplace)
			{
				bNeedReplace = CSetOfHierarchy::GetHierarchy()->IsObject(strRus, strRusInstance) ||
					CSetOfHierarchy::GetHierarchy()->IsObject(strEng, strEngInstance);
			}
		}
		else
		{
			bNeedReplace = !(strRus+"."+strRusKind).CompareNoCase(strNameClass) ||
				!(strEng+"."+strRusKind).CompareNoCase(strNameClass);
			if (bNeedReplace)
				bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
					(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
		}
		if (!bNeedReplace && bInstanceIsGroupContext)
		{
			if (IS_KINDOF_CGroupContext(pCBLContext)
				&& !strNameClass.CompareNoCase("групповойконтекст") && !strRus.CompareNoCase("групповойконтекст"))
				bNeedReplace = true;
			else
				if (strRusKind.IsEmpty())
				{
					bNeedReplace = !strRus.CompareNoCase(strNameClass) ||
						!strEng.CompareNoCase(strNameClass);
				}
				else
				{
					bNeedReplace = !(strRus+"."+strRusKind).CompareNoCase(strNameClass) ||
						!(strEng+"."+strRusKind).CompareNoCase(strNameClass);
					if (bNeedReplace)
						bNeedReplace = !strRusKind.CompareNoCase(strRusInstanceKind) &&
							(!strRus.CompareNoCase(strRusInstance) || (!strEng.IsEmpty() && !strEng.CompareNoCase(strEngInstance)));
				}
		}

		if (bNeedReplace)
        {
			{
				*iter = Instance.GetContext();
			}

			rValue = CNumeric(1);
			res = 1;
			return res;
			break;
        }
        else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
        {
          CValue* params[2] = {const_cast<CValue*>(&NameClass), const_cast<CValue*>(&Instance)};
          if (pCBLContext->CallAsFunc(funcReplaceInstBaseClasses, rValue, params) == 1)
          {
            rValue = CNumeric(1);
            res = 1;
			return res;
            break;
          }
        }
      }
    }
  }
  return res;
}

int CComponentClass::IsObject(const CValue &NameClass, CValue &rValue) const
{

	if (CSetOfHierarchy::GetHierarchy()->IsObject(m_NameOfInstance, NameClass.GetString()))
		rValue = CNumeric(1);
	else
		rValue = CNumeric(0);

	return 1;
}

int  CComponentClass::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
  int res = 0;

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  switch(iMethNum)
	{


    case funcGetBaseClass: // ПолучитьБазовыйКласс(<Название класса>) : возврат ссылка на базовый класс
    {
      res = GetBaseClass(*ppValue[0], rValue);
    }
    break;


    case funcReplaceInstBaseClasses:
    case funcReplaceInstBaseClasses_SYN1:
    {
      res = ReplaceInstBaseClasses(*ppValue[0], *ppValue[1], rValue);
    }
    break;

	case funcIsObject: // ЯвляетсяОбъектом(<Название объекта>) : возвращает 1 или 0
	{
	  res = IsObject(*ppValue[0], rValue);
	}
	break;

    case funcSendMessageOwnMod:
    {
      if (m_pCurModule)
      {
        int iProc = m_pCurModule->FindProc("ОбработкаСобытияОтКласса", 1);
        if (iProc != -1)
        {
			CValue retValue; // если в функцию передавать напрямую rValue, будет баг с возвращаемым значением
			res = m_pCurModule->CallAsFunc(iProc, retValue, (*defFnNames)[iMethNum].NumberOfParams, ppValue);
			rValue = retValue;
        }
        else
          RuntimeError("Не найдена функция обратного вызова ОбработкаСобытияОтКласса()", mmBlackErr); // удобнее исключение artbear
      }
      break;
    }
    case funcGetPathName:
      {
        CString str = CSetOfHierarchy::GetHierarchy()->GetNameOfFile(m_NameOfInstance);
        CSetOfHierarchy::MakeFullFileName(&str);
        rValue = str;
        res = 1;
      }
      break;
    case funcGetEnvContext:
      {
			CBLContext *pCont  = CBLModuleWrapper::GetContextFromModule(m_pCurModule);
			if (pCont)
			  rValue.AssignContext(pCont);
			else
			  rValue = CNumeric(0);
			res = 1;
      }
      break;
    case funcGetParamsList:
      {
        CBLContext *pContOfList = pImplicitParams->GetImplicitParamsList(this, ppValue[0]->GetString());
        if (pContOfList)
          rValue.AssignContext(pContOfList);
        else
          RuntimeError("Нельзя вызывать функцию GetParamsList в методе с отсутствующими неявными параметрами (...)!");
        res = 1;
      }
      break;
    case funcSetOnIndexParams:
      {
        res = pImplicitParams->SetImplicitParamOfInd(this, ppValue[0]->GetString(), ppValue[1]->GetNumeric().operator long(), ppValue[2]);
        rValue = CNumeric(res);
      }
      break;

	default:
	  res = CallAsMethod(iMethNum, ppValue, true, &rValue);
	};

	return res;
}

int  CComponentClass::CallAsProc(int iMethNum,class CValue * * ppValue)
{

  int res = 0;

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  switch(iMethNum)
	{

/*// этот функционал закрыт общим решением разработчиков 1С++ - artbear
// TODO возможно, в дальнейшем он должен быть открыт внутри класса, а снаружи невидим
//
  case procAddBaseClass:
    {
    }
    break;*/
  case procOnOpen: // оставлен для совместимости
    {
      res = 1;
    }
    break;
  case procThrow:
    {
		CValue& value = *ppValue[0];
		if (2 == value.GetTypeCode()) // строка
			CRuntimeExceptionClass::RuntimeError(value, value.GetString());
		else
			CRuntimeExceptionClass::RuntimeError(value);
    }
    break;
  case procAddDynamicProperty:
    {
		// TODO подобная проверка должна работать и для классов-наследников, а не только для текущего КОП-а
		if (!defFnNames->IsDefineFreePropFunctions())
			RuntimeError("КОП::ДобавитьДинамическоеСвойство: в классе должны быть реализованы соответствующие методы _ПриЧтенииСвойства и _ПриЗаписиСвойства");

		CValue& value = *ppValue[0];
		//CString errMsg;
		//errMsg.FormatV("%s.ДобавитьДинамическоеСвойство: необходимо передавать строку", GetTypeString());
		CString errMsg("КОП::ДобавитьДинамическоеСвойство: необходимо передавать строку");
		ValidateParamNotEmptyString(value, errMsg);

		CComponentClass* pHandlerClass = this;
		CComponentClass* pBaseClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
		if (pBaseClass)
			pHandlerClass = pBaseClass;

		int iNewPropertyNumber = pHandlerClass->GetNProps();

		pHandlerClass->m_pDynamicPropertyManager->Add(value.GetString(), iNewPropertyNumber);
      return 1;
    }
	break;

	default:
		res = CallAsMethod(iMethNum, ppValue);
	};
	return res;
}


int  CComponentClass::FindMethod(char const * lpMethodName)const
{
  int res = defFnNames->GetIndexByName(lpMethodName);

  if (-1 != res)
  {
	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[res];
    if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams || MethodParam.m_bIsMethodHaveParamsReceivedByValue)
    {
      pImplicitParams->m_strClassAndMethCurrent = m_NameOfInstance;
      pImplicitParams->m_strClassAndMethCurrent += ":";
      pImplicitParams->m_strClassAndMethCurrent += lpMethodName;
    }
  }
  else
  {
    int nCountMeth = defFnNames->Size();
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
      CBLContext *pCont = *iter;

      res = pCont->FindMethod(lpMethodName);

      if (res != -1)
      {
        res += nCountMeth;
        break;
      }
      nCountMeth += pCont->GetNMethods();
    }
  }
  return res;
}


char const *  CComponentClass::GetMethodName(int iMethodNum,int iMethodAlias)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->GetMethodName(nMeth, iMethodAlias);
    return NULL;
  }
	else
		return (*defFnNames)[iMethodNum].Names[iMethodAlias];
}

int  CComponentClass::GetNMethods(void)const
{
  int nCountMeth = 0;
  if (defFnNames.isValid())
    nCountMeth = defFnNames->Size();

  CONST_ITER_CONT iEnd(vecOfBaseCont.end());
  for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
  {
    CBLContext *pCont = *iter;
    nCountMeth += pCont->GetNMethods();
  }
	return nCountMeth;
}

int  CComponentClass::HasRetVal(int iMethodNum)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->HasRetVal(nMeth);
    return 0;
  }
  else
    return (*defFnNames)[iMethodNum].HasReturnValue;
}

int CComponentClass::GetNParams(int iMethodNum)const
{
  if (IsMethodFromBaseContexts(iMethodNum))
  {
    int nMeth;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
	if (pCont)
         return pCont->GetNParams(nMeth);
    return 0;
  }
  else
  {
    if (pImplicitParams && iMethodNum >= lastMethod)
    {
	  const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
      if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams)
        return pImplicitParams->GetNParams(this, iMethodNum, MethodParam.NumberOfParams);
      else
        return MethodParam.NumberOfParams;
    }
    else if (iMethodNum <= lastMethod)
      return (*defFnNames)[iMethodNum].NumberOfParams;
    else
      return -1;
    //return (*defFnNames)[iMethodNum].NumberOfParams;
  }
}

bool CComponentClass::VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue( const CString& strNamesClassAndMeth, const CBLProcInfo &procInfo)
{
	bool bResult = false;

	const iParamByValue = 2;
	int nVars = procInfo.GetNVars();
	int iMethodNum = procInfo.GetProcPos(); //m_ProcID

	CCompiledModule* pCompiledModule = procInfo.GetModule()->GetInternalData()->pCompiledModule;
	CCompileProcInfo* pCompileProcInfo = pCompiledModule->ProcArray[iMethodNum];
	for (int i = 0; i < nVars;i++)
	{
		CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
		if (iParamByValue == pVar->Type){
			bResult = true;
			break;
		}
	}

	if (bResult){
		CMetaDataOfClasses& m_Meta = CSetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();
		CParamDefs::CArrayOfMethodsParamData_PTR pArr( CParamDefs::Create_ArrayOfMethodsParamData() ); //new CParamDefs::ARR_TYPE_NAME); // TODO неявная работа с памятью, проще исправить на смарт-пойнтер

		for (int i = 0; i < nVars;i++)
		{
			CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
			bool bParamByValue = iParamByValue == pVar->Type;
			pArr->Add(CMethodsParamData::Create(pVar->Name, "", "", bParamByValue));
		}
		m_Meta.AddMethHaveParVal(strNamesClassAndMeth, pArr);
	}
	return bResult;
}

BOOL CComponentClass::GetParamDefValueFromClassDefinitionOrModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	bool bIsExistParam = false;
	const CSetOfHierarchy::CParamDefValueEnum ParamDefValueEnum = CSetOfHierarchy::GetHierarchy()->GetParamDefValue(iMethodNum, iParamNum, pDefValue, this);
	if (ParamDefValueEnum == CSetOfHierarchy::pdve_DefValueIsExists)
		return TRUE;
	else if (ParamDefValueEnum == CSetOfHierarchy::pdve_ParamDefineWithoutDefValue)
		return FALSE;

	return GetParamDefValueFromClassModule(iMethodNum, iParamNum, pDefValue);
}

BOOL CComponentClass::GetParamDefValueFromClassModule(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	const CParamDefs::paramdefs& ParamDefs = (*defFnNames)[iMethodNum];
	int iRealMethodNum = ParamDefs.nProcPos;

	CCompiledModule* pCompiledModule = m_pMod->GetInternalData()->pCompiledModule;
	CBLModuleProcVarDescr* pVar = pCompiledModule->ProcArray[iRealMethodNum]->VarList[iParamNum];
	if(pVar->Type2 >= 0)
	{
		*pDefValue = pCompiledModule->ConstArray[pVar->Type2]->val;
		return TRUE;
	}
	return FALSE;
}

int  CComponentClass::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
  // Данная функция вызывается перед вызовом CallAsFunc или CallAsProc для
  // назначения типа и значения переменной pDefValue

	// http://www.1cpp.ru/forum/YaBB.pl?num=1198844355/20#20
	// В случае наступления следующих событий:
	// 1. Вызов метода предка через Шаблон без Сам().
	// 2. В вызываемом методе должен быть параметр, для которого указано значение по умолчанию.
	// НО! указано НЕ в defcls.prm, а в коде класса (непонятно зачем, типа как для приватного метода)

	// ... сюда прилетает пустой указатель на pDefValue
	// в зависимости от нашего желания мы можем запретить (FALSE) или разрешить (TRUE) использование
	// параметра, указанного непосредственно в определении метода
	if (!pDefValue)
		return TRUE;

	if (IsMethodFromBaseContexts(iMethodNum))
  {
    bool bFind = false;
    int  nMeth = -1;
    CBLContext *pCont = NULL;
    if (m_MethParamsDef.nNumberOfMeth == iMethodNum && m_MethParamsDef.pContext != NULL)
    {
      nMeth = m_MethParamsDef.nRealNumberOfMeth;
      pCont = m_MethParamsDef.pContext;
      bFind = true;
    }
    else
    {
		pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		if (pCont)
			 bFind = true;
    }
    if (bFind && pCont)
    {
      m_MethParamsDef.nNumberOfMeth = iMethodNum;
      m_MethParamsDef.nRealNumberOfMeth = nMeth;
      m_MethParamsDef.pContext = pCont;

      return pCont->GetParamDefValue(nMeth, iParamNum, pDefValue);
    }
		return FALSE;
  }
  else
  {
    if (funcGetBaseClass == iMethodNum && iParamNum == 0)
    {
      pDefValue->Reset();
      return TRUE;
    }
    else if (funcGetBaseClass == procThrow)
    {
      if      (iParamNum == 0)
        pDefValue->Reset();
      else if (iParamNum == 1)
        *pDefValue = "";
      else
        return FALSE;

      return TRUE;
    }
    else
		return GetParamDefValueFromClassDefinitionOrModule(iMethodNum, iParamNum, pDefValue);
  }
	return FALSE; // Значит параметры по умолчанию не обрабатываются
}

void  CComponentClass::IncrRef(void)
{
	++m_RefCount;
}

void  CComponentClass::DecrRef(void)
{
	static bool _bAlreadyInsideDecRefCount = false;
	static CComponentClass* pExceptionInsideDestructorClass = NULL;

	if (m_RefCount != 1)
		--m_RefCount;
	else
	{
		if (m_pMod)
		{
			if (defFnNames->nPosDestructor != -1)
			{
				// некая хитрость для работы с исключениями в деструкторах
				// после выброса исключения DecrRef всегда тут же вызывается второй раз !!
				// и в этот второй раз нужно просто удалить объект без повторного вызова деструктора
				// и все ОК
				//
				//  ----------  НО  -----------------
				// но на след. коде (в котором нет блока Попытка-Исключения (например, запустив его в обычной обработке)
				// 1С будет падать
				// но имхо это уже не так страшно :( -
				// т.к. маловероятно, чтобы были созданы два одинаковых класса  и у обоих классов возникали исключения в деструкторе !!
				//
				//		Процедура ТестКлассСИсключениемВДеструктореСДвумяКопиями()
				//			объект = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//			объект1 = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//
				//			объект = 0;
				//		КонецПроцедуры	// ТестКлассСИсключениемВДеструкторе
				//
				// а на след. двух не падает !
				//
				// 1.	Процедура ТестКлассСИсключениемВДеструктореСДвумяКопиями2()
				//			объект = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//			объект1 = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//
				//		КонецПроцедуры	// ТестКлассСИсключениемВДеструкторе
				//
				// 2.	Процедура ТестКлассСИсключениемВДеструктореСДвумяКопиями_СозданПозжеУдаленРаньше() Экспорт
				//			объект = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//			объект1 = СоздатьОбъект("КлассСИсключениемВДеструкторе");
				//
				//			объект1 = 0; // или объект = 0;
				//		КонецПроцедуры
				//
				// в тестовой конфе, если закинуть код в класс, падать не будет! т.к. там все заключено в блок Попытка-Исключение !

				// если это условие вставить после вызова деструктора класса (пара строк ниже)
				// тогда падают некоторые тесты исключения в деструкторах
				//

				bool bNeedExceptionHandler = false;
				if (_bAlreadyInsideDecRefCount && this == pExceptionInsideDestructorClass)
				{
					bNeedExceptionHandler = true;
				}
				if (!_bAlreadyInsideDecRefCount)
				{
					CRuntimeExceptionClass::SaveStatusOfTryingBlock();
				} // временно
				//if (_bAlreadyInsideDecRefCount)
				if (!bNeedExceptionHandler && !m_bDonNotCallDestructor)
					int res = CBLModuleWrapper::CallAsProc0(m_pMod, defFnNames->nPosDestructor);

				if (!_bAlreadyInsideDecRefCount)
					{
						_bAlreadyInsideDecRefCount = true;
						pExceptionInsideDestructorClass =  this;

						CRuntimeExceptionClass::IfNeedExceptionThenThrowException(); // TODO

						_bAlreadyInsideDecRefCount = false;
					}
				//} // временно
				if (bNeedExceptionHandler)
				{
					_bAlreadyInsideDecRefCount = false;
					pExceptionInsideDestructorClass = NULL;
				}
			}
		}

		ReplaceContextInModulesOfBaseClasses();

		ClearAllBasesClass();

		m_RefCount = 0;

		delete this;

	}
}

// данная функция используется при вызове Строка(ОбъектКласса) в 1С
// разрешаю наследование функции _ПолучитьКод для КОП,
// если в КОП предке данный метод описан с модификатором Экспорт
// если же данного модификатора нет, значит, этот метод не будет наследоваться
//
char const *  CComponentClass::GetCode(void) const
{
  if (m_pMod)
  {
	  // разрешаю наследование функции _ПолучитьКод
	  // сначала пытаюсь найти метод _ПолучитьКод в текущем модуле,
	  // если не нашел, тогда в базовых классах
	int nPosCode = FindMethod(m_pMod, "_ПолучитьКод", "_GetCode");

    if (-1 != nPosCode)
    {
      CValue ret;
	  const_cast<CComponentClass*>(this)->CallAsMethod(nPosCode, NULL, true, &ret);
      return ret.GetString().operator LPCTSTR();
    }
  }
  return NULL;
}

class CObjID   CComponentClass::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CComponentClass::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long  CComponentClass::GetTypeID(void)const
{
	return 100;
}

char const *  CComponentClass::GetTypeString(void)const
{
	return m_NameOfInstance;
}

class CType   CComponentClass::GetValueType(void)const
{
	CType tType(100);
	return tType;
}

void  CComponentClass::InitObject(char const * strName)
{
	//m_NameOfInstance = strName;

	// чтобы название класса писалось не как пользователь напишет,
	// а бралось из файла описания artbear
	CString strResult = CSetOfHierarchy::GetHierarchy()->GetClassNameFromDefinition(strName);

	if (strResult.IsEmpty())
		RuntimeError("Нельзя создать класс без имени класса");

	m_NameOfInstance = strResult;

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	InitInstanceComp();
}

int  CComponentClass::IsExactValue(void)const
{
	return 1;
}

int  CComponentClass::IsOleContext(void)const
{
  int regEBX = 0;
  __asm {
    mov regEBX, ebx // В этом регистре 1С хранит количество параметров при вызове метода
  }
  pImplicitParams->m_CountParametersInCallMethod = regEBX; // сохраним это значение
	return 0;
}

int  CComponentClass::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CComponentClass::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CComponentClass::IsSerializable(void)
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  int iProc = m_pMod->FindFunc("КлассСохраняемый");
  iProc = iProc == -1 ? m_pMod->FindFunc("IsSerializable") : iProc;
  if (iProc != -1)
  {
       CValue retVal;
       int res = CBLModuleWrapper::CallAsFunc0(m_pMod, iProc, retVal);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

    return retVal.GetNumeric();
  }
  else
    return 0;
}

int  CComponentClass::SaveToString(class CString & csStr)
{
#ifndef _DEBUG

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  if (m_pMod)
  {
    int iProc = m_pMod->FindFunc("СохранитьКлассВСтроку");
    iProc = iProc == -1 ? m_pMod->FindFunc("SaveToString") : iProc;
    if (iProc != -1)
    {
       CValue retVal;
       int res = CBLModuleWrapper::CallAsFunc0(m_pMod, iProc, retVal);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      csStr = retVal.GetString();
    }
  }
#else
  ::ShowMsg("В _DEBUG режиме с методом SaveToString работать нельзя, если его вызывает 1С движок!!!",mmInformation);
#endif
	return 1;
}

void  CComponentClass::SelectByID(class CObjID cID,long lNum)
{
	//CBLContext::SelectByID(cID,lNum);
}

int  CComponentClass::GetNProps(void)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// динамические свойства реализованы иначе
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     static int nPosNProps = -1;
	// 	nPosNProps = m_pMod->FindFunc("_ПолучитьКолвоДСвойств");
	//     if (-1 == nPosNProps)
	//       nPosNProps = m_pMod->FindFunc("_GetNDynamicProps");
	//     if (nPosNProps != -1)
	//     {
	//        CValue rValue;
	// 		CBLModuleWrapper::CallAsFunc0(m_pMod, nPosNProps, rValue);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//       return rValue.GetNumeric();
	//     }
	//   }
	//   else
	//   {

    int nCountProps = m_pMod->GetNStaticVars();
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
      CBLContext *pCont = *iter;
      nCountProps += pCont->GetNProps();
    }

    nCountProps += m_pDynamicPropertyManager->Size();
	return nCountProps;

	//  }
  return 0;
}

char const *  CComponentClass::GetPropName(int A,int B)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// динамические свойства реализованы иначе
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     static int nPosGetPropName = -1;
	// 		nPosGetPropName = m_pMod->FindFunc("_ПолучитьИмяДСвойства");
	//     if (-1 == nPosGetPropName)
	//       nPosGetPropName = m_pMod->FindFunc("_GetPropDynamicName");
	//     if (nPosGetPropName != -1)
	//     {
	//       CValue rValue;
	//       CBLModuleWrapper::CallAsFunc1(m_pMod, nPosGetPropName, rValue, A+1);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//       return rValue.GetString();
	//     }
	//   }
	//   else
	//   {

  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(A);
	if (szName)
		return szName;
  }

    int nCountProps = m_pMod->GetNStaticVars();
    if (nCountProps > A)
    {
      CBLVarInfo VarInfo;
	    m_pMod->GetStaticVarDescr(A,VarInfo);
	    return VarInfo.GetName();
    }
    else
    {
      CONST_ITER_CONT iEnd(vecOfBaseCont.end());
      for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
      {
        CBLContext *pCont = *iter;
        int nNProps = pCont->GetNProps();
        if (A < nCountProps + nNProps)
        {
          return pCont->GetPropName(A-nCountProps , B);
        }
        else
          nCountProps += nNProps;
      }
    }

	//}

  return NULL;
}

int  CComponentClass::GetPropVal(int iPropNum,class CValue & rValue)const
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	//   if (iPropNum > 65536)
	//   {
	//     POSITION pos;
	//     CString key;
	//     int *val = NULL;
	//     for (pos = defFnNames->mapNamesFreeProp.GetStartPosition(); pos != NULL;)
	//     {
	//       defFnNames->mapNamesFreeProp.GetNextAssoc( pos, key, (void*&)val );
	//       if ((int)val == iPropNum - 65536)
	//       {
	// 		int ret = CBLModuleWrapper::CallAsFunc1(m_pMod, defFnNames->nPosOnReadFreeProp, rValue, CValue(key));
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//         return ret;
	//       }
	//     }
	//
	//   }
  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
	if (szName)
	{
		int ret = CBLModuleWrapper::CallAsFunc1(m_pMod, defFnNames->nPosOnReadFreeProp, rValue, CValue(szName));

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

		return ret;
	}
  }

  int nCountProps = m_pMod->GetNStaticVars();

  if (nCountProps > iPropNum)
  {
    int res = m_pMod->GetStaticVarValue(iPropNum,rValue,0);
    int iProc = -1;
    if (defFnNames.isValid())
      iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, false);

    if (iProc != -1)
    {
	  int res = CBLModuleWrapper::CallAsProc1(m_pMod, iProc, rValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      return res;
    }
    return res;
  }
	else
  {
    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      int nNProps = pCont->GetNProps();
      if (iPropNum < nCountProps + nNProps)
      {
        return pCont->GetPropVal(iPropNum - nCountProps, rValue);
      }
      else
        nCountProps += nNProps;
    }
    return 0;
  }
}

int  CComponentClass::SetPropVal(int iPropNum,class CValue const & vValue)
{

  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	// динамические свойства реализованы иначе
	//   if (iPropNum > 65536)
	//   {
	//     POSITION pos;
	//     CString key;
	//     int *val = NULL;
	//     for (pos = defFnNames->mapNamesFreeProp.GetStartPosition(); pos != NULL;)
	//     {
	//       defFnNames->mapNamesFreeProp.GetNextAssoc( pos, key, (void*&)val );
	//       if ((int)val == iPropNum - 65536)
	//       {
	//
	// 	  int ret = CBLModuleWrapper::CallAsProc2(m_pMod, defFnNames->nPosOnWriteFreeProp, const_cast<LPSTR>(LPCSTR(key)), vValue);
	//
	// 		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();
	//
	//         return ret;
	//       }
	//     }
	//   }
  if (defFnNames->IsDefineFreePropFunctions())
  {
	LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
	if (szName)
	{
		int ret = CBLModuleWrapper::CallAsProc2(m_pMod, defFnNames->nPosOnWriteFreeProp, const_cast<LPSTR>(szName), vValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

		return ret;
	}
  }

  int nCountProps = m_pMod->GetNStaticVars();

  if (nCountProps > iPropNum)
  {

    int iProc = -1;
    if (defFnNames.isValid())
      iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, true);

    if (iProc != -1)
    {

	  int ret = CBLModuleWrapper::CallAsProc1(m_pMod, iProc, vValue);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

      return ret;
    }
    return m_pMod->SetStaticVarValue(iPropNum,vValue,0);
  }
	else
  {
    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      if (iPropNum >= nCountProps && iPropNum < nCountProps + pCont->GetNProps())
      {
        return pCont->SetPropVal(iPropNum - nCountProps, vValue);
      }
      else
        nCountProps += pCont->GetNProps();
    }
    return 0;
  }
}

int  CComponentClass::FindProp(char const * Name)const
{
	// динамические свойства реализованы иначе
	//   if (defFnNames->IsDefineFreePropFunctions())
	//   {
	//     int *i = NULL;
	//     int nCount = 0;
	//     if (!defFnNames->mapNamesFreeProp.Lookup(Name, (void*&)i))
	//     {
	//       nCount = defFnNames->mapNamesFreeProp.GetCount() + 1;
	//       defFnNames->mapNamesFreeProp[Name] = (int *)nCount;
	//     }
	//     else
	//       nCount = (int)i;
	//     return 65536 + nCount;
	//   }

  if (defFnNames->IsDefineFreePropFunctions())
  {
	const int iPropNumber = m_pDynamicPropertyManager->FindNumberByName(Name);
	if (-1 != iPropNumber)
		return iPropNumber;
  }

  int nNumProp = m_pMod->FindStaticVar(Name);
  if (nNumProp == -1)
  {
    int nCountProps = m_pMod->GetNStaticVars();

    CONST_ITER_CONT iEnd(vecOfBaseCont.end());
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
    {
      CBLContext *pCont = *iter;
      int res = pCont->FindProp(Name);
      if (res != -1)
        return nCountProps + res;
      nCountProps += pCont->GetNProps();
    }
  }
  else
  {
    CBLVarInfo VarInfo;
	  m_pMod->GetStaticVarDescr(nNumProp,VarInfo);
    if (VarInfo.IsExported() == 0)
      nNumProp = -1;
  }

	return nNumProp;
}

// это первое обращение, необходимо заполнить команды и скомпилировать модуль
CComponentClassModulePtr CComponentClass::FirstInit(const CString& FullFileName)
{
    //CBLModule* pModLocale = NULL;
	CComponentClassModulePtr pModLocale(NULL);

	  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

	  CModuleString ModuleString;
	  if (!ModuleString.LoadModule(FullFileName, m_NameOfInstance))
	  {
		::ShowMsg(FullFileName, mmBlackErr);
		return NULL;
	  }
	  try
	  {
		CPreprocessor::Preprocess(true, ModuleString, pHierarchy->m_SymbolOfPreprocessor, CMapStringToString(), FullFileName);
	  }
	  catch (CPreprocessor::ExeptionPreprocessor &ex)
	  {
		::ShowMsg("В модуле класса: %s (%s)", mmBlackErr, m_NameOfInstance, FullFileName);
		::ShowMsg(ex.GetErrorString(), mmBlackErr);

		if (!ex.GetCodeString().IsEmpty())
		{
			::ShowMsg("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString());
		}
		pHierarchy->m_SymbolOfPreprocessor.RemoveKey("_NOW_PREPARE_CLASS");
		return NULL;
	  }

	  if (ModuleString.IsEmpty())
		RuntimeError("Класс: %s Из программного файла (%s) не получен модуль!", m_NameOfInstance, FullFileName);

	  //pModLocale = new CBLModuleWrapper(m_NameOfInstance, NULL, str, FullFileName);
	  //pModLocale = new CComponentClassModule(m_NameOfInstance, NULL, str, FullFileName);
	  pModLocale = CComponentClassModulePtr(new CBLModuleWrapper(m_NameOfInstance, NULL, ModuleString));

	  int nComp = pModLocale->Compile();

	  if (nComp != 1) {
		::ShowMsg("Класс: %s Файл: %s компиляция не выполнена!", mmRedErr, m_NameOfInstance, FullFileName);
		return NULL;
	  }

	  //////////////////////////////////////////////////////////////////////
	  // Описание методов и функций класса для 1С языка
	  // {{<"English_description">,<"Русское_название">},<0-это процедура, 1-это функция>,<Количество параметров>}
	  //////////////////////////////////////////////////////////////////////
	  // delete defFnNames; // artbear
	  //defFnNames = new CParamDefs();
	  //defFnNames = new CParamDefsWithRefCount();
	  defFnNames = CParamDefsWithRefCountPtr(new CParamDefs());

	  int iProc = pModLocale->FindProc("Конструктор", 0);
	  iProc = (iProc == -1) ? pModLocale->FindProc("Constructor", 1) : iProc;
	  defFnNames->nPosConstructor = iProc;
	  iProc = pModLocale->FindProc("Деструктор", 0);
	  iProc = (iProc == -1) ? pModLocale->FindProc("Destructor", 1) : iProc;
	  defFnNames->nPosDestructor = iProc;

	  int nReadPos = pModLocale->FindFunc("_ПриЧтенииСвойства");
	  nReadPos = (nReadPos != -1) ? nReadPos : pModLocale->FindFunc("_OnReadProperty");
	  if (nReadPos != -1)
	  {
		int nWritePos = pModLocale->FindProc("_ПриЗаписиСвойства", 0);
		nWritePos = (nWritePos != -1) ? nWritePos :pModLocale->FindProc("_OnWriteProperty", 1);
		if (nWritePos != -1)
		{
		  defFnNames->nPosOnReadFreeProp  = nReadPos;
		  defFnNames->nPosOnWriteFreeProp = nWritePos;
		}
	  }

// artbear - перенесу в Init
// 	  if(pMainAddIn->m_EnableOptimizate)  // добавим модуль в банк модулей для данного класса
// 		MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);

	  int nProc = pModLocale->GetNProcs(); // Получим общее количество процедур и фукций

	  if (pHierarchy) // Инициализируем описание класса дополнительной информацией полученной из описания класса, по его методам
		pHierarchy->InitParamDefsClass(defFnNames.get(), m_NameOfInstance);



	  defFnNames->SetPoolSize(nProc + lastMethod + 1, pModLocale->GetNStaticVars());

	  defFnNames->AddParam("GetBaseClass", "ПолучитьБазовыйКласс", funcGetBaseClass, 1, 1);
	  defFnNames->AddParam("AssignFriendModule", "ПрисоединитьТекущийМодуль",procAssignFriendModule, 0, 0);

	  // этот функционал закрыт общее решение artbear
	  // TODO возможно, в дальнейшем он должен быть открыт внутри класса, а снаружи невидим
	  //
	  //defFnNames->AddParam("AssignBaseClass", "НазначитьБазовыйКласс", procAddBaseClass, 0, 1);

	  defFnNames->AddParam("SendMessageOwnMod", "ОтправитьСообщениеМодулюХоз", funcSendMessageOwnMod, 1, 3);
	  defFnNames->AddParam("GetPathName", "ПолучитьПуть", funcGetPathName, 1, 0);
	  defFnNames->AddParam("GetEnvContext", "ПолучитьКонтекстОкружения", funcGetEnvContext, 1, 0);
	  defFnNames->AddParam("GetParamsList", "ПолучитьСписокПараметров", funcGetParamsList, 1, 1);

	  defFnNames->AddParam("ReplaсeBaseObject", "ЗаменитьБазовыйОбъект", funcReplaceInstBaseClasses, 1, 2);  //recommended
	  defFnNames->AddParam("ReplaceInstBaseClasses", "ЗаменитьЭксзБазовогоКласса", funcReplaceInstBaseClasses_SYN1, 1, 2);  //deprecated

	  defFnNames->AddParam("SetOnIndexParams", "УстановитьПараметрПоИндексу", funcSetOnIndexParams, 1, 3);
	  defFnNames->AddParam("_OnOpen", "_ПриОткрытии", procOnOpen, 0, 0);
	  defFnNames->AddParam("_Throw", "_ВыброситьИскл", procThrow, 0, 1);

	  defFnNames->AddParam("IsObject", "ЯвляетсяОбъектом", funcIsObject, 1, 1);

	  defFnNames->AddParam("AddDynamicProperty", "ДобавитьДинамическоеСвойство", procAddDynamicProperty, 0, 1);


	  for (int count = 1; count < nProc; count++)
	  {
		char nameProc[255] = {0};
		CBLProcInfo procInfo;
		pModLocale->GetProcInfo(count, procInfo); // Получим расширенную инф. по процедурам и функциям
		strcpy(nameProc, procInfo.GetName());
		int nParamas   = procInfo.GetNParams();
		int nVars      = procInfo.GetNVars();
		int hasRetVal  = procInfo.HasRetVal();
		int procPos    = procInfo.GetProcPos();

		if (procInfo.IsExported() == 1)
		{
		  CString strNamesClassAndMeth = m_NameOfInstance;
		  strNamesClassAndMeth +=":";
		  strNamesClassAndMeth +=nameProc;

			bool bIsMethodHaveParamByValue = pHierarchy->isMethHaveParVal(strNamesClassAndMeth);

// TODO если метод имеет неограниченное число параметров, значит, он точно описан в defcls.prm и из модуля ничего брать нельзя !!

			if (!bIsMethodHaveParamByValue)
				bIsMethodHaveParamByValue = VerifyMethodForParamsByValueAndSetMetaInfoClassesDataForParamsByValue(strNamesClassAndMeth, procInfo);


		  defFnNames->AddParam(NULL,nameProc, procPos, hasRetVal, nParamas,
			pHierarchy->ThisIsUnLimMethod(strNamesClassAndMeth), //узнаем имеет ли метод неявные параметры
			bIsMethodHaveParamByValue); //узнаем имеет ли метод параметры передаваемые по значению
		}
	  }

	  return pModLocale;
}

CBLModule* CComponentClass::Init(const char *file)
{
  // file - имя файла, где расположен текст модуля компоненты

  // Попробуем получить из общего пула класса MapDef список методов и
  // откомпилированный модуль относящийся к данному экземпляру класса,
  // т.к. модули и его команды (процедуры, функции, общие переменные)
  // общие для всех экзепляров дочернего класса. Выполнять компиляцию,
  // теперь необходимо только один раз при первом создании данного класса.
  // Последующие обращения будут располагать готовыми данными
//   try
  {
    //CBLModule* pModLocale = NULL;
    CComponentClassModulePtr pModLocale(NULL);

    CString FullFileName(file);
    CSetOfHierarchy::MakeFullFileName(&FullFileName);

	//CParamDefs *pParDefs = MapDef.GetDefs(m_NameOfInstance);
	CParamDefsWithRefCountPtr pParDefs = MapDef.GetDefs(m_NameOfInstance);
    if (pMainAddIn->m_EnableOptimizate && pParDefs.isValid()) {
      defFnNames = pParDefs;
      pModLocale = MapDef.GetModule(m_NameOfInstance);
    }
    else
    {
       // это первое обращение, необходимо заполнить команды и скомпилировать модуль
		pModLocale = FirstInit(FullFileName);

		if(pMainAddIn->m_EnableOptimizate && pModLocale.isValid())  // добавим модуль в банк модулей для данного класса
			MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);
    }
    if (pModLocale.isValid())
    {
      if (m_pMod)
        m_pMod = NULL;

	  m_DebugInfo.OpenDebugForm(this);

// artbear kms выбрал такой вариант
//       if (!m_pMod)
// 			m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedCompModule, NULL, FullFileName);
//
//       m_pMod->AssignCompiled(*pModLocale);

       if (!m_pMod)
 			m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedCompModule, pModLocale.get());

      if(!pMainAddIn->m_EnableOptimizate)
      {
		MapDef.DelDefParam(m_NameOfInstance);
        MapDef.DelModule(m_NameOfInstance);
        //delete pModLocale;
        pModLocale = CComponentClassModulePtr(NULL);
      }

      if (m_pCurModule)
		m_pMod->AssignFriendModule(m_pCurModule);

      //m_DebugInfo.AssignDebugModule(this, m_pMod);
	CSafeModulePtr pMod;
	m_DebugInfo.AssignDebugModule(this, pMod); // у kms в отладке этого нет !!
	if (pMod)
		m_pMod = pMod;


      if (m_pMod->Execute() != 1)
		RuntimeError("Выполнить модуль класса: %s при инициализации не удалось!", m_NameOfInstance);
    }
    if (m_pMod)
    {
      m_MapOfModulesAndComp[m_pMod] = this;
    }
    return m_pMod;
  }
//   catch (...)
//   {
// 	RuntimeError("Неизвестное исключение! При инициализации класса: %s!", m_NameOfInstance);
//   }
  return NULL;
}

class GuardOfFlagCreateWithoutConstructor
{
private:
	bool m_bCreateWithoutConstructorSaved;
public:
	GuardOfFlagCreateWithoutConstructor() { m_bCreateWithoutConstructorSaved = CComponentClass::IsCreateWithoutConstructor(); };
	~GuardOfFlagCreateWithoutConstructor() { if (m_bCreateWithoutConstructorSaved) CComponentClass::SetFlagCreateWithoutConstructor(); };
};

void CComponentClass::InvokeConstructor()
{
  CRuntimeExceptionClass::SaveStatusOfTryingBlock();

  {
	  GuardOfFlagCreateWithoutConstructor guard;
	  ClearFlagCreateWithoutConstructor();

	  CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	  for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	  {
		CBLContext *pCBLContext = *iter;
		if (IS_KINDOF_CComponentClass(pCBLContext))
		{
		  CComponentClass* pCont = static_cast<CComponentClass*>(pCBLContext);

		  pCont->InvokeConstructor();
		}
	  }
  }

  if (m_pMod)
  {
    if (-1 != defFnNames->nPosConstructor && !IsCreateWithoutConstructor())
    {

	  int res = CBLModuleWrapper::CallAsProc0(m_pMod, defFnNames->nPosConstructor);

		CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

    }
  }
}

// CBLModule * CComponentClass::GetHostModule() const
// {
//   return pCurModule;
// }
//
// CBLModule * CComponentClass::GetModule() const
// {
//   return m_pMod;
// }

BOOL CComponentClass::GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const
{
    for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
    {
		CBLContext *pCont = *iter;
        if (pCont->GetRuntimeClass() == pRTClass)
        {
            *ppBaseObj = *iter;
            return TRUE;
        }
        else if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CComponentClass))
        {
            if ((static_cast<CComponentClass*>(pCont))->GetBaseClass(pRTClass, ppBaseObj))
                return TRUE;
        }
    }
    return FALSE;
}


// найти метод класса по названию
// сначала ищется в модуле класса на случай, если задан без Экспорт
// а затем в экспортируемых методах класса, затем в наследниках
//
// сначала ищется русское имя, а затем английское
//
inline int CComponentClass::FindMethod(CBLContext* pContext, CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng)
{
	int nPosCode = -1;

	// пытаюсь в модуле текущего класса найти
	// нужный метод , заданнный без "Экспорт" или с Экспорт
    if (-1 == nPosCode)
		nPosCode = pMod->FindFunc(sNameRus);

	// далее пройдусь по наследникам
    if (-1 == nPosCode)
		nPosCode = pContext->FindMethod(sNameRus);

	// пытаюсь в модуле текущего класса найти
	// нужный метод , заданнный без "Экспорт" или с Экспорт
    if (-1 == nPosCode)
		nPosCode = pMod->FindFunc(sNameEng);

	// далее пройдусь по наследникам
    if (-1 == nPosCode)
	      nPosCode = pContext->FindMethod(sNameEng);

	return nPosCode;
}

// проверить, является ли текущий исполняемый модуль модулем базового класса для текущего класса
// используется для исключения рекурсии
CComponentClass* CComponentClass::VerifyCurrentExecuteContextIsBaseClassForThisClass() const
{

	CComponentClass* const pCompClass = CComponentClass::GetComponentClassCurExModule();

	CBLContext* const pContExecMod = CBLModuleWrapper::GetContextFromModule(CBLModule::GetExecutedModule());
    if (pCompClass != NULL && pCompClass != this
      && (CBLContext*)this == pContExecMod)
		return pCompClass;

	return NULL;

}

CComponentClass* CComponentClass::GetComponentClassCurExModule(CBLModule* pModWrapper)
{
  CBLModule* pMod = pModWrapper == NULL ? CBLModule::GetExecutedModule() : pModWrapper;
  return static_cast<CComponentClass*>(m_MapOfModulesAndComp[pMod]);
}

bool CComponentClass::AddBaseClass(const CString& name)
{
	  CSafeContextPtr<CBLContext> pCont(name);
      if (pCont)
      {
		// Установка переданного контекста в список контекстов
		//  для реализации моделирования наследования

		vecOfBaseCont.push_back(pCont);

		return true;
      }
	  return false;
}

void CComponentClass::ClearAllBasesClass()
{
	// чтобы деструкторы вызывались строго в обратном порядке относительно вызова конструкторов
	while(!vecOfBaseCont.empty())
		vecOfBaseCont.pop_back();
}

void CComponentClass::ReplaceContextInModulesOfBaseClasses(CComponentClass* pMainCont)
{
	CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	{
		CBLContext *pCBLContext = *iter;
		if (IS_KINDOF_CComponentClass(pCBLContext))
		{
			CComponentClass* pBaseClassContext = static_cast<CComponentClass*>(pCBLContext);
			// artbear - иначе возможен баг при уничтожении потомка раньше, чем предка
			// см. тесты ТестУдалениеПотомкаРаньшеПредка и ТестУдалениеПотомкаРаньшеПредка2
			//
			// TODO возможна оптимизация с проверкой m_RefCount и
			//		занесением только, если m_RefCount базы больше, чем m_RefCount потомка
			//
			if (!pMainCont)
			{
				pBaseClassContext->m_pMod->GetInternalData()->pGeneralContext = pCBLContext;
				pBaseClassContext->ReplaceContextInModulesOfBaseClasses(pBaseClassContext);
			}
			else
				pBaseClassContext->m_pMod->GetInternalData()->pGeneralContext = pMainCont;
		}
	}

}

void CComponentClass::OnStartInit(void)
{
	if (!pCreatedCompModule)
	{
	  // Запоминаем создаваемый объект КОП, это первый вызов метода-инициализации
	  pCreatedCompModule = this;

	  CDebug::SetDebugObject(this);
	}

}

void CComponentClass::OnEndInit(bool bIsBadExit)
{
	pCreatedCompModule=NULL;
	if (bIsBadExit){
		for (ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter){
			CBLContext* pCBLContext = *iter;
			if (IS_KINDOF_CComponentClass(pCBLContext))
			  static_cast<CComponentClass*>(pCBLContext)->SetFlagDonNotCallDestructor();
		}
		vecOfBaseCont.erase(vecOfBaseCont.begin(), vecOfBaseCont.end());
	}
}

void CComponentClass::FinishInit(void)
{
	if (this == pCreatedCompModule)
  {
    // Теперь объект КОП полностью создан, можно вызвать все его конструкторы
	OnEndInit();

    InvokeConstructor();
    m_DebugInfo.ClearDebugObject();

  }
}

class CInitGuard
{
private:
	CComponentClass* m_pComponentClass;
	bool m_bIsBadExit;
public:
	CInitGuard(CComponentClass* pComp) :
		m_pComponentClass(pComp), m_bIsBadExit(true)
	{
		m_pComponentClass->OnStartInit();
	}
	~CInitGuard()
	{
		if (m_bIsBadExit) // этот блок сработает только на исключениях и неверном выходе
			m_pComponentClass->OnEndInit(true);
	}

	void FinishInit(void) // этот метод должен быть вызван в случае нормального завершения
	{
		m_bIsBadExit = false;
		m_pComponentClass->FinishInit();
	}
};

// Выполняет инициализацию экземпляра класса на основе информации полученной
// из файла составленного пользователем
void CComponentClass::InitInstanceComp(void)
{
	CInitGuard InitGuard(this); // сторож освобождения ресурсов

	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CMetaDataOfClasses& m_Meta = pHierarchy->GetMetaDataOfClasses();

	CString NameOfComponent = GetTypeString();
	CString NameOfFile = m_Meta.m_ClassBindFile[NameOfComponent];
	if (NameOfFile.IsEmpty() )
		//return false;
		RuntimeError("Не задано имя файла для класса %s!", NameOfComponent);

	if ( !Init(NameOfFile) )
	  RuntimeError("Внимание! Инициализация класса: %s закончилась неудачно!", NameOfComponent);

	CMetaDataOfClasses::CStringList_SharedPtr pList = m_Meta.m_ClassesHierarchyMap[NameOfComponent]; //CStringList* pList = (CStringList*)m_Meta.m_ClassesHierarchyMap[NameOfComponent];
	if (pList)
	{
		// проверю рекурсию
		CString sRecursiveHierarchy = "";
		if (pHierarchy->VerifyRecursiveInBaseClasses(NameOfComponent, NameOfComponent, sRecursiveHierarchy))
			RuntimeError("Ошибка создания класса " + NameOfComponent + "\r\nНайдена рекурсия (предок-потомок): "+sRecursiveHierarchy);

		for(POSITION posList = pList->GetHeadPosition(); posList != NULL; )
		{
		  CString str = pList->GetNext( posList );

		  if (!AddBaseClass(str))
			RuntimeError("Базовый класс: %s для класса: %s не создан!", str, NameOfComponent);
		}
	}

	InitGuard.FinishInit();
}

void CComponentClass::ClearOptimizationData(void)
{
	//CComponentClass::MapDef.Clear();
	CComponentClass::MapDef.ClearWithoutDelete();
}

// метод для вызова пользовательского конструктора с несколькими параметрами
// используется в методе ФабрикаОбъектов::Новый()
void CComponentClass::CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const
{
	if (m_pMod)
	{
		int iProc = m_pMod->FindProc(strConstructorName, 0);
		iProc = -1 == iProc ? m_pMod->FindProc(strConstructorName, 1) : iProc;
		if (-1 == iProc)
			RuntimeError("В модуле класса <%s> не удалось найти процедуру-конструктор <%s>", GetTypeString(), strConstructorName);

		// Конструкторы не должны иметь атрибут Экспорт для большей безопасности
		int iPublicProc = FindMethod(strConstructorName);
		if (-1 != iPublicProc)
			RuntimeError("Конструктор %s::%s не должен иметь атрибут Экспорт", GetTypeString(), strConstructorName);

		CBLProcInfo info;
		m_pMod->GetProcInfo(iProc, info);
		int iNParams = info.GetNParams();
		if (iNParams < nParamsCount)
			RuntimeError("Конструктору %s::%s передано слишком большое число параметров - нужно %d, а передали %d",
				GetTypeString(), strConstructorName, iNParams, nParamsCount);

		if (iNParams > nParamsCount)
			RuntimeError("Конструктору %s::%s передано слишком малое число параметров - нужно %d, а передали %d",
				GetTypeString(), strConstructorName, iNParams, nParamsCount);

		int iRes = m_pMod->CallAsProc(iProc, nParamsCount, ppValue);
	}

}

BOOL CComponentClass::SendMessageOwnMod( CValue &rValue, CValue ** ppValue )
{
	return FALSE; // TODO пока просто заглушки
}

void CComponentClass::GetPathName( CValue &rValue )
{
	return; // TODO пока просто заглушки
}

void CComponentClass::GetEnvContext( CValue &rValue )
{
	return; // TODO пока просто заглушки
}

void CComponentClass::GetParamsList( CValue &rValue, CValue * * ppValue )
{
	return; // TODO пока просто заглушки
}

BOOL CComponentClass::SetOnIndexParams( CValue &rValue, CValue * * ppValue )
{
	return FALSE; // TODO пока просто заглушки
}

void CComponentClass::ThrowException(CValue ** ppValue)
{
	return; // TODO пока просто заглушки
}

void CComponentClass::AddDynamicProperty(CValue ** ppValue)
{
	return; // TODO пока просто заглушки
}

// Получить количество базовых объектов.
DWORD CComponentClass::GetBaseObjCount()
{
	return vecOfBaseCont.size();
}

// Получить имя базового объекта по индексу.
CString CComponentClass::GetBaseObjName(DWORD idx)
{
	// TODO проверить индекс на границы
	return vecOfBaseCont[idx]->GetTypeString();
}

// Получить базовый объект по индексу.
CBLContext* CComponentClass::GetBaseObj(DWORD idx)
{
	// TODO проверить индекс на границы
	return vecOfBaseCont[idx];
}

// Для upcast'а
CBLContext* CComponentClass::GetDerivedObj()
{
	// TODO вернуть правильный объект
	return NULL;
}
