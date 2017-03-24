#if !defined(AFX_MACROSDLG_H__A893D860_530C_42D0_B4DA_D4BE877150FE__INCLUDED_)
#define AFX_MACROSDLG_H__A893D860_530C_42D0_B4DA_D4BE877150FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacrosDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMacrosDlg dialog

class CMacrosDlg : public CDialog
{
// Construction
public:
	void GetLastSel(bool& IsScript,CString& modul, CString& macros);
	static void InvokeMacros();
	void InsertMacros(IDispatchPtr& pDisp,CString name,bool IsScript);
	CMacrosDlg(CWnd* pParent = NULL);   // standard constructor
	struct Info{
		IDispatchPtr pDisp;
		DISPID dispID;
		bool IsScript;
	};
	IDispatchPtr m_selDispatch;
	DISPID m_selMacros;
	static CString m_LastSelPath;
	static bool m_LastSelIsScript;

// Dialog Data
	//{{AFX_DATA(CMacrosDlg)
	enum { IDD = IDD_MACROS };
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacrosDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMacrosDlg)
	afx_msg void OnPressOK();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACROSDLG_H__A893D860_530C_42D0_B4DA_D4BE877150FE__INCLUDED_)
