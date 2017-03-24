//database.cpp
#include "StdAfx.h"
#include <io.h>
#include "database.h"
#include "utex.h"
#include "vtab_info.h"
#include "ValueWork.hpp"
#include "resultloader.h"
#include "../_1Common/ctxtree.h"

CString SQLiteBase::m_trace;

BL_INIT_CONTEXT(CSLDataBase);

extern "C" BOOL bNeedProfile = FALSE;
CString strProfile;
extern "C" void doProfileMsg(const char* msg)
{
	CString tmp(msg);
	tmp.Replace("\r\n", "\n");
	strProfile += tmp;
}

void baseStr2Id(sqlite3_context* pCtx, int nParam, sqlite3_value** ppArgs)
{
	CString text = (LPCSTR)sqlite3_value_text(ppArgs[0]);
	DWORD id = 0;
	const BYTE* ptr = (const BYTE*)(LPCSTR)text;
	for(;;)
	{
		DWORD s = (DWORD)*ptr;
		if(!s)
			break;
		id = id * 36 + (s >= 'A' ? s -'A' + 10 : s - '0');
		ptr++;
	}
	sqlite3_result_int(pCtx, id);
}

void baseId2Str(sqlite3_context* pCtx, int nParam, sqlite3_value** ppArgs)
{
	DWORD id = sqlite3_value_int(ppArgs[0]), len = sqlite3_value_int(ppArgs[1]);
	CString text;
	BYTE* ptr = (BYTE*)text.GetBufferSetLength(len);
	memset(ptr, ' ', len);
	id2str(id, ptr, len);
	sqlite3_result_text(pCtx, text, len, SQLITE_TRANSIENT);
}

// Функция, которая в отличие от coalesce, умеет возвращать не только
// первое не-нуль значение, но и любое заданное первым аргументом.
void coalesceEx(sqlite3_context* pCtx, int nParam, sqlite3_value** ppArgs)
{
	if(nParam > 1)
	{
		DWORD number = sqlite3_value_int(ppArgs[0]) - 1;
		if(number < nParam - 1)
		{
			for(int k = 1; k < nParam; k++)
			{
				if(SQLITE_NULL != sqlite3_value_type(ppArgs[k]))
				{
					if(!number)
					{
						sqlite3_result_value(pCtx, ppArgs[k]);
						return;
					}
					number--;
				}
			}
		}
	}
	sqlite3_result_null(pCtx);
}

struct bindSqlParam
{
	bindSqlParam(const CString& pn, int idxNum, sqlite3* b, sqlite3_stmt* s)
		:paramName(pn), nParamNum(idxNum), pBase(b), pStmt(s){}
	const CString& paramName;
	int nParamNum;
	sqlite3* pBase;
	sqlite3_stmt* pStmt;
	
	void error(LPCSTR text)
	{
		CString err;
		err.Format("Ошибка установки sql-параметра %s - %s", (LPCSTR)paramName, text);
		CBLModule::RaiseExtRuntimeError(err, FALSE);
	}
	void bindText(const CString& text)
	{
		CString str(text);
		u8text::toUtf8(str);
		checkErr(sqlite3_bind_text(pStmt, nParamNum, str, str.GetLength(), SQLITE_TRANSIENT));
	}
	
	void bindNumeric(const CValue* pValue)
	{
		if(pValue->m_Number.m_nScaleLen)
			checkErr(sqlite3_bind_double(pStmt, nParamNum, pValue->m_Number.GetDouble()));
		else
		{
			long val = pValue->m_Number;
			if(0 != pValue->m_Number.CompareLong(val))
				checkErr(sqlite3_bind_int64(pStmt, nParamNum, _atoi64(pValue->Format())));
			else
				checkErr(sqlite3_bind_int(pStmt, nParamNum, val));
		}
	}
	
	void bindNull()
	{
		checkErr(sqlite3_bind_null(pStmt, nParamNum));
	}
	void bindFragment(LPCSTR)
	{
		error("Неверный модификатор");
	}
	
	void checkErr(int res)
	{
		if(SQLITE_OK != res)
		{
			CString err(u8text::fromUtf8(sqlite3_errmsg(pBase)));
			error(err);
		}
	}
};

struct tran_guard
{
	int ac, err;
	sqlite3* _pDB;
	tran_guard(sqlite3* pDB) : _pDB(pDB), err(TRUE), ac(sqlite3_get_autocommit(pDB))
	{
		if(ac)
			sqlite3_exec(_pDB, "begin", NULL, NULL, NULL);
	}
	~tran_guard()
	{
		if(ac)
			sqlite3_exec(_pDB, err ? "rollback" : "commit", NULL, NULL, NULL);
	}
};

inline BOOL isIDequal(LPCSTR id1, LPCSTR id2)
{
	return *(ui64*)id1 == *(ui64*)id2 && (DWORD)(BYTE)id1[8] == (DWORD)(BYTE)id2[8];
}

typedef unsigned __int64 id8;

id8 id9to8(LPCSTR pID)
{
	return ((*(id8*)(pID + 6) & 0xFFFFFF) << 32) | str2id((const BYTE*)pID, 6);
}

id8 objIDto8(const CObjID& oid)
{
	return ((*(id8*)oid.DBSign.Sign) << 32) | oid.ObjID;
}

void id8to9(id8 id, LPSTR ptr)
{
	*(DWORD*)ptr = '    ';
	*(DWORD*)(ptr + 4) = '    ';
	id2str((DWORD)id, (BYTE*)ptr, 6);
	*(DWORD*)(ptr + 6) = (DWORD)(id >> 32);
}

struct ObjIDSet
{
	ObjIDSet()
	{
		memset(ppAssoc, 0, sizeof(ppAssoc));
		cnt = 0;
	}
	~ObjIDSet()
	{
		if(cnt)
		{
			DWORD e = cnt;
			assoc** ppAss = ppAssoc;
			for(;;)
			{
				while(!*ppAss)
					ppAss++;
				assoc* pAss = *ppAss++;
				while(pAss)
				{
					assoc* pDel = pAss;
					pAss = pAss->next;
					delete pDel;
					if(!--e)
						return;
				}
			}
		}
	}
	void insert(id8 id)
	{
		DWORD pos = id % hashSize;
		assoc* pAss = ppAssoc[pos];
		while(pAss)
		{
			if(pAss->id == id)
				return;
			pAss = pAss->next;
		}
		ppAssoc[pos] = new assoc(ppAssoc[pos], id);
		cnt++;
	}
	BOOL exist(id8 id)
	{
		for(assoc* pAss = ppAssoc[id % hashSize]; pAss ; pAss = pAss->next)
			if(pAss->id == id)
				return TRUE;
		return FALSE;
	}
	
	DWORD count() {return cnt;}
	
