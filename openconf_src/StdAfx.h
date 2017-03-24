// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1862B8ED_45EB_41BF_A51B_B0BA0C0B4143__INCLUDED_)
#define AFX_STDAFX_H__1862B8ED_45EB_41BF_A51B_B0BA0C0B4143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>         // MFC extensions
#include <comdef.h>

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
#include <afxctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <activscp.h>
#include "activdbg.h"
#include <mshtmhst.h>

#include <afxtempl.h>
#include <functional>
#include <set>
#include <vector>
#include <map>

#include ".\\1cheaders\\types.h"
#include ".\\1cheaders\\type32.h"
#include ".\\1cheaders\\bkend.h"
#include ".\\1cheaders\\br32.h"
#include ".\\1cheaders\\frame.h"
#include ".\\1cheaders\\basic.h"
#include ".\\1cheaders\\blang.h"
#include ".\\1cheaders\\dbeng32.h"
#include ".\\1cheaders\\editr.h"
#include ".\\1cheaders\\moxel.h"
//#include ".\\1cheaders\\mxl2xl.h"
#include ".\\1cheaders\\seven.h"
#include ".\\1cheaders\\rgproc.h"
#include ".\\1cheaders\\txtedt.h"
#include ".\\1cheaders\\userdef.h"
using namespace std;
extern CBkEndUI *pBkEndUI;
extern CApp7* pMainApp;
extern HINSTANCE h1CResource;

#include "olepro32.h"
#import <fm20.dll> no_implementation raw_interfaces_only no_namespace exclude("OLE_COLOR","OLE_HANDLE")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1862B8ED_45EB_41BF_A51B_B0BA0C0B4143__INCLUDED_)
