/*--------------------------------------------------------------------------------
1cheaders.h
Данный файл предназначен для его включения в stdafx.h разнообразных проектов,
нуждающихся в доступе к 1С dll'кам.
--------------------------------------------------------------------------------*/

#pragma once

// Стандартные заголовочные файлы
#include <afxcview.h> 
#include <comdef.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <functional>
#include <set>
#include <vector>
#include <map>

// Добавим хедеры 1С классов
#define IMPORT_1C __declspec(dllimport)
#include "type32.h"
#include "types.h"
#include "br32.h"
#include "bkend.h"
#include "frame.h"
#include "blang.h"
#include "userdef.h"
#include "seven.h"
#include "editr.h"
#include "basic.h"
#include "dbeng32.h"
#include "moxel.h"
#include "rgproc.h"
#include "txtedt.h"

#include "ext_header.h"

using namespace std;

// Объявим несколько полезных глобальных объектов
__declspec(selectany) CApp7*			pMainApp		= NULL;
__declspec(selectany) CBkEndUI*			pBkEndUI		= NULL;
__declspec(selectany) CMetaDataCont*	pMetaDataCont	= NULL;
//__declspec(selectany) SECMDIFrameWnd*	pMainFrame		= NULL;
__declspec(selectany) CMainFrame*		pMainFrame		= NULL;
__declspec(selectany) CDataBase7*		pDataBase7		= NULL;
__declspec(selectany) HINSTANCE			h1CResource		= NULL;
__declspec(selectany) HINSTANCE			hMyInst			= NULL;

void Init1CGlobal(HINSTANCE hInst);

// И несколько полезных функций

// Вывод текста в строку состояния (Состояние)
void DoStsLine(LPCSTR format, ...);

// Получить описание ошибки Windows по ее коду. При err==0, вернет описание для GetLastError
CString GetErrorDescription(DWORD err = 0);

//Формироване рантайм-исключения
void RuntimeError(LPCSTR format, ...);
void RuntimeErrorRes(UINT nID);

// КаталогПрограммы()
__forceinline const CString& BinDir()		{return pMainApp->GetProps()->GetStringProp(0);}
// КаталогИБ()
__forceinline const CString& IBDir()		{return pMainApp->GetProps()->GetStringProp(1);}
// КаталогПользователя()
__forceinline const CString& UserDir()		{return pMainApp->GetProps()->GetStringProp(2);}
// КаталогВременныхФайлов()
__forceinline const CString& TempDir()		{return pMainApp->GetProps()->GetStringProp(3);}
// Имя базы
__forceinline const CString& IBName()		{return pMainApp->GetProps()->GetStringProp(0x0C);}
// ИмяПользователя()
__forceinline const CString& UserName()		{return pMainApp->GetProps()->GetStringProp(0x0D);}
// ПолноеИмяПользователя()
__forceinline const CString& FullUserName()	{return pMainApp->GetProps()->GetStringProp(0x0E);}
// НазваниеНабораПрав()
__forceinline const CString& RightsName()	{return pMainApp->GetProps()->GetStringProp(0x10);}
// НазваниеИнтерфейса()
__forceinline const CString& GUIName()		{return pMainApp->GetProps()->GetStringProp(0x11);}

