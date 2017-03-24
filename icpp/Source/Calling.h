// Calling.h: interface for the CBLModuleWrapper class.
//
// Автор - artbear, 2007
//
//////////////////////////////////////////////////////////////////////

#if !defined(CALLING_H__INCLUDED_)
	#define CALLING_H__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

extern int CallAsFunc(CBLModule* pMod, int iProcNum, CValue& retVal, int iParamsCount, CValue** ppParams);

// функции для удобного вызова процедур модуля с проверкой количества параметров
static int CallAsProc0(CBLModule* pMod, int iProcNum)
{
	CValue ValueParam;
	CValue *pParams[1] = {0};
	pParams[0] = &ValueParam;
	return pMod->CallAsProc(iProcNum, 0, pParams);
};

static int CallAsProc1(CBLModule* pMod, int iProcNum, CValue const & param1)
{
	const CValue *pParams[1] = {0};
	pParams[0] = &param1;
	return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));
};

static int CallAsProc2(CBLModule* pMod, int iProcNum, CValue const & param1, CValue const & param2)
{
	const CValue* pParams[2] = {0};
	pParams[0] = &param1;
	pParams[1] = &param2;
	return pMod->CallAsProc(iProcNum, 2, const_cast<CValue**>(pParams));
};

// функции для удобного вызова процедур модуля с проверкой количества параметров
static int CallAsFunc0(CBLModule* pMod, int iProcNum, CValue& retVal)
{
	CValue ValueParam;
	CValue *pParams[1] = {0};
	pParams[0] = &ValueParam;
	//return pMod->CallAsProc(iProcNum, 0, pParams);
	//return pMod->CallAsFunc(iProcNum, retVal, 0,pParams);
	return ::CallAsFunc(pMod, iProcNum, retVal, 0,pParams);
};

static int CallAsFunc1(CBLModule* pMod, int iProcNum, CValue& retVal, CValue const & param1)
{
	const CValue *pParams[1] = {0};
	pParams[0] = &param1;
	//return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));
	//return pMod->CallAsFunc(iProcNum, retVal, 1, const_cast<CValue**>(pParams));
	return ::CallAsFunc(pMod, iProcNum, retVal, 1, const_cast<CValue**>(pParams));
};

static int CallAsFunc2(CBLModule* pMod, int iProcNum, CValue& retVal, CValue const & param1, CValue const & param2)
{
	const CValue* pParams[2] = {0};
	pParams[0] = &param1;
	pParams[1] = &param2;
	//return pMod->CallAsProc(iProcNum, 2, const_cast<CValue**>(pParams));
	//return pMod->CallAsFunc(iProcNum, retVal, 2, const_cast<CValue**>(pParams));
	return ::CallAsFunc(pMod, iProcNum, retVal, 2, const_cast<CValue**>(pParams));
};

#endif // CALLING_H__INCLUDED_