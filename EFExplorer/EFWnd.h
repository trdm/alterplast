#if !defined(AFX_EFWND_H__0CF7625F_7027_4BE9_B6E2_BD9621E82EE7__INCLUDED_)
#define AFX_EFWND_H__0CF7625F_7027_4BE9_B6E2_BD9621E82EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EFWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEFWnd window

class CEFWnd : public CWnd
{
	DECLARE_DYNCREATE(CEFWnd)
protected:
	CEFWnd();

// Attributes
public:
	CSplitterWnd m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEFWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEFWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFWND_H__0CF7625F_7027_4BE9_B6E2_BD9621E82EE7__INCLUDED_)
