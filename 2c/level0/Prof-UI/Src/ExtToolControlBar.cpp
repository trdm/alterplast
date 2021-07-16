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

#include "stdafx.h"

#if _MFC_VER < 0x700
	#include <../src/AfxImpl.h>
#else
	#include <../src/mfc/AfxImpl.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_LOCALIZATION_H)
	#include <../Src/ExtLocalization.h>
#endif

#include <../profuisdll/resource.h>

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CX_BORDER   1
#define CY_BORDER   1

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar::CExtToolControlBarCmdUI

class CExtToolControlBar::CExtToolControlBarCmdUI : public CCmdUI
{
	void _SetCheckImpl(
		int nCheck,
		bool bUpdaeteInCmdManager
		);

public: // re-implementations only

	virtual void Enable(BOOL bOn);

	virtual void SetRadio(BOOL bOn);

	virtual void SetCheck(int nCheck)
	{
		_SetCheckImpl(nCheck,true);
	};

	virtual void SetText(LPCTSTR lpszText)
	{
		lpszText;
		// ignore it
	};

}; // class CExtToolControlBar::CExtToolControlBarCmdUI

void CExtToolControlBar::CExtToolControlBarCmdUI::_SetCheckImpl(
	int nCheck,
	bool bUpdaeteInCmdManager
	)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

UINT nNewStyle =
		pToolBar->GetButtonStyle(m_nIndex)
		&
		~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if( nCheck == 1 )
		nNewStyle |= TBBS_CHECKED;
	else if( nCheck == 2 )
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT( !(nNewStyle & TBBS_SEPARATOR) );
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);

	if( bUpdaeteInCmdManager )
	{
		CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),
			pToolBar->GetButtonID(m_nIndex)
			);
		if( pCmdItem != NULL )
		{
			pCmdItem->StateSetCheck(
				(nNewStyle &
					(TBBS_CHECKED|TBBS_INDETERMINATE)
					) ? true : false
				);
		}
	}
}

void CExtToolControlBar::CExtToolControlBarCmdUI::Enable(BOOL bOn)
{
	CCmdUI::m_bEnableChanged = TRUE;
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
	if( !bOn )
		nNewStyle |= TBBS_DISABLED;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle);

CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),															
		pToolBar->GetButtonID(m_nIndex)
		);
	if( pCmdItem != NULL )
		pCmdItem->StateEnable( bOn ? true : false );
}

void CExtToolControlBar::CExtToolControlBarCmdUI::SetRadio(BOOL bOn)
{
	_SetCheckImpl( bOn ? 1 : 0, false );
	
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),
		pToolBar->GetButtonID(m_nIndex)
		);
	if( pCmdItem != NULL )
		pCmdItem->StateSetRadio( bOn ? true : false );
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarButton

IMPLEMENT_DYNCREATE(CExtBarButton, CObject)

bool CExtBarButton::IsWrap(
	CExtBarButton::e_wrap_t _evt
	) const
{
	ASSERT( __EVT_MIN_VALUE <= _evt && _evt <= __EVT_MAX_VALUE );
	switch( _evt )
	{
	case __EVT_HORZ:
		return m_bWrapH;
	case __EVT_VERT:
		return m_bWrapV;
	default:
		ASSERT( _evt == __EVT_FLOAT );
		return m_bWrapF;
	} // switch( _evt )
}

void CExtBarButton::SetWrap(
	CExtBarButton::e_wrap_t _evt,
	bool bWrap // = true
	)
{
	ASSERT( __EVT_MIN_VALUE <= _evt && _evt <= __EVT_MAX_VALUE );
	switch( _evt )
	{
	case __EVT_HORZ:
		m_bWrapH = bWrap;
		break;
	case __EVT_VERT:
		m_bWrapV = bWrap;
		break;
	default:
		ASSERT( _evt == __EVT_FLOAT );
		m_bWrapF = bWrap;
		break;
	} // switch( _evt )
}

UINT CExtBarButton::GetCmdID(
	bool bEffective // = true
	) const
{
	return bEffective ? m_nEffectiveCmdID : m_nButtonCmdID;
}

void CExtBarButton::SetCmdID(
	UINT nCmdID,
	bool bEffectiveOnly, // = false
	bool bEffectiveRecalcLayout // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );

bool bRecalcLayout = false;
	if(		bEffectiveRecalcLayout
		&&	m_nEffectiveCmdID != nCmdID
		)
		bRecalcLayout = true; 
	
	m_nEffectiveCmdID = nCmdID;
	
	if( !bEffectiveOnly )
	{
		m_nButtonCmdID = nCmdID;
		if( m_nButtonCmdID == ID_SEPARATOR )
		{
			m_nStyle |= TBBS_SEPARATOR;
			m_nStyle &= ~(TBBS_BUTTON);
		}
		else
		{
			m_nStyle |= TBBS_BUTTON;
			m_nStyle &= ~(TBBS_SEPARATOR);
		}
	} // if( !bEffectiveOnly )
	
	if( bRecalcLayout )
		m_pBar->_RecalcLayoutImpl();
}

bool CExtBarButton::IsAbleToTrackMenu(
	bool bCustomizeMode // = false
	)
{
	bCustomizeMode;
	if(		GetMenu() != NULL
		||	IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		||	(	m_pCmdNodeC != NULL
			&&	(	(m_pCmdNodeC->GetFlags() & __ECTN_AUTO_POPUP) != 0
				||	m_pCmdNodeC->GetNodeCount() > 0
				)
			)
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		)
		return true;
	return false;
}

bool CExtBarButton::IsContainerOfPopupLikeMenu()
{
	ASSERT(
			( m_hMenu == NULL )
		||	( m_hMenu != NULL && ::IsMenu(m_hMenu) )
		);
	return m_bPopupMenu;
}

CString CExtBarButton::GetText() const
{
CString sText( _T("") );
	if( IsSeparator() )
		return sText;
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
			GetCmdID( true )
			);
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * pNode =
		((CExtBarButton*)this)->GetCmdNode();
	if( pNode != NULL )
	{
		ASSERT_VALID( pNode );
		return CString( pNode->GetTextInToolbar( pCmdItem ) );
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	ASSERT( pCmdItem != NULL );
	if( !pCmdItem->m_sToolbarText.IsEmpty() )
		sText = pCmdItem->m_sToolbarText;
int nTabChrPos = sText.Find( _T('\t') );
	if( nTabChrPos < 0 )
		return sText;
	return sText.Left( nTabChrPos );
}

BOOL CExtBarButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );
	if( m_pCtrl != NULL )
	{
		if( m_pCtrl->SendMessage(
				CExtToolControlBar::g_nMsgPutToPopupMenu,
				reinterpret_cast <WPARAM> ( pPopup ),
				0
				)
			)
			return TRUE;
	}
	if( IsAbleToTrackMenu() )
	{
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID( false )
				);
		ASSERT( pCmdItem != NULL );
		CString sText = pCmdItem->m_sMenuText;
		if( sText.IsEmpty() )
			sText = pCmdItem->m_sToolbarText;
		HICON hIcon =
			g_CmdManager->CmdGetHICON(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID( false )
				);
		VERIFY(
			pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_POPUP,
				-1,
				(LPCTSTR)sText,
				hIcon
				)
			);
		CExtPopupMenuWnd * pChildPopup =
			pPopup->ItemGetPopup(
				pPopup->ItemGetCount() - 1
				);
		ASSERT( pChildPopup != NULL );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		CExtCustomizeCmdTreeNode * pNode = GetCmdNode();
		if( pNode != NULL )
		{
			ASSERT_VALID( pNode );
			VERIFY(
				pChildPopup->UpdateFromCmdTree(
					GetCmdTargetWnd()->GetSafeHwnd(),
					pNode,
					false
					)
				);
		} // if( pNode != NULL )
		else
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		{
			HMENU hMenu = GetMenu();
			ASSERT( hMenu != NULL );
			ASSERT( ::IsMenu(hMenu) );
			CMenu _menu;
			_menu.Attach( hMenu );
			pChildPopup->UpdateFromMenu(
				GetCmdTargetWnd()->GetSafeHwnd(),
				&_menu,
				IsContainerOfPopupLikeMenu(),
				false
				);
			_menu.Detach();
		} // else from if( pNode != NULL )
	} // if( IsAbleToTrackMenu() )
	else
	{
		if( !pPopup->ItemInsert( GetCmdID(true) ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
	} // else from if( IsAbleToTrackMenu() )
	return TRUE;
}

CRect CExtBarButton::RectDropDown() const
{
CRect rcBtnDropDownArea = Rect();

BOOL bHorz = TRUE;
	ASSERT_VALID( m_pBar );

	if( (!m_bNoRotateVerticalLayout) && m_pBar->IsDockedVertically() )
		bHorz = FALSE;

	if( bHorz )
	{
		rcBtnDropDownArea.left =
			rcBtnDropDownArea.right
			- __DROPDOWN_ARROW_GAP*2
			- CExtPaintManager::g_glyph_btn_expand_bottom.Size().cx
			;
//		if( m_bSeparatedDropDown )
//			rcBtnDropDownArea.left -= __DROPDOWN_ARROW_GAP;
	} // if( bHorz )
	else
	{
		rcBtnDropDownArea.top =
			rcBtnDropDownArea.bottom
			- __DROPDOWN_ARROW_GAP*2
			- CExtPaintManager::g_glyph_btn_expand_right.Size().cy
			;
//		if( m_bSeparatedDropDown )
//			rcBtnDropDownArea.top -= __DROPDOWN_ARROW_GAP;
	} // else from if( bHorz )

	return rcBtnDropDownArea;
}

CRect CExtBarButton::Rect() const
{
CRect rcBtn = m_ActiveRect;
	if(		m_pBar != NULL
		&&	g_PaintManager->GetCb2DbTransparentMode( (CObject *)(this) )
		&&	g_PaintManager->IsMenuMustCombineExcludeArea()
		&&	m_pBar->m_pDockSite != NULL
		&&	(! m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
		)
	{
		CSize sizeToolBarRgnRounders =
			g_PaintManager->GetToolBarRgnRoundersSize();
		CRect rcClient;
		m_pBar->GetClientRect( &rcClient );
		CRect rcBtn2(rcBtn);
		if( m_pBar->IsDockedVertically() )
		{
			rcBtn2.OffsetRect(
				rcBtn.left - rcClient.left,
				rcClient.bottom - rcBtn.bottom
				);
			rcBtn2.top = rcBtn.top - sizeToolBarRgnRounders.cy*2-1;
			rcBtn2.right = rcClient.right;
		} // if( m_pBar->IsDockedVertically() )
		else if( m_pBar->IsDockedHorizontally() )
		{
			rcBtn2.OffsetRect(
				rcClient.right - rcBtn.right,
				rcBtn.top - rcClient.top
				);
			rcBtn2.left = rcBtn.left - sizeToolBarRgnRounders.cx*2-1;
			rcBtn2.bottom = rcClient.bottom;
		} // else if( m_pBar->IsDockedHorizontally() )
	}
	return rcBtn;
}

CSize CExtBarButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( m_pBar );
	ASSERT_VALID( (&dc) );

	if( m_bNoRotateVerticalLayout )
		bHorz = TRUE;

	if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
	{
		ASSERT_VALID( m_pCtrl );
		CRect rcCtrl;
		m_pCtrl->GetWindowRect( &rcCtrl );
		m_ActiveSize.cx = rcCtrl.Width();
		m_ActiveSize.cy = sizePreCalc.cy;
		return m_ActiveSize;
	} // if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
	m_ActiveSize = sizePreCalc;

	if( IsSeparator() )
	{
		if( bHorz )
			m_ActiveSize.cx = __EXT_TB_BUTTON_SEPARATOR_WIDTH;
		else
			m_ActiveSize.cy = __EXT_TB_BUTTON_SEPARATOR_HEIGHT;
	} // if( IsSeparator() )
	else
	{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if( GetCmdNode() == NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
		{
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
					GetCmdID( true )
					);
			if( pCmdItem == NULL )
				return m_ActiveSize; // shutdown mode
		} // if( GetCmdNode() == NULL )

		CExtCmdIcon * pIcon = GetIconPtr();
		CSize _size( 0, 0 );
		if( pIcon != NULL && (! pIcon->IsEmpty() ) )
		{
			_size = pIcon->GetSize();
			_size.cx += __EXT_TB_BUTTON_INNER_MARGIN*2;
			_size.cy += __EXT_TB_BUTTON_INNER_MARGIN*2;
		}
		CString sMeasureText = GetText();
		if( !sMeasureText.IsEmpty() )
		{
			static TCHAR stat_strDummyAmpSeq[] = _T("\001\001");
			sMeasureText.Replace( _T("&&"), stat_strDummyAmpSeq );
			sMeasureText.Remove( _T('&') );
			sMeasureText.Replace( stat_strDummyAmpSeq, _T("&") );
			CRect rcTextMeasure( 0, 0, 0, 0 );
			dc.DrawText(
				sMeasureText,
				&rcTextMeasure,
				DT_SINGLELINE|DT_LEFT|DT_CALCRECT
				);
			if( bHorz )
			{
				_size.cx +=
					rcTextMeasure.Width()
					+ __EXT_TB_BUTTON_TEXT_MARGIN*2;
				_size.cy =
					max(
						_size.cy,
						rcTextMeasure.Height()
						);
			} // if( bHorz )
			else
			{
				_size.cx =
					max(
						_size.cx,
						rcTextMeasure.Height()
						);
				_size.cy +=
					rcTextMeasure.Width()
					//+ __EXT_TB_BUTTON_TEXT_MARGIN*2
					+ __EXT_TB_BUTTON_TEXT_MARGIN
					;
			} // else from if( bHorz )
			_size.cx += __EXT_TB_BUTTON_INNER_MARGIN*2;
			_size.cy += __EXT_TB_BUTTON_INNER_MARGIN*2;
		} // if( !sMeasureText.IsEmpty() )
		_size.cx += __EXT_TB_BUTTON_OUTER_MARGIN*2;
		_size.cy += __EXT_TB_BUTTON_OUTER_MARGIN*2;
		if( m_ActiveSize.cx < _size.cx )
			m_ActiveSize.cx = _size.cx;
		if( m_ActiveSize.cy < _size.cy )
			m_ActiveSize.cy = _size.cy;
	} // else from if( IsSeparator() )
	if(		IsAbleToTrackMenu()
		&&	(!m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
		)
	{
		if( bHorz )
			m_ActiveSize.cx +=
				__DROPDOWN_ARROW_GAP*2
				+ CExtPaintManager::g_glyph_btn_expand_bottom.Size().cx
				;
		else
			m_ActiveSize.cy +=
				__DROPDOWN_ARROW_GAP*2
				+ CExtPaintManager::g_glyph_btn_expand_right.Size().cy
				;
	}
	return m_ActiveSize;
}

CExtCmdIcon * CExtBarButton::GetIconPtr()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	if( m_pBar->GetSafeHwnd() == NULL )
		return NULL;
LPCTSTR strProfile = g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() );
	if( strProfile == NULL )
		return NULL;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtCustomizeCmdTreeNode * pOwnNode = GetCmdNode();
	if( pOwnNode != NULL )
	{
		ASSERT_VALID( pOwnNode );
		if( !CExtCmdManager::IsCommand( GetCmdID( true ) ) )
			return NULL;
		CExtCmdIcon * pIcon =
			pOwnNode->GetIconPtrInToolbar(
				g_CmdManager->CmdGetPtr(
					strProfile,
					GetCmdID( true )
					)
				);
		if( pIcon == NULL )
			return NULL;
		if( (pOwnNode->GetFlags()&__ECTN_TBB_AUTOCHANGE_ID) == 0 )
			return pIcon;
	} // if( pOwnNode != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	return
		g_CmdManager->CmdGetIconPtr(
			strProfile,
			GetCmdID( true )
			);
}

void CExtBarButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT_VALID( (&dc) );

	if( m_bNoRotateVerticalLayout )
		bHorz = TRUE;

	if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
		return;

	ASSERT( !IsSeparator() );
CRect rcArea( m_ActiveRect );
	rcArea.DeflateRect(
		__EXT_TB_BUTTON_INNER_MARGIN,
		__EXT_TB_BUTTON_INNER_MARGIN
		);
	if( rcArea.right <= rcArea.left
		||
		rcArea.bottom <= rcArea.top
		)
		return;
	if( GetStyle() & TBBS_HIDDEN )
		return;
	if( (!IsVisible())
		||
		(!dc.RectVisible(&m_ActiveRect))
		)
		return;
	ASSERT(
			m_ActiveSize.cx >= __EXT_TB_BUTTON_INNER_MARGIN*2
		&&	m_ActiveSize.cy >= __EXT_TB_BUTTON_INNER_MARGIN*2
		);
bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
bool bPushed =
		( IsPressed() && (!bDockSiteCustomizeMode) )
			? true : false;
bool bEnabled =
		( IsDisabled() && (!bDockSiteCustomizeMode) )
			? false : true;
bool bHover =
		( IsHover() && (!bDockSiteCustomizeMode) )
			? true : false;
	if( !bDockSiteCustomizeMode )
	{
		if(		CExtToolControlBar::g_bMenuTracking
			||	CExtPopupMenuWnd::IsMenuTracking()
			)
			bHover = false;
		else if( !bHover )
			bHover = IsPressedTracking();
		//if( bPushed && bHover && IsPressedTracking() )
			//bHover = false;
	} // if( !bDockSiteCustomizeMode )

bool bIndeterminate =
		( IsIndeterminate() && (!bDockSiteCustomizeMode) )
			? true : false;
CString sText = GetText();

CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
		this,
		bHorz,
		rcArea,
		(LPCTSTR)sText,
		GetIconPtr(),
		true,
		bHover,
		bPushed,
		bIndeterminate,
		bEnabled,
		true,false,false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		NULL,
		(	IsAbleToTrackMenu()
			&& (!m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
			) ? true : false,
		0,
		(!bEnabled) || ( bEnabled && (!bHover) && (!bPushed) )
		);

	if( m_bSeparatedDropDown )
	{
		_ppbd.m_bSeparatedDropDown = true;
		if(		(m_bDropDownHT || CExtToolControlBar::g_bMenuTracking)
			&&	bPushed
			&&	(!bDockSiteCustomizeMode)
			)
		{
			//_ppbd.m_bPushed = false;
			_ppbd.m_bPushedDropDown = true;
		}
	}

	g_PaintManager->PaintPushButton( dc, _ppbd );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = m_pBar->GetCustomizeSite();
	if(		pSite != NULL
		&&	pSite->IsCustomizeMode()
		&&	pSite->CustomizedNodeGet() != NULL
		&&	pSite->CustomizedNodeGet() == GetCmdNode( false )
		)
	{
		g_PaintManager->PaintDragSourceRect(
			dc,
			rcArea
			);
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

}

void CExtBarButton::OnUpdateCmdUI(
	CWnd * pTarget,
	BOOL bDisableIfNoHndler,
	int nIndex
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT_VALID( pTarget );
	if(		IsSeparator() // ignore separators
		||	(	IsAbleToTrackMenu() // ignore menu drop buttons
			&&	(m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
			)
		)
		return;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = m_pBar->GetCustomizeSite();
	if( pSite != NULL )
	{
		CExtCustomizeCmdTreeNode * pNode = GetCmdNode( false );
		if(		pNode != NULL
			&&	(pNode->GetFlags() & __ECTN_AUTO_POPUP) != 0
			)
			return;
		if( pSite->IsUserBarCommand( GetCmdID( false ) ) )
			return;
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	// send the update notification
CExtToolControlBar::CExtToolControlBarCmdUI state;
	state.m_pOther = m_pBar;
	state.m_nIndex = nIndex;
	state.m_nIndexMax = m_pBar->GetButtonsCount();
	state.m_nID = GetCmdID( false );
	state.DoUpdate( pTarget, bDisableIfNoHndler );
}

bool CExtBarButton::CanStartCommandDrag()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	if( IsSeparator() )
		return false;
	if( m_pCtrl->GetSafeHwnd() != NULL )
		return false;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( GetCmdNode() == NULL )
		return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	return true;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

int CExtBarButton::CalculateDropOverEffect(
	CPoint point,
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	bool bAllowTrackNodePopup // = true
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( !_dragInfo.IsEmpty() );
	_dragInfo;
	if( IsSeparator() )
		return 0;
	if( !CanStartCommandDrag() )
		return 0;
	if( GetCmdNode() == NULL )
		return false;
//CPoint ptMid = Rect().CenterPoint();
CPoint ptMidL = Rect().TopLeft();
	ptMidL.x += Rect().Size().cx / 4;
	ptMidL.y += Rect().Size().cy / 4;
CPoint ptMidR( ptMidL );
	ptMidR.x += Rect().Size().cx / 2;
	ptMidR.y += Rect().Size().cy / 2;
int nRetVal = 0;
	if( (m_pBar->m_dwStyle & CBRS_ORIENT_HORZ) != 0 )
	{
		if( point.x <= ptMidL.x  )
			nRetVal = -1;
		else if( point.x >= ptMidR.x  )
			nRetVal = 1;
		else
			nRetVal = -2;
	} // if( (m_pBar->m_dwStyle & CBRS_ORIENT_HORZ) != 0 )
	else
	{
		if( point.y <= ptMidL.y  )
			nRetVal = -1;
		else if( point.y >= ptMidR.y  )
			nRetVal = 1;
		else
			nRetVal = -2;
	} // else from if( (m_pBar->m_dwStyle & CBRS_ORIENT_HORZ) != 0 )

	if( bAllowTrackNodePopup )
	{
		CExtCustomizeSite * pSite = m_pBar->GetCustomizeSite();
		ASSERT( pSite != NULL );
		if( IsAbleToTrackMenu( true ) )
		{
			CExtCustomizeCmdTreeNode * pOwnNode = GetCmdNode();
			ASSERT_VALID( pOwnNode );
			CRect rcBtn = Rect();
			m_pBar->ClientToScreen( &rcBtn );
			m_pBar->ClientToScreen( &point );
			DWORD dwTrackFlags = OnGetTrackPopupFlags();
			pSite->DropTargetPopupTrack(
				pOwnNode,
				rcBtn,
				point,
				dwTrackFlags
				);
		} // if( IsAbleToTrackMenu() )
		else
			pSite->DropTargetPopupCancel();
	} // if( bAllowTrackNodePopup )
	else
	{
		CExtCustomizeSite * pSite = m_pBar->GetCustomizeSite();
		if( pSite != NULL )
			pSite->DropTargetPopupCancel();
	} // else from if( bAllowTrackNodePopup )

	return nRetVal;
}

CExtCustomizeCmdTreeNode * CExtBarButton::GetCmdNode(
	bool bInitial // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	if( IsSeparator() )
	{
		ASSERT( m_pCmdNodeI == NULL );
		ASSERT( m_pCmdNodeC == NULL );
		return NULL;
	}
CExtCustomizeCmdTreeNode * pNode =
		bInitial ? m_pCmdNodeI : m_pCmdNodeC;
#ifdef _DEBUG
	if( pNode != NULL )
	{
		ASSERT_VALID( pNode );
	}
#endif // _DEBUG
	return pNode;
}

void CExtBarButton::OnCustomizeUpdateProps(
	CExtCustomizeCmdTreeNode * pNode // = NULL // update from basic node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	if( pNode == NULL )
		pNode = m_pCmdNodeI;
	ASSERT_VALID( pNode );
#ifdef _DEBUG
	if( m_pCmdNodeI != NULL )
	{
		ASSERT_VALID( m_pCmdNodeI );
	}
#endif // _DEBUG
	ASSERT( !IsSeparator() );
	SetCmdID( pNode->GetCmdID(false), false ); // reset all IDs
	SetCmdID( pNode->GetCmdID(true), true ); // reset effective ID
DWORD dwNodeFlags = pNode->GetFlags();
	if( dwNodeFlags&__ECTN_TBB_APPEND_MDI_MENU )
		SetAppendMdiWindowsMenu( true );
	if( dwNodeFlags&__ECTN_TBB_SEPARATED_DROPDOWN )
		SetSeparatedDropDown( true );
	if( dwNodeFlags&__ECTN_TBB_AUTOCHANGE_ID )
		SetAutoChangeID( true );
	if( dwNodeFlags&__ECTN_TBB_NO_ROTATE_VL )
		SetNoRotateVerticalLayout( true );
}

void CExtBarButton::SetBasicCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT_VALID( pNode );
	ASSERT( !IsSeparator() );
	m_pCmdNodeI = pNode;
	ASSERT( m_pCmdNodeI->GetCmdID(false) == GetCmdID(false) );
}

void CExtBarButton::SetCustomizedCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT_VALID( pNode );
#ifdef _DEBUG
	if( m_pCmdNodeI != NULL )
	{
		ASSERT_VALID( m_pCmdNodeI );
	}
#endif // _DEBUG
	ASSERT( !IsSeparator() );
	m_pCmdNodeC = pNode;
	ASSERT( m_pCmdNodeC->GetCmdID(false) == GetCmdID(false) );
}

CExtCustomizeCmdTreeNode * CExtBarButton::OnCustomizeNodeInit(
	CExtCustomizeSite * pSite,
	CExtCustomizeCmdTreeNode * pParentNode // toolbar node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( pSite != NULL );
	ASSERT_VALID( pParentNode );
	if( IsSeparator() )
	{
		ASSERT( m_pCmdNodeI == NULL );
		ASSERT( m_pCmdNodeC == NULL );
		return NULL;
	}
	m_pCmdNodeI = m_pCmdNodeC = NULL;
	m_pCmdNodeI =
		new CExtCustomizeCmdTreeNode(
			GetCmdID( false ),
			GetCmdID( true )
			);
	pParentNode->InsertNode( pSite, m_pCmdNodeI );
INT nOwnIdx = m_pBar->_GetIndexOf( this );
	ASSERT( 0 <= nOwnIdx && nOwnIdx < m_pBar->GetButtonsCount() );
	if( nOwnIdx > 0 )
	{
		CExtBarButton * pPrevTBB =
			m_pBar->GetButton( nOwnIdx - 1 );
		if( pPrevTBB->IsSeparator() )
			m_pCmdNodeI->ModifyFlags( __ECTN_GROUP_START );
	} // __ECTN_GROUP_START
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( m_pBar->m_hWnd ),
			GetCmdID()
			);
	ASSERT( pCmdItem != NULL );
	m_pCmdNodeI->SetTextInToolbar( pCmdItem->m_sToolbarText );
	m_pCmdNodeI->SetTextInMenu( pCmdItem->m_sMenuText );
