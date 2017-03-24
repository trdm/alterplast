// ItemScripts.cpp: implementation of the CItemScripts class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ItemScripts.h"
#include "scriptmaneger.h"
#include "itemscript.h"
#include "activedoc.h"
#include "oleformsview.h"
#include <afxpriv.h>
#include "htmlhelp.h"
#include "helper.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CItemScripts::~CItemScripts()
{
}

HTREEITEM CItemScripts::InsertInTree(HTREEITEM pParent)
{
	HTREEITEM item=CTreeItemInfo::InsertInTree(pParent);
	CScriptManager* pAllScript=CScriptManager::GetScriptManager();
	if(!pAllScript)
		return item;
	long count=pAllScript->GetScriptCount();
	for(int i=0;i<count;i++)
	{
		CScripting* pScript=pAllScript->GetScriptByNum(i);
		CItemScript* pItem=new CItemScript(pScript);
		pItem->InsertInTree(item);
	}
	return item;
}

void CItemScripts::OnAction()
{
}
void CItemScripts::OnSelect()
{
	CStringArray titles;
	titles.Add("Скрипт");
	titles.Add("Состояние");
	CStringArray data;
	CTreeCtrl& tree=COCView::m_pView->m_tree;
	HTREEITEM me=tree.GetSelectedItem();
	HTREEITEM child=tree.GetNextItem(me,TVGN_CHILD);
	while(child)
	{
		CItemScript* pItem=(CItemScript*)tree.GetItemData(child);
		data.Add(pItem->m_pScript->m_ScriptName);
		data.Add(pItem->m_pScript->GetCurrentState());
		child=tree.GetNextItem(child,TVGN_NEXT);
	}
	COCView::m_pView->ShowTab(this,titles,data);
}
void CItemScripts::GetContextMenu(CString& list)
{
	list="Загрузить скрипт...\nСправка\nО Программе...";
}

void CItemScripts::OnContextMenu(LPCTSTR txt,int pos)
{
	switch(pos){
	case 1:		// Загрузить скрипт
		{
			CScripting* pScript=CScriptManager::GetScriptManager()->LoadScript("");
		}
		break;
	case 2:		// Справка
		{
			CHelper::ShowHelp();
		}
		break;
	case 3:		// О программе
		{
			CHelper::ShowAbout();
		}
		break;
	};
}

void CItemScripts::InsertScript(CScripting *pScript)
{
	CItemScript* pItem=new CItemScript(pScript);
	pItem->InsertInTree(GetItemFromData(this));
}
