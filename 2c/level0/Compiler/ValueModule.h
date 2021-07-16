// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueModule.h: interface for the CValueModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
#define AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"

class CValueModule : public CValue  
{
DECLARE_DYNCREATE(CValueModule);
public:

	CValueModule();
	virtual ~CValueModule();

	CProcUnit *pRun;
	CValue vContext;
	void Clear();



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

#endif // !defined(AFX_ValueModule_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