DWORD dwAddFlags = 0;
	if( IsAppendMdiWindowsMenu() )
		dwAddFlags |= __ECTN_TBB_APPEND_MDI_MENU;
	if( GetSeparatedDropDown() )
		dwAddFlags |= __ECTN_TBB_SEPARATED_DROPDOWN;
	if( GetAutoChangeID() )
		dwAddFlags |= __ECTN_TBB_AUTOCHANGE_ID;
	if( GetNoRotateVerticalLayout() )
		dwAddFlags |= __ECTN_TBB_NO_ROTATE_VL;
	if( dwAddFlags != 0 )
		m_pCmdNodeI->ModifyFlags( dwAddFlags );
	if( GetMenu() != NULL )
	{
		CMenu * pMenu = CMenu::FromHandle( GetMenu() );
		ASSERT( pMenu != NULL );
		VERIFY(
			m_pCmdNodeI->LoadMenuTree(
				m_pBar,
				m_pBar->GetCustomizeSite(),
				pMenu,
				IsContainerOfPopupLikeMenu()
				)
			);
		SetMenu( NULL, true, true ); // no longer needed
	} // if( GetMenu() != NULL )
	return m_pCmdNodeI;
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

void CExtBarButton::RedrawButton(
	bool bUpdateNow // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	if(		IsVisible()
		&& ( (m_nStyle & TBBS_HIDDEN) == 0 )
		)
	m_pBar->_InvalidateButton(
		m_pBar->_GetIndexOf(this)
		);
	if( bUpdateNow )
		m_pBar->UpdateWindow();
}

void CExtBarButton::SetMenu(
	HMENU hMenu,
	bool bPopupMenu,
	bool bAutoDestroyMenu
	)
{
	_DestroyMenu();
	m_hMenu = hMenu;
	if( m_hMenu != NULL )
	{
		ASSERT( ::IsMenu(hMenu) );
		VERIFY(
			g_CmdManager->UpdateFromMenu(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				hMenu
				)
			);
		m_bPopupMenu = bPopupMenu ? true : false;
		ModifyStyle(0,TBBS_DISABLED);
		m_bAutoDestroyMenu = bAutoDestroyMenu;
	}
}

UINT CExtBarButton::OnGetTrackPopupFlags()
{
	ASSERT_VALID( m_pBar );
	if( m_pBar->m_bPresubclassDialogMode )
	{
		if( m_pBar->IsDockedAtRight() )
			return TPMX_RIGHTALIGN;
		if( m_pBar->IsDockedAtLeft() )
			return TPMX_LEFTALIGN;
		if( m_pBar->IsDockedAtBottom() )
			return TPMX_BOTTOMALIGN;
		return TPMX_TOPALIGN;
	}

	switch( m_pBar->GetSafeDockBarDlgCtrlID() )
	{
	case AFX_IDW_DOCKBAR_TOP:
		return TPMX_TOPALIGN;
	case AFX_IDW_DOCKBAR_BOTTOM:
		return TPMX_BOTTOMALIGN;
	case AFX_IDW_DOCKBAR_LEFT:
		return TPMX_LEFTALIGN;
	case AFX_IDW_DOCKBAR_RIGHT:
		return TPMX_RIGHTALIGN;
	default: // floating
		return TPMX_TOPALIGN;
	} // switch( m_pBar->GetSafeDockBarDlgCtrlID() )

}

CWnd * CExtBarButton::GetCmdTargetWnd()
{
	ASSERT_VALID( m_pBar );
//CWnd * pWnd = m_pBar->_GetDockingFrameImpl();
//	if( pWnd == NULL )
//		pWnd = m_pBar->GetOwner();
CWnd * pWnd = m_pBar->GetOwner();
	ASSERT_VALID( pWnd );
	return pWnd;
}

UINT CExtBarButton::OnTrackPopup(
	CPoint point,
	bool bSelectAny
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return UINT(-1L);

	if( !IsEnabled() )
		return UINT(-1L);

CWnd * pWnd = CtrlGet();
	if( pWnd != NULL )
	{
		LRESULT lResult =
			pWnd->SendMessage(
				CExtToolControlBar::g_nMsgContinueTrackMenu,
				WPARAM( this ),
				bSelectAny ? 1L : 0L
				);
		lResult;
		return UINT(-1L);
	} // if( pWnd != NULL )

	if( !IsAbleToTrackMenu() )
		return UINT(-1L);
	
bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;
	if( CExtToolControlBar::g_bMenuTracking
		//&& CExtPopupMenuWnd::IsMenuTracking()
		&& m_pBar->_GetIndexOf(this) ==
			m_pBar->m_nBtnIdxMenuTracking
		)
		return UINT(-1L);

	CExtToolControlBar::_CloseTrackingMenus();

	if( m_pBar->IsFloating() )
	{
		m_pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			m_pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}

CWnd * pWndCmdTarget = GetCmdTargetWnd();
	ASSERT_VALID( pWndCmdTarget );

CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * pNode = GetCmdNode();
	if( pNode != NULL )
	{
		ASSERT_VALID( pNode );
		VERIFY(
			pPopup->UpdateFromCmdTree(
				pWndCmdTarget->GetSafeHwnd(),
				pNode
				)
			);
	} // if( pNode != NULL )
	else
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	{
		HMENU hMenu = GetMenu();
		ASSERT( hMenu != NULL );
		ASSERT( ::IsMenu(hMenu) );
		CMenu _menu;
		_menu.Attach( hMenu );
		VERIFY(
			pPopup->UpdateFromMenu(
				pWndCmdTarget->GetSafeHwnd(),
				&_menu,
				IsContainerOfPopupLikeMenu()
				)
			);
		_menu.Detach();
	} // else from if( pNode != NULL )

	if(		IsAppendMdiWindowsMenu()
		&&	(! m_pBar->_IsDockSiteCustomizeMode() )
		)
		pPopup->UpdateMdiWindowsMenu( pWndCmdTarget );

	CExtToolControlBar::g_bMenuTracking = bPrevTBMT;

CRect rcBtn = Rect();
	m_pBar->ClientToScreen( &rcBtn );
	m_pBar->ClientToScreen( &point );

DWORD dwTrackFlags =
		OnGetTrackPopupFlags()
		| TPMX_COMBINE_DEFAULT
		| TPMX_OWNERDRAW_FIXED
		;
	if( bSelectAny )
		dwTrackFlags |= TPMX_SELECT_ANY;
	if( CExtToolControlBar::g_bMenuTrackingExpanded )
		dwTrackFlags |= TPMX_NO_HIDE_RARELY;

	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);

	if(		GetAutoChangeID()
		&&	(! CExtControlBar::FindHelpMode(m_pBar) )
		)
	{
		HWND hWndBar = m_pBar->GetSafeHwnd();
		CExtToolControlBar::g_bMenuTracking = true;
		UINT nResultCmdID = 0;
		if( !pPopup->TrackPopupMenu(
				dwTrackFlags|TPMX_NO_WM_COMMAND|TPMX_DO_MESSAGE_LOOP,
				point.x,point.y,
				&rcBtn,
				m_pBar,
				CExtToolControlBar::_CbPaintCombinedContent,
				&nResultCmdID,
				true
				)
			)
		{
			delete pPopup;
			CExtToolControlBar::_CloseTrackingMenus();
			return UINT(-1L);
		}
		if( nResultCmdID == 0 )
				return UINT(-1L);
		if( hWndBar != NULL && ::IsWindow(hWndBar) )
		{
			SetCmdID( nResultCmdID, true, true );
			OnDeliverCmd();
		}
	} // if( GetAutoChangeID() )
	else
	{
		if( !pPopup->TrackPopupMenu(
				dwTrackFlags,
				point.x,point.y,
				&rcBtn,
				m_pBar,
				CExtToolControlBar::_CbPaintCombinedContent,
				NULL,
				true
				)
			)
		{
			delete pPopup;
			CExtToolControlBar::_CloseTrackingMenus();
			return UINT(-1L);
		}
		CExtToolControlBar::g_bMenuTracking = true;
		m_pBar->_SwitchMenuTrackingIndex(
			m_pBar->_GetIndexOf( this )
			);
	} // else from if( GetAutoChangeID() )
	return UINT(-1L);
}

void CExtBarButton::OnHover(
	CPoint point,
	bool bOn
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return;

	if( bOn )
	{

///		if( CExtToolControlBar::g_bMenuTracking
///			&& CExtPopupMenuWnd::IsMenuTracking()
///			&& m_pBar->m_nBtnIdxHover != m_pBar->m_nBtnIdxMenuTracking
///			)
///			CExtPopupMenuWnd::CancelMenuTracking();
		if( CExtToolControlBar::g_bMenuTracking )
			OnTrackPopup( point, false );
		else
		{
			CExtControlBar::stat_SetMessageString(
				GetCmdTargetWnd(),
				(UINT)(
					(		( ! m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
						&&	( ! IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
						&&	CExtCmdManager::IsCommand( GetCmdID(false) )
					)
						? GetCmdID(true)
						: AFX_IDS_IDLEMESSAGE
					)
				);
		}
		CWnd * pCtrl = CtrlGet();
		if( pCtrl == NULL
			|| (pCtrl->GetStyle() & WS_VISIBLE) == 0
			)
			((CExtMouseCaptureSink *)m_pBar)->SetCapture( m_pBar->GetSafeHwnd() );
	} // if( bOn )
	else
	{
		CExtControlBar::stat_SetMessageString( GetCmdTargetWnd() );
		CExtMouseCaptureSink::ReleaseCapture();
	} // else from if( bOn )
}

void CExtBarButton::OnClick(
	CPoint point,
	bool bDown
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return;

bool bDropDownHT = false;
	if( m_bSeparatedDropDown )
	{
		CRect rcBtnDropDownArea = RectDropDown();
		if( rcBtnDropDownArea.PtInRect(point) )
			bDropDownHT = true;
	}

	if( bDown )
	{
		CExtToolControlBar::_CloseTrackingMenus();

		bool bAllowTrackMenu = true;
		m_bDropDownHT = false;
		if( m_bSeparatedDropDown )
		{
			m_bDropDownHT = bDropDownHT;
			if(!bDropDownHT )
				bAllowTrackMenu = false;
		}


		if( bAllowTrackMenu && IsAbleToTrackMenu() )
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			OnTrackPopup( point, false );
			return;
		}

		CExtControlBar::stat_SetMessageString(
			GetCmdTargetWnd(),
			(WPARAM)GetCmdID(false)
			);

		return;
	} // if( bDown )

	CExtControlBar::stat_SetMessageString( GetCmdTargetWnd() );

	if( !m_bDropDownHT )
		bDropDownHT = false;
	m_bDropDownHT = false;

	if(		(IsAbleToTrackMenu() && (!GetSeparatedDropDown()))
		||	bDropDownHT
		)
		return;

CWnd * pCtrl = CtrlGet();
	if(	pCtrl != NULL
		&& (pCtrl->GetStyle() & WS_VISIBLE)
		)
	{
		ASSERT_VALID( pCtrl );
		CExtToolControlBar * pBar = GetSafeBar();
		ASSERT_VALID( pBar );
		if( CWnd::GetCapture() == pBar )
			pBar->SendMessage( WM_CANCELMODE );
		//pCtrl->SetFocus();
		return;
	}

	OnDeliverCmd();
}

void CExtBarButton::OnDeliverCmd()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );

	if( !CExtCmdManager::IsCommand( GetCmdID(false) ) )
		return;
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
			GetCmdID(true)
			);
	ASSERT( pCmdItem != NULL );
	if( pCmdItem == NULL )
		return;
	VERIFY(
		pCmdItem->Deliver(
			m_pBar,
			true
			)
		);
}

int CExtBarButton::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	point; // should be inside this button

	if(	IsSeparator() )
		return -1;

int nCmdID = (int)GetCmdID(true);

CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetSafeBar()->GetSafeHwnd() ),
			nCmdID
			);
	if( pCmdItem == NULL
		|| pCmdItem->m_sTipTool.IsEmpty()
		)
		return -1;

	if( pTI != NULL )
	{
		CRect rcArea = Rect();
		::CopyRect(
			&(pTI->rect),
			&rcArea
			);

		pTI->uId = (UINT)nCmdID;
		pTI->hwnd = GetSafeBar()->GetSafeHwnd();
		pTI->lpszText = (LPTSTR)
			::calloc(
				(pCmdItem->m_sTipTool.GetLength() + 1),
				sizeof(TCHAR)
				);
		if( pTI->lpszText != NULL )
			_tcscpy(
				pTI->lpszText,
				(LPCTSTR)pCmdItem->m_sTipTool
				);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;
	} // if( pTI != NULL )

	return nCmdID;
}

LRESULT CExtBarButton::OnHelpHitTest(
	CPoint point
	)
{
	ASSERT_VALID( this );
	point; // should be inside this button

	if( IsSeparator() )
		return -1;

	if( IsAbleToTrackMenu() )
	{
		//CExtToolControlBar::_CloseTrackingMenus();
		//m_pBar->TrackButtonMenu( m_pBar->_GetIndexOf(this), FALSE, TRUE );
		//return -2;
		if(		m_pBar->m_pDockSite == NULL
			||	(! m_pBar->m_pDockSite->m_bHelpMode )
			)
			return -3L;
		OnTrackPopup( point, false );
		return CExtToolControlBar::_ContinueHelpHitTestMode( m_pBar );
	} // if( IsAbleToTrackMenu() )

int nCmdID = (int)GetCmdID(false);

CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetSafeBar()->GetSafeHwnd() ),
			nCmdID
			);
	if( pCmdItem == NULL
		//|| pCmdItem->m_sTipTool.IsEmpty()
		)
		return -1;

	return nCmdID;
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarColorButton

IMPLEMENT_DYNCREATE(CExtBarColorButton, CExtBarButton)

void CExtBarColorButton::_SyncIcon()
{
	ASSERT_VALID( this );

HICON hIcon = NULL;
	OnGenerateColorIcon( &hIcon );
	ASSERT( hIcon != NULL );
	if( hIcon == NULL )
		return;

	VERIFY(
		g_CmdManager->CmdSetIcon(
			g_CmdManager->ProfileNameFromWnd( GetSafeBar()->GetSafeHwnd() ),
			GetCmdID(true),
			hIcon,
			false
			)
		);
}

void CExtBarColorButton::OnGenerateColorIcon( HICON * p_hIcon )
{
	ASSERT_VALID( this );
	ASSERT( p_hIcon != NULL );
	ASSERT( (*p_hIcon) == NULL );
	
	switch( m_eDIT )
	{
	
	case __DIT_FRAME:
	{
		(*p_hIcon) =
			CExtPaintManager::stat_GenerateColorIconFrame(
				ColorGet(true)
				);
		ASSERT( (*p_hIcon) != NULL );
	}
	break;
	
	case __DIT_CHAR:
	{
		LOGFONT lf;
		::memset( &lf, 0 , sizeof(LOGFONT) );
		g_PaintManager->m_FontBold.GetLogFont( &lf );
		_tcscpy( lf.lfFaceName, _T("Times New Roman") );
		lf.lfHeight = ::MulDiv( lf.lfHeight, 3, 2 );
		lf.lfWeight = 900;
		CFont fontForLetter;
		VERIFY(
			fontForLetter.CreateFontIndirect( &lf )
			);
		ASSERT( fontForLetter.GetSafeHandle() != NULL );
		(*p_hIcon) =
			CExtPaintManager::stat_GenerateColorIconChar(
				ColorGet(true),
				(HFONT)fontForLetter.GetSafeHandle()
				);
		ASSERT( (*p_hIcon) != NULL );
	}
	break;

	//case __DIT_BOX:
	default:
	{
		(*p_hIcon) =
			CExtPaintManager::stat_GenerateColorIconBox(
				ColorGet(true)
				);
		ASSERT( (*p_hIcon) != NULL );
	}
	break;
	
	} // switch( m_eDIT )
}

BOOL CExtBarColorButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( CtrlGet() == NULL );
	ASSERT( IsAbleToTrackMenu() );

	if( !IsEnabled() )
	{
		if( !pPopup->ItemInsert( GetCmdID(true) ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		return TRUE;
	}

CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetSafeBar()->GetSafeHwnd() ),
			GetCmdID(false)
			);

CExtCmdIcon * pIcon = GetIconPtr();
CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
	VERIFY(
		pPopup->ItemInsertSpecPopup(
			pColorPopup,
			-1,
			pCmdItem->m_sMenuText,
			(pIcon == NULL || pIcon->IsEmpty()) ? NULL : pIcon->GetIcon()
			)
		);
	pColorPopup->m_lParamCookie = LParamCookieGet();
	pColorPopup->m_bEnableBtnColorDefault = IsEnabledBtnColorDefault();
	pColorPopup->m_bEnableBtnColorCustom = IsEnabledBtnColorCustom();
	pColorPopup->m_clrDefault = ColorGet( false );
	pColorPopup->m_clrInitial = ColorGet( true );
CString _s( BtnTextDefaultGet() );
	if( !_s.IsEmpty() )
		pColorPopup->m_sBtnTextColorDefault = _s;
	_s = BtnTextCustomGet();
	if( !_s.IsEmpty() )
		pColorPopup->m_sBtnTextColorCustom = _s;

	return TRUE;
}

bool CExtBarColorButton::IsAbleToTrackMenu(
	bool bCustomizeMode // = false
	)
{
	ASSERT_VALID( this );
	if( bCustomizeMode )
		return false;
	return true;
}

UINT CExtBarColorButton::OnTrackPopup(
	CPoint point,
	bool bSelectAny
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( CtrlGet() == NULL );
	ASSERT( IsAbleToTrackMenu() );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return UINT(-1L);

	if( !IsEnabled() )
		return UINT(-1L);

bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;
	if( CExtToolControlBar::g_bMenuTracking
		//&& CExtPopupMenuWnd::IsMenuTracking()
		&& m_pBar->_GetIndexOf(this) ==
			m_pBar->GetMenuTrackingButton()
		)
		return UINT(-1L);

	CExtToolControlBar::_CloseTrackingMenus();

	if( m_pBar->IsFloating() )
	{
		m_pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			m_pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}

CWnd * pWndCmdTarget = GetCmdTargetWnd();
	ASSERT_VALID( pWndCmdTarget );

CExtPopupColorMenuWnd * pPopup = new CExtPopupColorMenuWnd;
	VERIFY(
		pPopup->CreatePopupMenu(
			GetCmdTargetWnd()->GetSafeHwnd()
			)
		);
	pPopup->m_lParamCookie = LParamCookieGet();
	pPopup->m_bEnableBtnColorDefault = IsEnabledBtnColorDefault();
	pPopup->m_bEnableBtnColorCustom = IsEnabledBtnColorCustom();
	pPopup->m_clrDefault = ColorGet( false );
	pPopup->m_clrInitial = ColorGet( true );
CString _s( BtnTextDefaultGet() );
	if( !_s.IsEmpty() )
		pPopup->m_sBtnTextColorDefault = _s;
	_s = BtnTextCustomGet();
	if( !_s.IsEmpty() )
		pPopup->m_sBtnTextColorCustom = _s;

	CExtToolControlBar::g_bMenuTracking = bPrevTBMT;

CRect rcBtn = Rect();
	m_pBar->ClientToScreen( &rcBtn );
	m_pBar->ClientToScreen( &point );

DWORD dwTrackFlags =
		OnGetTrackPopupFlags()
		| TPMX_COMBINE_DEFAULT
		| TPMX_OWNERDRAW_FIXED
		;
	//if( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
	if( bSelectAny )
		dwTrackFlags |= TPMX_SELECT_ANY;
	if( CExtToolControlBar::g_bMenuTrackingExpanded )
		dwTrackFlags |= TPMX_NO_HIDE_RARELY;

	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);

UINT nResultCmdID = (UINT)-1L;
	if( !pPopup->TrackPopupMenu(
			dwTrackFlags,
			point.x,point.y,
			&rcBtn,
			m_pBar,
			CExtToolControlBar::_CbPaintCombinedContent,
			&nResultCmdID,
			true
			)
		)
	{
		delete pPopup;
		CExtToolControlBar::_CloseTrackingMenus();
		return UINT(-1L);
	}
	CExtToolControlBar::g_bMenuTracking = true;
	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);
	return UINT(-1L);
}

int CExtBarColorButton::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	return
		CExtBarButton::OnToolHitTest( point, pTI );
}

LRESULT CExtBarColorButton::OnHelpHitTest(
	CPoint point
	)
{
	ASSERT_VALID( this );
//	return
//		CExtBarButton::OnHelpHitTest( point );
	point;
	return GetCmdID();
}

void CExtBarColorButton::OnUpdateCmdUI(
	CWnd * pTarget,
	BOOL bDisableIfNoHndler,
	int nIndex
	)
{
	ASSERT_VALID( this );
	CExtBarButton::OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHndler,
		nIndex
		);
}

bool CExtBarColorButton::CanStartCommandDrag()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( GetCmdNode() == NULL )
		return false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	return true;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
int CExtBarColorButton::CalculateDropOverEffect(
	CPoint point,
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	bool bAllowTrackNodePopup // = true
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( !_dragInfo.IsEmpty() );
	bAllowTrackNodePopup;
	return
		CExtBarButton::CalculateDropOverEffect(
			point,
			_dragInfo,
			false
			);
}
CExtCustomizeCmdTreeNode * CExtBarColorButton::GetCmdNode(
	bool bInitial // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	return CExtBarButton::GetCmdNode( bInitial );
}
void CExtBarColorButton::SetBasicCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	CExtBarButton::SetBasicCmdNode( pNode );
}
void CExtBarColorButton::SetCustomizedCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	CExtBarButton::SetCustomizedCmdNode( pNode );
}
CExtCustomizeCmdTreeNode * CExtBarColorButton::OnCustomizeNodeInit(
	CExtCustomizeSite * pSite,
	CExtCustomizeCmdTreeNode * pParentNode // toolbar node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	return CExtBarButton::OnCustomizeNodeInit( pSite, pParentNode );
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

void CExtBarColorButton::OnDeliverCmd()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );

	GetCmdTargetWnd()->SendMessage(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		(WPARAM)ColorGet(true),
		LParamCookieGet()
		);
}


/////////////////////////////////////////////////////////////////////////////
// CExtBarContentExpandButton

IMPLEMENT_DYNCREATE(CExtBarContentExpandButton, CExtBarButton)

CSize CExtBarContentExpandButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	dc;
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );
	ASSERT( GetCmdID(false) == __ID_TOOLBAR_RIGHT_BUTTON__ );
CSize _size(
			bHorz ? __RIGHT_BUTTON_HORZ_DX__ : sizePreCalc.cx,
			bHorz ? sizePreCalc.cy : __RIGHT_BUTTON_VERT_DY__
			);
	m_ActiveSize = _size;
	m_ActiveSize.cx += __EXT_TB_BUTTON_INNER_MARGIN*2;
	m_ActiveSize.cy += __EXT_TB_BUTTON_INNER_MARGIN*2;
	return m_ActiveSize;
}

void CExtBarContentExpandButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT( m_pCtrl == NULL );
	ASSERT_VALID( (&dc) );
	ASSERT( !IsSeparator() );
	ASSERT( GetCmdID(false) == __ID_TOOLBAR_RIGHT_BUTTON__ );
//	ASSERT( !IsDisabled() );

CRect rcArea( m_ActiveRect );
	rcArea.DeflateRect(
		__EXT_TB_BUTTON_INNER_MARGIN,
		__EXT_TB_BUTTON_INNER_MARGIN
		);

	if( rcArea.right <= rcArea.left
		||
		rcArea.bottom <= rcArea.top
		)
		return;
//	if( // (!IsVisible()) ||
//		(!dc.RectVisible(&m_ActiveRect))
//		)
//		return;
	ASSERT(
		m_ActiveSize.cx >= __EXT_TB_BUTTON_INNER_MARGIN*2
		&&
		m_ActiveSize.cy >= __EXT_TB_BUTTON_INNER_MARGIN*2
		);
bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
bool bEnabled = 
		( (!IsDisabled()) || bDockSiteCustomizeMode )
			? true : false;
bool bBarIsCompletelyVisible =
		(GetButtons().GetSize() == 0) ? true : false;
bool bEmpty = 
		(	(!m_pBar->m_bRightButtonDisplayBarsList)
			&& bBarIsCompletelyVisible
		) ? true : false;
bool bPushed =
		( (!bEmpty) && bEnabled && IsPressed() && (!bDockSiteCustomizeMode) )
			? true : false;
bool bHover =
		(	(!bEmpty)
			&& bEnabled
			&& (!bDockSiteCustomizeMode)
			&& IsHover()
			&& !CExtToolControlBar::g_bMenuTracking
			&& !CExtPopupMenuWnd::IsMenuTracking()
		) ? true : false;
	g_PaintManager->PaintToolbarExpandButton(
		dc,
		rcArea,
		bHorz,
		bBarIsCompletelyVisible,
		bEnabled && !bEmpty,
		bPushed,
		bHover,
		this,
		0L,
		( (!bHover) && (!bPushed) )
		);
}

BOOL CExtBarContentExpandButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	pPopup;
	ASSERT( FALSE );
	return FALSE;
}

UINT CExtBarContentExpandButton::OnTrackPopup(
	CPoint point,
	bool bSelectAny
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return UINT(-1L);

	if( !IsEnabled() )
		return UINT(-1L);

bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;

	if(		CExtToolControlBar::g_bMenuTracking
			//&&	CExtPopupMenuWnd::IsMenuTracking()
		&&	m_pBar->_GetIndexOf(this) ==
				m_pBar->m_nBtnIdxMenuTracking
		)
		return UINT(-1L);

bool bBarIsCompletelyVisible =
		(GetButtons().GetSize() == 0) ? true : false;
bool bEmpty = 
		(	!m_pBar->m_bRightButtonDisplayBarsList
			&& bBarIsCompletelyVisible
		) ? true : false;
	if( bEmpty )
		return UINT(-1L);

	CExtToolControlBar::_CloseTrackingMenus();

	CExtToolControlBar::g_bMenuTracking = bPrevTBMT;

	if( m_pBar->IsFloating() )
	{
		m_pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			m_pBar->GetParentFrame();
		if( pFrame != NULL )
			pFrame->BringWindowToTop();
	}

HWND hWndTrack = GetCmdTargetWnd()->GetSafeHwnd();
	ASSERT(
		hWndTrack != NULL
		&& ::IsWindow(hWndTrack)
		);

CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(hWndTrack) );

