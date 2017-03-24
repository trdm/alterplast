// FormCnt.h : header file
#ifndef AFX_FORMCNT_H
#define AFX_FORMCNT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CFormCnt form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "resource.h"

class CFormCnt : public CView
{
protected:
	CFormCnt();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormCnt)
	COleControlSite* m_pActiveObject;

// Form Data
public:
	//{{AFX_DATA(CFormCnt)
	enum { IDD = IDD_EMPTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	static void CreateForm();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormCnt)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC){}
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormCnt();

	// Generated message map functions
	//{{AFX_MSG(CFormCnt)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFormCnt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
