// IPlugins.cpp: implementation of the CIPlugins class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "IPlugins.h"

ITypeInfo* CIPlugins::m_pTypeInfo=NULL;
CIPlugins* CIPlugins::m_pPlugins=NULL;

CIPlugins::CIPlugins()
{
	m_pPlugins=this;
}

CIPlugins::~CIPlugins()
{

}

STDMETHODIMP CIPlugins::get_item(VARIANT Idx,IDispatch **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	PINFO pInfo=GetIdx(Idx);
	if(!pInfo)
		return E_FAIL;
	*pVal=pInfo->pPlugin;
	if(*pVal)
		(*pVal)->AddRef();
	return S_OK;
}

STDMETHODIMP CIPlugins::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CPluginManager::GetPM()->m_Plugins.GetCount();
	return S_OK;
}

STDMETHODIMP CIPlugins::LoadPlugin(BSTR From)
{
	_bstr_t path=From;
	CPluginManager::GetPM()->LoadPlugin((LPCTSTR)path);
	return S_OK;
}

STDMETHODIMP CIPlugins::get_Path(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	PINFO pInfo=GetIdx(idx);
	if(!pInfo)
		return E_FAIL;
	*pVal=pInfo->path.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIPlugins::get_Name(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	PINFO pInfo=GetIdx(idx);
	if(!pInfo)
		return E_FAIL;
	*pVal=pInfo->name.AllocSysString();
	return S_OK;
}

PINFO CIPlugins::GetIdx(VARIANT& idx)
{
	PINFO pInfo=NULL;
	CPluginManager* pPM=CPluginManager::GetPM();
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bname=idx;
		pInfo=pPM->GetPluginByName((LPCTSTR)bname);
		if(!pInfo)
			SetError(E_FAIL,"Недопустимый индекс: %s",(LPCTSTR)bname);
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса");
		else
		{
			pInfo=pPM->GetPluginByNum(idx.lVal);
			if(!pInfo)
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
		}
	}
	return pInfo;
}

void CIPlugins::Done()
{
	if(m_pPlugins)
		delete m_pPlugins;
}