	id8 first()	// Не вызывать, если нет элементов!!!!
	{
		for(assoc** ppA = ppAssoc; !*ppA; ppA++);
		return (*ppA)->id;
	}
	void remove(id8 id)
	{
		assoc** ppAss = ppAssoc + id % hashSize;
		for(;;)
		{
			if((*ppAss)->id == id)
			{
				assoc* pDel = *ppAss;
				*ppAss = pDel->next;
				delete pDel;
				cnt--;
				return;
			}
			ppAss = &(*ppAss)->next;
		}
	}
	void doInsert(sqlite3_stmt* pInsert)
	{
		if(!cnt)
			return;
		
		CString text;
		DWORD e = cnt;
		assoc** ppAss = ppAssoc;
		
		for(;;)
		{
			while(!*ppAss)
				ppAss++;
			for(assoc* pAss = *ppAss++; pAss; pAss = pAss->next)
			{
				id8to9(pAss->id, text.GetBufferSetLength(9));
				u8text::toUtf8(text);
				sqlite3_bind_text(pInsert, 1, text, text.GetLength(), SQLITE_TRANSIENT);
				sqlite3_step(pInsert);
				sqlite3_reset(pInsert);
				if(!--e)
					return;
			}
		}
	}
protected:
	enum {hashSize = 117};

	struct assoc 
	{
		assoc(assoc* n, id8 i) : next(n), id(i){}
		id8 id;
		assoc* next;
	};
	assoc* ppAssoc[hashSize];
	DWORD cnt;
};

template<int nField>
struct keyObj : CKeyObj
{
	keyObj(CIndex* pI) : CKeyObj(pI, 0, 0){}
	char id[10];
	virtual void PrepareKey()
	{
		m_pStoreObj->FX_String(nField, id, 9, 1);
	}
};

inline void processRefValue(CValue* pValue, CStoreObj& store, keyObj<0>& key, ObjIDSet& elements, ObjIDSet& groups, LPSTR pRecIsFolder)
{
	id8 id = objIDto8(pValue->m_ObjID);
	if(0x20202000000000 == id)
	{
		groups.insert(id);
		return;
	}
	id8to9(id, key.id);

	if(store.Goto(&key, ccE, 0))
	{
		if((DWORD)(BYTE)*pRecIsFolder == '2')
			elements.insert(id);
		else
			groups.insert(id);
	}
}

void putvlReference(CSbCntTypeDef* pRefDef, CPtrArray* pVL, CValue* pVal, sqlite3_stmt* pInsert, BOOL bWithGroups)
{
	ObjIDSet elements;
	ObjIDSet groups;
	ObjIDSet processedGroups;

	CTableEx* pTable = (CTableEx*)pDataDict->GetTable(pRefDef->GetTableName());
	LPSTR pRecID = pTable->recordBuffer();
	LPSTR pRecPID = pRecID + 9;
	LPSTR pRecIsFolder = pRecID + 18;
	for(DWORD i = 2; ; i++)
	{
		CField* pField = pTable->field(i);
		if(*(DWORD*)pField->szName == 'OFSI')
			break;
		pRecIsFolder += pField->sizeCField;
	}
	// Ищем по идшнику
	{
		CIndex* pI = pTable->index(0);
		CStoreObj store(pTable, pI);
		keyObj<0> key(pI);
		if(pVL)
		{
			CValueItem** ppItems = (CValueItem**)pVL->GetData();
			for(DWORD c = pVL->GetSize(); c--; ppItems++)
				processRefValue(&(*ppItems)->m_value, store, key, elements, groups, pRecIsFolder);
		}
		else
			processRefValue(pVal, store, key, elements, groups, pRecIsFolder);
	}
	if(groups.count())
	{
		// Теперь будем разворачивать группы
		CIndex* pI = pTable->GetIndex(1);
		CStoreObj store(pTable, pI);
		keyObj<1> key(pI);
		while(groups.count())
		{
			id8 id = groups.first();
			processedGroups.insert(id);
			id8to9(id, key.id);
			if(store.Goto(&key, ccGE, 0))
			{
				while(isIDequal(key.id, pRecPID))
				{
					id8 eid = id9to8(pRecID);
					if((DWORD)(BYTE)*pRecIsFolder == '2')
						elements.insert(eid);
					else if(!processedGroups.exist(eid))
						groups.insert(eid);
					if(!store.Goto(navNext, 0))
						break;
				}
			}
			groups.remove(id);
		}
	}
	// И теперь запихаем все в таблицу
	elements.doInsert(pInsert);
	if(bWithGroups)
		processedGroups.doInsert(pInsert);
};

struct keyByPlanKode : CKeyObj 
{
	keyByPlanKode(CIndex* pI) : CKeyObj(pI, 0, 0), plan('    '){}
	DWORD plan;
	CString code;
	virtual void PrepareKey()
	{
		m_pStoreObj->FX_String(1, (char*)&plan, 4, 1);
		m_pStoreObj->FX_String(2, (char*)(LPCSTR)code, code.GetLength(), 1);
	}
};

inline void processAccValue(CValue* pValue, CStoreObj& store, keyObj<0>& key, ObjIDSet& elements,
		DWORD planID, DWORD codeLen, CMapStringToPtr& groups, LPSTR pRecIsFolder, LPSTR pRecCode, LPSTR pRecPlanID)
{
	id8 id = objIDto8(pValue->m_ObjID);
	if(0x20202000000000 == id)
	{
		groups[""] = 0;
		return;
	}
	id8to9(id, key.id);

	if(store.Goto(&key, ccE, 0))
	{
		if(*(DWORD*)pRecPlanID == planID)
		{
			if((DWORD)(BYTE)*pRecIsFolder == '0')
				elements.insert(id);
			else
			{
				CString code(pRecCode, codeLen);
				code.TrimRight();
				groups[code] = 0;
			}
		}
	}
}

