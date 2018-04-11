// MenuItem.cpp: implementation of the CMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuItem.h"
#include "RMenuItem.h"

extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;
extern stDefMenuItems theDefMenuItems[];

class CRMenu;

#define GAP 2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuItem::CMenuItem()
{
	// Create main menu
	m_strID.Empty();
	mIsMainMenu = TRUE;
	itType = itMenu;
	m_strCaption.Empty();
	m_strCode1C.Empty();
	m_bEnabled = TRUE;
	m_bCheck = FALSE;
	m_pPictureHolder7 = NULL;

	mMenu.CreateMenu();
}

CMenuItem::CMenuItem(CString strID)
{
	// Если в конструктор передается иднт, то это уже не главное меню
	m_strID = strID;
	mIsMainMenu = FALSE;
	m_bEnabled = TRUE;
	m_bCheck = FALSE;
	m_pPictureHolder7 = NULL;
}

CMenuItem::~CMenuItem()
{
	if(m_pPictureHolder7)
		delete m_pPictureHolder7;

	if(mIsMainMenu)
	{
		CWinApp* pApp = AfxGetApp();
		CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
		CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
		CString strID;
		CMenuItem* pMenuItem;
		
		// Если это главное меню, удаляем добавленные ранее подменю
		for(POSITION pos = mListSubMenu.GetHeadPosition(); pos != NULL; )
		{
			pMenuItem = mListSubMenu.GetNext(pos);
			{
				int i;
				i = pV7Menu1->FindSubMenu(pMenuItem->m_strCaption);
				if(i) pV7Menu1->DeleteMenu(i, MF_BYPOSITION);
				
				i = pV7Menu2->FindSubMenu(pMenuItem->m_strCaption);
				if(i) pV7Menu2->DeleteMenu(i, MF_BYPOSITION);
			}
		};
		theMapIDtoMenuItem.RemoveAll();
	}
	ClearList();
}

CBLPtr<CRMenuItem> CMenuItem::GetContext()
{
    CBLPtr<CRMenuItem> pRMenu;
	pRMenu.Create();
	pRMenu->m_pMenuItem = this;

	return pRMenu;
}

void CMenuItem::ClearMainMenu()
{
	CWinApp* pApp = AfxGetApp();
	CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
	CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);
	CString strID;
	CMenuItem* pMenuItem;

	for(POSITION pos = mListSubMenu.GetHeadPosition(); pos != NULL; )
	{
		pMenuItem = mListSubMenu.GetNext(pos);
		{
			int i;
			i = pV7Menu1->FindSubMenu(pMenuItem->m_strCaption);
			if(i) pV7Menu1->DeleteMenu(i, MF_BYPOSITION);
			
			i = pV7Menu2->FindSubMenu(pMenuItem->m_strCaption);
			if(i) pV7Menu2->DeleteMenu(i, MF_BYPOSITION);
		}
	};

	for(int i=0; i<4; i++)
	{
		CMenu* pMenu;
		int nSubMenu = pV7Menu1->FindSubMenu(theDefMenuItems[i].Name);
		if(nSubMenu>=0)
		{
			pMenu = pV7Menu1->GetSubMenu(nSubMenu);
			
			MENUITEMINFO info;
			info.cbSize = sizeof (MENUITEMINFO);
			info.fMask = MIIM_DATA;
			for(int i = pMenu->GetMenuItemCount()-1; i >= 0; i--)
			{
				pMenu->GetMenuItemInfo(i, &info, TRUE);
				
				if(info.dwItemData == ID_MYFIRSTCOMMAND)
					pMenu->DeleteMenu(i, MF_BYPOSITION);
				
			}
		}
	}
	
	for(i=0; i<4; i++)
	{
		CMenu* pMenu;
		int nSubMenu = pV7Menu2->FindSubMenu(theDefMenuItems[i].Name);
		if(nSubMenu>=0)
		{
			pMenu = pV7Menu2->GetSubMenu(nSubMenu);
			
			MENUITEMINFO info;
			info.cbSize = sizeof (MENUITEMINFO);
			info.fMask = MIIM_DATA;
			for(int i = pMenu->GetMenuItemCount()-1; i >= 0; i--)
			{
				pMenu->GetMenuItemInfo(i, &info, TRUE);
				
				if(info.dwItemData == ID_MYFIRSTCOMMAND)
					pMenu->DeleteMenu(i, MF_BYPOSITION);
				
			}
		}
	}
}

