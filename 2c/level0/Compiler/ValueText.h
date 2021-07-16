// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueText.h: interface for the CValueText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueText_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
#define AFX_ValueText_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../ModuleDoc.h"

class CValueText : public CValue  
{
DECLARE_DYNCREATE(CValueText);
public:
	CValueText();
	virtual ~CValueText();

enum
{
	enLinesCnt=0,
	enGetLine,
	enOpen,
	enTemplate,
	enFixTemplate,
	enInsertLine,
	enAddLine,
	enReplaceLine,
	enDeleteLine,
	enReadOnly,
	enShow,
	enClear,
	enCodePage,
	enWrite,
	enGetText,
};


	CString csTitle;
	CArray<CString,CString> aText;
	CModuleDoc* pDoc;
	BOOL bReadOnly;
	int nCodePage;
	int nTemplate;
	int nFixTemplate;
	CString GetStr();


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

};

#endif // !defined(AFX_ValueText_H__FE97FE25_72E5_4E2A_AE1E_1959128DDF57__INCLUDED_)
