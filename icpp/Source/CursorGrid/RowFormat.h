#pragma once

#include "CursorGridCtrl.h"
#include "../mycontextbase.h"
#include "../BLCollection.h"

class CCellFormat : public CMyContextBase
{
	friend class CV7GridColumns;
	friend class CRowFormat;
	DECLARE_DYNCREATE(CCellFormat)
public:
	CCellFormat();
	virtual ~CCellFormat();

	LPCSTR GetName() const;
	BOOL GetPropName(CValue& Value)const;

	BOOL MethSetText(CValue** ppValue);
	BOOL GetPropShowText(CValue& Value)const;  //для повторения интерфейса ОформленияЯчейки
	BOOL SetPropShowText(CValue const& Value); //для повторения интерфейса ОформленияЯчейки
	BOOL GetPropText(CValue& Value)const;
	BOOL SetPropText(CValue const& Value);
	DECLARE_MY_CONTEXT()

	CString& GetText() { return m_strText; };
	void SetText(CString str) { m_strText = str; };
private:
	CString m_strText;
	BOOL m_bShowText;
};

class CRowFormat
{
	friend class CV7CursorGridCtrl;
public:
	CRowFormat();
	~CRowFormat();
public:
	int GetSize() const;
	CCellFormat* Add(LPCSTR szName) {return NULL;};
	CCellFormat* Insert(int nIndex, LPCSTR szName) {return NULL;};
	CCellFormat* GetAt(int nIndex) const;
	CCellFormat* GetAt(LPCSTR szName) const;
	int IndexOf(LPCSTR szName) const;
	int IndexOf(CCellFormat* pItemType) const;
	BOOL Remove(int nIndex) {return FALSE;};
	BOOL Move(int nIndex, int nOffset) {return FALSE;};
	void RemoveAll() {};

public:
	void SetColumns(CGridColumns* pColumns);
	void Format(const CDataRowPtr &pDataRow);

private:
	CDataRow* GetDataRow();

private:
	CGridColumns* m_pColumns;
	CDataRowPtr m_pDataRow;
};

class CV7RowFormat : public CBLCollection<CRowFormat, CCellFormat, RUNTIME_CLASS(CCellFormat), BLC_READONLY>
{
	DECLARE_DYNAMIC(CV7RowFormat)
public:
	CV7RowFormat() {};
	virtual ~CV7RowFormat() {};
};
