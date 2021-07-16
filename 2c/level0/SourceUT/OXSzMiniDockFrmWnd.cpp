// ===================================================================================
// 					Class Implementation : COXSizableMiniDockFrameWnd
// ===================================================================================

// Header file : OXSzMiniDockFrmWnd.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxole.h>
#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif


#include "OXSzMiniDockFrmWnd.h"

#include "OXSizeCtrlBar.h"
#include "OXSizeDockBar.h"
#include "OXFrameWndDock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COXSizableMiniDockFrameWnd, CMiniDockFrameWnd)

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

BEGIN_MESSAGE_MAP(COXSizableMiniDockFrameWnd, CMiniDockFrameWnd)
	//{{AFX_MSG_MAP(COXSizableMiniDockFrameWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_NCLBUTTONDBLCLK()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_WINDOWPOSCHANGED()
    ON_WM_NCHITTEST()
	ON_WM_MOUSEACTIVATE()
    ON_WM_NCACTIVATE()
	ON_WM_PARENTNOTIFY()
	ON_WM_SYSCOMMAND()
    ON_WM_INITMENUPOPUP()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
    // Global help commands
END_MESSAGE_MAP()

int COXSizableMiniDockFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
	if (CMiniDockFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    m_nContainedBarType = Unknown;

	m_bBeingDestroyed=FALSE;

	return 1;
	}

void COXSizableMiniDockFrameWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
	// in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
			if(::IsWindow(pMDIChild->m_hWnd))
			{
				if(nID==pMDIChild->m_nDockMessageID)
				{
					pMDIChild->OnMakeItDockable();
					return;
				}
			}
		}
	}

	CMiniDockFrameWnd::OnSysCommand(nID, lParam);
}

// respond to the miniframe resizing. If we've got a sizeable control
// bar in the window, then we set it's size. Need to adjust for the
// window borders. The window will then get repositioned by a ReCalcLayout()
void COXSizableMiniDockFrameWnd::OnSize(UINT /* nType */, int cx, int cy)
	{
    if (cx == 0 && cy == 0)
		return;
	
    // We don't support CBRS_FLOAT_MULTI
    if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
		{
        // CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
        // in the array
        COXSizeControlBar* pBar = ((COXSizeDockBar*)(&m_wndDockBar))->GetFirstControlBar();
        // ignore size request if not visible....
        if (pBar != NULL && COXSizeDockBar::IsSizeable(pBar) && pBar->IsVisible())
			{
			CRect rect(0, 0, cx, cy);
			AdjustForBorders(rect, pBar->m_dwStyle);
			
            pBar->m_FloatSize.cx = rect.Width();
            pBar->m_FloatSize.cy = rect.Height();
			}
		}
    RecalcLayout();
	}


BOOL COXSizableMiniDockFrameWnd::PreCreateWindow(CREATESTRUCT &cs)
	{
	// modify frame style so it is fully sizeable - we will modify this again later
	// if we discover that we have a standard MFC control bar inside the frame
	// turn on thick frame styles. MFS_THICKFRAME is what's expected, but also need WS_THICKFRAME,
	// as NT expects this to be able to do the resizing.
	cs.style |= MFS_THICKFRAME | WS_THICKFRAME; 
    cs.style &= ~( MFS_MOVEFRAME | MFS_4THICKFRAME );		
    return CMiniDockFrameWnd::PreCreateWindow(cs);
	}



// over-ridden so we can find out the type of the bar in this window
UINT COXSizableMiniDockFrameWnd::OnNcHitTest(CPoint point)
{
	return CMiniDockFrameWnd::OnNcHitTest(point);
}


// if we've got a COXSizeControlBar inside this frame, and it's a resize,
// then we can use the default window's sizing. 
void COXSizableMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	enum ContainedBarType Type = GetContainedBarType();
	if (Type == MRCSizeBar)
	{
		if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
		{
			// special activation for floating toolbars
			ActivateTopParent();
			CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
			return;
		}
		else if (nHitTest == HTSYSMENU)
		{
			// do the system menu - ie give move,size,hide, etc options as would Win95.						
			InvertSysMenu();
			CPoint pt(0,0);
			ClientToScreen(&pt);
			CRect rcSysIcon;
			GetWindowRect(&rcSysIcon);
			rcSysIcon.right = rcSysIcon.left + 12;		// NB:hard-coded value for width of system icon
			rcSysIcon.bottom = pt.y;					
			
			CMenu* pSysMenu = GetSystemMenu(FALSE);
			int nCmd = pSysMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this, & rcSysIcon);
			InvertSysMenu();
			if (nCmd != 0)
			{
				SendMessage(WM_SYSCOMMAND, nCmd, 0);
			}
			return;
		}
	}

	CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
}


int COXSizableMiniDockFrameWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// activate this window if necessary
	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
	// in the array
	CControlBar* pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
	{
		pBar->SendMessage(WM_ACTIVATEVIEWBAR,TRUE,ID_SOURCE_MINIDOCK);
	}

	enum ContainedBarType Type = GetContainedBarType();
	if (Type == MRCSizeBar)
	{
		if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
			return CMiniFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return CMiniDockFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}



// returns the type of the contained bar
// Floating frames are created from CFrameWnd::CreateFloatingFrame(), and at this point,
// we don't know what type of control bar will eventually be in this frame. This routine
// determines the type of control bar, and sets the neccessary styles. Generally this routine gets
// called as soon as we do a WM_NCHITTEST on the window - ie before the user can get a chance to
// manipulate it with the mouse
//
// COXSizeControlBar - default style of WS_THICKFRAME is ok (we've overridden the create too)
//					    but we need to ensure "SC_SIZE" is on the menu, otherwise we can't
//						the default WM_NCLBUTTONDOWN won't generate an SC_SIZE.
//
// CControlBar, CBRS_SIZE_DYNAMIC
//					  - set MFS_4THICKFRAME (disallow diagonal sizing)
//
// CControlBar, not CBRS_SIZE_DYNAMIC
//					  - remove WS_THICKFRAME & add MFS_MOVEFRAME (we don't want sizing hit test values)
enum COXSizableMiniDockFrameWnd::ContainedBarType 
COXSizableMiniDockFrameWnd::GetContainedBarType()
	{
	if (m_nContainedBarType == Unknown)
		{
		m_nContainedBarType = MFCBase;
		if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0) // don't handle this
			{
			COXSizeControlBar* pBar = ((COXSizeDockBar*)(&m_wndDockBar))->GetFirstControlBar();
			if (pBar != NULL)
				{
				if (COXSizeDockBar::IsSizeable(pBar))
					{		
					m_nContainedBarType = MRCSizeBar;
					GetSystemMenu(TRUE);					// reset the system menu
					
					// delete sys menu items that might be present....
					CMenu* pSysMenu = GetSystemMenu(FALSE);
					pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
					pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
					pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
					pSysMenu->DeleteMenu(SC_TASKLIST, MF_BYCOMMAND);
					while (pSysMenu->DeleteMenu(0, MF_BYCOMMAND));	// remove anything with ID=0
					
					// if window is meant to close, ensure sysmenu has "Close" on it, not "Hide"
					// MFC4.0 replaces "Close" with "Hide"
					// extract close text from ID_FILE_CLOSE after the "\n" for the tooltip prompt
					if (!(pBar->m_Style & SZBARF_DESTROY_ON_CLOSE))
						{
						CString strHide;
						if (strHide.LoadString(AFX_IDS_HIDE))
							{
							// modify menu text to be "Hide" instrad of close
							VERIFY(pSysMenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_STRING, SC_CLOSE, strHide));  
							} 
						}
					}
				else
					{
					if ((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC))	 // dynamic bar - turn on MFS_4THICKFRAME
						{
						ModifyStyle(0, MFS_4THICKFRAME);	
						return m_nContainedBarType;
						}
					
					}
				}
			}
		if (m_nContainedBarType == MFCBase)
			{
			ModifyStyle(WS_THICKFRAME, MFS_MOVEFRAME);
			}
		}		
	// if bar is MFC bar (and not CBRS_SIZE_DYNAMIC, turn on MFS_MOVEFRAME)
	return m_nContainedBarType;
	}


void COXSizableMiniDockFrameWnd::OnClose()
{
	if(m_bBeingDestroyed)
	{
		return;
	}

	m_bBeingDestroyed=TRUE;

	if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
	{
		CControlBar* pBar = NULL;
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		if(pBar!=NULL && pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)))
		{
			if(((COXSizeControlBar*)pBar)->m_Style & SZBARF_DESTROY_ON_CLOSE)
            {
				// close the Frame Window
				CFrameWnd::OnClose();       // close the window
				delete pBar;            // now explicitly delete the control bar
				return;
			}
		}
		
		ASSERT(pBar != NULL);
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXSizeViewBar* pViewBar=(COXSizeViewBar*)pBar;
			if(::IsWindow(pViewBar->m_hWnd) && !pViewBar->IsBeingDestroyed())
			{
				if(!pViewBar->QueryCloseView())
				{
					// document can't be closed right now -- don't close it
					m_bBeingDestroyed=FALSE;
					return;
				}
				
				pViewBar->m_pChildWnd->GetMDIFrame()->
					ShowControlBar(pViewBar,FALSE,FALSE);
				pViewBar->m_pDockContext->ToggleDocking();
				pViewBar->m_WrapperWnd.SendMessage(WM_CLOSE);
				return;
			}
		}
	}

    // otherwise just hide it.
    CMiniDockFrameWnd::OnClose();

	m_bBeingDestroyed=FALSE; 

    return;
}

