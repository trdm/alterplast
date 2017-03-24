
#ifndef OpenIn1C
#define OpenIn1C

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

//#define DEBUG_MSG 1

typedef enum {
	obj_file,
	obj_report,
	obj_processing,
	obj_subconto,
	obj_journal
} TObjectType;


extern char* _1CExecutable;
extern char* _1CExecParameters;

extern TObjectType ObjectType;
extern int SleepTime; //пауза, которую нужно сделать при запуске

void debug(char *msg, ...);

HWND FindProcessWindow(DWORD ProcessID);
HWND FindOpenDialog(HWND hMainWnd);
HWND FindByClass(HWND hDialogWnd, char* Class);
HWND FindEdit(HWND hDialogWnd);
HWND FindListBox(HWND hDialogWnd);

void FillList(HWND hListWnd);
void FillListByRegistry(HWND hListWnd);

bool CheckFor1CExecutable();

bool OpenSnapshot();
void CloseSnapshot();
int ProcessCount();
HWND FindWindowByIndex(int index);
bool TryToOpenInChildProcess(const char* fname);
void FillList(HWND hListWnd);

void WaitForOpen1C(DWORD ProcessID);

void OpenObjectIn1C(HWND hMainWnd, LPCSTR FileName);
void OpenByIndex(int index, const char* fname);
void OpenByListIndex(HWND hListWnd, const char* fname);

void RunByListIndex(HWND hListWnd, const char* fname);


#endif

