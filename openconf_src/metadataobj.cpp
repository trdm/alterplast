// metadataobj.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "dispimpl.h"
#include "iconfig.h"
#include "metadataobj.h"
#include "metadata.h"
#include "metadataobjarray.h"
#include "itaskdef.h"
#include "IMetaDataObjChilds.h"
#include "IMetaDataObjProps.h"
#include "IType1C.h"
#include "IConstDef.h"
#include "IMetaEnum.h"
#include "IMetaEnumVal.h"

ITypeInfo* CIMetaDataObj::m_pTypeInfo=NULL;
CMapPtrToPtr CIMetaDataObj::m_mapObjToIObj;

CIMetaDataObj::CIMetaDataObj(CMetaDataObj* pObj)
{
	m_pObj=pObj;
	m_id=pObj->GetID();
}
CIMetaDataObj::~CIMetaDataObj()
{
	m_mapObjToIObj.RemoveKey(m_pObj);
}
CIMetaDataObj* CIMetaDataObj::GetObj(CMetaDataObj* pObj)
{
	if(!pObj)
		return NULL;
	try{
		CIMetaDataObj* pIObj=(CIMetaDataObj*)m_mapObjToIObj[(void*)pObj];
		if(pIObj)
			pIObj->AddRef();
		else
		{
			pIObj=new CIMetaDataObj(pObj);
			switch(pObj->WhatIsIt())
			{
			case tmdTask:
				pIObj=(CIMetaDataObj*)new CITaskDef(pIObj);
				break;
			case tmdConstant:
				pIObj=(CIMetaDataObj*)new CIConstDef(pIObj);
				break;
			case tmdEnum:
				pIObj=(CIMetaDataObj*)new CIMetaEnum(pIObj);
				break;
			case tmdEnumVal:
				pIObj=(CIMetaDataObj*)new CIMetaEnumVal(pIObj);
				break;
			};
			m_mapObjToIObj[(void*)pObj]=pIObj;
		}
		return pIObj;
	}
	catch(...){}
	return NULL;
}

CIMetaDataObj* CIMetaDataObj::GetObj(long id)
{
	return GetObj(CIMetaData::m_pMetaData->FindObject(id));
}

long CIMetaDataObj::IsValid()
{
	try{
		CMetaDataObj* pObj=CIMetaData::m_pMetaData->FindObject(m_id);
		return m_pObj==pObj?1:0;
	}
	catch(...)
	{
		return 0;
	}
}