void putvlAccounts(CPlanDef* pPlanDef, CPtrArray* pVL, CValue* pVal, sqlite3_stmt* pInsert)
{
	ObjIDSet elements;
	CMapStringToPtr processedGroups;
	CMapStringToPtr groups;

	CTableEx* pTable = (CTableEx*)pDataDict->GetTable("1SACCS");
	LPSTR pRecID = pTable->recordBuffer();
	LPSTR pRecPID = pRecID + 9;
	LPSTR pRecCode = pRecPID + 4;
	int codeLen = pTable->field(2)->sizeCField;
	LPSTR pRecIsFolder = pRecCode + codeLen + pTable->field(3)->sizeCField + 3;
	
	CIndex* pIdxCode = pTable->GetIndex(1);
	keyByPlanKode keyCode(pIdxCode);
	id2str(pPlanDef->m_ID, (BYTE*)&keyCode.plan, 4);
	

	// Ищем по идшнику
	{
		CIndex* pI = pTable->index(0);
		CStoreObj store(pTable, pI);
		keyObj<0> key(pI);
		if(pVL)
		{
			CValueItem** ppItems = (CValueItem**)pVL->GetData();
			for(DWORD c = pVL->GetSize(); c--; ppItems++)
				processAccValue(&(*ppItems)->m_value, store, key, elements, keyCode.plan, codeLen, groups, pRecIsFolder, pRecCode, pRecPID);
		}
		else
			processAccValue(pVal, store, key, elements, keyCode.plan, codeLen, groups, pRecIsFolder, pRecCode, pRecPID);
	}
	// Теперь будем разворачивать группы
	CStoreObj store(pTable, pIdxCode);
	
	for(POSITION pos = groups.GetStartPosition(); pos;)
	{
		void* pv;
		groups.GetNextAssoc(pos, keyCode.code, pv);
		if(!processedGroups.Lookup(keyCode.code, pv))
		{
			processedGroups[keyCode.code] = 0;
			if(store.Goto(&keyCode, ccG, 0))
			{
				DWORD lenOfCode = keyCode.code.GetLength();
				for(;;)
				{
					if(*(DWORD*)pRecPID != keyCode.plan)
						break;
					if(0 != u8text::compareLen(pRecCode, keyCode.code, lenOfCode))
						break;
					if((DWORD)(BYTE)*pRecIsFolder == '0')	// счет
						elements.insert(id9to8(pRecID));
					else // Группа
					{
						CString code(pRecCode, codeLen);
						code.TrimRight();
						processedGroups[code] = 0;
					}
					if(!store.Goto(navNext, 0))
						break;
				}
			}
		}
	}
	// И теперь запихаем все в таблицу
	elements.doInsert(pInsert);
}
 


/*
Установка параметра.
*/

int SQLiteBase::xCreate(sqlite3* pBase, void*, int argc, const char *const*argv, sqlite3_vtab **ppVTab, char** pErr)
{
	if(argc<4)
		return SQLITE_ERROR;
	CVtabInfo* pVtabInfo = CVtabInfo::tabInfoForName(u8text::fromUtf8(argv[3]));
	if(!pVtabInfo)
		return SQLITE_ERROR;
	
	sqlite3_declare_vtab(pBase, pVtabInfo->textSqlCreate());
	*ppVTab = pVtabInfo;
	return SQLITE_OK;
}

int SQLiteBase::xDestroy(sqlite3_vtab *pVTab)
{
	return SQLITE_OK;
}

int SQLiteBase::xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info* pIdx)
{
	return static_cast<CVtabInfo*>(pVTab)->bestIndex(pIdx);
}

int SQLiteBase::xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor)
{
	return static_cast<CVtabInfo*>(pVTab)->openCursor(ppCursor);
}

int SQLiteBase::xClose(sqlite3_vtab_cursor* pC)
{
	static_cast<CursorImpl*>(pC)->Close();
	return SQLITE_OK;
}

int SQLiteBase::xFilter(sqlite3_vtab_cursor* pC, int idxNum, const char *idxStr, int argc, sqlite3_value **argv)
{
	return static_cast<CursorImpl*>(pC)->Filter(idxNum, idxStr, argc, argv);
}

int SQLiteBase::xNext(sqlite3_vtab_cursor* pC)
{
	return static_cast<CursorImpl*>(pC)->Next();
}

int SQLiteBase::xEof(sqlite3_vtab_cursor* pC)
{
	return static_cast<CursorImpl*>(pC)->IsEof();
}

int SQLiteBase::xColumn(sqlite3_vtab_cursor* pC, sqlite3_context* pCtx, int nCol)
{
	return static_cast<CursorImpl*>(pC)->Column(pCtx, nCol);
}

int SQLiteBase::xRowid(sqlite3_vtab_cursor* pC, sqlite3_int64 *pRowid)
{
	return static_cast<CursorImpl*>(pC)->RowID(pRowid);
}

struct finder
{
	CNoCaseMap<int> names;
	finder()
	{
		static struct{LPCSTR n1, n2; int s;} data[] = 
		{
			{"Строка", "String", ttString},
			{"Дата", "Date", ttDate},
			{"Число", "Number", ttNumber},
			{"Справочник", "Reference", ttReference},
			{"Документ", "Document", ttDocument},
			{"ВидДокумента", "DocumentKind", ttDocKind},
			{"ВидДокументаПредставление", "DocumentKindPresent", ttDocPresent},
			{"Перечисление", "Enum", ttEnum},
			{"Счет", "Account", ttAccount},
			{"Неопределенный", "Undefine", ttUndefine},
			{"ВидРасчета", "CalculationKind", ttCalcKind},
			{"Календарь", "Calendar", ttCalendar},
			{"Субконто", "Subconto", ttSubconto},
			{"Время", "Time", ttTime},
			{"ИмяВида", "KindName", ttKindName},
			{"ПредставлениеВида", "KindPresent", ttKindPresent},
			{"ВидСубконто", "SubcontoKind", ttSubcKind},
			{"ВидСубконтоПредставление", "SubcontoKindPresent", ttSubcPresent},
		};
		for(DWORD i = 0; i < sizeof(data) / sizeof(data[0]) ; i++)
		{
			names[data[i].n1] = data[i].s;
			names[data[i].n2] = data[i].s;
		}
	}
	int find(const CString& str)
	{
		int res = 0;
		names.Lookup(str, res);
		return res;
	}
};

