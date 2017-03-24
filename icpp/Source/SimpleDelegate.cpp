#include "StdAfx.h"
#include "SimpleDelegate.h"

SIMPLE_IMPLEMENT(CSimpleDelegate, CSimpleContext, "1cpp.Delegate", "1cpp.Делегат")
size_t CSimpleDelegate::m_idxInvoke = -1;

void CSimpleDelegate::InitDef()
{
					AddDef("Set",		"Установить",		2,		&CSimpleDelegate::procSet);
					AddDef("IsEmpty",	"Пустой",			0,		&CSimpleDelegate::funcIsEmpty);
	m_idxInvoke =	AddDef("Invoke",	"Вызвать",			0,		&CSimpleDelegate::funcInvoke);
}

CBLContext* CSimpleDelegate::aux_GetContext() const
{
	return m_vCont.GetContext();
};

CBLContext* CSimpleDelegate::safe_GetContext() const
{
	CBLContext *pCont = m_vCont.GetContext();
	if (!pCont)
		RuntimeError("Недопустимый контекст делегата!");
	return pCont;
};

int CSimpleDelegate::aux_FindMethod(LPCSTR szMethName) const
{
	CBLContext* pCont = aux_GetContext();
	return pCont ? pCont->FindMethod(szMethName) : -1;
}

int CSimpleDelegate::safe_FindMethod(LPCSTR szMethName) const
{
	if (*szMethName == 0)
		RuntimeError("Метод делегата не задан!");
	int nMethIndex = aux_FindMethod(szMethName);
	if (nMethIndex == -1)
		RuntimeError("Метод '%s' в контексте делегата не найден!", szMethName);
	return nMethIndex;
}

int CSimpleDelegate::safe_GetMethIndex() const
{
	if (m_nMethIndex == -1)
		RuntimeError("Метод делегата не задан!");
	return m_nMethIndex;
}

// ppValues[0]: Context
// ppValues[1]: MethodName
BOOL CSimpleDelegate::procSet(CValue** ppValues)
{
	m_vCont = *ppValues[0];
	m_nMethIndex = -1;
	safe_GetContext();

	LPCSTR szMethName = (LPCSTR)ppValues[1]->GetString();
	m_nMethIndex = safe_FindMethod(szMethName);

	return TRUE;
}

BOOL CSimpleDelegate::funcIsEmpty(CValue& RetValue, CValue** ppValues)
{
	
	RetValue = !aux_GetContext() || m_nMethIndex == -1 ? 1L : 0L;
	return TRUE;
}

BOOL CSimpleDelegate::funcInvoke(CValue& RetValue, CValue** ppValues)
{
	// never used
	return TRUE;
}

int	CSimpleDelegate::GetNParams(int nMethIndex) const
{
	if (nMethIndex != m_idxInvoke)
		return CSimpleContext::GetNParams(nMethIndex);

	CBLContext* pCont = safe_GetContext();
	nMethIndex = safe_GetMethIndex();
	return pCont->GetNParams(m_nMethIndex);
}

int CSimpleDelegate::GetParamDefValue(int nMethIndex, int nParamIndex, CValue* pDefVal) const
{
	if (nMethIndex != m_idxInvoke)
		return CSimpleContext::GetParamDefValue(nMethIndex, nParamIndex, pDefVal);

	CBLContext* pCont = safe_GetContext();
	nMethIndex = safe_GetMethIndex();
	return pCont->GetParamDefValue(nMethIndex, nParamIndex, pDefVal);
}

int	CSimpleDelegate::HasRetVal(int nMethIndex) const
{
	if (nMethIndex != m_idxInvoke)
		return CSimpleContext::HasRetVal(nMethIndex);

	CBLContext* pCont = safe_GetContext();
	nMethIndex = safe_GetMethIndex();
	return pCont->HasRetVal(nMethIndex);
}

int	CSimpleDelegate::CallAsProc(int nMethIndex, CValue** ppVal)
{
	if (nMethIndex != m_idxInvoke)
		return CSimpleContext::CallAsProc(nMethIndex, ppVal);

	CBLContext* pCont = safe_GetContext();
	nMethIndex = safe_GetMethIndex();
	return pCont->CallAsProc(nMethIndex, ppVal);
}

int	CSimpleDelegate::CallAsFunc(int nMethIndex, CValue& RetVal, CValue** ppVal)
{
	if (nMethIndex != m_idxInvoke)
		return CSimpleContext::CallAsFunc(nMethIndex, RetVal, ppVal);

	CBLContext* pCont = safe_GetContext();
	nMethIndex = safe_GetMethIndex();
	return pCont->CallAsFunc(nMethIndex, RetVal, ppVal);
}
