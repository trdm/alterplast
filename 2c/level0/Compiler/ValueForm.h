// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueForm.h: interface for the CValueForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_)
#define AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueControl.h"

//ПОДДЕРЖКА ФОРМЫ
#define PWINDOW ((CWnd *)pWnd)
class CValueForm:public CValueControl
{

	DECLARE_DYNCREATE(CValueForm);

	CValueForm();
	~CValueForm();

	BOOL bIsView;
	BOOL bMicroForm;
public:
	CValue vParam;//параметр для формы
	CValue vLayers;//закладки для формы (аля 7.7)


	virtual int Modify(int);

	void Attach(void *pObj);
	CValue GetValue(int);
	CString GetTypeString(void)const;



	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	virtual CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	virtual CValue Method(int iName,CValue **aParams);//вызов метода
	virtual void SetAttribute(int iName,CValue &Val);//установка атрибута
	virtual CValue GetAttribute(int iName);//значение атрибута


	virtual int  FindAttribute(CString csName);
	virtual CString GetAttributeName(int nNumber,int nAlias=0);
	virtual int  GetNAttributes(void);

	void SetString(CString Str)
	{
		Caption(Str);
	};
	CString GetString()
	{
		return Caption();
	};

	virtual int Visible()
	{
		ASSERT(pWnd);
		return PWINDOW->IsWindowVisible();
	};
	virtual int Visible(BOOL bStat)
	{
		ASSERT(pWnd);
		int nRes=Visible();
		PWINDOW->ShowWindow(bStat);
		return nRes;
	};

	//Доступность
	virtual int Enable()
	{
		ASSERT(pWnd);
		return PWINDOW->IsWindowEnabled();
	};
	virtual int Enable(BOOL bStat)
	{
		ASSERT(pWnd);
		int nRes=Enable();
		PWINDOW->EnableWindow(bStat);
		return nRes;
	};

	//Заголовок
	virtual CString Caption();
	void Caption(CString Str);
	virtual void Close();

};
#undef PWINDOW

#endif // !defined(AFX_VALUEFORM_H__45D6EE67_E73A_4075_A6B3_F0D5961E1FDA__INCLUDED_)
