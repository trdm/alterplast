//Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// Value.h: interface for the CValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_)
#define AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"plugdetect.h"


#define DATE_DELTA 2440589 //совмещение числового представления даты с 1С
#define SEC1_DELTA 75600
#define SEC2_DELTA 86400//число секунд в одном дне
#include "Methods.h"

enum
{
	TYPE_EMPTY=0,
	TYPE_NUMBER=1,
	TYPE_STRING=2,
	TYPE_DATE=3,
	TYPE_TIME=4,		//дата+время
	TYPE_ARRAY=5,		//массив
	TYPE_DEFAULT,
	TYPE_REFFER,
	TYPE_LAST,
};

#define NOT_DEFINED CString("Неопределенный")

typedef double NUMBER;



//Элементарный тип данных
class CValue : public CObject  
{


#define static FORPLUGIN static
#define virtual FORPLUGIN virtual
	DECLARE_DYNCREATE(CValue);
#undef static 
#undef virtual 
	

		//FORPLUGIN static AFX_DATA CRuntimeClass classCValue; 
		//FORPLUGIN virtual CRuntimeClass* GetRuntimeClass() const;
		//FORPLUGIN static CObject* PASCAL CreateObject();


public:
	//АТРИБУТЫ:
	int nType;
	union
	{
		NUMBER fData;	//TYPE_NUMBER
		int dData;		//TYPE_DATE
		CValue *pRef;	//TYPE_REFFER
	};
	int nRef;
	int bReadOnly;
	CString sData;		//TYPE_STRING

public:
	//МЕТОДЫ:
	//конструкторы:
	FORPLUGIN CValue();
	FORPLUGIN CValue (const CValue& cParam);//конструктор копирования:
	FORPLUGIN CValue (NUMBER cParam);

	FORPLUGIN CValue (CString sParam);
	FORPLUGIN CValue (LPCTSTR sParam);

	FORPLUGIN CValue (int);
	FORPLUGIN CValue (DWORD);
	FORPLUGIN CValue (__int64);

	//деструктор:
	FORPLUGIN virtual ~CValue();

#ifdef _DEBUG
#define MYNEW
#elif MYDEBUG
#define MYNEW
#endif

#ifdef MYNEW2
	void* operator new(size_t nSize)
	{
	   return malloc(nSize);
	}
	void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
	   return malloc(nSize);
	}
	void operator delete (void *p)
	{
	   free(p);
	}
	void operator delete(void *p, LPCSTR lpszFileName, int nLine)
	{
	   free(p);
	}
#endif

	//Очистка значений
	void Reset()
	{
		if(nType&&bReadOnly)
			SetError("Попытка присвоить значение запрещенной для записи переменной");

		if(pRef&&nType==TYPE_REFFER)
			pRef->DecrRef();
		nType=0;
		fData=0;
		sData="";
	}

	//операторы:
	FORPLUGIN operator = (CValue &cParam);
	FORPLUGIN operator = (__int64 cParam);
	FORPLUGIN BOOL operator > (CValue &cParam);
	FORPLUGIN BOOL operator >= (CValue &cParam);
	FORPLUGIN BOOL operator < (CValue &cParam);
	FORPLUGIN BOOL operator <= (CValue &cParam);
	FORPLUGIN BOOL operator == (CValue &cParam);
	FORPLUGIN operator NUMBER ();
	FORPLUGIN operator CString ();

private:
	FORPLUGIN static CMapStringToPtr ObjectsTree;
	FORPLUGIN friend CValue CreateObject(CString Str,CValue Val);
	FORPLUGIN friend void RegisterObject(CRuntimeClass* pRuntimeClass,CString Str);
	FORPLUGIN friend void UnRegisterObject(CString Str);
	FORPLUGIN friend void AttachRegisterObject(CString StrParent,CString StrChild);
	FORPLUGIN friend BOOL IsRegisterObject(CString Str);


