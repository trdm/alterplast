// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_EDITWND_H__50C319D0_D83E_4D41_9451_FD45065D1327__INCLUDED_)
#define AFX_EDITWND_H__50C319D0_D83E_4D41_9451_FD45065D1327__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditWnd window

class CEditWnd : public CWnd
{
// Construction
public:
	CEditWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWND_H__50C319D0_D83E_4D41_9451_FD45065D1327__INCLUDED_)