CExtControlBar::POPUP_MENU_EVENT_DATA _pmed(
		CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CTXEXPBTN_TOP,
		pPopup,
		m_pBar,
		CPoint( -1, -1 ),
		WM_NCLBUTTONDOWN,
		this
		);

	if( !_pmed.NotifyTarget( false ) )
	{

		// append hidden buttons
		int nCount = GetButtons().GetSize();
		if( nCount != 0 )
		{
			ASSERT( nCount > 0 );
			int nCountBefore = pPopup->ItemGetCount();
			for( int i=0; i< nCount; i++ )
			{
				CExtBarButton * pTBB = GetButtons() [i];
				ASSERT( pTBB != NULL );
				if( i==0 && pTBB->IsSeparator() )
					continue;
				ASSERT( !pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) );
				VERIFY( pTBB->PutToPopupMenu(pPopup) );
			} // for( int i=0; i< nCount; i++ )
			int nCountAfter = pPopup->ItemGetCount();
			ASSERT( nCountAfter >= nCountBefore );
			if( nCountAfter != nCountBefore
				&& (!m_pBar->m_bPresubclassDialogMode)
				&& m_pBar->m_bRightButtonDisplayBarsList
				)
			{
				// append separator
				VERIFY(
					pPopup->ItemInsert(
						CExtPopupMenuWnd::TYPE_SEPARATOR
						)
					);
			}
		} // if( nCount != 0 )
		// append show/hide popup

		if(		(!m_pBar->m_bPresubclassDialogMode)
			&&	m_pBar->m_bRightButtonDisplayBarsList
			)
		{
			CFrameWnd * pFrame = m_pBar->_GetDockingFrameImpl();
			if( pFrame != NULL )
			{
				ASSERT_VALID( pFrame );

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
				CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

				CString sShowHidePanels;
				if( !sShowHidePanels.LoadString(IDS_SHOW_HIDE_PANELS) )
				{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
					sShowHidePanels = _T("&Show/hide panels");
				}

				CExtControlBar::POPUP_MENU_EVENT_DATA _pmed(
					CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CTXEXPBTN_APPEND,
					pPopup,
					m_pBar,
					CPoint( -1, -1 ),
					WM_NCLBUTTONDOWN,
					this
					);
			
				if( !_pmed.NotifyTarget( false ) )
				{
					VERIFY(
						pPopup->ItemInsert(
							(UINT)CExtPopupMenuWnd::TYPE_POPUP,
							-1,
							sShowHidePanels
							)
						);
					CExtPopupMenuWnd::MENUITEMDATA & mi = pPopup->ItemGetInfo( pPopup->ItemGetCount() - 1 );
					mi.SetChildCombine();
					mi.SetToolButton();
					CExtPopupMenuWnd * pPopupBars = mi.GetPopup();
					ASSERT( pPopupBars != NULL );
					if( ! CExtDockBar::_ConstructDockSiteControlBarPopupMenu(
							pFrame,
							pPopupBars,
							CExtControlBar::POPUP_MENU_EVENT_DATA::__PMED_CTXEXPBTN_BARS,
							m_pBar,
							WM_NCLBUTTONDOWN,
							this
							)
						)
					{
						VERIFY(
							pPopup->ItemRemove(
								pPopup->ItemGetCount() - 1
								)
							);
					}
				} // if( !_pmed.NotifyTarget( false ) )

				_pmed.NotifyTarget( true );
			
			} // if( pFrame != NULL )

		} // if( (!m_pBar->m_bPresubclassDialogMode) ...
	
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		if(		m_pBar->m_bCustomizationAllowed
			&&	m_pBar->GetCustomizeSite() != NULL
			)
		{ // if customization allowed
			VERIFY(
				CExtDockBar::_AppendAddRemoveButtonsCustomizeMenu(
					pPopup,
					m_pBar
					)
				);
		} // if customization allowed
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	} // if( !_pmed.NotifyTarget( false ) )
	_pmed.NotifyTarget( true );

	
	if( pPopup->ItemGetCount() == 0 )
	{
		delete pPopup;
		return UINT(-1L);
	}

CRect rcBtn = Rect();
	m_pBar->ClientToScreen( &rcBtn );
	m_pBar->ClientToScreen( &point );
DWORD dwTrackFlags =
		OnGetTrackPopupFlags()
		| TPMX_COMBINE_DEFAULT
		| TPMX_OWNERDRAW_FIXED
		;
	//if( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
	if( bSelectAny )
		dwTrackFlags |= TPMX_SELECT_ANY;
	if( CExtToolControlBar::g_bMenuTrackingExpanded )
		dwTrackFlags |= TPMX_NO_HIDE_RARELY;

	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);

	if( !pPopup->TrackPopupMenu(
			dwTrackFlags,
			point.x,point.y,
			&rcBtn,
			m_pBar,
			CExtToolControlBar::_CbPaintCombinedContent,
			NULL,
			true
			)
		)
	{
		delete pPopup;
		CExtToolControlBar::_CloseTrackingMenus();
		return UINT(-1L);
	}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = m_pBar->GetCustomizeSite();
	if( pSite != NULL )
		pSite->BarDefCmdTargetSet( m_pBar );
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	CExtToolControlBar::g_bMenuTracking = true;
	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);
	return UINT(-1L);
}

void CExtBarContentExpandButton::OnHover(
	CPoint point,
	bool bOn
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return;

	CExtBarButton::OnHover( point, bOn );
}

void CExtBarContentExpandButton::OnClick(
	CPoint point,
	bool bDown
	)
{
	ASSERT_VALID( m_pBar );

bool bDockSiteCustomizeMode =
		m_pBar->_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return;

	if( !bDown )
		return;

	CExtToolControlBar::g_bMenuTrackingExpanded = false;

	OnTrackPopup( point, false );
}

int CExtBarContentExpandButton::OnToolHitTest(
	CPoint point,
	TOOLINFO * pTI
	)
{
	ASSERT_VALID( this );
	point; // should be inside this button

	if( pTI != NULL )
	{
		CRect rcArea = Rect();
		::CopyRect(
			&(pTI->rect),
			&rcArea
			);

		int nCmdID = AFX_IDS_IDLEMESSAGE; //(int)GetBar()->GetDlgCtrlID();
		
		pTI->uId = (UINT)nCmdID;
		pTI->hwnd = GetSafeBar()->GetSafeHwnd();

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

		CString sExpandTip;
		if( !sExpandTip.LoadString(IDS_CONTENT_EXPAND_TIP) )
		{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			ASSERT( FALSE );
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			sExpandTip = _T("Where are buttons?");
		}

		pTI->lpszText = (LPTSTR)
			::calloc(
				(sExpandTip.GetLength() + 1),
				sizeof(TCHAR)
				);
		if( pTI->lpszText != NULL )
			_tcscpy(
				pTI->lpszText,
				(LPCTSTR)sExpandTip
				);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;

		return nCmdID;
	} // if( pTI != NULL )
	
	return -1;
}

LRESULT CExtBarContentExpandButton::OnHelpHitTest(
	CPoint point
	)
{
	ASSERT_VALID( this );
	return
		CExtBarButton::OnHelpHitTest( point );
}

bool CExtBarContentExpandButton::CanStartCommandDrag()
{
	ASSERT_VALID( this );
	return false;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeCmdTreeNode * CExtBarContentExpandButton::GetCmdNode(
	bool bInitial // = false
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	bInitial;
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	return NULL;
}
void CExtBarContentExpandButton::SetBasicCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
void CExtBarContentExpandButton::SetCustomizedCmdNode(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( m_pCmdNodeI == NULL );
	ASSERT( m_pCmdNodeC == NULL );
	ASSERT( FALSE ); // should be never called
	pNode;
}
CExtCustomizeCmdTreeNode * CExtBarContentExpandButton::OnCustomizeNodeInit(
	CExtCustomizeSite * pSite,
	CExtCustomizeCmdTreeNode * pParentNode // toolbar node
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pBar );
	ASSERT( pSite != NULL );
	ASSERT_VALID( pParentNode );
	pSite;
	pParentNode;
	return NULL; // no customization support
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar

const UINT CExtToolControlBar::g_nMsgTrackButtonMenu =
	::RegisterWindowMessage(
		_T("CExtToolControlBar::g_nMsgTrackButtonMenu")
		);
const UINT CExtToolControlBar::g_nMsgContinueTrackMenu =
	::RegisterWindowMessage(
		_T("CExtToolControlBar::g_nMsgContinueTrackMenu")
		);

IMPLEMENT_DYNCREATE(CExtToolControlBar, CExtControlBar)

CExtBarButton * CExtToolControlBar::_GetButtonPtr(int nIndex) const
{
int nButtonsCount = m_buttons.GetSize();
	if( !(nIndex >= 0 && nIndex < nButtonsCount) )
	{
		ASSERT( FALSE );
		return NULL;
	}
CExtBarButton * pTBB =
		m_buttons[nIndex];
	ASSERT_VALID( pTBB );
	return pTBB;
}

void CExtToolControlBar::_RemoveAllButtonsImpl()
{
	if( m_pRightBtn != NULL )
		m_pRightBtn->GetButtons().RemoveAll();
	for( INT nBtnIdx = 0; nBtnIdx < m_buttons.GetSize(); nBtnIdx++ )
	{
		CExtBarButton * pTBB = m_buttons[nBtnIdx];
		ASSERT( pTBB != NULL );
		delete pTBB;
	}
	m_buttons.RemoveAll();
	m_pRightBtn = NULL;
	m_nBtnIdxCapture = -1;      // nothing captured
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;
}

int CExtToolControlBar::_GetButtonsCountImpl() const
{
int nCountOfButtons = m_buttons.GetSize();
	return nCountOfButtons;
}

bool CExtToolControlBar::g_bMenuTracking = false;
bool CExtToolControlBar::g_bMenuTrackingExpanded = false;
const CSize CExtToolControlBar::g_sizeDefTBB( 23, 22 );

CExtToolControlBar::CExtToolControlBar()
{
	m_bGripperStaticallyAtTop = false;

	m_pRightBtn = NULL;
	m_bRightButtonDisplayBarsList = true;
	m_bPaletteMode = false;
	m_bHelperTempFullRowMode = false;
	m_bInsideCalcLayout = false;

	m_bFixedMode = true;
	m_bReposSingleChildMode = false;
	m_nBtnIdxCapture = -1;
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_bCustomizationAllowed = true;
	m_pCustomizeSite = NULL;
	m_bVisibleInCustomizeListBox = true;
	m_pDragSourceTBB = NULL;
	m_pDragTargetTBB = NULL;
	m_nDragTargetShift = 0;
	m_bHelperSeparatorAction = false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	 // additional spaces
	m_cxLeftBorder		= 1;
	m_cxRightBorder		= 1;
	m_cyTopBorder		= 0; //1;
	m_cyBottomBorder	= 0; //1;

	m_nGripWidthAtLeft = 8;
	m_nGripHeightAtTop = 10;
}

CExtToolControlBar::~CExtToolControlBar()
{
	_RemoveAllButtonsImpl();

CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
int nCountOfBars = g_AllBars.GetSize();
	for( int i=0; i<nCountOfBars; i++ )
	{
		CExtControlBar * pBar = g_AllBars[i];
		ASSERT( pBar != NULL );
		if( pBar == this )
		{
			g_AllBars.RemoveAt( i );
			break;
		}
	}
	_slCsCB.Unlock();
}

BOOL CExtToolControlBar::LoadToolBar(LPCTSTR lpszResourceName)
{
LPUINT pCmdArray = NULL;
INT nCmdCount = 0;
	if( !g_CmdManager->UpdateFromToolBar(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			lpszResourceName,
			&pCmdArray,
			&nCmdCount
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( pCmdArray != NULL && nCmdCount > 0 );
BOOL bRetVal =  SetButtons( pCmdArray, nCmdCount );
	ASSERT( bRetVal );
	delete [] pCmdArray;
	return bRetVal;
}

bool CExtToolControlBar::_IsShowContentWhenDragging() const
{
//BOOL bDragShowContent = FALSE;
//    ::SystemParametersInfo(
//		SPI_GETDRAGFULLWINDOWS,
//		0,
//        &bDragShowContent,
//		0
//		);
//	return bDragShowContent ? true : false;

//	if( IsKindOf(RUNTIME_CLASS(CExtToolControlBar)) )
//		return true;
//	return false;

//	if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
//		return false;

	return true;
}

BOOL CExtToolControlBar::RemoveButton(
	int nPos,
	BOOL bDoRecalcLayout // = TRUE
	)
{
	if( nPos < 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
int nCountOfButtons = _GetButtonsCountImpl();
	if( nPos >= nCountOfButtons )
	{
		ASSERT( FALSE );
		return FALSE;
	}
CExtBarButton * pTBB =
		_GetButtonPtr( nPos );
	ASSERT_VALID( pTBB );
	if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
//	if(! pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
//		delete pTBB;

	m_buttons.RemoveAt( nPos );
	m_nBtnIdxCapture = -1;      // nothing captured
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return TRUE;
}

BOOL CExtToolControlBar::InsertSpecButton(
	int nPos, // -1 - append
	CExtBarButton * pButton,
	BOOL bDoRecalcLayout // = TRUE
	)
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nPos < 0 )
		nPos = nCountOfButtons;
	if( nPos > nCountOfButtons )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( nCountOfButtons > 0
		&& nPos == nCountOfButtons
		)
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( nCountOfButtons - 1 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
			nPos--;
	}
	ASSERT_VALID( pButton );
	ASSERT( pButton->GetSafeBar() != NULL );
	ASSERT( pButton->GetSafeBar() == this );
	if( _GetIndexOf(pButton) >= 0 )
	{
		ASSERT( FALSE ); // already inserted
		return FALSE;
	}
	m_buttons.InsertAt( nPos, pButton );
	ASSERT( _GetIndexOf(pButton) >= 0 );
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return TRUE;
}

BOOL CExtToolControlBar::InsertButton(
	int nPos, // = -1, // append
	UINT nCmdID, // = ID_SEPARATOR
	BOOL bDoRecalcLayout, // = TRUE
	int nData // = 0
	)
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nPos < 0 )
		nPos = nCountOfButtons;
	if( nPos > nCountOfButtons )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( nCountOfButtons > 0
		&& nPos == nCountOfButtons
		)
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( nCountOfButtons - 1 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
			nPos--;
	}
	try
	{
		CExtBarButton * pTBB =
			new CExtBarButton(
				this,
				nCmdID
				);
		pTBB->m_nData=nData;
		ASSERT_VALID( pTBB );
		m_buttons.InsertAt(
			nPos,
			pTBB
			);
		if( bDoRecalcLayout )
			_RecalcLayoutImpl();
	} // try
//	catch( std::exception * pXept )
//	{
//		delete pXept;
//		ASSERT( FALSE );
//		return FALSE;
//	} // catch( std::exception * pXept )
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
		return FALSE;
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
		return FALSE;
	} // catch( ... )
	return TRUE;
}

CExtBarContentExpandButton * CExtToolControlBar::OnCreateBarRightBtn()
{
	if( m_bPaletteMode )
		return NULL;
CExtBarContentExpandButton * pRightBtn =
		new CExtBarContentExpandButton( this );
	ASSERT_VALID( pRightBtn );
	return pRightBtn;
}

BOOL CExtToolControlBar::SetButtons(
	const UINT * lpIDArray, // = NULL
	int nIDCount // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT(
		lpIDArray == NULL
		|| nIDCount == 0
		|| AfxIsValidAddress(
			lpIDArray,
			sizeof(UINT) * nIDCount,
			FALSE
			)
		);
	_RemoveAllButtonsImpl();
	if( lpIDArray == NULL
		|| nIDCount == 0
		)
		return TRUE;
	try
	{
		for( int i = 0; i < nIDCount; i++ )
		{
			CExtBarButton * pTBB =
				new CExtBarButton(
					this,
					*lpIDArray++
					);
			ASSERT_VALID( pTBB );
			m_buttons.Add( pTBB );
		} // for( int i = 0; i < nIDCount; i++ )
		ASSERT( m_pRightBtn == NULL );
		m_pRightBtn = OnCreateBarRightBtn();
		if( m_pRightBtn != NULL )
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT_KINDOF( CExtBarContentExpandButton, m_pRightBtn );
			m_buttons.Add( m_pRightBtn );
		} // if( m_pRightBtn != NULL )
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
		return FALSE;
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
		return FALSE;
	} // catch( ... )
	return TRUE;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
BOOL CExtToolControlBar::SetButtons(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	SetButtons();
	if( pNode == NULL )
		return TRUE;
	ASSERT_VALID( pNode );
CExtCustomizeSite * pSite = GetCustomizeSite();
	if( pSite == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
int nCount = pNode->GetNodeCount();
	for( int i = 0; i < nCount; i++ )
	{
		CExtCustomizeCmdTreeNode * pNodeBtn =
			pNode->ElementAt( i );
		ASSERT_VALID( pNodeBtn );
		if( pNodeBtn->GetFlags() & __ECTN_GROUP_START )
		{
			if( i == 0 )
			{
				ASSERT( FALSE );
				continue;
			}
			VERIFY( InsertButton(-1,ID_SEPARATOR,FALSE) ); // separator
		}
		ASSERT( CExtCmdManager::IsCommand(pNodeBtn->GetCmdID(false)) || pNodeBtn->GetCmdID(false) == UINT(-1) );
		ASSERT( CExtCmdManager::IsCommand(pNodeBtn->GetCmdID(true)) || pNodeBtn->GetCmdID(true) == UINT(-1) );
		CExtBarButton * pTBB =
			pSite->OnCreateToolbarButton(
				this,
				NULL,
				pNodeBtn
				);
		if( pTBB == NULL )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		ASSERT_VALID( pTBB );
		m_buttons.Add( pTBB );
	} // for( int i = 0; i < nCount; i++ )
	ASSERT( m_pRightBtn == NULL );
	m_pRightBtn = OnCreateBarRightBtn();
	if( m_pRightBtn != NULL )
	{
		ASSERT_VALID( m_pRightBtn );
		ASSERT_KINDOF( CExtBarContentExpandButton, m_pRightBtn );
		m_buttons.Add( m_pRightBtn );
	} // if( m_pRightBtn != NULL )
	return TRUE;
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

LRESULT CExtToolControlBar::DoHelpHitTest( CPoint ptHelpHitTest )
{
	ASSERT_VALID( this );

LRESULT nHelpHit =
		((CExtToolControlBar*)this)->
			HitTest(ptHelpHitTest);
	if( nHelpHit >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( nHelpHit );
		ASSERT_VALID( pTBB );
		if( pTBB != NULL )
		{
			nHelpHit = pTBB->OnHelpHitTest( ptHelpHitTest );
			if( nHelpHit == -1 || nHelpHit == -2 )
				return -2;
			return nHelpHit;
		}
	} // if( nHelpHit >= 0 )
	return CExtControlBar::DoHelpHitTest( ptHelpHitTest );
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar attribute access

int CExtToolControlBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);

int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( pTBB->GetCmdID(false) == nIDFind )
			return nBtnIdx;
	}
	return -1;
}

UINT CExtToolControlBar::GetButtonID(int nIndex) const
{
//	ASSERT_VALID(this);
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return ID_SEPARATOR;
	ASSERT_VALID( pTBB );
	return pTBB->GetCmdID(false);
}

void CExtToolControlBar::GetButtonRect(int nIndex, LPRECT lpRect) const
{
//	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		::memset( lpRect, 0, sizeof(RECT) );
		return;
	}
	ASSERT_VALID( pTBB );
	*lpRect = *pTBB;
}

UINT CExtToolControlBar::GetButtonStyle(int nIndex) const
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return 0;
	ASSERT_VALID( pTBB );
	return pTBB->GetStyle();
}

void CExtToolControlBar::SetButtonStyle(int nIndex, UINT nStyle)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return;
	ASSERT_VALID( pTBB );
UINT nOldStyle = pTBB->GetStyle();
	if( nOldStyle != nStyle )
	{
		pTBB->SetStyle( nStyle );
		_InvalidateButton( nIndex );
		UpdateWindow();
	}
}

CWnd * CExtToolControlBar::GetButtonCtrl(
	int nIndex
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return NULL;
	ASSERT_VALID( pTBB );
CWnd * pCtrl = pTBB->CtrlGet();
	return pCtrl;
}

void CExtToolControlBar::SetButtonCtrlVisibleVertically(
	int nIndex,
	bool bVisible // = true
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return;
	pTBB->SetCtrlVisibleVertically( bVisible );
}

bool CExtToolControlBar::GetButtonCtrlVisibleVertically(
	int nIndex
	) const
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return false;
	return pTBB->GetCtrlVisibleVertically();
}


bool CExtToolControlBar::SetButtonCtrl(
	int nIndex,
	CWnd * pCtrl, // = NULL
	bool bCtrlAutoDestroyed // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	if( pCtrl != NULL )
	{
		ASSERT_VALID( pCtrl );
		DWORD dwCtrlStyle = pCtrl->GetStyle();
		ASSERT( (dwCtrlStyle&WS_CHILD) != 0 );
		if( (dwCtrlStyle&WS_CHILD) == 0 )
			return FALSE;
		ASSERT( IsChild( pCtrl ) );
		if( !IsChild( pCtrl ) )
			return FALSE;
		UINT nDlgCtrlID = (UINT)pCtrl->GetDlgCtrlID();
		ASSERT( nDlgCtrlID == pTBB->GetCmdID(false) );
		if( nDlgCtrlID != pTBB->GetCmdID(false) )
			return FALSE;
	}
	pTBB->CtrlSet(
		pCtrl,
		bCtrlAutoDestroyed
		);
	_RecalcLayoutImpl();
	return TRUE;
}

int CExtToolControlBar::GetButtonByAccessKey(TCHAR vkTCHAR)
{
int nCountOfButtons = _GetButtonsCountImpl();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return -1;
	for( int nBtnIdx=0; nBtnIdx<nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr(nBtnIdx);
		ASSERT_VALID( pTBB );
		if(		pTBB->IsSeparator()
			||	( pTBB->GetStyle() & TBBS_HIDDEN )
			||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
			||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
			||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton))
			)
			continue;
		ASSERT( CExtCmdManager::IsCommand(pTBB->GetCmdID(false)) );
		ASSERT( CExtCmdManager::IsCommand(pTBB->GetCmdID(true)) );

		CString strButtonText( pTBB->GetText() );
		if( strButtonText.IsEmpty() )
			continue;

		int nTextLen = strButtonText.GetLength();
		ASSERT( nTextLen > 0 );
		int nAmdIndex = strButtonText.Find(_T('&'));
		if( nAmdIndex == nTextLen - 1 ) // !?!?!
			continue;
		TCHAR vk = strButtonText[nAmdIndex+1];
		TCHAR szChar[2] = { vk, _T('\0') };
		::CharUpper( szChar );
		vk = szChar[0];

		if( vkTCHAR == vk )
		{
			if( ( !pTBB->IsVisible() ) )
			{
				if( m_pRightBtn != NULL )
					return _GetIndexOf( m_pRightBtn );
				continue;
			}
			return nBtnIdx;
		}
	} // for( int nBtnIdx=0; nBtnIdx<nCountOfButtons; nBtnIdx++ )
	return -1;
}

HMENU CExtToolControlBar::GetButtonMenu(
	int nIndex
	)
{
CExtBarButton * pTBB = _GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return NULL;
	ASSERT_VALID( pTBB );
	return ((HMENU)(*pTBB));
}

bool CExtToolControlBar::MarkButtonAsMdiWindowsMenu(
	int nIndex,
	bool bAppendMdiWindowsMenu // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return false;
	ASSERT_VALID( pTBB );
	pTBB->SetAppendMdiWindowsMenu(
		bAppendMdiWindowsMenu
		);
	return true;
}

bool CExtToolControlBar::SetButtonMenu(
	int nIndex,
	HMENU hMenu, // = NULL
	bool bPopupMenu, // = TRUE
	bool bAutoDestroyMenu, // = TRUE
	bool bDoRecalcLayout // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return false;
	ASSERT_VALID( pTBB );
	pTBB->SetMenu(
		hMenu,
		bPopupMenu,
		bAutoDestroyMenu
		);
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return true;
}

int CExtToolControlBar::GetVisibleButton(
	int nBtnIdx,
	BOOL bNext,
	BOOL bPassDisabled // = TRUE
	)
{
int nCountOfButtons = _GetButtonsCountImpl();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return -1;
	if( nCountOfButtons == 1 )
		return 0;
	if( bNext )
	{
		int iNewButton = nBtnIdx + 1;
		if( iNewButton >= nCountOfButtons )
			iNewButton = 0;
		while( iNewButton != nBtnIdx )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(iNewButton);
			ASSERT_VALID( pTBB );
			if( pTBB->IsVisible()
				&& ( pTBB->GetStyle() & TBBS_HIDDEN ) == 0
				&& (!pTBB->IsSeparator())
				&& (!pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton)))
				&& (	(!bPassDisabled)
						|| (bPassDisabled && (!pTBB->IsDisabled()))
					)
				)
			{
				return iNewButton;
			}
			iNewButton++;
			if( iNewButton >= nCountOfButtons )
				iNewButton = 0;
		}
		return iNewButton;
	} // if( bNext )
	else
	{
		int iNewButton =
			nBtnIdx - 1;
		if( iNewButton < 0 )
			iNewButton = nCountOfButtons - 1;
		while( iNewButton != nBtnIdx )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(iNewButton);
			ASSERT_VALID( pTBB );
			if( pTBB->IsVisible()
				&& ( pTBB->GetStyle() & TBBS_HIDDEN ) == 0
				&& (!pTBB->IsSeparator())
				&& (!pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton)))
				&& (	(!bPassDisabled)
						|| (bPassDisabled && (!pTBB->IsDisabled()))
					)
				)
				return iNewButton;
			iNewButton--;
			if( iNewButton < 0 )
				iNewButton = nCountOfButtons - 1;
		}
		return iNewButton;
	} // else from if( bNext )
}

BOOL CExtToolControlBar::TrackButtonMenu(
	int nIndex,
	BOOL bSelectAny, // = FALSE
	BOOL bDelayedTracking // = FALSE
	)
{
bool bDockSiteCustomizeMode =
		_IsDockSiteCustomizeMode();
	if( bDockSiteCustomizeMode )
		return FALSE;

	if( g_bMenuTracking
		&& m_nBtnIdxMenuTracking == nIndex
		)
		return TRUE;

	if( !SafeDisplayBar() )
		return FALSE;

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	if( pTBB->GetStyle() & (TBBS_HIDDEN|TBBS_DISABLED) )
		return FALSE;
	if( !pTBB->IsAbleToTrackMenu() )
		return FALSE;
CPoint point;
	VERIFY( ::GetCursorPos(&point) );
	ScreenToClient( &point );
	if( bDelayedTracking )
		PostMessage(
			g_nMsgTrackButtonMenu,
			(WPARAM)nIndex
			);
	else
		pTBB->OnTrackPopup( point, bSelectAny ? true : false );
	return TRUE;
}

