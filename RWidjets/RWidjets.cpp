// RWidjets.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "RWidjets.h"
#include <initguid.h>
#include "RWidjets_i.c"

#include ".\\MyContext\\mycontextbase.h"
#include "MenuItem.h"
#include "RaiseEvent.h"

/////////////////////////////////////////////////////////////////////////////
extern CMenuItem theMenu;
extern CMenuItem theMenu;
extern CMapWordToPtr theMapIDtoMenuItem;
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRWidjetsApp

BEGIN_MESSAGE_MAP(CRWidjetsApp, CWinApp)
	//{{AFX_MSG_MAP(CRWidjetsApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWidjetsApp construction

CRWidjetsApp::CRWidjetsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRWidjetsApp object

CMapStringToPtr theMapDefMenuItem;
stDefMenuItems theDefMenuItems[]={
	{"&Файл", NULL, 5},
	{"Опе&рации", NULL, 22},
	{"&Сервис", NULL, 6},
	{"&Помощь", NULL, 6},
};


CRWidjetsApp theApp;
CMenuItem theMenu;
CMapWordToPtr theMapIDtoMenuItem;
HHOOK hMenuHook, hMenuHookRet;
WNDPROC m_OldWndProc;

HMENU m_menuBeingProcessed;
CFont m_menuFont;

CComModule _Module;
LRESULT CALLBACK MenuHook(int code, WPARAM wp, LPARAM lp);
LRESULT CALLBACK MenuHookRet(int code, WPARAM wp, LPARAM lp);
LRESULT CALLBACK MenuHookProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}
/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer(TRUE); //TRUE indicates that typelib is unreg'd
	return S_OK;
}

BOOL CALLBACK EnumThreadWndProc(
  HWND hwnd,      // handle to window
  LPARAM lParam   // application-defined value
)
{
	CString str;

	::GetClassName(hwnd, str.GetBuffer(256), 256);
	str.ReleaseBuffer();

	if(str.Left(16) == "Afx:400000:0:0:0")
	{
		HWND* pHWND = (HWND*)lParam;	
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}
	else
		return TRUE;
}

BOOL CRWidjetsApp::InitInstance()
{
	if (!InitATL())
		return FALSE;

	// Hook Menu
	hMenuHook = ::SetWindowsHookEx(WH_GETMESSAGE, MenuHook, NULL, ::GetCurrentThreadId());
	hMenuHookRet = ::SetWindowsHookEx(WH_CALLWNDPROCRET, MenuHookRet, NULL, ::GetCurrentThreadId());
	//hMenuHook = ::SetWindowsHookEx(WH_GETMESSAGE, MenuHook, ::GetModuleHandle(NULL), ::GetCurrentThreadId());
	//hMenuHookRet = ::SetWindowsHookEx(WH_CALLWNDPROCRET, MenuHookRet, ::GetModuleHandle(NULL), ::GetCurrentThreadId());

	HWND hWnd = NULL;
	
	CWnd* pCWnd = NULL;
	pCWnd = AfxGetMainWnd();
	if(pCWnd)
		hWnd = pCWnd->m_hWnd;
	else
		::EnumThreadWindows(::GetCurrentThreadId(), EnumThreadWndProc, (LPARAM)&hWnd);

	m_OldWndProc = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
	LONG l = ::SetWindowLong(hWnd, GWL_WNDPROC, (long)MenuHookProc);

	CString str;
	for(int i=0; i<4; i++)
	{
		str = theDefMenuItems[i].Name;
		str.Replace("&", "");
		str.MakeUpper();

		theMapDefMenuItem.SetAt(str, &theDefMenuItems[i]); 

		theDefMenuItems[i].pMenuItem = new CMenuItem(str);
		theDefMenuItems[i].pMenuItem->SetMenu(theDefMenuItems[i].Name);
	}

    LOGFONT lf;
    
    //ZeroMemory((PVOID) &lf,sizeof(LOGFONT));
    
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, nm.cbSize,&nm, 0));
    // these fonts should auto destroy when the DLL unloads
    // create the normal menu item font
    lf = nm.lfMenuFont;
    m_menuFont.CreateFontIndirect(&lf);

	CMyContextBase::InitAllContextClasses();

	return CWinApp::InitInstance();

}

