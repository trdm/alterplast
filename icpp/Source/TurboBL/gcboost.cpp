// attachedcontext.cpp
#include "stdafx.h"
#include "gcboost.h"

#include "..\MethodsWrapper.h"

// для возможности компиляции без Перехватчика
#define USE_GROUP_CONTEXT_HOOK

#ifdef USE_GROUP_CONTEXT_HOOK
	#include "..\System\DllHooker.h"
#endif

DWORD* CGCBoost::m_pMyVTable=NULL;
DWORD* CGCBoost::m_pRealVTable=NULL;
CMapStringToPtr CGCBoost::SGCData::m_DataMap;

static CTrapSwap trAssign;
static CTrapSwap trGrContDestroy;
static CTrapSwap trCompile;
static CTrapSwap trAccept;
static CTrapSwap trHashMethods;
static CTrapSwap trHashProps;
static CTrapSwap trFindMethod1;
static CTrapSwap trFindProp1;

CGCBoost::~CGCBoost(void){}

int	CGCBoost::FindProp(char const *name)const
{
	return GetData()->FindProp(name);
}

int	CGCBoost::GetPropVal(int iPropNum,CValue &PropVal)const
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
int	CGCBoost::GetParamDefValue(int iMethNum,int iParamNum,CValue *pDefVal)const
{
	return GetData()->GetParamDefValue(this,iMethNum,iParamNum,pDefVal);
}
int	CGCBoost::HasRetVal(int iMethNum)const
{
	return GetData()->HasRetVal(iMethNum);
}
int	CGCBoost::CallAsProc(int iMethNum,CValue **ppParam)
{
	return GetData()->CallAsProc(this,iMethNum,ppParam);
}
int	CGCBoost::CallAsFunc(int iMethNum,CValue& retVal,CValue **ppParam)
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
	CBLModule &pPreCompiledModule=((CTxtModuleReader*)this)->pPreCompiledModule;
	CString txt=((CTxtModuleReader*)this)->module;
	BOOL optimize=TRUE;	// Показывает, надо ли создавать для этого контекста
						// отдельную карту методов (если используется ЗагрузитьИзФайла)
	if((pPreCompiledModule.IsCompiled() == 1) && txt[0]!='#')
	{
		pMod->AssignCompiled(pPreCompiledModule);
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
			pPreCompiledModule.AssignCompiled(*pMod);
	}
	if(ret)
		FirstBoost(pMod,optimize);
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
		ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy();
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

				//CPreprocessor::Preprocess(false, txt, pHierarchy->m_SymbolOfPreprocessor, mapLoadedIncludeFiles, FullFileName);
				CPreprocessor::PreprocessString(txt, pHierarchy->GetMapOfSymbolOfPreprocessor(), mapLoadedIncludeFiles, FullFileName);
			}
			catch(CPreprocessor::ExeptionPreprocessor &ex)
			{
				CString strErr;
				strErr.Format("В модуле: %s", FullFileName.operator LPCTSTR());
				pBkEndUI->DoMessageLine(strErr, mmBlackErr);
				strErr = ex.GetErrorString();
				pBkEndUI->DoMessageLine(strErr, mmBlackErr);
				strErr.Empty();
				if (!ex.GetCodeString().IsEmpty())
				{
					strErr.Format("(%d) %s",ex.GetNumString(),ex.GetCodeString().operator LPCTSTR());
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
			FirstBoost(pMod, FALSE);
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

int CGCBoost::Accept()
{
	trapswapper tr(trAccept);
	CGroupContext* pCont = reinterpret_cast<CGroupContext*>(this);
	int ret = pCont->Accept();

	if(*(DWORD**)pCont==m_pMyVTable)
	{
		*(DWORD**)pCont=m_pRealVTable;
		SGCInfo* pInfo = GetInfo();

		std::vector<CBLPtr<CBLContext> > vcont;
		std::swap(vcont, pInfo->m_AddedContexts);

		CBLModule7* pMod = static_cast<CBLModule7*>(GetModule());
		delete pInfo;
		Boost(pMod, FALSE);

		pInfo = GetInfo();
		std::swap(vcont, pInfo->m_AddedContexts);
	}

	return ret;
}

void CGCBoost::HashMethods(int nParam)
{
// 	trapswapper tr(trHashMethods);
// 	CBLContext* pCont = (CBLContext*)this;
// 	pCont->HashMethods(nParam);
// 	Msg("HashMethods: %s %i", pCont->GetRuntimeClass()->m_lpszClassName, nParam);
}

void CGCBoost::HashProps(int nParam)
{
}

int CGCBoost::FindMethod1(LPCSTR szName) const
{
	CBLContext* pCont = (CBLContext*)this;

	trapswapper tr(trFindMethod1);
	int result = pCont->FindMethod(szName);

	LPCSTR szCont = pCont->GetRuntimeClass()->m_lpszClassName;
	CString cs(szCont);
	cs += ":";
	cs += szName;

	Msg("FindMethod: %s.%s = %i", szCont, szName, result);
	return result;
}

int CGCBoost::FindProp1(LPCSTR szName) const
{
	CBLContext* pCont = (CBLContext*)this;

	trapswapper tr(trFindProp1);
	int result = pCont->FindProp(szName);

	LPCSTR szCont = pCont->GetRuntimeClass()->m_lpszClassName;
	CString cs(szCont);
	cs += ":";
	cs += szName;

	Msg("FindProp: %s.%s = %i", szCont, szName, result);
	return result;
}

void CGCBoost::Init()
{
	HMODULE hmBkEnd = GetModuleHandle("bkend.dll");
	trAssign.SetTrapOnFunc(&CTxtModuleReader::AssignAndCompile, &AssignAndCompile);
	trGrContDestroy.SetTrapByName(hmBkEnd, "??1CGroupContext@@UAE@XZ", &GroupDestruct);
	trCompile.SetTrapOnFunc(&CBLModule::Compile, &WrapCompile);
	trAccept.SetTrapOnFunc(&CGroupContext::Accept, &Accept);
	trHashMethods.SetTrapByName(hmBkEnd, "?HashMethods@CBLContext@@QAEXH@Z", &HashMethods);
	trHashProps.SetTrapByName(hmBkEnd, "?HashProperties@CBLContext@@QAEXH@Z", &HashProps);

// 	временно
//  trFindMethod1.SetTrapByName(hmBkEnd, "?FindMethod@CBLContext@@UBEHPBD@Z", &FindMethod1);
//  trFindProp1.SetTrapByName(hmBkEnd, "?FindProp@CBLContext@@UBEHPBD@Z", &FindProp1);
}

void CGCBoost::Done()
{
	// broken chain possibility!
	if (trHashMethods.IsTrapped())
		trHashMethods.Swap();

	if (trHashProps.IsTrapped())
		trHashProps.Swap();
}

CBLModule* CGCBoost::GetModuleByContext(const CGroupContext* pCont)
{
	if(pCont && pCont->m_FlagAutoDestroy > 100)
		return ((CGCBoost::SGCInfo*)pCont->m_FlagAutoDestroy)->m_pMod;

	return NULL;
}

// обновление данных контекста будет использоваться, например, при
// динамическом добавлении атрибутов на форму
void CGCBoost::RefreshGroupContextData(CGroupContext* pGroupContext)
{
	pGroupContext->Accept();

	int nCount = pGroupContext->GetNMethods(); // test
}

BOOL CGCBoost::FirstBoost( CBLModule7 *pMod,BOOL optimize )
{
	BOOL bRet = Boost(pMod,optimize);

#ifdef USE_GROUP_CONTEXT_HOOK
	if (bRet)
		CHookGroupContext::GetHooker()->OnAfterModuleCompile(pMod);
#endif

	return bRet;
}