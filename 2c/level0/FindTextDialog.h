#if !defined(AFX_FINDTEXTDIALOG_H__888CDD9E_1245_4703_BB2C_2CB454201245__INCLUDED_)
#define AFX_FINDTEXTDIALOG_H__888CDD9E_1245_4703_BB2C_2CB454201245__INCLUDED_

#include "Resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindTextDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindTextDialog dialog

class CFindTextDialog : public CDialog
{
// Construction
public:
	CFindTextDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindTextDialog)
	enum { IDD = IDD_FIND_ALL_DIALOG };
	CString	m_csFind;
	BOOL	m_bMatchCase;
	//}}AFX_DATA


	void LoadState();
	void SaveState();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindTextDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindTextDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTEXTDIALOG_H__888CDD9E_1245_4703_BB2C_2CB454201245__INCLUDED_)
