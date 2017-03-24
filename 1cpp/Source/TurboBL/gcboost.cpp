// attachedcontext.cpp
#include "stdafx.h"
#include "gcboost.h"

DWORD* CGCBoost::m_pMyVTable=NULL;
DWORD* CGCBoost::m_pRealVTable=NULL;
CMapStringToPtr CGCBoost::SGCData::m_DataMap;

/*
extern HANDLE hProcess;

typedef unsigned __int64 ui64;
// Класс для перехвата функций путем установки переходника в начало функции.
class CTrapSwap
{
public:
	CTrapSwap():m_pFunc(NULL), m_SwapBuf(0){}

	template<typename T>
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, T newFunc)
	{
		DWORD* pFunc=reinterpret_cast<DWORD*>(GetProcAddress(hDll, strFuncName));
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	template<typename T>
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, T newFunc)
	{
		SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
	}

	template<typename T, typename M>
	void SetTrapOnImportFunc(T oldFunc, M newFunc)
	{
		BYTE* ptr=*reinterpret_cast<BYTE**>(&oldFunc);
		ptr+=2;
		DWORD* pFunc=**reinterpret_cast<DWORD***>(ptr);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	 // TODO перехват работает, но исходный адрес через Swap получить не удается
	// например, исходники Визуал1С++ глобальный метод ResString()
	template<typename T, typename M>
	void SetTrapOnFunc(T oldFunc, M newFunc)
	{
		DWORD* pFunc=*reinterpret_cast<DWORD**>(&oldFunc);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	void Swap()
	{
		ui64 t=*m_pFunc;
		*m_pFunc=m_SwapBuf;
		m_SwapBuf=t;
	}
	ui64* m_pFunc;
protected:
	ui64 m_SwapBuf;
	void _SetTrap(DWORD* pFunc, DWORD pNewFunc)
	{
		m_pFunc=reinterpret_cast<ui64*>(pFunc);
		DWORD old;
		VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &old);
		// Формируем джамп на адрес новой функции
		m_SwapBuf = (static_cast<ui64>(pNewFunc - reinterpret_cast<DWORD>(pFunc)-5)<<8) | 0xE9;
		// И вписываем в начало функции сформированный джамп
		Swap();
		FlushInstructionCache(hProcess, m_pFunc, 8);
	}
};

struct trapswaper
{
	CTrapSwap& trap;
	trapswaper(CTrapSwap& t):trap(t){trap.Swap();}
	~trapswaper(){trap.Swap();}
};
*/
static CTrapSwap trAssign;
static CTrapSwap trGrContDestroy;
static CTrapSwap trCompile;

CGCBoost::~CGCBoost(void){}

int	CGCBoost::FindProp(char const *name)const
{
	return GetData()->FindProp(name);
}

int	CGCBoost::GetPropVal(int iPropNum,class CValue &PropVal)const
{
	return GetData()->GetPropVal(this,iPropNum,PropVal);
}
int	CGCBoost::SetPropVal(int iPropNum,CValue const &PropVal)
{
	return GetData()->SetPropVal(this,iPropNum,PropVal);
}
int	CGCBoost::IsPropReadable(int iPropNum)const
{
	return GetData()->IsPropReadable(this,iPropNum);
}
int	CGCBoost::IsPropWritable(int iPropNum)const
{
	return GetData()->IsPropWritable(this,iPropNum);
}
int	CGCBoost::FindMethod(char const *strMethodName)const
{
	return GetData()->FindMethod(strMethodName);
}

// artbear
int CGCBoost::GetNMethods(void)const
{
	return GetData()->GetNMethods();
}

int CGCBoost::GetNProps(void)const
{
	return GetData()->GetNProps();
}

//virtual char const *	GetMethodName(int,int)const;					//27 // artbear
char const *	CGCBoost::GetMethodName(int iMethNum, int iFlag)const
{
	return GetData()->GetMethodName(this, iMethNum, iFlag);
}
char const *	CGCBoost::GetPropName(int iMethNum, int iFlag)const
{
	return GetData()->GetPropName(this, iMethNum, iFlag);
}
// end

int	CGCBoost::GetNParams(int iMethNum)const
{
	return GetData()->GetNParams(iMethNum);
}
int	CGCBoost::GetParamDefValue(int iMethNum,int iParamNum,class CValue *pDefVal)const
{
	return GetData()->GetParamDefValue(this,iMethNum,iParamNum,pDefVal);
}
int	CGCBoost::HasRetVal(int iMethNum)const
{
	return GetData()->HasRetVal(iMethNum);
}
int	CGCBoost::CallAsProc(int iMethNum,class CValue **ppParam)
{
	return GetData()->CallAsProc(this,iMethNum,ppParam);
}
int	CGCBoost::CallAsFunc(int iMethNum,class CValue& retVal,class CValue **ppParam)
{
	return GetData()->CallAsFunc(this,iMethNum,retVal,ppParam);
}

