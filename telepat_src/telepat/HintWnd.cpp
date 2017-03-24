// HintWnd.cpp : implementation file
//

#include "stdafx.h"
#include "telelist.h"
#include "HintWnd.h"
#include "tentrydrawing.h"
#include "TeleGroup.h"
#include "MethComboBox.h"

CString CHintWnd::m_ClassName;
CHintWnd::SInit CHintWnd::m_init;
/////////////////////////////////////////////////////////////////////////////
// CHintWnd

//typedef BOOL (*PSLWA)(HWND hwnd,COLORREF,BYTE,DWORD);

CHintWnd::CHintWnd(CWnd* pList)
{
	CWnd* parent=pList;
	if(pList!=CTeleList::m_pOneList)
		parent=pMainFrame;
	CreateEx(0/*x00080000*/,m_ClassName,NULL,WS_POPUP|WS_BORDER,CRect(0,0,0,0),parent,0);
	/*
	HINSTANCE hUser=GetModuleHandle("user32.dll");
	PSLWA pFunc=(PSLWA)GetProcAddress(hUser,"SetLayeredWindowAttributes");
	if(pFunc)
		pFunc(m_hWnd,0,210,2);
	*/

	m_pList=pList;
	m_timer=0;
	m_list=true;
}

CHintWnd::~CHintWnd()
{
}


BEGIN_MESSAGE_MAP(CHintWnd, CWnd)
	//{{AFX_MSG_MAP(CHintWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHintWnd message handlers

void CHintWnd::Hide(bool delay)
{
	if(m_timer)
		KillTimer(m_timer);
	ShowWindow(SW_HIDE);
	if(delay)
		m_timer=SetTimer(1,500,NULL);
}

void CHintWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CGdiObject* pOld=dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.SetBkMode(TRANSPARENT);
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,CTEntryDrawing::m_ToolTipColor);
	dc.SetTextColor(CTEntryDrawing::m_ToolTipFColor);
	CString txt;
	GetWindowText(txt);
	dc.DrawText(txt,rc,DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL|DT_EXPANDTABS);
	dc.SelectObject(pOld);
}

void CHintWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	m_timer=0;
	UINT GetSelMsg,GetDataMsg,GetTopMsg;
	HWND hParentWnd;
	if(m_list)
	{
		GetSelMsg=LB_GETCURSEL,GetDataMsg=LB_GETITEMDATA,GetTopMsg=LB_GETTOPINDEX;
		hParentWnd=m_pList->GetParent()->m_hWnd;
	}
	else
	{
		GetSelMsg=CB_GETCURSEL,GetDataMsg=CB_GETITEMDATA,GetTopMsg=CB_GETTOPINDEX;
		hParentWnd=((CMethComboBox*)m_pList)->m_hList;
	}

	int pos=m_pList->SendMessage(GetSelMsg);
	if(pos<0)
		return;
	CTeleEntry* pEntry=(CTeleEntry*)m_pList->SendMessage(GetDataMsg,pos);
	CString hint;
	pEntry->GetHint(hint);
	if(hint.IsEmpty())
		return;

	int screenW=GetSystemMetrics(SM_CXFULLSCREEN);
	int screenH=GetSystemMetrics(SM_CYFULLSCREEN);
	CRect rcParent;
	::GetWindowRect(hParentWnd,&rcParent);
	
	int xLeft=rcParent.left-4;
	int xRight=screenW-rcParent.right-4;
	int width=(xLeft>xRight?xLeft:xRight)-10;
	int yPos=(pos-m_pList->SendMessage(GetTopMsg)+1)*CTEntryDrawing::m_EntryHeight+2;
	int yLine=rcParent.top+2+yPos;
	int yUp=yLine-CTEntryDrawing::m_EntryHeight-4;
	int yDown=screenH-yLine;
	int height=(yUp>yDown?yUp:yDown)-10;


	int parentWidth=rcParent.Width();
	CRect rc;
	rc.top=0;
	rc.bottom=height;
	rc.left=0;
	rc.right=width;

	SetWindowText(hint);
	HDC dc=::GetDC(m_hWnd);
	::SelectObject(dc,GetStockObject(DEFAULT_GUI_FONT));
	DrawText(dc,hint,-1,&rc,
		DT_LEFT|DT_CALCRECT|DT_WORDBREAK|DT_EDITCONTROL|DT_EXPANDTABS);
	::ReleaseDC(m_hWnd,dc);
	rc.bottom+=3;
	rc.right+=10;
	rc.OffsetRect(parentWidth-2,yPos);

	if(m_list)
		m_pList->ClientToScreen(rc);
	else
	{
		::ClientToScreen(((CMethComboBox*)m_pList)->m_hList, (LPPOINT)&rc);
		::ClientToScreen(((CMethComboBox*)m_pList)->m_hList, ((LPPOINT)&rc)+1);
	}
	if(rc.right>screenW)
	{
		int w=rc.Width();
		rc.right=rc.left-parentWidth-6;
		rc.left=rc.right-w;
	}
	if(rc.top>screenH)
	{
		int h=rc.Height();
		rc.bottom=rc.top-CTEntryDrawing::m_EntryHeight-4;
		rc.top=rc.bottom-h;
	}
	SetWindowPos(&wndTopMost,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW|SWP_NOACTIVATE);
}
