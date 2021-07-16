#if !defined(AFX_MENUBUTTON_H__C789E26C_DA4B_11D2_BF44_006008085F93__INCLUDED_)
#define AFX_MENUBUTTON_H__C789E26C_DA4B_11D2_BF44_006008085F93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuButton.h : header file
//

#define ID_DS_POPUP_BUTTON		20002	// Default is 20002.

/////////////////////////////////////////////////////////////////////////////
// CMyMenuButton window
class CMyMenuButton : public CButton
{
// Construction
public:
	CMyMenuButton();

// Attributes
public:
	BOOL		m_bLBtnDown;	// true if left mouse button is pressed
	BOOL		m_bPainted;		// used during paint operations

// Operations
public:
	virtual void DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMenuButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyMenuButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyMenuButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUBUTTON_H__C789E26C_DA4B_11D2_BF44_006008085F93__INCLUDED_)
