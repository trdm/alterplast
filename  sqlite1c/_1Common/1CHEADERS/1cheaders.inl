/*
1cheaders.inl
Данный файл содержит определения некоторых глобальных объектов,
практически всегда используемых в 1С-проектах, и несколько полезных
функций.
Автоматически подключает и необходимые lib'ы/
Лучше всего включить его в stdafx.cpp в нужных проектах
*/

// Сразу добавим упаковку сегментов
#pragma comment(linker,"/FILEALIGN:512")

// Подключим либы
// Путь к либам должен задаватся от каталога текущего проекта, а не от каталога этого файла
// По умолчанию каталог проекта на одном уровне с каталогом _1Common
// Если это не так, то перед включением 1cheaders.inl нужно определить нужный HEADER1C_PATH
#ifndef HEADER1C_PATH
	#define HEADER1C_PATH "../_1Common/1cheaders"
#endif

#pragma comment (lib,HEADER1C_PATH "/libs/basic.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/bkend.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/blang.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/br32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/dbeng32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/editr.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/frame.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/moxel.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/rgproc.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/seven.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/txtedt.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/type32.lib")
#pragma comment (lib,HEADER1C_PATH "/libs/userdef.lib")

void DoStsLine(LPCSTR format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoStatusLine(txt);
}

CString GetErrorDescription(DWORD err/*=0*/)
{
	if(!err)
		err=GetLastError();
	CString errMsg;
	errMsg.Format("Код ошибки Windows: 0x%X",err);
	LPTSTR lpMsgBuf;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,err,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf,0,NULL))
	{
		errMsg=errMsg+"\r\n"+lpMsgBuf;
		LocalFree(lpMsgBuf);
	}
	return errMsg;
}

// Инициализация полезных глобальных переменных
void Init1CGlobal(HINSTANCE hInst)
{
	pMainApp		= (CApp7*)AfxGetApp();
	pBkEndUI		= GetBkEndUI();
	pMetaDataCont	= GetMetaData();
	pDataBase7		= CDataBase7::GetDefault();
	h1CResource		= AfxGetResourceHandle();
	hMyInst			= hInst;
	// В режиме работы конфигуратора при загрузке плагина
	// основное окно еще не создано, pMainFrame есть сплэш-заставка
	// Основное окно передается в событии ON_CREATE_MAIN_WND
	pMainFrame		= (CMainFrame*)pMainApp->m_pMainWnd;
//	pMainFrame		= (SECMDIFrameWnd*)pMainApp->m_pMainWnd;
}

void RuntimeError(LPCSTR format, ...)
{
        va_list arg;
        va_start(arg, format);
        CString txt;
        txt.FormatV(format, arg);
        va_end(arg);
        CBLModule::RaiseExtRuntimeError((LPCSTR)txt, mmRedErr);
}

void RuntimeErrorRes(UINT nID)
{
	CString err;
	err.LoadString(nID);
	CBLModule::RaiseExtRuntimeError(err, mmRedErr);
}
