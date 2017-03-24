// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ComponentClass.h"
#include "UserClassesImpl.h"
#include "RuntimeException.h"

#include "System\GroupContextWrap.h"
#include "MetaDataOfClasses.h"
#include "Calling.h"
#include "VTExt/SerialStorage.h"

#include "SimpleBLCollections.h"

using boost::shared_ptr;

extern CMetaDataCont * pMetaDataCont;

class CObjID CComponentClassImpl::ObjID;

class CMapDefMethod CComponentClassImpl::MapDef;

CComponentClass* CComponentClassImpl::pCreatedUserClass = NULL;
bool CComponentClassImpl::bFlagCreateWithoutConstructor = false;

// Карта с указателями на модули с их реальными контекстами
// Ключ - указатель на модуль, Значение - указателя на контекст CComponentClass
CComponentClassImpl::CMapOfModulesAndComponentClasses CComponentClassImpl::m_MapOfModulesAndComp;//CMapPtrToPtr CComponentClassImpl::m_MapOfModulesAndComp;

CIStringMap<CComponentClassImpl::CMethodsMetadataManager::CPtr, CComponentClassImpl::CMethodsMetadataManager::CPtr&> CComponentClassImpl::mapOfMethodsWithParamsByValueManagers;

// решение для бага http://cvs.alterplast.ru/bugs/show_bug.cgi?id=3889
const int iNumOfSpecialMethod_Virt = 0x10000; // вирт()
const int iNumOfSpecialMethod_MySelf = 0x10001; // я()

class CDynamicPropertyManager
{
public:
	BOOL IsEmpty() const { return Size() == 0;};
	int Size() const { return m_MapPropertyNumbers.GetCount(); };

	void Add(LPCTSTR sNewProperty, int iNewPropertyNumber);

	int FindNumberByName(LPCTSTR sProperty) const;
	LPCTSTR FindNameByNumber(const int iPropertyNumber) const;

	BOOL IsExist(const int iPropertyNumber) { return !FindNameByNumber(iPropertyNumber); };
protected:

private:
	CIStringMapToInt m_MapPropertyNumbers;
	std::vector<LPCTSTR> m_MapPropertyNames;
};

void CDynamicPropertyManager::Add(LPCTSTR sNewProperty, int iNewPropertyNumber)
{
	int iPropNumber = -1;
	if (m_MapPropertyNumbers.Lookup(sNewProperty, iPropNumber))
		RuntimeError("Динамическое свойство %s уже задано ранее, второй раз добавить нельзя", sNewProperty);

	//Инвариант
	const int size = m_MapPropertyNames.size();
	assert(iNewPropertyNumber >= size);
	if (iNewPropertyNumber < size)
		RuntimeError("КОП::ДобавитьДинамическоеСвойство: Ошибка логики");

	m_MapPropertyNumbers.SetAt(sNewProperty, iNewPropertyNumber);
	LPCTSTR key;
	BOOL bSuccess = m_MapPropertyNumbers.LookupKey(sNewProperty, key);
	assert(TRUE == bSuccess);
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

CComponentClassImpl::CComponentClassImpl(CComponentClass* pOwner) : m_pOwner(pOwner), m_pMod(NULL), m_pCurModule(),
m_bDonNotCallDestructor(false), m_pDynamicPropertyManager(new CDynamicPropertyManager)
, m_bInsideDestructor(false), m_pParamsByValueManager(new CParamsReceivedByValueManager),
pImplicitParams(new CImplicitParams)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pCurModule = CBLModule::GetExecutedModule();

	m_MethParamsDef.nNumberOfMeth = -1;
	m_MethParamsDef.nRealNumberOfMeth = -1;
	m_MethParamsDef.pContext = NULL;
	m_isCreateOptimizeMode = pMainAddIn->m_EnableOptimizate==0 ? false : true;

	//if (!pImplicitParams)
	//pImplicitParams = new CImplicitParams; // TODO скорее всего, нужен всего один статический объект

}

CComponentClassImpl::~CComponentClassImpl()
{
	m_DebugInfo.Release();

	if (m_pMod)
	{
		m_MapOfModulesAndComp.RemoveKey(m_pMod);

		if (!m_DebugInfo)
		{
			if (!pMainAddIn->m_EnableOptimizate && MapDef.GetModule(m_NameOfInstance))
				MapDef.DelModule(m_NameOfInstance);

			//UnlinkContext();
			m_pMod = NULL;

		}

	}

	if(!pMainAddIn->m_EnableOptimizate && defFnNames)
	{
		if (MapDef.GetDefs(m_NameOfInstance))
		{
			MapDef.DelDefParam(m_NameOfInstance);
		}
		else
			if (!m_isCreateOptimizeMode)
				; //delete defFnNames; // artbear
		
		mapOfMethodsWithParamsByValueManagers.RemoveKey(m_NameOfInstance);
	}

	//delete pImplicitParams; // TODO решить, когда же нужно удалить ?
}

CSafeContextPtr<CBLContext> CComponentClassImpl::FindBaseContextByMethodNumber(const int iMethodNum, int& iMethNumInSearchedContext) const
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


int CComponentClassImpl::CallAsMethod(int iMethodNum, CValue **ppValue, bool bHasRetVal, CValue *pRetValue)
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->CallAsMethod(iMethodNum, ppValue, bHasRetVal, pRetValue);

	int res = 0;

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (IsMethodFromBaseContexts(iMethodNum))
	{
		int nMeth;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		assert(pCont.operator bool());
		//if (pCont)
			if (bHasRetVal)
				return pCont->CallAsFunc(nMeth, *pRetValue, ppValue);
			else
				return pCont->CallAsProc(nMeth, ppValue);
		//return res;
	}

	if (!m_pMod)
		return 0;

	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
	int iProc = MethodParam.nProcPos;

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
	assert(pHierarchy!=NULL);

	if (pMainAddIn->m_EnabledCheckType)
	{
		// Проверим сначала параметры метода
		CBLProcInfo ProcInfo;
		m_pMod->GetProcInfo(iProc, ProcInfo);
		
		CString strTemp = pImplicitParams->GetKeyOfUserClassMethod();
		CString strTemp1 = m_pParamsByValueManager->GetCurrenMethodKey();
		
		pHierarchy->CheckTypeOfParam(GetOwner(), iMethodNum, ppValue, &ProcInfo);
		
		pImplicitParams->SetKeyOfUserClassMethod(this, strTemp);
		m_pParamsByValueManager->SetCurrenMethodKey(this, strTemp1);
	}

	BOOL bHaveDefinitionsDiff = MethodParam.GetHaveDefinitionsDiff();
	if (bHaveDefinitionsDiff)
		RuntimeError("Метод %s - различаются параметры в описании класса (prm-файл) и его реализации (модуль класса)", pImplicitParams->GetKeyOfUserClassMethod().operator LPCTSTR());

	bool isUnLimParMethod = MethodParam.m_bIsMethodHaveUnlimitedCountOfParams;

	shared_ptr<CParamByValueGuard> ParamByValueGuard;

	if (isUnLimParMethod)
		pImplicitParams->SetImplicitParams(this, ppValue, iMethodNum);

	if (MethodParam.m_bIsMethodHaveParamsReceivedByValue){
		ParamByValueGuard = m_pParamsByValueManager->GetParamsByValueGuard(m_pParamsByValueManager->GetCurrenMethodKey(), 
			this, iMethodNum, ppValue);
	}

	int nSaved_m_CountParametersInCallMethod = pImplicitParams->CountParametersInCallMethod();

	int nCountParams = MethodParam.NumberOfParams;
	//if (nCountParams < nSaved_m_CountParametersInCallMethod)
	//	nCountParams = nSaved_m_CountParametersInCallMethod;

	if (bHasRetVal)
	{
		CValue locVal;
		//res = m_pMod->CallAsFunc(iProc, locVal, MethodParam.NumberOfParams, ppValue);
		res = m_pMod->CallAsFunc(iProc, locVal, nCountParams, ppValue);
		*pRetValue = locVal;
	}
	else
	{
		//res = m_pMod->CallAsProc(iProc, MethodParam.NumberOfParams, ppValue);
		res = m_pMod->CallAsProc(iProc, nCountParams, ppValue);
	}

	if (isUnLimParMethod)
		pImplicitParams->CleanSavedParams(this);

	pImplicitParams->CountParametersInCallMethod( nSaved_m_CountParametersInCallMethod );

	ParamByValueGuard.reset();

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	if (bHasRetVal && pMainAddIn && pMainAddIn->m_EnabledCheckType && res == 1)
	{
		// Затем проверим возвращенный тип метода
		CBLProcInfo ProcInfo;
		m_pMod->GetProcInfo(iProc, ProcInfo);
		pHierarchy->CheckTypeOfParam(GetOwner(), iMethodNum, ppValue, &ProcInfo, pRetValue);
	}

	return res;
}

