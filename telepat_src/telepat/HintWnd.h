// HintWnd.h : header file
#ifndef AFX_HINTWND_H
#define AFX_HINTWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CHintWnd window
DWORD GetOSVersion();

class CHintWnd : public CWnd
{
// Construction
public:
	CHintWnd(CWnd* pList);
	CWnd* m_pList;
	bool m_list;
	UINT m_timer;
	static CString m_ClassName;
	static struct SInit{
		SInit()
		{
			DWORD style=CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
			if(GetOSVersion()>5000)
				style|=(1<<17);
			m_ClassName=AfxRegisterWndClass(style,0,(HBRUSH)GetStockObject(WHITE_BRUSH),0);
		}
	}m_init;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHintWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Hide(bool delay=true);
	virtual ~CHintWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHintWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
