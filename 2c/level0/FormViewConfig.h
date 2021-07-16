#if !defined(AFX_FORMVIEWCONFIG_H__6B64860F_8053_4F7E_B74A_49FD51F05347__INCLUDED_)
#define AFX_FORMVIEWCONFIG_H__6B64860F_8053_4F7E_B74A_49FD51F05347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormViewConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormViewConfig form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormViewConfig : public CFormView
{
protected:
	CFormViewConfig();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormViewConfig)

// Form Data
public:
	//{{AFX_DATA(CFormViewConfig)
	enum { IDD = IDD_FORM1C_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewConfig)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormViewConfig();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormViewConfig)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWCONFIG_H__6B64860F_8053_4F7E_B74A_49FD51F05347__INCLUDED_)
