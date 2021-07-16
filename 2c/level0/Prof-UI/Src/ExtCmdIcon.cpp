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

#include "StdAfx.h"

#if (!defined __EXT_CMD_ICON_H)
	#include <ExtCmdIcon.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon::BitmapCash

CExtCmdIcon::BitmapCash::BitmapCash(
	HBITMAP hBitmap, //  = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
	: m_hBitmap( NULL )
	, m_clrTransparent( RGB( 0, 0, 0 ) )
	, m_sizeBitmap( 0, 0 )
{
	AssignIndirect(
		hBitmap,
		clrTransparent,
		pRectBitmapSrc
		);
}

CExtCmdIcon::BitmapCash::BitmapCash(
	const BitmapCash & other
	)
	: m_hBitmap( NULL )
	, m_clrTransparent( RGB( 0, 0, 0 ) )
	, m_sizeBitmap( 0, 0 )
{
	AssignFromOther( other );
}

CExtCmdIcon::BitmapCash::~BitmapCash()
{
	Empty();
}

void CExtCmdIcon::BitmapCash::Empty()
{
	if( m_hBitmap == NULL )
	{
		ASSERT( m_sizeBitmap.cx == 0 && m_sizeBitmap.cy == 0 );
		return;
	}
	ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
	m_sizeBitmap.cx = m_sizeBitmap.cy = 0;
	::DeleteObject( m_hBitmap );
	m_hBitmap = NULL;
	m_clrTransparent = RGB( 0, 0, 0 );
}

bool CExtCmdIcon::BitmapCash::IsEmpty() const
{
	return ( m_hBitmap == NULL ) ? true : false;
}

void CExtCmdIcon::BitmapCash::AssignIndirect(
	HBITMAP hBitmap,
	COLORREF clrTransparent,
	LPCRECT pRectBitmapSrc // = NULL
	)
{
	if( hBitmap == NULL )
	{
		Empty();
		ASSERT( m_sizeBitmap.cx == 0 && m_sizeBitmap.cy == 0 );
		return;
	}
	if(		m_hBitmap == hBitmap
		&&	pRectBitmapSrc == NULL
		)
	{
		ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
		m_clrTransparent = clrTransparent;
		return;
	}
HBITMAP hBitmapNew =
		CExtPaintManager::stat_CloneBitmap(
			hBitmap,
			pRectBitmapSrc
			);
	if( hBitmapNew == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	Empty();
	m_hBitmap = hBitmapNew;
	m_clrTransparent = clrTransparent;
BITMAP bmpSrcDescription;
	::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
	::GetObject( m_hBitmap, sizeof(BITMAP), &bmpSrcDescription );
	ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );
	m_sizeBitmap.cx = bmpSrcDescription.bmWidth;
	m_sizeBitmap.cy = bmpSrcDescription.bmHeight;
}

void CExtCmdIcon::BitmapCash::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		ASSERT( !IsEmpty() );
		VERIFY(
			CExtPaintManager::stat_SerializeBitmap(
				ar,
				&m_hBitmap
				)
			);
		ar << m_clrTransparent;
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		ASSERT( m_hBitmap == NULL );
		VERIFY(
			CExtPaintManager::stat_SerializeBitmap(
				ar,
				&m_hBitmap
				)
			);
		ASSERT( m_hBitmap != NULL );
		ar >> m_clrTransparent;
		BITMAP bmpSrcDescription;
		::memset( &bmpSrcDescription, 0, sizeof(BITMAP) );
		::GetObject( m_hBitmap, sizeof(BITMAP), &bmpSrcDescription );
		ASSERT( bmpSrcDescription.bmWidth > 0 && bmpSrcDescription.bmHeight > 0 );
		m_sizeBitmap.cx = bmpSrcDescription.bmWidth;
		m_sizeBitmap.cy = bmpSrcDescription.bmHeight;
		ASSERT( !IsEmpty() );
	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::BitmapCash::AssignFromOther(
	const CExtCmdIcon::BitmapCash & other
	)
{
	AssignIndirect(
		other.m_hBitmap,
		other.m_clrTransparent
		);
}

CExtCmdIcon::BitmapCash &
	CExtCmdIcon::BitmapCash::operator=(
		const CExtCmdIcon::BitmapCash & other
		)
{
	AssignFromOther( other );
	return (*this);
}

HICON CExtCmdIcon::BitmapCash::CreateHICON(
	bool bReplaceSysColors // = true
	) const
{
	if( IsEmpty() )
	{
		ASSERT( FALSE );
		return NULL;
	}
CRect rcClone( CPoint(0,0), m_sizeBitmap );
HICON hIcon =
		CExtPaintManager::stat_HBITMAP2HICON(
			m_hBitmap,
			m_clrTransparent,
			&rcClone,
			bReplaceSysColors
			);
	ASSERT( hIcon != NULL );
	return hIcon;
}

CSize CExtCmdIcon::BitmapCash::GetSize() const
{
	ASSERT( m_sizeBitmap.cx > 0 && m_sizeBitmap.cy > 0 );
	return m_sizeBitmap;
}

COLORREF CExtCmdIcon::BitmapCash::GetTransparentColor() const
{
	return m_clrTransparent;
}

HBITMAP CExtCmdIcon::BitmapCash::GetBitmap() const
{
	ASSERT( m_hBitmap != NULL );
	return (HBITMAP)m_hBitmap;
}

INT CExtCmdIcon::BitmapCash::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
	return
		CExtPaintManager::stat_GetBitmapColorMap(
			(HBITMAP)m_hBitmap,
			_map
			);
}

