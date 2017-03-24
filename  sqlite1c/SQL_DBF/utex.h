// utex.h
#pragma once
// Этот файл содержит утилиты для работы с UTF-16 и UTF-8 текстом
#include <fixalloc.h>

struct u8text
{
	static void init();
	//static CString toUtf8(const CString& strSource);
	static void toUtf8(CString& strWinText)
	{
		int sl = strWinText.GetLength() * 3 + 3;
		char buffer[1024];
		char *pWrite = (sl <= sizeof(buffer) ? buffer : new char[sl]), *pStart = pWrite;
		LPCSTR pRead = strWinText;
		while(DWORD s = static_cast<DWORD>(static_cast<BYTE>(*pRead)))
		{
			int d = s - 128;
			if(d >= 0)
			{
				DWORD us = w12512u[d];
				*((DWORD*)pWrite) = us;
				pWrite++;
				if(us & 0xFF0000)
					pWrite++;
			}
			else
				*pWrite = s;
			pWrite++;
			pRead++;
		}
		int newLen = pWrite - pStart;
		if(newLen != strWinText.GetLength())
			memcpy(strWinText.GetBufferSetLength(newLen), pStart, newLen);
		if(sl > sizeof(buffer))
			delete [] pStart;
	}

	static CString fromUtf8(LPCSTR strSrc);
	static void fromUtf8(LPCSTR strSrc, CString& res);
	static int _1Ccollate(void*, int l1, const void* str1, int l2, const void* str2);
	static int compareRtrimNoCase(LPCSTR str1, LPCSTR str2);
	static int compareRtrim(LPCSTR str1, LPCSTR str2);
	static int compareLen(LPCSTR str1, LPCSTR str2, DWORD len)
	{
		return memcmp(str1, str2, len);
	}
	static int compareNoCaseLen(LPCSTR str1, LPCSTR str2, DWORD len);
	static void dbUpper(CString& str);

private:
	static const DWORD w12512u[128];
	static CMapPtrToPtr uw1251;
};




