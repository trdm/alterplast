#if !defined(AFX_SELECTDLG_H__C36FFED8_553C_44CA_9849_CA6871DE154C__INCLUDED_)
#define AFX_SELECTDLG_H__C36FFED8_553C_44CA_9849_CA6871DE154C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog

class CSelectDlg : public CDialog
{
// Construction
public:
	static void DoSelect(BSTR caption, BSTR values, BSTR* ret,int mode);
	CSelectDlg(CWnd* pParent = NULL);   // standard constructor
	CWnd* m_list;
	CString m_caption;
	CString m_vals;
	CString m_ret;
	int m_mode;
	CRect m_rcMin;
	int m_btnHeight;
	int m_okLeft;
	int m_cancelLeft;
	void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDblclkList1();

// Dialog Data
	//{{AFX_DATA(CSelectDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDLG_H__C36FFED8_553C_44CA_9849_CA6871DE154C__INCLUDED_)
