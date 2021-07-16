// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// ExtStatusControlBar.cpp : implementation file
//

#include "stdafx.h"

#if (!defined __EXT_STATUSCONTROLBAR_H)
	#include <ExtStatusControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	#if (!defined __EXTCUSTOMIZE_H)
		#include <ExtCustomize.h>
	#endif
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar

IMPLEMENT_DYNCREATE( CExtStatusControlBar, CStatusBar )

CExtStatusControlBar::CExtStatusControlBar()
	: m_bOuterRectInFirstBand( false )
{
	m_clrStatusBk = (COLORREF)(-1);
}

CExtStatusControlBar::~CExtStatusControlBar()
{
}

BEGIN_MESSAGE_MAP(CExtStatusControlBar, CStatusBar)
	//{{AFX_MSG_MAP(CExtStatusControlBar)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar message handlers

void CExtStatusControlBar::OnPaint() 
{
	_SyncStatusBarColors();
CPaintDC dc( this );
	//Default();
	CStatusBar::UpdateAllPanes( FALSE, TRUE );
	CStatusBar::DefWindowProc(WM_PAINT,(WPARAM)dc.GetSafeHdc(),0);

CWnd * pWnd = GetTopLevelParent();
	if( pWnd != NULL )
	{
		WINDOWPLACEMENT _wp;
		::memset( (void*)&_wp, 0, sizeof(WINDOWPLACEMENT) );
		_wp.length = sizeof(WINDOWPLACEMENT);
		pWnd->GetWindowPlacement( &_wp );
		if( _wp.showCmd != SW_SHOWMAXIMIZED )
		{
			CRect rcGrip;
			GetClientRect( &rcGrip );
			rcGrip.left = rcGrip.right - ::GetSystemMetrics( SM_CXVSCROLL );
			dc.FillSolidRect(
				&rcGrip,
				g_PaintManager->GetColor(CExtPaintManager::CLR_3DFACE_OUT)
				);
			CFont * pOldFont =
				dc.SelectObject( &g_PaintManager->m_FontMarlett );
			ASSERT( pOldFont != NULL );
			COLORREF clrOld =
				dc.SetTextColor(
					g_PaintManager->GetColor( CExtPaintManager::CLR_TEXT_OUT )
					);
			INT nOldBkMode = dc.SetBkMode(TRANSPARENT);
			rcGrip.OffsetRect( -2, -2 );
			dc.DrawText(
				_T("o"), 1, rcGrip, DT_SINGLELINE|DT_RIGHT|DT_BOTTOM
				);
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrOld );
			dc.SelectObject( pOldFont );
		} // if( _wp.showCmd != SW_SHOWMAXIMIZED )
	} // if( pWnd != NULL )

    dc.SelectStockObject( NULL_BRUSH );
INT nItem = m_bOuterRectInFirstBand ? 0 : 1;
	for( ; nItem < m_nCount; nItem++ )
	{
		CRect rc;
		GetItemRect( nItem, rc );
		CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
			this,
			CExtPaintManager::__CB_INNER_STATUSBAR_ITEM,
			0,
			rc
			);
		g_PaintManager->PaintControlBarBorders( dc, _pcbbd );
	} // for( ; nItem < m_nCount; nItem++ )
}

void CExtStatusControlBar::OnNcPaint() 
{
	_SyncStatusBarColors();
CRect rcClient, rcWnd;
	GetClientRect( rcClient );
	ClientToScreen( rcClient );
	GetWindowRect( rcWnd );
	rcClient.OffsetRect( -rcWnd.TopLeft() );
	rcWnd.OffsetRect( -rcWnd.TopLeft() );
CWindowDC dc( this );
	dc.ExcludeClipRect( &rcClient );
CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
		this,
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		rcWnd
		);
	g_PaintManager->PaintControlBarBorders( dc, _pcbbd );
}

void CExtStatusControlBar::DrawBorders( CDC * pDC, CRect& rc )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );
CExtPaintManager::PAINTCONTROLBARBORDERSDATA _pcbbd(
		this,
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		rc
		);
	g_PaintManager->PaintControlBarBorders( *pDC, _pcbbd );
}

void CExtStatusControlBar::_SyncStatusBarColors()
{
COLORREF clrStatusBkNeeded =
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);
	if( m_clrStatusBk != clrStatusBkNeeded )
	{
		m_clrStatusBk = clrStatusBkNeeded;
		GetStatusBarCtrl().SetBkColor( m_clrStatusBk );
	}
}

void CExtStatusControlBar::OnSysColorChange() 
{
	CStatusBar::OnSysColorChange();
	g_PaintManager.OnSysColorChange( this );
	g_CmdManager.OnSysColorChange( this );
	_SyncStatusBarColors();
}

void CExtStatusControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CStatusBar::OnSettingChange( uFlags, lpszSection );
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( this, uFlags, lpszSection );
	_SyncStatusBarColors();
}

LRESULT CExtStatusControlBar::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CStatusBar::OnDisplayChange( wParam, lParam );
	g_PaintManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	g_CmdManager.OnDisplayChange( this, (INT)wParam, CPoint(lParam) );
	return lResult;
}

LRESULT CExtStatusControlBar::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( this, wParam, lParam );
	return lResult;
}

int CExtStatusControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CStatusBar::OnCreate(lpCreateStruct) == -1 )
		return -1;
	
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow(GetSafeHwnd()) );
HWND hWndParent = ::GetParent( GetSafeHwnd() );
	ASSERT( hWndParent != NULL );
	ASSERT( ::IsWindow(hWndParent) );
	VERIFY(
		::SetWindowPos(
			GetSafeHwnd(),
			HWND_TOP,
			0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE
				|SWP_NOREDRAW|SWP_NOACTIVATE|SWP_NOCOPYBITS
				|SWP_NOSENDCHANGING
			)
		);

	_SyncStatusBarColors();
	
	return 0;
}

void CExtStatusControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite =
		CExtCustomizeSite::GetCustomizeSite( m_hWnd );
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		)
		return;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if( CExtControlBar::FindHelpMode(this) )
		return;

CFrameWnd * pFrame = _GetDockingFrameImpl();
	if( pFrame == NULL )
	{
		CStatusBar::OnContextMenu( pWnd, point );
		return;
	}

	ASSERT_VALID( pFrame );
HWND hWndTrack = GetOwner()->GetSafeHwnd();
	ASSERT( hWndTrack != NULL && ::IsWindow(hWndTrack) );
CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(hWndTrack) );

	if(	! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
			pFrame,
			pPopup,
			CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_STATUSBAR_CTX,
			this,
			WM_CONTEXTMENU,
			NULL
			)
		)
	{
		delete pPopup;
		return;
	}
	if( pPopup->ItemGetCount() == 0 )
	{
		delete pPopup;
		return;
	}
	::SetFocus( hWndTrack );
	if( ! pPopup->TrackPopupMenu(
			TPMX_OWNERDRAW_FIXED,
			point.x,
			point.y
			)
		)
	{
		delete pPopup;
	}
}

CFrameWnd * CExtStatusControlBar::_GetDockingFrameImpl()
{
	ASSERT_VALID( this );

CFrameWnd * pFrame = GetParentFrame();
	if( pFrame == NULL )
		return NULL;

	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		//ASSERT_VALID( pFrame );
		ASSERT( pFrame != NULL );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}

	return pFrame;
}



