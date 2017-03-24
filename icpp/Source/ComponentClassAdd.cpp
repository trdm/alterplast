// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "ComponentClass.h"
#include "UserClassesImpl.h"

//#include "System\GroupContextWrap.h"
#include "System\DllHooker.h"

//extern CMetaDataCont * pMetaDataCont;
#include "addin.h"
#include "MetaDataOfClasses.h"

using boost::shared_ptr;

// ======================================================================
//
// begin of CDebug
//

CComponentClass* CComponentClassImpl::CDebug::m_CurDebugObject = NULL;

void CComponentClassImpl::CDebug::ClearDebugObject()
{
  m_CurDebugObject = NULL;
}

void CComponentClassImpl::CDebug::SetDebugObject(CComponentClass *pComp)
{
  if (pMainAddIn->m_EnableDebugging)
    m_CurDebugObject = pComp;
}


void CComponentClassImpl::CDebug::Release(void)
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

void CComponentClassImpl::CDebug::OpenDebugForm(CComponentClass* pComponent)
{
	if(!pMainAddIn->m_EnableDebugging)
		return;
	
	//CString m_NameOfInstance = pComponent->m_NameOfInstance;
	CString m_NameOfInstance = pComponent->GetTypeString();
	
	CBLContext* pContLoaded = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts("OpenForm", pContLoaded);
	if (-1 == nMethNum)
		return;
	
	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
	CString str = ISetOfHierarchy::GetMetaDataOfClasses().GetNameOfFile(m_NameOfInstance);
	ISetOfHierarchy::MakeFullFileName(&str);

	CString strPar1;
	if (0 == str.Right(4).CompareNoCase(".ert"))
		strPar1 = "Отчет#";
	else if (0 == str.Right(3).CompareNoCase("@md"))
	{
		strPar1 = "Обработка.";
		strPar1 += str.Left(str.GetLength() - 3);
		strPar1 += '#';
	}
	else
		return;
	
	// artbear отладка возможна только для внешних/встроенных отчетов/обработок
	// иначе выдается дурацкое, неудобное предупреждение
	// artbear - при открытии формы класса в виде группового контекста директива _NOW_PREPARE_CLASS выключена
	// http://cvs.alterplast.ru/bugs/show_bug.cgi?id=2505
	
	const char* cstr_NOW_PREPARE_CLASS = "_NOW_PREPARE_CLASS";
	void *pV = NULL;
	
	CMapStringToPtr &m_RefMap = pHierarchy->GetMapOfSymbolOfPreprocessor();
	
	bool bEnable_NOW_PREPARE_CLASS_Prev = m_RefMap.Lookup(cstr_NOW_PREPARE_CLASS, pV) > 0 ? true : false;
	
	m_RefMap[cstr_NOW_PREPARE_CLASS] = NULL;
	
	pHierarchy->SetIsOpenCompForm(true);
	
	CValue retval;
	CValue Par1 = strPar1.operator LPCTSTR();
	CValue Par2;
	Par2.SetValTypeID(100);
	Par2.AssignContext(pComponent);
	
	
	CValue Par3 = str.operator LPCTSTR();
	
	CValue* MasPar[6] = {&Par1, &Par2, &Par3, 0, 0, 0};
	
	pDebugContext.reset();

	try
	{
		int iRes = pContLoaded->CallAsFunc(nMethNum, retval, &MasPar[0]);
		pDebugContext = Par2.GetContext(); // sedmin
		if (pDebugContext.get() == pComponent){
			//RuntimeError("Не удалось");
			pDebugContext.reset();
		}
		else		
			CHookGroupContext::DisableToFormClose(pDebugContext);
	}
	catch(...)
	{
		pDebugContext.reset();
	}
	if (!pDebugContext)
		::ShowMsg("Не удалось открыть форму класса <%s> для отладки", mmBlackErr, m_NameOfInstance.operator LPCTSTR());
	
	pHierarchy->SetIsOpenCompForm(false);
	
	if (bEnable_NOW_PREPARE_CLASS_Prev)
		m_RefMap.SetAt(cstr_NOW_PREPARE_CLASS, pV);
	else
		m_RefMap.RemoveKey(cstr_NOW_PREPARE_CLASS);
}

