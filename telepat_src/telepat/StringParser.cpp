// StringParser.cpp: implementation of the CStringParser class.
#include "stdafx.h"
#include "StringParser.h"
#include "telegroup.h"
#include "txttrap.h"
#include "telelist.h"
#include "option.h"
static CTeleGroup gr;

static const struct
{
	LPCTSTR names[2];
	int num;
}keynames[]=
{
	{{"Справочник","Reference"},2},
	{{"Документ","Document"},3},
	{{"Журнал","Journal"},4},
	{{"Регистр","Register"},5},
	{{"Отчет","Report"},6},
	{{"Обработка","CalcVar"},7},
	{{"ПланСчетов","AccountChart"},8},
	/*{{"Операция","Operation"},9},
	{{"Счет","Account"},10},
	{{"ЖурналОпераций",""},11},
	{{"ЖурналРасчетов",""},12},
	{{"ЖурналПроводок",""},13},
	{{"Константа","Const"},14},
	{{"",""},0},
	{{"",""},0},
	{{"",""},0},
	{{"",""},0},
	{{"",""},0},
	*/
};
LPCTSTR qnames[][2]={
	{"Период","Period"},
	{"Обрабатывать","Process"},
	{"ОбрабатыватьОперации","ProcessOperations"},
	{"ОбрабатыватьДокументы","ProcessDocuments"},
	{"Функция","Function"},
	{"Группировка","Group"},
	{"Условие","Condition"},
	{"Сумма","Sum"},
	{"Среднее","Avg"},
	{"Минимум","Min"},
	{"Максимум","Max"},
	{"НачОст","BegRest"},
	{"КонОст","EndRest"},
	{"Приход","Debit"},
	{"Расход","Credit"},
	{"Счётчик","Counter"},
	{"СНД","DB"},
	{"СКД","FDB"},
	{"СНК","ICD"},
	{"СКК","FCD"},
	{"ДО","TD"},
	{"КО","TC"},
	{"КорДо","CorTD"},
	{"КорКо","CorTC"},
	{"Когда","When"},
	{"Все","All"},
	{"ПомеченныеНаУдаление","MarkedOnRemoving"},
	{"НеПомеченныеНаУдаление","NonMarkedOnRemoving"},
	{"Непроведенные","NonTransacted"},
	{"Проведенные","Transacted"},
	{"Включенные","On"},
	{"Выключенные","Off"},
	{"Упорядочить","Order"},
	{"Без","Without"},
	{"Итогов","Totals"},
	{"ВошедшиеВЗапрос","IncludedInQuery"},
	{"День","Day"},
	{"Неделя","Week"},
	{"Месяц","Month"},
	{"Квартал","Quarter"},
	{"Год","Year"},
	{"СтрокаДокумента","DocumentLine"},
	{"Групп","Groups"},
};

static CStringArray strUserWords;

void CStringParser::Init()
{
	for(int i=0;i<sizeof(keynames)/sizeof(keynames[0]);i++)
	{
		m_map1.InsertKey(keynames[i].names[0],(void*)keynames[i].num);
		m_map1.InsertKey(keynames[i].names[1],(void*)keynames[i].num);
	}
	ReadWords();
	gr.m_Image=IDI_STRINGS_;
}

CStringParser::CStringParser()
{
}

CStringParser::~CStringParser()
{
}

CBLMap CStringParser::m_map1;
static int Nothing(int, CString,CMetaDataObj*&)
{
	return 0;
}

static int Check0(int, CString txt,CMetaDataObj*&)
{
	return (int)CStringParser::m_map1.GetKey(txt);
}

static int CheckRefs(int,CString txt,CMetaDataObj*& pObj)
{
	pObj=pMetaDataCont->GetSTypeDef(txt);
	return pObj? 15:0;
}

static int CheckRefsAtr(int,CString txt,CMetaDataObj*& pObj)
{
	if(!pObj)
		return 0;
	CMetaDataObj* pNewObj=(CMetaDataObj*)((CSbCntTypeDef*)pObj)->GetParamDef(txt);
	if(pNewObj)
	{
		if(pNewObj->IsTypedObj())
			pNewObj=pMetaDataCont->FindObject(((CMetaDataTypedObj*)pNewObj)->GetType().GetTypeID());
		else
			pNewObj=NULL;
	}
	else
	{
		if(!txt.CompareNoCase("Родитель"))
			pNewObj=pObj;
		else if(!txt.CompareNoCase("Владелец"))
			pNewObj=pMetaDataCont->FindObject(((CSbCntTypeDef*)pObj)->GetParentID());
	}
	pObj=pNewObj;
	if(pObj)
	{
		int t=pObj->WhatIsIt();
		if(t==3)
			return 15;
		else if(t==9)
			return 16;
	}
	return 0;
}

