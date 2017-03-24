//register.cpp
#include "StdAfx.h"
#include "register.h"

CRegTabInfo::CRegTabInfo(CStringArray& arrOfNames)
{
	CRegDef* pRegDef = pMetaDataCont->GetRegDef(arrOfNames[1]);
	if(!pRegDef)
	{
		setError("Регистр %s не найден.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pRegDef->GetRegActTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	
	CNoCaseMap<CString> aliaces;
	fillNamesFromObjs(pRegDef->GetFigure(), aliaces);
	fillNamesFromObjs(pRegDef->GetFilds(), aliaces);
	fillNamesFromObjs(pRegDef->GetProps(), aliaces);
	fillTabInfo(aliaces);
}

CRegTotalsTabInfo::CRegTotalsTabInfo(CStringArray& arrOfNames)
{
	CRegDef* pRegDef = pMetaDataCont->GetRegDef(arrOfNames[1]);
	if(!pRegDef)
	{
		setError("Регистр %s не найден.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pRegDef->GetRegTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	
	CNoCaseMap<CString> aliaces;
	fillNamesFromObjs(pRegDef->GetFigure(), aliaces);
	fillNamesFromObjs(pRegDef->GetProps(), aliaces);
	fillTabInfo(aliaces);
}

