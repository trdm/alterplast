// IConstants.cpp: implementation of the CIConstants class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IConstants.h"
#include "metadata.h"
#include "metadataobj.h"

ITypeInfo* CIConstants::m_pTypeInfo=NULL;

CIConstants::CIConstants()
{
}

STDMETHODIMP CIConstants::get_item(VARIANT idx,IConstDef **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CConstDef* pDef=GetDef(idx);
	if(!pDef)
		return E_FAIL;
	*pVal=(IConstDef*)CIMetaDataObj::GetObj(pDef);
	return S_OK;
}

STDMETHODIMP CIConstants::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CIMetaData::m_pMetaData->GetNConstDefs();
	return S_OK;
}

STDMETHODIMP CIConstants::get_Name(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CConstDef* pDef=GetDef(idx);
	if(!pDef)
		return E_FAIL;
	CString n=pDef->GetCode();
	*pVal=n.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIConstants::Add(BSTR name,BSTR Present,IConstDef **pRetVal)
{
	if(IsBadWritePtr(pRetVal,sizeof(*pRetVal)))
		return E_POINTER;
	CString sName=name;
	if(sName.IsEmpty() || !CMetaDataObj::ValidateCode(sName,0))
		return SetError(E_FAIL,"Недопустимое имя: \"%s\"",sName);
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	CConstDef* pConst=pMD->GetConstDef(sName);
	if(pConst)
		return SetError(E_FAIL,"Константа с именем \"%s\" уже существует",sName);
	CString pres=Present;
	pConst=new CConstDef(pMD->GetNewID(),sName,pres,0,CType(0));
	pMD->AddConstDef(pConst);
	*pRetVal=(IConstDef*)CIMetaDataObj::GetObj(pConst);
	CIMetaData::m_pOneMetaData->SetMDModified();
	return S_OK;
}

STDMETHODIMP CIConstants::Remove(VARIANT idx)
{
	CConstDef* pDef=GetDef(idx);
	if(!pDef)
		return E_FAIL;
	CIMetaData::m_pMetaData->DelConstDef(pDef,TRUE);
	CIMetaData::m_pOneMetaData->SetMDModified();
	CIMetaData::m_pOneMetaData->RefreshMDWnd();
	return S_OK;
}

CConstDef* CIConstants::GetDef(VARIANT& idx)
{
	CConstDef* pRet=NULL;
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bName=idx;
		pRet=pMD->GetConstDef(bName);
		if(!pRet)
			SetError(E_FAIL,"Константа с именем \"%s\" не существует",(LPCTSTR)bName);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса");
		else
		{
			if(idx.lVal<0 || idx.lVal>=pMD->GetNConstDefs())
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
			else
				pRet=pMD->GetConstDefAt(idx.lVal);
		}
	}
	return pRet;
}
