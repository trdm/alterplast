#if !defined(AFX_MENULISTBOX_H__8B28544B_9486_448D_A799_EFA5963BC73D__INCLUDED_)
#define AFX_MENULISTBOX_H__8B28544B_9486_448D_A799_EFA5963BC73D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuListBox.h : header file
//

#include "../IconListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuListBox window

class CMenuListBox : public CIconListBox
{
// Construction
public:
	CMenuListBox();

// Attributes
public:

// Operations
public:
	int ChooseValue();
	int nTracking;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuListBox)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENULISTBOX_H__8B28544B_9486_448D_A799_EFA5963BC73D__INCLUDED_)
