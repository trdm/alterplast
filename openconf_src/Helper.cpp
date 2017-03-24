// Helper.cpp: implementation of the CHelper class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "Helper.h"
#include "htmlhelp.h"
#include "firedlg.h"
#include "silenser.h"

HINSTANCE CHelper::hHtmlHelp=NULL;
typedef HWND (WINAPI *PHtmlHelp)(HWND,LPCTSTR,UINT,DWORD);

CHelper::CHelper()
{
}

CHelper::~CHelper()
{
}

void CHelper::ShowHelp()
{
	if(!hHtmlHelp)
		hHtmlHelp=LoadLibrary("hhctrl.ocx");
	if(!hHtmlHelp)
	{
		DoMsgLine("Не удалось загрузить hhctrl.ocx",mmRedErr);
		return;
	}
	CString fname=CConfigSvcImpl::m_pTheService->m_BinPath+"openconf.chm";
	DWORD atr=GetFileAttributes(fname);
	if(atr==0xFFFFFFFF || (atr & FILE_ATTRIBUTE_DIRECTORY) || !hHtmlHelp)
	{
		DoMsgLine("Не найден файл openconf.chm",mmRedErr);
		return;
	}
	PHtmlHelp pHtmlHelp=(PHtmlHelp)GetProcAddress(hHtmlHelp,ATOM_HTMLHELP_API_ANSI);
	pHtmlHelp(NULL,fname,HH_DISPLAY_TOPIC,0);
}

void CHelper::ShowAbout()
{
	AfxSetResourceHandle(g_hInst);
	CFireDlg dlg;
	CSilenser::SetSilense(false);
	dlg.DoModal();
	CSilenser::SetSilense(true);
	AfxSetResourceHandle(h1CResource);
}
