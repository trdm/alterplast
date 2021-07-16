// ===================================================================================
// 					Class Implementation : COXDragDockContext
// ===================================================================================

// Header file : OXDragDockContext.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxpriv.h>

#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif


#include "OXDragDockContext.h"
//#include "OXDockCtrlBarRes.h"  // for some resource strings

#include "OXSizeCtrlBar.h"
#include "OXSizeDockBar.h"
#include "OXMDIFloatWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)

// adjust supplied rectangle, so that point, pt is inside it.
// Adjusted slightly from original orientation to ensure point is now
// within specificed rectangle.
static void AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
//	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
//					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
//	rect.OffsetRect(nXOffset, nYOffset);
	rect.OffsetRect(nXOffset, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXDragDockContext::COXDragDockContext(CControlBar* pBar)
	: CDockContext(pBar)
{
}

COXDragDockContext::~COXDragDockContext()
{
}


/////////////////////////////////////////////////////////////////////////////
// COXDragDockContext operations

// start dragging. This is the only routine exposed externally. 
// pt = mouse position at start of drag (screen co-ords) 
void COXDragDockContext::StartDrag(CPoint pt)
{
   	ASSERT_VALID(m_pBar);
   	ASSERT(m_pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)));

   	COXSizeControlBar* pSzBar = (COXSizeControlBar*)m_pBar;

    // get styles from bar
   	m_dwDockStyle = m_pBar->m_dwDockStyle;
   	m_dwStyle = m_pBar->m_dwStyle & CBRS_ALIGN_ANY;
   	ASSERT(m_dwStyle != 0);
	
	// check to see we're not hanging from a COXMDIFloatWnd. 
	// Disallow dragging if we are...
	if (m_pBar->IsFloating())
	{
		CFrameWnd* pFrameWnd = m_pBar->GetParentFrame();
		ASSERT(pFrameWnd != NULL);
		ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
		if (pFrameWnd->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
			return;				// do nothing if floating inside a COXMDIFloatWnd
	}
	
	// dragging has started message (only if window will actually dock !)
    if ((m_dwDockStyle & CBRS_ALIGN_ANY) == CBRS_ALIGN_ANY)
		AfxGetMainWnd()->SendMessage(WM_SETMESSAGESTRING, IDS_MRC_STARTDOCKING);
	
    // handle pending WM_PAINT messages
    MSG msg;
    while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return;
		ASSERT(msg.message == WM_PAINT);
        DispatchMessage(&msg);
	}
	
    // initialize drag state
   	m_rectLast.SetRectEmpty();
   	m_sizeLast.cx = m_sizeLast.cy = 0;
   	m_bForceFrame = m_bFlip = m_bDitherLast = FALSE;
	
	
   	// get current bar location 
   	CRect rect;
   	m_pBar->GetWindowRect(rect);
   	m_ptLast = pt;
   	m_ptStart = pt;
	BOOL bHorz = HORZF(m_dwStyle);
	
   	// MFC includes code for flipping orientation using the shift key - I wasn't keen
	// on this... (sorry) so I've left it out for now. Some references are still left
	// in for it, in case I decide to implement it.
   	
   	// Start by working out the possible rectangles that dragging could result in.
	// These are:
	// m_rectFrameDragHorz	: floating frame, horizontal orientation
	// m_rectFrameDragVert	: floating frame, vertical orientation (not used, 'cos
	//									flipping not allowed)
	//
	// m_rectDragHorz		: docking horizontally, another bar already on this row
	// m_rectDragVert		: docking vertically, another bar already on this row
	
	// m_rectDragHorzAlone  : docking horizontally, on a new row
	// m_rectDragVertAlone  : docking vertically, on a new row
	
	
	// calculate dragging rects if you drag on the new row/column
	//
	CRect rectBorder;
	m_pDockSite->RepositionBars(0,0xffff,AFX_IDW_PANE_FIRST, 
		CFrameWnd::reposQuery,&rectBorder);
	m_pDockSite->ClientToScreen(rectBorder);
	CWnd* pLeftDockBar=m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT);
	if(pLeftDockBar!=NULL && pLeftDockBar->GetStyle()&WS_VISIBLE)
	{
		CRect rectDockBar;
		pLeftDockBar->GetWindowRect(rectDockBar);
		rectBorder.left-=rectDockBar.Width();
	}
	CWnd* pRightDockBar=m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_RIGHT);
	if(pRightDockBar!=NULL && pRightDockBar->GetStyle()&WS_VISIBLE)
	{
		CRect rectDockBar;
		pRightDockBar->GetWindowRect(rectDockBar);
		rectBorder.right+=rectDockBar.Width();
	}
	m_rectDragHorzAlone=CRect(CPoint(rectBorder.left,rect.top),rectBorder.Size());
	m_rectDragVertAlone=CRect(CPoint(rect.left,rectBorder.top),rectBorder.Size());
	m_rectDragHorzAlone.bottom=m_rectDragHorzAlone.top+pSzBar->m_HorzDockSize.cy;
	m_rectDragVertAlone.right=m_rectDragVertAlone.left+pSzBar->m_VertDockSize.cx;
	//
	//////////////////////////////////////////////////////////////


	//////////////////
	//
	int nDockAreaWidth = rectBorder.Width();
	int nDockAreaHeight = rectBorder.Height();

	CSize HorzAloneSize(nDockAreaWidth, pSzBar->m_HorzDockSize.cy);
	CSize VertAloneSize(pSzBar->m_VertDockSize.cx, nDockAreaHeight);
	
	// sizes to use when docking into a row that already has some bars.
	// use the stored sizes - unless they are > the max dock area - 
	// in which case make a guess.
	if (pSzBar->m_VertDockSize.cy >= nDockAreaHeight - 16)
		VertAloneSize.cy = nDockAreaHeight / 3;
	else 
		VertAloneSize.cy = pSzBar->m_VertDockSize.cy;
	
	if (pSzBar->m_HorzDockSize.cx >= nDockAreaWidth - 16)
		HorzAloneSize.cx = nDockAreaWidth / 3;
	else
		HorzAloneSize.cx = pSzBar->m_HorzDockSize.cx;
	

	m_rectDragHorz = CRect(rect.TopLeft(), HorzAloneSize);
	m_rectDragVert = CRect(rect.TopLeft(), VertAloneSize);
	//
	///////////////////


   	// rectangle for the floating frame...
   	m_rectFrameDragVert = m_rectFrameDragHorz = 
		CRect(rect.TopLeft(), pSzBar->m_FloatSize);
   	
   	// To work out the size we actually create a floating mini frame, and then see how big
   	// it is
	CMiniDockFrameWnd* pFloatFrame =
		m_pDockSite->CreateFloatingFrame(bHorz ? CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT);
   	if (pFloatFrame == NULL)
		AfxThrowMemoryException();
   	pFloatFrame->CalcWindowRect(&m_rectFrameDragHorz);
   	pFloatFrame->CalcWindowRect(&m_rectFrameDragVert);