// artbear
void CComponentClassImpl::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng,
									  CString& rusKind, bool bAddKind)
{
	const CString strClassName(pCont->GetRuntimeClass()->m_lpszClassName);
	rus.Empty();
	eng.Empty();
	rusKind.Empty();
	//LogErr("на что меняем strClassName %s", strClassName);

	rus = pCont->GetTypeString();
	//LogErr("на что меняем rus %s", rus);

	if (rus.IsEmpty() || !rus.CompareNoCase("ГрупповойКонтекст") || strClassName == "CRegActContext" || strClassName == "CAllRegsContext")
	{
		// Здесь агрегатные объекты 1С только такие как "Справочник" и "Документ"
		const int meth = pCont->FindMethod("Kind");
		if (meth != -1) // Нашли номер метода
		{
			CValue value;
			CValue ValueParam;
			CValue *pMassVal[1] = {&ValueParam};

			pCont->CallAsFunc(meth, value, pMassVal);

			int meth = pCont->FindMethod("ТекущийДокумент");
			bool bIsDoc = -1 != meth;

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

					int n = str.Find('.');
					if(n >= 0)
					{
						strKind = str.Mid(n+1);
						str = str.Left(n);
						int n = strKind.Find('.');
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
		RuntimeError("Имя класса: %s для которого не удалось сформировать имя в методе CComponentClassImpl::GetFullTypeName! Обратитесь за консультацией к разработчику.", strClassName.operator LPCTSTR());
}

void CComponentClassImpl::GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng, bool bAddKind)
{
	CString rusKind;
	GetFullTypeName(pCont, rus, eng, rusKind,bAddKind);
	if (!rusKind.IsEmpty())
	{
		rus += '.'+rusKind;
		eng += '.'+rusKind;
	}
}

//int CComponentClassImpl::GetBaseClass(const CValue &NameClass, CValue &rValue) const
int CComponentClassImpl::GetBaseClass(const CString & NameClass, CValue & rValue) const
{
	rValue = CNumeric(0);

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
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

	//if (!m_NameOfInstance.CompareNoCase(NameClass.GetString()))
	if (!m_NameOfInstance.CompareNoCase(NameClass))
	{
		rValue.AssignContext(const_cast<CComponentClass*>(GetOwner()));
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

			//if (!strRus.CompareNoCase(NameClass.GetString()) || !strEng.CompareNoCase(NameClass.GetString()))
			if (!strRus.CompareNoCase(NameClass) || !strEng.CompareNoCase(NameClass))
			{
				rValue.AssignContext(pCBLContext);
				return 1;
			}

			// artbear если у базы тип Справочник.ВидСправочника, а запрашиваем Справочник
			GetFullTypeName(pCBLContext, strRus, strEng, false);
			//if (!strRus.CompareNoCase(NameClass.GetString()) || (!strEng.IsEmpty() && !strEng.CompareNoCase(NameClass.GetString())))
			if (!strRus.CompareNoCase(NameClass) || (!strEng.IsEmpty() && !strEng.CompareNoCase(NameClass)))
			{
				rValue.AssignContext(pCBLContext);
				return 1;
			}
			//else if (IS_KINDOF_CGroupContext(pCBLContext) && !NameClass.GetString().CompareNoCase("групповойконтекст"))
			else if (IS_KINDOF_CGroupContext(pCBLContext) && !NameClass.CompareNoCase("групповойконтекст"))
			{
				rValue.AssignContext(pCBLContext);
				return 1;
			}

			//else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
			else if(IS_KINDOF_CComponentClass(pCBLContext)) // TODO проверить правильность замены
			{
				//CValue Value(NameClass);
				//CValue* params[1] = {&Value}; //CValue* params[1] = {const_cast<CValue*>(&NameClass)};

				//if (pCBLContext->CallAsFunc(funcGetBaseClass, rValue, params) == 1)
				if (static_cast<IUserClassContext*>(pCBLContext)->GetBaseClass(NameClass, rValue))
					return 1;
			}
		}
	}

	return 0;
}

int CComponentClassImpl::ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue)
{
	int res = 0;
	rValue = CNumeric(0);
	if (!Instance.GetContext())
		RuntimeError("Заменить экземпляр базового класса можно только объектом созданным функцией СоздатьОбъект или контекстом модуля!");

	if (!NameClass.GetString().GetLength())
		RuntimeError("Первым параметром должна быть не пустая строка!");

	{
		CString strNameClass(NameClass.GetString());
		CString strRusInstance;
		CString strEngInstance;
		CString strRusInstanceKind;

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

			GetFullTypeName(GetOwner(), strRus, strEng, strRusKind);
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
							bNeedReplace = ISetOfHierarchy::GetMetaDataOfClasses().IsObject(strRus, strRusInstance) ||
								ISetOfHierarchy::GetMetaDataOfClasses().IsObject(strEng, strEngInstance);
						}
					}
					else
					{
						bNeedReplace = !(strRus+'.'+strRusKind).CompareNoCase(strNameClass) ||
							!(strEng+'.'+strRusKind).CompareNoCase(strNameClass);
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
								bNeedReplace = !(strRus+'.'+strRusKind).CompareNoCase(strNameClass) ||
									!(strEng+'.'+strRusKind).CompareNoCase(strNameClass);
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
					//else if(pCBLContext->GetRuntimeClass()==&classCComponentClass)
					else if(IS_KINDOF_CComponentClass(pCBLContext)) // TODO проверить правильность замены
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

int CComponentClassImpl::IsObject(const CValue &NameClass, CValue &rValue) const
{

	if (ISetOfHierarchy::GetMetaDataOfClasses().IsObject(m_NameOfInstance, NameClass.GetString()))
		rValue = CNumeric(1);
	else
		rValue = CNumeric(0);

	return 1;
}

int  CComponentClassImpl::CallAsFunc(int iMethNum,CValue & rValue,CValue * *ppValue)
{
	rValue.Reset(); // без этого сброса возможен баг http://cvs.alterplast.ru/bugs/show_bug.cgi?id=3903

	if(iNumOfSpecialMethod_Virt == iMethNum)
	{
		CBLContext* pCont = CBLModuleWrapper::GetContextFromModule(m_pMod);
		rValue.AssignContext(pCont);

		return TRUE;
	}
	if(iNumOfSpecialMethod_MySelf == iMethNum)
	{
		CBLContext* pCont = CComponentClassImpl::GetComponentClassCurExModule();
		rValue.AssignContext(pCont);
		//rValue.AssignContext(GetOwner());

		return TRUE;
	}

	int res = 0;

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	switch(iMethNum)
	{


	case funcGetBaseClass: // ПолучитьБазовыйКласс(<Название класса>) : возврат ссылка на базовый класс
		{
			//return GetBaseClass(*ppValue[0], rValue);
			return GetBaseClass(ppValue[0]->GetString(), rValue);
		}
		break;


	case funcReplaceInstBaseClasses:
	case funcReplaceInstBaseClasses_SYN1:
		{
			return ReplaceInstBaseClasses(*ppValue[0], *ppValue[1], rValue);
		}
		break;

	case funcIsObject: // ЯвляетсяОбъектом(<Название объекта>) : возвращает 1 или 0
		{
			return IsObject(*ppValue[0], rValue);
		}
		break;

	case funcSendMessageOwnMod:
		{
			return SendMessageOwnMod(rValue, ppValue);
			break;
		}
	case funcGetPathName:
		{
			GetPathName(rValue);
			return TRUE;
		}
		break;
	case funcGetEnvContext:
		{
			GetEnvContext(rValue);
			return TRUE;
		}
		break;
	case funcGetParamsList:
		{
			GetParamsList(rValue, ppValue);
			return TRUE;
		}
		break;
	case funcSetOnIndexParams:
		{
			return SetOnIndexParams(rValue, ppValue);
		}
		break;

	default:
		return CallAsMethod(iMethNum, ppValue, true, &rValue);
	};

	return res;
}

int  CComponentClassImpl::CallAsProc(int iMethNum, CValue ** ppValue)
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
			return TRUE;
		}
		break;
	case procThrow:
		{
			ThrowException(ppValue);
		}
		break;
	case procAddDynamicProperty:
		{
			AddDynamicProperty(ppValue);
			return TRUE;
		}
		break;

	default:
		return CallAsMethod(iMethNum, ppValue);
	};
	return res;
}


int  CComponentClassImpl::FindMethod(char const * lpMethodName)const
{
	if(!FastCompareNoCase.Compare(lpMethodName, "вирт")) // заготовка для бага http://cvs.alterplast.ru/bugs/show_bug.cgi?id=3889
		return iNumOfSpecialMethod_Virt;
	if(!FastCompareNoCase.Compare(lpMethodName, "я"))
		return iNumOfSpecialMethod_MySelf;

	if(!IsFullyCreated()) // сюда попадаем при назначенном контексе во время компиляции модуля класса
		return -1;

	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->FindMethod(lpMethodName);

	int res = defFnNames->GetIndexByName(lpMethodName);

	if (-1 != res)
	{
		const CParamDefs::paramdefs& MethodParam = (*defFnNames)[res];
		
		//if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams || MethodParam.m_bIsMethodHaveParamsReceivedByValue)
		if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams)
		{
			pImplicitParams->SetKeyOfUserClassMethod(this, GetKeyOfUserClassMethod(lpMethodName));
			InfoUnlimitsParams::CPtr pInfo = pImplicitParams->CreateInfoAndSetIntoUserClass(this);
		}
		if (MethodParam.m_bIsMethodHaveParamsReceivedByValue)
		{
			m_pParamsByValueManager->SetCurrenMethodKey(this, lpMethodName); //m_pParamsByValueManager->SetCurrenMethodKey(this, GetKeyOfUserClassMethod(lpMethodName));
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

char const *  CComponentClassImpl::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetMethodName(iMethodNum, iMethodAlias);

	if (IsMethodFromBaseContexts(iMethodNum))
	{
		int nMeth;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		assert(pCont.operator bool());
		//if (pCont)
			return pCont->GetMethodName(nMeth, iMethodAlias);
		//return NULL;
	}
	else
		return (*defFnNames)[iMethodNum].Names[iMethodAlias];
}

int  CComponentClassImpl::GetNMethods(void)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetNMethods();

	int nCountMeth = 0;
	if (IsFullyCreated())
		nCountMeth = defFnNames->Size();

	CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	{
		CBLContext *pCont = *iter;
		nCountMeth += pCont->GetNMethods();
	}
	return nCountMeth;
}

