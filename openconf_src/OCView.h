#ifndef OCVIEW_H
#define OCVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OCView.h : header file
//
#include "pluginmanager.h"
#include <occimpl.h>
#include "lefttree.h"
class CTreeItemInfo;
class CPluginItem;

class COleControlSiteWrap:public COleControlSite
{
public:
	void AttachWnd(){AttachWindow();}
	void SetExtentEx(){SetExtent();	}
};
/////////////////////////////////////////////////////////////////////////////
// COCView view
class COCView : public CView
{
protected:
	COCView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COCView)

// Attributes
public:
	static COCView* m_pView;
	static CImageList m_ImageList;
	HWND m_right;
	HWND m_focus;
	bool m_isMove;
	CPoint m_movePoint;
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	CLeftTree m_tree;
	CListCtrl m_list;
	COleControlSite* m_pActiveObject;
	CTreeItemInfo* m_pLastRightInfo;

// Operations
public:
	void ShowTab(CTreeItemInfo* pInfo,CStringArray& titles,CStringArray& data);
	void CreatePluginControl(CPluginItem* pItem);
	void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	void OnReturnTree(NMHDR* pNMHDR, LRESULT* pResult);
	void OnChildSetFocus(UINT id,NMHDR* pNMHDR, LRESULT* pResult);
	void OnChildKillFocus(UINT id,NMHDR* pNMHDR, LRESULT* pResult);
	void FillScripts();
	void OnDraw(CDC*){}
	void InvertSplit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COCView();

	// Generated message map functions
protected:
	void DestroyRightPane();
	//{{AFX_MSG(COCView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
