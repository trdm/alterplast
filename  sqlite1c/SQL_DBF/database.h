// database.h
#pragma once
#include "utex.h"
#include "metaparser.h"

class SQLiteQuery;

class SQLiteBase
{
public:
	SQLiteBase() : m_pDataBase(NULL)		{}
	virtual ~SQLiteBase()					{ close(); }
	sqlite3* base() const					{ return m_pDataBase; }
	void open(const CString& fileName);
	void close()
	{
		if(m_pDataBase)
		{
			sqlite3_close(m_pDataBase);
			m_pDataBase = NULL;
		}
	}
	SQLiteQuery* newQuery();
	void traceOn();
	const CString& traceResult() const		{ return m_trace; }
	void putVT(CValueTable* pTable, const CString& tableName, BOOL bAsPersistent);
	void putObjects(CValue* pObjects, const CString& tableName, BOOL bAsPersistent, const CString& hierarchy);
	void setQueryTimeout(int tm)
	{
		sqlite3_busy_timeout(m_pDataBase, tm);
	}

protected:

	void raiseDBError()
	{
		CString err(m_pDataBase ? u8text::fromUtf8(sqlite3_errmsg(m_pDataBase)) : "Нет базы");
		CBLModule::RaiseExtRuntimeError(err, FALSE);
	}
	friend SQLiteQuery;
	static int xCreate(sqlite3*, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVTab, char**);
	static int xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info*);
	static int xDestroy(sqlite3_vtab *pVTab);
	static int xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
	static int xClose(sqlite3_vtab_cursor*);
	static int xFilter(sqlite3_vtab_cursor*, int idxNum, const char *idxStr, int argc, sqlite3_value **argv);
	static int xNext(sqlite3_vtab_cursor*);
	static int xEof(sqlite3_vtab_cursor*);
	static int xColumn(sqlite3_vtab_cursor*, sqlite3_context*, int);
	static int xRowid(sqlite3_vtab_cursor*, sqlite3_int64 *pRowid);
	
	sqlite3* m_pDataBase;
	static CString m_trace;
};

enum typesOfFields
{
	ttAsIs,	
	ttString,
	ttDate,
	ttNumber,
	ttReference,
	ttReferenceOne,
	ttDocument,
	ttDocumentWithLink,
	ttDocumentOne,
	ttDocKind,
	ttDocPresent,
	ttEnum,
	ttEnumOne,
	ttAccount,
	ttAccountOne,
	ttUndefine,
	ttCalcKind,
	ttCalendar,
	ttSubconto,
	ttTime,
	ttKindName,
	ttKindPresent,
	ttSubcKind,
	ttSubcPresent,
	ttError,
};

typedef CArray<typesOfFields, typesOfFields> TFArray;

class SQLiteQuery
{
public:
	virtual ~SQLiteQuery()			{ close(); }
	SQLiteBase* parent() const		{ return m_pParent; }
	void prepare(const CString& query);
	void execute(CValue* pDst, CValue* pDstParam, CValue& retVal);
	void setSqlParam(const CValue& param, const CValue& value, int mod, BOOL bThrow = TRUE);
	void setTextParam(const CString& name, const CValue* pValue)
	{
		m_parser.setTextParam(name, pValue);
	}
	void close();
	void parseSqlText(CString& query)
	{
		m_parser.processSql(query);
	}
	void setDebug(BOOL bDebug)
	{
		m_bIsDebug = bDebug;
	}
	BOOL isPrepared() const	{ return m_pStmt != NULL; }
	BOOL needTransaction() const { return m_bNeedTransaction; }
	void setNeedTransaction(BOOL bNT) { m_bNeedTransaction = bNT; }
	
	void getFields(CStringArray& fields, TFArray& types);
	
	static void typeField(CString& name, CString* pType = NULL);

protected:

	friend SQLiteBase;
	SQLiteQuery(SQLiteBase* pBase)
		: m_pParent(pBase), m_pStmt(NULL), m_bIsDebug(FALSE), m_columns(NULL), m_bNeedTransaction(FALSE)
	{
	}
	
	struct column_info
	{
		column_info(const CString& colName, DWORD col, column_info*& pNext);
		void toValue(sqlite3_stmt* pStmt, CValue* pValue);

		CString name;
		DWORD column;
		CType typeForVTColumn;
		typesOfFields type;
		column_info* linkedField;
		column_info* next;
	} *m_columns;
	
	sqlite3_stmt* m_pStmt;
	SQLiteBase* m_pParent;
	MetaParser m_parser;
	BOOL m_bIsDebug;
	BOOL m_bNeedTransaction;
};

inline SQLiteQuery* SQLiteBase::newQuery() { return new SQLiteQuery(this); }


class CSLDataBase : public CContextImpl<CSLDataBase>, public SQLiteBase
{
public:
	BL_BEGIN_CONTEXT("SQLiteBase", "SQLiteBase");

