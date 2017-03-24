#ifndef RESULT_LOADER_H_INCLUDED
#define RESULT_LOADER_H_INCLUDED

struct sqlite3;

// Интерфейс для загрузки результата запроса в объект-приемник
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
	virtual DWORD init(DWORD colCountInResult, sqlite3*, CValue* pClear)
	{
		if(pClear->IsEmpty())
		{
			CValueItem** ppItems = (CValueItem**)m_pList->GetData();
			for(DWORD c = m_pList->GetSize(); c--;)
				delete *ppItems++;
			m_pList->RemoveAll();
		}
		return m_colCount = colCountInResult;
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
		DWORD s = m_pList->GetSize(), count = m_colCount;
		m_pList->SetSize(s + count);
		CValueItem** ppWriteValues = (CValueItem**)(m_pList->GetData()) + s;
		while(count--)
			*ppWriteValues++ = new CValueItem(**ppValues++, CString());

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

class IInterface
{
public:
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) { return E_NOINTERFACE; };
	ULONG __stdcall AddRef() { return 0; };
	ULONG __stdcall Release() { return 0; };
};

class IErrorDescription : public IInterface
{
public:
	// Описание ошибки
	// метод может быть вызван непосредственно после возникновения критической ошибки
	// возвращенный указатель действителен до следующего вызова любого метода объекта, реализующего IErrorDescription
	virtual LPCSTR GetLastError() throw() = 0;
};

// Коды и правила обработки ошибок:
// S_OK:
// - статус: нет ошибки
// - действия инициатора: продолжение передачи данных
// x >= S_FALSE:
// - статус: требование корректного завершения передачи данных
// - действия инициатора: прекращение передачи данных в штатном режиме
// FAILED(x) != FALSE
// - статус: требование аварийного завершения передачи данных (критическая ошибка)
// - действия инициатора: прекращение передачи данных в аварийном режиме, получение кода ошибки, выброс исключения

class IResultLoader : public IInterface, public IErrorDescription
{
public:
	// Подготовка приемника к приему результатов
	//
	// Источник передает в nColCount количество колонок в наборе данных (0 < nColCount)
	// Приемник возвращает в nColCount количество колонок, которое он готов принять (0 < nColCount_ret <= nColCount)
	//
	// Источник передает в nRowCount количество строк в наборе данных ( 0 <= nRowCount)
	// nRowCount == 0 означает, что количество строк в наборе данных не определено
	// Приемник возвращает в nRowCount количество строк, которое он готов принять (0 <= nRowCount_ret)
	// nRowCount_ret == 0 означает, что число строк, которые может принять приемник, не определено
	//
	// Если приемник не может принять данные, он возвращает код ошибки
	//
	// nRowCount используется только для оптимизации инициализации; реальное количество строк
	// источник и приемник контролируют самостоятельно, не полагаясь на другую сторону
	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw() = 0;

	// Завершение транзакции
	// гарантированно вызывается для транзации, открытой в Init
	// код ошибки должен быть S_OK
	virtual HRESULT ldrDone() throw() = 0;

	// Добавление колонки в результат запроса
	// szColName can be NULL (нет наименования), pType can be NULL (не задан тип)
	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw() = 0;

	// Добавление строки в результат запроса
	// параметр - массив указателей на CValue, количеством равный количеству добавленных колонок
	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw() = 0;
};

class IResultPublisher : public IInterface, public IErrorDescription
{
public:
	// Подготовка источника к публикации результатов
	//
	// Приемник передает в nColCount требуемое количество колонок в наборе данных (0 <= nColCount)
	// nColCount == 0 означает, что количество колонок, которое готов принять приемник, не определено
	// Источник возвращает в nColCount количество колонок, которое он готов передать (0 < nColCount_ret <= nColCount)
	//
	// Приемник передает в nRowCount требуемое количество строк в наборе данных ( 0 <= nRowCount)
	// nRowCount == 0 означает, что количество строк в наборе данных не определено
	// Источник возвращает в nRowCount количество строк, которое он готов передать (0 <= nRowCount_ret)
	// nRowCount == 0 означает, что число строк, которые может передать приемник, не определено
	//
	// Если источник не может передать данные, он возвращает код ошибки
	//
	// nRowCount используется только для оптимизации инициализации; реальное количество строк
	// источник и приемник контролируют самостоятельно, не полагаясь на другую сторону
	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw() = 0;

	// Завершение транзакции
	// гарантированно вызывается для транзации, открытой в Init
	// Код ошибки должен быть S_OK
	virtual HRESULT pubDone() throw() = 0;

	// Получение информации о колонке
	// szColName can be NULL (нет наименования), pType can be NULL (не задан тип)
	// (0 <= nIndex < nColCount_ret)
	virtual HRESULT pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw() = 0;

	// Получение данных
	// параметр - массив указателей на CValue, количеством равный количеству согласованных колонок (nColCount_ret)
	// Если источник не может передать данные, он возвращает код ошибки
	virtual HRESULT pubGetRow(CValue const* const*& ppValues) throw() = 0;
};

#endif
