#include "OpenIn1C.h"
#include <windowsx.h>
#include <wingdi.h>
#include <locale.h>
#include "get_opt.h"

#define VK_ESC 0x1b
#define VK_ENTER 0x0d
//#define VK_UP 0x26
//#define VK_DOWN 0x28

//#define _SHOW_ADVERTS_
#ifdef _SHOW_ADVERTS_
const char* AdvertisingSite = "http://www.1cpp.ru";
const char* AdvertisingMessage = "Заходи на www.1cpp.ru! Там столько вкусного!";
#endif

HINSTANCE hInst = NULL;		// Instance handle
HWND hMainWnd = NULL;		//Main window handle
HWND hListWnd = NULL;
HWND hInfoWnd = NULL;
HWND hAdvertsWnd = NULL;

int argc = 0;
char **argv = NULL;
int SleepTime = 500; //пауза, которую нужно сделать при запуске

char* _1CExecutable = NULL, *_1CExecParameters = NULL;
TObjectType ObjectType;
char* ObjectName = NULL;


#ifdef DEBUG_MSG
	#define DEBUG_FNAME "c:\\debug"
	FILE* DebugFile;
#endif

void debug(char *msg, ...)
{
	#ifdef DEBUG_MSG
	va_list ap;
	va_start(ap, msg);
	vfprintf(DebugFile, msg, ap);
	fflush(DebugFile);
	va_end(ap);
	#endif
}

int InfoBarHeight()
{
	return 20;
}

void SetInfoText()
{
	int index = SendMessage(hListWnd, LB_GETCURSEL, 0, 0);
	char *ItemData = (char*)SendMessage(hListWnd, LB_GETITEMDATA, (WPARAM)index, 0);
	
	if( ItemData == NULL )
	{
		char ItemText[4096];
		SendMessage(hListWnd, LB_GETTEXT, (WPARAM)index, (LPARAM)ItemText);
		SetWindowText(hInfoWnd, ItemText);
	}
	else
	{
		SetWindowText(hInfoWnd, ItemData);
	}
}

void WM_Size(HWND hwnd, UINT SizeType, int Width, int Height)
{
	RECT Rect;
	int IBHeight = InfoBarHeight();

	GetClientRect(hwnd, &Rect);
	Width = Rect.right - Rect.left - 2;
	Height = Rect.bottom - Rect.top - IBHeight;
	
	#ifdef _SHOW_ADVERTS_
	Height -= IBHeight;
	#endif
	
	MoveWindow(hListWnd, 1, 1, Width, Height, TRUE);

	MoveWindow(hInfoWnd, 1, 2+Height, Width, IBHeight, TRUE);

	#ifdef _SHOW_ADVERTS_
	MoveWindow(hAdvertsWnd, 1, 2+Height+IBHeight, Width, IBHeight, TRUE);
	#endif
}

void ExecuteListItem()
{
	int nItem = SendMessage(hListWnd, LB_GETCURSEL, 0, 0);
	void* ItemData = (void*)SendMessage(hListWnd, LB_GETITEMDATA, (WPARAM)nItem, 0);

	if( ItemData == NULL )
	{
		OpenByListIndex(hListWnd, ObjectName);
	}
	else
	{
		RunByListIndex(hListWnd, ObjectName);
	}
}


void WM_Command(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if( hwndCtl == hListWnd )
	{
		switch( codeNotify )
		{
		case LBN_DBLCLK:
			ExecuteListItem();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case LBN_SELCHANGE:
			SetInfoText();
			break;
		}
	}
	else if( hwndCtl == hAdvertsWnd )
	{
		switch( codeNotify )
		{
		case STN_DBLCLK:
		case STN_CLICKED:
			#ifdef _SHOW_ADVERTS_
			ShellExecute(NULL, "open", AdvertisingSite, NULL, NULL, SW_SHOW);
			#endif
			break;
		}
	}
}

