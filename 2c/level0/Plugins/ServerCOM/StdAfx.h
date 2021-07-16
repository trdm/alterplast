// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__E3BFD48C_EF19_4ABB_82EA_5D3B30C28D8E__INCLUDED_)
#define AFX_STDAFX_H__E3BFD48C_EF19_4ABB_82EA_5D3B30C28D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#define IS_PLUGIN
#include <afxtempl.h>
#include "../../Compiler/plugdetect.h"
#include "../../Compiler/Methods.h"
#include "../../Compiler/value.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__E3BFD48C_EF19_4ABB_82EA_5D3B30C28D8E__INCLUDED)
