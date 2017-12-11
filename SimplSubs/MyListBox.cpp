// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListBox

CMyListBox::CMyListBox()
{
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	//{{AFX_MSG_MAP(CMyListBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers


void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	CString sText;
	GetText(lpDrawItemStruct->itemID, sText);

	COLORREF crWnd = GetSysColor(COLOR_WINDOW); // белый
	COLORREF crTextBlue = GetSysColor(COLOR_HIGHLIGHT); // синий
	COLORREF crText_foc_sel = RGB(255,255,0); // ;желтенький )
	COLORREF crTextBl = GetSysColor(COLOR_BTNTEXT); // черный
	COLORREF crBKColorMenu = GetSysColor(COLOR_MENU); // ;желтенький ) 

	COLORREF crColorText, crColorBk;

	COLORREF crTextRed = RGB(255,0,0); // красный 

	CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);
    
	CRect rect;

	COLORREF br_text_clr_old = dc.GetTextColor();
	COLORREF br_fon_clr_old = dc.GetBkColor();
	crColorText = br_text_clr_old;
	crColorBk = br_fon_clr_old;

	// оставим на потом.....
	bool bListHasFicus = true; //false; // наш список в фокусе?... нет - рисуем селекшионс серым..
	//HWND hFoWnh = ::GetFocus();	if (hFoWnh == m_hWnd){ 		bListHasFicus = true;	}
	//hFoWnh = NULL;

    rect=lpDrawItemStruct->rcItem;
	if (lpDrawItemStruct->itemState & ODS_FOCUS){ 
		crColorText = crTextRed;
		if (lpDrawItemStruct->itemState & ODS_SELECTED) 
		{
			crColorText = crText_foc_sel;
			crColorBk = bListHasFicus? crTextBlue:crBKColorMenu;
		}
	} else {
		if (lpDrawItemStruct->itemState & ODS_SELECTED) 
		{
			//crColorBk = crTextBlue;
			crColorBk = bListHasFicus? crTextBlue:crBKColorMenu;
			crColorText = crWnd;
		}

	}
  	CBrush 	br_fon_clr(crColorBk);    
	dc.FillRect(&rect,&br_fon_clr);
	dc.SetTextColor(crColorText);
	dc.SetBkColor(crColorBk);

	if (lpDrawItemStruct->itemState & ODS_FOCUS){ 
		dc.DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	::InflateRect(&rect, -1, -1);

    dc.DrawText(sText, rect, DT_LEFT);   

	dc.SetTextColor(br_text_clr_old);
	dc.SetBkColor(br_fon_clr_old);

    dc.Detach(); 
}

void CMyListBox::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	
	
}

void CMyListBox::OnSetfocus() 
{
	// TODO: Add your control notification handler code here
	
}
