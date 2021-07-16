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
#include <ExtBtnOnFlat.h>
#include <ExtPopupMenuWnd.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWinApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CExtBtnOnFlat

CExtBtnOnFlat::CExtBtnOnFlat()
{
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	m_bToolTipSet = FALSE;
#endif

	m_bCurrentlyIsFlat = TRUE;
}

CExtBtnOnFlat::~CExtBtnOnFlat()
{
}

IMPLEMENT_DYNCREATE(CExtBtnOnFlat, CButton);

BEGIN_MESSAGE_MAP(CExtBtnOnFlat, CButton)
	//{{AFX_MSG_MAP(CExtBtnOnFlat)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtBtnOnFlat message handlers
void CExtBtnOnFlat::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);
	
	if( CExtPopupMenuWnd::TestHoverEnabledFromActiveHWND(
			GetSafeHwnd()
			)
		)
	{
		SetTimer(1, 50, NULL);
		OnTimer(1);
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
		// RTF tool tip support
		if(m_bToolTipSet)
			m_wndRtfToolTipCtrl.
				SetupRtfFromString(
					point, m_strToolTip );
#endif
	}
}

void CExtBtnOnFlat::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 1 )
	{
		POINT pt;
		GetCursorPos(&pt);
		CRect rcItem;
		GetWindowRect(&rcItem);

		static bool bPainted = false;

		if(!rcItem.PtInRect(pt))
		{
			KillTimer(1);
			if(!m_bCurrentlyIsFlat)
			{
				m_bCurrentlyIsFlat = TRUE;
				ModifyStyle(0,BS_FLAT);
				RedrawWindow();
			}
		}
		else
		{
			if(m_bCurrentlyIsFlat)
			{
				m_bCurrentlyIsFlat = FALSE;
				ModifyStyle(BS_FLAT,0);
				RedrawWindow();
			}
		}
		return;
	} // if( nIDEvent == 1 )

	CButton::OnTimer(nIDEvent);
}

BOOL CExtBtnOnFlat::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
//	return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExtBtnOnFlat::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
//	m_bDefault=GetStyle() & (DWORD)BS_DEFPUSHBUTTON; //this is necessary as 
//	ModifyStyle(0, BS_OWNERDRAW);
	ModifyStyle(0, BS_FLAT);
}
