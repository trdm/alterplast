#if !defined(AFX_MDSPLITTER_H__2A720E31_7127_11D7_8D5E_97BCD729C404__INCLUDED_)
#define AFX_MDSPLITTER_H__2A720E31_7127_11D7_8D5E_97BCD729C404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDSplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDSplitter view

class CMDSplitter : public CSplitterWnd
{
protected:
	DECLARE_DYNCREATE(CMDSplitter)

// Attributes
public:
	CMDSplitter();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDSplitter)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMDSplitter();
#ifdef _DEBUG
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDSplitter)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDSPLITTER_H__2A720E31_7127_11D7_8D5E_97BCD729C404__INCLUDED_)
