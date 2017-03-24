// docrefboost.cpp
#include "stdafx.h"
#include "docrefboost.h"
#include "allboost.h"

CManyPropBoost* CDocBoost::m_props;
CManyPropBoost* CRefBoost::m_props;
CManyPropBoost* CMetadataBoost::m_props;
CManyPropBoost* CRegBoost::m_props;
int CRegBoost::m_offset;
CManyPropBoost* CEnumBoost::m_props;
int CEnumBoost::m_offset;
CManyPropBoost* CCalcBoost::m_props;
int CCalcBoost::m_offset;
DWORD CVTBoost::Vtable[64];
CVTBoost::PD CVTBoost::pRealDecr;

DWORD CQueryBoost::Vtable[64];
CQueryBoost::PD CQueryBoost::pRealDecr;


void CDocBoost::Init()
{
	CBLContext* pCont=CBLContext::CreateInstance("Документ");
	if(!pCont)
		return;
	new CAllBoost(pCont,false);
	m_props=new CManyPropBoost(TRUE, FALSE, pMetaDataCont->GetNDocDefs());
	m_props->InitAllProps(pCont);
	PF pFindFunc=&FindProp;
	CFastFindProp::SetHookOnProp(pCont,&pFindFunc);
	delete pCont;
}

void CRefBoost::Init()
{
	if (!GetMetaData()->GetNSTypeDefs())
		return;
	CBLContext* pCont = CBLContext::CreateInstance("Справочник");
	if (!pCont)
		return;
	new CAllBoost(pCont,false);
	m_props=new CManyPropBoost(TRUE, FALSE, pMetaDataCont->GetNSTypeDefs());
	m_props->InitAllProps(pCont);
	PF pFindFunc=&FindProp;
	CFastFindProp::SetHookOnProp(pCont,&pFindFunc);
	delete pCont;
}

void CMetadataBoost::Init(CBLContext* pCont)
{
	if(!pCont)
		return;
	CMetadataContext* mcont = (CMetadataContext*)pCont;
// 	new CAllBoost(pCont,false);
	m_props=new CManyPropBoost(TRUE, TRUE, 64);
	m_props->InitAllProps(pCont);
	PF pFindProp=&FindProp;
	CFastFindProp::SetHookOnProp(pCont,&pFindProp);
	m_props->InitAllMeths(pCont);
	PF pFindMeth=&FindMeth;
	CFastFindProp::SetHookOnMeth(pCont,&pFindMeth);
}

void CRegBoost::Init()
{
	int count=pMetaDataCont->GetNRegDefs();
	if(!count)
		return;
	CBLContext* pCont=CBLContext::CreateInstance("Регистры");
	if(!pCont)
		return;
	new CAllBoost(pCont,true);
	m_props=new CManyPropBoost(TRUE, FALSE, count);
	CValue val;
	pCont->GetPropVal(0,val);
	delete pCont;
	pCont=val.GetContext();
	CRegDef* pDef=pMetaDataCont->GetRegDefAt(0);
	int size=pCont->GetRuntimeClass()->m_nObjectSize;
	for(m_offset=1;m_offset<size;m_offset++)
	{
		if(*(CRegDef**)(((char*)pCont)+m_offset)==pDef)
			break;
	}
	if(m_offset==size)
	{
		ShowMsg("Не удалось ускорить регистры. Обратитесь к разработчику.");
		return;
	}
	new CAllBoost(pCont,false);
	PF pFindFunc=&FindProp;
	CFastFindProp::SetHookOnProp(pCont,&pFindFunc);
}

void CEnumBoost::Init(CBLContext *pCont)
{
	int count=pMetaDataCont->GetNEnumDefs();
	if(!count)
		return;
	new CAllBoost(pCont,true);
	CValue val;
	pCont->GetPropVal(0,val);
	CBLContext* pEnum=val.GetContext();
	CEnumDef* pDef=pMetaDataCont->GetEnumDefAt(0);
	int size=pEnum->GetRuntimeClass()->m_nObjectSize;
	for(m_offset=1;m_offset<size;m_offset++)
	{
		if(*(CEnumDef**)(((char*)pEnum)+m_offset)==pDef)
			break;
	}
	if(m_offset==size)
		return;
// 	{
		for(int i=0;i<count;i++)
		{
			pCont->GetPropVal(i,val);
			pEnum=val.GetContext();
			if(pEnum)
			{
				CFastFindProp* pProp=new CFastFindProp(pEnum, TRUE, FALSE);
				pProp->MakeFast(pEnum);
			}
		}
// 		return;
// 	}
// 	m_props=new CManyPropBoost(count);
// 	PF pFindFunc=&FindProp;
// 	CFastFindProp::SetHookOnProp(pCont,&pFindFunc);
}

void CCalcBoost::Init()
{
	int count=pMetaDataCont->GetNCJDefs();
	if(!count)
		return;
	CString name="ЖурналРасчетов.";
	CCJDef* pDef=pMetaDataCont->GetCJDefAt(0);
	name+=((CMetaDataObj*)pDef)->GetCode();
	
	CBLContext* pCont=CBLContext::CreateInstance(name);
	if(!pCont)
		return;
	new CAllBoost(pCont,false);
	int size=pCont->GetRuntimeClass()->m_nObjectSize;
	for(m_offset=4;m_offset<size;m_offset++)
	{
		if(*(CCJDef**)(((char*)pCont)+m_offset)==pDef)
			break;
	}
	if(m_offset==size)
		return;
	m_props=new CManyPropBoost(TRUE, FALSE, count);
	PF pFindFunc=&FindProp;
	CFastFindProp::SetHookOnProp(pCont,&pFindFunc);
	delete pCont;
}

void CVTBoost::Init()
{
	CBLContext* pVT=CBLContext::CreateInstance("ValueTable");
	new CAllBoost(pVT,false);
	memcpy(Vtable,*(DWORD**)pVT,148);
	DWORD old;
	VirtualProtect(Vtable, sizeof(Vtable), PAGE_EXECUTE_READWRITE, &old);
	pRealDecr=*(PD*)&Vtable[6];
	PF pFindFunc=&FindProp;
	Vtable[19]=*(DWORD*)&pFindFunc;
	PD pD=&DecrRef;
	Vtable[6]=*(DWORD*)&pD;
	pFindFunc=&FindPropFirst;
	CFastFindProp::SetHookOnProp(pVT,&pFindFunc);
}

void CQueryBoost::Init()
{
	CBLContext* pVT=CBLContext::CreateInstance("Query");
	new CAllBoost(pVT,false);
	memcpy(Vtable,*(DWORD**)pVT,148);
	DWORD old;
	VirtualProtect(Vtable, sizeof(Vtable), PAGE_EXECUTE_READWRITE, &old);
	pRealDecr=*(PD*)&Vtable[6];
	PF pFindFunc=&FindProp;
	Vtable[19]=*(DWORD*)&pFindFunc;
	PD pD=&DecrRef;
	Vtable[6]=*(DWORD*)&pD;
	pFindFunc=&FindPropFirst;
	CFastFindProp::SetHookOnProp(pVT,&pFindFunc);
}