BOOL CGCBoost::Boost(CBLModule7 *pMod, BOOL optimize)
{
	//CBLContext* pCont=pMod->pIntInfo->pGeneralContext;
	CBLContext* pCont = CBLModuleWrapper::GetContextFromModule(pMod);
	if(!pCont)
		return FALSE;
// artbear
//	if(!pCont->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CGroupContext)))
	if(!IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext))
		return FALSE;

	DWORD*& addrVTable = *(DWORD**)(CBLContext*)pCont;
	//if(*(DWORD**)pCont==m_pMyVTable)
	if(addrVTable == m_pMyVTable)
		return TRUE;
	SGCData* pData=SGCData::GetData(pMod, pCont, optimize);
// 	CGetDoc7* pDoc = CSafeContextPtr::GetDoc7(pCont);
// 	CGetDoc7* pDoc = CSafeContextPtr<CBLContext>::GetDoc7(pCont);
	CGetDoc7* pDoc = ::GetDoc7(pCont);

	pCont->m_FlagAutoDestroy=reinterpret_cast<int>(new SGCInfo(pMod, pData, pDoc));

	if(!m_pMyVTable)
	{
		//m_pRealVTable=*(DWORD**)pCont;
		m_pRealVTable = addrVTable;
		CGCBoost* pBoost=new CGCBoost;
		DWORD* pMyVTable=*(DWORD**)pBoost;
		delete pBoost;
		m_pMyVTable=new DWORD[64]; // TODO лик ? 
		memcpy(m_pMyVTable,m_pRealVTable,256);

		m_pMyVTable[18]=pMyVTable[18];	// GetNProps artbear
		m_pMyVTable[19]=pMyVTable[19];	// FindProp
		m_pMyVTable[20]=pMyVTable[20];	// GetPropName artbear
		m_pMyVTable[21]=pMyVTable[21];	// GetPropVal
		m_pMyVTable[22]=pMyVTable[22];	// SetPropVal
		m_pMyVTable[23]=pMyVTable[23];	// IsPropReadable
		m_pMyVTable[24]=pMyVTable[24];	// IsPropWritable
		m_pMyVTable[25]=pMyVTable[25];	// GetNMethods artbear
		m_pMyVTable[26]=pMyVTable[26];	// FindMethod
		m_pMyVTable[27]=pMyVTable[27];	// GetMethodName artbear
		m_pMyVTable[28]=pMyVTable[28];	// GetNParams
		m_pMyVTable[29]=pMyVTable[29];	// GetParamDefValue
		m_pMyVTable[30]=pMyVTable[30];	// HasRetVal
		m_pMyVTable[31]=pMyVTable[31];	// CallAsProc
		m_pMyVTable[32]=pMyVTable[32];	// CallAsProc
	}
	//*(DWORD**)pCont=m_pMyVTable;
	addrVTable = m_pMyVTable;
	return TRUE;
}

int CGCBoost::AssignAndCompile(CBLModule7* pMod)
{
	int ret = 0;
	CBLModule &pPreCompileModule=((CTxtModuleReader*)this)->pPreCompileModule;
	CString txt=((CTxtModuleReader*)this)->module;
	BOOL optimize=TRUE;	// Показывает, надо ли создавать для этого контекста
						// отдельную карту методов (если используется ЗагрузитьИзФайла)
	if((pPreCompileModule.IsCompiled() == 1) && txt[0]!='#')
	{
		pMod->AssignCompiled(pPreCompileModule);
		ret=1;
	}
	else
	{
		if(txt[0]=='#')
			optimize=FALSE;
		OnLoadSource(&txt);
		pMod->AssignSource(txt);
		ret=pMod->Compile();
		if(ret==1)
			pPreCompileModule.AssignCompiled(*pMod);
	}
	if(ret)
		Boost(pMod,optimize);
	return ret;
}

