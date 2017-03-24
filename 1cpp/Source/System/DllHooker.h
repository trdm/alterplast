#if !defined(DLLHOKER_H_INCLUDED)
	#define DLLHOKER_H_INCLUDED

#pragma once

#include "..\MethodsWrapper.h"
#include "ModuleEvents.h"

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
private:
	void AddGlobalEvent(void);
	void AddGlobalEvent(int EventIndex);

	static CModuleEventManager& GetModuleEventManager() { return GetHooker()->m_EventManager; };
	
private:
#ifndef VK_FormEx_Hooks
	int  CBLModule_FindFunc(char const * fName); //const;
	int  CBLModule_FindProc(char const * pName,int flag); //const;
	int  CBLModule_CallAsFunc(int num,class CValue & retValue,int argNum,class CValue * * ppValue);
	int  CBLModule_CallAsProc(int num,int argNum,class CValue * * ppValue);
	int  CBLModule_GetProcInfo(int nProc, class CBLProcInfo & cProcInfo); //const;

	//void CGetDoc7_OnCloseDocument(void);
	void CGetDoc7_Destructor(void);
#endif

	CBLProcInfo* CBLProcInfo_CBLProcInfo(void);
	int  CBLProcInfo_GetType(void); //const;
	int  CBLProcInfo_GetNParams(void); //const;

	int  CBLModule7_ExecProc(unsigned int i1,int i2,class CValue *value1,int i3, class CValue * *params,int i4, int i5);	//592
	
	int CBLModule_IsCompiled(void);

	//CValue* CValue_CopyConstructor(const CValue & Value);
	CNumeric* CNumeric_Constructor(void);

	int  CBLProcInfo_GetType(void)const;	//98
	int  CBLProcInfo_GetNParams(void)const;	//65
	int  CBLProcInfo_IsExported(void);

	void  CMainMessageHandler_WriteError(char const * szErrorMessage, enum MessageMarker marker, char const * szFileName, long param_IntegerNegativeOne, int iErrorRow, int param_IntegerOne);

	//int CAdminService_ReportEventA(class CString EventCategory,class CString Event,enum EventType EvType,class CString comment,class CString object,class CString ObjDescr);
	//int CAdminService_ReportUserEvent(class CString EventCategory, class CString Event, enum EventType EvType, class CString comment, class CValue * value, class CString ObjDescr);
	int CAdminService_ReportEventA(class CString EventCategory,class CString Event,int EvType,class CString comment,class CString object,class CString ObjDescr);
	int CAdminService_ReportUserEvent(class CString EventCategory, class CString Event, int EvType, class CString comment, class CValue * value, class CString ObjDescr);
	
	void  CMainMessageHandler_WriteMessage_DoRestructureVeryLongString(char const * str,class CMessageInfo &,int iparam1);

	void VerifyEnabled() const;

	// карта модулей, которые запрещено закрывать (например, форма отлаживаемого класса)
	CMapPtrToPtr m_MapNotClosedModules;

	// карта модулей, в которых не будет вызываться метод ПриЗакрытии (например, форма отлаживаемого класса)
	CMapPtrToPtr m_MapDisableOnCloseEvent;

	CModuleEventManager m_EventManager;

	bool m_bEnable;
};

#ifndef VK_FormEx_Hooks

typedef int(CBLModule::*PF_FF)(char const * ); //const;
extern CDllMethodWrapper<PF_FF> wrapperFindFunc;

typedef int(CBLModule::*PF_FP)(char const * ,int ); //const;
extern CDllMethodWrapper<PF_FP> wrapperFindProc;

typedef int(CBLModule::*PF_CAP)(int,int,class CValue * *);
extern CDllMethodWrapper<PF_CAP> wrapperCallAsProc;

typedef int(CBLModule::*PF_CAF)(int,class CValue &,int,class CValue * *);
extern CDllMethodWrapper<PF_CAF> wrapperCallAsFunc;

typedef int(CBLModule::*PF_GPI)(int,class CBLProcInfo &); //const;
extern CDllMethodWrapper<PF_GPI> wrapperGetProcInfo;

//typedef void (CGetDoc7::*PF_ONCD)(void);
//extern CDllMethodWrapper<PF_ONCD> wrapperOnCloseDocument;

typedef void (CGetDoc7::*PF_CGetDoc7Destructor)(void);
//extern CDllMethodWrapper<PF_CGetDoc7Destructor> wrapperGetDoc7Destructor;

#endif

typedef CBLProcInfo*  (CBLProcInfo::*PF_CBLProcInfo)(void);
//extern CDllMethodWrapper<PF_CBLProcInfo> wrapperCBLProcInfo_CBLProcInfo;

typedef int  (CBLProcInfo::*PF_GetNParams)(void); //const;
extern CDllMethodWrapper<PF_GetNParams> wrapperCBLProcInfo_GetNParams;

typedef int  (CBLProcInfo::*PF_GetType)(void); //const;
//extern CDllMethodWrapper<PF_GetType> wrapperCBLProcInfo_GetType;

typedef int  (CBLProcInfo::*PF_CBLProcInfo_IsExported)(void); //const;
extern CDllMethodWrapper<PF_CBLProcInfo_IsExported> wrapperCBLProcInfo_IsExported;

typedef	int  (CBLModule7::*CBLModule7_PF_ExecProc)(unsigned int,int,class CValue *,int,class CValue * *,int,int);	//592
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

	static void RestoreWrapAddress()
	{
		if (isNeedAddWrapCallAsMethods)
		{
			wrapperCallAsProc.RestoreWrapAddress();
			wrapperCallAsFunc.RestoreWrapAddress();
			wrapperFindFunc.RestoreWrapAddress();
			wrapperFindProc.RestoreWrapAddress();
			wrapperCBLProcInfo_GetNParams.RestoreWrapAddress();
			wrapperGetProcInfo.RestoreWrapAddress();

			//isNeedAddWrapExecProc = true;
//LogErr("CBLModule_IsCompiled RestoreWrapAddress of ExecProc");
		}
		isNeedAddWrapCallAsMethods = false;

	}
};

#endif // !defined(DLLHOKER_H_INCLUDED)