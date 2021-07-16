// Авторские права - VTOOLS.RU (info@vtools.ru)
// SampleView.h : interface of the CModuleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEVIEW_H__B1B69ED3_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
#define AFX_SAMPLEVIEW_H__B1B69ED3_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SyntaxColor/CCrystalEditView.h"
#include "AutocomplectionCtrl/AutocomplectionCtrl.h"
class CModuleDoc;

class CModuleView : public CAutocomplectionCtrl
{
public:
	CModuleView();
	DECLARE_DYNCREATE(CModuleView)

// Attributes
public:
	bool bSingleModule;

	CString	csGotoModuleName;
	int		nGotoLine;

// Operations
public:
	CModuleDoc* GetDocument(){ return (CModuleDoc*)m_pDocument; }

	int GotoProc(CString csObj,CString csName);
	void DelCurLine();
	CString GetName();
	void RunDebug();
	void SaveAsHtml(CString lpszPathName);
	int GetContextProc();//CArray <CString,CString> &aListName,CArray <CString,CString> &aListDescription,CArray <UINT,UINT> &aListImage,CArray <UINT,UINT> &aListLine);//,BOOL bGetVariable=0);
	virtual CCrystalTextBuffer *LocateTextBuffer();
	void GotoLine(UINT nLine,BOOL bSetRunLine=0);
	void AddGotoInfo(CMenu* pPopup);

protected:
//	virtual DWORD ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModuleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
public:
	//{{AFX_MSG(CModuleView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnButtoncomment();
	afx_msg void OnButtondelcomment();
	afx_msg void OnButtonproc();
	afx_msg void OnButtongotoline();
	afx_msg void OnUpdateFormatBlock(CCmdUI* pCmdUI);
	afx_msg void OnHelpSearchCombo();
	afx_msg void OnEditDebugPoint();
	afx_msg void OnUpdateEditDebugPoint(CCmdUI* pCmdUI);
	afx_msg void OnNextDebug();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNextDebugOver();
	afx_msg void OnRunDebug();
	afx_msg void OnStopDebug();
	afx_msg void OnUpdateStopDebug(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditGoto();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEVIEW_H__B1B69ED3_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
