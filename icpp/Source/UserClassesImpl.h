#if !defined(_USER_CLASSES_IMPL_H__INCLUDED_)
#define _USER_CLASSES_IMPL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CastContext.h"
#include "ComponentClass.h"
#include "SimpleCollection.h"

typedef boost::shared_ptr<CValue> CValue_SharedPtr;
typedef CArray<CValue_SharedPtr, CValue_SharedPtr&> ARRAY_VAL; //CArray<CValue*, CValue*> ARRAY_VAL;

class CDynamicPropertyManager;
class CBLSimpleVector;

// класс С++ для работы с пользовательскими классами (КОП)
class CComponentClassImpl : public IContextTree, public IGetModule, public IV7SerializeToString, 
	boost::noncopyable //, public Utils1cpp::IQueryInterface_Loader_Publisher
{
	friend class CImplicitParams;
	friend class CComponentClass;

	//DECLARE_DYNCREATE(CComponentClass);
	//boost::shared_ptr<CComponentClass> m_pOwner;
	CComponentClass* m_pOwner;

protected:
	CComponentClassImpl(CComponentClass* pOwner);
public:
	virtual ~CComponentClassImpl();

	//int GetBaseClass(const CValue &NameClass, CValue & rValue) const;
	int GetBaseClass(const CString & NameClass, CValue & rValue) const;
	BOOL GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const;
	//template <class T>
	//BOOL GetBaseClass(T** ppT) const
	//{
	//	for (CONST_ITER_CONT iter = vecOfBaseCont.begin(); iter != vecOfBaseCont.end(); ++iter)
	//	{
	//		CBLContext *pCont = *iter;
	//		if (GetBaseClassFromContext(pCont, ppT))
	//			return TRUE;
	//		if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CComponentClass))
	//		{
	//			if (static_cast<CComponentClass*>(pCont)->GetBaseClass(ppT))
	//				return TRUE;
	//		}
	//		else
	//		{
	//			T* p = dynamic_cast<T*>(pCont);
	//			if (p)
	//			{
	//				*ppT = p;
	//				return TRUE;
	//			}
	//		}
	//	}
	//	return FALSE;
	//};
	//boost::shared_ptr<CIUserClassContext::CBaseClassIterator> GetBaseClassesIterator() const;

	// IContextTree

	// Получить количество базовых объектов.
	virtual DWORD GetBaseObjCount();
	// Получить имя базового объекта по индексу.
	virtual CString GetBaseObjName(DWORD idx);
	// Получить базовый объект по индексу.
	virtual CBLContext* GetBaseObj(DWORD idx);
	// Для upcast'а
	virtual CBLContext* GetDerivedObj();

	// end IContextTree

	// IGetModule

	CBLModule * GetModule() const;
	CBLModule * GetHostModule() const;

	// end IGetModule

	// интерфейс Utils1cpp::IQueryInterface_Loader_Publisher

		virtual BOOL IsLoader() const;
		virtual BOOL IsPublisher() const;

	// end Utils1cpp::IQueryInterface_Loader_Publisher

	// интерфейс IResultLoader
		virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw();

		virtual HRESULT ldrDone() throw();

		virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw();

		virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw();

		virtual LPCSTR GetLastError() throw();
	// end IResultLoader

	BOOL SendMessageOwnMod( CValue &rValue, CValue ** ppValue );
	void GetPathName( CValue &rValue );
	void GetEnvContext( CValue &rValue );
	void GetParamsList( CValue &rValue, CValue ** ppValue );
	BOOL SetOnIndexParams( CValue &rValue, CValue * * ppValue );
	void ThrowException(CValue ** ppValue);
	void AddDynamicProperty(CValue ** ppValue);

	virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	//virtual int  GetDestroyUnRefd(void)const;	//1657
	//virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	virtual void  IncrRef(void);	//2668
	// virtual void  InitObject(CType const &);	//2721 // artbear не нужно - там стоит тупой вызов метода из базового класса
	virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(CString &);	//3295
	virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,CValue const &);	//3694

	static CComponentClass* GetComponentClassCurExModule(CBLModule* pModWrapper = NULL);

	static bool StillInitInstanceComp() {return pCreatedUserClass != NULL;}

	static void ClearOptimizationData(void);

	// метод для вызова пользовательского конструктора с несколькими параметрами
	// используется в методе ФабрикаОбъектов::Новый()
	void CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const;

	//class CComponentClassImpl::InfoUnlimitsParams;