STDMETHODIMP CIMetaDataObj::get_ID(long *pVal)
{
	CHECK_VALID();
	*pVal=m_id;
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Name(BSTR *pVal)
{
	CHECK_VALID();
	CString txt;
	try
	{
		txt=m_pObj->GetCode();
	}
	catch(...)
	{
		return E_FAIL;
	}
	*pVal=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::put_Name(BSTR newVal)
{
	CHECK_VALID();
	CString txt=newVal;
	try
	{
		if(!CMetaDataObj::ValidateCode(txt,m_id))
			return SetError(E_FAIL,"Недопустимый идентификатор: %s",txt);
		CString old=m_pObj->GetCode();
		if(txt!=old)
		{
			m_pObj->SetCode(txt);
			CIMetaData::m_pOneMetaData->SetMDModified();
		}
	}
	catch(...)
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Present(BSTR *pVal)
{
	CHECK_VALID();
	CString txt;
	try
	{
		txt=m_pObj->GetPresent();
	}
	catch(...)
	{
		return E_FAIL;
	}
	*pVal=txt.AllocSysString();
	return S_OK;
}
    
STDMETHODIMP CIMetaDataObj::put_Present(BSTR newVal)
{
	CHECK_VALID();
	CString txt=newVal;
	try
	{
		CString old=m_pObj->GetPresent();
		if(txt!=old)
		{
			m_pObj->SetPresent(txt);
			CIMetaData::m_pOneMetaData->SetMDModified();
		}
	}
	catch(...)
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Descr(BSTR *pVal)
{
	CHECK_VALID();
	CString txt;
	try
	{
		txt=m_pObj->GetDescr();
	}
	catch(...)
	{
		return E_FAIL;
	}
	*pVal=txt.AllocSysString();
	return S_OK;
}
    
STDMETHODIMP CIMetaDataObj::put_Descr(BSTR newVal)
{
	CHECK_VALID();
	CString txt=newVal;
	try
	{
		CString old=m_pObj->GetDescr();
		if(txt!=old)
		{
			m_pObj->SetDescr(txt);
			CIMetaData::m_pOneMetaData->SetMDModified();
		}
	}
	catch(...)
	{
		return E_FAIL;
	}
	return S_OK;
}
    
STDMETHODIMP CIMetaDataObj::IsValid(VARIANT_BOOL *pRet)
{
	*pRet=IsValid()?-1:0;
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_FullName(BSTR *pVal)
{
	CHECK_VALID();
	CString txt=CIMetaData::GetFullName(m_id);
	*pVal=txt.AllocSysString();
	return S_OK;
}
/*    
STDMETHODIMP CIMetaDataObj::get_PropCount(long *pVal)
{
	CHECK_VALID();
	*pVal=m_pObj->GetNProps();
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_PropName(long idx,BSTR *pVal)
{
	CHECK_VALID();
	if(idx<0 || idx>=m_pObj->GetNProps())
		return SetError(E_FAIL,"Недопустимый индекс свойства объекта метаданных");
	CString txt=m_pObj->GetPropName(idx,CIMetaData::m_pMetaData->GetTaskDef()->GetDefaultLanguage());
	*pVal=txt.AllocSysString();
	return S_OK;

}

STDMETHODIMP CIMetaDataObj::get_PropVal(long idx,BSTR *pVal)
{
	CHECK_VALID();
	if(idx<0 || idx>=m_pObj->GetNProps())
		return SetError(E_FAIL,"Недопустимый индекс свойства объекта метаданных");
	CValue val;
	CMetaDataObj* pNew=NULL;
	m_pObj->GetPropVal(idx,CIMetaData::m_pMetaData,val,&pNew);
	CString name=val.GetString();
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
			CString ob=CIMetaData::GetFullName(pNew->GetID());
			if(!name.IsEmpty())
				name+=" ";
			name=name+"["+ob+"]";
		}
	}
	*pVal=name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CIMetaDataObj::get_ChildCount(long *pVal)
{
	CHECK_VALID();
	*pVal=m_pObj->GetNChilds();
	return S_OK;
}
STDMETHODIMP CIMetaDataObj::get_ChildName(long idx,BSTR *pVal)
{
	CHECK_VALID();
	if(idx<0 || idx>=m_pObj->GetNChilds())
		return SetError(E_FAIL,"Недопустимый индекс");
	CString txt=m_pObj->GetChildName(idx,CIMetaData::m_pMetaData->GetTaskDef()->GetDefaultLanguage());
	*pVal=txt.AllocSysString();
	return S_OK;
}
STDMETHODIMP CIMetaDataObj::get_Child(long idx,IMetaDataObjArray **pVal)
{
	CHECK_VALID();
	if(idx<0 || idx>=m_pObj->GetNChilds())
		return SetError(E_FAIL,"Недопустимый индекс");
	*pVal=new CIMetaDataObjArray(m_pObj->GetChild(idx,CIMetaData::m_pMetaData));
	return S_OK;
}
*/
STDMETHODIMP CIMetaDataObj::get_WhatIsIt(TypeOfMetaDataObj *pVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	*pVal=(TypeOfMetaDataObj)m_pObj->WhatIsIt();
	return S_OK;
}

static void DescrObj(CString& txt,CMetaDataObj* pObj,int level,bool WithChild)
{
	for(int i=0;i<level;i++)
		txt+='\t';
	CString line;
	line.Format("- id=%i, %s\r\n",pObj->GetID(),CIMetaData::GetFullName(pObj->GetID()));
	txt+=line;
	pObj->DescribeObject(txt,CIMetaData::m_pMetaData,"",level);
	if(WithChild)
	{
		int nChilds=pObj->GetNChilds();
		for(i=0;i<nChilds;i++)
		{
			CMetaDataObjArray* pArray=pObj->GetChild(i,CIMetaData::m_pMetaData);
			int nItems=pArray->GetNItems();
			for(int j=0;j<nItems;j++)
				DescrObj(txt,pArray->GetAt(j),level+1,WithChild);
		}
	}
}

STDMETHODIMP CIMetaDataObj::DescribeObject(VARIANT_BOOL WithChilds,BSTR *retVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(retVal,4))
		return E_POINTER;
	CString txt;
	DescrObj(txt,m_pObj,0,WithChilds!=0);
	*retVal=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Props(IMetaDataObjProps **pVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=new CIMetaDataObjProps(m_pObj);
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Childs(IMetaDataObjChilds **pVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=new CIMetaDataObjChilds(m_pObj);
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Type(IType1C **pVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=NULL;
	if(m_pObj->IsTypedObj())
	{
		*pVal=new CIType1C(((CMetaDataTypedObj*)m_pObj)->GetType());
	}
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::put_Type(IType1C *newVal)
{
	CHECK_VALID();
	if(m_pObj->IsTypedObj())
	{
		CType& typeNew=((CIType1C*)newVal)->m_type;
		if(!typeNew.IsValid())
			return SetError(E_FAIL,"Недопустимый тип данных.");
		CMetaDataTypedObj* pObj=(CMetaDataTypedObj*)m_pObj;
		if(typeNew!=pObj->GetType())
		{
			CIMetaData::m_pOneMetaData->SetMDModified();
			pObj->SetType(typeNew);
		}
	}
	return S_OK;
}

STDMETHODIMP CIMetaDataObj::get_Parent(IMetaDataObj **pVal)
{
	CHECK_VALID();
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	long idParent;
	CIMetaData::m_pMetaData->GetFullName(m_pObj->GetID(),&idParent,0);
	if(idParent)
		return CIMetaData::m_pOneMetaData->FindObject(idParent,pVal);
	*pVal=NULL;
	return S_OK;
}