CSize CExtToolControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if(	(nLength == -1)
		&& !(dwMode & (LM_MRUWIDTH|LM_COMMIT))
		&&  (dwMode & (LM_HORZDOCK|LM_VERTDOCK))
		)
		return
			CalcFixedLayout(
				dwMode & LM_STRETCH,
				dwMode & LM_HORZDOCK
				);
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
	return _CalcLayout( dwMode, nLength );
}

CSize CExtToolControlBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
	return _CalcLayout( dwMode );
}

CSize CExtToolControlBar::_CalcLayout(
	DWORD dwMode,
	int nLength
	)
{
	ASSERT_VALID(this);
	if( !m_bInsideCalcLayout )
		m_bHelperTempFullRowMode = false;
CSize sizeCalcLayout = _GetDefButtonSize();
	if( _GetButtonsCountImpl() > 0 )
	{
		if( m_pRightBtn != NULL )
			m_pRightBtn->ClearContent( false );
		BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;
		INT nLengthToSizeTB =
			(dwMode & LM_HORZ) ? 32767 : 0;
		BOOL bVertSizeTB = FALSE;
		if( bDynamic )
		{
			if( dwMode & LM_MRUWIDTH )
				nLengthToSizeTB = m_nMRUWidth;
			else if( dwMode & LM_HORZDOCK )
				nLengthToSizeTB = 32767;
			else if( dwMode & LM_VERTDOCK )
				nLengthToSizeTB = 0;
			else if( nLength != -1 )
			{
				CRect rcInside( 0, 0, 0, 0 );
				_CalcInsideRect(
					rcInside,
					(dwMode & LM_HORZ)
					);
				bVertSizeTB = (dwMode & LM_LENGTHY);
				nLengthToSizeTB =
					nLength +
					( bVertSizeTB ?
						rcInside.Height() : rcInside.Width()
					);
			}
			else if( (m_dwStyle & CBRS_FLOATING) != 0 )
					nLengthToSizeTB = m_nMRUWidth;
		} // if( bDynamic )

		bool bMultiRowLayout =
			OnQueryMultiRowLayout();
		if(		bMultiRowLayout
			&&	((dwMode & LM_HORZ) == 0)
			&&	(!m_bPaletteMode)
			)
		{
			CRect rcDockBarClient;
			if( m_pDockBar != NULL )
				m_pDockBar->GetClientRect( &rcDockBarClient );
			else
				GetParent()->GetClientRect( &rcDockBarClient );
			nLength = nLengthToSizeTB = rcDockBarClient.Height();
			bVertSizeTB = TRUE;
		}

		_SizeToolBar( nLengthToSizeTB, bVertSizeTB );
		sizeCalcLayout =
			_CalcSize(
				( (dwMode & LM_HORZ) == 0 ) ? TRUE : FALSE
				);
		if(		bDynamic
			&&	bMultiRowLayout
			&&	(!m_bPaletteMode)
			)
		{
			CSize sizeDiff(
				__EXT_TB_BUTTON_OUTER_MARGIN*2,
				__EXT_TB_BUTTON_OUTER_MARGIN*2
				);
			if(		m_pRightBtn != NULL
				&&	m_pRightBtn->IsVisible()
				)
			{
				CSize sizeRb = m_pRightBtn->Size();
				sizeDiff +=
					CSize(
						((dwMode&LM_HORZ) == 0) ? 0 : sizeRb.cx,
						((dwMode&LM_HORZ) == 0) ? sizeRb.cy : 0
						);
			}
			CRect rcDockBarClient;
			if( m_pDockBar != NULL )
				m_pDockBar->GetClientRect( &rcDockBarClient );
			else
				GetParent()->GetClientRect( &rcDockBarClient );
			CSize _sizeDockBarClient = rcDockBarClient.Size();
			INT nNewLengthToSizeTB = nLengthToSizeTB;
			if( (dwMode & LM_HORZ) == 0 )
			{
				if( (sizeCalcLayout.cy+sizeDiff.cy) > _sizeDockBarClient.cy )
					nNewLengthToSizeTB = _sizeDockBarClient.cy-sizeDiff.cy;
			} // if( (dwMode & LM_HORZ) == 0 )
			else
			{
				if( (sizeCalcLayout.cx+sizeDiff.cx) > _sizeDockBarClient.cx )
					nNewLengthToSizeTB = _sizeDockBarClient.cx-sizeDiff.cx;
			} // else from if( (dwMode & LM_HORZ) == 0 )
			if( nNewLengthToSizeTB < 1 )
				nNewLengthToSizeTB = 1;
			if( nNewLengthToSizeTB != nLengthToSizeTB )
			{
				if( !m_bInsideCalcLayout )
				{
					m_bHelperTempFullRowMode = true;
					m_bInsideCalcLayout = true;
					sizeCalcLayout = _CalcLayout( dwMode, nNewLengthToSizeTB );
					if( nLengthToSizeTB != 32767 )
						_RecalcPositionsImpl();
					m_bInsideCalcLayout = false;
					return sizeCalcLayout;
				}
			}
			if( nLengthToSizeTB != 32767 )
				_RecalcPositionsImpl();
		}
		if(		m_pRightBtn != NULL
			&&	m_pRightBtn->GetButtons().GetSize() == 0
			)
		{
			m_pRightBtn->ClearContent();
			sizeCalcLayout = _CalcSize( (dwMode & LM_HORZ) == 0 );
		}
		if( dwMode & LM_COMMIT )
			if( (m_dwStyle & (CBRS_FLOATING|CBRS_SIZE_DYNAMIC)) != 0 )
				if( dwMode & LM_HORZ )
					m_nMRUWidth = sizeCalcLayout.cx;
		CRect rcInside( 0, 0, 0, 0 );
		_CalcInsideRect(
			rcInside,
			(dwMode & LM_HORZ)
			);
		sizeCalcLayout.cy -= rcInside.Height();
		sizeCalcLayout.cx -= rcInside.Width();
		CSize sizeCalcFixedLayout =
			CControlBar::CalcFixedLayout(
				dwMode & LM_STRETCH,
				(dwMode & LM_HORZ)
				);
		sizeCalcLayout.cx =
			max( sizeCalcLayout.cx, sizeCalcFixedLayout.cx );
		sizeCalcLayout.cy =
			max( sizeCalcLayout.cy, sizeCalcFixedLayout.cy );
		if( IsFloating() && IsBarWithGripper() )
		{
			int nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height() );
			sizeCalcLayout.cy += nCyGripper;
		}
	} // if( _GetButtonsCountImpl() > 0 )

	if( IsDockedHorizontally() )
	{
		m_sizeDockedH = sizeCalcLayout;
		m_nMinHW = sizeCalcLayout.cy;
	}
	else if( IsDockedVertically() )
	{
		m_sizeDockedV = sizeCalcLayout;
		m_nMinVH = sizeCalcLayout.cx;
	}
	else if( IsFloating() )
		m_sizeFloated = sizeCalcLayout;

	if( m_pDockSite == NULL )
	{ // specific for dialog mode
		if( IsDockedHorizontally() )
			sizeCalcLayout.cy += 4;
		else
			sizeCalcLayout.cx += 4;
	} // specific for dialog mode

//	_RecalcPositionsImpl();

	return sizeCalcLayout;
}

CSize CExtToolControlBar::_CalcSize( BOOL bVerticallyDocked )
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return _GetDefButtonSize();
CClientDC dc( this );
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			(		bVerticallyDocked
				&&	(!m_bPaletteMode)
			)	? ( &g_PaintManager->m_FontNormalVert )
				: ( &g_PaintManager->m_FontNormal )
			);
	ASSERT( pOldFont != NULL );
bool bMultiRowLayout = OnQueryMultiRowLayout();
CExtBarButton::e_wrap_t _evt = GetBtnWrapCode();
CSize sizeDefButton = _GetDefButtonSize();
CSize sizeCalc( sizeDefButton );
CPoint ptCurrent( __EXT_TB_BUTTON_INNER_MARGIN, __EXT_TB_BUTTON_INNER_MARGIN );
int nRowExtent = sizeDefButton.cy;
	if( bMultiRowLayout && bVerticallyDocked && (!m_bPaletteMode) )
		nRowExtent = sizeDefButton.cx;
INT nRowDistance = g_PaintManager->FixedBar_GetRowDistance( this ); // (+ v.2.21)
INT nWrapCount = 0;
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( pTBB->GetStyle() & TBBS_HIDDEN )
			continue;
		CSize sizeTBB(
			pTBB->CalculateLayout(
				dc,
				sizeDefButton,
				(!bVerticallyDocked) || m_bPaletteMode
				)
			);
		if(		nBtnIdx == (nCountOfButtons-1)
			&&	m_pRightBtn != NULL
			&&	(IsFloating() || bMultiRowLayout)
			)
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT( m_pRightBtn == pTBB );
			if( bMultiRowLayout )
			{
				if( bVerticallyDocked )
					sizeCalc.cy += sizeTBB.cy;
				else
					sizeCalc.cx += sizeTBB.cx;
				if( nWrapCount > 0 && (!m_bPaletteMode) )
					m_bHelperTempFullRowMode = true;
			}
			break;
		}
		if( bMultiRowLayout && bVerticallyDocked && (!m_bPaletteMode) )
		{
			if(		( ptCurrent.y == __EXT_TB_BUTTON_INNER_MARGIN
						|| pTBB->IsWrap(_evt) )
				&&	pTBB->IsSeparator()
				)
				sizeTBB = CSize( 0, 0 );
			nRowExtent = max( nRowExtent, sizeTBB.cx );
			sizeCalc.cx =
				max(
					ptCurrent.x + sizeTBB.cx,
					sizeCalc.cx
					);
			sizeCalc.cy =
				max(
					ptCurrent.y + sizeTBB.cy,
					sizeCalc.cy
					);
			ptCurrent.y += sizeTBB.cy;
			if( pTBB->IsWrap(_evt) )
			{
				ptCurrent.y = __EXT_TB_BUTTON_INNER_MARGIN;
				ptCurrent.x +=
					nRowExtent
					+ nRowDistance;
				nRowExtent = sizeDefButton.cx;
				nWrapCount++;
			}
		} // if( bMultiRowLayout && bVerticallyDocked && (!m_bPaletteMode) )
		else
		{
			if(		( (!bVerticallyDocked) || m_bPaletteMode )
				&&	( ptCurrent.x == __EXT_TB_BUTTON_INNER_MARGIN
						|| pTBB->IsWrap(_evt) )
				&&	pTBB->IsSeparator()
				)
				sizeTBB = CSize( 0, 0 );
			nRowExtent = max( nRowExtent, sizeTBB.cy );
			sizeCalc.cx =
				max(
					ptCurrent.x + sizeTBB.cx,
					sizeCalc.cx
					);
			sizeCalc.cy =
				max(
					ptCurrent.y + sizeTBB.cy,
					sizeCalc.cy
					);
			if(		bVerticallyDocked
					&&	(!m_bPaletteMode)
				)
			{
				ptCurrent.x = __EXT_TB_BUTTON_INNER_MARGIN;
				ptCurrent.y += sizeTBB.cy;
			} // if( bVerticallyDocked && (!m_bPaletteMode) ...
			else
			{
				ptCurrent.x += sizeTBB.cx;
				if( pTBB->IsWrap(_evt) )
				{
					ptCurrent.x = __EXT_TB_BUTTON_INNER_MARGIN;
					ptCurrent.y +=
						nRowExtent
						+ nRowDistance;
					nRowExtent = sizeDefButton.cy;
					nWrapCount++;
				}
			} // else from if( bVerticallyDocked && (!m_bPaletteMode) ...
		} // else from if( bMultiRowLayout && bVerticallyDocked && (!m_bPaletteMode) )
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	dc.SelectObject( pOldFont );

	if( !bVerticallyDocked )
	{
		sizeCalc.cx += m_cxLeftBorder + m_cxRightBorder;
		sizeCalc.cx += nRowDistance/2; // (+ v.2.21)
	}
	else
	{
		sizeCalc.cx += 1;
		sizeCalc.cy += m_cyTopBorder + m_cyBottomBorder;
		sizeCalc.cy += nRowDistance; // (+ v.2.21)
	}

	if( IsFloating() ) // (+ v.2.21)
	{
		sizeCalc.cx += 1;
		sizeCalc.cy += 1;
	}

	if( m_bPaletteMode && m_bPresubclassDialogMode )
	{
		CRect rcTest( 0, 0, 1, 1 );
		CSize _sizeInitial = rcTest.Size();
		CalcWindowRect( &rcTest );
		CSize _sizeUpdated = rcTest.Size();
		ASSERT(
				_sizeUpdated.cx >= _sizeInitial.cx
			&&	_sizeUpdated.cy >= _sizeInitial.cy
			);
		sizeCalc += _sizeUpdated - _sizeInitial;
		sizeCalc.cx += m_cxLeftBorder + m_cxRightBorder;
		sizeCalc.cy += m_cyTopBorder + m_cyBottomBorder;
	}

	return sizeCalc;
}

void CExtToolControlBar::_SizeToolBar(int nLength, BOOL bVert)
{
	if( m_bPaletteMode )
		return;
int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;
bool bMultiRowLayout = OnQueryMultiRowLayout();
	if( !(		IsFloating()
			||	bMultiRowLayout
			)
		)
	{
		_WrapToolBarH(
			bVert ? 32767 : nLength,
			bVert ? nLength : 32767
			);
		return;
	}
CSize sizeDefButton = _GetDefButtonSize();
	if( bMultiRowLayout )
	{
		if( bVert )
		{
			int nLengthCurr = nLength - sizeDefButton.cy;
			if( nLengthCurr < sizeDefButton.cy )
				nLengthCurr = sizeDefButton.cy;
			for( ; true;  )
			{
				_WrapToolBarV( nLengthCurr );
				CSize _size = _CalcSize( TRUE );
				if( _size.cy <= nLength )
					break;
				nLengthCurr -= sizeDefButton.cy / 2;
				if( nLengthCurr <= 0 )
					break;
			}
		} // if( bVert )
		else
			_WrapToolBarH( nLength );
		return;
	} // if( bMultiRowLayout )
INT nMinExtent = sizeDefButton.cx / 2;
	ASSERT( nMinExtent > 0 );
	if( bVert )
	{
		CExtPaintManager::monitor_parms_t _mp;
		CExtPaintManager::stat_GetMonitorParms( _mp, this );
		INT nCalcExtentMax = _mp.m_rcWorkArea.Width() + nMinExtent;
		// NOTES:
		// 1) bug was found by Eugene Pl.
		// 2) there is no reason to make toolbar wider
		//    then nCalcExtentMax value
		// 3) loop is locked when toolbar button icons are higher
		//    then 16px and user drags top/bottom border of a
		//    floating toolbar with 1-row layout (and attempting
		//    to decrease toolbar height)
		CSize _size( 32767, 32767 );
		for(	INT nCalcExtent = nMinExtent;
				nLength < _size.cy;
				nCalcExtent += nMinExtent
				)
		{
			_WrapToolBarH( nCalcExtent );
			_size = _CalcSize( FALSE );
			if( nCalcExtent > nCalcExtentMax )
				return;
		}
		_WrapToolBarH( _size.cx );
		return;
	}
INT nCountOfRowsDesired = _WrapToolBarH( nLength );
INT nCountOfRowsCurrent = _WrapToolBarH( nMinExtent );
	if( nCountOfRowsCurrent == nCountOfRowsDesired
		|| nMinExtent >= nLength
		)
	{
		_WrapToolBarH( _CalcSize(FALSE).cx );
		return;
	}
	while( nMinExtent < nLength )
	{
		INT nCurrentExtent = (nMinExtent + nLength) / 2;
		nCountOfRowsCurrent = _WrapToolBarH( nCurrentExtent );
		if( nCountOfRowsCurrent == nCountOfRowsDesired )
		{
			nLength = nCurrentExtent;
			continue;
		}
		if( nMinExtent == nCurrentExtent )
		{
			_WrapToolBarH( nLength );
			break;
		}
		nMinExtent = nCurrentExtent;
	}
	_WrapToolBarH( _CalcSize(FALSE).cx );
}

void CExtToolControlBar::_DraggingUpdateState(
	const CPoint & point,
	bool bForceFloatMode
	)
{
	CExtControlBar::_DraggingUpdateState( point, bForceFloatMode );
//	if( IsFloating() )
//		return;
//	if(		_IsDockSiteCustomizeMode()
//		&&	(! IsSingleOnRow() )
//		)
//		MakeSingleOnRow();
}

int CExtToolControlBar::_WrapToolBarV(
	int nHeight
	)
{
	ASSERT( !m_bPaletteMode );
//	ASSERT( !IsFloating() );

CClientDC dc( this );
CFont* pOldFont = (CFont*)
		dc.SelectObject( &g_PaintManager->m_FontNormalVert );
	ASSERT( pOldFont != NULL );
CSize sizeDefButton = _GetDefButtonSize();
CRect rcClient;
	GetClientRect( &rcClient );
CPoint ptCurrent( rcClient.right, rcClient.top );
    if( m_pRightBtn != NULL )
        nHeight -=
			m_pRightBtn->CalculateLayout(
				dc, sizeDefButton, FALSE ).cy;
CExtBarButton::e_wrap_t _evt = GetBtnWrapCode();
int nCountOfColumns = 1;
int nCountOfButtons = _GetButtonsCountImpl();
CExtBarButton * pPrevButton = NULL;
INT nRowDistance = g_PaintManager->FixedBar_GetRowDistance( this );
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		pTBB->SetWrap( _evt, false );
		if( nBtnIdx == (nCountOfButtons-1)
			&& m_pRightBtn != NULL
			)
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT( m_pRightBtn == pTBB );
			break;
		}
		if( pTBB->GetStyle() & TBBS_HIDDEN )
			continue;
		CSize sizeTBB =
			pTBB->CalculateLayout(
				dc,
				sizeDefButton,
				FALSE
				);
		if( ptCurrent.y == 0 && pTBB->IsSeparator() )
			sizeTBB = CSize(0, 0);
		if(	pPrevButton != NULL
			&&	( (ptCurrent.y + sizeTBB.cy) > nHeight )
			&&	(! pTBB->IsSeparator() )
			)
		{
			ASSERT_VALID( pPrevButton );
			pPrevButton->SetWrap( _evt );
			ptCurrent.x -=
				sizeTBB.cx
				+ nRowDistance;
			ptCurrent.y = 0;
			nCountOfColumns++;
		}
		ptCurrent.y += sizeTBB.cy;
		pPrevButton = pTBB;
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	dc.SelectObject( pOldFont );
	return nCountOfColumns;
}

int CExtToolControlBar::_WrapToolBarH(int nWidth, int nHeight /*= 32767*/)
{
bool bVerticallyDocked =
		((m_dwStyle & CBRS_ORIENT_HORZ) == 0)
			? true : false;
bool bFloatLikeMode =
		(	IsFloating()
		||	nHeight == 32767 // (+ v.2.21)
		||	OnQueryMultiRowLayout()
		) ? true : false;
CClientDC dc( this );
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			(		bVerticallyDocked
				&&	(!m_bPaletteMode)
			)	? ( &g_PaintManager->m_FontNormalVert )
				: ( &g_PaintManager->m_FontNormal )
			);
	ASSERT( pOldFont != NULL );
CSize sizeDefButton = _GetDefButtonSize();
CRect rcClient;
	GetClientRect( &rcClient );
CPoint ptCurrent( 0, rcClient.top );
    if(		(!bFloatLikeMode)
		&&	(!bVerticallyDocked)
		&&	m_pRightBtn != NULL
		&&	(!m_bPaletteMode)
		)
        nWidth -=
			m_pRightBtn->CalculateLayout(
				dc, sizeDefButton, TRUE ).cx;
CExtBarButton::e_wrap_t _evt = GetBtnWrapCode();
int nCountOfRows = 1;
int nCountOfButtons = _GetButtonsCountImpl();
CExtBarButton * pPrevButton = NULL;
INT nRowDistance = g_PaintManager->FixedBar_GetRowDistance( this );
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( !m_bPaletteMode )
			pTBB->SetWrap( _evt, false );
		if( nBtnIdx == (nCountOfButtons-1)
			&& m_pRightBtn != NULL
			)
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT( m_pRightBtn == pTBB );
			break;
		}

		if( pTBB->GetStyle() & TBBS_HIDDEN )
			continue;
		
		CSize sizeTBB =
			pTBB->CalculateLayout(
				dc,
				sizeDefButton,
				(!bVerticallyDocked)
					|| m_bPaletteMode
				);
		if( ptCurrent.x == 0 && pTBB->IsSeparator() )
			sizeTBB = CSize(0, 0);
		if(		(	m_bPaletteMode
				&&	pPrevButton != NULL
				&&	pPrevButton->IsWrap( _evt )
				)
			||	(	(!m_bPaletteMode)
				&&	bFloatLikeMode
				&&	pPrevButton != NULL
				&&	(bFloatLikeMode || ptCurrent.y + sizeTBB.cy < nHeight)
				&&	ptCurrent.x + sizeTBB.cx > nWidth
				&&	(!pTBB->IsSeparator())
				)
			)
		{
			ASSERT_VALID( pPrevButton );
			if( !m_bPaletteMode )
				pPrevButton->SetWrap( _evt );
			ptCurrent.x = 0;
			ptCurrent.y +=
				sizeTBB.cy
				+ nRowDistance;
			nCountOfRows++;
		}
		ptCurrent.x += sizeTBB.cx;
		pPrevButton = pTBB;
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	dc.SelectObject( pOldFont );
	return nCountOfRows;
}

CSize CExtToolControlBar::_CalcDesiredMinOuterSize( BOOL bHorz )
{
	ASSERT_VALID( this );
	if( m_bPaletteMode )
		return CalcFixedLayout( FALSE, bHorz );
CSize sizeDefButton = _GetDefButtonSize();
CSize _size( 0, 0 );
	if( m_pRightBtn != NULL ) 
	{
		CClientDC dc( this );
		CFont * pOldFont =
			dc.SelectObject(
				(		(!bHorz)
					&&	(!m_bPaletteMode)
				)	? ( &g_PaintManager->m_FontNormalVert )
					: ( &g_PaintManager->m_FontNormal )
				);
		CSize sizeTBBRight =
			m_pRightBtn->CalculateLayout(
				dc,
				sizeDefButton,
				bHorz //|| m_bPaletteMode
				);
		_size += sizeTBBRight;
		if( bHorz )
			_size.cx += __EXT_TB_BUTTON_INNER_MARGIN*2;
		else
			_size.cy += __EXT_TB_BUTTON_INNER_MARGIN*2;
		dc.SelectObject( pOldFont );
	}
//	if( IsFloating() )
//		return _size;

int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons > 0 )
	{
		CSize _sizeAdjust( 0, 0 );
		CClientDC dc( this );
		CFont * pOldFont =
			dc.SelectObject(
				(		(!bHorz)
					&&	(!m_bPaletteMode)
				)	? ( &g_PaintManager->m_FontNormalVert )
					: ( &g_PaintManager->m_FontNormal )
				);
		for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			
			if( pTBB->IsSeparator() )
				continue;
			if( pTBB->GetStyle() & TBBS_HIDDEN )
				continue;

			if(		nBtnIdx == (nCountOfButtons-1)
				&&	m_pRightBtn != NULL
				)
			{
				ASSERT_VALID( m_pRightBtn );
				ASSERT( m_pRightBtn == pTBB );
				break;
			}
			CSize sizeTBB(
				pTBB->CalculateLayout(
					dc,
					sizeDefButton,
					bHorz
					)
				);
			if( bHorz )
			{
				if( _sizeAdjust.cx > 0 )
					_sizeAdjust.cx = min( _sizeAdjust.cx, sizeTBB.cx );
				else
					_sizeAdjust = sizeTBB;
			}
			else
			{
				if( _sizeAdjust.cy > 0 )
					_sizeAdjust.cy = min( _sizeAdjust.cy, sizeTBB.cy );
				else
					_sizeAdjust = sizeTBB;
			}
		} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		if( bHorz )
		{
			if( _sizeAdjust.cx > 0 )
				_size.cx += _sizeAdjust.cx + __EXT_TB_BUTTON_INNER_MARGIN*2;
		}
		else
		{
			if( _sizeAdjust.cy > 0 )
				_size.cy += _sizeAdjust.cy + __EXT_TB_BUTTON_INNER_MARGIN*2;
		}
		dc.SelectObject( pOldFont );
	} // if( nCountOfButtons > 0 )
	
	return _size;
}

bool CExtToolControlBar::_GetFullRowMode() const
{
	if( m_bHelperTempFullRowMode )
		return true;
	return CExtControlBar::_GetFullRowMode();
}

bool CExtToolControlBar::OnQueryMultiRowLayout() const
{
	ASSERT_VALID( this );
	if(		m_bPresubclassDialogMode
		||	m_pDockSite == NULL
		||	m_bPaletteMode
		)
		return false;
	if( IsFloating() )
		return false;
	if( !_IsDockSiteCustomizeMode() )
		return false;
	return true;
}

CExtControlBar::InternalAffixmentData *
	CExtToolControlBar::_AffixmentGetOuter()
{
	ASSERT_VALID( this );
	return &m_AffixmentData;
}

void CExtToolControlBar::_AffixmentSetOuter(
	CExtControlBar::InternalAffixmentData * pAffixmentData
	)
{
	ASSERT_VALID( this );
	ASSERT( pAffixmentData != NULL );
	m_AffixmentData = *pAffixmentData;
}

bool CExtToolControlBar::_AffixmentIsAlignedHW() const
{
	ASSERT_VALID( this );
	if( m_bPaletteMode )
		return false;
	return true;
}

