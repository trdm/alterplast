
#include "StdAfx.h"
#include "ModuleEvents.h"

CModuleEventManager::CModuleEventManager() : m_bIsGlobalModule(false)
{
}

CModuleEventManager::~CModuleEventManager()
{
}

void CModuleEventManager::Init(CBLModule* pFormModule)
{
	m_pModule = pFormModule;
	m_bIsGlobalModule = ::GetGlobalModule() == pFormModule;
}

bool CModuleEventManager::AddEvent(stModuleEventData& ModuleEventData,
                                  const char* szPrefixName)
{
	stEvent event;

	CString strProcName;
	if (szPrefixName)
		strProcName = szPrefixName;
	strProcName += ModuleEventData.szRusName;

	bool bFind = FindEvent(ModuleEventData, strProcName, event);
	if (!bFind)
	{
		if (szPrefixName)
			strProcName = szPrefixName;
		strProcName += ModuleEventData.szEngName;

		bFind = FindEvent(ModuleEventData, strProcName, event);
		
	}

	//if (bFind){
		event.nObjEvent = -1;

		m_events.Add(event);
		m_pModuleEventData.Add(ModuleEventData);
	//}
	return bFind;
}

bool CModuleEventManager::FindEvent(stModuleEventData& ModuleEventData,
                                  const char* strProcName, stEvent& event)
{
	CBLProcInfo BLProcInfo;
	bool bSuccess = false;

	//event.nFormEvent = m_pModule->FindProc(strProcName, 0);
	event.nFormEvent = m_pModule->FindFunc(strProcName);
	if (event.nFormEvent != -1)
	{
		bSuccess = true;
		//event.bIsFunction = false;
		event.bIsFunction = true;

		m_pModule->GetProcInfo(event.nFormEvent,BLProcInfo);
		//if (BLProcInfo.GetNParams() > pModuleEventData->nParamCount+1)
		if (BLProcInfo.GetNParams() != ModuleEventData.nParamCount)
		{
			CString str;
			//str.Format("Количество параметров в событии '%s' должно быть меньше или равно %u.",
			str.Format("Количество параметров в событии '%s' должно быть точно равно %u.",
				strProcName, ModuleEventData.nParamCount);
			pBkEndUI->DoMessageLine(str, mmBlackErr);
			bSuccess = FALSE;
		}

		return bSuccess;
	}
	else{
		//event.nFormEvent = m_pModule->FindFunc(strProcName);
		event.nFormEvent = m_pModule->FindProc(strProcName, 0);
		if (event.nFormEvent != -1)
		{
			bSuccess = true;
			//event.bIsFunction = true;
			event.bIsFunction = false;

			m_pModule->GetProcInfo(event.nFormEvent,BLProcInfo);
			//if (BLProcInfo.GetNParams() > pModuleEventData->nParamCount+1)
			if (BLProcInfo.GetNParams() != ModuleEventData.nParamCount)
			{
				CString str;
				//str.Format("Количество параметров в событии '%s' должно быть меньше или равно %u.",
				str.Format("Количество параметров в событии '%s' должно быть точно равно %u.",
					strProcName, ModuleEventData.nParamCount);
				pBkEndUI->DoMessageLine(str, mmBlackErr);
				bSuccess = FALSE;
			}
			return bSuccess;
		}
	}

	return bSuccess;
}

BOOL CModuleEventManager::IsAppointed(int nEventIndex)
{
	stEvent& event = m_events[nEventIndex];
	return event.nFormEvent != -1 || event.nObjEvent != -1;
}

BOOL CModuleEventManager::Invoke(int nEventIndex, CValue& vRetVal, ...)
{
    CTypedPtrArray<CPtrArray, CValue*> params;
//     CValue vCtrl;
//     vCtrl.AssignContext(m_pCtrlObject);
//     params.Add(&vCtrl);

    va_list par_it;
    va_start(par_it, nEventIndex);
    for (int i = 0; i < m_pModuleEventData[nEventIndex].nParamCount; i++)
        params.Add(va_arg(par_it, CValue*));
    va_end(par_it);

    return InvokeEvent(nEventIndex, vRetVal, (CValue**)(params.GetData()));
}

BOOL CModuleEventManager::Invoke0(int nEventIndex, CValue& vRetVal)
{
    CValue vCtrl;
    //vCtrl.AssignContext(m_pCtrlObject);
    CValue* pValue = &vCtrl;
    return InvokeEvent(nEventIndex, vRetVal, &pValue);
}

BOOL CModuleEventManager::Invoke1(int nEventIndex, CValue& vRetVal, CValue& vP1)
{
    CValue vCtrl;
    //vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1};
    return InvokeEvent(nEventIndex, vRetVal, ppValues);
}

BOOL CModuleEventManager::Invoke2(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2)
{
    CValue vCtrl;
    //vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2};
    return InvokeEvent(nEventIndex, vRetVal, ppValues);
}

BOOL CModuleEventManager::Invoke3(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2, CValue& vP3)
{
    CValue vCtrl;
    //vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2, &vP3};
    return InvokeEvent(nEventIndex, vRetVal, ppValues);
}

BOOL CModuleEventManager::Invoke4(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2, CValue& vP3, CValue& vP4)
{
    CValue vCtrl;
    //vCtrl.AssignContext(m_pCtrlObject);
    CValue* ppValues[] = {&vCtrl, &vP1, &vP2, &vP3, &vP4};
    return InvokeEvent(nEventIndex, vRetVal, ppValues);
}

BOOL CModuleEventManager::InvokeEvent(int nEventIndex, CValue& vRetVal, CValue** ppValuesParams)
{
    BOOL bSuccess = TRUE;
    stEvent& event = m_events[nEventIndex];
    //stEvent event1 = m_events[nEventIndex];

// 	if (event.nObjEvent != -1)
// 		bSuccess = m_pDerivedObject->CallAsProc(event.nObjEvent, ppValuesParams + 1);

//	if (bSuccess && event.nFormEvent != -1)
	if (event.nFormEvent != -1)
		if (event.bIsFunction)
			//m_pModule->CallAsFunc(event.nFormEvent, vRetVal, m_pModuleEventData[nEventIndex].nParamCount + 1, ppValuesParams);
			m_pModule->CallAsFunc(event.nFormEvent, vRetVal, m_pModuleEventData[nEventIndex].nParamCount, ppValuesParams);
		else{
			//m_pModule->CallAsProc(event.nFormEvent, m_pModuleEventData[nEventIndex].nParamCount + 1, ppValuesParams);
			m_pModule->CallAsProc(event.nFormEvent, m_pModuleEventData[nEventIndex].nParamCount, ppValuesParams);
			vRetVal = 1;
		}
    
    return bSuccess;
}
