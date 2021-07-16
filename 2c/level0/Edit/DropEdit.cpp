/////////////////////////////////////////////////////////////////////////////
// DropEdit.cpp : implementation of SECDropEdit - subclassed edit with little
//				  drop down button.
//
// Stingray Software Extension Classes
// Copyright (C) 1996-1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Don			20/01/97
//


#include "stdafx.h"

#ifdef _SECDLL
#undef AFXAPI_DATA
#define AFXAPI_DATA __based(__segname("_DATA"))
#endif //_SECDLL

#ifndef __DROPEDIT_H__
#include "dropedit.h"
#endif

#ifndef __SECBTNS_H__
#include "SLoadBmp.h"
#endif

#ifndef __SECIMPL_H__
#include "SecImpl.h"
#endif

#ifndef __SECAUX_H__
#include "secaux.h"
#endif

#ifndef __SLOADBMP_H__
#include "SLoadBmp.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Internal structure - holds cache of system colors
extern SEC_AUX_DATA secData;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//  SECDropEdit
//
//  CPARENTEDIT derivative providing a combo like drop down button
//

IMPLEMENT_DYNAMIC(SECDropEdit, CEdit)

BEGIN_MESSAGE_MAP(SECDropEdit, CPARENTEDIT)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ENABLE()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit - construction/destruction
//
//@doc SECDropEdit
//@mfunc Constructs an SECDropEdit object.
//@xref <c SECDropEdit>
SECDropEdit::SECDropEdit() : m_wState(RightAlign), m_wLeftBorder(1),
							 m_wRightBorder(1), m_btnRect(0, 0, 0, 0)
{
}

//@doc SECDropEdit
//@mfunc SECDropEdit destructor.
//@xref <c SECDropEdit>
SECDropEdit::~SECDropEdit()
{
}

//@doc SECDropEdit
//@mfunc Attaches an existing edit control to the drop edit object.
//@rdesc Nonzero if the edit control was attached successfully, otherwise 0.
//@parm int | nCtlID | The ID of the existing edit control.
//@parm  CWnd* | pParentWnd | Pointer to the parent window.
//@xref <c SECDropEdit> <mf SECDropEdit::Create>
//@xref <mf SECDropEdit::CreateEx>
BOOL SECDropEdit::AttachEdit(int nCtlID, CWnd* pParentWnd)
{
	// Attach to an existing edit control
	ASSERT_VALID(this);
	ASSERT(pParentWnd != NULL);

	ASSERT(::IsWindow(pParentWnd->m_hWnd));
	ASSERT(::GetDlgItem(pParentWnd->m_hWnd, nCtlID) != NULL);	

	// If this ASSERT fails, the resource associated with this control id
	// is already attached to a CWnd object.  If this is a dialog resource,
	// a good place to look to see if this is happening is in your DoDataExchange 
	// method.  
	ASSERT(FromHandlePermanent(::GetDlgItem(pParentWnd->m_hWnd, nCtlID)) == NULL);

	secData.UpdateSysColors();

	if(!SubclassDlgItem(nCtlID, pParentWnd))
	{
		TRACE1("SECDropEdit could not attach to edit %d\n", nCtlID);
		return FALSE;
	}

	ASSERT_VALID(this);

	if(m_bmp.m_hObject)
	{
		// We have a bitmap, so we need to adjust the non-client area to include
		// the button. This calculation is performed in the WM_NCCALCSIZE 
		// handler, so force the non-client area to be recalculated.
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					   SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	}

	return TRUE;
}

//@doc SECDropEdit
//@mfunc Creates a window and attaches it to the drop edit object.
//@rdesc Nonzero if the control is created, otherwise 0.
//@parm DWORD | dwStyle | Specifies the window style attributes.
//@parm  const RECT& | rect | Specifies the rectangle for the drop-down button.
//@parm  CWnd* | pParentWnd | Pointer to the parent window.
//@parm  UINT | nID | The ID for the child control.
//@xref <c SECDropEdit> <mf SECDropEdit::CreateEx>
// <mf SECDropEdit::AttachEdit>
BOOL SECDropEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CreateEx(0,dwStyle,rect,pParentWnd,nID);
}


