// BLModuleWrapper.h: interface for the CBLModuleWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_)
#define AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModuleString.h"

#include <boost/utility.hpp>
#include <string>

class CBLModuleWrapper  : public CBLModule7, boost::noncopyable
{
public:	
	static CBLContext* GetContextFromModule (CBLModule* pMod);

	// TODO лучше возвращать какой-нибудь смарт-пойнтер
	static CBLModuleWrapper* CreateSimpleModule();
	static CBLModuleWrapper* CreateClassModule(const CString& strNameOfComponent, CBLContext* pContext, CBLModuleWrapper* pMod);
	static CBLModuleWrapper* CreateClassModule(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString);

	virtual ~CBLModuleWrapper();

protected:
	CBLModuleWrapper(CBLContext* pContext);
	CBLModuleWrapper(CBLContext* pContext, LPCSTR szModuleString);

	////	artbear - при нативной отладке этот метод только мешает !
	//virtual int GetKind(void)const = 0; // artbear по рекомендации јль‘-а

	virtual int				OnSyntaxError(void) = 0;
	virtual int				OnRuntimeError(void) = 0;
	virtual int				OnStartExecution(void) = 0;
	virtual int				OnNextLine(void) = 0;
	virtual void			OnEnterProc(int) = 0;
	virtual void			OnExitProc(int) = 0;
	virtual void			OnStopExecution(void) = 0;
	virtual void			OnErrorMessage(char const *) = 0;
};

typedef boost::shared_ptr<CBLModuleWrapper> CComponentClassModulePtr;

#endif // !defined(AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_)
