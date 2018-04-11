// MenuItem.h: interface for the CMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUITEM_H__C0E13F39_FE53_4480_91CE_1EDBD1F89F24__INCLUDED_)
#define AFX_MENUITEM_H__C0E13F39_FE53_4480_91CE_1EDBD1F89F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RMenu.h"
#include "RMenuItem.h"

class CRMenuItem;

enum ItemType {itMenu, itSeparator , itCommand};


class CMenuItem  
{
	//////////////////////////////////////////////////////////////////
	typedef struct _PIXEL {
		BYTE r, g, b, empty;
	} PIXEL;
	
	//////////////////////////////////////////////////////////////////////
	typedef enum _IMGTYPE {
		TypeBitmap = 1,
			TypeIcon,
			TypeImagelist
	} IMGTYPE, *LPIMGTYPE;
	
	//////////////////////////////////////////////////////////////////////
	typedef struct _IMG {
		IMGTYPE eImgType;
		union {
			HBITMAP hBmp;
			HICON   hIcon;
			struct _ImgList {
				int cx;
				int cy;
				HBITMAP hBmp;
				HBITMAP hMask;
				unsigned int uFlag;
				HIMAGELIST hImgListOld;
			} ImgList;
		};
	} IMG;
	

public:
	CMenuItem();
	CMenuItem(CString strID);
	virtual ~CMenuItem();

	void ClearMainMenu();
	void ClearList();
	void Build(CMenu* pMenu = NULL, int PosItems = 0);
	void SetCommand(CString strText, CString strCode1C);
	void SetMenu(CString strText);
	void SetSeparator();
	CMenuItem* AddMenuItem(CString strID);
	CBLPtr<CRMenuItem> GetContext();
	POSITION Find(CString strID);
	BOOL RemoveAt(CString strID);
	CMenuItem* Get(CString strID);
	void DrawItem_OldStyle (LPDRAWITEMSTRUCT lpDIS);
private:
	void DrawSpecialChar(CDC* pDC, LPCRECT pRect, TCHAR Sign, BOOL bBold);
	HANDLE Grayed(IMG *pImg, HDC hDC);

public:
	enum ItemType itType;
	CString m_strID;
	UINT m_nID;
	CString m_strCaption;
	CString m_strCode1C;
	CString m_strQHTML;
	CString m_strTips;
	BOOL m_bEnabled;
	BOOL m_bCheck;
	CPictureHolder7* m_pPictureHolder7;

	CTypedPtrList<CPtrList, CMenuItem*> mListSubMenu;
	CMenu mMenu;
	BOOL mIsMainMenu;
	CMapStringToPtr mMapStrIDtoMenuItem;
	CMenuItem* mParentMenuItem;
};

 
#endif // !defined(AFX_MENUITEM_H__C0E13F39_FE53_4480_91CE_1EDBD1F89F24__INCLUDED_)
