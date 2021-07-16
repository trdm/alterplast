// PromptEdit.cpp : implementation file
//

#include "StdAFX.h"
#include "PromptEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPromptEdit

CPromptEdit::CPromptEdit(LPCTSTR lpszPrompt /*= _T("<Enter Text Here>")*/, 
												 bool bRegainPrompt /*= true*/,
												 PromptShowModeEnum PromptShowMode /*= pmGray*/)
:m_strPrompt(lpszPrompt), 
m_bRegainPrompt(bRegainPrompt),
m_PromptShowMode(PromptShowMode)
{
	UpdatePromptShowMode();
}

void CPromptEdit::UpdatePromptShowMode()
{
	switch(m_PromptShowMode)
	{
		case pmNormal:
			m_crForeground = ::GetSysColor(COLOR_WINDOWTEXT);
			break;
		
		case pmHighlight:
			m_crForeground = ::GetSysColor(COLOR_HIGHLIGHT);
			break;

		case pmGray:
			m_crForeground = ::GetSysColor(COLOR_GRAYTEXT);
			break;
		
		case pmHot:
			#if(WINVER >= 0x0500)
			m_crForeground = ::GetSysColor(COLOR_HOTLIGHT);
			#else
			m_crForeground = RGB(0, 0, 255);
			#endif
			break;					
	}	
}

void CPromptEdit::SetPromptText(LPCTSTR lpszPrompt)
{
	ASSERT(lpszPrompt != NULL);
	m_strPrompt = lpszPrompt;
	
	ASSERT(IsWindow(m_hWnd));
	if(!GetPromptTextActive())
		SetWindowText((LPCTSTR)m_strPrompt);
}

void CPromptEdit::SetPromptText(LPCTSTR lpszPrompt, bool bRegainPrompt, PromptShowModeEnum PromptShowMode)
{
	SetPromptText(lpszPrompt);
	m_bRegainPrompt = bRegainPrompt;
	m_PromptShowMode = PromptShowMode;
}

CString CPromptEdit::GetPromptText() const
{
	return m_strPrompt;
}

bool CPromptEdit::GetPromptTextActive()
{	
	CString strWndText;
	GetWindowTextTrimmed(strWndText);
	return (strWndText.CompareNoCase(m_strPrompt) == 0);
}

void CPromptEdit::SetRegainPrompt(bool bRegainPrompt /*= true*/)
{
	m_bRegainPrompt = bRegainPrompt;
}

bool CPromptEdit::GetRegainPrompt() const
{
	return m_bRegainPrompt;
}

void CPromptEdit::SetPromptShowMode(PromptShowModeEnum PromptShowMode)
{
	m_PromptShowMode = PromptShowMode;
	UpdatePromptShowMode();
}

PromptShowModeEnum CPromptEdit::GetPromptShowMode() const
{
	return m_PromptShowMode;
}

void CPromptEdit::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();
	
	ASSERT(IsWindow(m_hWnd));
	SetWindowText((LPCTSTR)m_strPrompt);		
}

int CPromptEdit::GetWindowTextTrimmed(CString& strWindowTextTrimmed)
{	
	ASSERT(IsWindow(m_hWnd));

	if (GetWindowTextLength() == 0)
		return 0;
	
	GetWindowText(strWindowTextTrimmed);
	
	// Get rid of spaces and tabs
	strWindowTextTrimmed.TrimLeft();
	strWindowTextTrimmed.TrimRight(); // Where's Trim? MFC Sucks
	
	return strWindowTextTrimmed.GetLength();
}

//////////////////////////////////////////////////////////////////////
// CPromptEdit message mappings...

BOOL CPromptEdit::OnSetfocus()
{
	if (GetPromptTextActive())
		SetWindowText(_T(""));			
		
	return FALSE; // Continue message routing...
}

BOOL CPromptEdit::OnKillfocus()
{	
	if(GetPromptTextActive())
	{
		SetWindowText(m_strPrompt);
		return FALSE;
	}		

	CString strWndText;
	GetWindowTextTrimmed(strWndText);
	
	if(m_bRegainPrompt && strWndText.IsEmpty())
		SetWindowText(m_strPrompt);
		
	return FALSE; // Continue message routing...
}

void CPromptEdit::OnLButtonDown(UINT nFlags, CPoint point)
{	
	CEdit::OnLButtonDown(nFlags, point);
	OnSetfocus();
}

HBRUSH CPromptEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{	
	if(nCtlColor != CTLCOLOR_EDIT)
		return NULL;
	
	if(::GetFocus() == m_hWnd)
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));	
	else
		pDC->SetTextColor(m_crForeground);
	
	return ::GetSysColorBrush(COLOR_WINDOW);
}

void CPromptEdit::OnSysColorChange() 
{
	CEdit::OnSysColorChange();
	UpdatePromptShowMode();
}


BEGIN_MESSAGE_MAP(CPromptEdit, CEdit)
	//{{AFX_MSG_MAP(CPromptEdit)
	ON_CONTROL_REFLECT_EX(EN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS, OnKillfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
