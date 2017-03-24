//////////////////////////////////////////////////////////////////////////
// CVTExtProvider class (c) kms 2008
// Indexed table data provider for CursorGrid control
//////////////////////////////////////////////////////////////////////////

#ifndef VTEXTPROVIDER_H_INCLUDED
#define VTEXTPROVIDER_H_INCLUDED

#include "DataProvider.h"
#include "V7DataProvider.h"
#include "../mycontextbase.h"
#include "../VTExt/VTExtended.h"
#include <memory>

class CVTExtDataRow : public CDataRow, public CV7DataRow
{
    friend class CVTExtProvider;
public:
    CVTExtDataRow(CVTExtended* pVT, UINT nRowIndex);
    virtual ~CVTExtDataRow();

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
    CVTExtended* m_pVT;
    UINT m_nRowIndex;
    CValue m_vRowIndex;
};

class CVTExtProvider : public CMyContextBase, public CDataProvider, public CV7DataProvider
{
    DECLARE_DYNCREATE(CVTExtProvider)
public:
    CVTExtProvider();
    virtual ~CVTExtProvider();

private:
	// CVTExtProvider
	BOOL FindRow(CVTExtIterator* pIterator, int nRowIndex);
	CVTExtRow* GetRow(int nRowIndex);
public:
	static BOOL IsRowNumberField(LPCSTR szFieldName);
private:
	void ReleaseIterator();

public:
	// CBLContext
    BOOL MethSetValueTable(CValue** ppValue);
    
	BOOL GetPropValueTable(CValue& Value) const;
	BOOL SetPropValueTable(CValue const& Value);

	BOOL GetPropIndexedTable(CValue& Value) const;
	BOOL SetPropIndexedTable(CValue const& Value);

	BOOL GetPropIndex(CValue& Value) const;
	BOOL SetPropIndex(CValue const& Value);

	DECLARE_MY_CONTEXT()

    // CDataProvider
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

    // CV7DataProvider
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

private:
    int m_nQueryRowsCount;
	CBLPtr<CVTExtended> m_pVT;
	CVTExtIndex* m_pIndex;
	auto_ptr<CVTExtIterator> m_pIterator;
};

#endif
