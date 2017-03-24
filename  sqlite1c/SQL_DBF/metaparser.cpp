//metaparser.cpp
#include "StdAfx.h"
#include "metaparser.h"
#include "valuework.hpp"

struct parsingData
{
	parsingData(CString& str) : text(str)
	{
		// Переданная строка может быть расшарена, но так как будет модифицироваться
		// буфер строки, то форсируем создание своей копии строки
		pWrite = str.GetBufferSetLength(str.GetLength());
		pStartAll = pWrite;
		pRead = pStartAll;
		pStartOfRead = pStartAll;
		extraLen = 0;
		modCount = 0;
	}

	void insertSubst(const CString& param)
	{
		// Для начала если есть дыра в строке, сдвинем блок в нее.
		if(pStartOfRead > pWrite)
		{
			DWORD lenOfBlock = pStartOfParam - pStartOfRead;
			memmove(pWrite, pStartOfRead, lenOfBlock);
			pWrite += lenOfBlock;
		}
		else
			pWrite = (LPSTR)pStartOfParam;

		// Теперь посмотрим, есть ли в строке дыра размером со вставляемый параметр
		DWORD freeSpace = pRead - pWrite, paramLen = param.GetLength();
		if(paramLen <= freeSpace)
		{
			memcpy(pWrite, (LPCSTR)param, paramLen);
			pWrite += paramLen;
		}
		else
		{
			extraLen += paramLen;
			subsStrings.Add(param);
			subsOffsets.Add(pWrite - pStartAll);
		}
		pStartOfRead = pRead;
	}
	void makeInserts()
	{
		if(pStartOfRead > pWrite)
		{
			memmove(pWrite, pStartOfRead, pRead - pStartOfRead);
			pRead -= pStartOfRead - pWrite;
		}
		
		int newLen = pRead - pStartAll + extraLen;
		if(newLen != text.GetLength())
			text.GetBufferSetLength(newLen);
		
		if(extraLen)
		{
			LPSTR pNewWrite = (LPSTR)(LPCSTR)text;
			LPSTR pBlockEnd = pNewWrite + (pRead - pStartAll);
			DWORD el = extraLen;
			for(DWORD idx = subsStrings.GetSize(); idx--; )
			{
				LPSTR pStartOfBlock = pNewWrite + subsOffsets[idx];
				const CString& param = subsStrings[idx];
				memmove(pStartOfBlock + el, pStartOfBlock, pBlockEnd - pStartOfBlock);
				DWORD len = param.GetLength();
				el -= len;
				memcpy(pStartOfBlock + el, (LPCSTR)param, len);
				pBlockEnd = pStartOfBlock;
			}
		}
	}
	CString& text;
	LPCSTR pStartAll, pStartOfParam, pEndOfParam, pRead, pStartOfRead;
	LPSTR pWrite;
	CStringArray subsStrings;
	CDWordArray subsOffsets;
	DWORD extraLen;
	int modCount;

};

struct bindTextParam
{
	const CString& paramName;
	CString result;
	MetaParser& mp;

	bindTextParam(const CString& pn, MetaParser& _mp) : paramName(pn), mp(_mp){}

	void error(LPCSTR text)
	{
		CString err;
		err.Format("Ошибка подстановки текстового параметра %s - %s", (LPCSTR)paramName, text);
		CBLModule::RaiseExtRuntimeError(err, FALSE);
	}

	void bindText(const CString& text)
	{
		CString tmp(text);
		tmp.Replace("'", "''");
		DWORD len = tmp.GetLength();
		LPSTR ptr = result.GetBufferSetLength(len + 2);
		*ptr = '\'';
		memcpy(ptr + 1, (LPCSTR)tmp, len);
		ptr[len + 1] = '\'';
	}
	
	void bindNumeric(const CValue* pValue)
	{
		result = pValue->GetString();
	}
	
	void bindNull()
	{
		result = "NULL";
	}