//1
//	// Увязывает указатели на экземпляры классов с указателями на список значений парметров
//	// полученных вместо точек в методе (...)
//	class InfoUnlimitsParams : boost::noncopyable
//	{
//	public:
//		//typedef CInfoUnlimitsParamsPtr CPtr;
//		typedef boost::shared_ptr<InfoUnlimitsParams> CPtr;
//
//		static CPtr Create() { return CPtr(new InfoUnlimitsParams); };
//		~InfoUnlimitsParams()
//		{
//			int i  = 1;
//		};
//
//		void Setup(int nRes, int nNParams);
//		void RemoveAllFromValueList();
//
//		//typedef CBLPtr<CBLContext> CListContextPtr;
//		typedef CSafeContextPtrDef CListContextPtr;
//		CListContextPtr GetValueListContext() const { return m_pValueListContext; };
//		//CBLPtr<CBLContext> GetValueListContext() { return m_pValueListContext; };
//		int GetCountOfAllParams() const { return m_nNumAllParams; };
//		int GetCountOfExplicitParams() const { return m_nNumExplicitParams; };
//
//		std::vector<CValue*> vecPointOfPars;
//
//	private:
//		InfoUnlimitsParams(): m_pValueListContext("ValueList")
//		{
//			int i = 1;
//		};
//
//		CListContextPtr m_pValueListContext; //CBLContext* m_pValueListContext;
//		int         m_nNumAllParams;
//		int         m_nNumExplicitParams;
//	};
//	//typedef boost::shared_ptr<InfoUnlimitsParams> CInfoUnlimitsParamsPtr;
//	typedef InfoUnlimitsParams::CPtr CInfoUnlimitsParamsPtr;
//
//	typedef CIStringMap<CInfoUnlimitsParamsPtr, CInfoUnlimitsParamsPtr&> CMapOfInfoUnlimitsParamsPtr;
	CMapOfInfoUnlimitsParamsPtr& GetBindInstanceValueList() // const
	{ return m_BindInstanceValueList; }

	// флаг создания без вызова дефолтного конструктора, даже если он есть в модуле
	// удобно для использования с методом ФабрикаОбъектов::Новый()
	//
	static void SetFlagCreateWithoutConstructor(void) { bFlagCreateWithoutConstructor = true; }
	static void ClearFlagCreateWithoutConstructor(void) { bFlagCreateWithoutConstructor = false; }
	static bool IsCreateWithoutConstructor() { return bFlagCreateWithoutConstructor; }

	CParamDefs::CArrayOfMethodsParamData_PTR GetMethodParamData(int iMethodNum) const;

	//class IV7SerializeToString
	virtual BOOL LoadFromString(LPCSTR szSource);

