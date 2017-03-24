//////////////////////////////////////////////////////////////////////
// Factory.h: interface for the CObjectFactory class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Added by јртур јюханов aka artbear, mailto: artbear@inbox.ru 2006
// Version: 1.0
//////////////////////////////////////////////////////////////////////
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#pragma once

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
	virtual int				GetParamDefValue(int nMethIndex, int nParamIndex, class CValue * pValue)const;
	virtual int				HasRetVal(int nMethIndex)const;

	virtual int				CallAsProc(int nMethIndex, class CValue** ppValues);
	virtual int				CallAsFunc(int nMethIndex, class CValue& rValue, class CValue** ppValues);

    static void RegisterClass(LPCSTR szClassName, CRuntimeClass* pRuntimeClass, int nCtorParamsCount);

	BOOL funcNewObject(int nParamsCount, CValue &RetVal, CValue **ppValue);

    DECLARE_MY_CONTEXT()
};

#endif