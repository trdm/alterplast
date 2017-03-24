#ifndef AFX_PLUGINPAGEVIEW_H
#define AFX_PLUGINPAGEVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginPageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginPageView view
class CPluginPageDoc;
class CPluginPageView : public CView
{
protected:
	DECLARE_DYNCREATE(CPluginPageView)

// Attributes
public:
	CPluginPageView();           // protected constructor used by dynamic creation
	CPluginPageDoc* GetDoc(){return (CPluginPageDoc*)m_pDocument;}
	COleControlSite* m_pActiveObject;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginPageView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPluginPageView();
	// Generated message map functions
protected:
	//{{AFX_MSG(CPluginPageView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