int  CComponentClassImpl::HasRetVal(int iMethodNum)const
{
	if(iNumOfSpecialMethod_Virt == iMethodNum)
	{
		return TRUE;
	}
	if(iNumOfSpecialMethod_MySelf == iMethodNum)
	{
		return TRUE;
	}

	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->HasRetVal(iMethodNum);

	if (IsMethodFromBaseContexts(iMethodNum))
	{
		int nMeth;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		assert(pCont.operator bool());
		//if (pCont)
			return pCont->HasRetVal(nMeth);
		//return 0;
	}
	else
		return (*defFnNames)[iMethodNum].HasReturnValue;
}

int CComponentClassImpl::GetNParams(int iMethodNum)const
{
	if(iNumOfSpecialMethod_Virt == iMethodNum)
		return 0;
	if(iNumOfSpecialMethod_MySelf == iMethodNum)
		return 0;

	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetNParams(iMethodNum);

	if (IsMethodFromBaseContexts(iMethodNum))
	{
		int nMeth;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByMethodNumber(iMethodNum, nMeth);
		assert(pCont.operator bool());
		//if (!pCont)
		//	return -1;

		//if (IS_KINDOF_CComponentClass(pCont.operator CBLContext())
		//	pCont->pImplicitParams->m_CountParametersInCallMethod = pImplicitParams->m_CountParametersInCallMethod;

		return pCont->GetNParams(nMeth);
	}

	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
	if (MethodParam.m_bIsMethodHaveUnlimitedCountOfParams)
		return pImplicitParams->GetNParams(const_cast<CComponentClassImpl*>(this), iMethodNum, MethodParam.NumberOfParams);

	return MethodParam.NumberOfParams;
}

BOOL CComponentClassImpl::GetParamDefValueFromClassDefinitionOrModule(int iMethodNum,int iParamNum,CValue * pDefValue)const
{
	const CParamDefs::paramdefs& MethodParam = (*defFnNames)[iMethodNum];
	BOOL bHaveDefinitionsDiff = MethodParam.GetHaveDefinitionsDiff();
	if (bHaveDefinitionsDiff)
		RuntimeError("Метод %s - различаются параметры в описании класса (prm-файл) и его реализации (модуль класса)", pImplicitParams->GetKeyOfUserClassMethod().operator LPCTSTR());

	const ISetOfHierarchy::CParamDefValueEnum ParamDefValueEnum = ISetOfHierarchy::GetHierarchy()->GetParamDefValue(iMethodNum, iParamNum, pDefValue, GetOwner());
	if (ParamDefValueEnum == ISetOfHierarchy::pdve_DefValueIsExists)
		return TRUE;
	else if (ParamDefValueEnum == ISetOfHierarchy::pdve_ParamDefineWithoutDefValue)
		return FALSE;

	return GetParamDefValueFromClassModule(iMethodNum, iParamNum, pDefValue);
}

BOOL CComponentClassImpl::GetParamDefValueFromClassModule(int iMethodNum,int iParamNum,CValue * pDefValue)const
{
	const CParamDefs::paramdefs& ParamDefs = (*defFnNames)[iMethodNum];
	int iRealMethodNum = ParamDefs.nProcPos;

	CCompiledModule* pCompiledModule = m_pMod->GetInternalData()->pCompiledModule;
	CTypedPtrArray<CPtrArray, CBLModuleProcVarDescr*>& VarList = pCompiledModule->ProcArray[iRealMethodNum]->VarList;
	
	// возможен ABR для методов с неопределенным числом параметров
	if (VarList.GetSize() > iParamNum)
	{
		CBLModuleProcVarDescr* pVar = VarList[iParamNum];
		if(pVar->Type2 >= 0)
		{
			*pDefValue = pCompiledModule->ConstArray[pVar->Type2]->val;
			return TRUE;
		}
	}
	return FALSE;
}

int  CComponentClassImpl::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
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

	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetParamDefValue(iMethodNum, iParamNum, pDefValue);

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

	if (funcGetBaseClass == iMethodNum && iParamNum == 0)
	{
		pDefValue->Reset();
		return TRUE;
	}
	else
	if (procThrow == iMethodNum)
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

	return FALSE; // Значит параметры по умолчанию не обрабатываются
}

void  CComponentClassImpl::IncrRef(void)
{
	++m_pOwner->m_RefCount;
}

void  CComponentClassImpl::DecrRef(void)
{
	static bool _bAlreadyInsideDecRefCount = false;
	static CComponentClass* pExceptionInsideDestructorClass = NULL;


	if (m_pOwner->m_RefCount > 1){
		--m_pOwner->m_RefCount;
		return;
	}

	if (m_pOwner->m_RefCount < 1)
		return;

	if (m_pMod)
	{
		//static bool bRunDestructorAfterException = false;
		//
		//if (!bRunDestructorAfterException){ // для обеспечения нормального поведения деструкторов - http://www.1cpp.ru/bugs/show_bug.cgi?id=4227
		//	if (CRuntimeExceptionClass::IsNeedException()){
		//		bRunDestructorAfterException = true;
		//		CRuntimeExceptionClass::Empty();
		//	}
		//}
		//else
		//	bRunDestructorAfterException = false;

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
			if (_bAlreadyInsideDecRefCount && GetOwner() == pExceptionInsideDestructorClass)
			{
				bNeedExceptionHandler = true;
			}
			if (!_bAlreadyInsideDecRefCount)
			{
				CRuntimeExceptionClass::SaveStatusOfTryingBlock();
			} // временно
			//if (_bAlreadyInsideDecRefCount)
			if (!bNeedExceptionHandler && !m_bDonNotCallDestructor)
				int res = ::CallAsProc0(m_pMod, defFnNames->nPosDestructor);

			if (!_bAlreadyInsideDecRefCount)
			{
				_bAlreadyInsideDecRefCount = true;
				pExceptionInsideDestructorClass =  GetOwner();

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

	m_bInsideDestructor = true;

	ClearAllBasesClass();

	m_pOwner->m_RefCount = 0;

	delete GetOwner();
}

// данная функция используется при вызове Строка(ОбъектКласса) в 1С
// разрешаю наследование функции _ПолучитьКод для КОП,
// если в КОП предке данный метод описан с модификатором Экспорт
// если же данного модификатора нет, значит, этот метод не будет наследоваться
//
char const *  CComponentClassImpl::GetCode(void) const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetCode();

	if (m_pMod)
	{
		// разрешаю наследование функции _ПолучитьКод
		// сначала пытаюсь найти метод _ПолучитьКод в текущем модуле,
		// если не нашел, тогда в базовых классах
		//int nPosCode = FindMethod(m_pMod, "_ПолучитьКод", "_GetCode");
		int nPosCode = FindMethod("_ПолучитьКод");
		if (-1 == nPosCode)
			nPosCode = FindMethod("_GetCode");

		if (-1 != nPosCode)
		{
			CValue ret;
			const_cast<CComponentClassImpl*>(this)->CallAsMethod(nPosCode, NULL, true, &ret);
			return ret.GetString().operator LPCTSTR();
		}
	}
	return NULL;
}

class CObjID   CComponentClassImpl::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CComponentClassImpl::GetInternalData(void)
{
	return m_pOwner->GetInternalData();
}

long  CComponentClassImpl::GetTypeID(void)const
{
	return 100;
}

char const *  CComponentClassImpl::GetTypeString(void)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetTypeString();

	return m_NameOfInstance;
}

CType   CComponentClassImpl::GetValueType(void)const
{
	CType tType(100);
	return tType;
}

void  CComponentClassImpl::InitObject(char const * strName)
{
	// чтобы название класса писалось не как пользователь напишет,
	// а бралось из файла описания - artbear
	CString strResult = ISetOfHierarchy::GetMetaDataOfClasses().GetClassNameFromDefinition(strName);

	if (strResult.IsEmpty())
		RuntimeError("Нельзя создать класс без имени класса");

	m_NameOfInstance = strResult;

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	InitInstanceComp();
}

int  CComponentClassImpl::IsExactValue(void)const
{
	return 1;
}

int  CComponentClassImpl::IsOleContext(void)const
{
	int regEBX = 0;
	__asm {
		mov regEBX, ebx // В этом регистре 1С хранит количество параметров при вызове метода
	}
	pImplicitParams->CountParametersInCallMethod( regEBX ); // сохраним это значение
	return 0;
}

int  CComponentClassImpl::IsPropReadable(int iPropNum)const
{
	return GetIsPropAccess(true, iPropNum); //return 1;
}

int  CComponentClassImpl::IsPropWritable(int iPropNum)const
{
	return GetIsPropAccess(false, iPropNum); //return 1;
}

