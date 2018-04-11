// RHook.h: interface for the CRHook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHOOK_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_)
#define AFX_RHOOK_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\\MyContext\\mycontextbase.h"

class CRHook  : public CMyContextBase
{
	DECLARE_DYNCREATE(CRHook);

public:
	CRHook();
	virtual ~CRHook();

	BOOL funcSet(CValue& RetVal, CValue **params);
	BOOL funcScript(CValue& RetVal, CValue **params);
	BOOL funcSetHookEsc(CValue& RetVal, CValue **params);

	BOOL DefaultValue(int nParam, CValue* param) const;

	RDECLARE_MY_CONTEXT;

	BOOL m_Hook;
	CString m_Batch;
	BOOL m_HookEsc;
	CString m_BatchEsc;
	HHOOK hHook;
	CBLModule7 * mod;
};

#endif // !defined(AFX_RHOOK_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_)
