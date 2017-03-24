// Delegate.h: interface for the CDelegate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELEGATE_H__4EBA8CA4_6A7D_40B7_8586_680C8F95100C__INCLUDED_)
#define AFX_DELEGATE_H__4EBA8CA4_6A7D_40B7_8586_680C8F95100C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycontextbase.h"
#include "SafeContext.h"

class CDelegate : public CMyContextBase
{
public:
	DECLARE_DYNCREATE(CDelegate);

	CDelegate();
	~CDelegate();
	BOOL procDefineType(CValue **params);
	int	 defsDefineType(int nParam, CValue* param)const;
	BOOL funcAdd(CValue& RetVal, CValue **params);
	int	 defsAdd(int nParam, CValue* param)const;
	BOOL funcRemove(CValue& RetVal, CValue **params);
	BOOL funcRemoveByKey(CValue& RetVal, CValue **params);
	BOOL funcInvoke(CValue& RetVal, CValue **params);
	BOOL funcInvokeVL(CValue& RetVal, CValue **params);
	BOOL funcInvokeByKey(CValue& RetVal, CValue **params);
	BOOL funcInvokeVLByKey(CValue& RetVal, CValue **params);
	BOOL procErase(CValue **params);
	BOOL funcIsEmpty(CValue& RetVal, CValue **params);

	BOOL funcGetHandlersTable(CValue& RetVal, CValue **params);
	BOOL funcGetHandlersTableByKey(CValue& RetVal, CValue **params);

	// checks
	void CheckType() const;
	void CheckMethName(CString& sMethName, LPCSTR sFuncName, LPCSTR sParamNum);
	void CheckContext(CBLContext *pCont, LPCSTR sFuncName, LPCSTR sParamNum);
	int  CheckFindMethod(CBLContext *pCont, CString& sMethName);

	// service
	LPCSTR ResolveKey(LPCSTR key) const;
	LPCSTR ResolveKey(CValue *cvKey) const;
	CBLContext* GetContext(const CValue *cvCont) const;
	CValue** GetVLParams(CValue const& val) const;

	// backend
	void DefineType(long nParamCount, long nIsFunc, long nHonorBreakRequests);
	void ResetType();
	long Add(CValue **params, LPCSTR key);
	void Invoke(CValue &RetVal, CValue **params, bool bHonorBreakRequests);
	void InvokeByKey(CValue &RetVal, CValue **params, LPCSTR key, bool bHonorBreakRequests);
	void Erase(bool bResetType);
	int GetDefinedParamCount() const;

	void GetHandlersTable(CValue& RetVal) const;
	void GetHandlersTableByKey(CValue &RetVal, LPCSTR key) const;

	// context
	int GetNParams(int iMethodNum) const;
private:
	struct InfoEventHandlers
	{
		CBLContext * pBLCont;
		int nNumMethOfCall;
		sh_ptr<CValue> pvCont;

//		InfoEventHandlers() : pBLCont(NULL), nNumMethOfCall(-1)  {}
		InfoEventHandlers(CBLContext *p, int n, CValue *pv) : pBLCont(p), nNumMethOfCall(n), pvCont(pv) {}

		bool operator == (const InfoEventHandlers& rhs)
		{
			return (pBLCont == rhs.pBLCont && nNumMethOfCall == rhs.nNumMethOfCall);
		}        
	};

	typedef list<InfoEventHandlers> LIST;
	typedef list<InfoEventHandlers>::iterator ITER;
	typedef list<InfoEventHandlers>::const_iterator CONST_ITER;
	typedef list<InfoEventHandlers>::reverse_iterator REVERSE_ITER;

	int m_nParamCount;
	int m_nIsFunc;
	bool m_bHonorBreakRequests;

	CIStringMap<LIST*, LIST*> m_map;
	
	// используется для хранения методов без указания ключа
	static LPCSTR m_voidKey;

	// используется для вызова по ключу, когда заданный ключ не найден
	static LPCSTR m_defaultKey;

	#define nInvokeMethods 2
	int aInvokeMethods[nInvokeMethods];

	bool CallFromList(LIST *hl, CValue &RetVal, CValue **params, bool bHonorBreakRequests);
	int EmptyList(LIST *hl);

	CValueTable* PrepareHandlersTable(CValue& RetVal, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const;
	void SaveToHandlersTable(LIST *hl, CValueTable* pVT, sh_array<CValue>& pv, sh_array<CValue*>& ppv) const;

	DECLARE_MY_CONTEXT();
};

#endif // !defined(AFX_DELEGATE_H__4EBA8CA4_6A7D_40B7_8586_680C8F95100C__INCLUDED_)