int CComponentClassImpl::GetIsPropAccess(bool bIsReadable, int iPropNum)const
{
	if (-1 == iPropNum)
		return FALSE;

	if (m_bInsideDestructor)
		if (bIsReadable)
			return GetRightClassPtrIfInsideObjectDestructor()->IsPropReadable(iPropNum);
		else
			return GetRightClassPtrIfInsideObjectDestructor()->IsPropWritable(iPropNum);

	if (defFnNames->IsDefineFreePropFunctions())
	{
		LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
		if (szName)
			return TRUE;
	}

	int nCountProps = m_pMod->GetNStaticVars();

	if (!IsPropertyFromBaseContexts(iPropNum)) //if (nCountProps > iPropNum)
		return TRUE;

	int iRealPropNumInSearchedContext;
	CSafeContextPtr<CBLContext> pCont = FindBaseContextByPropertyNumber(iPropNum, iRealPropNumInSearchedContext);
	assert(pCont.operator bool());
	if (bIsReadable)
		return pCont->IsPropReadable(iRealPropNumInSearchedContext);
	else
		return pCont->IsPropWritable(iRealPropNumInSearchedContext);
}

CSafeContextPtr<CBLContext> CComponentClassImpl::FindBaseContextByPropertyNumber(const int iPropNum, int& iPropNumInSearchedContext) const
{
	iPropNumInSearchedContext = -1;
	if (!IsPropertyFromBaseContexts(iPropNum)) // TODO доп. проверка, скорее всего, нужна !
		return CSafeContextPtr<CBLContext>();

	int nCountProps = m_pMod->GetNStaticVars();

	CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	{
		CSafeContextPtr<CBLContext> pResultCont = *iter;

		int nNProps = pResultCont->GetNProps();
		if (iPropNum < nCountProps + nNProps)
		{
			iPropNumInSearchedContext = iPropNum - nCountProps;
			return pResultCont;
		}

		nCountProps += nNProps;
	}

	return CSafeContextPtr<CBLContext>();
}

int  CComponentClassImpl::IsSerializable(void)
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->IsSerializable();

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	int iProc = defFnNames->nPosOfIsSerializableMethod;
	if (-1 == iProc)
		return FALSE;

	CValue retVal;
	int res = ::CallAsFunc0(m_pMod, iProc, retVal);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return retVal.GetNumeric();
}

int  CComponentClassImpl::SaveToString(CString & csStr)
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->SaveToString(csStr);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (!m_pMod)
		return TRUE;

	int iProc = defFnNames->nPosOfSaveToStringMethod;
	if (-1 == iProc)
		return TRUE;

	CValue retVal;
	int res = ::CallAsFunc0(m_pMod, iProc, retVal);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	csStr = retVal.GetString();

	// serialization support
	CString csQuotedPrefix = CSerializeToString::QuotePrefix(m_NameOfInstance);
	CSerializeToString::QuoteString(csStr, csQuotedPrefix);

	return TRUE;
}

//class IV7SerializeToString
BOOL CComponentClassImpl::LoadFromString(LPCSTR szSource)
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->LoadFromString(szSource);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (!m_pMod)
		return TRUE;

	int iProc = defFnNames->nPosOfLoadFromStringMethod;
	if (-1 == iProc)
		return TRUE;

	CValue retVal;
	CValue ValueString(szSource);
	int res = ::CallAsProc1(m_pMod, iProc, ValueString);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return TRUE;
}

void  CComponentClassImpl::SelectByID(class CObjID cID,long lNum)
{
	//CBLContext::SelectByID(cID,lNum);
}

int  CComponentClassImpl::GetNProps(void)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetNProps();

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	int nCountProps = m_pMod->GetNStaticVars();
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
	{
		CBLContext *pCont = *iter;
		nCountProps += pCont->GetNProps();
	}

	nCountProps += m_pDynamicPropertyManager->Size();

	return nCountProps;
}

char const *  CComponentClassImpl::GetPropName(int iPropNum, int B)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetPropName(iPropNum, B);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (defFnNames->IsDefineFreePropFunctions())
	{
		LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
		if (szName)
			return szName;
	}

	int nCountProps = m_pMod->GetNStaticVars();
	if (!IsPropertyFromBaseContexts(iPropNum)) //if (nCountProps > iPropNum)
	{
		CBLVarInfo VarInfo;
		m_pMod->GetStaticVarDescr(iPropNum,VarInfo);
		return VarInfo.GetName();
	}
	else
	{
		int iRealPropNumInSearchedContext;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByPropertyNumber(iPropNum, iRealPropNumInSearchedContext);
		assert(pCont.operator bool());
		return pCont->GetPropName(iRealPropNumInSearchedContext, B);
	}

	return NULL;
}

int  CComponentClassImpl::GetPropVal(int iPropNum,CValue & rValue)const
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->GetPropVal(iPropNum, rValue);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (defFnNames->IsDefineFreePropFunctions())
	{
		LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
		if (szName)
		{
			int ret = ::CallAsFunc1(m_pMod, defFnNames->nPosOnReadFreeProp, rValue, CValue(szName));

			CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

			return ret;
		}
	}

	int nCountProps = m_pMod->GetNStaticVars();

	if (!IsPropertyFromBaseContexts(iPropNum)) //if (nCountProps > iPropNum)
	{
		int res = m_pMod->GetStaticVarValue(iPropNum,rValue,0);
		int iProc = -1;
		if (defFnNames)
			iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, false);

		if (iProc != -1)
		{
			int res = ::CallAsProc1(m_pMod, iProc, rValue);

			CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

			return res;
		}
		return res;
	}
	else
	{
		int iRealPropNumInSearchedContext;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByPropertyNumber(iPropNum, iRealPropNumInSearchedContext);
		assert(pCont.operator bool());
		return pCont->GetPropVal(iRealPropNumInSearchedContext, rValue);
	}
}

int  CComponentClassImpl::SetPropVal(int iPropNum,CValue const & vValue)
{
	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->SetPropVal(iPropNum, vValue);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	if (defFnNames->IsDefineFreePropFunctions())
	{
		LPCTSTR szName = m_pDynamicPropertyManager->FindNameByNumber(iPropNum);
		if (szName)
		{
			int ret = ::CallAsProc2(m_pMod, defFnNames->nPosOnWriteFreeProp, const_cast<LPSTR>(szName), vValue);

			CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

			return ret;
		}
	}

	int nCountProps = m_pMod->GetNStaticVars();

	if (!IsPropertyFromBaseContexts(iPropNum)) //if (nCountProps > iPropNum)
	{

		int iProc = -1;
		if (defFnNames)
			iProc = defFnNames->GetIndexPropetry(GetPropName(iPropNum, 0), m_pMod, true);

		if (iProc != -1)
		{

			int ret = ::CallAsProc1(m_pMod, iProc, vValue);

			CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

			return ret;
		}
		return m_pMod->SetStaticVarValue(iPropNum,vValue,0);
	}
	else
	{
		int iRealPropNumInSearchedContext;
		CSafeContextPtr<CBLContext> pCont = FindBaseContextByPropertyNumber(iPropNum, iRealPropNumInSearchedContext);
		assert(pCont.operator bool());
		return pCont->SetPropVal(iRealPropNumInSearchedContext, vValue);
	}
}