//@doc SECDropEdit
//@mfunc Creates a window with extended attributes and attaches it to the 
// drop edit object.
//@rdesc Nonzero if the control is created, otherwise 0.
//@parm DWORD | dwExStyle | Specifies the extended window style attributes.
//@parm DWORD | dwStyle | Specifies the window style attributes.
//@parm  const RECT& | rect | Specifies the rectangle for the drop-down button.
//@parm  CWnd* | pParentWnd | Pointer to the parent window.
//@parm  UINT | nID | The ID for the child control.
//@xref <c SECDropEdit> <mf SECDropEdit::Create>
// <mf SECDropEdit::AttachEdit>
BOOL SECDropEdit::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	secData.UpdateSysColors();

//	if(!CPARENTEDIT::Create(dwStyle, rect, pParentWnd, nID))
//		return FALSE;

//	if(!CPARENTEDIT::CreateEx(dwExStyle, _T("EDIT"), _T(""), dwStyle, rect, pParentWnd, nID))
//		return FALSE;

	CRect r( rect );
	if ( !CPARENTEDIT::CreateEx(dwExStyle, _T("EDIT"),_T(""), dwStyle, r.left, r.top,
		r.Width(), r.Height(), pParentWnd->m_hWnd, (HMENU)nID))
		return FALSE;


	ASSERT_VALID(this);

	if(m_bmp.m_hObject)
	{
		// We have a bitmap, so we need to adjust the non-client area to include
		// the button. This calculation is performed in the WM_NCCALCSIZE 
		// handler (ignored on edit creation), so force the non-client area 
		// to be recalculated.
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					   SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit attributes
//
//@doc SECDropEdit
//@mfunc Manipulates alignment of the button. 
//@comm The button may be aligned to the
// left or the right of the edit control.
//@rdesc void 
//@parmopt BOOL | bRightAlign | TRUE | If true, aligns the button to 
// the right of the edit control, otherwise, to the left.
//@xref <c SECDropEdit> <mf SECDropEdit::IsRightAligned>
//@end
void SECDropEdit::SetRightAlign(BOOL bRightAlign /* = TRUE */)
{
	// Button will be aligned to right of edit control if bRightAlign is
	// TRUE, otherwise it will be aligned to the left.

	WORD wSaveState = m_wState;
	if(bRightAlign)
		m_wState |= RightAlign;
	else
		m_wState &= ~RightAlign;

	if(m_wState != wSaveState && m_hWnd && m_bmp.m_hObject)
	{
		// Alignment has changed, so Force the non-client to be recalculated
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					   SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	}
}

//@doc SECDropEdit
//@mfunc Determines whether drop-down button is right-aligned within 
// the edit control.
//@rdesc Nonzero if the button is aligned to the right of the edit control,
// or 0 if the button is aligned to the left of the edit control.
//@xref <c SECDropEdit> <mf SECDropEdit::SetsRightAligned>
BOOL SECDropEdit::IsRightAligned() const
{
	// Returns TRUE if we are currently right aligned.
	return (m_wState & RightAlign) ? TRUE : FALSE;
}

//@doc SECDropEdit
//@mfunc Manipulates the border settings.
//@rdesc void 
//@parm WORD | wLeftBorder | Value for left border.
//@parm  WORD | wRightBorder | Value for right border
//@xref <c SECDropEdit> <mf SECDropEdit::GetBorders>
void SECDropEdit::SetBorder(WORD wLeftBorder, WORD wRightBorder)
{
	// We set the the gap between the button borders and the bitmap

	if(wLeftBorder == m_wLeftBorder && wRightBorder == m_wRightBorder)
		// No change
		return;

	m_wLeftBorder  = wLeftBorder;
	m_wRightBorder = wRightBorder;

	if(m_hWnd && m_bmp.m_hObject)
	{
		// Force the non-client to be recalculated
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					   SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	}
}

//@doc SECDropEdit
//@mfunc Returns the left and right border offsets in pixels.
//@rdesc The left and right border offset, in pixels.  The left border 
// is the low word of the returned DWORD, and the right border is the high word.
//@xref <c SECDropEdit> <mf SECDropEdit::SetBorder>
DWORD SECDropEdit::GetBorders() const
{
	// Returns left border in low word and right border in high word
	return MAKELONG(m_wLeftBorder, m_wRightBorder);
}


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit operations
//
//@doc SECDropEdit
//@mfunc Sets the bitmap on the drop-down button.  
//@comm The third version of 
// SetBitmap (the one with no parameters) removes the button.
//@syntax SetBitmap(LPCTSTR lpszBmpName);
//@syntax SetBitmap(UINT nBmpID);
//@syntax SetBitmap();
//@rdesc Nonzero if the bitmap is set; otherwise 0.
//@parm LPCTSTR | lpszBmpName | Null-terminated string containing bitmap’s name.
//@parm UINT | nBmpID | ID of a bitmap.
//@xref <c SECDropEdit> <mf SECDropEdit::GetBitmap>
BOOL SECDropEdit::SetBitmap(LPCTSTR lpszBmpName)
{
	// Loads the given bitmap for display on the button face.

	BOOL bRes = TRUE;

	// Remove the existing bitmap.
	if(m_bmp.m_hObject)
		m_bmp.DeleteObject();

	m_btnRect.SetRectEmpty();
	m_lpszBmpName = lpszBmpName;
	if(m_lpszBmpName != NULL)
	{
		// Load the given bitmap, mapping it to the current system colors.
		if(!SECLoadSysColorBitmap(m_bmp, m_lpszBmpName))
		{
			m_lpszBmpName = NULL;

			TRACE0("SECDropEdit failed to load button bitmap\n");
			bRes = FALSE;
		}
	}

	if(m_hWnd)
	{
		// Force the non-client to be recalculated for the new bitmap
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					   SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	}

	return bRes;
}

BOOL SECDropEdit::SetBitmap(UINT nBmpID)
{
	return SetBitmap(MAKEINTRESOURCE(nBmpID));
}

BOOL SECDropEdit::SetBitmap()
{
	// Removes the current bitmap which results in the button being removed
	// from the edit control.

	return SetBitmap((LPCTSTR) NULL);
}

//@doc SECDropEdit
//@mfunc Retrieves the bitmap of the drop-down button.
//@rdesc Handle of the drop-down button’s bitmap.
//@xref <c SECDropEdit> <mf SECDropEdit::SetBitmap>
HBITMAP SECDropEdit::GetBitmap() const
{
	return (HBITMAP)(m_bmp.m_hObject);
}

//@doc SECDropEdit
//@mfunc Determines whether the given point (in screen or client coordinates) 
// is a hit on the drop-down button.
//@rdesc Nonzero if the point is a hit on the button.
//@parm CPoint | point | A point in either screen or client coordinates.
//@parmopt  BOOL | bClient | FALSE | True indiates the point is given in 
// client coordinates, otherwise screen coordinate
//@xref <c SECDropEdit>
BOOL SECDropEdit::HitTestBtn(CPoint point, BOOL bClient) const
{
	if (m_btnRect.IsRectEmpty())
		return FALSE;
	
	// Returns TRUE if the given point is within the button. If bClient is
	// TRUE then point is in our client coordinates otherwise it is in
	// screen coordinates.
	ASSERT(m_bmp.m_hObject);

	CWnd* pParentWnd = GetParent();
	if(bClient)
	{
		// Point is in our client coordinates. Convert to parent client coords.
		if(pParentWnd)
		{
			ClientToScreen(&point);
			pParentWnd->ScreenToClient(&point);
		}
	}
	else
	{
		// Point is in screen coordinates. Convert to parent client coords.
		if(pParentWnd)
			pParentWnd->ScreenToClient(&point);
	}

	return m_btnRect.PtInRect(point);
}

//@doc SECDropEdit
//@mfunc Forces a redraw of the drop-down button.
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::InvalidateBtn()
{
	// Invalidate the button.
	ASSERT(m_bmp.m_hObject);

	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->InvalidateRect(m_btnRect, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit overrideables
//
//@doc SECDropEdit
//@mfunc Called when the drop-down button has been clicked.
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::OnClicked()
{
	// The button has been clicked. This implementation fires a
	// BN_CLICKED to our parent to handle the event.
	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
	{
#ifdef WIN32
		WPARAM wParam = MAKELONG(GetDlgCtrlID(), BN_CLICKED);
		LPARAM lParam = (LPARAM) m_hWnd;
#else
		WPARAM wParam = GetDlgCtrlID();
		LPARAM lParam = MAKELONG(m_hWnd, BN_CLICKED);
#endif
		pParentWnd->SendMessage(WM_COMMAND, wParam, lParam);
	}
}

//@doc SECDropEdit
//@mfunc Draws the button.
//@parm CDC& | dc | Device context with which to draw.
//@parm CRect | r | Bounding rect for the button.
void SECDropEdit::DrawBtn(CDC& dc, CRect r)
{
	// Handles drawing the button to the given rect 'r' in the given DC.

	if(m_wState & Pressed)
	{
		// Draw the button borders in the down state
		Draw3dRect1(&dc, r, secData.clrBtnShadow, secData.clrBtnShadow);
		r.InflateRect(-1, -1);
		FillSolidRect1(&dc, r, secData.clrBtnFace);
		r.InflateRect(-1, -1);
	}
	else
	{
		// Draw the normal button borders
		Draw3dRect1(&dc, r, secData.clrBtnFace, secData.clrWindowFrame);
		r.InflateRect(-1, -1);
		Draw3dRect1(&dc, r, secData.clrBtnHilite, secData.clrBtnShadow);
		r.InflateRect(-1, -1);

		FillSolidRect1(&dc, r, secData.clrBtnFace);
	}

	// Now draw the bitmap to the button face
	CDC memDC;
	if(memDC.CreateCompatibleDC(&dc))
	{
		BITMAP bm;
		VERIFY(m_bmp.GetObject(sizeof(bm), &bm));

		// Calculate position of bitmap
		int x = r.left + m_wLeftBorder;
		int y = (r.Height() - bm.bmHeight) / 2 + r.top;

		if(m_wState & Pressed)
			// Button is down, so offset bitmap to give impression of movement
			x++, y++;

		CBitmap* pOldBmp = memDC.SelectObject(&m_bmp);
		dc.BitBlt(x, y, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
	}
}


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit message handlers
//
//@doc SECDropEdit
//@mfunc Called when the size and position of the client area needs 
// to be calculated.
//@parm BOOL | bCalcValidRects | Specifies whether the application should specify 
// which part of the client area contains valid information. 
//@parm NCCALCSIZE_PARAMS FAR* | lpncsp | Points to a NCCALCSIZE_PARAMS data 
// structure that contains information an application can use to calculate 
// the new size and position of the CWnd rectangle (including client area, 
// borders, caption, scroll bars, and so on).
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// We override WM_NCCALCSIZE to increase the non-client area to include
	// space for a button.

	CPARENTEDIT::OnNcCalcSize(bCalcValidRects, lpncsp);

	if(!m_bmp.m_hObject)
		// We have no bitmap - so no button to add.
		return;

	// Calculate width of button, based on width of bitmap, space for borders
	// and any user defined margins.
	m_btnRect.SetRectEmpty();

	BITMAP bm;
	VERIFY(m_bmp.GetObject(sizeof(bm), &bm));
	int nWidth = m_btnRect.right = bm.bmWidth + 4 + m_wLeftBorder + 
								   m_wRightBorder;

	// Now we can setup the rectangle of the button
	if(m_wState & RightAlign)		// Right aligned
	{
		lpncsp->rgrc[0].right -= nWidth;
		m_btnRect.left         = lpncsp->rgrc[0].right;
		m_btnRect.right        = m_btnRect.left + nWidth;
	}
	else							// Left aligned
	{
		m_btnRect.left         = lpncsp->rgrc[0].left;
		m_btnRect.right        = 
		lpncsp->rgrc[0].left   = m_btnRect.left + nWidth;
	}

	m_btnRect.top              = lpncsp->rgrc[0].top;
	m_btnRect.bottom           = lpncsp->rgrc[0].bottom;

	// m_btnRect now contains the coordinates of the button relative to our
	// parents client area.
}

//@doc SECDropEdit
//@mfunc Called for the CWnd object that contains the cursor (or the CWnd object 
// that used the SetCapture member function to capture the mouse input) 
// every time the mouse is moved.
//@parm CPoint | point | Contains the x- and y-coordinates of the cursor. 
//@xref <c SECDropEdit>
UINT SECDropEdit::OnNcHitTest(CPoint point)
{
	// We look for hits on the button, returning HTBORDER (rather than
	// HTNOWHERE which would result in mouse events being swallowed).

	UINT nHit = CPARENTEDIT::OnNcHitTest(point);

	if(nHit == HTNOWHERE && HitTestBtn(point))
		nHit = HTBORDER;

	return nHit;
}

//@doc SECDropEdit
//@mfunc Called when the user presses the left mouse button while the 
// cursor is within a nonclient area of the CWnd object. 
//@parm UINT | nHitTest | Specifies the hit-test code. 
//@parm CPoint | point | Specifies a CPoint object that contains the x and y 
//screen coordinates of the cursor position.
//@xref <c SECDropEdit>
void SECDropEdit::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	ASSERT(!(m_wState & Pressed));

	if(!(m_wState & Pressed) && HitTestBtn(point))
	{
		// The button has been clicked - We now redraw the button as down
		// and capture the mouse so that we can detect the button being
		// released.
		m_wState |= Pressed | Capture;
		InvalidateBtn();
		UpdateWindow();
		SetCapture();

		// Note that when we are capturing, the mouse events we are interested
		// in will appear as normal mouse events rather than their non-client
		// equivalents.

	}
	else
		CPARENTEDIT::OnNcLButtonDown(nHitTest, point);
}

//@doc SECDropEdit
//@mfunc Called after the CWnd object has been moved.
//@rdesc void 
//@parm int | x | Specifies the new x-coordinate location of the 
// upper-left corner of the client area. 
//@parm  int | y | Specifies the new y-coordinate location of the 
// upper-left corner of the client area. 
//@xref <c SECDropEdit>
void SECDropEdit::OnMove(int x, int y) 
{

	CPARENTEDIT::OnMove(x, y);
	
	// The SWP_FRAMECHANGED flag will cause a WM_NCCALCSIZE message 
	// to be sent to the control, which will then recalculate the 
	// location of the button so that it can be drawn in the correct 
	// location relative to the client area.
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
	SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
}

//@doc SECDropEdit
//@mfunc Called when the mouse cursor moves. 
//@rdesc void 
//@parm UINT | nFlags |Indicates whether various virtual keys are down. 
//@parm  CPoint | point | Specifies the x- and y-coordinate of the cursor.
//@xref <c SECDropEdit>
void SECDropEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_wState & Capture)
	{
		// We are currently tracking a button press. Look for changes in state

		BOOL bHit = HitTestBtn(point, TRUE);
		if(bHit && !(m_wState & Pressed))
		{
			// The mouse has moved back into the button (it was previously
			// drawn up), so draw it as down.
			m_wState |= Pressed;
			InvalidateBtn();
			UpdateWindow();
		}
		else if(!bHit && (m_wState & Pressed))
		{
			// The mouse has moved out of the button, so draw it as up.
			m_wState ^= Pressed;
			InvalidateBtn();
			UpdateWindow();
		}
	}
	else
		CPARENTEDIT::OnMouseMove(nFlags, point);
}

