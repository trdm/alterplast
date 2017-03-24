// ITaskDef.cpp: implementation of the CITaskDef class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "metadataobj.h"
#include "metadata.h"
#include "ITaskDef.h"
ITypeInfo* CITaskDef::m_pTypeInfo=NULL;


STDMETHODIMP CITaskDef::get_Language(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=GetMD()->GetDefaultLanguage();
	return S_OK;
}

STDMETHODIMP CITaskDef::put_Language(long newVal)
{
	if(newVal)
		newVal=1;
	BOOL old=GetMD()->GetDefaultLanguage();
	if(old!=newVal)
	{
		GetMD()->SetDefaultLanguage(newVal);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}

STDMETHODIMP CITaskDef::get_ImmediateDelete(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=GetMD()->GetEnableImmediateDelete()?-1:0;
	return S_OK;
}

STDMETHODIMP CITaskDef::put_ImmediateDelete(VARIANT_BOOL newVal)
{
	if(newVal)
		newVal=TRUE;
	BOOL old=GetMD()->GetEnableImmediateDelete();
	if(old!=newVal)
	{
		GetMD()->SetEnableImmediateDelete(newVal);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}

STDMETHODIMP CITaskDef::get_Round15_2(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=GetMD()->GetUseBuchRounding()?0:-1;
	return S_OK;
}

STDMETHODIMP CITaskDef::put_Round15_2(VARIANT_BOOL newVal)
{
	newVal=newVal?FALSE:TRUE;
	BOOL old=GetMD()->GetUseBuchRounding();
	if(old!=newVal)
	{
		GetMD()->SetUseBuchRounding(newVal);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}
