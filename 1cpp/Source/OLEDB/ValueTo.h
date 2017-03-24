// ValueTo.h: interface for the CValueTo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUETO_H__C3A72190_4823_4CD1_A164_B861732A40D9__INCLUDED_)
#define AFX_VALUETO_H__C3A72190_4823_4CD1_A164_B861732A40D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

#include "sp_oledbptr.h"
#include "sp_ConvHlpr.h"
#include "..\\SQL\\MetaDataWork.h"

class CValueToDBTYPE  
{
public:
	DBTIME		 AsDBTIME();
	DBTIMESTAMP	 AsDBTIMESTAMP();
	DB_NUMERIC	 AsNUMERIC();
	GUID		 AsGUID();
	ULARGE_INTEGER AsUI8();
	LARGE_INTEGER AsI8();
	ULONG		 AsUI4();
	USHORT		 AsUI2();
	signed char	 AsI1();
	BYTE		 AsUI1();
	VARIANT_BOOL AsBOOL();
	DATE		 AsDATE();
	LARGE_INTEGER AsCY();
	DOUBLE		 AsR8();
	FLOAT		 AsR4();
	LONG		 AsI4();
	SHORT		 AsI2();
	DECIMAL		 AsDECIMAL();
	DBDATE		 AsDBDATE();
	CString		 AsSTR(BYTE nParamLen);
	
	CValueToDBTYPE(CValue* pVal, int nTypeCode) : m_pValue(pVal), m_nTypeCode(nTypeCode) {m_ConvHlpr.FinalConstruct();};
	virtual ~CValueToDBTYPE() {};

private:
	CValue* m_pValue;
	int m_nTypeCode;
	
	COLEDBConvertHelper m_ConvHlpr;
};

#endif // !defined(AFX_VALUETO_H__C3A72190_4823_4CD1_A164_B861732A40D9__INCLUDED_)