//void CComponentClassImpl::CDebug::AssignDebugModule(CBLContext* pComponent, CBLModule*& pMod)
void CComponentClassImpl::CDebug::AssignDebugModule(CBLContext* pComponent, CSafeModulePtr& pMod)
{
	if (IsDebug()) 
	{
//		if (pMod != NULL) 
//			delete pMod;
		assert(pDebugContext.operator bool());
		assert(m_CurDebugObject != NULL);

		pMod = NULL;
		//pMod = GetModuleByContext((CGroupContext*)(CBLContext*)pDebugContext);
		CGroupContext* pGroupContext = static_cast<CGroupContext*>(pDebugContext.operator CBLContext*());
		pMod = GetModuleByContext(pGroupContext);

		if (!pMod)
			return;

		pDebugContext->IncrRef();                        
		pMod->AssignContext(m_CurDebugObject);

		pComponent->DecrRef();
	}
}

//
// ======================================================================

// ---------------------------------------------------------------------------------
//
// CComponentClassImpl::CImplicitParams - работа с неопределенным числом параметров
//
// ---------------------------------------------------------------------------------
//CComponentClassImpl::CImplicitParams* CComponentClassImpl::pImplicitParams = NULL;
int CComponentClassImpl::CImplicitParams::m_CountParametersInCallMethod = -1;

CComponentClassImpl::CImplicitParams::CImplicitParams()
{
}

BOOL CComponentClassImpl::CImplicitParams::LookupInBindInstanceValueList(CComponentClassImpl* pComp, const CString& m_strClassAndMethCurrent, InfoUnlimitsParams::CPtr& pInfo) //const
{ 
	return pComp->GetBindInstanceValueList().Lookup(m_strClassAndMethCurrent, pInfo);
}
InfoUnlimitsParams::CPtr CComponentClassImpl::CImplicitParams::CreateInfoAndSetIntoUserClass( const CComponentClassImpl * pComp )
{
	CComponentClassImpl * pComp1 = const_cast<CComponentClassImpl*>(pComp);
	InfoUnlimitsParams::CPtr pInfo;
	if (LookupInBindInstanceValueList(pComp1, m_strKeyOfUserClassMethod, pInfo))
	{
		pInfo->RemoveAllFromValueList();
	}
	else
	{
		pInfo = InfoUnlimitsParams::Create();
		pComp1->GetBindInstanceValueList()[m_strKeyOfUserClassMethod] = pInfo;
	}    
	return pInfo;
}

void CComponentClassImpl::CImplicitParams::CleanSavedParams( const CComponentClassImpl * pComp )
{
	CreateInfoAndSetIntoUserClass(pComp);
}

int CComponentClassImpl::CImplicitParams::GetNParams(CComponentClassImpl *pComp, int nNumMeth, int nNParams)
{  
  int nRes = CountParametersInCallMethod() < 0 ? 0 : CountParametersInCallMethod();
  int nResult = nRes < nNParams ? nNParams : nRes;
  
  if (nRes < nNParams)
    return nNParams;  

  if (!m_strKeyOfUserClassMethod.GetLength())
	  return nResult;
  
  //InfoUnlimitsParams::CPtr pInfo = CreateInfoAndSetIntoUserClass(pComp);
  InfoUnlimitsParams::CPtr pInfo;
  LookupInBindInstanceValueList(pComp, m_strKeyOfUserClassMethod, pInfo);
  assert(NULL != pInfo);

  pInfo->Setup(nRes, nNParams);
  
  //return nRes;    
  return nResult;
}

