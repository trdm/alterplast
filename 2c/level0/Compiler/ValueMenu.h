// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
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
	CValue  vObject;//MENU_OBJECT � MENU_POPUP
	CString sFunction;//MENU_OBJECT � MENU_FUNCTION
	CProcUnit	*pRun;//MENU_FUNCTION
	CValue vData;//(1-� �������� ��� ������ ������ ��� ������) MENU_OBJECT � MENU_FUNCTION
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

	//������ ��� ����������� �������
	CString csObjectName;
	static CMethods Methods;
	//��� ������ ����� �������������� � ����� ���������� ��������:
	CMethods* GetPMethods(void){return &Methods;};//�������� ������ �� ����� �������� ������� ���� ��������� � �������
	void PrepareNames(void);//���� ����� ������������� ���������� ��� ������������� ���� ��������� � �������
	CValue Method(int iName,CValue **aParams);//����� ������

	CString GetString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};

	//void SetAttribute(int iName,CValue &Val); //��������� ��������
	//CValue GetAttribute(int iName); //��������� �������� ��������

};
extern TMenuItemArray aAllMenuItems;

#endif // !defined(AFX_ValueMenu_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
