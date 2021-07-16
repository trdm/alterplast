// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// 1CCtrl.h: interface for the C1CCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_1CCTRL_H__B3FD0ADB_0053_4EF6_BFE0_08D3022515A9__INCLUDED_)
#define AFX_1CCTRL_H__B3FD0ADB_0053_4EF6_BFE0_08D3022515A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "Edit/DropEdit.h"
#include "Edit/SecCalc.h"
#include "Edit/amsEdit.h"
#include "GUI/XColorStatic.h"

//ПОДДЕРЖКА ТЕКСТА
void SetStaticParams(CXColorStatic &m_StaticText,int hAlign,int vAlign,CString csFont,bool bBold,bool bItalic,bool bUnderL,int nHeight,int nColor);

//ПОДДЕРЖКА ЗАКЛАДОК
class CLayerTabCtrl
{
public:
	CString Name;	//имя слоя
	CString Present;//представление
};
class CElementTabCtrl
{
public:
	CString Name;	//имя слоя
	UINT	ID;		//диалоговый элемент
};

class CMyTabCtrl : public CTabCtrl
{
public:
	CArray<CLayerTabCtrl,CLayerTabCtrl>	aLayer;//список слоев
	CArray<CElementTabCtrl,CElementTabCtrl>	aList;//список идентификаторов
	//заполнение слоев
	void Add(CString csLayer,UINT nID);

	//управление закладками
	void RemoveAll();
	void AddValue(CString csName,CString csPresent);
	void UseLayer(CString csName,int nMode=2);
	void UseLayer(int nIndex);
};

//Диалог редактирования с кнопкой
class CDropEdit : public CAMSMultiMaskedEdit//CAMSMaskedEdit//SECDropEdit//CAMSEdit
{
DECLARE_DYNCREATE(CDropEdit );
public:
	CDropEdit(BOOL nBitmap=0):
	Behavior(this)
	{
		if(nBitmap)
			SetBitmap(IDB_CHOSECBTN);
	};
	virtual void OnClicked();
	DECLARE_MESSAGE_MAP()

protected:
};

//Диалог редактирования в виде калькулятора
class CCalcEdit : public CDropEdit
{
DECLARE_DYNCREATE(CCalcEdit);
public:
	CCalcEdit(BOOL nBitmap=0):
	Behavior(this)
	{
		if(nBitmap)
			SetBitmap(IDB_CALCBTN);
		nUseFilter=1;
	};

	int nUseFilter;
protected:
	virtual void OnClicked();
	afx_msg void OnChar( UINT, UINT, UINT );
	DECLARE_MESSAGE_MAP()
};

//ПЕРЕКЛЮЧАТЕЛИ
class CRadio : public CButton
{
DECLARE_DYNCREATE(CRadio);
};
//РАМКА
class CGroup : public CButton
{
DECLARE_DYNCREATE(CGroup);
};

#endif // !defined(AFX_1CCTRL_H__B3FD0ADB_0053_4EF6_BFE0_08D3022515A9__INCLUDED_)
