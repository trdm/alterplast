#if !defined(AFX_FORMUNIT2_H__C7A0758B_2666_4701_A939_E3B8C3A0DD8D__INCLUDED_)
#define AFX_FORMUNIT2_H__C7A0758B_2666_4701_A939_E3B8C3A0DD8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormUnit2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormUnit2 dialog

class CFormUnit2 : public CDialog
{
// Construction
public:
	CFormUnit2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormUnit2)
	enum { IDD = IDD_FORMUNIT2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormUnit2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormUnit2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMUNIT2_H__C7A0758B_2666_4701_A939_E3B8C3A0DD8D__INCLUDED_)
