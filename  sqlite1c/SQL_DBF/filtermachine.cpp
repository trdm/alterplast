// filtermachine.cpp
#include "StdAfx.h"
#include "filtermachine.h"
#include "vtab_info.h"
#include "index_selector.h"
#include "utex.h"
#include "longstrreader.h"

static CNumeric staticNumValue;

inline void writeNum(unsigned char*& ptr, WORD val)
{
	*(WORD*)ptr = val;
	ptr += 2;
}

inline DWORD readNum(const unsigned char*& ptr)
{
	WORD val = *(WORD*)ptr;
	ptr += 2;
	return val;
}

struct text_compare
{
	static int compare(LPCSTR str, LPCSTR pFieldBuf, DWORD dwFieldLen)
	{
		return u8text::compareLen(str, pFieldBuf, dwFieldLen);
	}
};

struct text_nocase_compare
{
	static int compare(LPCSTR str, LPCSTR pFieldBuf, DWORD dwFieldLen)
	{
		return u8text::compareNoCaseLen(str, pFieldBuf, dwFieldLen);
	}
};

struct numeric_compare
{
	static int compare(LPCSTR pNum, LPCSTR pFieldBuf, DWORD dwFieldLen)
	{
		BOOL bothNegate = FALSE;
		while(dwFieldLen--)
		{
			int numSymb = static_cast<DWORD>(static_cast<BYTE>(*pNum));
			int valSymb = static_cast<DWORD>(static_cast<BYTE>(*pFieldBuf));
			int dif = numSymb - valSymb;
			
			if(dif)
			{
				if('-' == numSymb)		// Первое число отрицательно, и значит меньше
					return -1;
				else if('-' == valSymb) // Второе число отрицательно, значит первое больше
					return 1;
				else
					return bothNegate ? -dif : dif;
			}
			else if('-' == numSymb)
				bothNegate = TRUE;
			pNum++, pFieldBuf++;
		}
		return 0;
	}
};

template<TestOp Op, typename T>
struct tcTestField : public testConstraint
{
	tcTestField(testConstraint*& p, const CString& t, const field_info& fi)
		: testConstraint(p), m_value(t), m_fOffset(fi.offsetInRecord()),
		m_fLen(fi.length()){}

	CString m_value;
	int m_fOffset, m_fLen;
	
	virtual BOOL test(const cursor_data& cursor)
	{
		int res = T::compare(m_value, cursor.tableRecordBuf + m_fOffset, m_fLen);
		
		if(Op == toEqual)
		{
			if(0 == res)
				return TRUE;
		}
		else if(Op == toLess)
		{
			if(res > 0)
				return TRUE;
		}
		else if(Op == toLessEq)
		{
			if(res >= 0)
				return TRUE;
		}
		else if(Op == toGrat)
		{
			if(res < 0)
				return TRUE;
		}
		else if(Op == toGratEq)
		{
			if(res <= 0)
				return TRUE;
		}
		return FALSE;
	}
};

template<TestOp op>
struct tcFullKey : public testConstraint
{
	tcFullKey(testConstraint*& p, CString& key, const index_info* pIndexInfo)
		: testConstraint(p), recNum(0), m_pIndexInfo(pIndexInfo)
	{
		int keyLen = key.GetLength(), keySize = m_pIndexInfo->keySize();
		if(keyLen > keySize)
			recNum = atol((LPCSTR)key + keySize);
		else if(keyLen < keySize)
			memset(key.GetBufferSetLength(keySize) + keyLen, ' ', keySize - keyLen);
		
		DWORD countFields = m_pIndexInfo->fieldsCount();
		keys.SetSize(countFields);
		LPCSTR pRead = key;
		const idx_field_info* pFields = m_pIndexInfo->fields();
		CString* pString = keys.GetData();
		for(DWORD i = countFields; i-- ; )
		{
			const field_info& info = pFields->info();
			memcpy(pString->GetBufferSetLength(info.length()), pRead, info.length());
			pRead += info.length();
			pString++;
			pFields++;
		}
	}
	
	CStringArray keys;
	int recNum;
	const index_info* m_pIndexInfo;

	virtual BOOL test(const cursor_data& cursor)
	{
		int size = m_pIndexInfo->fieldsCount();
		const idx_field_info* pFields = m_pIndexInfo->fields();
		CString* pString = keys.GetData();
		while(size--)
		{
			const field_info& field = pFields->info();
			int res;
			res = u8text::compareNoCaseLen(*pString, cursor.tableRecordBuf + field.offsetInRecord(), field.length());
			switch(op)
			{
			case toEqual:
				if(0 != res)
					return FALSE;
				break;
			case toLess:
				if(res > 0)
					return TRUE;
				else if(res < 0)
					return FALSE;
				else if(size == 0 && recNum == 0)	// Подключ равен в последнем поле, номер записи не задан
					return FALSE;
				break;
			case toLessEq:
				if(res > 0)
					return TRUE;
				else if(res < 0)
					return FALSE;
				break;
			case toGrat:
				if(res < 0)
					return TRUE;
				else if(res > 0)
					return FALSE;
				else if(size == 0 && recNum == 0)	// Подключ равен в последнем поле, номер записи не задан
					return FALSE;
				break;
			case toGratEq:
				if(res < 0)
					return TRUE;
				else if(res > 0)
					return FALSE;
				break;
			}
			pFields++;
			pString++;
		}
		// Прошли все поля индекса, то бишь он полностью равен ключу.
		// Поэтому если номер записи не задан, то все хорошо.
		if(0 == recNum)
			return TRUE;
		switch(op)
		{
		case toEqual:
			return recNum == cursor.store.m_pos;
		case toLess:
			return recNum > cursor.store.m_pos;
		case toLessEq:
			return recNum >= cursor.store.m_pos;
		case toGrat:
			return recNum < cursor.store.m_pos;
		case toGratEq:
			return recNum <= cursor.store.m_pos;
		}
		return FALSE;
	}
};

template<TestOp Op>
struct tcLongStr : public testConstraint
{
	tcLongStr(testConstraint*& p, const CString& value, DWORD mdID)
		: testConstraint(p), m_value(value), m_mdID(mdID){}

	DWORD m_mdID;
	CString m_value;

	virtual BOOL test(const cursor_data& cursor)
	{
		static CString value;
		LongStrReader::get().ReadStr(cursor.tableRecordBuf, m_mdID, value);
		int res = u8text::compareRtrimNoCase(m_value, value);
		if(Op == toEqual)
		{
			if(0 == res)
				return TRUE;
		}
		else if(Op == toLess)
		{
			if(res > 0)
				return TRUE;
		}
		else if(Op == toLessEq)
		{
			if(res >= 0)
				return TRUE;
		}
		else if(Op == toGrat)
		{
			if(res < 0)
				return TRUE;
		}
		else if(Op == toGratEq)
		{
			if(res <= 0)
				return TRUE;
		}
		return FALSE;
	}
};

template<TestOp Op>
struct tcRecNo : public testConstraint
{
	tcRecNo(testConstraint*& p, long value)
		: testConstraint(p), m_value(value){}

	long m_value;

	virtual BOOL test(const cursor_data& cursor)
	{
		long res = m_value - cursor.store.m_pos;
		if(Op == toEqual)
		{
			if(0 == res)
				return TRUE;
		}
		else if(Op == toLess)
		{
			if(res > 0)
				return TRUE;
		}
		else if(Op == toLessEq)
		{
			if(res >= 0)
				return TRUE;
		}
		else if(Op == toGrat)
		{
			if(res < 0)
				return TRUE;
		}
		else if(Op == toGratEq)
		{
			if(res <= 0)
				return TRUE;
		}
		return FALSE;
	}
};

inline int nextGratStringUpper(CString& text, DWORD fLen)
{
	extern BYTE nextGratUSymbols[256];
	unsigned char *pStart = (unsigned char*)text.GetBufferSetLength(fLen), *pWrite = pStart + fLen - 1;
	while(pWrite >= pStart)
	{
		BYTE ngs = nextGratUSymbols[static_cast<DWORD>(*pWrite)];
		if(ngs)
		{
			*pWrite = ngs;
			return 1;
		}
		else
			*pWrite-- = ' ';
	}
	return 2; // Не удалось получить большую строку ("ЯЯЯЯЯ");
}

// >0 - полный ключ БОЛЬШЕ полный ключ МЕНЬШЕ
// ВАЖНО. Считаем, что полный ключ всегда длиннее частного ключа,
// алгоритм рассчитывает на это, не подведите его.
int compareFk_Pk(const CString& fullKey, const CString& partKey, DWORD fillSymbol)
{
	LPCSTR pFull = fullKey, pPart = partKey;
	int sFull, sPart;
	
	for(;;)
	{
		sFull = static_cast<DWORD>(static_cast<BYTE>(*pFull));
		sPart = static_cast<DWORD>(static_cast<BYTE>(*pPart));

		if(!sPart)
			return fillSymbol == 0xFF ? -1 : 1;
		int dif = sFull - sPart;
		if(dif)
			return dif;
		pFull++;
		pPart++;
	}
	return 0;
}

template<int BoundType> // 0 - equal, 1 - min, 2 - max
struct normalizeString
{
	// 0 - success, 1 - trimmed, 2 - error
	inline static int set(CString& str, DWORD keyLength)
	{
		DWORD strLength = str.GetLength();

		if(strLength > keyLength)	// Строка длиннее нужного размера. Попробуем обрезать ее справа.
		{
			str.TrimRight();
			strLength = str.GetLength();
		}
		if(strLength > keyLength)
		{
			if(0 == BoundType)
				return 2; // Строка длиннее поля. Равенство невозможно.
			else if(1 == BoundType)
			{
				// Замена условия. Допустим для поля длинной 1 применяется условие >'ad' или >='ad'
				// Понятно, что оба условия надо изменить на >='b'.
				// Также понятно, что для такого поля условие >='яА' никогда не выполняется.
				// Вобщем, усекаем строку до нужной длины, получаем следующую бОльшую строку,
				// и убираем признак hard у границы.
				str.GetBufferSetLength(keyLength);
				return nextGratStringUpper(str, keyLength);
			}
			else if(2 == BoundType)
			{
				// Замена условия. Допустим для поля длинной 1 применяется условие <'ad' или <='ad'
				// Понятно, что оба условия надо изменить на <='a'.
				// Вобщем, усекаем строку до нужной длины и убираем признак hard у границы.
				str.GetBufferSetLength(keyLength);
				return 1;
			}
		}
		if(strLength < keyLength)
			memset(str.GetBufferSetLength(keyLength) + strLength, ' ', keyLength - strLength);
		return 0;
	}
};

