/////////////////////////////////////////////////////////////////////////////
// SecCalc.cpp : implementation of SECCalculator and SECPopupCalculator
//				 custom controls
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

#ifndef __SECCALC_H__
#include "SecCalc.h"
#endif

#ifndef __SECRES_H__
#include "secres.h"
#endif

#ifndef __CALCBTN_H__
#include "CalcBtn.h"
#endif

#ifndef __SECAUX_H__
#include "SecAux.h"
#endif

#ifndef __SECIMPL_H__
#include "SecImpl.h"
#endif

#ifndef __SLOADBMP_H__
#include "SLoadBmp.h"
#endif

#ifndef _INC_LOCALE
#include <locale.h>
#endif

#if _MFC_VER <= 0x0410
#include <WINNLS.H>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Handle macro for determining number of elements in an array
#define dimensionof(a) (sizeof(a)/sizeof(a[0]))


// Internal structure - holds cache of system colors
extern SEC_AUX_DATA secData;

static const int nMaxSigFig  = 15;		// The no. of significant figures we
										// work to
static const int nMaxIntSize = 13;		// Maximum integer is 13 digits
static const int nDividerSpc = 2;		// Additional Vertical Gap around
										// divider

static CString strDum;	//used for accessing the resource ID's from secres.rc string table

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SECCalculator
//
// Implements a simple calculator (window)
//
IMPLEMENT_DYNAMIC(SECCalculator, CWnd)

BEGIN_MESSAGE_MAP(SECCalculator, CWnd)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SECCalculator - Construction
//

//@doc SECCalculator
//@mfunc Constructs an SECCalculator object.
//@xref <c SECCalculator>
SECCalculator::SECCalculator()
{
	m_nBtnWidth    =				// Width of a button
	m_nBtnHeight   =				// Height of a button
	m_nVIndent     =				// Indent of button matrix from top edge
	m_nHIndent     =				// Indent of button matrix from left edge
	m_nVSpace      =				// Vertical space between buttons
	m_nHSpace      =				// Horizontal space between buttons
	m_nCapture     =				// Index of button with capture
	m_nTotalDigits =				// Maximum number of displayable digits
	m_nState       = 0;				// State flags
	m_val          = 0.0;			// Current value
	m_bReplace     = FALSE;			// Next digit starts a new number
	m_lastOp       = OpNull;		// Last operator entered
	m_hEqualsBmp   = NULL;			// Handle of equals bitmap
	m_nDP          = nMaxSigFig;	// Max no. of decimal places

	
	m_panelRect.SetRectEmpty();		// Rectangle of LCD display panel
	m_strDisplay.Empty();			// Current display string
}

//@doc SECCalculator
//@mfunc SECCalculator destructor.
//@xref <c SECCalculator>
SECCalculator::~SECCalculator()
{
	if(m_hEqualsBmp)
	{
		::DeleteObject(m_hEqualsBmp);
		m_hEqualsBmp = NULL;
	}

	for(int i=0; i<m_btns.GetSize(); i++)
		delete m_btns[i];
	m_btns.RemoveAll();
}

//@doc SECCalculator
//@mfunc Creates the calculator window with extended style attributes and attaches 
// it to the SECCalculator object.
//@rdesc Nonzero if the window is successfully created; otherwise 0.
//@parm DWORD | dwExStyle | Specifies the calculator window’s extended style attributes.
//@parm DWORD | dwStyle | Specifies the calculator window’s style attributes:
//@flag SEC_CS_RAISEDEDGE  | Same look as WS_EX_WINDOWEDGE  except compatible with Win 3.x.
//@flag SEC_CS_DIVIDER | Divider at top of calculator.
//@parm int | x | The x-coordinate of the calculator window in the parent window.
//@parm int | y | The y-coordinate of the calculator window in the parent window.
//@parm CWnd* | pParentWnd | The parent window.
//@parm UINT | nID | The ID of the child window.
//@parm CCreateContext* | pContext | The create context of the calculator window.
//@xref <c SECCalculator>
BOOL SECCalculator::CreateEx(DWORD dwExStyle, DWORD dwStyle, int x, int y,
							 CWnd* pParentWnd, UINT nID,
							 CCreateContext* pContext)
{
        if (m_strDecSep.IsEmpty())
	{
		m_strDecSep	= LoadDecSeparator(); // Load decimal separator (virtual method)
	}

	secData.UpdateSysColors();

	ResetCalc();

	if(dwStyle & SEC_CS_RAISEDEDGE)
	{
		// If we our going to use our own border style, ensure we ignore all
		// other border styles
#ifdef WIN32
		dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE);
#endif
		dwStyle   &= ~WS_BORDER;
	}

	// Create the calculator button objects
	if(m_btns.GetSize() == 0)
		CreateBtns(pParentWnd);

	// Create LCD panel
	if(m_panelFont.m_hObject == NULL)
		CreatePanel(pParentWnd);

	// We need to calculator our size (so we can create the window rectangle
	// as the correct size)
	CSize sz = CalcLayout(dwStyle);

	// Allow for non-client space
	{
		CRect r;
		r.SetRectEmpty();
		::AdjustWindowRectEx(&r, dwStyle, FALSE, dwExStyle);

		sz.cx += r.Width();
		sz.cy += r.Height();

		if(dwStyle & SEC_CS_RAISEDEDGE)
		{
			sz.cx += 4;
			sz.cy += 4;
		}
	}

	LPCTSTR lpszClassName = AfxRegisterWndClass(GetClassStyle(),
												secData.hcurArrow);

	if(!CWnd::CreateEx(dwExStyle,
					   lpszClassName,
					   NULL,
					   dwStyle,
					   x,
					   y,
					   sz.cx,
					   sz.cy,
					   pParentWnd->GetSafeHwnd(),
					   (HMENU) nID,
#ifdef WIN32
					   (LPVOID) pContext))
#else 
					   (LPSTR) pContext))
#endif
	{
		return FALSE;
	}

	m_nState &= ~HasInitialValue;

	return TRUE;
}

//@doc SECCalculator
//@mfunc Creates a window for the calculator and attaches it to the SECCalculator object.
//@rdesc Nonzero if the window is successfully created; otherwise 0.
//@parm DWORD dwStyle Specifies the calculator window style attributes:
//@flag SEC_CS_RAISEDEDGE | Same look as WS_EX_WINDOWEDGE  except compatible with Win 3.x.
//@flag SEC_CS_DIVIDER | Divider at top of calculator.
//@parm int x The x-coordinate of the calculator window in the parent window.
//@parm int y The y-coordinate of the calculator window in the parent window.
//@parm CWnd* pParentWnd The parent window.
//@parm UINT nID The ID of the child window.
//@parm CCreateContext* pContext The create context of the calculator window.
//@xref <c SECCalculator>
BOOL SECCalculator::Create(DWORD dwStyle, int x, int y, CWnd* pParentWnd,
						   UINT nID, CCreateContext* pContext)
{
	return CreateEx(0, dwStyle|WS_CHILD, x, y, pParentWnd, nID, pContext);
}


/////////////////////////////////////////////////////////////////////////////
// SECCalculator - Operations
//

//---------------------------------------------------------------------------
// SECCalculator::InvalidateBtn
//
// Invalidates the given button
//
//@doc SECCalculator
//@mfunc Invalidates the calculator button at the given index.
//@rdesc void 
//@parm int | nIndex | Zero-based index of the button to invalidate.
//@parmopt  BOOL | bErase | FALSE | Specifies whether the background within the 
// update region is to be erased. 
//@xref <c SECCalculator> <mf SECCalculator::InvalidatePanel>
void SECCalculator::InvalidateBtn(int nIndex, BOOL bErase /* = FALSE */)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0 && nIndex < m_btns.GetSize());

	InvalidateRect(m_btns[nIndex]->GetRect(), bErase);
}

