// tentrydrawing.cpp
#include "stdafx.h"
#include "tentrydrawing.h"
#include "telegroup.h"
#include "resource.h"

COLORREF CTEntryDrawing::m_BkColor;
COLORREF CTEntryDrawing::m_ForeColor;
COLORREF CTEntryDrawing::m_InvertColor;
COLORREF CTEntryDrawing::m_ToolTipColor;
COLORREF CTEntryDrawing::m_ToolTipFColor;

int CTEntryDrawing::m_MaxTextWidth;
int CTEntryDrawing::m_EntryHeight;
int CTEntryDrawing::m_TextHeight;
CPtrArray CTEntryDrawing::m_iconList;


void CTEntryDrawing::Init()
{
	HWND hDesk=GetDesktopWindow();
	HDC hDC=GetDC(hDesk);

	HFONT hOldFont=(HFONT)SelectObject(hDC,GetStockObject(DEFAULT_GUI_FONT));

	TEXTMETRIC tm;
	GetTextMetrics(hDC,&tm);
	m_TextHeight=tm.tmHeight;
	m_EntryHeight=m_TextHeight+2;
	if(m_EntryHeight<16)
		m_EntryHeight=16;
	m_MaxTextWidth=176-GetSystemMetrics(SM_CXFIXEDFRAME)*2-GetSystemMetrics(SM_CXVSCROLL);
	SelectObject(hDC,hOldFont);
	ReleaseDC(hDesk,hDC);

	CString iconPath=pConfSvc->BinDir()+"telepat.icl";
	HINSTANCE hRes=LoadLibraryEx(iconPath,NULL,LOAD_LIBRARY_AS_DATAFILE);
	GetSysColors();

	if(!hRes)
		hRes=hMyInst;
	for(UINT i=IDI_CMATREXP;i<=IDI_STRINGS;i++)
		m_iconList.Add(LoadImage(hRes,(LPCTSTR)i,IMAGE_ICON,16,16,0));
	if(hRes!=hMyInst)
		FreeLibrary(hRes);
}

void CTEntryDrawing::Draw(LPDRAWITEMSTRUCT pds)
{
	CTeleEntry* pEntry=(CTeleEntry*)pds->itemData;
	HDC hdc=pds->hDC;
	HGDIOBJ hOldFont=SelectObject(hdc,GetStockObject(DEFAULT_GUI_FONT));
	CRect rc=pds->rcItem;
	
	DWORD color=m_BkColor;
	SetBkColor(hdc,color);
	ExtTextOut(hdc,0,0,ETO_OPAQUE,&rc,NULL,0,NULL);
	if(!pEntry)
		return;

	if(pds->hwndItem!=(HWND)1)	// not tooltip
	{
		HICON hIcon=(HICON)m_iconList[pEntry->m_pParent->m_Image];
		DrawIconEx(hdc,rc.left,(rc.bottom+rc.top-16)/2,hIcon,0,0,0,NULL,DI_NORMAL);
		rc.left+=18;
	}
	SetTextColor(hdc,m_ForeColor);
	SetBkColor(hdc,m_BkColor);
	if(pds->itemState & ODS_SELECTED)
	{
		DrawFocusRect(pds->hDC,&rc);
		color=m_ForeColor;
		SetTextColor(hdc,m_InvertColor);
	}
	else if(pds->hwndItem==(HWND)1)
		Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
	rc.DeflateRect(1,1);
	SetBkColor(hdc,color);
	ExtTextOut(hdc,0,0,ETO_OPAQUE,&rc,NULL,0,NULL);

	SetBkMode(hdc,TRANSPARENT);
	LPCTSTR txt=pEntry->m_Name;
	TextOut(hdc,rc.left+2,(rc.top+rc.bottom-m_TextHeight)/2,
		txt,strlen(txt));
	SelectObject(hdc,hOldFont);
}

void CTEntryDrawing::GetSysColors()
{
	m_BkColor		= GetSysColor(COLOR_WINDOW);
	m_ForeColor		= GetSysColor(COLOR_WINDOWTEXT);
	m_ToolTipColor	= GetSysColor(COLOR_INFOBK);
	m_ToolTipFColor = GetSysColor(COLOR_INFOTEXT);
	HWND hDesk=GetDesktopWindow();
	HDC hDC=GetDC(hDesk);
	m_InvertColor	= GetNearestColor(hDC,~m_ForeColor & 0xFFFFFF);
	ReleaseDC(hDesk,hDC);
}