INT CExtCmdIcon::BitmapCash::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
BITMAP bi;
	::memset( &bi, 0, sizeof(BITMAP) );
	::GetObject( m_hBitmap, sizeof(BITMAP), &bi );
	ASSERT(
			bi.bmWidth == m_sizeBitmap.cx
		&&	bi.bmHeight == m_sizeBitmap.cy
		&&	bi.bmBitsPixel > 0
		);
	return (INT)bi.bmBitsPixel;
}

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon::IconCash
CExtCmdIcon::IconCash::IconCash()
	: m_hIcon( NULL )
	, m_sizeIcon( 0, 0 )
{
}

CExtCmdIcon::IconCash::IconCash(
	const CExtCmdIcon::IconCash & other
	)
	: m_hIcon( NULL )
	, m_sizeIcon( 0, 0 )
{
	AssignFromOther( other );
}

CExtCmdIcon::IconCash::~IconCash()
{
	Empty();
}

void CExtCmdIcon::IconCash::Empty()
{
	if( m_hIcon == NULL )
	{
		ASSERT( m_sizeIcon.cx == 0 && m_sizeIcon.cy == 0 );
		return;
	}
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	::DestroyIcon( m_hIcon );
	m_hIcon = NULL;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
}

bool CExtCmdIcon::IconCash::IsEmpty() const
{
	return (m_hIcon == NULL) ? true : false;
}

