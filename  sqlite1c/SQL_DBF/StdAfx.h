// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__018C1C57_8C16_4DEB_9547_EC4A7E8AD9E4__INCLUDED_)
#define AFX_STDAFX_H__018C1C57_8C16_4DEB_9547_EC4A7E8AD9E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../_1Common/1CHEADERS/1cheaders.h"
#include "../_1Common/contextimpl.hpp"
#include "../_1Common/traps.hpp"
#include "../_1Common/nocasemap.hpp"
#include "../_1Common/dev_serv.h"
#include "SQLite/sqlite3.h"

__declspec(selectany) CDataDictionary* pDataDict = NULL;
__declspec(selectany) BOOL bDoTrace = FALSE;

enum DataBaseModes {dbSql, dbDbfShare, dbDbfMono};
__declspec(selectany) DataBaseModes dbMode = dbSql;

#define NOTHROW __declspec(nothrow)

enum NeedOrderOutput{
	nooNa = 0,
	nooAsc = 1,
	nooDesc = 2,
};

typedef sqlite3_index_info::sqlite3_index_constraint_usage* usage_ptr;
typedef sqlite3_index_info::sqlite3_index_constraint* constraint_ptr;
typedef sqlite3_index_info::sqlite3_index_orderby* orderby_ptr;

static inline BOOL isConstraint(sqlite3_index_info::sqlite3_index_constraint* pCtr)
{
	return pCtr->usable && pCtr->op != SQLITE_INDEX_CONSTRAINT_MATCH;
}

enum TestOp{ toEqual, toGrat, toGratEq, toLess, toLessEq};

NOTHROW static void SplitStr2Array(const CString& str, CStringArray& arr, char delim = '.')
{
	if(str.IsEmpty())
		return;
	LPCTSTR pRead = str, pStart = pRead;
	for(;;)
	{
		while(*pRead && *pRead != delim)
			pRead++;
		arr.Add(CString(pStart, pRead - pStart));
		if(!*pRead)
			break;
		pStart=++pRead;
	}
}

#include <typeinfo.h>
template<typename T, typename K>
BOOL safeDynCast(T* ptr, K*& dst)
{
	try{
		dst = dynamic_cast<K*>(ptr);
		return TRUE;
	}
	catch(bad_cast){}
	catch(bad_typeid){}
	dst = NULL;
	return FALSE;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__018C1C57_8C16_4DEB_9547_EC4A7E8AD9E4__INCLUDED_)