int WM_OnVkeyToItem(HWND hWnd, UINT vk, HWND hListWnd, int iCaret)
{
	if( vk == VK_UP && SendMessage(hListWnd, LB_GETCURSEL, 0, 0) == 0)
		return SendMessage(hListWnd, LB_GETCOUNT, 0, 0) - 1; //wrap cursor to end of list
	else if( vk == VK_DOWN && SendMessage(hListWnd, LB_GETCURSEL, 0, 0) == (SendMessage(hListWnd, LB_GETCOUNT, 0, 0) - 1) )
		return 0; //wrap cursor to beginning of list
	else if( vk == VK_ESC )
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	else if( vk == VK_ENTER )
	{
		ExecuteListItem();
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	return -1;
}		 

void ClearListBoxData()
{
	int i = 0, nItems;

	nItems = SendMessage(hListWnd, LB_GETCOUNT, 0, 0);

	while( i < nItems )
	{
		char *ItemData = (char*)SendMessage(hListWnd, LB_GETITEMDATA, (WPARAM)i++, 0);
		if( ItemData ) free(ItemData);
	}
}

LRESULT CALLBACK MainWndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
		HANDLE_MSG(hWnd, WM_COMMAND, WM_Command);
		HANDLE_MSG(hWnd, WM_SIZE, WM_Size);
		HANDLE_MSG(hWnd, WM_VKEYTOITEM, WM_OnVkeyToItem);
		
		case WM_ACTIVATE:
			SetFocus(hListWnd);
			break;
/*		case WM_LBUTTONDOWN:
			ExecuteListItem();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
*/			
		case WM_CTLCOLORSTATIC:
			if( (HWND)lParam == hAdvertsWnd )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor(hdc, RGB(00, 00, 0xff));
			}
			break;
		case WM_CLOSE:
			ClearListBoxData();
			if( _1CExecutable != NULL ) free(_1CExecutable);
			if( _1CExecParameters != NULL ) free(_1CExecParameters);

			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}

void SetWinPos()
{
	int count = SendMessage(hListWnd, LB_GETCOUNT, 0, 0);
	char str[1024];
	int x, y, w, h;
	HDC dc = GetDC(NULL);
	SIZE sz;
	
	GetWindowText(hMainWnd, str, sizeof(str)-1);
	GetTextExtentPoint32(dc, str, strlen(str), &sz);
	w = sz.cx;
	
	#ifdef _SHOW_ADVERTS_
	GetTextExtentPoint32(dc, AdvertisingMessage, strlen(AdvertisingMessage), &sz);
	if( sz.cx > w ) w = sz.cx;
	#endif
	
#if defined(WS_ACTIVECAPTION) //MSVC почему-то упорно не хочет видеть то, что в winuser.h прописано. А GCC видит прекрасно.
	WINDOWINFO wi;
	wi.cbSize = sizeof(wi);
	GetWindowInfo(hMainWnd, &wi);
	h = (wi.rcWindow.bottom - wi.rcWindow.top) + 5 + (SendMessage(hListWnd, LB_GETITEMHEIGHT, 0, 0) * count);
#else
	h = 30 + 5 + (SendMessage(hListWnd, LB_GETITEMHEIGHT, 0, 0) * count);
#endif
	
	for( int i = 0; i < count; i++ )
	{
		SendMessage(hListWnd, LB_GETTEXT, i, (LPARAM)str);
		GetTextExtentPoint32(dc, str, strlen(str), &sz);
		w = max(w, sz.cx);
	}
	w += 30; //всякие рамки и отступы

	int max_w = (int)((double)GetDeviceCaps(dc, HORZRES) * 0.7);
	int max_h = (int)((double)GetDeviceCaps(dc, VERTRES) * 0.5);
	if( w > max_w ) w = max_w;
	if( h > max_h ) h = max_h;
	
	x = (GetDeviceCaps(dc, HORZRES) - w) / 2;
	y = (GetDeviceCaps(dc, VERTRES) - h) / 2;
	ReleaseDC(NULL, dc);
	
	h += InfoBarHeight();
	#ifdef _SHOW_ADVERTS_
	h += InfoBarHeight();
	#endif
	
	MoveWindow(hMainWnd, x, y, w, h, TRUE);
}

void RegisterWindowClass()
{
	WNDCLASS wc;
	memset(&wc,0,sizeof(WNDCLASS));
	wc.style = CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "OpenIn1C_WndClass";
	wc.lpszMenuName = NULL;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = NULL;
	RegisterClass(&wc);
}

