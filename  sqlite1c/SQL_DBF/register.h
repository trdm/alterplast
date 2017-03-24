// register.h
#pragma once
#include "vtab_info.h"

class CRegTabInfo : public CVtabInfo
{
public:
	FIND_INFO("Register", "–егистр", "reg")
	
	CRegTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};

class CRegTotalsTabInfo : public CVtabInfo
{
public:
	FIND_INFO("RegisterTotals", "–егистр»тоги", "regt")
	
	CRegTotalsTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};

