#include "stdafx.h"

#include "MyAboutWindowManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef FORMEX_VK_DEFINED
	const int iResID = IDD_ABOUT_FORMEX;
#else
	const int iResID = IDD_ABOUT_1CPP;
	extern HINSTANCE hDllInstance;
#endif

static LRESULT WINAPI AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CLOSE)
	{
		HWND hWnd = ::GetParent(hDlg);
		if(hWnd)
		{
			hWnd = ::GetParent(hWnd);
			if(hWnd)
			{
				hWnd = ::GetParent(hWnd);
				if(hWnd)
					::SendMessage(hWnd, uMsg, wParam, lParam);
			}
		}
	}
	else if(uMsg == WM_INITDIALOG)
	{
		return TRUE;
	}
	return FALSE;
}

CWnd* CMyAboutWindowManager::CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID )
{
	CWnd* pNewOwnPage = new CWnd;
	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW,PageRect, pOwnerWindow, nID))
	{
		if(!m_bLogoShowed)
		{
			AfxInitRichEdit();
			HWND hWnd = ::CreateDialog(hDllInstance,MAKEINTRESOURCE(iResID), pNewOwnPage->GetSafeHwnd(), (DLGPROC)AboutDlgProc);
			if(hWnd)
			{
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				CRect rect(11,11,77,77);
				m_picLogo.CreateEx(WS_EX_NOPARENTNOTIFY,"Static","",
					WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|WS_BORDER|
					SS_BITMAP|SS_CENTERIMAGE,
					rect,pWnd,0x10FF);
				m_picLogo.SetBitmap(::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_LOGO)));
				
				CString text;
				
				pWnd->GetDlgItemText(IDC_ABOUT_STATIC, text.GetBufferSetLength(250), 250);
				text.ReleaseBuffer();
				CString sVersion = ::GetFullVersion();
				text.Replace("_VersionInfo_", sVersion);
				pWnd->SetDlgItemText(IDC_ABOUT_STATIC, text);
				
				// информация о разработчиках
				m_cRich.SubclassDlgItem(IDC_ABOUT_RICHEDIT, pWnd);
				m_cRich.SetBackgroundColor(FALSE, ::GetSysColor(COLOR_BTNFACE));
				text = "{\\rtf1\\ansi\\deff0\\deftab720"
					"\\fs20\\b1\\ul1Разработчики:\\b0\\ul0\\par\\fs10\\par"
					"\\b1Валерий Атаманюк\\b0\\par автор проекта Rainbow, заголовочные файлы классов 1С\\par"
					"\\b1Carlos Antollini\\b0\\par odbccore.cpp\\par"
					"\\b1Дмитрий Гусев aka Deb\\b0\\par основной движок 1С++ и классы РаботаСРегистром, ВыполняемыйМодуль, Делегат, МенеджерСобытий, Структура, MetaInfoClasses, подсистема прямого доступа к данным через ODBC\\par"
					"\\b1Андрей Рачкин\\b0\\par классы DynaValue, DynaCrypt\\par"
					"\\b1Алексей Фёдоров aka АЛьФ\\b0\\par подсистема FormEx\\par"
					"\\b1Александр Орефков\\b0\\par класс TurboBL, Visual1C++\\par"
					"\\b1Павел Шемякин aka ToyPaul\\b0\\par первоначальная версия подсистемы прямого доступа к данным через ODBC\\par"
					"\\b1Дмитрий Ощепков aka DmitrO\\b0\\par доработка подсистемы прямого доступа к данным через ODBC, метапарсер имен, класс ТабличноеПоле\\par"
					"\\b1Кирилл Мирошниченко aka spock\\b0\\par подсистема прямого доступа к данным через OLE DB\\par"
					"\\b1Алексей Диркс aka ADirks\\b0\\par класс ИндексированнаяТаблица\\par"
					"\\b1Федор Езеев aka fez\\b0\\par технический менеджер проекта\\par"
					"\\b1Артур Аюханов aka artbear\\b0\\par доработка ООП, классов ВыполняемыйМодуль, Поток и т.п.\\par"
					"\\b0а также автор конфигурации полноценного юнит-тестирования 1С++\\par"
					"\\fs20\\par\\b1\\ul1Бета-тестеры:\\b0\\ul0\\par\\fs10\\par"
					//****************************************************************************************
					// Начало списка бета-тестеров
					"Олег Брянцев aka ob36\\par"
					"Валерий Федюшин aka Varelchik\\par"
					"Марина Дяченко aka marida\\par"
					"Сергей Накопьюк\\par"
					"Алексей Ефремов aka GavaH\\par"
					"\\par"
					// Конец списка бета-тестеров
					//****************************************************************************************
					" }";
				m_cRich.SetWindowText(text);
				m_cRich.SetTargetDevice(NULL, 0);
				//m_pWrapperForAboutDlg->m_cRich.SetEventMask(ENM_KEYEVENTS|ENM_UPDATE);
				//pWnd->SetDlgItemText(IDC_ABOUT_RICHEDIT, text);
				
				m_cHyperLink.SubclassDlgItem(IDC_STATIC, pWnd);
				
				char buffer[MAX_PATH];
				::GetModuleFileName(hDllInstance,buffer,MAX_PATH);
				pWnd->SetDlgItemText(IDC_DLLPATH, buffer);
				
			}
			
			m_bLogoShowed = true;
		}
	}

	m_pOwnPage = pNewOwnPage;

	return pNewOwnPage;
}

