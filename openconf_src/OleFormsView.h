// OleFormsView.h : interface of the COleFormsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEFORMSVIEW_H__6874A1EF_F464_11D7_8D5E_C70D7B797303__INCLUDED_)
#define AFX_OLEFORMSVIEW_H__6874A1EF_F464_11D7_8D5E_C70D7B797303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleFormsCntrItem;

class COleFormsView : public CView
{
protected: // create from serialization only
	COleFormsView();
	DECLARE_DYNCREATE(COleFormsView)

// Attributes
public:
	//COleFormsDoc* GetDocument();
	// m_pSelection holds the selection to the current COleFormsCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not COleFormsCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	COleDocObjectItem* m_pSelection;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COleFormsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COleFormsView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COleFormsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelEditCntr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OleFormsView.cpp
//inline COleFormsDoc* COleFormsView::GetDocument()
//{ return (COleFormsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLEFORMSVIEW_H__6874A1EF_F464_11D7_8D5E_C70D7B797303__INCLUDED_)
