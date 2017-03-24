#if !defined(AFX_FIREDLG_H__AE3BE83B_253E_4079_A313_E460B554252B__INCLUDED_)
#define AFX_FIREDLG_H__AE3BE83B_253E_4079_A313_E460B554252B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FireDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFireDlg dialog
#include "resource.h"
#include "firewnd.h"
class CFireDlg : public CDialog
{
// Construction
public:
	CFireDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_version[2];

// Dialog Data
	//{{AFX_DATA(CFireDlg)
	enum { IDD = IDD_FIREWND1 };
	CFireWnd m_fire;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFireDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREDLG_H__AE3BE83B_253E_4079_A313_E460B554252B__INCLUDED_)
