// XColorStatic.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
// CXColorStatic

IMPLEMENT_DYNCREATE(CXColorStatic,CStatic)

BEGIN_MESSAGE_MAP(CXColorStatic, CStatic)
	//{{AFX_MSG_MAP(CXColorStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXColorStatic::CXColorStatic()
{
	m_rgbText       = GetSysColor(COLOR_BTNTEXT);
	m_rgbBackground = GetSysColor(COLOR_BTNFACE);
	m_pBrush        = new CBrush(m_rgbBackground);
	m_bBold         = FALSE;
	m_hIcon         = NULL;
	m_nXMargin = m_nYMargin = 0;
	m_nHeight	= 0;
}

BOOL CXColorStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect,
						   CWnd* pParentWnd, UINT nID /* = 0xffff */)
{
	// Look for special CStatic control styles and trace if any.
#ifdef _DEBUG	
	if ((dwStyle & ~(WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_BORDER)) != 0)
		TRACE0("in CXColorStatic::Create : dwStyle contains more styles than permitted. They will be ignored\n");
#endif

	dwStyle&=WS_CHILD|WS_VISIBLE|WS_DISABLED;
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXColorStatic::~CXColorStatic()
{
	TRACE(_T("in CXColorStatic::~CXColorStatic\n"));

	if (m_font.GetSafeHandle())
		m_font.DeleteObject();

	if (m_pBrush)
	{
		m_pBrush->DeleteObject();
		delete m_pBrush;
	}
	m_pBrush = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// GetFontPointSize()
int CXColorStatic::GetFontPointSize(int nHeight)
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nPointSize = MulDiv(nHeight, 72, cyPixelsPerInch);
	if (nPointSize < 0)
		nPointSize = -nPointSize;

	return nPointSize;
}

///////////////////////////////////////////////////////////////////////////////
// GetFontHeight()
int CXColorStatic::GetFontHeight(int nPointSize)
{
	HDC hdc = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hdc);
	int cyPixelsPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY);
	::DeleteDC(hdc);

	int nHeight = -MulDiv(nPointSize, cyPixelsPerInch, 72);

	return nHeight;
}
///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXColorStatic::PreSubclassWindow() 
{
	TRACE(_T("in CXColorStatic::PreSubclassWindow\n"));
	
	// get current font
	CFont* pFont = GetFont();
	if (!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
		if (hFont)
			pFont = CFont::FromHandle(hFont);
	}
	ASSERT(pFont);
	ASSERT(pFont->GetSafeHandle());

	// create the font for this control
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_font.CreateFontIndirect(&lf);
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXColorStatic::OnPaint() 
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
	{
		NMHDR hdr;
		hdr.hwndFrom = m_hWnd;
		hdr.idFrom   = GetDlgCtrlID();
		hdr.code     = NM_CUSTOMDRAW;
        pOwner->SendMessage(WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr);
	}
	CPaintDC dc(this); // device context for painting
	
	dc.SaveDC();

	dc.SetTextColor(m_rgbText);
	dc.SetBkColor(m_rgbBackground);
	dc.SetBkMode(OPAQUE);
	dc.SelectObject(m_pBrush);

	CRect rect;
	GetClientRect(rect); 

	// cannot have both an icon and text

	if (m_hIcon)
	{
		int nIconX = ::GetSystemMetrics(SM_CXICON);
		int nIconY = ::GetSystemMetrics(SM_CYICON);

		rect.left = rect.left + (rect.Width() - nIconX) / 2;
		rect.top = rect.top + (rect.Height() - nIconY) / 2;

		dc.DrawIcon(rect.left, rect.top, m_hIcon);
	}
	else
	{
		dc.SelectObject(&m_font);

		// get static's text
		CString strText = _T("");
		GetWindowText(strText);

		UINT nFormat = 0;
		DWORD dwStyle = GetStyle();

		// set DrawText format from static style settings
		if (m_nHorzAlignment==X_ALIGNHORZ_CENTER)
			nFormat |= DT_CENTER;
		else if (m_nHorzAlignment==X_ALIGNHORZ_LEFT)
			nFormat |= DT_LEFT;
		else if (m_nHorzAlignment==X_ALIGNHORZ_RIGHT)
			nFormat |= DT_RIGHT;

		if (m_nVertAlignment==X_ALIGNVERT_CENTER)	// vertical centering ==> single line only
			nFormat |= DT_VCENTER | DT_SINGLELINE;
		//else if (m_nVertAlignment==X_ALIGNVERT_TOP)
		//	nFormat |= DT_TOP;
		//else if (m_nVertAlignment==X_ALIGNVERT_BOTTOM)
		//	nFormat |= DT_BOTTOM;
		else
			nFormat |= DT_WORDBREAK;

		rect.left += m_nXMargin;
		rect.top  += m_nYMargin;
		dc.DrawText(strText, rect, nFormat);
	}
	
	dc.RestoreDC(-1);
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXColorStatic::OnEraseBkgnd(CDC* pDC) 
{
	CRect cr;
	GetClientRect(cr); 
	pDC->FillRect(&cr, m_pBrush);

	return TRUE; //CStatic::OnEraseBkgnd(pDC);
}

///////////////////////////////////////////////////////////////////////////////
// SetFont
void CXColorStatic::SetFont(LOGFONT *pLogFont, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(pLogFont);
	if (!pLogFont)
		return;

	if (m_font.GetSafeHandle())
		m_font.DeleteObject();

	LOGFONT lf = *pLogFont;

	lf.lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;
	lf.lfHeight = m_nHeight;
	lf.lfItalic = (BYTE)m_bItalic;

	m_font.CreateFontIndirect(&lf);

	if (bRedraw)
		RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// SetFont
void CXColorStatic::SetFont(LPCTSTR lpszFaceName, 
							int nPointSize, 
							BOOL bRedraw /*= TRUE*/)
{
	// null face name is ok - we will use current font

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	if (lpszFaceName == NULL)
	{
		CFont *pFont = GetFont();
		ASSERT(pFont);
		pFont->GetLogFont(&lf);
	}
	else
	{
		_tcsncpy(lf.lfFaceName, lpszFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR)-1);
	}

	m_nHeight = GetFontHeight(nPointSize);
	lf.lfHeight = m_nHeight;

	SetFont(&lf, bRedraw);
}

///////////////////////////////////////////////////////////////////////////////
// SetFont
void CXColorStatic::SetFont(CFont *pFont, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(pFont);
	if (!pFont)
		return;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	pFont->GetLogFont(&lf);

	SetFont(&lf, bRedraw);
}

///////////////////////////////////////////////////////////////////////////////
// SetTextColor
void CXColorStatic::SetTextColor(COLORREF rgb, BOOL bRedraw /*= TRUE*/) 
{ 
	m_rgbText = rgb; 
	if (bRedraw)
		RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// SetBold
void CXColorStatic::SetBold(BOOL bFlag, BOOL bRedraw /*= TRUE*/)
{ 
	m_bBold = bFlag;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	CFont *pFont = GetFont();
	ASSERT(pFont);
	pFont->GetLogFont(&lf);

	lf.lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;

	SetFont(&lf, bRedraw);
}

void CXColorStatic::SetItalic(BOOL bFlag, BOOL bRedraw /*= TRUE*/)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	CFont *pFont = GetFont();
	ASSERT(pFont);
	pFont->GetLogFont(&lf);
	
	m_bItalic   = bFlag;
	lf.lfItalic = (BYTE)bFlag; 

	SetFont(&lf, bRedraw);
}

void CXColorStatic::SetUnderline(BOOL bFlag, BOOL bRedraw /*= TRUE*/)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	CFont *pFont = GetFont();
	ASSERT(pFont);
	pFont->GetLogFont(&lf);

	lf.lfUnderline = (BYTE)bFlag; 

	SetFont(&lf, bRedraw);
}
///////////////////////////////////////////////////////////////////////////////
// SetBackgroundColor
void CXColorStatic::SetBackgroundColor(COLORREF rgb, BOOL bRedraw /*= TRUE*/) 
{ 
	m_rgbBackground = rgb; 
	if (m_pBrush)
	{
		m_pBrush->DeleteObject();
		delete m_pBrush;
	}
	m_pBrush = new CBrush(m_rgbBackground);
	if (bRedraw)
		RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// SetIcon
void CXColorStatic::SetIcon(HICON hIcon, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(hIcon);

	m_hIcon = hIcon;
	if (bRedraw)
		RedrawWindow();
}

COLORREF CXColorStatic::GetTextColor()
{
	return m_rgbText;
}

BOOL CXColorStatic::SetPlainBorder(BOOL bSet)
{
	if (bSet)
		{
		// This odd line is to force the NC area to be invalidated
		ModifyStyleEx(0, WS_EX_STATICEDGE | WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 0);

		ModifyStyle(0, WS_BORDER, 0);
		ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE, 0, SWP_FRAMECHANGED);
		}
	else
		ModifyStyle(WS_BORDER|WS_TABSTOP, 0, SWP_FRAMECHANGED);
	RedrawWindow();
	return TRUE;
}

BOOL CXColorStatic::SetHorzAlignment(int nAlignment, BOOL bRedraw /* = FALSE */)
{
	if(nAlignment!=X_ALIGNHORZ_LEFT && nAlignment!=X_ALIGNHORZ_CENTER && 
		nAlignment!=X_ALIGNHORZ_RIGHT)
	{
		return FALSE;
	}
	m_nHorzAlignment = nAlignment;
	if (bRedraw)
		RedrawWindow();
	return TRUE;
}

BOOL CXColorStatic::SetVertAlignment(int nAlignment, BOOL bRedraw /* = FALSE */)
{
	if(nAlignment!=X_ALIGNVERT_TOP && nAlignment!=X_ALIGNVERT_CENTER && 
		nAlignment!=X_ALIGNVERT_BOTTOM)
	{
		return FALSE;
	}
	m_nVertAlignment = nAlignment;
	if (bRedraw)
		RedrawWindow();
	return TRUE;
}