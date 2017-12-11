#if !defined(AFX_EFVIEW_H__FF3979E7_6C9F_41E3_B66E_20CC5003D9A3__INCLUDED_)
#define AFX_EFVIEW_H__FF3979E7_6C9F_41E3_B66E_20CC5003D9A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EFView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEFView view

class CEFView : public CView
{
protected:
	CEFView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEFView)

// Attributes
public:
	CSplitterWnd m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEFView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEFView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFVIEW_H__FF3979E7_6C9F_41E3_B66E_20CC5003D9A3__INCLUDED_)
