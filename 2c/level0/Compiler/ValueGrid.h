// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueGrid.h: interface for the CValueGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
#define AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "../GridCtrl_src/GridCtrl.h"

class CValueGrid : public CValueControl  
{
DECLARE_DYNCREATE(CValueGrid);

public:
	CValueGrid();
	virtual ~CValueGrid();

	//CGridCtrl *pGrid;
	

	void Attach(CGridCtrl *pSetGrid){pWnd=pSetGrid;};
	int nCurRow;


	
	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	CString csObjectName;
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода


	int  FindColumn(CString csName);
	void SetAttribute(int iName,CValue &Val);//установка атрибута
	CValue GetAttribute(int iName);//получение значения атрибута
	int  FindAttribute(CString csName);
	CString GetAttributeName(int nNumber,int nAlias=0);
	int  GetNAttributes(void);

	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetString(void)const;
	CString GetTypeString(void)const{return GetString();};
	int GetColumnNumber(CValue *pVal);

};

#endif // !defined(AFX_ValueGrid_H__C3DF9D06_DF27_4E39_B1DF_22D163612530__INCLUDED_)