int  CComponentClassImpl::FindProp(char const * Name)const
{
	if(!IsFullyCreated()) // сюда попадаем при назначенном контексе во время компиляции модуля класса
		return -1;

	if (m_bInsideDestructor)
		return GetRightClassPtrIfInsideObjectDestructor()->FindProp(Name);

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
CComponentClassModulePtr CComponentClassImpl::FirstInit(const CString& FullFileName)
{
	CComponentClassModulePtr pModLocale;

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();

	CModuleString ModuleString;
	if (!ModuleString.LoadModule(FullFileName, m_NameOfInstance))
	{
		::ShowMsg(FullFileName.operator LPCTSTR(), mmBlackErr);
		return CComponentClassModulePtr ();
	}
	try
	{
		CPreprocessor::PreprocessStringForUserClass(ModuleString, pHierarchy->GetMapOfSymbolOfPreprocessor(), FullFileName);
	}
	catch (CPreprocessor::ExeptionPreprocessor &ex)
	{
		::ShowMsg("В модуле класса: %s (%s)", mmBlackErr, m_NameOfInstance.operator LPCTSTR(), FullFileName.operator LPCTSTR());
		::ShowMsg(ex.GetErrorString().operator LPCTSTR(), mmBlackErr);

		if (!ex.GetCodeString().IsEmpty())
		{
			::ShowMsg("(%d) %s", mmBlackErr, ex.GetNumString(), ex.GetCodeString().operator LPCTSTR());
		}
		pHierarchy->GetMapOfSymbolOfPreprocessor().RemoveKey("_NOW_PREPARE_CLASS");
		return CComponentClassModulePtr ();
	}

	if (ModuleString.IsEmpty())
		RuntimeError("Класс: %s Из программного файла (%s) не получен модуль!", m_NameOfInstance.operator LPCTSTR(), FullFileName.operator LPCTSTR());

	// для правильной работы спец.методов вирт() и я() нужно перед компиляцией назначить контекст
	pModLocale = CComponentClassModulePtr(CBLModuleWrapper::CreateClassModule(m_NameOfInstance, pCreatedUserClass, ModuleString)); //pModLocale = CComponentClassModulePtr(CBLModuleWrapper::CreateClassModule(m_NameOfInstance, NULL, ModuleString));

	// для возможности использования загрузки 1С++ в теле глобального модуля и создания объектом там же, а не в ПриНачалеРаботыСистемы
	CBLModule7* pGlobalMod = ::GetGlobalModule();
	if (!pGlobalMod->IsLoaded())
		pModLocale->AssignFriendModule(pGlobalMod);

	int nComp = pModLocale->Compile();

	if (nComp != 1) {
		::ShowMsg("Класс: %s Файл: %s компиляция не выполнена!", mmRedErr, m_NameOfInstance.operator LPCTSTR(), FullFileName.operator LPCTSTR());
		return CComponentClassModulePtr ();
	}

	defFnNames = CMyParamDefsWithRefCountPtr(new CMyParamDefs()); //defFnNames = CParamDefsWithRefCountPtr(new CParamDefs());

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

	{
		int nIsFullDefineSerializableFunctions = 0;

		int iIsSerializableProc = pModLocale->FindFunc("КлассСохраняемый");
		iIsSerializableProc = iIsSerializableProc == -1 ? pModLocale->FindFunc("IsSerializable") : iIsSerializableProc;
		if (iIsSerializableProc != -1){
			defFnNames->nPosOfIsSerializableMethod = iIsSerializableProc;
			nIsFullDefineSerializableFunctions++;
		}

		int iSaveToStringProc = pModLocale->FindFunc("СохранитьКлассВСтроку");
		iSaveToStringProc = -1 == iSaveToStringProc ? pModLocale->FindFunc("SaveToString") : iSaveToStringProc;
		if (-1 != iSaveToStringProc){
			defFnNames->nPosOfSaveToStringMethod = iSaveToStringProc;
			nIsFullDefineSerializableFunctions++;
		}

		int iLoadFromStringProc = pModLocale->FindProc("ЗагрузитьИзСтроки", 0);
		iLoadFromStringProc = -1 == iLoadFromStringProc ? pModLocale->FindProc("LoadFromString", 0) : iLoadFromStringProc;
		if (-1 != iLoadFromStringProc){
			defFnNames->nPosOfLoadFromStringMethod = iLoadFromStringProc;
			nIsFullDefineSerializableFunctions++;
		}
		bool bIsFullDefineSerializableFunctions = (3 == nIsFullDefineSerializableFunctions);
		bool bIsNotSerializableFunctions = (0 == nIsFullDefineSerializableFunctions);

		if (!bIsFullDefineSerializableFunctions && !bIsNotSerializableFunctions)
			LogErr("В классе %s не полностью реализованы 3 метода сериализации класса, один из них отсутствует", GetTypeString());
	}

	// artbear - перенесу в Init
	// 	  if(pMainAddIn->m_EnableOptimizate)  // добавим модуль в банк модулей для данного класса
	// 		MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);

	int nProc = pModLocale->GetNProcs(); // Получим общее количество процедур и фукций

	// Инициализируем описание класса дополнительной информацией полученной из описания класса, по его методам
	InitMethodsDefs();

	defFnNames->SetPoolSize(nProc + lastMethod + 1, pModLocale->GetNStaticVars());

	defFnNames->AddParam("GetBaseClass", "ПолучитьБазовыйКласс", funcGetBaseClass, 1, 1);
	defFnNames->AddParam("AssignFriendModule", "ПрисоединитьТекущийМодуль",procAssignFriendModule, 0, 0);

	// artbear - этот функционал закрыт общим решением разработчиков
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

	FirstInitUserMethods(pModLocale);

	try
	{
		CSerializeToString::Add(m_NameOfInstance, GetOwner()->GetRuntimeClass()->m_lpszClassName, m_NameOfInstance);
	}
	catch (CSerializeToString::s_exception e) // может быть после сброса оптимизации для классов
	{
	}

	FirstInitUserMethodsFor_Loader_Publisher_Support();

	return pModLocale;
}

void CComponentClassImpl::FirstInitUserMethods( CComponentClassModulePtr& pModLocale )
{
	const CMetaDataOfClasses& MetaDataOfClasses = ISetOfHierarchy::GetMetaDataOfClasses();

	if(!m_pMethodsWithParamsByValueManager)
		m_pMethodsWithParamsByValueManager = CMethodsMetadataManager::CPtr(new CMethodsMetadataManager);

	int nProc = pModLocale->GetNProcs();

	//m_IsFunctionErrorDefineFlagVector.resize(defFnNames->Size() + nProc);
	int defFnNamesSize = defFnNames->Size();
	int iii = 0;

	for (int count = 1; count < nProc; count++) // первый метод (с номером 0) в модуле 1С всегда без названия, поэтому нумерация с 1
	{
		CBLProcInfo procInfo;
		pModLocale->GetProcInfo(count, procInfo);

		if (procInfo.IsExported() != 1)
			continue;

		const int iMethodNumber = defFnNames->Size();

		CString nameProc(procInfo.GetName());

		const int nParamas   = procInfo.GetNParams();
		const int nVars      = procInfo.GetNVars();
		const int hasRetVal  = procInfo.HasRetVal();
		const int procPos    = procInfo.GetProcPos();

		const CString strKeyOfUserClassMethod = GetKeyOfUserClassMethod(nameProc);

		const bool bMethodIsDefinedInDefinitionFile = MetaDataOfClasses.IsMethExist(m_NameOfInstance, nameProc);

		BOOL bHaveDefinitionsDiff = FALSE;

		const bool bIsFunctionInDefinitionFile = MetaDataOfClasses.IsFunction(strKeyOfUserClassMethod);
		const bool bIsFunction = hasRetVal ? true : false;
		const BOOL iIsFunctionInDefinitionFile = bMethodIsDefinedInDefinitionFile ? 
			(bIsFunctionInDefinitionFile ? TRUE : FALSE) : hasRetVal;
		
		//m_IsFunctionErrorDefineFlagVector[iMethodNumber] = false;

		if (bMethodIsDefinedInDefinitionFile && (hasRetVal != bIsFunctionInDefinitionFile) ){
			bHaveDefinitionsDiff = TRUE;
			if (pMainAddIn->m_EnabledCheckType)
				RuntimeError("Метод %s::%s - различается описание возвращаемого значения (Функция/Процедура) в описании класса (prm-файл) и его реализации (модуль класса)", GetTypeString(), nameProc.operator LPCTSTR());
			else
				// TODO возможно, что лучше сразу выдавать исключение !!
				Msg("Метод %s::%s - различается описание возвращаемого значения (Функция/Процедура) в описании класса (prm-файл) и его реализации (модуль класса)", GetTypeString(), nameProc.operator LPCTSTR());

			//m_IsFunctionErrorDefineFlagVector[iMethodNumber] = true;
		}

		bool bIsMethodHaveParamByValue = m_pMethodsWithParamsByValueManager->IsExists(nameProc); //m_pMethodsWithParamsByValueManager->IsExists(strKeyOfUserClassMethod);

		if (!bIsMethodHaveParamByValue){
			CParamDefs::CArrayOfMethodsParamData_PTR paramFromDefinitionFile;
			bool bIsMethodHaveParamByValueFromDefinitionFile = MetaDataOfClasses.LookupMethHaveParVal(strKeyOfUserClassMethod, paramFromDefinitionFile);
			if (bIsMethodHaveParamByValueFromDefinitionFile){
				m_pMethodsWithParamsByValueManager->SetAt(nameProc, paramFromDefinitionFile);
				bIsMethodHaveParamByValue = true;
			}
		}

		const bool bIsUnLimMethod = MetaDataOfClasses.ThisIsUnLimMethod(strKeyOfUserClassMethod);
		assert(!bIsUnLimMethod || bIsUnLimMethod && bMethodIsDefinedInDefinitionFile); //если метод имеет неограниченное число параметров, значит, он точно описан в defcls.prm и из модуля ничего брать нельзя !!

		const bool bIsMethodHaveParamByValueFromModule = IsMethodHaveParamsByValueInOwnModule(procInfo);

		if (!bMethodIsDefinedInDefinitionFile){
			if (bIsMethodHaveParamByValueFromModule){
				SetMethodsMetaInfoForParamsByValue(strKeyOfUserClassMethod, procInfo);
				bIsMethodHaveParamByValue = bIsMethodHaveParamByValueFromModule;
			}
		}
		else
			if (bIsMethodHaveParamByValue != bIsMethodHaveParamByValueFromModule) {
				bHaveDefinitionsDiff = TRUE;
				if (pMainAddIn->m_EnabledCheckType)
					RuntimeError("Метод %s::%s - различаются параметры ( Знач или без Знач ) в описании класса (prm-файл) и его реализации (модуль класса)", GetTypeString(), nameProc.operator LPCTSTR());
				else
					// TODO возможно, что лучше сразу выдавать исключение !!
					Msg("Метод %s::%s - различаются параметры ( Знач или без Знач ) в описании класса (prm-файл) и его реализации (модуль класса)", GetTypeString(), nameProc.operator LPCTSTR());
			}

			//defFnNames->AddParam(NULL,nameProc, procPos, hasRetVal, nParamas,
			defFnNames->AddParam(NULL,nameProc, procPos, iIsFunctionInDefinitionFile, nParamas,
				bIsUnLimMethod,
				bIsMethodHaveParamByValue,
				bHaveDefinitionsDiff);
	}
}

void CComponentClassImpl::FirstInitUserMethodsFor_Loader_Publisher_Support()
{
	int summa = 0;

	int nPos = defFnNames->GetIndexByName("СобытиеЗагрузки_НачалоЗагрузки");
	if (-1 != nPos){
		const CParamDefs::paramdefs& params = (*defFnNames)[nPos];
		if (2 == params.NumberOfParams){
			defFnNames->m_nPosOfldrInit = params.nProcPos;
			++summa;
		}
	}
	else
		defFnNames->m_nPosOfldrInit = -1;

	nPos = defFnNames->GetIndexByName("СобытиеЗагрузки_ОкончаниеЗагрузки");
	if (-1 != nPos){
		const CParamDefs::paramdefs& params = (*defFnNames)[nPos];
		if (0 == params.NumberOfParams){
			defFnNames->m_nPosOfldrDone = params.nProcPos;
			++summa;
		}
	}
	else
		defFnNames->m_nPosOfldrDone = -1;

	nPos = defFnNames->GetIndexByName("СобытиеЗагрузки_ДобавитьКолонку");
	if (-1 != nPos){
		const CParamDefs::paramdefs& params = (*defFnNames)[nPos];
		if (2 == params.NumberOfParams){
			defFnNames->m_nPosOfldrAddColumn = params.nProcPos;
			++summa;
		}
	}
	else
		defFnNames->m_nPosOfldrAddColumn = -1;

	nPos = defFnNames->GetIndexByName("СобытиеЗагрузки_ДобавитьДанные");
	if (-1 != nPos){
		const CParamDefs::paramdefs& params = (*defFnNames)[nPos];
		if (1 == params.NumberOfParams){
			defFnNames->m_nPosOfldrAddRow = params.nProcPos;
			++summa;
		}
	}
	else
		defFnNames->m_nPosOfldrAddRow = -1;

	nPos = defFnNames->GetIndexByName("СобытиеЗагрузки_СообщениеОбОшибке");
	if (-1 != nPos){
		const CParamDefs::paramdefs& params = (*defFnNames)[nPos];
		if (0 == params.NumberOfParams){
			defFnNames->m_nPosOfGetLastError = params.nProcPos;
			++summa;
		}
	}
	else
		defFnNames->m_nPosOfGetLastError = -1;

	if (5 != summa && 0 != summa){
		Msg("ОШИБКА: в классе %s отсутствует один из методов интерфейса универсальной загрузки данных или не в  одном из методов интерфейса не совпадает количество параметров", GetTypeString());
		Msg("ОШИБКА: или в одном из методов интерфейса количество параметров не соответствует документации", GetTypeString());
	}
}

CBLModule* CComponentClassImpl::Init(const char *file)
{
	// file - имя файла, где расположен текст модуля компоненты

	// Попробуем получить из общего пула класса MapDef список методов и
	// откомпилированный модуль относящийся к данному экземпляру класса,
	// т.к. модули и его команды (процедуры, функции, общие переменные)
	// общие для всех экзепляров дочернего класса. Выполнять компиляцию,
	// теперь необходимо только один раз при первом создании данного класса.
	// Последующие обращения будут располагать готовыми данными

	CComponentClassModulePtr pModLocale;

	CString FullFileName(file);
	ISetOfHierarchy::MakeFullFileName(&FullFileName);

	CMyParamDefsWithRefCountPtr pParDefs = MapDef.GetDefs(m_NameOfInstance);
	if (pMainAddIn->m_EnableOptimizate && pParDefs) {
		defFnNames = pParDefs;

		pModLocale = MapDef.GetModule(m_NameOfInstance);
		
		mapOfMethodsWithParamsByValueManagers.Lookup(m_NameOfInstance, m_pMethodsWithParamsByValueManager);
	}
	else
	{
		// это первое обращение, необходимо заполнить команды и скомпилировать модуль
		pModLocale = FirstInit(FullFileName);

		if(pMainAddIn->m_EnableOptimizate && pModLocale){
			MapDef.AddModule(m_NameOfInstance, pModLocale, defFnNames);
			
			mapOfMethodsWithParamsByValueManagers.SetAt(m_NameOfInstance, m_pMethodsWithParamsByValueManager);
		}
	}
	if (!pModLocale)
		return NULL;

	m_pMod = NULL;

	m_DebugInfo.OpenDebugForm(GetOwner());

	// artbear kms выбрал такой вариант
	//       if (!m_pMod)
	// 			m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedUserClass, pModLocale);
	//
	//       m_pMod->AssignCompiled(*pModLocale);

	assert(NULL == m_pMod);

	//m_pMod = * new CBLModuleWrapper(m_NameOfInstance, pCreatedUserClass, pModLocale.get());
	CBLModuleWrapper* pNewMod = CBLModuleWrapper::CreateClassModule(m_NameOfInstance, pCreatedUserClass, pModLocale.get());
	m_pMod = * pNewMod;

	assert(m_pMod);

	if(!pMainAddIn->m_EnableOptimizate)
	{
		MapDef.DelDefParam(m_NameOfInstance);
		MapDef.DelModule(m_NameOfInstance);

		pModLocale = CComponentClassModulePtr();
	}

	if (m_pCurModule)
		m_pMod->AssignFriendModule(m_pCurModule);

	//m_DebugInfo.AssignDebugModule(GetOwner(), m_pMod); // у kms в отладке этого нет !!
	CSafeModulePtr pMod;
	m_DebugInfo.AssignDebugModule(GetOwner(), pMod); // у kms в отладке этого нет !!
	if (pMod)
		m_pMod = pMod;

	if (m_pMod->Execute() != 1)
		RuntimeError("Выполнить модуль класса: %s при инициализации не удалось!", m_NameOfInstance.operator LPCTSTR());

	m_MapOfModulesAndComp[m_pMod] = GetOwner();

	return m_pMod;
}

class GuardOfFlagCreateWithoutConstructor
{
private:
	bool m_bCreateWithoutConstructorSaved;
public:
	GuardOfFlagCreateWithoutConstructor() { m_bCreateWithoutConstructorSaved = CComponentClassImpl::IsCreateWithoutConstructor(); };
	~GuardOfFlagCreateWithoutConstructor() { if (m_bCreateWithoutConstructorSaved) CComponentClassImpl::SetFlagCreateWithoutConstructor(); };
};

void CComponentClassImpl::InvokeConstructor()
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
				pCont->m_pImpl->InvokeConstructor();
			}
		}
	}

	if (m_pMod)
	{
		if (-1 != defFnNames->nPosConstructor && !IsCreateWithoutConstructor())
		{

			int res = ::CallAsProc0(m_pMod, defFnNames->nPosConstructor);

			CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

		}
	}
}

