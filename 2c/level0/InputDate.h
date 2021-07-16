#if !defined(AFX_INPUTDATE_H__FD5E55CA_7FF8_48FE_9994_AEB85B0A3161__INCLUDED_)
#define AFX_INPUTDATE_H__FD5E55CA_7FF8_48FE_9994_AEB85B0A3161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CInputDate dialog

class CInputDate : public CDialog
{
// Construction
public:
	CString csTitle;
	int		nTimer;
	
	CInputDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDate)
	enum { IDD = IDD_INPUTDATE };
	CDateTimeCtrl	m_Date1;
	CTime	m_Date2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDate)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CInputDate)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
#endif
