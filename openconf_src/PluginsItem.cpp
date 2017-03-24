// PluginsItem.cpp: implementation of the CPliginsItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "PluginsItem.h"
#include "PluginItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HTREEITEM CPluginsItem::InsertInTree(HTREEITEM pParent)
{
	HTREEITEM me=CTreeItemInfo::InsertInTree(pParent);
	CPluginManager::Info* pInfo;
	CMapStringToPtr& map=CPluginManager::GetPM()->m_Plugins;
	for(POSITION pos=map.GetStartPosition();pos;)
	{
		CString key;
		map.GetNextAssoc(pos,key,(void*&)pInfo);
		CPluginItem* pItem=new CPluginItem(pInfo);
		pItem->InsertInTree(me);
	}
	return me;
}

void CPluginsItem::OnSelect()
{
	CStringArray titles;
	titles.Add("Плагин");
	titles.Add("Тип");
	titles.Add("ProgID");
	CStringArray data;

	CPluginManager::Info* pInfo;
	CMapStringToPtr& map=CPluginManager::GetPM()->m_Plugins;
	for(POSITION pos=map.GetStartPosition();pos;)
	{
		CString key;
		map.GetNextAssoc(pos,key,(void*&)pInfo);
		data.Add(pInfo->name);
		CString text;
		if(pInfo->pPlugin!=NULL)
		{
			IOleControlPtr pCtrl=pInfo->pPlugin;
			if(pCtrl!=NULL)
				text="Визуальный ";
			IConfigPluginPtr pConfig=pInfo->pPlugin;
			if(pConfig!=NULL)
				text+="Совместимый ";
			text+="Плагин";
		}
		else
			text="dll";
		data.Add(text);
		data.Add(pInfo->progID.IsEmpty()?" ":pInfo->progID);
	}
	COCView::m_pView->ShowTab(this,titles,data);
}

void CPluginsItem::GetContextMenu(CString& list)
{
	list="Загрузить плагин...";
}

void CPluginsItem::OnContextMenu(LPCTSTR name,int pos)
{
	CPluginManager::GetPM()->LoadPlugin("");
}
