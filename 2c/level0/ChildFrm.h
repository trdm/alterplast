// Авторские права - VTOOLS.RU (info@vtools.ru)
// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__EAD94A5B_F3C3_4BB6_BD18_7FF7631DAC24__INCLUDED_)
#define AFX_CHILDFRM_H__EAD94A5B_F3C3_4BB6_BD18_7FF7631DAC24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SourceUT/OXTabView.h"
#include "EditForm/FormEditor.h"
#include "ModuleView.h"
#include "DocList.h"
#include "ModuleDoc.h"

class CMyCodeView : public COXTabViewPage<CModuleView>
{
	DECLARE_DYNCREATE(CMyCodeView)

};

#define PARENTFORDESCRIPTION CEditView//CRichEditView//CRichEgView
class CMyDescriptionView : public COXTabViewPage<PARENTFORDESCRIPTION>
{
	DECLARE_DYNCREATE(CMyDescriptionView)


	void OnInitialUpdate();
};



class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:
	CToolBar m_wndMyToolBar;
	COXTabViewContainer m_TabViewContainer;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void SetActivePage(CRuntimeClass *pSet);

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnViewDescription();
	afx_msg void OnViewForm();
	afx_msg void OnViewModule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__EAD94A5B_F3C3_4BB6_BD18_7FF7631DAC24__INCLUDED_)