int CRWidjetsApp::ExitInstance()
{
	_Module.Term();

	::UnhookWindowsHookEx(hMenuHook);
	::UnhookWindowsHookEx(hMenuHookRet);

	theMapDefMenuItem.RemoveAll();
	delete theDefMenuItems[0].pMenuItem;
	delete theDefMenuItems[1].pMenuItem;
	delete theDefMenuItems[2].pMenuItem;
	delete theDefMenuItems[3].pMenuItem;

	return CWinApp::ExitInstance();

}

BOOL CRWidjetsApp::InitATL()
{
	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	return TRUE;

}

LRESULT CALLBACK EscHook(int code, WPARAM wp, LPARAM lp)
{
	long addr;
	addr = (long)CBLModule7::GetExecutedModule();
	if((wp == 27) && (addr != 0)) return 1;

	return 0;//::CallNextHookEx(hHook, code, wp, lp);
}

void ExecuteBatch(CString &strCode1C)
{
	CMDIFrameWnd * pMainFrame1 = GetMainFrame();
	CWnd* pActiveWnd = pMainFrame1->GetActiveWindow();
	CGetDoc7* pDoc7 = NULL;
	if(pMainFrame1->GetSafeHwnd() != pActiveWnd->GetSafeHwnd())
		pDoc7 = CGetDlg::GetTopmostDocunent();

	bool bNeedCallGlob = true;
	if(pDoc7)
	{
		CWnd* pWndFrame = pDoc7->GetFrame();
		pDoc7->m_pBLModule->ExecuteBatch(strCode1C, NULL);
		bNeedCallGlob = false;
	}
	else
	{
		CMDIFrameWnd * pMainFrame = GetMainFrame();
		if(pMainFrame)
		{
			CMDIChildWnd * pChild = pMainFrame->MDIGetActive();
			if(pChild)
			{
				CWnd* wndFocus = pMainFrame->GetFocus();
				if(wndFocus)
				{
					CFrameWnd * pParentFrame = wndFocus->GetParentFrame();
					if(pParentFrame && pParentFrame->GetSafeHwnd() == pChild->GetSafeHwnd())
					{
						CDocument* pDoc = pChild->GetActiveDocument();
						if(pDoc)
							if(!strcmp(pDoc->GetRuntimeClass()->m_pfnGetBaseClass()->m_lpszClassName,"CGetDoc7"))
							{
								pDoc7 = (CGetDoc7*)pDoc;
								if(pDoc7->m_pBLModule->IsCompiled() == 1)
								{
									pDoc7->m_pBLModule->ExecuteBatch(strCode1C, NULL);
									bNeedCallGlob = false;
								}
							}
					}
				}
			}
		}
	}
	if(bNeedCallGlob)
	{
		long lId = CBLModule::GetFirstLoadedModuleID();
		CBLModule* pModule = CBLModule::GetLoadedModule(lId);
		pModule->ExecuteBatch(strCode1C, NULL);
	}
}

