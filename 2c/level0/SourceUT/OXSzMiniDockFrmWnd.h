// ===================================================================================
// 					Class Specification : COXSizableMiniDockFrameWnd
// ===================================================================================

// Header file : OXSzMiniDockFrmWnd.h

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CMiniDockFrameWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// This class implements all the sizeable functionality a mini frame window needs.
// Normally a CMiniDockFrameWindow is not resizeable.

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SIZEMINIDOCKFRM_H__
#define __SIZEMINIDOCKFRM_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxpriv.h>
#include "OXDragDockContext.h"


class OX_CLASS_DECL COXSizableMiniDockFrameWnd : public CMiniDockFrameWnd
{
	friend COXDragDockContext; 		// access to IgnoreSysMove flag

// Data members -------------------------------------------------------------
public:

	enum ContainedBarType { Unknown, MFCBase, MRCSizeBar } ;

	BOOL m_bBeingDestroyed;

protected:

	enum ContainedBarType m_nContainedBarType;	

private :

// Member functions ---------------------------------------------------------
public:

	enum ContainedBarType GetContainedBarType();
	// --- In  :
	// --- Out : 
	// --- Returns : the type of the contained bar
	// --- Effect : Floating frames are created from CFrameWnd::CreateFloatingFrame(), and at
	//				this point, we don't know what type of control bar will eventually be in
	//				this frame. This routine determines the type of control bar, and sets the
	//				neccessary styles. Generally this routine gets called as soon as we do a
	//				WM_NCHITTEST on the window - ie before the user can get a chance to
	//				manipulate it with the mouse
	//
	//				COXSizeControlBar - default style of WS_THICKFRAME is ok (we've overridden
	//									the create too) but we need to ensure "SC_SIZE" is on
	//									the menu, otherwise we can't the default WM_NCLBUTTONDOWN
	//									won't generate an SC_SIZE.
	//
	//				CControlBar, CBRS_SIZE_DYNAMIC
	//								  - set MFS_4THICKFRAME (disallow diagonal sizing)
	//
	//				CControlBar, not CBRS_SIZE_DYNAMIC
	//								  - remove WS_THICKFRAME & add MFS_MOVEFRAME (we don't want
	//									sizing hit test values)

	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }
	BOOL IsDockingViewBar();

protected:

//{{AFX_MSG(COXSizableMiniDockFrameWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint pt);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnInitMenuPopup(CMenu*, UINT, BOOL);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//}}AFX_MSG

    DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNCREATE(COXSizableMiniDockFrameWnd)

//{{AFX_VIRTUAL(COXSizableMiniDockFrameWnd)
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//}}AFX_VIRTUAL

};

#endif // __SIZEMINIDOCKFRM_H__