/*<---------------------------------------------------------------------->*/
void CreateMainWnd()
{
	RegisterWindowClass();

	char str[1024];
	sprintf(str, "Открыть %s", argv[1]);
	hMainWnd = CreateWindow("OpenIn1C_WndClass", str,
		WS_VISIBLE|WS_CLIPCHILDREN|WS_BORDER|WS_THICKFRAME,
		0, 0, 0, 0,
		NULL,
		NULL,
		hInst,
		NULL);

	//Create list control
	hListWnd = CreateWindowEx(0, "LISTBOX", "Запущеные копии 1С",
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | 
			LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT | LBS_NOTIFY,
		0, 0, 0, 0,
		hMainWnd, NULL, hInst, NULL);
		
	//Create info bar
	hInfoWnd = CreateWindowEx(0, "STATIC", "",
		WS_VISIBLE | WS_CHILD | SS_CENTER,
		0, 0, 12, 0,
		hMainWnd, NULL, hInst, NULL);

	#ifdef _SHOW_ADVERTS_
	hAdvertsWnd = CreateWindowEx(0, "STATIC", "",
		WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
		0, 0, 12, 0,
		hMainWnd, NULL, hInst, NULL);
	SetWindowText(hAdvertsWnd, AdvertisingMessage);
	#endif
		
	SetWinPos();

	FillList(hListWnd);
	int nItems = SendMessage(hListWnd, LB_GETCOUNT, 0, 0);
	if( nItems == 0 )
	{
		if( CheckFor1CExecutable() )
		{
			FillListByRegistry(hListWnd);
		}
	}

	SetWinPos();

	nItems = SendMessage(hListWnd, LB_GETCOUNT, 0, 0);
	if( nItems == 0 )
	{
		PostMessage(hMainWnd, WM_CLOSE, 0, 0);
	}
	else
	{
		SendMessage(hListWnd, LB_SETCURSEL, 0, 0);
		SetInfoText();
	}
}

//Может быть 3 аргумента. Аргументы позиционные
// 1) имя объекта, который нужно открыть
// 2) тип объекта:
//    -r - отчёт
//    -p - обработка
//    -s - справочник
//    -j - журнал
//    любое другое значение - внешний отчёт
// 3) время паузы после открытия основного окна (если 1С только что запущена). В миллисекундах.
bool ParseCommandLine()
{
	argv = (char**)StrToArgv((char*)GetCommandLine(), &argc);
	if( argc < 2 ) return false;
	ObjectName = argv[1];
	ObjectType = obj_file;
	if( argc >= 3 )
	{
		if( strcmp(argv[2], "-r") == 0 )
			ObjectType = obj_report;
		else if( strcmp(argv[2], "-p") == 0 )
			ObjectType = obj_processing;
		else if( strcmp(argv[2], "-s") == 0 )
			ObjectType = obj_subconto;
		else if( strcmp(argv[2], "-j") == 0 )
			ObjectType = obj_journal;
	}
	
	if( argc >= 4 )
	{
		SleepTime = atoi(argv[3]);
	}
			 
	return true;
}

bool CheckForDefaults()
{
	int nProcesses = ProcessCount();
	debug("nProcesses = %i\n", nProcesses);

	//if( nProcesses <= 0 ) return true;
	if( nProcesses == 1 )
	{
		OpenByIndex(nProcesses-1, ObjectName);
		return true;
	}

	if( TryToOpenInChildProcess(ObjectName) ) return true;

	return false;
}		 

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	//*
	#ifdef DEBUG_MSG
	DebugFile = fopen(DEBUG_FNAME, "wt");
	#endif
	debug("START\n");

	debug("SetLocale\n");
	setlocale(LC_ALL, ".ACP");

	debug("ParseCommandLine\n");
	if( !ParseCommandLine() ) goto end;
	debug("OpenSnapshot\n");
	if( !OpenSnapshot() ) goto end;
	debug("CheckForDefaults\n");
	if( CheckForDefaults() ) goto end;

	debug("CreateMainWnd\n");
	CreateMainWnd();
	if( hMainWnd == NULL ) goto end;
	ShowWindow(hMainWnd, SW_SHOW);
	SetFocus(hListWnd);
	
	MSG msg;
	while( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
end:
	if( argv != NULL ) free(argv);
	CloseSnapshot();
	#ifdef DEBUG_MSG
	fclose(DebugFile);
	#endif
	//*/
	return 0;
}

