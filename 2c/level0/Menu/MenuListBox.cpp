// MenuListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MenuListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuListBox

CMenuListBox::CMenuListBox()
{
	nTracking=0;
}

CMenuListBox::~CMenuListBox()
{
	nTracking=0;
}


BEGIN_MESSAGE_MAP(CMenuListBox, CListBox)
	//{{AFX_MSG_MAP(CMenuListBox)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuListBox message handlers

int CMenuListBox::ChooseValue()
{
	nTracking=1;
	// do popup menu message loop
	CWnd *pPrev=SetFocus();
	SetCapture();
	while( nTracking>0 )
	{
		WaitMessage();
		MSG msg;
		while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if(msg.message==WM_KEYDOWN&&msg.wParam==VK_ESCAPE)
			{
				nTracking=-1;
			}
			if( ! ::GetMessage( &msg, NULL, 0, 0 ) )
				break;
			::DispatchMessage( &msg );
		}
	}
	ReleaseCapture();
	if(pPrev)
		pPrev->SetFocus();

	if(nTracking<0)
		return -1;
	else
		return GetCurSel();
}

void CMenuListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(' '==nChar||'\r'==nChar||'\n'==nChar)
	if(nTracking>0)
		nTracking=0;
	
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMenuListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(nTracking>0)
		nTracking=0;
	
	CListBox::OnLButtonUp(nFlags, point);
}

void CMenuListBox::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(nTracking>0)
		nTracking=0;
	
	CListBox::OnRButtonUp(nFlags, point);
}

void CMenuListBox::OnKillFocus(CWnd* pNewWnd) 
{
	if(nTracking>0)
		nTracking=0;

	CListBox::OnKillFocus(pNewWnd);
}
