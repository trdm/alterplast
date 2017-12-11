#if !defined(AFX_MYVIEW_H__FAEF5830_563D_4479_AC2F_C4185BE17A83__INCLUDED_)
#define AFX_MYVIEW_H__FAEF5830_563D_4479_AC2F_C4185BE17A83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "resource.h"
#include "ColumnTreeView.h"


class CMyView : public CColumnTreeView //CFormView
{
protected:
	CMyView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyView)

// Form Data
public:
	static CMyView* m_pView;
	//{{AFX_DATA(CMyView)
	enum { IDD = IDD_MY_DLG };
	//CTreeCtrlDrag	m_TreeMd;
	//CNewTreeListCtrl m_TreeMd;
	//}}AFX_DATA
	CMapStringToOb m_MapMDso;

// Attributes
public:

// Operations
public:
	HTREEITEM InsertAdd (CMetaDataObj *pObj, HTREEITEM parent);
	void OpenMetaObjSpec (CString nMetaObjType, CString nMetaObjName, int nAction);
	void OnOpenMetaObj();
	HTREEITEM Insert(CMetaDataObj *pObj,HTREEITEM parent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CHeaderCtrl m_Horz;
	virtual ~CMyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMyView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYVIEW_H__FAEF5830_563D_4479_AC2F_C4185BE17A83__INCLUDED_)
