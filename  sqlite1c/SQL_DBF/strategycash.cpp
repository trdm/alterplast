// strategycash.cpp
#include "StdAfx.h"
#include "strategycash.h"

BOOL StrategyCash::getFromCash(sqlite3_index_info* pIdx, CString& cashKey)
{
	// Строим строку
	cashKey.Empty();
	DWORD c = pIdx->nConstraint;
	for(constraint_ptr ptr = pIdx->aConstraint; c--; ptr++)
	{
		if(isConstraint(ptr))
		{
			DWORD val = ptr->iColumn;
			do
			{
				DWORD mod = val % 36;
				val /= 36;
				cashKey += mod < 10 ? '0' + mod : 'A' + mod - 10;
			}while(val);
			switch(ptr->op)
			{
			case SQLITE_INDEX_CONSTRAINT_EQ:
				cashKey += '=';
				break;
			case SQLITE_INDEX_CONSTRAINT_GT:
				cashKey += '>';
				break;
			case SQLITE_INDEX_CONSTRAINT_LE:
				cashKey += "<=";
				break;
			case SQLITE_INDEX_CONSTRAINT_LT:
				cashKey += '<';
				break;
			case SQLITE_INDEX_CONSTRAINT_GE:
				cashKey += ">=";
				break;
			}
		}
	}
	cashKey += ';';
	orderby_ptr pOrd = pIdx->aOrderBy;
	for(c = pIdx->nOrderBy; c--; pOrd++)
	{
		DWORD val = pOrd->iColumn;
		do
		{
			DWORD mod = val % 36;
			val /= 36;
			cashKey += mod < 10 ? '0' + mod : 'A' + mod - 10;
		}while(val);
		cashKey += pOrd->desc ? '<' : '>';
	}
	// Строка построена. Будем искать в кэше.
	cash_entry* node = NULL;
	for(POSITION pos = cash.GetHeadPosition(); pos ;)
	{
		POSITION next = pos;
		node = (cash_entry*)cash.GetNext(next);
		if(node->cashKey == cashKey)
			break;
		pos = next;
	}
	
	if(!pos)
		return FALSE;

	// Нашли запись. Заполним инфу.
	usage_ptr pUsg = pIdx->aConstraintUsage;
	ptr = pIdx->aConstraint;
	c = pIdx->nConstraint;
	for(DWORD i = 0, *pArg=(DWORD*)node->args.GetData(); c--; ptr++, pUsg++)
	{
		if(isConstraint(ptr))
		{
			pUsg->argvIndex = *pArg++;
			pUsg->omit = TRUE;
		}
	}
	pIdx->idxNum = node->idxNum;
	pIdx->orderByConsumed = node->bOrderBy;
	int strLen = node->idxStr.GetLength() + 1;
	pIdx->idxStr = (char*)sqlite3_malloc(strLen);
	memcpy(pIdx->idxStr, (LPCSTR)node->idxStr, strLen);
	pIdx->needToFreeIdxStr = TRUE;
	pIdx->estimatedCost = node->cost;

	// И переместим запись в начало списка.
	cash.RemoveAt(pos);
	cash.AddHead(node);
	return TRUE;
}

void StrategyCash::addToCash(const CString& cashKey, sqlite3_index_info* pIdx)
{
	cash_entry* node = new cash_entry;
	node->cashKey = cashKey;
	node->idxNum = pIdx->idxNum;
	node->idxStr = pIdx->idxStr;
	node->cost = pIdx->estimatedCost;
	node->bOrderBy = pIdx->orderByConsumed;
		

	constraint_ptr ptr = pIdx->aConstraint;
	usage_ptr pUsg = pIdx->aConstraintUsage;
	for(DWORD c = pIdx->nConstraint; c--; ptr++, pUsg++)
	{
		if(isConstraint(ptr))
			node->args.Add(pUsg->argvIndex);
	}
	cash.AddHead(node);
	if(cash.GetCount() > 32)
	{
		delete (cash_entry*)cash.GetTail();
		cash.RemoveAt(cash.GetTailPosition());
	}
}

StrategyCash::~StrategyCash()
{
	for(POSITION pos = cash.GetHeadPosition(); pos;)
		delete (cash_entry*)cash.GetNext(pos);
}
