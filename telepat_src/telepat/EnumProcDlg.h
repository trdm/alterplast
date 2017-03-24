#if !defined(AFX_ENUMPROCDLG_H__19EA9B6B_A769_413E_86B5_04780F81D2BF__INCLUDED_)
#define AFX_ENUMPROCDLG_H__19EA9B6B_A769_413E_86B5_04780F81D2BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnumProcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnumProcDlg dialog
class CTxtTrap;
#include "resource.h"
#include "telelist.h"

class CFindEdit:public CEdit
{
public:
	//{{AFX_MSG(CFindEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CEnumProcDlg : public CDialog
{
// Construction
public:
	static void FillMethodList(CPtrArray& arr,CTxtTrap* pEditor, CWnd* list, int& sort, int& filter,bool listbox=true, int* pFunc=NULL, int* total=NULL);
	void FillList();
	BOOL OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CPtrArray m_array;
	int m_sort;
	int m_filtr;
	CToolBar	m_tb;
	HACCEL		m_hAccel;
	void LayoutControls(CRect& newRect);
	bool m_lock;
	CTxtTrap* m_pEditor;
	int m_CurLine;
	static void ShowEnumProc(CTxtTrap* pEditor);
	CEnumProcDlg(CWnd* pParent = NULL);   // standard constructor
	void MoveSel();
	void OnOK();

// Dialog Data
	//{{AFX_DATA(CEnumProcDlg)
	enum { IDD = IDD_ENUMPROC };
	CFindEdit	m_edit;
	CTeleList	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumProcDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnumProcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit();
	afx_msg void OnDblclkList1();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnFilter();
	afx_msg void OnSort();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMPROCDLG_H__19EA9B6B_A769_413E_86B5_04780F81D2BF__INCLUDED_)