void CMenuItem::ClearList()
{
	CMenuItem* pMenuItem;
	for(POSITION pos = mListSubMenu.GetHeadPosition(); pos != NULL; )
	{
		pMenuItem = mListSubMenu.GetNext(pos);
		delete pMenuItem;
	}
	mListSubMenu.RemoveAll();

	mMenu.DestroyMenu();
	mMapStrIDtoMenuItem.RemoveAll();
}

void CMenuItem::Build(CMenu* pMenu, int PosItems)
{
	// Очищаем карты
	if(mIsMainMenu)
		theMapIDtoMenuItem.RemoveAll();

	mMapStrIDtoMenuItem.RemoveAll();

	// Создаем все заново
	if(pMenu == 0)
	{
		pMenu = &mMenu;
		pMenu->CreateMenu();
	}
	
	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_DATA;             // get the state of the menu item
	info.dwItemData = ID_MYFIRSTCOMMAND;

	CMenuItem* pMenuItem;
	for(POSITION pos = mListSubMenu.GetHeadPosition(); pos != NULL; )
	{
		pMenuItem = mListSubMenu.GetNext(pos);
		pMenuItem->mParentMenuItem = this;
		
		if(pMenuItem->itType == itCommand)
		{
			UINT nIDNewItem = ID_MYFIRSTCOMMAND+1+theMapIDtoMenuItem.GetCount();
			pMenu->InsertMenu(PosItems, MF_BYPOSITION|MF_OWNERDRAW, nIDNewItem, pMenuItem->m_strCaption);

			if(pMenuItem->m_bCheck)
				pMenu->CheckMenuItem(nIDNewItem, MF_CHECKED);

			theMapIDtoMenuItem.SetAt(nIDNewItem, pMenuItem);
			mMapStrIDtoMenuItem.SetAt(pMenuItem->m_strID, pMenuItem);
			pMenuItem->m_nID = nIDNewItem;
		}
		else if(pMenuItem->itType == itMenu)
		{
			pMenuItem->Build();
			pMenu->InsertMenu(PosItems, MF_BYPOSITION|MF_POPUP, (UINT)pMenuItem->mMenu.GetSafeHmenu(), pMenuItem->m_strCaption);
			mMapStrIDtoMenuItem.SetAt(pMenuItem->m_strID, pMenuItem);

			struct MENUITEMINFO_WIN50 : MENUITEMINFO
			{
				HBITMAP  hbmpItem;
			};

			MENUITEMINFO_WIN50 menuItemInfo;
			menuItemInfo.cbSize = sizeof(menuItemInfo);
			menuItemInfo.fMask  = 0x00000080;
			menuItemInfo.hbmpItem = ((HBITMAP) -1);
			SetMenuItemInfo(pMenu->m_hMenu, PosItems, TRUE, &menuItemInfo);
		}
		else if(pMenuItem->itType == itSeparator)
		{
			pMenu->InsertMenu(PosItems, MF_BYPOSITION|MF_SEPARATOR);
			pMenuItem->m_nID = 0;
			mMapStrIDtoMenuItem.SetAt(pMenuItem->m_strID, pMenuItem);
		}

		::SetMenuItemInfo(pMenu->GetSafeHmenu(), PosItems, TRUE, &info);

		PosItems++;
	};


	if(mIsMainMenu)
	{
		CAppFrame* pApp = (CAppFrame*)AfxGetApp();
		CMenu7* pV7Menu1 = (CMenu7*)(((DWORD*)pApp)+0x66);
		CMenu7* pV7Menu2 = (CMenu7*)(((DWORD*)pApp)+0x68);

		for(POSITION pos = mListSubMenu.GetHeadPosition(); pos != NULL; )
		{
			pMenuItem = mListSubMenu.GetNext(pos);
			
			pV7Menu1->InsertMenu(pV7Menu1->GetMenuItemCount()-3, MF_BYPOSITION|MF_POPUP
				, (UINT)pMenuItem->mMenu.GetSafeHmenu(), pMenuItem->m_strCaption);
			pV7Menu2->InsertMenu(pV7Menu2->GetMenuItemCount()-5, MF_BYPOSITION|MF_POPUP
				, (UINT)pMenuItem->mMenu.GetSafeHmenu(), pMenuItem->m_strCaption);
		}

		for(int i=0; i<4; i++)
		{
			CMenu* pMenu;
			int nSubMenu = 0;

			nSubMenu = pV7Menu1->FindSubMenu(theDefMenuItems[i].Name);
			if(nSubMenu>=0)
			{
				pMenu = pV7Menu1->GetSubMenu(nSubMenu);
				theDefMenuItems[i].pMenuItem->Build(pMenu, theDefMenuItems[i].PosInDefMenu);
			};

			nSubMenu = pV7Menu2->FindSubMenu(theDefMenuItems[i].Name);
			if(nSubMenu>=0)
			{
				pMenu = pV7Menu2->GetSubMenu(nSubMenu);
				theDefMenuItems[i].pMenuItem->Build(pMenu, theDefMenuItems[i].PosInDefMenu);
			}
		}
	}
}

