// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_INPUTDIALOG_H__FD5E55CA_7FF8_48FE_9994_AEB85B0A316F__INCLUDED_)
#define AFX_INPUTDIALOG_H__FD5E55CA_7FF8_48FE_9994_AEB85B0A316F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDialog dialog

class CInputDialog : public CDialog
{
// Construction
public:
	CInputDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDialog)
	enum { IDD = IDD_INPUTSTRING };
	CEdit	m_String;
	//}}AFX_DATA

	CString csString;
	CString csTitle;
	int nMultiLine;
	int nLimit;
	int nTimer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDIALOG_H__FD5E55CA_7FF8_48FE_9994_AEB85B0A316F__INCLUDED_)
