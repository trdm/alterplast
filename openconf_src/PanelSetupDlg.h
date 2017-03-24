#if !defined(AFX_PANELSETUPDLG_H__97361DC3_E2F5_407C_85DE_595CFE55B2A0__INCLUDED_)
#define AFX_PANELSETUPDLG_H__97361DC3_E2F5_407C_85DE_595CFE55B2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelSetupDlg dialog
#include "PanelManager.h"
#include "resource.h"

class CPanelSetupDlg : public CDialog
{
// Construction
public:
	CPanelSetupDlg(CWnd* pParent = NULL);   // standard constructor
	SPanelInfo m_panels[16];
	CMapStringToPtr m_mapPaneNames;
	bool m_isNameEdit;
	bool m_isModified;
	HIMAGELIST m_hImgList;
	int m_iPaneCnt;

// Dialog Data
	//{{AFX_DATA(CPanelSetupDlg)
	enum { IDD = IDD_PANELS_SETTING };
	CListCtrl	m_list;
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelSetupDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelSetupDlg)
	afx_msg void OnAddPanel();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnRemovePanel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELSETUPDLG_H__97361DC3_E2F5_407C_85DE_595CFE55B2A0__INCLUDED_)