void CMyAboutWindowManager::DestroyOwnPage()
{
	if(m_bLogoShowed)
	{
		// 		if(m_bEditShowed)
		// 		{
		// 			cEdit.DestroyWindow();
		// 			bEditShowed = false;
		// 		}
		m_picLogo.DestroyWindow();
		m_bLogoShowed = false;
	}
	m_cRich.UnsubclassWindow();
	m_cHyperLink.UnsubclassWindow();
	
	delete m_pOwnPage;
}

#ifdef NDEBUG

CWnd* CMyAboutWindowManagerExample::CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID )
{
	CWnd* pNewOwnPage = new CWnd;
	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW,PageRect, pOwnerWindow, nID)) // nID = 0x3000
	{
		if(!m_bLogoShowed)
		{
			AfxInitRichEdit();
			HWND hWnd = ::CreateDialog(hDllInstance,MAKEINTRESOURCE(IDD_ABOUT_FORMEX), pNewOwnPage->GetSafeHwnd(), (DLGPROC)AboutDlgProc);
			if(hWnd)
			{
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				CRect rect(11,11,77,77);
				m_picLogo.CreateEx(WS_EX_NOPARENTNOTIFY,"Static","",
					WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|WS_BORDER|
					SS_BITMAP|SS_CENTERIMAGE,
					rect,pWnd,0x10FF);
				m_picLogo.SetBitmap(::LoadBitmap(hDllInstance,MAKEINTRESOURCE(IDB_LOGO)));
				
				CString text;
				
				pWnd->GetDlgItemText(IDC_ABOUT_STATIC, text.GetBufferSetLength(250), 250);
				text.ReleaseBuffer();
				CString sVersion = ::GetFullVersion();
				text.Replace("_VersionInfo_", sVersion);
				pWnd->SetDlgItemText(IDC_ABOUT_STATIC, text);
				
				// информация о разработчиках
				m_cRich.SubclassDlgItem(IDC_ABOUT_RICHEDIT, pWnd);
				m_cRich.SetBackgroundColor(FALSE, ::GetSysColor(COLOR_BTNFACE));
				text = "";
				m_cRich.SetWindowText(text);
				m_cRich.SetTargetDevice(NULL, 0);
				
				m_cHyperLink.SubclassDlgItem(IDC_STATIC, pWnd);
			}
			
			m_bLogoShowed = true;
		}
	}
	
	m_pOwnPage = pNewOwnPage;
	
	return pNewOwnPage;
}

#endif // #ifdef NDEBUG