LRESULT CALLBACK MenuHook(int code, WPARAM wp, LPARAM lp)
{
	MSG* wpstr = (MSG*)lp;
	UINT uMsg = wpstr->message;
	WPARAM wParam = wpstr->wParam; 
	HWND hwnd = wpstr->hwnd;

	int iCommandID = 0;
	int iCommandSource = 0;

	if(uMsg == WM_LBUTTONUP)
	{
		CWnd *pWnd = CWnd::FromHandle(hwnd);
		//MSG3("%s %d %s", uMsg == WM_LBUTTONUP?"WM_LBUTTONUP":"WM_LBUTTONDOWN", wp, pWnd->GetRuntimeClass()->m_lpszClassName);
		if(!strcmp(pWnd->GetRuntimeClass()->m_lpszClassName,"CCustomToolBar7")
			|| !strcmp(pWnd->GetRuntimeClass()->m_lpszClassName,"CToolBar7"))
		{
			CToolBar7* pTB = (CToolBar7*)pWnd;
			//MSG1("GetCurBtn %d", pTB->GetCurBtn());
			if(pTB->GetCurBtn() >= 0)
			{
				iCommandID = pTB->GetItemID(pTB->GetCurBtn());
				iCommandSource = 2;
			}
				
		}
	}
	else if(uMsg == 0x004D && wp)
	{
		// F1
		iCommandID = 57670;
		iCommandSource = 3;
	}
	else if(uMsg == WM_KEYDOWN)
	{
		// artur
		if (!GetMainFrame())
			return ::CallNextHookEx(hMenuHook, code, wp, lp);
		// artur end
	
		HACCEL hAccel = GetMainFrame()->GetDefaultAccelerator();
		
		//MSG2("%X %d", wParam, wParam);
		int nAccel;
		if (hAccel && (nAccel = ::CopyAcceleratorTable(hAccel, NULL, 0)) > 0) 
		{
			ACCEL*	pAccel;
			pAccel = new ACCEL [nAccel];
			ASSERT(pAccel);
			::CopyAcceleratorTable(hAccel, pAccel, nAccel);
			
			BOOL ShiftPressFlag = ::GetKeyState(VK_SHIFT) & 0x80 ? 1 : 0;
			BOOL CtrlPressFlag = ::GetKeyState(VK_CONTROL) & 0x80 ? 1 : 0;
			BOOL AltPressFlag = ::GetKeyState(VK_MENU) & 0x80 ? 1 : 0;
			
			for (int i=0; i<nAccel; i++) 
			{
				ACCEL& ac = pAccel[i];
				//MSG4("	%X FSHIFT=%d FCONTROL=%d FALT=%d", ac.key, ac.fVirt & FSHIFT?1:0, ac.fVirt & FCONTROL?1:0, ac.fVirt & FALT?1:0)
				if(wParam == ac.key)
				{
					if((ac.fVirt & FSHIFT?1:0) == ShiftPressFlag
						&& (ac.fVirt & FCONTROL?1:0) == CtrlPressFlag
						&& (ac.fVirt & FALT?1:0) == AltPressFlag)
					{
						iCommandID = ac.cmd;
						iCommandSource = 3;
					}
				}
			}
			
			delete pAccel;
		}
	}
	else if(uMsg == WM_COMMAND && wp)
	{
		iCommandID = wParam;
		iCommandSource = 1;
	}


	BOOL bBreakCommand = FALSE;

	if(iCommandID 
		&& iCommandID != 32819 
		&& iCommandID != 32822 
		&& iCommandID != 32818 
		&& iCommandID != 32816)
	{
		CValue vCommandID;
		CValue vRet = 0L;
		vCommandID = iCommandID;
		CValue *lArray[2] = {&vCommandID, &vRet};

		CallModuleProc("ПриПолученииКоманды", "OnGetCommand", 2, lArray);

		if(vRet.GetNumeric() == 1)
		{
			bBreakCommand = TRUE;

			if(iCommandSource == 2)
				wpstr->lParam = 0;
			else
				wpstr->message = 0;
		}

	}

	if(iCommandID > ID_MYFIRSTCOMMAND && !bBreakCommand)
	{
		CMenuItem* pMenuItem;
		if(theMapIDtoMenuItem.Lookup(iCommandID, (void*&) pMenuItem ))
		{
			pMenuItem->m_strCode1C.TrimLeft(" ");
			if(pMenuItem->m_strCode1C.GetAt(0) == '#')
			{
				CString str = pMenuItem->m_strCode1C;
				str.Replace("#", "");
				str.Replace(" ", "");
				int iCmdID = 0;
				for(int i=0; i<str.GetLength();i++)
					iCmdID = iCmdID*10+((int)str.GetAt(i)-48);
				
				AfxGetApp()->GetMainWnd()->PostMessage(WM_COMMAND, iCmdID, 0);
			}
			else
				ExecuteBatch(pMenuItem->m_strCode1C);
		}
	};

	return ::CallNextHookEx(hMenuHook, code, wp, lp);
}

