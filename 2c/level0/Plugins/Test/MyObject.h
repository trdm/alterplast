// Авторские права: VTOOLS.RU (info@vtools.ru) 2002-2004г.
// SQLLite.h: interface for the CSQLLite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"

class CSQLLite : public CValue  
{
DECLARE_DYNCREATE(CSQLLite );
public:
	CSQLLite();
	virtual ~CSQLLite();
	

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//Эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода
	virtual CString GetString(void);
	virtual CString GetTypeString(void)const;
	//virtual CValue GetAttribute(int iName);//значение атрибута

	//////////////////////////////////////////////////////////////////////
	//Пользовательские методы
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CSQLLite ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Connect(CValue &rez,CValue**ppV);

	//хранение выборки запроса
	char **pResultSQL;
	int nRowCount;
	int nColCount;
	int nCurLine;

	//вспомогательные методы
	void Clear();
	int Query(CString Str);
	CString GetField(int n);
};

#endif // !defined(AFX_SQLLite_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
