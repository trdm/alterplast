// PluginsItem.h: interface for the CPliginsItem class.
#ifndef PLIGINSITEM_H
#define PLIGINSITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeItemInfo.h"

class CPluginsItem : public CTreeItemInfo  
{
public:
	void OnSelect();
	CPluginsItem():CTreeItemInfo("Плагины",ITREE_SCRIPTS_PLUGINS){}
	virtual ~CPluginsItem(){}
	virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	virtual void GetContextMenu(CString& list);
	virtual void OnContextMenu(LPCTSTR name,int pos);
};

#endif
