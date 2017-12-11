#if !defined(AFX_EFFRAME_H__DCA07EC8_EB35_4A5A_A05D_47D7AC13430B__INCLUDED_)
#define AFX_EFFRAME_H__DCA07EC8_EB35_4A5A_A05D_47D7AC13430B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EFFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEFFrame frame

class CEFFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CEFFrame)
protected:
	CEFFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CSplitterWnd m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEFFrame();

	// Generated message map functions
	//{{AFX_MSG(CEFFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFRAME_H__DCA07EC8_EB35_4A5A_A05D_47D7AC13430B__INCLUDED_)
