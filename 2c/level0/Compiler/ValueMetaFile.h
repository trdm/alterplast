// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueMetadata.h: interface for the CValueMetadata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEMETADATA_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_)
#define AFX_VALUEMETADATA_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "MetaObject.h"
#include "Metadata.h"

class CValueMetadata : public CValue  
{

DECLARE_DYNCREATE(CValueMetadata);
public:
	CValueMetadata();
	virtual ~CValueMetadata();


	BOOL bWasOpenZip;
	CZipArchive m_zip;




	CString csPathObject;
	void SetObjectName(CString csSetPathObject){csPathObject=csSetPathObject+"\\";};


	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода


	CString csObjectName;
	CString GetString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};
	CString GetTypeString(void)const{return csObjectName;};


	void Close();

};

#endif // !defined(AFX_VALUEMETADATA_H__8EABDAB2_0DCD_478F_8492_73FDD4CF51E2__INCLUDED_)