//---------------------------------------------------------------------------
// SECCalculator::InvalidatePanel
//
// Invalidates the LCD display panel
//
//@doc SECCalculator
//@mfunc Invalidates the LCD panel.
//@rdesc void 
//@parmopt BOOL | bErase | FALSE | Specifies whether the background is to 
// be erased.
//@xref <c SECCalculator> <mf SECCalculator::InvalidateButton>
void SECCalculator::InvalidatePanel(BOOL bErase /* = FALSE */)
{
	ASSERT_VALID(this);

	if(m_hWnd)
		InvalidateRect(m_panelRect, bErase);
}

//---------------------------------------------------------------------------
// SECCalculator::ResetCalc
//
// Resets the calculator back to a "zero" state
//
//@doc SECCalculator
//@mfunc Resets the calculator back to a "zero" state
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::ResetCalc()
{
	ASSERT_VALID(this);

	if(!(m_nState & HasInitialValue) || m_hWnd)
	{
		// Only initialise if required
		m_val = 0.0;
		m_strDisplay.Empty();
	}

	m_nState   &= ~CalcError;
	m_bReplace  = TRUE;
	m_lastOp    = OpNull;

	if(m_hWnd)
		InvalidatePanel();
}

//---------------------------------------------------------------------------
// SECCalculator::SetValue
//
// Sets the currently displayed value
//
//@doc SECCalculator
//@mfunc Sets the currently displayed value.
//@rdesc void 
//@parm double | val | The new calculator value.
//@xref <c SECCalculator> <mf SECCalculator::GetValue>
void SECCalculator::SetValue(double val)
{
	ASSERT_VALID(this);

	ResetCalc();
	m_val = val;

	if(m_hWnd == NULL)
		// We don't have a window yet. Creation of the window causes ResetCalc
		// to be called which will reset the calculator back to zero. To
		// avoid this, we set a state flag telling ResetCalc to preserve the
		// current value
		m_nState |= HasInitialValue;

	SetDisplayString();
}

//---------------------------------------------------------------------------
// SECCalculator::GetValue
//
//
//@doc SECCalculator
//@mfunc Returns the current calculator value. 
//@comm If the calculator is in the middle
// of an operation then it is unlikely to be the same as the current display
// value. You could force the end of the current calculation with
// HandleEvent(OpEquals).
//@rdesc Returns the currently displayed calculator value.
//@xref <c SECCalculator> <mf SECCalculator::SetValue>
double SECCalculator::GetValue() const
{
	ASSERT_VALID(this);

	return m_val;
}

//---------------------------------------------------------------------------
// SECCalculator::HitTest
//
//
//@doc SECCalculator
//@mfunc Returns the index of the button under the given point
//@rdesc The zero-based index of the button.
//@parm CPoint | point | The given point to hit text (relative to the calculator's
// client area.
//@xref <c SECCalculator>
int SECCalculator::HitTest(CPoint point) const
{
	ASSERT_VALID(this);

	for(int i=0; i<m_btns.GetSize(); i++)
	{
		if(m_btns[i]->GetRect().PtInRect(point))
			return i;
	}

	return -1;
}

//---------------------------------------------------------------------------
// SECCalculator::SetDecimalPlaces
//
//
//@doc SECCalculator
//@mfunc Sets the maximum number of decimal places to display to. 
//@comm The calculator works to 15 significant places. Restricting the number of
// decimal places is a cause of inaccurate results.
//@rdesc void 
//@parm int | nDP | The number of decimal places to display to.
//@xref <c SECCalculator> <mf SECCalculator::GetDecimalPlaces>
void SECCalculator::SetDecimalPlaces(int nDP)
{
	ASSERT_VALID(this);

	if(m_hWnd)
		m_nDP = min(nDP, m_nTotalDigits);
	else
		m_nDP = nDP;
}

//---------------------------------------------------------------------------
// SECCalculator::GetDecimalPlaces
//
//
//@doc SECCalculator 
//@mfunc Returns the current maximum number of decimal places.
//@rdesc The current maximum number of decimal places.
//@xref <c SECCalculator> <mf SECCalculator::SetDecimalPlaces>
int SECCalculator::GetDecimalPlaces() const
{
	ASSERT_VALID(this);

	return m_nDP;
}

//---------------------------------------------------------------------------
// SECCalculator::GetFormatDecimal
//
//
//@doc SECCalculator
//@mfunc Returns the decimal character used by scanf and printf.  The character
// that scanf/printf use is determined by the ANSI C Runtime call to 
// setlocale().  The default decimal character displayed on the calculator 
// is determined by the Win32 call to SetThreadLocale(), and can be different
// from the character used by scanf and printf.
//@rdesc The current decimal character used by the ANSI C Runtime libraries based
// on the current locale.
//@xref <c SECCalculator>
TCHAR SECCalculator::GetFormatDecimal() {

	struct lconv* pLocale = localeconv();
	TCHAR cDecimal = *pLocale->decimal_point;
	
	return cDecimal;
}


/////////////////////////////////////////////////////////////////////////////
// SECCalculator overrides
//

