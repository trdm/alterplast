// journal.h
#pragma once
#include "vtab_info.h"

class CJournInfo : public CVtabInfo
{
public:
	FIND_INFO("Journal", "Журнал", "jrn")
	
	CJournInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 1)
			return FALSE;
		strName.Empty();
		return TRUE;
	}
		
protected:
};
