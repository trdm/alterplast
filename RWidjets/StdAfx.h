// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__58E96797_F2E6_11D5_AE41_0002B31F6532__INCLUDED_)
#define AFX_STDAFX_H__58E96797_F2E6_11D5_AE41_0002B31F6532__INCLUDED_

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


	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <atlwin.h>
#include <afxtempl.h>


#include ".\\1cheaders\\1cheaders.h"

#define ID_MYFIRSTCOMMAND 0xD000

#define MSG(str) \
	GetBkEndUI()->DoMessageLine(str, mmNone);

#define MSG1(str, arg1) \
	{CString strmsg; \
	strmsg.Format(str, arg1); \
	GetBkEndUI()->DoMessageLine(strmsg, mmNone);}

#define MSG2(str, arg1, arg2) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2); \
	GetBkEndUI()->DoMessageLine(strmsg, mmNone);}

#define MSG3(str, arg1, arg2, arg3) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2, arg3); \
	GetBkEndUI()->DoMessageLine(strmsg, mmNone);}

#define MSG4(str, arg1, arg2, arg3, arg4) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2, arg3, arg4); \
	GetBkEndUI()->DoMessageLine(strmsg, mmNone);}

#define MSG5(str, arg1, arg2, arg3, arg4, arg5) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2, arg3, arg4, arg5); \
	GetBkEndUI()->DoMessageLine(strmsg, mmNone);}

#define DUMPMENU(pMenu) \
{CString str;\
MENUITEMINFO info;\
info.cbSize = sizeof (MENUITEMINFO);\
info.fMask = MIIM_DATA;\
MSG1("Menu %x", pMenu)\
for(int i = 0; i < pMenu->GetMenuItemCount(); i++)\
{\
	pMenu->GetMenuString(i, str, MF_BYPOSITION);\
	pMenu->GetMenuItemInfo(i, &info, TRUE);\
	MSG3("	%d %s dwItemData=%x", i, str, info.dwItemData)\
}}

#define RWidjets_BEGIN try{
#define RWidjets_END }catch(...){CBLModule::RaiseExtRuntimeError("Неизвестная ошибка.", mmRedErr);} return TRUE;

#define ERR(str) \
	CBLModule::RaiseExtRuntimeError(str, mmRedErr);

#define ERR1(str, arg1) \
	{CString strmsg; \
	strmsg.Format(str, arg1); \
	ERR(strmsg)}

#define ERR2(str, arg1, arg2) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2); \
	ERR(strmsg)}

#define ERR3(str, arg1, arg2, arg3) \
	{CString strmsg; \
	strmsg.Format(str, arg1, arg2, arg3); \
	ERR(strmsg)}


#define IDS_ERR_WRONGTYPE1      "Неправильный тип элемента меню. Возможные значения: 1 - подменю, 2 - разделитель, 3 - команда."
#define IDS_ERR_REDEFINEID      "Элемент с идентификатором %s уже существует на этом уровне меню."
#define IDS_ERR_UNKNOWNID       "Нет элемента меню с идентификатором %s на этом уровне меню."
#define IDS_WRONGINDEX          "Неверный индекс элемента."
#define IDS_ERR_WRONGTYPE2      "Неправильный тип элемента меню. Возможные значения: 1 - подменю, 2 - разделитель."
#define IDS_ERR_EMPTYID         "Значение идентификатора не должно быть пустым."
#define IDS_ERR_NOPICTURE       "Значение первого параметра не картинка."
#define IDS_ERR_NORES           "Картинка с номером %d не найдена в файле ресурсов."

#define AGREGATE_TYPE_1C        100

class CMenuItem;

struct stDefMenuItems
{
	LPCSTR Name;
	CMenuItem* pMenuItem;
	int PosInDefMenu;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__58E96797_F2E6_11D5_AE41_0002B31F6532__INCLUDED_)
