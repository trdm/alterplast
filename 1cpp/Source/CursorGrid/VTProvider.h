//////////////////////////////////////////////////////////////////////
// VTProvider.h: interface for Value table data provider for CursorGrid
// control for V7.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataProvider.h"
#include "V7DataProvider.h"
#include "../mycontextbase.h"

class CVTProvider;

class CVTRow : public CDataRow, public CV7DataRow
{
    friend class CVTProvider;
public:
    CVTRow(CValueTable* pVT, UINT nRowIndex);
    virtual ~CVTRow();

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
    CValueTable* m_pVT;
    UINT m_nRowIndex;
    CValue m_vRowIndex;
};

class CVTProvider : public CMyContextBase, public CDataProvider, public CV7DataProvider
{
    DECLARE_DYNCREATE(CVTProvider)
public:
    CVTProvider();
    virtual ~CVTProvider();

    BOOL MethSetValueTable(CValue** ppValue);
    
    DECLARE_MY_CONTEXT()

    //CDataProvider
public:
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint);
    virtual CDataRow* Fetch();
    virtual BOOL RefreshRow(CDataRow* pRowFrom);
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const;
    virtual BOOL GetRowCount(UINT* pCount);

    virtual DataType GetQuickSearchType(LPCSTR szFieldName);
    virtual void QuickSearch(stQuickSearchPattern& QSPattern);
protected:
    virtual void OnAttach();

    //CV7DataProvider
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

private:
    CBLPtr<CBLContext> m_pBLVT;
    CValueTableContextData* m_pVTContextData;
    int m_nQueryRowsCount;
    int m_nQueryRowIndex;
    int m_nCurrentRow;
};
