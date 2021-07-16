#if !defined(AFX_INPUTNUMERIC_H__55F4D95C_31B9_4A0A_8352_A3915135F06B__INCLUDED_)
#define AFX_INPUTNUMERIC_H__55F4D95C_31B9_4A0A_8352_A3915135F06B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputNumeric.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputNumeric dialog

class CInputNumeric : public CDialog
{
// Construction
public:
	
	CInputNumeric(CWnd* pParent = NULL);   // standard constructor
	int nLength;
	int nPrec;
	int nTimer;
	CString csTitle;
	
// Dialog Data
	//{{AFX_DATA(CInputNumeric)
	enum { IDD = IDD_INPUTNUMERIC };
	CEdit	CtrlEdit;
	CString	csValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputNumeric)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CheckText(CString Str);
	// Generated message map functions
	//{{AFX_MSG(CInputNumeric)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditnumeric();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTNUMERIC_H__55F4D95C_31B9_4A0A_8352_A3915135F06B__INCLUDED_)
