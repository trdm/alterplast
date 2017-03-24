// PluginItem.h: interface for the CPluginItem class.
#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeItemInfo.h"
#include "pluginmanager.h"

class CPluginItem : public CTreeItemInfo  
{
public:
	void OnSelect();
	CPluginItem(CPluginManager::Info* pInfo);
	virtual ~CPluginItem();
	virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	void InvokeMacros(DISPID dispID);
	void OnAction();
	CPluginManager::Info* m_pInfo;
	static void CreateConfigPages();
	static void CreatePluginPage(CPluginManager::Info* pInfo);
	virtual void GetContextMenu(CString& list);
	virtual void OnContextMenu(LPCTSTR name,int pos);
};
#endif