bool CComponentClassImpl::CImplicitParams::SetImplicitParams(CComponentClassImpl *pComp, CValue **ppPar, int nNumMeth)
{
	InfoUnlimitsParams::CPtr pInfo; // = NULL;  

  if (!LookupInBindInstanceValueList(pComp, m_strKeyOfUserClassMethod, pInfo))
	  return false;

    //m_strKeyOfUserClassMethod_Implicit = m_strKeyOfUserClassMethod;
    pInfo->vecPointOfPars.clear();

    for (int i = 0; i < pInfo->GetCountOfAllParams() - pInfo->GetCountOfExplicitParams(); i++)
    {
      pInfo->vecPointOfPars.push_back(ppPar[i+pInfo->GetCountOfExplicitParams()]);      
    }
    
    CStringArray ArrString;
    for (int ind = 0; ind < pInfo->GetCountOfExplicitParams(); ind++)
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
      int nSizeArr = ArrString.GetSize();
      for (int i = pInfo->GetCountOfExplicitParams(); i < pInfo->GetCountOfAllParams(); i++)
      {                       
        CValue par2("");
        int nInd = i - pInfo->GetCountOfExplicitParams();
		if (nInd < nSizeArr)
          par2 = ArrString[nInd];

        if ((ppPar[i]->IsEmpty())&&(ppPar[i]->GetTypeCode() == UNDEFINE_TYPE_1C))
        {          
          CValue emptyVal("");
          ppPar[i]->MakeExactValueFrom(&emptyVal);          
        }

		pInfo->AddValueIntoValueList(*ppPar[i], par2);
      }
      return true;
}

InfoUnlimitsParams::CPtr CComponentClassImpl::CImplicitParams::GetImplicitParamsInfo(CComponentClassImpl *pComp, const char* lpcsNameMeth) //const
{
  if (!lpcsNameMeth)
    return InfoUnlimitsParams::CPtr();
  
  CComponentClass* pCompReal = CComponentClassImpl::GetComponentClassCurExModule();
  //CComponentClass* pCompReal = pComp;
  if (!pCompReal)
    RuntimeError("Нельзя вызывать функцию GetParamsList в модуле не принадлежащему текущему классу!");
  
  //if (pCompReal != pComp)
  //  pComp = pCompReal;
  
  CString strKeyOfUserClassMethod = pCompReal->m_pImpl->GetKeyOfUserClassMethod(lpcsNameMeth);
  
  InfoUnlimitsParams::CPtr pInfo;
  if (LookupInBindInstanceValueList(pCompReal->m_pImpl.get(), strKeyOfUserClassMethod, pInfo))
    return pInfo;

  return InfoUnlimitsParams::CPtr();  
}


//InfoUnlimitsParams::CListContextPtr CComponentClassImpl::CImplicitParams::GetImplicitParamsList(CComponentClassImpl *pComp, const char* lpcsNameMeth)
shared_ptr<CValueListContextFriend> CComponentClassImpl::CImplicitParams::GetImplicitParamsList(CComponentClassImpl *pComp, const char* lpcsNameMeth)
{
	InfoUnlimitsParams::CPtr pInfo = GetImplicitParamsInfo(pComp, lpcsNameMeth);
	if (!pInfo)
		return shared_ptr<CValueListContextFriend>(); //InfoUnlimitsParams::CListContextPtr();  

	return Clone_CValueListContextFriend(pInfo->GetValueListContext().get()); //return pInfo->GetValueListContext();
}

shared_ptr<CValueListContextFriend> CComponentClassImpl::CImplicitParams::Clone_CValueListContextFriend( CValueListContextFriend* pSrcList )
{
	if (!pSrcList)
		return shared_ptr<CValueListContextFriend>();

	CBLPtr<CValueListContextFriend> pContOfNewList("");
	assert(NULL != pContOfNewList.get());

	if (pSrcList->GetSize()){
		shared_ptr<CVLResultPublisher> pVLPublisher(new CVLResultPublisher);
		pVLPublisher->setContext(pSrcList);

		shared_ptr<CVLResultLoader> pVLLoader(new CVLResultLoader);
		pVLLoader->setContext(pContOfNewList.get());

		CSC_CMN::DataExchange( pVLPublisher.get(), pVLLoader.get() );
		//pVLLoader.reset(); // TODO временно, т.к. на самом деле не нужно
		//pVLPublisher.reset(); // TODO временно, т.к. на самом деле не нужно

		assert(pContOfNewList->GetSize() == pSrcList->GetSize());
	}

	boost::shared_ptr<CValueListContextFriend> p( pContOfNewList.get_shared_ptr() );

	assert(NULL != p.get());
	return p;
}

