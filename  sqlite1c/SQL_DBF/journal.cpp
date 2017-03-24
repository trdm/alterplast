// journal.cpp
#include "StdAfx.h"
#include "journal.h"

CJournInfo::CJournInfo(CStringArray& /*arrOfNames*/)
{
	m_strTableName = "1SJOURN";
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	
	CNoCaseMap<CString> aliaces;
	CDWordArray longStr;
	fillNamesFromObjs(pMetaDataCont->GetGenJrnlFlds(), aliaces);

	for(int i = 0, c = pMetaDataCont->GetNRegDefs(); i < c; i++)
	{
		CRegDef* pReg = pMetaDataCont->GetRegDefAt(i);
		CString name;
		name.Format("%sÔð", pReg->m_Code);
		aliaces[pReg->GetFieldName()] = name;
	}

	CMetaDataObjArrayTemplate<class CDocStreamDef>* pStreams = pMetaDataCont->GetDocStreamDefs();
	for(i = 0, c = pStreams->GetNItems(); i < c ;i++)
	{
		CDocStreamDef* pDef = pStreams->GetAt(i);
		CString name;
		name.Format("%sÏñ", pDef->m_Code);
		aliaces[pDef->GetFieldName()] = name;
	}
	fillTabInfo(aliaces, &longStr);
}
