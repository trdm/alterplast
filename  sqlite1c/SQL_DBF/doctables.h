// doctables.h
#pragma once
#include "vtab_info.h"

class CDTTabInfo : public CVtabInfo
{
public:
	FIND_INFO("DocumentLines", "ДокументСтроки", "docl")
	
	CDTTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};