static void FillRefs(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetSTypeDefs());
}

static void FillRefsAtr(CStringParser* parser,CMetaDataObj* pObj)
{
	CSbCntTypeDef* pRef=(CSbCntTypeDef*)pObj;
	if(pRef)
	{
		parser->FillFromMetaArray(pRef->GetParams());
		parser->FillFromMetaArray((CMetaDataObjArray*)pRef->GetFormDefsArray());
	}
	if(!pRef || pRef->GetDescLen())
	{
		parser->AddKey("Наименование");
		parser->AddKey("descr");
	}
	if(!pRef || pRef->GetCodeLen())
	{
		parser->AddKey("Код");
		parser->AddKey("code");
	}
	if(!pRef || pRef->GetLevelsLimit()>1)
	{
		parser->AddKey("Родитель");
		parser->AddKey("parentid");
		parser->AddKey("isfolder");
	}
	if(!pRef || pRef->GetParentID())
	{
		parser->AddKey("Владелец");
		parser->AddKey("parentext");
	}
	parser->AddKey("ТекущийЭлемент");
	parser->AddKey("id");
	parser->AddKey("ismark");
}

static int CheckDocs(int,CString txt,CMetaDataObj*& pObj)
{
	pObj=pMetaDataCont->GetDocDef(txt);
	return pObj?16:0;
}

static int CheckDocsAtr(int,CString txt,CMetaDataObj*& pObj)
{
	CMetaDataObj* pNewObj=NULL;
	if(pObj)
	{
		pNewObj=(CMetaDataObj*)((CDocDef*)pObj)->GetHeadFldDef(txt);
		if(!pNewObj)
			pNewObj=(CMetaDataObj*)((CDocDef*)pObj)->GetTblFldDef(txt);
	}
	if(!pNewObj)
	{
		CMetaDataObjArray* pGen=(CMetaDataObjArray*)pMetaDataCont->GetGenJrnlFlds();
		pNewObj=pGen->GetItem(txt);
	}
	if(pNewObj)
	{
		if(pNewObj->IsTypedObj())
			pNewObj=pMetaDataCont->FindObject(((CMetaDataTypedObj*)pNewObj)->GetType().GetTypeID());
		else
			pNewObj=NULL;
	}
	pObj=pNewObj;
	if(pObj)
	{
		int t=pObj->WhatIsIt();
		if(t==3)
			return 15;
		else if(t==9)
			return 16;
	}
	return 0;
}

static void FillDocs(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetDocDefs());
}

static void FillDocsAtr(CStringParser* parser,CMetaDataObj* pObj)
{
	if(pObj)
	{
		parser->FillFromMetaArray(((CDocDef*)pObj)->GetHeads());
		parser->FillFromMetaArray(((CDocDef*)pObj)->GetTables());
	}
	parser->FillFromMetaArray((CMetaDataObjArray*)pMetaDataCont->GetGenJrnlFlds());
	parser->AddKey("ТекущийДокумент");
	parser->AddKey("ДатаДок");
	parser->AddKey("НомерДок");
	parser->AddKey("ВремяДок");
	parser->AddKey("iddoc");
}

static int CheckJournal(int,CString txt,CMetaDataObj*& pObj)
{
	pObj=pMetaDataCont->GetJournalDef(txt);
	return pObj?17:0;
}

static void FillJournals(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetJournalDefs());
}

static void FillJournalsForms(CStringParser* parser,CMetaDataObj* pObj)
{
	if(pObj)
		parser->FillFromMetaArray((CMetaDataObjArray*)((CJournalDef*)pObj)->GetFormDefsArray());
}

static int CheckRegister(int,CString txt,CMetaDataObj*& pObj)
{
	pObj=(CMetaDataObj*)pMetaDataCont->GetRegDef(txt);
	return pObj?18:0;
}
static int CheckRegisterAtr(int,CString txt,CMetaDataObj*& pObj)
{
	CMetaDataObj* pNewObj=(CMetaDataObj*)((CRegDef*)pObj)->GetPropDef(txt);
	if(!pNewObj)
		pNewObj=(CMetaDataObj*)((CRegDef*)pObj)->GetFilds()->GetItem(txt);
	if(pNewObj)
	{
		if(pNewObj->IsTypedObj())
		{
			const CType& type=((CMetaDataTypedObj*)pNewObj)->GetType();
			long id=type.GetTypeID();
			if(id)
				pNewObj=pMetaDataCont->FindObject(id);
			else
			{
				pObj=NULL;
				if((id=type.GetTypeCode())==12)
					return 16;
				else if(id==11)
					return 15;
			}
		}
		else
			pNewObj=NULL;
	}
	else
	{
		if(!txt.CompareNoCase("ТекущийДокумент"))
		{
			pObj=NULL;
			return 3;
		}
	}
	pObj=pNewObj;
	if(pObj)
	{
		int t=pObj->WhatIsIt();
		if(t==3)
			return 15;
		else if(t==9)
			return 16;
	}
	return 0;
}


