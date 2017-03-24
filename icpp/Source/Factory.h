//////////////////////////////////////////////////////////////////////
// Factory.h: interface for the CObjectFactory class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Added by Артур Аюханов aka artbear, mailto: artbear@inbox.ru 2006
// Version: 1.0
//////////////////////////////////////////////////////////////////////
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#pragma once

#include "mycontextbase.h"

//#include "System/CreationBoost.h" // TODO для TEST_FOR_COMPARE_CREATION_SPEED

//class CObjectFactory : public CBLContext
class CObjectFactory : public CMyContextBase
{
    DECLARE_DYNCREATE(CObjectFactory);
public:
    CObjectFactory();
//     virtual ~CObjectFactory();

    //CBLContext
	//virtual char const *	GetTypeString(void)const;

	virtual int				GetNMethods(void)const;
	virtual int				FindMethod(char const* szName)const;
	virtual char const *	GetMethodName(int nMethIndex, int bAlias)const;
	virtual int				GetNParams(int nMethIndex)const;
	virtual int				GetParamDefValue(int nMethIndex, int nParamIndex, CValue * pValue)const;
	virtual int				HasRetVal(int nMethIndex)const;

	virtual int				CallAsProc(int nMethIndex, CValue** ppValues);
	virtual int				CallAsFunc(int nMethIndex, CValue& rValue, CValue** ppValues);

    static void RegisterClass(LPCSTR szClassName, CRuntimeClass* pRuntimeClass, int nCtorParamsCount);
	static void UnRegisterClass(LPCSTR szClassName);

	BOOL funcNewObject(int nParamsCount, CValue &RetVal, CValue **ppValue);

//#ifdef TEST_FOR_COMPARE_CREATION_SPEED
//	BOOL procDisableFastCreationAllContexts(CValue **ppValue);
//#endif

    DECLARE_MY_CONTEXT()
};

#endif