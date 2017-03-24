// referencetabinfo.cpp
#include "StdAfx.h"
#include "referencetabinfo.h"

CReferenceTabInfo::CReferenceTabInfo(CStringArray& arrOfNames)
{
	CSbCntTypeDef* pSbDef = pMetaDataCont->GetSTypeDef(arrOfNames[1]);
	if(!pSbDef)
	{
		setError("Справочник %s не найден.", (LPCSTR)arrOfNames[1]);
		return;
	}
	m_strTableName = pSbDef->GetTableName();
	m_pTable = static_cast<CTableEx*>(pDataDict->GetTable(m_strTableName));
	
	CNoCaseMap<CString> aliaces;
	CDWordArray longStr;
	fillNamesFromObjs(pSbDef->GetParams(), aliaces, &longStr);
	fillTabInfo(aliaces, &longStr);
}