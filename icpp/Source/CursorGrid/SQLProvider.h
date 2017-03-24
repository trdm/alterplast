/////////////////////////////////////////////////////////////////////
// SQLProvider.h : interface of helper class for implementation data
// providers used for CursorGridCtrl
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Copyright (c) 2005-2008. All Rights Reserved.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataProvider.h"
#include "commons.h"

#define IS_FIELD        0x00000001
#define AUTO_DELETE     0x00000002
#define IS_ORDERKEY     0x00000004
#define BACK_ORDER      0x00000008
#define IS_IDFIELD      0x00000010

enum QueryStyle
{
    qsMSSQL,
    qsVFP,
    qsMySQL,
    qsOracle,

    qsLastStyle
};

class CSQLProvider;
class CSQLRow;

class CSQLQueryParam
{
public:
    CSQLQueryParam() {};
    virtual ~CSQLQueryParam() {};
	virtual CString GetName() = 0;
};

typedef CTypedPtrArray<CPtrArray, CSQLQueryParam*> CSQLParamArray;

class CSQLParamMap : public CMapStringToPtr
{
public:
	void Append(CSQLParamArray& params)
	{
		for (int i=0; i<params.GetSize(); i++)
			Add(params[i]);
	};
	void Add(CSQLQueryParam* pParam)
	{
		CString name = pParam->GetName();
		void* nothing;
		if (!Lookup(name, nothing))
			SetAt(name, pParam);
	};
};

class CSelectListItem : public CCollectionItem
{
	struct CSelectListItemLess
	{
		inline bool operator()(CSelectListItem* lhs, CSelectListItem* rhs);
	};

    friend class CSQLProvider;
    friend class CSelectList;
    friend class CSQLRow;
//	friend CSelectListItemLess; // vs6 compatibility

public:
    CSelectListItem();
    virtual ~CSelectListItem();

    LPCSTR GetExpression() const;
    BOOL IsOrderKey() const;
    BOOL GetBackOrder() const;
    BOOL GetAutoDelete() const;
    void SetAutoDelete(BOOL bAutoDelete = TRUE);
    int GetFieldIndex() const;
protected:
    virtual void Release() {delete this;};
private:
    CString m_strExpression;
    DWORD m_nFlags;
    UINT m_nFieldIndex;
    CSQLParamArray m_params;
};

class CSelectList : private CCollection
{
    friend class CSQLProvider;
public:
    CSelectList();
    virtual ~CSelectList();

    int GetSize() const;
    CSelectListItem* Add(LPCSTR szFieldName);
    CSelectListItem* Insert(int nIndex, LPCSTR szFieldName);
    CSelectListItem* GetAt(int nIndex) const;
    CSelectListItem* GetAt(LPCSTR szFieldName) const;
    int IndexOf(LPCSTR szFieldName) const;
    int IndexOf(CSelectListItem* pSelectListItem) const;
    BOOL Remove(int nIndex);
    BOOL Move(int nIndex, int nOffset) {return FALSE;};
    void RemoveAll();
protected:
    virtual void Release() {delete this;};
    virtual CSelectListItem* NewSelectListItem() {return new CSelectListItem;};
private:
    //virtual void OnAdd(CCollectionItem* pItem);
    virtual void OnRemove(CCollectionItem* pItem, int nIndexWas);
    //virtual void OnRename(CCollectionItem* pItem);

    CSQLProvider* m_pProvider;
};

class CSQLRow : public CDataRow
{
public:
    CSQLRow(CSQLProvider* pSQLProvider);
    virtual ~CSQLRow();

    //CDataRow
    virtual int GetFieldCount();
    virtual int GetFieldIndexOf(LPCSTR szFieldName);
    virtual LPCSTR GetFieldName(int nIndex);
    virtual DataType GetFieldType(int nIndex) = 0;
    virtual void FormatField(int nIndex, CString& strValue) = 0;

    virtual void SQLFormatField(int nIndex, CString& strValue) = 0;
    virtual BOOL CompareFields(int nIndex, CSQLRow* pRow) const = 0;

    CSQLProvider* GetProvider() {return m_pSQLProvider;};
private:
    CSQLProvider* m_pSQLProvider;
};

class CSQLProvider: public CDataProvider
{
    friend class CSQLRow;
    friend class CSelectList;

public:
	CSQLProvider();
	virtual ~CSQLProvider();
    
    //Query rows interface
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint) = 0;
    virtual CDataRow* Fetch() = 0;
    virtual BOOL RefreshRow(CDataRow* pRowFrom) = 0;
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const;
    virtual LPCSTR GetLastError() {return m_strLastError;};
    virtual void QueryField(LPCSTR szFieldName);
    virtual void ReleaseField(LPCSTR szFieldName);

protected:
    virtual void OnAttach() = 0;
    
public:
    virtual void Init(QueryStyle queryStyle);
    virtual BOOL SetQuery(LPCSTR szQueryText);
    virtual BOOL SetOrderKey(LPCSTR szOrderKey);
    virtual BOOL SetIDField(LPCSTR szName);
    BOOL BuildQuery(CString& strQT, CSQLRow* pRowFrom, int nRowsCount, CSQLParamMap& params);
    BOOL BuildQuickSearchQuery(CString& strQT, CSelectListItem* pSearchItem, CSQLParamMap& params, LPCSTR szCondition, LPCSTR szSearchLabel);
    BOOL BuildByIDQuery(CString& strQT, CSQLParamMap& params, LPCSTR szIDText, CSQLQueryParam* pIDParam);
    CSelectList* GetSelectList() const;
    int GetOrderKeyFieldCount() const;
    CSelectListItem* GetOrderKeyField(int nIndex) const;
    CSelectListItem* GetIDField() const;
    int GetQueryFieldCount() const;
    CSelectListItem* GetQueryField(int nIndex) const;
    CSelectListItem* GetQueryField(LPCSTR szName) const;
    int IndexOf(LPCSTR szName) const;
    QueryStyle GetQueryStyle() const {return m_QueryStyle;};
protected:
    virtual CSelectList* NewSelectList() {return new CSelectList;};
    virtual BOOL OnAddToSelectList(CSelectListItem* pSelectListItem) {return TRUE;};
    CString m_strLastError;
    virtual CSQLQueryParam* NewSQLQueryParam(const CString& strName, const CStringList& params, CString&) = 0;
    virtual CSQLQueryParam* GetOrderKeyParam(int nIndex) = 0;
private:
    QueryStyle m_QueryStyle;
    BOOL ParseQuery(CString& strQueryText);
    BOOL FindKeyword(LPCSTR szText, LPCSTR szKeyword, int& nResult);
    BOOL ParseSelectListItem(CString& strText);
    void ClearFields();
    void AddLimitRowCount(CString& strQT, int nCount);
    BOOL RemoveComments(CString& strQT);

    CSelectList* m_pSelectList;
    CString m_strFrom;
    CString m_strWhere;
    CTypedPtrArray<CPtrArray, CSelectListItem*> m_fields;
    CTypedPtrArray<CPtrArray, CSelectListItem*> m_orderkey;
    
    //the sql query params
    CSQLParamArray m_FromParams;
    CSQLParamArray m_WhereParams;
    BOOL ParseSQLParams(CString& strText, CSQLParamArray& params);
    void ClearQuery();
    void ClearParams(CSQLParamArray& params);

    //
    CSelectListItem* m_pIDField;
    CSelectListItem* IncludeToFields(LPCSTR szName);
	void ReorderFields();
};
