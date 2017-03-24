// DBPropSet.h: interface for the CDBPropSet class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPROPSET_H__B18E562E_D97F_4FE5_B4F7_EE7115FF2127__INCLUDED_)
#define AFX_DBPROPSET_H__B18E562E_D97F_4FE5_B4F7_EE7115FF2127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

class CDBPropSet : public tagDBPROPSET  
{
public:
	CDBPropSet();
	CDBPropSet(const GUID& guid);
	CDBPropSet(const CDBPropSet& propset);
	virtual ~CDBPropSet();
	
	CDBPropSet& operator=(CDBPropSet& propset);

	void SetGUID(const GUID& guid);
	void InternalCopy(const CDBPropSet& propset);

	bool AddProperty(DWORD dwPropertyID, LPCSTR szValue);
	bool AddProperty(DWORD dwPropertyID, LPCWSTR szValue);
	bool AddProperty(DWORD dwPropertyID, bool bValue);
	bool AddProperty(DWORD dwPropertyID, BYTE bValue);
	bool AddProperty(DWORD dwPropertyID, short nValue);
	bool AddProperty(DWORD dwPropertyID, long nValue);
	bool AddProperty(DWORD dwPropertyID, float fltValue);
	bool AddProperty(DWORD dwPropertyID, double dblValue);
	bool AddProperty(DWORD dwPropertyID, CY cyValue);

	bool Add();
};

#endif // !defined(AFX_DBPROPSET_H__B18E562E_D97F_4FE5_B4F7_EE7115FF2127__INCLUDED_)
