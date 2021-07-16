#if !defined(AFX_GRIDCTRLEXT_H__FD104928_28FD_4344_BEB5_48FB4F415F80__INCLUDED_)
#define AFX_GRIDCTRLEXT_H__FD104928_28FD_4344_BEB5_48FB4F415F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridCtrlExt.h : header file
//
#include "Moxcel.h"//поддержка формата 1С
#include "../GridCtrl.h"

class CGridCtrlExt;
enum
{
	NONE_MODE,
	LEFT_MODE,
	UPPER_MODE,
};

/////////////////////////////////////////////////////////////////////////////
// CSectionCtrl window
//поддержка работы с секциями
class CSectionCtrl : public CWnd
{
// Construction
public:
	CSectionCtrl();

// Attributes
public:
	int nMode;
	CGridCtrlExt *pGrid;
// Operations
public:
	CPoint GetRange(CString);
	int FindInSection(int nCurRow,CString &csStr);
	CSectionArray *GetSectionsArray(){return &cSection;};
	int GetNSectionFromPt(CPoint point);

	void Add(int nRange1,int nRange2);
	void Remove(int nRange1,int nRange2);

	void InsertRow(int n);
	void RemoveRow(int n);

	BOOL EditName(int n);

	CSectionArray cSection;


	//серилизация
	void save(CFile &sf)
	{
		cSection.save(sf);
	};
	void load(CFile &sf)
	{
		cSection.load(sf);
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSectionCtrl();

public:
	int GetSize();

    BOOL Create(CGridCtrlExt *pSetGrid, CWnd* parent, UINT nID,int nSetMode);
	// Generated message map functions
public:
	//{{AFX_MSG(CSectionCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

    // Drawing
    virtual void  OnDraw(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};














/////////////////////////////////////////////////////////////////////////////
// CGridCtrlExt window

class CGridCtrlExt : public CGridCtrl
{
// Construction
public:
	CGridCtrlExt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrlExt)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridCtrlExt();

	void Clear();

	CMoxcel m_Data;
	BOOL bWasCreate;

	BOOL LoadMXL(CString csBuffer);
	BOOL LoadMXL(CMoxcel &cMxl);
	BOOL SaveMXL(CString &csBuffer);
	BOOL SaveMXL();
	//BOOL Save(CString &csBuffer,CGridCellBase* pCell);

	void ViewSection(int n=-1);
	BOOL IsViewSection(){return m_bViewSection;};
	void ViewHeader(BOOL);
	BOOL IsViewHeader();
	void ViewGridLines(BOOL);
	BOOL IsViewGridLines();

	void AddSection();
	void RemoveSection();

//поддержка работы с секциями
public:

	CPoint GetHorizRange(CString csName)
	{
		return m_Left.GetRange(csName);
	}
	CPoint GetVertRange(CString csName)
	{
		return m_Upper.GetRange(csName);
	}

protected:
// Attributes
	BOOL m_bViewSection;
	CSectionCtrl m_Left,m_Upper,m_TopLeft;

	BOOL GetWorkSection(int &nRange1,int &nRange2,CSectionCtrl *&pSectionCtrl);

public:
	//работа со свойствами
	CDocument *pDoc;
	void ShowProperty(bool bShow);
    virtual CCellID SetFocusCell(CCellID cell);
	CCell *GetOrigCell(){return &m_OrigCell;};
	void SetOrigCell(CCell *pSet){m_OrigCell=*pSet;};
protected:
	CCell m_OrigCell;//ячейка для сравнения



public:
	BOOL m_bHasModified;
    virtual void SetModified(BOOL bModified = TRUE, int nRow = -1, int nCol = -1);

	//переопределяем виртуальные методы для корректного отображения секций
    virtual int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE,
                      int nColumn = -1);
    virtual int  InsertRow(LPCTSTR strHeading, int nRow = -1);
    virtual BOOL DeleteColumn(int nColumn);
    virtual BOOL DeleteRow(int nRow);

	//Копирование
    virtual COleDataSource* CopyTextFromGrid();
    virtual BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);
public:
	CWnd* SetParent(CWnd *pWndNewParent);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGridCtrlExt)
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

    // Drawing
    virtual void  OnDraw(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRLEXT_H__FD104928_28FD_4344_BEB5_48FB4F415F80__INCLUDED_)
