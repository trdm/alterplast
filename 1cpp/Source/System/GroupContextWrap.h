#if !defined(GROUPCONTEXTWRAP_H_INCLUDED)
#define GROUPCONTEXTWRAP_H_INCLUDED

#pragma once

#include "..\MethodsWrapper.h"
#include "ModuleEvents.h"

class CModuleEventsManager //: public CModulesManager
{
 protected:
 	class CComponentClassWrapper;
public:
	typedef const CComponentClass* CConstComponentClassPtr;
	//typedef CComponentClassWrapper CConstComponentClassPtr;

protected:
	class CComponentClassWrapper
	{
	private:
		CSafeContextPtr<CComponentClass> m_pCont;
	public:
		CComponentClassWrapper(const CComponentClass* pCont): m_pCont(pCont) {};
		const CComponentClass* Get() const { return m_pCont; };

		//const CComponentClass* operator->() const { return m_pCont; }
		bool isNull() const { return NULL == m_pCont; }
		bool isValid() const { return !isNull(); }
		operator !() const	{ return isNull(); }

 		int  CallAsFunc(int,class CValue &,class CValue * *);
 		int  CallAsProc(int,class CValue * *);
 		int  FindMethod(char const *)const;
		int  GetNParams(int)const;
		int  HasRetVal(int)const;

		inline int  CallMethodOfContext(bool bFuncFlag, int num,class CValue * pRetValue, class CValue * * ppValue) //const
		{
			if (bFuncFlag)
				return CallAsFunc(num, *pRetValue, ppValue);
			else
				return CallAsProc(num, ppValue);
		}
	};

	class stEventHandlerData
	{
	private:
		typedef CSafeContextPtr<CComponentClass> CContextPtr;
		//typedef CComponentClassWrapper CContextPtr;
		typedef CMap<int,int,int,int> CMethodsMap;
		
		CContextPtr m_pHookCont;
		CMethodsMap m_MethodsNumMap; // соответствие ключ номер метода в модуле -> значение номер метода в обработчике
		CMethodsMap m_MethodsParamsMap;// соответствие ключ номер метода в обработчике -> значение количество параметров в обработчике

	public:
		stEventHandlerData(CConstComponentClassPtr pHookCont) : m_pHookCont(pHookCont)
		{
		}

		CConstComponentClassPtr GetHandler() const { return m_pHookCont; }

		void AddMethodsNum(int iNumInMod, int iNumInContext) { m_MethodsNumMap.SetAt(iNumInMod, iNumInContext); }
		int LookupMethodsNum(int iNumInMod, int& iNumInContext) const
		{ return m_MethodsNumMap.Lookup(iNumInMod, iNumInContext); }

		void AddMethodsParams(int iNumInContext, int nParamsCount) { m_MethodsParamsMap.SetAt(iNumInContext, nParamsCount); }
		int LookupMethodsParams(int iNumInContext, int& nParamsCount) const
		{ return m_MethodsParamsMap.Lookup(iNumInContext, nParamsCount); }
	};
	
	class CModulesManager
	{
		CMap<const CBLModule*, const CBLModule*, const stEventHandlerData*, const stEventHandlerData*> m_ModulesMap;
		bool m_bHaveAddedModules;

		bool FindEventHandlerDataWithoutVerifyUserWrapper(const CBLModule* pMod, stEventHandlerData*& pModData) const;
	public:
		CModulesManager() : m_bHaveAddedModules(false) {};
		~CModulesManager() { m_ModulesMap.RemoveAll(); }
	
		bool IsWrapperExists(const CBLModule* pMod) const;

		bool FindEventHandlerDataAndUserWrapper(const CBLModule* pMod, stEventHandlerData*& pModData, CConstComponentClassPtr& pCont) const;

		CConstComponentClassPtr FindWrapper(const CBLModule* pMod) const;

		bool LookupMethod(const CBLModule* pMod, stEventHandlerData*& pModData, CConstComponentClassPtr& pCont, int iMethNumOfModule, int& iNumMethOfContext) const;

		void Add(const CBLModule* pMod, CConstComponentClassPtr pUserClassCont);

		bool IsAddedModules(void)const
		{ return m_bHaveAddedModules ;}

		void RemoveKey(const CBLModule* pMod) { m_ModulesMap.RemoveKey(pMod); }

		void DestroyWrapper(const CBLModule* pMod);

	};
	
	class CValidator
	{
	public:
		void ValidateParamUserClass(const CValue& UserClassContValue) const;
		void ValidateParamUserClass(CConstComponentClassPtr pUserClassCont, bool bIsHookOfGlobalModule = false) const;
		void ValidateParam(const CGroupContext* pGroupCont, CConstComponentClassPtr pUserClassCont) const;
		void ValidateParam(const CValue& GroupContValue, const CValue& UserClassContValue) const;
	};
	CValidator m_Validator;

public:
	CModuleEventsManager(): m_map(new CModulesManager)// : bHaveSetHooks(false)
	{};