//@doc SECDropEdit
//@mfunc Called when the user releases teh left mouse button.
//@rdesc void 
//@parm UINT | nFlags | Indicates whether various virtual keys are down. 
//@parm  CPoint | point | Specifies the x- and y-coordinate of the cursor. 
//@xref <c SECDropEdit>
void SECDropEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_wState & Capture)
	{
		// We are currently tracking the mouse.
		ReleaseCapture();
		m_wState ^= Capture;

		if(m_wState & Pressed)
		{
			// It is currently drawn down, so now draw it up.
			m_wState ^= Pressed;
			InvalidateBtn();
			UpdateWindow();

		}

		if(HitTestBtn(point, TRUE))
			// The hit was over the button.
			OnClicked();
	}
	else
		CPARENTEDIT::OnLButtonUp(nFlags, point);
}

//@doc SECDropEdit
//@mfunc Called to inform CWnd to cancel any internal mode. 
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::OnCancelMode()
{
	if(m_wState & Capture)
	{
		// We have to cancel our internal mode, so stop capturing.
		ReleaseCapture();
		m_wState ^= Capture;

		if(m_wState & Pressed)
		{
			// Need to draw the button as up.
			m_wState ^= Pressed;
			InvalidateBtn();
			UpdateWindow();
		}
	}
}