	void bindFragment(const CString& str)
	{
		result = str;
		mp.processSql(result);
	}

	void bindMDID36(CMetaDataObj* pObj, int mod)
	{
		if(!pObj)
			error("Объект не найден");
		switch(mod)
		{
		case 0:
			{
				BYTE* ptr = (BYTE*)result.GetBufferSetLength(6);
				ptr[0] = '\'';
				*(DWORD*)(ptr + 1) = '    ';
				id2str(pObj->m_ID, ptr + 1, 4);
				ptr[5] = '\'';
			}
			break;
		case 1:
			result.Format("%i", pObj->m_ID);
			break;
		default:
			error("Неверный модификатор");
		}
	}
};

CMetaDataObj* findConstant(const CString& name)		{return pMetaDataCont->GetConstDef(name);}
CMetaDataObj* findReference(const CString& name)	{return pMetaDataCont->GetSTypeDef(name);}
CMetaDataObj* findDocument(const CString& name)		{return pMetaDataCont->GetDocDef(name);}
CMetaDataObj* findEnum(const CString& name)			{return pMetaDataCont->GetEnumDef(name);}
CMetaDataObj* findJournal(const CString& name)		{return pMetaDataCont->GetJournalDef(name);}
CMetaDataObj* findSelCol(const CString& name)		{return pMetaDataCont->GenDocSelRefs()->GetItem(name);}
CMetaDataObj* findCalendar(const CString& name)		{return pMetaDataCont->GetCalendarTypeDef(name);}
CMetaDataObj* findCalcJournal(const CString& name)	{return pMetaDataCont->GetCJDef(name);}
CMetaDataObj* findAlgorithm(const CString& name)	{return pMetaDataCont->GetAlgorithmDef(name);}
CMetaDataObj* findSbcntKind(const CString& name)
{
	CBuhDef* pBuh = pMetaDataCont->GetBuhDef();
	if(pBuh)
		return pBuh->GetSbKindDefs()->GetItem(name);
	return NULL;
}
CMetaDataObj* findChartOfAccs(const CString& name)
{
	CBuhDef* pBuh = pMetaDataCont->GetBuhDef();
	if(pBuh)
		return pBuh->GetPlanDef(name);
	return NULL;
}

template<typename Op>
void bindMetaNameObj(bindTextParam& binder, int mod, CStringArray& parts, Op op)
{
	if(parts.GetSize() == 2)
		binder.bindMDID36(op(parts[1]), mod);
}


inline static void bindHistoryProp(bindTextParam& binder, int mod, CStringArray& parts)
{
	if(parts.GetSize() == 3)
	{
		CSbCntTypeDef* pRef = pMetaDataCont->GetSTypeDef(parts[1]);
		if(pRef)
		{
			CSbCntParamDef* pProp = pRef->GetParamDef(parts[2]);
			if(pProp && pProp->IsDateDep())
				binder.bindMDID36(pProp, mod);
		}
	}
}

