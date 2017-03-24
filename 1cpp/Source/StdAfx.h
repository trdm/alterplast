// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED_)
	#define AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED_

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

// добавлено kms дл€ компил€ции с PSDK
#define __NOTHROW_T_DEFINED
#define OLD_ATL_CRITSEC_CODE

#include <afxwin.h>
#include <afxdisp.h>
#include <afxcmn.h>
#include <afxdlgs.h>
#include <afxmt.h>
#include <afxdb.h>

// artbear - минимум Win98
#define _WIN32_WINNT 0x0400
// artbear - минимум Win2000 - дл€ поддержки GetLastInputInfo - получени€ времени просто€
//#define _WIN32_WINNT 0x0500
//#define WINVER 0x0500

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

//#pragma warning(disable : 4100)

//#pragma warning(push, 3)
#include <functional>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <math.h>

using namespace std;

#include <boost/shared_ptr.hpp>
//using namespace boost; // если раскомментировать, в некоторых файлах 1—++ возникнет баг http://www.1cpp.ru/forum/YaBB.pl?num=1195384686/63#63

#include <assert.h>

// Stingray Objective Toolkit
//#include <ot_dockingwindows.h>		// Objective Toolkit Docking Windows
//#include <ot_toolbar.h>				// Objective Toolkit Customizable Toolbar/Menubar
//#include <ot_wdi.h>					// Objective Toolkit Workbook Interface
//#include <ot_treectrl.h>			// Objective Toolkit Tree control & Tree view
//#include <ot_tabwnd.h>				// Objective Toolkit Tab controls

#include ".\\1cheaders\\1cheaders.h"

// все глобальные методы должны быть в следующем файле
//
// надоело при изменении stdafx.cpp ждать компил€ции всего проекта, а не одного файла,
// как при изменении и компил€ции других файлов :)
// файлы stdafx.h и stdafx.cpp лучше вообще не мен€ть

#include "system\\GlobalMethods.h"

// пусть будет здесь, здесь лучше видно
// не самое лучшее решение дл€ того, чтобы ловить исключени€ dynamic_cast со старым CRT, но другого нет
class bad_typeid : public exception
{
private:
	// не создавать, только ловить
	virtual ~bad_typeid() throw();
};

template<typename T, typename ARG>
T safe_dynamic_cast(ARG arg)
{
	T ret = NULL;
	try
	{
		ret = dynamic_cast<T>(arg);
	}
	catch (::bad_typeid const& e) // std::__non_rtti_object
	{
	}
	return ret;
}

class CBLMyEx:public CBLExportContext
{
public:
	static long DispParamToValueEx(CValue** ppVal, DISPPARAMS* pDisp,unsigned int* pUI)
	{
		return static_cast<CBLMyEx*>(NULL)->DispParamToValue(ppVal, pDisp,pUI);
	}
	int  TryReturnByRefEx(CValue ** ppVal,DISPPARAMS* pDisp)
	{
		return TryReturnByRef(ppVal,pDisp);
	}
	static int  ValueToVariantEx(CValue& Val,tagVARIANT * pVariant){
		return static_cast<CBLMyEx*>(NULL)->ValueToVariant(Val,pVariant);
	}
};

void DispatchToValue(IUnknown** ppUnk, CValue* pVal);

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FC223DD4_F108_11D0_A459_004095E1DAEA__INCLUDED)