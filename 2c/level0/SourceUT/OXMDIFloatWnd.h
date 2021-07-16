// ===================================================================================
// 					Class Specification : COXMDIFloatWnd
// ===================================================================================

// Header file : OXMDIFloatWnd.h

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CMDIFloat_Parent

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
//	Used when floating a control bar in an MDI client frame

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __MDIFLOATWND_H__
#define __MDIFLOATWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXFrameWndDock.h"

#ifndef CMDIFloat_Parent
#define CMDIFloat_Parent COXMDIChildWndSizeDock		// in case a change in derivation !
#endif


class OX_CLASS_DECL COXMDIFloatWnd : public CMDIFloat_Parent
{
DECLARE_DYNCREATE(COXMDIFloatWnd)

// Data members -------------------------------------------------------------
public:

	CDockBar	m_wndMDIDockBar;		// the single docking site for controlbars

protected:

	COXMDIFloatWnd();           // protected constructor used by dynamic creation

private :

// Member functions ---------------------------------------------------------
public:

	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
	// --- In  : pParent : parent window
	//			 dwBarStyle : The style of the dockbar within this CMDIChildWnd
	//				Specifies which sides of the frame window can serve as docking sites for control bars. It can be one or more of the following:
	//				CBRS_ALIGN_TOP   Allows docking at the top of the client area.
	//				CBRS_ALIGN_BOTTOM   Allows docking at the bottom of the client area.
	//				CBRS_ALIGN_LEFT   Allows docking on the left side of the client area.
	//				CBRS_ALIGN_RIGHT   Allows docking on the right side of the client area.
	//				CBRS_ALIGN_ANY   Allows docking on any side of the client area.
	// --- Out : 
	// --- Returns :
	// --- Effect : 

	void RecalcLayout(BOOL bNotify = TRUE);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See CFrameWnd::RecalcLayout

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See CWnd::PreCreateWindow

	void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXMDIFloatWnd)
	protected:
	//}}AFX_VIRTUAL

protected:

	virtual ~COXMDIFloatWnd();

	// Generated message map functions
	//{{AFX_MSG(COXMDIFloatWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

#endif // __MDIFLOATWND_H__

