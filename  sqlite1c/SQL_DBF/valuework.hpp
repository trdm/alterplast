//valuework.hpp
#pragma once

enum Types1C{
	typeUndefined       = 0,
	typeNumber          = 1,
	typeString          = 2,
	typeDate            = 3,
	typeEnum            = 10,
	typeReference       = 11,
	typeDocument        = 12,
	typeCalendar        = 13,
	typeCalcKind        = 14,
	typeAccount         = 15,
	typeSubcKind        = 16,
	typeChartOfAcc      = 17,
};

inline static DWORD str2id(const unsigned char* ptr, DWORD len)
{
	DWORD id = 0;
	while(len--)
	{
		DWORD s = (DWORD)*ptr++;
		if(s >= 'A')
			id = id * 36 + s - 'A' + 10;
		else if(s >= '0')
			id = id * 36 + s - '0';
	}
	return id;
}

inline static DWORD str2dec(const unsigned char* ptr, DWORD len)
{
	DWORD id = 0;
	while(len--)
	{
		DWORD s = (DWORD)*ptr++;
		if(s >= '0')
			id = id * 10 + s - '0';
	}
	return id;
}

inline static void dec2str(DWORD dec, unsigned char* ptr, DWORD len)
{
	ptr += len;
	do 
	{
		*--ptr = (dec % 10) + '0';
		dec /= 10;
	}while(dec);
}

inline static void id2str(DWORD id, unsigned char* ptr, DWORD len)
{
	ptr += len;
	do 
	{
		DWORD v = id % 36;
		*--ptr = v < 10 ? v + '0' : v - 10 + 'A';
		id /= 36;
	}while(id);
}


enum TypeOfString {
	tos9 = 9,
	tos13 = 13,
	tos23 = 23
};

template<TypeOfString tos>
struct ValueStrWork
{
	inline static void str2val(CValue* pValue, LPCSTR str, Types1C type = typeUndefined, long mdID = 0)
	{
		char test[tos == tos9 || tos == tos13 || tos == tos23 ? 1 : -1] = {0};

		unsigned char* ptr = (unsigned char*)str;
		
		if(tos == tos23)
		{
			switch((DWORD)*ptr)
			{
			case 'U':
				return;
			case 'N':
				{
					static CNumeric num;
					*pValue = num.FromString(str + 1, NULL);
				}
				return;
			case 'S':
				*pValue = str + 1;
				return;
			case 'D':
				*pValue = CDate(str2dec(ptr + 1, 4), str2dec(ptr + 5, 2), str2dec(ptr + 7, 2));
				return;
			case 'E':
				type = typeEnum;
				break;
			case 'B':
				type = typeReference;
				break;
			case 'O':
				type = typeDocument;
				break;
			case 'C':
				type = typeCalendar;
				break;
			case 'A':
				type = typeCalcKind;
				break;
			case 'T':
				type = typeAccount;
				break;
			case 'K':
				type = typeSubcKind;
				break;
			case 'P':
				type = typeChartOfAcc;
				break;
			default:
				return;
			}
			ptr++;
			ptr++;
		}
		pValue->type = type;
		pValue->m_mdid = mdID;
		
		if(tos == tos13 || tos == tos23)
		{
			if(typeCalendar == type || typeCalcKind == type)
				mdID = 0;
			else
				mdID = str2id(ptr, 4);
			ptr += 4;
		}
		pValue->ValTypeID = mdID;
		pValue->m_ObjID.ObjID = str2id(ptr, 6);
		*(DWORD*)pValue->m_ObjID.DBSign.Sign = (*(DWORD*)(ptr + 6)) & 0xFFFFFF;
	}
	