LRESULT CALLBACK MenuHookRet(int code, WPARAM wp, LPARAM lp)
{
	CWPRETSTRUCT* wpstr = (CWPRETSTRUCT*)lp;
	UINT uMsg = wpstr->message;
	WPARAM wParam = wpstr->wParam;
	LPARAM lParam = wpstr->lParam;

	switch(uMsg)
	{
		case WM_INITMENUPOPUP:
		{
			CMenu menu;
			menu.Attach((HMENU)wParam);

			m_menuBeingProcessed = (HMENU)wParam;

			if((int)menu.GetMenuItemCount()>0)
				for(int i = 0; i < menu.GetMenuItemCount(); i++)
				{
					if(menu.GetMenuItemID(i) >= ID_MYFIRSTCOMMAND)
					{
						CMenuItem* pMenuItem;
						if(theMapIDtoMenuItem.Lookup(menu.GetMenuItemID(i), (void*&) pMenuItem ))
						{
							if(pMenuItem->m_bEnabled)
								menu.EnableMenuItem(i, MF_BYPOSITION|MF_ENABLED);
						}
					}
				}

			menu.Detach();

			break;
		}
		case WM_MENUSELECT:
		{
			UINT iCommandID = (UINT) LOWORD(wParam);   

			if(iCommandID > ID_MYFIRSTCOMMAND)
			{
				CMenuItem* pMenuItem;
				if(theMapIDtoMenuItem.Lookup(iCommandID, (void*&) pMenuItem ))
				{
					GetBkEndUI()->DoStatusLine(pMenuItem->m_strTips);
				}
			};
			break;
		}
		case WM_COMMAND:
		{
			// Отображение результатов сложения и вычитания чисел в статусной строке
			if(wParam == 33708 || wParam == 32833 || wParam == 32834) 
			{
				if (::OpenClipboard(NULL)) 
				{
					char *data;
					HGLOBAL handle = ::GetClipboardData(CF_TEXT);
					if (handle) 
					{
						data = (char*)::GlobalLock(handle);
						GetBkEndUI()->DoStatusLine(data);
						::GlobalUnlock(handle);
					}
					::CloseClipboard();
				}
			}
			break;
		}
		case WM_DRAWITEM:
		{
			//LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;//wpstr->lParam;
			//MSG3("%x %x %x", lpDrawItemStruct->itemID, lpDrawItemStruct->itemData, lpDrawItemStruct->hwndItem  );

			if(wParam == 0)
			{
				LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)wpstr->lParam;
				//MSG3("%x %x %x", lpDrawItemStruct->itemID, lpDrawItemStruct->itemData, lpDrawItemStruct->hwndItem  );

				if(lpDrawItemStruct->itemID >= ID_MYFIRSTCOMMAND && lpDrawItemStruct->CtlType == ODT_MENU)
				{
					CMenuItem* pMenuItem;
					if(theMapIDtoMenuItem.Lookup(lpDrawItemStruct->itemID, (void*&) pMenuItem ))
					{
						pMenuItem->DrawItem_OldStyle(lpDrawItemStruct);
						//MSG3("%s %d %d", pMenuItem->m_strCaption, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.right);
					}/*
					else
					{
						CMenu* menu = CMenu::FromHandle((HMENU)lpDrawItemStruct->hwndItem);
						CString strText;
						menu->GetMenuString(lpDrawItemStruct->itemID, strText, MF_BYCOMMAND);
						
						MSG3("%s %d %d", strText, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.right);
						////////////////////////////
						CRect rect = CRect(lpDrawItemStruct->rcItem);
						rect.left = 0;
						int m_iconX = 16;
						int m_iconY = 15;
						CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
						
						COLORREF colorBack = GetSysColor(COLOR_MENU);
						COLORREF colorMenu = GetSysColor(COLOR_MENUTEXT);
						// Draw the background
						CSize sizeText = pDC->GetTextExtent(strText);
						rect.InflateRect(0, 0, sizeText.cx, 0);

						//::SleepEx(1000, TRUE);
						pDC->FillSolidRect (rect, colorBack);
						
						// Draw the selection
						if(lpDrawItemStruct->itemState&ODS_SELECTED)
						{
							//pDC->FillRect(rect2,&m_brSelect);
						}

						//::SleepEx(1000, TRUE);
						// Вот здесь рисуем текст
						if(!strText.IsEmpty())
						{
							COLORREF crText = GetSysColor(COLOR_MENUTEXT);
							
							if(lpDrawItemStruct->itemState&ODS_SELECTED)
							{
								crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
							}
							
							rect.left += m_iconX + 6;
							CRect rectt(rect.left,rect.top-1,rect.right,rect.bottom-1);
							
							// Find tabs
							CString leftStr,rightStr;
							leftStr.Empty();rightStr.Empty();
							
							int tablocr = strText.ReverseFind(_T('\t'));
							if(tablocr!=-1)
							{
								rightStr = strText.Mid(tablocr+1);
								leftStr = strText.Left(strText.Find(_T('\t')));
								rectt.right -= m_iconX;
							}
							else
							{
								leftStr = strText;
							}
							
							int iOldMode = pDC->SetBkMode(TRANSPARENT);
							// Draw the text in the correct colour:
							UINT nFormat  = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
							UINT nFormatr = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;
							
							CFont fontMenu;
							LOGFONT logFontMenu;
							
							NONCLIENTMETRICS nm = {0};
							nm.cbSize = sizeof (nm);
							VERIFY (SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0)); 
							logFontMenu =  nm.lfMenuFont;
							
							fontMenu.CreateFontIndirect (&logFontMenu);
							CFont* pOldFont = pDC->SelectObject(&fontMenu);
							
							pDC->SetTextColor(crText);
							pDC->DrawText (leftStr,rectt,nFormat);
							if(tablocr!=-1)
							{
								pDC->DrawText (rightStr,rectt,nFormatr);
							}

							pDC->SelectObject(pOldFont);
							pDC->SetBkMode( iOldMode );
						//::SleepEx(1000, TRUE);
						}
						////////////////////////////

					}*/
				}
			}
			break;
		}
	}

	return ::CallNextHookEx(hMenuHookRet, code, wp, lp);
}