void isMetaName(bindTextParam& binder, int mod)
{
	static struct prefix_finder
	{
		enum{
			mnConst, mnRefKind, mnDocKind, mnEnumKind, mnPeriodic,
			mnJournal, mnSelCol, mnCalendar, mnHolidays, mnCalcJournal, mnAlgorithm,
			mnEmptyID, mnEmptyID13, mnEnum, mnSubcKind, mnChartOfAccs,
		};
		prefix_finder()
		{
			static struct {LPCSTR name1, name2; int idx;} prefixes[] = {
				{"Константа",			"Constant", mnConst},
				{"ВидСправочника",		"ReferenceKind", mnRefKind},
				{"ВидДокумента",		"DocumentKind", mnDocKind},
				{"ВидПеречисления",		"EnumKind", mnEnumKind},
				{"ИсторияРеквизита",	"PropertyHistory", mnPeriodic},
				{"ЖурналДокументов",	"DocsJournal", mnJournal},
				{"ГрафаОтбора",			"SelectionColumn", mnSelCol},
				{"Календарь",			"Calendar", mnCalendar},
				{"Праздники",			"Holidays", mnHolidays},
				{"ВидЖурналаРасчетов",	"CalcJournalKind", mnCalcJournal},
				{"ВидРасчета",			"Algorithm", mnAlgorithm},
				{"ПустойИД",			"EmptyID", mnEmptyID},
				{"ПустойИД13",			"EmptyID13", mnEmptyID13},
				{"Перечисление",		"Enumeration", mnEnum},
				{"ВидСубконто",			"SubcKind", mnSubcKind},
				{"ПланСчетов",			"ChartOfAccounts", mnChartOfAccs},
			};
			for(DWORD i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++)
			{
				prefMap[prefixes[i].name1] = prefixes[i].idx;
				prefMap[prefixes[i].name2] = prefixes[i].idx;
			}
		}
		int find(const CString& name)
		{
			int idx = -1;
			prefMap.Lookup(name, idx);
			return idx;
		}
		CNoCaseMap<int> prefMap;
	}finder;

	CStringArray parts;
	SplitStr2Array(binder.paramName, parts, '.');
	
	int countOfParts = parts.GetSize();
	if(!countOfParts)
		binder.error("Плохое имя параметра");
	
	switch(finder.find(parts[0]))
	{
	case prefix_finder::mnConst:
		bindMetaNameObj(binder, mod, parts, findConstant);
		break;
	case prefix_finder::mnRefKind:
		bindMetaNameObj(binder, mod, parts, findReference);
		break;
	case prefix_finder::mnDocKind:
		bindMetaNameObj(binder, mod, parts, findDocument);
		break;
	case prefix_finder::mnEnumKind:
		bindMetaNameObj(binder, mod, parts, findEnum);
		break;
	case prefix_finder::mnPeriodic:
		bindHistoryProp(binder, mod, parts);
		break;
	case prefix_finder::mnJournal:
		bindMetaNameObj(binder, mod, parts, findJournal);
		break;
	case prefix_finder::mnSelCol:
		bindMetaNameObj(binder, mod, parts, findSelCol);
		break;
	case prefix_finder::mnCalendar:
		bindMetaNameObj(binder, mod, parts, findCalendar);
		break;
	case prefix_finder::mnHolidays:
		if(countOfParts == 1)
			binder.bindMDID36(pMetaDataCont->GetHolidaysDef(), mod);
		break;
	case prefix_finder::mnCalcJournal:
		bindMetaNameObj(binder, mod, parts, findCalcJournal);
		break;
	case prefix_finder::mnAlgorithm:
		bindMetaNameObj(binder, mod, parts, findAlgorithm);
		break;
	case prefix_finder::mnEmptyID:
		if(countOfParts == 1)
			binder.result = "'     0   '";
		break;
	case prefix_finder::mnEmptyID13:
		if(countOfParts == 1)
			binder.result = "'   0     0   '";
		break;
	case prefix_finder::mnEnum:
		if(countOfParts == 3)
		{
			CEnumDef* pDef = pMetaDataCont->GetEnumDef(parts[1]);
			if(pDef)
			{
				CEnumValDef* pVal = pDef->GetValDef(parts[2]);
				if(pVal)
				{
					unsigned char* ptr = (unsigned char*)binder.result.GetBufferSetLength(11);
					*ptr = '\'';
					*(DWORD*)(ptr + 1) = '    ';
					*(DWORD*)(ptr + 5) = '    ';
					*(WORD*)(ptr + 9) = '\' ';
					id2str(pVal->m_ID, ptr + 1, 6);
				}
			}
		}
		break;
	case prefix_finder::mnSubcKind:
		bindMetaNameObj(binder, mod, parts, findSbcntKind);
		break;
	case prefix_finder::mnChartOfAccs:
		bindMetaNameObj(binder, mod, parts, findChartOfAccs);
		break;
	}
	if(binder.result.IsEmpty())
		binder.error("Неизвестный параметр");
}