	virtual ~CModuleEventsManager()
	{ };

	bool IsWrapEnable(void)const { return m_map->IsAddedModules(); }

	inline int  FindFunc(const CBLModule* pMod, char const * fName)const
	{
		return FindMethod(pMod, true, fName);
	}
	inline int  FindProc(const CBLModule* pMod, char const * pName,int flag)const
	{
		return FindMethod(pMod, false, pName, flag);
	}

	inline int  CallAsFunc(const CBLModule7* pMod, int num,class CValue & retValue,int argNum,class CValue * * ppValue) const
	{	
		return CallMethod(pMod, true, num, &retValue, argNum, ppValue);
	};

	inline int  CallAsProc(const CBLModule7* pMod, int num,int argNum,class CValue * * ppValue) const
	{
		return CallMethod(pMod, false, num, NULL, argNum, ppValue);
	}

protected:
	int  FindWrapMethod(const CBLModule* pMod, char const * sRusName, char const * sEngName, CConstComponentClassPtr& pCont)const;

	int  FindMethodOfContext(CConstComponentClassPtr pCont, char const * sName, bool bIsGlobalModuleEvent) const;
	int  FindMethod(const CBLModule* pMod, bool bFuncFlag, char const * pName, int flag = 0)const;

	int  CallMethod(const CBLModule7* pMod, bool bFuncFlag, int num,class CValue * pRetValue,int argNum,class CValue * * ppValue) const;

	int VerifyParamsAndRunMethodOfUserClassObject( CComponentClass* pEventHandlerCont, int iMethodNum, int nParamsCount, int iNotUsedParams, CValue &RetVal, CValue ** ppValue, const CString& ErrorMessage );

protected:
	sh_ptr<CModulesManager> m_map;
};
	
class CEventsWrapperManager : public CModuleEventsManager
{

public:
	typedef sh_ptr<CEventsWrapperManager> PGroupContextWrap;
	static PGroupContextWrap& GetWrapper() { 
		if (!wrapperForGroupContext)
			wrapperForGroupContext = PGroupContextWrap(new CEventsWrapperManager);
		return wrapperForGroupContext; 
	} 
private:
	static PGroupContextWrap wrapperForGroupContext;

	static bool bLoadedOptimizedFormEx; 

private:

	void SetWrapperForModule(const CBLModule* pMod, CConstComponentClassPtr pUserClassCont, bool bIsHookOfGlobalModule);
	
	// если ‘орм≈кс загружена, заставим оптимизатор событий FormEx заново перечитать данные методов модул€ формы и глобального модул€
	void RefreshAllEventsDataOfGroupContext_From_FormEx(const CGroupContext* pGroupCont) const;
	void RefreshAllEventsDataOfGlobalModule_From_FormEx(void) const;

public:

	void SetWrapper(const CGroupContext* pGroupCont, CConstComponentClassPtr pUserClassCont);
	void SetWrapper(const CValue& GroupContValue, const CValue& UserClassContValue);

	void SetWrapperForGlobalModule(const CValue& UserClassContValue);
	void SetWrapperForGlobalModule(CConstComponentClassPtr pUserClassCont);

	CConstComponentClassPtr GetWrapper(const CGroupContext* pGroupCont) const;
	CConstComponentClassPtr GetWrapperForGlobalModule() const;

	void DestroyWrapper(const CBLModule* pMod);
	void DestroyWrapper(CGroupContext* pGroupCont);

	int  GetProcInfo(const CBLModule* pMod, int nProc, class CBLProcInfo & cProcInfo); //const;
	
	void CBLProcInfo_CBLProcInfo(void);
	int  CBLProcInfo_GetType(CBLProcInfo* pProcInfo); //const;
	int  CBLProcInfo_GetNParams(CBLProcInfo* pProcInfo); //const;
	int	 CBLProcInfo_IsExported(CBLProcInfo* pProcInfo);
private:
	CBLProcInfo* m_pCBLProcInfo; // указатель на провер€емую структуру

public:
	int RunOriginalEventOfGroupContext(CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);
	int RunOriginalEventOfGlobalModule(const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);

	int RunEventOfGroupContext( CComponentClass* pEventHandlerCont, CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue );
	int RunEventOfGlobalModule(CComponentClass* pEventHandler, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);

private:
	int RunModuleHandler(bool bReturn,  CBLModule7* pThisMod, CComponentClass* pCont, int iNum, class CValue * * params );
public:
	int FindAndRunModuleEventHandler( CBLModule7* pThisMod, char* sRusName, char* sEngName, class CValue * * params );
	int FindAndRunGlobalModuleEventHandler( char* sRusName, char* sEngName, CBLModule7* pThisMod, int iParamsCount, class CValue * * params );

};

#endif // !defined(GROUPCONTEXTWRAP_H_INCLUDED)