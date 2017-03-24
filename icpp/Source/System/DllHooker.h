#if !defined(DLLHOKER_H_INCLUDED)
	#define DLLHOKER_H_INCLUDED

#pragma once

#include "..\MethodsWrapper.h"
#include "ModuleEvents.h"

#include "sh_ptr.hpp"

class CHookGroupContext
{
friend class CWrapper_ExecProc_Guard;

private:

public:
	typedef sh_ptr<CHookGroupContext> PHookGroupContext;

	static PHookGroupContext& GetHooker() {
		if (!hookInstaller)
			hookInstaller = PHookGroupContext(new CHookGroupContext);
		return hookInstaller; };
private:
	static PHookGroupContext hookInstaller;
public:

	static void InitHooks(void);
	static void DestroyHooks(void);

	CHookGroupContext() : m_bEnable(false) {};

	void SetHook(void);
	void DestroyHook(void);

	bool IsEnabled() const { return m_bEnable; };

	static void DisableToFormClose(CBLContext* pCont);
	static void EnableToFormClose(CBLContext* pCont);

	void IfWrapEnableThenRestoreWrapAddressForEvents() const;

	static int FindOriginalEventOfGlobalModule(const CString& sEventName);

	void AddEvent_UnHandledError(void);
	void AddEvent_ReportEventA(void);
	void AddEvent_ReportUserEvent(void);

	void AddEvent_WriteMessageEvent();

	void OnAfterModuleCompile(CBLModule7 *pMod);
private:
	void AddGlobalEvent(void);
	void AddGlobalEvent(int EventIndex);

	static CModuleEventManager& GetModuleEventManager() { return GetHooker()->m_EventManager; };
	
private:
	int  CBLModule_FindFunc(char const * fName); //const;
	int  CBLModule_FindProc(char const * pName,int flag); //const;
	int  CBLModule_CallAsFunc(int num,CValue & retValue,int argNum,CValue * * ppValue);
	int  CBLModule_CallAsProc(int num,int argNum,CValue * * ppValue);
	int  CBLModule_GetProcInfo(int nProc, class CBLProcInfo & cProcInfo); //const;

	//void CGetDoc7_OnCloseDocument(void);
	void CGetDoc7_Destructor(void);

	CBLProcInfo* CBLProcInfo_CBLProcInfo(void);
	int  CBLProcInfo_GetType(void); //const;
	int  CBLProcInfo_GetNParams(void); //const;

	int  CBLModule7_ExecProc(unsigned int i1,int i2,CValue *value1,int i3, CValue * *params,int i4, int i5);	//592
	
	int CBLModule_IsCompiled(void);

	//CValue* CValue_CopyConstructor(const CValue & Value);
	CNumeric* CNumeric_Constructor(void);

	int  CBLProcInfo_GetType(void)const;	//98
	int  CBLProcInfo_IsExported(void);

	void  CMainMessageHandler_WriteError(char const * szErrorMessage, enum MessageMarker marker, char const * szFileName, long param_IntegerNegativeOne, int iErrorRow, int param_IntegerOne);

	//int CAdminService_ReportEventA(CString EventCategory,CString Event,enum EventType EvType,CString comment,CString object,CString ObjDescr);
	//int CAdminService_ReportUserEvent(CString EventCategory, CString Event, enum EventType EvType, CString comment, CValue * value, CString ObjDescr);
	int CAdminService_ReportEventA(CString EventCategory,CString Event,int EvType,CString comment,CString object,CString ObjDescr);
	int CAdminService_ReportUserEvent(CString EventCategory, CString Event, int EvType, CString comment, CValue * value, CString ObjDescr);

	void  CMainMessageHandler_WriteMessage_DoRestructureVeryLongString(char const * str,class CMessageInfo &,int iparam1);

	void VerifyEnabled() const;

	bool InputCausedEventHanlderExistsInModule(CBLModule7* pMod, unsigned int iStringResourceId, LPCSTR sEngName, LPCSTR sRusName);