private:
	enum {
		funcGetBaseClass,
		procAssignFriendModule,

		// этот функционал закрыт общее решение artbear
		// TODO возможно, в дальнейшем он должен быть открыт внутри класса, а снаружи невидим
		//
		//procAddBaseClass,

		funcSendMessageOwnMod,
		funcGetPathName,

		funcGetEnvContext,
		funcGetParamsList,

		funcReplaceInstBaseClasses,
		funcReplaceInstBaseClasses_SYN1,

		funcSetOnIndexParams,
		procOnOpen,
		procThrow,
		funcIsObject,
		procAddDynamicProperty,

		lastMethod
	};
	//CComponentClass* GetOwner() const { return m_pOwner.get(); };
	CComponentClass* GetOwner() const { return m_pOwner; };

	static void GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng, bool bAddKind = true);
	static void GetFullTypeName(CBLContext *pCont, CString& rus, CString& eng,
		CString& rusKind, bool bAddKind = true); // artbear

	inline int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue);

	int IsObject(const CValue &NameClass, CValue &rValue) const;

	void InvokeConstructor();

	virtual CBLModule* Init(const char* file);
	virtual CComponentClassModulePtr FirstInit(const CString& FullFileName);

	// проверить, является ли текущий исполняемый модуль модулем базового класса для текущего класса
	// используется для исключения рекурсии
	CComponentClass* VerifyCurrentExecuteContextIsBaseClassForThisClass() const;

	// Выполняет инициализацию экземпляра класса на основе информации полученной
	// из файла составленного пользователем
	void InitInstanceComp(void);

	friend class CInitGuard;
	void OnStartInit(void);
	void OnEndInit(bool bIsBadExit = false);
	void FinishInit(void);

	bool AddBaseClass(const CString& name);
	void ClearAllBasesClass();

	void ReplaceContextInModulesOfBaseClasses(CComponentClass* pMainCont =  NULL);

	// найти метод класса по названию
	// сначала ищется в модуле класса на случай, если задан без Экспорт
	// а затем в экспортируемых методах класса, затем в наследниках
	//
	// сначала ищется русское имя, а затем английское
	//
	static int FindMethod(CBLContext* pContext, CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng);

	// тоже самое, но для текущего класса
	inline int FindMethod(CBLModule *pMod, LPCTSTR sNameRus, LPCTSTR sNameEng) const
	{
		//return CComponentClassImpl::FindMethod(GetOwner(), pMod, sNameRus, sNameEng);
		return FindMethod(GetOwner(), pMod, sNameRus, sNameEng);
	};

	BOOL GetParamDefValueFromClassDefinitionOrModule(int iMethodNum,int iParamNum,CValue * pDefValue)const;
	BOOL GetParamDefValueFromClassModule(int iMethodNum,int iParamNum,CValue * pDefValue)const;

	static bool bFlagCreateWithoutConstructor;

	// карта методов, используется для оптимизации
	static class CMapDefMethod MapDef;

	bool m_bDonNotCallDestructor;
	void SetFlagDonNotCallDestructor() { m_bDonNotCallDestructor = true; };

	//CBLModule *pMod;
	CSafeModulePtr m_pMod;
	//CComponentClassModulePtr m_pModLocale;
	bool m_bInsideDestructor;

	//mutable CMapOfInfoUnlimitsParamsPtr m_BindInstanceValueList; //CMapStringToPtr   m_BindInstanceValueList;
	CMapOfInfoUnlimitsParamsPtr m_BindInstanceValueList; //CMapStringToPtr   m_BindInstanceValueList;

	static class CObjID ObjID;

	//CBLModule  *m_pCurModule;
	CSafeModulePtr m_pCurModule; // модуль, в котором объект был создан

	CString m_NameOfInstance;

	std::vector<CSafeContextPtr<CBLContext> > vecOfBaseCont;
	typedef std::vector<CSafeContextPtr<CBLContext> >::const_iterator CONST_ITER_CONT;
	typedef std::vector<CSafeContextPtr<CBLContext> >::iterator ITER_CONT;

	//class CParamDefs *defFnNames;
	//CParamDefsWithRefCountPtr defFnNames;
	typedef boost::shared_ptr<CMyParamDefs> CMyParamDefsWithRefCountPtr;
	CMyParamDefsWithRefCountPtr defFnNames;

	struct MethParamsDef
	{
		int nNumberOfMeth;
		int nRealNumberOfMeth;
		CBLContext* pContext;
	};
	mutable MethParamsDef m_MethParamsDef;
	//unsigned char *pPoint;

	int CallAsMethod(int iMethNum, CValue * *ppValue, bool bHasRetVal = false, CValue * pRetValue = NULL);

	bool m_isCreateOptimizeMode;

	// Карта с указателями на модули с их реальными контекстами
	// Ключ - указатель на модуль, Значение - указателя на контекст CComponentClass
	typedef CMap<CBLModule*, CBLModule*, CComponentClass*, CComponentClass*> CMapOfModulesAndComponentClasses;
	static CMapOfModulesAndComponentClasses m_MapOfModulesAndComp; //CMapPtrToPtr m_MapOfModulesAndComp;

	// Указатель на текущий создаваемый объекта КОП
	// Необходим для установки его как контекста во всех модулях иерархии
	static CComponentClass* pCreatedUserClass;
	/*
	//public:
	// Карта с указателями на контексты с их модулями
	// Ключ - указатель на на контекст CComponentClass, Значение - указатель на модуль
	//	pMod->GetInternalData()->pGeneralContext
	// pGeneralContext это будет ключ, а pMod - значение
	// нужно для правильного получения контекстов классов и их предков
	//static CMap<CBLContext*, CBLContext*, CBLModule*, CBLModule*> m_MapOfContextOnModules;
	typedef vector<CSafeModulePtr> CVectorModulePtr;
	typedef CMap<CComponentClass*, CComponentClass*, CVectorModulePtr*, CVectorModulePtr*> CMapOfContextToModules;
	//private:
	static CMapOfContextToModules m_mapOfContextToModules; // карта связанных модулей

	//void LinkContext(CBLModule* pMod);
	//void UnlinkContext();
	//void ClearModulePtr(CSafeModulePtr* ptr);

	void LinkContext(const CSafeModulePtr& pMod);
	void UnlinkContext();
	static void ClearContextFromModule(const CSafeModulePtr& ptr);
	*/
	// работа с отладкой
	class CDebug
	{
	public:
		static void SetDebugObject(CComponentClass *pComp);
		static void ClearDebugObject();

		bool IsDebug() const
		{ return NULL != pDebugContext; }

		operator bool() const
		{ return IsDebug(); }

		bool operator !() const
		{ return !IsDebug(); }

		void Release(void);

		void OpenDebugForm(CComponentClass* pComponent);

		//void AssignDebugModule(CBLContext* pComponent, CBLModule*& pMod);
		void AssignDebugModule(CBLContext* pComponent, CSafeModulePtr& pMod);
	private:
		CSafeContextPtr<CBLContext> pDebugContext;

		static CComponentClass* m_CurDebugObject;

	};

	CDebug m_DebugInfo;

	CString GetKeyOfUserClassMethod(LPCSTR strMethodName) const
	{
		return ISetOfHierarchy::GetKeyOfUserClassMethod(m_NameOfInstance, strMethodName);
	};

	// CComponentClassImpl::CImplicitParams - работа с неопределенным числом параметров
	class CImplicitParams : boost::noncopyable
	{
	private:
		CString m_strKeyOfUserClassMethod;
		const CComponentClassImpl *m_pCurrentComponentClass;

		//CString m_strKeyOfUserClassMethod_Implicit;
		static int m_CountParametersInCallMethod;

	public:
		static int CountParametersInCallMethod() { return m_CountParametersInCallMethod; }
		static void CountParametersInCallMethod(int val) { m_CountParametersInCallMethod = val; }

		CString GetKeyOfUserClassMethod() const { return m_strKeyOfUserClassMethod; }
		void SetKeyOfUserClassMethod(const CComponentClassImpl *pComponentClass, const CString& strNewValue)
		{ 
			m_strKeyOfUserClassMethod = strNewValue; 
			m_pCurrentComponentClass = pComponentClass;
		}

	public:
		CImplicitParams();

		////CBLPtr<CBLContext> GetImplicitParamsList(CComponentClass *pComp, const char* lpcsNameMeth);
		//InfoUnlimitsParams::CListContextPtr GetImplicitParamsList(CComponentClassImpl *pComp, const char* lpcsNameMeth);
		boost::shared_ptr<CValueListContextFriend> GetImplicitParamsList(CComponentClassImpl *pComp, const char* lpcsNameMeth);
		
		static boost::shared_ptr<CValueListContextFriend> Clone_CValueListContextFriend(CValueListContextFriend* pSrcList); // TODO перенести в CValueListContextFriend

		bool SetImplicitParams(CComponentClassImpl *pComp, CValue **ppPar, int nNumMeth);
		int SetImplicitParamOfInd(CComponentClassImpl *pComp, const char* lpcsNameMeth, int nIndex, CValue* pVal);

		//int GetNParams(const CComponentClass *pComp, int nNumMeth, int nNParams);
		int GetNParams(CComponentClassImpl *pComp, int nNumMeth, int nNParams);

		InfoUnlimitsParams::CPtr CreateInfoAndSetIntoUserClass( const CComponentClassImpl * pComp );
		void CleanSavedParams( const CComponentClassImpl * pComp );

	private:
		//BOOL LookupInBindInstanceValueList(const CComponentClass* pComp, const CString& m_strClassAndMethCurrent, InfoUnlimitsParams::CPtr& pInfo) const;
		BOOL LookupInBindInstanceValueList(CComponentClassImpl* pComp, const CString& m_strClassAndMethCurrent, InfoUnlimitsParams::CPtr& pInfo);// const;

		InfoUnlimitsParams::CPtr GetImplicitParamsInfo(CComponentClassImpl *pComp, const char* lpcsNameMeth); // const;

		void VerifyCComponentClassParam( CComponentClassImpl * pComponentClass )
		{
			assert(m_pCurrentComponentClass == pComponentClass);
			//if (m_pCurrentComponentClass != pComponentClass)
			//	RuntimeError("Ошибка логики");
		}
	};

	//static CImplicitParams* pImplicitParams;
	//mutable 
	boost::scoped_ptr<CImplicitParams> pImplicitParams;

	class CParamByValueGuard
	{
		CString m_strCurrenMethodKey;
		CComponentClassImpl* m_pComponentClass;
		int m_nNumMeth;
		CValue** m_ppParam;
		std::vector<CValue_SharedPtr> m_SavedVector;

		void Save();
		void Restore();

		//void VerifyCComponentClassParam( CComponentClassImpl * pComponentClass );
	public:
		CParamByValueGuard(const CString& strCurrenMethodKey, CComponentClassImpl* pComponentClass, int nNumMeth, CValue** ppParam) :
		  m_strCurrenMethodKey(strCurrenMethodKey), m_pComponentClass(pComponentClass), m_nNumMeth(nNumMeth), m_ppParam(ppParam)
		  { 
			  assert(m_pComponentClass == pComponentClass);
			  assert(0 != m_strCurrenMethodKey.GetLength());
			  Save(); 
		  };

		  ~CParamByValueGuard() { Restore(); };
	};

	class CParamsReceivedByValueManager : boost::noncopyable
	{
		CString m_strCurrenMethodKey;
		const CComponentClassImpl *m_pCurrentComponentClass;
	public:
		//CParamsReceivedByValueManager();

		CString GetCurrenMethodKey() const { return m_strCurrenMethodKey; }
		void SetCurrenMethodKey(const CComponentClassImpl *pComponentClass, const CString& strNewValue) 
		{ 
			m_strCurrenMethodKey = strNewValue; 
			m_pCurrentComponentClass = pComponentClass;
		}

		//// Сохраняет параметры, которые переданы в метод по значению перед вызовом
		//bool SaveParams(CComponentClassImpl* pComponentClass, int nNumMeth, CValue** ppParam, ARRAY_VAL *arrValOfMeth);

		//// Восстанавливает параметры, которые переданы в метод по значению после вызова
		//void RestoreParams(CComponentClassImpl *pComponentClass, int nNumMeth, CValue **ppParam, ARRAY_VAL *arrValOfMeth);

		//void VerifyCComponentClassParam( CComponentClassImpl * pComponentClass )
		//{
		//	assert(m_pCurrentComponentClass == pComponentClass);
		//	//if (m_pCurrentComponentClass != pComponentClass)
		//	//	RuntimeError("Ошибка логики");
		//};

		boost::shared_ptr<CParamByValueGuard> GetParamsByValueGuard(const CString& m_strCurrenMethodKey, CComponentClassImpl* pComponentClass, int nNumMeth, CValue** ppParam);
	};
	boost::scoped_ptr<CParamsReceivedByValueManager> m_pParamsByValueManager;

	bool IsMethodFromBaseContexts(const int iMethodNum) const { return iMethodNum >= defFnNames->Size(); };
	CSafeContextPtr<CBLContext> FindBaseContextByMethodNumber(const int iMethNum, int& iMethNumInSearchedContext) const;

	bool IsPropertyFromBaseContexts(const int iPropNum) const { return iPropNum >= m_pMod->GetNStaticVars(); };
	CSafeContextPtr<CBLContext> FindBaseContextByPropertyNumber(const int iPropNum, int& iPropNumInSearchedContext) const;

	boost::shared_ptr<CDynamicPropertyManager> m_pDynamicPropertyManager;

	void InitMethodsDefs();
	CComponentClassImpl* GetRightClassPtrIfInsideObjectDestructor() const;

	CMyParamDefsWithRefCountPtr GetDefFnNamesPtr(void) const
	{ return defFnNames; }

	// TODO нужен рефакторинг дублирования с CMetaDataOfClasses::m_ValParamInMeth
	class CMethodsMetadataManager
	{
		// Ключ - имя класса плюс имя метода в котором присутствует параметр передаваемый по значению
		CParamDefs::CMapMethodsNameToArrayOfMethodsParamData m_map;
	public:
		typedef boost::shared_ptr<CMethodsMetadataManager> CPtr;
		void SetAt(LPCSTR strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR param)
			{ 	m_map[strKeyOfUserClassMethod] = param; }
		
		bool Lookup(LPCSTR strKeyOfUserClassMethod, CParamDefs::CArrayOfMethodsParamData_PTR& param) const
			{
				if (m_map.Lookup(strKeyOfUserClassMethod, param))
					return true;
				else
					return false;
			}

		bool IsExists(LPCSTR strKeyOfUserClassMethod) const 
			{ CParamDefs::CArrayOfMethodsParamData_PTR param; return Lookup(strKeyOfUserClassMethod, param); }

		void Remove(LPCSTR strKeyOfUserClassMethod)
			{ m_map.RemoveKey(strKeyOfUserClassMethod); }

		void RemoveAll()
			{ m_map.RemoveAll(); }
	};

	CMethodsMetadataManager::CPtr m_pMethodsWithParamsByValueManager; // ключ имя метода

	// TODO фактически нужно добавить к CMapDefMethod !
	static CIStringMap<CMethodsMetadataManager::CPtr, CMethodsMetadataManager::CPtr&> mapOfMethodsWithParamsByValueManagers;

	bool IsMethodHaveParamsByValueInOwnModule( const CBLProcInfo &procInfo) const;
	void SetMethodsMetaInfoForParamsByValue( const CString& strKeyOfUserClassMethod, const CBLProcInfo &procInfo);

	void FirstInitUserMethods( CComponentClassModulePtr& pModLocale);
	int GetIsPropAccess(bool bIsReadable, int iPropNum) const;

	bool IsFullyCreated() const { return defFnNames; };

	//CParamDefsWithRefCountPtr m_defFnNames_LoaderUnloader;
	void FirstInitUserMethodsFor_Loader_Publisher_Support();

	boost::shared_ptr<CBLSimpleVector> m_pCBLSimpleVector;
	int m_iColCount;
};


#endif // _USER_CLASSES_IMPL_H__INCLUDED_

