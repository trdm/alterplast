#ifndef SIMPLE_DELEGATE_H_INCLUDED
#define SIMPLE_DELEGATE_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CSimpleDelegate class (c) kms 2010
//////////////////////////////////////////////////////////////////////////

#include "SimpleContext.h"
class CSimpleDelegate : public CSimpleContext
{
	SIMPLE_DECLARE(CSimpleDelegate, CSimpleContext)
public:
	I_PROC(procSet);
	I_FUNC(funcIsEmpty);
	I_FUNC(funcInvoke);

	virtual int GetNParams(int nMethIndex) const;
	virtual int	GetParamDefValue(int nMethIndex, int nParamIndex, CValue* pDefVal) const;
	virtual int	HasRetVal(int nMethIndex) const;
	virtual int	CallAsProc(int nMethIndex, CValue** ppVal);
	virtual int	CallAsFunc(int nMethIndex, CValue& RetVal, CValue** ppVal);

private:
	CBLContext* aux_GetContext() const;
	CBLContext* safe_GetContext() const;
	int aux_FindMethod(LPCSTR szMethName) const;
	int safe_FindMethod(LPCSTR szMethName) const;
	int CSimpleDelegate::safe_GetMethIndex() const;

private:
	CValue m_vCont;
	int m_nMethIndex; // can be uninitalized
	static size_t m_idxInvoke;
};

#endif
