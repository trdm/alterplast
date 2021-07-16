#if !defined(AFX_EDITDIALOG_H__72C277EC_4A12_4928_9347_405E9DC3F824__INCLUDED_)
#define AFX_EDITDIALOG_H__72C277EC_4A12_4928_9347_405E9DC3F824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditDialog form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CEditDialog : public CFormView
{
protected:
	CEditDialog();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditDialog)

// Form Data
public:
	//{{AFX_DATA(CEditDialog)
	enum { IDD = IDD_EDITDIALOG_FORM };
	CEdit	m_Edit;
	CButton	m_Group;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditDialog)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditDialog();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStatic2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDIALOG_H__72C277EC_4A12_4928_9347_405E9DC3F824__INCLUDED_)