void MetaParser::processParam(parsingData& pd)
{
	CString paramName(pd.pStartOfParam + 1, pd.pEndOfParam - pd.pStartOfParam - 1);
	
	bindTextParam bt(paramName, *this);
	
	if(textParams.IsExist(paramName))
		bindValue(bt, &textParams[paramName], pd.modCount);
	else
		isMetaName(bt, pd.modCount);
	pd.insertSubst(bt.result);
}

void MetaParser::processSql(CString& sql)
{
	enum States{
		stNone,
		stMinus,
		stLineRemark,
		stDiv,
		stMultiRemark,
		stMultiRemarkMult,
		stQuote,
		stBraket,
		stParam,
		stParamMod,
	};
	// Сканируем весь текст запроса, пропуская
	// комментарии --, /**/
	// литералы '' []
	// Для каждого найденного : вычисляем имя параметра и модификатор
	// Если для параметра есть текстовая подстановка, вычисляем строку
	// для подстановки, прибавляем к доп-размеру, запоминаем смещение.
	parsingData pd(sql);
	States state = stNone;
	
	for(;;)
	{
		DWORD s = (DWORD)(BYTE)*pd.pRead;
		
		switch(state)
		{
		case stNone:
			if('-' == s)
				state = stMinus;
			else if('/' == s)
				state = stDiv;
			else if('\'' == s)
				state = stQuote;
			else if('[' == s)
				state = stBraket;
			else if(':' == s)
			{
				pd.pStartOfParam = pd.pRead;
				state = stParam;
			}
			break;
		case stMinus:
			if('-' == s)
				state = stLineRemark;
			else if('/' == s)
				state = stDiv;
			else if('\'' == s)
				state = stQuote;
			else if('[' == s)
				state = stBraket;
			else if(':' == s)
			{
				pd.pStartOfParam = pd.pRead;
				state = stParam;
			}
			else
				state = stNone;
			break;
		case stLineRemark:
			if('\n' == s)
				state = stNone;
			break;
		case stDiv:
			if('*' == s)
				state = stMultiRemark;
			else if('\'' == s)
				state = stQuote;
			else if('[' == s)
				state = stBraket;
			else if(':' == s)
			{
				pd.pStartOfParam = pd.pRead;
				state = stParam;
			}
			else if('-' == s)
				state = stMinus;
			else if('/' != s)
				state = stNone;
			break;
		case stMultiRemark:
			if('*' == s)
				state = stMultiRemarkMult;
			break;
		case stMultiRemarkMult:
			if('/' == s)
				state = stNone;
			else if('*' != s)
				state = stMultiRemark;
			break;
		case stQuote:
			if('\'' == s)
				state = stNone;
			break;
		case stBraket:
			if(']' == s)
				state = stNone;
			break;
		case stParam:
			if('~' == s)
			{
				pd.pEndOfParam = pd.pRead;
				pd.modCount = 1;
				state = stParamMod;
			}
			else if('*' == s)
			{
				pd.pEndOfParam = pd.pRead;
				pd.modCount = -1;
				pd.pRead++;
				processParam(pd);
				pd.pRead--;
				state = stNone;
			}
			else if(! (s >= 129 || (s >= '0' && s <= '9') || s == '.' || s == '_' || (s >= 'A' && s <= 'Z') ||
				(s >= 'a' && s <= 'z')))
			{
				pd.pEndOfParam = pd.pRead;
				pd.modCount = 0;
				processParam(pd);
				state = stNone;
			}
			break;
		case stParamMod:
			if('~' == s)
				pd.modCount++;
			else
			{
				processParam(pd);
				state = stNone;
			}
			break;
		}
		if(!s)
			break;
		pd.pRead++;
	}
	pd.makeInserts();
}