// IGetModule
CBLModule * CComponentClassImpl::GetHostModule() const
{
	return m_pCurModule;
}

// IGetModule
CBLModule * CComponentClassImpl::GetModule() const
{
	return m_pMod;
}

BOOL CComponentClassImpl::GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const
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
inline int CComponentClassImpl::FindMethod(CBLContext* pContext, CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng)
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
CComponentClass* CComponentClassImpl::VerifyCurrentExecuteContextIsBaseClassForThisClass() const
{
	CComponentClass* const pCompClass = CComponentClassImpl::GetComponentClassCurExModule();

	CBLContext* const pContExecMod = CBLModuleWrapper::GetContextFromModule(CBLModule::GetExecutedModule());
	if (pCompClass != NULL && pCompClass != GetOwner()
		//&& (CBLContext*)GetOwner() == pContExecMod)
		&& GetOwner() == pContExecMod)
		return pCompClass;

	return NULL;

}

CComponentClass* CComponentClassImpl::GetComponentClassCurExModule(CBLModule* pModWrapper)
{
	CBLModule* pMod = pModWrapper == NULL ? CBLModule::GetExecutedModule() : pModWrapper;
	return m_MapOfModulesAndComp[pMod]; //static_cast<CComponentClass*>(m_MapOfModulesAndComp[pMod]);
}

bool CComponentClassImpl::AddBaseClass(const CString& name)
{
	CSafeContextPtr<CBLContext> pCont(name);
	if (pCont)
	{
		vecOfBaseCont.push_back(pCont);

		return true;
	}
	return false;
}

void CComponentClassImpl::ClearAllBasesClass()
{
	// чтобы деструкторы вызывались строго в обратном порядке относительно вызова конструкторов
	while(!vecOfBaseCont.empty())
		vecOfBaseCont.pop_back();
}

void CComponentClassImpl::ReplaceContextInModulesOfBaseClasses(CComponentClass* pMainCont)
{
	CONST_ITER_CONT iEnd(vecOfBaseCont.end());
	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != iEnd; ++iter)
	{
		CBLContext *pCBLContext = *iter;

		// TODO блок if и static_cast<CComponentClass*> проще заменить на dynamic_cast
		if (IS_KINDOF_CComponentClass(pCBLContext))
		{
			CComponentClass* pBaseClassContext = static_cast<CComponentClass*>(pCBLContext);
			CComponentClassImpl* pBaseClassContextImpl = pBaseClassContext->m_pImpl.get();
			// artbear - иначе возможен баг при уничтожении потомка раньше, чем предка
			// см. тесты ТестУдалениеПотомкаРаньшеПредка и ТестУдалениеПотомкаРаньшеПредка2
			//
			// TODO возможна оптимизация с проверкой m_RefCount и
			//		занесением только, если m_RefCount базы больше, чем m_RefCount потомка
			//
			if (!pMainCont)
			{
				pBaseClassContextImpl->m_pMod->GetInternalData()->pGeneralContext = pCBLContext;
				pBaseClassContextImpl->ReplaceContextInModulesOfBaseClasses(pBaseClassContext);
			}
			else
				pBaseClassContextImpl->m_pMod->GetInternalData()->pGeneralContext = pMainCont;
		}
	}

}

