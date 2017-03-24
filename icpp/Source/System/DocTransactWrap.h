#if !defined(DOC_TRANSACT_WRAP_H_INCLUDED)
#define DOC_TRANSACT_WRAP_H_INCLUDED

#pragma once

#include "GroupContextWrap.h"

class CDocTransactWrap : public CModuleEventsManager, boost::noncopyable
{
public:
	typedef boost::shared_ptr<CDocTransactWrap> CDocTransactWrapPtr;
	static CDocTransactWrapPtr& Get() { 
		if (!pWrapperForDocTransact)
			pWrapperForDocTransact = CDocTransactWrapPtr(new CDocTransactWrap);
		return pWrapperForDocTransact; 
	} 

	bool IsWrapEnable(void) const
	{ return m_bHaveSetHooks ;}

	void SetWrapper(const CString& strDocKind, IUserClassContext* pUserClassCont);
	void SetWrapper(const CValue& DocKindValue, const CValue& UserClassContValue);

	CUserHandlersManager GetWrapper(const CString& strDocKind) const;

	void DestroyWrapper(const CString& strDocKind);

	int FindAndRunModuleEventHandler( CBLModule7* pThisMod, char* sRusName, char* sEngName, int iParamsCount, CValue * * params );

private:
	typedef long CTypeID;

	CMap<CTypeID, CTypeID, CUserHandlersManager, CUserHandlersManager> m_ModuleMap;
	bool m_bHaveSetHooks;

	static CDocTransactWrapPtr pWrapperForDocTransact;

	CDocTransactWrap();

	LPCTSTR GetDocKind(CTypeID DocTypeID);
	void SetWrapperFromDocID(CTypeID DocTypeID, CUserHandlersManager pUserClassCont);

	bool Lookup(CTypeID DocTypeID, CUserHandlersManager& ppCont) const;
	int  FindWrapMethod(CTypeID DocTypeID, CBLModule7* pThisMod, char const * sRusName, char const * sEngName, CUserHandlersManager& pCont)const;
};

#endif // !defined(DOC_TRANSACT_WRAP_H_INCLUDED)