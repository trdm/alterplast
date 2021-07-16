//Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.

#if !defined(AFX_CHOOSEVALUE_H__FC6977E0_18D8_44E6_AC52_A561FA1DF2EF__INCLUDED_)
#define AFX_CHOOSEVALUE_H__FC6977E0_18D8_44E6_AC52_A561FA1DF2EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChooseValue.h : header file
//
#include "../IconListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CChooseValue dialog

class CChooseValue : public CDialog
{
// Construction
public:
	CChooseValue(CWnd* pParent = NULL);   // standard constructor

	//входные параметры
	int nStart;
	int nMode;
	CString csTitle;
	CArray <CString,CString> List;
	CArray <BOOL,BOOL> ListCheck;
	CArray <int,int> ListImage;

	//результат:
	int nRes;
	
	void Small()
	{
		m_lpszTemplateName=MAKEINTRESOURCE(IDD_CHOOSEVALUESMALL);
	};

// Dialog Data
	//{{AFX_DATA(CChooseValue)
	enum { IDD = IDD_CHOOSEVALUE };
	CIconListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChooseValue)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSEVALUE_H__FC6977E0_18D8_44E6_AC52_A561FA1DF2EF__INCLUDED_)
