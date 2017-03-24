// EventManager.h: interface for the CEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMANAGER_H__D1684BCD_0BD1_465C_85EF_A98E8DD54E26__INCLUDED_)
#define AFX_EVENTMANAGER_H__D1684BCD_0BD1_465C_85EF_A98E8DD54E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Delegate.h"

class CEventManager : public CMyContextBase
{
DECLARE_DYNCREATE(CEventManager);
public:
	CEventManager();

	BOOL funcIsEmpty(CValue& RetVal, CValue **params);
	BOOL funcAddEventHandle(CValue& RetVal, CValue **params);
	BOOL funcRemEventHandle(CValue& RetVal, CValue **params);
	BOOL funcSendEvents(CValue& RetVal, CValue **params);
	BOOL funcSetDelegate(CValue& RetVal, CValue **params);

	BOOL procClear(CValue **params);
	BOOL procSetCountOfParams(CValue **params);

	BOOL funcGetHandlersTable(CValue& RetVal, CValue **params);
	
 	virtual int  GetNParams(int)const;	//2008
 	virtual char const *  GetTypeString(void)const;	//2513
private:
  CBLPtr<CDelegate> m_pDelegate;

  DECLARE_MY_CONTEXT();
};



#endif // !defined(AFX_EVENTMANAGER_H__D1684BCD_0BD1_465C_85EF_A98E8DD54E26__INCLUDED_)
