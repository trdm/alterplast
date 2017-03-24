// longstrreader.cpp
#include "StdAfx.h"
#include "longstrreader.h"

struct key : CKeyObj
{
	key(CIndex *pI) : CKeyObj(pI, 0, 0){}
	virtual void PrepareKey()
	{
		m_pStoreObj->FX_String(0, (char*)pFieldID, 4, 1);
		m_pStoreObj->FX_String(1, (char*)pObjID, 9, 1);
		m_pStoreObj->FX_String(2, "   0", 4, 1);
	}
	DWORD* pFieldID;
	const char* pObjID;
};


void LongStrReader::ReadStr(LPCSTR pRecordBufer, DWORD mdCharID, CString& val)
{
	key k(store.pIndex);
	k.pFieldID = &mdCharID;
	k.pObjID = pRecordBufer;
	if(store.Goto(&k, ccE, 0))
	{
		LPCSTR ptr = pReadBuf + 17;
		DWORD size = 0;
		for(;;)
		{
			DWORD s = static_cast<DWORD>(static_cast<BYTE>(*ptr++));
			if(s >= '0' && s <= '9')
				size = size * 16 + s - '0';
			else if(s != ' ')
				size = size * 16 + s -'a' + 10;
			else
				break;
		}
		LPSTR pWrite = val.GetBufferSetLength(size);
		if(size <= 71)
			memcpy(pWrite, pReadBuf + 17 + 9, size);
		else
		{
			memcpy(pWrite, pReadBuf + 17 + 9, 71);
			pWrite += 71;
			size -= 71;
			ptr = pReadBuf + 17;
			do
			{
				store.Goto(navNext, 0);
				int copy = size < 80 ? size : 80;
				memcpy(pWrite, ptr, copy);
				pWrite += copy;
				size -= copy;
			}while(size);
		}
	}
	else
		val.Empty();
}
