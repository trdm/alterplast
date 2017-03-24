//////////////////////////////////////////////////////////////////////
// V7ODBCProvider.h: interface for ODBC data provider for CursorGrid
// control for V7.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxcoll.h>
#include "SQLProvider.h"
#include "V7DataProvider.h"
#include "../mycontextbase.h"
#include "../BLCollection.h"
#include "../ODBC/odbccore.h"
#include "../ODBC/1CPP_ODBCDatabase.h"
#include "../SQL/MetaDataWork.h"
#include <afxcoll.h>
#include "../ValueCollection.h"
#include "../istrmap.h"
#include "../ODBC/MetaNameParser.h"

class CV7ODBCProvider;
class CProviderRS;

class CV7SelectListItem : public CMyContextBase, public CSelectListItem
{
    friend class CV7ODBCProvider;
    friend class CProviderRS;
    DECLARE_DYNCREATE(CV7SelectListItem)
public:
    CV7SelectListItem();
    virtual ~CV7SelectListItem();

    BOOL GetPropName(CValue& Value) const;

    BOOL GetPropAutoDelete(CValue& Value) const;
    BOOL SetPropAutoDelete(CValue const& Value);

    BOOL GetPropQuickSearchType(CValue& Value) const;
    BOOL SetPropQuickSearchType(CValue const& Value);

    BOOL GetPropQuickSearchCondition(CValue& Value) const;
    BOOL SetPropQuickSearchCondition(CValue const& Value);

	BOOL GetPropType(CValue& Value) const;
    BOOL SetPropType(CValue const& Value);

protected:
    virtual void Release() {DecrRef();};
    BOOL IsTypeEqual(const CType& type) const;

	DECLARE_MY_CONTEXT()

private:
	CType m_rt;
    e1CAddType m_AddType;
    int m_AddFieldIndex;
    CString m_BLCTypeName;
    DataType m_QSType;
    CString m_strCondition;
};

class CV7SelectList : public CBLCollection<CSelectList, CV7SelectListItem, RUNTIME_CLASS(CV7SelectListItem), BLC_MODIFED>
{
    DECLARE_DYNAMIC(CV7SelectList)
public:
    CV7SelectList() {};
    virtual ~CV7SelectList() {};
protected:
    virtual void Release() {DecrRef();};
    virtual CSelectListItem* NewSelectListItem() {return new CV7SelectListItem;};
public:
    virtual char const* 	GetTypeString(void)const {return "ÏîëÿÄàííûõODBC";};
};

class CV7ODBCDataRow : public CSQLRow, public CV7DataRow
{
    friend class CV7ODBCProvider;
    friend class CProviderRS;
public:
    CV7ODBCDataRow(CV7ODBCProvider* pV7ODBCProvider);
    virtual ~CV7ODBCDataRow();

    //CDataRow
    virtual DataType GetFieldType(int nIndex);
    virtual void FormatField(int nIndex, CString& strValue);

    //CSQLRow
    virtual void SQLFormatField(int nIndex, CString& strValue);
    virtual BOOL CompareFields(int nIndex, CSQLRow* pRow) const;
    
    //CV7DataRow
    virtual CValue const& GetValue(int nIndex) const;
    
    static void SQLFormatValue(const CValue &value, CString& strValue);
private:
    CTypedPtrArray<CPtrArray, CValue*> m_values;
};

class CQueryParams : public CValueCollection
{
    friend class CV7ODBCProvider;
    DECLARE_DYNAMIC(CQueryParams)
public:
    CQueryParams();
    virtual ~CQueryParams();
    virtual char const* 	GetTypeString(void)const;

    virtual int				GetNProps(void)const;
	virtual int				FindProp(char const* szName)const;
	virtual char const* 	GetPropName(int nPropIndex, int nAlias)const;
	virtual int				GetPropVal(int nPropIndex, class CValue& Value)const;
	virtual int				SetPropVal(int nPropIndex, class CValue const& Value);
private:
    CValue* AddOrGet(LPCSTR szName);
    void Clear();
    class CItem : public CCollectionItem
    {
    public:
        CValue m_Value;
    };
    CCollection m_values;
};