//---------------------------------------------------------------------------
// SECCalculator::CreatePanel
//
//
//@doc SECCalculator
//@mfunc Creates the calculator’s LCD panel in the parent window. 
//@comm Creates and initialises the panel data.
//@rdesc Nonzero if the panel is successfully created; otherwise 0.
//@parm CWnd* | pParentWnd | Pointer to the parent window.
//@xref <c SECCalculator> <mf SECCalculator::CreateBtns>
BOOL SECCalculator::CreatePanel(CWnd* pParentWnd)
{
	// Create Font used by panel
	CFont* pFont = pParentWnd->GetFont();
	if(!pFont)
		pFont = CFont::FromHandle((HFONT) ::GetStockObject(SYSTEM_FONT));

	LOGFONT lf;
	pFont->GetObject(sizeof(lf), &lf);
	lf.lfWeight = FW_NORMAL;
	if(!m_panelFont.CreateFontIndirect(&lf))
		return FALSE;

	m_panelRect.SetRectEmpty();
	
	// Calculate height of panel
	{
		CWindowDC dc(NULL);
		CRect r;
		r.SetRectEmpty();
		CFont* pOldFont = dc.SelectObject(&m_panelFont);
		strDum.LoadString(IDS_SEC_CALC_CHCK);
		int cy = dc.DrawText(strDum, 1, &r, 
							 DT_CALCRECT|DT_EXTERNALLEADING|DT_RIGHT|DT_SINGLELINE);
		dc.SelectObject(pOldFont);

		m_panelRect.bottom = 4 + cy;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::CreateBtns
//
// Initialises button dimensions, and fills the button array (m_btns) with
// button objects.
//
// Note that the m_btns array works left to right from top to bottom.
//
//@doc SECCalculator
//@mfunc Creates the calculator’s buttons in the parent window.
//@rdesc Nonzero if the buttons are successfully created; otherwise 0.
//@parm CWnd* | pParentWnd | Pointer to the parent window.
//@xref <c SECCalculator> <mf SECCalculator::CreatePanel>
BOOL SECCalculator::CreateBtns(CWnd* pParentWnd)
{
        ASSERT(!m_strDecSep.IsEmpty());

	// Initialise dimensions
	m_nVIndent = 1;				// Indent from top/bottom
	m_nHIndent = 4;				// Indent from left/right
	m_nVSpace  = 1;				// Space between button columns
	m_nHSpace  = 0;				// Space between button rows

	// Create Font used by btn
	CFont* pFont = pParentWnd->GetFont();
	if(!pFont)
		pFont = CFont::FromHandle((HFONT) ::GetStockObject(SYSTEM_FONT));

	LOGFONT lf;
	pFont->GetObject(sizeof(lf), &lf);
	lf.lfWeight = FW_BOLD;
	if(!m_btnFont.CreateFontIndirect(&lf))
		return FALSE;

	// Calculate button size
	CSize sz;
	sz = SECConvertDBU(16, 14, (HFONT) m_btnFont.m_hObject);
	m_nBtnWidth  = sz.cx;
	m_nBtnHeight = sz.cy;

	// Create standard button attributes
	SECCalcBtn::Attrib blueTextBtn(0L, RGB(0, 0, 128), 
								   (HFONT) m_btnFont.m_hObject);
	SECCalcBtn::Attrib redTextBtn (0L, RGB(128, 0, 0), 
								   (HFONT) m_btnFont.m_hObject);
	SECCalcBtn* pBtn;

	// First row of buttons ......

		// Btn: Clear
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT1);
		pBtn->Initialise(blueTextBtn, OpC, strDum, 1, TRUE);
		m_btns.Add(pBtn);

		// Btn: Clear Error
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT2);
		pBtn->Initialise(blueTextBtn, OpCE, strDum, _T('E'));
		m_btns.Add(pBtn);

		// Btn: BackSpace
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT3);
		pBtn->Initialise(blueTextBtn, OpBack, strDum, (TCHAR) VK_BACK);
		pBtn->SetKeyDownAccel(VK_DELETE);
		m_btns.Add(pBtn);

		// Btn: Percentage
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT4);
		pBtn->Initialise(blueTextBtn, OpPercent, strDum);
		m_btns.Add(pBtn);

	// Second row of buttons ......

		// Btn: Digit 7
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT5);
		pBtn->Initialise(redTextBtn, Op7, strDum, 1, TRUE);
		m_btns.Add(pBtn);

		// Btn: Digit 8
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT6);
		pBtn->Initialise(redTextBtn, Op8, strDum);
		m_btns.Add(pBtn);
		
		// Btn: Digit 9
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT7);
		pBtn->Initialise(redTextBtn, Op9, strDum);
		m_btns.Add(pBtn);

		// Btn: Divide
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT8);
		pBtn->Initialise(blueTextBtn, OpDivide, strDum);
		m_btns.Add(pBtn);

	// Third row of buttons ......

		// Btn: Digit 4
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT9);
		pBtn->Initialise(redTextBtn, Op4, strDum, 1, TRUE);
		m_btns.Add(pBtn);

		// Btn: Digit 5
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT10);
		pBtn->Initialise(redTextBtn, Op5, strDum);
		m_btns.Add(pBtn);

		// Btn: Digit 6
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT11);
		pBtn->Initialise(redTextBtn, Op6, strDum);
		m_btns.Add(pBtn);

		// Btn: Multiply
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT12);
		pBtn->Initialise(blueTextBtn, OpMultiply, strDum);
		m_btns.Add(pBtn);

	// Fourth row of buttons ......

		// Btn: Digit 1
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT13);
		pBtn->Initialise(redTextBtn, Op1, strDum, 1, TRUE);
		m_btns.Add(pBtn);

		// Btn: Digit 2
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT14);
		pBtn->Initialise(redTextBtn, Op2, strDum);
		m_btns.Add(pBtn);

		// Btn: Digit 3
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT15);
		pBtn->Initialise(redTextBtn, Op3, strDum);
		m_btns.Add(pBtn);

		// Btn: Minus
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT16);
		pBtn->Initialise(blueTextBtn, OpMinus, strDum);
		m_btns.Add(pBtn);

	// Fifth row of buttons ......

		// Btn: Digit 0
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT17);
		pBtn->Initialise(redTextBtn, Op0, strDum, 2, TRUE);
		m_btns.Add(pBtn);

		// Btn: Decimal Point
		pBtn = new SECCalcBtn;
		pBtn->Initialise(redTextBtn, OpDP, m_strDecSep);
		pBtn->SetKeyDownAccel(VK_DECIMAL);
		m_btns.Add(pBtn);

		// Btn: Plus
		pBtn = new SECCalcBtn;
		strDum.LoadString(IDS_SEC_CALC_BUT18);
		pBtn->Initialise(blueTextBtn, OpPlus, strDum);
		m_btns.Add(pBtn);


	// Sixth row of buttons ......

		// Load bitmap for equals sign
		CBitmap bmp;
		if(SECLoadSysColorBitmap(bmp, MAKEINTRESOURCE(IDB_SECCALCEQUALS)))
			m_hEqualsBmp = (HBITMAP) bmp.Detach();
		else
			m_hEqualsBmp = NULL;

		// Setup attributes for equals button
		SECCalcBtn::Attrib equalsBtn(0L, RGB(0, 0, 0), 
									 (HFONT) m_btnFont.m_hObject, 
									 &m_hEqualsBmp);

		// Create equals button
		CString strEnter;
		strEnter.LoadString(IDS_SECCALCENTER);

		pBtn = new SECCalcBtn;
		pBtn->Initialise(equalsBtn, OpEquals, strEnter, _T('='), 4, TRUE);
		pBtn->SetKeyDownAccel(VK_RETURN);
		m_btns.Add(pBtn);

	// End of button creation

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleEvent
//
// Processes events caused by button presses, dispatching the event to an
// appropriate function to handle it.
//
//@doc SECCalculator
//@mfunc Handles an event (e.g., a button click) associated with a 
// calculator function/button.
//@rdesc Nonzero if the event associated with a calculator 
// function/button is successfully handled; otherwise 0.
//@parm SECCalcOp | op | A calculator operation.  See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleEvent(SECCalcOp op)
{
	// Map button events onto functions
	struct
	{
		SECCalcOp op;
		BOOL (SECCalculator::*fn)(SECCalcOp op);
	}
	static const LookupTable[] =
	{
		{ Op0,        HandleDigit        },
		{ Op1,        HandleDigit        },
		{ Op2,        HandleDigit        },
		{ Op3,        HandleDigit        },
		{ Op4,        HandleDigit        },
		{ Op5,        HandleDigit        },
		{ Op6,        HandleDigit        },
		{ Op7,        HandleDigit        },
		{ Op8,        HandleDigit        },
		{ Op9,        HandleDigit        },
		{ OpDP,       HandleDecimalPoint },
		{ OpBack,     HandleBackspace    },
		{ OpPlus,     HandleOperator     },
		{ OpMinus,    HandleOperator     },
		{ OpDivide,   HandleOperator     },
		{ OpMultiply, HandleOperator     },
		{ OpEquals,   HandleOperator     },
		{ OpPercent,  HandleOperator     },
		{ OpC,        HandleClear        },
		{ OpCE,       HandleClearError   }
	};

	ASSERT_VALID(this);
	
	for(int i=0; i<dimensionof(LookupTable); i++)
	{
		if(LookupTable[i].op == op)
			// Dispatch the event
			return (this->*LookupTable[i].fn)(op);
	}
	
	// Event not found ......
	ASSERT(0);

	return FALSE;
}

//---------------------------------------------------------------------------
// SECCalculator::LoadDecSeparator
//
//@doc SECCalculator
//@mfunc Virtual overrideable to load custom decimal separator. By default, loads
// from system resources.
//@rdesc Returns the decimal separator to be used as a string.
//@xref <c SECCalculator>
CString SECCalculator::LoadDecSeparator() {
	CString strDecSep      = _T(".");		// Default decimal separator
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT		// System default decimal separator
		,LOCALE_SDECIMAL
		,strDecSep.GetBuffer(3)				// store in m_strDecSep
		,2);								// maximum Length = 1
	strDecSep.ReleaseBuffer();
	return strDecSep;
}

/////////////////////////////////////////////////////////////////////////////
// SECCalculator implementation
//

