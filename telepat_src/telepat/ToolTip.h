#if !defined(AFX_TOOLTIP_H__3B27D4D7_0B7E_4618_99CA_D7048B477897__INCLUDED_)
#define AFX_TOOLTIP_H__3B27D4D7_0B7E_4618_99CA_D7048B477897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTip window

class CToolTip : public CWnd
{
// Construction
public:
	CToolTip(CListBox* pParent);
	int m_Idx;
	static CString m_ClassName;
	static struct SInit{
		SInit()
		{
	        m_ClassName=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS);
		}
	}m_init;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTip)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Hide();
	void Show(int nItem);
	virtual ~CToolTip();
	CListBox* m_pParent;

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTip)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIP_H__3B27D4D7_0B7E_4618_99CA_D7048B477897__INCLUDED_)
