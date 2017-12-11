#if !defined(AFX_SETUPDLG_H__05403BF1_1864_49BC_9179_615161407DC8__INCLUDED_)
#define AFX_SETUPDLG_H__05403BF1_1864_49BC_9179_615161407DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

class CSetupDlg : public CDialog
{
// Construction
public:
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_SETUP_DLG };
	CComboBox	m_Combo;
	CListCtrl	m_LiistAddDDText;
	CStatic	m_labelCharTerminator;
	CString	m_strCharTerminator;
	BOOL	m_bEnableDDGroup;
	BOOL	m_bShowColComm;
	BOOL	m_bShowColID;
	BOOL	m_bShowColType;
	BOOL	m_bEnableFullPathDD;
	BOOL	m_bShow_ID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEnabbleFullPathDd();
	afx_msg void OnEnabbleGrDd();
	afx_msg void OnSelchangeCmbPlaceSubs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPDLG_H__05403BF1_1864_49BC_9179_615161407DC8__INCLUDED_)
