// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED_)
	#define AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED_

#pragma once
#define STRICT
#define _SECURE_ATL 0
#define _CRT_SECURE_NO_DEPRECATE
#define _USE_32BIT_TIME_T

// disable common warnings
//#ifdef __INTEL_COMPILER
//#pragma warning (disable : 1738)	// warning #1738: base class dllexport/dllimport specification differs from that of the derived class
//#pragma warning (disable : 1744)	// warning #1744: field of class type without a DLL interface used in a class with a DLL interface
//#pragma warning (error : 1011)		// error: missing return statement at end of non-void function
//#endif

// artbear - минимум Win98
#define _WIN32_WINNT 0x0400
// artbear - минимум Win2000 - дл€ поддержки GetLastInputInfo - получени€ времени просто€
//#define _WIN32_WINNT 0x0500
//#define WINVER 0x0500

//#define _ATL_NO_DEFAULT_LIBS

#include <afxwin.h>
#include <afxdisp.h>
#include <afxcmn.h>
#include <afxdlgs.h>
#include <afxmt.h>
#include <afxdb.h>

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>

#include <afxtempl.h>
#include <afxext.h>       // MFC extensions
#include <afxole.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>
#include <mmsystem.h>
#include <Winspool.h>
#include <afxcview.h>
#include <afxctl.h>

//#pragma warning(push, 3)
#include <functional>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <math.h>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <assert.h>

#include ".\\1cheaders\\1cheaders.h"

// все глобальные методы должны быть в следующем файле
//
// надоело при изменении stdafx.cpp ждать компил€ции всего проекта, а не одного файла,
// как при изменении и компил€ции других файлов :)
// файлы stdafx.h и stdafx.cpp лучше вообще не мен€ть

#include "system\\GlobalMethods.h"

// пусть будет здесь, здесь лучше видно
// не самое лучшее решение дл€ того, чтобы ловить исключени€ dynamic_cast со старым CRT, но другого нет
#ifndef _CRT_ASSEMBLY_VERSION
#define NON_RTTI_OBJECT ::bad_typeid
class bad_typeid : public std::exception
{
private:
	// не создавать, только ловить
	virtual ~bad_typeid() throw();
};
#else
#define NON_RTTI_OBJECT std::__non_rtti_object
#endif

template<typename T, typename ARG>
T safe_dynamic_cast(ARG* arg)
{
	try
	{
		return dynamic_cast<T>(arg);
	}
	catch (NON_RTTI_OBJECT const&)
	{
		return NULL;
	}
}

#endif // !defined(AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED)