template<int BoundType, BOOL bAllowNegate, BOOL bForIndex>
struct normalizeNumeric
{
	// 0 - все нормально, 1 - сменить границу на нестрогую, 2 - невыполнимо, 3 - выполнимо всегда
	inline static int set(double dblVal, const field_info& info, CString& strValue)
	{
		// AllowNegate и bForIndex допустимо только для равенства
		char testParam[bAllowNegate && bForIndex && BoundType != 0 ? -1 : 1] = {0};
		DWORD lenOfField = info.length(), precOfField = info.precession();

		BOOL bNegate = dblVal < 0;
		
		if(!bAllowNegate && bNegate)
		{
			if(1 == BoundType)
			{
				// Для поля индекса в котором "неотрицательные", условие > аргумент,
				// а аргумент отрицательный. Условие выполняется всегда
				if(bForIndex)
				{
					memset(strValue.GetBufferSetLength(lenOfField), ' ', lenOfField);
					return 0;
				}
				return 3;
			}
			else
				return 2;
		}
		staticNumValue = dblVal;
		staticNumValue.Round(precOfField);
		
		char buffer[60];
		LPSTR pStart = buffer + 30;
		char* p = staticNumValue.Convert(pStart, lenOfField + 2, precOfField);
		int lenOfStr = strlen(pStart);

		if(!bAllowNegate)
		{
			// Тут могут быть тока положительные числа. Для них в 1С длина всегда должна быть
			// не более ДлинаПоля-1 символов.
			if(lenOfStr >= lenOfField)	// Переполнение.
			{
				if(0 == BoundType)
					return 2;		// Равенство
				else if(1 == BoundType)
					return 2;		// Минимум
				else if(2 == BoundType)	// Максимум
				{
					// <todo>
					if(bForIndex)
					{
						LPSTR ptr = strValue.GetBufferSetLength(lenOfField);
						memset(ptr, '9', lenOfField);
						*ptr = ' ';
						ptr[lenOfField - precOfField - 1] = '.';
						return 1;
					}
					return 3;
				}
			}
		}
		else
		{
			// Возможны случаи.
			if(lenOfStr > lenOfField)	// Переполнение
			{
				if(0 == BoundType)
					return 2;

				if(bNegate)	// Отрицательное переполнение.
				{
					if(1 == BoundType)
						return 3;		// Для поля numeric(4.1) наименьшее число -9.9 а условие > -99.1
					else if(2 == BoundType)
						return 2;
				}
				else // Положительное переполнение
				{
					if(1 == BoundType)
						return 2;
					else if(2 == BoundType)
						return 3;
				}
			}
			else if(lenOfStr == lenOfField)	// Число уместилось в длину поля, но может быть отрицательным
			{
				if(!bNegate)	// Число положительно, значит переполнение
				{
					if(0 == BoundType)		// Поле не может быть равно числу, длиннее поля
						return 2;
					else if(1 == BoundType) // Поле не может быть больше числа длиннее поля
						return 2;
					else if(2 == BoundType)	// Поле всегда меньше числа длиннее поля
						return 3;
				}
			}
		}
		while(lenOfStr < lenOfField)
		{
			*--pStart = ' ';
			lenOfStr++;
		}
		memcpy(strValue.GetBufferSetLength(lenOfField), pStart, lenOfField);
		return 0;
	}
};

template<int BoundType, BOOL bAllowNegate, BOOL bForIndex>
struct normalizeLong
{
	// 0 - все нормально, 1 - сменить границу на нестрогую, 2 - невыполнимо, 3 - выполнимо всегда
	inline static int set(__int64& numValue, const field_info& info, CString& strValue)
	{
		// AllowNegate и bForIndex допустимо только для равенства
		char testParam[bAllowNegate && bForIndex && BoundType != 0 ? -1 : 1] = {0};

		DWORD lenOfField = info.length();
		if(!bAllowNegate && numValue < 0)
		{
			if(1 == BoundType)
			{
				if(bForIndex)
				{
					memset(strValue.GetBufferSetLength(lenOfField), ' ', lenOfField);
					return 0;
				}
				return 3;
			}
			return 2;
		}

		if(lenOfField == 1)
		{
			if(numValue >=0 && numValue <= 10)
			{
				strValue.GetBufferSetLength(1)[0] = numValue + '0';
				return 0;
			}
			if(0 == BoundType)
				return 2;
			else if(1 == BoundType)
				return numValue < 0 ? 3 : 2;
			else if(2 == BoundType)
				return numValue > 0 ? 3 : 2;
		}
		
		int lenOfStr = 0;
		char buffer[60];
		LPSTR pStart = buffer + 60;

		__int64 val = numValue < 0 ? -numValue : numValue;
		do
		{
			*--pStart = (val % 10) + '0';
			lenOfStr++;
			val /= 10;
		}while(val && lenOfStr < lenOfField);

		if(!bAllowNegate)
		{
			// Тут могут быть тока положительные числа. Для них в 1С длина всегда должна быть
			// не более ДлинаПоля-1 символов, кроме полей длинной 1 (isfolder и тп)
			if(lenOfStr == lenOfField)	// Переполнение.
			{
				if(0 == BoundType)
					return 2;		// Равенство
				else if(1 == BoundType)
					return 2;		// Минимум
				else if(2 == BoundType)	// Максимум
				{
					// <todo>
					if(bForIndex)
					{
						LPSTR ptr = strValue.GetBufferSetLength(lenOfField);
						memset(ptr, '9', lenOfField);
						*ptr = ' ';
						return 1;
					}
					return 3;
				}
			}
		}
		else
		{
			// Возможны случаи.
			if(lenOfStr == lenOfField)
			{
				if(0 == BoundType)
					return 2;
				else if(1 == BoundType)
				{
					if(numValue < 0)
						return 3;
					else
						return 2;
				}
				else if(2 == BoundType)
				{
					if(numValue < 0)
						return 2;
					else
						return 3;
				}
			}
		}
		if(numValue < 0)
		{
			*--pStart = '-';
			lenOfStr++;
		}
		lenOfStr = lenOfField - lenOfStr;

		while(lenOfStr)
		{
			*--pStart = ' ';
			lenOfStr--;
		}
		memcpy(strValue.GetBufferSetLength(lenOfField), pStart, lenOfField);
		return 0;
	}
};

template<int BoundType>
struct addTestText
{
	inline static BOOL add(const unsigned char*& pByteCode, CString& value, const CVtabInfo& table, DWORD bHard, testConstraint*& pC)
	{
		DWORD dwField = readNum(pByteCode);
		const one_field& field = table.field(dwField);
		
		if(field.isField())
		{
			const field_info& fi = table.phisInfo().field(field.pos());
			DWORD len = fi.length();
			int res = normalizeString<BoundType>::set(value, len);
			if(res == 2)
				return FALSE;
			else if(res == 1)
				bHard = FALSE;
			if(0 == BoundType)
				new tcTestField<toEqual, text_nocase_compare>(pC, value, fi);
			else if(1 == BoundType)
			{
				if(bHard)
					new tcTestField<toGrat, text_nocase_compare>(pC, value, fi);
				else
					new tcTestField<toGratEq, text_nocase_compare>(pC, value, fi);
			}
			else if(2 == BoundType)
			{
				if(bHard)
					new tcTestField<toLess, text_nocase_compare>(pC, value, fi);
				else
					new tcTestField<toLessEq, text_nocase_compare>(pC, value, fi);
			}
		}
		else if(field.isIndex())
		{
			const index_info* pIndexInfo = table.phisInfo().index(field.pos());
			DWORD len = pIndexInfo->keySize() + 10;
			int res = normalizeString<BoundType>::set(value, len);
			if(res == 2)
				return FALSE;
			else if(res == 1)
				bHard = 0;
			if(0 == BoundType)
			{
				new tcFullKey<toEqual>(pC, value, pIndexInfo);
			}
			else if(1 == BoundType)
			{
				if(bHard)
					new tcFullKey<toGrat>(pC, value, pIndexInfo);
				else
					new tcFullKey<toGratEq>(pC, value, pIndexInfo);
			}
			else if(2 == BoundType)
			{
				if(bHard)
					new tcFullKey<toLess>(pC, value, pIndexInfo);
				else
					new tcFullKey<toLessEq>(pC, value, pIndexInfo);
			}
		}
		else if(field.isLnStr())
		{
			DWORD mdID = field.pos();
			if(0 == BoundType)
			{
				new tcLongStr<toEqual>(pC, value, mdID);
			}
			else if(1 == BoundType)
			{
				if(bHard)
					new tcLongStr<toGrat>(pC, value, mdID);
				else
					new tcLongStr<toGratEq>(pC, value, mdID);
			}
			else if(2 == BoundType)
			{
				if(bHard)
					new tcLongStr<toLess>(pC, value, mdID);
				else
					new tcLongStr<toLessEq>(pC, value, mdID);
			}
		}
		return TRUE;
	}
};

template<int BoundType>
struct addTestNumeric
{
	inline static BOOL add(const unsigned char*& pByteCode, double value, const CVtabInfo& table, DWORD bHard, testConstraint*& pC)
	{
		static CString strValue;
		DWORD dwField = readNum(pByteCode);
		const field_info& field = table.phisInfo().field(dwField);
		int res = normalizeNumeric<BoundType, TRUE, FALSE>::set(value, field, strValue);
		if(res == 3)
			return TRUE;	// Условие всегда выполнимо
		else if(res == 2)	// Условие не выполнимо
			return FALSE;
		else if(res == 1)
			bHard = 0;
		if(0 == BoundType)
		{
			new tcTestField<toEqual, numeric_compare>(pC, strValue, field);
		}
		else if(1 == BoundType)
		{
			if(bHard)
				new tcTestField<toGrat, numeric_compare>(pC, strValue, field);
			else
				new tcTestField<toGratEq, numeric_compare>(pC, strValue, field);
		}
		else if(2 == BoundType)
		{
			if(bHard)
				new tcTestField<toLess, numeric_compare>(pC, strValue, field);
			else
				new tcTestField<toLessEq, numeric_compare>(pC, strValue, field);
		}
		return TRUE;
	}
};
 
template<int BoundType>
struct addTestLong
{
	inline static BOOL add(const unsigned char*& pByteCode, __int64 value, const CVtabInfo& table, DWORD bHard, testConstraint*& pC)
	{
		static CString strValue;
		DWORD dwField = readNum(pByteCode);
		const field_info& field = table.phisInfo().field(dwField);
		int res = normalizeLong<BoundType, TRUE, FALSE>::set(value, field, strValue);
		if(res == 3)
			return TRUE;	// Условие всегда выполнимо
		else if(res == 2)	// Условие не выполнимо
			return FALSE;
		else if(res == 1)
			bHard = 0;
		if(0 == BoundType)
		{
			new tcTestField<toEqual, numeric_compare>(pC, strValue, field);
		}
		else if(1 == BoundType)
		{
			if(bHard)
				new tcTestField<toGrat, numeric_compare>(pC, strValue, field);
			else
				new tcTestField<toGratEq, numeric_compare>(pC, strValue, field);
		}
		else if(2 == BoundType)
		{
			if(bHard)
				new tcTestField<toLess, numeric_compare>(pC, strValue, field);
			else
				new tcTestField<toLessEq, numeric_compare>(pC, strValue, field);
		}
		return TRUE;
	}
};
 

