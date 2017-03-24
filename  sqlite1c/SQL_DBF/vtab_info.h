// vtab_info.h
#pragma once

#include "phisicalinfo.h"
#include "strategycash.h"
#include "filtermachine.h"


struct record_bufer
{
	LPCSTR bufer() const {return _bufer;}

private:
	friend struct table_reader;
	LPSTR _bufer;
};

struct table_reader
{
private:
	friend class CVtabInfo;
	friend struct read_tran_guard;

	table_reader() : pFirstReaderBuffer(NULL) {}

	void connectReader(record_bufer& readerBuffer, CTableEx* pTable, LPCSTR pTableBuf, DWORD bufLen) const
	{
		if(!pFirstReaderBuffer) // Подключается первый читатель
		{
			// Запомним его буфер.
			pFirstReaderBuffer = &readerBuffer;
			// Назначим его буфер равным буферу таблицы, чтоб лишний раз не копировать.
			readerBuffer._bufer = (LPSTR)pTableBuf;
		}
		else
		{
			// Так как теперь читатель не один, то придется каждому копировать табличный буфер.
			if(pFirstReaderBuffer->_bufer == pTableBuf)	// Если первому читателю не создавали свой буфер, то надо создать
			{
				// Надо первому создать свою персональную копию буфера, и скопировать туда то,
				// что сейчас лежит в буфере таблицы.
				pFirstReaderBuffer->_bufer = new char[bufLen];
				memcpy(pFirstReaderBuffer->_bufer, pTableBuf, bufLen);
			}
			// И создать новому читателю персональный буфер
			readerBuffer._bufer = new char[bufLen];
		}
		// Теперь насчет транзакции
		if(!allReaderCount)	// Подключается самый первый читатель
		{
			if(!pDataDict->dw_48)	// Транзакция не начата
			{
				if(dbMode == dbDbfMono || bNeedTransaction)
					bWeOpenTransaction = pDataDict->BeginTransaction();
			}
		}
		allReaderCount++;
	}
	void disconnectReader(record_bufer& readerBuffer, CTableEx* pTable, LPCSTR pTableBuf) const
	{
		allReaderCount--;
		if(!allReaderCount && bWeOpenTransaction)
		{
			pDataDict->EndTransaction((TransactionCtl)0);
			bWeOpenTransaction = FALSE;
		}
		if(readerBuffer._bufer != pTableBuf)	// У этого читателя свой буфер, а не табличный
			delete [] readerBuffer._bufer;
		if(&readerBuffer == pFirstReaderBuffer)
			pFirstReaderBuffer = NULL;
	}

	void read(record_bufer& readerBuffer, LPCSTR pTableBuf, DWORD bufLen) const
	{
		if(readerBuffer._bufer != pTableBuf)	// У этого читателя свой буфер, а не табличный
			memcpy(readerBuffer._bufer, pTableBuf, bufLen);
	}

	mutable record_bufer* pFirstReaderBuffer;
	
	static DWORD allReaderCount;
	static BOOL bWeOpenTransaction;
	static BOOL bNeedTransaction;
};

struct read_tran_guard 
{
	read_tran_guard(BOOL doInTransaction)
	{
		table_reader::bNeedTransaction = doInTransaction;
	}
	~read_tran_guard()
	{
		table_reader::bNeedTransaction = FALSE;
		table_reader::allReaderCount = 0;
		if(table_reader::bWeOpenTransaction)
		{
			if(pDataDict->dw_48)
				pDataDict->EndTransaction((TransactionCtl)0);
			table_reader::bWeOpenTransaction = FALSE;
		}
	}
};


class CVtabInfo;

struct cursor_data
{
	cursor_data(CTableEx* pTable, LPCSTR pRB) : store(pTable, NULL), tableRecordBuf(pRB){}
	CStoreObj store;
	LPCSTR tableRecordBuf;
	record_bufer record;
};

struct one_field
{

	const CString& name() const {return m_name;}
	BOOL isRecNo() const {return type == fRecNo;}
	BOOL isField() const {return 0 != (type & (fField|fNumNegateField));}
	BOOL isIndex() const {return 0 != (type & fVirtIdx);}
	BOOL isLnStr() const {return 0 != (type & fLongStr);}
	DWORD pos() const {return position;}
	
	BOOL canUseFieldInIdx(TestOp op) const
	{
		if(op != toEqual && (type & fNumNegateField) != 0)
			return FALSE;
		return TRUE;
	}
	void column(sqlite3_context* pCtx, const cursor_data& cursor, const CVtabInfo& table) const;
private:
	friend class CVtabInfo;
	enum fType {fRecNo = 0, fField = 1, fNumNegateField = 2, fVirtIdx = 4, fLongStr = 8, } type;
	DWORD position;
	CString m_name;
};

class select_iterator
{
public:
	virtual ~select_iterator(){}
	virtual BOOL doNext(cursor_data& cursor, const CVtabInfo& table) = 0;
};


