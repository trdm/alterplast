// 
// Stingray Software Extension Classes
// Copyright (C) 1996 Jim Beveridge
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//		Author:		Don			04/02/96
//

// stdafx.h for SEC includes secall.h
#include "stdafx.h"

#ifndef __SLOADBMP_H__
#include "SLoadBmp.h"
#endif

#define SECNOELEMENTS(a)	(sizeof(a)/sizeof(a[0]))

/////////////////////////////////////////////////////////////////////////////
// SECLoadSysColorBitmap
//
// Function to load a bitmap changing it's colors to the current system color
// scheme.
//
//@func Function to load a bitmap, changing it's colors to the current system 
// color scheme.
//@rdesc Returns non-zero if successful; otherwise, zero.
//@parm CBitmap& | bmp | The bitmap object to return.
//@parm  LPCTSTR | lpszName | The name of the bitmap.
BOOL SECLoadSysColorBitmap(CBitmap& bmp, LPCTSTR lpszName)
{
	// Table used to map default system colors onto the system color identifiers.
	static const DWORD sysColorMap[][2] =
	//	      b     g     r
	{{ RGB(0x00, 0x00, 0x00), COLOR_BTNTEXT },
	 { RGB(0x80, 0x80, 0x80), COLOR_BTNSHADOW },
	 { RGB(0xc0, 0xc0, 0xc0), COLOR_BTNFACE },
	 { RGB(0xff, 0xff, 0xff), COLOR_BTNHIGHLIGHT }};

	// Loads the bitmap changing the color table for the current color scheme
	ASSERT(bmp.m_hObject == NULL);

	HINSTANCE hInst = 
#if defined(WIN32) || defined(_AFXDLL)
	AfxFindResourceHandle(lpszName, RT_BITMAP);
#else
	AfxGetResourceHandle();
#endif // WIN32

	HRSRC hRsrc = ::FindResource(hInst, lpszName, RT_BITMAP);
	if(hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlb = ::LoadResource(hInst, hRsrc);
	if(hGlb == NULL)
		return FALSE;

	LPBITMAPINFOHEADER lpBitmap = (LPBITMAPINFOHEADER)::LockResource(hGlb);
	if(lpBitmap == NULL)
	{
		::FreeResource(hGlb);
		return FALSE;
	}

	// Take a copy of the BITMAPINFOHEADER structure
	int nColorTableSize = 1 << lpBitmap->biBitCount;
	UINT nSize = (UINT)(lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD));
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER) new char[nSize];
	if(lpBitmapInfo == NULL)
	{
		::FreeResource(hGlb);
		return FALSE;
	}

	memcpy(lpBitmapInfo, lpBitmap, nSize);

	// Modifiy the color table for the current system colors.
	DWORD* pColorTable = (DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);
	for(int i=0; i < nColorTableSize; i++)
	{
		for(int j=0; j<SECNOELEMENTS(sysColorMap); j++)
		{
			if(pColorTable[i] == sysColorMap[j][0])
			{
				COLORREF newColor = ::GetSysColor((int)sysColorMap[j][1]);
				pColorTable[i] = RGB(GetBValue(newColor),
									 GetGValue(newColor),
									 GetRValue(newColor));
				break;
			}
		}
	}

	int nWidth  = (int)lpBitmapInfo->biWidth;		// Width of bitmap
	int nHeight = (int)lpBitmapInfo->biHeight;		// Height of bitmap

	// Create a new a bitmap and copy the bitmap over with the new color table.
	CWindowDC screenDC(NULL);
	CDC dc;
	if(!bmp.CreateCompatibleBitmap(&screenDC, nWidth, nHeight) ||
	   !dc.CreateCompatibleDC(NULL))
	{
		::FreeResource(hGlb);
		delete (char*)lpBitmapInfo;
		return FALSE;
	}
	
	CBitmap* pBmpOld = (CBitmap*)dc.SelectObject(&bmp);

	LPBYTE lpBits = LPBYTE(lpBitmap+1);
	lpBits += (1 << (lpBitmapInfo->biBitCount)) * sizeof(RGBQUAD);

	StretchDIBits(dc.m_hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight, 
				  lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

	dc.SelectObject(pBmpOld);

	::FreeResource(hGlb);
	delete (char*)lpBitmapInfo;

	return TRUE;
}