//---------------------------------------------------------------------------
// SECCalculator::CalcLayout
//
// By this point the m_btns array has been filled with buttons. These buttons
// are now layed out.
//
//@doc SECCalculator
//@mfunc Lays out the buttons on the calculator based on the given style.
//@rdesc CSize 
//@parm DWORD | dwStyle | The style flags for the calculator. The only flag
// examined in this method is the SEC_CS_DIVIDER style flag.
//@xref <c SECCalculator>
CSize SECCalculator::CalcLayout(DWORD dwStyle)
{
	CSize sz(0, 0);

	int x = m_nHIndent;
	int y = m_nVIndent;

	// Allow for divider
	if(dwStyle & SEC_CS_DIVIDER)
		y += 2 /* Height of divider */ + (nDividerSpc * 2) + m_nVIndent;

	// Position panel
	m_panelRect.OffsetRect(-m_panelRect.left, -m_panelRect.top);	// Origin
	m_panelRect.OffsetRect(x, y);
	y += m_panelRect.Height() + m_nVSpace;
	
	// Position each button in turn
	for(int i=0; i<m_btns.GetSize(); i++)
	{
		SECCalcBtn& btn = *(m_btns[i]);

		// Offset button from last button
		if(btn.GetState() & SECCalcBtn::StartsRow)
		{
			if(i != 0)
			{
				// This button starts a new row.
				y += m_nBtnHeight + m_nVSpace;
				x = m_nHIndent;
			}
		}
		else
			x += m_nHSpace;

		// Position button
		int cx = m_nBtnWidth * btn.GetWidth();
		btn.SetRect(CRect(x, y, x + cx, y + m_nBtnHeight));

		// Move past button
		x += cx;

		sz.cx = max(sz.cx, x);
		sz.cy = max(sz.cy, y + m_nBtnHeight);
	}

	// Set panel width
	m_panelRect.right = sz.cx;
	SizePanel();

	sz.cx += m_nHIndent;
	sz.cy += m_nVIndent;

	return sz;
}

//---------------------------------------------------------------------------
// SECCalculator::SizePanel
//
//
//@doc SECCalculator
//@mfunc Calculate the displayable area of the panel.
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::SizePanel()
{
	ASSERT(!m_strDecSep.IsEmpty());

        int cx = m_panelRect.Width() - 4 /* Allows for borders */;
	
	HDC hDC = ::GetDC(NULL);
	HFONT hFontOld = (HFONT) ::SelectObject(hDC, m_panelFont.m_hObject);
	
	// Remove width of decimal point from total width
	SIZE sz;
	VERIFY(::GetTextExtentPoint(hDC, m_strDecSep, 1, &sz));
	cx -= sz.cx;

	// Now get width of a digit
	strDum.LoadString(IDS_SEC_CALC_EXTPT);
	VERIFY(::GetTextExtentPoint(hDC, strDum, 1, &sz));
	m_nTotalDigits = cx / sz.cx;
	m_nTotalDigits--;				// Allows for a sign.

	m_nDP          = min(m_nTotalDigits, m_nDP);

	::SelectObject(hDC, hFontOld);
	::ReleaseDC(NULL, hDC);
}

//---------------------------------------------------------------------------
// SECCalculator::SetDisplayString
//
//
//@doc SECCalculator
//@mfunc Formats the current value into a string for display.
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::SetDisplayString()
{
	ASSERT_VALID(this);

        if (m_strDecSep.IsEmpty())
	{
		m_strDecSep = LoadDecSeparator();
	}

	TCHAR buf[255];
	FormatNo(buf, m_nDP);

	// We need to validate some of the lengths
revalidate:
	int nIntLen, nFracLen, nLen, nMaxLen;
	LPTSTR ptr = buf;

	if(m_val < 0.0)
		ptr = _tcsinc(ptr);

	if(m_val > -1.0 && m_val < 1.0)
		// For values that have leading zero ("0.xxx") we don't want to count
		// leading zero as significant.
		nMaxLen = min(m_nTotalDigits, nMaxSigFig + 1);
	else
		nMaxLen = min(m_nTotalDigits, nMaxSigFig);

	nLen = lstrlen(buf);
	if((ptr = _tcschr(buf, m_strDecSep.GetAt(0))) != NULL)
	{
		nFracLen = lstrlen(_tcsinc(ptr));
		nIntLen  = nLen - nFracLen - 1;
	}
	else
	{
		nFracLen = 0;
		nIntLen  = nLen;
	}

	if(nIntLen > nMaxLen || nIntLen > nMaxIntSize)
	{
		// Number too large.
		m_nState |= CalcError;
		InvalidatePanel();
		return;
	}

	if((nIntLen + nFracLen) > nMaxLen)
	{
		// We have too many digits ... reduce size of fraction to accomodate
		int nDP = nMaxLen - nIntLen;
		ASSERT(nDP >= 0);
		FormatNo(buf, nDP);

		// The Integer part of the number could have been round to include an
		// additional digit ... ensure it hasn't
		goto revalidate;
	}

	m_strDisplay = buf;
}

//---------------------------------------------------------------------------
// SECCalculator::FormatNo
//
//
//@doc SECCalculator
//@mfunc Formats the number m_val into the given buffer to the given 
// number of decimal places.
//@rdesc void 
//@parm LPTSTR | ptr | Destination string buffer.
//@parm  int | nDP | The number of decimal places to format.
//@xref <c SECCalculator>
void SECCalculator::FormatNo(LPTSTR ptr, int nDP)
{
	ASSERT_VALID(this);
	ASSERT(ptr != NULL);
	ASSERT(nDP >= 0 && nDP <= nMaxSigFig);
        ASSERT(!m_strDecSep.IsEmpty());

	TCHAR fmt[20];
        TCHAR dec;

	_stprintf(fmt, _T("%%.%dlf"), nDP);
	_stprintf(ptr, fmt, m_val);

        // determine the decimal character being used by printf
	dec = GetFormatDecimal();	

	// Strip trailing zeros from fraction
	if((ptr = _tcschr(ptr, dec)) != NULL)
        {
		LPTSTR lpszLast = NULL;

                // replace the printf dec char with current decimal
		_tcsncset(ptr, m_strDecSep.GetAt(0), 1);

		while(*ptr != 0)
		{
			if(*ptr == _T('0'))
			{
				if(lpszLast == NULL)
					lpszLast = ptr;
			}
			else
				lpszLast = NULL;

			ptr = _tcsinc(ptr);
		}

		if(lpszLast != NULL)
			*lpszLast = 0;
	}
}

//---------------------------------------------------------------------------
// SECCalculator::PressBtn
//
//
//@doc SECCalculator
//@mfunc Presses the button at the given index, handles the associated
// operation, and unpresses the button.
//@rdesc void 
//@parm int | nIndex | The index of the button to press.
//@xref <c SECCalculator>
void SECCalculator::PressBtn(int nIndex)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0 && nIndex < m_btns.GetSize());

	// We show the button has been pressed
	m_btns[nIndex]->SetState(m_btns[nIndex]->GetState() | SECCalcBtn::Down);
	InvalidateBtn(nIndex);
	UpdateWindow();

	// Action the key press
	HandleEvent(m_btns[nIndex]->GetOp());

	// And release the button
	m_btns[nIndex]->SetState(m_btns[nIndex]->GetState() ^ SECCalcBtn::Down);
	InvalidateBtn(nIndex);
	UpdateWindow();
}

//---------------------------------------------------------------------------
// SECCalculator::GetClassStyle
//
//
//@doc SECCalculator
//@mfunc Returns the style that the window class "will" be registered with.
//@rdesc Always returns zero.
//@xref <c SECCalculator>
UINT SECCalculator::GetClassStyle() const
{
	return 0;
}

//---------------------------------------------------------------------------
// SECCalculator::Draw
//
//
//@doc SECCalculator
//@mfunc Draws the calculator!
//@rdesc void 
//@parm CDC& | dc | Device context with which to draw.
//@xref <c SECCalculator>
void SECCalculator::Draw(CDC& dc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&dc);

	// Start by filling the background
	CRect client;
	GetClientRect(&client);
	FillSolidRect1(&dc, &client, secData.clrBtnFace);

	if(GetStyle() & SEC_CS_DIVIDER)
	{
		CRect r;
		r.left = client.left + 1;
		r.right = client.right - 1;
		r.top  = client.top + m_nVIndent + nDividerSpc;
		r.bottom = r.top + 2;

		Draw3dRect1(&dc, &r, secData.clrBtnHilite, secData.clrBtnShadow);
	}

	// Draw the panel
	if(dc.RectVisible(m_panelRect))
		DrawPanel(dc);

	// Draw the buttons
	for(int i=0; i<m_btns.GetSize(); i++)
	{
		if(dc.RectVisible(m_btns[i]->GetRect()))
			m_btns[i]->Draw(dc);
	}
}

