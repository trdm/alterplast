// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueMenu.h: interface for the CValueMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueMenu_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
#define AFX_ValueMenu_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"

typedef CArray<class CMyMenuItem,class CMyMenuItem> TMenuItemArray;
enum
{
	MENU_SEPARATOR=0,
	MENU_OBJECT,
	MENU_FUNCTION,
	MENU_POPUP,
	MENU_BAR,
};
class CMyMenuItem
{
public:
	CMyMenuItem()
	{
//		nId=0;
		nType=MENU_SEPARATOR;
	};
	//int nId;
	CString csName;
	int		nType;
	CValue  vObject;//MENU_OBJECT и MENU_POPUP
	CString sFunction;//MENU_OBJECT и MENU_FUNCTION
	CProcUnit	*pRun;//MENU_FUNCTION
	CValue vData;//(1-й параметр при вызове фукции или метода) MENU_OBJECT и MENU_FUNCTION
	//CMenuItem vParent;
	//int     nPosition;
	//int     nEnabled;
	//int     nVisible;
	//int     nRadioItem;
	//int     nChecked;
	//int     nPicture;
	//int     nSystem;
	//int     nCountItems;
};
class CValueMenu : public CValue  
{
DECLARE_DYNCREATE(CValueMenu);
public:
	CValueMenu();
	virtual ~CValueMenu();

enum
{
	enShowMenu=0,
    enAddMenu,
	enDeleteMenu,
	enLoadMenu,

};

	TMenuItemArray aMenu;
    CMyMenuItem CurrentItem;

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	CString csObjectName;
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода

	CString GetString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};

	//void SetAttribute(int iName,CValue &Val); //установка атрибута
	//CValue GetAttribute(int iName); //получение значения атрибута

};
extern TMenuItemArray aAllMenuItems;

#endif // !defined(AFX_ValueMenu_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