SQLiteQuery::column_info::column_info(const CString& colName, DWORD col, column_info*& pNext)
	:name(colName), column(col), typeForVTColumn(0, 0), type(ttError), linkedField(NULL), next(pNext)
{
	pNext = this;
	CString typeName;
	SQLiteQuery::typeField(name, &typeName);
	
	if(!typeName.IsEmpty())
	{
		CStringArray strTypes;
		SplitStr2Array(typeName, strTypes, '.');
		
		int size = strTypes.GetSize();
		if(size > 0)
		{
			static finder fnd;
			switch(fnd.find(strTypes[0]))
			{
			case ttString:
				if(size < 3)
				{
					type = ttString;
					typeForVTColumn.type = typeString;
					if(2 == size)
						typeForVTColumn.m_length = atol(strTypes[1]);
				}
				break;
			case ttDate:
				if(1 == size)
				{
					type = ttDate;
					typeForVTColumn.type = typeDate;
				}
				break;
			case ttNumber:
				if(size < 4)
				{
					type = ttNumber;
					typeForVTColumn.type = typeNumber;
					if(size > 1)
						typeForVTColumn.m_length = atol(strTypes[1]);
					if(size > 2)
						typeForVTColumn.m_prec = atol(strTypes[2]);
				}
				break;
			case ttReference:
				if(1 == size)
				{
					type = ttReference;
					typeForVTColumn.type = typeReference;
				}
				else if(2 == size)
				{
					CSbCntTypeDef* pDef = pMetaDataCont->GetSTypeDef(strTypes[1]);
					if(pDef)
					{
						type = ttReferenceOne;
						typeForVTColumn.type = typeReference;
						typeForVTColumn.m_mdid = pDef->m_ID;
					}
				}
				break;
			case ttDocument:
				if(1 == size)
				{
					type = ttDocument;
					typeForVTColumn.type = typeDocument;
				}
				else if(2 == size)
				{
					CDocDef* pDef = pMetaDataCont->GetDocDef(strTypes[1]);
					if(pDef)
					{
						type = ttDocumentOne;
						typeForVTColumn.type = typeDocument;
						typeForVTColumn.m_mdid = pDef->m_ID;
					}
				}
				break;
			case ttDocKind:
				if(1 == size)
				{
					type = ttDocKind;
					typeForVTColumn.type = typeString;
				}
				break;
			case ttDocPresent:
				if(1 == size)
				{
					type = ttDocPresent;
					typeForVTColumn.type = typeString;
				}
				break;
			case ttEnum:
				if(1 == size)
				{
					type = ttEnum;
					typeForVTColumn.type = typeEnum;
				}
				else if(2 == size)
				{
					CEnumDef* pDef = pMetaDataCont->GetEnumDef(strTypes[1]);
					if(pDef)
					{
						type = ttEnumOne;
						typeForVTColumn.type = typeEnum;
						typeForVTColumn.m_mdid = pDef->m_ID;
					}
				}
				break;
			case ttAccount:
				if(1 == size)
				{
					type = ttAccount;
					typeForVTColumn.type = typeAccount;
				}
				else if(2 == size)
				{
					CBuhDef* pDef = pMetaDataCont->GetBuhDef();
					if(pDef)
					{
						CPlanDef* pPlan = pDef->GetPlanDef(strTypes[1]);
						if(pPlan)
						{
							type = ttAccountOne;
							typeForVTColumn.type = typeAccount;
							typeForVTColumn.m_mdid = pPlan->m_ID;
						}
					}
				}
				break;
			case ttUndefine:
				if(1 == size)
					type = ttUndefine;
				break;
			case ttCalcKind:
				if(1 == size)
				{
					type = ttCalcKind;
					typeForVTColumn.type = typeCalcKind;
				}
				break;
			case ttCalendar:
				if(1 == size)
				{
					type = ttCalendar;
					typeForVTColumn.type = typeCalendar;
				}
				break;
			case ttSubconto:
				if(1 == size)
				{
					type = ttSubconto;
					typeForVTColumn.type = typeUndefined;
				}
				break;
			case ttTime:
				if(1 == size)
				{
					type = ttTime;
					typeForVTColumn.type = 2;
					typeForVTColumn.m_length = 8;
				}
				break;
			case ttKindName:
				if(1 == size)
				{
					type = ttKindName;
					typeForVTColumn.type = typeString;
				}
				break;
			case ttKindPresent:
				if(1 == size)
				{
					type = ttKindPresent;
					typeForVTColumn.type = typeString;
				}
				break;
			case ttSubcKind:
				if(1 == size)
				{
					type = ttSubcKind;
					typeForVTColumn.type = typeString;
				}
				break;
			case ttSubcPresent:
				if(1 == size)
				{
					type = ttSubcPresent;
					typeForVTColumn.type = typeString;
				}
				break;
			}
		}
	}
	else
		type = ttAsIs;
}


