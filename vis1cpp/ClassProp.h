#if !defined(AFX_CLASSPROP_H__2BC30D37_BDC6_47EA_B420_43234CD95500__INCLUDED_)
#define AFX_CLASSPROP_H__2BC30D37_BDC6_47EA_B420_43234CD95500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClassProp dialog
#include "resource.h"

class CClassProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CClassProp)

// Construction
public:
	CClassProp();
	CClassProp(const CString& strDefinePath, const CString& strImplementPath);
	~CClassProp();

// Dialog Data
	//{{AFX_DATA(CClassProp)
	enum { IDD = IDD_CLASS };
	CString	m_def;
	CString	m_mod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassProp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSPROP_H__2BC30D37_BDC6_47EA_B420_43234CD95500__INCLUDED_)