//---------------------------------------------------------------------------
// SECCalculator::DrawPanel
//
//
//@doc SECCalculator
//@mfunc Draws the LCD display panel.
//@rdesc void 
//@parm CDC& | dc | The device context with which to draw.
//@xref <c SECCalculator>
void SECCalculator::DrawPanel(CDC& dc)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&dc);
        ASSERT(!m_strDecSep.IsEmpty());

	// We draw inset borders.
	CRect r = m_panelRect;
	Draw3dRect1(&dc, r, secData.clrBtnShadow, secData.clrBtnHilite);
	r.InflateRect(-1, -1);
	Draw3dRect1(&dc, r, secData.clrWindowFrame, secData.clrBtnFace);
	r.InflateRect(-1, -1);

	CString temp;
	
	// Determine what should be displayed
	if(m_nState & CalcError)
		// The calculator is in error.
		temp.LoadString(IDS_SECCALCERROR);
	else
	{
		temp = m_strDisplay;
		if(temp.IsEmpty())
		{
			// No current display string, so display zero.
			strDum.LoadString(IDS_SEC_CALC_EXTPT);
			temp = strDum;
			temp += m_strDecSep;
		}
		else if(temp.Find(m_strDecSep) == -1)
			// We always display a decimal place.
			temp += m_strDecSep;
	}

	// and display it
	CFont* pOldFont = dc.SelectObject(&m_panelFont);
	dc.DrawText(temp, temp.GetLength(), r, 
				DT_SINGLELINE|DT_RIGHT|DT_VCENTER|DT_NOPREFIX|DT_EXTERNALLEADING);
	dc.SelectObject(pOldFont);
}


/////////////////////////////////////////////////////////////////////////////
// SECCalculator Calculator function handlers
//

