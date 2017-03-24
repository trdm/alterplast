#if !defined(AFX_CUSTOMCMDDLG1_H__5F032644_791F_4BDD_BC2C_9CCE7E96CA03__INCLUDED_)
#define AFX_CUSTOMCMDDLG1_H__5F032644_791F_4BDD_BC2C_9CCE7E96CA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomCmdDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomCmdDlg dialog
#include "resource.h"

class CCustomCmdDlg : public CDialog
{
// Construction
public:
	int GetListSel();
	void SetEnabledButtons();
	CCustomCmdDlg(CWnd* pParent = NULL);   // standard constructor
	CToolBar m_tb;
	DWORD m_enabled;
	HACCEL m_haccel;
	static void ShowDlg();
	BOOL OnToolTipText(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
// Dialog Data
	//{{AFX_DATA(CCustomCmdDlg)
	enum { IDD = IDD_CUSTOM_CMD };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomCmdDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustomCmdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelCcmd();
	afx_msg void OnEditCcmd();
	afx_msg void OnMacrosCcmd();
	afx_msg void OnNewCcmd();
	afx_msg void OnUpCcmd();
	afx_msg void OnDownCcmd();
	afx_msg void OnDestroy();
	afx_msg void OnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMCMDDLG1_H__5F032644_791F_4BDD_BC2C_9CCE7E96CA03__INCLUDED_)
