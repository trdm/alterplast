#pragma once

#include "../mycontextbase.h"
#include "RowFormat.h"

#define ID_FIND_FORWARD  0x8043
#define ID_FIND_BACKWARD 0x8044
#define ID_MEM_SAV 0x83AC
#define ID_MEM_ADD 0x8042
#define ID_MEM_SUB 0x8041

class CDataProviderScaner
{
public:
	CDataProviderScaner();
	~CDataProviderScaner();
	void Init(CV7CursorGridCtrl* pV7Grid, CRowFormat*, CDataProvider*, CGridColumns*);
	void SetStartPosition(CDataRowPtr const& , int nCurColIndex);
	void SetSearchConditions(CString str, CValue val, BOOL bBack, BOOL bCS, BOOL bOneCol);
	int Find(BOOL bFromBegin);
	CString& const GetSearchString() {return m_str;};
	CDataRow* GetCurrentRow();
	CDataRow* GetFindRow();
	CGridColumn* GetCurrentColumn();

private:
	BOOL EscCheck(CString strMsg);
	BOOL EscCheck(UINT ResID);
	
	BOOL NextRow();
	std::list<CDataRowPtr> m_rows;
	int m_nScanCnt;

private:
	CV7CursorGridCtrl* m_pV7Grid;
	CRowFormat* m_pRowFormat;
	CGridColumns* m_pColumns;
	CDataProvider* m_pProvider;
	CDataRowPtr m_pCurDataRow;
	CDataRowPtr m_pPosDataRow;
	int m_nCurColumnIndex;
	BOOL m_bIsStringFind;

	CString m_str;
	CValue m_val;
	BOOL m_bBack;
	BOOL m_bCS;
	BOOL m_bOneCol;
};

class CTableFieldGetCtrl : public CEditGet
{
public:
	CTableFieldGetCtrl():CEditGet(0,0){};
	virtual ~CTableFieldGetCtrl(void){};
	virtual BOOL PreTranslateMessage(struct tagMSG *);
	virtual void SetCtrlText(char const *);
	virtual void GetCtrlText(char *,int);
	virtual void GetCtrlText(class CString &);
	virtual void SetReadOnly(int);
	virtual int DataExchange(int);
	virtual void Enable(int);

	DECLARE_MESSAGE_MAP()

protected:
	//{{AFX_MSG(CTableFieldGetCtrl)
	afx_msg BOOL OnFindCommand(UINT dwID);
	afx_msg BOOL OnBufferCommand(UINT dwID);
	afx_msg void OnUpdateCommandUI(CCmdUI*);
	//}}AFX_MSG

private:
	CString GetCurrentCellText();
};


class CV7GlobalFindData : public CMyContextBase
{
	DECLARE_DYNCREATE(CV7GlobalFindData)
public:
	CV7GlobalFindData();
	virtual ~CV7GlobalFindData();
	
	BOOL GetPropValue(CValue& Value)const;
	BOOL SetPropValue(CValue const& Value);
	BOOL GetPropString(CValue& Value)const;
	BOOL SetPropString(CValue const& Value);
	BOOL GetPropBack(CValue& Value)const;
	BOOL SetPropBack(CValue const& Value);
	BOOL GetPropCS(CValue& Value)const;
	BOOL SetPropCS(CValue const& Value);
	BOOL GetPropOneCol(CValue& Value)const;
	BOOL SetPropOneCol(CValue const& Value);

	DECLARE_MY_CONTEXT()

private:
	CValue m_val;
	CString m_str;
	BOOL m_bBack;
	BOOL m_bCS;
	
	CValue* m_pval;
	CString* m_pstr;
	BOOL* m_pbBack;
	BOOL* m_pbCS;
};
