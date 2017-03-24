// IMetaEnum.cpp: implementation of the CIMetaEnum class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMetaEnum.h"
#include "Metadata.h"
ITypeInfo* CIMetaEnum::m_pTypeInfo=NULL;

CIMetaEnum::~CIMetaEnum()
{
}

STDMETHODIMP CIMetaEnum::get_ValsCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEnumDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	*pVal=pDef->GetNValDefs();
	return S_OK;
}

STDMETHODIMP CIMetaEnum::get_EnumVal(VARIANT idx,IMetaEnumVal **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEnumValDef* pDef=GetValDef(idx);
	if(!pDef)
		return E_FAIL;
	*pVal=(IMetaEnumVal*)CIMetaDataObj::GetObj(pDef);
	return S_OK;
}

STDMETHODIMP CIMetaEnum::Add(BSTR Name,IMetaEnumVal **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEnumDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	CString name=(LPCTSTR)_bstr_t(Name);
	if(name.IsEmpty() || !CMetaDataObj::ValidateCode(name,0))
		return SetError(E_FAIL,"Недопустимое имя: \"%s\"",name);

	CEnumValDef* pValDef=pDef->GetValDef(name);
	if(pValDef)
		return SetError(E_FAIL,"Значение перечисления с именем \"%s\" уже существует",name);
	pValDef=new CEnumValDef(CIMetaData::m_pMetaData->GetNewID(),name,NULL);
	pDef->AddValDef(pValDef);
	CIMetaData::m_pOneMetaData->SetMDModified();
	*pVal=(IMetaEnumVal*)CIMetaDataObj::GetObj(pDef);
	return S_OK;
}

STDMETHODIMP CIMetaEnum::Remove(VARIANT idx)
{
	CEnumDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	CEnumValDef* pValDef=GetValDef(idx);
	if(!pValDef)
		return E_FAIL;
	pDef->DelValDef(pValDef,TRUE);
	CIMetaData::m_pOneMetaData->SetMDModified();
	CIMetaData::m_pOneMetaData->RefreshMDWnd();
	return S_OK;
}

CEnumValDef* CIMetaEnum::GetValDef(VARIANT& idx)
{
	CEnumDef* pDef=GetMD();
	if(!pDef)
		return NULL;
	CEnumValDef* pRet=NULL;
	if(idx.vt==(VT_VARIANT|VT_BYREF))
	{
		if(VariantChangeType(&idx,&idx,0,VT_I4)!=S_OK)
			VariantChangeType(&idx,&idx,0,VT_BSTR);
	}
	if(idx.vt==VT_BSTR)
	{
		CString name=(LPCTSTR)_bstr_t(idx.bstrVal);
		pRet=pDef->GetValDef(name);
		if(!pRet)
			SetError(E_FAIL,"Значения перечисления с именем \"%s\" не существует",name);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопутимый тип индекса");
		else
		{
			if(idx.lVal<0 || idx.lVal>=pDef->GetNValDefs())
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
			else
				pRet=pDef->GetValDefAt(idx.lVal);
		}
	}
	return pRet;
}
