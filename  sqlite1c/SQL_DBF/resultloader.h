// resultloader.h
// Интерфейс для загрузки результата запроса в объект-приемник
struct sqlite3;

class ISQLiteResultLoader
{
public:
	virtual ~ISQLiteResultLoader(){}
	// Подготовка приемника к приему результатов
	// Должен возвращать количество колонок, которое готов принять приемник.
	virtual DWORD init(DWORD colCountInResult, sqlite3* pBase, CValue* pParam) = 0;
	// Добавление колонки в результат запроса
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type) = 0;
	// Добавление строки в результат запроса.
	// Параметр - массив указателей на CValue, количеством равный количеству
	// добавленных колонок
	virtual void addValues(CValue** ppValues) = 0;
	// Присвоение возвращаемого значения
	virtual void assignRetValue(CValue& retVal) = 0;
};

class IVTResultLoader : public ISQLiteResultLoader
{
public:
	IVTResultLoader() : m_pCont(NULL) {}
	~IVTResultLoader() { if(m_pCont) m_pCont->DecrRef(); }

	// Подготовка приемника к приему результатов
	virtual DWORD init(DWORD colCountInResult, sqlite3*, CValue* pClear)
	{
		if(pClear->IsEmpty())
		{
			m_pTable->Clear(TRUE);
			return colCountInResult;
		}
		else if(1 == (long)pClear->GetNumeric())
			m_pTable->Clear(FALSE);
		DWORD colInTables = m_pTable->GetColumnCount();
		return colInTables < colCountInResult ? colInTables : colCountInResult;
	}
	// Добавление колонки в результат запроса
	virtual void setColumn(DWORD colIdx, const CString& colName, const CType& type)
	{
		if(m_pTable->GetColumnCount() <= colIdx)
			m_pTable->AddColumn(colName, type, colName, 0, CString(), 0);
	}
	// Добавление строки в результат запроса.
	// Параметр - массив указателей на CValue, количеством равный количеству
	// добавленных колонок
	virtual void addValues(CValue** ppValues)
	{
		m_pTable->AddRow(ppValues, -1);
	}
	// Присвоение возвращаемого значения
	virtual void assignRetValue(CValue& retVal)
	{
		retVal.AssignContext(m_pCont);
	}
	void setContext(CBLContext* pCont)
	{
		m_pCont = pCont;
		m_pCont->IncrRef();
		m_pTable = ((CValueTableContextData*)pCont->GetInternalData())->GetValueTable();
	}
protected:
	CBLContext* m_pCont;
	CValueTable* m_pTable;
};

class IVLResultLoader : public ISQLiteResultLoader
{
public:
	IVLResultLoader() : m_pCont(NULL) {}
	~IVLResultLoader()	{ if(m_pCont) m_pCont->DecrRef(); }
	virtual DWORD init(DWORD colCountInResult, sqlite3*, CValue* pParam)
	{
		int param = 0;
		if(!pParam->IsEmpty() && pParam->type == 1)
			param = (long)pParam->m_Number;
		if(0 == (param & 1))	// Очистить список
		{
			CValueItem** ppItems = (CValueItem**)m_pList->GetData();
			for(DWORD c = m_pList->GetSize(); c--;)
				delete *ppItems++;
			m_pList->RemoveAll();
		}
		if(param & 2)
		{
			m_putMode = putAllInValue;
			m_colCount = colCountInResult < 3 ? colCountInResult : 3;
		}
		else
		{
			m_putMode = putAllInValue;
			m_colCount = colCountInResult;
		}
		return m_colCount;
	}
	// Добавление колонки в результат запроса
	virtual void setColumn(DWORD, const CString&, const CType&)
	{
	}
	// Добавление строки в результат запроса.
	// Параметр - массив указателей на CValue, количеством равный количеству
	// добавленных колонок
	virtual void addValues(CValue** ppValues)
	{
		if(putAllInList == m_putMode)
		{
			DWORD s = m_pList->GetSize(), count = m_colCount;
			m_pList->SetSize(s + count);
			CValueItem** ppWriteValues = (CValueItem**)(m_pList->GetData()) + s;
			while(count--)
				*ppWriteValues++ = new CValueItem(**ppValues++, CString());
		}
		else
		{
			CValueItem *pItem = new CValueItem(*ppValues[0], CString());
			if(m_colCount > 1)
			{
				pItem->m_text = ppValues[1]->GetString();
				if(m_colCount > 2)
					pItem->m_bCheck = 0 != (long)ppValues[2]->GetNumeric();
			}
			m_pList->Add(pItem);
		}
	}
	// Присвоение возвращаемого значения
	virtual void assignRetValue(CValue& retVal)
	{
		retVal.AssignContext(m_pCont);
	}
	
	void setContext(CBLContext* pCont)
	{
		m_pCont = pCont;
		m_pCont->IncrRef();
		m_pList = *(CPtrArray**)(((char*)m_pCont) + 0x30);
	}
protected:
	CBLContext* m_pCont;
	CPtrArray* m_pList;
	DWORD m_colCount;
	enum {putAllInList, putAllInValue } m_putMode;
};

class IScalarResultLoader : public ISQLiteResultLoader
{
public:
	virtual DWORD init(DWORD, sqlite3*, CValue*)
	{
		return m_column + 1;
	}
	// Добавление колонки в результат запроса
	virtual void setColumn(DWORD, const CString&, const CType&)
	{
	}
	// Добавление строки в результат запроса.
	// Параметр - массив указателей на CValue, количеством равный количеству
	// добавленных колонок
	virtual void addValues(CValue** ppValues)
	{
		m_value = *ppValues[m_column];
	}
	// Присвоение возвращаемого значения
	virtual void assignRetValue(CValue& retVal)
	{
		retVal = m_value;
	}
	void setCol(DWORD col) {m_column = col;}
protected:
	DWORD m_column;
	CValue m_value;
};