class CVtabInfo : public sqlite3_vtab
{
public:
	CTableEx* table()				const { return m_pTable; }
	const CString& tableName()		const { return m_strTableName; }
	const CString& textSqlCreate()	const { return m_strSqlCreate; }
	const phisical_info& phisInfo()	const { return *m_phisInfo; }
	const one_field& field(DWORD idx)const{ return m_pFields[idx]; }
	static const CString& lastError()	  { return m_lastError; }

	int bestIndex(sqlite3_index_info* pIdx) const;

	void beginRead(record_bufer& bufer) const
	{
		m_tableReader.connectReader(bufer, m_pTable, m_phisInfo->tableReadBufer(), m_phisInfo->recordSize());
	}
	void doneRead(record_bufer& bufer) const
	{
		m_tableReader.disconnectReader(bufer, m_pTable, m_phisInfo->tableReadBufer());
	}
	void storeRecord(cursor_data& data) const
	{
		m_tableReader.read(data.record, m_phisInfo->tableReadBufer(), m_phisInfo->recordSize());
	}
	
	int openCursor(sqlite3_vtab_cursor **ppCursor);
	// Получить экземпляр CVtabInfo по переданному пользовательскому имени
	// (Справочник.ТакойТо и тп)
	static CVtabInfo* tabInfoForName(const CString& strUserName);
	static void doneWork();

protected:
	CVtabInfo() : m_pTable(NULL), m_phisInfo(NULL), m_pFields(NULL)
	{
		zErrMsg = NULL;
	}
	virtual ~CVtabInfo()
	{
		delete m_phisInfo;
		if(m_pFields)
			delete [] (m_pFields - 1);
	}

	CVtabInfo(const CVtabInfo&) {}
	CVtabInfo& operator = (const CVtabInfo&) {}
	
	static void setError(LPCSTR strFormat, ...)
	{
		va_list arg;
		va_start(arg, strFormat);
		m_lastError.FormatV(strFormat, arg);
	}
	static void fillNamesFromObjs(CMetaDataObjArray* arr, CNoCaseMap<CString>& aliaces, CDWordArray* longStr = NULL);
	friend struct tab_deleter;

	
	void fillTabInfo(CNoCaseMap<CString>& aliaces, CDWordArray* longStr = NULL);

	static CNoCaseMap<CVtabInfo*> m_allTabs;
	static CString m_lastError;

	CString m_strTableName;
	CString m_strSqlCreate;
	CTableEx* m_pTable;
	
	phisical_info* m_phisInfo;
	one_field* m_pFields;
	table_reader m_tableReader;
	mutable StrategyCash stratCash;
	DWORD logRowCount;
};

class CursorImpl : public sqlite3_vtab_cursor
{
public:
	BOOL IsEof() {return m_pIterator == NULL; }
	
	int RowID(sqlite3_int64 *pRowid)
	{
		if(m_pIterator)
		{
			*pRowid = m_data.store.m_pos;
			return SQLITE_OK;
		}
		return SQLITE_ERROR;
	}
	int Next()
	{
		if(m_pIterator)
		{
			if(m_pIterator->doNext(m_data, table()))
				table().storeRecord(m_data);
			else
			{
				delete m_pIterator;
				m_pIterator = NULL;
				table().doneRead(m_data.record);
			}
		}
		return SQLITE_OK;
	}
	void Close()
	{
		delete this;
	}
	
	int Filter(int idxNum, const char *str, int, sqlite3_value **argv)
	{
		table().beginRead(m_data.record);

		if(m_pIterator = FilterMachine::parseFilter(m_data, table(), str, argv))
			table().storeRecord(m_data);
		else
			table().doneRead(m_data.record);
		
		return SQLITE_OK;
	}
	int Column(sqlite3_context* pCtx, int nCol)
	{
		table().field(nCol).column(pCtx, m_data, table());
		return SQLITE_OK;
	}

protected:
	friend CVtabInfo;
	CursorImpl(CVtabInfo* pTab) : m_data(pTab->table(), pTab->phisInfo().tableReadBufer()), m_pIterator(NULL)
	{
		pVtab = pTab;
	}
	~CursorImpl()
	{
		if(m_pIterator)
		{
			// Нас закрывают, не прокачав до конца, может там limit стояло
			table().doneRead(m_data.record);
			delete m_pIterator;
		}
	}
	const CVtabInfo& table() {return *static_cast<CVtabInfo*>(pVtab);}
	cursor_data m_data;
	select_iterator* m_pIterator;
};

inline int CVtabInfo::openCursor(sqlite3_vtab_cursor **ppCursor)
{
	*ppCursor = new CursorImpl(this);
	return SQLITE_OK;	
}


#define FIND_INFO(p1, p2, p3)\
static void GetFindInfo(LPCSTR& n1, LPCSTR& n2){n1 = p1; n2=p2;}\
static LPCSTR GetPrefix() {return p3;}\
void Free(){delete this;}


