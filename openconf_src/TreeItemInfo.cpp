// TreeItemInfo.cpp: implementation of the CTreeItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "TreeItemInfo.h"
#include "ocview.h"

HTREEITEM CTreeItemInfo::m_curItem=NULL;
CMapPtrToPtr CTreeItemInfo::m_mapAllItems;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeItemInfo::CTreeItemInfo(LPCTSTR name,int iconCollapsed,int iconExpanded)
{
	m_DisplayName=name;
	m_IconCollapsed=iconCollapsed;
	m_IconExpanded=iconExpanded==-1?iconCollapsed:iconExpanded;
}

CTreeItemInfo::~CTreeItemInfo()
{
	if(COCView::m_pView)
	{
		CTreeCtrl& tree=COCView::m_pView->m_tree;
		HTREEITEM item=GetItemFromData(this);
		item=tree.GetNextItem(item,TVGN_CHILD);
		while(item)
		{
			CTreeItemInfo* pInfo=(CTreeItemInfo*)tree.GetItemData(item);
			if(pInfo)
				delete pInfo;
			item=tree.GetNextItem(item,TVGN_NEXT);
		}
	}
	m_mapAllItems.RemoveKey(this);
}

HTREEITEM CTreeItemInfo::InsertInTree(HTREEITEM pParent)
{
	if(COCView::m_pView)
	{
		HTREEITEM item=COCView::m_pView->m_tree.InsertItem(GetName(),m_IconCollapsed,
			m_IconExpanded,pParent);
		COCView::m_pView->m_tree.SetItemData(item,(DWORD)this);
		m_mapAllItems[this]=item;
		return item;
	}
	return NULL;
}