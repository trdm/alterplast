#if !defined(AFX_INPUTPERIOD_H__3C874BE1_B3EE_4190_8A2B_C5EC2B7B0253__INCLUDED_)
#define AFX_INPUTPERIOD_H__3C874BE1_B3EE_4190_8A2B_C5EC2B7B0253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPeriod.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InputPeriod dialog

class CInputPeriod : public CDialog
{
// Construction
public:
	CInputPeriod(CWnd* pParent = NULL);   // standard constructor
	CString csTitle;
	CTime	m_DateBeg1;
	CTime	m_DateEnd1;
// Dialog Data
	//{{AFX_DATA(CInputPeriod)
	enum { IDD = IDD_INPUTPERIOD };
	CButton	m_CheckMonthCtrl;
	CButton	m_CheckQuartCtrl;
	CButton	m_CheckYearCtrl;
	CEdit	m_PeriodEditCtrl;
	CComboBox	m_MonthCtrl;
	CComboBox	m_QuartCtrl;
	CDateTimeCtrl	m_DateEndCtrl;
	CDateTimeCtrl	m_DateBegCtrl;
	CDateTimeCtrl	m_Date1Ctrl;
	BOOL	m_CheckYear;
	BOOL	m_CheckQuart;
	BOOL	m_CheckMonth;
	CString	m_QuartData;
	CString	m_MonthData;
	CTime	m_Date1;
	CTime	m_DateBeg;
	CTime	m_DateEnd;
	int		m_RadioInterval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPeriod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPeriod)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnSelendokCombo1();
	afx_msg void OnSelendokCombo2();
	afx_msg void OnUserstringDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker3(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdatePeriod();
	SYSTEMTIME sTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPERIOD_H__3C874BE1_B3EE_4190_8A2B_C5EC2B7B0253__INCLUDED_)
