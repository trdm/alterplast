// IMetaDataObjProps.cpp: implementation of the CIMetaDataObjProps class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMetaDataObjProps.h"
#include "metadata.h"

ITypeInfo* CIMetaDataObjProps::m_pTypeInfo=NULL;

STDMETHODIMP CIMetaDataObjProps::get_item(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	int num=GetNum(idx);
	if(num<0)
		return E_FAIL;
	CString name;
	CValue val;
	CMetaDataObj* pNew=NULL;
	m_pObj->GetPropVal(num,CIMetaData::m_pMetaData,val,&pNew);
	name=val.GetString();
	if(pNew)
	{
		if(pNew->IsRefObj())
		{
			CMetaDataRefObj* pRef=(CMetaDataRefObj*)pNew;
			for(int z=0;z<pRef->GetNRefDefs();z++)
			{
				name=name+(name.IsEmpty()?"":",")+pRef->GetRefDefAt(z)->GetString();
			}
		}
		else
		{
			long l;
			CString ob=CIMetaData::m_pMetaData->GetFullName(pNew->GetID(),&l,1);
			name=name+(name.IsEmpty()?"":" ")+" ["+ob+"]";
		}
	}
	*pVal=name.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMetaDataObjProps::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_pObj->GetNProps();
	return S_OK;
}

STDMETHODIMP CIMetaDataObjProps::get_Name(VARIANT idx,VARIANT_BOOL Rus,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	int num=GetNum(idx);
	if(num<0)
		return E_FAIL;
	CString name=m_pObj->GetPropName(num,Rus?1:0);
	*pVal=name.AllocSysString();
	return S_OK;
}

int CIMetaDataObjProps::GetNum(VARIANT& idx)
{
	int num=-1;
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bName=idx;
		CString sName=(LPCTSTR)bName;
		int cnt=m_pObj->GetNProps();
		for(int i=0;i<cnt;i++)
		{
			if(!sName.CompareNoCase(m_pObj->GetPropName(i,0)) || 
				!sName.CompareNoCase(m_pObj->GetPropName(i,1)))
			{
				num=i;
				break;
			}
		}
		if(num<0)
			SetError(E_FAIL,"Недопустимое имя свойства: %s",sName);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса");
		else
		{
			if(idx.lVal<0 || idx.lVal>=m_pObj->GetNProps())
				SetError(E_FAIL,"Индекс выходит за границы массива: %i",idx.lVal);
			else
				num=idx.lVal;
		}
	}
	return num;
}
