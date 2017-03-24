// vtab_info.cpp
#include "StdAfx.h"
#include "vtab_info.h"
#include "utex.h"
#include "referencetabinfo.h"
#include "journal.h"
#include "docheaders.h"
#include "doctables.h"
#include "register.h"
#include "index_selector.h"
#include "longstrreader.h"
#include "systabs.h"
#include "calcjournal.h"

CNoCaseMap<CVtabInfo*> CVtabInfo::m_allTabs;
CString CVtabInfo::m_lastError;
struct tab_deleter{ void operator()(CVtabInfo* pTab){delete pTab;}};

DWORD table_reader::allReaderCount = 0;
BOOL table_reader::bWeOpenTransaction = FALSE;
BOOL table_reader::bNeedTransaction = FALSE;

void CVtabInfo::doneWork()
{
	m_allTabs.ForEachValue(tab_deleter());
}

NOTHROW static void TraceConstraints(sqlite3_index_info* pInfo, const CVtabInfo* pVTab)
{
	if(!bDoTrace)
		return;
	CString text;
	text.Format("Подбор индекса для таблицы %s:\r\n\tОграничения: ", (LPCSTR)pVTab->tableName());
	CTableEx* pTable = pVTab->table();

	sqlite3_index_info::sqlite3_index_constraint* pC = pInfo->aConstraint;
	for(int i = 0, c = pInfo->nConstraint ; i < c ; i++, pC++)
	{
		if(!pC->usable)
			continue;
		text += pVTab->field(pC->iColumn).name();
		
		switch(pC->op)
		{
		case SQLITE_INDEX_CONSTRAINT_EQ:
			text += '=';
			break;
		case SQLITE_INDEX_CONSTRAINT_GT:
			text += '>';
			break;
		case SQLITE_INDEX_CONSTRAINT_LE:
			text += "<=";
			break;
		case SQLITE_INDEX_CONSTRAINT_LT:
			text += '<';
			break;
		case SQLITE_INDEX_CONSTRAINT_GE:
			text += ">=";
			break;
		case SQLITE_INDEX_CONSTRAINT_MATCH:
			text += " match";
			break;
		}
		text += "; ";
	}
	if(pInfo->nOrderBy)
	{
		text += "\r\n\tУпорядочить: ";
		orderby_ptr pOrderBy = pInfo->aOrderBy;
		for(int i = 0; i < pInfo->nOrderBy; i++, pOrderBy++)
		{
			text += pVTab->field(pOrderBy->iColumn).name();
			
			if(pOrderBy->desc)
				text += " desc";
			text += ", ";
		}
	}
	DoMsgLine("%s", mmNone, text);
}

NOTHROW static void TraceIndex(sqlite3_index_info* pIdx, BOOL bFromCash, const CVtabInfo* pVTab)
{
	if(!bDoTrace)
		return;
	CString text;
	if(pIdx->idxNum >= 0)
	{
		CIndexEx* pIndex = pVTab->table()->index(pIdx->idxNum);
		text.Format("\tВыбран индекс %s: %s", pIndex->p_10->szName, pIndex->p_10->szIdxExpr);
	}
	else
		text = "\tИндекс не выбран.";
	DoMsgLine(bFromCash ? "\tНайдено в кэше" : "\tВ кэше не найдено");
	DoMsgLine("%s", mmNone, text);
	if(pIdx->orderByConsumed)
		DoMsgLine("\tПопадает в сортировку");
	DoMsgLine("\tСтоимость: %i", mmNone, (int)pIdx->estimatedCost);
}


