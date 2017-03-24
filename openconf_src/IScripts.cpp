// IScripts.cpp: implementation of the CIScripts class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IScripts.h"
#include "scriptmaneger.h"

ITypeInfo* CIScripts::m_pTypeInfo=NULL;
CIScripts* CIScripts::m_pScripts=NULL;

CIScripts::CIScripts()
{
	m_pScripts=this;
}

CIScripts::~CIScripts()
{
	m_pScripts=NULL;
}

STDMETHODIMP CIScripts::get_item(VARIANT Idx,IDispatch **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CScripting* pScript=GetScript(Idx);
	if(!pScript)
		return E_FAIL;
	return pScript->m_pEngine->GetScriptDispatch(NULL,pVal);
}

STDMETHODIMP CIScripts::get_Count(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CScriptManager::GetScriptManager()->GetScriptCount();
	return S_OK;
}

STDMETHODIMP CIScripts::Load(BSTR Path,IDispatch **pRetVal)
{
	if(IsBadWritePtr(pRetVal,sizeof(*pRetVal)))
		return E_POINTER;
	_bstr_t bPath=Path;
	CScripting* pScript=CScriptManager::GetScriptManager()->LoadScript((LPCTSTR)bPath);
	if(!pScript)
		return SetError(E_FAIL,"Не удалось загрузить скрипт %s",(LPCTSTR)bPath);
	return pScript->m_pEngine->GetScriptDispatch(NULL,pRetVal);
}

STDMETHODIMP CIScripts::ReLoad(VARIANT Idx)
{
	CScripting* pScript=GetScript(Idx);
	if(!pScript)
		return E_FAIL;
	pScript->ReloadScript();
	return S_OK;
}

STDMETHODIMP CIScripts::UnLoad(VARIANT Idx)
{
	CScripting* pScript=GetScript(Idx);
	if(!pScript)
		return E_FAIL;
	CScriptManager::GetScriptManager()->UnLoadScript(pScript);
	return S_OK;
}

STDMETHODIMP CIScripts::get_Path(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CScripting* pScript=GetScript(idx);
	if(!pScript)
		return E_FAIL;
	*pVal=pScript->m_FileName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIScripts::get_Name(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CScripting* pScript=GetScript(idx);
	if(!pScript)
		return E_FAIL;
	*pVal=pScript->m_ScriptName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIScripts::get_Engine(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CScripting* pScript=GetScript(idx);
	if(!pScript)
		return E_FAIL;
	*pVal=pScript->m_EngineName.AllocSysString();
	return S_OK;
}

CScripting* CIScripts::GetScript(VARIANT& idx)
{
	CScripting* pScript=NULL;
	CScriptManager* pSM=CScriptManager::GetScriptManager();
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bname=idx;
		pScript=pSM->GetScriptByName((LPCTSTR)bname);
		if(!pScript)
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
			pScript=pSM->GetScriptByNum(idx.lVal);
			if(!pScript)
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
		}
	}
	return pScript;
}

void CIScripts::Done()
{
	if(m_pScripts)
		delete m_pScripts;
}
