#if !defined(AFX_HTMLHELPVIEW_H__24C9B124_0432_11D2_BFD3_006008162563__INCLUDED_)
#define AFX_HTMLHELPVIEW_H__24C9B124_0432_11D2_BFD3_006008162563__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HTMLHelpView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpView view
#pragma comment(lib, "html_help_view//htmlhelp.lib")

class CHTMLHelpView : public CView
{
protected:
	CHTMLHelpView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHTMLHelpView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLHelpView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHTMLHelpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHTMLHelpView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HWND m_hHelpWindow;
public:
	void DestroyHelpWindow() 
	{ 
		if (IsWindow(m_hHelpWindow))
		{
			::DestroyWindow(m_hHelpWindow); 
		}
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLHELPVIEW_H__24C9B124_0432_11D2_BFD3_006008162563__INCLUDED_)
