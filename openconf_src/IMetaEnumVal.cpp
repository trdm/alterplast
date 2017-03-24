// IMetaEnumVal.cpp: implementation of the CIMetaEnumVal class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMetaEnumVal.h"
#include "Metadata.h"
ITypeInfo* CIMetaEnumVal::m_pTypeInfo=NULL;

CIMetaEnumVal::~CIMetaEnumVal()
{
}

STDMETHODIMP CIMetaEnumVal::get_Title(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CEnumValDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	CString name=pDef->GetTitle();
	*pVal=name.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMetaEnumVal::put_Title(BSTR newVal)
{
	CEnumValDef* pDef=GetMD();
	if(!pDef)
		return E_FAIL;
	CString name=pDef->GetTitle();
	CString newName=(LPCTSTR)_bstr_t(newVal);
	if(name!=newName)
	{
		pDef->SetTitle(newName);
		CIMetaData::m_pOneMetaData->SetMDModified();
	}
	return S_OK;
}