//---------------------------------------------------------------------------
// SECCalculator::HandleDigit
//
//
//@doc SECCalculator
//@mfunc Handles the input of a numeric digit
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp | op | A digit operation code. See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleDigit(SECCalcOp op)
{
	ASSERT_VALID(this);
        ASSERT(!m_strDecSep.IsEmpty());

	if(m_nState & CalcError)
	{
		// When in error we do nothing!
		MessageBeep((UINT) -1);
		return FALSE;
	}

	// Get the character code
	TCHAR c = (TCHAR)(_T('0') + (op - Op0));

	if(m_bReplace)
	{
		// The new digit is to replace the current display string
		m_strDisplay.Empty();
		m_bReplace = FALSE;
	}

	// We ignore the case where leading zeros are entered
	if(!(m_strDisplay.IsEmpty() && op == Op0))
	{
		// Make sure we are not over-running any preset lengths
		LPCTSTR ptr = m_strDisplay;
		if(*ptr == _T('-'))
			// Move past sign - we don't count this
			ptr = _tcsinc(ptr);
		
		int nMaxLen;
		if(*ptr == _T('0'))
			// For values that have leading zero ("0.xxx") we don't want to count
			// leading zero as significant.
			nMaxLen = min(m_nTotalDigits, nMaxSigFig + 1);
		else
			nMaxLen = min(m_nTotalDigits, nMaxSigFig);

		int nLen = lstrlen(ptr);
		int nFracLen = 0;
		ptr = _tcschr(ptr, m_strDecSep.GetAt(0));
		BOOL bHasDP = FALSE;
		if(ptr)
		{
			bHasDP = TRUE;
			ptr = _tcsinc(ptr);
			nFracLen = lstrlen(ptr);
			nLen -= nFracLen + 1;
		}

		// Validate lengths
		if(nFracLen >= m_nDP ||							// No. of DPs
		   (bHasDP == FALSE && nLen >= nMaxIntSize) ||	// Integer size
		   (nLen + nFracLen) >= nMaxLen)				// Total width
			// Adding digit would exceed a max. length.
			MessageBeep((UINT) -1);
		else
			// This character is allowed, so add it to the display string
			m_strDisplay += c;
	}

	InvalidatePanel();

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleDecimalPoint
//
//
//@doc SECCalculator
//@mfunc Handles the input of a decimal point
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp |  | The operation code. See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleDecimalPoint(SECCalcOp /* op */)
{
	ASSERT_VALID(this);
        ASSERT(!m_strDecSep.IsEmpty());

	if(m_nState & CalcError)
	{
		// When in error we do nothing!
		MessageBeep((UINT) -1);
		return FALSE;
	}

	if(m_bReplace)
	{
		// We replace the current display string
		m_strDisplay.Empty();
		m_bReplace = FALSE;
	}

	if(m_strDisplay.Find(m_strDecSep) != -1)
		// Can't have two decimal points
		return FALSE;

	if(m_strDisplay.IsEmpty())
	{
		// Nothing entered yet so we become zero point.
		m_strDisplay += _T('0');
		m_strDisplay += m_strDecSep;
	}
	else
		m_strDisplay += m_strDecSep;

	InvalidatePanel();

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleBackspace
//
//
//@doc SECCalculator
//@mfunc Deletes the last character input.
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp |  | The operation code. See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleBackspace(SECCalcOp /* op */)
{
	ASSERT_VALID(this);

	if(m_nState & CalcError)
	{
		// When in error we do nothing!
		MessageBeep((UINT) -1);
		return FALSE;
	}

	if(m_bReplace || m_strDisplay.IsEmpty())
		// Nothing to delete
		return FALSE;

	if(m_strDisplay.GetLength() == 1)
		// We have deleted the last character
		m_strDisplay.Empty();
	else
		// Remove the right most character
		m_strDisplay = m_strDisplay.Left(m_strDisplay.GetLength()-1);

	InvalidatePanel();

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleOperator
//
//
//@doc SECCalculator
//@mfunc Handles the input of an operator
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp | op | The operation code.  See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleOperator(SECCalcOp op)
{
	ASSERT_VALID(this);

	if(m_nState & CalcError)
	{
		// When in error we do nothing!
		MessageBeep((UINT) -1);
		return FALSE;
	}

	double val = 0.0;
	if(!m_strDisplay.IsEmpty())
	{
                TCHAR dec;
		LPTSTR szDisplay = NULL;
		LPTSTR szTemp    = NULL;
		
		// determine the decimal character being used by scanf/printf
		dec = GetFormatDecimal();

		// replace the displayed decimal with the one used by scanf
		// if it is different
		if (dec != m_strDecSep.GetAt(0))
		{
			szDisplay = m_strDisplay.GetBuffer(3);
			if((szTemp = _tcschr(szDisplay, m_strDecSep.GetAt(0))) != NULL)
			{
				_tcsncset(szTemp, dec, 1);
				m_strDisplay.ReleaseBuffer();
			}
		}

		// Extract the current value from the display string
		if(_stscanf(m_strDisplay, _T("%lf"), &val) != 1)
		{
			ASSERT(0);
			return FALSE;
		}
	}

	if(op == OpPercent)
	{
		// Percentage we handle differently. These are actioned as a 
		// percentage of last value entered (operator must have
		// been '+' or '/').
		if(m_lastOp == OpMultiply || m_lastOp == OpDivide)
			val = (m_val * val) / 100.0;
		else
			val = 0.0;

		op = OpNull;
	}
	else if(m_lastOp != OpNull)
	{
		// Action the operator
		switch(m_lastOp)
		{
		case OpPlus:
			val = m_val + val;
			break;

		case OpMinus:
			val = m_val - val;
			break;

		case OpDivide:
			if(val == 0.0)
				// Divide by zero error
				m_nState |= CalcError;
			else
				val = m_val / val;
			break;

		case OpMultiply:
			val = m_val * val;
			break;

		default:
			ASSERT(0);
			return FALSE;
		}
	}

	if(!(m_nState & CalcError))
	{
		// Calculation was successful. 
		m_val = val;				// New value
		m_bReplace = TRUE;			// Next char. replaces current displayed 
									// value
		m_lastOp = op;				// Store the operator to be actioned on 
									// receipt of the next operator.
		if(m_lastOp == OpEquals)	
			m_lastOp = OpNull;		// We don't store '=' since this is the
									// end of the transaction.

		SetDisplayString();
	}

	InvalidatePanel();

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleClear
//
//
//@doc SECCalculator
//@mfunc Resets the state of the calculator.
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp |  | The operation code. See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleClear(SECCalcOp /* op */)
{
	ASSERT_VALID(this);

	ResetCalc();

	return TRUE;
}

//---------------------------------------------------------------------------
// SECCalculator::HandleClearError
//
//
//@doc SECCalculator
//@mfunc Clears the current transaction (resets display to zero).
//@rdesc Nonzero if the operation was successful; otherwise 0.
//@parm SECCalcOp |  | The operation code. See <md SECCalculator::SECCalcOp>.
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent> <md SECCalculator::SECCalcOp>
BOOL SECCalculator::HandleClearError(SECCalcOp /* op */)
{
	ASSERT_VALID(this);

	if(m_nState & CalcError)
		// We're in error, so reset.
		HandleClear(OpC);
	else
	{
		m_strDisplay.Empty();
		m_bReplace = TRUE;

		InvalidatePanel();
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// SECCalculator message handlers
//

//@doc SECCalculator
//@mfunc Called when Windows or an application makes a request to repaint.
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::OnPaint() 
{
	CPaintDC paintDC(this); // device context for painting

	CRect clientRect;
	GetClientRect(&clientRect);

	// Attempt to create an off-screen DC to draw to.
	CDC memDC;
	CBitmap bmp;
	if(memDC.CreateCompatibleDC(&paintDC) == FALSE ||
	   bmp.CreateCompatibleBitmap(&paintDC, clientRect.Width(), 
								  clientRect.Height()) == FALSE)
	{
		// Failed to create it, so draw directly to the screen
		Draw(paintDC);
	}
	else
	{
		CBitmap* pOldBmp = memDC.SelectObject(&bmp);

		CRect clipRect;
		paintDC.GetClipBox(clipRect);

		// Set the clipping region for the off-screen DC
		CRgn clipRgn;
		clipRgn.CreateRectRgn(clipRect.left, clipRect.top, clipRect.right, 
							  clipRect.bottom);
		memDC.SelectClipRgn(&clipRgn);
		
		// and draw to the off-screen DC
		Draw(memDC);

		// Copy the results to the screen DC. 
		paintDC.BitBlt(clipRect.left, clipRect.top, clipRect.Width(), 
					   clipRect.Height(), &memDC, clipRect.left, 
					   clipRect.top, SRCCOPY);

		memDC.SelectObject(pOldBmp);
	}
}

//@doc SECCalculator
//@mfunc Called when a change is made in the system color setting.
//@comm This method is overridden to account for system color changes.
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::OnSysColorChange() 
{
	// System color change, so reset my current colors and reload any bitmaps.
	secData.UpdateSysColors();

	if(m_hEqualsBmp)
	{
		::DeleteObject(m_hEqualsBmp);
		CBitmap bmp;
		if(SECLoadSysColorBitmap(bmp, MAKEINTRESOURCE(IDB_SECCALCEQUALS)))
			m_hEqualsBmp = (HBITMAP) bmp.Detach();
		else
			m_hEqualsBmp = NULL;
	}

	CWnd::OnSysColorChange();
}

//@doc SECCalculator
//@mfunc Called when the nonclient area needs to be painted.
//@rdesc void 
//@xref <c SECCalculator> <mf SECCalculator::OnPaint>
void SECCalculator::OnNcPaint() 
{
	if(GetStyle() & SEC_CS_RAISEDEDGE)
	{
		// Handle drawing my own border style.
		CDC* pDC = CDC::FromHandle(::GetDCEx(m_hWnd, NULL, 
											 DCX_WINDOW|DCX_CACHE));

		CRect rect;
		GetWindowRect(&rect);
		rect.OffsetRect(-rect.left, -rect.top);

		Draw3dRect1(pDC, rect, secData.clr3DLight, secData.clr3DDkShadow);
		rect.InflateRect(-1, -1);
		Draw3dRect1(pDC, rect, secData.clrBtnHilite, secData.clrBtnShadow);

		ReleaseDC(pDC);
	}
	else
		CWnd::OnNcPaint();
}

//@doc SECCalculator
//@mfunc Called when the size and position of the client area needs 
// to be calculated.
//@parm BOOL | bCalcValidRects | Specifies whether the application should specify 
// which part of the client area contains valid information. 
//@parm NCCALCSIZE_PARAMS FAR* | lpncsp | Points to a NCCALCSIZE_PARAMS data 
// structure that contains information an application can use to calculate 
// the new size and position of the CWnd rectangle (including client area, 
// borders, caption, scroll bars, and so on).
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::OnNcCalcSize(BOOL bCalcValidRects, 
								 NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if(GetStyle() & SEC_CS_RAISEDEDGE)
	{
		// Decrease rectangle to allow for my own border
		if(lpncsp)
			::InflateRect(&lpncsp->rgrc[0], -2, -2);
	}
	else
		CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

//@doc SECCalculator
//@mfunc Called when the user presses the left mouse button.
//@comm This method performs hit testing to detect which button was
// the mouse was over, if any, and redraws the button as appropriate.
//@rdesc void 
//@parm UINT | nFlags | Indicates whether various virtual keys are down. 
//@parm  CPoint | point | Specifies the x- and y-coordinate of the cursor. 
//@xref <c SECCalculator>
void SECCalculator::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this)
	{
		CRect rect;
		GetClientRect(&rect);
		if(rect.PtInRect(point))
			// I have been clicked, so I want focus
			SetFocus();
		else
			return;
	}

	// Determine button hit occured on.
	int nIndex = HitTest(point);
	if(nIndex != -1)
	{
		ASSERT(!(m_nState & Capture));

		// Change the button to down, and set capture so we can tell when it
		// has been released.
		m_nState |= Capture;
		m_nCapture = nIndex;
		SetCapture();
		m_btns[nIndex]->SetState(m_btns[nIndex]->GetState() | SECCalcBtn::Down);
		InvalidateBtn(nIndex);
		UpdateWindow();
	}
	else
		CWnd::OnLButtonDown(nFlags, point);
}

//@doc SECCalculator
//@mfunc Called when the user releases the left mouse button.
//@rdesc void 
//@comm This method performs hit testing to detect which button was
// the mouse was over, if any, and redraws the button as appropriate,
// and then calls <mf SECCalculator::HandleEvent>.
//@parm UINT | nFlags | Indicates whether various virtual keys are down. 
//@parm  CPoint | point | Specifies the x- and y-coordinate of the cursor. 
//@xref <c SECCalculator> <mf SECCalculator::HandleEvent>
void SECCalculator::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_nState & Capture)
	{
		// We did have a button down ... reset my state.
		ReleaseCapture();
		m_nState ^= Capture;

		int nState = m_btns[m_nCapture]->GetState();
		if(nState & SECCalcBtn::Down)
		{
			// Change button to up.
			m_btns[m_nCapture]->SetState(nState ^ SECCalcBtn::Down);
			InvalidateBtn(m_nCapture);
		}

		if(HitTest(point) == m_nCapture)
		{
			// Release occured over button, so fire event
			HandleEvent(m_btns[m_nCapture]->GetOp());
		}

		if(GetSafeHwnd())
			UpdateWindow();
	}
	else
		CWnd::OnLButtonUp(nFlags, point);
}

//@doc SECCalculator
//@mfunc Called when the mouse cursor moves. 
//@comm This method performs hit testing to detect which button was
// the mouse was over, if any, and redraws the button as appropriate.
//@rdesc void 
//@parm UINT | nFlags |Indicates whether various virtual keys are down. 
//@parm  CPoint | point | Specifies the x- and y-coordinate of the cursor.
//@xref <c SECCalculator>
void SECCalculator::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_nState & Capture)
	{
		// We currently are tracking a button press. See if we have changed
		// state (mouse dragged in/out of button).
		int nIndex = HitTest(point);
		int nState = m_btns[m_nCapture]->GetState();
		if((nIndex == m_nCapture && !(nState & SECCalcBtn::Down)) ||
		   (nIndex != m_nCapture && (nState & SECCalcBtn::Down)))
		{
			// Toggle state
			m_btns[m_nCapture]->SetState(nState ^ SECCalcBtn::Down);
			InvalidateBtn(m_nCapture);
			UpdateWindow();
		}
	}
	else
		CWnd::OnMouseMove(nFlags, point);
}

//@doc SECCalculator
//@mfunc Called to inform CWnd to cancel any internal mode. 
//@rdesc void 
//@xref <c SECCalculator>
void SECCalculator::OnCancelMode() 
{
	if(m_nState & Capture)
	{
		// We were tracking a button press ... reset state
		ReleaseCapture();
		m_nState ^= Capture;

		int nState = m_btns[m_nCapture]->GetState();
		if(nState & SECCalcBtn::Down)
		{
			// Button was down, draw it up.
			m_btns[m_nCapture]->SetState(nState ^ SECCalcBtn::Down);
			InvalidateBtn(m_nCapture);
		}
	}

	CWnd::OnCancelMode();
}

//@doc SECCalculator
//@mfunc Override to control the particular types of input to process.
//@rdesc Returns the following flags bitwise or'ed:
//@flag DLGC_WANTMESSAGE \| DLGC_WANTCHARS | (to receive all keyboard input)
//@xref <c SECCalculator>
UINT SECCalculator::OnGetDlgCode() 
{
	// We want to receive all keyboard input.
	return DLGC_WANTMESSAGE|DLGC_WANTCHARS;
}

//@doc SECCalculator
//@mfunc Called when a keystroke translates to a nonsystem character. 
//@rdesc void 
//@parm UINT | nChar | Contains the character code value of the key.
//@parm  UINT | nRepCnt | Contains the repeat count.
//@parm  UINT | nFlags | Contains the scan code, key-transition code, 
// previous key state, and context code.
//@xref <c SECCalculator>
void SECCalculator::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	TCHAR c[3];

	// Handle case of receiving multi-byte character
#ifdef WIN32
	if(m_nState & HaveLead)
	{
		m_nState ^= HaveLead;
		c[0] = m_cLeadByte;
		c[1] = TCHAR(nChar);
		c[2] = TCHAR(NULL);
	}
	else if(_istlead(nChar))
	{
		m_nState |= HaveLead;
		m_cLeadByte = TCHAR(nChar);
		return;
	}
	else
#endif
	{
		c[0] = TCHAR(nChar);
		c[1] = TCHAR(NULL);
	}

	// We always match againt lower case (makes life easier)
#ifdef WIN32
	::CharLower(c);
#else 
	::AnsiLower(c);
#endif

	for(int i=0; i<m_btns.GetSize(); i++)
	{
		if(m_btns[i]->IsAccelerator(c))
		{
			// The char. acts as an accelerator for this button. Press it ...
			PressBtn(i);
			return;
		}
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

//@doc SECCalculator
//@mfunc Called when a nonsystem key is pressed.
//@comm If appropriate, this method simulates clicking a button.
//@rdesc void 
//@parm UINT | nChar | Contains the character code value of the key.
//@parm  UINT | nRepCnt | Contains the repeat count.
//@parm  UINT | nFlags | Contains the scan code, key-transition code, 
// previous key state, and context code.
//@xref <c SECCalculator>
void SECCalculator::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// By requesting all chars in OnGetDlgCode, we also receive the tab key.
	// We want our parent to handle this, so we pass it on.
	if(nChar == VK_TAB && !IsKindOf(RUNTIME_CLASS(SECPopupCalculator)))
	{
		BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE;
		CWnd* pParentWnd = GetParent();
		if(pParentWnd)
			pParentWnd->PostMessage(WM_NEXTDLGCTL, (WPARAM) bShift, 
									(LPARAM) FALSE);

		return;
	}

	for(int i=0; i<m_btns.GetSize(); i++)
	{
		if(m_btns[i]->IsKeyDownAccelerator(nChar))
		{
			// This WM_KEYDOWN char acts as an accelerlator for this button.
			// Press it ....
			PressBtn(i);
			return;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


#ifdef _DEBUG
void SECCalculator::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);

	dc << "m_val          = " << m_val << "\n";
	dc << "m_nState       = " << m_nState << "\n";
	dc << "m_lastOp       = " << (UINT) m_lastOp << "\n";
	dc << "m_strDisplay   = " << m_strDisplay << "\n";
	dc << "m_nDP          = " << m_nDP << "\n";
	dc << "m_nTotalDigits = " << m_nTotalDigits << "\n";
}

void SECCalculator::AssertValid() const
{
	CWnd::AssertValid();

	if(m_hWnd != NULL)
	{
		ASSERT(m_btns.GetSize());
		ASSERT(m_nCapture >= 0 && m_nCapture < m_btns.GetSize());
	}
}
#endif



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  SECPopupCalculator window
//
//  Implements a popup variation of SECCalculator
//

BEGIN_MESSAGE_MAP(SECPopupCalculator, SECCalculator)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(SECPopupCalculator, SECCalculator)


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalculator construction
//

//@doc SECPopupCalculator
//@mfunc Constructs an SECPopupCalculator object.
//@xref <c SECPopupCalculator>
SECPopupCalculator::SECPopupCalculator()
{
	m_bAutoDelete = FALSE;
}

//@doc SECPopupCalculator
//@mfunc SECPopupCalculator destructor.
//@xref <c SECPopupCalculator>
SECPopupCalculator::~SECPopupCalculator()
{
}

//@doc SECPopupCalculator
//@mfunc Creates a popup window for the calculator and attaches it to the 
// SECPopupCalculator object.
//@rdesc BOOL Nonzero if the popup window is successfully created; otherwise 0. 
//@parm  DWORD | dwStyle | Specifies the calculator popup window style 
// attributes
//@flag SEC_CS_RAISEDEDGE | Same look as WS_EX_WINDOWEDGE  except compatible 
// with Win 3.x. <nl>
//@flag SEC_CS_DIVIDER | Divider at top of calculator.
//@flag SEC_PCS_DESTROY_ON_EQUALS | Popup calculator destroyed everytime 
// equals is pressed.
//@parm  const RECT& | ctrlRect | References the RECT structure of the calculator edit control.
//@parm  CWnd* | pParentWnd | The parent window.
//@parm  CCreateContext* | pContext | The create context of the calculator popup window.
//@xref <c SECPopupCalculator> <mf SECPopupCalculator::CreateEx>
BOOL SECPopupCalculator::Create(DWORD dwStyle, const RECT& ctrlRect,
								CWnd* pParentWnd, CCreateContext* pContext)
{
	return CreateEx(0, dwStyle, ctrlRect, pParentWnd, pContext);
}

//@doc SECPopupCalculator
//@mfunc Creates a popup window with extended styles for the calculator and 
// attaches it to the SECPopupCalculator object.
//@rdesc BOOL Nonzero if the window is successfully created; otherwise 0.
//@parm DWORD | dwExStyle | Specifies the calculator window’s extended style 
// attributes.
//@parm  DWORD | dwStyle | Specifies the calculator popup window style 
// attributes:
//@flag SEC_CS_RAISEDEDGE | Same look as WS_EX_WINDOWEDGE  except compatible 
// with Win 3.x.
//@flag SEC_CS_DIVIDER | Divider at top of calculator.
//@flag SEC_PCS_DESTROY_ON_EQUALS | Popup calculator destroyed everytime 
// equals is pressed.
//@parm  const RECT& | ctrlRect | References the RECT structure of the 
// calculator edit control.
//@parm  CWnd* | pParentWnd | The parent window.
//@parm  CCreateContext* | pContext | The create context of the calculator 
// popup window.
//@xref <c SECPopupCalculator> <mf SECPopupCalculator::CreateEx>
BOOL SECPopupCalculator::CreateEx(DWORD dwExStyle, DWORD dwStyle,
								  const RECT& ctrlRect, CWnd* pParentWnd,
								  CCreateContext* pContext)
{
        if (m_strDecSep.IsEmpty())
	{
		m_strDecSep	= LoadDecSeparator(); // Load decimal separator (virtual method)
	}

	if(dwStyle & SEC_CS_RAISEDEDGE)
	{
		// If we our going to use our own border style, ensure we ignore all
		// other border styles
#ifdef WIN32
		dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE);
#endif
		dwStyle   &= ~WS_BORDER;
	}

	// We need to calculate the size of the calculator to determine where we 
	// will place it. This duplicates some of what is done in 
	// SECCalculator::Create ... such is life!

	// Create the calculator button objects
	if(m_btns.GetSize() == 0)
		CreateBtns(pParentWnd);

	// Create LCD panel
	if(m_panelFont.m_hObject == NULL)
		CreatePanel(pParentWnd);

	// We need to calculator our size (so we can create the window rectangle
	// as the correct size)
	CSize sz = CalcLayout(dwStyle);

	// Allow for non-client space
	{
		CRect r;
		r.SetRectEmpty();
		::AdjustWindowRectEx(&r, dwStyle, FALSE, dwExStyle);

		sz.cx += r.Width();
		sz.cy += r.Height();

		if(dwStyle & SEC_CS_RAISEDEDGE)
		{
			sz.cx += 4;
			sz.cy += 4;
		}
	}

	// Determine the working screen area for proper placement
	CRect rectWorkArea;

#ifdef WIN32
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0);	
#else 
	int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
	
	rectWorkArea.SetRect(0,0,cxScreen,cyScreen);
#endif

	int x, y;

	// Position below the ctrl if possible (otherwise above)
	if((ctrlRect.bottom + sz.cy) >= rectWorkArea.bottom)
		y = ctrlRect.top - sz.cy;
	else
		y = ctrlRect.bottom;

	// Align with left edge of ctrl (if space) otherwise right align
	if((ctrlRect.left + sz.cx) >= rectWorkArea.right)
		x = ctrlRect.right - sz.cx;
	else
		x = ctrlRect.left;


	if(SECCalculator::CreateEx(dwExStyle, dwStyle | WS_POPUP, x, y, 
							   pParentWnd, NULL, pContext))
	{
		SetFocus();
		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalculator Overrides
//

//@doc SECPopupCalculator
//@mfunc Handles events related to the calculator’s operations.  Called when 
// a button is pressed on the calculator.
//@rdesc Nonzero if the event associated with a calculator function
// button is successfully handled; otherwise 0.
//@parm SECCalcOp | op | The calculator operation to handle. See <md SECCalculator::SECCalcOp>
//@xref <c SECPopupCalculator> <md SECCalculator::SECCalcOp>
BOOL SECPopupCalculator::HandleEvent(SECCalcOp op)
{
	BOOL bRes = SECCalculator::HandleEvent(op);
	if(op == OpEquals && (GetStyle() & SEC_PCS_DESTROY_ON_EQUALS))
	{
		Notify();

		// Since we are down several layers of function, and calling 
		// WM_DESTROY causes us to be deleted, we post a WM_CLOSE, to
		// destroy us at some later date.
		PostMessage(WM_CLOSE);
	}

	return bRes;
}

//@doc SECPopupCalculator
//@mfunc Handles dispatching the result to interested windows.
//@rdesc void 
//@xref <c SECPopupCalculator>
void SECPopupCalculator::Notify()
{
	// Tell our owner (if we have one) about the new display string
	if(m_hWndOwner && ::IsWindow(m_hWndOwner) && !(m_nState & CalcError))
	{
		if(!m_strDisplay.IsEmpty())
		{
			m_strDisplay.Replace(",",".");
			m_strDisplay.TrimRight('.');
		}
		::SendMessage(m_hWndOwner, WM_SETTEXT, 0, 
					  (LPARAM)(LPCTSTR) m_strDisplay);
	}
/*	extern CValue *pEditValue;
	if(pEditValue)
	{
		pEditValue->SetNumber(m_strDisplay);
	}
*/
}


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalculator implementation
//

//@doc SECPopupCalculator
//@mfunc Returns the style that the window class "will" be registered with.
//@rdesc Always returns CS_SAVEBITS.
//@xref <c SECPopupCalculator>
UINT SECPopupCalculator::GetClassStyle() const
{
	return CS_SAVEBITS;
}


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalculator message handlers
//

//@doc SECPopupCalculator
//@mfunc Called by the default OnNcDestroy member function after the 
// window has been destroyed. 
//@comm This method deletes this SECPopupCalculator instance if the 
// <md SECPopupCalculator::m_bAutoDelete> member evaluates to TRUE.
//@rdesc void 
//@xref <c SECPopupCalculator>
void SECPopupCalculator::PostNcDestroy()
{
	if(m_bAutoDelete)
		delete this;
}

//@doc SECPopupCalculator
//@mfunc Called immediately before losing the input focus. 
//@comm When another window receives the focus, the popup calculator
// is destroyed.
//@rdesc void 
//@parm CWnd* | pNewWnd | Specifies a pointer to the window that receives the 
// input focus (may be NULL or may be temporary).
//@xref <c SECPopupCalculator>
void SECPopupCalculator::OnKillFocus(CWnd* pNewWnd)
{
	if(pNewWnd != this)
	{
		Notify();
		DestroyWindow();
	}
}

//@doc SECPopupCalculator
//@mfunc Called when a nonsystem key is pressed.
//@comm This method simulates button click events as appropriate.
//@rdesc void 
//@parm UINT | nChar | Contains the character code value of the key.
//@parm  UINT | nRepCnt | Contains the repeat count.
//@parm  UINT | nFlags | Contains the scan code, key-transition code, 
// previous key state, and context code.
//@xref <c SECPopupCalculator>
void SECPopupCalculator::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_ESCAPE)
		// Escape kills us
		DestroyWindow();
	else if(nChar == VK_RETURN)
	{
		// Return actions the current operator, then kills us
		HandleOperator(OpEquals);
		Notify();
		DestroyWindow();
	}
	else
		SECCalculator::OnKeyDown(nChar, nRepCnt, nFlags);
}


