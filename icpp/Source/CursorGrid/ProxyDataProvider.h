// DataProvider.h: interface for the CProxyDataProvider class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPROVIDER_H__37C851C2_A5B4_4692_9102_2367F3A9A545__INCLUDED_)
#define AFX_DATAPROVIDER_H__37C851C2_A5B4_4692_9102_2367F3A9A545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../blptr.h"
#include "../mycontextbase.h"
#include "../CursorGrid/DataProvider.h"
#include "../CursorGrid/V7DataProvider.h"

class CProxyDataProvider;
class CBaseDataProvider;

enum enProxyMethods
{
	//DataRow
	pmGetFieldCount,
	pmGetFieldIndexOf,
	pmGetFieldName,
	pmGetFieldType,
	pmFormatField,
	pmGetRowIndex,
	pmGetValue,
	//Provider
	pmQueryRows,
	pmFetch,
	pmRefreshRow,
	pmCompareRows,
	pmGetLastError,
	pmGetRowCount,
	pmQueryField,
	pmReleaseField,
	pmGetQuickSearchType,
	pmQuickSearch,
	pmOnAttach,
	pmOnDetach,
	pmBuildRowByValue,
	pmLast
};

//////////////////////////////////////////////////////////////////////////
//CProxyDataRow
//////////////////////////////////////////////////////////////////////////
class CProxyDataRow : public CDataRow, public CV7DataRow
{
    friend class CProxyDataProvider;
public:
    CProxyDataRow(CValue &Val, CBLContext* pProvm, int* ProxyMethNums);
    virtual ~CProxyDataRow();

    //CDataRow
    virtual int GetFieldCount();
    virtual int GetFieldIndexOf(LPCSTR szFieldName);
    virtual LPCSTR GetFieldName(int nIndex);
    virtual DataType GetFieldType(int nIndex);
    virtual void FormatField(int nIndex, CString& strValue);
    virtual UINT GetRowIndex();
	
    //CV7DataRow
    virtual const CValue& GetValue(int nIndex) const;

private:
	CValue m_DataRowVal;
	mutable CValue m_val;
	CBLContext* m_pProv;
	int* m_ProxyMethNums;
};

//////////////////////////////////////////////////////////////////////////
//CProxyDataProvider
//////////////////////////////////////////////////////////////////////////

//class CProxyDataProvider : public CContextImpl<CProxyDataProvider>, public CDataProvider, public CV7DataProvider
class CProxyDataProvider : public CMyContextBase, public CDataProvider, public CV7DataProvider
{
	friend CBaseDataProvider;
	DECLARE_DYNCREATE(CProxyDataProvider);
public:
	CProxyDataProvider();
	virtual ~CProxyDataProvider();
	static DataType NumericToDataType(const CNumeric &num);
	static long DataTypeToLong(DataType dt);

	//BL_BEGIN_CONTEXT("ProxyDataProvider", "ПроксиПоставщикДанных");
	//BL_PROC(SetProvider, "УстПоставщика", 1);
	//BL_END_CONTEXT();
	BOOL SetProvider(CValue** ppValue);
	DECLARE_MY_CONTEXT()

private:
	CBLPtr<CBLContext> m_pProv;

    //CDataProvider
public:
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint);
    virtual CDataRow* Fetch();
    virtual BOOL RefreshRow(CDataRow* pRowFrom);
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const;
    virtual LPCSTR GetLastError();
    virtual BOOL GetRowCount(UINT* pCount);

    virtual void QueryField(LPCSTR szFieldName);
    virtual void ReleaseField(LPCSTR szFieldName);
    virtual void OnFieldsChanged();

    virtual DataType GetQuickSearchType(LPCSTR szFieldName);
    virtual void QuickSearch(stQuickSearchPattern& QSPattern);
	
    virtual void OnAttach();
    virtual void OnDetach();

    //CV7DataProvider
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

private:
	void UpdateProxeMethNums();
	int m_ProxyMethNums[pmLast];

	void InvokeSetCurrentRow(CValue& val);
	void InvokeResetData(CValue& val);
};

//////////////////////////////////////////////////////////////////////////
//CBaseDataProvider
//////////////////////////////////////////////////////////////////////////


class CBaseDataProvider : public CMyContextBase
{
    DECLARE_DYNCREATE(CBaseDataProvider);
public:
	CBaseDataProvider();
	virtual ~CBaseDataProvider();
	void SetProxy(CProxyDataProvider* pProxy){m_pProxy = pProxy;};
    
	BOOL MethGetControl(CValue& Value,CValue** ppValue);
	BOOL MethSetControl(CValue** ppValue);
	BOOL MethResetData(CValue** ppValue);
	BOOL MethSetCurrentRow(CValue** ppValue);
	BOOL MethOnFieldsChanged(CValue** ppValue);
    DECLARE_MY_CONTEXT()

private:
	CProxyDataProvider* m_pProxy;
};


#endif // !defined(AFX_DATAPROVIDER_H__37C851C2_A5B4_4692_9102_2367F3A9A545__INCLUDED_)
