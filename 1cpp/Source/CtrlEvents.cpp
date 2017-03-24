
#include "StdAfx.h"
#include "CtrlEvents.h"

CCtrlEventManager::CCtrlEventManager() : m_nRefCount(0)
{
	IncrRef();
}

CCtrlEventManager::~CCtrlEventManager()
{
}

void CCtrlEventManager::IncrRef()
{
	++m_nRefCount;
}

void CCtrlEventManager::DecrRef()
{
	if (!--m_nRefCount)
		delete this;
};

void CCtrlEventManager::SetEvents(stCtrlEvents* pCtrlEvents, int nCount,
                                  const char* szCodeName, CBLModule* pFormModule, CBLContext* pCtrlObject,
                                  CBLContext* pDerivedObject)
{
	m_pCtrlEvents = pCtrlEvents;
	m_pModule = pFormModule;
	m_pCtrlObject = pCtrlObject;
	m_pDerivedObject = pDerivedObject;

	CBLProcInfo BLProcInfo;
	stEvent event;
	BOOL bSuccess = TRUE;

	for (; nCount > 0; nCount--, pCtrlEvents++)
	{
		CString strProcName;
		if (szCodeName)
			strProcName = szCodeName;
		strProcName += pCtrlEvents->szRusName;
		event.nFormEvent = m_pModule->FindProc(strProcName, 0);
		if (event.nFormEvent != -1)
		{
			m_pModule->GetProcInfo(event.nFormEvent,BLProcInfo);
			if (BLProcInfo.GetNParams() > pCtrlEvents->nParamCount+1)
			{
				CString str;
				str.Format(" оличество параметров в событии '%s' должно быть меньше или равно %u.",
					strProcName, pCtrlEvents->nParamCount+1);
				pBkEndUI->DoMessageLine(str, mmBlackErr);
				bSuccess = FALSE;
			}
		}
		else
		{
			if (szCodeName)
				strProcName = szCodeName;
			strProcName += pCtrlEvents->szEngName;
			event.nFormEvent = m_pModule->FindProc(strProcName, 0);
			if (event.nFormEvent != -1)
			{
				m_pModule->GetProcInfo(event.nFormEvent,BLProcInfo);
				if (BLProcInfo.GetNParams() > pCtrlEvents->nParamCount+1)
				{
					CString str;
					str.Format(" оличество параметров в событии '%s' должно быть меньше или равно %u.",
						strProcName, pCtrlEvents->nParamCount+1);
					pBkEndUI->DoMessageLine(str, mmBlackErr);
					bSuccess = FALSE;
				}
			}
		}

		if (pDerivedObject)
		{
			event.nObjEvent = pDerivedObject->FindMethod(pCtrlEvents->szRusName);
			if (event.nObjEvent != -1)
			{
				if (pDerivedObject->GetNParams(event.nObjEvent) > pCtrlEvents->nParamCount)
				{
					CString str;
					str.Format(" оличество параметров в событии '%s::%s'  должно быть меньше или равно %u.",
						pDerivedObject->GetTypeString(), pCtrlEvents->szRusName, pCtrlEvents->nParamCount);
					pBkEndUI->DoMessageLine(str, mmBlackErr);
					bSuccess = FALSE;
				}
			}
			else
			{
				event.nObjEvent = pDerivedObject->FindMethod(pCtrlEvents->szEngName);
				if (event.nObjEvent != -1)
				{
					if (pDerivedObject->GetNParams(event.nObjEvent) > pCtrlEvents->nParamCount)
					{
						CString str;
						str.Format(" оличество параметров в событии '%s::%s'  должно быть меньше или равно %u.",
							pDerivedObject->GetTypeString(), pCtrlEvents->szEngName, pCtrlEvents->nParamCount);
						pBkEndUI->DoMessageLine(str, mmBlackErr);
						bSuccess = FALSE;
					}
				}
			}
		}
		else
			event.nObjEvent = -1;

		m_events.Add(event);

	}

	if (!bSuccess)
		RuntimeError("ќшибка подключени€ событий.");
}

BOOL CCtrlEventManager::IsAppointed(int nEventIndex)
{
	stEvent& event = m_events[nEventIndex];
	return event.nFormEvent != -1 || event.nObjEvent != -1;
}

BOOL CCtrlEventManager::Invoke(int nEventIndex, ...)
{
    CTypedPtrArray<CPtrArray, CValue*> params;
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    params.Add(&vCtrl);

    va_list par_it;
    va_start(par_it, nEventIndex);
    for (int i = 0; i < m_pCtrlEvents[nEventIndex].nParamCount; i++)
        params.Add(va_arg(par_it, CValue*));
    va_end(par_it);

    return InvokeEvent(nEventIndex, (CValue**)(params.GetData()));
}

BOOL CCtrlEventManager::Invoke0(int nEventIndex)
{
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    CValue* pValue = &vCtrl;
    return InvokeEvent(nEventIndex, &pValue);
}

BOOL CCtrlEventManager::Invoke1(int nEventIndex, CValue& vP1)
{
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1};
    return InvokeEvent(nEventIndex, ppValues);
}

BOOL CCtrlEventManager::Invoke2(int nEventIndex, CValue& vP1, CValue& vP2)
{
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2};
    return InvokeEvent(nEventIndex, ppValues);
}

BOOL CCtrlEventManager::Invoke3(int nEventIndex, CValue& vP1, CValue& vP2, CValue& vP3)
{
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2, &vP3};
    return InvokeEvent(nEventIndex, ppValues);
}

BOOL CCtrlEventManager::Invoke4(int nEventIndex, CValue& vP1, CValue& vP2, CValue& vP3, CValue& vP4)
{
    CValue vCtrl;
    vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2, &vP3, &vP4};
    return InvokeEvent(nEventIndex, ppValues);
}

CBLContext* CCtrlEventManager::GetCtrlObject() const
{
    return m_pCtrlObject;
}

void CCtrlEventManager::SetCtrlObject(CBLContext* pCtrlObject)
{
    m_pCtrlObject = pCtrlObject;
}

CBLContext* CCtrlEventManager::GetDerivedObject() const
{
    return m_pDerivedObject;
}

BOOL CCtrlEventManager::InvokeEvent(int nEventIndex, CValue** ppValuesParams)
{
    BOOL bSuccess = TRUE;
    stEvent& event = m_events[nEventIndex];

	if (event.nObjEvent != -1)
		bSuccess = m_pDerivedObject->CallAsProc(event.nObjEvent, ppValuesParams + 1);

	if (bSuccess && event.nFormEvent != -1)
		m_pModule->CallAsProc(event.nFormEvent, m_pCtrlEvents[nEventIndex].nParamCount + 1, ppValuesParams);
    
    return bSuccess;
}