#ifdef _DEBUG
void SECPopupCalculator::Dump(CDumpContext& dc) const
{
	SECCalculator::Dump(dc);

	dc << "m_hWndOwner = " << m_hWndOwner << "\n";
}

void SECPopupCalculator::AssertValid() const
{
	SECCalculator::AssertValid();

	ASSERT(m_hWndOwner == NULL || ::IsWindow(m_hWndOwner));
}
#endif



///////////////////////////////////////////////////////////////////////////////
// SECConvertDBU
//
//
//@func Converts Dialog Base Units to pixels.
//@rdesc A CSize object containing the size in pixels.
//@parm int | x	| Horizontal size in dialog based units.
//@parm  int | y |Vertical size in dialog based units.
//@parmopt  HFONT | hFont | NULL | Font to use for conversion (uses default
// font if null).
CSize SECConvertDBU(int x, int y, HFONT hFont /* = NULL */)
{
	CSize sz;

	if (hFont != NULL)
	{
		HDC hDC = ::GetDC(NULL);
		HFONT hFontOld = (HFONT)::SelectObject(hDC, hFont);
		TEXTMETRIC tm;

		::GetTextMetrics(hDC, &tm);
		strDum.LoadString(IDS_SEC_CALC_TEXTPT);
		::GetTextExtentPoint(hDC, strDum, 52, &sz);
		sz.cx = (sz.cx + 26) / 52;
		sz.cy = tm.tmHeight + tm.tmExternalLeading;
		::SelectObject(hDC, hFontOld);
		::ReleaseDC(NULL, hDC);
	}
	else
	{
		sz.cx = LOWORD(GetDialogBaseUnits());
		sz.cy = HIWORD(GetDialogBaseUnits());
	}

	// Translate dialog units to pixels
	sz.cx = MulDiv(x, sz.cx, 4);
	sz.cy = MulDiv(y, sz.cy, 8);

	return sz;
}