void CComponentClassImpl::OnStartInit(void)
{
	if (!pCreatedUserClass)
	{
		// Запоминаем создаваемый объект КОП, это первый вызов метода-инициализации
		pCreatedUserClass = GetOwner();

		CDebug::SetDebugObject(GetOwner());
	}

}

void CComponentClassImpl::OnEndInit(bool bIsBadExit)
{
	pCreatedUserClass=NULL;
	if (bIsBadExit){
		for (ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter){
			CBLContext* pCBLContext = *iter;
			if (IS_KINDOF_CComponentClass(pCBLContext))
				static_cast<CComponentClass*>(pCBLContext)->m_pImpl->SetFlagDonNotCallDestructor();
		}
		vecOfBaseCont.erase(vecOfBaseCont.begin(), vecOfBaseCont.end());
	}
}

void CComponentClassImpl::FinishInit(void)
{
	if (GetOwner() == pCreatedUserClass)
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
	CComponentClassImpl* m_pComponentClass;
	bool m_bIsBadExit;
public:
	CInitGuard(CComponentClassImpl* pComp) :
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
void CComponentClassImpl::InitInstanceComp(void)
{
	CInitGuard InitGuard(this); // сторож освобождения ресурсов

	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
	const CMetaDataOfClasses& MetaDataOfClasses = pHierarchy->GetMetaDataOfClasses();

	CString NameOfComponent = GetTypeString();

	CString NameOfFile;
	if (!MetaDataOfClasses.m_ClassBindFile.Lookup(NameOfComponent, NameOfFile) || NameOfFile.IsEmpty() )
		RuntimeError("Не задано имя файла для класса %s!", NameOfComponent.operator LPCTSTR());

	if ( !Init(NameOfFile) )
		RuntimeError("Внимание! Инициализация класса: %s закончилась неудачно!", NameOfComponent.operator LPCTSTR());

	CMetaDataOfClasses::CStringList_SharedPtr pBaseClassesList;
	if (MetaDataOfClasses.m_ClassesHierarchyMap.Lookup(NameOfComponent, pBaseClassesList) && pBaseClassesList)
	{
		CString sRecursiveHierarchy = "";
		if (MetaDataOfClasses.VerifyRecursiveInBaseClasses(NameOfComponent, NameOfComponent, sRecursiveHierarchy))
			RuntimeError("Ошибка создания класса " + NameOfComponent + "\r\nНайдена рекурсия (предок-потомок): "+sRecursiveHierarchy.operator LPCTSTR());

		CUserClassMetaData::CStringVectorConstIterator iter_end = pBaseClassesList->end();
		for(CUserClassMetaData::CStringVectorIterator iter = pBaseClassesList->begin(); iter != iter_end; iter++)
		{
			CString str = *iter;

			if (!AddBaseClass(str))
				RuntimeError("Базовый класс: %s для класса: %s не создан!", str.operator LPCTSTR(), NameOfComponent.operator LPCTSTR());
		}
	}

	InitGuard.FinishInit();
}

void CComponentClassImpl::ClearOptimizationData(void)
{
	//CComponentClassImpl::MapDef.Clear();
	CComponentClassImpl::MapDef.ClearWithoutDelete();
}

// метод для вызова пользовательского конструктора с несколькими параметрами
// используется в методе ФабрикаОбъектов::Новый()
void CComponentClassImpl::CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const
{
	if (m_pMod)
	{
		int iProc = m_pMod->FindProc(strConstructorName, 0);
		iProc = -1 == iProc ? m_pMod->FindProc(strConstructorName, 1) : iProc;
		if (-1 == iProc)
			RuntimeError("В модуле класса <%s> не удалось найти процедуру-конструктор <%s>", GetTypeString(), strConstructorName.operator LPCTSTR());

		// Конструкторы не должны иметь атрибут Экспорт для большей безопасности
		int iPublicProc = FindMethod(strConstructorName);
		if (-1 != iPublicProc)
			RuntimeError("Конструктор %s::%s не должен иметь атрибут Экспорт", GetTypeString(), strConstructorName.operator LPCTSTR());

		CBLProcInfo info;
		m_pMod->GetProcInfo(iProc, info);
		int iNParams = info.GetNParams();
		if (iNParams < nParamsCount)
			RuntimeError("Конструктору %s::%s передано слишком большое число параметров - нужно %d, а передали %d",
			GetTypeString(), strConstructorName.operator LPCTSTR(), iNParams, nParamsCount);

		if (iNParams > nParamsCount)
			RuntimeError("Конструктору %s::%s передано слишком малое число параметров - нужно %d, а передали %d",
			GetTypeString(), strConstructorName.operator LPCTSTR(), iNParams, nParamsCount);

		int iRes = m_pMod->CallAsProc(iProc, nParamsCount, ppValue);
	}

}

void CComponentClassImpl::InitMethodsDefs()
{
	CParamDefs* param = defFnNames.get();
	if (!ISetOfHierarchy::GetMetaDataOfClasses().m_MapClassNamesToMethodsMap.Lookup(m_NameOfInstance, param->GetPtrOfMapMethodsNameToArrayOfMethodsParamData()))
		param->CleanPtrOfMapMethodsNameToArrayOfMethodsParamData(); //par->pMapFunc = NULL;
}

BOOL CComponentClassImpl::SendMessageOwnMod( CValue &rValue, CValue ** ppValue )
{
	if (!m_pCurModule)
		return FALSE;

	const int iProc = m_pCurModule->FindProc("ОбработкаСобытияОтКласса", 1);
	if (-1 == iProc)
		RuntimeError("Не найдена функция обратного вызова ОбработкаСобытияОтКласса()"); // удобнее исключение artbear

	CValue retValue; // если в функцию передавать напрямую rValue, будет баг с возвращаемым значением
	BOOL res = m_pCurModule->CallAsFunc(iProc, retValue, (*defFnNames)[funcSendMessageOwnMod].NumberOfParams, ppValue);
	rValue = retValue;
	return res;
}

void CComponentClassImpl::GetPathName( CValue &rValue )
{
	CString str = ISetOfHierarchy::GetMetaDataOfClasses().GetNameOfFile(m_NameOfInstance);
	ISetOfHierarchy::MakeFullFileName(&str);
	rValue = str;
}

void CComponentClassImpl::GetEnvContext( CValue &rValue )
{
	CBLContext *pCont  = CBLModuleWrapper::GetContextFromModule(m_pCurModule);
	if (pCont)
		rValue.AssignContext(pCont);
	else
		rValue = CNumeric(0);
}

void CComponentClassImpl::GetParamsList( CValue &rValue, CValue * * ppValue )
{
	//InfoUnlimitsParams::CListContextPtr pContOfList ( pImplicitParams->GetImplicitParamsList(this, ppValue[0]->GetString()) );
	shared_ptr<CValueListContextFriend> pContOfNewList ( pImplicitParams->GetImplicitParamsList(this, ppValue[0]->GetString()) );
	if (!pContOfNewList)
		RuntimeError("Нельзя вызывать функцию GetParamsList в методе с отсутствующими неявными параметрами (...)!");

	rValue.AssignContext(pContOfNewList.get()); //rValue.AssignContext(pContOfList.get());

	//pContOfNewList.reset(); // TODO временно, т.к. на самом деле не нужно
}

BOOL CComponentClassImpl::SetOnIndexParams( CValue &rValue, CValue * * ppValue )
{
	BOOL res = pImplicitParams->SetImplicitParamOfInd(this, ppValue[0]->GetString(), ppValue[1]->GetNumeric().operator long(), ppValue[2]);
	rValue = CNumeric(res);
	return res;
}

void CComponentClassImpl::ThrowException(CValue ** ppValue)
{
	CValue& value = *ppValue[0];
	if (2 == value.GetTypeCode()) // строка
		CRuntimeExceptionClass::RuntimeError(value, value.GetString());
	else
		CRuntimeExceptionClass::RuntimeError(value);
}

void CComponentClassImpl::AddDynamicProperty(CValue ** ppValue)
{
	// TODO подобная проверка должна работать и для классов-наследников, а не только для текущего КОП-а
	if (!defFnNames->IsDefineFreePropFunctions())
		RuntimeError("КОП::ДобавитьДинамическоеСвойство: в классе должны быть реализованы соответствующие методы _ПриЧтенииСвойства и _ПриЗаписиСвойства");

	CValue& value = *ppValue[0];

	CString errMsg("КОП::ДобавитьДинамическоеСвойство: необходимо передавать строку");
	ValidateParamNotEmptyString(value, errMsg);

	CComponentClass* pHandlerClass = GetOwner();
	CComponentClass* pBaseClass = VerifyCurrentExecuteContextIsBaseClassForThisClass();
	if (pBaseClass)
		pHandlerClass = pBaseClass;

	int iNewPropertyNumber = pHandlerClass->GetNProps();

	pHandlerClass->m_pImpl->m_pDynamicPropertyManager->Add(value.GetString(), iNewPropertyNumber);
}

CParamDefs::CArrayOfMethodsParamData_PTR CComponentClassImpl::GetMethodParamData(int iMethodNum) const
{
	CParamDefs::CPtrOfMapMethodsNameToArrayOfMethodsParamData pMapFunc = GetDefFnNamesPtr()->GetPtrOfMapMethodsNameToArrayOfMethodsParamData();

	if (!pMapFunc)
		return CParamDefs::CArrayOfMethodsParamData_PTR(); // NULL

	CParamDefs::CArrayOfMethodsParamData_PTR param = (*GetDefFnNamesPtr())[iMethodNum].m_pArrayOfMethodsParamData;
	if (!param)
	{
		CString NameMeth = GetMethodName(iMethodNum, 1);
		pMapFunc->Lookup(NameMeth, param);
	}

	return param;
}

CComponentClassImpl* CComponentClassImpl::GetRightClassPtrIfInsideObjectDestructor() const
{
	if (m_bInsideDestructor){
		CComponentClass* const pCompClass = CComponentClassImpl::GetComponentClassCurExModule();
		CComponentClassImpl* const pCompClassImpl = pCompClass->m_pImpl.get();

		// TODO вариант с получением контекста из CBLModule::GetExecutedModule() на всякий случай
		//CBLContext* const pContExecMod = CBLModuleWrapper::GetContextFromModule(CBLModule::GetExecutedModule());
		return pCompClassImpl;
	}
	return NULL;
}


// Получить количество базовых объектов.
DWORD CComponentClassImpl::GetBaseObjCount()
{
	return vecOfBaseCont.size();
}

// Получить имя базового объекта по индексу.
CString CComponentClassImpl::GetBaseObjName(DWORD idx)
{	
	// TODO проверить индекс на границы
	return vecOfBaseCont[idx]->GetTypeString();
}

// Получить базовый объект по индексу.
CBLContext* CComponentClassImpl::GetBaseObj(DWORD idx)
{
	// TODO проверить индекс на границы
	return vecOfBaseCont[idx];
}

// Для upcast'а
CBLContext* CComponentClassImpl::GetDerivedObj()
{
	// TODO вернуть правильный объект
	return NULL;
}

bool CComponentClassImpl::IsMethodHaveParamsByValueInOwnModule( const CBLProcInfo &procInfo) const
{
	const int iParamByValue = 2;
	LPCSTR szMethodName = procInfo.GetName();
	int nVars = procInfo.GetNVars();
	int iMethodNum = procInfo.GetProcPos(); //m_ProcID

	CCompiledModule* pCompiledModule = procInfo.GetModule()->GetInternalData()->pCompiledModule;
	CCompileProcInfo* pCompileProcInfo = pCompiledModule->ProcArray[iMethodNum];
	for (int i = 0; i < nVars;i++)
	{
		CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i];
		if (iParamByValue == pVar->Type){
			// TODO баг - если в методе есть хотя бы один параметр со Знач, все остальные также будут передаваться по Значению, а не по ссылке :( !!
			return true;
		}
	}
	return false;
}

