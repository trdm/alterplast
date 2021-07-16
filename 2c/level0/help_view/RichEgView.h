// RichEgView.h : interface of the CRichEgView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEGVIEW_H__8299E804_05A4_461C_BC54_5D38A7A149CA__INCLUDED_)
#define AFX_RICHEGVIEW_H__8299E804_05A4_461C_BC54_5D38A7A149CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


#include "RichEgDoc.h"

class CRichEgCntrItem;

class CRichEgView : public CRichEditView
{
protected: // create from serialization only
	CRichEgView();
	DECLARE_DYNCREATE(CRichEgView)

// Attributes
public:

// Operations
public:
	CRichEgDoc* GetDocument();


	void SetWindowText(CString Str);
	int GetWindowText(CString &Str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEgView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichEgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRichEgView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RichEgView.cpp
inline CRichEgDoc* CRichEgView::GetDocument()
   { return (CRichEgDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEGVIEW_H__8299E804_05A4_461C_BC54_5D38A7A149CA__INCLUDED_)
