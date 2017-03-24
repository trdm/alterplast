// IMigration.cpp: implementation of the CIMigration class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMigration.h"
#include "metadata.h"

ITypeInfo* CIMigration::m_pTypeInfo=NULL;

CIMigration::~CIMigration()
{

}

STDMETHODIMP CIMigration::get_RuleOfMigration(MigrationRules *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=(MigrationRules)m_pDistr->GetRuler();
	return S_OK;
}

STDMETHODIMP CIMigration::put_RuleOfMigration(MigrationRules newVal)
{
	int nr=newVal;
	if(nr<0 || nr>2)
		return SetError(E_FAIL,"Недопустимое правило миграции: %i",nr);
	if(nr!=m_pDistr->GetRuler())
	{
		m_pDistr->SetRuler(nr);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}

STDMETHODIMP CIMigration::get_AutoRegister(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_pDistr->IsAutoRegister()?-1:0;
	return S_OK;
}

STDMETHODIMP CIMigration::put_AutoRegister(VARIANT_BOOL newVal)
{
	m_pDistr->SetAutoRegister(newVal!=0);
	return S_OK;
}

STDMETHODIMP CIMigration::get_AdditDBSigns(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString addit=m_pDistr->GetAdditDBSigns();
	*pVal=addit.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMigration::put_AdditDBSigns(BSTR newVal)
{
	_bstr_t nr=newVal;
	CString old;
	m_pDistr->GetAdditDBSigns(old);
	if(old!=nr)
	{
		m_pDistr->SetAdditDBSigns(nr);
	}
	return S_OK;
}
