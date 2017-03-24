#if !defined(GROUPCONTEXTWRAP_H_INCLUDED)
#define GROUPCONTEXTWRAP_H_INCLUDED

#pragma once

#include "..\MethodsWrapper.h"
#include "ModuleEvents.h"

class CModuleEventsManager //: public CModulesManager
{
 protected:
 	//class CUserHandlersManager;
public:
	//typedef IUserClassContext* CConstComponentClassPtr;
	typedef CBLContext* CConstComponentClassPtr;

protected:
	class CUserHandlersManagerImpl;

	// TODO объект часто передаетс€ по значению, нужно исправить
	class CUserHandlersManager
	{
	private:
		boost::shared_ptr<CUserHandlersManagerImpl> m_pImpl;
	public:
		CUserHandlersManager();
		//virtual ~CUserHandlersManager()
		//{
		//	int i = 1;
		//}
		// TODO в методах Add и Get избавитьс€ от iNum
		void Add(int iNum, CConstComponentClassPtr pCont);
		CConstComponentClassPtr Get(int iNum) const;
		
		void reset(); // TODO возможно, нужно использовать Remove дл€ одного обработчика

		bool isNull() const;
		bool isValid() const { return !isNull(); }
		bool operator !() const	{ return isNull(); }

		int FindMethodOfContext(char const * szEventName, bool bIsGlobalModuleEvent);

		int  CallAsFunc(int iMethodNum, CValue & RetValue,CValue ** params);
 		int  CallAsProc(int iMethodNum, CValue ** params);
		int  GetNParams(int iMethodNum) const;
		int  HasRetVal(int iMethodNum) const;
	private:
		//int  FindMethod(char const * szMethodName)const;
	};

	class stEventHandlerData
	{
	public:
		//typedef CSafeContextPtr<IUserClassContext> CUserHandlersManager;
		//typedef CUserHandlersManager CUserHandlersManager;
	private:
		typedef CMap<int,int,int,int> CMethodsMap;
		
		CUserHandlersManager m_pHookCont;
		CMethodsMap m_MethodsNumMap; // соответствие ключ номер метода в модуле -> значение номер метода в обработчике
		CMethodsMap m_MethodsParamsMap;// соответствие ключ номер метода в обработчике -> значение количество параметров в обработчике

	public:
		//stEventHandlerData(CConstComponentClassPtr pHookCont) : m_pHookCont(pHookCont)
		explicit stEventHandlerData(CUserHandlersManager pHookCont) : m_pHookCont(pHookCont)
		{
		}

		//CConstComponentClassPtr GetHandler() const { return m_pHookCont; }
		CUserHandlersManager GetHandler() const { return m_pHookCont; }

		void AddMethodsNum(int iNumInMod, int iNumInContext) { m_MethodsNumMap.SetAt(iNumInMod, iNumInContext); }
		int LookupMethodsNum(int iNumInMod, int& iNumInContext) const
		{ return m_MethodsNumMap.Lookup(iNumInMod, iNumInContext); }

		void AddMethodsParams(int iNumInContext, int nParamsCount) { m_MethodsParamsMap.SetAt(iNumInContext, nParamsCount); }
		int LookupMethodsParams(int iNumInContext, int& nParamsCount) const
		{ return m_MethodsParamsMap.Lookup(iNumInContext, nParamsCount); }
	};

	friend static BOOL CallMethodOfContext(CUserHandlersManager pCont, bool bFuncFlag, int num,CValue * pRetValue, CValue * * ppValue);
	
	class CModulesManager
	{
		CMap<const CBLModule*, const CBLModule*, const stEventHandlerData*, const stEventHandlerData*> m_ModulesMap;
		bool m_bHaveAddedModules;

		bool FindEventHandlerDataWithoutVerifyUserWrapper(const CBLModule* pMod, const stEventHandlerData*& pModData) const;
	public:
		CModulesManager() : m_bHaveAddedModules(false) {};
		~CModulesManager() { m_ModulesMap.RemoveAll(); }
	
		bool IsWrapperExists(const CBLModule* pMod) const;

		//bool FindEventHandlerDataAndUserWrapper(const CBLModule* pMod, const stEventHandlerData*& pModData, CConstComponentClassPtr& pCont) const;
		bool FindEventHandlerDataAndUserWrapper(const CBLModule* pMod, const stEventHandlerData*& pModData, CUserHandlersManager& pCont) const;

		//CConstComponentClassPtr FindWrapper(const CBLModule* pMod) const;
		CUserHandlersManager FindWrapper(const CBLModule* pMod) const;

		//bool LookupMethod(const CBLModule* pMod, const stEventHandlerData*& pModData, CConstComponentClassPtr& pCont, int iMethNumOfModule, int& iNumMethOfContext) const;
		bool LookupMethod(const CBLModule* pMod, const stEventHandlerData*& pModData, CUserHandlersManager& pCont, int iMethNumOfModule, int& iNumMethOfContext) const;

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

	inline int  CallAsFunc(const CBLModule7* pMod, int num,CValue & retValue,int argNum,CValue * * ppValue) const
	{	
		return CallMethod(pMod, true, num, &retValue, argNum, ppValue);
	};

