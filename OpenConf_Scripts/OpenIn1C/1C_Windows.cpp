
#include "OpenIn1C.h"

#define CMD_FILEOPEN 0x0e101
#define CMD_JOURNALSOPEN 0x083bd
#define CMD_SUBCONTOOPEN 0x083be
#define CMD_REPORTSOPEN 0x08440
#define CMD_PROCESSINGSOPEN 0x08443

typedef struct {
	DWORD ProcessID;
	HWND hWnd;
	int count;
} TProcWindow;

BOOL CALLBACK FindProcessWindow_EnumProc(HWND hWnd, LPARAM lParam)
{
	TProcWindow* ProcWnd = (TProcWindow*)lParam;
	DWORD ProcID = 0;
	GetWindowThreadProcessId(hWnd, &ProcID);
	if( ProcID == ProcWnd->ProcessID && IsWindowVisible(hWnd) )
	{
		ProcWnd->count++;
		char Class[64], Caption[128];
		GetClassName(hWnd, Class, sizeof(Class)-1);
		GetWindowText(hWnd, Caption, sizeof(Caption)-1);
		debug("PID %i: class %s; caption %s\n", (int)ProcID, Class, Caption);

#ifndef _VERSION_FOR_1CV8
		if( strncmp(Class, "Afx:", 4) == 0 )
#else
		if( strncmp(Class, "V8TopLevelFrame", 15) == 0 )
#endif // _VERSION_FOR_1CV8
		{
			ProcWnd->hWnd = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

//Ждём, пока 1С по настоящему загрузится, и сможет отвечать на вопросы
void WaitForOpen1C(DWORD ProcessID)
{
	bool need_pause = false;
	int enum_count = 0;

	while( true )
	{
		TProcWindow ProcWnd = {ProcessID, 0, 0};
		EnumWindows(FindProcessWindow_EnumProc, (LPARAM)(&ProcWnd));

		if( ProcWnd.count == 0 ) //не нашли не одного окна с таким PID
		{
			if( enum_count > 100 ) //всё, дольше ждать уже нет смысла
			{
				break;
			}
		}
		else if( ProcWnd.hWnd != NULL )
		{
			break;
		}

		Sleep(50);

		need_pause = true;
		enum_count++;
	}
	if( need_pause ) Sleep(SleepTime);
}		 

HWND FindProcessWindow(DWORD ProcessID)
{
	WaitForOpen1C(ProcessID);

	TProcWindow ProcWnd = {ProcessID, 0, 0};
	EnumWindows(FindProcessWindow_EnumProc, (LPARAM)(&ProcWnd));
	return ProcWnd.hWnd;
}


BOOL CALLBACK FindOpenDialog_EnumProc(HWND hWnd, LPARAM lParam)
{
	char Class[16];
	GetClassName(hWnd, Class, sizeof(Class)-1);
#ifdef DEBUG_MSG
	char Caption[128];
	GetWindowText(hWnd, Caption, sizeof(Caption)-1);
	debug("FindOpenDialog_EnumProc: Class = %s, Caption = %s, Visible = %c\n", Class, Caption, IsWindowVisible(hWnd)? 'Y' : 'N');
#endif
	if( IsWindowVisible(hWnd) && strcmp(Class, "#32770") == 0 )
	{
		*((HWND*)lParam) = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND FindOpenDialog(HWND hMainWnd)
{
	DWORD ThreadID = GetWindowThreadProcessId(hMainWnd, NULL);
	HWND hDialogWnd = 0;
	EnumThreadWindows(ThreadID, FindOpenDialog_EnumProc, (LPARAM)&hDialogWnd);
	return hDialogWnd;
}

typedef struct {
	char* Class;
	HWND hWnd;
} TFindClass;

BOOL CALLBACK FindByClass_EnumProc(HWND hWnd, LPARAM lParam)
{
	TFindClass* find_struct = (TFindClass*)lParam;
	char Class[64];
	GetClassName(hWnd, Class, sizeof(Class)-1);
	if( IsWindowVisible(hWnd) && strcmp(Class, find_struct->Class) == 0 )
	{
		find_struct->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND FindByClass(HWND hDialogWnd, char* Class)
{
	TFindClass find = {Class, 0};
	HWND hEditWnd = 0;
	EnumChildWindows(hDialogWnd, FindByClass_EnumProc, (LPARAM)&find);
	return find.hWnd;
}

HWND FindEdit(HWND hDialogWnd)
{
	return FindByClass(hDialogWnd, "Edit");
}

HWND FindListBox(HWND hDialogWnd)
{
	return FindByClass(hDialogWnd, "ListBox");
}

HWND WaitForOpenDialog(HWND hMainWnd)
{
	HWND hOpenDialog;
	int counter = 0;
	while( (hOpenDialog = FindOpenDialog(hMainWnd)) == NULL && counter < 100 ) {Sleep(200); counter++; }
	return hOpenDialog;
}		 

//============================================================================================
void OpenObjectIn1C(HWND hMainWnd, LPCSTR FileName)
{
	HWND hOpenDialog = hOpenDialog = FindOpenDialog(hMainWnd);
	if( hOpenDialog != NULL )
		SendMessage(hOpenDialog, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), 0);

	switch( ObjectType )
	{
	case obj_file:
		debug("Open file: ");
		PostMessage(hMainWnd, WM_COMMAND, CMD_FILEOPEN, 0);
		break;
	case obj_report:
		debug("Open report: ");
		PostMessage(hMainWnd, WM_COMMAND, CMD_REPORTSOPEN, 0);
		break;
	case obj_processing:
		debug("Open processing: ");
		PostMessage(hMainWnd, WM_COMMAND, CMD_PROCESSINGSOPEN, 0);
		break;
	case obj_subconto:
		debug("Open subconto: ");
		PostMessage(hMainWnd, WM_COMMAND, CMD_SUBCONTOOPEN, 0);
		break;
	case obj_journal:
		debug("Open journal: ");
		PostMessage(hMainWnd, WM_COMMAND, CMD_JOURNALSOPEN, 0);
		break;
	}
	debug("%s\n", FileName);
	
	hOpenDialog = WaitForOpenDialog(hMainWnd);
	if( hOpenDialog == NULL )
	{
		debug("Open dialog don't appeared!\n");
		return;
	}
	//Check for script question
	char str[1024];
	GetWindowText(hOpenDialog, str, sizeof(str)-1);
	if( stricmp(str, "Навигация") == 0 )
	{
		debug("Reopen open dialog - потому что Навигация\n");
		SendMessage(hOpenDialog, WM_COMMAND, MAKEWPARAM(IDNO, BN_CLICKED), 0);
		hOpenDialog = WaitForOpenDialog(hMainWnd);
		if( hOpenDialog == NULL )
		{
			debug("Open dialog don't appeared!\n");
			return;
		}
	}
#ifdef DEBUG
	GetWindowText(hOpenDialog, str, sizeof(str)-1);
	debug("Open dialog caption: %s\n", str);
#endif

	if( ObjectType == obj_file )
	{
		HWND hEditWnd = FindEdit(hOpenDialog);
		if( hEditWnd == NULL )
		{
			debug("File name Edit not found!\n");
			return;
		}
		SendMessage(hEditWnd, WM_SETTEXT, 0, (LPARAM)FileName);
	}
	else
	{
		HWND hListWnd = FindListBox(hOpenDialog);
		if( hListWnd == NULL ) return;

		int count = SendMessage(hListWnd, LB_GETCOUNT, 0, 0);
		char str[1024];
		int i;
		for( i = 0; i < count; i++ )
		{
			SendMessage(hListWnd, LB_GETTEXT, i, (LPARAM)str);
			if( strcoll(str, FileName) == 0 )
			{
				debug("\t Object %s found in list\n", FileName);
				SendMessage(hListWnd, LB_SETCURSEL, i, 0);
				break;
			}
		}
		if( i >= count )
		{	
			debug("\t Object %s not found in list\n", FileName);
			SendMessage(hOpenDialog, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), 0);
			return;
		}
	}

	debug("Now we are ready to press OK button\n");
	SendMessage(hOpenDialog, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), 0);
	SetForegroundWindow(hMainWnd);
}

void OpenByIndex(int index, const char* fname)
{
	HWND hWnd = FindWindowByIndex(index);
	debug("1C hWnd = %lx\n", (long int)hWnd);
	if( hWnd != NULL ) OpenObjectIn1C(hWnd, fname);
}

void OpenByListIndex(HWND hListWnd, const char* fname)
{
	int index = SendMessage(hListWnd, LB_GETCURSEL, 0, 0);
	OpenByIndex(index, fname);
}