//@doc SECDropEdit
//@mfunc Called when the nonclient area needs to be painted.
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::OnNcPaint()
{
	// We override WM_NCPAINT to handle drawing a button in non-client space
	// we reserved during WM_NCCALCSIZE.
	CPARENTEDIT::OnNcPaint();

	if(!m_bmp.m_hObject)
		// No button - so nothing to draw
		return;

	// We need to adjust the coordinates of rectange to have the Non-Client
	// area top left pixel as 0, 0.
	CRect r = m_btnRect;
	CRect windowRect;
	GetWindowRect(&windowRect);

	CWnd* pParentWnd = GetParent();
	if(pParentWnd)
		pParentWnd->ScreenToClient(&windowRect);

	r.OffsetRect(-windowRect.left, -windowRect.top);

	CWindowDC dc(this);

	// We attempt to draw to an off-screen DC - helps reduce flicker.
	CDC memDC;
	CBitmap bmp;
	if(memDC.CreateCompatibleDC(&dc) &&
	   bmp.CreateCompatibleBitmap(&dc, m_btnRect.Width(), m_btnRect.Height()))
	{
		CBitmap* pOldBmp = memDC.SelectObject(&bmp);

		// Draw the button face.
		CRect tempRect = m_btnRect;
		tempRect.OffsetRect(-m_btnRect.left, -m_btnRect.top);
		DrawBtn(memDC, tempRect);

		// If the window is disabled, dither the off-screen bitmap to reflect
		// this state.
		if(!IsWindowEnabled())
		{
			SECDrawDisabled(memDC, 2, 2, tempRect.Width() - 4, 
							tempRect.Height() - 4);
		}

		// Now we can copy the off-screen DC to the screen DC.
		dc.BitBlt(r.left, r.top, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
	}
	else
		// Unable to create the off-screen DC - so draw directly to the screen
		DrawBtn(dc, r);
}

//@doc SECDropEdit
//@mfunc Called when an application changes the enabled state of the CWnd object. 
//@parm BOOL | bEnable | Specifies whether the CWnd object has been enabled or disabled.
//@rdesc void
//@xref <c SECDropEdit>
void SECDropEdit::OnEnable(BOOL bEnable)
{
	// We have either been enabled or disabled. We need to force a redraw
	// of the button to reflect the new state.

	if(m_bmp.m_hObject)
	{
		InvalidateBtn();
		CPARENTEDIT::OnEnable(bEnable);
	}
}

//@doc SECDropEdit
//@mfunc Called when a change is made in the system color setting.
//@rdesc void 
//@xref <c SECDropEdit>
void SECDropEdit::OnSysColorChange()
{
	// The system colors have changed. Adjust our colors accordingly.
	secData.UpdateSysColors();

	if(m_bmp.m_hObject)
	{
		ASSERT(m_lpszBmpName);

		m_bmp.DeleteObject();
		VERIFY(SECLoadSysColorBitmap(m_bmp, m_lpszBmpName));

		if(m_hWnd)
	    {
		    // Force the non-client to be recalculated for the new bitmap
		    ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, 
					       SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME);
	    }

	}

	CPARENTEDIT::OnSysColorChange();
}

