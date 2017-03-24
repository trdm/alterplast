//systabs.h
#pragma once
#include "vtab_info.h"


class CCoreTabs : public CVtabInfo
{
public:
	FIND_INFO("_1S", "_1Ñ", "core")
	
	CCoreTabs(CStringArray& arrOfNames)
	{
		m_strTableName = CString("1S") + arrOfNames[1];
		m_strTableName.MakeUpper();
		m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
		if(m_pTable)
			fillTabInfo(CNoCaseMap<CString>());
	}
	static BOOL GetNameFrom(CStringArray& arrOfNames, CString& strName)
	{
		if(arrOfNames.GetSize() < 2)
			return FALSE;
		strName = arrOfNames[1];
		return TRUE;
	}
		
protected:
};
