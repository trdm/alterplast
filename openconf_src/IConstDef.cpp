// IConstDef.cpp: implementation of the CIConstDef class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IConstDef.h"
#include "metadata.h"
ITypeInfo* CIConstDef::m_pTypeInfo=NULL;

STDMETHODIMP CIConstDef::get_Periodic(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CConstDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	int p=pDef->IsDateDep();
	*pVal=p?-1:0;
	return S_OK;
}

STDMETHODIMP CIConstDef::put_Periodic(VARIANT_BOOL newVal)
{
	if(newVal)
		newVal=TRUE;
	CConstDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	if(pDef->IsDateDep()!=newVal)
	{
		pDef->SetDateDep(newVal);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}
