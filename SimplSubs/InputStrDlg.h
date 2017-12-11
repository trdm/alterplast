#if !defined(AFX_INPUTSTRDLG_H__591ADDB5_6339_4118_8B10_3BE798E5F44E__INCLUDED_)
#define AFX_INPUTSTRDLG_H__591ADDB5_6339_4118_8B10_3BE798E5F44E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputStrDlg.h : header file
//
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CInputStrDlg dialog

class CInputStrDlg : public CDialog
{
// Construction
public:
	CInputStrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputStrDlg)
	enum { IDD = IDD_INPUT_STR_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputStrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CString GetInput();
	void SetOptions( CString nsInputStr, int n_iMaxLen =128, CString nsCaption="");
	CString m_StringInput;
	CString m_StringCaption;
	int m_iMaxLen;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputStrDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTSTRDLG_H__591ADDB5_6339_4118_8B10_3BE798E5F44E__INCLUDED_)
