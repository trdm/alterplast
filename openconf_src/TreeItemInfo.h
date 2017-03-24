// TreeItemInfo.h: interface for the CTreeItemInfo class.
#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H
#include "ocview.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTreeItemInfo  
{
public:
	CTreeItemInfo(LPCTSTR name,int iconCollapsed,int iconExpanded=-1);
	virtual ~CTreeItemInfo();
	virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	virtual LPCTSTR GetName(){return m_DisplayName;}
	virtual void OnAction(){}
	virtual void OnSelect(){}
	virtual void GetContextMenu(CString& list){}
	virtual void OnContextMenu(LPCTSTR name,int pos){}
	virtual void Refresh(){}

	static CTreeItemInfo* GetCurItem()
	{
		m_curItem=COCView::m_pView->m_tree.GetSelectedItem();
		return (CTreeItemInfo*)COCView::m_pView->m_tree.GetItemData(m_curItem);
	}
	static CTreeItemInfo* GetParentItem(HTREEITEM item=NULL)
	{
		HTREEITEM parent=COCView::m_pView->m_tree.GetParentItem(item?item:m_curItem);
		return (CTreeItemInfo*)COCView::m_pView->m_tree.GetItemData(parent);
	}
	CString m_DisplayName;
	int m_IconExpanded;
	int m_IconCollapsed;
	static HTREEITEM m_curItem;
	static CMapPtrToPtr m_mapAllItems;
	static HTREEITEM GetItemFromData(CTreeItemInfo* pItem){return (HTREEITEM)m_mapAllItems[pItem];}
};
#define ITREE_SCRIPTS_FOLDER		0
#define ITREE_SCRIPTS_SCRIPT		1
#define ITREE_SCRIPTS_MACROS		2
#define ITREE_SCRIPTS_PLUGINS		3
#define ITREE_SCRIPTS_PLUGIN		4

#endif