	inline static void val2str(const CValue& val, CString& res)
	{
		char test[tos == tos9 || tos == tos13 || tos == tos23 ? 1 : -1] = {0};

		unsigned char* ptr = (unsigned char*)res.GetBufferSetLength(tos);
		if(tos == tos23)
		{
			// Заполним строку пробелами
			*(DWORD*)(ptr + 0) = 0x20202020;
			*(DWORD*)(ptr + 4) = 0x20202020;
			*(DWORD*)(ptr + 8) = 0x20202020;
			*(DWORD*)(ptr + 12) = 0x20202020;
			*(DWORD*)(ptr + 16) = 0x20202020;
			*(DWORD*)(ptr + 20) = 0x00202020;
			switch(val.type)
			{
			case typeUndefined:
				*ptr = 'U';
				return;
			case typeNumber:
				*ptr++ = 'N';
				{
					CString str = val.GetString();
					int l = str.GetLength();
					if(l <= 22)
						memcpy(ptr, (LPCSTR)str, l);
				}
				return;
			case typeString:
				*ptr++ = 'S';
				{
					int l = val.m_String.GetLength();
					memcpy(ptr, (LPCSTR)val.m_String, l < 22 ? l : 22);
				}
				return;
			case typeDate:
				*ptr++ = 'D';
				*(DWORD*)(ptr + 4) = '0000';
				dec2str(val.m_Date.GetYear(), ptr, 4);
				dec2str(val.m_Date.GetMonth(), ptr + 4, 2);
				dec2str(val.m_Date.GetMonthDay(), ptr + 6, 2);
				return;
			case typeEnum:
				*ptr++ = 'E';
				break;
			case typeReference:
				*ptr++ = 'B';
				break;
			case typeDocument:
				*ptr++ = 'O';
				break;
			case typeCalendar:
				*ptr++ = 'C';
				break;
			case typeCalcKind:
				*ptr++ = 'A';
				break;
			case typeAccount:
				*ptr++ = 'T';
				break;
			case typeSubcKind:
				*ptr++ = 'K';
				break;
			case typeChartOfAcc:
				*ptr++ = 'P';
				break;
			}
			*ptr++ = '1';
		}
		// Простых типов здесь быть не должно!!!
		if(val.type <= typeDate)
		{
			memset(ptr, ' ', tos);
			return;
		}
		
		if(tos == tos13 || tos == tos23)
		{
			// Укажем метаид
			if(typeCalendar == val.type || typeCalcKind == val.type)
				*(DWORD*)ptr = '0   ';
			else
			{
				if(tos == tos13)	// Надо указать метаид, но строка пробелами не заполнялась
					*(DWORD*)ptr = 0x20202020;
				id2str(val.m_mdid ? val.m_mdid : val.ValTypeID, ptr, 4);
			}
			ptr += 4;
		}
		
		if(tos != tos23)	// Надо указать объектид, но строка пробелами не заполнялась
		{
			*(DWORD*)ptr = 0x20202020;
			*(WORD*)(ptr + 4) = 0x2020;
		}
		// Укажем объектид и дбзнак.
		id2str(val.m_ObjID.ObjID, ptr, 6);
		*(DWORD*)(ptr + 6) = *(DWORD*)val.m_ObjID.DBSign.Sign;

		// При указании дбзнака в длинной строке последний пробел затрется на 0
		if(tos == 23)
			ptr[9] = ' ';
	}
};

template<typename T>
inline static void bindNumeric(T& binder, const CValue* pValue, int mod)
{
	if(0 != mod)
		binder.error("Неизвестный модификатор для типа \"Число\"");
	binder.bindNumeric(pValue);
}

template<typename T>
inline static void bindString(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:		// Строка как есть
		binder.bindText(pValue->m_String);
		break;
	case 1:		// Позиция документа
	case 2:		// Позиция документа для верхней границы по индкесам, начинающимся с DATE,TIME,IDDOC
		if(pValue->m_String.GetLength() == 32)
		{
			CString text;
			const BYTE* pSrc = (const BYTE*)(LPCSTR)pValue->m_String;
			BYTE* pDst = (BYTE*)text.GetBufferSetLength(2 == mod ? 24 : 23);
			//memcpy(pDst, pSrc + 1, 8);
			*(DWORD*)(pDst +  0) = *(DWORD*)(pSrc +  1);
			*(DWORD*)(pDst +  4) = *(DWORD*)(pSrc +  5);
			*(DWORD*)(pDst +  8) = '    ';
			*(DWORD*)(pDst + 12) = '    ';
			*(DWORD*)(pDst + 16) = '    ';
			id2str(str2dec(pSrc + 10, 9), pDst + 8, 6);
			id2str(str2dec(pSrc + 19, 10), pDst + 14, 6);
			*(DWORD*)(pDst + 20) = *(DWORD*)(pSrc + 29);
			if(2 == mod)
				pDst[23] = 'Я';
			binder.bindText(text);
		}
		else
			binder.error("Переданная строка не позиция документа");
		break;
	case 3:
		binder.bindFragment(pValue->m_String);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Строка\"");
	}
}

template<typename T>
inline static void bindDate(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:		// Дата как есть
	case 1:
		{
			CString text;
			BYTE* buf = (BYTE*)text.GetBufferSetLength(mod ? 9 : 8);
			DWORD day = pValue->m_Date.GetMonthDay();
			if(day)		// Непустая дата
			{
				dec2str(pValue->m_Date.GetYear(), buf, 4);
				*(DWORD*)(buf + 4) = '0000';
				dec2str(pValue->m_Date.GetMonth(), buf + 4, 2);
				dec2str(day, buf + 6, 2);
			}
			else
				*(unsigned __int64*)buf = 0x2020202020202020;
			if(mod)
				buf[8] = 'Я';
			binder.bindText(text);
		}
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Дата\"");
	}
}

