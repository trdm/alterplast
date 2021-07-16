// Авторские права - VTOOLS.RU (info@vtools.ru)
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6B63C5BC_567E_4E01_A26C_376A8132BD3B__INCLUDED_)
#define AFX_MAINFRM_H__6B63C5BC_567E_4E01_A26C_376A8132BD3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MessageBar.h"
#include "ObjectInspector\PropertyBar.h"
#include "DocumentBar_src\DocSelect.h"
//#include "BCMenu\BCMenu.h"
#include "DebugMessageBar.h"
#include "StackBar.h"



#include "MyDataExchange.h"
#include "WindowManager\WindowManager.h"


class CDocSelector2//заглушка
{
public:
	BOOL AddButton( CWnd* wnd, WORD wIcon )
	{
		return TRUE;
	}
	BOOL RemoveButton(CWnd* wnd)
	{
		return TRUE;
	}
};

class CMainFrame : public CMDIFrameWnd//COXMDIFrameWndSizeDock//CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	int nCurAppRunMode;

// Attributes
public:
	//CToolTipCtrl m_tooltip;

// Operations
public:
	BOOL DoCloseMessageBox(MSG* pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CExtStatusControlBar	m_wndStatusBar;
	CExtToolControlBar		m_ToolMain;
	CExtToolControlBar		m_ToolModule;
	CExtToolControlBar		m_ToolEdit;

	CExtControlBar			m_ParentMessageBar;
	CMessageBar				m_MessageBar;

	CExtControlBar			m_ParentPropertyBar;
	CPropertyBar			m_PropertyBar;


	CExtMenuControlBar		m_wndMenuBar;

	CComboBox				m_ComboHelpSearch;
	
	CExtControlBar			m_ParentDebugMessageBar;
	CDebugMessageBar		m_DebugMessageBar;

	CExtControlBar			m_ParentStackBar;
	CStackBar				m_StackBar;

	CMyDataExchange			MyDDE;

	void ShowPropertyBar(BOOL bShow=1)
	{
		ShowControlBar(&m_ParentPropertyBar,bShow,0);	
		RecalcLayout();
	};

	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

	void GetControlBarsEx(CArray<CControlBar*, CControlBar*>& arrBars);


public:
	CDocSelector m_wndDocSelector;
//	CMDIClient   m_MDIClient;

	int DoCloseWnd(CExtControlBar &Box);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnViewLikeOffice2k();
	afx_msg void OnUpdateViewLikeOffice2k(CCmdUI* pCmdUI);
	afx_msg void OnViewLikeOfficeXp();
	afx_msg void OnUpdateViewLikeOfficeXp(CCmdUI* pCmdUI);
	afx_msg void OnPauseDebug();
	afx_msg void OnUpdatePauseDebug(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopDebug(CCmdUI* pCmdUI);
	afx_msg void OnStopProgram();
	afx_msg void OnUpdateRunDebug(CCmdUI* pCmdUI);
	afx_msg void OnRunDebug();
	afx_msg void OnNextDebugOver();
	afx_msg void OnNextDebug();
	afx_msg void OnPostShow();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnSelectMenu(UINT nID);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6B63C5BC_567E_4E01_A26C_376A8132BD3B__INCLUDED_)