void CComponentClassImpl::SetMethodsMetaInfoForParamsByValue( const CString& strKeyOfUserClassMethod, const CBLProcInfo &procInfo)
{
	const int iParamByValue = 2;
	LPCSTR szMethodName = procInfo.GetName();
	const int nVars = procInfo.GetNVars();
	const int iMethodNum = procInfo.GetProcPos(); //m_ProcID
	const bool bIsFunc = 1 == procInfo.HasRetVal();

	CCompiledModule* pCompiledModule = procInfo.GetModule()->GetInternalData()->pCompiledModule;
	CCompileProcInfo* pCompileProcInfo = pCompiledModule->ProcArray[iMethodNum];
	
	CParamDefs::CArrayOfMethodsParamData_PTR pArr;
	const CMetaDataOfClasses& metaDataOfClasses = ISetOfHierarchy::GetHierarchy()->GetMetaDataOfClasses();
	if (metaDataOfClasses.ThisIsUnLimMethod(strKeyOfUserClassMethod)){
		if (!metaDataOfClasses.LookupUnLimMethod(strKeyOfUserClassMethod, pArr))
			pArr = CParamDefs::Create_ArrayOfMethodsParamData();
	}
	else
		pArr = CParamDefs::Create_ArrayOfMethodsParamData();

	const int nParams = nVars + (bIsFunc ? 1 : 0);

	pArr->resize(max(pArr->size(), nParams));

	if (bIsFunc){
		// TODO artbear - в инфе о параметрах метода первым хранится инфа о возвращаемом значении, если это функция
		CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(CString("#return"), "");
		pArr->SetAt(0, pNewData); // TODO возможно, все-таки SetAtGrow
	}

	const int iInit = (bIsFunc ? 1 : 0);
	for (int i = iInit; i < nParams;i++)
	{
		CBLModuleProcVarDescr* pVar = pCompileProcInfo->VarList[i-iInit];
		bool bParamByValue = iParamByValue == pVar->Type;
		//LogErr("SetMethodsMetaInfoForParamsByValue pVar->Name = <%s>", pVar->Name.operator LPCTSTR());
		CMethodsParamData::Ptr pNewData = CMethodsParamData::Create(pVar->Name, "", "", bParamByValue);

		//pArr->SetAtGrow(i, pNewData);
		pArr->SetAt(i, pNewData); // TODO возможно, все-таки SetAtGrow
	}

	m_pMethodsWithParamsByValueManager->SetAt(szMethodName, pArr);//m_pMethodsWithParamsByValueManager->SetAt(strKeyOfUserClassMethod, pArr);
}

// интерфейс Utils1cpp::IQueryInterface_Loader_Publisher

BOOL CComponentClassImpl::IsLoader() const
{ return defFnNames->IsDefineLoaderFunctions() ? TRUE : FALSE; }; // TODO пока не реализовано

BOOL CComponentClassImpl::IsPublisher() const 
{ return FALSE; }; // TODO пока не реализовано

// интерфейс IResultLoader
HRESULT CComponentClassImpl::ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	assert(defFnNames->IsDefineLoaderFunctions());

	CValue vColCount(nColCount);
	CValue vRowCount(nRowCount);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	CValue retVal;
	int res = ::CallAsFunc2(m_pMod, defFnNames->m_nPosOfldrInit, retVal, vColCount, vRowCount);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	if (!m_pCBLSimpleVector) // TODO см. примечание в ldrDone
		m_pCBLSimpleVector = shared_ptr<CBLSimpleVector>(new CBLSimpleVector);
	m_iColCount = vColCount.GetNumeric();

	return retVal.GetNumeric();
}

HRESULT CComponentClassImpl::ldrDone() throw()
{
	assert(defFnNames->IsDefineLoaderFunctions());
	assert(NULL != m_pCBLSimpleVector.get());

	// TODO здесь можно и сразу удалять m_pCBLSimpleVector, но для оптимизации, наверное, проще не удалять, а только очищать
	m_pCBLSimpleVector->sc_Clear();

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	CValue retVal;
	int res = ::CallAsFunc0(m_pMod, defFnNames->m_nPosOfldrDone, retVal);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return retVal.GetNumeric();
}

HRESULT CComponentClassImpl::ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
{
	assert(defFnNames->IsDefineLoaderFunctions());

	CValue vColName(szColName);
	int type = pType ? pType->type : 0;
	CValue vType(type);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	CValue retVal;
	int res = ::CallAsFunc2(m_pMod, defFnNames->m_nPosOfldrAddColumn, retVal, vColName, vType);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return retVal.GetNumeric();
}

HRESULT CComponentClassImpl::ldrAddRow(CValue const* const* ppValues) throw()
{
	assert(defFnNames->IsDefineLoaderFunctions());

	m_pCBLSimpleVector->sc_Clear();
	CValue vContainer; vContainer.AssignContext(m_pCBLSimpleVector.get());
	for (int i = 0; i < m_iColCount; i++)
		m_pCBLSimpleVector->sc_Add(*ppValues[i]);

	CRuntimeExceptionClass::SaveStatusOfTryingBlock();

	CValue retVal;
	int res = ::CallAsFunc1(m_pMod, defFnNames->m_nPosOfldrAddRow, retVal, vContainer);

	CRuntimeExceptionClass::IfNeedExceptionThenThrowException();

	return retVal.GetNumeric();
}

LPCSTR CComponentClassImpl::GetLastError() throw()
{
	assert(defFnNames->IsDefineLoaderFunctions());

	return "";
}

// end IResultLoader

class CMethodsMetadata  // класс можно копировать
{
public:
	typedef boost::shared_ptr<CMethodsMetadata> CPtr;

	CMethodsMetadata();
	~CMethodsMetadata();

	int GetParamsCount() const;
	bool IsHaveUnlimitedParams() const;
	bool IsHaveParamsReceivedByValue() const;
	bool IsHaveDeclarationDiff() const;

	const CMethodsParamData::Vector& GetParamsVector() const; //{ return *m_pParamsVector; }
	void SetParamsVector(const CMethodsParamData::VectorPTR& val); //{ m_pParamsVector = val; }

	const InfoUnlimitsParams& GetUnlimitsParamsInfo() const; // { return *m_pUnlimitsParamsInfo; }
	void SetUnlimitsParamsInfo(const InfoUnlimitsParams::CPtr& val); // { m_pUnlimitsParamsInfo = val; }
};