void SQLiteQuery::column_info::toValue(sqlite3_stmt* pStmt, CValue* pValue)
{
	pValue->Reset();
	int typeOfData = sqlite3_column_type(pStmt, column);
	if(SQLITE_NULL == typeOfData)
	{
		switch(type)
		{
		case ttNumber:
			*pValue = 0L;
			break;
		case ttString:
		case ttDocKind:
		case ttDocPresent:
		case ttTime:
		case ttKindName:
		case ttKindPresent:
		case ttSubcKind:
		case ttSubcPresent:
			*pValue = "";
			break;
		case ttDate:
			*pValue = CDate(0, 0, 0);
			break;
		case ttReference:
			pValue->type = typeReference;
			break;
		case ttReferenceOne:
			pValue->type = typeReference;
			pValue->m_mdid = typeForVTColumn.m_mdid;
			break;
		case ttDocument:
		case ttDocumentWithLink:
			pValue->type = typeDocument;
			break;
		case ttDocumentOne:
			pValue->type = typeDocument;
			pValue->m_mdid = typeForVTColumn.m_mdid;
			break;
		case ttEnum:
			pValue->type = typeEnum;
			break;
		case ttEnumOne:
			pValue->type = typeEnum;
			pValue->m_mdid = typeForVTColumn.m_mdid;
			break;
		case ttAccount:
			pValue->type = typeAccount;
			break;
		case ttAccountOne:
			pValue->type = typeAccount;
			pValue->m_mdid = typeForVTColumn.m_mdid;
			break;
		case ttCalcKind:
			pValue->type = typeCalcKind;
			break;
		case ttCalendar:
			pValue->type = typeCalendar;
			break;
		}
		return;
	}
	
	static CString colText;

	switch(type)
	{
	case ttAsIs:
		switch(typeOfData)
		{
		case SQLITE_INTEGER:
			*pValue = sqlite3_column_int(pStmt, column);
			break;
		case SQLITE_FLOAT:
			*pValue = CNumeric(sqlite3_column_double(pStmt, column));
			break;
		case SQLITE_BLOB:
		case SQLITE_TEXT:
			*pValue = u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column));
			break;
		}
		break;
	case ttNumber:
		pValue->type = typeNumber;
		pValue->m_length = typeForVTColumn.m_length;
		pValue->m_prec = typeForVTColumn.m_prec;

		switch(typeOfData)
		{
		case SQLITE_INTEGER:
			pValue->m_Number = sqlite3_column_int(pStmt, column);
			break;
		case SQLITE_FLOAT:
			pValue->m_Number = sqlite3_column_double(pStmt, column);
			break;
		default:
			pValue->m_Number.FromString((LPCSTR)sqlite3_column_text(pStmt, column), NULL);
		}
		if(pValue->m_length && typeOfData != SQLITE_INTEGER)
			pValue->m_Number.FromString(pValue->Format(), NULL);
		break;
	case ttString:
		*pValue = u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column));
		break;
	case ttDate:	// из utf-8 не преобразуем, тк там должны быть тока цифры
		if(SQLITE_TEXT == typeOfData)
		{
			colText = (LPCSTR)sqlite3_column_text(pStmt, column);
			if(colText.GetLength() == 8)
			{
				const unsigned char* ptr = (const unsigned char*)(LPCSTR)colText;
				*pValue = CDate(str2dec(ptr, 4), str2dec(ptr + 4, 2), str2dec(ptr + 6, 2));
			}
		}
		break;
	case ttReference:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 13)
				ValueStrWork<tos13>::str2val(pValue, colText, typeReference);
		}
		break;
	case ttReferenceOne:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 9)
				ValueStrWork<tos9>::str2val(pValue, colText, typeReference, typeForVTColumn.m_mdid);
		}
		break;
	case ttDocument:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 13)
				ValueStrWork<tos13>::str2val(pValue, colText, typeDocument);
		}
		break;
	case ttDocumentWithLink:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 9)
			{
				long mdID = 0;
				if(SQLITE_INTEGER == sqlite3_column_type(pStmt, linkedField->column))
					mdID = sqlite3_column_int(pStmt, linkedField->column);
				else
					mdID = str2id(sqlite3_column_text(pStmt, linkedField->column), 4);
				if(mdID)
					ValueStrWork<tos9>::str2val(pValue, colText, typeDocument, mdID);
			}
		}
		break;
	case ttDocumentOne:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 9)
				ValueStrWork<tos9>::str2val(pValue, colText, typeDocument, typeForVTColumn.m_mdid);
		}
		break;
	case ttDocKind:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				if(CMetaDataObjArray* pDocs = pMetaDataCont->GetDocDefs())
				{
					if(CMetaDataObj* pObj = pDocs->GetItem(id))
						*pValue = pObj->m_Code;
				}
			}
		}
		break;
	case ttDocPresent:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				if(CMetaDataObjArray* pDocs = pMetaDataCont->GetDocDefs())
				{
					if(CMetaDataObj* pObj = pDocs->GetItem(id))
						*pValue = pObj->GetRealPresent();
				}
			}
		}
		break;
	case ttEnum:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 13)
				ValueStrWork<tos13>::str2val(pValue, colText, typeEnum);
		}
		break;
	case ttEnumOne:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 9)
				ValueStrWork<tos9>::str2val(pValue, colText, typeEnum, typeForVTColumn.m_mdid);
		}
		break;
	case ttAccount:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 13)
				ValueStrWork<tos13>::str2val(pValue, colText, typeAccount);
		}
		break;
	case ttAccountOne:
		if(SQLITE_TEXT == typeOfData)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			if(colText.GetLength() == 9)
				ValueStrWork<tos9>::str2val(pValue, colText, typeAccount, typeForVTColumn.m_mdid);
		}
		break;
	case ttUndefine:
		u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
		if(colText.GetLength() == 23)
			ValueStrWork<tos23>::str2val(pValue, colText);
		break;
	case ttCalcKind:
		if(SQLITE_TEXT == typeOfData)
		{
			colText = (LPCSTR)sqlite3_column_text(pStmt, column);
			int len = colText.GetLength();
			if(4 == len)
			{
				pValue->type = typeCalcKind;
				pValue->m_ObjID.ObjID = str2id((const unsigned char*)(LPCSTR)colText, 4);
				*(DWORD*)pValue->m_ObjID.DBSign.Sign = 0x00202020;
			}
			else if(13 == len)
				ValueStrWork<tos13>::str2val(pValue, colText, typeCalcKind);
		}
		else if(SQLITE_INTEGER == typeOfData)
		{
			pValue->type = typeCalcKind;
			pValue->m_ObjID.ObjID = sqlite3_column_int(pStmt, column);
			*(DWORD*)pValue->m_ObjID.DBSign.Sign = 0x00202020;
		}
		break;
	case ttCalendar:
		if(SQLITE_TEXT == typeOfData)
		{
			colText = (LPCSTR)sqlite3_column_text(pStmt, column);
			if(13 == colText.GetLength())
				ValueStrWork<tos13>::str2val(pValue, colText, typeCalendar);
		}
		break;
	case ttSubconto:
		if(SQLITE_TEXT == typeOfData && linkedField)
		{
			u8text::fromUtf8((LPCSTR)sqlite3_column_text(pStmt, column), colText);
			long sbKindID = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, linkedField->column))
				sbKindID = sqlite3_column_int(pStmt, linkedField->column);
			else
				sbKindID = str2id(sqlite3_column_text(pStmt, linkedField->column), 4);
			if(sbKindID)
			{
				CBuhDef* pDef = pMetaDataCont->GetBuhDef();
				if(pDef)
				{
					CSbKindDef* pSbKindDef = pDef->GetSbKindDefs()->GetItem(sbKindID);
					if(pSbKindDef && pSbKindDef->m_TypeCode >= typeEnum)
					{
						if(0 == pSbKindDef->m_Kind)
						{
							if(colText.GetLength() >= 13)
								ValueStrWork<tos13>::str2val(pValue, colText, (Types1C)pSbKindDef->m_TypeCode);
						}
						else
						{
							if(colText.GetLength() >= 9)
								ValueStrWork<tos9>::str2val(pValue, colText, (Types1C)pSbKindDef->m_TypeCode, pSbKindDef->m_Kind);
						}
					}
				}
			}
		}
		break;
	case ttTime:
		{
			DWORD time;
			if(SQLITE_TEXT == typeOfData)
				time = str2id(sqlite3_column_text(pStmt, column), sqlite3_column_bytes(pStmt, column)) / 10000;
			else if(SQLITE_INTEGER == typeOfData)
				time = sqlite3_column_int(pStmt, column) / 10000;
			else
				time = 0;
			colText.Format("%02i:%02i:%02i", time / 3600, (time % 3600) / 60, time % 60);
			*pValue = colText;
		}

		break;
	case ttKindName:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				CMetaDataObj* pObj = pMetaDataCont->FindObject(id);
				if(pObj)
					*pValue = pObj->m_Code;
			}
		}
		break;
	case ttKindPresent:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				CMetaDataObj* pObj = pMetaDataCont->FindObject(id);
				if(pObj)
					*pValue = pObj->GetRealPresent();
			}
		}
		break;
	case ttSubcKind:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				if(CBuhDef* pDef = pMetaDataCont->GetBuhDef())
				{
					if(CTypedFldDefsArray<CSbKindDef>* pSB = pDef->GetSbKindDefs())
					{
						if(CMetaDataObj* pObj = pSB->GetItem(id))
							*pValue = pObj->m_Code;
					}
				}
			}
		}
		break;
	case ttSubcPresent:
		{
			long id = 0;
			if(SQLITE_INTEGER == sqlite3_column_type(pStmt, column))
				id = sqlite3_column_int(pStmt, column);
			else
				id = str2id(sqlite3_column_text(pStmt, column), 4);
			if(id)
			{
				if(CBuhDef* pDef = pMetaDataCont->GetBuhDef())
				{
					if(CTypedFldDefsArray<CSbKindDef>* pSB = pDef->GetSbKindDefs())
					{
						if(CMetaDataObj* pObj = pSB->GetItem(id))
							*pValue = pObj->GetRealPresent();
					}
				}
			}
		}
		break;
	}
}

