#if !defined(AFX_CPPVIEW_H__CB0C7002_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_)
	#define AFX_CPPVIEW_H__CB0C7002_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

// CPPView.h : header file
//

#include <boost\scoped_ptr.hpp>

/////////////////////////////////////////////////////////////////////////////
// CCPPView view

class CInfoWrcSpace;

class CCPPView : public CTreeView
{
protected:
	CCPPView();           // protected constructor used by dynamic creation
	
	DECLARE_DYNCREATE(CCPPView)

// Attributes
private:
	CPoint m_click;
	boost::scoped_ptr<CInfoWrcSpace> m_pRootData;
	
	static CImageList m_ImageList;
	static HACCEL m_HotKeys;
	static CCPPView* m_pView;

public:
	
// Operations
public:
	void OnSave();
	void UpdateClass(CDocument* pDoc=NULL);

	void OnCopy();
	void Refresh();
	void Sort(HTREEITEM hParent);

	static CBodyTextView* FindTextView(CDocument*);
	
	static bool IsInit() { return NULL != m_pView; };
	static CCPPView* GetView() { assert(m_pView); return m_pView; };
	static CTreeCtrl& GetMyTreeCtrl() { assert(GetView()->GetTreeCtrl()); return GetView()->GetTreeCtrl();};

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPPView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
private:
	void MiniRefresh();
	void OnUpdateFileSave(CCmdUI* pcm);
	void UpdateModified();
	void OnProp();
	void SortOneNode(HTREEITEM item);
	void FillList();

	static void LoadTypeIcon();

protected:
	virtual ~CCPPView();
#ifdef _DEBUG
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCPPView)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPPVIEW_H__CB0C7002_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_)