// 	m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
// 	m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
   	pFloatFrame->DestroyWindow();

	// adjust rectangles so that point is inside
	AdjustRectangle(m_rectDragHorzAlone, pt);
	AdjustRectangle(m_rectDragVertAlone, pt);
   	AdjustRectangle(m_rectDragHorz, pt);
   	AdjustRectangle(m_rectDragVert, pt);
   	AdjustRectangle(m_rectFrameDragHorz, pt);
   	AdjustRectangle(m_rectFrameDragVert, pt);
	
   	// lock window update while dragging
   	ASSERT(m_pDC == NULL);
   	CWnd* pWnd = CWnd::GetDesktopWindow();
#ifndef _MAC
    if (pWnd->LockWindowUpdate()) 
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
    else
#endif
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);
    ASSERT(m_pDC != NULL); 
	
	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = CanDock();
    Move(pt);   // call it here to handle special keys
    Track();
}


void COXDragDockContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;
	
#ifdef _MAC
    // prevent dragging the floating window completely under the menu bar
    GDHandle hgd = _AfxFindDevice(pt.x, pt.y);
    if (hgd == NULL || hgd == GetMainDevice() ||
		TestDeviceAttribute(hgd, hasAuxMenuBar))
	{
		CRect rect;
		
        if ((HORZF(m_dwStyle) && !m_bFlip) || (VERTF(m_dwStyle) && m_bFlip))
			rect = m_rectFrameDragHorz;
        else
			rect = m_rectFrameDragVert;
		
        // determine our new position
		rect.OffsetRect(ptOffset);
		
        // keep us on the screen if we were getting too close to the menu bar
        int yMBarBottom = (*hgd)->gdRect.top + GetMBarHeight() + 4;
        if (rect.bottom < yMBarBottom)
		{
			pt.y += yMBarBottom - rect.bottom;
            ptOffset.y += yMBarBottom - rect.bottom;
		}
	}
