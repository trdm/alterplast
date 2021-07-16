// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_MYEDIT_H__7CBDE109_15D2_4483_BC50_015E034DFB26__INCLUDED_)
#define AFX_MYEDIT_H__7CBDE109_15D2_4483_BC50_015E034DFB26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

#include "DropTarget/MyDropTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window
class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:
	CString	csEventName;
	CString	csIdName;
	BOOL	m_bWasLDown;

    //CMyDropTarget m_droptarget;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMyButton window
class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__7CBDE109_15D2_4483_BC50_015E034DFB26__INCLUDED_)