void CExtCmdIcon::IconCash::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		ASSERT( !IsEmpty() );
		ASSERT( m_hIcon != NULL );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );

		BYTE nFlags = 0;
		if( ii.fIcon )
			nFlags |= 0x01;
		if( ii.hbmColor != NULL )
			nFlags |= 0x02;
		if( ii.hbmMask != NULL )
			nFlags |= 0x04;
		ar << nFlags;
		ar << ii.xHotspot;
		ar << ii.yHotspot;
		if( ii.hbmColor != NULL )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmColor
					)
				);
		}
		if( ii.hbmMask != NULL )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmMask
					)
				);
		}
		
		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		ASSERT( m_hIcon == NULL );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		BYTE nFlags = 0;
		ar >> nFlags;
		ar >> ii.xHotspot;
		ar >> ii.yHotspot;
		if( nFlags & 0x01 )
			ii.fIcon = TRUE;
		else
			ii.fIcon = TRUE;
		if( nFlags & 0x02 )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmColor
					)
				);
			ASSERT( ii.hbmColor != NULL );
		}
		if( nFlags & 0x04 )
		{
			VERIFY(
				CExtPaintManager::stat_SerializeBitmap(
					ar,
					&ii.hbmMask
					)
				);
			ASSERT( ii.hbmMask != NULL );
		}
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );

		m_hIcon = ::CreateIconIndirect( &ii );
		ASSERT( m_hIcon != NULL );

		m_sizeIcon.cx = (INT)( (BYTE)(ii.xHotspot * 2) );
		m_sizeIcon.cy = (INT)( (BYTE)(ii.yHotspot * 2) );
		if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		{
			BITMAP bi;
			::memset( &bi, 0, sizeof(BITMAP) );
			::GetObject(
				(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
				sizeof(BITMAP),
				&bi
				);
			ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
			m_sizeIcon.cx = (INT)bi.bmWidth;
			m_sizeIcon.cy = (INT)bi.bmHeight;
		} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		ASSERT( m_sizeIcon.cx > 0 );
		ASSERT( m_sizeIcon.cy > 0 );

		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
		if( m_hIcon == NULL )
			m_sizeIcon.cx = m_sizeIcon.cy = 0;

	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::IconCash::AssignFromOther(
	const CExtCmdIcon::IconCash & other
	)
{
	Empty();
	if( other.IsEmpty() )
		return;
	AssignFromHICON( (HICON)other.m_hIcon, true );
}

CExtCmdIcon::IconCash &
	CExtCmdIcon::IconCash::operator=(
		const CExtCmdIcon::IconCash & other
	)
{
	AssignFromOther( other );
	return (*this);
}

void CExtCmdIcon::IconCash::AssignFromHICON(
	HICON & hIcon,
	bool bCreateCopy
	)
{
	if(		(!bCreateCopy)
		&&	m_hIcon != NULL
		&&	m_hIcon == hIcon
		)
	{
		ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
		return;
	}
	Empty();
	ASSERT( IsEmpty() );
	if( hIcon == NULL )
		return;
	if( bCreateCopy )
	{
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
		m_sizeIcon.cx = (INT)( (BYTE)(ii.xHotspot * 2) );
		m_sizeIcon.cy = (INT)( (BYTE)(ii.yHotspot * 2) );
		if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		{
			BITMAP bi;
			::memset( &bi, 0, sizeof(BITMAP) );
			::GetObject(
				(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
				sizeof(BITMAP),
				&bi
				);
			ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
			m_sizeIcon.cx = (INT)bi.bmWidth;
			m_sizeIcon.cy = (INT)bi.bmHeight;
		} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
		ASSERT( m_sizeIcon.cx > 0 );
		ASSERT( m_sizeIcon.cy > 0 );
		m_hIcon = ::CreateIconIndirect( &ii );
		ASSERT( m_hIcon != NULL );
		::DeleteObject( ii.hbmColor );
		::DeleteObject( ii.hbmMask );
		if( m_hIcon == NULL )
			m_sizeIcon.cx = m_sizeIcon.cy = 0;
	} // if( bCreateCopy )
	else
	{
		m_hIcon = hIcon;
		RecalcIconOriginalSize();
		ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	} // else from if( bCreateCopy )
}

void CExtCmdIcon::IconCash::RecalcIconOriginalSize()
{
	if( m_hIcon == NULL )
		return;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
	m_sizeIcon.cx = (BYTE)(ii.xHotspot * 2);
	m_sizeIcon.cy = (BYTE)(ii.yHotspot * 2);
	if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
	{
		BITMAP bi;
		::memset( &bi, 0, sizeof(BITMAP) );
		::GetObject(
			(ii.hbmColor != NULL) ? ii.hbmColor : ii.hbmMask,
			sizeof(BITMAP),
			&bi
			);
		ASSERT( bi.bmWidth > 0 && bi.bmHeight > 0 );
		m_sizeIcon.cx = bi.bmWidth;
		m_sizeIcon.cy = bi.bmHeight;
	} // if( m_sizeIcon.cx == 0 || m_sizeIcon.cy == 0 )
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
}

HICON CExtCmdIcon::IconCash::GetIcon() const
{
	ASSERT( m_hIcon != NULL );
	return (HICON)m_hIcon;
}

CSize CExtCmdIcon::IconCash::GetSize() const
{
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
	return m_sizeIcon;
}

HICON CExtCmdIcon::IconCash::Detach()
{
	ASSERT( m_hIcon != NULL );
	ASSERT( m_sizeIcon.cx > 0 && m_sizeIcon.cy > 0 );
HICON hIcon = m_hIcon;
	m_hIcon = NULL;
	m_sizeIcon.cx = m_sizeIcon.cy = 0;
	return hIcon;
}

void CExtCmdIcon::IconCash::Paint(
	HDC hDC,
	const POINT & pt,
	CExtCmdIcon::e_draw_type_t eDT,
	HBRUSH hBrush
	) const
{
	ASSERT( !IsEmpty() );
	ASSERT( hDC != NULL );
	ASSERT(
			CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MIN <= eDT
		&&	eDT <= CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MAX
		);
UINT nFlags = DST_ICON|DSS_NORMAL;
	if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_DISABLED )
		nFlags |= DSS_DISABLED;
	else
		if( eDT == CExtCmdIcon::e_draw_type_t::__DRAW_MONO )
			nFlags |= DSS_MONO;
	::DrawState(
		hDC,
		hBrush,
		NULL,
		(LPARAM)m_hIcon,
		0,
		pt.x,
		pt.y,
		m_sizeIcon.cx,
		m_sizeIcon.cy,
		nFlags
		);
}

INT CExtCmdIcon::IconCash::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
INT nRetVal = 0;
	if( ii.hbmColor == NULL )
	{
		ASSERT( ii.hbmMask != NULL );
		nRetVal =
			CExtPaintManager::stat_GetBitmapColorMap(
				ii.hbmMask,
				_map
				);
	} // if( ii.hbmColor == NULL )
	else
	{
		nRetVal =
			CExtPaintManager::stat_GetBitmapColorMap(
				ii.hbmColor,
				_map
				);
	} // else from if( ii.hbmColor == NULL )
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
	return nRetVal;
}