#endif
	
	// offset all drag rects to new position
    m_rectDragHorz.OffsetRect(ptOffset);
    m_rectFrameDragHorz.OffsetRect(ptOffset);
    m_rectDragVert.OffsetRect(ptOffset);
    m_rectFrameDragVert.OffsetRect(ptOffset);
	
	// these rectangles only move in 1 direction

	m_rectDragHorzAlone.top += ptOffset.y;
	m_rectDragHorzAlone.bottom += ptOffset.y;
	m_rectDragVertAlone.left  += ptOffset.x;
	m_rectDragVertAlone.right += ptOffset.x;
	
    m_ptLast = pt;
	
    // if control key is down don't dock
    m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();
	
    // update feedback
    DrawFocusRect();
}


void COXDragDockContext::OnKey(int nChar, BOOL bDown)
	{
	if (nChar == VK_CONTROL)
		UpdateState(&m_bForceFrame, bDown);
	}


void COXDragDockContext::EndDrag()
{
    CancelDrag();
	if (m_ptStart == m_ptLast)
		return;
	
	m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();
	if (m_dwOverDockStyle != 0)
	{
		// dockbar we're going to dock at.
		CDockBar* pDockBar = GetDockBar();
        ASSERT(pDockBar != NULL);
    
        // check the original dockbar - if a valid CSizeDockBar...
		// work out the row number.
		CDockBar* pOrigDockBar = m_pBar->m_pDockBar;
		int nOrigCheckSum = -1;
		if (pOrigDockBar != NULL && 
			pOrigDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)))
			nOrigCheckSum = ((COXSizeDockBar*)pOrigDockBar)->CheckSumBars();

		// Now we're going to actually dock the window.
		
		// Update the appropriate size in the control bar.
		if (HORZF(m_dwOverDockStyle)) 
		{
			((COXSizeControlBar*)m_pBar)->m_HorzDockSize = m_rectDragDock.Size();
		}
		else
		{
			((COXSizeControlBar*)m_pBar)->m_VertDockSize = m_rectDragDock.Size();
		}
		
        m_pDockSite->DockControlBar(m_pBar, pDockBar, m_rectDragDock);
        
		// if into a sizeable dockbar (always we be !), then adjust other bars in the same row
		// to attempt to maintain size
		if (pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)))
		{
			if (pOrigDockBar != pDockBar || 
				((COXSizeDockBar*)pDockBar)->CheckSumBars() != nOrigCheckSum)
			{
				((COXSizeDockBar*)pDockBar)->AdjustForNewBar(m_pBar);
			}
			// force RecalcLayout below to adjust sizes always for the bar into 
			// which we have docked  - this is needed as if the bar doesn't 
			// actually changed position in the array, but has changed size 
			// (because the docking algorithm above guess the size wrong, then 
			// we need to set it back again.
			((COXSizeDockBar*)pDockBar)->m_CountBars = 0;			
		}
		// This RecalcLayout is what will adjust the size.
		m_pDockSite->RecalcLayout();
	}
    else
	{
    	m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_rectFrameDragHorz.TopLeft(),
			CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI));
		m_pBar->SendMessage(WM_USER_AFTERFLOAT_MSG);
		
		// set flag to indicate user has moved the bar - done as a style flag on the window.
		CWnd* pFrameWnd = m_pBar->GetParentFrame();
		ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)));
		pFrameWnd->ModifyStyle(0, CBRS_MOVED_BY_USER);		
	}
}


