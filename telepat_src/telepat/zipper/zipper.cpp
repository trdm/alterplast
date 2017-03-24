#include "windows.h"
#import "k:\zlibeng.dll" no_namespace raw_interfaces_only named_guids

void main(int argv, char** argc)
{
	if(argv<2)
		return;
	HANDLE hFile=CreateFile(argc[1],GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==(HANDLE)-1)
		return;
	CoInitialize(NULL);
	IzlibEnginePtr pZlib;
	pZlib.CreateInstance(L"V75.zlibEngine");

	IStream *pFrom,*pTo;
	CreateStreamOnHGlobal(NULL,TRUE,&pFrom);
	CreateStreamOnHGlobal(NULL,TRUE,&pTo);
	DWORD size=GetFileSize(hFile,NULL);
	char* buf=new char[size];
	DWORD rw;
	ReadFile(hFile,buf,size,&rw,NULL);
	ULONG writen;
	pFrom->Write(buf,size,&writen);
	delete [] buf;

	LARGE_INTEGER lNull;
	lNull.QuadPart=0;
	pFrom->Seek(lNull,0,NULL);
	HRESULT hr=pZlib->pkCompress(pFrom,pTo);

	HGLOBAL hOut=NULL;
	LPVOID  lpMemStream = NULL;
	STATSTG St;
	pTo->Stat(&St,STATFLAG_NONAME);
	ULONG cbStreamSizeOut=St.cbSize.LowPart;
	GetHGlobalFromStream(pTo, &hOut);
	if(hOut)
	{
		lpMemStream=GlobalLock(hOut);
		buf=new char[cbStreamSizeOut];
		memcpy(buf, lpMemStream, cbStreamSizeOut);            
		GlobalUnlock(hOut);
		GlobalFree(hOut);
		CloseHandle(hFile);
		char name[MAX_PATH];
		wsprintf(name,"%s.bin",argc[1]);
		hFile=CreateFile(name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
		WriteFile(hFile,buf,cbStreamSizeOut,&rw,NULL);
		CloseHandle(hFile);
		delete [] buf;

	}
	pFrom->Release();
	pTo->Release();
	pZlib=NULL;
	CoUninitialize();
}