	BL_PROC(Open, "Открыть", 1)				{ open(ppParams[0]->GetString()); return TRUE;}
	BL_FUNC(IsOpen, "Открыта", 0)			{ retVal = base() != NULL; return TRUE; }
	BL_FUNC(BaseHandle, "ОписательБазы", 0)	{ retVal = (long)m_pDataBase; return TRUE; }
	BL_PROC(DoProfile, "Замерять", 0)		{ traceOn(); return TRUE; }
	BL_FUNC(Profile, "Замер", 0)			{ retVal = m_trace; m_trace.Empty(); return TRUE; }
	BL_PROC(Close, "Закрыть", 0)
	{
		for(POSITION pos = m_querys.GetHeadPosition(); pos;)
		{
			CSLQuery* ptr = m_querys.GetNext(pos);
			ptr->close();
		}
		close();
		return TRUE;
	}

	BL_FUNC(NewQuery, "НовыйЗапрос", 0)
	{
		CSLQuery* pQuery = new CSLQuery(this);
		retVal.AssignContext(pQuery);
		pQuery->DecrRef();
		return TRUE;
	}

	BL_PROC_WITH_DEFVAL(PutVT, "УложитьТЗ", 3);
	BL_DEFVAL_FOR(PutVT)
	{
		if(2 == nParam)
		{
			if(pValue)
				*pValue = 0L;
			return TRUE;
		}
		return FALSE;
	}
	BL_PROC_WITH_DEFVAL(PutObjects, "УложитьОбъекты", 4)
	{
		putObjects(ppParams[0], ppParams[1]->GetString(), 0 != (long)ppParams[2]->GetNumeric(), ppParams[3]->GetString());
		return TRUE;
	}
	BL_DEFVAL_FOR(PutObjects)
	{
		if(nParam == 3)
		{
			if(pValue)
				pValue->Reset();
			return TRUE;
		}
		else if(nParam == 2)
		{
			if(pValue)
				*pValue = 0L;
			return TRUE;
		}
		return FALSE;
	}
	BL_PROC(SetQueryTimeout, "ТаймаутЗапроса", 1)
	{
		setQueryTimeout(ppParams[0]->GetNumeric());
		return TRUE;
	}
	BL_FUNC(FromUTF8, "ИзУТФ8", 1)
	{
		retVal.Reset();
		retVal.type = 2;
		u8text::fromUtf8(ppParams[0]->GetString(), retVal.m_String);
	}
	BL_END_CONTEXT();
private:
	class CSLQuery : public CContextImpl<CSLQuery, no_init_done, FALSE>, public SQLiteQuery
	{
	public:
		CSLQuery(CSLDataBase* parent) : SQLiteQuery(parent)
		{
			parent->IncrRef();
			m_posInParentList = parent->m_querys.AddTail(this);
		}
		~CSLQuery()
		{
			static_cast<CSLDataBase*>(m_pParent)->m_querys.RemoveAt(m_posInParentList);
			static_cast<CSLDataBase*>(m_pParent)->DecrRef();
		}

		BL_BEGIN_CONTEXT("SQLiteQuery", "SQLiteQuery");
		BL_FUNC_WITH_DEFVAL(ExecuteQuery, "ВыполнитьЗапрос", 3)
		{
			prepare(ppParams[0]->GetString());
			execute(ppParams[1], ppParams[2], retVal);
			close();
			return TRUE;
		}
		BL_DEFVAL_FOR(ExecuteQuery)
		{
			if(nParam > 0)
			{
				if(pValue)
					pValue->Reset();
				return TRUE;
			}
			return FALSE;
		}
		
		BL_PROC(Prepare, "Подготовить", 1)
		{
			prepare(ppParams[0]->GetString());
			return TRUE;
		}
		BL_PROC_WITH_DEFVAL(SetParam, "УстановитьПараметр", 3)
		{
			setSqlParam(*ppParams[0], *ppParams[1], ppParams[2]->GetNumeric());
			return TRUE;
		}
		BL_DEFVAL_FOR(SetParam)
		{
			if(2 == nParam)
			{
				if(pValue)
					pValue = 0L;
				return TRUE;
			}
			return FALSE;
		}
		BL_PROC(Substitute, "Подставлять", 2)
		{
			setTextParam(ppParams[0]->GetString(), ppParams[1]);
			return TRUE;
		}
		BL_FUNC_WITH_DEFVAL(Exec, "Выполнить", 2)
		{
			execute(ppParams[0], ppParams[1], retVal);
			return TRUE;
		}
		BL_DEFVAL_FOR(Exec)
		{
			if(pValue)
				pValue->Reset();
			return TRUE;
		}
		BL_PROC_WITH_DEFVAL(Debug, "Отладка", 1)
		{
			setDebug((long)ppParams[0]->GetNumeric() != 0);
			return TRUE;
		}
		BL_DEFVAL_FOR(Debug)
		{
			if(pValue)
				*pValue = 1;
			return TRUE;
		}
		BL_FUNC(ParseSqlText, "ОбработатьТекстЗапроса", 1)
		{
			CString text = ppParams[0]->GetString();
			parseSqlText(text);
			retVal = text;
			return TRUE;
		}
		BL_PROP_RW(NeedTransaction, "ВыполнятьВТранзакции")
		{
			value = needTransaction();
			return TRUE;
		}
		BL_PROP_WRITE(NeedTransaction)
		{
			setNeedTransaction(0 != (long)value.GetNumeric());
			return TRUE;
		}
		
		BL_END_CONTEXT();
		POSITION m_posInParentList;
	};
	friend CSLQuery;
	CList<CSLQuery*, CSLQuery*> m_querys;
};