void COXDragDockContext::CancelDrag()
{
	DrawFocusRect(TRUE);    // gets rid of focus rect
    ReleaseCapture();
	
    CWnd* pWnd = CWnd::GetDesktopWindow();
#ifndef _MAC
    pWnd->UnlockWindowUpdate();
#endif
    if (m_pDC != NULL)
	{
		pWnd->ReleaseDC(m_pDC);
        m_pDC = NULL;
	}
	
	// Tell main window to clear it's status bar
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT(pMainWnd != NULL);
	pMainWnd->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void COXDragDockContext::DrawFocusRect(BOOL bRemoveRect)
{
	ASSERT(m_pDC != NULL);
	
    // default to thin frame
    CSize size(CX_BORDER, CY_BORDER);
	
    // determine new rect and size
    CRect rect;
    CBrush* pWhiteBrush = 
		CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
    CBrush* pDitherBrush = CDC::GetHalftoneBrush();
    CBrush* pBrush = pWhiteBrush;
	
    if (m_dwOverDockStyle != 0)
	{
		rect = m_rectDragDock;
	}
    else
	{
#ifndef _MAC
		// use thick frame instead
        size.cx = GetSystemMetrics(SM_CXFRAME) - CX_BORDER;
        size.cy = GetSystemMetrics(SM_CYFRAME) - CY_BORDER;
#endif
		rect = m_rectFrameDragHorz;
		pBrush = pDitherBrush;
	}
	if (bRemoveRect)
		size.cx = size.cy = 0;
	
    if ( afxData.bWin4 && 
		(HORZF(m_dwOverDockStyle) || VERTF(m_dwOverDockStyle)))
	{
		// looks better one pixel in (makes the bar look pushed down)
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
	}
	
    // draw it and remember last size
    m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
		pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
    m_rectLast = rect;
    m_sizeLast = size;
    m_bDitherLast = (pBrush == pDitherBrush);
}


void COXDragDockContext::UpdateState(BOOL* pFlag, BOOL bNewValue)
{
    if (*pFlag != bNewValue)
	{
		*pFlag = bNewValue;
        m_dwOverDockStyle = (m_bForceFrame) ? 0 : CanDock();
        DrawFocusRect();
	}
}


DWORD COXDragDockContext::CanDock()
{
	BOOL bStyleHorz;
    DWORD dwDock = 0; // Dock Canidate
    DWORD dwCurr = 0; // Current Orientation
	
    // let's check for something in our current orientation first
    // then if the shift key is not forcing our orientation then
    // check for horizontal or vertical orientations as long
    // as we are close enough
    ASSERT(m_dwStyle != 0);
	
    bStyleHorz = HORZF(m_dwStyle);
	
    m_pTargetDockBar = NULL;
	
    // can simplify this, 'cos most of the rectangles are actually the same
    if (dwDock == 0 && HORZF(m_dwDockStyle)/* && m_rectDragHorzAlone.PtInRect(m_ptLast)*/)
	{
        dwDock = m_pDockSite->CanDock(m_rectDragHorzAlone,
			m_dwDockStyle & ~CBRS_ORIENT_VERT, (CDockBar**)&m_pTargetDockBar);
	}
    if (dwDock == 0 && VERTF(m_dwDockStyle)/* && m_rectDragVertAlone.PtInRect(m_ptLast)*/)
	{
        dwDock = m_pDockSite->CanDock(m_rectDragVertAlone,
			m_dwDockStyle & ~CBRS_ORIENT_HORZ, (CDockBar**)&m_pTargetDockBar);
	}
	
	if (dwDock != 0)	// will dock somewhere - now look for 1/2 bars on the row...
	{
		if (HORZF(dwDock))
		{
			m_rectDragDock = m_rectDragHorzAlone;
			dwCurr = m_pDockSite->CanDock(m_rectDragHorz, 
				m_dwDockStyle & ~CBRS_ORIENT_VERT, (CDockBar**)&m_pTargetDockBar);

			if (dwCurr != 0)
			{
				int nBars=m_pTargetDockBar->BarsOnThisRow(m_pBar,m_rectDragHorzAlone);
				if(nBars!=0)
				{
					m_rectDragDock = m_rectDragHorz;
					m_rectDragDock.right = m_rectDragDock.left+
						m_rectDragHorzAlone.Width()/(nBars+1);
				}
			}
		}		
		
		if (VERTF(dwDock))
		{
			m_rectDragDock = m_rectDragVertAlone;
			dwCurr = m_pDockSite->CanDock(m_rectDragVert, 
				m_dwDockStyle & ~CBRS_ORIENT_HORZ, (CDockBar**)&m_pTargetDockBar);
			
			if (dwCurr != 0)
			{
				int nBars=m_pTargetDockBar->BarsOnThisRow(m_pBar,m_rectDragVertAlone);
				if(nBars!=0)
				{
					m_rectDragDock = m_rectDragVert;
					m_rectDragDock.bottom = m_rectDragDock.top+
						m_rectDragVertAlone.Height()/(nBars+1);
				}
			}
		}		
	}

    return dwCurr;
}

// should have m_dwOverDockStyle and m_rectDragDock set before calling this
// routine.
CDockBar* COXDragDockContext::GetDockBar()
{
	DWORD dw = 0;
    CDockBar* pBar;
    if (HORZF(m_dwOverDockStyle))
	{
    	dw = m_pDockSite->CanDock(m_rectDragDock,
			m_dwOverDockStyle & ~CBRS_ORIENT_VERT, &pBar);
        ASSERT(dw != 0);
        ASSERT(pBar != NULL);
        return pBar;
	}
	if (VERTF(m_dwOverDockStyle))
	{
    	dw = m_pDockSite->CanDock(m_rectDragDock,
			m_dwOverDockStyle & ~CBRS_ORIENT_HORZ, &pBar);
        ASSERT(dw != 0);
        ASSERT(pBar != NULL);
        return pBar;
	}
    return NULL;
}


BOOL COXDragDockContext::Track()
{
    // don't handle if capture already set
    if (::GetCapture() != NULL)
        return FALSE;
	
    // set capture to the window which received this message
    m_pBar->SetCapture();
    ASSERT(m_pBar == CWnd::GetCapture());
	
    // get messages until capture lost or cancelled/accepted
    while(CWnd::GetCapture() == m_pBar)
	{
        MSG msg;
#ifndef _MAC
        if (!::GetMessage(&msg, NULL, 0, 0))
#else
			// don't allow yielding while tracking since we don't have LockWindowUpdate
			if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE|PM_NOYIELD))
            continue;
        if (msg.message == WM_QUIT)
#endif
		{
            AfxPostQuitMessage(msg.wParam);
            break;
		}
		
        switch (msg.message)
		{
            case WM_LBUTTONUP:		// drag finished 
                EndDrag();
                return TRUE;
				
            case WM_MOUSEMOVE:
                Move(msg.pt);
                break;
				
            case WM_KEYUP:
                OnKey((int)msg.wParam, FALSE);
                break;
				
            case WM_KEYDOWN:
				OnKey((int)msg.wParam, TRUE);
                if (msg.wParam == VK_ESCAPE)
					goto exit_cancel_drag;
                break;
				
			case WM_RBUTTONDOWN:
				goto exit_cancel_drag;
				
				// just dispatch rest of the messages
            default:
                DispatchMessage(&msg);
				break;
		}
	}
    
exit_cancel_drag:			// goto  - can't use break as we're inside a switch()    
	CancelDrag();
	return FALSE;
}

// Wow - another genuine virtual function in CDockContext !
// overridden to prevent windows floated in an MDI window from being double-clicked
void COXDragDockContext::ToggleDocking()
{
   	ASSERT_VALID(m_pBar);
   	ASSERT(m_pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)));

	// check to see we're not hanging from a COXMDIFloatWnd. Disallow toggle docking if we are
	if (m_pBar->IsFloating())
	{
		CFrameWnd* pFrameWnd = m_pBar->GetParentFrame();
		ASSERT(pFrameWnd != NULL);
		ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
		if (pFrameWnd->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
			return;				// do nothing if floating inside a COXMDIFloatWnd
	}
	

	CDockContext::ToggleDocking();
}