LRESULT CExtToolControlBar::_ContinueHelpHitTestMode(
	CExtToolControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT( pBar->GetSafeHwnd() != NULL && ::IsWindow(pBar->GetSafeHwnd()) );
	if(		pBar->m_pDockSite == NULL
		||	(! pBar->m_pDockSite->m_bHelpMode )
		)
		return -3L;
static bool g_bContinuedMode = false;
	if( g_bContinuedMode )
		return -3L;
HWND hWndDockSite = pBar->m_pDockSite->m_hWnd;
HWND hWndBar = pBar->m_hWnd;
MSG msg;
	g_bContinuedMode = true;
	for( LRESULT lResult = -3L; true; )
	{
		if( !PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		{
			if(!(	::IsWindow( hWndDockSite )
				&&	::IsWindow( hWndBar )
				&&	pBar->m_pDockSite->m_bHelpMode
				))
				break;
			::WaitMessage();
			continue;
		} // if( !PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		
		if( msg.hwnd != NULL )
		{
			if( msg.message == WM_LBUTTONDOWN )
			{
				CWnd * pWndPerm = CWnd::FromHandlePermanent(msg.hwnd);
				if( pWndPerm != NULL )
				{
					CExtControlBar * pTestBar =
						DYNAMIC_DOWNCAST( CExtControlBar, pWndPerm );
					if( pTestBar != NULL )
					{
						lResult = pTestBar->DoHelpHitTest( CPoint(msg.lParam) );
						if( lResult > 0 )
						{
							PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
							break;
						}
						lResult = -3L;
					} // if( pTestBar != NULL )
				} // if( pWndPerm != NULL )
			} // if( msg.message == WM_LBUTTONDOWN )
			else if( msg.message == WM_MOUSEMOVE )
			{
				CWnd * pWndPerm = CWnd::FromHandlePermanent(msg.hwnd);
				if( pWndPerm != NULL )
				{
					CExtControlBar * pTestBar =
						DYNAMIC_DOWNCAST( CExtControlBar, pWndPerm );
					if( pTestBar != NULL )
					{
						CPoint pt( (DWORD)msg.lParam );
						if( pTestBar->_OnMouseMoveMsg((UINT)msg.wParam,pt) )
						{
							PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
							continue;
						} // if( pTestBar->_OnMouseMoveMsg((UINT)msg.wParam,pt) )
					} // if( pTestBar != NULL )
				} // if( pWndPerm != NULL )
			} // else if( msg.message == WM_MOUSEMOVE )
			else if( msg.message == WM_COMMAND )
			{
				if( ((HIWORD(msg.wParam))==0) || ((HIWORD(msg.wParam))==1) )
				{
					lResult = (LRESULT)( LOWORD(msg.wParam) );
					PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
					break;
				} // if( ((HIWORD(msg.wParam))==0) || ((HIWORD(msg.wParam))==1) )
			} // else if( msg.message == WM_COMMAND )
			else if( msg.message == WM_SYSCOMMAND )
			{
				switch( msg.wParam )
				{
				case SC_SIZE:
				case SC_MOVE:
				case SC_MINIMIZE:
				case SC_MAXIMIZE:
				case SC_NEXTWINDOW:
				case SC_PREVWINDOW:
				case SC_CLOSE:
				case SC_RESTORE:
				case SC_TASKLIST:
					lResult =
						HID_BASE_COMMAND +
							ID_COMMAND_FROM_SC( msg.wParam );
					PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
					break;
				default:
					lResult = (LRESULT)( msg.wParam );
					PeekMessage(&msg,NULL,msg.message,msg.message,PM_REMOVE);
					break;
				}
				break;
			} // else if( msg.message == WM_SYSCOMMAND )
		} // if( msg.hwnd != NULL )

		if( !AfxGetApp()->PumpMessage() )
			break;
	} // for( LRESULT lResult = -3L; true; )
	g_bContinuedMode = false;
	return lResult;
}

void CExtToolControlBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle) const
{
	ASSERT_VALID(this);

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		ASSERT( FALSE );
		nID = 0;
		nStyle = 0;
		return;
	}
	ASSERT_VALID( pTBB );
	nID = pTBB->GetCmdID(false);
	nStyle = pTBB->GetStyle();
}

void CExtToolControlBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle)
{
	ASSERT_VALID(this);

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT_VALID( pTBB );
	pTBB->SetCmdID( nID );
	pTBB->SetStyle( nStyle );
	_InvalidateButton(nIndex);
	UpdateWindow();
}

void CExtToolControlBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	CExtPaintManager::stat_ExcludeChildAreas(
		*pDC,
		*this
		);
CExtBarButton::e_wrap_t _evt = GetBtnWrapCode();
CRect rcClient;
	GetClientRect( &rcClient );
CExtMemoryDC dc( pDC, &rcClient );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( dc.GetSafeHdc() != NULL )
		pDC = &dc;
	g_PaintManager->PaintControlBarClientArea(
		*pDC,
		rcClient,
		this
		);

	pDC->SetTextColor(
		g_PaintManager->GetColor(CExtPaintManager::CLR_TEXT_OUT)
		);
	pDC->SetBkMode( TRANSPARENT );
CFont * pOldFont =
		pDC->SelectObject(
			(		( m_dwStyle & CBRS_ORIENT_HORZ )
				||	m_bPaletteMode
				)
				? &g_PaintManager->m_FontNormal
				: &g_PaintManager->m_FontNormalVert
			);
	ASSERT( pOldFont != NULL );
int nCountOfButtons = _GetButtonsCountImpl();
INT nRowDistance = g_PaintManager->FixedBar_GetRowDistance( this );
bool bPaintRowBkMode = g_PaintManager->FixedBar_IsPaintRowBkMode( this );
bool bMultiRowLayout = OnQueryMultiRowLayout();
	if( bPaintRowBkMode )
	{
		CRect rcRowBk( rcClient );
		rcRowBk.bottom = rcRowBk.top;
		for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			
			if( pTBB->GetStyle() & TBBS_HIDDEN )
				continue;
			if( !pTBB->IsVisible() )
				continue;
			
			if( !pTBB->IsSeparator() )
			{
				CRect rcTBB = *pTBB;
				rcRowBk.top = min( rcRowBk.top, rcTBB.top );
				rcRowBk.bottom = max( rcRowBk.bottom, rcTBB.bottom );
			}
			if( pTBB->IsWrap(_evt) && ( m_dwStyle & CBRS_ORIENT_HORZ ) )
			{
				if(		!rcRowBk.IsRectEmpty()
					&&	pDC->RectVisible( &rcRowBk )
					)
					g_PaintManager->PaintFloatToolbarRowBk(
						*pDC,
						this,
						nBtnIdx,
						rcRowBk
						);
				rcRowBk.top = rcRowBk.bottom + 1;
			}
		} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		if(		!rcRowBk.IsRectEmpty()
			&&	pDC->RectVisible( &rcRowBk )
			)
			g_PaintManager->PaintFloatToolbarRowBk(
				*pDC,
				this,
				nBtnIdx,
				rcRowBk
				);
	} // pre-fill row backgrounds

	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		
		if( pTBB->GetStyle() & TBBS_HIDDEN )
			continue;
		if( !pTBB->IsVisible() )
			continue;
		
		CRect rcTBB = *pTBB;
		if( !pDC->RectVisible( &rcTBB ) )
			continue;
		if( !pTBB->IsSeparator() )
		{
			pTBB->Paint(
				*pDC,
				(		( m_dwStyle & CBRS_ORIENT_HORZ ) != 0
					||	m_bPaletteMode
					)
					? true : false
				);
			continue;
		} // if( !pTBB->IsSeparator() )
		bool bSeparatorHorz =
			(		( m_dwStyle & CBRS_ORIENT_HORZ ) != 0
				||	m_bPaletteMode
				)
				? true : false;
		bool bSeparatorRotate = false;
		CRect rectSeparator( rcTBB );
		if( (!bSeparatorHorz) && (!bMultiRowLayout) )
		{
			bSeparatorHorz = true;
			rcTBB.OffsetRect( 0, -rcTBB.Height() );
		}
		if( pTBB->IsWrap(_evt) )
		{
			if( bSeparatorHorz )
			{
				rectSeparator.left = rcClient.left;
				rectSeparator.right = rcClient.right;
				rectSeparator.top = rcTBB.bottom;
				rectSeparator.bottom =
					rectSeparator.top
					+ nRowDistance;
				bSeparatorRotate = true;
			} // if( bSeparatorHorz )
			else
			{
				rectSeparator.top = rcClient.top;
				rectSeparator.bottom = rcClient.bottom;
				rectSeparator.left =
					rcTBB.left
					- nRowDistance;
				rectSeparator.right =
					rectSeparator.left
					+ nRowDistance;
				bSeparatorRotate = true;
			} // else from if( bSeparatorHorz )
		} // if( pTBB->IsWrap(_evt) )
		if( nRowDistance == 0 && bSeparatorRotate )
			continue;
		if( bSeparatorRotate )
			bSeparatorHorz = !bSeparatorHorz;
		rectSeparator.DeflateRect(
			bSeparatorHorz ? 0 : 1,
			bSeparatorHorz ? 1 : 0
			);
		if(		bMultiRowLayout
			&&	m_pRightBtn != NULL
			&&	m_pRightBtn->IsVisible()
			&&	bSeparatorRotate
			)
		{
			CRect rcRightBtn = *m_pRightBtn;
			if( bSeparatorHorz )
			{
				if( ( m_dwStyle & CBRS_ORIENT_HORZ ) == 0 )
				{
					if( rectSeparator.bottom > (rcRightBtn.top-__EXT_TB_BUTTON_OUTER_MARGIN) )
						rectSeparator.bottom = (rcRightBtn.top-__EXT_TB_BUTTON_OUTER_MARGIN);
				}
			} // if( bSeparatorHorz )
			else
			{
				if( ( m_dwStyle & CBRS_ORIENT_HORZ ) != 0 )
				{
					if( rectSeparator.right > (rcRightBtn.left-__EXT_TB_BUTTON_OUTER_MARGIN) )
						rectSeparator.right = (rcRightBtn.left-__EXT_TB_BUTTON_OUTER_MARGIN);
				}
			} // else from if( bSeparatorHorz )
		}
		g_PaintManager->PaintSeparator(
			*pDC,
			rectSeparator,
			bSeparatorHorz,
			true,
			this
			);
		// DEBUG SEPARATOR AREA PAINTING:
		// pDC->FillSolidRect( &rectSeparator, RGB(0,255,0) );
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( m_pDragTargetTBB != NULL && (abs(m_nDragTargetShift)) == 1 )
	{
		CRect rcItem = *m_pDragTargetTBB;
		g_PaintManager->PaintDropDivider(
			*pDC,
			rcItem,
			(m_nDragTargetShift < 0) ? true : false,
			( ( m_dwStyle & CBRS_ORIENT_HORZ ) != 0 ) ? true : false
			);
	} // if( m_pDragTargetTBB != NULL && (abs(m_nDragTargetShift)) == 1 )
	if( m_pDragSourceTBB != NULL )
	{
		CRect rcItem = *m_pDragSourceTBB;
		g_PaintManager->PaintDragSourceRect(
			*pDC,
			rcItem
			);
	} // if( m_pDragSourceTBB != NULL )
CExtCustomizeSite * pSite = GetCustomizeSite();
	if( pSite != NULL )
	{
		CExtCustomizeCmdTreeNode * pPopupDropTargetNode =
			pSite->GetPopupDropTargetNode();
		if( pPopupDropTargetNode != NULL )
		{
			ASSERT_VALID( pPopupDropTargetNode );
			for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
			{
				CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
				ASSERT_VALID( pTBB );
				if( pTBB->GetStyle() & TBBS_HIDDEN )
					continue;
				if( !pTBB->IsVisible() )
					continue;
				if( pTBB->IsSeparator() )
					continue;
				CRect rcTBB = *pTBB;
				if( !pDC->RectVisible( &rcTBB ) )
					continue;
				CExtCustomizeCmdTreeNode * pNode =
					pTBB->GetCmdNode();
				if( ((LPVOID)pNode) != ((LPVOID)pPopupDropTargetNode) )
					continue;
				g_PaintManager->PaintDragSourceRect(
					*pDC,
					rcTBB
					);
				break;
			} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		} // if( pPopupDropTargetNode != NULL )
	} // if( pSite != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	pDC->SelectObject( pOldFont );
	g_PaintManager->OnPaintSessionComplete( this );
}

void CExtToolControlBar::_InvalidateButton(int nIndex)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT_VALID( pTBB );
CRect rect = pTBB->Rect();
	if( rect.IsRectEmpty() )
		return;
	if(		pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
		&&	g_PaintManager->GetCb2DbTransparentMode(this)
		)
	{
		CSize sizeToolBarRgnRounders =
			g_PaintManager->GetToolBarRgnRoundersSize();
		rect.InflateRect(
			sizeToolBarRgnRounders.cx + 1,
			sizeToolBarRgnRounders.cy + 1
			);
	}
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if(		pTBB == m_pDragSourceTBB
		||	pTBB == m_pDragTargetTBB
		)
	{
		CSize _sizeDDM = g_PaintManager->GetDropDividerMerics();
		rect.InflateRect( _sizeDDM );
		rect.InflateRect( 1, 1 );
	} // if( pTBB == m_pDragSourceTBB )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	InvalidateRect( &rect );
}

int CExtToolControlBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID( this );

	if(		CExtToolControlBar::g_bMenuTracking
		||	CExtPopupMenuWnd::IsMenuTracking()
		)
		return -1;

int nToolTipHit =
		((CExtToolControlBar*)this)->
			HitTest(point);
	if( nToolTipHit >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( nToolTipHit );
		ASSERT_VALID( pTBB );
		if( pTBB != NULL )
		{
			nToolTipHit = pTBB->OnToolHitTest( point, pTI );
			if( nToolTipHit != -1 )
				return nToolTipHit;
		}
	} // if( nToolTipHit >= 0 )

	nToolTipHit =
		CExtControlBar::OnToolHitTest(
			point,
			pTI
			);
	return nToolTipHit;
}

int CExtToolControlBar::_HitTestImpl(
	CPoint point,
	UINT nButtonStyleInclude, // = 0, // button must have style
	UINT nButtonStyleExclude, // = 0  // button must have not style
	bool bAllowFindNearest // = false // if did not hit - find nearest
	) const
{
	ASSERT_VALID( this );
	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) )  )
		return -1; // nowhere
	if( (!bAllowFindNearest) && ::GetCapture() == m_hWnd )
	{
		CPoint ptScreen( point );
		ClientToScreen( &ptScreen );
		if( ::WindowFromPoint(ptScreen) != m_hWnd )
			return -1; // nowhere
	}
int nCountOfButtons = _GetButtonsCountImpl();
int nNearestIdx = -1;
int nNearestDistance = 32767;
bool bNearestRowMatch = false;
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(nBtnIdx);
		ASSERT_VALID( pTBB );
		
		if( pTBB->GetStyle() & TBBS_HIDDEN )
			continue;
		if( !(pTBB->IsVisible()) )
			continue;
		
		if( (pTBB->GetStyle() & nButtonStyleInclude)
				!= nButtonStyleInclude
			)
			continue;
		if( (pTBB->GetStyle() & nButtonStyleExclude)
				!= 0
			)
			continue;
		CRect rect = *pTBB;
		if( rect.PtInRect(point) )
			return nBtnIdx;
		if( !bAllowFindNearest )
			continue;
		CPoint ptCenter = rect.CenterPoint();
		double lfDX = double( point.x - ptCenter.x );
		double lfDY = double( point.y - ptCenter.y );
		int nDistance = (int)
			::sqrt( lfDX*lfDX + lfDY*lfDY );
		bool bRowMatch = false;
		if( (m_dwStyle & CBRS_ORIENT_HORZ) != 0 )
		{
			bRowMatch =
				(rect.top <= point.y && point.y <= rect.bottom)
					? true : false;
		} // if( (m_dwStyle & CBRS_ORIENT_HORZ) != 0 )
		else
		{
			bRowMatch =
				(rect.left <= point.x && point.x <= rect.right)
					? true : false;
		} // if( (m_dwStyle & CBRS_ORIENT_HORZ) != 0 )
		if( (!bNearestRowMatch) && bRowMatch )
		{
			nNearestIdx = nBtnIdx;
			nNearestDistance = nDistance;
			bNearestRowMatch = true;
		} // if( (!bNearestRowMatch) && bRowMatch )
		else if(
				bNearestRowMatch == bRowMatch
			&&	nNearestDistance > nDistance
			)
		{
			nNearestIdx = nBtnIdx;
			nNearestDistance = nDistance;
		}
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	if( bAllowFindNearest && nNearestIdx >= 0 )
		return nNearestIdx;
	return -1; // nowhere
}

int CExtToolControlBar::HitTest(
	CPoint point // in window relative coords
	) const
{
	return
		_HitTestImpl(
			point,
			0,
			TBBS_SEPARATOR
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar message handlers

BEGIN_MESSAGE_MAP(CExtToolControlBar, CExtControlBar)
	//{{AFX_MSG_MAP(CExtToolControlBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtToolControlBar::g_nMsgTrackButtonMenu, OnTrackButtonMenu)
END_MESSAGE_MAP()

LRESULT CExtToolControlBar::OnTrackButtonMenu(WPARAM wParam,LPARAM lParam)
{
	lParam;
	if( !(	::IsWindowEnabled(
				m_bPresubclassDialogMode
					? GetOwner()->GetSafeHwnd()
					: _GetDockingFrameImpl()->GetSafeHwnd()
				)
			)
		) 
		return 0;
int nCountOfButtons = GetButtonsCount();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return 0;
int nIndex = int(wParam);
	if( nIndex < 0 || nIndex>=nCountOfButtons )
	{
		_CloseTrackingMenus();
		return 0;
	}
CExtBarButton * pTBB = _GetButtonPtr(nIndex);
	ASSERT_VALID( pTBB );
BOOL bDroppedMode =
		CExtPopupMenuWnd::IsMenuTracking();
	if( !pTBB->IsAbleToTrackMenu() )
		bDroppedMode = FALSE;
	if( bDroppedMode )
	{
		CExtToolControlBar::TrackButtonMenu( nIndex, true );
		return 0;
	}
	_CloseTrackingMenus();
	if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
		CExtMouseCaptureSink::ReleaseCapture();

	return 0;
}

void CExtToolControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonDown(nFlags, point);
		return;
	}
	
	_ActivateOnClick();

	if( (m_nBtnIdxCapture = HitTest(point)) < 0 ) // nothing hit
	{
		CExtControlBar::OnLButtonDown(nFlags, point);
		return;
	}

CExtBarButton * pTBB =
		_GetButtonPtr(m_nBtnIdxCapture);
	ASSERT_VALID( pTBB );
	ASSERT( !(pTBB->IsSeparator()) );

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtCustomizeSite * pSite = GetCustomizeSite();
	if( pSite != NULL )
	{
		bool bAlt = false;
		bool bCustomizeMode = pSite->IsCustomizeMode();
		if( !bCustomizeMode )
			bAlt =
				( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 )
					? true : false;
		if( bAlt || bCustomizeMode )
		{
			int nBtnDragSrc = m_nBtnIdxCapture;
			m_nBtnIdxCapture = -1;
			if( !pTBB->CanStartCommandDrag() )
				return;
//			CExtCmdItem * pCmdItem =
//				g_CmdManager->CmdGetPtr(
//					g_CmdManager->ProfileNameFromWnd( m_hWnd ),
//					pTBB->GetCmdID()
//					);
//			if( pCmdItem == NULL )
//			{
//				ASSERT( FALSE );
//				return;
//			}
			CExtCustomizeCmdTreeNode * pNodeC =
				pTBB->GetCmdNode( false );
			if( pNodeC == NULL )
			{
				ASSERT( FALSE );
				return;
			}
			ASSERT_VALID( pNodeC );
			CExtCustomizeCmdTreeNode * pNodeI =
				pTBB->GetCmdNode( true );
			SendMessage( WM_CANCELMODE );
			CRect rcItem( 0, 0, 1, 1 );

			CExtCustomizeCmdTreeNode * pPopupDropTargetNode =
				pSite->GetPopupDropTargetNode();
			if(		bCustomizeMode
				&&	pTBB->IsAbleToTrackMenu( true )
				&&	pPopupDropTargetNode != pNodeC
				)
			{
				pSite->DropTargetPopupCancel();
				pSite->DropTargetPopupCancel();
				CRect rcBtn = *pTBB;
				ClientToScreen( &rcBtn );
				CPoint ptTrack( point );
				ClientToScreen( &ptTrack );
				DWORD dwTrackFlags = pTBB->OnGetTrackPopupFlags();
				pSite->DropTargetPopupTrack(
					pNodeC,
					rcBtn,
					ptTrack,
					dwTrackFlags
					);
			} // if( bCustomizeMode && pTBB->IsAbleToTrackMenu() ...
			else
				pSite->DropTargetPopupCancel();

			m_pDragSourceTBB = pTBB;
			m_pDragTargetTBB = NULL;
			m_nDragTargetShift = 0;
			m_bHelperSeparatorAction = false;
			_InvalidateButton( nBtnDragSrc );
			UpdateWindow();
			pSite->DoDragCmdNode( this, pNodeI, pNodeC, rcItem );
			m_pDragSourceTBB = NULL;
			m_pDragTargetTBB = NULL;
			m_nDragTargetShift = 0;
			m_bHelperSeparatorAction = false;
			return;
		} // if( bAlt || bCustomizeMode )
	} // if( pSite != NULL )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	// update the button before checking for disabled status
	_UpdateButton(m_nBtnIdxCapture);

	if( pTBB->IsDisabled()
		|| m_nBtnIdxMenuTracking == m_nBtnIdxCapture
		)
	{
		m_nBtnIdxCapture = -1;
		return;     // don't press it
	}

	pTBB->ModifyStyle( TBBS_PRESSED );
	pTBB->OnPressedTrackingStart();
	_InvalidateButton( m_nBtnIdxCapture );
	UpdateWindow(); // immediate feedback

CWnd * pCtrlTBB = pTBB->CtrlGet();
	pTBB->OnClick( point, true );
	
	if( !g_bMenuTracking )
	{
		if( pCtrlTBB == NULL
			|| (pCtrlTBB->GetStyle() & WS_VISIBLE) == 0
			)
			CExtMouseCaptureSink::SetCapture( GetSafeHwnd() );
		else
			m_nBtnIdxCapture = -1;
	}
	else
		m_nBtnIdxCapture = -1;
}

bool CExtToolControlBar::_OnMouseMoveMsg(UINT nFlags, CPoint point)
{
	if( CExtPopupMenuWnd::IsMenuTracking()
		&& !CExtToolControlBar::g_bMenuTracking
		)
		return true;
	if( IsFloating()
		&& CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent
		)
	{
		CFrameWnd * pParentFrame = GetParentFrame();
		if( pParentFrame != NULL )
			pParentFrame->BringWindowToTop();
	}
    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
		return CExtControlBar::_OnMouseMoveMsg(nFlags, point);;
	if( m_nBtnIdxCapture >= 0 )
	{
		CExtBarButton* pTBB =
			_GetButtonPtr(m_nBtnIdxCapture);
		ASSERT_VALID( pTBB );
		ASSERT(!(pTBB->IsSeparator()));

		UINT nNewStyle = ( pTBB->GetStyle() & (~TBBS_PRESSED) );
		int nBtnIdxCapture = m_nBtnIdxCapture;
		if( CExtMouseCaptureSink::GetCapture() != m_hWnd )
		{
			m_nBtnIdxCapture = -1; // lost capture
		}
		else
		{
			// should be pressed if still hitting the captured button
			if( HitTest(point) == m_nBtnIdxCapture )
				nNewStyle |= TBBS_PRESSED;
		}
//		if( (nNewStyle & TBBS_PRESSED) == 0 )
//			pTBB->OnPressedTrackingStop();
//		else
//			pTBB->OnPressedTrackingStart();
		SetButtonStyle( nBtnIdxCapture, nNewStyle );
		UpdateWindow(); // immediate feedback
		return true;
	} // if( m_nBtnIdxCapture >= 0 )

bool bHoverChanged =
		_UpdateHoverButton( point );

	if(		CExtMouseCaptureSink::GetCapture() == GetSafeHwnd()
		&&	m_nBtnIdxHover < 0
		&&	m_nBtnIdxMenuTracking < 0
		&&	m_nBtnIdxCapture < 0
		)
	{
		CPoint ptScreen;
		VERIFY( ::GetCursorPos(&ptScreen) );
		HWND hWnd = ::WindowFromPoint(ptScreen);
		if( hWnd != m_hWnd )
			CExtMouseCaptureSink::ReleaseCapture();
	}
	if( bHoverChanged )
		return true;

	return CExtControlBar::_OnMouseMoveMsg(nFlags,point);
}

bool CExtToolControlBar::_UpdateHoverButton(
	CPoint point // = CPoint(-1,-1) // default is use ::GetCursorPos()
	)
{
	if( point.x < 0 || point.y < 0 )
	{
		VERIFY( ::GetCursorPos(&point) );
		ScreenToClient( &point );
	}

bool bHoverChanged = false;
int nBtnIdxHoverNew =
		(	CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND( GetSafeHwnd() )
			|| CExtPopupMenuWnd::IsMenuTracking()
		) ? HitTest(point) : (-1);

int nBtnIdxHoverOld =  m_nBtnIdxHover;
	if( nBtnIdxHoverOld != nBtnIdxHoverNew )
	{
		bHoverChanged = true;
		m_nBtnIdxHover = -1;
		if( nBtnIdxHoverOld >= 0 )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(nBtnIdxHoverOld);
			ASSERT_VALID( pTBB );
			pTBB->SetHover( false );
			_InvalidateButton( nBtnIdxHoverOld );
			UpdateWindow();
			pTBB->OnHover( point, false );
		}
		if(		CExtControlBar::FindHelpMode(this)
			&&	nBtnIdxHoverNew >= 0
			)
		{
			CExtBarButton * pTBB = _GetButtonPtr(nBtnIdxHoverNew);
			if(		pTBB->IsAbleToTrackMenu()
				&&	pTBB->OnHelpHitTest(point) == -3
				)
				m_nBtnIdxHover = nBtnIdxHoverNew;
		}
		else
			m_nBtnIdxHover = nBtnIdxHoverNew;
		if( m_nBtnIdxHover >= 0 )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nBtnIdxHover);
			ASSERT_VALID( pTBB );
			bool bEnableHoverOnNewBtn = true;
			if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton))
				|| pTBB->IsDisabled()
				)
			{
				if( g_bMenuTracking )
					bEnableHoverOnNewBtn = false;
			}
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
			if( bEnableHoverOnNewBtn )
			{
				CExtCustomizeSite * pSite =
					GetCustomizeSite();
				if(		pSite != NULL
					&&	(	pSite->IsCustomizeMode()
						||	pSite->GetPopupDropTargetNode() != NULL
						||	pSite->DraggedNodeGet() != NULL
						)
					)
					bEnableHoverOnNewBtn = false;
			} // if( bEnableHoverOnNewBtn )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
			if( bEnableHoverOnNewBtn )
			{
				pTBB->SetHover();
				_InvalidateButton( m_nBtnIdxHover );
				UpdateWindow();
				pTBB->OnHover( point, true );
			}
		}
	} // if( nBtnIdxHoverOld != nBtnIdxHoverNew )
	return bHoverChanged;
}

void CExtToolControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( (m_nBtnIdxCapture < 0 && m_nBtnIdxHover < 0)
		|| m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonUp(nFlags, point);
		return;     // not captured
	}

