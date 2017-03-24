#if !defined(AFX_CJSTACK_H__44BC5A68_D809_432B_89E6_29B492C61E61__INCLUDED_)
#define AFX_CJSTACK_H__44BC5A68_D809_432B_89E6_29B492C61E61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CJStack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCJStack dialog
#include "resource.h"

class CCJStack : public CDialog
{
// Construction
public:
	void OnOK();
	static void ShowStack();
	CCJStack(CWnd* pParent = NULL);   // standard constructor
	int m_selpos;
	HBRUSH m_hBG;

// Dialog Data
	//{{AFX_DATA(CCJStack)
	enum { IDD = IDD_CJSTACK };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJStack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCJStack)
	afx_msg void OnSelchangeList1();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkList1();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CJSTACK_H__44BC5A68_D809_432B_89E6_29B492C61E61__INCLUDED_)