BOOL COXSizableMiniDockFrameWnd::OnNcActivate(BOOL bActive)
{
	// CMiniDockFrameWnd class assumes if there is only 1 bar, 
	// then it's at position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
			if(::IsWindow(pMDIChild->m_hWnd))
			{
				if(pMDIChild->MDIGetActive()!=pMDIChild && bActive)
					return FALSE;
				else
					return Default();
			}
		}
	}

	return CMiniDockFrameWnd::OnNcActivate(bActive);
}

void COXSizableMiniDockFrameWnd::OnParentNotify(UINT message, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if(message==WM_LBUTTONDOWN || message==WM_RBUTTONDOWN || message==WM_MBUTTONDOWN)
	{
		// activate this window if necessary
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		CControlBar* pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		pBar->SendMessage(WM_ACTIVATEVIEWBAR,TRUE,ID_SOURCE_MINIDOCK);
	}
}

void COXSizableMiniDockFrameWnd::OnInitMenuPopup(CMenu* pMenu, UINT, BOOL bSysMenu)
{
	AfxCancelModes(m_hWnd);

	if (bSysMenu)
	{
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		if(pBar!=NULL)
		{
			ASSERT_KINDOF(CControlBar, pBar);
			if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
			{
				COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
				if(::IsWindow(pMDIChild->m_hWnd))
				{
					if(pMenu->GetMenuState(pMDIChild->m_nDockMessageID,	
						MF_BYCOMMAND)==0xffffffff)
					{
						//HACK
//						pMenu->RemoveMenu(MF_BYPOSITION,2);

						pMenu->AppendMenu(MF_SEPARATOR);
						pMenu->AppendMenu(pMDIChild->m_bDockable ? 
							MF_CHECKED : MF_UNCHECKED, pMDIChild->m_nDockMessageID, 
							pMDIChild->m_sDockMenuItem);
					}
				}
			}
		}
	}
}

void COXSizableMiniDockFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMiniDockFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at 
	// position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			pBar->SendMessage(WM_ACTIVATEVIEWBAR,nState==WA_INACTIVE ? FALSE : TRUE,
				ID_SOURCE_MINIDOCK);
		}
	}
}


BOOL COXSizableMiniDockFrameWnd::IsDockingViewBar()
{
	ASSERT_VALID(this);

	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at 
	// position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
			return TRUE;
	}

	return FALSE;
}

// convert MiniDock size to a client size.. or vice versa.
void MiniDockToClient(CRect & rect, BOOL bConvertToClient)
{
	static int nCaptionY = -1;
	static int nBorderX, nBorderY;
	
	// if not set up, create a temporary floating frame to see how big it is.
	if (nCaptionY == -1)
	{
		CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		CMiniDockFrameWnd* pTmpFloatFrame = pMainFrame->CreateFloatingFrame(0);
		
		// calculate frame dragging rectangle
		CRect rcFloat(0,0,0,0);
		
		pTmpFloatFrame->CalcWindowRect(&rcFloat);
		
		rcFloat.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		
		nCaptionY = - rcFloat.top;
		nBorderY = rcFloat.bottom;
		nBorderX = rcFloat.right;
		
		pTmpFloatFrame->DestroyWindow();
	}		
	
	if (bConvertToClient)
	{
		rect.left += nBorderX;
		rect.right -= nBorderX;
		rect.top += nCaptionY;
		rect.bottom -= nBorderY;
	}
	else
	{
		rect.left -= nBorderX;
		rect.right += nBorderX;
		rect.top -= nCaptionY;
		rect.bottom += nBorderY;
	}
}


// Helper function: used below
// adjusts the size, depending on the borders specified by the CControlBar style
void AdjustForBorders(CRect& rect, DWORD dwStyle)
{
	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top -= afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom += afxData.cyBorder2;
}
