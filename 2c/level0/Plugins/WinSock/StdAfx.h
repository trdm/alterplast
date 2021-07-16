// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_)
#define AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h>

#include <winsock2.h>
 
 
// TODO: reference additional headers your program requires here
#define IS_PLUGIN
#include "../../Compiler/plugdetect.h"
#include "../../Compiler/Methods.h"
#include "../../Compiler/value.h"


#include "OutToDebug.h"

#define PORT_2C			19760
#define MAX_CONNECTION	10000

//#define TIME_OUT		(5*1000)
#define BUF_SIZE		0x10000



////////////////////////////////////////////////////////////////////
#define	NC_CLIENT_CONNECT		0x0001
#define	NC_CLIENT_DISCONNECT	0x0002
#define	NC_TRANSMIT				0x0003
#define	NC_RECEIVE				0x0004
#define	NC_CLIENT_ERROR			0x0005


CString GetGetLastStringError();

//#import "C:\Visual C++\2C\Enterprise\Plugins\ServerCOM\ServerCOM.tlb" 
//no_namespace, named_guids, no_implementation,raw_interfaces_only, raw_dispinterfaces, raw_native_types

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_)
