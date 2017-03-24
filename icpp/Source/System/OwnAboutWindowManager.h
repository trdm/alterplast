//////////////////////////////////////////////////////////////////////
//
// Разработчик: Артур Аюханов aka artbear, mailto: artbear@inbox.ru
//
//////////////////////////////////////////////////////////////////////
//
// интерфейс IOwnAboutWindowManager - для возможности встраивания собственных окон "О программе" в стандартное окно 1С "О программе"
//
// Важно: обратите внимание на особенность реализации оконной функции для своего окна (ниже примерный код AboutDlgProc)
//
// Пример подключения собственного окна ( должны быть загружены 1С++ или FormEx )
//	
// 	CBLPtr<CBLContext> pCont;
// 	try
// 	{
// 		LPCSTR szClassName = "Перехватчик";
// 		pCont.Create(szClassName);
// 	}
// 	catch(...)
// 	{
// 		try
// 		{
// 			LPCSTR szClassName = "Сервис";
// 			pCont.Create(szClassName);
// 		}
// 		catch(...)
// 		{
// 		}
// 	}
// 
// 	if (pCont)
// 		CBLContext* pCont2 = pCont;
// 		IAddOwnAboutWindowManager *pAddOwnAboutWindowManagerContext = dynamic_cast<IAddOwnAboutWindowManager *>(pCont2);
// 		if (pAddOwnAboutWindowManagerContext)
// 			pAddOwnAboutWindowManagerContext->Add(GetOwnAboutWindowManager()); // собственный объект !
// 	}
// 
//
// ВАЖНО: оконная функция нового окна должна выполнять следующие функции
//
// LRESULT WINAPI AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
// 	if(uMsg == WM_CLOSE)
// 	{
// 		HWND hWnd = ::GetParent(hDlg);
// 		if(hWnd)
// 		{
// 			hWnd = ::GetParent(hWnd);
// 			if(hWnd)
// 			{
// 				hWnd = ::GetParent(hWnd);
// 				if(hWnd)
// 					::SendMessage(hWnd, uMsg, wParam, lParam);
// 			}
// 		}
// 	}
// 	else if(uMsg == WM_INITDIALOG)
// 	{
// 		return TRUE;
// 	}
// 	return FALSE;
// }

#if !defined(_OWN_ABOUT_WINDOW_MANAGER__INCLUDED_)
	#define _OWN_ABOUT_WINDOW_MANAGER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IOwnAboutWindowManager
{
public:
	// текст закладки
	virtual LPCSTR GetTabName() = 0;
	
	// возвращает хендл картинки, показываемой рядом с текстом закладки
	// если вернуть NULL, картинки не будет
	// если не NULL, то хендл будет удален после выполнения данного метода, т.е. управление хендлом от объекта забирается
	virtual HBITMAP GetBitmapHandle() = 0; // должна быть использована маска RGB(0x00,0x80,0x80)

	// метод CreateOwnPage должен создать через new окно CWnd или его наследника
	// далее выполнить 	создание окна через метод CWnd::Create()
	// Пример кода:
	// 	CWnd* pNewOwnPage = new CWnd;
	// 	if(pNewOwnPage->Create("STATIC","",WS_CHILDWINDOW, PageRect, pOwnerWindow, nID))
	//	далее код наполнения окна содержимым
	virtual CWnd* CreateOwnPage( CWnd* pOwnerWindow, const CRect& PageRect, UINT nID ) = 0;

	// возвращает окно, ранее созданное в CreateOwnPage
	virtual CWnd* GetPage() = 0;

	// удаление данных окна, а деструктор объекта будет вызван только при завершении работы 1С
	virtual void DestroyOwnPage() = 0;
};

// этот интерфейс будет реализован объектов CService
class IAddOwnAboutWindowManager
{
public:
	// фактически это статический метод, он не должен зависеть от времени жизни основного объекта !
	virtual void Add(IOwnAboutWindowManager* pOwnAboutWindowManager) = 0;
};

// интерфейс IAddOwnAboutWindowManagerContext должен использоваться при добавлении объектов собственных окон "О программе"
// сторонними ВК
// первой базой указан CBLContext для возможности использования исходников, созданных в разных средах MS Visual C++
class IAddOwnAboutWindowManagerContext : public IAddOwnAboutWindowManager, public CBLContext
{
protected:
	virtual void Dummy() {};	
};


#endif