public:
	FORPLUGIN void CreateObject(CString Str,CValue Val=CValue());

	FORPLUGIN void Copy(const CValue& cOld);
	FORPLUGIN void static SetError(CString Str);


	FORPLUGIN int FromDate(int &nYear,int &nMonth,int &nDay)const;
	FORPLUGIN int FromDate(int &nYear,int &nMonth,int &nDay,int &DayOfWeek,int &DayOfYear, int &WeekOfYear)const;
	

	FORPLUGIN virtual void Attach(void *pObj);
	FORPLUGIN virtual void Detach();
	FORPLUGIN virtual void *GetAttach();



	//Виртуальные методы:
	FORPLUGIN virtual int  GetType(void)const;
	FORPLUGIN virtual void  IncrRef(void);
	FORPLUGIN virtual void  DecrRef(void);
	FORPLUGIN virtual BOOL IsEmpty(void);

	FORPLUGIN virtual CString GetTypeString(void)const;
	FORPLUGIN virtual void Init(CString StrVid,CValue Param);//строка как была передана в функцию СоздатьОбъект, например "Документ.РасходнаяНакладная"


	FORPLUGIN virtual void SetValue(CValue &Val);
	FORPLUGIN virtual void SetString(CString);
	FORPLUGIN virtual void SetNumber(CString s);
	FORPLUGIN virtual void SetDate(CString s);
	FORPLUGIN void SetData(CValue &Val);//установка значения без изменения типа

	FORPLUGIN virtual CValue GetValue(int nThis=0);
	FORPLUGIN virtual NUMBER GetNumber(void) const;
	FORPLUGIN virtual CString GetString(void) const;
	FORPLUGIN virtual int GetDate(void)const;


	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА


	//эти методы нужно переопределить в ваших агрегатных объектах:
	FORPLUGIN virtual CMethods* GetPMethods(void);//получить ссылку на класс помощник разбора имен атрибутов и методов
	FORPLUGIN virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	FORPLUGIN virtual CValue Method(int iName,CValue **aParams);
	FORPLUGIN virtual void SetAttribute(int iName,CValue &Val);//установка атрибута
	FORPLUGIN virtual CValue GetAttribute(int iName);//значение атрибута


	//а эти методы необязательны для переопределения (т.к. они автоматически поддерживаются данным базовым классом):
	FORPLUGIN CValue Method(CString csName,CValue **aParams);
	FORPLUGIN void SetAttribute(CString csName,CValue &Val);//установка атрибута
	FORPLUGIN CValue GetAttribute(CString csName);//значение атрибута

	FORPLUGIN virtual int  FindMethod(CString csName);
	FORPLUGIN virtual int  FindAttribute(CString csName);
	FORPLUGIN virtual CString GetMethodName(int nNumber,int nAlias=0);
	FORPLUGIN virtual char* GetMethodDescription(int nNumber,int nAlias=0);
	FORPLUGIN virtual CString GetAttributeName(int nNumber,int nAlias=0);
	FORPLUGIN virtual int  GetNMethods(void);
	FORPLUGIN virtual int  GetNAttributes(void);

	FORPLUGIN CValue CallFunctionV(CString csName,CValue **p);


	FORPLUGIN virtual void SaveToString(CString &Str);
	FORPLUGIN virtual void LoadFromString(CString &Str);


	FORPLUGIN CValue& GetAt(int nKey);
	FORPLUGIN void SetAt(int nKey,CValue Val);
	FORPLUGIN CValue& GetAt(CString Key);
	FORPLUGIN void SetAt(CString Key,CValue Val);


	FORPLUGIN int  GetSizeArray(void);
	FORPLUGIN void SetSizeArray(int nSize);
	FORPLUGIN int  GetSizeStrArray(void);
	FORPLUGIN void SetSizeStrArray(int nSize);


	FORPLUGIN CValue CallFunction(CString csName,...);
	/*CValue CallFunction(CString csName,CValue *vParam1);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6,CValue *vParam7);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6,CValue *vParam7,CValue *vParam8);
	*/

	FORPLUGIN void UnitTest(void);
};

#ifndef IS_PLUGIN

class CValueNumber0 : public CValue
{
	CValueNumber0()
	{
		nType=TYPE_NUMBER;
	};
	DECLARE_DYNCREATE(CValueNumber0);
};
class CValueDate0 : public CValue
{
	CValueDate0()
	{
		nType=TYPE_DATE;
	};
	DECLARE_DYNCREATE(CValueDate0);
};
class CValueString0 : public CValue
{
	CValueString0()
	{
		nType=TYPE_STRING;
	};
	DECLARE_DYNCREATE(CValueString0);
};
#endif //IS_PLUGIN

#define ReplaceBySave(Str)\
Str.Replace("&","&0");\
Str.Replace("/","&1");\
Str.Replace("{","&2");\
Str.Replace("}","&3");\
Str.Replace("\0","&4");


#define ReplaceByLoad(Str)\
Str.Replace("&0","&");\
Str.Replace("&1","/");\
Str.Replace("&2","{");\
Str.Replace("&3","}");\
Str.Replace("&4","\0");



//Системные функции
FORPLUGIN CValue ValueByRef (CValue *pRef);

FORPLUGIN CValue Number(NUMBER cParam);
FORPLUGIN CValue Number(CValue cParam);
FORPLUGIN CValue String(CString cParam);
FORPLUGIN CValue String(LPCTSTR cParam);
FORPLUGIN CValue String(CValue cParam);
FORPLUGIN CValue Date(CString Str);
FORPLUGIN CValue Date(CString Str,int &nYear,int &nMonth,int &nDay);
FORPLUGIN CValue Date(CValue cParam);
FORPLUGIN CValue Date(CValue cParam,int &nYear,int &nMonth,int &nDay,BOOL bReturn=FALSE);
FORPLUGIN CValue Date(int nYear,int nMonth,int nDay);
FORPLUGIN void DateFromIntToYMD(int nCurDate,int &nYear,int &nMonth,int &nDay);
//CValue ReturnType(int nType);

FORPLUGIN CValue CreateObject(CString Str,CValue Val=CValue());
FORPLUGIN void RegisterObject(CRuntimeClass* pRuntimeClass,CString Str);
FORPLUGIN void UnRegisterObject(CString Str);
FORPLUGIN BOOL IsRegisterObject(CString Str);
FORPLUGIN void Error(LPCTSTR s,...);


FORPLUGIN CValue Eval(CString StrRun);
FORPLUGIN void RunModule(CString Str);

FORPLUGIN void SetStatus(CString s);
FORPLUGIN CValue GetRunContext();
FORPLUGIN void DoMessage2C();




typedef CValue (*DEFFUNCTION)(CValue **);

//Структура элемента заголовков системных функций
extern struct SCallFunction
{
public:
	char *sName;
	int nCountParams;
	DEFFUNCTION Func;
	char *sShortDescription;
	char *sLongDescription;
}DefSystemFunctionsArray[];

FORPLUGIN void AddSystemFunctions(SCallFunction *pSysArray);



#endif // !defined(AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_)
//Авторские права - www.vtools.ru (info@vtools.ru)
