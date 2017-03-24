//////////////////////////////////////////////////////////////////////
//
// Разработчик: Артур Аюханов aka artbear, mailto: artbear@inbox.ru
//
//////////////////////////////////////////////////////////////////////

#if !defined(_MY_ABOUT_WINDOW_MANAGER__INCLUDED_)
#define _MY_ABOUT_WINDOW_MANAGER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OwnAboutWindowManager.h"

#ifdef FORMEX_VK_DEFINED
	#include "..\HyperLink.h"
#else
	#include "HyperLink.h"
#endif

class CMyAboutWindowManager : public IOwnAboutWindowManager
{
public:
	CWnd* m_pOwnPage;
	
	CStatic m_picLogo;
	bool m_bLogoShowed;
	//static bool m_bEditShowed;
	CRichEditCtrl m_cRich;
	CHyperLink m_cHyperLink;
	
public:
	CMyAboutWindowManager() : m_bLogoShowed(false) 
	{};
	
	~CMyAboutWindowManager()
	{};
	
	// реализация IOwnAboutWindowManager
	
	virtual LPCSTR GetTabName() { return "1С++";};
	
	virtual CWnd* CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID );
	
	virtual CWnd* GetPage() { return m_pOwnPage; };
	
	virtual HBITMAP GetBitmapHandle() { return NULL; }; // для 1C++ битмап задан уже в коде инициализации
	//virtual HBITMAP GetBitmapHandle() { return ::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_ABOUTTAB)); };
	
	virtual void DestroyOwnPage();
};


#ifdef ABOUT_WINDOW_EXAMPLE

class CMyAboutWindowManagerExample : public CMyAboutWindowManager
{
	LPCSTR m_szTabName;
public:
	CMyAboutWindowManagerExample(LPCSTR szTabName) : m_szTabName(szTabName) {};
	
	virtual LPCSTR GetTabName() { return m_szTabName;};
	
	virtual CWnd* CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID );
	
	//virtual CWnd* GetPage() { return m_pOwnPage; };
	
	virtual HBITMAP GetBitmapHandle() { return ::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_PROPTAB)); };
};

#endif // #ifdef ABOUT_WINDOW_EXAMPLE

#endif