extern "C" int connect1CTable(sqlite3* db, const char* zName)
{
	if(!pDataDict)
		return 0;
	CString strName = u8text::fromUtf8(zName), strOrigName(strName);
	CVtabInfo* pInfo = CVtabInfo::tabInfoForName(strName);
	
	if(!pInfo && strName.Find('_') >= 0)
	{
		DWORD len = strName.GetLength();
		char *buf = new char[len + 1], *pWrite = buf + len;
		LPCSTR pStart = strName, pRead = pStart + len - 1;
		*pWrite = 0;
		while(pRead >= pStart)
		{
			if(*pRead == '_')
			{
				if(pRead[-1] == '_')
				{
					*--pWrite = *pRead--;
					pRead--;
				}
				else
				{
					*--pWrite = '.';
					pRead--;
				}
			}
			else
				*--pWrite = *pRead--;
		}
		strName = pWrite;
		delete [] buf;
		pInfo = CVtabInfo::tabInfoForName(strName);
	}
	if(pInfo)
	{
		CString strSqlCreate;
		strSqlCreate.Format("create virtual table [%s] using dbeng(%s)", strOrigName, strName);
		u8text::toUtf8(strSqlCreate);
		return SQLITE_OK == sqlite3_exec(db, strSqlCreate, NULL, NULL, NULL);
	}
	return 0;
}


void SQLiteBase::open(const CString& fileName)
{
	close();
	CString u8FileName = fileName;
	u8text::toUtf8(u8FileName);

	if(SQLITE_OK != sqlite3_open(u8FileName, &m_pDataBase))
	{
		CString err;
		if(m_pDataBase)
		{
			u8text::fromUtf8(sqlite3_errmsg(m_pDataBase), err);
			sqlite3_close(m_pDataBase);
			m_pDataBase = NULL;
		}
		else
			err = "Недостаточно памяти";
		CBLModule::RaiseExtRuntimeError(err, FALSE);
	}
	if(pDataDict)
	{
		static sqlite3_module mod = {
			1,
			xCreate,
			xCreate,
			xBestIndex,
			xDestroy,
			xDestroy,
			xOpen,
			xClose,
			xFilter,
			xNext,
			xEof,
			xColumn,
			xRowid,
			NULL,//   int (*xUpdate)(sqlite3_vtab *, int, sqlite3_value **, sqlite3_int64 *);
			//   int (*xBegin)(sqlite3_vtab *pVTab);
			//   int (*xSync)(sqlite3_vtab *pVTab);
			//   int (*xCommit)(sqlite3_vtab *pVTab);
			//   int (*xRollback)(sqlite3_vtab *pVTab);
			//   int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),void **ppArg);
			//   int (*xRename)(sqlite3_vtab *pVtab, const char *zNew);
		};
		sqlite3_create_module(m_pDataBase, "dbeng", &mod, NULL);
	}
	sqlite3_create_collation(m_pDataBase, "_1C", SQLITE_UTF8, NULL, &u8text::_1Ccollate);
	sqlite3_create_function(m_pDataBase, "str2id", 1, SQLITE_UTF8, NULL, baseStr2Id, NULL, NULL);
	sqlite3_create_function(m_pDataBase, "id2str", 2, SQLITE_UTF8, NULL, baseId2Str, NULL, NULL);
	sqlite3_create_function(m_pDataBase, "coalesceex", -1, SQLITE_UTF8, NULL, coalesceEx, NULL, NULL);
	//sqlite3_progress_handler(m_pDataBase, 1000, progress_func, 0);
}


void SQLiteBase::traceOn()
{
	bNeedProfile = TRUE;
}

void SQLiteBase::putVT(CValueTable* pVT, const CString& strNameOfTable, BOOL bAsPersistent)
{
	if(!m_pDataBase)
		CBLModule::RaiseExtRuntimeError("База данных не открыта", FALSE);
	
	if(strNameOfTable.IsEmpty())
		CBLModule::RaiseExtRuntimeError("Не задано имя таблицы", FALSE);
	
	
	DWORD cols = pVT->GetColumnCount();
	if(!cols)
		CBLModule::RaiseExtRuntimeError("В таблице значений нет колонок", FALSE);

	CString strSqlCreate, strSqlInsert;
	strSqlCreate.Format("drop table if exists %s; create%s table %s (", strNameOfTable, bAsPersistent? "" : " temp", strNameOfTable);
	strSqlInsert.Format("insert into %s values(", strNameOfTable);
	
	CDWordArray modificators;
	
	for(DWORD idx = 0; idx < cols ; idx++)
	{
		CVTColumn* pCol = pVT->GetColumn(idx);
		strSqlCreate += pCol->GetCode() + " not null,";
		strSqlInsert += "?,";
		
		int mod = 0;
		LPCSTR pMod = strstr(pCol->GetTitle(), "mod=");
		if(pMod)
			mod = atol(pMod + 4);
		else
		{
			const CType& type = pCol->GetType();
			if(type.type >= typeEnum && !type.m_mdid)
				mod = 1;
			else if(type.type == typeUndefined)
				mod = -1;
		}
		modificators.Add(mod);
	}
	strSqlCreate.SetAt(strSqlCreate.GetLength() - 1, ')');
	strSqlInsert.SetAt(strSqlInsert.GetLength() - 1, ')');

	u8text::toUtf8(strSqlCreate);
	u8text::toUtf8(strSqlInsert);

	tran_guard tg(m_pDataBase);

	if(SQLITE_OK != sqlite3_exec(m_pDataBase, strSqlCreate, NULL, NULL, NULL))
		raiseDBError();

	DWORD rows = pVT->GetRowCount();
	if(!rows)
	{
		tg.err = FALSE;
		return;
	}
	
	sqlite3_stmt* pInsert = NULL;
	const char* pTail;
	if(SQLITE_OK != sqlite3_prepare_v2(m_pDataBase, strSqlInsert, strSqlInsert.GetLength(), &pInsert, &pTail))
		raiseDBError();

	CString text;

	for(DWORD r = 0; r < rows; r++)
	{ 
		for(DWORD c = 0; c < cols; )
		{
			CVTColumn* pCol = pVT->GetColumn(c);
			int mod = modificators[c++];
			bindValue(bindSqlParam(pCol->GetCode(), c, m_pDataBase, pInsert), &pCol->Get(r), mod);
		}
		sqlite3_step(pInsert);
		sqlite3_reset(pInsert);
	}
	sqlite3_finalize(pInsert);
	tg.err = FALSE;
}