INT CExtCmdIcon::IconCash::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
ICONINFO ii;
	::memset( &ii, 0, sizeof(ICONINFO) );
	VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
	ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
	if( ii.hbmColor == NULL )
	{	
		ASSERT( ii.hbmMask != NULL );
		::DeleteObject(ii.hbmMask);
		return 1; // monochrome
	}
BITMAP bi;
	::memset( &bi, 0, sizeof(BITMAP) );
	::GetObject( ii.hbmColor, sizeof(BITMAP), &bi );
	ASSERT( bi.bmBitsPixel > 0 );
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
	return (INT)bi.bmBitsPixel;
}

/////////////////////////////////////////////////////////////////////////////
// CExtCmdIcon
// command icon class

CExtCmdIcon::CExtCmdIcon(
	HBITMAP hBitmap, // = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
	: m_BitmapCash( hBitmap, clrTransparent, pRectBitmapSrc )
{
}

CExtCmdIcon::CExtCmdIcon(
	HICON & hIcon,
	bool bCreateCopy
	)
{
	AssignFromHICON( hIcon, bCreateCopy );
}

CExtCmdIcon::CExtCmdIcon( const CExtCmdIcon & other )
{
	AssignFromOther( other );
}

CExtCmdIcon::~CExtCmdIcon()
{
	Empty();
}

bool CExtCmdIcon::IsEmpty() const
{
	if( m_BitmapCash.IsEmpty() )
		return m_IconCash.IsEmpty();
	return false;
}

void CExtCmdIcon::Empty()
{
	m_BitmapCash.Empty();
	m_IconCash.Empty();
}

void CExtCmdIcon::Serialize( CArchive & ar )
{
	if( ar.IsStoring() )
	{
		if( IsEmpty() )
		{
			BYTE nFlags = 0;
			ar << nFlags;
			return;
		}
		if( IsBitmapBased() )
		{
			BYTE nFlags = 0x01;
			ar << nFlags;
			m_BitmapCash.Serialize( ar );
			return;
		} // if( ar.IsStoring() )
		BYTE nFlags = 0x02;
		ar << nFlags;
		m_IconCash.Serialize( ar );
	} // if( ar.IsStoring() )
	else
	{
		Empty();
		BYTE nFlags = 0;
		ar >> nFlags;
		if( nFlags == 0 )
			return;
		if( nFlags & 0x01 )
			m_BitmapCash.Serialize( ar );
		else if( nFlags & 0x02 )
			m_IconCash.Serialize( ar );
	} // else from if( ar.IsStoring() )
}

void CExtCmdIcon::AssignFromOther( const CExtCmdIcon & other )
{
LPVOID lpvThis = (LPVOID)this;
LPVOID lpvOther = (LPVOID)(&other);
	if( lpvThis == lpvOther )
		return;
	if( other.m_BitmapCash.IsEmpty() )
	{
		m_BitmapCash.Empty();
		m_IconCash.AssignFromOther( other.m_IconCash );
	} // if( other.m_BitmapCash.IsEmpty() )
	else
	{
		m_IconCash.Empty();
		m_BitmapCash.AssignFromOther( other.m_BitmapCash );
	} // else from if( other.m_BitmapCash.IsEmpty() )
}

void CExtCmdIcon::AssignFromHICON(
	HICON hIcon,
	bool bCreateCopy
	)
{
	if( !m_BitmapCash.IsEmpty() )
		m_BitmapCash.Empty();
	m_IconCash.AssignFromHICON( hIcon, bCreateCopy );
}

void CExtCmdIcon::AssignFromHBITMAP(
	HBITMAP hBitmap, // = NULL
	COLORREF clrTransparent, // = RGB( 0, 0, 0 )
	LPCRECT pRectBitmapSrc // = NULL
	)
{
	if( !m_IconCash.IsEmpty() )
		m_IconCash.Empty();
	m_BitmapCash.AssignIndirect(
		hBitmap,
		clrTransparent,
		pRectBitmapSrc
		);
}


CExtCmdIcon & CExtCmdIcon::operator=( const CExtCmdIcon & other )
{
	AssignFromOther( other );
	return (*this);
}

CSize CExtCmdIcon::GetSize() const
{
	if( !m_BitmapCash.IsEmpty() )
		return m_BitmapCash.GetSize();
	if( !m_IconCash.IsEmpty() )
		return m_IconCash.GetSize();
	return CSize( 0, 0 );
}

bool CExtCmdIcon::IsBitmapBased() const
{
	ASSERT( !IsEmpty() );
	if( m_BitmapCash.IsEmpty() )
		return false;
	return true;
}

HBITMAP CExtCmdIcon::GetBitmap() const
{
	ASSERT( IsBitmapBased() );
	return m_BitmapCash.GetBitmap();
}

COLORREF CExtCmdIcon::GetBitmapTransparentColor() const
{
	ASSERT( IsBitmapBased() );
	return m_BitmapCash.GetTransparentColor();
}

bool CExtCmdIcon::ExtractEditableBitmap(
	HBITMAP & hBitmap,
	COLORREF & clrTransparent,
	CArray <COLORREF,COLORREF> * pArr, // = NULL // recommended palette, item 0 is clrTransparent
	COLORREF ** ppClrSurface // = NULL // if not NULL - force 32 bit
	) const
{
	if( IsEmpty() )
		return false;

static COLORREF arrTransparentColors[] =
{ RGB(255,0,255), RGB(0,255,0), RGB(192,192,192), };

	if( IsBitmapBased() )
	{
		CArray <COLORREF,COLORREF> _arr;
		CExtPaintManager::stat_GetBitmapColorArray(
			m_BitmapCash.GetBitmap(),
			_arr,
			true
			);
		ASSERT( _arr.GetSize() >= 1 );
		COLORREF clrFoundTransparent = ((COLORREF)(-1L));
		if( ppClrSurface != NULL )
		{
			for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
			{
				COLORREF clrAnalyze = arrTransparentColors[i];
				bool bAbandone = false;
				for( int j = 0; j < _arr.GetSize(); j++ )
				{
					COLORREF clrPal = _arr[j];
					if( clrAnalyze == clrPal )
					{
						bAbandone = true;
						break;
					} // if( clrAnalyze == clrPal )
				} // for( int j = 0; j < _arr.GetSize(); j++ )
				if( bAbandone )
					continue;
				clrFoundTransparent = clrAnalyze;
				break;
			} // for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
			if( clrFoundTransparent == ((COLORREF)(-1L)) )
			{
				// do not use black/white colors as transparent
				for( DWORD k = 0x00000001; k < 0x00FFFFFE; k++ )
				{
					COLORREF clrAnalyze = ((COLORREF)(k));
					bool bAbandone = false;
					for( int j = 0; j < _arr.GetSize(); j++ )
					{
						COLORREF clrPal = _arr[j];
						if( clrAnalyze == clrPal )
						{
							bAbandone = true;
							break;
						} // if( clrAnalyze == clrPal )
					} // for( int j = 0; j < _arr.GetSize(); j++ )
					if( bAbandone )
						continue;
					clrFoundTransparent = clrAnalyze;
					break;
				} // for( DWORD k = 0x00000001; k < 0x00FFFFFE; k++ )
			} // if( clrFoundTransparent == ((COLORREF)(-1L)) )
		} // if( ppClrSurface != NULL )
		else
		{
			clrFoundTransparent = m_BitmapCash.GetTransparentColor();
		} // else from if( ppClrSurface != NULL )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			ASSERT( false );
			return false;
		}

		hBitmap =
			CExtPaintManager::stat_CloneBitmap(
				m_BitmapCash.GetBitmap(),
				NULL,
				ppClrSurface
				);
		if( hBitmap == NULL )
		{
			ASSERT( FALSE );
			return false;
		}
	
		if( ppClrSurface != NULL )
		{
			COLORREF * pSurf = (*ppClrSurface);
			ASSERT( pSurf != NULL );
			COLORREF clrActualTransparent = m_BitmapCash.GetTransparentColor();
			if( clrActualTransparent != clrFoundTransparent )
			{
				CSize _sizeIcon = GetSize();
				ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
				LONG nIconPixelCount = _sizeIcon.cx * _sizeIcon.cy;
				COLORREF clrQuadSearch =
					RGB(
						GetBValue( clrActualTransparent ),
						GetGValue( clrActualTransparent ),
						GetRValue( clrActualTransparent )
						);
				COLORREF clrQuadReplace =
					RGB(
						GetBValue( clrFoundTransparent ),
						GetGValue( clrFoundTransparent ),
						GetRValue( clrFoundTransparent )
						);
				for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++, pSurf++ )
				{
					if( (*pSurf) == clrQuadSearch )
						(*pSurf) = clrQuadReplace;
				} // for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++, pSurf++ )
			} // if( clrActualTransparent != clrFoundTransparent )
		} // if( ppClrSurface != NULL )

		clrTransparent = clrFoundTransparent;

		if( pArr != NULL )
		{
			pArr->RemoveAll();
			pArr->Append( _arr );
			if( pArr->GetAt(0) != clrTransparent )
			{
				pArr->InsertAt( 0, clrTransparent, 1 );
				for( int i = 1; i < pArr->GetSize(); i++ )
				{
					if( pArr->GetAt(i) == clrTransparent )
					{
						pArr->RemoveAt( i, 1 );
						break;
					} // if( pArr->GetAt(i) == clrTransparent )
				} // for( int i = 1; i < pArr->GetSize(); i++ )
			} // if( pArr->GetAt(0) != clrTransparent )
		} // if( pArr != NULL )
		return true;
	} // if( IsBitmapBased() )
	else
	{
		HICON hIcon = m_IconCash.GetIcon();
		ASSERT( hIcon != NULL );
		CSize _sizeIcon = GetSize();
		ASSERT( _sizeIcon.cx > 0 && _sizeIcon.cy > 0 );
		
		ICONINFO ii;
		::memset( &ii, 0, sizeof(ICONINFO) );
		VERIFY( ::GetIconInfo( hIcon, &ii ) );
		ASSERT( ii.hbmColor != NULL || ii.hbmMask != NULL );
		CBitmap bmpDestroy1, bmpDestroy2;
		if( ii.hbmColor != NULL )
			bmpDestroy1.Attach( ii.hbmColor );
		if( ii.hbmMask != NULL )
			bmpDestroy2.Attach( ii.hbmMask );

		CMap < COLORREF, COLORREF, DWORD, DWORD > _map;
		if( ii.hbmColor == NULL )
		{
			ASSERT( ii.hbmMask != NULL );
			CArray <COLORREF,COLORREF> _arr;
			CExtPaintManager::stat_GetBitmapColorArray(
				ii.hbmMask,
				_arr,
				true
				);
			ASSERT( _arr.GetSize() >= 2 );
			for( int i=0; i < _arr.GetSize(); i++ )
				_map.SetAt( _arr[i], 1 );
		} // if( ii.hbmColor == NULL )
		else
		{
			ASSERT( ii.hbmColor != NULL );
			CArray <COLORREF,COLORREF> _arr;
			CExtPaintManager::stat_GetBitmapColorArray(
				ii.hbmColor,
				_arr,
				true
				);
			ASSERT( _arr.GetSize() >= 1 );
			for( int i=0; i < _arr.GetSize(); i++ )
				_map.SetAt( _arr[i], 1 );
		} // else

		COLORREF clrFoundTransparent = ((COLORREF)(-1L));
		for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
		{
			DWORD dwEntries = 0;
			if( !_map.Lookup( arrTransparentColors[i], dwEntries ) )
			{
				clrFoundTransparent = arrTransparentColors[i];
				break;
			} // if( !_map.Lookup( arrTransparentColors[i], dwEntries ) )
			ASSERT( dwEntries >= 1 );
		} // for( int i = 0; i < sizeof(arrTransparentColors)/(sizeof(arrTransparentColors[0])); i++ )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			// do not use black/white colors as transparent
			for( DWORD j = 0x00000001; j < 0x00FFFFFE; j++ )
			{
				DWORD dwEntries = 0;
				if( !_map.Lookup( ((COLORREF)(j)), dwEntries ) )
				{
					clrFoundTransparent = ((COLORREF)(j));
					break;
				} // if( !_map.Lookup( ((COLORREF)(j)), dwEntries ) )
				ASSERT( dwEntries >= 1 );
			} // for( DWORD j = 0x00000001; j < 0x00FFFFFE; j++ )
		} // if( clrFoundTransparent == ((COLORREF)(-1L)) )
		if( clrFoundTransparent == ((COLORREF)(-1L)) )
		{
			ASSERT( false );
			return false;
		}

		HDC hDC = ::CreateCompatibleDC( NULL );
		if( hDC == NULL )
		{
			ASSERT( FALSE );
			return 0;
		}
		
		LONG nIconPixelCount = _sizeIcon.cx * _sizeIcon.cy;
		BITMAPINFOHEADER bih;
		::memset( &bih, 0, sizeof(BITMAPINFOHEADER) );
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = _sizeIcon.cx;
		bih.biHeight = _sizeIcon.cy;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = nIconPixelCount;
		COLORREF * pOutSurface = NULL;
		HBITMAP hDIB =
			::CreateDIBSection(
				hDC,
				(LPBITMAPINFO)&bih,
				DIB_RGB_COLORS,
				(void**)&pOutSurface,
				NULL,
				NULL
				);
		if( hDIB == NULL )
		{
			ASSERT( FALSE );
			::DeleteDC( hDC );
			return false;
		}
		ASSERT( pOutSurface != NULL );
		COLORREF clrQuadTransparent =
			RGB(
				GetBValue( clrFoundTransparent ),
				GetGValue( clrFoundTransparent ),
				GetRValue( clrFoundTransparent )
				);
		for( LONG nPixel = 0; nPixel < nIconPixelCount ; nPixel++ )
			pOutSurface[nPixel] = clrQuadTransparent;
		HGDIOBJ hOldBitmap = ::SelectObject( hDC, hDIB );

		VERIFY(
			::DrawIconEx(
				hDC, 0, 0, hIcon, _sizeIcon.cx, _sizeIcon.cy,
				0, (HBRUSH)NULL, DI_NORMAL
				)
			);
		if( ii.hbmColor != NULL && ii.hbmMask != NULL )
		{
			::SelectObject( hDC, ii.hbmMask );	
			for( int y = 0; y < _sizeIcon.cy; y++ )
			{
				for( int x = 0; x < _sizeIcon.cx; x++ )
				{
					COLORREF clrMask = ::GetPixel( hDC, x, y );
					if( clrMask == RGB(255,255,255) )
					{
						COLORREF * pClr =
							pOutSurface + nIconPixelCount - 1
							- (y * _sizeIcon.cx)
							- _sizeIcon.cx + x + 1
							;
						*pClr = clrQuadTransparent;
					}
				} // for( int x = 0; x < _sizeIcon.cx; x++ )
			} // for( int y = 0; y < _sizeIcon.cy; y++ )
		} // if( ii.hbmColor != NULL && ii.hbmMask != NULL )

		::SelectObject( hDC, hOldBitmap );
		::DeleteDC( hDC );

		hBitmap = hDIB;
		clrTransparent = clrFoundTransparent;
		if( ppClrSurface != NULL )
			(*ppClrSurface) = pOutSurface;
		if( pArr != NULL )
		{
			ASSERT( _map.GetCount() >= 1 );
			pArr->Add( clrTransparent );
			POSITION pos = _map.GetStartPosition();
			ASSERT( pos != NULL );
			while( pos != NULL )
			{
				DWORD dwEntries = 0;
				COLORREF clr = RGB(0,0,0);
				_map.GetNextAssoc( pos, clr, dwEntries );
				ASSERT( dwEntries > 0 );
				if( clr != clrTransparent )
					pArr->Add( clr );
			} // while( pos != NULL )
		} // if( pArr != NULL )
		return true;

	} // else from if( IsBitmapBased() )
}

