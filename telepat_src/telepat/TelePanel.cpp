// TelePanel.cpp : implementation file
//

#include "stdafx.h"
#include "TelePanel.h"
#include "TEntryDrawing.h"
DWORD GetOSVersion();

CTelePanel::CTelePanel()
{
	DWORD style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_SAVEBITS;
	if(GetOSVersion()>5000)
		style|=(1<<17);
	CreateEx(0,AfxRegisterWndClass(style,::LoadCursor(NULL, IDC_ARROW)),NULL,
		WS_POPUP|WS_CLIPSIBLINGS|WS_DLGFRAME,CRect(0,0,0,0),pMainFrame,0);
}

BEGIN_MESSAGE_MAP(CTelePanel, CWnd)
	//{{AFX_MSG_MAP(CTelePanel)
	ON_WM_NCACTIVATE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTelePanel::OnNcActivate(BOOL bActive) 
{
	pMainFrame->SendMessage(WM_NCACTIVATE,1);
	return CWnd::OnNcActivate(bActive);
}

void CTelePanel::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	CTEntryDrawing::GetSysColors();
}