void SQLiteBase::putObjects(CValue* pObjects, const CString& strNameOfTable, BOOL bAsPersistent, const CString& kindOfObjects)
{
	if(!m_pDataBase)
		CBLModule::RaiseExtRuntimeError("База данных не открыта", FALSE);
	CPtrArray* pVL = NULL;
	
	if(pObjects->type == 100 && pObjects->m_Context && !strcmp(pObjects->m_Context->GetRuntimeClass()->m_lpszClassName, "CValueListContext"))
		pVL=*(CPtrArray**)(((char*)pObjects->m_Context) + 0x30);
	else if(!(pObjects->type == typeReference || pObjects->type == typeAccount))
		CBLModule::RaiseExtRuntimeError("Недопустимый тип первого параметра", FALSE);
	
	if(strNameOfTable.IsEmpty())
		CBLModule::RaiseExtRuntimeError("Не задано имя таблицы", FALSE);
	
	
	CSbCntTypeDef* pReference = NULL;
	CPlanDef* pChartOfAccount = NULL;
	BOOL bRefWithGroups = FALSE;
	if(!kindOfObjects.IsEmpty())
	{
		LPCSTR pKindOfObjects = kindOfObjects;
		if(pKindOfObjects[0] == '+')
		{
			bRefWithGroups = TRUE;
			pKindOfObjects++;
		}
		pReference = pMetaDataCont->GetSTypeDef(pKindOfObjects);
		if(!pReference)
		{
			CBuhDef* pBuh = pMetaDataCont->GetBuhDef();
			if(pBuh)
				pChartOfAccount = pBuh->GetPlanDef(kindOfObjects);
			if(!pChartOfAccount)
				CBLModule::RaiseExtRuntimeError("Не найден справочник или план счетов заданного вида", FALSE);
		}
	}
	
	if(pReference && pReference->GetLevelsLimit() <= 1)
		pReference = NULL; // разворачивать по одноуровнему справочнику глупо.
	if((pReference || pChartOfAccount) && !pDataDict)
		CBLModule::RaiseExtRuntimeError("Обработка иерархии возможна только в DBF базах", FALSE);
	
	CString strSqlCreate, strSqlInsert;
	strSqlCreate.Format(
		"drop table if exists %s; create%s table %s (val char(9))",
		strNameOfTable, bAsPersistent? "" : " temp", strNameOfTable);
	strSqlInsert.Format("insert into %s values(?)", strNameOfTable);
	
	u8text::toUtf8(strSqlCreate);
	u8text::toUtf8(strSqlInsert);
	
	tran_guard tg(m_pDataBase);
	
	if(SQLITE_OK != sqlite3_exec(m_pDataBase, strSqlCreate, NULL, NULL, NULL))
		raiseDBError();

	sqlite3_stmt* pInsert = NULL;
	const char* pTail;
	if(SQLITE_OK != sqlite3_prepare_v2(m_pDataBase, strSqlInsert, strSqlInsert.GetLength(), &pInsert, &pTail))
		raiseDBError();

	CString val;
	if(pReference)
		putvlReference(pReference, pVL, pObjects, pInsert, bRefWithGroups);
	else if(pChartOfAccount)
		putvlAccounts(pChartOfAccount, pVL, pObjects, pInsert);
	else	// Не надо разворачивать по группам
	{
		if(pVL)
		{
			CValue** ppVals = (CValue**)pVL->GetData();
			for(int i = pVL->GetSize(); i--; ppVals++)
			{
				ValueStrWork<tos9>::val2str(**ppVals, val);
				u8text::toUtf8(val);
				sqlite3_bind_text(pInsert, 1, val, val.GetLength(), SQLITE_TRANSIENT);
				sqlite3_step(pInsert);
				sqlite3_reset(pInsert);
			}
		}
		else
		{
			ValueStrWork<tos9>::val2str(*pObjects, val);
			u8text::toUtf8(val);
			sqlite3_bind_text(pInsert, 1, val, val.GetLength(), SQLITE_TRANSIENT);
			sqlite3_step(pInsert);
			sqlite3_reset(pInsert);
		}
	}
	sqlite3_finalize(pInsert);
	tg.err = FALSE;
}

void SQLiteQuery::prepare(const CString& query)
{
	close();
	if(!m_pParent->base())
		CBLModule::RaiseExtRuntimeError("База данных не открыта", FALSE);
	
	CString u8strQuery(query);

	m_parser.processSql(u8strQuery);
	m_parser.reset();

	if(m_bIsDebug)
	{
		DoMsgLine("%s", mmNone, u8strQuery);
		bDoTrace = TRUE;
	}
	
	u8text::toUtf8(u8strQuery);

	const char* pTail;
	CString prep_error;
	int res = sqlite3_prepare_v2(m_pParent->base(), u8strQuery, u8strQuery.GetLength(), &m_pStmt, &pTail);

	m_bIsDebug = bDoTrace = FALSE;

	if(SQLITE_OK == res)
	{
		int cols = sqlite3_column_count(m_pStmt);
		if(cols)
		{
			CNoCaseMap<column_info*> allCols;
			CString colName;
			while(cols)
			{
				u8text::fromUtf8(sqlite3_column_name(m_pStmt, --cols), colName);
				new column_info(colName, cols, m_columns);
				if(m_columns->type == ttError)
					prep_error = prep_error + "Неправильная типизация в колонке: " + colName + "\r\n";
				if(allCols.InsertExist(m_columns->name, m_columns))
					prep_error = prep_error + "Колонка с именем " + m_columns->name + " уже существует\r\n";
			}
			if(prep_error.IsEmpty())
			{
				for(column_info* ptr1 = m_columns; ptr1; ptr1 = ptr1->next)
				{
					if(ptr1->type == ttDocument || ptr1->type == ttSubconto)
					{
						if(!allCols.Lookup(ptr1->name + "_вид", ptr1->linkedField))
							allCols.Lookup(ptr1->name + "_kind", ptr1->linkedField);
						if(!ptr1->linkedField && ptr1->type == ttSubconto)
							prep_error = prep_error + "Для колонки " + ptr1->name + " не найдена типизирующая колонка.\r\n";
						if(ptr1->linkedField && ptr1->type == ttDocument)
							ptr1->type = ttDocumentWithLink;
					}
				}
			}
		}
	}
	else
		u8text::fromUtf8(sqlite3_errmsg(m_pParent->base()), prep_error);

	if(!prep_error.IsEmpty())
	{
		close();
		CBLModule::RaiseExtRuntimeError(prep_error, FALSE);
	}
}

struct reset_stmt_guard
{
	reset_stmt_guard(sqlite3_stmt* pStmt, CString& p) :
		m_pStmt(pStmt), m_profileDest(p){}
	~reset_stmt_guard()
	{
		sqlite3_reset(m_pStmt);
		if(bNeedProfile)
		{
			strProfile.Replace("\n", "\r\n");
			u8text::fromUtf8(strProfile, m_profileDest);
			strProfile.Empty();
			bNeedProfile = FALSE;
		}
	}
private:
	sqlite3_stmt* m_pStmt;
	CString& m_profileDest;
};