int CGCBoost::WrapCompile()
{
	CBLModule7* pMod=reinterpret_cast<CBLModule7*>(this);
	trapswapper tr(trCompile);
	int iGetKind = pMod->GetKind();
	BOOL bNormalModule = 7 == iGetKind || 8 == iGetKind ; // TODO
	if(bNormalModule)
	{
	    CString& txt = pMod->pIntInfo->mSource;
		CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
		if (!CComponentClass::StillInitInstanceComp() || pHierarchy->IsOpenCompForm())
		{
			#ifdef _DEBUG
				CString FullFileName = "";
			#else
				CString FullFileName = pMod->GetFullName();
			#endif
			try
			{
				// Содержит имена загруженных файлов директивой include в качестве ключей
				CMapStringToString mapLoadedIncludeFiles;

				CPreprocessor::Preprocess(false, txt, pHierarchy->m_SymbolOfPreprocessor, mapLoadedIncludeFiles, FullFileName);
			}
			catch(CPreprocessor::ExeptionPreprocessor &ex)
			{
				CString strErr;
				strErr.Format("В модуле: %s", FullFileName);
				pBkEndUI->DoMessageLine(strErr, mmBlackErr);
				strErr = ex.GetErrorString();
				pBkEndUI->DoMessageLine(strErr, mmBlackErr);
				strErr.Empty();
				if (!ex.GetCodeString().IsEmpty())
				{
					strErr.Format("(%d) %s",ex.GetNumString(),ex.GetCodeString());
					pBkEndUI->DoMessageLine(strErr, mmBlackErr);
				}
			}
	    }
	}
	int ret = pMod->Compile();
	if(ret && bNormalModule)
	{
		long id;
		CString txt;
		pMod->GetID(txt, id);
		if(id<=0)
			Boost(pMod, FALSE);
	}
	return ret;
}

void CGCBoost::GroupDestruct()
{
	SGCInfo* pInfo=*(DWORD**)this==m_pMyVTable ? GetInfo() : NULL;
	trapswapper tr(trGrContDestroy);
	reinterpret_cast<CGroupContext*>(this)->CGroupContext::~CGroupContext();
	delete pInfo;
}

void CGCBoost::Init()
{
	trAssign.SetTrapOnFunc(&CTxtModuleReader::AssignAndCompile, &AssignAndCompile);
	trGrContDestroy.SetTrapByName("bkend.dll", "??1CGroupContext@@UAE@XZ", &GroupDestruct);
	trCompile.SetTrapOnFunc(&CBLModule::Compile, &WrapCompile);
}

CBLModule* CGCBoost::GetModuleByContext(const CGroupContext* pCont)
{
	if(pCont->m_FlagAutoDestroy > 100)
		return ((CGCBoost::SGCInfo*)pCont->m_FlagAutoDestroy)->m_pMod;

	return NULL;
}

// обновление данных контекста будет использоваться, например, при
// динамическом добавлении атрибутов на форму
void CGCBoost::RefreshGroupContextData(CGroupContext* pGroupContext)
{
	DWORD*& addrVTable = *(DWORD**)(CBLContext*)pGroupContext;

	if(addrVTable != m_pMyVTable)
		RuntimeError("ВК 1C++ - Настройка TurboBL выключена, использование возможностей TurboBL невозможно");

	CBLModule7* pMod = static_cast<CBLModule7*>(GetModuleByContext(pGroupContext));

//	//if(*(DWORD**)pCont==m_pMyVTable)
//	if(addrVTable == m_pMyVTable)// TODO убрать заглушку !!
//		return TRUE;

	BOOL optimize=TRUE;	// Показывает, надо ли создавать для этого контекста
						// отдельную карту методов (если используется ЗагрузитьИзФайла)

	// TODO рефакторинг - дубль здесь и в WrapCompile
	int iGetKind = pMod->GetKind();
	BOOL bNormalModule = 7 == iGetKind || 8 == iGetKind ; // TODO
	if(bNormalModule)
	{
		long id;
		CString txt;
		pMod->GetID(txt, id);
		if(id<=0)
			optimize = FALSE;
	}

	//DWORD*& addrVTable = *(DWORD**)(CBLContext*)pGroupContext;
	addrVTable = m_pRealVTable;
	delete [] m_pMyVTable;
	m_pMyVTable = NULL;

	SGCInfo* pContextInfo = reinterpret_cast<SGCInfo*>(pGroupContext->m_FlagAutoDestroy);

	// TODO возможен лик при выделение виртуальной таблицы
	Boost(pMod, optimize);

// 1	// TODO удалить старый объект !!
// 	SGCData* pOldData = SGCData::GetDataByModule(pMod);
// 	
// 	SGCData* pData = SGCData::Create(pMod, pGroupContext, optimize);
// 
// 	SGCData* pNewData = SGCData::GetDataByModule(pMod);
// 
// 	CGetDoc7* pDoc = ::GetDoc7(pGroupContext);

	// TODO удалить старый объект !!
	SGCData* pData = pContextInfo->m_pData;
	bool bNeedDeleteData = !pData->m_Destroy;
	delete pContextInfo;
	if (bNeedDeleteData)
		delete pData;
	
	int nCount = pGroupContext->GetNMethods(); // test
}
//20209AFC