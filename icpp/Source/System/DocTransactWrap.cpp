// DocTransactWrap.cpp: implementation of the CDocTransactWrap class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocTransactWrap.h"

CDocTransactWrap::CDocTransactWrapPtr CDocTransactWrap::pWrapperForDocTransact;

CDocTransactWrap::CDocTransactWrap() : m_bHaveSetHooks(false)
{};

void CDocTransactWrap::SetWrapper(const CValue& DocKindValue, const CValue& UserClassContValue)
{
	::ValidateParamNotEmptyString(DocKindValue, "ѕерехватить—обыти€ѕроведени€ƒокумента: передана пуста€ строка вместо вида документа");
	::ValidateParamUserClass(UserClassContValue, "ѕерехватить—обыти€ѕроведени€ƒокумента: не передан объект-перехватчик");

	CString strDocKind = DocKindValue.GetString();
	SetWrapper(strDocKind, static_cast<IUserClassContext*>(UserClassContValue.GetContext()));
}

void CDocTransactWrap::SetWrapper(const CString& strDocKind, IUserClassContext* pUserClassCont)
{
	CBLPtr<CDocContext> pDocCont("ƒокумент."+strDocKind);
	if(!pDocCont)
		RuntimeError("ѕерехватить—обыти€ѕроведени€ƒокумента: документ указанного вида <%s> не существует", strDocKind.operator LPCTSTR());

	// вызываю вручную, иначе модуль проведени€ дока не будет создан до вызова метода "ѕровести"
	pDocCont->InitTransacter();
	CDocTransacter* pDocTransacter = pDocCont->m_pDocTransacter;

	long DocTypeID = pMetaDataCont->GetDocDefs()->GetIndex(pDocCont->CurDocDef()->GetID());

	//CDocActModule* pDocModule = GetDocumentTransactionModule(pDocCont);
	CDocActModule* pDocModule = pDocTransacter->GetAt(DocTypeID);
	if (!pDocModule)
		RuntimeError("ѕерехватить—обыти€ѕроведени€ƒокумента: неудача при получении модул€ проведени€ документа");

	int iRes = pDocModule->TestIsPostingProc();

	CUserHandlersManager pNewHandler;
	pNewHandler.Add(0, pUserClassCont);

	SetWrapperFromDocID(DocTypeID, pNewHandler);
}

LPCTSTR CDocTransactWrap::GetDocKind(CTypeID DocTypeID)
{
	CMetaDataObj* pObj = pMetaDataCont->GetDocDefs()->GetItem(DocTypeID); //v.m_mdid);
	if (!pObj)
		return NULL;

	return pObj->m_Code;
}

void CDocTransactWrap::SetWrapperFromDocID(CTypeID DocTypeID, CUserHandlersManager pUserClassCont)
{
	m_bHaveSetHooks = true;
	m_ModuleMap.SetAt(DocTypeID, CUserHandlersManager(pUserClassCont));
}

bool CDocTransactWrap::Lookup(CTypeID DocTypeID, CUserHandlersManager& pCont) const
{
	if(!m_ModuleMap.Lookup(DocTypeID, pCont))
	{
		//LogErr("Ќеудача Lookup, не нашли дл€ CBLModule* %d", pMod);
		return false;
	}

	assert(true == pCont.isValid());
	return true;
}

int CDocTransactWrap::FindWrapMethod(CTypeID DocTypeID, CBLModule7* pThisMod, char const * sRusName, char const * sEngName, CUserHandlersManager& pCont)const
{
	if (!IsWrapEnable())
		return -1;

	if (!Lookup(DocTypeID, pCont))
		return -1;
	
	//LogErr("%s - ѕолучили перехват метода %s", "FindWrapMethod", sRusName);
	int iNum = pCont.FindMethodOfContext(sRusName, false); //FindMethodOfContext(pCont, sRusName);
	if (-1 == iNum)
		iNum = pCont.FindMethodOfContext(sEngName, false); //FindMethodOfContext(pCont, sEngName);
	//LogErr("%s - ѕолучили перехват метода %s, вернули номер %d", "FindWrapMethod", sRusName, iNum);

	return iNum;
}

class CStoreObj_My: public CRecAddr
{
	DWORD m_dwFlag1;
	DWORD m_dwFlag2;
	DWORD m_dwFlag3;
	DWORD m_dwFlag4;
	DWORD m_dwFlag5;
	DWORD m_dwFlag6;
	DWORD m_dwFlag7;
	//DWORD m_dwFlag8;
	//CDBSign m_DBSign;

	// artbear - по смещению 34h находитс€ объект CObjID - IDA bkend.idb 200F8CA9
	// artbear - по смещению 48h находитс€ объект CString - IDA bkend.idb 200F8CA9
	// artbear - по смещению 4Ch находитс€ объект CString - IDA bkend.idb 200F8CA9
};

int CDocTransactWrap::FindAndRunModuleEventHandler( CBLModule7* pThisMod, char* sRusName, char* sEngName, int iParamsCount, CValue * * params )
{
	CDocActModule* pDocActModule = static_cast<CDocActModule*>(pThisMod);

	CDocContext* pDocContext = pDocActModule->GetDocContext();
	long DocTypeID = pMetaDataCont->GetDocDefs()->GetIndex(pDocContext->CurDocDef()->GetID());

	CUserHandlersManager pCont;
	int iNum = FindWrapMethod(DocTypeID, pThisMod, sRusName, sEngName, pCont);

	if (-1 == iNum)
		return FALSE;

	pThisMod->SetStatusCode(1);

	CValue param1;
	//param1.AssignContext(pDocActModule->GetDocContext());
	param1.AssignContext(&pDocActModule->m_GroupContext);

	std::vector<CValue*> paramsVector;//(iParamsCount+1);
	paramsVector.push_back(&param1);
	for(int i=0;i<iParamsCount;i++)
		paramsVector.push_back(*params++);

	return RunModuleHandler(false, pThisMod, pCont, iNum, &paramsVector[0]);
}

CDocTransactWrap::CUserHandlersManager CDocTransactWrap::GetWrapper( const CString& strDocKind ) const
{
	CUserHandlersManager pCont;

	const long DocTypeID = pMetaDataCont->GetDocDefs()->GetIndex(strDocKind);
	if (-1 == DocTypeID)
		RuntimeError("ѕолучитьѕерехватчик—обытийѕроведени€ƒокумента: документ указанного вида <%s> не существует", strDocKind.operator LPCTSTR());
	
	Lookup(DocTypeID, pCont);
	return pCont;
}

void CDocTransactWrap::DestroyWrapper( const CString& strDocKind )
{
	const long DocTypeID = pMetaDataCont->GetDocDefs()->GetIndex(strDocKind);
	if (-1 == DocTypeID)
		RuntimeError("—н€тьѕерехват—обытийѕроведени€ƒокумента: документ указанного вида <%s> не существует", strDocKind.operator LPCTSTR());

	m_ModuleMap.RemoveKey(DocTypeID); // TODO проверить деструктор
}