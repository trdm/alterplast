// metadataobjarray.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "dispimpl.h"
#include "iconfig.h"
#include "metadataobjarray.h"
#include "metadataobj.h"

ITypeInfo* CIMetaDataObjArray::m_pTypeInfo=NULL;

STDMETHODIMP CIMetaDataObjArray::get_Item(VARIANT idx,IMetaDataObj **pVal)
{
	CMetaDataObj* pObj=NULL;
	try{
		if(idx.vt==VT_BSTR)
		{
			_bstr_t bname=idx;
			pObj=m_pArray->GetItem(bname);
		}
		else
		{
			COleVariant oVar(idx);
			oVar.ChangeType(VT_I4);
			if(oVar.lVal>=0 && oVar.lVal<m_pArray->GetNItems())
				pObj=m_pArray->GetAt(oVar.lVal);
		}
		if(!pObj)
			return SetError(E_FAIL,"Недопустимый индекс");
	}
	catch(...)
	{
		return E_FAIL;
	}
	*pVal=CIMetaDataObj::GetObj(pObj);
	return S_OK;
}
