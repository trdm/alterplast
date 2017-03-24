// referencetabinfo.h
#pragma once
#include "vtab_info.h"

class CReferenceTabInfo : public CVtabInfo
{
public:
	FIND_INFO("Reference", "Справочник", "ref")
	
	CReferenceTabInfo(CStringArray& arrOfNames);
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() != 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};
