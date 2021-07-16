// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueList.h: interface for the CValueList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUELIST_H__C187CB48_A2FF_4265_B27D_36223957B489__INCLUDED_)
#define AFX_VALUELIST_H__C187CB48_A2FF_4265_B27D_36223957B489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ValueControl.h"

class CElementList
{
public:
	CElementList(){bCheck=FALSE;};
	CValue Value;	//значение
	CString Present;//представление
	BOOL bCheck;	//пометка
};

//класс реализации объекта СписокЗначений 
//(его "видимые" потомки: CValueListBox и CValueComboBox)
class CValueList : public CValueControl  
{
DECLARE_DYNCREATE(CValueList);
enum
{
	enAddValue=0,
	enInsertValue,
	enGetListSize,
	enSetValue,
	enSet,
	enSort,
	enSortByPresent,
	enBelong,
	enFindValue,
	enGetValue,
	enGet,
	enFromSeparatedString,
	enToSeparatedString,
	enChooseValue,
	enCheckValue,
	enCheck,
	enCurSel,
	enRemoveValue,
	enRemoveAll,
	enMoveValue,
	enUnload,
	enSetSize,
	enSetRedraw,

	enVisible,
	enEnable,
	enFocus,

	enLastMethod
};

	//Методы:
public:
	virtual int UseLayer(CString csName,int nMode);
	virtual int CurSel(int nCur=-1);
	virtual void SetSize(int nSize);
	virtual void MoveValue(int nCount,int nIndex);
	virtual void RemoveValue(int nIndex, int nCount=-1);
	virtual int Check(int nIndex,int bValue=-1);
	virtual void FromSeparatedString(CString Str);
	virtual void SortByPresent(int nDirect=-1);
	virtual void Sort(int nDirect=-1);
	virtual void RemoveAll(void);
	virtual int Set(CString Str,CValue Val);
	virtual void SetValue(int nIndex, CValue Val, CString Str="", int nCount=0);
	virtual void InsertValue(int nIndex,CValue Val,CString Str="",int nCount=0);
	virtual void AddValue(CValue Val,CString Str="");
	
	
	int GetListSize(void) const;
	void Unload(CValue &List2,int nStart=-1,int nEnd=-1);
	int CheckValue(CValue &Val,CString Title,int &nIndex);
	int ChooseValue(CValue &Val,CString Title,int &nIndex,int nMode=0);
	int ChooseValue(CValue &Val,CString Title);

	CString ToSeparatedString();
	CValue Get(CString Str);
	CValue GetItemValue(int nIndex,CString &Str);
	CValue GetItemValue(int nIndex);
	int FindValue(CValue Val);
	int Belong(CValue Val);

	void PrepareFind(int nIndex,CElementList data);
	void PrepareFind(void );

	//конструкторы
	CValueList();
	virtual ~CValueList();
	//virtual UpDate();

	//Атрибуты:
	CArray <CElementList,CElementList> aValue;//список значений и представлений
	
	//для быстрого поиска:
	BOOL bValidFind;//признак показывающий имеет ли смысл информация в ниже определенных деревьях:
	CMapStringToPtr treeValues;
	//CMap <CValue*,CValue*,int,int> treeValues;
	CMapStringToPtr treePresents;
	CString csObjectName;

	void SaveToString(CString &Str);
	void LoadFromString(CString &Str);


	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	virtual CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	virtual CValue Method(int iName,CValue **aParams);//вызов метода
	//virtual CValue &Attribute(int iName);//вызов атрибута

	CString GetString(void)const{return csObjectName;};
	CString GetTypeString(void)const{return csObjectName;};
	void Init(CString StrVid,CValue Param){csObjectName=StrVid;};

};

#endif // !defined(AFX_VALUELIST_H__C187CB48_A2FF_4265_B27D_36223957B489__INCLUDED_)
//Авторские права - VTOOLS.RU (info@vtools.ru)
