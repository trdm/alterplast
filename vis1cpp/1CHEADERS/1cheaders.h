// 1cheaders.h
/*
Данный файл предназначен для его включения в stdafx.h разнообразных проектов,
нуждающихся в доступе к 1С dll'кам.
*/
#ifndef _1C_HEADERS_H
#define _1C_HEADERS_H

#define IMPORT_1C __declspec(dllimport)

#include <afxcview.h> 
#include <comdef.h>
#include <afxctl.h>
#include <afxtempl.h>
#include <functional>
#include <set>
#include <vector>
#include <map>

struct CHintStruct77
{
	int m_null;
	int m_CtrlNumber;
	char* m_HintText;
};
// Состояние атрибутов формы
#define CS_EX_DONOTTAB         0x00000004L //Пропускать при вводе
#define CS_EX_QUICKCHOICE      0x00000008L //Быстрый выбор
#define CS_EX_USEDESCRIPTION   0x00000010L //Использовать описание, как подсказку
#define CS_EX_HAVEBUTTON       0x00000020L //Имеет кнопку выбора
#define CS_EX_FORMFONT         0x00000040L //Шрифт по умолчанию
#define CS_EX_ACCEL            0x00000100L //Выбран акселератор
#define CS_EX_VISIBLE          0x00002000L //Видимость
#define CS_EX_READONLY         0x00004000L //Доступность
#define CS_EX_TRANSPARENT      0x00010000L //Прозрачный фон
#define CS_EX_NOTSTORE         0x01000000L //Не сохранять при сохранении настройки
#define CS_EX_REDNEGATIVE      0x04000000L //Отрицательное красным
#define CS_EX_DONOTEDIT        0x10000000L //Запретить редактирование

struct  CFormCtrlSet // это CControlID
{
public:
	int m_ID;
	int m_TypeCtrl;
	int buf1[2];
	int m_Kind; // Вид объекта
	int m_Type; // Тип объекта
	int m_Left;
	int m_Top;
	int m_Right;
	int m_Bottom;
	char* m_Caption;
	char* m_Label;
	char* m_Formula;
	DWORD m_Style;
	int buf2[2];
	DWORD m_StyleEx; // Битовая маска (видимость, доступность и т.д.)
	int m_ListFormID; // Внутренний идентификатор формы для выбора реквизита
	char* m_Mask;
	char* m_Hint;
	char* m_Description;
	int buf3[21];
	char* m_LayerName;
	int buf4[5];
};

class CGetDoc77:public CDocument
{
public:
	int m_Flag;
	CObArray m_PropArray;
	int buf1[67];
	CHintStruct77** m_Hints;
	int m_AttrNum;
	int m_HintNum;
	int buf2[81];
	int m_Width;
	int m_Height;
	int buf4[8];
	char* m_Caption;
	int buf5[78];
	CObArray m_Array; // массив указателей атрибуты формы
};

class IMPORT_1C CFormCtrl:public CObject // это CGetField
{
public:
	int m_Flag1;
	int m_Flag2;
	int m_Flag3;
	CGetDoc77* m_GetDoc;
	int m_ID;
	CFormCtrlSet* m_Set;
	int buf[26];
	int m_RefFlag;
	int m_Flag4;
	int m_ReadOnly;
	int m_Visible;
};

// Добавим хедеры 1С классов
#include "type32.h"
#include "types.h"
#include "br32.h"
#include "bkend.h"
#include "frame.h"
#include "blang.h"
#include "seven.h"
#include "editr.h"
#include "basic.h"
#include "dbeng32.h"
#include "moxel.h"
#include "rgproc.h"
#include "txtedt.h"
#include "userdef.h"

//using namespace std;

// Объявим несколько полезных глобальных объектов
extern CApp7*			pMainApp;
extern CBkEndUI*		pBkEndUI;
extern CMetaDataCont*	pMetaDataCont;
extern HINSTANCE		h1CResource;
extern SECMDIFrameWnd*	pMainFrame;

#endif