template<typename T>
inline static void bindObject(T& binder, const CValue* pValue, int mod)
{
	CString text;
	switch(mod)
	{
	case 0:		// Строка 9
		ValueStrWork<tos9>::val2str(*pValue, text);
		binder.bindText(text);
		break;
	case 1:		// Строка 13
		ValueStrWork<tos13>::val2str(*pValue, text);
		binder.bindText(text);
		break;
	}
}

template<typename T>
inline static void bindKindOfObject(T& binder, const CValue* pValue)
{
	CString text;
	BYTE* ptr = (BYTE*)text.GetBufferSetLength(4);
	*(DWORD*)ptr = '    ';
	id2str(pValue->m_mdid ? pValue->m_mdid : pValue->ValTypeID, ptr, 4);
	binder.bindText(text);
}

template<typename T>
inline static void bindEnum(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Перечисление\"");
	}
}

template<typename T>
inline static void bindReference(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Справочник\"");
	}
}

template<typename T>
inline static void bindDocument(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	case 3:		// Позиция документа
	case 4:		// Позиция документа для верхней границы по индкесам, начинающимся с DATE,TIME,IDDOC
        {
			CString text;
			BYTE* ptr = (BYTE*)text.GetBufferSetLength(4 == mod ? 24 : 23);
			CEventTime EventTime;
			CDate Date(0,0,0);
			GetDateTimeFromValue(*pValue, Date, EventTime);
			dec2str(Date.GetYear(), ptr, 4);
			*(DWORD*)(ptr + 4) = '0000';
			dec2str(Date.GetMonth(), ptr + 4, 2);
			dec2str(Date.GetMonthDay(), ptr + 6, 2);
			*(DWORD*)(ptr +  8) = '    ';
			*(DWORD*)(ptr + 12) = '    ';
			*(DWORD*)(ptr + 16) = '    ';
			id2str(EventTime.GetlTime(), ptr + 8, 6);
			id2str(pValue->m_ObjID.ObjID, ptr + 14, 6);
			*(DWORD*)(ptr + 20) = *(DWORD*)pValue->m_ObjID.DBSign.Sign;
			if(4 == mod)
				ptr[23] = 'Я';
			binder.bindText(text);
		}
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Документ\"");
	}
}

template<typename T>
inline static void bindCalendar(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Календарь\"");
	}
}

template<typename T>
inline static void bindCalcKind(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"ВидРасчета\"");
	}
}

template<typename T>
inline static void bindAccount(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"Счет\"");
	}
}

template<typename T>
inline static void bindSubcKind(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"ВидСубконто\"");
	}
}

template<typename T>
inline static void bindChartOfAccount(T& binder, const CValue* pValue, int mod)
{
	switch(mod)
	{
	case 0:
	case 1:
		bindObject(binder, pValue, mod);
		break;
	case 2:
		bindKindOfObject(binder, pValue);
		break;
	default:
		binder.error("Неизвестный модификатор для типа \"ПланСчетов\"");
	}
}

template<typename T>
static void bindValue(T& binder, const CValue* pValue, int mod)
{
	if(-1 == mod)
	{
		CString text;
		ValueStrWork<tos23>::val2str(*pValue, text);
		binder.bindText(text);
		return;
	}
	switch(pValue->type)
	{
	case typeUndefined:
		binder.bindNull();
		break;
	case typeNumber:
		bindNumeric(binder, pValue, mod);
		break;
	case typeString:
		bindString(binder, pValue, mod);
		break;
	case typeDate:
		bindDate(binder, pValue, mod);
		break;
	case typeEnum:
		bindEnum(binder, pValue, mod);
		break;
	case typeReference:
		bindReference(binder, pValue, mod);
		break;
	case typeDocument:
		bindDocument(binder, pValue, mod);
		break;
	case typeCalendar:
		bindCalendar(binder, pValue, mod);
		break;
	case typeCalcKind:
		bindCalcKind(binder, pValue, mod);
		break;
	case typeAccount:
		bindAccount(binder, pValue, mod);
		break;
	case typeSubcKind:
		bindSubcKind(binder, pValue, mod);
		break;
	case typeChartOfAcc:
		break;
	default:
		binder.error("Неизвестный тип значения");
	}
}