	// карта модулей, которые запрещено закрывать (например, форма отлаживаемого класса)
	CMapPtrToPtr m_MapNotClosedModules;

	// карта модулей, в которых не будет вызываться метод ПриЗакрытии (например, форма отлаживаемого класса)
	CMapPtrToPtr m_MapDisableOnCloseEvent;

	CModuleEventManager m_EventManager;

	bool m_bEnable;
};

extern void IfWrapEnableThenRestoreWrapAddressForEvents();

typedef int(CBLModule::*PF_FF)(char const * ); //const;
extern CDllMethodWrapper<PF_FF> wrapperFindFunc;

typedef int(CBLModule::*PF_FP)(char const * ,int ); //const;
extern CDllMethodWrapper<PF_FP> wrapperFindProc;

typedef int(CBLModule::*PF_CAP)(int,int,CValue * *);
extern CDllMethodWrapper<PF_CAP> wrapperCallAsProc;

typedef int(CBLModule::*PF_CAF)(int,CValue &,int,CValue * *);
extern CDllMethodWrapper<PF_CAF> wrapperCallAsFunc;

typedef int(CBLModule::*PF_GPI)(int,class CBLProcInfo &); //const;
extern CDllMethodWrapper<PF_GPI> wrapperGetProcInfo;

//typedef void (CGetDoc7::*PF_ONCD)(void);
//extern CDllMethodWrapper<PF_ONCD> wrapperOnCloseDocument;

typedef void (CGetDoc7::*PF_CGetDoc7Destructor)(void);
//extern CDllMethodWrapper<PF_CGetDoc7Destructor> wrapperGetDoc7Destructor;

typedef CBLProcInfo*  (CBLProcInfo::*PF_CBLProcInfo)(void);
//extern CDllMethodWrapper<PF_CBLProcInfo> wrapperCBLProcInfo_CBLProcInfo;

typedef int  (CBLProcInfo::*PF_GetNParams)(void); //const;
extern CDllMethodWrapper<PF_GetNParams> wrapperCBLProcInfo_GetNParams;

typedef int  (CBLProcInfo::*PF_GetType)(void); //const;
//extern CDllMethodWrapper<PF_GetType> wrapperCBLProcInfo_GetType;

typedef int  (CBLProcInfo::*PF_CBLProcInfo_IsExported)(void); //const;
extern CDllMethodWrapper<PF_CBLProcInfo_IsExported> wrapperCBLProcInfo_IsExported;

typedef	int  (CBLModule7::*CBLModule7_PF_ExecProc)(unsigned int,int,CValue *,int,CValue * *,int,int);	//592
//extern CDllMethodWrapper<CBLModule7_PF_ExecProc> wrapperCBLModule7_ExecProc;

typedef	int (CBLModule::*CBLModule_PF_IsCompiled)(void)const;
//extern CDllMethodWrapper<CBLModule_PF_IsCompiled> wrapperCBLModule_IsCompiled;

// typedef	CValue* (CValue::*CValue_PF_CopyConstructor)(const CValue & Value);
// //extern CDllMethodWrapper<CValue_PF_CopyConstructor> wrapperCValue_CopyConstructor;

//CNumeric::CNumeric(void)
typedef	CNumeric* (CNumeric::*CNumeric_PF_Constructor)(void);
extern CDllMethodWrapper<CNumeric_PF_Constructor> wrapperCNumeric_Constructor;

class CWrapper_CBLModule_CallAs_Guard
{
	static bool isNeedAddWrapCallAsMethods;
public:
	CWrapper_CBLModule_CallAs_Guard()
	{
		wrapperCNumeric_Constructor.RestoreWrapAddress(); // устанавливаю перехват
	
		isNeedAddWrapCallAsMethods = true;
	};

	~CWrapper_CBLModule_CallAs_Guard()
	{
		wrapperCNumeric_Constructor.RestoreOrigAddress(); // снимаю перехват
	};

	static void RestoreWrapAddress();
};

#endif // !defined(DLLHOKER_H_INCLUDED)