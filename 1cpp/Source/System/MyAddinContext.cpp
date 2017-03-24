// Hooker.cpp: implementation of the CHooker class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MyAddInContext.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAddInContext_MyProxy, CBLContext);

class CObjID CAddInContext_MyProxy::ObjID;

CAddInContext_MyProxy* CAddInContext_MyProxy::CreateWithNewObject(LPCTSTR typeString, CAddInContext* pCont)
{
	if (!pCont)
		return static_cast<CAddInContext_MyProxy*>(NULL);

	CAddInContext_MyProxy* res = static_cast<CAddInContext_MyProxy*>(CBLContext::CreateInstance("CAddInContext_MyProxy"));

	if (!res)
	{
		pCont->DecrRef();
		return static_cast<CAddInContext_MyProxy*>(NULL);
	}
	res->m_BaseCont = pCont;
	pCont->DecrRef();
	
	res->m_TypeString = typeString;

	return res;
}

// 
CAddInContext_MyProxy::CAddInContext_MyProxy(int iParam) : CBLContext(iParam), m_BaseCont(NULL), m_TypeString("")
{
};

CAddInContext_MyProxy::~CAddInContext_MyProxy()
{
	m_BaseCont = NULL;
}

int  CAddInContext_MyProxy::CallAsFunc(int iMethNum,class CValue & rValue,class CValue** ppValue)
{
	if (!m_BaseCont)
		return CBLContext::CallAsFunc(iMethNum, rValue, ppValue);;
	return m_BaseCont->CallAsFunc(iMethNum, rValue, ppValue);
}

int  CAddInContext_MyProxy::CallAsProc(int iMethNum,class CValue * * ppValue)
{
	if (!m_BaseCont)
		return CBLContext::CallAsProc(iMethNum, ppValue);
	return m_BaseCont->CallAsProc(iMethNum, ppValue);
}


int  CAddInContext_MyProxy::FindMethod(char const * lpMethodName)const
{
	if (!m_BaseCont)
		return CBLContext::FindMethod(lpMethodName);;
	return m_BaseCont->FindMethod(lpMethodName);
}

char const *  CAddInContext_MyProxy::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (!m_BaseCont)
		return CBLContext::GetMethodName(iMethodNum, iMethodAlias);
	return m_BaseCont->GetMethodName(iMethodNum, iMethodAlias);
}

int  CAddInContext_MyProxy::GetNMethods(void)const
{
	if (!m_BaseCont)
		return CBLContext::GetNMethods();
	return m_BaseCont->GetNMethods();
}

int  CAddInContext_MyProxy::HasRetVal(int iMethodNum)const
{	
	if (!m_BaseCont)
		return CBLContext::HasRetVal(iMethodNum);
	return m_BaseCont->HasRetVal(iMethodNum);
}

int CAddInContext_MyProxy::GetNParams(int iMethodNum)const
{
	if (!m_BaseCont)
		return CBLContext::GetNParams(iMethodNum);
	return m_BaseCont->GetNParams(iMethodNum);
}

int  CAddInContext_MyProxy::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	if (!m_BaseCont)
		return CBLContext::GetParamDefValue(iMethodNum, iParamNum, pDefValue);
	return m_BaseCont->GetParamDefValue(iMethodNum, iParamNum, pDefValue);
}

void  CAddInContext_MyProxy::DecrRef(void)
{
	CBLContext::DecrRef();
}

char const *  CAddInContext_MyProxy::GetCode(void)const
{
	if (!m_BaseCont)
		return CBLContext::GetCode();
	return m_BaseCont->GetCode();
}

int  CAddInContext_MyProxy::GetDestroyUnRefd(void)const
{
	if (!m_BaseCont)
		return CBLContext::GetDestroyUnRefd();
	return m_BaseCont->GetDestroyUnRefd();
}

void  CAddInContext_MyProxy::GetExactValue(class CValue & vParam)
{
	if (!m_BaseCont)
		CBLContext::GetExactValue(vParam);
	m_BaseCont->GetExactValue(vParam);
}

class CObjID   CAddInContext_MyProxy::GetID(void)const
{
	return ObjID; // TODO m_BaseCont ???
}

