// ISelfScript.cpp: implementation of the CISelfScript class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ISelfScript.h"
#include "scripting.h"

ITypeInfo* CISelfScript::m_pTypeInfo=NULL;

CISelfScript::CISelfScript()
{
}

CISelfScript::~CISelfScript()
{
	ResetAdded();
}

STDMETHODIMP CISelfScript::get_Name(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=m_pScript->m_ScriptName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CISelfScript::AddNamedItem(BSTR Name,IDispatch *Object,VARIANT_BOOL AsGlobal)
{
	CString name=(LPCTSTR)_bstr_t(Name);
	if(name.IsEmpty())
		return SetError(E_INVALIDARG,"Пустое имя");
	name.MakeLower();
	void* pVal;
	if(m_Objects.Lookup(name,pVal))
		return SetError(E_INVALIDARG,"Имя \"%s\" уже зарегистрировано",(LPCTSTR)_bstr_t(Name));
	m_Objects[name]=Object;
	Object->AddRef();
	m_pScript->m_pEngine->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	HRESULT hr=m_pScript->m_pEngine->AddNamedItem(Name,SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE|(AsGlobal?SCRIPTITEM_GLOBALMEMBERS:0));
	m_pScript->m_pEngine->SetScriptState(SCRIPTSTATE_CONNECTED);

	if(S_OK!=hr)
	{
		m_Objects.RemoveKey(name);
		Object->Release();
		return hr;
	}
	return S_OK;
}

void CISelfScript::ResetAdded()
{
	IDispatch* pDisp;
	CString name;
	for(POSITION pos=m_Objects.GetStartPosition();pos;)
	{
		m_Objects.GetNextAssoc(pos,name,(void*&)pDisp);
		pDisp->Release();
	}
	m_Objects.RemoveAll();
}