void CMenuItem::SetCommand(CString strCaption, CString strCode1C)
{
	itType = itCommand;
	m_strCaption = strCaption;
	m_strCode1C = strCode1C;

	ClearList();

	mMenu.DestroyMenu();
}

void CMenuItem::SetMenu(CString strCaption)
{
	itType = itMenu;
	m_strCaption = strCaption;
	m_strCode1C.Empty();

	ClearList();

	mMenu.DestroyMenu();
	mMenu.CreateMenu();
}

void CMenuItem::SetSeparator()
{
	itType = itSeparator;
	m_strCaption.Empty();
	m_strCode1C.Empty();

	ClearList();

	mMenu.DestroyMenu();
}

CMenuItem* CMenuItem::AddMenuItem(CString strID)
{
	if(itType == itMenu)
	{
		CMenuItem* pMenu = new CMenuItem(strID);
		mListSubMenu.AddTail(pMenu);
		mMapStrIDtoMenuItem.SetAt(strID, pMenu);
		return pMenu;
	}

	return NULL;
}

CMenuItem* CMenuItem::Get(CString strID)
{
	POSITION pos = Find(strID);
	if(pos)
	{
		return mListSubMenu.GetAt(pos);
	}
	else
		return NULL;
}

POSITION CMenuItem::Find(CString strID)
{
	return mListSubMenu.Find(mMapStrIDtoMenuItem[strID]);
}

BOOL CMenuItem::RemoveAt(CString strID)
{
	POSITION pos = Find(strID);
	if(pos)
	{
		mListSubMenu.RemoveAt(pos);
		return TRUE;
	}
	else
		return FALSE;
}

