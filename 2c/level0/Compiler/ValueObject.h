// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueObject.h: interface for the CValueObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEOBJECT_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
#define AFX_VALUEOBJECT_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "../ModuleManager.h"

//Менеджер макрообъектов - объектов написанных на макроязыке 2С
class CValueObject : public CValueControl  
{
DECLARE_DYNCREATE(CValueObject);
public:
	CValueObject();
	virtual ~CValueObject();

	CString csObjectName;
	CString csObjectType;
	CString csObjectKind;

	CProcUnit *pRun;
	int nMethodGetAttr;
	int nMethodSetAttr;
	int bHasParent;//поддержка наследования
	CValue vParent;
	//CValue CurrentVid;

	void SetObjectType(CString StrTypeVid);
	void InitObject(CProcUnit *pSetRun,CValue Param);
	void SetParentContext();
	void Init(CString StrTypeVid,CValue Param);
	virtual void SetValue(CValue &Val);
	virtual CMethods* GetPMethods(void){return 0;};//отключаем помощника разбора имен родительского класса

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	CValue Method(int iName,CValue **aParams);
	int  FindMethod(CString csName);
	void SetAttribute(int iName,CValue &Val);//установка атрибута
	CValue GetAttribute(int iName);//получение значения атрибута
	int  FindAttribute(CString csName);

	CString GetAttributeName(int nNumber,int nAlias=0);
	int  GetNAttributes(void);

	virtual BOOL IsEmpty(void);

	CString GetTypeString()const;
	void SaveToString(CString &Str);
	void LoadFromString(CString &Str);
	CString GetString(void)const;

	CValue CallFunction(CString csName);
	CValue CallFunction(CString csName,CValue &vParam1);
	CValue CallFunction(CString csName,CValue &vParam1,CValue &vParam2);

#ifdef _DEBUG
	CString csName;
#endif
};

#endif // !defined(AFX_VALUEOBJECT_H__4573BCDF_4E79_488E_B07C_8844EFE66326__INCLUDED_)
