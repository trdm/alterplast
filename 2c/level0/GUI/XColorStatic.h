// XColorStatic.h  Version 1.0
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

#ifndef XCOLORSTATIC_H
#define XCOLORSTATIC_H

#define X_ALIGNHORZ_LEFT		0
#define X_ALIGNHORZ_CENTER		1
#define X_ALIGNHORZ_RIGHT		2
#define X_ALIGNVERT_TOP			3
#define X_ALIGNVERT_CENTER		4
#define X_ALIGNVERT_BOTTOM		5


/////////////////////////////////////////////////////////////////////////////
// CXColorStatic window

class CXColorStatic : public CStatic
{
	DECLARE_DYNCREATE(CXColorStatic)
// Construction
public:
	CXColorStatic();
	virtual ~CXColorStatic();

// Attributes
public:
	void SetUnderline(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetItalic(BOOL bFlag, BOOL bRedraw = TRUE);
	BOOL SetPlainBorder(BOOL bSet);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect,
						   CWnd* pParentWnd, UINT nID /* = 0xffff */);
	COLORREF GetTextColor();
	void SetBackgroundColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetTextColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetBold(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetFont(LPCTSTR lpszFaceName, int nPointSize, BOOL bRedraw = TRUE);
	void SetFont(LOGFONT * pLogFont, BOOL bRedraw = TRUE);
	void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void SetIcon(HICON hIcon, BOOL bRedraw = TRUE);
	void SetMargins(int x, int y) { m_nXMargin = x; m_nYMargin = y; }
	BOOL SetHorzAlignment(int nAlignment, BOOL bRedraw = FALSE);
	int GetHorzAlignment() const {return m_nHorzAlignment;};

	BOOL SetVertAlignment(int nAlignment, BOOL bRedraw = FALSE);
	int GetVertAlignment() const {return m_nVertAlignment;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorStatic)
protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont		m_font;
	COLORREF	m_rgbText;
	COLORREF	m_rgbBackground;
	CBrush *	m_pBrush;
	BOOL		m_bBold;
	int			m_nXMargin, m_nYMargin;
	HICON		m_hIcon;
	int			m_nHorzAlignment;
	int			m_nVertAlignment;

	int GetFontPointSize(int nHeight);
	int GetFontHeight(int nPointSize);
	// Generated message map functions
protected:
	BOOL m_bItalic;
	int m_nHeight;
	//{{AFX_MSG(CXColorStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOLORSTATIC_H
