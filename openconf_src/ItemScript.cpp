// ItemScript.cpp: implementation of the CItemScript class.

#include "stdafx.h"
#include "configsvcimpl.h"
#include "ItemScript.h"
#include "itemmacros.h"
#include "scriptmaneger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CItemScript::~CItemScript()
{
	m_pScript->m_pViewItem=NULL;
}

HTREEITEM CItemScript::InsertInTree(HTREEITEM pParent)
{
	m_pScript->m_pViewItem=this;
	HTREEITEM item=CTreeItemInfo::InsertInTree(pParent);
	FillMacros();
	return item;
}

void CItemScript::InvokeMacros(DISPID dispID)
{
	if(m_pScript)
	{
		IDispatchPtr pDisp=NULL;
		m_pScript->m_pEngine->GetScriptDispatch(NULL,&pDisp);
		if(pDisp!=NULL)
		{
			DISPPARAMS none={0,0,0,0};
			pDisp->Invoke(dispID,IID_NULL,0,DISPATCH_METHOD,&none,NULL,NULL,NULL);
		}
	}
}

void CItemScript::OnSelect()
{
	CStringArray titles;
	CStringArray data;
	titles.Add("Свойство");
	titles.Add("Значение");
	data.Add("Путь");
	data.Add(m_pScript->m_FileName);
	data.Add("ScriptEngine");
	data.Add(m_pScript->m_EngineName);
	data.Add("Состояние");
	data.Add(m_pScript->GetCurrentState());
	COCView::m_pView->ShowTab(this,titles,data);
}

void CItemScript::GetContextMenu(CString& list)
{
	list="Открыть\n"
		"Перезагрузить\n"
		"Выгрузить";
}
void CItemScript::OnAction()
{
	if(!m_pScript->m_FileName.IsEmpty())
	{
		CConfigSvcImpl::m_pTheService->OpenFile(m_pScript->m_FileName);
	}
}

void CItemScript::OnContextMenu(LPCTSTR name,int pos)
{
	switch(pos){
	case 1:
		OnAction();
		break;
	case 2:
		m_pScript->ReloadScript();
		break;
	case 3:
		CScriptManager::GetScriptManager()->UnLoadScript(m_pScript);
		break;
	};
}

void CItemScript::FillMacros()
{
	COCView::m_pView->m_tree.RemoveChilds(this);
	IDispatchPtr pDisp;
	m_pScript->m_pEngine->GetScriptDispatch(NULL,&pDisp);
	if(pDisp!=NULL)
	{
		HTREEITEM item=GetItemFromData(this);
		CStringArray macros;
		CDWordArray dispIDs;
		ExtractMacrosFromDispatch(pDisp,macros,dispIDs);
		for(int i=0;i<macros.GetSize();i++)
		{
			CItemMacros* pMacros=new CItemMacros(macros[i],dispIDs[i]);
			pMacros->InsertInTree(item);
		}
	}
}
