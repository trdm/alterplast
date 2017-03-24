/////////////////////////////////////////////////////////////////////
// ModuleEvents.h
//
//
// Автор Артур Аюханов aka artbear, mailto: artbear@inbox.ru
// Copyright (c) 2007-2009. All Rights Reserved.
//
// //////////////////////////////////////////////////////////////////////////
//
//	Основные отличия от CCtrlEventManager:
//	1. события могут добавляться в произвольный момент - 
//		для управления возможности произвольного подключения события в 1С
//		и это удобно использовать для перехвата таких событий Перехватчиком 1С++ для КОП-ов
//	2. Обработчики событий могут быть функциями, а не только процедурами
//		Возвращаемое значение подобных событий может использоваться, например,
//		для исключения системных действия - флаг ФСО
// //////////////////////////////////////////////////////////////////////////
//
// Инструкция для использования: 
//		сначала Init(pNeedModule);
//		далее при необходимости добавляем нужные событие через AddEvent
// 
//		вызов производится следующим образом
// 		if (ModuleEventManager.IsAppointed(MyEvent)) //ОБЯЗАТЕЛЬНО, а не только для оптимизации
// 		{
// 			//здесь нужно выполнить установку параметров
// 			ModuleEventManager.Invoke(MyEvent, p1, p2); //если вызов успешный, возвращает TRUE, даже если события в модуле нет
// 		}


#pragma once

//#include ".//..//blptr.h"
#include "BatchGuard.h"

struct stModuleEventData
{
	LPCSTR szRusName;
	LPCSTR szEngName;
	int nParamCount;
};

class CModuleEventManager
{
public:
	CModuleEventManager();
	~CModuleEventManager();
	
// 	void SetEvents(stModuleEventData* pModuleEventData, int nCount,
//         const char* szCodeName, CBLModule* pFormModule);
	void Init(CBLModule* pFormModule);

	bool AddEvent(stModuleEventData& ModuleEventData,
        const char* szCodeName);

	void ClearEvents() { m_events.RemoveAll(); m_pModuleEventData.RemoveAll(); };

	BOOL IsAppointed(int nEventIndex);
	BOOL Invoke(int nEventIndex, CValue& vRetVal, ...);
	BOOL Invoke0(int nEventIndex, CValue& vRetVal);
	BOOL Invoke1(int nEventIndex, CValue& vRetVal, CValue& vP1);
	BOOL Invoke2(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2);
	BOOL Invoke3(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2, CValue& vP3);
	BOOL Invoke4(int nEventIndex, CValue& vRetVal, CValue& vP1, CValue& vP2, CValue& vP3, CValue& vP4);
//     CBLContext* GetCtrlObject() const;
//     void SetCtrlObject(CBLContext*);
//     CBLContext* GetDerivedObject() const;

    BOOL InvokeEvent(int nEventIndex, CValue& vRetVal, CValue** ppValuesParams);

	bool IsGlobalModule() { return m_bIsGlobalModule; };
private:
	//stModuleEventData* m_pModuleEventData;
	CArray<stModuleEventData, stModuleEventData&> m_pModuleEventData;

	CSafeModulePtr m_pModule;
// 	CBLPtr<CBLContext> m_pCtrlObject;
// 	CBLPtr<CBLContext> m_pDerivedObject;

	struct stEvent
	{
		int nFormEvent;
		int nObjEvent;
		bool bIsFunction;
	};
	CArray<stEvent, stEvent&> m_events;

	bool m_bIsGlobalModule;

// 	bool FindEvent(stModuleEventData* pCtrlEvents,
// 					const char* szPrefixName, CBLModule* pFormModule, stEvent& event);
	bool FindEvent(stModuleEventData& pCtrlEvents,
					const char* szPrefixName, stEvent& event);
};