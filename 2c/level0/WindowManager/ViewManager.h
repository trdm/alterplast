#if !defined(AFX_VIEWMANAGER_H__2A1E6B81_0BC9_11D3_933D_0000F0510AC9__INCLUDED_)
#define AFX_VIEWMANAGER_H__2A1E6B81_0BC9_11D3_933D_0000F0510AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewManager.h : header file
//

#include <afxcoll.h>
#include "WindowTabCtrl.h"

// Support for different control bar implementations
#ifndef CCBaseControlBar
#define CCBaseControlBar  CControlBar 
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewManager window

class CViewManager : public CCBaseControlBar
{
	DECLARE_DYNAMIC(CViewManager)

private:
    CSize    m_sizeMin;
    CSize    m_sizeFloat;
	CSize    m_sizeDefault;
	CSize    m_sizeMRU;
	int      m_nLMargin;
	UINT     m_nDockID;
	BOOL     m_bWin2000;  // for the double-stripped gripper

// Construction
public:
	CViewManager();

// Attributes
public:

// Operations
public:
    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        CSize sizeDefault, UINT nID,
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);
	
	void OnActivateView(const BOOL bActivate, CMDIChildWnd* pView);
	int  GetWindowNum();
	void SetViewName(const TCHAR* cs, CMDIChildWnd* pView);
	void RemoveAll();
	void RemoveView(CMDIChildWnd* pView);
	void AddView(const TCHAR* csName, CMDIChildWnd* pView);

	CMDIChildWnd* GetTabInfo(int nTab, TCHAR* pszLabel = NULL) const;
	
	void SetWin2000(BOOL bWin2000);
	BOOL GetWin2000() const;

// Overrides
	virtual void DoPaint(CDC* pDC);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewManager)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewManager();
	BOOL CreateViewManager(CMDIFrameWnd* pMDIFrameWnd, UINT uID);
	CWindowTabCtrl m_ViewTabCtrl;
    CImageList     m_ViewTabImages;
	BOOL           m_bClosing;

	// Generated message map functions
protected:

	void DrawGripper(CDC* pDC);

	//{{AFX_MSG(CViewManager)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
    BOOL OnViewManagerToolTip(NMHDR* pTTTStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CViewManager inlines

inline void CViewManager::SetWin2000(BOOL bWin2000)
{
	m_bWin2000 = bWin2000;
	Invalidate();
}

inline BOOL CViewManager::GetWin2000()	const
{
	return m_bWin2000;
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMANAGER_H__2A1E6B81_0BC9_11D3_933D_0000F0510AC9__INCLUDED_)