void CMenuItem::DrawItem_OldStyle(LPDRAWITEMSTRUCT lpDIS)
{
	int m_iconX = 16;
	int m_iconY = 15;
	
	ASSERT(lpDIS != NULL);
	CRect rect;
	
	ASSERT(lpDIS->itemData);
	
	UINT state = 0;
	state = lpDIS->itemState;//|ODS_DISABLED;
	
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	COLORREF colorBack = GetSysColor(COLOR_MENU);
	
	CRect RectL(lpDIS->rcItem);
	CRect RectR(lpDIS->rcItem);
	CRect RectSel(lpDIS->rcItem);
	
    RectL.right = RectL.left + m_iconX + 6 + GAP;
    RectR.left  = RectL.right;
	
	{
		CRect rect2;
		BOOL standardflag=FALSE, selectedflag=FALSE, disableflag=FALSE;
		BOOL checkflag=FALSE;
		
		CBrush m_brSelect;
		int nIconNormal=-1;
		
		// set some colors and the font
		m_brSelect.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		
		// draw the colored rectangle portion
		rect.CopyRect(&lpDIS->rcItem);
		rect.OffsetRect (1,1);
		rect2=rect=RectSel;
		
		// draw the up/down/focused/disabled state
		UINT state = lpDIS->itemState;
		CString strText;
		
		nIconNormal = 0;
		strText = m_strCaption;
		
		// Draw the background
		pDC->FillSolidRect (rect,colorBack);
		
		// Draw the selection
		if(state&ODS_SELECTED)
		{
			pDC->FillRect(rect2,&m_brSelect);
		}
		
		// Draw the Bitmap or checkmarks
		CRect IconRect(rect.TopLeft(),CSize(m_iconX,m_iconY));
		
		if(m_bCheck)
		{
			// Здесь рисуем пометку
			IconRect.OffsetRect(2,((rect.Height()-m_iconY)>>1));
			IconRect.InflateRect(2,2);

			COLORREF oldColor;
			if(!m_bEnabled)
			{
				oldColor = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
			}
			else
			{ 
				oldColor = pDC->SetTextColor(::GetSysColor(COLOR_MENUTEXT));
			}
			
			DrawSpecialChar(pDC, IconRect, 98, FALSE);
			
			pDC->SetTextColor(oldColor);
		}
		else if(m_pPictureHolder7)
		{
			// Здесь рисуем иконку
			IconRect.OffsetRect(2,((rect.Height()-m_iconY)>>1));
			CPictureHolder7 * pic = m_pPictureHolder7;

			if(state & ODS_DISABLED)
			{
				CSize size = IconRect.Size();
				CRect rect = CRect(CPoint(0,0), size);

				CWindowDC windowDC(0);
				CDC colorDC;
				colorDC.CreateCompatibleDC(0);
				CBitmap colorBmp;
				colorBmp.CreateCompatibleBitmap(&windowDC, size.cx, size.cy);
				CBitmap *oldColorBmp = colorDC.SelectObject(&colorBmp);

				pic->Render(colorDC.GetSafeHdc(), rect, NULL, NULL);
				IMG Img;
				Img.eImgType = TypeBitmap;
				Img.hBmp = colorBmp;
				HANDLE h = Grayed(&Img, colorDC.GetSafeHdc());
				pDC->DrawState(IconRect.TopLeft(),IconRect.Size(), *(HBITMAP*)&h, DSS_NORMAL, (HBRUSH)NULL);

				// Delete the color DC and bitmap.
				colorDC.SelectObject(oldColorBmp);
				colorDC.DeleteDC();
				colorBmp.DeleteObject();
			}
			else
				pic->Render(pDC->GetSafeHdc(), IconRect, NULL, NULL);

		} 

		// Вот здесь рисуем текст
		if(!strText.IsEmpty())
		{
			COLORREF crText = GetSysColor(COLOR_MENUTEXT);
			
			if(lpDIS->itemState&ODS_SELECTED)
			{
				crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
			}
			
			rect.left += m_iconX + 6;
			CRect rectt(rect.left,rect.top-1,rect.right,rect.bottom-1);
			
			// Find tabs
			CString leftStr,rightStr;
			leftStr.Empty();rightStr.Empty();
			
			int tablocr = strText.ReverseFind(_T('\t'));
			if(tablocr!=-1)
			{
				rightStr = strText.Mid(tablocr+1);
				leftStr = strText.Left(strText.Find(_T('\t')));
				rectt.right -= m_iconX;
			}
			else
			{
				leftStr = strText;
			}
			
			int iOldMode = pDC->SetBkMode( TRANSPARENT);
			// Draw the text in the correct colour:
			UINT nFormat  = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
			UINT nFormatr = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;
			
			CFont fontMenu;
			LOGFONT logFontMenu;
			
			NONCLIENTMETRICS nm = {0};
			nm.cbSize = sizeof (nm);
			VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
			logFontMenu =  nm.lfMenuFont;
			
			fontMenu.CreateFontIndirect (&logFontMenu);
			CFont* pOldFont = pDC->SelectObject(&fontMenu);
			
			if(m_bEnabled)
			{
				pDC->SetTextColor(crText);
				pDC->DrawText (leftStr,rectt,nFormat);
				if(tablocr!=-1)
				{
					pDC->DrawText (rightStr,rectt,nFormatr);
				}
			}
			else
			{
				// Draw the disabled text
				if(!(state & ODS_SELECTED))
				{
					CRect offset = rectt;
					offset.OffsetRect (1,1);
					
					pDC->SetTextColor(GetSysColor(COLOR_BTNHILIGHT));
					pDC->DrawText(leftStr,&offset, nFormat);
					if(tablocr!=-1)
					{
						pDC->DrawText (rightStr,&offset,nFormatr);
					}
					
					pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
					pDC->DrawText(leftStr,rectt, nFormat);
					if(tablocr!=-1)
					{
						pDC->DrawText(rightStr,rectt,nFormatr);
					}
				}
				else
				{
					// And the standard Grey text:
					pDC->SetTextColor(colorBack);
					pDC->DrawText(leftStr,rectt, nFormat);
					if(tablocr!=-1)
					{
						pDC->DrawText (rightStr,rectt,nFormatr);
					}
				}
			}
			pDC->SelectObject(pOldFont);
			pDC->SetBkMode( iOldMode );
		}
		m_brSelect.DeleteObject();
  }
}

