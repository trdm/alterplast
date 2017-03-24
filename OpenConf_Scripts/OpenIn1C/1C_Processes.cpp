
#include "OpenIn1C.h"
#include <tlhelp32.h>
#include "OpenIn1C.h"

static HANDLE hSnapshot = 0;
static PROCESSENTRY32 Process;
static DWORD ParentProcessID = 0;
static DWORD ChildProcessID = 0;

DWORD FindParentProcess1C(HANDLE hSnapshot, PROCESSENTRY32* Process);
DWORD FindChildProcess(HANDLE hSnapshot, PROCESSENTRY32* Process, DWORD ProcessID);

bool OpenSnapshot()
{
	Sleep(50); //при запуске из конфигуратора, одновременно с 1С, процесс не успевает "проявиться"
	Process.dwSize = sizeof(Process);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hSnapshot == INVALID_HANDLE_VALUE ) return false;

	ParentProcessID = FindParentProcess1C(hSnapshot, &Process);
	if( ParentProcessID != 0 )
		ChildProcessID = FindChildProcess(hSnapshot, &Process, ParentProcessID);

	return true;
}

void CloseSnapshot()
{
	if( hSnapshot != 0 ) CloseHandle(hSnapshot);
}

bool Is1C(char* fname)
{

#ifndef _VERSION_FOR_1CV8
	return 
		stricmp(fname, "1cv7s.exe") == 0 || 
		stricmp(fname, "1cv7.exe")  == 0 ||
		stricmp(fname, "1cv7l.exe") == 0;
#else
	return stricmp(fname, "1cv8.exe") == 0;
#endif // _VERSION_FOR_1CV8

}

DWORD FindProcess(HANDLE hSnapshot, PROCESSENTRY32* Process, DWORD ProcessID)
{
	Process->dwSize = sizeof(PROCESSENTRY32);
	BOOL res = Process32First(hSnapshot, Process);
	while( res && Process->th32ProcessID != ProcessID )
	{
		Process->dwSize = sizeof(PROCESSENTRY32);
		res = Process32Next(hSnapshot, Process);
	}
	if( res ) return Process->th32ProcessID;
	return 0;
}

DWORD FindChildProcess(HANDLE hSnapshot, PROCESSENTRY32* Process, DWORD ProcessID)
{
	BOOL res = Process32First(hSnapshot, Process);
	while( res )
	{
		if( Process->th32ParentProcessID == ProcessID && Is1C(Process->szExeFile) )
			return Process->th32ProcessID;
		res = Process32Next(hSnapshot, Process);
	}
	return 0;
}

DWORD FindParentProcess1C(HANDLE hSnapshot, PROCESSENTRY32* Process)
{
	DWORD ProcessID = GetCurrentProcessId();

	FindProcess(hSnapshot, Process, ProcessID);
	while( (ProcessID = FindProcess(hSnapshot, Process, Process->th32ParentProcessID)) != 0 )
	{
		if( Is1C(Process->szExeFile) )
			return Process->th32ProcessID;
	}
	return 0;
}

int ProcessCount()
{
	int count = 0;
	BOOL res = Process32First(hSnapshot, &Process);
	while( res )
	{
		debug("%lx: %s\n", Process.th32ProcessID, Process.szExeFile);
		if( Is1C(Process.szExeFile) && Process.th32ProcessID != ParentProcessID )
			count++;
		res = Process32Next(hSnapshot, &Process);
	}
	return count;
}


bool TryToOpenInChildProcess(const char* ObjectName)
{
	if( ParentProcessID == 0 ) return false;
	if( ChildProcessID == 0 ) return false;

	HWND hWnd = FindProcessWindow(ChildProcessID);
	if( hWnd == NULL ) return false;

	OpenObjectIn1C(hWnd, ObjectName);

	return true;
}

void FillList(HWND hListWnd)
{
	BOOL res = Process32First(hSnapshot, &Process);
	while( res )
	{
		if( Is1C(Process.szExeFile) && Process.th32ProcessID != ParentProcessID )
		{
			HWND hWnd = FindProcessWindow(Process.th32ProcessID);
			if( hWnd != NULL )
			{	
				char str[1024];
				GetWindowText(hWnd, str, sizeof(str)-1);
				SendMessage(hListWnd, LB_ADDSTRING, 0, (LPARAM)str);
			}
		}
		res = Process32Next(hSnapshot, &Process);
	}
}

HWND FindWindowByIndex(int index)
{
	int i = 0;

	BOOL res = Process32First(hSnapshot, &Process);
	while( res )
	{
		if( Is1C(Process.szExeFile) && Process.th32ProcessID != ParentProcessID  && FindProcessWindow(Process.th32ProcessID) != NULL )
		{
			if( i == index )
			{
				return FindProcessWindow(Process.th32ProcessID);
			}
			i++;
		}
		res = Process32Next(hSnapshot, &Process);
	}
	return NULL;
}

