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

#if (!defined __EXT_STATUSCONTROLBAR_H)
#define __EXT_STATUSCONTROLBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtStatusControlBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar window

class __PROF_UIS_API CExtStatusControlBar : public CStatusBar
{
// Construction
public:

	DECLARE_DYNCREATE( CExtStatusControlBar )

	CExtStatusControlBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtStatusControlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtStatusControlBar();

	void DrawBorders( CDC * pDC, CRect & rc );

	bool m_bOuterRectInFirstBand:1;
protected:
	COLORREF m_clrStatusBk;
	void _SyncStatusBarColors();
	CFrameWnd * _GetDockingFrameImpl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtStatusControlBar)
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_STATUSCONTROLBAR_H