int nBtnIdxHover = m_nBtnIdxHover;
int nBtnIdxCapture = m_nBtnIdxCapture;

CExtBarButton * pHoverTBB = NULL;
	if( m_nBtnIdxHover >= 0 )
	{
		pHoverTBB =
			_GetButtonPtr( m_nBtnIdxHover );
		ASSERT_VALID( pHoverTBB );
		m_nBtnIdxHover = -1;
	} // if( m_nBtnIdxHover >= 0 )

	if( m_nBtnIdxCapture >= 0 )
	{
		CExtBarButton * pTempTBB = _GetButtonPtr( nBtnIdxCapture );
		ASSERT_VALID( pTempTBB );
		ASSERT( !(pTempTBB->IsSeparator()) );
		if( pTempTBB->IsPressedTracking() )
		{
			pTempTBB->OnPressedTrackingStop();
			_InvalidateButton( m_nBtnIdxCapture );
		}
		m_nBtnIdxCapture = -1;
		CExtBarButton * pCaptureTBB = NULL;
		UINT nNewStyle = 0;
		if( nBtnIdxCapture == HitTest(point) )
		{
			pCaptureTBB = pTempTBB;
			nNewStyle = ( pCaptureTBB->GetStyle() & ~TBBS_PRESSED );
			// give button a chance to update
			_UpdateButton( nBtnIdxCapture );
			// then check for disabled state
			if( !(pCaptureTBB->IsDisabled()) )
			{
				if( pCaptureTBB->GetStyle() & TBBS_CHECKBOX )
				{
					// auto check: three state => down
					if( nNewStyle & TBBS_INDETERMINATE )
						nNewStyle &= ~TBBS_INDETERMINATE;
					nNewStyle ^= TBBS_CHECKED;
				}
			} // if( !(pCaptureTBB->IsDisabled()) )
		} // if( nBtnIdxCapture == HitTest(point) )
		CExtMouseCaptureSink::ReleaseCapture();

		if( pCaptureTBB != NULL )
			pCaptureTBB->OnClick( point, false );

		if( !(::IsWindow(GetSafeHwnd())) )
			return;
		if( !g_bMenuTracking )
		{
			SetButtonStyle( nBtnIdxCapture, nNewStyle );
			_UpdateButton( nBtnIdxCapture );
		} // if( bUpdateBtnState )
	} // if( m_nBtnIdxCapture >= 0 )

	if( pHoverTBB != NULL )
	{
		ASSERT_VALID( pHoverTBB );
		pHoverTBB->SetHover( false );
		pHoverTBB->OnHover( point, false );
		_InvalidateButton( nBtnIdxHover );
	} // if( pHoverTBB != NULL )

	UpdateWindow(); // immediate feedback
}

void CExtToolControlBar::OnRButtonUp(UINT nFlags, CPoint point)
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( _IsDockSiteCustomizeMode() )
	{
		CExtCustomizeSite * pSite = GetCustomizeSite();
		if( pSite != NULL )
		{
			ASSERT( pSite->IsCustomizeMode() );
			INT nButton = HitTest( point );
			if( nButton < 0 )
				return;
			CExtBarButton * pTBB = _GetButtonPtr( nButton );
			ASSERT_VALID( pTBB );
			if(		pTBB->IsSeparator()
				||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
				||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
				||	pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton))
				)
				return;
			CExtCustomizeCmdTreeNode * pNodeC = pTBB->GetCmdNode( false );
			if( pNodeC == NULL )
				return;
			ASSERT_VALID( pNodeC );
			CExtCustomizeCmdTreeNode * pNodeI = pTBB->GetCmdNode( true );
#ifdef _DEBUG
			if( pNodeI != NULL )
			{
				ASSERT_VALID( pNodeI );
			}
#endif // _DEBUG
			pSite->DropTargetPopupCancel();
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(m_hWnd),
					pTBB->GetCmdID( false )
					);
			ASSERT( pCmdItem != NULL );
			if( !pSite->CustomizedNodeSet( pNodeC ) )
				return;
			Invalidate();
			UpdateWindow();
			bool bCanAddRemoveSeparator = false;
			if(		nButton > 0
				&&	(! _GetButtonPtr(nButton-1)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
				)
				bCanAddRemoveSeparator = true;
			bool bDelete = false;
			if( !pSite->OnCustomizeTreeNode(
					pNodeI,
					pNodeC,
					pCmdItem,
					bDelete,
					bCanAddRemoveSeparator,
					this
					)
				)
			{
				VERIFY( pSite->CustomizedNodeSet( NULL ) );
				Invalidate();
				UpdateWindow();
				return;
			}
			VERIFY( pSite->CustomizedNodeSet( NULL ) );
			if( bDelete )
			{
				pNodeC->RemoveSelf( pCmdItem );
				RemoveButton( nButton );
				if(		nButton > 0
					&&	_GetButtonPtr(nButton-1)->IsSeparator()
					)
					RemoveButton( nButton-1 );
				if(		nButton < (GetButtonsCount()-1)
					&&	_GetButtonPtr(nButton)->IsSeparator()
					)
				{
					RemoveButton( nButton );
					CExtBarButton * pTBB = _GetButtonPtr(nButton);
					ASSERT_VALID( pTBB );
					CExtCustomizeCmdTreeNode * pNodeNext = pTBB->GetCmdNode( false );
					ASSERT_VALID( pNodeNext );
					pNodeNext->ModifyFlags( 0, __ECTN_GROUP_START );
				}
			} // if( bDelete )
			else
			{
				if( pNodeC->GetFlags() & __ECTN_GROUP_START )
				{
					if(		nButton > 0
						&&	(! _GetButtonPtr(nButton-1)->IsSeparator() )
						)
						InsertButton( nButton, ID_SEPARATOR, FALSE );
				} // if( pNodeC->GetFlags() & __ECTN_GROUP_START )
				else
				{
					if(		nButton > 0
						&&	_GetButtonPtr(nButton-1)->IsSeparator()
						)
						RemoveButton( nButton-1, FALSE );
				} // else from if( pNodeC->GetFlags() & __ECTN_GROUP_START )
			} // else from if( bDelete )
			if( IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
				((CExtMenuControlBar*)this)->UpdateMenuBar();
			if( m_pDockSite == NULL )
			{
				_RecalcPositionsImpl();
				Invalidate();
				UpdateWindow();
				return;
			} // if( m_pDockSite == NULL )
			ASSERT_VALID( m_pDockSite );
			if( IsFloating() )
			{
				_RecalcLayoutImpl();
				Invalidate();
				UpdateWindow();
			}
			else
				m_pDockSite->RecalcLayout();
			return;
		} // if( pSite != NULL )
	} // if( _IsDockSiteCustomizeMode() )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtControlBar::OnRButtonUp(nFlags,point);
}

void CExtToolControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonDblClk(nFlags, point);
		return;
	}
int nBtnIdx = _HitTestImpl(point);
	if( nBtnIdx >= 0 )
		return;
	CExtControlBar::OnLButtonDblClk(nFlags,point);
}

void CExtToolControlBar::OnCancelMode()
{
	CExtControlBar::OnCancelMode();

//	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );

bool bUpdateState = false;
	if( m_nBtnIdxCapture >= 0 )
	{
		CExtBarButton* pTBB =
			_GetButtonPtr(m_nBtnIdxCapture);
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );

		if( m_nBtnIdxMenuTracking != m_nBtnIdxCapture )
		{
			UINT nNewStyle = (pTBB->GetStyle() & ~TBBS_PRESSED);
			SetButtonStyle(m_nBtnIdxCapture, nNewStyle);
		}
		pTBB->OnPressedTrackingStop();

		m_nBtnIdxCapture = -1;
		bUpdateState = true;
	}
	if( m_nBtnIdxHover >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(m_nBtnIdxHover);
		ASSERT_VALID( pTBB );
		pTBB->SetHover( false );
		m_nBtnIdxHover = -1;
		bUpdateState = true;
		CPoint point;
		VERIFY( ::GetCursorPos(&point) );
		ScreenToClient( &point );
		pTBB->OnHover( point, false );
	}

	if( bUpdateState )
	{
		if( CExtMouseCaptureSink::GetCapture() == GetSafeHwnd() )
			CExtMouseCaptureSink::ReleaseCapture();
		Invalidate();
		UpdateWindow();
	}
}

void CExtToolControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CExtControlBar::OnSettingChange(uFlags,lpszSection);

	if( m_bPresubclassDialogMode )
	{
		_RecalcLayoutImpl();
		return;
	}

CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	pFrame->DelayRecalcLayout();
}

bool CExtToolControlBar::IsRightExpandButton( int nBtnIdx )
{
CExtBarButton * pTBB =
		_GetButtonPtr( nBtnIdx );
	if( pTBB == NULL )
		return false;
	ASSERT_VALID( pTBB );
	if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
		return true;
	return false;
}

void CExtToolControlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
CWnd * pTargetUpdateWnd = pTarget;
	if( !m_bPresubclassDialogMode )
	{
		CExtControlBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

		//if( CExtPopupMenuWnd::IsMenuTracking() )
		//	return;
		if( _DraggingGetBar() != NULL )
			return;
	} // if( !m_bPresubclassDialogMode )
	else
	{
		if( pTargetUpdateWnd != NULL 
			&& pTargetUpdateWnd->IsKindOf( RUNTIME_CLASS(CFrameWnd) )
			)
			return;
		//ASSERT( pTargetUpdateWnd == NULL );
//		if( pTargetUpdateWnd == NULL )
			pTargetUpdateWnd = GetOwner();
		if( pTargetUpdateWnd == NULL )
			pTargetUpdateWnd = GetParent();
		ASSERT( pTargetUpdateWnd != NULL );
	}

	if( pTargetUpdateWnd == NULL )
		return;

	ASSERT_VALID( pTargetUpdateWnd );

	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;

int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		pTBB->OnUpdateCmdUI(
			pTargetUpdateWnd,
			bDisableIfNoHndler,
			nBtnIdx
			);
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTargetUpdateWnd, bDisableIfNoHndler);
}

void CExtToolControlBar::_UpdateButton(int nIndex)
{
CWnd * pTarget = GetOwner();
	if( pTarget == NULL && (!m_bPresubclassDialogMode) )
		pTarget = GetParentFrame();

	if( pTarget == NULL )
		return;
BOOL bDisableIfNoHandler = TRUE;
	if( pTarget->IsKindOf( RUNTIME_CLASS(CFrameWnd) ) )
		bDisableIfNoHandler = ((CFrameWnd *)pTarget)->m_bAutoMenuEnable;

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT_VALID( pTBB );
	pTBB->OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHandler,
		nIndex
		);
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar diagnostics

#ifdef _DEBUG
void CExtToolControlBar::AssertValid() const
{
	CExtControlBar::AssertValid();
}

void CExtToolControlBar::Dump(CDumpContext& dc) const
{
	CExtControlBar::Dump(dc);
}
#endif

// input CRect should be client rectangle size
void CExtToolControlBar::_CalcInsideRect(CRect& rect, BOOL bHorz) const
{
	ASSERT_VALID(this);
	CControlBar::CalcInsideRect(rect,bHorz);
}

void CExtToolControlBar::_RecalcPositionsImpl()
{
	if( GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
		return;
	ASSERT_VALID(this);

int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;

CExtBarButton::e_wrap_t _evt = GetBtnWrapCode();
BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) ? TRUE : FALSE;
BOOL bFloating = IsFloating();
bool bMultiRowLayout = OnQueryMultiRowLayout();

CRect rcInner;
	GetClientRect( &rcInner );
	//rcInner.DeflateRect( __EXT_TB_BUTTON_INNER_MARGIN, __EXT_TB_BUTTON_INNER_MARGIN );
	rcInner.DeflateRect(
		m_cxLeftBorder,
		m_cyTopBorder,
		m_cxRightBorder,
		m_cyBottomBorder
		);
CPoint ptLimitTL = rcInner.TopLeft();
CPoint ptLimitBR = rcInner.BottomRight();

CClientDC dc(this);
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			(bHorz || m_bPaletteMode )
				? ( &g_PaintManager->m_FontNormal )
				: ( &g_PaintManager->m_FontNormalVert )
			);
	ASSERT( pOldFont != NULL );

CSize sizeDefButton = _GetDefButtonSize();
int nReviewCount = nCountOfButtons;
CSize sizeTBBRight( 0, 0 );
	if( m_pRightBtn != NULL )
	{
		nReviewCount--;
		ASSERT_VALID( m_pRightBtn );
		ASSERT( m_buttons.GetSize() > 0 );
		ASSERT( m_buttons[nReviewCount] == m_pRightBtn );
		m_pRightBtn->GetButtons().RemoveAll();
		m_pRightBtn->ClearContent();
		sizeTBBRight =
			m_pRightBtn->CalculateLayout(
				dc,
				sizeDefButton,
				bHorz || m_bPaletteMode
				);
		if( bFloating || m_bPaletteMode )
		{
			m_pRightBtn->Show( false );
			m_pRightBtn->SetRect(
				CRect( ptLimitTL, sizeTBBRight )
				);
			if( nReviewCount == 0 )
				return;
		} // if( bFloating || m_bPaletteMode )
		else
		{
			CRect rcCalcRightBtn(
				bHorz ? (ptLimitBR.x - sizeTBBRight.cx) : ptLimitTL.x,
				bHorz ? ptLimitTL.y : (ptLimitBR.y - sizeTBBRight.cy),
				ptLimitBR.x,
				ptLimitBR.y
				);
			m_pRightBtn->SetRect( rcCalcRightBtn );
			m_pRightBtn->Show( true );
			CSize sizeRounders =
				g_PaintManager->GetToolBarRgnRoundersSize();
			if( bHorz )
				rcInner.right -=
					sizeTBBRight.cx
					+ __EXT_TB_BUTTON_INNER_MARGIN
					+ sizeRounders.cx
					;
			else
				rcInner.bottom -=
					sizeTBBRight.cy
					+ __EXT_TB_BUTTON_INNER_MARGIN
					+ sizeRounders.cy
					;
		} // else from if( bFloating || m_bPaletteMode )
	} // if( m_pRightBtn != NULL )

CArray < CRect, CRect >	arrBtnRects;
CArray < BOOL, BOOL >	arrBtnVisibility;
CArray < BOOL, BOOL >	arrBtnSeparators;
//CArray < BOOL, BOOL >	arrBtnEnabled;
	arrBtnRects.SetSize( nReviewCount );
	arrBtnVisibility.SetSize( nReviewCount );
	arrBtnSeparators.SetSize( nReviewCount );
//	arrBtnEnabled.SetSize( nReviewCount );
CSize sizeLastWrappedRow( 0, 0 );
bool bDockerTrasparentMode =
		g_PaintManager->GetCb2DbTransparentMode( this );
	if( (!bFloating) && bMultiRowLayout )
	{
		if( bHorz && bDockerTrasparentMode )
			ptLimitTL.y += 1;
		else if( !bHorz )
		{
			if( bDockerTrasparentMode )
				ptLimitBR.x -= 2;
			else
				ptLimitBR.x -= 1;
			ptLimitTL.y -= 1;
		}
	} // if( (!bFloating) && bMultiRowLayout )
CPoint ptBtnPosCurr( ptLimitTL );
INT nRowExtent = sizeDefButton.cy;
	if( bMultiRowLayout && (!bHorz) )
	{
		ptBtnPosCurr.x = ptLimitBR.x;
		nRowExtent = sizeDefButton.cx;
	}
BOOL bSeparatorPrev = FALSE, bPostResyncAllowed = FALSE;
INT nRowDistance = g_PaintManager->FixedBar_GetRowDistance( this );
	for( int nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		BOOL bVisibleTBB =
			( pTBB->GetStyle() & TBBS_HIDDEN ) ? FALSE : TRUE;
		arrBtnVisibility.SetAt( nBtnIdx, bVisibleTBB );
		if( !bVisibleTBB )
			continue;
		pTBB->SetVertDocked( (bHorz || m_bPaletteMode) ? false : true );
//		arrBtnEnabled.SetAt( nBtnIdx, pTBB->IsEnabled() );
		BOOL bSeparator = pTBB->IsSeparator();
		arrBtnSeparators.SetAt( nBtnIdx, bSeparator );
		if( (bSeparatorPrev && bSeparator) || (bSeparator && nBtnIdx == 0) )
		{ // remove double separators
			arrBtnVisibility.SetAt( nBtnIdx, FALSE );
			continue;
		} // remove double separators

		CSize sizeTBB =
			pTBB->CalculateLayout(
				dc,
				sizeDefButton,
				bHorz || m_bPaletteMode
				);
		
		CRect rcTBB( ptBtnPosCurr, sizeTBB );
		if( bMultiRowLayout && (!bHorz) )
			nRowExtent = max( nRowExtent, sizeTBB.cx );
		else
			nRowExtent = max( nRowExtent, sizeTBB.cy );

		if( (bFloating || bMultiRowLayout || m_bPaletteMode)
				&& pTBB->IsWrap(_evt)
			)
		{
			if( bMultiRowLayout && (!bHorz) )
			{
				ptBtnPosCurr.y = ptLimitTL.y;
				ptBtnPosCurr.x -=
					nRowExtent
					+ nRowDistance;
				sizeLastWrappedRow = CSize( 0, 0 );
				nRowExtent = sizeDefButton.cx;
			} // if( bMultiRowLayout && (!bHorz) )
			else
			{
				ptBtnPosCurr.x = ptLimitTL.x;
				ptBtnPosCurr.y +=
					nRowExtent // sizeDefButton.cy (+ v.2.21)
					+ nRowDistance;
				sizeLastWrappedRow = CSize( 0, 0 );
				nRowExtent = sizeDefButton.cy;
			} // else from if( bMultiRowLayout && (!bHorz) )
		} // if( (bFloating || ...
		else
		{
			bool bResyncVisibleTBB = false;
			if( bHorz || m_bPaletteMode )
			{
				ptBtnPosCurr.x += sizeTBB.cx;
				if(		(!bMultiRowLayout)
					&&	ptBtnPosCurr.x > rcInner.right
					)
					bResyncVisibleTBB = true;
			} // if( bHorz || m_bPaletteMode )
			else
			{
				ptBtnPosCurr.y += sizeTBB.cy;
				if(		(!bMultiRowLayout)
					&&	ptBtnPosCurr.y > rcInner.bottom
					)
					bResyncVisibleTBB = true;
			} // else from if( bHorz || m_bPaletteMode )
			if( bResyncVisibleTBB )
			{
				int nSpaceToFind =
					( bHorz || m_bPaletteMode )
						? ptBtnPosCurr.x - rcInner.right
						: ptBtnPosCurr.y - rcInner.bottom;
				int nBtnWithNearestWidth = nBtnIdx;
				int nNearestMetric =
					( bHorz || m_bPaletteMode )
						? rcTBB.Width() : rcTBB.Height();
				int nNearestDiff = nSpaceToFind - nNearestMetric;
				// hide nearest by size reviewed visible
				for( int nBtnIdx2 = nBtnIdx-1; nBtnIdx2 >= 0; nBtnIdx2-- )
				{
					if( !arrBtnVisibility[nBtnIdx2] )
						continue;
					if( arrBtnSeparators[nBtnIdx2] )
						continue;

#ifdef _DEBUG
					CExtBarButton * pTBB2 =
						_GetButtonPtr( nBtnIdx2 );
					ASSERT( (pTBB2->GetStyle() & TBBS_HIDDEN) == 0 );
#endif // _DEBUG

					int nMetric =
						( bHorz || m_bPaletteMode )
							? arrBtnRects[nBtnIdx2].Width()
							: arrBtnRects[nBtnIdx2].Height();
					//if( nMetric <= nNearestMetric ) // <<-- widest search algorithm
					//	continue;
					if( nMetric > nSpaceToFind )
						continue;
					int nDiff = nSpaceToFind - nMetric;
//					if( !arrBtnEnabled[nBtnIdx2] )
//					{ // hide disabled buttons first
//						nNearestDiff = nDiff;
//						nNearestMetric = nMetric;
//						nBtnWithNearestWidth = nBtnIdx2;
//						break;
//					} // hide disabled buttons first
					if( nDiff >= nNearestDiff )
						continue;
					nNearestDiff = nDiff;
					nNearestMetric = nMetric;
					nBtnWithNearestWidth = nBtnIdx2;
				} // for( int nBtnIdx2 = nBtnIdx-1; nBtnIdx2 >= 0; nBtnIdx2-- )
				if( nBtnWithNearestWidth < nBtnIdx )
				{
					ASSERT( bVisibleTBB );
					arrBtnVisibility.SetAt( nBtnWithNearestWidth, FALSE );
					if( (!m_bPaletteMode) && (!bMultiRowLayout) && (!IsFloating()) )
						bPostResyncAllowed = TRUE;
					if( m_pRightBtn!=NULL && !arrBtnSeparators[nBtnWithNearestWidth] )
					{
						CExtBarButton * pTBB2 =
							_GetButtonPtr( nBtnWithNearestWidth );
						ASSERT_VALID( pTBB2 );
						m_pRightBtn->GetButtons().Add( pTBB2 );
					}
					if( nBtnWithNearestWidth > 0 && nBtnWithNearestWidth < nReviewCount-1 )
					{ // remove double separators
						if(		arrBtnSeparators[nBtnWithNearestWidth-1]
							&&	arrBtnSeparators[nBtnWithNearestWidth+1]
							)
						{
							arrBtnVisibility.SetAt( nBtnWithNearestWidth-1, FALSE );
							nNearestMetric +=
								( bHorz || m_bPaletteMode )
									? arrBtnRects[nBtnWithNearestWidth-1].Width()
									: arrBtnRects[nBtnWithNearestWidth-1].Height();
						}
					} // remove double separators
					rcTBB.OffsetRect(
						( bHorz || m_bPaletteMode ) ? -nNearestMetric : 0,
						( bHorz || m_bPaletteMode ) ? 0 : -nNearestMetric
						);
					( ( bHorz || m_bPaletteMode ) ? ptBtnPosCurr.x : ptBtnPosCurr.y) -= nNearestMetric;
					for( nBtnIdx2 = nBtnWithNearestWidth+1; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
					{
						if( !arrBtnVisibility[nBtnIdx2] )
							continue;
						CRect rcTBB2 = arrBtnRects[nBtnIdx2];
						rcTBB2.OffsetRect(
							( bHorz || m_bPaletteMode ) ? -nNearestMetric : 0,
							( bHorz || m_bPaletteMode ) ? 0 : -nNearestMetric
							);
						arrBtnRects.SetAt( nBtnIdx2, rcTBB2 );
					} // for( nBtnIdx2 = nBtnWithNearestWidth+1; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
				} // if( nBtnWithNearestWidth < nBtnIdx )
				else
				{
					ASSERT( nBtnWithNearestWidth == nBtnIdx );
					bVisibleTBB = FALSE;
					if( (!m_bPaletteMode) && (!bMultiRowLayout) && (!IsFloating()) )
						bPostResyncAllowed = TRUE;
					( (bHorz || m_bPaletteMode ) ? ptBtnPosCurr.x : ptBtnPosCurr.y) -=
						( (bHorz || m_bPaletteMode ) ? sizeTBB.cx : sizeTBB.cy);
				} // else from if( nBtnWithNearestWidth < nBtnIdx )
			} // if( bResyncVisibleTBB )
		} // else from if( (bFloating || ...

		arrBtnVisibility.SetAt( nBtnIdx, bVisibleTBB );
		if( bMultiRowLayout && (!bHorz) )
		{
			CRect rcTBB2( rcTBB );
			rcTBB2.OffsetRect( -rcTBB.Width(), 0 );
			arrBtnRects.SetAt( nBtnIdx, rcTBB2 );
		}
		else
			arrBtnRects.SetAt( nBtnIdx, rcTBB );
		if( bVisibleTBB )
			bSeparatorPrev = bSeparator;

		if(		m_pRightBtn != NULL
			&&	(!bMultiRowLayout)
			&&	(!bVisibleTBB)
			&&	(!bSeparator)
			)
			m_pRightBtn->GetButtons().Add( pTBB );
	} // for( int nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )

	dc.SelectObject( pOldFont );

	if( bPostResyncAllowed )
	{
		ASSERT( !IsFloating() );
		ASSERT( !m_bPaletteMode );
		ASSERT( !bMultiRowLayout );
		INT nVisIdx0 = -1, nVisIdx1 = -1;
		for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
		{
			if( ! arrBtnVisibility[nBtnIdx] )
				continue;
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
				break;
			if( nVisIdx0 < 0 )
				nVisIdx0 = nBtnIdx;
			nVisIdx1 = nBtnIdx;
		} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
		if( nVisIdx0 >= 0 )
		{
			ASSERT( nVisIdx1 >= nVisIdx0 );
			INT nShiftMetric = 0;
			for( ; nVisIdx0 <= nVisIdx1; nVisIdx0++ )
			{
				if( ! arrBtnVisibility[nVisIdx0] )
					break;
				CExtBarButton * pTBB = _GetButtonPtr( nVisIdx0 );
				ASSERT_VALID( pTBB );
				ASSERT( !pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) );
				if( !pTBB->IsSeparator() )
					break;
				arrBtnVisibility[nVisIdx0] = FALSE;
				if( bHorz )
					nShiftMetric -= arrBtnRects[nVisIdx0].Width();
				else
					nShiftMetric -= arrBtnRects[nVisIdx0].Height();
			}
			for( ; nVisIdx1 >= nVisIdx0; nVisIdx1-- )
			{
				if( ! arrBtnVisibility[nVisIdx1] )
					break;
				CExtBarButton * pTBB = _GetButtonPtr( nVisIdx1 );
				ASSERT_VALID( pTBB );
				ASSERT( !pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) );
				if( !pTBB->IsSeparator() )
					break;
				arrBtnVisibility[nVisIdx1] = FALSE;
			}
			if( nShiftMetric != 0 )
			{
				for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
				{
					if( ! arrBtnVisibility[nBtnIdx] )
						continue;
					CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
					ASSERT_VALID( pTBB );
					if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
						break;
					arrBtnRects.ElementAt( nBtnIdx ).OffsetRect(
						bHorz ? nShiftMetric : 0,
						bHorz ? 0 : nShiftMetric
						);
				} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
			} // if( nShiftMetric != 0 )
		} // if( nVisIdx0 >= 0 )
	} // if( bPostResyncAllowed )

