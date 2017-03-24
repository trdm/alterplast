// sp_oledbparamhlpr.h: interface for the COLEDBParamHlpr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SP_OLEDBPARAMHLPR_H__DB9EF92A_769D_40FF_9238_305832520ED1__INCLUDED_)
#define AFX_SP_OLEDBPARAMHLPR_H__DB9EF92A_769D_40FF_9238_305832520ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

#include "sp_oledbptr.h"
#include "sp_oledbparam.h"
#include "ValueTo.h"

typedef CComPtr<ICommandWithParameters> ICommandWithParameters_;
typedef CComPtr<IAccessor> IAccessor_;

typedef COLEDBMemPtr<ULONG> ULONG_;
typedef COLEDBMemPtr<DBBINDING> DBBINDING_;
typedef COLEDBMemPtr<DBPARAMBINDINFO> DBPARAMBINDINFO_;
typedef COLEDBMemPtr<BYTE> BYTE_;

//_____________________________________________________________________________
//
class COLEDBParamHlpr  
{
public:
	DBPARAMS* GetDBPARAMS();
	
	int PrepareParamsBuf(CString& strError);
	void UnprepareParamsBuf();

	int GetFullSize();

	BOOL GetParamProps(int nIndex, CValue* pVal, BYTE& pLength, WORD& pType, DWORD& pIO, BYTE& pPrec, ULONG& pOrdinal, long& pSize);
	long GetParamSize(int nIndex);
	ULONG GetParamOrdinal(int nIndex);
	BYTE GetParamPrecision(int nIndex);
	DWORD GetParamIO(int nIndex);
	BYTE GetParamType(int nIndex);
	BYTE GetParamLength(int nIndex);
	CValue& GetParamValue(int nIndex);
	
	BOOL SetParamValue(int nOrdinal, CValue* pValue, CString& strError);
	
	BOOL DeleteParam(int nParamNum);
	void DeleteParams();
	
	void AddParam(DWORD dwParamIO, WORD nType, BYTE m_Length, BYTE m_Precision);
	int GetCount();
	
	COLEDBParamHlpr();
	virtual ~COLEDBParamHlpr();
public:
	static long GetTypeSize(WORD nType);

private:
	BOOL FillParamsBuf();
	
	BYTE_ m_pParamsBuffer;
	int m_nFullSize;
	DBPARAMS m_Params;

	CList<COLEDBParam*, COLEDBParam*> m_ParamList;
	COLEDBParam* GetAt(int nIndex);

public:
	DBPARAMBINDINFO_ m_pParamBindInfo;
	DBBINDING_ m_pDBBinding;
	ULONG_ m_pParamOrdinals;
	ICommandWithParameters_ m_pICmdWithParams;
	HACCESSOR m_hPrmAccessor;
	IAccessor_ m_pIAccessor;
};

#endif // !defined(AFX_SP_OLEDBPARAMHLPR_H__DB9EF92A_769D_40FF_9238_305832520ED1__INCLUDED_)
