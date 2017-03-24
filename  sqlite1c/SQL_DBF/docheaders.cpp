// docheaders.cpp
#include "StdAfx.h"
#include "docheaders.h"

CDHTabInfo::CDHTabInfo(CStringArray& arrOfNames)
{
	CDocDef* pDocDef = pMetaDataCont->GetDocDef(arrOfNames[1]);
	if(!pDocDef)
	{
		setError("Документ %s не найден.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pDocDef->GetHeadTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	if(!m_pTable)
	{
		setError("Документ %s не имеет шапки.", (LPCSTR)arrOfNames[1]);
		return;
	}
	
	CNoCaseMap<CString> aliaces;
	CDWordArray longStr;
	fillNamesFromObjs(pDocDef->GetHeads(), aliaces, &longStr);
	fillNamesFromObjs(pDocDef->GetTables(), aliaces);	// Там могут быть поля с "итог по колонке"
	fillNamesFromObjs(pMetaDataCont->GetGenJrnlFlds(), aliaces, &longStr);
	fillTabInfo(aliaces, &longStr);
}
