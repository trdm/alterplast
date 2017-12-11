#if !defined(AFX_BASEVIEW_H__2A720E36_7127_11D7_8D5E_97BCD729C404__INCLUDED_)
#define AFX_BASEVIEW_H__2A720E36_7127_11D7_8D5E_97BCD729C404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseView view
class CMDSplitter;

class CBaseView : public CFrameWnd
{
protected:
	CBaseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBaseView)

// Attributes
public:
	CMDSplitter* m_pSplitter;
	BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBaseView();
#ifdef _DEBUG
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEVIEW_H__2A720E36_7127_11D7_8D5E_97BCD729C404__INCLUDED_)