//@doc SECDropEdit
//@mfunc Called when a nonsystem key is pressed.
//@rdesc void 
//@parm UINT | nChar | Contains the character code value of the key.
//@parm  UINT | nRepCnt | Contains the repeat count.
//@parm  UINT | nFlags | Contains the scan code, key-transition code, 
// previous key state, and context code.
//@xref <c SECDropEdit>
void SECDropEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_F4)
		OnClicked();
	else
		CPARENTEDIT::OnKeyDown(nChar, nRepCnt, nFlags);
}

//@doc SECDropEdit
//@mfunc Called when a system key is pressed.
//@rdesc void 
//@parm UINT | nChar | Contains the character code value of the key.
//@parm  UINT | nRepCnt | Contains the repeat count.
//@parm  UINT | nFlags | Contains the scan code, key-transition code, 
// previous key state, and context code.
//@xref <c SECDropEdit>
void SECDropEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DOWN)
		OnClicked();
	else
		CPARENTEDIT::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


#ifdef _DEBUG
void SECDropEdit::Dump(CDumpContext& dc) const
{
	CPARENTEDIT::Dump(dc);

	dc << "m_wState  = " << m_wState << "\n";
	dc << "m_btnRect = " << m_btnRect << "\n";
	dc << "m_bmp     = " << m_bmp.m_hObject << "\n";
}

