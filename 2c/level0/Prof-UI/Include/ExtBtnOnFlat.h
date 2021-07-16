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

#if (!defined __EXTBTNONFLAT_H)
#define __EXTBTNONFLAT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FlatEdit.h : header file
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#define FE_MOUSEOVER 1
#define FE_MOUSEOUT 2

#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	#include <ExtRichToolTipWnd.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtBtnOnFlat window

class __PROF_UIS_API CExtBtnOnFlat : public CButton
{
	DECLARE_DYNCREATE(CExtBtnOnFlat);

// Construction
public:
	CExtBtnOnFlat();

// Attributes
public:

private:
	BOOL m_bCurrentlyIsFlat;

	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	CExtRichToolTipWnd m_wndRtfToolTipCtrl;
	BOOL m_bToolTipSet;
	CString m_strToolTip;
#endif

// Operations
public:
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
    void SetupRtfToolTip(
		const CString & tip
		)
	{
		m_strToolTip=tip;
		if(!m_bToolTipSet)
		{
			m_bToolTipSet=m_wndRtfToolTipCtrl.Create(this);
		}
	};
    void SetupRtfToolTip(
		LPCTSTR pszMsgResId,
		LPCTSTR pszMsgResType="RTF"
		)
	{
		if(!m_bToolTipSet)
		{
			if(CExtRichToolTipWnd::_LoadStringEx(
					pszMsgResId,
					pszMsgResType,
					m_strToolTip
				) )
			{
				m_bToolTipSet=m_wndRtfToolTipCtrl.Create(this);
			}
			else
				m_strToolTip="";
		}
	};
#endif // __EXT_MFC_NO_RTF_TOOLTIPS

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtBtnOnFlat)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtBtnOnFlat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtBtnOnFlat)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__EXTBTNONFLAT_H)
