// IMetaEnums.cpp: implementation of the CIMetaEnums class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMetaEnums.h"
#include "Metadata.h"
#include "metadataobj.h"
ITypeInfo* CIMetaEnums::m_pTypeInfo=NULL;

CIMetaEnums::CIMetaEnums()
{
}

CIMetaEnums::~CIMetaEnums()
{
}

STDMETHODIMP CIMetaEnums::get_Enum(VARIANT idx,IMetaEnum **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEnumDef* pDef=GetEnum(idx);
	if(!pDef)
		return E_FAIL;
	*pVal=(IMetaEnum*)CIMetaDataObj::GetObj(pDef);
	return S_OK;
}

STDMETHODIMP CIMetaEnums::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CIMetaData::m_pMetaData->GetNEnumDefs();
	return S_OK;
}

STDMETHODIMP CIMetaEnums::Remove(VARIANT idx)
{
	CEnumDef* pDef=GetEnum(idx);
	if(!pDef)
		return E_FAIL;
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	CPtrArray arr;
	pMD->TestRefers(pDef->GetID(),(CArray<CMetaDataObj*,CMetaDataObj*>&)arr);
	if(arr.GetSize())
		return SetError(E_FAIL,"На объект существуют ссылки");
	pMD->DelEnumDef(pDef,TRUE);
	CIMetaData::m_pOneMetaData->SetMDModified();
	CIMetaData::m_pOneMetaData->RefreshMDWnd();
	return S_OK;
}

STDMETHODIMP CIMetaEnums::Add(BSTR Name,IMetaEnum **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString name=(LPCTSTR)_bstr_t(Name);
	if(name.IsEmpty() || !CMetaDataObj::ValidateCode(name,tmdEnum))
		return SetError(E_FAIL,"Недопустимое имя: \"%s\"",name);
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	CEnumDef* pDef=pMD->GetEnumDef(name);
	if(pDef)
		return SetError(E_FAIL,"Перечисление с именем \"%s\" уже сущестует",name);
	pDef=new CEnumDef(pMD->GetNewID(),name,NULL);
	pMD->AddEnumDef(pDef);
	CIMetaData::m_pOneMetaData->SetMDModified();
	*pVal=(IMetaEnum*)CIMetaDataObj::GetObj(pDef);
	return S_OK;
}

CEnumDef* CIMetaEnums::GetEnum(VARIANT& idx)
{
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	CEnumDef* pRet=NULL;
	if(idx.vt==VT_BSTR)
	{
		CString name=(LPCTSTR)_bstr_t(idx.bstrVal);
		pRet=pMD->GetEnumDef(name);
		if(!pRet)
			SetError(E_FAIL,"Перечисления с именем \"%s\" не существует",name);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса");
		else
		{
			if(idx.lVal<0 || idx.lVal>=pMD->GetNEnumDefs())
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
			else
				pRet=pMD->GetEnumDefAt(idx.lVal);
		}
	}
	return pRet;
}
