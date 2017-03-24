//////////////////////////////////////////////////////////////////////////
// Extended support for object serialization
// ( «начение¬—троку¬нутр() / «начение»з—троки¬нутр() )
// Copyright (c) kms, 2008-2012
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7SerializeToString.h"
#include "System/compare.h"

//////////////////////////////////////////////////////////////////////////
// CSerializeToString class
//////////////////////////////////////////////////////////////////////////

CTrapSwap CSerializeToString::m_trFastSaveToString;
CTrapSwap CSerializeToString::m_trLoadValueFromList;
CSerializeToString::S_MAP CSerializeToString::m_prefix_map;

void CSerializeToString::Setup()
{
	if (m_prefix_map.IsEmpty())
	{
		HMODULE hm = GetModuleHandle("bkend.dll");
		m_trFastSaveToString.SetTrapByName(hm, "?FastSaveToString@CValue@@QAEHAAVCString@@@Z", &CSerializeToString::FastSaveToString);
		m_trLoadValueFromList.SetTrapByName(hm, "?LoadValueFromList@CValue@@QAEHPAVCItemList@@H@Z", &CSerializeToString::LoadValueFromList);
	}
}

void CSerializeToString::Add(LPCSTR szPrefix, LPCSTR szClassName, LPCSTR szInstanceName)
{
	s_record const* pRec;
	if (m_prefix_map.Lookup(szPrefix, pRec))
	{
		if (FastCompare(szClassName, pRec->m_csClassName) || FastCompare(szInstanceName, pRec->m_csInstanceName))
		{
			CString csErr;
			csErr.Format("ѕрефикс '%s' уже использован дл€ сериализации класса '%s'/'%s'",
				(LPCSTR)pRec->m_csPrefix, (LPCSTR)pRec->m_csInstanceName, (LPCSTR)pRec->m_csClassName);

			throw s_exception((LPCSTR)csErr);
		}
	}

	Setup();
	pRec = new s_record(szPrefix, szClassName, szInstanceName);
	m_prefix_map.SetAt(szPrefix, pRec);
}

BOOL CSerializeToString::FastSaveToString(CString& csTarget)
{
	CValue* pVal = reinterpret_cast<CValue*>(this);
	CBLContext* pCont = pVal->GetContext();

// 	Msg("FastSaveToString %s", pCont ? pCont->GetRuntimeClass()->m_lpszClassName : (LPCSTR)CString(pVal->GetCTypeCode()));

	if (pCont && pCont->IsSerializable())
		return pCont->SaveToString(csTarget);

	trapswapper ts(m_trFastSaveToString);
	return pVal->FastSaveToString(csTarget);
}

BOOL CSerializeToString::LoadValueFromList(CItemList* pList, BOOL bValidate)
{
// 	Msg("LoadValueFromList");

	CValue* pVal = reinterpret_cast<CValue*>(this);
	CGenericItem const* pGenericItem = pList->GetItemAt(0);
	if (pGenericItem->WhatIsIt() == CGenericItem::e_Item)
	{
		CItem const* pItem = static_cast<CItem const*>(pGenericItem);
		s_record const* pRec;
// 		Msg("functor %s", pItem->GetFunctor());
		if (m_prefix_map.Lookup(pItem->GetFunctor(), pRec))
		{
			BOOL bSuccess = FALSE;
			CBLContext* pCont = pVal->GetContext();
			if (!pCont || FastCompare(pCont->GetRuntimeClass()->m_lpszClassName, pRec->m_csClassName))
			{
				{ // спец. блок дл€ сторожа
					CComponentClass::GuardOfClearFlagCreateWithoutConstructor guard(true);

					pCont = CBLContext::CreateInstance(pRec->m_csInstanceName);
				}

				pVal->AssignContext(pCont);
 				if (pCont)
 					pCont->DecrRef();
			}
			if (pCont)
			{
				IV7SerializeToString* pSerialize = dynamic_cast<IV7SerializeToString*>(pCont);
				pItem = static_cast<CItem*>(pList->GetItemAt(1));
				bSuccess = pSerialize->LoadFromString(pItem->GetFunctor());
			}
			if (!bSuccess)
				pVal->Reset();

			return bSuccess;
		}
	}

	trapswapper ts(m_trLoadValueFromList);
	return pVal->LoadValueFromList(pList, bValidate);
};