select_iterator* FilterMachine::process(const unsigned char* pByteCode, sqlite3_value** args)
{
	static CString argString;
	static double argNumeric;
	static __int64 argLong;

	for(;;)
	{
		int res;
		switch(static_cast<DWORD>(*pByteCode++))
		{
		case selectIdx:
			m_dwCurIdx = readNum(pByteCode);
			m_pIndexInfo = m_table.phisInfo().index(m_dwCurIdx);
			m_pFields = m_pIndexInfo->fields();
			m_cursor.store.SetOrderIndex(m_table.table()->index(m_dwCurIdx));
			break;
		case getArgText:
			if(SQLITE_TEXT != sqlite3_value_type(*args))
				return NULL;
			u8text::fromUtf8((LPCSTR)sqlite3_value_text(*args++), argString);
			u8text::dbUpper(argString);
			break;
		case getArgNumeric:
			if(SQLITE_NULL == sqlite3_value_type(*args))
				return NULL;
			argNumeric = sqlite3_value_double(*args++);
			break;
		case getArgLong:
			if(SQLITE_NULL == sqlite3_value_type(*args))
				return NULL;
			argLong = sqlite3_value_int64(*args++);
			break;
		
		case setEqualString:
			m_crntEqualText = argString;
			break;
		case setEqualNumeric:
			m_crntEqualNum = argNumeric;
			break;
		case setEqualLong:
			m_crntEqualLong = argLong;
			break;
		
		case cmpStringEqual_Equal:
			if(0 != u8text::compareRtrim(m_crntEqualText, argString))
				return NULL;
			break;
		case cmpNumericEqual_Equal:
			if(m_crntEqualNum != argNumeric)
				return NULL;
			break;
		case cmpLongEqual_Equal:
			if(m_crntEqualLong != argLong)
				return NULL;
			break;
	// Работа с условиями >
		case cmpString_Min_Equal:
			if(u8text::compareRtrim(m_crntEqualText, argString) <= 0)
				return NULL;
			break;
		case cmpNumeric_Min_Equal:
			if(m_crntEqualNum <= argNumeric)
				return NULL;
			break;
		case cmpLong_Min_Equal:
			if(m_crntEqualLong <= argLong)
				return NULL;
			break;
	
		case setMinString:
			m_crntMinimumText = argString;
			m_hardCurMin = 1;
			break;
		case setMinNumeric:
			m_crntMinimumNum = argNumeric;
			m_hardCurMin = 1;
			break;
		case setMinLong:
			m_crntMinimumLong = argLong;
			m_hardCurMin = 1;
			break;
	
		case selMinString:
			if(u8text::compareRtrim(m_crntMinimumText, argString) <= 0)
			{
				m_crntMinimumText = argString;
				m_hardCurMin = 1;
			}
			break;
		case selMinNumeric:
			if(m_crntMinimumNum <= argNumeric)
			{
				m_crntMinimumNum = argNumeric;
				m_hardCurMin = 1;
			}
			break;
		case selMinLong:
			if(m_crntMinimumLong <= argLong)
			{
				m_crntMinimumLong = argLong;
				m_hardCurMin = 1;
			}
			break;
	// Работа с условиями >=
		case cmpString_MinEq_Equal:
			if(u8text::compareRtrim(m_crntEqualText, argString) < 0)
				return NULL;
			break;
		case cmpNumeric_MinEq_Equal:
			if(m_crntEqualNum < argNumeric)
				return NULL;
			break;
		case cmpLong_MinEq_Equal:
			if(m_crntEqualLong < argLong)
				return NULL;
			break;
	
		case setMinEqString:
			m_crntMinimumText = argString;
			m_hardCurMin = 0;
			break;
		case setMinEqNumeric:
			m_crntMinimumNum = argNumeric;
			m_hardCurMin = 0;
			break;
		case setMinEqLong:
			m_crntMinimumLong = argLong;
			m_hardCurMin = 0;
			break;
	
		case selMinEqString:
			if(u8text::compareRtrim(m_crntMinimumText, argString) < 0)
			{
				m_crntMinimumText = argString;
				m_hardCurMin = 0;
			}
			break;
		case selMinEqNumeric:
			if(m_crntMinimumNum < argNumeric)
			{
				m_crntMinimumNum = argNumeric;
				m_hardCurMin = 0;
			}
			break;
		case selMinEqLong:
			if(m_crntMinimumLong < argLong)
			{
				m_crntMinimumLong = argLong;
				m_hardCurMin = 0;
			}
			break;
		// Работа с условиями <
		case cmpString_Max_Equal:
			if(u8text::compareRtrim(m_crntEqualText, argString) >= 0)
				return NULL;
			break;
		case cmpNumeric_Max_Equal:
			if(m_crntEqualNum >= argNumeric)
				return NULL;
			break;
		case cmpLong_Max_Equal:
			if(m_crntEqualLong >= argLong)
				return NULL;
			break;

		case cmpString_Max_Minimum:
			if(u8text::compareRtrim(m_crntMinimumText, argString) >= 0)
				return NULL;
			break;
		case cmpNumeric_Max_Minimum:
			if(m_crntEqualNum >= argNumeric)
				return NULL;
			break;
		case cmpLong_Max_Minimum:
			if(m_crntMinimumLong >= argLong)
				return NULL;
			break;

		case setMaxString:
			m_crntMaximumText = argString;
			m_hardCurMax = 1;
			break;
		case setMaxNumeric:
			m_crntMaximumNum = argNumeric;
			m_hardCurMax = 1;
			break;
		case setMaxLong:
			m_crntMaximumLong = argLong;
			m_hardCurMax = 1;
			break;

		case selMaxString:
			if(u8text::compareRtrim(m_crntMaximumText, argString) >= 0)
			{
				m_crntMaximumText = argString;
				m_hardCurMax = 1;
			}
			break;
		case selMaxNumeric:
			if(m_crntMaximumNum >= argNumeric)
			{
				m_crntMaximumNum = argNumeric;
				m_hardCurMax = 1;
			}
			break;
		case selMaxLong:
			if(m_crntMaximumLong >= argLong)
			{
				m_crntMaximumLong = argLong;
				m_hardCurMax = 1;
			}
			break;
		// Работа с условиями <=
		case cmpString_MaxEq_Equal:
			if(u8text::compareRtrim(m_crntEqualText, argString) > 0)
				return NULL;
			break;
		case cmpNumeric_MaxEq_Equal:
			if(m_crntEqualNum > argNumeric)
				return NULL;
			break;
		case cmpLong_MaxEq_Equal:
			if(m_crntEqualLong > argLong)
				return NULL;
			break;

		case cmpString_MaxEq_Minimum:
			res = u8text::compareRtrim(m_crntMinimumText, argString);
			if(res > 0 || (res == 0 && m_hardCurMin != 0))
				return NULL;
			break;
		case cmpNumeric_MaxEq_Minimum:
			if(m_crntMinimumNum > argNumeric || (m_crntMinimumNum == argNumeric && m_hardCurMin != 0))
				return NULL;
			break;
		case cmpLong_MaxEq_Minimum:
			if(m_crntMinimumLong > argLong || (m_crntMinimumLong == argLong && m_hardCurMin != 0))
				return NULL;
			break;

		case setMaxEqString:
			m_crntMaximumText = argString;
			m_hardCurMax = 0;
			break;
		case setMaxEqNumeric:
			m_crntMaximumNum = argNumeric;
			m_hardCurMax = 0;
			break;
		case setMaxEqLong:
			m_crntMaximumLong = argLong;
			m_hardCurMax = 0;
			break;

		case selMaxEqString:
			if(u8text::compareRtrim(m_crntMaximumText, argString) > 0)
			{
				m_crntMaximumText = argString;
				m_hardCurMax = 0;
			}
			break;
		case selMaxEqNumeric:
			if(m_crntMaximumNum > argNumeric)
			{
				m_crntMaximumNum = argNumeric;
				m_hardCurMax = 0;
			}
			break;
		case selMaxEqLong:
			if(m_crntMaximumLong > argLong)
			{
				m_crntMaximumLong = argLong;
				m_hardCurMax = 0;
			}
			break;

		// Работа с полными ключами.	
		case setFullKeyEqual:
			if(2 == normalizeString<0>::set(m_crntEqualText, m_pIndexInfo->keySize() + 10))
				return NULL;
			m_fullKeyEq = m_crntEqualText;
			break;
		case setFullKeyMinimum:
			res = normalizeString<1>::set(m_crntMinimumText, m_pIndexInfo->keySize() + 10);
			if(2 == res)
				return NULL;
			m_fullKeyMin = m_crntMinimumText;
			m_hardFKMin = 0 == res ? m_hardCurMin : 0;
			break;
		case setFullKeyMaximum:
			res = normalizeString<2>::set(m_crntMaximumText, m_pIndexInfo->keySize() + 10);
			m_fullKeyMax = m_crntMaximumText;
			m_hardFKMax = 0 == res ? m_hardCurMax : 0;
			break;
		
		case setPartKeyEqualText:
			res = normalizeString<0>::set(m_crntEqualText, m_pFields->info().length());
			if(2 == res)
				return NULL;
			new tcTestField<toEqual, text_nocase_compare>(m_pPKEqConstr, m_crntEqualText, m_pFields->info());
			m_partKeyEq += m_crntEqualText;
			m_pFields++;
			break;
		case setPartKeyEqualNumeric:
			res = normalizeNumeric<0, TRUE, TRUE>::set(m_crntEqualNum, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			new tcTestField<toEqual, text_compare>(m_pPKEqConstr, argString, m_pFields->info());
			m_partKeyEq += argString;
			m_pFields++;
			break;
		case setPartKeyEqualLong:
			res = normalizeLong<0, TRUE, TRUE>::set(m_crntEqualLong, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			new tcTestField<toEqual, text_compare>(m_pPKEqConstr, argString, m_pFields->info());
			m_partKeyEq += argString;
			m_pFields++;
			break;

		case setPartKeyMinimumText:
			res = normalizeString<1>::set(m_crntMinimumText, m_pFields->info().length());
			if(2 == res)
				return NULL;
			m_partKeyMin = m_partKeyEq + m_crntMinimumText;
			m_hardPKMin = 0 == res ? m_hardCurMin : 0;
			if(m_hardPKMin)
				new tcTestField<toGrat, text_nocase_compare>(m_pPKMinConstr, m_crntMinimumText, m_pFields->info());
			else
				new tcTestField<toGratEq, text_nocase_compare>(m_pPKMinConstr, m_crntMinimumText, m_pFields->info());
			break;
		case setPartKeyMinimumNumeric:
			res = normalizeNumeric<1, FALSE, TRUE>::set(m_crntMinimumNum, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			m_hardPKMin = 0 == res ? m_hardCurMin : 0;
			if(m_hardPKMin)
				new tcTestField<toGrat, text_compare>(m_pPKMinConstr, argString, m_pFields->info());
			else
				new tcTestField<toGratEq, text_compare>(m_pPKMinConstr, argString, m_pFields->info());
			m_partKeyMin = m_partKeyEq + argString;
			break;
		case setPartKeyMinimumLong:
			res = normalizeLong<1, FALSE, TRUE>::set(m_crntMinimumLong, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			m_hardPKMin = 0 == res ? m_hardCurMin : 0;
			if(m_hardPKMin)
				new tcTestField<toGrat, text_compare>(m_pPKMinConstr, argString, m_pFields->info());
			else
				new tcTestField<toGratEq, text_compare>(m_pPKMinConstr, argString, m_pFields->info());
			m_partKeyMin = m_partKeyEq + argString;
			break;
		
		case setPartKeyMaximumText:
			res = normalizeString<2>::set(m_crntMaximumText, m_pFields->info().length());
			m_partKeyMax = m_partKeyEq + m_crntMaximumText;
			m_hardPKMax = 0 == res ? m_hardCurMax : 0;
			if(m_hardPKMax)
				new tcTestField<toLess, text_nocase_compare>(m_pPKMaxConstr, m_crntMaximumText, m_pFields->info());
			else
				new tcTestField<toLessEq, text_nocase_compare>(m_pPKMaxConstr, m_crntMaximumText, m_pFields->info());
			break;
		case setPartKeyMaximumNumeric:
			res = normalizeNumeric<2, FALSE, TRUE>::set(m_crntMaximumNum, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			m_hardPKMax = 0 == res ? m_hardCurMax : 0;
			if(m_hardPKMax)
				new tcTestField<toLess, text_compare>(m_pPKMaxConstr, argString, m_pFields->info());
			else
				new tcTestField<toLessEq, text_compare>(m_pPKMaxConstr, argString, m_pFields->info());
			m_partKeyMax = m_partKeyEq + argString;
			break;
		case setPartKeyMaximumLong:
			res = normalizeLong<2, FALSE, TRUE>::set(m_crntMaximumLong, m_pFields->info(), argString);
			if(2 == res)
				return NULL;
			m_hardPKMax = 0 == res ? m_hardCurMax : 0;
			if(m_hardPKMax)
				new tcTestField<toLess, text_compare>(m_pPKMaxConstr, argString, m_pFields->info());
			else
				new tcTestField<toLessEq, text_compare>(m_pPKMaxConstr, argString, m_pFields->info());
			m_partKeyMax = m_partKeyEq + argString;
			break;
		case storeRecNo:
			m_recNo = m_crntEqualLong;
			break;

		case addTestEqualText:
			if(!addTestText<0>::add(pByteCode, m_crntEqualText, m_table, 0, m_pAdditionalTests))
				return NULL;
			break;
		case addTestEqualNumeric:
			if(!addTestNumeric<0>::add(pByteCode, m_crntEqualNum, m_table, 0, m_pAdditionalTests))
				return NULL;
			break;
		case addTestEqualLong:
			if(!addTestLong<0>::add(pByteCode, m_crntEqualLong, m_table, 0, m_pAdditionalTests))
				return NULL;
			break;
		case addTestEqualRecNo:
			if(m_crntEqualLong <= 0 || m_crntEqualLong > 0x7FFFFFFF)
				return NULL;
			new tcRecNo<toEqual>(m_pAdditionalTests, m_crntEqualLong);
			break;

		case addTestMinimumText:
			if(!addTestText<1>::add(pByteCode, m_crntMinimumText, m_table, m_hardCurMin, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMinimumNumeric:
			if(!addTestNumeric<1>::add(pByteCode, m_crntMinimumNum, m_table, m_hardCurMin, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMinimumLong:
			if(!addTestLong<1>::add(pByteCode, m_crntMinimumLong, m_table, m_hardCurMin, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMinimumRecNo:
			if(m_crntMinimumLong > 0x7FFFFFFF)
				return NULL;
			if(m_crntMinimumLong > 0)
			{
				if(m_hardCurMin)
					new tcRecNo<toGrat>(m_pAdditionalTests, m_crntMinimumLong);
				else
					new tcRecNo<toGratEq>(m_pAdditionalTests, m_crntMinimumLong);
			}
			break;

		case addTestMaximumText:
			if(!addTestText<2>::add(pByteCode, m_crntMaximumText, m_table, m_hardCurMax, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMaximumNumeric:
			if(!addTestNumeric<2>::add(pByteCode, m_crntMaximumNum, m_table, m_hardCurMax, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMaximumLong:
			if(!addTestNumeric<2>::add(pByteCode, m_crntMaximumLong, m_table, m_hardCurMax, m_pAdditionalTests))
				return NULL;
			break;
		case addTestMaximumRecNo:
			if(m_crntMaximumLong <= 0)
				return NULL;
			if(m_crntMaximumLong < 0x7FFFFFFF)
			{
				if(m_hardCurMax)
					new tcRecNo<toLess>(m_pAdditionalTests, m_crntMaximumLong);
				else
					new tcRecNo<toLessEq>(m_pAdditionalTests, m_crntMaximumLong);
			}
			break;
	
	//// Выбор стартового и конечного ключа
		// Установить минимальный ключ равный наименьшему полному
		case setMinKey_FKMin:
			m_minKType = keyFKnEq;
			break;
		// Установить минимальный ключ равный строгому полному
		case setMinKey_FKEqual:
			m_minKType = keyFKEq;
			break;
		// Установить минимальный ключ равный наименьшему частичному
		case setMinKey_PKMin:
			m_minKType = keyPKnEq;
			break;
		// Установить минимальный ключ равный строгому частичному
		case setMinKey_PKEqual:
			m_minKType = keyPKEq;
			break;
		// Выбрать минимальный ключ из наименьшего полного и наименьшего частичного
		case selMinKey_FKMin_PKMin:
			res = compareFk_Pk(m_fullKeyMin, m_partKeyMin, m_hardPKMin ? 0xFF : 0x1);
			m_minKType = res > 0 ? keyFKnEq : keyPKnEq;
			break;
		// Выбрать минимальный ключ из наименьшего полного и строгого частичного
		case selMinKey_FKMin_PKEqual:
			res = compareFk_Pk(m_fullKeyMin, m_partKeyEq, 0x1);
			m_minKType = res > 0 ? keyFKnEq : keyPKEq;
			break;
		// Выбрать минимальный ключ из строгого полного и наименьшего частичного
		case selMinKey_FKEqual_PKMin:
			res = compareFk_Pk(m_fullKeyEq, m_partKeyMin, m_hardPKMin ? 0xFF : 0x1);
			m_minKType = res > 0 ? keyFKEq : keyPKnEq;
			break;
		// Выбрать минимальный ключ из строгого полного и строгого частичного
		case selMinKey_FKEqual_PKEqual:
			res = compareFk_Pk(m_fullKeyEq, m_partKeyEq, 0x1);
			m_minKType = res > 0 ? keyFKEq : keyPKEq;
			break;
		// Установить максимальный ключ равный максимальному полному
		case setMaxKey_FKMax:
			m_maxKType = keyFKnEq;
			break;
		// Установить максимальный ключ равный строгому полному
		case setMaxKey_FKEqual:
			m_maxKType = keyFKEq;
			break;
		// Установить максимальный ключ равный максимальному частичному
		case setMaxKey_PKMax:
			m_maxKType = keyPKnEq;
			break;
		// Установить максимальный ключ равный строгому частичному
		case setMaxKey_PKEqual:
			m_maxKType = keyPKEq;
			break;
		// Выбрать максимальный ключ из наибольшего полного и наибольшего частичного
		case selMaxKey_FKMax_PKMax:
			res = compareFk_Pk(m_fullKeyMax, m_partKeyMax, m_hardPKMax ? 0x1 : 0xFF);
			m_maxKType = res > 0 ? keyPKnEq : keyFKnEq;
			break;
		// Выбрать максимальный ключ из наибольшего полного и строгого частичного
		case selMaxKey_FKMax_PKEqual:
			res = compareFk_Pk(m_fullKeyMax, m_partKeyEq, 0xFF);
			m_maxKType = res > 0 ? keyPKEq : keyFKnEq;
			break;
		// Выбрать максимальный ключ из строгого полного и наибольшего частичного
		case selMaxKey_FKEqual_PKMax:
			res = compareFk_Pk(m_fullKeyEq, m_partKeyMax, m_hardPKMax ? 0x1 : 0xFF);
			m_maxKType = res > 0 ? keyPKnEq : keyFKEq;
			break;
		// Выбрать максимальный ключ из строгого полного и строгого частичного
		case selMaxKey_FKEqual_PKEqual:
			res = compareFk_Pk(m_fullKeyEq, m_partKeyEq, 0xFF);
			m_maxKType = res > 0 ? keyPKEq : keyFKEq;
			break;
		// Сравнение макс и мин ключей.
		case cmpMinMaxKeys:
			{
				switch(m_minKType | (m_maxKType << 2))
				{
				// Снизу ограничено минимальным полным ключом, сверху строгим частичным.	
				case keyFKnEq | (keyPKEq << 2):
					res = compareFk_Pk(m_fullKeyMin, m_partKeyEq, 0xFF);
					if(res > 0 || (res == 0 && m_hardFKMin))
						return NULL;
					break;
				// Снизу ограниченно минимальным полным ключом, сверху максимальным частичным
				case keyFKnEq | (keyPKnEq << 2):
					res = compareFk_Pk(m_fullKeyMin, m_partKeyMax, m_hardPKMax ? 0x1 : 0xFF);
					if(res > 0 || (res == 0 && (m_hardFKMin || m_hardPKMax)))
						return NULL;
					break;
				// Снизу ограничено строгим полным ключом, сверху строгим частным
				case keyFKEq | (keyPKEq << 2):
					res = compareFk_Pk(m_fullKeyEq, m_partKeyEq, 0xFF);
					if(res > 0)
						return NULL;
					break;
				// Снизу ограничено строгим полным ключом, сверху максимальным частным	
				case keyFKEq | (keyPKnEq << 2):
					res = compareFk_Pk(m_fullKeyEq, m_partKeyMax, m_hardPKMax ? 0x1 : 0xFF);
					if(res > 0 || (res == 0 && m_hardPKMax))
						return NULL;
					break;
				// Снизу ограничено минимальным частичным ключом, сверху строгим полным	
				case keyPKnEq | (keyFKEq << 2):
					res = compareFk_Pk(m_fullKeyEq, m_partKeyMin, m_hardPKMin ? 0xFF : 0x1);
					if(res < 0 || (res == 0 && m_hardPKMin))
						return NULL;
					break;
				// Снизу ограничено минимальным частичным ключом, сверху максимальным полным	
				case keyPKnEq | (keyFKnEq << 2):
					res = compareFk_Pk(m_fullKeyMax, m_partKeyMin, m_hardPKMin ? 0xFF : 0x1);
					if(res < 0 || (res == 0 && (m_hardPKMin || m_hardFKMax)))
						return NULL;
					break;
				// Снизу ограничено строгим частичным ключом, сверху строгим полным	
				case keyPKEq | (keyFKEq << 2):
					res = compareFk_Pk(m_fullKeyEq, m_partKeyEq, 0x1);
					if(res < 0)
						return NULL;
					break;
				// Снизу ограничено строгим частным ключом, сверху максимальным полным
				case keyPKEq | (keyFKnEq << 2):
					res = compareFk_Pk(m_fullKeyMax, m_partKeyEq, 0x1);
					if(res < 0 || (res == 0 && m_hardFKMax))
						return NULL;
					break;
				}
			}
			break;
		// Идти от начала до конца
		case gotoFirstToLast:
			return GotoFirstToLast();
		// Идти от начала до максимального ключа
		case gotoFirstToMax:
			return GotoFirstToMax();
		// Идти от минимального ключа до конца
		case gotoMinToLast:
			return GotoMinToLast();
		// Идти от минимального ключа до конечного ключа
		case gotoMinToMax:
			return GotoMinToMax();
		// Идти от конца до начала
		case gotoLastToFirst:
			return GotoLastToFirst();
		// Идти от конечного ключа до начала
		case gotoMaxToFirst:
			return GotoMaxToFirst();
		// Идти от конца до минимального ключа
		case gotoLastToMin:
			return GotoLastToMin();
		// Идти от конечного ключа до минимального ключа
		case gotoMaxToMin:
			return GotoMaxToMin();
		case gotoToRow:
			return GotoToRow();
		default:
			return NULL;
		}
	}
}

void byteCodeToStr(BYTE* pStart, BYTE* pEnd, CString& res)
{
	int len = pEnd - pStart;
	int newLen = (len + 6) / 7 * 8 + 7;
	LPSTR ptr = res.GetBufferSetLength(newLen);
	wsprintf(ptr, "%6i ", len);
	ptr += 7;
	while(pStart < pEnd)
	{
		*ptr++ = ' ' + ( pStart[0] & 0x7F);
		*ptr++ = ' ' + ((pStart[0]>>7) | ((pStart[1] & 0x3F)<<1));
		*ptr++ = ' ' + ((pStart[1]>>6) | ((pStart[2] & 0x1F)<<2));
		*ptr++ = ' ' + ((pStart[2]>>5) | ((pStart[3] & 0xF) <<3));
		*ptr++ = ' ' + ((pStart[3]>>4) | ((pStart[4] & 0x7) <<4));
		*ptr++ = ' ' + ((pStart[4]>>3) | ((pStart[5] & 0x3) <<5));
		*ptr++ = ' ' + ((pStart[5]>>2) | ((pStart[6] & 0x1) <<6));
		*ptr++ = ' ' + ( pStart[6]>>1);
		pStart += 7;
	}
	res.GetBufferSetLength(newLen);
}

BYTE* strToByteCode(LPCSTR pStr)
{
	int lenOfByteCode = atol(pStr), i = lenOfByteCode;
	BYTE* pB = (BYTE*)pStr + 7;
	BYTE *pBuf = new BYTE[(lenOfByteCode + 6) / 7 * 7], *ptr = pBuf;
	while(i > 0)
	{
		*ptr++ =  (pB[0] - 32)       | ((pB[1] - 32) << 7);
		*ptr++ = ((pB[1] - 32) >> 1) | ((pB[2] - 32) << 6);
		*ptr++ = ((pB[2] - 32) >> 2) | ((pB[3] - 32) << 5);
		*ptr++ = ((pB[3] - 32) >> 3) | ((pB[4] - 32) << 4);
		*ptr++ = ((pB[4] - 32) >> 4) | ((pB[5] - 32) << 3);
		*ptr++ = ((pB[5] - 32) >> 5) | ((pB[6] - 32) << 2);
		*ptr++ = ((pB[6] - 32) >> 6) | ((pB[7] - 32) << 1);
		pB += 8;
		i-=7;
	}
	return pBuf;
}

template<OpCodes op1, OpCodes op2, OpCodes op3>
struct typed_op 
{
	static void set(int t, unsigned char*& pByteCode)
	{
		switch(t)
		{
		case field_info::ftText:
			*pByteCode++ = op1;
			break;
		case field_info::ftNumeric:
			*pByteCode++ = op2;
			break;
		case field_info::ftNumeric + 1:
			*pByteCode++ = op3;
			break;
		}
	}
};

inline static void writeFieldOp(unsigned char*& pByteCode, int& argNum, usage_ptr pUsg, int t, TestOp op, BOOL& bHasEq, BOOL& bHasMin, BOOL& bHasMax)
{
	pUsg->argvIndex = argNum++;
	pUsg->omit = TRUE;
	typed_op<getArgText, getArgNumeric, getArgLong>::set(t, pByteCode);

	switch(op)
	{
	case toEqual:
		if(bHasEq)
			typed_op<cmpStringEqual_Equal, cmpNumericEqual_Equal, cmpLongEqual_Equal>::set(t, pByteCode);
		else
		{
			typed_op<setEqualString, setEqualNumeric, setEqualLong>::set(t, pByteCode);
			bHasEq = TRUE;
		}
		break;
	case toLess:
		if(bHasEq)
			typed_op<cmpString_Max_Equal, cmpNumeric_Max_Equal, cmpLong_Max_Equal>::set(t, pByteCode);
		else
		{
			if(bHasMin)
				typed_op<cmpString_Max_Minimum, cmpNumeric_Max_Minimum, cmpLong_Max_Minimum>::set(t, pByteCode);
			if(bHasMax)
				typed_op<selMaxString, selMaxNumeric, selMaxLong>::set(t, pByteCode);
			else
			{
				typed_op<setMaxString, setMaxNumeric, setMaxLong>::set(t, pByteCode);
				bHasMax = TRUE;
			}
		}
		break;
	case toLessEq:
		if(bHasEq)
			typed_op<cmpString_MaxEq_Equal, cmpNumeric_MaxEq_Equal, cmpLong_MaxEq_Equal>::set(t, pByteCode);
		else
		{
			if(bHasMin)
				typed_op<cmpString_MaxEq_Minimum, cmpNumeric_MaxEq_Minimum, cmpLong_MaxEq_Minimum>::set(t, pByteCode);
			if(bHasMax)
				typed_op<selMaxEqString, selMaxEqNumeric, selMaxEqLong>::set(t, pByteCode);
			else
			{
				typed_op<setMaxEqString, setMaxEqNumeric, setMaxEqLong>::set(t, pByteCode);
				bHasMax = TRUE;
			}
		}
		break;
	case toGrat:
		if(bHasEq)
			typed_op<cmpString_Min_Equal, cmpNumeric_Min_Equal, cmpLong_Min_Equal>::set(t, pByteCode);
		else
		{
			if(bHasMin)
				typed_op<selMinString, selMinNumeric, selMinLong>::set(t, pByteCode);
			else
			{
				typed_op<setMinString, setMinNumeric, setMinLong>::set(t, pByteCode);
				bHasMin = TRUE;
			}
		}
		break;
	case toGratEq:
		if(bHasEq)
			typed_op<cmpString_MinEq_Equal, cmpNumeric_MinEq_Equal, cmpLong_MinEq_Equal>::set(t, pByteCode);
		else
		{
			if(bHasMin)
				typed_op<selMinEqString, selMinEqNumeric, selMinEqLong>::set(t, pByteCode);
			else
			{
				typed_op<setMinEqString, setMinEqNumeric, setMinEqLong>::set(t, pByteCode);
				bHasMin = TRUE;
			}
		}
		break;
	}
}

void buildAdditionalTests(unsigned char*& pByteCode, sqlite3_index_info* pIdx, int argNum, const CVtabInfo& table)
{
	struct node
	{
		int nField;
		TestOp op;
		usage_ptr pUsage;
		node* next;
		static void insert(node*& first, node* pNode)
		{
			node** ppNode = &first;
			while(*ppNode)
			{
				if((*ppNode)->nField > pNode->nField)
					break;
				else if((*ppNode)->nField == pNode->nField && (*ppNode)->op >= pNode->op)
					break;
				ppNode = &(*ppNode)->next;
			}
			pNode->next = *ppNode;
			*ppNode = pNode;
		}
	};
	// Для начала упорядочим остатки по номерам полей и сравнениям.
	node* nodes = NULL;
	constraint_ptr pCtr = pIdx->aConstraint;
	usage_ptr pUsg = pIdx->aConstraintUsage;
	
	for(DWORD i = pIdx->nConstraint ; i--; pCtr++, pUsg++)
	{
		if(isConstraint(pCtr) && !pUsg->argvIndex)
		{
			node* pNode = new node;
			pNode->nField = pCtr->iColumn;
			pNode->pUsage = pUsg;
			pNode->op = index_selector::sqlite2TestOp(pCtr->op);
			node::insert(nodes, pNode);
		}
	}
	node* pNode = nodes;
	
	BOOL bHasMin = FALSE, bHasMax = FALSE, bHasEqual = FALSE;
	
	while(pNode)
	{
		// Определим тип работы с полем
		int t;
		const one_field& field = table.field(pNode->nField);
		
		if(field.isField())
		{
			t = table.phisInfo().field(field.pos()).type();
			if(t == field_info::ftNumeric && table.phisInfo().field(field.pos()).precession() == 0)
				t = field_info::ftNumeric + 1;
		}
		else if(field.isIndex() || field.isLnStr())
			t = field_info::ftText;
		else if(field.isRecNo())
			t = field_info::ftNumeric + 1;

		writeFieldOp(pByteCode, argNum, pNode->pUsage, t, pNode->op, bHasEqual, bHasMin, bHasMax);
		
		if(!pNode->next || pNode->nField != pNode->next->nField)
		{
			if(bHasEqual)
			{
				if(field.isRecNo())
					*pByteCode++ = addTestEqualRecNo;
				else
				{
					typed_op<addTestEqualText, addTestEqualNumeric, addTestEqualLong>::set(t, pByteCode);
					writeNum(pByteCode, pNode->nField);
				}
				bHasEqual = FALSE;
			}
			else
			{
				if(bHasMin)
				{
					if(field.isRecNo())
						*pByteCode++ = addTestMinimumRecNo;
					else
					{
						typed_op<addTestMinimumText, addTestMinimumNumeric, addTestMinimumLong>::set(t, pByteCode);
						writeNum(pByteCode, pNode->nField);
					}
					bHasMin = FALSE;
				}
				if(bHasMax)
				{
					if(field.isRecNo())
						*pByteCode++ = addTestMaximumRecNo;
					else
					{
						typed_op<addTestMaximumText, addTestMaximumNumeric, addTestMaximumLong>::set(t, pByteCode);
						writeNum(pByteCode, pNode->nField);
					}
					bHasMax = FALSE;
				}
			}
		}
		pNode = pNode->next;
	}

	while(nodes)
	{
		node* pDel = nodes;
		nodes = nodes->next;
		delete pDel;
	}
}

static int buildIndexWork(unsigned char*& pByteCode, const CVtabInfo& table,
		sqlite3_index_info* pIdx, const idx_node* pBest, BOOL& bHasMinKey, BOOL& bHasMaxKey)
{
	int argNum = 1;
	BOOL bHasFullEqual = FALSE, bHasFullMin = FALSE, bHasFullMax = FALSE;
	BOOL bHasPartEqual = FALSE, bHasPartMin = FALSE, bHasPartMax = FALSE;
	
	if(pBest->indexNum == -1)	// recNo
	{
		for(op_node* pOp = pBest->fields->compares; pOp ; pOp = pOp->next)
			writeFieldOp(pByteCode, argNum, pOp->pUsage, field_info::ftNumeric + 1, pOp->CompareOp, bHasFullEqual, bHasFullMin, bHasFullMax);
		*pByteCode++ = storeRecNo;
		return argNum;	
	}

	
	DWORD posInIdx = 0;
	const index_info* pIndexInfo = table.phisInfo().index(pBest->indexNum);
	const idx_field_info* pFieldInfo = pIndexInfo->fields();

	// Выберем индекс
	*pByteCode++ = selectIdx;
	writeNum(pByteCode, pBest->indexNum);

	for(idx_field_node* pField = pBest->fields; pField; pField = pField->next)
	{
		if(pField->posInIdx == -1)	// Полный индекс
		{
			for(op_node* pOp = pField->compares ; pOp ; pOp = pOp->next)
				writeFieldOp(pByteCode, argNum, pOp->pUsage, field_info::ftText, pOp->CompareOp, bHasFullEqual, bHasFullMin, bHasFullMax);
			
			if(bHasFullEqual)
				*pByteCode++ = setFullKeyEqual;
			else
			{
				if(bHasFullMin)
					*pByteCode++ = setFullKeyMinimum;
				if(bHasFullMax)
					*pByteCode++ = setFullKeyMaximum;
			}
		}
		else
		{
			if(pField->posInIdx > posInIdx)
				break;

			int t = pFieldInfo->info().type();
			if(t == field_info::ftNumeric && pFieldInfo->info().precession() == 0)
				t = field_info::ftNumeric + 1;

			bHasPartEqual = bHasPartMin = bHasPartMax = FALSE;
			for(op_node* pOp = pField->compares; pOp; pOp = pOp->next)
				writeFieldOp(pByteCode, argNum, pOp->pUsage, t, pOp->CompareOp, bHasPartEqual, bHasPartMin, bHasPartMax);
			
			// Прошли все условя для этого поля.
			if(bHasPartEqual)
			{
				typed_op<setPartKeyEqualText, setPartKeyEqualNumeric, setPartKeyEqualLong>::set(t, pByteCode);
				// Для поля есть условие =, можно двигаться дальше по индексу.
				++pFieldInfo;
				posInIdx++;
			}
			else
			{
				if(bHasPartMin)
					typed_op<setPartKeyMinimumText, setPartKeyMinimumNumeric, setPartKeyMinimumLong>::set(t, pByteCode);
				if(bHasPartMax)
					typed_op<setPartKeyMaximumText, setPartKeyMaximumNumeric, setPartKeyMaximumLong>::set(t, pByteCode);
				break;
			}
		}
	}
	// Определим, есть ли мин/макс границы, и выберем их.
	BOOL bPossiblyChangeTypeOfBound = FALSE;
	DWORD isPartEqual = bHasPartEqual || (!bHasPartMin && posInIdx > 0);
	// Выбор минимального ключа
	//                        1                 2                   4                    8
	DWORD typeOfBound = bHasFullEqual | (bHasFullMin << 1) | (isPartEqual<< 2) | (bHasPartMin << 3);
	bHasMinKey = TRUE;
	switch(typeOfBound)
	{
	case 0 | 0:	// Нижней границы нет
		bHasMinKey = FALSE;
		break;
	case 0 | 4:	// Нижняя граница задана полеиндекса=
		*pByteCode++ = setMinKey_PKEqual;
		break;
	case 0 | 8:	// Нижняя граница задана полеиндекса>
		*pByteCode++ = setMinKey_PKMin;
		break;
	case 1 | 0:	// Нижняя граница задана индекс=
		*pByteCode++ = setMinKey_FKEqual;
		break;
	case 1 | 4:	// Нижняя граница задана индекс= and полеиндекса=
		*pByteCode++ = selMinKey_FKEqual_PKEqual;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 1 | 8: // Нижняя граница задана индекс= and полеиндекса>
		*pByteCode++ = selMinKey_FKEqual_PKMin;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 2 | 0:	// Нижняя граница задана индекс>
		*pByteCode++ = setMinKey_FKMin;
		break;
	case 2 | 4:	// Нижняя граница задана индекс> and полеиндекса=
		*pByteCode++ = selMinKey_FKMin_PKEqual;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 2 | 8:	// Нижняя граница задана индекс> and полеиндекса>
		*pByteCode++ = selMinKey_FKMin_PKMin;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	}
	// Выбор максимального ключа
	isPartEqual = bHasPartEqual || (!bHasPartMax && posInIdx > 0);
	typeOfBound = bHasFullEqual | (bHasFullMax << 1) | (isPartEqual << 2) | (bHasPartMax << 3);
	bHasMaxKey = TRUE;
	switch(typeOfBound)
	{
	case 0 | 0:	// Верхней границы нет
		bHasMaxKey = FALSE;
		break;
	case 0 | 4:	// Верхняя граница задана полеиндекса=
		*pByteCode++ = setMaxKey_PKEqual;
		break;
	case 0 | 8:	// Верхняя граница задана полеиндекса<
		*pByteCode++ = setMaxKey_PKMax;
		break;
	case 1 | 0:	// Верхняя граница задана индекс=
		*pByteCode++ = setMaxKey_FKEqual;
		break;
	case 1 | 4:	// Верхняя граница задана индекс= and полеиндекса=
		*pByteCode++ = selMaxKey_FKEqual_PKEqual;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 1 | 8: // Верхняя граница задана индекс= and полеиндекса<
		*pByteCode++ = selMaxKey_FKEqual_PKMax;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 2 | 0:	// Верхняя граница задана индекс<
		*pByteCode++ = setMaxKey_FKMax;
		break;
	case 2 | 4:	// Верхняя граница задана индекс< and полеиндекса=
		*pByteCode++ = selMaxKey_FKMax_PKEqual;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	case 2 | 8:	// Верхняя граница задана индекс< and полеиндекса<
		*pByteCode++ = selMaxKey_FKMax_PKMax;
		bPossiblyChangeTypeOfBound = TRUE;
		break;
	}
	if(bHasMinKey && bHasMaxKey && bPossiblyChangeTypeOfBound)
	{
		// Есть и верхняя и нижняя граница, и ее тип может изменится
		*pByteCode++ = cmpMinMaxKeys;
	}
	return argNum;
}

void FilterMachine::build(const CVtabInfo& table, sqlite3_index_info* pIdx, const idx_node* pBest)
{
	// Тест, что опкоды вмещаются в 1 байт
	char testOpCodeSize[lastOpCode > 0xFF ? -1 : 1] = {0};
	
	// Примерно примем, что на каждое ограничение уходит аж целых 64 байта
	// нашего байт-кода. Думаю, в этот размер уложимся.
	BYTE *pStartByteCode = new BYTE[(pIdx->nConstraint + 10) * 64], *pByteCode = pStartByteCode;
	BOOL bHasMinKey, bHasMaxKey;
	int argNum;
	
	if(pBest)
	{
		pIdx->idxNum = pBest->indexNum;
		if(pBest->order != nooNa)
			pIdx->orderByConsumed = TRUE;
		argNum = buildIndexWork(pByteCode, table, pIdx, pBest, bHasMinKey, bHasMaxKey);
	}
	else
	{
		pIdx->idxNum = -1;
		pIdx->orderByConsumed = FALSE;
		argNum = 1;
	}

	// Здесь добавим создание кода для проверки доп.условий, не попавших в индекс
	buildAdditionalTests(pByteCode, pIdx, argNum, table);

	if(pBest)
	{
		if(pBest->indexNum == -1)
			*pByteCode++ = gotoToRow;
		else
		{
			// Определим направление выборки.
			BOOL bReverse = FALSE;
			if(pBest->order == nooDesc)	// Если подписались выдать в обратном порядке, ничего не попишешь.
				bReverse = TRUE;
			else if(pBest->order == nooNa)	// Не подписывались на сортировку.
			{
				if(bHasMaxKey && !bHasMinKey)	// Проще двинуть в конец, и идти к началу, не проверяя ключа
					bReverse = TRUE;
			}
			if(bReverse)
				*pByteCode++ = bHasMaxKey ? (bHasMinKey ? gotoMaxToMin : gotoMaxToFirst) : (bHasMinKey ? gotoLastToMin : gotoLastToFirst);
			else
				*pByteCode++ = bHasMinKey ? (bHasMaxKey ? gotoMinToMax : gotoMinToLast) : (bHasMaxKey ? gotoFirstToMax : gotoFirstToLast);
		}
	}
	else
	{
		// Индекс не выбран. Создадим код простого перебора.
		*pByteCode++ = gotoFirstToLast;
	}
	*pByteCode++ = stop;

	CString idxStr, byteCode;
	byteCodeToStr(pStartByteCode, pByteCode, byteCode);

	idxStr.Format("%s;%s", pBest ? (pBest->indexNum == -1 ? "recNo" : table.table()->index(pBest->indexNum)->szName) : "noIdx", byteCode);
	int idxStrLen = idxStr.GetLength() + 1;

	pIdx->idxStr = (char*)sqlite3_malloc(idxStrLen);
	pIdx->needToFreeIdxStr = TRUE;
	memcpy(pIdx->idxStr, (LPCSTR)idxStr, idxStrLen);
	delete [] pStartByteCode;
}

select_iterator* FilterMachine::parseFilter(cursor_data& cursor, const CVtabInfo& table, const char* idxStr, sqlite3_value** args)
{
	FilterMachine machine(table, cursor);
	BYTE* pByteCode = strToByteCode(strchr(idxStr, ';') + 1);
	select_iterator* it = machine.process(pByteCode, args);
	delete [] pByteCode;
	return it;
}

///////////////////////////////////////////////////////////////////////////////////////
// Классы разных итераторов

// Итератор без проверки стоп-ключа и доп.условий.
template<NavCtrl direction>
struct iterNoStopKeyNoTest : select_iterator
{
	virtual BOOL doNext(cursor_data& cursor, const CVtabInfo& table)
	{
		return cursor.store.Goto(direction, 0);
	}
};


// Итератор с проверкой стоп-ключа но без проверки доп.условий.
template<NavCtrl direction>
struct iterStopKeysNoTests : select_iterator
{
	testConstraint* pUntil;
	iterStopKeysNoTests(testConstraint*& pS) : pUntil(pS)
	{
		pS = NULL;
	}
	~iterStopKeysNoTests()
	{
		delete pUntil;
	}
	virtual BOOL doNext(cursor_data& cursor, const CVtabInfo& table)
	{
		if(!cursor.store.Goto(direction, 0))
			return FALSE;
		if(!testConstraint::doTest(pUntil, cursor))
			return FALSE;
		return TRUE;
	}
};

// Итератор без проверки стоп-ключа, но с проверкой доп.условий.
template<NavCtrl direction>
struct iterNoStopKeysTests : select_iterator
{
	testConstraint* pTests;
	iterNoStopKeysTests(testConstraint*& pT) : pTests(pT)
	{
		pT = NULL;
	}
	~iterNoStopKeysTests()
	{
		delete pTests;
	}
	virtual BOOL doNext(cursor_data& cursor, const CVtabInfo& table)
	{
		for(;;)
		{
			if(!cursor.store.Goto(direction, 0))
				return FALSE;
			if(!testConstraint::doTest(pTests, cursor))
				continue;
			return TRUE;
		}
	}
};

// Итератор с проверкой стоп-ключа и доп.условий
template<NavCtrl direction>
struct iterStopKeysTests : select_iterator
{
	testConstraint *pUntil, *pTest;
	iterStopKeysTests(testConstraint*& pS, testConstraint*& pT) : pUntil(pS), pTest(pT)
	{
		pS = NULL;
		pT = NULL;
	}
	~iterStopKeysTests()
	{
		delete pUntil;
		delete pTest;
	}
	virtual BOOL doNext(cursor_data& cursor, const CVtabInfo& table)
	{
		for(;;)
		{
			if(!cursor.store.Goto(direction, 0))
				return FALSE;
			if(!testConstraint::doTest(pUntil, cursor))
				return FALSE;
			if(!testConstraint::doTest(pTest, cursor))
				continue;
			return TRUE;
		}
	}
};

// Класс универсального установщика ключа на индекс
class CUniKeyObj : public CKeyObj
{
public:
	CUniKeyObj(CIndex* pI, const CString& key, const index_info* pIndexInfo)
		: CKeyObj(pI, 0, 0), m_strKey(key), m_pIndexInfo(pIndexInfo)
	{
	}
	virtual void PrepareKey()
	{
		LPCSTR ptr = m_strKey;
		const idx_field_info* pIdxFieldInfo = m_pIndexInfo->fields();
		for(DWORD i = m_pIndexInfo->fieldsCount(); i-- ; pIdxFieldInfo++)
		{
			int len = pIdxFieldInfo->info().length();
			m_pStoreObj->FX_String(pIdxFieldInfo->numInTable(), (char*)ptr, len, 1);
			ptr += len;
		}
	}
	LPCSTR m_strKey;
	const index_info* m_pIndexInfo;
};

// Шаблонная функция перехода по ключу
template <TestOp op, BOOL bFullKey>
struct Key
{
	static int Move(CStoreObj& store, const CString& keyOrig, const index_info* pIndexInfo)
	{
		// Сюда не должно попадать условие toEqual.
		char test_buf[op == toEqual ? 0 : 1]={0};
		int CountByteInIdx = pIndexInfo->keySize();
		CompCtl direction;
		CString key(keyOrig);

		if(toLess == op)
			direction = ccL;
		else if(toLessEq == op)
			direction = ccLE;
		else if(toGrat == op)
			direction = ccG;
		else if(toGratEq == op)
			direction = ccGE;
		
		int len = key.GetLength();
		
		if(bFullKey)
		{
			if(len > CountByteInIdx)
			{
				int row = atol((LPCSTR)key + CountByteInIdx);
				key.GetBufferSetLength(CountByteInIdx);
				if(row)
				{
					// В ключе еще и номер строки.
					// Поэтому возможно надо заменить еще и тип условия.
					// Так, если было >"aaaaa|10", то нельзя делать поиск >"aaaaa",
					// надо делать поиск >="aaaaa", и потом ручками додвигать вперед,
					// пока выполняется условие (ключ=="aaaaa" и номер записи <=10)
					// Аналогично при условии <.
					if(toGrat == op)
						direction = ccGE;
					else if(toLess == op)
						direction = ccLE;
				}
			}
			else if(len < CountByteInIdx)
				memset(key.GetBufferSetLength(CountByteInIdx), ' ', CountByteInIdx - len);
		}
		else	// Частичный ключ
		{
			if(len < CountByteInIdx)
			{
				// Надо дополнить ключ до полного.
				// При этом если сравнение заканчивалось на > или <=, то дополнять надо символами 0xFF
				memset(key.GetBufferSetLength(CountByteInIdx) + len, toGrat == op || toLessEq == op ? 0xFF : ' ', CountByteInIdx - len);
			}
		}
		// Далее небольшая закавыка с движком 1С
		// если есть таблица
		// 1. 'a'
		// 2. 'a'
		// 3. 'd'
		// и делаем поиск <='a', то 1C найдет запись № 1.
		// Но у нас такое условие может применяться только для обхода
		// индекса в обратном порядке, и в данном случае хотелось бы встать
		// на запись № 2. Поэтому условие <='a' надо изменить на <'b'.
		if(ccLE == direction)
		{
			if(2 == nextGratStringUpper(key, CountByteInIdx))
			{
				// Следующую большую строку получить не удалось, например было <='яяяяяяя'
				// Очевидно, что надо двигать в конец индекса
				return store.Goto(navLast, 0);
			}
			else
				direction = ccL;
		}
		
		CUniKeyObj idxKey(store.pIndex, key, pIndexInfo);
		return store.Goto(&idxKey, direction, 0);
	}
};


// Обработчики разных порядков выборки

// Двигаться по таблице от начала до конца
select_iterator* FilterMachine::GotoFirstToLast()
{
	// Двигаемся в начало
	if(!m_cursor.store.Goto(navFirst, 0))
		return NULL;
	// Если доп.условий нет, вернем итератор
	if(!m_pAdditionalTests)
		return new iterNoStopKeyNoTest<navNext>;
	
	// Будем искать первую запись, удовлетворяющую доп.условиям
	for(;;)
	{
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
			return new iterNoStopKeysTests<navNext>(m_pAdditionalTests);
		if(!m_cursor.store.Goto(navNext, 0))
			return NULL;
	}
}

// Двигаться по таблице от начала до максимального ключа
select_iterator* FilterMachine::GotoFirstToMax()
{
	// Идем в начало
	if(!m_cursor.store.Goto(navFirst, 0))
		return NULL;
	// Построим проверяльщики максимального ключа, взависимости от типа границы.
	testConstraint* pUntil = NULL;
	switch(m_maxKType)
	{
	//case keyPKEq:	Такого типа границы здесь быть не может, так как когда один
	// из типов границы "равенство", то обязательно есть граница с другой стороны	
	//	pUntil = m_pPartEqConstr;
	//	m_pPartEqConstr = NULL;	// Чтоб не удалили
	//	break;
	case keyPKnEq:
		testConstraint::append(pUntil, m_pPKMaxConstr, m_pPKEqConstr);
		break;
	//case keyFKEq:
	//	new tcFullKey(pUntil, toEqual, m_fullKeyEq, m_pIndexInfo);
	case keyFKnEq:
		if(m_hardFKMax)
			new tcFullKey<toLess>(pUntil, m_fullKeyMax, m_pIndexInfo);
		else
			new tcFullKey<toLessEq>(pUntil, m_fullKeyMax, m_pIndexInfo);
		break;
	}
	for(;;)
	{
		// Проверим, а не пора ли уже остановится?
		if(!testConstraint::doTest(pUntil, m_cursor))
			break;
		// Проверим на доп.условия (если их нет, проверка всегда будет TRUE)
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
		{
			if(m_pAdditionalTests)
				return new iterStopKeysTests<navNext>(pUntil, m_pAdditionalTests);
			else
				return new iterStopKeysNoTests<navNext>(pUntil);
		}
		// Запись не подходит по доп.условиям. Попробуем следующую
		if(!m_cursor.store.Goto(navNext, 0))
			break;
	}
	delete pUntil;
	return NULL;
}

// Двигаться по таблице от минимального ключа до конца таблицы
select_iterator* FilterMachine::GotoMinToLast()
{
	// Удалось ли перейти на минимальный ключ
	BOOL bMoved;
	// Возможно, мин.ключ задан полным ключом, с номером строки, и надо будет
	// додвигать "ручками"
	testConstraint* pStartFullKey = NULL;
	
	switch(m_minKType)
	{
	// Данные кэйсы не может возникнуть, так как если одна из границ обозначена как
	// появившеяся из равенства, то обязательно будет и вторая граница.
	//case keyPKEq:
	//case keyFKEq:	
	case keyPKnEq:
		if(m_hardPKMin)
			bMoved = Key<toGrat, FALSE>::Move(m_cursor.store, m_partKeyMin, m_pIndexInfo);
		else
			bMoved = Key<toGratEq, FALSE>::Move(m_cursor.store, m_partKeyMin, m_pIndexInfo);
		break;
	case keyFKnEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		if(m_hardFKMin)
		{
			new tcFullKey<toGrat>(pStartFullKey, m_fullKeyMin, m_pIndexInfo);
			bMoved = Key<toGrat, TRUE>::Move(m_cursor.store, m_fullKeyMin, m_pIndexInfo);
		}
		else
		{
			new tcFullKey<toGratEq>(pStartFullKey, m_fullKeyMin, m_pIndexInfo);
			bMoved = Key<toGratEq, TRUE>::Move(m_cursor.store, m_fullKeyMin, m_pIndexInfo);
		}
		break;
	//default:
	//	bMoved = FALSE;
	}
	if(pStartFullKey)
	{
		if(bMoved)
		{
			// Ключ задан полным ключом, возможно в нем номер записи и надо додвигать запись
			for(;;)
			{
				if(testConstraint::doTest(pStartFullKey, m_cursor))
					break;
				if(!m_cursor.store.Goto(navNext, 0))
				{
					bMoved = FALSE;
					break;
				}
			}
		}
		delete pStartFullKey;
	}

	if(!bMoved)
		return NULL;

	if(!m_pAdditionalTests)
		return new iterNoStopKeyNoTest<navNext>;
	
	for(;;)
	{
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
			return new iterNoStopKeysTests<navNext>(m_pAdditionalTests);
		if(!m_cursor.store.Goto(navNext, 0))
			return NULL;
	}
}

select_iterator* FilterMachine::GotoMinToMax()
{
	BOOL bMoved;
	testConstraint* pStartFullKey = NULL;
	switch(m_minKType)
	{
	case keyPKEq:
		bMoved = Key<toGratEq, FALSE>::Move(m_cursor.store, m_partKeyEq, m_pIndexInfo);
		break;
	case keyPKnEq:
		if(m_hardPKMin)
			bMoved = Key<toGrat, FALSE>::Move(m_cursor.store, m_partKeyMin, m_pIndexInfo);
		else
			bMoved = Key<toGratEq, FALSE>::Move(m_cursor.store, m_partKeyMin, m_pIndexInfo);
		break;
	case keyFKEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		new tcFullKey<toGratEq>(pStartFullKey, m_fullKeyEq, m_pIndexInfo);
		bMoved = Key<toGratEq, TRUE>::Move(m_cursor.store, m_fullKeyEq, m_pIndexInfo);
		break;
	case keyFKnEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		if(m_hardFKMin)
		{
			new tcFullKey<toGrat>(pStartFullKey, m_fullKeyMin, m_pIndexInfo);
			bMoved = Key<toGrat, TRUE>::Move(m_cursor.store, m_fullKeyMin, m_pIndexInfo);
		}
		else
		{
			new tcFullKey<toGratEq>(pStartFullKey, m_fullKeyMin, m_pIndexInfo);
			bMoved = Key<toGratEq, TRUE>::Move(m_cursor.store, m_fullKeyMin, m_pIndexInfo);
		}
		break;
	}
	
	if(pStartFullKey)
	{
		if(bMoved)
		{
			// Ключ задан полным ключом, возможно в нем номер записи и надо додвигать запись
			for(;;)
			{
				if(testConstraint::doTest(pStartFullKey, m_cursor))
					break;
				if(!m_cursor.store.Goto(navNext, 0))
				{
					bMoved = FALSE;
					break;
				}
			}
		}
		delete pStartFullKey;
	}
	if(!bMoved)
		return NULL;
	// Теперь конечный ключ
	testConstraint* pUntil = NULL;
	switch(m_maxKType)
	{
	case keyPKEq:
		pUntil = m_pPKEqConstr;
		m_pPKEqConstr = NULL;	// Чтоб не удалили
		break;
	case keyPKnEq:
		testConstraint::append(pUntil, m_pPKMaxConstr, m_pPKEqConstr);
		break;
	case keyFKEq:
		new tcFullKey<toEqual>(pUntil, m_fullKeyEq, m_pIndexInfo);
		break;
	case keyFKnEq:
		if(m_hardFKMax)
			new tcFullKey<toLess>(pUntil, m_fullKeyMax, m_pIndexInfo);
		else
			new tcFullKey<toLessEq>(pUntil, m_fullKeyMax, m_pIndexInfo);
		break;
	}

	for(;;)
	{
		if(!testConstraint::doTest(pUntil, m_cursor))
			break;
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
		{
			if(m_pAdditionalTests)
				return new iterStopKeysTests<navNext>(pUntil, m_pAdditionalTests);
			else
				return new iterStopKeysNoTests<navNext>(pUntil);
		}
		if(!m_cursor.store.Goto(navNext, 0))
			break;
	}
	delete pUntil;
	return NULL;
}

select_iterator* FilterMachine::GotoLastToFirst()
{
	if(!m_cursor.store.Goto(navLast, 0))
		return NULL;
	if(!m_pAdditionalTests)
		return new iterNoStopKeyNoTest<navPrev>;
	
	for(;;)
	{
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
			return new iterNoStopKeysTests<navPrev>(m_pAdditionalTests);
		if(!m_cursor.store.Goto(navPrev, 0))
			return NULL;
	}
}

select_iterator* FilterMachine::GotoMaxToFirst()
{
	BOOL bMoved;
	testConstraint* pStartFullKey = NULL;
	switch(m_maxKType)
	{
	// Данные кэйсы не может возникнуть, так как если одна из границ обозначена как
	// появившеяся из равенства, то обязательно будет и вторая граница.
	//case keyPKEq:
	//case keyFKEq:	
	case keyPKnEq:
		if(m_hardPKMax)
			bMoved = Key<toLess, FALSE>::Move(m_cursor.store, m_partKeyMax, m_pIndexInfo);
		else
			bMoved = Key<toLessEq, FALSE>::Move(m_cursor.store, m_partKeyMax, m_pIndexInfo);
		break;
	case keyFKnEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		if(m_hardFKMax)
		{
			new tcFullKey<toLess>(pStartFullKey, m_fullKeyMax, m_pIndexInfo);
			bMoved = Key<toLess, TRUE>::Move(m_cursor.store, m_fullKeyMax, m_pIndexInfo);
		}
		else
		{
			new tcFullKey<toLessEq>(pStartFullKey, m_fullKeyMax, m_pIndexInfo);
			bMoved = Key<toLessEq, TRUE>::Move(m_cursor.store, m_fullKeyMax, m_pIndexInfo);
		}
		break;
	}
	if(pStartFullKey)
	{
		if(bMoved)
		{
			// Ключ задан полным ключом, возможно в нем номер записи и надо додвигать запись
			for(;;)
			{
				if(testConstraint::doTest(pStartFullKey, m_cursor))
					break;
				if(!m_cursor.store.Goto(navPrev, 0))
				{
					bMoved = FALSE;
					break;;
				}
			}
		}
		delete pStartFullKey;
	}

	if(!bMoved)
		return NULL;
	
	if(!m_pAdditionalTests)
		return new iterNoStopKeyNoTest<navPrev>;
	
	for(;;)
	{
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
			return new iterNoStopKeysTests<navPrev>(m_pAdditionalTests);
		if(!m_cursor.store.Goto(navPrev, 0))
			return NULL;
	}
}

select_iterator* FilterMachine::GotoLastToMin()
{
	if(!m_cursor.store.Goto(navLast, 0))
		return NULL;
	testConstraint* pUntil = NULL;
	switch(m_minKType)
	{
	case keyPKEq:
		pUntil = m_pPKEqConstr;
		m_pPKEqConstr = NULL;	// Чтоб не удалили
		break;
	case keyPKnEq:
		testConstraint::append(pUntil, m_pPKMinConstr, m_pPKEqConstr);
		break;
	case keyFKEq:
		new tcFullKey<toEqual>(pUntil, m_fullKeyEq, m_pIndexInfo);
		break;
	case keyFKnEq:
		if(m_hardFKMin)
			new tcFullKey<toGrat>(pUntil, m_fullKeyMin, m_pIndexInfo);
		else
			new tcFullKey<toGratEq>(pUntil, m_fullKeyMin, m_pIndexInfo);
		break;
	}

	for(;;)
	{
		if(!testConstraint::doTest(pUntil, m_cursor))
			break;
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
		{
			if(m_pAdditionalTests)
				return new iterStopKeysTests<navPrev>(pUntil, m_pAdditionalTests);
			else
				return new iterStopKeysNoTests<navPrev>(pUntil);
		}
		if(!m_cursor.store.Goto(navPrev, 0))
			break;
	}
	delete pUntil;
	return NULL;
}

select_iterator* FilterMachine::GotoMaxToMin()
{
	BOOL bMoved;
	testConstraint* pStartFullKey = NULL;
	switch(m_maxKType)
	{
	case keyPKEq:
		bMoved = Key<toLessEq, FALSE>::Move(m_cursor.store, m_partKeyEq, m_pIndexInfo);
		break;
	case keyPKnEq:
		if(m_hardPKMax)
			bMoved = Key<toLess, FALSE>::Move(m_cursor.store, m_partKeyMax, m_pIndexInfo);
		else
			bMoved = Key<toLessEq, FALSE>::Move(m_cursor.store, m_partKeyMax, m_pIndexInfo);
		break;
	case keyFKEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		new tcFullKey<toLessEq>(pStartFullKey, m_fullKeyEq, m_pIndexInfo);
		bMoved = Key<toLessEq, TRUE>::Move(m_cursor.store, m_fullKeyEq, m_pIndexInfo);
		break;
	case keyFKnEq:
		// Начальный ключ задается полным ключом, поэтому возможно, в нем еще и номер записи.
		// И тогда его надо додвигать ручками.
		if(m_hardFKMax)
		{
			new tcFullKey<toLess>(pStartFullKey, m_fullKeyMax, m_pIndexInfo);
			bMoved = Key<toLess, TRUE>::Move(m_cursor.store, m_fullKeyMax, m_pIndexInfo);
		}
		else
		{
			new tcFullKey<toLessEq>(pStartFullKey, m_fullKeyMax, m_pIndexInfo);
			bMoved = Key<toLessEq, TRUE>::Move(m_cursor.store, m_fullKeyMax, m_pIndexInfo);
		}
		break;
	}
	if(pStartFullKey)
	{
		if(bMoved)
		{
			// Ключ задан полным ключом, возможно в нем номер записи и надо додвигать запись
			for(;;)
			{
				if(testConstraint::doTest(pStartFullKey, m_cursor))
					break;
				if(!m_cursor.store.Goto(navPrev, 0))
				{
					bMoved = FALSE;
					break;
				}
			}
		}
		delete pStartFullKey;
	}
	if(!bMoved)
		return NULL;
	// Теперь конечный ключ
	testConstraint* pUntil = NULL;
	switch(m_minKType)
	{
	case keyPKEq:
		pUntil = m_pPKEqConstr;
		m_pPKEqConstr = NULL;	// Чтоб не удалили
		break;
	case keyPKnEq:
		testConstraint::append(pUntil, m_pPKMinConstr, m_pPKEqConstr);
		break;
	case keyFKEq:
		new tcFullKey<toEqual>(pUntil, m_fullKeyEq, m_pIndexInfo);
		break;
	case keyFKnEq:
		if(m_hardFKMin)
			new tcFullKey<toGrat>(pUntil, m_fullKeyMin, m_pIndexInfo);
		else
			new tcFullKey<toGratEq>(pUntil, m_fullKeyMin, m_pIndexInfo);
		break;
	}

	for(;;)
	{
		if(!testConstraint::doTest(pUntil, m_cursor))
			break;
		if(testConstraint::doTest(m_pAdditionalTests, m_cursor))
		{
			if(m_pAdditionalTests)
				return new iterStopKeysTests<navPrev>(pUntil, m_pAdditionalTests);
			else
				return new iterStopKeysNoTests<navPrev>(pUntil);
		}
		if(!m_cursor.store.Goto(navPrev, 0))
			break;
	}
	delete pUntil;
	return NULL;
}

select_iterator* FilterMachine::GotoToRow()
{
	char recno[sizeof(CRecAddr)];
	CRecAddr* pAddr = (CRecAddr*)recno;
	pAddr->m_pos = m_recNo;
	pAddr->data = 0;

	if(!m_cursor.store.Goto(pAddr, 0))
		return NULL;
	if(!testConstraint::doTest(m_pAdditionalTests, m_cursor))
		return NULL;
	
	struct iterRow : select_iterator 
	{
		virtual BOOL doNext(cursor_data&, const CVtabInfo&)
		{
			return FALSE;
		}
	};
	return new iterRow;
}
