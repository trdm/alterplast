#if !defined(AFX_SUBSVIEWMAIN_H__67CBBC4B_D846_4739_B208_08165886436F__INCLUDED_)
#define AFX_SUBSVIEWMAIN_H__67CBBC4B_D846_4739_B208_08165886436F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubsViewMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubsViewMain frame

class CSubsViewMain : public CFrameWnd
{
	DECLARE_DYNCREATE(CSubsViewMain)
protected:
	CSubsViewMain();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	CToolBar m_wndToolBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubsViewMain)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSubsViewMain();

	// Generated message map functions
	//{{AFX_MSG(CSubsViewMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBSVIEWMAIN_H__67CBBC4B_D846_4739_B208_08165886436F__INCLUDED_)
