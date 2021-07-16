//Авторские права - VTOOLS.RU (info@vtools.ru)
// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_bWasLDown=0;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers


void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==9)
	{
		nChar=VK_DOWN;
		if(IsSHIFTpressed())
			nChar=VK_UP;
	}

	if(nChar==VK_DOWN||nChar==VK_UP||nChar==VK_ESCAPE||nChar==VK_RETURN)
	{
		if(GetParent())
			GetParent()->SendMessage(WM_KEYDOWN,(UINT)nChar);
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers
void CMyButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_DOWN||nChar==VK_UP||nChar==VK_ESCAPE||nChar==VK_RETURN)
	{
		if(GetParent())
			GetParent()->SendMessage(WM_KEYDOWN,(UINT)nChar);
		return;
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}


CString GetIDName(CString csStr);
CString GetNormalProcName(CString Str,CString csEventName,CString csIdName);
int GotoModuleProc(CString csIdName,CString Str);

void CMyEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CString Str;
	GetWindowText(Str);
	Str.TrimRight();
	BOOL bSetNewText=0;
	if(Str.IsEmpty())
	{
		bSetNewText=1;
	}
	Str=GetNormalProcName(Str,csEventName,csIdName);
	if(bSetNewText)
		SetWindowText(Str);
	
	if(GotoModuleProc(csIdName,Str))
		return;
	else
		CEdit::OnLButtonDblClk(nFlags, point);
}

void CMyEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bWasLDown)
	{
		m_bWasLDown=0;
		OnLButtonDblClk(nFlags,point);
		return;
	}

	CEdit::OnLButtonDown(nFlags, point);
}

void CMyEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	//m_bWasLDown=0;
	CEdit::OnMouseMove(nFlags, point);
}

BOOL CMyEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::PreCreateWindow(cs);
}
