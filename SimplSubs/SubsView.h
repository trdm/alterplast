#if !defined(AFX_SUBSVIEW_H__377676B3_E7B1_49A9_A9C6_7A24AE8F0D7F__INCLUDED_)
#define AFX_SUBSVIEW_H__377676B3_E7B1_49A9_A9C6_7A24AE8F0D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubsView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSubsView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "MyListBox.h"


class CSubsView : public CFormView
{
protected:
	CSubsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSubsView)

// Form Data
public:
	//{{AFX_DATA(CSubsView)
	enum { IDD = IDD_SUBS_DLG };
	//CListBox	m_ListCtrl;
	CMyListBox	m_ListCtrl;
	
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	long GetCurentStoredID();
	static CSubsView* m_pView;

	void OnSelectMetaData();
	void SetModeFiltred();
	void SetModeMarkUp();
	void OnSubsSaveData();
	void OnSubsHelp();

	void OnSubsSettings();
	void OnSubsDelete();
	void OnSubsRename();
	void OnSubsNew();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubsView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnUpdateConrol(CCmdUI* pCmdUI);
	virtual ~CSubsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSubsView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeSubList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBSVIEW_H__377676B3_E7B1_49A9_A9C6_7A24AE8F0D7F__INCLUDED_)
