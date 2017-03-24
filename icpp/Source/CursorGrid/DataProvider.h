/////////////////////////////////////////////////////////////////////
// DataProvider.h : interface for data providers used for CursorGridCtrl
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Copyright (c) 2005-2008. All Rights Reserved.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum RegionType {rtNone, rtHeader, rtHead, rtBody, rtTail, rtBodyNSpace};
enum DataType
{
    dtUndefined,
    dtText,
    dtNumeric,
    dtDate,
    dtFalse,
    dtTrue
};

class CDataRow
{
public:
	virtual ~CDataRow() {}
	virtual int GetFieldCount() = 0;
    virtual int GetFieldIndexOf(LPCSTR szFieldName) = 0;
    virtual LPCSTR GetFieldName(int nIndex) = 0;
    virtual DataType GetFieldType(int nIndex) {return dtUndefined;};
    virtual void FormatField(int nIndex, CString& strValue) = 0;
    virtual UINT GetRowIndex() {return 0;};
};

typedef boost::shared_ptr<CDataRow> CDataRowPtr;

#define DP_DATA_CHANGE  (WM_USER + 0x0010)
//the wParam for DP_DATA_CHANGE:
#define DP_RESET        0x0000  //lParam is pointer to a CDataRow or may be NULL
#define DP_FIELDS       0x0001
#define DP_CURROW       0x0002  //lParam is pointer to a CDataRow must be not NULL

struct stQuickSearchPattern
{
    LPCSTR szFieldName;
    DataType dataType;
    union
    {
        CString* pTextData; //for text fields;
         //for numeric fields;
         //for data fields;
         //for some other fields;
    };
};

class CDataProvider
{
public:
    CDataProvider() : m_hCtrl(NULL), m_RegionType(rtNone) {};
    virtual ~CDataProvider() {};

public:
    HWND GetControl() const
    {return m_hCtrl;};
    void SetControl(HWND hControl)
    {
        m_hCtrl = hControl;
        if (m_hCtrl)
            OnAttach();
        else
            OnDetach();
    };

	RegionType GetRegionType() { return m_RegionType; };
	void SetRegionType(RegionType rType) { m_RegionType = rType; };

    void ResetData(CDataRow* pDataRow = NULL)
    {
        if (m_hCtrl)
			::SendMessage(m_hCtrl, DP_DATA_CHANGE, MAKEWORD(DP_RESET, (BYTE)m_RegionType), (LPARAM)pDataRow);
    };
	
    void SetCurrentRow(CDataRow* pDataRow)
    {
        if (m_hCtrl && pDataRow)
            ::SendMessage(m_hCtrl, DP_DATA_CHANGE, MAKEWORD(DP_CURROW, (BYTE)m_RegionType), (LPARAM)pDataRow);
    };

    //Query rows interface
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint) = 0;
    virtual CDataRow* Fetch() = 0;
    virtual BOOL RefreshRow(CDataRow* pRowFrom) = 0;
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const = 0;
    virtual LPCSTR GetLastError() {return NULL;};
    virtual BOOL GetRowCount(UINT* pCount) {return FALSE;};

    virtual void QueryField(LPCSTR szFieldName) {};
    virtual void ReleaseField(LPCSTR szFieldName) {};
    virtual void OnFieldsChanged()
    {
        if (m_hCtrl)
        {
#ifdef OPTIMIZE_FIELDS_CHANGE
            MSG msg;
            ::PeekMessage(&msg, m_hCtrl, DP_DATA_CHANGE, DP_DATA_CHANGE, PM_REMOVE);
            ::PostMessage(m_hCtrl, DP_DATA_CHANGE, DP_FIELDS, 0);
#else
            ::SendMessage(m_hCtrl, DP_DATA_CHANGE, MAKEWORD(DP_FIELDS, (BYTE)m_RegionType), 0);
#endif
        }
    };

    virtual DataType GetQuickSearchType(LPCSTR szFieldName) {return dtUndefined;};
    virtual void QuickSearch(stQuickSearchPattern& QSPattern) {};

protected:
    virtual void OnAttach() {};
    virtual void OnDetach() {};
private:
    HWND m_hCtrl;
	RegionType m_RegionType;
};
