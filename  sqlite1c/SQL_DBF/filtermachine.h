// filtermachine.h
#pragma once

enum OpCodes
{
	stop,
	// Установить нужный индекс
	selectIdx,
	// Чтение аргументов
	getArgText,
	getArgNumeric,
	getArgLong,
	// Работа с условиями =
	setEqualString,
	setEqualNumeric,
	setEqualLong,
	
	cmpStringEqual_Equal,
	cmpNumericEqual_Equal,
	cmpLongEqual_Equal,

	// Работа с условиями >
	cmpString_Min_Equal,
	cmpNumeric_Min_Equal,
	cmpLong_Min_Equal,
	
	setMinString,
	setMinNumeric,
	setMinLong,
	
	selMinString,
	selMinNumeric,
	selMinLong,

	// Работа с условиями >=
	cmpString_MinEq_Equal,
	cmpNumeric_MinEq_Equal,
	cmpLong_MinEq_Equal,
	
	setMinEqString,
	setMinEqNumeric,
	setMinEqLong,
	
	selMinEqString,
	selMinEqNumeric,
	selMinEqLong,

	// Работа с условиями <
	cmpString_Max_Equal,
	cmpNumeric_Max_Equal,
	cmpLong_Max_Equal,

	cmpString_Max_Minimum,
	cmpNumeric_Max_Minimum,
	cmpLong_Max_Minimum,

	setMaxString,
	setMaxNumeric,
	setMaxLong,

	selMaxString,
	selMaxNumeric,
	selMaxLong,

	// Работа с условиями <=
	cmpString_MaxEq_Equal,
	cmpNumeric_MaxEq_Equal,
	cmpLong_MaxEq_Equal,

	cmpString_MaxEq_Minimum,
	cmpNumeric_MaxEq_Minimum,
	cmpLong_MaxEq_Minimum,

	setMaxEqString,
	setMaxEqNumeric,
	setMaxEqLong,

	selMaxEqString,
	selMaxEqNumeric,
	selMaxEqLong,
	
	// Когда обработаны все условия для одного поля индекса
	setFullKeyEqual,
	setFullKeyMinimum,
	setFullKeyMaximum,

	setPartKeyEqualText,
	setPartKeyEqualNumeric,
	setPartKeyEqualLong,

	setPartKeyMinimumText,
	setPartKeyMinimumNumeric,
	setPartKeyMinimumLong,
	
	setPartKeyMaximumText,
	setPartKeyMaximumNumeric,
	setPartKeyMaximumLong,

	// Запомнить номер записи при переходе по номеру записи
	storeRecNo,
	
	// Когда обработаны все условия для одного поля, не входящего в индекс
	addTestEqualText,
	addTestEqualNumeric,
	addTestEqualLong,
	addTestEqualRecNo,

	addTestMinimumText,
	addTestMinimumNumeric,
	addTestMinimumLong,
	addTestMinimumRecNo,

	addTestMaximumText,
	addTestMaximumNumeric,
	addTestMaximumLong,
	addTestMaximumRecNo,



//// Выбор стартового и конечного ключа
/*
Выбор наименьшего ключа.
Возможны варианты:
Задан мин-полный ключ
Задан строгий полный ключ
Задан мин-частичный ключ
Задан строгий частичный ключ
Задан мин-полный ключ и мин-частичный ключ
Задан мин-полный ключ и строгий частичный ключ
Задан строгий полный ключ и мин-частичный ключ
Задан строгий полный ключ и строгий частичный ключ.
Нужно сравнением ключей определить, что из этого будет использоваться как мин-ключ
Варианты ответа - мин-полный ключ, строгий-полный ключ, мин-частичный ключ, строгий частичный ключ.
*/
	// Установить минимальный ключ равный наименьшему полному
	setMinKey_FKMin,
	// Установить минимальный ключ равный строгому полному
	setMinKey_FKEqual,
	// Установить минимальный ключ равный наименьшему частичному
	setMinKey_PKMin,
	// Установить минимальный ключ равный строгому частичному
	setMinKey_PKEqual,
	// Выбрать минимальный ключ из наименьшего полного и наименьшего частичного
	selMinKey_FKMin_PKMin,
	// Выбрать минимальный ключ из наименьшего полного и строгого частичного
	selMinKey_FKMin_PKEqual,
	// Выбрать минимальный ключ из строгого полного и наименьшего частичного
	selMinKey_FKEqual_PKMin,
	// Выбрать минимальный ключ из строгого полного и строгого частичного
	selMinKey_FKEqual_PKEqual,
// Выбор наибольшего ключа - аналогично наименьшему.
	// Установить максимальный ключ равный максимальному полному
	setMaxKey_FKMax,
	// Установить максимальный ключ равный строгому полному
	setMaxKey_FKEqual,
	// Установить максимальный ключ равный максимальному частичному
	setMaxKey_PKMax,
	// Установить максимальный ключ равный строгому частичному
	setMaxKey_PKEqual,
	// Выбрать максимальный ключ из наибольшего полного и наибольшего частичного
	selMaxKey_FKMax_PKMax,
	// Выбрать максимальный ключ из наибольшего полного и строгого частичного
	selMaxKey_FKMax_PKEqual,
	// Выбрать максимальный ключ из строгого полного и наибольшего частичного
	selMaxKey_FKEqual_PKMax,
	// Выбрать максимальный ключ из строгого полного и строгого частичного
	selMaxKey_FKEqual_PKEqual,

// Сравнение макс и мин ключей.
// Если диапазон ограничен с обоих сторон, и хотя бы для одной границы был выбор из двух вариантов
// ключей, то надо проверить результирующие ключи, что они не дали пустой диапазон
// те мин ключ должен быть не больше максимального. Если с обоих границ диапазона использовались
// ключи одного типа, то такую проверку делать не надо, так как она делалается при построении
// ключей.
	cmpMinMaxKeys,

//// Выборка
	// Идти от начала до конца
	gotoFirstToLast,
	// Идти от начала до максимального ключа
	gotoFirstToMax,
	// Идти от минимального ключа до конца
	gotoMinToLast,
	// Идти от минимального ключа до конечного ключа
	gotoMinToMax,
	// Идти от конца до начала
	gotoLastToFirst,
	// Идти от конечного ключа до начала
	gotoMaxToFirst,
	// Идти от конца до минимального ключа
	gotoLastToMin,
	// Идти от конечного ключа до минимального ключа
	gotoMaxToMin,
	// Перейти по номеру записи
	gotoToRow,