INT nRowStart = 0;
INT nSizeRow = 0;
int nSubRowCount = 0;
bool bReAlignHorz =
		(	bHorz
		||	bFloating
		||	m_bPaletteMode
		) ? true : false;
	if( bMultiRowLayout && (!bHorz) )
		bReAlignHorz = false;
	for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		//BOOL bVis = arrBtnVisibility[nBtnIdx];
		//if( bVis )
		//{
			CSize sizeTBB = *pTBB; // arrBtnRects[nBtnIdx].Size();
			INT nSizeBtn = bReAlignHorz ? sizeTBB.cy : sizeTBB.cx;
			nSizeRow = max( nSizeRow, nSizeBtn );
		//} // if( bVis )
		if(	( (bFloating || bMultiRowLayout || m_bPaletteMode ) && pTBB->IsWrap(_evt) )
			|| nBtnIdx == nReviewCount-1
			)
		{
			for( INT nBtnIdx2 = nRowStart; nBtnIdx2 <= nBtnIdx; nBtnIdx2++ )
			{
				CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx2 );
				ASSERT_VALID( pTBB );
				BOOL bVis = arrBtnVisibility[nBtnIdx2];
				pTBB->Show( bVis ? true : false );
				if( !bVis )
					continue;
				CRect rcBtn = arrBtnRects[nBtnIdx2];
				if( pTBB->IsSeparator() )
				{
					if( bReAlignHorz )
						rcBtn.bottom = rcBtn.top + nSizeRow;
					else
						rcBtn.left = rcBtn.right - nSizeRow;
				} // if( pTBB->IsSeparator() )
				else
				{
					if( bReAlignHorz )
						rcBtn.OffsetRect(
							0,
							( nSizeRow - rcBtn.Height() ) / 2
							);
					else 
						rcBtn.OffsetRect(
							- ( nSizeRow - rcBtn.Width() ) / 2,
							0
							);
				} // else from if( pTBB->IsSeparator() )
				if( bFloating || bMultiRowLayout )
					rcBtn.OffsetRect( 0, 1 );
				pTBB->SetRect( rcBtn );
			}
			nRowStart = nBtnIdx + 1;
			nSizeRow = 0;
			nSubRowCount++;
		}
	} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )

	if( nSubRowCount == 1 && (!m_bPaletteMode) && (!bFloating) && (!bMultiRowLayout) )
	{
		CRect rcBarClient;
		GetClientRect( &rcBarClient );
		nSizeRow = bHorz
			? rcBarClient.Height()
			: rcBarClient.Width()
			;
		for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			BOOL bVis = arrBtnVisibility[nBtnIdx];
			if( !bVis )
				continue;
			CRect rcBtn( pTBB->Rect() );
			if( pTBB->IsSeparator() )
			{
				if( bHorz )
				{
					rcBtn.top = m_cyTopBorder;
					rcBtn.bottom = m_cyBottomBorder + nSizeRow;
				} // if( bHorz )
				else
				{
					rcBtn.left = m_cxLeftBorder;
					rcBtn.right = m_cxRightBorder + nSizeRow;
				} // else from if( bHorz )
			} // if( pTBB->IsSeparator() )
			else
			{
				rcBtn.OffsetRect(
					bHorz
						? 0
						: ( ( nSizeRow - rcBtn.Width() ) / 2  - rcBtn.left)
					,
					bHorz
						? ( ( nSizeRow - rcBtn.Height() ) / 2 - rcBtn.top) 
						: 0
					);
			} // else from if( pTBB->IsSeparator() )
			pTBB->SetRect( rcBtn );
		} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	} // if( nSubRowCount == 1 && (!m_bPaletteMode) && (!bFloating) && (!bMultiRowLayout) )
	else if( !m_bPaletteMode )
	{ // just re-align separators in each row
		INT nRowMinLoc = 0, nRowMaxLoc = 0;
		INT nRowStart = 0;
		for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			if( pTBB->IsVisible() )
			{
				CRect rcTBB = *pTBB;
				if( nBtnIdx == nRowStart )
				{
					if( bHorz )
					{
						nRowMinLoc = rcTBB.top;
						nRowMaxLoc = rcTBB.bottom;
					} // if( bHorz )
					else
					{
						nRowMinLoc = rcTBB.left;
						nRowMaxLoc = rcTBB.right;
					} // else from if( bHorz )
				} // if( nBtnIdx == nRowStart )
				else
				{
					if( bHorz )
					{
						nRowMinLoc = min( nRowMinLoc, rcTBB.top );
						nRowMaxLoc = max( nRowMaxLoc, rcTBB.bottom );
					} // if( bHorz )
					else
					{
						nRowMinLoc = min( nRowMinLoc, rcTBB.left );
						nRowMaxLoc = max( nRowMaxLoc, rcTBB.right );
					} // else from if( bHorz )
				} // else from if( nBtnIdx == nRowStart )
			} // if( pTBB->IsVisible() )
			if(		(! pTBB->IsWrap(_evt) )
				&&	nBtnIdx != (nReviewCount-1)
				)
				continue;
			for( INT nBtnIdx2 = nRowStart; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
			{
				CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx2 );
				ASSERT_VALID( pTBB );
				if(		(! pTBB->IsVisible() )
					||	(! pTBB->IsSeparator() )
					)
					continue;
				CRect rcTBB = *pTBB;
				if( bHorz )
				{
					rcTBB.top = nRowMinLoc;
					rcTBB.bottom = nRowMaxLoc;
					rcTBB.InflateRect(
						0,
						bDockerTrasparentMode ? 2 : 1
						);
				} // if( bHorz )
				else
				{
					rcTBB.left = nRowMinLoc;
					rcTBB.right = nRowMaxLoc;
					rcTBB.InflateRect(
						bDockerTrasparentMode ? 2 : 1,
						0,
						bDockerTrasparentMode ? 3 : 2,
						0
						);
				} // else from if( bHorz )
				pTBB->SetRect( rcTBB );
			} // for( INT nBtnIdx2 = nRowStart; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
			nRowStart = nBtnIdx + 1;
		} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	} // just re-align separators in each row

	if(	m_pRightBtn != NULL && m_bPresubclassDialogMode )
	{
		if( m_pRightBtn->GetButtons().GetSize() == 0 )
			m_pRightBtn->ModifyStyle( TBBS_DISABLED, 0 );
		else
			m_pRightBtn->ModifyStyle( 0, TBBS_DISABLED );
	}

	// insert separators into right button
	if(		bFloating
		||	bMultiRowLayout
		||	m_bPaletteMode
		||	bMultiRowLayout
		||	m_pRightBtn == NULL
		||	(	m_pRightBtn != NULL
				&& m_pRightBtn->GetButtons().GetSize() == 0
			)
		)
	{
		return;
	}

int nHiddenCount = m_pRightBtn->GetButtons().GetSize();
	if( nHiddenCount < 2 )
		return;
int nEndMeasure = nHiddenCount-1;
	for( int iHidden = 0; iHidden < nEndMeasure; iHidden++ )
	{
		CExtBarButton * pTbbHidden0 =
			m_pRightBtn->GetButtons().GetAt(iHidden);
		ASSERT( pTbbHidden0 != NULL );
		ASSERT( ! pTbbHidden0->IsSeparator() );
		ASSERT( ! pTbbHidden0->IsVisible() );
		ASSERT( (pTbbHidden0->GetStyle() & TBBS_HIDDEN) == 0 );
		CExtBarButton * pTbbHidden1 =
			m_pRightBtn->GetButtons().GetAt(iHidden+1);
		ASSERT( pTbbHidden1 != NULL );
		ASSERT( ! pTbbHidden1->IsSeparator() );
		ASSERT( ! pTbbHidden1->IsVisible() );
		ASSERT( (pTbbHidden1->GetStyle() & TBBS_HIDDEN) == 0 );
		CExtBarButton * pTbbSeparatorToInsert = NULL;
		int nIdx0=-1,nIdx1=-1;
		for( nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			if( pTBB == pTbbHidden0 )
			{
				nIdx0 = nBtnIdx;
				ASSERT( nIdx1 < 0 );
				continue;
			}
			if( nIdx0 >= 0
				&& pTBB->IsSeparator()
				&& pTbbSeparatorToInsert == NULL
				)
				pTbbSeparatorToInsert = pTBB;
			if( pTBB == pTbbHidden1 )
			{
				nIdx1 = nBtnIdx;
				ASSERT( nIdx0 >= 0 && nIdx0 < nIdx1 );
				break;
			}
		}
		ASSERT(
			nIdx0 < nIdx1
			&&
			nIdx0 >= 0 && nIdx0 < nCountOfButtons
			&& 
			nIdx1 >= 0 && nIdx1 < nCountOfButtons
			);
		if( (nIdx0+1) == nIdx1 )
			continue;
		if( pTbbSeparatorToInsert != NULL )
		{
			nEndMeasure++;
			iHidden++;
			m_pRightBtn->GetButtons().InsertAt(
				iHidden,
				pTbbSeparatorToInsert
				);
		}
	} // for( int iHidden = 0; iHidden < nEndMeasure; iHidden++ )
}

DWORD CExtToolControlBar::RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout)
{
DWORD dwRes = CControlBar::RecalcDelayShow( lpLayout );
	if( (!IsFloating()) && IsVisible() )
		_RecalcPositionsImpl();
	return dwRes;
}

void CExtToolControlBar::OnSize(UINT nType, int cx, int cy) 
{
	CExtControlBar::OnSize(nType, cx, cy);
	_RecalcLayoutImpl();
}

void CExtToolControlBar::_RecalcLayoutImpl()
{
	if( GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
		return;
	CExtControlBar::_RecalcLayoutImpl();
	_RecalcPositionsImpl();
	if( m_bPresubclassDialogMode )
	{
		Invalidate();
		UpdateWindow();
		return;
	}
	if(		m_pDockSite != NULL
		&&	m_pDockBar != NULL
		&&	OnQueryMultiRowLayout()
		&&	m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))
		)
		((CExtDockOuterBar*)m_pDockBar)->_DelayDockSiteRecalc();
}

LRESULT CExtToolControlBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_WINDOWPOSCHANGED )
		_RecalcLayoutImpl();
	return CExtControlBar::WindowProc(message,wParam,lParam);
}


void CExtToolControlBar::OnTimer(UINT nIDEvent) 
{
	CExtControlBar::OnTimer(nIDEvent);
}

void CExtToolControlBar::_SwitchMenuTrackingIndex(
	int iNewMenuTrackingIndex  // = -1
	)
{
	if( m_nBtnIdxMenuTracking >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( m_nBtnIdxMenuTracking );
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );
		UINT nNewStyle =
			pTBB->GetStyle()
			&
			~(TBBS_PRESSED|TBBS_CHECKED|TBBS_INDETERMINATE);
		pTBB->SetStyle( nNewStyle );
		pTBB->OnPressedTrackingStop();
		_InvalidateButton( m_nBtnIdxMenuTracking );
		m_nBtnIdxMenuTracking = -1;
	} // if( m_nBtnIdxMenuTracking >= 0 )

	if( iNewMenuTrackingIndex >= 0 )
	{
		m_nBtnIdxMenuTracking = iNewMenuTrackingIndex;
		CExtBarButton * pTBB =
			_GetButtonPtr( m_nBtnIdxMenuTracking );
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );
		UINT nNewStyle =
			pTBB->GetStyle()
			&
			~(TBBS_CHECKED|TBBS_INDETERMINATE);
		nNewStyle |= TBBS_PRESSED;
		pTBB->SetStyle( nNewStyle );
		pTBB->OnPressedTrackingStart();
		_InvalidateButton( m_nBtnIdxMenuTracking );
	} // if( iNewMenuTrackingIndex >= 0 )

	if(m_hWnd)
		UpdateWindow();
}

CExtToolControlBar * CExtToolControlBar::_GetMenuTrackingBar()
{
CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
CExtToolControlBar * pRetVal = NULL;
	for( INT iBar=0; iBar<g_AllBars.GetSize(); ++iBar )
	{
		CExtControlBar * pBar = g_AllBars[iBar];
		ASSERT_VALID( pBar );
		CExtToolControlBar * pToolControlBar =
			DYNAMIC_DOWNCAST(CExtToolControlBar,pBar);
		if( pToolControlBar == NULL )
			continue;
		if( pToolControlBar->m_nBtnIdxMenuTracking >= 0 )
		{
			pRetVal = pToolControlBar;
			break;
		}
	}
	_slCsCB.Unlock();
	return NULL;
}

void CExtToolControlBar::_CloseTrackingMenus()
{
	g_bMenuTracking = false;

CSingleLock _slCsCB( &g_csCB );
	_slCsCB.Lock();
	for( INT iBar=0; iBar<g_AllBars.GetSize(); ++iBar )
	{
		CExtControlBar * pBar = g_AllBars[iBar];
		ASSERT_VALID( pBar );
		CExtToolControlBar * pToolControlBar =
			DYNAMIC_DOWNCAST(CExtToolControlBar,pBar);
		if( pToolControlBar == NULL )
			continue;
		pToolControlBar->_SwitchMenuTrackingIndex();
	}
	_slCsCB.Unlock();

//	CExtPopupMenuWnd::CancelMenuTracking();
//	CWnd::CancelToolTips();

	if( !CExtPopupMenuSite::g_DefPopupMenuSite.IsEmpty()
		&& !CExtPopupMenuSite::g_DefPopupMenuSite.IsShutdownMode()
		)
		CExtPopupMenuSite::g_DefPopupMenuSite->PostMessage( WM_CANCELMODE );
	CWnd::CancelToolTips();

// 	TO FIX: can be tracked other menu
//	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
}

void CExtToolControlBar::_CbPaintCombinedContent(
	LPVOID pCookie,
	CDC & dc,
	const CWnd & refWndMenu,
	const CRect & rcExcludeArea, // in screen coords
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	pCookie;
	dc;
	refWndMenu;
	rcExcludeArea;
	eCombineAlign;
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( refWndMenu.GetSafeHwnd() != NULL );
CExtToolControlBar * pBar = (CExtToolControlBar *)pCookie;
	ASSERT_VALID( pBar );
	ASSERT( pBar->IsKindOf(RUNTIME_CLASS(CExtToolControlBar)) );
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;
	if( pBar->m_nBtnIdxMenuTracking < 0 )
		return;
	ASSERT(
		pBar->m_nBtnIdxMenuTracking <
			pBar->GetButtonsCount()
		);

CExtBarButton * pTBB =
		pBar->_GetButtonPtr(
			pBar->m_nBtnIdxMenuTracking
			);
	ASSERT_VALID( pTBB );

	if(		g_PaintManager->GetCb2DbTransparentMode(pTBB)
		&&	pTBB->IsKindOf( RUNTIME_CLASS(CExtBarContentExpandButton) )
		&&	(! pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
		&&	(! pBar->m_bPresubclassDialogMode )
		&&	CExtPaintManager::stat_GetBPP() > 8
		)
		return;

UINT nStyle = pTBB->GetStyle();
bool bHover = pTBB->IsHover();
	pTBB->SetHover( false );
	pTBB->ModifyStyle(
		0,
		TBBS_PRESSED|TBBS_CHECKED|TBBS_CHECKBOX
		);
CRect rcClientBar,rcClientExcludeArea;
	pBar->GetClientRect( &rcClientBar );
	pBar->ClientToScreen( &rcClientBar );
	refWndMenu.GetClientRect( &rcClientExcludeArea );
	refWndMenu.ClientToScreen( &rcClientExcludeArea );
CPoint ptOffset =
		rcClientBar.TopLeft()
		- rcClientExcludeArea.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
bool bHorz = pBar->IsDockedVertically() ? false : true;
	pTBB->Paint(dc,bHorz);
	dc.SetViewportOrg( ptViewportOrg );
	pTBB->SetHover( bHover );
	pTBB->SetStyle(nStyle);
}

CExtBarButton * CExtToolControlBar::GetButton( int nIndex )
{
	return _GetButtonPtr( nIndex );
}

CExtBarContentExpandButton * CExtToolControlBar::GetRightButton()
{
	return m_pRightBtn;
}

void CExtToolControlBar::OnCaptureChanged(CWnd *pWnd) 
{
	if(	(m_nBtnIdxCapture >= 0 || m_nBtnIdxHover >= 0)
		&& pWnd != this
		)
		SendMessage( WM_CANCELMODE );
	CExtControlBar::OnCaptureChanged(pWnd);
}

BOOL CExtToolControlBar::InitContentExpandButton()
{
	if( m_pRightBtn != NULL )
		return TRUE;
	m_pRightBtn = OnCreateBarRightBtn();
	if( m_pRightBtn == NULL )
		return FALSE;
	ASSERT_VALID( m_pRightBtn );
	ASSERT_KINDOF( CExtBarContentExpandButton, m_pRightBtn );
	m_buttons.Add( m_pRightBtn );
	return TRUE;
}

bool CExtToolControlBar::_CanDockToInnerCircles() const
{
	return false;
}

bool CExtToolControlBar::_CanDockToTabbedContainers() const
{
	return false;
}

void CExtToolControlBar::ToggleDocking()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );

	ASSERT_VALID( m_pDockBar );
#ifdef _DEBUG
	if( !IsFloating() )
	{
		ASSERT_KINDOF( CExtDockBar, m_pDockBar );
	}
#endif // _DEBUG
	ASSERT( m_pDockContext != NULL );
	
	m_pDockContext->ToggleDocking();

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	pFrame->DelayRecalcLayout();
	_RecalcNcArea();
}


CExtBarButton::e_wrap_t CExtToolControlBar::GetBtnWrapCode() const
{
	ASSERT_VALID( this );
	if(		m_bPresubclassDialogMode
		||	GetSafeHwnd() == NULL
		||	( ! ::IsWindow(GetSafeHwnd()) )
		||	IsFloating()
		)
		return CExtBarButton::e_wrap_t::__EVT_FLOAT;
	if( IsDockedVertically() )
		return CExtBarButton::e_wrap_t::__EVT_VERT;
	return CExtBarButton::e_wrap_t::__EVT_HORZ;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

bool CExtToolControlBar::_IsDockSiteCustomizeMode() const
{
CExtCustomizeSite * pSite = GetCustomizeSite();
	if( pSite == NULL )
//		return CExtControlBar::_IsDockSiteCustomizeMode();
		return false;
	return pSite->IsCustomizeMode();
}

CExtCustomizeSite * CExtToolControlBar::GetCustomizeSite()
{
	ASSERT_VALID( this );
	return m_pCustomizeSite;
}

CExtCustomizeSite * CExtToolControlBar::GetCustomizeSite() const
{
	ASSERT_VALID( this );
	return ((CExtToolControlBar*)this)->GetCustomizeSite();
}

bool CExtToolControlBar::OnCustomizeRegisterBar(
	CExtCustomizeSite * pCustomizeSite
	)
{
	ASSERT_VALID( this );
	ASSERT( m_pCustomizeSite == NULL ); // should called only once
	ASSERT( pCustomizeSite != NULL );
	if(		(! m_bCustomizationAllowed )
		||	m_bPaletteMode
		)
		return false;
	m_pCustomizeSite = pCustomizeSite;
	return true;
}

void CExtToolControlBar::OnCustomizeGetBarInfo(
	CExtCustomizeSite::CToolBarCustomizeInfo * pInfo
	)
{
	ASSERT_VALID( this );
	ASSERT( m_pCustomizeSite != NULL );
	ASSERT( pInfo != NULL );
	ASSERT( m_hWnd != NULL && ::IsWindow(m_hWnd) );

UINT nBarID = (UINT)GetDlgCtrlID();
CExtCmdItem * pCmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(m_hWnd),
			nBarID
			);
	if( pCmd != NULL )
		pInfo->m_strDisplayName =
			pCmd->OnGetToolBarCustomizeName();
	if( pInfo->m_strDisplayName.IsEmpty() )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		CExtLocalResourceHelper _LRH;
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		if( !pInfo->m_strDisplayName.LoadString(nBarID) )
		{
//#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
//			ASSERT( FALSE );
//#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			GetWindowText( pInfo->m_strDisplayName );
			if( pInfo->m_strDisplayName.IsEmpty() )
				pInfo->m_strDisplayName.Format(
					_T("Toolbar #%08X"),
					nBarID
					);
		} // if( !pInfo->m_strDisplayName.LoadString(nBarID) )
	} // if( pInfo->m_strDisplayName.IsEmpty() )
	if( pInfo->m_pHelperBarButtonsI != NULL )
	{
		CExtCustomizeSite * pCustomizeSite = GetCustomizeSite();
		ASSERT( pCustomizeSite != NULL );
		ASSERT_VALID( pInfo->m_pHelperBarButtonsI );
		ASSERT( pInfo->m_pHelperBarButtonsI->GetCmdID() == ((UINT)GetDlgCtrlID()) );
		ASSERT( pInfo->m_pHelperBarButtonsI->GetNodeCount() == 0 );
		pInfo->m_pHelperBarButtonsI->SetTextUser( pInfo->m_strDisplayName );
		INT nCountOfButtons = _GetButtonsCountImpl();
		for( INT nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			pTBB->OnCustomizeNodeInit(
				pCustomizeSite,
				pInfo->m_pHelperBarButtonsI
				);
		} // for( INT nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	} // if( pInfo->m_pHelperBarButtonsI != NULL )
}

void CExtToolControlBar::OnCustomizeUpdateInitialBarState(
	CExtCustomizeCmdTreeNode * pParentNode,
	bool bRecalcLayout
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pParentNode );
	ASSERT( m_pCustomizeSite != NULL );
INT nNodeCount = pParentNode->GetNodeCount();
INT nCountOfButtons = _GetButtonsCountImpl();
	ASSERT( nNodeCount <= nCountOfButtons );
INT nNodePos = 0;
	for( INT nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		CExtCustomizeCmdTreeNode * pCmdNodeI =
			pTBB->GetCmdNode( true );
		if( pCmdNodeI == NULL )
			continue;
		ASSERT( !pTBB->IsSeparator() );
		ASSERT_VALID( pCmdNodeI );
		CExtCustomizeCmdTreeNode * pCmdNodeC =
			pParentNode->ElementAt( nNodePos++ );
		ASSERT_VALID( pCmdNodeC );
//		pTBB->SetCustomizedCmdNode( pCmdNodeC );
		
		// re-init spec buttons
		pTBB =
			m_pCustomizeSite->OnCreateToolbarButton(
				this,
				pCmdNodeI,
				pCmdNodeI
				);
		ASSERT_VALID( pTBB );
		VERIFY( InsertSpecButton( nBtnIdx, pTBB, FALSE ) );
		pTBB->SetBasicCmdNode( pCmdNodeI );
		pTBB->SetCustomizedCmdNode( pCmdNodeC );
		pTBB->OnCustomizeUpdateProps();
		RemoveButton( nBtnIdx + 1 );

	} // for( INT nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	if( bRecalcLayout )
		_RecalcLayoutImpl();
}

void CExtToolControlBar::OnCustomizeModeEnter()
{
	ASSERT_VALID( this );
	if(		m_pDockSite == NULL
		||	m_pDockBar == NULL
		||	(! m_pDockBar->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar)) )
		)
		return;
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

CExtCustomizeSite * pSite = GetCustomizeSite();
	if( pSite != NULL )
		pSite->RegisterCommandDropTarget( this, this );

	if(		(! IsDocked() )
		||	_GetFullRowMode()
		)
		return;
INT nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;
	if( m_pRightBtn == NULL )
		return;
	if( m_pRightBtn->GetButtons().GetSize() == 0 )
		return;
INT nOwnIdxInDocker = m_pDockBar->FindBar(this);
	ASSERT( 0 < nOwnIdxInDocker && nOwnIdxInDocker < (m_pDockBar->m_arrBars.GetSize()-1) );
	if( m_pDockBar->m_arrBars[nOwnIdxInDocker+1] != NULL )
		m_pDockBar->m_arrBars.InsertAt( nOwnIdxInDocker+1, (CControlBar *)NULL );
	if( m_pDockBar->m_arrBars[nOwnIdxInDocker-1] != NULL )
		m_pDockBar->m_arrBars.InsertAt( nOwnIdxInDocker, (CControlBar *)NULL );
	Invalidate();
}

void CExtToolControlBar::OnCustomizeModeLeave()
{
	ASSERT_VALID( this );
}

void CExtToolControlBar::OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pWndList );
	CExtControlBar::OnCustomizeListBoxUpdate( pWndList );
}

DROPEFFECT CExtToolControlBar::OnCustomizeTargetOver(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DWORD dwKeyState
	)
{
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );
	_dragInfo;
	if(		m_pDockBar != NULL
		&&	IsFloating()
		)
		GetParentFrame()->SetWindowPos(
			&CWnd::wndTop, 0, 0, 0, 0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE
			);

//CExtCustomizeSite * pSite = GetCustomizeSite();
//	ASSERT( pSite != NULL );
//CExtCustomizeCmdTreeNode * pPopupDropTargetNode =
//		pSite->GetPopupDropTargetNode();
//	if(		pSite->IsCustomizeMode()
//		&&	m_pDragTargetTBB != NULL
//		&&	m_pDragTargetTBB->IsAbleToTrackMenu()
//		&&	pPopupDropTargetNode == m_pDragTargetTBB->GetCmdNode()
//		)
//		pSite->DropTargetPopupCancel();

int nCountTest = _GetButtonsCountImpl();
int nCountReal = nCountTest;
	if( m_pRightBtn != NULL )
	{
		nCountTest--;
		ASSERT( nCountTest >= 0 );
	}
	if(		nCountTest > 0
		&&	_GetButtonPtr(0)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
		)
	{
		nCountTest--;
		ASSERT( nCountTest >= 0 );
	}
	if(		nCountReal > 1
		&&	_GetButtonPtr(
				( m_pRightBtn != NULL )
					? (nCountReal-2)
					: (nCountReal-1)
				)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
		)
	{
		nCountTest--;
		ASSERT( nCountTest >= 0 );
	}
	if( nCountTest == 0 )
	{
		if( (dwKeyState & MK_CONTROL) != NULL )
			return DROPEFFECT_COPY;
		return DROPEFFECT_MOVE;
	}

	if( m_pDragTargetTBB != NULL )
		_InvalidateButton( _GetIndexOf(m_pDragTargetTBB) );
int nHitIdx = _HitTestImpl( point, 0, 0, true );
	if( nHitIdx < 0 )
	{
		m_pDragTargetTBB = NULL;
		m_nDragTargetShift = 0;
		UpdateWindow();
		return DROPEFFECT_NONE;
	}
