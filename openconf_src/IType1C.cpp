// IType1C.cpp: implementation of the CIType1C class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IType1C.h"
#include "metadata.h"
ITypeInfo* CIType1C::m_pTypeInfo=NULL;

STDMETHODIMP CIType1C::get_Type(tTypes *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=(tTypes)m_type.GetTypeCode();
	return S_OK;
}

STDMETHODIMP CIType1C::put_Type(tTypes newVal)
{
	m_type.SetTypeCode(newVal);
	return S_OK;
}

STDMETHODIMP CIType1C::get_FullName(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString t=m_type.GetTypeTitle();
	long kind=m_type.GetTypeID();
	if(kind)
	{
		CMetaDataObj* pObj=CIMetaData::m_pMetaData->FindObject(kind);
		if(pObj)
			t=t+"."+pObj->GetCode();
	}
	*pVal=t.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIType1C::get_Name(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.GetTypeTitle().AllocSysString();
	return S_OK;
}

STDMETHODIMP CIType1C::get_Code(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString t=m_type.GetCTypeCode();
	*pVal=t.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIType1C::get_ID(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.GetTypeID();
	return S_OK;
}

STDMETHODIMP CIType1C::put_ID(long newVal)
{
	m_type.SetTypeID(newVal);
	return S_OK;
}

STDMETHODIMP CIType1C::get_Length(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.GetLength();
	return S_OK;
}

STDMETHODIMP CIType1C::put_Length(long newVal)
{
	long p=m_type.GetPrecision();
	newVal=newVal<p?p:newVal;
	m_type.SetFormat(newVal,p);
	return S_OK;
}

STDMETHODIMP CIType1C::get_Precision(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.GetPrecision();
	return S_OK;
}

STDMETHODIMP CIType1C::put_Precision(long newVal)
{
	long l=m_type.GetLength();
	newVal=newVal>l?l:newVal;
	m_type.SetFormat(l,newVal);
	return S_OK;
}

STDMETHODIMP CIType1C::get_PositiveOnly(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.IsPositiveOnly()?-1:0;
	return S_OK;
}

STDMETHODIMP CIType1C::put_PositiveOnly(VARIANT_BOOL newVal)
{
	m_type.SetPositiveOnly(newVal?1:0);
	return S_OK;
}

STDMETHODIMP CIType1C::get_NumSeparated(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_type.IsNumSeparated()?-1:0;
	return S_OK;
}

STDMETHODIMP CIType1C::put_NumSeparated(VARIANT_BOOL newVal)
{
	m_type.SetNumSeparated(newVal?1:0);
	return S_OK;
}
