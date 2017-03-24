// IAllTypedObj.cpp: implementation of the CIAllTypedObj class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IAllTypedObj.h"
#include "icfgdoc.h"
#include "icfgdocuments.h"
#include "metadata.h"

ITypeInfo* CIAllTypedObj::m_pTypeInfo=NULL;
static CIAllTypedObj* pCurrentObj;

void SplitStr2Array(const CString& str,CStringArray& arr,char delim)
{
	if(str.IsEmpty())
		return;
	LPCTSTR pRead=str,pStart=pRead;
	while(1)
	{
		while(*pRead && *pRead!=delim)
			pRead++;
		CString key=CString(pStart,pRead-pStart);
		key.TrimLeft(" \t\r\n");
		key.TrimRight(" \t\r\n");
		arr.Add(key);
		if(!*pRead)
			break;
		pStart=++pRead;
	}
}
static CIAllMetaInfo* FindParent(CIAllMetaInfo* pRoot,CString path)
{
	CStringArray keys;
	SplitStr2Array(path,keys);
	for(int i=0,size=keys.GetSize();i<size;i++)
	{
		CString key=keys[i];
		pRoot=pRoot->child;
		while(pRoot)
		{
			int cmp=pRoot->name.Compare(key);
			if(cmp>0)
				return NULL;
			if(!cmp)
				break;
			pRoot=pRoot->next;
		}
		if(!pRoot)
			return NULL;
	}
	return pRoot;
}

int CIAllTypedObj::EnumTypedObj(class CString &name,long id,long l2,enum PageType pt)
{
	pCurrentObj->m_count++;
	CIAllMetaInfo* pInfo=pCurrentObj->m_pRoot->InsertObj(name,id);
	pCurrentObj->m_allObjects.Add(pInfo);
	return TRUE;
}

CIAllTypedObj::CIAllTypedObj():m_count(0)
{
	pCurrentObj=this;
	m_pRoot=new CIAllMetaInfo;
	CConfigCont::GetAllTypedItem(EnumTypedObj,0,PageType(0),0);
	pCurrentObj=NULL;
}

CIAllTypedObj::~CIAllTypedObj()
{
	delete m_pRoot;
}

STDMETHODIMP CIAllTypedObj::get_item(VARIANT idx,ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CIAllMetaInfo* pInfo=NULL;
	if(idx.vt!=VT_BSTR && idx.vt!=VT_I4)
		VariantChangeType(&idx,&idx,0,VT_BSTR);
	if(idx.vt==VT_BSTR)
	{
		CString str=(LPCTSTR)_bstr_t(idx.bstrVal);
		pInfo=FindParent(m_pRoot,str);
		if(!pInfo)
			return SetError(E_FAIL,"Не найден объект с именем \"%s\"",str);
	}
	if(!pInfo)
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			return SetError(E_INVALIDARG,"Неправильный тип индекса");
		if(idx.lVal<0 || idx.lVal>=m_allObjects.GetSize())
			return SetError(E_INVALIDARG,"Недопустимый индекс: %i",idx.lVal);
		pInfo=(CIAllMetaInfo*)m_allObjects[idx.lVal];
	}
	while(pInfo->child)
	{
		if(pInfo->child->next)
			return SetError(E_FAIL,"Неполный путь к объекту");
		pInfo=pInfo->child;
	}
	CString path;
	CDocument* pDoc=NULL;
	if(pInfo->kind<4)
	{
		CModuleCont* pCont=NULL;
		CConfigCont::IDToTextModule(pInfo->id,CICfgDocuments::TypesOfModules[pInfo->kind],path,&pCont,0);
		if(pCont)
			pDoc=pCont->GetTextDocument();
	}
	else
	{
		CTypedCont* pCont=NULL;
		CConfigCont::IDToPath(pInfo->id,CICfgDocuments::TypesOfModules[pInfo->kind],path,&pCont,0);
		if(pCont)
			pDoc=pCont->GetWorkBook();
	}
	*pVal=CICfgDoc::CreateDoc(pDoc);
	return S_OK;
}

STDMETHODIMP CIAllTypedObj::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_count;
	return S_OK;
}

STDMETHODIMP CIAllTypedObj::get_ChildsCount(BSTR Parent,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString str=(LPCTSTR)_bstr_t(Parent);
	CIAllMetaInfo* pParent=FindParent(m_pRoot,str);
	if(!pParent)
		return SetError(E_INVALIDARG,"Неправильно указан родитель: \"%s\"",str);
	int i=0;
	pParent=pParent->child;
	while(pParent)
	{
		i++;
		pParent=pParent->next;
	}
	*pVal=i;
	return S_OK;
}

STDMETHODIMP CIAllTypedObj::get_ChildName(BSTR Parent,long idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(idx<0)
		return SetError(E_INVALIDARG,"Неправильный индекс");
	CString str=(LPCTSTR)_bstr_t(Parent);
	CIAllMetaInfo* pParent=FindParent(m_pRoot,str);
	if(!pParent)
		return SetError(E_INVALIDARG,"Неправильно указан родитель: \"%s\"",str);
	pParent=pParent->child;
	while(pParent && idx>0)
	{
		idx--;
		pParent=pParent->next;
	}
	if(idx)
		return SetError(E_INVALIDARG,"Неправильный индекс");
	*pVal=pParent->name.AllocSysString();
	return S_OK;
}

static int CalcAllObject(CIAllMetaInfo* pParent)
{
	if(!pParent)
		return 0;
	int ret;
	if(pParent->child)
		ret=CalcAllObject(pParent->child);
	else
		ret=1;
	ret+=CalcAllObject(pParent->next);
	return ret;
}

STDMETHODIMP CIAllTypedObj::get_ObjectCount(BSTR Parent,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CString str=(LPCTSTR)_bstr_t(Parent);
	CIAllMetaInfo* pParent=FindParent(m_pRoot,str);
	if(!pParent)
		return SetError(E_INVALIDARG,"Неправильно указан родитель: \"%s\"",str);
	*pVal=CalcAllObject(pParent->child);
	return S_OK;
}

CIAllMetaInfo* CIAllMetaInfo::InsertObj(CString _sname,long id)
{
	int modKind=CICfgDocuments::NameToType(_sname);	
	CString fullName=GetModuleFullName(id,_sname,1,CIMetaData::m_pMetaData);
	CStringArray keys;
	SplitStr2Array(fullName,keys);
	int size=keys.GetSize()-(modKind<4?0:1);
	CIAllMetaInfo* pParent=this;
	for(int i=0;i<size;i++)
	{
		CString keyName=keys[i];
		CIAllMetaInfo* pInsert=pParent->child;
		CIAllMetaInfo* pPrev=NULL;
		bool found=false;
		while(pInsert)
		{
			int cmp=pInsert->name.Compare(keyName);
			if(cmp>=0)
			{
				if(!cmp)
					found=true;
				break;
			}
			pPrev=pInsert;
			pInsert=pInsert->next;
		}
		if(!found)
		{
			CIAllMetaInfo* pNew=new CIAllMetaInfo(id,modKind,keyName);
			pNew->parent=pParent;
			if(pPrev)
			{
				pNew->next=pPrev->next;
				pPrev->next=pNew;
			}
			else	// либо нет первого, либо он один
			{
				if(!pParent->child)
					pParent->child=pNew;
				else
				{
					if(pParent->child->name>keyName)
					{
						pNew->next=pParent->child;
						pParent->child=pNew;
					}
					else
						pParent->child->next=pNew;
				}
			}
			pInsert=pNew;
		}
		pParent=pInsert;
	}
	return pParent;
}
