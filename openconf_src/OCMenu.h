#if !defined(AFX_OCMENU_H__82804088_ED12_4696_B9F3_4EA5FCF8D77E__INCLUDED_)
#define AFX_OCMENU_H__82804088_ED12_4696_B9F3_4EA5FCF8D77E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OCMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COCMenu window
#define ID_FIRST_HOTKEY		21000
#define ID_LAST_HOTKEY		21255

class COCMenu : public CFrameWnd
{
// Construction
public:
	COCMenu();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCMenu)
	public:
	//}}AFX_VIRTUAL
	BOOL PreTranslateMessageWrap(MSG* pMsg);

// Implementation
public:
	void OnShowHidePanels(UINT id);
	void OnHotKeyCommand(UINT id);
	static void RemoveCmdTarget(CCmdTarget* pTarget);
	static void AddCmdTarget(CCmdTarget* pTarget);
	BOOL OnCmdMsgMy(UINT nID, int nCode,void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	void OnUpdateCustomCmd(CCmdUI* pcm);
	BOOL COCMenu::OnToolTipTextMy1(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	BOOL OnToolTipTextMy(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	void GetMsgString(UINT nID,CString& rMessage);
	void OnChildActivateWrap(int,class CWnd *,class CWnd *);
	void OnCmdMacros(UINT id);
	void OnUpdateForm(CCmdUI* pCm);
	void AddRecentRun(CString file);
	static void AddRecentEdit(CString file);
	void OnRecentRun(UINT cmd);
	void OnRecentEdit(UINT cmd);
	static CStringArray m_RecentEdit;
	static CStringArray m_RecentRun;
	void OnMenu(UINT cmd);
	static void Init(HWND hWnd);
	virtual ~COCMenu();

	// Generated message map functions
protected:
	static const AFX_MSGMAP* PASCAL _GetBaseWrap();
	static AFX_PMSG FindRealHandler(UINT nMessage);
	//{{AFX_MSG(COCMenu)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetupPanels();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OCMENU_H__82804088_ED12_4696_B9F3_4EA5FCF8D77E__INCLUDED_)