class CV7ODBCSQLParam: public CSQLQueryParam
{
    friend class CV7ODBCProvider;
    friend class CProviderRS;
	friend class CV7ODBCDataRow;
public:
    CV7ODBCSQLParam() {};
    virtual ~CV7ODBCSQLParam() {};
	void FormatValue(CString& strValue);
	CString GetName(){return m_strLabel;};
private:
    SQLSMALLINT m_nSQLType;
    SQLSMALLINT m_nScale;
    SQLUINTEGER m_nPrecision;
    BOOL m_bIsOK;
    union
    {
        const CValue* m_pValue;
        int m_nFieldIndex;
    };
	CString m_strLabel;
};

class CProviderRS : public CODBCRecordset
{
    friend class CV7ODBCProvider;
private:
    CProviderRS(CV7ODBCProvider* pProvider);
    void Init();

	void InjectParamValues(CString& strQT, CV7ODBCDataRow* pRowFrom);
    BOOL ExecuteQuery(CV7ODBCDataRow* pRowFrom, int nRowsCount);
protected:
    virtual BOOL TypifyField(int nIndex, CDBField& field);
    virtual BOOL InternalMapFields() const;
    virtual int GetFieldIndex(const char* szFieldName) const;
    virtual CBLContext* GetVCtorParam();
private:
    CV7ODBCProvider* m_pProvider;
    static CBLPtr<CMetaDataWork> m_pMDW;
    CSQLParamMap m_params;
    int m_nRows;
};

class CV7ODBCProvider : public CMyContextBase, public CSQLProvider, public CV7DataProvider
{
    friend class CProviderRS;
    friend class CParamRow;
	friend class CV7ODBCDataRow;
    DECLARE_DYNCREATE(CV7ODBCProvider)
public:
    CV7ODBCProvider();
    virtual ~CV7ODBCProvider();
    
    //CDataProvider
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint);
    virtual CDataRow* Fetch();
    virtual BOOL RefreshRow(CDataRow* pRowFrom);
    virtual void OnFieldsChanged();

    virtual DataType GetQuickSearchType(LPCSTR szFieldName);
    virtual void QuickSearch(stQuickSearchPattern& QSPattern);
protected:
    virtual void OnAttach();

public:

    //CV7DataProvider
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

    //CBLContext
    virtual void InitObject(char const* szName);
    virtual char const* GetTypeString(void) const;

    BOOL MethSetOrderKey(CValue** ppValue);
    BOOL MethSetQueryText(CValue** ppValue);
    BOOL MethSetTextParam(CValue** ppValue);
    BOOL MethRefresh(CValue** ppValue); BOOL MethRefreshDefVal(int nIndex, CValue* pValue) const;
    BOOL MethSetIDField(CValue** ppValue);
	BOOL MethDebug(CValue** ppValue);
    BOOL funcGetQueryText(CValue& RetVal, CValue** ppValues);

    BOOL GetPropFields(CValue& Value) const;

    BOOL GetPropDatabase(CValue& Value) const;
    BOOL SetPropDatabase(CValue const& Value);
    
    BOOL GetPropParams(CValue& Value) const;

protected:
    virtual CSelectList* NewSelectList() {return new CV7SelectList;};
    virtual BOOL OnAddToSelectList(CSelectListItem* pSelectListItem);
    virtual CSQLQueryParam* NewSQLQueryParam(const CString& strName, const CStringList& params, CString&);
    virtual CSQLQueryParam* GetOrderKeyParam(int nIndex);

    DECLARE_MY_CONTEXT()
private:
    CString m_strType;
    static void InitTypes();

    CBLPtr<C1CPP_ODBCDatabase> m_pDatabase;

    CMetaNameParser* m_pMNParser;
    CQueryParams m_QueryParams;
    CV7ODBCSQLParam* m_pOKParams;

    CProviderRS* m_pUpRS;
    CProviderRS* m_pDownRS;
    CProviderRS* m_pTopRS;
    CProviderRS* m_pBottomRS;
    CProviderRS* m_pRefreshRS;

    CProviderRS* m_pIteratedRS;
    BOOL m_bRSIsOKDeterminator;

    void InitRecordsets(BOOL bInitOrderKey = FALSE);
    void FreeRecordsets();
    void DefineSQLParams(CProviderRS* pRS);

    CV7ODBCSQLParam m_IDFieldParam;
    BOOL m_bIDFieldParamDefined;
    CProviderRS* m_pRowByValueRS;

	BOOL m_bDebug;

};