class CBLContextInternalData *  CAddInContext_MyProxy::GetInternalData(void)
{
	if (!m_BaseCont)
		return CBLContext::GetInternalData();
	return m_BaseCont->GetInternalData();
}

long  CAddInContext_MyProxy::GetTypeID(void)const
{
	if (!m_BaseCont)
		return CBLContext::GetTypeID();
	return m_BaseCont->GetTypeID(); //100;
}

char const *  CAddInContext_MyProxy::GetTypeString(void)const
{
	return m_TypeString;
}

class CType   CAddInContext_MyProxy::GetValueType(void)const
{
// 	CType tType(100);
// 	
// 	return tType;
	if (!m_BaseCont)
		return CBLContext::GetValueType();
	return m_BaseCont->GetValueType();
}


void  CAddInContext_MyProxy::IncrRef(void)
{
	//m_BaseCont->IncrRef();
	CBLContext::IncrRef();
}

void  CAddInContext_MyProxy::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
	if (m_BaseCont)
		m_BaseCont->InitObject(tType);
}

void  CAddInContext_MyProxy::InitObject(char const * strName)
{
	m_TypeString = strName;
	CBLContext::InitObject(strName);
	if (m_BaseCont)
		m_BaseCont->InitObject(strName);
}

int  CAddInContext_MyProxy::IsExactValue(void)const
{
	if (!m_BaseCont)
		return CBLContext::IsExactValue();
	return m_BaseCont->IsExactValue();
}

int  CAddInContext_MyProxy::IsOleContext(void)const
{
	if (!m_BaseCont)
		return CBLContext::IsOleContext();
	return m_BaseCont->IsOleContext();
}

int  CAddInContext_MyProxy::IsPropReadable(int iPropNum)const
{
	if (!m_BaseCont)
		return CBLContext::IsPropReadable(iPropNum);
	return m_BaseCont->IsPropReadable(iPropNum);
}

int  CAddInContext_MyProxy::IsPropWritable(int iPropNum)const
{
	if (!m_BaseCont)
		return CBLContext::IsPropWritable(iPropNum);
	return m_BaseCont->IsPropWritable(iPropNum);
}

int  CAddInContext_MyProxy::IsSerializable(void)
{
	if (!m_BaseCont)
		return CBLContext::IsSerializable();
	return m_BaseCont->IsSerializable();
}

int  CAddInContext_MyProxy::SaveToString(class CString & csStr)
{
	if (!m_BaseCont)
		return CBLContext::SaveToString(csStr);
	return m_BaseCont->SaveToString(csStr);
}

void  CAddInContext_MyProxy::SelectByID(class CObjID cID,long lNum)
{
	if (!m_BaseCont)
		CBLContext::SelectByID(cID,lNum);
	else
		m_BaseCont->SelectByID(cID,lNum);
	//CBLContext::SelectByID(cID,lNum); // TODO
}


int  CAddInContext_MyProxy::GetNProps(void)const
{
	if (!m_BaseCont)
		return CBLContext::GetNProps();
	return m_BaseCont->GetNProps();
}

char const *  CAddInContext_MyProxy::GetPropName(int A,int B)const
{
	if (!m_BaseCont)
		return CBLContext::GetPropName(A, B);
	return m_BaseCont->GetPropName(A, B);
}

int  CAddInContext_MyProxy::GetPropVal(int iPropNum,class CValue & rValue)const
{
	if (!m_BaseCont)
		return CBLContext::GetPropVal(iPropNum, rValue);
	return m_BaseCont->GetPropVal(iPropNum, rValue);
}

int  CAddInContext_MyProxy::SetPropVal(int iPropNum,class CValue const & vValue)
{
	if (!m_BaseCont)
		return CBLContext::SetPropVal(iPropNum, vValue);
	return m_BaseCont->SetPropVal(iPropNum, vValue);
}

int  CAddInContext_MyProxy::FindProp(char const * Name)const
{
	if (!m_BaseCont)
		return CBLContext::FindProp(Name);
	return m_BaseCont->FindProp(Name);
}
