// MyHelpView.h : interface of the CMyHelpView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYHELPVIEW_H__2798AD80_5851_424C_BC65_34524FA22426__INCLUDED_)
#define AFX_MYHELPVIEW_H__2798AD80_5851_424C_BC65_34524FA22426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

class CTestForMyHelpDocCntrItem;

class CMyHelpView : public CRichEditView
{
protected: // create from serialization only
	CMyHelpView();
	DECLARE_DYNCREATE(CMyHelpView)

// Attributes
public:
	CMyHelpDoc* GetDocument();

// Operations
public:
	void SetText(CString Str);
	int GetText(CString &Str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyHelpView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyHelpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyHelpView)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyHelpView.cpp
inline CMyHelpDoc* CMyHelpView::GetDocument()
   { return (CMyHelpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYHELPVIEW_H__2798AD80_5851_424C_BC65_34524FA22426__INCLUDED_)