int CComponentClassImpl::CImplicitParams::SetImplicitParamOfInd(CComponentClassImpl *pComp, const char *lpcsNameMeth, int nIndex, CValue* pVal)
{
	InfoUnlimitsParams::CPtr pInfo = GetImplicitParamsInfo(pComp, lpcsNameMeth);
	if (!pInfo)
		return 0;

	if (nIndex >  (pInfo->GetCountOfAllParams() - pInfo->GetCountOfExplicitParams()) || (nIndex < 1))
	  RuntimeError("Указан неверный индекс в параметре 2 метода УстановитьПараметрПоИндексу!");

	//*pInfo->vecPointOfPars[nIndex-1] = *pVal;
	CValue* ValueRef = pInfo->vecPointOfPars[nIndex-1];
	*ValueRef = *pVal;
	return 1;
}

void InfoUnlimitsParams::AddValueIntoValueList(CValue& param1, CValue& param2)
{
	CValue *Params[2] = {0};        
	Params[0] = &param1;
	Params[1] = &param2;
	m_pValueListContext->P_Add(Params);
}

void InfoUnlimitsParams::RemoveAllFromValueList()
{
	//m_pValueListContext->P_RemoveAll(NULL);
	assert(1 == m_pValueListContext->m_RefCount);
	m_pValueListContext.reset( new CValueListContextFriend );
}

void InfoUnlimitsParams::Setup(int nRes, int nNParams)
{
	m_nNumAllParams      = nRes;
	m_nNumExplicitParams = nNParams;        
}

//CComponentClassImpl::CParamsReceivedByValueManager::CParamsReceivedByValueManager()
//{}
//
//bool CComponentClassImpl::CParamsReceivedByValueManager::SaveParams(CComponentClassImpl *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
//{  
//	VerifyCComponentClassParam(pComponentClass);
//	assert(0 != m_strCurrenMethodKey.GetLength());
//
//	CParamDefs::CArrayOfMethodsParamData_PTR param;
//	const CMetaDataOfClasses& m_Meta = ISetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();
//
//	//if (!m_Meta.LookupMethHaveParVal(m_strCurrenMethodKey, param))
//	if (!pComponentClass->m_pMethodsWithParamsByValueManager->Lookup(m_strCurrenMethodKey, param))
//		return false;
//	
//	int num = pComponentClass->GetNParams(nNumMeth);    
//	int nSize = param->GetSize();
//	int nPlus = 0;
//
//	if (pComponentClass->HasRetVal(nNumMeth) == 1)
//	{
//		nPlus = 1;
//		nSize--;
//	}
//
//	nSize =  min(num,nSize);
//	int nNumParam = 0;
//
//	for (int i = 0; i < nSize; i++, nNumParam++)
//	{          
//		CMethodsParamData::Ptr NameBind = param->operator[](i+nPlus);
//		if (NameBind->IsByVal())
//		{     
//			CValue_SharedPtr pnew(new CValue(*(ppParam[i])));
//			arrValOfMeth->Add(pnew); // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
//		}
//		else{
//			CValue_SharedPtr empty;
//			arrValOfMeth->Add(empty); //NULL);
//		}
//	}
//	return true;
//}
//
//
//void CComponentClassImpl::CParamsReceivedByValueManager::RestoreParams(CComponentClassImpl *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
//{
//	// TODO проверить вызов только после предварительного вызова SaveParams, а еще лучше сделать стековый сторож !
//
//	VerifyCComponentClassParam(pComponentClass);
//	assert(0 != m_strCurrenMethodKey.GetLength());
//
//	int nParams = pComponentClass->GetNParams(nNumMeth);
//	int nSize   = arrValOfMeth->GetSize();
//	int nS = min(nParams, nSize);
//
//	for(int i = 0; i < nS; i++)
//	{
//		CValue_SharedPtr pVal = arrValOfMeth->GetAt(i);
//		if (pVal)
//		{      
//			*(ppParam[i]) = *pVal;
//			//delete pVal; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
//		}
//	}  
//}
//
//class CComponentClassImpl::CParamByValueGuard
//{
//	CString m_strCurrenMethodKey;
//	CComponentClassImpl* m_pComponentClass;
//	int m_nNumMeth;
//	CValue** m_ppParam;
//	std::vector<CValue_SharedPtr> m_SavedVector;
//
//	void Save();
//	void Restore();
//
//	void VerifyCComponentClassParam( CComponentClassImpl * pComponentClass );
//public:
//	CParamByValueGuard(const CString& strCurrenMethodKey, CComponentClassImpl* pComponentClass, int nNumMeth, CValue** ppParam) :
//		m_strCurrenMethodKey(strCurrenMethodKey), m_pComponentClass(pComponentClass), m_nNumMeth(nNumMeth), m_ppParam(ppParam)
//	{ Save(); };
//
//	~CParamByValueGuard() { Restore(); };
//};

