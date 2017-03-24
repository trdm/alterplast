/////////////////////////////////////////////////////////////////////
// CtrlEvents.h
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro-75@mail.ru
// Copyright (c) 2006-2009. All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////
/* sample to use: firstly call SetEvents and pass array of stCtrlEvents structure, last item must have nulled members

    //////////////////////////////////////////////////////////////////////////
    
	if (CtrlEventManager.IsAppointed(MyEvent)) //this check not requed, only for optimal performance
	{
		//setup params
		CtrlEventManager.Invoke(MyEvent, p1, p2); //when the call is successful, returns TRUE, also if event is not appointed
	}
*/

#pragma once

#include "blptr.h"
#include "System/BatchGuard.h"

struct stCtrlEvents
{
	LPCSTR szRusName;
	LPCSTR szEngName;
	int nParamCount;
};

class CCtrlEventManager
{
public:
	CCtrlEventManager();
	~CCtrlEventManager();
	
	void SetEvents(stCtrlEvents* pCtrlEvents, int nCount,
        const char* szCodeName, CBLModule* pFormModule, CBLContext* pCtrlObject,
        CBLContext* pDerivedObject);
	void SetEvents(stCtrlEvents* pCtrlEvents, int nCount,
        CControlID* pControlID, CBLModule* pFormModule, CBLContext* pCtrlObject,
        CBLContext* pDerivedObject)
	{
		SetEvents(pCtrlEvents, nCount, pControlID->m_CodeName, pFormModule, pCtrlObject, pDerivedObject);
	};

	void ClearEvents() { m_events.RemoveAll(); };

	BOOL IsAppointed(int nEventIndex);
	BOOL Invoke(int nEventIndex, ...);
	BOOL Invoke0(int nEventIndex);
	BOOL Invoke1(int nEventIndex, CValue& vP1);
	BOOL Invoke2(int nEventIndex, CValue& vP1, CValue& vP2);
	BOOL Invoke3(int nEventIndex, CValue& vP1, CValue& vP2, CValue& vP3);
	BOOL Invoke4(int nEventIndex, CValue& vP1, CValue& vP2, CValue& vP3, CValue& vP4);
	BOOL Invoke5(int nEventIndex, CValue& vP1, CValue& vP2, CValue& vP3, CValue& vP4, CValue& vP5);
    CBLContext* GetCtrlObject() const;
    void SetCtrlObject(CBLContext*);
    CBLContext* GetDerivedObject() const;

	void IncrRef();
	void DecrRef();

private:
    BOOL InvokeEvent(int nEventIndex, CValue** ppValuesParams);
	stCtrlEvents* m_pCtrlEvents;
	CBLModule* m_pModule;
	CBLPtr<CBLContext> m_pCtrlObject;
	CBLPtr<CBLContext> m_pDerivedObject;

	struct stEvent
	{
		int nFormEvent;
		int nObjEvent;
	};
	CArray<stEvent, stEvent&> m_events;
	int m_nRefCount;
};