HICON CExtCmdIcon::GetIcon(
	bool bReplaceSysColors // = true // for bimap based items
	) const
{
	if( !m_IconCash.IsEmpty() )
	{
		HICON hIcon = m_IconCash.GetIcon();
		ASSERT( hIcon != NULL );
		return hIcon;
	}
HICON hIcon = m_BitmapCash.CreateHICON( bReplaceSysColors );
	ASSERT( hIcon != NULL );
	if( hIcon != NULL )
		((IconCash *)&m_IconCash)->AssignFromHICON( hIcon, false );
	return hIcon;
}

HICON CExtCmdIcon::DetachAsHICON(
	bool bReplaceSysColors // = true // for bimap based items
	)
{
	if( IsEmpty() )
		return NULL;
	if( !m_IconCash.IsEmpty() )
	{
		m_BitmapCash.Empty();
		HICON hIcon = m_IconCash.Detach();
		ASSERT( hIcon != NULL );
		return hIcon;
	}
HICON hIcon = m_BitmapCash.CreateHICON( bReplaceSysColors );
	ASSERT( hIcon != NULL );
	m_BitmapCash.Empty();
	return hIcon;
}

void CExtCmdIcon::SyncSysColors()
{
	if(		IsEmpty()
		||	(! IsBitmapBased())
		)
		return;
	m_IconCash.Empty();
}