void CMenuItem::DrawSpecialChar(CDC* pDC, LPCRECT pRect, TCHAR Sign, BOOL bBold)
{
	//  48 Min
	//  49 Max
	//  50 Restore
	//  98 Checkmark 
	// 105 Bullet
	// 114 Close
	
	CFont MyFont;
	LOGFONT logfont;
	
	CRect rect(pRect) ;
	rect.DeflateRect(2,2);
	
	logfont.lfHeight = -rect.Height();
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = (bBold) ? FW_BOLD:FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	
	_tcscpy(logfont.lfFaceName,_T("Marlett"));
	
	MyFont.CreateFontIndirect (&logfont);
	
	CFont* pOldFont = pDC->SelectObject (&MyFont); 
	int OldMode = pDC->SetBkMode(TRANSPARENT);  
	
	pDC->DrawText (&Sign,1,rect,DT_CENTER|DT_SINGLELINE);
	
	pDC->SetBkMode(OldMode);
	pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////
// create new grayed handle/delete old object
HANDLE CMenuItem::Grayed(IMG *pImg, HDC hDC) {
   HANDLE hReturn = NULL;
   BITMAP bmp = {0};
   ICONINFO ii = {0};
   HBITMAP hBmp = NULL;
   PBITMAPINFO pbmi  = {0};
   WORD wClrBits = 0;

   switch (pImg->eImgType) {
       case TypeBitmap:
           hBmp = pImg->hBmp;
           break;
       case TypeImagelist:
           hBmp = pImg->ImgList.hBmp;
           break;
       case TypeIcon:
           ::GetIconInfo(pImg->hIcon, &ii);
           hBmp = ii.hbmColor;
           break;
   }

   ::GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);
   wClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
   if (wClrBits == 1)
       wClrBits = 1;
   else if (wClrBits <= 4)
       wClrBits = 4;
   else if (wClrBits <= 8)
       wClrBits = 8;
   else if (wClrBits <= 16)
       wClrBits = 16;
   else if (wClrBits <= 24)
       wClrBits = 24;
   else
       wClrBits = 32;

   if (wClrBits != 24)
       pbmi = (PBITMAPINFO)::LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(1<<wClrBits));
   else
       pbmi = (PBITMAPINFO)::LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

   pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   pbmi->bmiHeader.biWidth = bmp.bmWidth;
   pbmi->bmiHeader.biHeight = bmp.bmHeight;
   pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
   pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

   if (wClrBits < 24)
       pbmi->bmiHeader.biClrUsed = (1<<wClrBits);

   pbmi->bmiHeader.biCompression = BI_RGB;
   pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth*wClrBits+31)&~31)/8*pbmi->bmiHeader.biHeight;
   PBYTE pBits = (PBYTE)::LocalAlloc(LPTR, pbmi->bmiHeader.biSizeImage);
   ::GetDIBits(hDC, hBmp, 0, pbmi->bmiHeader.biHeight, pBits, pbmi, DIB_RGB_COLORS);

   BYTE bR, bG, bB, bH;
   int iArraySize = pbmi->bmiHeader.biSizeImage/sizeof(PIXEL);
   PIXEL *ppx = (PIXEL*)pBits;

   for (int i = 0; i < iArraySize; i++) {
       bR = ppx[i].r;
       bG = ppx[i].g;
       bB = ppx[i].b;
       bH = BYTE(bR*0.299 + bG*0.587 + bB*0.114);
       ppx[i].r = ppx[i].g = ppx[i].b = bH;
   }

   hBmp = ::CreateDIBitmap(hDC, &pbmi->bmiHeader, CBM_INIT, pBits, pbmi, DIB_RGB_COLORS);

   switch (pImg->eImgType) {
       case TypeBitmap:
           hReturn = (HANDLE)hBmp;
           ::DeleteObject((HGDIOBJ)pImg->hBmp);
           break;
       case TypeIcon:
           ::DeleteObject((HGDIOBJ)ii.hbmColor);
           ii.hbmColor = hBmp;
           hReturn = (HANDLE)::CreateIconIndirect(&ii);
           ::DeleteObject((HGDIOBJ)ii.hbmColor);
           ::DeleteObject((HGDIOBJ)ii.hbmMask);
           ::DestroyIcon(pImg->hIcon);
           break;
       case TypeImagelist:
           hReturn = (HANDLE)ImageList_Create(pImg->ImgList.cx, pImg->ImgList.cy, pImg->ImgList.uFlag, 16, 8);
           ImageList_Add((HIMAGELIST)hReturn, hBmp, pImg->ImgList.hMask);
           ::DeleteObject((HGDIOBJ)hBmp);
           ::DeleteObject((HGDIOBJ)pImg->ImgList.hBmp);
           ::DeleteObject((HGDIOBJ)pImg->ImgList.hMask);
           ImageList_Destroy(pImg->ImgList.hImgListOld);
           break;
   }

   if (pbmi) {
       ::LocalFree(pbmi);
       pbmi = NULL;
   }

   if (pBits) {
       ::LocalFree(pBits);
       pBits = NULL;
   }

   return hReturn;
}