static void FillRegs(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetRegDefs());
}

static void FillRegsAtr(CStringParser* parser,CMetaDataObj* pObj)
{
	if(pObj)
	{
		parser->FillFromMetaArray(((CRegDef*)pObj)->GetFigure());
		parser->FillFromMetaArray(((CRegDef*)pObj)->GetFilds());
		parser->FillFromMetaArray(((CRegDef*)pObj)->GetProps());
		parser->AddKey("ТекущийДокумент");
		parser->AddKey("НомерСтроки");
		parser->AddKey("iddoc");
		parser->AddKey("date_time_iddoc");
		parser->AddKey("period");
		parser->AddKey("actno_");
		parser->AddKey("lineno_");
		parser->AddKey("iddocdef");
		parser->AddKey("debkred");
	}
	else
	{
		int c=pMetaDataCont->GetNRegDefs();
		for(int i=0;i<c;i++)
			FillRegsAtr(parser,(CMetaDataObj*)pMetaDataCont->GetRegDefAt(i));
	}
}

static int CheckReportCV(int,CString txt,CMetaDataObj*& pObj)
{
	return 0;
}
static void FillReports(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetReportDefs());
}

static void FillCalcVars(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetCalcVarDefs());
}

static void FillAccChart(CStringParser* parser,CMetaDataObj*)
{
	parser->FillFromMetaArray(pMetaDataCont->GetBuhDef()->GetPlanDefs());
}

static int CheckAccChart(int,CString txt,CMetaDataObj*& pObj)
{
	pObj=(CMetaDataObj*)pMetaDataCont->GetBuhDef()->GetPlanDef(txt);
	return pObj?19:0;
}

static void FillAccChartForms(CStringParser* parser,CMetaDataObj* pObj)
{
	if(pObj)
		parser->FillFromMetaArray((CMetaDataObjArray*)pMetaDataCont->GetBuhDef()->GetAccFormDefsArray());
}

static void FillKeyWords(CStringParser* parser,CMetaDataObj*)
{
	int lang=COption::m_pOption->GetLanguage();
	for(int i=0;i<sizeof(keynames)/sizeof(keynames[0]);i++)
	{
		if(lang & 1)
			parser->AddKey(keynames[i].names[1]);
		if(lang & 2)
			parser->AddKey(keynames[i].names[0]);
	}
	for(i=0;i<sizeof(qnames)/sizeof(qnames[0]);i++)
	{
		if(lang & 2)
			parser->AddKey(qnames[i][0]);
		if(lang & 1)
			parser->AddKey(qnames[i][1]);
	}
	for(int s = strUserWords.GetSize(), k=0; k<s ;k++)
		parser->AddKey(strUserWords[k]);
}

class CStringEntry : public CTeleEntry
{
public:
	CStringEntry(CString insert) : CTeleEntry(GetDisplayName(insert)), m_insert(insert){}
	virtual void GetInsert(CString& buf)
	{
		if(m_insert.Find("\\n") > -1)
		{
			CTxtTrap* pEditor=CTeleList::m_pOneList->m_pEditor;
			CTextDocument* pDoc=pEditor->GetDocument();
			CPoint ptStart=CTeleList::m_pOneList->m_CaretPoint;
			CString line;
			pDoc->GetLine(ptStart.y, line);
			for(int i = 0; i < ptStart.x; i++)
			{
				if(line[i]>' ')
				{
					line.GetBufferSetLength(i);
					break;
				}
			}
			line.Insert(0, "\r\n");
			line+='|';
			m_insert.Replace("\\n", line);
		}
		buf=m_insert;
	}
protected:
	static CString GetDisplayName(const CString& str)
	{
		CString ret(str);
		ret.Replace("\\n", "•");
		ret.Replace("\t", "»");
		return ret;
	}
	CString m_insert;
};