	inline int  CallAsProc(const CBLModule7* pMod, int num,int argNum,CValue * * ppValue) const
	{
		return CallMethod(pMod, false, num, NULL, argNum, ppValue);
	}

protected:
	//int  FindWrapMethod(const CBLModule* pMod, char const * sRusName, char const * sEngName, CConstComponentClassPtr& pCont)const;
	int  FindWrapMethod(const CBLModule* pMod, char const * sRusName, char const * sEngName, CUserHandlersManager& pCont)const;

	////int  FindMethodOfContext(CConstComponentClassPtr pCont, char const * sName, bool bIsGlobalModuleEvent) const;
	//int  FindMethodOfContext(CUserHandlersManager& pCont, char const * sName, bool bIsGlobalModuleEvent) const;
	
	int  FindMethod(const CBLModule* pMod, bool bFuncFlag, char const * pName, int flag = 0)const;

	int  CallMethod(const CBLModule7* pMod, bool bFuncFlag, int num,CValue * pRetValue,int argNum,CValue * * ppValue) const;

	int VerifyParamsAndRunMethodOfUserClassObject( CConstComponentClassPtr pEventHandlerCont, int iMethodNum, int nParamsCount, int iNotUsedParams, CValue &RetVal, CValue ** ppValue, const CString& ErrorMessage );

	int RunModuleHandler(bool bReturn,  CBLModule7* pThisMod, CUserHandlersManager pCont, int iNum, CValue * * params );

protected:
	boost::shared_ptr<CModulesManager> m_map; //sh_ptr<CModulesManager> m_map;
};
	
class CEventsWrapperManager : public CModuleEventsManager, boost::noncopyable
{
private:
	CEventsWrapperManager() {};
public:
	typedef boost::shared_ptr<CEventsWrapperManager> PGroupContextWrap; //typedef sh_ptr<CEventsWrapperManager> PGroupContextWrap;
	static PGroupContextWrap& GetWrapper() { 
		if (!wrapperForGroupContext)
			wrapperForGroupContext = PGroupContextWrap(new CEventsWrapperManager);
		return wrapperForGroupContext; 
	} 

	void SetWrapper(const CValue& GroupContValue, const CValue& UserClassContValue);

	void SetWrapperForGlobalModule(const CValue& UserClassContValue);

	//CConstComponentClassPtr GetWrapper(const CGroupContext* pGroupCont) const;
	CUserHandlersManager GetWrapper(const CGroupContext* pGroupCont) const;
	//CConstComponentClassPtr GetWrapperForGlobalModule() const;
	CUserHandlersManager GetWrapperForGlobalModule() const;

	void DestroyWrapper(const CBLModule* pMod);
	void DestroyWrapper(CGroupContext* pGroupCont);

	int  GetProcInfo(const CBLModule* pMod, int nProc, class CBLProcInfo & cProcInfo); //const;
	
	void CBLProcInfo_CBLProcInfo(void);
	int  CBLProcInfo_GetType(CBLProcInfo* pProcInfo); //const;
	int  CBLProcInfo_GetNParams(CBLProcInfo* pProcInfo); //const;
	int	 CBLProcInfo_IsExported(CBLProcInfo* pProcInfo);

	int RunOriginalEventOfGroupContext(CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);
	int RunOriginalEventOfGlobalModule(const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);

	int RunEventOfGroupContext( CConstComponentClassPtr pEventHandlerCont, CGroupContext* pGroupContext, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue );
	int RunEventOfGlobalModule(CConstComponentClassPtr pEventHandler, const CString& strEventName, int nParamsCount, CValue& RetVal, CValue **ppValue);

	int FindAndRunModuleEventHandler( CBLModule7* pThisMod, char* sRusName, char* sEngName, CValue * * params );
	int FindAndRunGlobalModuleEventHandler( LPCSTR sRusName, LPCSTR sEngName, CBLModule7* pThisMod, int iParamsCount, CValue * * params );

private:
	static PGroupContextWrap wrapperForGroupContext;

	static bool bLoadedOptimizedFormEx; 

	void SetWrapperForModule(const CBLModule* pMod, CConstComponentClassPtr pUserClassCont, bool bIsHookOfGlobalModule);

	// если ‘орм≈кс загружена, заставим оптимизатор событий FormEx заново перечитать данные методов модул€ формы и глобального модул€
	void RefreshAllEventsDataOfGroupContext_From_FormEx(const CGroupContext* pGroupCont) const;
	void RefreshAllEventsDataOfGlobalModule_From_FormEx(void) const;

	CBLProcInfo* m_pCBLProcInfo; // указатель на провер€емую структуру

	////int RunModuleHandler(bool bReturn,  CBLModule7* pThisMod, CConstComponentClassPtr pCont, int iNum, CValue * * params );
	//int RunModuleHandler(bool bReturn,  CBLModule7* pThisMod, CUserHandlersManager pCont, int iNum, CValue * * params );

	void SetWrapper(const CGroupContext* pGroupCont, CConstComponentClassPtr pUserClassCont);
	void SetWrapperForGlobalModule(CConstComponentClassPtr pUserClassCont);

};

#endif // !defined(GROUPCONTEXTWRAP_H_INCLUDED)