void SECDropEdit::AssertValid() const
{
	CPARENTEDIT::AssertValid();
}
#endif



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  SECDrawDisabled
//
//  Helper function for dithering a bitmap to be disabled.
//
#define ROP_PSDPxax	0xB8074AL

void SECDrawDisabled(CDC& dc, int x, int y, int nWidth, int nHeight)
{
	// We take the drawn button and effectively gray it and add a shadow.
	CDC monoDC;
	CBitmap bmp;

	if(!monoDC.CreateCompatibleDC(&dc) ||
	   !bmp.CreateBitmap(nWidth, nHeight, 1, 1, NULL))
		return;

	COLORREF clrFace      = ::GetSysColor(COLOR_BTNFACE);
	COLORREF clrHighlight = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	COLORREF clrShadow    = ::GetSysColor(COLOR_BTNSHADOW);

	// Initialise to 0.
	CBitmap* pOldBmp = monoDC.SelectObject(&bmp);
	monoDC.PatBlt(0, 0, nWidth, nHeight, WHITENESS);
	
	// Create mask based on the button bitmap.
	dc.SetBkColor(clrFace);
	monoDC.BitBlt(0, 0, nWidth, nHeight, &dc, x, y, SRCCOPY);

	dc.SetBkColor(clrHighlight);
	monoDC.BitBlt(0, 0, nWidth, nHeight, &dc, x, y, SRCPAINT);

	dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(0L);

	CBrush btnHighLight, btnShadow;
	btnHighLight.CreateSolidBrush(clrHighlight);
	btnShadow.CreateSolidBrush(clrShadow);

	// Draw high light shadow where we have zeros in our mask.
	CBrush* pBrOld = dc.SelectObject(&btnHighLight);
	dc.BitBlt(x+1, y+1, nWidth-1, nHeight-1, &monoDC, 0, 0, ROP_PSDPxax);

	// Draw the shadow where we have zeros in our mask.
	dc.SelectObject(&btnShadow);
	dc.BitBlt(x, y, nWidth, nHeight, &monoDC, 0, 0, ROP_PSDPxax);

	dc.SelectObject(pBrOld);
	monoDC.SelectObject(pOldBmp);
}

///////////////////////////////////////////////////////////////////////////////

