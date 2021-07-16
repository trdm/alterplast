/////////////////////////////////////////////////////////////////////////////
// SecLdBmp.h : Declaration for SECLoadSysColorBitmap
//
// Stingray Software Extension Classes
// Copyright (C) 1996 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Don			04/02/96
//


#ifndef __SLOADBMP_H__
#define __SLOADBMP_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

/////////////////////////////////////////////////////////////////////////////
// 
//
// Function to load a bitmap, changing it's colors to the current system 
// color scheme.
BOOL SECLoadSysColorBitmap(CBitmap& bmp, LPCTSTR lpszName);

#endif // __SLOADBMP_H__