	lastOpCode
};

struct cursor_data;
class select_iterator;
class CVtabInfo;

struct testConstraint
{
	virtual BOOL test(const cursor_data& cursor) = 0;
	virtual ~testConstraint() { delete next; }

	static BOOL doTest(testConstraint* ptr, const cursor_data& cursor)
	{
		while(ptr)
		{
			if(!ptr->test(cursor))
				return FALSE;
			ptr = ptr->next;
		}
		return TRUE;
	}
	testConstraint(testConstraint*& p) : next(p) {p = this;}
	static void append(testConstraint*& ret, testConstraint*& n1, testConstraint*& n2)
	{
		ret = n1;
		n1->next = n2;
		n2 = NULL;
		n1 = NULL;
	}
protected:
	testConstraint* next;
};


class FilterMachine
{
public:
	FilterMachine(const CVtabInfo& t, cursor_data& c)
		: m_pPKEqConstr(NULL), m_pPKMinConstr(NULL), m_pPKMaxConstr(NULL), m_pAdditionalTests(NULL),
		m_table(t), m_cursor(c)
	{
		m_hardCurMin =
		m_hardCurMax =
		m_hardFKMin  =
		m_hardFKMax  =
		m_hardPKMin  =
		m_hardPKMax  =
		m_hardTSMin  =
		m_hardTSMax  =
		m_minKType   =
		m_maxKType   = 0;
	}
	~FilterMachine()
	{
		delete m_pPKEqConstr;
		delete m_pPKMinConstr;
		delete m_pPKMaxConstr;
		delete m_pAdditionalTests;
	}
	
	select_iterator* process(const unsigned char* pByteCode, sqlite3_value** args);

	static void build(const class CVtabInfo& table, sqlite3_index_info* pIdx, const struct idx_node* pBest);
	static class select_iterator* parseFilter(struct cursor_data&, const class CVtabInfo&, const char*, sqlite3_value**);
protected:

	enum keySrc {keyFKEq, keyFKnEq, keyPKEq, keyPKnEq};

	select_iterator* GotoFirstToLast();
	select_iterator* GotoFirstToMax();
	select_iterator* GotoMinToLast();
	select_iterator* GotoMinToMax();
	select_iterator* GotoLastToFirst();
	select_iterator* GotoMaxToFirst();
	select_iterator* GotoLastToMin();
	select_iterator* GotoMaxToMin();
	select_iterator* GotoToRow();

	CString m_crntEqualText;
	CString m_crntMinimumText;
	CString m_crntMaximumText;

	double m_crntEqualNum;
	double m_crntMinimumNum;
	double m_crntMaximumNum;

	__int64 m_crntEqualLong;
	__int64 m_crntMinimumLong;
	__int64 m_crntMaximumLong;

	long m_recNo;
	
	CString m_fullKeyMin;
	CString m_fullKeyEq;
	CString m_fullKeyMax;
	CString m_partKeyMin;
	CString m_partKeyEq;
	CString m_partKeyMax;
	
	testConstraint* m_pPKEqConstr;
	testConstraint* m_pPKMinConstr;
	testConstraint* m_pPKMaxConstr;
	testConstraint* m_pAdditionalTests;

	const CVtabInfo& m_table;
	cursor_data& m_cursor;

	DWORD m_hardCurMin: 1;
	DWORD m_hardCurMax: 1;
	DWORD m_hardFKMin : 1;
	DWORD m_hardFKMax : 1;
	DWORD m_hardPKMin : 1;
	DWORD m_hardPKMax : 1;
	DWORD m_hardTSMin : 1;
	DWORD m_hardTSMax : 1;
	DWORD m_minKType  : 2;
	DWORD m_maxKType  : 2;

	const struct index_info* m_pIndexInfo;
	const struct idx_field_info* m_pFields;
	DWORD m_dwCurIdx;
};
