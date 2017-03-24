// IRefers.cpp: implementation of the CIRefers class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IRefers.h"
#include "metadataobj.h"

STDMETHODIMP CIRefers::get_Item(VARIANT idx,IMetaDataObj **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(idx.vt!=VT_I4)
		VariantChangeType(&idx,&idx,0,VT_I4);
	if(idx.vt!=VT_I4)
		return SetError(E_FAIL,"Недопустимый тип индекса");
	if(idx.lVal<0 || idx.lVal>=m_pArr->GetSize())
		return SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
	*pVal=CIMetaDataObj::GetObj((CMetaDataObj*)m_pArr->GetAt(idx.lVal));
	return S_OK;
}

STDMETHODIMP CIRefers::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_pArr->GetSize();
	return S_OK;
}
