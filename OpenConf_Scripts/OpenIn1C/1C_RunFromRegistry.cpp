
#include <process.h>
#include "OpenIn1C.h"

bool FileExists(char* fname)
{
	HANDLE f;

	f = CreateFile(fname, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if( f != INVALID_HANDLE_VALUE )
	{
		CloseHandle(f);
		return true;
	}

	return false;
}

bool DatabaseExists(char* BasePath)
{
	char MDPath[8192];
	sprintf(MDPath, "%s1cv7.md", BasePath);

	return FileExists(MDPath);
}

HKEY _OpenRegKey(HKEY KEY_ROOT, const char* KeyName)
{
	HKEY key;
	DWORD Disposition;
	LONG res = RegCreateKeyEx(KEY_ROOT, KeyName, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &Disposition);

	if( res != ERROR_SUCCESS ) return NULL;

	return key;
}

bool GetRegKeyValue(HKEY KEY_ROOT, const char* KeyName, const char* ValName, char* Value, DWORD val_len)
{
	HKEY key = _OpenRegKey(KEY_ROOT, KeyName);

	if( key == NULL ) return false;

	DWORD Type = REG_SZ;

	LONG res = RegQueryValueEx(key, TEXT(ValName), NULL, &Type, (LPBYTE)Value, &val_len);
	
	if( res != ERROR_SUCCESS )
	{
		return false;
	}

	RegCloseKey(key);

	if( Type != REG_SZ )
	{
		return false;
	}

	return true;
}

void SetRegKeyValue(HKEY KEY_ROOT, const char* KeyName, const char* ValName, const char* Value)
{
	HKEY key = _OpenRegKey(KEY_ROOT, KeyName);

	if( key == NULL ) return;

	RegSetValueEx(key, ValName, 0, REG_SZ, (CONST BYTE*)Value, strlen(Value)+1);

	RegCloseKey(key);
}

bool CheckFor1CExecutable()
{
	char Path[8192];
	DWORD PathSize = sizeof(Path);
	enum {_1C_path_NotFound, _1C_path_InOpenIn1C, _1C_path_From1C} PathFound = _1C_path_NotFound;

#ifdef _VERSION_FOR_1CV8
	const char* RegValName_1CExecutable = "1CExecutableV8";
	const char* DefExecPath = "C:\\Program Files\\1Cv8\\Bin\\1cv8.exe";
	const char* RegValName_ExecParameters = "ExecParametersV8";
	//ApplicationClass для 8.1
	const char* RegKey_ApplicationClass = "SOFTWARE\\Classes\\CLSID\\{b3a7d9db-3cba-47f4-b80a-5dda79d8925a}\\LocalServer32";
	const char* RegKeyHKCLS_ApplicationClass = "CLSID\\{b3a7d9db-3cba-47f4-b80a-5dda79d8925a}\\LocalServer32";
#else
	const char* RegValName_1CExecutable = "1CExecutable";
	const char* DefExecPath = "C:\\Program Files\\1Cv77\\Bin\\1cv7.exe";
	const char* RegValName_ExecParameters = "ExecParameters";
	const char* RegKey_ApplicationClass = "SOFTWARE\\Classes\\CLSID\\{63112D80-94F0-11D2-9F39-008048DA120F}\\LocalServer32";
	const char* RegKeyHKCLS_ApplicationClass = "CLSID\\{63112D80-94F0-11D2-9F39-008048DA120F}\\LocalServer32";
#endif


	if( GetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_1CExecutable, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_InOpenIn1C;
	}
	else if( GetRegKeyValue(HKEY_LOCAL_MACHINE, RegKey_ApplicationClass, NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_CURRENT_USER, RegKey_ApplicationClass, NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_CLASSES_ROOT, RegKeyHKCLS_ApplicationClass, NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
#ifdef _VERSION_FOR_1CV8
	//Проверим регистрацию Application для 8.0
	else if( GetRegKeyValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\CLSID\\{FB17AD3E-5F20-453A-AE83-2711006155CF}\\LocalServer32", NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\CLSID\\{FB17AD3E-5F20-453A-AE83-2711006155CF}\\LocalServer32", NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_CLASSES_ROOT, "CLSID\\{FB17AD3E-5F20-453A-AE83-2711006155CF}\\LocalServer32", NULL, Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
#else
	else if( GetRegKeyValue(HKEY_CURRENT_USER, "Software\\1C\\1Cv7\\7.7\\1С:Предприятие для SQL", "1CPath", Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_LOCAL_MACHINE, "Software\\1C\\1Cv7\\7.7\\1С:Предприятие для SQL", "1CPath", Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_CURRENT_USER, "Software\\1C\\1Cv7\\7.7\\1С:Предприятие", "1CPath", Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
	else if( GetRegKeyValue(HKEY_LOCAL_MACHINE, "Software\\1C\\1Cv7\\7.7\\1С:Предприятие", "1CPath", Path, PathSize) && FileExists(Path) )
	{
		PathFound = _1C_path_From1C;
	}
#endif

	if( PathFound == _1C_path_NotFound )
	{
		SetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_1CExecutable, DefExecPath);
		char ErrMsg[8192];
		GetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_1CExecutable, Path, PathSize);
		sprintf(ErrMsg, "Файл '%s' не найден!\nСм. реестр, HKCU\\Software\\OpenIn1C\\%s", Path, RegValName_1CExecutable);
		MessageBox(NULL, ErrMsg, "Ошибка запуска 1С", MB_OK | MB_ICONERROR);
		return false;
	}

	if( PathFound == _1C_path_From1C )
	{
		SetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_1CExecutable, Path);
	}

	

	_1CExecutable = (char*)malloc(strlen(Path) + 1);
	strcpy(_1CExecutable, Path);


	//Execute parameters
	char Parameters[1024];
	DWORD ParamsSize = sizeof(Parameters);
	if( !GetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_ExecParameters, Parameters, ParamsSize) )
	{
		strcpy(Parameters, "config");
		SetRegKeyValue(HKEY_CURRENT_USER, "Software\\OpenIn1C", RegValName_ExecParameters, Parameters);
	}

	_1CExecParameters = (char*)malloc(strlen(Parameters) + 1);
	strcpy(_1CExecParameters, Parameters);


	return true;
}

void FillListByRegistry(HWND hListWnd)
{
	HKEY keyBases = NULL;
	LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\1C\\1Cv7\\7.7\\Titles", 0, KEY_READ, &keyBases);

	if( res != ERROR_SUCCESS ) return;

	int i = 0;
	char BasePath[4096];
	DWORD BasePathSize = sizeof(BasePath);
	DWORD ValType;
	char BaseName[4096];
	DWORD BaseNameSize = sizeof(BaseName);

	int nItem = 0;

	while( ERROR_NO_MORE_ITEMS != RegEnumValue(keyBases, i++, BasePath, &BasePathSize, NULL, &ValType, (LPBYTE)BaseName, &BaseNameSize) )
	{
		if( ValType == REG_SZ )
		{
			if( DatabaseExists(BasePath) )
			{
				char *ItemData = (char*)malloc(BasePathSize+1);
				strcpy(ItemData, BasePath);

				SendMessage(hListWnd, LB_ADDSTRING, 0, (LPARAM)BaseName);
				SendMessage(hListWnd, LB_SETITEMDATA, (WPARAM)nItem, (LPARAM)ItemData);
				nItem++;
			}
		}

		BasePathSize = sizeof(BasePath);
		BaseNameSize = sizeof(BaseName);
	}

	RegCloseKey(keyBases);
}


void RunByListIndex(HWND hListWnd, const char* fname)
{
	int index = SendMessage(hListWnd, LB_GETCURSEL, 0, 0);
	char *ItemData = (char*)SendMessage(hListWnd, LB_GETITEMDATA, (WPARAM)index, 0);

	char CommandLine[8192];
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcInfo;

	StartupInfo.cb = sizeof(StartupInfo);
    StartupInfo.lpReserved = NULL;
    StartupInfo.lpDesktop = NULL;
    StartupInfo.lpTitle = NULL;
    StartupInfo.dwFlags = 0;
    StartupInfo.cbReserved2 = 0;
    StartupInfo.lpReserved2 = NULL;

	sprintf(CommandLine, "\"%s\" %s /d\"%s\"", _1CExecutable, _1CExecParameters, ItemData);
	CreateProcess(_1CExecutable, CommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcInfo);

	DWORD idProcess = ProcInfo.dwProcessId;
	debug("Run 1C. PID = %x\n", idProcess);

	WaitForOpen1C(idProcess);
	HWND hProcWnd = FindProcessWindow(idProcess);

	debug("PID = %x (%i): HWND = %x\n", idProcess, idProcess, hProcWnd);

	if( hProcWnd != NULL )
	{
		OpenObjectIn1C(hProcWnd, fname);
	}
}
