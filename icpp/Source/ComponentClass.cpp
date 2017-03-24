// ComponentClass.cpp: implementation of the CComponentClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ComponentClass.h"
#include "UserClassesImpl.h"

#include "System\GroupContextWrap.h"

extern CMetaDataCont * pMetaDataCont;

IMPLEMENT_DYNCREATE(CComponentClass, CBLContext)

CComponentClass::CComponentClass() : IUserClassContext(), m_pImpl(new CComponentClassImpl(this))
{

}

CComponentClass::~CComponentClass()
{
}

//int CComponentClass::GetBaseClass(const CValue &NameClass, CValue &rValue) const
int CComponentClass::GetBaseClass(const CString & NameClass, CValue & rValue) const
{
	return m_pImpl->GetBaseClass(NameClass, rValue);
}

int CComponentClass::IsObject(const CValue &NameClass, CValue &rValue) const
{
	return m_pImpl->IsObject(NameClass, rValue);
}

int  CComponentClass::CallAsFunc(int iMethNum,CValue & rValue,CValue * *ppValue)
{
	return m_pImpl->CallAsFunc(iMethNum, rValue, ppValue);
}

int  CComponentClass::CallAsProc(int iMethNum, CValue ** ppValue)
{
	return m_pImpl->CallAsProc(iMethNum, ppValue);
}


int  CComponentClass::FindMethod(char const * lpMethodName)const
{
	return m_pImpl->FindMethod(lpMethodName);
}

char const *  CComponentClass::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return m_pImpl->GetMethodName(iMethodNum, iMethodAlias);
}

int  CComponentClass::GetNMethods(void)const
{
	return m_pImpl->GetNMethods();
}

int  CComponentClass::HasRetVal(int iMethodNum)const
{
	return m_pImpl->HasRetVal(iMethodNum);
}

int CComponentClass::GetNParams(int iMethodNum)const
{
	return m_pImpl->GetNParams(iMethodNum);
}


int  CComponentClass::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	return m_pImpl->GetParamDefValue(iMethodNum, iParamNum, pDefValue);
}

void  CComponentClass::IncrRef(void)
{
	m_pImpl->IncrRef();
}

void  CComponentClass::DecrRef(void)
{
	m_pImpl->DecrRef();
}

// данная функция используется при вызове Строка(ОбъектКласса) в 1С
// разрешаю наследование функции _ПолучитьКод для КОП,
// если в КОП предке данный метод описан с модификатором Экспорт
// если же данного модификатора нет, значит, этот метод не будет наследоваться
//
char const *  CComponentClass::GetCode(void) const
{
	return m_pImpl->GetCode();
}

class CObjID   CComponentClass::GetID(void)const
{
	return m_pImpl->GetID();
}

class CBLContextInternalData *  CComponentClass::GetInternalData(void)
{
	return m_pImpl->GetInternalData();
}

long  CComponentClass::GetTypeID(void)const
{
	return m_pImpl->GetTypeID();
}

char const *  CComponentClass::GetTypeString(void)const
{
	return m_pImpl->GetTypeString();
}

CType   CComponentClass::GetValueType(void)const
{
	return m_pImpl->GetValueType();
}

void  CComponentClass::InitObject(char const * strName)
{
	m_pImpl->InitObject(strName);
}

int  CComponentClass::IsExactValue(void)const
{
	return m_pImpl->IsExactValue();
}

int  CComponentClass::IsOleContext(void)const
{
	return m_pImpl->IsOleContext();
}

int  CComponentClass::IsPropReadable(int iPropNum)const
{
	return m_pImpl->IsPropReadable(iPropNum);
}

int  CComponentClass::IsPropWritable(int iPropNum)const
{
	return m_pImpl->IsPropWritable(iPropNum);
}

int  CComponentClass::IsSerializable(void)
{
	return m_pImpl->IsSerializable();
}

int  CComponentClass::SaveToString(CString & csStr)
{
	return m_pImpl->SaveToString(csStr);
}

void  CComponentClass::SelectByID(class CObjID cID,long lNum)
{
	return m_pImpl->SelectByID(cID, lNum);
}

int  CComponentClass::GetNProps(void)const
{
	return m_pImpl->GetNProps();
}

char const *  CComponentClass::GetPropName(int A,int B)const
{
	return m_pImpl->GetPropName(A, B);
}

int  CComponentClass::GetPropVal(int iPropNum,CValue & rValue)const
{
	return m_pImpl->GetPropVal(iPropNum, rValue);
}

int  CComponentClass::SetPropVal(int iPropNum,CValue const & vValue)
{
	return m_pImpl->SetPropVal(iPropNum, vValue);
}

int  CComponentClass::FindProp(char const * Name)const
{
	return m_pImpl->FindProp(Name);
}


// IGetModule
CBLModule * CComponentClass::GetHostModule() const
{
	return m_pImpl->GetHostModule();
}

// IGetModule
CBLModule * CComponentClass::GetModule() const
{
	return m_pImpl->GetModule();
}

