//Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// Value.h: interface for the CValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_)
#define AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
DECLARE_DYNCREATE(CValue);

public:
	//АТРИБУТЫ:
	int nType;
	union
	{
		NUMBER fData;	//TYPE_NUMBER
		int dData;		//TYPE_DATE
		CValue *pRef;	//TYPE_REFFER
	};
	union
	{
		int nRef;
		int bReadOnly;
	};
	CString sData;		//TYPE_STRING

public:
	//МЕТОДЫ:
	//конструкторы:
	CValue();
	CValue (const CValue& cParam);//конструктор копирования:
	CValue (NUMBER cParam);

	CValue (CString sParam);
	CValue (LPCTSTR sParam);

	CValue (int);
	CValue (DWORD);
	CValue (__int64);

	//деструктор:
	virtual ~CValue();

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

		if(nType==TYPE_REFFER)
			pRef->DecrRef();
		nType=0;
	}

	//операторы:
	operator = (CValue &cParam);
	operator = (__int64 cParam);
	BOOL operator > (CValue &cParam);
	BOOL operator >= (CValue &cParam);
	BOOL operator < (CValue &cParam);
	BOOL operator <= (CValue &cParam);
	BOOL operator == (CValue &cParam);
	operator NUMBER ();
	operator CString ();

private:
	static CMapStringToPtr ObjectsTree;
	friend CValue CreateObject(CString Str,CValue Val);
	friend void RegisterObject(CRuntimeClass* pRuntimeClass,CString Str);
	friend void UnRegisterObject(CString Str);
	friend void AttachRegisterObject(CString StrParent,CString StrChild);

public:
	void CreateObject(CString Str,CValue Val=CValue());

	void Copy(const CValue& cOld);
	void static SetError(CString Str);


	int FromDate(int &nYear,int &nMonth,int &nDay)const;
	int FromDate(int &nYear,int &nMonth,int &nDay,int &DayOfWeek,int &DayOfYear, int &WeekOfYear)const;
	

	virtual void Attach(void *pObj);
	virtual void Detach();
	virtual void *GetAttach();



	//Виртуальные методы:
	virtual int  GetType(void)const;
	virtual void  IncrRef(void);
	virtual void  DecrRef(void);
	virtual BOOL IsEmpty(void);

	virtual CString GetTypeString(void)const;
	virtual void Init(CString StrVid,CValue Param);//строка как была передана в функцию СоздатьОбъект, например "Документ.РасходнаяНакладная"


	virtual void SetValue(CValue &Val);
	virtual void SetString(CString);
	virtual void SetNumber(CString s);
	virtual void SetDate(CString s);
	void SetData(CValue &Val);//установка значения без изменения типа

	virtual CValue GetValue(int nThis=0);
	virtual NUMBER GetNumber(void) const;
	virtual CString GetString(void) const;
	virtual int GetDate(void)const;


	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА


	//эти методы нужно переопределить в ваших агрегатных объектах:
	virtual CMethods* GetPMethods(void);//получить ссылку на класс помощник разбора имен атрибутов и методов
	virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	virtual CValue Method(int iName,CValue **aParams);
	virtual void SetAttribute(int iName,CValue &Val);//установка атрибута
	virtual CValue GetAttribute(int iName);//значение атрибута


	//а эти методы необязательны для переопределения (т.к. они автоматически поддерживаются данным базовым классом):
	CValue Method(CString csName,CValue **aParams);
	void SetAttribute(CString csName,CValue &Val);//установка атрибута
	CValue GetAttribute(CString csName);//значение атрибута

	virtual int  FindMethod(CString csName);
	virtual int  FindAttribute(CString csName);
	virtual CString GetMethodName(int nNumber,int nAlias=0);
	virtual CString GetAttributeName(int nNumber,int nAlias=0);
	virtual int  GetNMethods(void);
	virtual int  GetNAttributes(void);

	CValue CallFunctionV(CString csName,CValue **p);


	virtual void SaveToString(CString &Str);
	virtual void LoadFromString(CString &Str);


	CValue& GetAt(int nKey);
	void SetAt(int nKey,CValue Val);
	CValue& GetAt(CString Key);
	void SetAt(CString Key,CValue Val);


	int  GetSizeArray(void);
	void SetSizeArray(int nSize);
	int  GetSizeStrArray(void);
	void SetSizeStrArray(int nSize);


	CValue CallFunction(CString csName,...);
	/*CValue CallFunction(CString csName,CValue *vParam1);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6,CValue *vParam7);
	CValue CallFunction(CString csName,CValue *vParam1,CValue *vParam2,CValue *vParam3,CValue *vParam4,CValue *vParam5,CValue *vParam6,CValue *vParam7,CValue *vParam8);
	*/

	void UnitTest(void);
};
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


unsigned hash( const char* v, int M );
class CString2 : public CString
{
public:
	CString2():CString()
	{
	};
	CString2(CString Str):CString(Str)
	{
	};
	operator DWORD ()
	{
		CString Str=mUpper(*this);
		return hash(Str.GetBuffer(0),0xFFFFF);
	};
};

BOOL CompareElements(CString2 *pElement1,CString2 *pElement2);



#define ReplaceBySave(Str)\
Str.Replace("&","&0");\
Str.Replace("/","&1");\
Str.Replace("{","&2");\
Str.Replace("}","&3");


#define ReplaceByLoad(Str)\
Str.Replace("&1","/");\
Str.Replace("&2","{");\
Str.Replace("&3","}");\
Str.Replace("&0","&");



//Системные функции
CValue ValueByRef (CValue *pRef);

CValue Number(NUMBER cParam);
CValue Number(CValue cParam);
CValue String(CString cParam);
CValue String(LPCTSTR cParam);
CValue String(CValue cParam);
CValue Date(CString Str);
CValue Date(CString Str,int &nYear,int &nMonth,int &nDay);
CValue Date(CValue cParam);
CValue Date(CValue cParam,int &nYear,int &nMonth,int &nDay,BOOL bReturn=FALSE);
CValue Date(int nYear,int nMonth,int nDay);
void DateFromIntToYMD(int nCurDate,int &nYear,int &nMonth,int &nDay);
//CValue ReturnType(int nType);

CValue CreateObject(CString Str,CValue Val=CValue());
void RegisterObject(CRuntimeClass* pRuntimeClass,CString Str);
void UnRegisterObject(CString Str);



#endif // !defined(AFX_VALUE_H__271B302B_525E_42FB_9D84_7AF50C42E0E4__INCLUDED_)
//Авторские права - www.vtools.ru (info@vtools.ru)
