/////////////////////////////////////////////////////////////////////////////
// CMyMenuButton
// Yogesh Jagota :- Please note that this class is originally written by 
//					Mr. Kirk Stowell, CodeJockey.com named CJFlatButton as 
//					a part of his great CJLibrary. I only copied the code 
//					snippets and modified them to get what I wanted. If you
//					want to use this code commercially, Please contact Kirk
//					at kstowell@codejock.com.
//
//					Thanks Kirk!..., for your awesome CJLibrary.
//					

#include "stdafx.h"
#include "menubutton.h"
#include "../resource.h"

CMyMenuButton::CMyMenuButton()
{
	m_bLBtnDown = FALSE;
	m_bPainted  = FALSE;
}

CMyMenuButton::~CMyMenuButton()
{
}


BEGIN_MESSAGE_MAP(CMyMenuButton, CButton)
	//{{AFX_MSG_MAP(CMyMenuButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuButton message handlers
void CMyMenuButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1, 10, NULL);
	OnTimer(1);

	CButton::OnMouseMove(nFlags, point);
}

void CMyMenuButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CMyMenuButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	CButton::OnLButtonUp(nFlags, point);

	GetParent()->SendMessage( ID_DS_POPUP_BUTTON, 0, 0 );
}

void CMyMenuButton::OnTimer(UINT nIDEvent) 
{
	CRect rcItem;
	GetWindowRect(rcItem);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(( m_bLBtnDown == TRUE ) || ( !rcItem.PtInRect( ptCursor )))
	{
		KillTimer(1);

		if (m_bPainted == TRUE) {
			InvalidateRect (NULL);
		}

		m_bPainted = FALSE;
		return;
	}

	// On mouse over, show raised button.
	else if (!m_bPainted) {
		CDC* pDC = GetDC();
		GetClientRect(rcItem);
		pDC->Draw3dRect( rcItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
		m_bPainted = TRUE;
		ReleaseDC(pDC);
	}
	
	CButton::OnTimer(nIDEvent);
}

void CMyMenuButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	// define some temporary variables.
	CDC*  pDC	 = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;
	int   nState = lpDrawItemStruct->itemState;

	COLORREF clrHilite = ::GetSysColor(COLOR_3DHIGHLIGHT);
	COLORREF clrShadow = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF clrNormal = ::GetSysColor(COLOR_BTNFACE);

	// Paint the background.
	pDC->FillSolidRect( rcItem, clrNormal );

	// Draw flat button border.
	pDC->Draw3dRect( rcItem, 
		(nState & ODS_SELECTED) ? clrShadow : clrNormal,
		(nState & ODS_SELECTED) ? clrHilite : clrNormal );

	// Set the background mode to transparent.
	pDC->SetBkMode( TRANSPARENT );
	
	// draw the button text, icon or bitmap.
	DrawButtonBitmap(pDC, nState, rcItem);
}

void CMyMenuButton::DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem)
{
	CBitmap bBitmap;
	bBitmap.LoadBitmap( IDB_MENU_BUTTON );

	CPoint pt( 1, 2 );
	// if the button is selected.
	if( nState & ODS_SELECTED ) {
		pt.x++;
		pt.y++;
	}

	// draw the bitmap associated with this button.
	pDC->DrawState( pt, CSize( 9, 6 ), &bBitmap, 
		( nState & ODS_DISABLED )?DSS_DISABLED:DSS_NORMAL);
}

BOOL CMyMenuButton::OnEraseBkgnd(CDC* pDC) 
{
	// KStowell - overridden for flicker-free drawing.
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CMyMenuButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// KStowell - Get the client rect.
	CRect rcClient, rcClip;
	dc.GetClipBox( &rcClip );
	GetClientRect( &rcClient );

	// KStowell - Create a memory device-context. This is done to help reduce
	// screen flicker, since we will paint the entire control to the
	// off screen device context first.
	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	COLORREF clrNormal = ::GetSysColor(COLOR_BTNFACE);

	// KStowell - Repaint the background.
	memDC.FillSolidRect(rcClient, clrNormal);

	// let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// KStowell - Copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
		rcClip.left, rcClip.top, SRCCOPY );

	// KStowell - Cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