int CVtabInfo::bestIndex(sqlite3_index_info* pIdx) const
{
	//bDoTrace = TRUE;
	TraceConstraints(pIdx, this);


	BOOL fromCash = TRUE;
	
	CString cashKey;
	if(!stratCash.getFromCash(pIdx, cashKey))
	{
		index_selector idx_usage;
		const idx_node* pBest = idx_usage.bestIndex(pIdx, *this);
		FilterMachine::build(*this, pIdx, pBest);
		if(pBest)
		{
			if(pBest->indexNum == -1)	// recNo
				pIdx->estimatedCost = 1;
			else
				pIdx->estimatedCost = double(logRowCount * pBest->fullLenInIdx) / (pBest->usedLenOfIndex + pBest->orderByLen);
		}
		else
			pIdx->estimatedCost = 10000 - logRowCount;
		stratCash.addToCash(cashKey, pIdx);
		fromCash = FALSE;
	}
	
	TraceIndex(pIdx, fromCash, this);
	
	return SQLITE_OK;
}


void CVtabInfo::fillNamesFromObjs(CMetaDataObjArray* arr, CNoCaseMap<CString>& aliaces, CDWordArray* longStr)
{
	for(int i = 0, c = arr->GetNItems(); i<c ; i++)
	{
		CMetaDataObj* pObj = arr->GetAt(i);
		if(pObj->IsTypedObj())
		{
			if(longStr)
			{
				const CType& t = ((CMetaDataTypedObj*)pObj)->GetType();
				if(t.type == 2 && 0 == t.m_length)
					longStr->Add(pObj->m_ID);
			}
			aliaces[((CMetaDataTypedObj*)pObj)->GetFieldName()] = pObj->m_Code;
		}
	}
}

void CVtabInfo::fillTabInfo(CNoCaseMap<CString>& aliaces, CDWordArray* longStr)
{
	m_phisInfo = phisical_info::buildInfo(m_pTable);
	m_pFields = new one_field[m_phisInfo->fieldsCount() + m_phisInfo->indexesCount() + 1 + (longStr ? longStr->GetSize() : 0)];
	m_pFields->type = one_field::fRecNo;
	m_pFields->m_name = "recNo[rowid]";

	// Заполним физические поля
	one_field* pOneField = ++m_pFields;
	CNoCaseMap<int> allFields;

	CString create, name, fmt, fldName;
	for(DWORD i = 0; i < m_phisInfo->fieldsCount(); i++)
	{
		CField* pField = m_pTable->field(i);
		fldName = pField->szName;
		if(!aliaces.Lookup(pField->szName, name))
			name = fldName;
		else
			fldName = fldName + '[' + name + ']';
		one_field::fType type = one_field::fField;

		while(allFields.IsExist(name))
			name += '_';
		allFields[name] = 0;

		create = create + ",[" + name + "]";
		switch(pField->TypeCField)
		{
		case 1: // char
			fmt.Format(" char(%i) collate _1C", pField->sizeCField);
			create += fmt;
			break;
		case 2:	// numeric
			fmt.Format(" numeric(%i, %i)", pField->sizeCField, pField->precCField);
			create += fmt;
			{
				LPCSTR pRead = pField->szName;
				while(*pRead && *pRead > '9')
					pRead++;
				if(*pRead)
				{
					CMetaDataObj* pObj = pMetaDataCont->FindObject(atol(pRead));
					if(pObj && pObj->IsTypedObj() && !((CMetaDataTypedObj*)pObj)->GetType().IsPositiveOnly())
						type = one_field::fNumNegateField;
				}
			}
			break;
		case 3:	// date
			create += " char(8)";
			break;
		default:
			create += " text collate _1C";
		}
		create += " not null\r\n";
		pOneField->type = type;
		pOneField->position = i;
		pOneField->m_name = fldName;
		pOneField++;
	}
	// Заполним поля длинных строк
	if(longStr)
	{
		for(i = 0; i<longStr->GetSize(); i++)
		{
			DWORD id = longStr->GetAt(i);
			CMetaDataObj* pObj = pMetaDataCont->FindObject(id);
			DWORD buf = 0x20202020;
			char* ptr = ((char*)&buf) + 3;
			do
			{
				DWORD mod = id % 36;
				*ptr-- = mod < 10 ? mod + '0' : mod - 10 + 'A';
				id /= 36;
			}while(id);

			pOneField->type = one_field::fLongStr;
			pOneField->m_name = pObj->m_Code;
			pOneField->position = buf;
			create = create + "," + pOneField->m_name + " text collate _1C default null\r\n";
			pOneField++;
		}
	}

	CString idxName;

	for(i = 0; i < m_phisInfo->indexesCount() ; i++)
	{
		CIndexEx* pIndex = m_pTable->index(i);
		
		DWORD len = 0, nCount = pIndex->fieldsCount();
		idxName = "idx";
		for(DWORD j = 0; j < nCount ; j++)
		{
			CField* pField = pIndex->field(j);

			if(!aliaces.Lookup(pField->szName, name))
				name = pField->szName;
			idxName = idxName + '_' + name;
			len += pField->sizeCField;
		}
		fmt.Format(", %s char(%i) collate _1C default null\r\n", idxName, len);
		create += fmt;
		pOneField->type = one_field::fVirtIdx;
		pOneField->position = i;
		pOneField->m_name = CString(pIndex->szName) + '[' + ((LPCSTR)idxName + 1) + ']';
		pOneField++;
	}
	
	create.SetAt(0, ' ');
	m_strSqlCreate.Format("create table x(\r\n%s)", create);
	if(bDoTrace)
		DoMsgLine("%s", mmNone, m_strSqlCreate);
	u8text::toUtf8(m_strSqlCreate);

	// Получим приблизительное количество строк.
	CStoreObj store(m_pTable, NULL);
	store.Goto(navLast, 0);
	DWORD approxRowCount = store.m_pos;
	// Вычислим приблизительное значение log2 от количества строк,
	// как 32 - количество ведущих нулей в битах числа
	logRowCount = 32;
	DWORD r = approxRowCount;
	if(0 == (r & 0xFFFF0000)){logRowCount -= 16; r <<= 16;}
	if(0 == (r & 0xFF000000)){logRowCount -= 8; r <<= 8;}
	if(0 == (r & 0xF0000000)){logRowCount -= 4; r <<= 4;}
	if(0 == (r & 0xC0000000)){logRowCount -= 2;}
}

