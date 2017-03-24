  // ComponentClass.h: interface for the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_)
#define AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

#include "MapDefMethod.h"

#include "CastContext.h"

#include "SafeContext.h"
#include "V7SerializeToString.h"

#include "Interfaces/ICloneCreated.h"

// интерфейс IGetModule используется в КЗК-2 от АльФ-а - менять интерфейс нельзя
// аналогично он реализован в 1С++ 2.5.0.6
class IGetModule 
{
public:
	virtual CBLModule * GetModule() const = 0;
	virtual CBLModule * GetHostModule() const = 0;
};

namespace Utils1cpp{

	struct IQueryInterface_Loader_Publisher
	{
		virtual BOOL IsLoader() const = 0;
		virtual BOOL IsPublisher() const = 0;
	};

};

// в дальнейшем желательно данные интерфейсы не менять, а только добавлять новые интерфейсы
// т.к. этот интерфейс используется, например, в КЗК-2 при шифровке/расшифровке классов 1С++
//
class IUserClassContext : public CBLContext, public IContextTree, public IGetModule, 
			public IV7SerializeToString, public Utils1cpp::ICloneCreated
			,public Utils1cpp::IQueryInterface_Loader_Publisher, public IResultLoader
{
protected:
	IUserClassContext() : CBLContext(1) {};
public:
	virtual ~IUserClassContext() {};

	//virtual int GetBaseClass(const CValue &NameClass, CValue & rValue) const = 0;
	virtual int GetBaseClass(const CString & NameClass, CValue & rValue) const = 0;
	virtual BOOL GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const = 0;

	template <class T>
	BOOL GetBaseClass(T** ppT) const
	{
		return !CastContext::Dynamic(const_cast<IUserClassContext*>(this), *ppT) ? FALSE :  TRUE;
	};

	//virtual CBLModule * GetModule() const = 0;
	//virtual CBLModule * GetHostModule() const = 0;

	virtual BOOL SendMessageOwnMod( CValue &rValue, CValue ** ppValue ) = 0;
	virtual void GetPathName( CValue &rValue ) = 0;
	virtual void GetEnvContext( CValue &rValue ) = 0;
	virtual void GetParamsList( CValue &rValue, CValue ** ppValue ) = 0;
	virtual BOOL SetOnIndexParams( CValue &rValue, CValue * * ppValue ) = 0;
	virtual void ThrowException(CValue ** ppValue) = 0;
	virtual void AddDynamicProperty(CValue ** ppValue) = 0;
	virtual int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue) = 0;
	virtual int IsObject(const CValue &NameClass, CValue &rValue) const = 0;

	virtual int  CallAsFunc(int,CValue &,CValue * *) = 0;	//935
	virtual int  CallAsProc(int,CValue * *) = 0;	//937
	virtual void  DecrRef(void) = 0;	//1086
	virtual int  FindMethod(char const *)const = 0;	//1366
	virtual int  FindProp(char const *)const = 0;	//1369
	virtual char const *  GetCode(void)const = 0;	//1544
	//virtual int  GetDestroyUnRefd(void)const;	//1657
	//virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const = 0;	//1804
	virtual class CBLContextInternalData *  GetInternalData(void) = 0;	//1826
	virtual char const *  GetMethodName(int,int)const = 0;	//1964
	virtual int  GetNMethods(void)const = 0;	//2005
	virtual int  GetNParams(int)const = 0;	//2008
	virtual int  GetNProps(void)const = 0;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const = 0;	//2122
	virtual char const *  GetPropName(int,int)const = 0;	//2187
	virtual int  GetPropVal(int,CValue &)const = 0;	//2221
	virtual long  GetTypeID(void)const = 0;	//2505
	virtual char const *  GetTypeString(void)const = 0;	//2513
	virtual CType   GetValueType(void)const = 0;	//2547
	virtual int  HasRetVal(int)const = 0;	//2657
	virtual void  IncrRef(void) = 0;	//2668
	// virtual void  InitObject(CType const &);	//2721 // artbear не нужно - там стоит тупой вызов метода из базового класса
	virtual void  InitObject(char const *) = 0;	//2722
	virtual int  IsExactValue(void)const = 0;	//2808
	virtual int  IsOleContext(void)const = 0;	//2846
	virtual int  IsPropReadable(int)const = 0;	//2863
	virtual int  IsPropWritable(int)const = 0;	//2865
	virtual int  IsSerializable(void) = 0;	//2874
	virtual int  SaveToString(CString &) = 0;	//3295
	virtual void  SelectByID(class CObjID,long) = 0;	//3350
	virtual int  SetPropVal(int,CValue const &) = 0;	//3694

	// метод для вызова пользовательского конструктора с несколькими параметрами
	// используется в методе ФабрикаОбъектов::Новый()
	virtual void CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const = 0;
};

