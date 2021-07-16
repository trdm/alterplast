// GridView.h : interface of the CGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GridView_H__19E18FBF_DFF6_11D1_8CE7_000000000000__INCLUDED_)
#define AFX_GridView_H__19E18FBF_DFF6_11D1_8CE7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../GridCtrl.h"
#include "GridDoc.h"
#include "GridCtrlExt.h"

class CGridView : public CView
{
protected: // create from serialization only
	CGridView();
	DECLARE_DYNCREATE(CGridView)

// Attributes
public:
	CGridDoc* GetDocument();
	CGridCtrlExt* m_pGridCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL


	void OnGridBeginDelete(NMHDR *pNotifyStruct, LRESULT* pResult);
	void OnGridEndDelete(NMHDR *pNotifyStruct, LRESULT *pResult);
	void OnGridBeginPaste(NMHDR *pNotifyStruct, LRESULT*  pResult);
	void OnGridEndPaste(NMHDR *pNotifyStruct, LRESULT* pResult);

	afx_msg int OnPropertyUpdateMessage(WPARAM, LPARAM);

// Implementation
public:
	virtual ~CGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnGridCovered();
	afx_msg void OnUpdateGridCovered(CCmdUI* pCmdUI);
	afx_msg void OnGridAddSection();
	afx_msg void OnGridHeader();
	afx_msg void OnUpdateGridHeader(CCmdUI* pCmdUI);
	afx_msg void OnGridReadonly();
	afx_msg void OnUpdateGridReadonly(CCmdUI* pCmdUI);
	afx_msg void OnGridSection();
	afx_msg void OnGridSectionRemove();
	afx_msg void OnGridViewSection();
	afx_msg void OnUpdateGridViewSection(CCmdUI* pCmdUI);
	afx_msg void OnGridSetka();
	afx_msg void OnUpdateGridSetka(CCmdUI* pCmdUI);
	afx_msg void OnEditAdd();
	afx_msg void OnEditDel();
	afx_msg void OnFormatCells();
	afx_msg void OnChangingRowHeight();
	afx_msg void OnChangingColWidth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GridView.cpp
inline CGridDoc* CGridView::GetDocument()
   { return (CGridDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GridView_H__19E18FBF_DFF6_11D1_8CE7_000000000000__INCLUDED_)