CExtBarButton * pHitTBB = _GetButtonPtr( nHitIdx );
	ASSERT_VALID( pHitTBB );
	if( pHitTBB->IsSeparator() )
	{
		nHitIdx--;
		if( nHitIdx < 0 )
		{
			m_pDragTargetTBB = NULL;
			m_nDragTargetShift = 0;
			UpdateWindow();
			return DROPEFFECT_NONE;
		}
		pHitTBB = _GetButtonPtr( nHitIdx );
		ASSERT_VALID( pHitTBB );
		if( pHitTBB->IsSeparator() )
		{
			ASSERT( FALSE ); // double  separators?!
			m_pDragTargetTBB = NULL;
			m_nDragTargetShift = 0;
			UpdateWindow();
			return DROPEFFECT_NONE;
		} // if( pHitTBB->IsSeparator() )
	} // if( pHitTBB->IsSeparator() )
	m_nDragTargetShift =
		pHitTBB->CalculateDropOverEffect( point, _dragInfo );
	if( m_nDragTargetShift == 0 )
	{
		m_pDragTargetTBB = NULL;
		m_nDragTargetShift = 0;
		UpdateWindow();
		return DROPEFFECT_NONE;
	} // if( m_nDragTargetShift == 0 )
	if( m_pDragTargetTBB != pHitTBB )
	{
		m_pDragTargetTBB = pHitTBB;
		_InvalidateButton( _GetIndexOf(m_pDragTargetTBB) );
	}
	UpdateWindow();
	if( (dwKeyState & MK_CONTROL) != NULL )
		return DROPEFFECT_COPY;
	return DROPEFFECT_MOVE;
}

void CExtToolControlBar::OnCustomizeTargetLeave()
{
	ASSERT_VALID( this );
	if( m_pDragTargetTBB == NULL )
		return;
	_InvalidateButton( _GetIndexOf(m_pDragTargetTBB) );
	m_pDragTargetTBB = NULL;
	m_nDragTargetShift = 0;
	UpdateWindow();
}

bool CExtToolControlBar::OnCustomizeTargetDrop(
	CExtCustomizeSite::CCmdDragInfo & _dragInfo,
	CPoint point,
	DROPEFFECT de
	)
{
	// returns true if drop with any effect
	// has been performed
	ASSERT_VALID( this );
	ASSERT( !_dragInfo.IsEmpty() );
	_dragInfo;
	point;
	ASSERT(
			de == DROPEFFECT_NONE
		||	de == DROPEFFECT_MOVE
		||	de == DROPEFFECT_COPY
		);
	if(		de == DROPEFFECT_NONE
//		||	m_pDragTargetTBB == NULL
//		||	m_nDragTargetShift == 0
		)
		return false;
	if( m_pDragTargetTBB == NULL )
	{
		CExtCustomizeSite * pSite =
			GetCustomizeSite();
		ASSERT( pSite != NULL );
		int nInsertPos = 0;
		if(		GetButtonsCount() > 0
			&&	_GetButtonPtr(0)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
			)
			nInsertPos = 1;
		pSite->OnInsertToolbarCmdNode(
			_dragInfo,
			this,
			nInsertPos,
			true
			);
		if(		m_pDockBar != NULL
			&&	IsFloating()
			)
		{
			GetParentFrame()->RecalcLayout();
			Invalidate();
		}
		return true;
	} // if( m_pDragTargetTBB == NULL )

	ASSERT_VALID( m_pDragTargetTBB );
	
int nBtn = _GetIndexOf(m_pDragTargetTBB);
	ASSERT( 0 <= nBtn && nBtn < _GetButtonsCountImpl() );
	_InvalidateButton( nBtn );

bool bSeparatorAdd = false;
bool bSeparatorRemove = false;
	if( de == DROPEFFECT_MOVE )
	{
		if( m_pDragSourceTBB == m_pDragTargetTBB )
		{
			if( m_nDragTargetShift == -2 )
				return false; // no reinsert
			if( m_nDragTargetShift < 0 )
			{ // if remove separator
				if(		nBtn > 0
					&&	_GetButtonPtr(nBtn-1)->IsSeparator()
					)
					bSeparatorRemove = true;
				else
					return false; // no reinsert
			} // if remove separator
			else
			{ // if add separator
				if(	(	nBtn > 1
					||	(	nBtn == 1
						&&	(! _GetButtonPtr(0)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
						)
					)
					&&	(! _GetButtonPtr(nBtn-1)->IsSeparator() )
					)
					bSeparatorAdd = true;
				else
					return false; // no reinsert
			} // if add separator
		} // if( m_pDragSourceTBB == m_pDragTargetTBB )
		else if(
			m_pDragSourceTBB != NULL
			&&	nBtn > 0
			&&	m_nDragTargetShift < 0
			&&	_GetIndexOf(m_pDragSourceTBB) == (nBtn-1)
			&&	(!(	nBtn == 2
				&&	_GetButtonPtr(0)->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
				))
			)
		{
			if(	(	nBtn > 1
				&&	(! _GetButtonPtr(nBtn-2)->IsSeparator() )
				)
				||	nBtn <= 1
				)
			{ // avoid unwanted separator insert
				bSeparatorAdd = true;
				nBtn--;
			} // avoid unwanted separator insert
		}
	} // if( de == DROPEFFECT_MOVE )

	if( bSeparatorRemove )
	{
		CExtBarButton * pModifyTBB = _GetButtonPtr(nBtn);
		ASSERT_VALID( pModifyTBB );
		CExtCustomizeCmdTreeNode * pNode = pModifyTBB->GetCmdNode();
		ASSERT_VALID( pNode );
		pNode->ModifyFlags( 0, __ECTN_GROUP_START );
		RemoveButton(nBtn-1);
		UpdateWindow();
		if(		m_pDockBar != NULL
			&&	IsFloating()
			)
		{
			GetParentFrame()->RecalcLayout();
			Invalidate();
		}
		m_bHelperSeparatorAction = true;
	} // if( bSeparatorRemove )
	else if( bSeparatorAdd )
	{
		CExtBarButton * pModifyTBB = _GetButtonPtr(nBtn);
		ASSERT_VALID( pModifyTBB );
		CExtCustomizeCmdTreeNode * pNode = pModifyTBB->GetCmdNode();
		ASSERT_VALID( pNode );
		pNode->ModifyFlags( __ECTN_GROUP_START );
		InsertButton(nBtn);
		UpdateWindow();
		if(		m_pDockBar != NULL
			&&	IsFloating()
			)
		{
			GetParentFrame()->RecalcLayout();
			Invalidate();
		}
		m_bHelperSeparatorAction = true;
	} // else if( bSeparatorAdd )
	else
	{ // if command drop
		int nInsertPos = (m_nDragTargetShift < 0) ? nBtn : (nBtn+1);
		m_pDragTargetTBB = NULL;
		m_nDragTargetShift = 0;
		UpdateWindow();
		//InsertButton( nInsertPos, pCmdItem->m_nCmdID );
		CExtCustomizeSite * pSite =
			GetCustomizeSite();
		ASSERT( pSite != NULL );
		pSite->OnInsertToolbarCmdNode(
			_dragInfo,
			this,
			nInsertPos,
			true
			);
		if(		m_pDockBar != NULL
			&&	IsFloating()
			)
		{
			GetParentFrame()->RecalcLayout();
			Invalidate();
		}
	} // if command drop
	return true;
}

void CExtToolControlBar::OnCustomizeSourceDragComplete(
	DROPEFFECT de,
	bool bCanceled
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDragSourceTBB );
	ASSERT(
			de == DROPEFFECT_NONE
		||	de == DROPEFFECT_MOVE
		||	de == DROPEFFECT_COPY
		);
int nBtnSrc = _GetIndexOf(m_pDragSourceTBB);
	if( (!bCanceled) && (!m_bHelperSeparatorAction) )
	{
		if(		de == DROPEFFECT_NONE
			||	(	de == DROPEFFECT_MOVE
				&&	m_pDragSourceTBB != m_pDragTargetTBB
				)
			)
		{ // if remove source button
			m_pDragSourceTBB = NULL;
			CExtCustomizeSite * pSite = GetCustomizeSite();
			ASSERT( pSite != NULL );
			CExtCustomizeCmdTreeNode * pNode =
				pSite->DraggedNodeGet();
			ASSERT_VALID( pNode );
			ASSERT( ((LPVOID)pNode) == ((LPVOID)_GetButtonPtr(nBtnSrc)->GetCmdNode(false)) );
			if( pSite->GetPopupDropTargetNode() == pNode )
			{
				pSite->DropTargetPopupCancel();
				ASSERT( pSite->GetPopupDropTargetNode() != pNode );
			}
			RemoveButton( nBtnSrc );
			pSite->DraggedNodeSet( NULL );
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(m_hWnd),
					pNode->GetCmdID( false )
					);
			ASSERT( pCmdItem != NULL );
			ASSERT_VALID( pNode->GetParentNode() );
			int nOwnNodeIndex = pNode->GetOwnIndex();
			int nSiblingCount = pNode->GetParentNode()->GetNodeCount();
			bool bRemovePrevSeparator =
				(pNode->GetFlags() & __ECTN_GROUP_START)
					? true : false;
			bool bRemoveNextSeparator = false;
			if(		bRemovePrevSeparator
				&&	nOwnNodeIndex < (nSiblingCount-1)
				)
			{
				ASSERT( nOwnNodeIndex > 0 );
				ASSERT( nBtnSrc > 0 );
				if( (pNode->GetParentNode()->ElementAt(nOwnNodeIndex+1)
						->GetFlags()&__ECTN_GROUP_START) == 0
					)
				{
					pNode->GetParentNode()->ElementAt(nOwnNodeIndex+1)
						->ModifyFlags( __ECTN_GROUP_START );
					bRemovePrevSeparator = false;
				}
			}
			if(		nOwnNodeIndex == 0
				&&	nSiblingCount > 1
				&&	(pNode->GetParentNode()->ElementAt(1)
						->GetFlags()&__ECTN_GROUP_START) != 0
				)
			{
				pNode->GetParentNode()->ElementAt(1)->ModifyFlags( 0, __ECTN_GROUP_START );
				bRemoveNextSeparator = true;
			}
			pNode->RemoveSelf( pCmdItem );
			if( bRemoveNextSeparator )
			{
				ASSERT( _GetButtonPtr(nBtnSrc)->IsSeparator() );
				RemoveButton( nBtnSrc );
			}
			if( bRemovePrevSeparator )
			{
				ASSERT( _GetButtonPtr(nBtnSrc-1)->IsSeparator() );
				RemoveButton( nBtnSrc-1 );
			}
			if(		m_pDockBar != NULL
				&&	IsFloating()
				)
			{
				GetParentFrame()->RecalcLayout();
				Invalidate();
			}
		} // if remove source button
		else
			_InvalidateButton( nBtnSrc );
	} // if( (!bCanceled) && (!m_bHelperSeparatorAction) )
	else
		_InvalidateButton( nBtnSrc );
	m_pDragSourceTBB = NULL;
	m_pDragTargetTBB = NULL;
	m_nDragTargetShift = 0;
	m_bHelperSeparatorAction = false;
	UpdateWindow();
}

INT CExtToolControlBar::OnCustomizeCalcCmdNodePos(
	INT nButtonPos,
	bool * p_bReinitGroupStart // = NULL
	)
{
	ASSERT_VALID( this );
	if( p_bReinitGroupStart != NULL )
		(*p_bReinitGroupStart) = false;
INT nButtonsCount = _GetButtonsCountImpl();
	ASSERT( nButtonPos <= nButtonsCount );
	if( nButtonsCount == 0 )
		return 0;
INT nCmdPos = 0;
	for( INT nBtnIdx = 0; nBtnIdx < nButtonPos; nBtnIdx++  )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( pTBB->GetCmdNode() == NULL )
			continue;
		ASSERT( !pTBB->IsSeparator() );
		ASSERT_VALID( pTBB->GetCmdNode() );
		nCmdPos ++;
	} // for( INT nBtnIdx = 0; nBtnIdx < nButtonPos; nBtnIdx++  )
	ASSERT( nCmdPos <= nButtonsCount );
	if( p_bReinitGroupStart != NULL && nButtonPos > 0 )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nButtonPos - 1 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsSeparator() )
			(*p_bReinitGroupStart) = true;
	} // if( p_bReinitGroupStart != NULL && nButtonPos > 0 )
	return nCmdPos;
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

/////////////////////////////////////////////////////////////////////////////
// CExtPanelControlBar

IMPLEMENT_DYNCREATE(CExtPanelControlBar, CExtToolControlBar)

CExtPanelControlBar::CExtPanelControlBar()
	: m_rcMargins( 5, 5, 5, 5 )
{
	m_bPaletteMode = true;
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	m_bVisibleInCustomizeListBox = false;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
}

CExtPanelControlBar::~CExtPanelControlBar()
{
}

BEGIN_MESSAGE_MAP(CExtPanelControlBar, CExtToolControlBar)
	//{{AFX_MSG_MAP(CExtPanelControlBar)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar message handlers

CSize CExtPanelControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	ASSERT_VALID( this );

	if(	(nLength == -1)
		&& !(dwMode & (LM_MRUWIDTH|LM_COMMIT))
		&&  (dwMode & (LM_HORZDOCK|LM_VERTDOCK))
		)
		return
			CalcFixedLayout(
				dwMode & LM_STRETCH,
				dwMode & LM_HORZDOCK
				);
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
CSize sizeCalcLayout = _CalcLayout( dwMode, nLength );
	return sizeCalcLayout;
}

CSize CExtPanelControlBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );

DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
CSize sizeCalcLayout = _CalcLayout( dwMode );
	_RecalcNcArea();
	return sizeCalcLayout;
}

CSize CExtPanelControlBar::_CalcLayout(
	DWORD dwMode,
	int nLength // = -1
	)
{
	ASSERT_VALID( this );
	dwMode;
	nLength;

CSize sizeCalcLayout( 0, 0 );
	if( !IsFloating() )
	{
		sizeCalcLayout += CSize( 2, 2 );
		bool bGripperAtTop = false;
		if( IsBarWithGripper( &bGripperAtTop ) )
		{
			CSize sizeGrip = _GetRectGrip().Size();
			if( bGripperAtTop )
				sizeCalcLayout.cy += sizeGrip.cy;
			else
				sizeCalcLayout.cx += sizeGrip.cx;
		}
	}

	if( IsDockedHorizontally() )
	{
		sizeCalcLayout += _QueryFixedBarNewSize( __FSQ_DOCKED_H );
		SetInitDesiredSizeHorizontal( sizeCalcLayout );
		m_nMinHW = sizeCalcLayout.cx;
	}
	else if( IsDockedVertically() )
	{
		sizeCalcLayout += _QueryFixedBarNewSize( __FSQ_DOCKED_V );
		SetInitDesiredSizeVertical( sizeCalcLayout );
		m_nMinVH = sizeCalcLayout.cy;
	}
	else if( IsFloating() )
	{
		sizeCalcLayout += _QueryFixedBarNewSize( __FSQ_FLOATING );
		SetInitDesiredSizeFloating( sizeCalcLayout );
	}
	else
	{
		sizeCalcLayout += CSize( 20, 20 );
	}

	if( m_pDockSite == NULL )
	{ // specific for dialog mode
		if( IsDockedHorizontally() )
			sizeCalcLayout.cy += 4;
		else
			sizeCalcLayout.cx += 4;
	} // specific for dialog mode

	return sizeCalcLayout;
}

INT CExtPanelControlBar::_CalcDesiredMinHW() const
{
	ASSERT_VALID( this );

CSize sizeCalcLayout = _QueryFixedBarNewSize( __FSQ_DOCKED_H );
	return sizeCalcLayout.cx;
}

CSize CExtPanelControlBar::_CalcDesiredMinOuterSize( BOOL bHorz )
{
	ASSERT_VALID( this );

CSize sizeCalcLayout =
		_QueryFixedBarNewSize(
			bHorz ? __FSQ_DOCKED_H : __FSQ_DOCKED_V
			);
	return sizeCalcLayout;
}

INT CExtPanelControlBar::_CalcDesiredMinVH() const
{
	ASSERT_VALID( this );

CSize sizeCalcLayout = _QueryFixedBarNewSize( __FSQ_DOCKED_V );
	return sizeCalcLayout.cy;
}

CSize CExtPanelControlBar::_CalcDesiredMinFloatedSize() const
{
	ASSERT_VALID( this );

CSize sizeCalcLayout = _QueryFixedBarNewSize( __FSQ_FLOATING );
	return sizeCalcLayout;
}

CSize CExtPanelControlBar::_QueryFixedBarNewSize(
	CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
	) const
{
CSize sizeResult( OnQueryFixedBarNewSize(eWhichSizeNeedToKnow) );
//	if( eWhichSizeNeedToKnow != __FSQ_FLOATING )
//	{
//		CRect rcClient, rcWnd;
//		GetClientRect( &rcClient );
//		GetWindowRect( &rcWnd );
//		CSize sizeClient( rcClient.Size() );
//		CSize sizeWnd( rcWnd.Size() );
//		sizeResult += sizeWnd;
//		sizeResult -= sizeClient;
//	}
	return sizeResult;
}

CSize CExtPanelControlBar::OnQueryFixedBarNewSize(
	CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
	) const
{
	ASSERT_VALID( this );
	eWhichSizeNeedToKnow;

//CSize sizeResult;
//	switch( eWhichSizeNeedToKnow )
//	{
//	case __FSQ_DOCKED_H:
//		sizeResult = CSize( 20, 20 );
//	break;
//	case __FSQ_DOCKED_V:
//		sizeResult = CSize( 20, 20 );
//	break;
//	default:
//		ASSERT( eWhichSizeNeedToKnow == __FSQ_FLOATING );
//		sizeResult = CSize( 20, 20 );
//	break;
//	}
//	return sizeResult;

CSize sizeResult( _GetChildSize() );
CRect rcMargins = OnQueryFixedBarChildWindowMargins();
	sizeResult.cx += rcMargins.left + rcMargins.right;
	sizeResult.cy += rcMargins.top + rcMargins.bottom;
	return sizeResult;
}

CRect CExtPanelControlBar::OnQueryFixedBarChildWindowMargins() const
{
	ASSERT_VALID( this );

	return m_rcMargins;
}

CSize CExtPanelControlBar::_GetChildSize() const
{
	ASSERT_VALID( this );
	if(		GetSafeHwnd() == NULL
		||	(! ::IsWindow(GetSafeHwnd()))
		)
		return CSize( 0, 0 );

CWnd * pWndChild = GetWindow( GW_CHILD );
	if(		pWndChild->GetSafeHwnd() == NULL
		||	(! ::IsWindow(pWndChild->GetSafeHwnd()))
		)
		return CSize( 0, 0 );
CRect rcChild;
	pWndChild->GetWindowRect( &rcChild );
CSize sizeChild( rcChild.Size() );
	return sizeChild;
}

void CExtPanelControlBar::_RepositionChild(
	bool bRedraw // = true
	)
{
	ASSERT_VALID( this );

CWnd * pWndChild = GetWindow( GW_CHILD );
	if( pWndChild == NULL )
		return;
CRect rcMargins = OnQueryFixedBarChildWindowMargins();
CRect rcRequired, rcReal;
	GetClientRect( &rcRequired );
	rcRequired.DeflateRect( &rcMargins );
	pWndChild->GetWindowRect( &rcReal );
	ScreenToClient( &rcReal );
//	if( rcReal == rcRequired )
	if( rcReal.TopLeft() == rcRequired.TopLeft() )
		return;
	rcReal.OffsetRect( - rcReal.TopLeft() + rcRequired.TopLeft() );
	pWndChild->MoveWindow(
		&rcReal, // &rcRequired,
		bRedraw ? TRUE : FALSE
		);
}

void CExtPanelControlBar::OnSize(UINT nType, int cx, int cy) 
{
	ASSERT_VALID( this );

	CExtToolControlBar::OnSize(nType, cx, cy);
	_RepositionChild();	
}

void CExtPanelControlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	ASSERT_VALID( this );

	CExtToolControlBar::OnWindowPosChanged(lpwndpos);
	_RepositionChild();
}

void CExtPanelControlBar::OnSetFocus(CWnd* pOldWnd) 
{
	ASSERT_VALID( this );

	CExtToolControlBar::OnSetFocus(pOldWnd);
CWnd * pWndChild = GetWindow( GW_CHILD );
	if( pWndChild == NULL )
		return;	
	pWndChild->SetFocus();
}

void CExtPanelControlBar::OnPaint() 
{
	ASSERT_VALID( this );

CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint );

CRect rcClient;
	GetClientRect( &rcClient );

CWnd * pWndChild = GetWindow( GW_CHILD );
	if( pWndChild == NULL )
	{ // if no child - fill entire client area
		bool bFillBk = true;
		if(		g_PaintManager->GetCb2DbTransparentMode(this)
			&&	g_PaintManager->PaintDockerBkgnd( dc, this )
			)
			bFillBk = false;
		if( bFillBk )
			dc.FillSolidRect(
				&rcClient,
				g_PaintManager->GetColor(
					CExtPaintManager::CLR_3DFACE_OUT
					)
				);
//
// DEBUG BLOCK BEGIN: - fill gradient & draw text
//
//		rcClient.DeflateRect( 2, 2 );
//		COLORREF clrFc = g_PaintManager->GetColor(COLOR_3DFACE);
//		COLORREF clrDk = g_PaintManager->GetColor(COLOR_3DDKSHADOW);
//		COLORREF clrHi = g_PaintManager->GetColor(COLOR_3DHILIGHT);
//		COLORREF clrLo = g_PaintManager->GetColor(COLOR_3DSHADOW);
//		dc.Draw3dRect( &rcClient, clrFc, clrDk );
//		rcClient.DeflateRect( 1, 1 );
//		dc.Draw3dRect( &rcClient, clrHi, clrLo );
//		rcClient.DeflateRect( 1, 1 );
//		dc.Draw3dRect( &rcClient, clrLo, clrHi );
//		rcClient.DeflateRect( 1, 1 );
//		dc.Draw3dRect( &rcClient, clrDk, clrFc );
//		rcClient.DeflateRect( 2, 2 );
//		UINT nDockBarID = GetSafeDockBarDlgCtrlID();
//		CExtPaintManager::stat_PaintGradientRect(
//			dc,
//			rcClient,
//			RGB( 128, 128, 255 ),
//			RGB( 0, 0, 0 ),
//			(nDockBarID==AFX_IDW_DOCKBAR_LEFT || nDockBarID==AFX_IDW_DOCKBAR_RIGHT)
//				? true : false
//			);
//		
//		CRect rcDrawText( rcClient );
//		rcDrawText.DeflateRect( 5, 5 );
//		CRect rcDrawTextShadow( rcDrawText );
//		CString sText;
//		CFont * pFont = &g_PaintManager->m_FontNormal;
//		switch( nDockBarID )
//		{
//		case AFX_IDW_DOCKBAR_TOP:
//			sText = _T("Docked at top");
//			rcDrawTextShadow.OffsetRect( 1, 1 );
//		break;
//		case AFX_IDW_DOCKBAR_BOTTOM:
//			sText = _T("Docked at bottom");
//			rcDrawTextShadow.OffsetRect( 1, 1 );
//		break;
//		case AFX_IDW_DOCKBAR_LEFT:
//			sText = _T("Docked at left");
//			pFont = &g_PaintManager->m_FontNormalVert;
//			rcDrawText.OffsetRect( rcDrawText.Width(), 0 );
//			rcDrawText.OffsetRect( -1, -2 );
//			rcDrawTextShadow = rcDrawText;
//			rcDrawTextShadow.OffsetRect( 1, 1 );
//		break;
//		case AFX_IDW_DOCKBAR_RIGHT:
//			sText = _T("Docked at right");
//			pFont = &g_PaintManager->m_FontNormalVert;
//			rcDrawText.OffsetRect( rcDrawText.Width(), 0 );
//			rcDrawText.OffsetRect( -1, -2 );
//			rcDrawTextShadow = rcDrawText;
//			rcDrawTextShadow.OffsetRect( 1, 1 );
//		break;
//		default:
//			ASSERT( nDockBarID == AFX_IDW_DOCKBAR_FLOAT );
//			sText = _T("Floating");
//			rcDrawTextShadow.OffsetRect( 1, 1 );
//		break;
//		} // switch( nDockBarID )
//		CFont * pOldFont = dc.SelectObject( pFont );
//		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
//		COLORREF clrOldTextColor = dc.SetTextColor( RGB(128,128,128) );
//		dc.DrawText( sText, &rcDrawTextShadow, DT_SINGLELINE|DT_NOCLIP );
//		dc.SetTextColor( RGB(255,255,255) );
//		dc.DrawText( sText, &rcDrawText, DT_SINGLELINE|DT_NOCLIP );
//		dc.SetTextColor( clrOldTextColor );
//		dc.SetBkMode( nOldBkMode );
//		dc.SelectObject( pOldFont );
//
// DEBUG BLOCK END: - fill gradient & draw text
//
	} // if no child - fill entire client area
	else
	{ // if has child - fill margins around child
//		CRect rcMargins = OnQueryFixedBarChildWindowMargins();
//		CRect rcExclude( rcClient );
//		rcExclude.DeflateRect( &rcMargins );
//		dc.ExcludeClipRect( &rcExclude );
		CExtPaintManager::stat_ExcludeChildAreas(
			dc,
			m_hWnd
			);
		bool bFillBk = true;
		if(		g_PaintManager->GetCb2DbTransparentMode(this)
			&&	g_PaintManager->PaintDockerBkgnd( dc, this )
			)
			bFillBk = false;
		if( bFillBk )
			dc.FillSolidRect(
				&rcClient,
				g_PaintManager->GetColor(
					CExtPaintManager::CLR_3DFACE_OUT
					)
				);
		dc.SelectClipRgn( NULL );
	} // if has child - fill margins around child
}

bool CExtPanelControlBar::_AffixmentIsAlignedHW() const
{
	ASSERT_VALID( this );
	return false;
}

LRESULT CExtPanelControlBar::DoHelpHitTest( CPoint ptHelpHitTest )
{
	ASSERT_VALID( this );
	return CExtControlBar::DoHelpHitTest( ptHelpHitTest );
}

bool CExtPanelControlBar::OnQueryMultiRowLayout() const
{
	ASSERT_VALID( this );
	return false;
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)

CExtCustomizeSite * CExtPanelControlBar::GetCustomizeSite()
{
	ASSERT_VALID( this );
	return NULL;
}

bool CExtPanelControlBar::OnCustomizeRegisterBar(
	CExtCustomizeSite * pCustomizeSite
	)
{
	ASSERT_VALID( this );
	ASSERT( pCustomizeSite != NULL );
	pCustomizeSite;
	return false;
}

void CExtPanelControlBar::OnCustomizeModeEnter()
{
	ASSERT_VALID( this );
}

void CExtPanelControlBar::OnCustomizeModeLeave()
{
	ASSERT_VALID( this );
}

void CExtPanelControlBar::OnCustomizeListBoxUpdate( CExtCustomizeToolbarsListBox * pWndList )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pWndList );
	CExtToolControlBar::OnCustomizeListBoxUpdate( pWndList );
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)


