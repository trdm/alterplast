// ===================================================================================
// 					Class Implementation : COXMDIFloatWnd
// ===================================================================================

// Header file : OXMDIFloatWnd.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxpriv.h>
#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif


#include "OXMDIFloatWnd.h"

#include "OXSizeDockBar.h"
#include "OXSizeCtrlBar.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

// Helper function: used below
void AdjustForBorders(CRect& rect, DWORD dwStyle);

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

IMPLEMENT_DYNCREATE(COXMDIFloatWnd, CMDIFloat_Parent)

COXMDIFloatWnd::COXMDIFloatWnd()
	: m_wndMDIDockBar(TRUE)
	{
	m_wndMDIDockBar.m_bAutoDelete = FALSE;
	}

COXMDIFloatWnd::~COXMDIFloatWnd()
	{
	}


BEGIN_MESSAGE_MAP(COXMDIFloatWnd, CMDIFloat_Parent)
//{{AFX_MSG_MAP(COXMDIFloatWnd)
ON_WM_SIZE()
ON_WM_SHOWWINDOW()
ON_WM_CLOSE()
ON_WM_WINDOWPOSCHANGED()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXMDIFloatWnd message handlers

BOOL COXMDIFloatWnd::Create(CWnd* pParent, DWORD dwBarStyle)
	{
	// set m_bInRecalcLayout to avoid flashing during creation
	// RecalcLayout will be called once something is docked
	m_bInRecalcLayout = TRUE;
	
	static CString strFloatClass;
	if (strFloatClass.IsEmpty())
		strFloatClass = AfxRegisterWndClass(CS_DBLCLKS);

	DWORD dwStyle = WS_CLIPCHILDREN | WS_CHILD | WS_OVERLAPPEDWINDOW | FWS_SNAPTOBARS;
	if (!CMDIFloat_Parent::Create(strFloatClass, NULL, dwStyle, rectDefault, NULL  /*(CMDIFrameWnd *)pParent)*/))
		{
		m_bInRecalcLayout = FALSE;
		return FALSE;
		}
	
	// Create a single dockbar for this frame with ID=AFX_IDW_DOCKBAR_FLOAT (picked up later)
	dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT) ? CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
	dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;
	
	if (!m_wndMDIDockBar.Create(pParent, WS_CHILD | WS_VISIBLE | dwStyle, AFX_IDW_DOCKBAR_FLOAT))
		{
		m_bInRecalcLayout = FALSE;
		return FALSE;
		}
	m_wndMDIDockBar.SetParent(this);
	
	m_bInRecalcLayout = FALSE;
	return TRUE;			 
	}

BOOL COXMDIFloatWnd::PreCreateWindow(CREATESTRUCT& cs) 
	{
	// Turn off the WS_EX_CLIENTEDGE style on the Frame.
	if (!CMDIFloat_Parent::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
	}


// respond to the miniframe resizing. If we've got a sizeable control
// bar in the window, then we set it's size. Need to adjust for the
// window borders. The window will then get repositioned by a ReCalcLayout()
void COXMDIFloatWnd::OnSize(UINT nType, int cx, int cy) 
	{
    if (cx == 0 && cy == 0)
		return;
	
	CDockBar* pDock = &m_wndMDIDockBar;
	
    // We don't support CBRS_FLOAT_MULTI
    if ((pDock->m_dwStyle & CBRS_FLOAT_MULTI) == 0)
		{
        // CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
        // in the array
        COXSizeControlBar* pBar = ((COXSizeDockBar*)pDock)->GetFirstControlBar();
        // ignore size request if not visible....
        if (pBar != NULL && COXSizeDockBar::IsSizeable(pBar) && pBar->IsVisible())
			{
			CRect rect(0, 0, cx, cy);
			AdjustForBorders(rect, pBar->m_dwStyle);
			
            pBar->m_FloatSize.cx = rect.Width();
            pBar->m_FloatSize.cy = rect.Height();
			}
		}
	CMDIFloat_Parent::OnSize(nType, cx, cy);	// call parent to update menu's etc.
	}


void COXMDIFloatWnd::OnClose() 
	{
	CControlBar* pBar = NULL;
	CDockBar* pDock = &m_wndMDIDockBar;
	
	if ((pDock->m_dwStyle & CBRS_FLOAT_MULTI) == 0)
		{
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		pBar = ((COXSizeDockBar*) pDock)->GetFirstControlBar();
		if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)) )
			if (((COXSizeControlBar*)pBar)->m_Style & SZBARF_DESTROY_ON_CLOSE)
            {
			// close the Frame Window
			CMDIFloat_Parent::OnClose();     // close the window
			delete pBar;					// now explicitly delete the control bar
			return;
			}
		} 
	
    // otherwise just close it. .. really just want to hide it...
	((COXSizeControlBar*)pBar)->OnHide();
	}


void COXMDIFloatWnd::RecalcLayout(BOOL bNotify)
	{
	if (!m_bInRecalcLayout)
		{
		CMDIFloat_Parent::RecalcLayout(bNotify);
		
		// syncronize window text of frame window with dockbar itself
		CString strTitle;
		m_wndMDIDockBar.GetWindowText(strTitle);
		AfxSetWindowText(m_hWnd, strTitle);
		}
	}


// Updates the floating position for a bar. 
void COXMDIFloatWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
	{
	CMDIFloat_Parent::OnWindowPosChanged(lpwndpos);
	
	if ((m_wndMDIDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
	    {
		COXSizeControlBar* pBar = ((COXSizeDockBar*) &m_wndMDIDockBar)->GetFirstControlBar();
     	if (pBar != NULL)
 			{
			ASSERT(pBar->m_pDockContext != NULL);
			CRect rcWindow;
			GetWindowRect(rcWindow);
			pBar->m_pDockContext->m_ptMRUFloatPos = rcWindow.TopLeft();
 			}	
		}
	}

void COXMDIFloatWnd::ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay)
	{
	CMDIFloat_Parent::ShowControlBar(pBar, bShow, bDelay);
	}

void COXMDIFloatWnd::OnShowWindow(BOOL bShow, UINT nStatus)
	{
	if (nStatus == 0 && bShow == TRUE)
		SetWindowPos(&wndTop, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE);
	CMDIFloat_Parent::OnShowWindow(bShow, nStatus);
	}