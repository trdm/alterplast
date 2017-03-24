// IMetaDataObjChilds.cpp: implementation of the CIMetaDataObjChilds class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMetaDataObjChilds.h"
#include "metadataobjarray.h"
#include "metadata.h"

ITypeInfo* CIMetaDataObjChilds::m_pTypeInfo=NULL;

STDMETHODIMP CIMetaDataObjChilds::get_item(VARIANT idx,IMetaDataObjArray **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	int num=GetNum(idx);
	if(num<0)
		return E_FAIL;
	*pVal=new CIMetaDataObjArray(m_pObj->GetChild(num,CIMetaData::m_pMetaData));
	return S_OK;
}

STDMETHODIMP CIMetaDataObjChilds::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_pObj->GetNChilds();
	return S_OK;
}

STDMETHODIMP CIMetaDataObjChilds::get_Name(VARIANT idx,VARIANT_BOOL Rus,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	int num=GetNum(idx);
	if(num<0)
		return E_FAIL;
	CString name=m_pObj->GetChildName(num,Rus?1:0);
	*pVal=name.AllocSysString();
	return S_OK;
}

int CIMetaDataObjChilds::GetNum(VARIANT& idx)
{
	int num=-1;
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bName=idx;
		CString sName=(LPCTSTR)bName;
		int cnt=m_pObj->GetNChilds();
		for(int i=0;i<cnt;i++)
		{
			if(!sName.CompareNoCase(m_pObj->GetChildName(i,0)) || 
				!sName.CompareNoCase(m_pObj->GetChildName(i,1)))
			{
				num=i;
				break;
			}
		}
		if(num<0)
			SetError(E_FAIL,"Недопустимое имя потомка: %s",sName);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса");
		else
		{
			if(idx.lVal<0 || idx.lVal>=m_pObj->GetNChilds())
				SetError(E_FAIL,"Индекс выходит за границы массива: %i",idx.lVal);
			else
				num=idx.lVal;
		}
	}
	return num;
}