void CExtCmdIcon::OnSysColorChange()
{
	SyncSysColors();
}

void CExtCmdIcon::OnSettingChange(
	UINT uFlags,
	LPCTSTR lpszSection
	)
{
	uFlags;
	lpszSection;
	SyncSysColors();
}

void CExtCmdIcon::OnDisplayChange(
	INT nDepthBPP,
	CPoint ptSizes
	)
{
	nDepthBPP;
	ptSizes;
	SyncSysColors();
}

void CExtCmdIcon::OnThemeChanged(
	WPARAM wParam,
	LPARAM lParam
	)
{
	wParam;
	lParam;
	SyncSysColors();
}

void CExtCmdIcon::Paint(
	HDC hDC,
	const POINT & pt,
	CExtCmdIcon::e_draw_type_t eDT, // = __DRAW_NORMAL
	HBRUSH hBrush // = NULL
	) const
{
	ASSERT( !IsEmpty() );
	ASSERT( hDC != NULL );
	ASSERT(
			CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MIN <= eDT
		&&	eDT <= CExtCmdIcon::e_draw_type_t::__DRAW_VAL_MAX
		);
HICON hIcon = GetIcon();
	if( hIcon == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT( !m_IconCash.IsEmpty() );
	m_IconCash.Paint( hDC, pt, eDT, hBrush );
}

INT CExtCmdIcon::GetColorMap(
	CMap < COLORREF, COLORREF, DWORD, DWORD > & _map
	) const
{
	ASSERT( !IsEmpty() );
	if( !m_BitmapCash.IsEmpty() )
		return m_BitmapCash.GetColorMap( _map );
	else
		return m_IconCash.GetColorMap( _map );
}

INT CExtCmdIcon::GetBitsPerPixel() const
{
	ASSERT( !IsEmpty() );
	if( !m_BitmapCash.IsEmpty() )
		return m_BitmapCash.GetBitsPerPixel();
	else
		return m_IconCash.GetBitsPerPixel();
}
