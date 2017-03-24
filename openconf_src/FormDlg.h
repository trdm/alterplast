#if !defined(AFX_FORMDLG_H__930A3086_F138_11D7_8D5E_B340FDE9FB02__INCLUDED_)
#define AFX_FORMDLG_H__930A3086_F138_11D7_8D5E_B340FDE9FB02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormDlg dialog
#include "resource.h"
class CFormDlg : public CDialog
{
// Construction
public:
	CFormDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormDlg)
	enum { IDD = 1};//IDD_EMPTY1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFormDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMDLG_H__930A3086_F138_11D7_8D5E_B340FDE9FB02__INCLUDED_)