shared_ptr<CComponentClassImpl::CParamByValueGuard> CComponentClassImpl::CParamsReceivedByValueManager::GetParamsByValueGuard(const CString& strCurrenMethodKey, CComponentClassImpl* pComponentClass, int nNumMeth, CValue** ppParam)
{
	assert(m_pCurrentComponentClass == pComponentClass);
	return shared_ptr<CParamByValueGuard>(new CParamByValueGuard(strCurrenMethodKey, pComponentClass, nNumMeth, ppParam));
}

//bool CComponentClassImpl::CParamsReceivedByValueManager::SaveParams(CComponentClassImpl *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
void CComponentClassImpl::CParamByValueGuard::Save()
{  
	CParamDefs::CArrayOfMethodsParamData_PTR param;
	const CMetaDataOfClasses& m_Meta = ISetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();

	//if (!m_Meta.LookupMethHaveParVal(m_strCurrenMethodKey, param))
	if (!m_pComponentClass->m_pMethodsWithParamsByValueManager->Lookup(m_strCurrenMethodKey, param))
		return; // false;

	int num = m_pComponentClass->GetNParams(m_nNumMeth);    
	int nSize = param->GetSize();
	int nPlus = 0;

	// TODO artbear - в инфе о параметрах метода первым хранится инфа о возвращаемом значении, если это функция
	if (m_pComponentClass->HasRetVal(m_nNumMeth) == 1)
	{
		nPlus = 1;
		nSize--;
	}

	nSize =  min(num,nSize);
	int nNumParam = 0;
	
	m_SavedVector.reserve(nSize);

	for (int i = 0; i < nSize; i++, nNumParam++)
	{          
		CMethodsParamData::Ptr NameBind = param->operator[](i+nPlus);
		if (NameBind->IsByVal())
		{     
			CValue_SharedPtr pnew(new CValue(*(m_ppParam[i])));
			m_SavedVector.push_back(pnew); //arrValOfMeth->Add(pnew);
		}
		else{
			CValue_SharedPtr empty;
			m_SavedVector.push_back(empty); //arrValOfMeth->Add(empty);
		}
	}
	//return true;
}

//void CComponentClassImpl::CParamsReceivedByValueManager::RestoreParams(CComponentClassImpl *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth)
void CComponentClassImpl::CParamByValueGuard::Restore()
{
	//assert(m_pComponentClass == pComponentClass);
	//assert(0 != m_strCurrenMethodKey.GetLength());

	int nParams = m_pComponentClass->GetNParams(m_nNumMeth);
	int nSize   = m_SavedVector.size(); //arrValOfMeth->GetSize();
	int nS = min(nParams, nSize);

	for(int i = 0; i < nS; i++)
	{
		CValue_SharedPtr pVal = m_SavedVector[i]; //arrValOfMeth->GetAt(i);
		if (pVal)
		{      
			*(m_ppParam[i]) = *pVal;
			//delete pVal; // TODO неявная работа с памятью, проще исправить на смарт-пойнтер
		}
	}  
}