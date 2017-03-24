// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__C0465E65_7215_48DC_A8C1_DD73504F844B__INCLUDED_)
#define AFX_STDAFX_H__C0465E65_7215_48DC_A8C1_DD73504F844B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>
#include <afxext.h>         // MFC extensions
#include <afxcview.h>         // MFC extensions
#include <comdef.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlhost.h>
#include <atlctl.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#include <afxctl.h>
#include <afxdtctl.h>
#include <afxcmn.h>
#include <afxtempl.h>

// Подключаем 1С хедеры и либы
#include <functional>
#include <set>
#include <vector>
#include <map>
using namespace std;
#include "..\\config\\1cheaders\\types.h"
#include "..\\config\\1cheaders\\type32.h"
#include "..\\config\\1cheaders\\bkend.h"
#include "..\\config\\1cheaders\\br32.h"
#include "..\\config\\1cheaders\\frame.h"
#include "..\\config\\1cheaders\\basic.h"
#include "..\\config\\1cheaders\\blang.h"
#include "..\\config\\1cheaders\\dbeng32.h"
#include "..\\config\\1cheaders\\editr.h"
#include "..\\config\\1cheaders\\moxel.h"
#include "..\\config\\1cheaders\\rgproc.h"
#include "..\\config\\1cheaders\\seven.h"
#include "..\\config\\1cheaders\\txtedt.h"
#include "..\\config\\1cheaders\\userdef.h"

#pragma comment (lib,"..\\config\\libs\\basic.lib")
#pragma comment (lib,"..\\config\\libs\\bkend.lib")
#pragma comment (lib,"..\\config\\libs\\blang.lib")
#pragma comment (lib,"..\\config\\libs\\br32.lib")
#pragma comment (lib,"..\\config\\libs\\dbeng32.lib")
#pragma comment (lib,"..\\config\\libs\\editr.lib")
#pragma comment (lib,"..\\config\\libs\\frame.lib")
#pragma comment (lib,"..\\config\\libs\\moxel.lib")
#pragma comment (lib,"..\\config\\libs\\rgproc.lib")
#pragma comment (lib,"..\\config\\libs\\seven.lib")
#pragma comment (lib,"..\\config\\libs\\txtedt.lib")
#pragma comment (lib,"..\\config\\libs\\type32.lib")
#pragma comment (lib,"..\\config\\libs\\userdef.lib")

// Мои глобальные переменные
extern CBkEndUI *pBkEndUI;
extern CMetaDataCont *pMetaDataCont;
#include "..\config\config.h"
extern CConfigService *pConfSvc;
extern HINSTANCE hMyInst;
extern CMDIFrameWnd* pMainFrame;
extern CApp7* pMainApp;
extern HINSTANCE h1CResource;
void DoMsgLine(const char* format,MessageMarker marker=mmNone,...);
void* GetZipResource(UINT id);
#import "..\config\config.tlb" no_namespace raw_interfaces_only named_guids
#import "c:\Program Files\1Cv77_25\BIN\zlibeng.dll" no_namespace raw_interfaces_only named_guids
void SplitStrToArray(LPCTSTR str,CStringArray& arr,char delim='.');
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C0465E65_7215_48DC_A8C1_DD73504F844B__INCLUDED)
