#if !defined(AFX_SELECTFRM_H__95ADC8DD_37BE_4B1D_B6D6_9879411D3C1C__INCLUDED_)
#define AFX_SELECTFRM_H__95ADC8DD_37BE_4B1D_B6D6_9879411D3C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SelectFrm dialog

class SelectFrm : public CDialog
{
// Construction
public:
	void SetFilter();
	CString FilterValue;
	void ShowSelDlg();
	long ModuleDescription;

	void SetInitialText(LPCSTR text) { InitialText = text; }
	CString DoChoice(CStringArray* AllStrRef);
	SelectFrm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SelectFrm)
	enum { IDD = IDD_SELECTFRM_DIALOG };
	CListCtrl	m_List;
	CEdit	m_Wrd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SelectFrm)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectFrm)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit1();
	afx_msg void OnDblclkListBox(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ExitDialog();
	int GetSelectedPos();
	void InitImagesList();
	CImageList LstImages;
	long ListRight;
	long ListBottom;
	void SortList(CString WrdBegin);

	CString InitialText;
	CString SelectedItem;
	CStringArray AllStrings;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFRM_H__95ADC8DD_37BE_4B1D_B6D6_9879411D3C1C__INCLUDED_)
