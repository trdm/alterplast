// calcjournal.h
#pragma once
#include "vtab_info.h"

class CCJTabInfo : public CVtabInfo
{
public:
	FIND_INFO("CalcJournal", "ЖурналРасчета", "cj")
	
	CCJTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};