// Инфраструктура для построения цепочки классов-обработчиков
struct null_object{enum{val = -1};};
template<int I> struct prev_type{typedef prev_type<I-1>::type type;};
template<> struct prev_type<__LINE__>{ typedef null_object type; };

#define FIND_CLASS(className)\
struct className##_fd\
{\
	typedef prev_type<__LINE__ - 1>::type prev;\
	typedef className classType;\
	enum{val = prev::val + 1};\
};\
template<> struct prev_type<__LINE__> {typedef className##_fd type;}

struct first_string_hash
{
	CNoCaseMap<int> TypeNums;
	template<typename T>
	__forceinline first_string_hash(T* p)
	{
		Fill(p);
	}
	template<typename T>
	__forceinline void Fill(T*)
	{
		LPCSTR p1, p2;
		T::classType::GetFindInfo(p1, p2);
		TypeNums[p1] = T::val;
		TypeNums[p2] = T::val;
		Fill((T::prev*)NULL);
	}
	template<> void Fill<null_object>(null_object*){}
};

template<typename T>
__forceinline CVtabInfo* _GetObject(CStringArray& arrOfNames, CNoCaseMap<CVtabInfo*>& allObjects, const int iTypeNum, T* p=NULL)
{
	if(T::val == iTypeNum)
	{
		CString strName;
		typedef T::classType ct;
		if(!ct::GetNameFrom(arrOfNames, strName))
			return NULL;
		strName.Insert(0, ct::GetPrefix());
		CVtabInfo* pInfo;
		if(allObjects.Lookup(strName, pInfo))
			return pInfo;
		pInfo = new ct(arrOfNames);
		if(!pInfo->table())
		{
			static_cast<ct*>(pInfo)->Free();
			pInfo = NULL;
		}
		else
			allObjects[strName] = pInfo;
		return pInfo;
	}
	else
		return _GetObject(arrOfNames, allObjects, iTypeNum, (T::prev*)NULL);
}
template<>
__forceinline CVtabInfo* _GetObject<null_object>(CStringArray& arrOfNames, CNoCaseMap<CVtabInfo*>& allObjects, const int iTypeNum, null_object* p)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// НАЧАЛО ОПИСАНИЯ КЛАССОВ ОБЪЕКТОВ РАЗЛИЧНЫХ ТАБЛИЦ
//////////////////////////////////////////////////////////////////////////
FIND_CLASS(CReferenceTabInfo);
FIND_CLASS(CJournInfo);
FIND_CLASS(CDHTabInfo);
FIND_CLASS(CDTTabInfo);
FIND_CLASS(CRegTabInfo);
FIND_CLASS(CRegTotalsTabInfo);
FIND_CLASS(CCoreTabs);
FIND_CLASS(CCJTabInfo);
// КОНЕЦ ОПИСАНИЯ КЛАССОВ ОБЪЕКТОВ РАЗЛИЧНЫХ ТАБЛИЦ

typedef prev_type<__LINE__>::type lastFindType;

CVtabInfo* CVtabInfo::tabInfoForName(const CString& strUserName)
{
	static first_string_hash fsh((lastFindType*)NULL);
	CStringArray arrOfNames;
	SplitStr2Array(strUserName, arrOfNames);
	if(!arrOfNames.GetSize())
		return NULL;
	int iTypeNum;
	if(!fsh.TypeNums.Lookup(arrOfNames[0], iTypeNum))
		return NULL;
	return _GetObject<lastFindType>(arrOfNames, m_allTabs, iTypeNum);
}

void one_field::column(sqlite3_context* pCtx, const cursor_data& cursor, const CVtabInfo& table) const
{
	static CString val;
	if(isField())
	{
		const field_info& fi = table.phisInfo().field(position);
		memcpy(val.GetBufferSetLength(fi.length()), cursor.record.bufer() + fi.offsetInRecord(), fi.length());
		if(fi.type() == field_info::ftNumeric)
		{
			if(fi.precession())
				sqlite3_result_double(pCtx, atof(val));
			else
				sqlite3_result_int(pCtx, atol(val));
		}
		else
		{
			u8text::toUtf8(val);
			sqlite3_result_text(pCtx, val, val.GetLength(), SQLITE_TRANSIENT);
		}
	}
	else if(isIndex())
	{
		const index_info* pIndexInfo = table.phisInfo().index(position);
		LPSTR pWrite = val.GetBufferSetLength(pIndexInfo->keySize() + 10);
		const idx_field_info* pFieldInfo = pIndexInfo->fields();
		for(DWORD c = pIndexInfo->fieldsCount(); c--;)
		{
			int len = pFieldInfo->info().length();
			memcpy(pWrite, cursor.record.bufer() + pFieldInfo->info().offsetInRecord(), len);
			pWrite += len;
			pFieldInfo++;
		}
		pWrite += 9;
		long v = cursor.store.m_pos;
		for(c = 10; c--; pWrite--)
		{
			if(v)
			{
				*pWrite = (v % 10) + '0';
				v /= 10;
			}
			else
				*pWrite = ' ';
		}
		u8text::toUtf8(val);
		sqlite3_result_text(pCtx, val, val.GetLength(), SQLITE_TRANSIENT);
	}
	else if(isLnStr())
	{
		LongStrReader::get().ReadStr(cursor.record.bufer(), position, val);
		u8text::toUtf8(val);
		sqlite3_result_text(pCtx, val, val.GetLength(), SQLITE_TRANSIENT);
	}
	else
		sqlite3_result_null(pCtx);
}
