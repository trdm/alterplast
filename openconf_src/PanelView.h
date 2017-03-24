#if !defined(AFX_PANELVIEW_H__B392EACB_779D_4720_AC16_AEEF7FBBF53A__INCLUDED_)
#define AFX_PANELVIEW_H__B392EACB_779D_4720_AC16_AEEF7FBBF53A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelView view

class CPanelView : public CView
{
protected:
	CPanelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPanelView)

// Attributes
public:
	CWnd* m_pActiveWnd;
	CPtrArray m_tabs;
	CTabCtrl* m_pTab;

// Operations
public:
	void OnSwitchTab(NMHDR* phdr, LRESULT* pRes);
	void RepositionWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPanelView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanelView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELVIEW_H__B392EACB_779D_4720_AC16_AEEF7FBBF53A__INCLUDED_)