// класс С++ для работы с пользовательскими классами (КОП)
class CComponentClass : public IUserClassContext
{
	friend class CComponentClassImpl;
	
	boost::scoped_ptr<CComponentClassImpl> m_pImpl;

DECLARE_DYNCREATE(CComponentClass);

protected:
	CComponentClass();
public:
	virtual ~CComponentClass();

	//int GetBaseClass(const CValue &NameClass, CValue & rValue) const;
	virtual int GetBaseClass(const CString & NameClass, CValue & rValue) const;
	virtual BOOL GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const;
	
	template <class T>
	BOOL GetBaseClass(T** ppT) const
	{
		return IUserClassContext::GetBaseClass(ppT);
	};

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

	// интерфейс Utils1cpp::ICloneCreated

	virtual CBLContext* Clone(LPCSTR szClassName) const;

	// end Utils1cpp::ICloneCreated

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
	int ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue);
	int IsObject(const CValue &NameClass, CValue &rValue) const;

	virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544

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

	static bool StillInitInstanceComp();

	static void ClearOptimizationData(void);

	// метод для вызова пользовательского конструктора с несколькими параметрами
	// используется в методе ФабрикаОбъектов::Новый()
	void CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const;

	class GuardOfClearFlagCreateWithoutConstructor
	{
	public:
		//GuardOfClearFlagCreateWithoutConstructor() {};
		GuardOfClearFlagCreateWithoutConstructor(bool bSetFlag = false) { if(bSetFlag) CComponentClass::SetFlagCreateWithoutConstructor(); };
		~GuardOfClearFlagCreateWithoutConstructor() { CComponentClass::ClearFlagCreateWithoutConstructor(); };
	};

	// флаг создания без вызова дефолтного конструктора, даже если он есть в модуле
	// удобно для использования с методом ФабрикаОбъектов::Новый()
	//
	static void SetFlagCreateWithoutConstructor(void);
	static void ClearFlagCreateWithoutConstructor(void);
	static bool IsCreateWithoutConstructor();

	CParamDefs::CArrayOfMethodsParamData_PTR GetMethodParamData(int iMethodNum) const;

	//class IV7SerializeToString
	virtual BOOL LoadFromString(LPCSTR szSource);
};

template <typename POINTERTYPE>
POINTERTYPE CValue2Type(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

	CBLContext* pVTContext = Value.GetContext();
	return CBLContext2Type<POINTERTYPE>(pVTContext);
}

template <typename POINTERTYPE>
POINTERTYPE CBLContext2Type(CBLContext* pVTContext)
{
	POINTERTYPE pVector = safe_dynamic_cast<POINTERTYPE>(pVTContext);
	if (NULL != pVector)
		return pVector;

	IUserClassContext* pUserClassContext = safe_dynamic_cast<IUserClassContext*>(pVTContext);
	if (pUserClassContext && pUserClassContext->GetBaseClass(&pVector) )
		return pVector;

	return NULL;
}

template <typename POINTERTYPE>
POINTERTYPE CValue2AfxType(CValue const& Value, LPCSTR szContextTypeName)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

	CBLContext* pVTContext = Value.GetContext();
	return CBLContext2AfxType<POINTERTYPE>(pVTContext, szContextTypeName);
}

template <typename POINTERTYPE>
POINTERTYPE CBLContext2AfxType(CBLContext* pVTContext, LPCSTR szContextTypeName)
{
	if (!pVTContext) return NULL;

	LPCSTR szTypeName = pVTContext->GetTypeString();

	POINTERTYPE pVector = NULL;
	if (!FastCompare.Compare(szTypeName, szContextTypeName))
		pVector = static_cast<POINTERTYPE>(pVTContext);

	if (NULL != pVector)
		return pVector;

	IUserClassContext* pUserClassContext = safe_dynamic_cast<IUserClassContext*>(pVTContext);
	if (pUserClassContext)
	{
		CValue vBaseClass;
		if ( pUserClassContext->GetBaseClass(szContextTypeName, vBaseClass) )
		{
			CBLContext* pVTContext = vBaseClass.GetContext();
			pVector = static_cast<POINTERTYPE>(pVTContext);
			return pVector;
		}
	}

	return NULL;
}

#endif // !defined(AFX_COMPONENTCLASS_H__5F1978A2_67C1_4AAB_8C12_44790D3700B2__INCLUDED_)
