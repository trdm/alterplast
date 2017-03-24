// docheaders.h
#pragma once
#include "vtab_info.h"

class CDHTabInfo : public CVtabInfo
{
public:
	FIND_INFO("Document", "Документ", "doc")
	
	CDHTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};
