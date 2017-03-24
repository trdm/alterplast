// allboost.cpp
#include "stdafx.h"
#include "allboost.h"
#include "docrefboost.h"
#include "gcboost.h"

HANDLE hProcess=NULL;

static struct{
	const char *strName;
	bool BoostProp;
	int MaxNumProps;
}defEmbedded[]={
	{"CGeneralContext",true,99},
	{"CAllConstsContext",true,-1},

	{"CAccountContext",true,99},
	{"CAllSbKindsContext",true,1},
	{"CSbKindContext",false,-1},
	{"CAllPlanKindsContext",true,1},
	{"CPlanKindContext",false,-1},

	{"CCalcContext",true,99},
	{"CAllAlgorithmContext",true,1},
	{"CAlgorithmContext",true,-1},
	{"CAllGroupContext",true,1},
	{"CGrContext",true,-1},
	{"CAllRecalcRulesContext",true,1},
	{"CRecalcRuleContext",true,-1},
	{"CAllCalendarsContext",true,1},
	{"CLayerContext",true,-1},

	{"CBasicExtContext",true,99},
	{"CFSContext",false,-1},

	{NULL,false,false}
	};


static void BoostObject(const char* name,bool boostProp=false)
{
	CBLContext* pCont=CBLContext::CreateInstance(name);
	if(pCont)
	{
// 		DumpContext(pCont);
		new CAllBoost(pCont,boostProp);
		delete pCont;
	}
}

static void BoostRTC(CRuntimeClass* pRTC, bool boostProp=false)
{
	CBLContext* pCont=reinterpret_cast<CBLContext*>(pRTC->CreateObject());
	if(pCont)
	{
// 		DumpContext(pCont);
		new CAllBoost(pCont,boostProp);
		delete pCont;
	}
}

static void BoostContAndProps(CBLContext* pCont)
{
	if(!pCont)
		return;
	CString rtc=pCont->GetRuntimeClass()->m_lpszClassName;
	int i;
	for(i=0;defEmbedded[i].strName;i++)
	{
		if(rtc==defEmbedded[i].strName)
			break;
	}
	if(!defEmbedded[i].strName)
		return;
	new CAllBoost(pCont,defEmbedded[i].BoostProp);
	if(defEmbedded[i].MaxNumProps<0)
		return;
	int nProps=pCont->GetNProps();
	if(nProps>defEmbedded[i].MaxNumProps)
		nProps=defEmbedded[i].MaxNumProps;
	CValue val;
	CBLContext *pProp;
	for(i=0;i<nProps;i++)
	{
		pCont->GetPropVal(i,val);
		pProp=val.GetContext();
		if(!pProp)
		{
			val.LinkContext(TRUE);
			pProp=val.GetContext();
		}
		if(pProp)
		{
// 			CString name=pCont->GetPropName(i,1);
// 			Msg("boosted %s : %s - %s", (LPCSTR)rtc, pProp->GetRuntimeClass()->m_lpszClassName, LPCSTR(name));
			BoostContAndProps(pProp);
		}
	}
}

static void InitGeneralContext(CBLContext *pCont)
{
	BoostContAndProps(pCont);
	CValue val;
	int num=pCont->FindProp("Перечисление");
	if(num!=-1)
	{
		pCont->GetPropVal(num,val);
		CBLContext* pBoost=val.GetContext();
		if(pBoost)
			CEnumBoost::Init(pBoost);
	}

	num =pCont->FindProp("Метаданные");
	if (num != -1)
	{
		pCont->GetPropVal(num, val);
		CBLContext* pBoost = val.GetContext();
		if(pBoost)
 			CMetadataBoost::Init(pBoost);
	}
}

static void InitTradeContext(CBLContext *pCont)
{
	new CAllBoost(pCont,true);
	CRegBoost::Init();
}

static void InitAccContext(CBLContext *pCont)
{
	BoostContAndProps(pCont);
	BoostObject("Счет",true);
	BoostObject("Операция",true);
	BoostObject("БухгалтерскиеИтоги");
}

static void InitCalcContext(CBLContext *pCont)
{
	BoostContAndProps(pCont);
	BoostObject("Праздники",true);
	CCalcBoost::Init();
}

static void InitBasicExtContext(CBLContext *pCont)
{
	BoostContAndProps(pCont);
};

void InitAllBoost()
{
	hProcess=GetCurrentProcess();
	CGCBoost::Init();
	CDocBoost::Init();				// Документ
	CRefBoost::Init();				// Справочник
	CVTBoost::Init();
	CQueryBoost::Init();

	BoostObject("СписокЗначений");
	BoostObject("Текст");
	BoostObject("Картинка");
	BoostObject("XBase");
//  	BoostObject("ФС");				// перенесено в CBasicExtContext
	BoostObject("Таблица");
	BoostObject("Периодический");

	BoostRTC(RUNTIME_CLASS(CAccountContextDef), true);

	// Обработка загруженных компонент
	CString name;
	for(DWORD id=CBLContext::GetFirstLoadedContextID();id;id=CBLContext::GetNextLoadedContextID(id))
	{
		CBLContext* pCont=CBLContext::GetLoadedContext(id);
		name=pCont->GetRuntimeClass()->m_lpszClassName;
		if(name=="CGeneralContext")
			InitGeneralContext(pCont);
		else if(name=="CTradeContext")
			InitTradeContext(pCont);
		else if(name=="CAccountContext")
			InitAccContext(pCont);
		else if(name=="CCalcContext")
			InitCalcContext(pCont);
		else if(name=="CBasicExtContext")
			InitBasicExtContext(pCont);
	}
}
