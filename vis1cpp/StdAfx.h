// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__646DE22B_CB0B_46B1_9B98_020CE38926A2__INCLUDED_)
#define AFX_STDAFX_H__646DE22B_CB0B_46B1_9B98_020CE38926A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

// TODO только дл€ отладки
//#define _ATL_DEBUG_INTERFACES

#include "1CPP\\MyAssert.h"

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include <set>
#include <vector>
#include <map>
#include <list>

#include ".\\1cheaders\\1cheaders.h"

// все глобальные методы должны быть в следующем файле
//
// надоело при изменении stdafx.cpp ждать компил€ции всего проекта, а не одного файла,
// как при изменении и компил€ции других файлов :)
// файлы stdafx.h и stdafx.cpp лучше вообще не мен€ть

#include "1CPP\\GlobalMethods.h"

#endif // !defined(AFX_STDAFX_H__646DE22B_CB0B_46B1_9B98_020CE38926A2__INCLUDED)
