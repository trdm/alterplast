#if !defined(AFX_ROOTPROP_H__E8735EE4_34B6_4323_8897_B439155A8233__INCLUDED_)
#define AFX_ROOTPROP_H__E8735EE4_34B6_4323_8897_B439155A8233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RootProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRootProp dialog
#include "resource.h"
class CRootProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CRootProp)

// Construction
public:
	CRootProp();
	CRootProp(const CString& strPath);
	~CRootProp();

// Dialog Data
	//{{AFX_DATA(CRootProp)
	enum { IDD = IDD_ROOT };
	CString	m_path;
	CString	m_bin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRootProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRootProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOTPROP_H__E8735EE4_34B6_4323_8897_B439155A8233__INCLUDED_)
