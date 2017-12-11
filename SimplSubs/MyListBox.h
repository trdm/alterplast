#if !defined(AFX_MYLISTBOX_H__B75AE3F7_631C_47E2_946F_EBAEC1EED54A__INCLUDED_)
#define AFX_MYLISTBOX_H__B75AE3F7_631C_47E2_946F_EBAEC1EED54A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListBox window

class CMyListBox : public CListBox
{
// Construction
public:
	CMyListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTBOX_H__B75AE3F7_631C_47E2_946F_EBAEC1EED54A__INCLUDED_)