BOOL CComponentClass::GetBaseClass(const CRuntimeClass* pRTClass, CBLContext** ppBaseObj) const
{
	return m_pImpl->GetBaseClass(pRTClass, ppBaseObj);
}

// проверить, является ли текущий исполняемый модуль модулем базового класса для текущего класса
// используется для исключения рекурсии
CComponentClass* CComponentClass::GetComponentClassCurExModule(CBLModule* pModWrapper)
{
	return CComponentClassImpl::GetComponentClassCurExModule(pModWrapper);
}

void CComponentClass::ClearOptimizationData(void)
{
	CComponentClassImpl::ClearOptimizationData();
}

// метод для вызова пользовательского конструктора с несколькими параметрами
// используется в методе ФабрикаОбъектов::Новый()
void CComponentClass::CallUserConstructor(const CString&  strConstructorName, int nParamsCount, CValue **ppValue) const
{
	m_pImpl->CallUserConstructor(strConstructorName, nParamsCount, ppValue);
}


BOOL CComponentClass::SendMessageOwnMod( CValue &rValue, CValue ** ppValue )
{
	return m_pImpl->SendMessageOwnMod(rValue, ppValue);
}

void CComponentClass::GetPathName( CValue &rValue )
{
	m_pImpl->GetPathName(rValue);
}

void CComponentClass::GetEnvContext( CValue &rValue )
{
	m_pImpl->GetEnvContext(rValue);
}

void CComponentClass::GetParamsList( CValue &rValue, CValue * * ppValue )
{
	m_pImpl->GetParamsList(rValue, ppValue);
}

BOOL CComponentClass::SetOnIndexParams( CValue &rValue, CValue * * ppValue )
{
	return m_pImpl->SetOnIndexParams(rValue, ppValue);
}

void CComponentClass::ThrowException(CValue ** ppValue)
{
	m_pImpl->ThrowException(ppValue);
}

void CComponentClass::AddDynamicProperty(CValue ** ppValue)
{
	m_pImpl->AddDynamicProperty(ppValue);
}

CParamDefs::CArrayOfMethodsParamData_PTR CComponentClass::GetMethodParamData(int iMethodNum) const
{
	return m_pImpl->GetMethodParamData(iMethodNum);
}

// Получить количество базовых объектов.
DWORD CComponentClass::GetBaseObjCount()
{
	return m_pImpl->GetBaseObjCount();
}

// Получить имя базового объекта по индексу.
CString CComponentClass::GetBaseObjName(DWORD idx)
{
	return m_pImpl->GetBaseObjName(idx);
}

// Получить базовый объект по индексу.
CBLContext* CComponentClass::GetBaseObj(DWORD idx)
{
	return m_pImpl->GetBaseObj(idx);
}

// Для upcast'а
CBLContext* CComponentClass::GetDerivedObj()
{
	return m_pImpl->GetDerivedObj();
}

int CComponentClass::ReplaceInstBaseClasses(const CValue &NameClass, const CValue &Instance, CValue &rValue)
{
	return m_pImpl->ReplaceInstBaseClasses(NameClass, Instance, rValue);
}

void CComponentClass::SetFlagCreateWithoutConstructor(void) 
{ 
	return CComponentClassImpl::SetFlagCreateWithoutConstructor(); 
}

void CComponentClass::ClearFlagCreateWithoutConstructor(void) 
{ 
	return CComponentClassImpl::ClearFlagCreateWithoutConstructor(); 
}

bool CComponentClass::IsCreateWithoutConstructor() 
{ 
	return CComponentClassImpl::IsCreateWithoutConstructor(); 
}

bool CComponentClass::StillInitInstanceComp()
{
	return CComponentClassImpl::StillInitInstanceComp(); 
}

//class IV7SerializeToString
BOOL CComponentClass::LoadFromString(LPCSTR szSource)
{
	return m_pImpl->LoadFromString(szSource);
}

// интерфейс Utils1cpp::ICloneCreated
CBLContext* CComponentClass::Clone(LPCSTR szClassName) const
{
	CComponentClass* pClassCont = new CComponentClass();
	pClassCont->InitObject(szClassName);
	//pClassCont->m_pImpl->Clone(m_pImpl);
	
	return pClassCont;
}

// интерфейс Utils1cpp::IQueryInterface_Loader_Publisher

BOOL CComponentClass::IsLoader() const
{	return m_pImpl->IsLoader(); }

BOOL CComponentClass::IsPublisher() const 
{ return m_pImpl->IsPublisher(); };

// интерфейс IResultLoader
HRESULT CComponentClass::ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
{
	return m_pImpl->ldrInit(nColCount, nRowCount);
}

HRESULT CComponentClass::ldrDone() throw()
{
	return m_pImpl->ldrDone();
}

HRESULT CComponentClass::ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
{
	return m_pImpl->ldrAddColumn(szColName, pType);
}

HRESULT CComponentClass::ldrAddRow(CValue const* const* ppValues) throw()
{
	return m_pImpl->ldrAddRow(ppValues);
}

LPCSTR CComponentClass::GetLastError() throw()
{
	return m_pImpl->GetLastError();
}

// end IResultLoader
