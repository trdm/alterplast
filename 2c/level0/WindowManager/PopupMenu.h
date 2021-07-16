// PopupMenu.h: interface for the CPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPMENU_H__F2CEE8B2_014D_11D2_83E4_0000B43382FE__INCLUDED_)
#define AFX_POPUPMENU_H__F2CEE8B2_014D_11D2_83E4_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPopupMenu : public CMenu  
{
public:
	struct ToolBarData	
	{
		// struct for toolbar resource; guess you already know it ;)
		WORD wVersion;
		WORD wWidth;			
		WORD wHeight;			
		WORD wItemCount;		
	};
	struct SpawnItem
	{
		int		iImageIdx;
		int		iCmd;
		TCHAR   cText[128];
	};
	struct ImageItem
	{
		int	iImageIdx;
		int	iCmd;
	};

	CPopupMenu();
	virtual ~CPopupMenu();

	HFONT hMenuBoldFont;
	BOOL CreateBoldFont();

	HFONT	hMenuFont, hGuiFont;
	CSize szImage;
	int FindImageItem(const int cmd);

	SpawnItem* AddSpawnItem(const TCHAR* txt, const int cmd);
	void AddImageItem(const int idx, WORD cmd);

	void RemapMenu(CMenu* pMenu);

	CImageList ilList;
	CImageList ilOther;

	COLORREF crMenuText, crMenuTextSel;
	COLORREF cr3dFace, crMenu, crHighlight, cr3dHilight, cr3dShadow, crGrayText;
	COLORREF m_clrBtnFace, m_clrBtnHilight, m_clrBtnShadow;

	int iSpawnItem;
	SpawnItem** pSpawnItem;

	int iImageItem;
	ImageItem* pImageItem;

	BOOL LoadToolBarResource(UINT resId);
	BOOL AddToolBarResource(UINT resId);

	void EnableMenuItems(CMenu* pMenu, CWnd* pParent);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
	BOOL DrawCheckmark(CDC& dc, const CRect& rc, BOOL bSelected);
};

#endif // !defined(AFX_POPUPMENU_H__F2CEE8B2_014D_11D2_83E4_0000B43382FE__INCLUDED_)