LRESULT CALLBACK MenuHookProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT res = ::CallWindowProc(m_OldWndProc, hwnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_MEASUREITEM:
		{
			LPMEASUREITEMSTRUCT lpMeasureItemStruct = (LPMEASUREITEMSTRUCT)lParam;

			CMenu* menu = CMenu::FromHandle(m_menuBeingProcessed);

			// Obtain the width of the text:
			CString str;
			CWnd *pWnd = AfxGetMainWnd();   // Get main window
			if (pWnd == NULL)
			{
				pWnd = CWnd::GetDesktopWindow();
			}
			CDC *pDC = pWnd->GetDC();    // Get device context
			CFont* pFont = NULL; // Select menu font in...
			
			pFont = pDC->SelectObject(&m_menuFont);

			CMenuItem* pMenuItem;
			if(theMapIDtoMenuItem.Lookup(lpMeasureItemStruct->itemID, (void*&) pMenuItem ))
			{
				str = pMenuItem->m_strCaption;

				CSize size(0, 0);
				if (str.GetLength() > 0)
				{
					size = pDC->GetTextExtent(str);
					size.cx += 3;
				}
				
				CSize t = CSize(size);
				pDC->SelectObject(pFont); // Select old font in
				pWnd->ReleaseDC(pDC); // Release the DC
				
				// Set width and height:        
				lpMeasureItemStruct->itemWidth = 16 + 8 + size.cx;
			}
			else if(lpMeasureItemStruct->itemData == ID_MYFIRSTCOMMAND)
			{
				// это субменю
				lpMeasureItemStruct->itemWidth = 5;
				//menu->GetMenuString(lpMeasureItemStruct->itemID, str, MF_BYCOMMAND);
				//MSG2("%x %s", lpMeasureItemStruct->itemData, str);
			}

			
			break;
		}
	}

	return res;
}

