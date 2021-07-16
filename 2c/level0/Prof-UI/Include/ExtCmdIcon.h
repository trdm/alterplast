// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_CMD_ICON_H)
#define __EXT_CMD_ICON_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon
// command icon class

class __PROF_UIS_API CExtCmdIcon
{
public:
	enum e_draw_type_t
	{
		__DRAW_NORMAL = 0,
		__DRAW_DISABLED = 1,
		__DRAW_MONO = 2,

		__DRAW_VAL_MIN = 0,
		__DRAW_VAL_MAX = 2,
	};
private:
	class __PROF_UIS_API BitmapCash
	{
		HBITMAP m_hBitmap;
		COLORREF m_clrTransparent;
		CSize m_sizeBitmap;
	public:
		BitmapCash(
			HBITMAP hBitmap = NULL,
			COLORREF clrTransparent = RGB( 0, 0, 0 ),
			LPCRECT pRectBitmapSrc = NULL
			);
		BitmapCash( const BitmapCash & other );
		~BitmapCash();
		void Empty();
		bool IsEmpty() const;
		void AssignIndirect(
			HBITMAP hBitmap,
			COLORREF clrTransparent,
			LPCRECT pRectBitmapSrc = NULL
			);
		void Serialize( CArchive & ar );
		void AssignFromOther( const BitmapCash & other );
		BitmapCash & operator=( const BitmapCash & other );
		HICON CreateHICON(
			bool bReplaceSysColors = true
			) const;
		CSize GetSize() const;
		COLORREF GetTransparentColor() const;
		HBITMAP GetBitmap() const;
		INT GetColorMap(
			CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
			) const;
		INT GetBitsPerPixel() const;
	}; // class BitmapCash

	class __PROF_UIS_API IconCash
	{
		HICON m_hIcon;
		CSize m_sizeIcon;
	public:
		IconCash();
		IconCash( const IconCash & other );
		~IconCash();
		void Empty();
		bool IsEmpty() const;
		void Serialize( CArchive & ar );
		void AssignFromOther( const IconCash & other );
		IconCash & operator=( const IconCash & other );
		void AssignFromHICON(
			HICON & hIcon,
			bool bCreateCopy
			);
		void RecalcIconOriginalSize();
		HICON GetIcon() const;
		CSize GetSize() const;
		HICON Detach();
		void Paint(
			HDC hDC,
			const POINT & pt,
			e_draw_type_t eDT,
			HBRUSH hBrush
			) const;
		INT GetColorMap(
			CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
			) const;
		INT GetBitsPerPixel() const;
	}; // class IconCash

	BitmapCash m_BitmapCash;
	IconCash m_IconCash;

public:
	CExtCmdIcon(
		HBITMAP hBitmap = NULL,
		COLORREF clrTransparent = RGB( 0, 0, 0 ),
		LPCRECT pRectBitmapSrc = NULL
		);
	CExtCmdIcon(
		HICON & hIcon,
		bool bCreateCopy
		);
	CExtCmdIcon( const CExtCmdIcon & other );
	virtual ~CExtCmdIcon();
	virtual bool IsEmpty() const;
	virtual void Empty();
	virtual void Serialize( CArchive & ar );
	virtual void AssignFromOther( const CExtCmdIcon & other );
	virtual void AssignFromHICON(
		HICON hIcon,
		bool bCreateCopy
		);
	virtual void AssignFromHBITMAP(
		HBITMAP hBitmap = NULL,
		COLORREF clrTransparent = RGB( 0, 0, 0 ),
		LPCRECT pRectBitmapSrc = NULL
		);
	CExtCmdIcon & operator=( const CExtCmdIcon & other );

	virtual CSize GetSize() const;
	operator CSize() const
	{
		return GetSize();
	}

	virtual bool IsBitmapBased() const;
	virtual HBITMAP GetBitmap() const;
	virtual COLORREF GetBitmapTransparentColor() const;
	virtual bool ExtractEditableBitmap(
		HBITMAP & hBitmap,
		COLORREF & clrTransparent,
		CArray <COLORREF,COLORREF> * pArr = NULL, // recommended palette, item 0 is clrTransparent
		COLORREF ** ppClrSurface = NULL // if not NULL - force 32 bit
		) const;

	virtual HICON GetIcon(
		bool bReplaceSysColors = true // for bimap based items
		) const;
	operator HICON()
	{
		return GetIcon();
	}
	virtual HICON DetachAsHICON(
		bool bReplaceSysColors = true // for bimap based items
		);
	virtual void SyncSysColors();
	virtual void OnSysColorChange();
	virtual void OnSettingChange(
		UINT uFlags,
		LPCTSTR lpszSection
		);
	virtual void OnDisplayChange(
		INT nDepthBPP,
		CPoint ptSizes
		);
	virtual void OnThemeChanged(
		WPARAM wParam,
		LPARAM lParam
		);
	virtual void Paint(
		HDC hDC,
		const POINT & pt,
		e_draw_type_t eDT = __DRAW_NORMAL,
		HBRUSH hBrush = NULL
		) const;
	void Paint(
		HDC hDC,
		INT x,
		INT y,
		e_draw_type_t eDT = __DRAW_NORMAL,
		HBRUSH hBrush = NULL
		) const
	{
		Paint(
			hDC,
			CPoint( x, y ),
			eDT,
			hBrush
			);
	}
	virtual INT GetColorMap(
		CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
		) const;
	virtual INT GetBitsPerPixel() const;
}; // class CExtCmdIcon

#endif // __EXT_CMD_ICON_H