void SQLiteQuery::execute(CValue* pDst, CValue* pDstParam, CValue& retVal)
{
	if(!m_pStmt)
		CBLModule::RaiseExtRuntimeError("Запрос не подготовлен", FALSE);

	read_tran_guard qg(m_bNeedTransaction);
	reset_stmt_guard rg(m_pStmt, SQLiteBase::m_trace);

	DWORD cols = sqlite3_column_count(m_pStmt);
	if(cols)
	{
		ISQLiteResultLoader* pLoader = NULL;
		
		IVTResultLoader vtLoader;
		IVLResultLoader vlLoader;
		IScalarResultLoader scLoader;

		if(pDst->type == typeNumber)
		{
			DWORD colInScalar = (long)pDst->m_Number;
			if(colInScalar < cols)
			{
				scLoader.setCol(colInScalar);
				pLoader = &scLoader;
			}
			else
				CBLModule::RaiseExtRuntimeError("Неверный номер колонки скалярного результата", FALSE);
		}
		else if(pDst->type == 100)
		{
			if(CBLContext* pCont = pDst->m_Context)
			{
				if(0 == strcmp(pCont->GetRuntimeClass()->m_lpszClassName, "CValueTableContext"))
				{
					vtLoader.setContext(pCont);
					pLoader = &vtLoader;
				}
				else if(0 == strcmp(pCont->GetRuntimeClass()->m_lpszClassName, "CValueListContext"))
				{
					vlLoader.setContext(pCont);
					pLoader = &vlLoader;
				}
				else
					CastContext::Dynamic(pCont, pLoader);
			}
		}
		else if(pDst->IsEmpty())
		{
			CValue val;
			LoadValueFromString(&val, "{\"VT\",\"1\",{\"0\",{{\"\",\"0\",\"0\",\"0\",\"\",\"2\"}}}}");
			vtLoader.setContext(val.m_Context);
			pLoader = &vtLoader;
		}
		if(!pLoader)
			CBLModule::RaiseExtRuntimeError("Не удалось создать объект-приемник", FALSE);
		
		cols = pLoader->init(cols, m_pParent->base(), pDstParam);
		
		struct val_array
		{
			CValue *pVals, **ppVals;
			val_array(DWORD cols)
			{
				pVals = new CValue[cols];
				ppVals = new CValue*[cols];
			}
			~val_array()
			{
				delete [] ppVals;
				delete [] pVals;
			}
		};

		val_array va(cols);
		CValue* pVals = va.pVals;
		CValue** ppVals = va.ppVals;

		column_info* pColInfo = m_columns;
		
		for(DWORD i = 0; i < cols; i++)
		{
			pLoader->setColumn(i, pColInfo->name, pColInfo->typeForVTColumn);
			ppVals[i] = pVals + i;
			pColInfo = pColInfo->next;
		}
		for(;;)
		{
			int ret = sqlite3_step(m_pStmt);
			if(ret == SQLITE_ROW)
			{
				pColInfo = m_columns;
				for(int i = 0; i<cols ; i++)
				{
					pColInfo->toValue(m_pStmt, pVals + i);
					pColInfo = pColInfo->next;
				}
				pLoader->addValues(ppVals);
			}
			else if(SQLITE_DONE == ret)
				break;
			else
				m_pParent->raiseDBError();
		}
		pLoader->assignRetValue(retVal);
	}
	else
	{
		int res = sqlite3_step(m_pStmt);
		if(res == SQLITE_DONE)
			retVal = sqlite3_changes(m_pParent->base());
		else
			m_pParent->raiseDBError();
	}
}

void SQLiteQuery::setSqlParam(const CValue& param, const CValue& value, int mod, BOOL bThrow /*= TRUE*/)
{
	if(!m_pStmt)
		CBLModule::RaiseExtRuntimeError("Запрос не подготовлен", FALSE);

	CString paramName;
	int idx;
	if(param.type == 2)	// Строка
	{
		paramName = param.m_String;
		CString name = paramName;
		u8text::toUtf8(name);
		idx = sqlite3_bind_parameter_index(m_pStmt, name);
	}
	else
	{
		idx = (long)param.GetNumeric();
		paramName.Format("%i", idx);
	}

	if(0 == idx || idx > sqlite3_bind_parameter_count(m_pStmt))
	{
		if(bThrow)
			CBLModule::RaiseExtRuntimeError("Неверный номер параметра", FALSE);
		return;
	}
	bindValue(bindSqlParam(paramName, idx, m_pParent->base(), m_pStmt), &value, mod);
}

void SQLiteQuery::close()
{
	if(m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
	}
	if(m_columns)
	{
		column_info* pInfo = m_columns;
		while(pInfo)
		{
			column_info* pDel = pInfo;
			pInfo = pInfo->next;
			delete pDel;
		}
		m_columns = NULL;
	}
}

void SQLiteQuery::getFields(CStringArray& fields, TFArray& types)
{
	if(!m_columns)
		CBLModule::RaiseExtRuntimeError("Запрос не подготовлен", FALSE);
	column_info* pInfo = m_columns;
	DWORD s = sqlite3_column_count(m_pStmt);
	fields.SetSize(s);
	types.SetSize(s);
	typesOfFields* pTypes = types.GetData();
	CString* pString = fields.GetData();
	while(pInfo)
	{
		*pString++ = pInfo->name;
		*pTypes++ = pInfo->type;
		pInfo = pInfo->next;
	}
}

void SQLiteQuery::typeField(CString& name, CString* pType)
{
	name.TrimLeft('[');
	name.TrimRight(']');
	
	int isTyped = name.Find(':');
	if(isTyped < 0)
		isTyped = name.Find('$');
	
	if(isTyped > 0)
	{
		if(pType)
		{
			*pType = name.Mid(isTyped + 1);
			pType->TrimLeft();
			pType->TrimRight();
		}
		name.GetBufferSetLength(isTyped);
	}
	name.TrimLeft();
	name.TrimRight();
	name.Replace(' ', '_');
	name.Replace('.', '_');
}

BOOL CSLDataBase::PutVT(CValue** ppParams)
{
	CValueTable* pVT = NULL;//, const CString& strNameOfTable, BOOL bAsPersistent
	if(ppParams[0]->type == 100 && ppParams[0]->m_Context)
	{
		CBLContext* pCont = CastContext::ByRTCName(ppParams[0]->m_Context, "CValueTableContext");
		if(pCont)
			pVT = ((CValueTableContextData*)pCont->GetInternalData())->GetValueTable();
	}
	if(!pVT)
		CBLModule::RaiseExtRuntimeError("Не удалось получить таблицу значений из первого параметра", FALSE);
	putVT(pVT, ppParams[1]->GetString(), 0 != (long)ppParams[2]->GetNumeric());

	return TRUE;
}

/*
Применяется для поиска утечек памяти.
struct mem_test
{
	CMapPtrToPtr allocks;
	~mem_test()
	{
		DoMsgLine("Count deallocated %i", mmNone, allocks.GetCount());
		for(POSITION pos = allocks.GetStartPosition(); pos;)
		{
			DWORD s;
			void* p;
			allocks.GetNextAssoc(pos, p, (void*&)s);
		}
	}
} _mem_;

void* operator new (size_t s)
{
	if(s == 0)
	{
		DoMsgLine("aa");
	}
	void* p = malloc(s);
	_mem_.allocks[p] = (void*)s;
	return p;
}

void operator delete(void* p)
{
	if(p)
	{
		_mem_.allocks.RemoveKey(p);
		free(p);
	}
}
*/

