// ToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "Telepat.h"
#include "ToolTip.h"
#include "tentrydrawing.h"
#include "telelist.h"
#include "TeleGroup.h"

CString CToolTip::m_ClassName;
CToolTip::SInit CToolTip::m_init;

/////////////////////////////////////////////////////////////////////////////
// CToolTip

CToolTip::CToolTip(CListBox* pParent)
{
	m_pParent=pParent;
	CWnd* parent=m_pParent;
	if(pParent!=CTeleList::m_pOneList)
		parent=pMainFrame;
    CreateEx(0,m_ClassName,NULL,
        WS_POPUP, 0, 0, 0, 0,
		parent->GetSafeHwnd(),NULL,NULL);
	m_Idx=-1;
}

CToolTip::~CToolTip()
{
}


BEGIN_MESSAGE_MAP(CToolTip, CWnd)
	//{{AFX_MSG_MAP(CToolTip)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTip message handlers

void CToolTip::OnPaint() 
{
    CPaintDC DC(this);
    int nSavedDC = DC.SaveDC();
    CRect ClientRect;
	GetClientRect(ClientRect);


	DRAWITEMSTRUCT DrawItemStruct;
	DrawItemStruct.CtlType = ODT_LISTBOX;
	DrawItemStruct.CtlID = 0;
	DrawItemStruct.itemID = m_Idx;
	DrawItemStruct.itemAction = ODA_DRAWENTIRE;
	DrawItemStruct.hwndItem = (HWND)1;//NULL;//m_pParent->GetSafeHwnd();
	DrawItemStruct.hDC = DC.GetSafeHdc();
	DrawItemStruct.rcItem = ClientRect;
	DrawItemStruct.itemData = m_pParent->GetItemData(m_Idx);
	DrawItemStruct.itemState = (m_pParent->GetSel(m_Idx)?ODS_SELECTED:0)|
		(m_pParent->GetCaretIndex()==m_Idx?ODS_FOCUS:0);
	m_pParent->DrawItem(&DrawItemStruct);
}

void CToolTip::Show(int pos)
{
	if(!m_pParent->IsWindowVisible())
		return;
	if(pos==m_Idx)
	{
		if(IsWindowVisible())
		{
			Invalidate();
			UpdateWindow();
		}
		return;
	}
	ShowWindow(SW_HIDE);
	m_Idx=pos;
	if(m_Idx<0 || m_Idx>=m_pParent->GetCount())
		return;
	CRect rc;
	m_pParent->GetItemRect(pos,rc);
	CTeleEntry* pEntry=(CTeleEntry*)m_pParent->GetItemData(pos);
	CString text=pEntry->m_Name;
	HDC hDC=::GetDC(m_pParent->m_hWnd);

	HGDIOBJ hOldFont=SelectObject(hDC,GetStockObject(DEFAULT_GUI_FONT));
	CSize size;
	GetTextExtentPoint(hDC,text,text.GetLength(),&size);
	SelectObject(hDC,hOldFont);
	::ReleaseDC(m_pParent->m_hWnd,hDC);
	if(size.cx<rc.Width()-20)
	{
		Hide();
		return;
	}
	rc.left=18;
	rc.right=size.cx+25;
	m_pParent->ClientToScreen(rc);
	int cx=GetSystemMetrics(SM_CXFULLSCREEN);
	if(rc.right>cx)
		rc.OffsetRect(cx-rc.right,0);
	MoveWindow(rc);
    SetWindowPos(&wndTopMost,0,0,0,0,
		SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	if(::GetCapture()!=m_pParent->m_hWnd)
		::SetCapture(m_pParent->m_hWnd);
}

void CToolTip::OnSetFocus(CWnd* pOldWnd) 
{
}

void CToolTip::OnLButtonDown(UINT nFlags, CPoint point) 
{
}

void CToolTip::Hide()
{
	ShowWindow(SW_HIDE);
	m_Idx=-1;
	if(::GetCapture()==m_pParent->m_hWnd)
		ReleaseCapture();
}