void CStringParser::ParseString(CString str,CTxtTrap* pEditor)
{
	CStringArray arr;
	int count=0;
	LPCTSTR ptr=str,start=ptr;
	while(1)
	{
		if(*ptr=='.' || !*ptr)
		{
			arr.Add(CString(start,ptr-start));
			count++;
			if(!*ptr)
				break;
			start=ptr+1;
		}
		ptr++;
	}
	m_result.RemoveAll();
	m_strMap.RemoveAll();

	static int (*checkfuncs[])(int,CString,CMetaDataObj*&)=
	{
		Nothing,Check0,CheckRefs,CheckDocs,CheckJournal,CheckRegister,
		CheckReportCV,CheckReportCV,CheckAccChart,Nothing,Nothing,Nothing,
		Nothing,Nothing,Nothing,CheckRefsAtr,CheckDocsAtr,Nothing,CheckRegisterAtr,
	};
	static void (*fillfuncs[])(CStringParser*,CMetaDataObj*)=
	{
		FillKeyWords,		// 0x1
		FillRefs,			// 0x2
		FillDocs,			// 0x4
		FillJournals,		// 0x8
		FillRegs,			// 0x10
		FillReports,		// 0x20
		FillCalcVars,		// 0x40
		FillAccChart,		// 0x80
		FillRefsAtr,		// 0x100
		FillDocsAtr,		// 0x200
		FillJournalsForms,	// 0x400
		FillRegsAtr,		// 0x800
		FillAccChartForms,	// 0x1000
	};

	static DWORD fills[]=
	{
		0,		0xA9F,	0x2,		0x4,		0x8,
		0x10,	0x20,	0x40,		0x80,		0,
		0,		0,		0,			0,			0,
		0x100,	0x200,	0x400,		0x800,		0x1000,

	};
	int state=1;
	CMetaDataObj* pObj=NULL;
	for(int i=0;i<count-1;i++)
	{
		CString word = arr[i];
		if(word[0]=='$')
			word.Delete(0);
		state=(*checkfuncs[state])(state, word, pObj);
	}

	for(i=0;i<sizeof(fillfuncs)/sizeof(fillfuncs[0]);i++)
	{
		if(fills[state] & (1<<i))
			(*fillfuncs[i])(this,pObj);
	}
	CString filter=arr[count-1];
	if(!filter.CompareNoCase("ыуд"))
		filter = "sel";
	else if(!filter.CompareNoCase("акщ"))
		filter = "fro";
	else if(!filter.CompareNoCase("црук"))
		filter = "wher";
	
	if((count=m_result.GetSize()))
	{
		gr.ClearEntries();
		CTeleList::m_pOneList->ResetContent();
		for(i=0;i<count;i++)
		{
			CTeleEntry* pEntry=new CStringEntry(m_result[i]);
			gr.AddEntry(pEntry);
		}
		CTeleList::m_pOneList->AddTGroup(&gr);
		CTeleList::m_pOneList->SetBufer(filter);
		CTeleList::m_pOneList->FilterList(filter);
		if(CTeleList::m_pOneList->GetCount())
		{
			CTeleList::m_pOneList->Prepare(pEditor);
			CTeleList::m_pOneList->ShowList();
		}
	}
}

void CStringParser::AddKey(LPCTSTR key)
{
	if(!m_strMap.GetKey(key))
	{
		m_strMap.InsertKey(key,(void*)1);
		m_result.Add(key);
	}
}

void CStringParser::FillFromMetaArray(CMetaDataObjArray *arr)
{
	if(!arr)
		return;
	int cnt=arr->GetNItems();
	for(int i=0;i<cnt;i++)
	{
		CMetaDataObj* pObj=arr->GetAt(i);
		if(pObj)
			AddKey(pObj->GetCode());
	}
}

void CStringParser::ReadWords()
{
	try
	{
		CString name = pMainApp->GetProps()->GetStringProp(0) + "config\\telepat.words.txt";
		CFile file(name, CFile::modeRead | CFile::shareDenyWrite);
		DWORD size = file.GetLength();
		if(size)
		{
			strUserWords.RemoveAll();
			file.Read(name.GetBufferSetLength(size), size);
			LPCSTR ptr = name;
			for(;;)
			{
				LPCSTR pStart = ptr;
				while(*ptr && *ptr !='\r')
					ptr++;
				if(ptr > pStart)
					strUserWords.Add(CString(pStart, ptr - pStart));
				if(*ptr=='\r')
					ptr++;
				if(!*ptr)
					break;
				ptr++;
			}
		}
	}
	catch(CFileException* e)
	{
		e->Delete();
	}
}