/////////////////////////////////////////////////////////////////////////////
// CalcBtn.cpp : implementation of SECCalcBtn - the base class for 
//				 SECCalculator buttons.
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

#ifndef __SECAUX_H__
#include "SecAux.h"
#endif

#ifndef __SECIMPL_H__
#include "SecImpl.h"
#endif

#ifndef __SECCALC_H__
#include "SecCalc.h"
#endif

#ifndef __CALCBTN_H__
#include "CalcBtn.h"
#endif



extern SEC_AUX_DATA secData;

#ifdef _SECDLL
#undef AFXAPP_DATA
#define AFXAPP_DATA AFXAPI_DATA
#endif //_SECDLL


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SECCalcBtn
//
// Base class (and implementation) for SECCalculator buttons.
//


//---------------------------------------------------------------------------
//
// SECCalcBtn  Default constructor
//
//@doc SECCalcBtn
//@mfunc Constructs a SECCalcBtn object.
SECCalcBtn::SECCalcBtn()
{
	m_nWidth        = 1;
	m_nState        = 0;
	m_nKeyDownAccel = 0;
	m_rect.SetRectEmpty();
}

//---------------------------------------------------------------------------
//
// SECCalcBtn  Destructor
//
//@doc SECCalcBtn
//@mfunc SECCalcBtn destructor.
SECCalcBtn::~SECCalcBtn()
{
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Initialise
//
// Initialise button where accelerators are same as caption
//
//@doc SECCalcBtn
//@mfunc Initialize button with accelerators same as caption, different 
// from caption, or with a single character accelerator.
//@syntax BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op,
// LPCTSTR lpszCaption, int nWidth /* = 1 */, BOOL bStartsRow /* = FALSE */);
//@syntax BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
// LPCTSTR lpszCaption, LPCTSTR lpszAccel, int nWidth /* = 1 */, 
// BOOL bStartsRow /* = FALSE */);
//@syntax BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
// LPCTSTR lpszCaption, LPCTSTR lpszAccel, int nWidth /* = 1 */, 
// BOOL bStartsRow /* = FALSE */);
//@rdesc Nonzero if the button is successfully initialized; otherwise 0.
//@parm const Attrib& | attrib | References attributes used for drawing 
// button on the calculator.	
//@parm SECCalculator::SECCalcOp | op | Specifies a calculator operation.
//@parm LPCTSTR | lpszCaption | Points to string containing button’s text.
//@parm TCHAR | cAccel | Specifies a single character accelerator.
//@parm LPCTSTR | cAccel | Specifies a single character accelerator.
//@parmopt int |nWidth | 1 | Value for button’s width.
//@parmopt BOOL | bStartsRow | FALSE | Flag indicating whether button starts row.
//@end
BOOL SECCalcBtn::Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op,
							LPCTSTR lpszCaption, int nWidth /* = 1 */,
							BOOL bStartsRow /* = FALSE */)
{
	return Initialise(attrib, op, lpszCaption, lpszCaption, nWidth, bStartsRow);
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Initialise
//
// Initialise button with single character accelerator
//
BOOL SECCalcBtn::Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
							LPCTSTR lpszCaption, TCHAR cAccel, 
							int nWidth /* = 1 */, 
							BOOL bStartsRow /* = FALSE */)
{
	CString temp = cAccel;
	return Initialise(attrib, op, lpszCaption, temp, nWidth, bStartsRow);
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Initialise
//
// Initialise button with different accelerators to caption
//
BOOL SECCalcBtn::Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
							LPCTSTR lpszCaption, LPCTSTR lpszAccel, 
							int nWidth /* = 1 */,  
							BOOL bStartsRow /* = FALSE */)
{
	m_attribs    = attrib;
	m_op         = op;
	m_strCaption = lpszCaption;
	m_nWidth     = nWidth;
	if(bStartsRow)
		m_nState |= StartsRow;
	else
		m_nState &= ~StartsRow;

	// We keep accelerators as lower case.
	m_strAccels  = lpszAccel;
	m_strAccels.MakeLower();

	return TRUE;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::GetWidth
//
//
//@doc SECCalcBtn
//@mfunc Returns the width of the button.  This is specified in the number
// of columns it occupies.
//@rdesc The width of the button in columns.
BOOL SECCalcBtn::GetWidth() const
{
	ASSERT_VALID(this);

	return m_nWidth;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::GetRect
//
//
//@doc SECCalcBtn
//@mfunc Gets the rectangle of the button (relative to the calculator’s client).
//@rdesc The rectangle of the button.
const CRect& SECCalcBtn::GetRect() const
{
	ASSERT_VALID(this);

	return m_rect;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::SetRect
//
//
//@doc SECCalcBtn
//@mfunc Sets the rectangle of the button (relative to the calculator’s client).
//@rdesc void 
//@parm const RECT& | rect | A reference to the button’s RECT structure.
void SECCalcBtn::SetRect(const RECT& rect)
{
	ASSERT_VALID(this);

	m_rect = rect;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::GetState
//
//
//@doc SECCalcBtn
//@mfunc Get the button state.
//@rdesc Gets the button state.
//@flag	SECCalcBtn::Down | Button is currently down
//@flag SECCalcBtn::StartsRow | Button starts a new row
  int  SECCalcBtn::GetState() const
{
	ASSERT_VALID(this);

	return m_nState;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::SetState
//
//
//@doc SECCalcBtn
//@mfunc Sets the button state.
//@rdesc void
//@parm int | nState | An integer value corresponding to a button state. The
// state is represented by the following bit flags:
//@flag	SECCalcBtn::Down | Button is currently down
//@flag SECCalcBtn::StartsRow | Button starts a new row
void SECCalcBtn::SetState(int nState)
{
	ASSERT_VALID(this);

	m_nState = nState;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::SetKeyDownAccel
//
//
//@doc SECCalcBtn
//@mfunc Sets the WM_KEYDOWN accelerator for the button.  This is not
// normally set.
//@rdesc void 
//@parm UINT | nChar | A char associated with a WM_KEYDOWN message.
void SECCalcBtn::SetKeyDownAccel(UINT nChar)
{
	ASSERT_VALID(this);

	m_nKeyDownAccel = nChar;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::IsKeyDownAccelerator
//
//
//@doc SECCalcBtn
//@mfunc Tests whether the given WM_KEYDOWN char is an accelerator for 
// the button.
//@rdesc Returns non-zero if the given WM_KEYDOWN char is an accelerator for 
// the button; otherwise 0;
//@parm UINT | nChar | A char associated with a WM_KEYDOWN message.
BOOL SECCalcBtn::IsKeyDownAccelerator(UINT nChar) const
{
	ASSERT_VALID(this);

	return (nChar == m_nKeyDownAccel);
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::IsAccelerator
//
//
//@doc SECCalcBtn
//@mfunc Test whether the given char is an accelerator for the button.
//@rdesc Returns non-zero if the given char is an accelerator for 
// the button; otherwise 0;
//@parm LPCTSTR | lpszChar | Pointer to a char that is an accelerator for 
// the button.
BOOL SECCalcBtn::IsAccelerator(LPCTSTR lpszChar) const
{
	ASSERT_VALID(this);

	return (m_strAccels.Find(lpszChar) != -1);
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::GetOp
//
//
//@doc SECCalculator
//@mfunc Queries the operation code for this button.
//@rdesc The operation code for the button.  See <md SECCalculator::SECCalcOp>.
SECCalculator::SECCalcOp SECCalcBtn::GetOp() const
{
	ASSERT_VALID(this);

	return m_op;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Draw
//
//
//@doc SECCalcBtn
//@mfunc Draws the button to the given DC. Note that the button is drawn 
// relative to the SECCalculator's client rectangle.
//@rdesc void 
//@parm CDC& | dc | A reference to the button’s device context.
void SECCalcBtn::Draw(CDC& dc)
{
	// Draw the outer borders around the button
	CRect r = m_rect;
	Draw3dRect1(&dc, r, secData.clrBtnShadow, secData.clrBtnHilite);
	r.InflateRect(-1, -1);
	Draw3dRect1(&dc, r, secData.clrWindowFrame, secData.clrWindowFrame);
	r.InflateRect(-1, -1);

	if(m_nState & Down)
	{
		// The button is current depressed. This has just a single width
		// inner border.
		Draw3dRect1(&dc, r, secData.clrBtnShadow, secData.clrBtnShadow);
		r.InflateRect(-1, -1);

		FillSolidRect1(&dc, r, secData.clrBtnFace);

		r.InflateRect(-1, -1);

		// Offset the button caption (gives appearance of movement).
		r.left += 2;
		r.top += 2;
	}
	else
	{
		// Up
		Draw3dRect1(&dc, r, secData.clrBtnHilite, secData.clrBtnShadow);
		r.InflateRect(-1, -1);
		Draw3dRect1(&dc, r, secData.clr3DLight, secData.clrBtnShadow);
		r.InflateRect(-1, -1);

		FillSolidRect1(&dc, r, secData.clrBtnFace);
	}

	// If we have a bitmap then draw it now.
	HBITMAP hBmp = m_attribs.GetBitmap();
	if(hBmp)
	{
		CDC memDC;
		if(memDC.CreateCompatibleDC(&dc))
		{
			HGDIOBJ hOldBmp = memDC.SelectObject((HGDIOBJ) hBmp);

			BITMAP bm;
			::GetObject(hBmp, sizeof(bm), &bm);

			int x;
			if(!m_strCaption.IsEmpty())
				// The button has a caption so, offset bitmap from left hand
				// edge
				x = r.left + 5;
			else
				// The button has no caption so bitmap will be centered.
				x = max((r.Width() - bm.bmWidth)/2 + r.left, r.left);

			// Bitmap is always centered on the Y axis.
			int y = max((r.Height() - bm.bmHeight)/2 + r.top, r.top);

			int cx = min(r.right - x, bm.bmWidth);
			int cy = min(r.bottom - y, bm.bmHeight);

			dc.BitBlt(x, y, cx, cy, &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(hOldBmp);

			// Offset the remaining rectangle past the bitmap.
			r.left += cx;
		}
	}

	if(!m_strCaption.IsEmpty())
	{
		// We have a caption so draw it now.
		HGDIOBJ hFontOld = dc.SelectObject((HGDIOBJ) m_attribs.GetFont());
		dc.SetTextColor(m_attribs.GetTextColor());
		dc.SetBkColor(secData.clrBtnFace);
		dc.DrawText(m_strCaption, m_strCaption.GetLength(), r, 
					DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		dc.SelectObject(hFontOld);
	}
}


#ifdef _DEBUG
void SECCalcBtn::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "m_op     = " << (UINT) m_op << "\n";
	dc << "m_nState = " << m_nState << "\n";
	dc << "m_rect   = " << m_rect << "\n";
}

void SECCalcBtn::AssertValid() const
{
	CObject::AssertValid();

	ASSERT(m_nWidth > 0);
}
#endif




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SECCalcBtn::Attrib
//
// SECCalcBtn attribute class. Defines the attributes for drawing the face
// of the button
//


//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib  Default constructor
//
//@doc SECCalcBtn
//@mfunc Constructs an Attrib object.
SECCalcBtn::Attrib::Attrib()
{
	m_dwStyle   = 0L;					// Style flags for drawing button
	m_textColor = RGB(0, 0, 0);			// Color for drawing caption
	m_hFont     = NULL;					// Font used for drawing caption
	m_lphBitmap = NULL;					// Ptr to bitmap handle. This is an
										// optional bitmap which can be
										// drawn on the face of the button.
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib  constructor
//
//@doc SECCalcBtn
//@mfunc Consructs an Attrib object.
//@parm DWORD | dwStyle | Specifies the button’s style attributes.
//@parmopt COLORREF | textColor | 0L | Specifies the button’s text color.
//@parmopt HFONT | hFont | NULL | Specifies the font of the button’s text.
//@parmopt HBITMAP* | lphBitmap | NULL | Pointer to the handle for the button’s bitmap.
SECCalcBtn::Attrib::Attrib(DWORD dwStyle, COLORREF textColor /* = 0L */,
						   HFONT hFont /* = NULL */,
						   HBITMAP* lphBitmap /* = NULL */)
{
	m_dwStyle   = dwStyle;
	m_textColor = textColor;
	m_hFont     = hFont;
	m_lphBitmap = lphBitmap;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib  Copy constructor
//

//@doc SECCalcBtn
//@mfunc Copy constructor.
//@parm const Attrib& | attrib | Attrib object to copy.
SECCalcBtn::Attrib::Attrib(const Attrib& attrib)
{
	m_hFont     = attrib.m_hFont;
	m_lphBitmap = attrib.m_lphBitmap;
	m_textColor = attrib.m_textColor;
	m_dwStyle   = attrib.m_dwStyle;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib Destructor
//
//@doc SECCalcBtn
//@mfunc Attrib destructor
SECCalcBtn::Attrib::~Attrib()
{
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib  Assignment operator
//
//@doc SECCalcBtn
//@mfunc Equals operator.
//@rdesc Reference to the new instance of Attrib.
//@parm const SECCalcBtn::Attrib& | attrib | Copy constructor for Attrib.  
// See <mf Attrib::Attrib>.
const SECCalcBtn::Attrib& SECCalcBtn::Attrib::operator=(const SECCalcBtn::Attrib& attrib)
{
	m_hFont     = attrib.m_hFont;
	m_lphBitmap = attrib.m_lphBitmap;
	m_textColor = attrib.m_textColor;
	m_dwStyle   = attrib.m_dwStyle;

	return *this;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib::GetFont
//
//
//@doc SECCalcBtn
//@mfunc Gets the font of the calculator button’s text.
//@rdesc The font handle of the button’s text.
HFONT SECCalcBtn::Attrib::GetFont() const
{
	return m_hFont;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib::GetTextColor
//
//
//@doc SECCalcBtn
//@mfunc Gets the foreground text color for drawing the button text.
//@rdesc Returns the color of the button’s text as a COLORREF structure.
COLORREF SECCalcBtn::Attrib::GetTextColor() const
{
	return m_textColor;
}

//---------------------------------------------------------------------------
//
// SECCalcBtn::Attrib::GetBitmap
//
//
//@doc SECCalcBtn
//@mfunc Gets the handle of the calculator button’s bitmap. Note this can 
// be (and probably is) NULL.
//@rdesc The handle of the button’s bitmap.
HBITMAP SECCalcBtn::Attrib::GetBitmap() const
{
	if(m_lphBitmap)
		return *m_lphBitmap;
	else
		return NULL;
}


