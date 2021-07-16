// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_LINEINPUT_H__F5D2233D_4D38_487F_8E37_88161BAF3425__INCLUDED_)
#define AFX_LINEINPUT_H__F5D2233D_4D38_487F_8E37_88161BAF3425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineInput dialog

class CLineInput : public CDialog
{
// Construction
public:
	CLineInput(CWnd* pParent = NULL);   // standard constructor
	int nLine;

// Dialog Data
	//{{AFX_DATA(CLineInput)
	enum { IDD = IDD_DIALOG_GOTOLINE };
	CEdit	m_Line;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineInput)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEINPUT_H__F5D2233D_4D38_487F_8E37_88161BAF3425__INCLUDED_)
