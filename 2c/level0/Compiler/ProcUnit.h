// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// CProcUnit.h: interface for the CProcUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CProcUnit_H__30FAD305_5C15_11D4_8318_0050229BB234__INCLUDED_)
#define AFX_CProcUnit_H__30FAD305_5C15_11D4_8318_0050229BB234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CompileModule.h"
#include "Value.h"

#define MAX_STATIC_VAR	10

class CErrorPlace
{
public:
	CErrorPlace()
	{
		Reset();
	};
	void Reset()
	{
		pByteCode=0;
		pSkipByteCode=0;
		nLine=-1;
	};
	CByteCode* pByteCode;
	CByteCode* pSkipByteCode;
	int nLine;
};
extern CErrorPlace afxErrorPlace;


class CRunContext
{
public:
	CRunContext(int nLocal=-1)
	{
		nVarCount=0;
		nParamCount=0;
		pCompileContext=0;
		nCurLine=0;
		pProcUnit=0;
		if(nLocal>=0)
			SetLocalCount(nLocal);
	};
	~CRunContext();
	int nStart;
	int nCurLine;//текущая исполняемая строка байт-кода
	class CProcUnit *pProcUnit;

	int nParamCount;
	CValue cLocVars[MAX_STATIC_VAR];
	CValue *pLocVars;
	CValue *cRefLocVars[MAX_STATIC_VAR];
	CValue **pRefLocVars;

	CCompileContext *pCompileContext;
	CMapStringToPtr aEvalString;

private:
	int nVarCount;

public:
	void SetLocalCount(int nLocal)
	{
		nVarCount=nLocal;
		if(nVarCount>MAX_STATIC_VAR)
		{
			pLocVars =  new CValue[nVarCount];
			pRefLocVars =  new CValue*[nVarCount];
		}
		else
		{
			pLocVars=cLocVars;
			pRefLocVars=cRefLocVars;
		}
		for(int i=0;i<nVarCount;i++)
			pRefLocVars[i]=&pLocVars[i];
	};
	int GetLocalCount()
	{
		return nVarCount;
	};
};


#define MAX_STATIC_VAR3 1
class CRunContextSmall
{
public:
	CRunContextSmall(int nLocal=-1)
	{
		nVarCount=0;
		nParamCount=0;
		if(nLocal>=0)
			SetLocalCount(nLocal);
	};
	~CRunContextSmall()
	{
		if(nVarCount>MAX_STATIC_VAR3)
		{
			delete []pLocVars;
			delete []pRefLocVars;
		}
	}
	int nStart;

	int nParamCount;
	CValue cLocVars[MAX_STATIC_VAR3];
	CValue *pLocVars;
	CValue *cRefLocVars[MAX_STATIC_VAR3];
	CValue **pRefLocVars;
private:
	int nVarCount;

public:
	void SetLocalCount(int nLocal)
	{
		nVarCount=nLocal;
		if(nVarCount>MAX_STATIC_VAR3)
		{
			pLocVars =  new CValue[nVarCount];
			pRefLocVars =  new CValue*[nVarCount];
		}
		else
		{
			pLocVars=cLocVars;
			pRefLocVars=cRefLocVars;
		}
		for(int i=0;i<nVarCount;i++)
			pRefLocVars[i]=&pLocVars[i];
	};
	int GetLocalCount()
	{
		return nVarCount;
	};
};
//////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////
class CProcUnit  
{
public:
	//Конструкторы/деструкторы
	CProcUnit();
	virtual ~CProcUnit();

	///Атрибуты
	CByteCode *pByteCode;
	CRunContext cCurContext;
	CValue	***pppArrayList;//указатели на массивы указателей переменных (0 - локальные переменные,1-переменные текущего модуля,2 и выше - переменные родительских модулей)
	CProcUnit **ppArrayCode;//указатели на массивы выполняемых модулей (0-текущий модуль,1 и выше - родительские модули)
	int nCommonModule;		//признак общего модуля (служебная инфрмация для заполнителя, например для Менеджера модулей)
	CValue	vContext;		//контекст текущего модуля для доступа из языка
	int nAutoDeleteParent;	//признак удаления родительского модуля
	
	CValue	vParentObj;		//объект родителя если он есть
	class CValueObject* pCurrentObj;//указатель на обработчик функции СоздатьОбъект


protected:
	//CProcUnit *m_pParent;
	CArray <CProcUnit *,CProcUnit *> aParent;

public:
	//Методы
	void Clear();
	void SetParent(CProcUnit *pSetParent);
	CProcUnit *GetParent(int nLevel=0);
	int	GetParentCount();
	CValue Exec(CByteCode &ByteCode,int nRunModule=1);
	CValue Exec(CRunContext *pContext,int bStartModule);//bStartModule=1 - признак выполнения операторов модуля, которые идут в конце функций и процедур
	static CValue Eval(CString StrRun,CRunContext *pRunContext=0,int bCompileBlock=0);
	int CompileExpr(CString StrRun,CRunContext *pRunContext,CCompileModule &cModule,int bCompileBlock);


	//вызов произвольной функции исполняемого модуля
	int FindExportFunction(CString csName);
	int FindFunction(CString csName0,BOOL bError=0,int bExportOnly=0);

	CValue CallFunction(CString csName);
	CValue CallFunction(CString csName,CValue &vParam1);
	CValue CallFunction(CString csName,CValue &vParam1,CValue &vParam2);
	CValue CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3);
	CValue CallFunction(CString csName,CValue **ppParams,int nParamCount);
	CValue CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5,CValue &vParam6,CValue &vParam7);

	CValue CallFunction(int lptr,CValue &vParam1);
	CValue CallFunction(int lptr,CValue &vParam1,CValue &vParam2);
	CValue CallFunction(int nAddr,CValue **ppParams,int nReceiveParamCount=MAX_STATIC_VAR);


	void SetAttribute(CString csName,CValue &Val);
	void SetAttribute(int iName,CValue &Val);//установка атрибута
	CValue GetAttribute(int iName);//значение атрибута
	int  FindAttribute(CString csName);
};




#endif // !defined(AFX_CProcUnit_H__30FAD305_5C15_11D4_8318_0050229BB234__INCLUDED_)
