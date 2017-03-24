#if !defined(AFX_HOTKEYDLG_H__0193290E_83B7_4048_BBA6_3B8113278D9E__INCLUDED_)
#define AFX_HOTKEYDLG_H__0193290E_83B7_4048_BBA6_3B8113278D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HotKeyDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CHotKeyDlg dialog
#define ID_HOTKEY_FIRST		20200
#define ID_HOTKEY_LAST		20455

class CHotKeyDlg : public CDialog
{
// Construction
public:
	DWORD GetHotKeyCode();
	struct SHKeyInfo
	{
		CPtrArray		IsScript;
		CPtrArray		codes;
		CStringArray	moduls;
		CStringArray	macros;
		void Remove(int idx)
		{
			IsScript.RemoveAt(idx);
			codes.RemoveAt(idx);
			moduls.RemoveAt(idx);
			macros.RemoveAt(idx);
		}
		void Swap(int idx1,int idx2)
		{
			void* v=IsScript[idx1];
			IsScript[idx1]=IsScript[idx2];
			IsScript[idx2]=v;
			v=codes[idx1];
			codes[idx1]=codes[idx2];
			codes[idx2]=v;
			CString t=moduls[idx1];
			moduls[idx1]=moduls[idx2];
			moduls[idx2]=t;

			t=macros[idx1];
			macros[idx1]=macros[idx2];
			macros[idx2]=t;
		}
	};
	void SwapItems(int sel,int dif);
	static SHKeyInfo* LoadHotKeys();
	static SHKeyInfo* m_hkeyInfo;
	static void Init();
	static void CreateAccel(SHKeyInfo* pInfo);
	void FillList();
	void SetEnabledButtons();
	SHKeyInfo* m_curInfo;
	static HACCEL m_hMacrosAccel;
	static bool m_CancelHotkey;
	static CHotKeyDlg* m_ActiveDlg;

	CHotKeyDlg(CWnd* pParent = NULL);   // standard constructor
	CToolBar m_tb;
	DWORD m_enabled;
	HACCEL m_haccel;
	static void ShowDlg();
	static void RunHKEy(UINT cmd);

	BOOL OnToolTipText(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	int GetListSel();
// Dialog Data
	//{{AFX_DATA(CHotKeyDlg)
	enum { IDD = IDD_HOTKEYS };
	CListCtrl	m_list;
	CHotKeyCtrl	m_hkBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHotKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDelHotKey();
	afx_msg void OnDownHotKey();
	afx_msg void OnMacrosHotKey();
	afx_msg void OnNewHotKey();
	afx_msg void OnUpHotKey();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeHotKey();
	afx_msg void OnAssign();
	virtual void OnOK();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYDLG_H__0193290E_83B7_4048_BBA6_3B8113278D9E__INCLUDED_)
