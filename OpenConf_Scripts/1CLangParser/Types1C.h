
#ifndef _TYPES_1C_
#define _TYPES_1C_

#include "istrmap.h"
//#import "C:\Program Files\1Cv77\BIN\config.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids 


//описание метода, добавляющего свойства конкретному экземпляру типа
//Считаем, что метод имеет общий вид <Name>(... <PropName>, ... <PropType>)
//Например, НоваяКолонка(Имя, Тип) в типе ТаблицаЗначений: 
// - Name = "НоваяКолонка"
// - nParam_PropName = 1 (имя колонки)
// - nParam_PropType = 2 (тип колонки)
class CTypeExemplarModifyingMethod
{
public:
	CString Name;
	int nParam_PropName;
	int nParam_PropType;
};

class CTypeExemplarModifyingMethodList : public CIStringMapToPtr
{
public:
	~CTypeExemplarModifyingMethodList();

	CTypeExemplarModifyingMethod* Add(const char* MethodName, int nParam_PropName, int nParam_PropType);
	CTypeExemplarModifyingMethod* GetMethod(const char* MethodName);
};



class CType1C
{
private:
	CPtrArray BaseTypes;
	int nBaseTypesCount;

	CString TypeName;
	CIStringMapToString Properties; //Формат: Имя: Тип
	CIStringMapToString Synonyms;

	//методы, добавляющие новые свойства к экземпляру данного типа
	CTypeExemplarModifyingMethodList ExemplarModifyingMethods;

	POSITION IteratorPos;
	int nIteratedBaseType;

public:
	CType1C(CString aTypeName);

	void AddBaseType(CType1C* _pBaseType);

	void AddProperty(CString Name, CString Synonym, CString Type);
	virtual bool PropExists(const char* PropName);
	virtual CString GetPropType(const char* PropName);

	virtual void StartPropsIteration();
	virtual bool NextProp(CString& PropName);

	CTypeExemplarModifyingMethod* AddExemplarModifyingMethod(const char* MethodName, int nParam_PropName, int nParam_PropType)
	{ return ExemplarModifyingMethods.Add(MethodName, nParam_PropName, nParam_PropType); };
	CTypeExemplarModifyingMethod* GetExemplarModifyingMethod(const char* MethodName)
	{ return ExemplarModifyingMethods.GetMethod(MethodName); };
};

/*
class CMetaDataType1C : public CType1C
{
private:
	IMetaDataObj* pMetaDataObj;
public:
	CMetaDataType1C(CString& strType, CString& strKind, IConfigurator* pConfigurator);

	virtual bool PropExists(const char* PropName);
	virtual CString GetPropType(const char* PropName);

	virtual void StartPropsIteration();
	virtual bool NextProp(CString& PropName);
};
*/

class CTypes1C
{
private:
	static int InstanceCounter;
	static CIStringMapToPtr* pTypes;
	static CIStringMapToString* pSynonyms;

public:
	CTypes1C();
	~CTypes1C();

	static void AddType(const char* TypeName, const char* Synonym, CType1C* Type);
	static void RemoveType(const char* TypeName);
	static void RemoveAll();

	static CType1C* GetType(const char* TypeName);
	static CString GetSubType(const char* TypeName, const char* PropName);
};

class CVariable
{
private:
	CIStringMapToString Properties; //Формат: Имя: Тип
	POSITION IteratorPos;

public:
	CString Type;

public:
	CVariable(const char* aType) { Type = aType; IteratorPos = NULL; };

	void AddProperty(const char* PropName, const char* PropType);
	CString GetPropType(const char* PropName);

	void StartPropsIteration();
	bool NextProp(CString& PropName);
};

class CVarList : public CIStringMapToPtr
{
public:
	~CVarList();
	void Add(const char* VarName, CVariable* Var);
	CVariable* Add(const char* VarName, const char* VarType);
	CVariable* GetVar(const char* VarName);
};

#endif
