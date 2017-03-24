// Мирошниченко Кирилл aka spock.
// #163860836
// e-mail: spock@km.ru
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

#ifndef __msdasc_h__
#include <msdasc.h>
#endif // __msdasc_h__

#include "..\\ODBC\\MetaNameParser.h"
#include "..\\SQL\\MetaDataWork.h"

//trad
//_____________________________________________________________________________
//
class CIBSetOLEDB : public CIBSet
{
public:
    CDate GetDateTimeTA(CEventTime &TimeTA);
    int GetDataSourceType();
    enum PeriodType GetRestsSnapShotPeriod();
};
//

//_____________________________________________________________________________
//
class COLEDBData : public CMyContextBase
{
	DECLARE_DYNCREATE(COLEDBData);

public:
	COLEDBData();
	virtual ~COLEDBData();

	BOOL funcConnect(CValue& RetVal, CValue** ppValue);
	BOOL defsConnect(int nParam, CValue* param) const;
	
	BOOL funcCreateCommand(CValue& RetVal, CValue** ppValue);
	BOOL procClose(CValue** ppValue);

	BOOL funcAttachIB(CValue& RetVal, CValue** ppValue);
	BOOL defsAttachIB(int nParam, CValue* param) const;

private:
	CComPtr<IDBInitialize> m_pInit;
	CComPtr<IDBCreateCommand> m_pIDBCreateCommand;
	
	//trad
	//
	CMetaDataCont* m_pMetaDataCont;
	CIBSetOLEDB m_OLEDBIBSet;
	//

	void GetErrorDescription(CString& strError) const;
	
	bool m_bConnected;

	DECLARE_MY_CONTEXT();
};