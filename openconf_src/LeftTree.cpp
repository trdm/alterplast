// LeftTree.cpp : implementation file
#include "stdafx.h"
#include "configsvcimpl.h"
#include "LeftTree.h"
#include "ocview.h"
#include "treeiteminfo.h"
#include "itemscripts.h"
#include "itemscript.h"
#include "pluginmanager.h"
#include "pluginitem.h"

CLeftTree::CLeftTree()
{
	m_click.x=m_click.y=0;
}

CLeftTree::~CLeftTree()
{
}


BEGIN_MESSAGE_MAP(CLeftTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CLeftTree)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftTree message handlers
static CStringArray menuKeys;

static void ProcessMenu(HMENU menu,LPCTSTR& ptr,int& level,int& count)
{
	int newLevel;
	while(*ptr)
	{
		LPCTSTR pStart=ptr;
		DWORD flag=0;
		while(*ptr && *ptr!='\r' && *ptr!='\n')
		{
			if(*ptr=='|')
			{
				if(ptr[1]=='|')
					ptr++;
				else
					break;
			}
			ptr++;
		}
		CString name(pStart,ptr-pStart);
		CString key;
		name.Replace("||","|");
		if(*ptr=='|')
		{
			ptr++;
			while(*ptr && *ptr!='\r' && *ptr!='\n' && *ptr!='|')
			{
				if(*ptr=='c' || *ptr=='C')
					flag|=MF_CHECKED;
				else if(*ptr=='d' || *ptr=='D')
					flag|=MF_GRAYED;
				ptr++;
			}
			if(*ptr=='|')
			{
				pStart=++ptr;
				while(*ptr && *ptr!='\r' && *ptr!='\n')
					ptr++;
				key=CString(pStart,ptr-pStart);
			}
		}
		while(*ptr && (*ptr=='\r' || *ptr=='\n'))
			ptr++;
		if(*ptr)
		{
			newLevel=0;
			while(*ptr && *ptr=='\t')
				*ptr++,newLevel++;
		}
		else
			newLevel=level;
		if(newLevel>level)
		{
			HMENU hNewMenu=CreatePopupMenu();
			ProcessMenu(hNewMenu,ptr,newLevel,count);
			AppendMenu(menu,MF_POPUP|MF_STRING,(UINT)hNewMenu,name);
		}
		else
		{
			if(name=="-")
				AppendMenu(menu,MF_SEPARATOR,0,NULL);
			else
			{
				AppendMenu(menu,MF_STRING|flag,++count,name);
				menuKeys.Add(key);
			}

		}
		if(newLevel<level)
			break;
	}
	level=newLevel;
}
void CLeftTree::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	HTREEITEM item=GetSelectedItem();
	if(!item)
		return;
	if(!m_click.x && !m_click.y)
	{
		CRect rc;
		if(GetItemRect(item,rc,TRUE))
		{
			if(rc.bottom>0 && rc.left>0)
				m_click.x=rc.left,m_click.y=rc.bottom;
		}
	}
	CPoint pt=m_click;
	m_click.x=m_click.y=0;
	CTreeItemInfo* pInfo=(CTreeItemInfo*)GetItemData(item);
	if(pInfo)
	{
		CString items;
		pInfo->GetContextMenu(items);
		items.TrimLeft();
		items.TrimRight();
		if(items.IsEmpty())
			return;
		HMENU menu=CreatePopupMenu();
		LPCTSTR ptr=items;int lev=0;
		int count=0;
		menuKeys.RemoveAll();
		ProcessMenu(menu,ptr,lev,count);
		ClientToScreen(&pt);
		UINT ret=TrackPopupMenu(menu,
			TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD,
			pt.x,pt.y,0,m_hWnd,NULL);
		if(ret)
		{
			CString key=menuKeys[ret-1];
			if(key.IsEmpty())
			{
				MENUITEMINFO info;
				info.cbSize=sizeof(info);
				info.fMask=MIIM_TYPE;
				info.cch=512;
				info.dwTypeData=key.GetBuffer(512);
				GetMenuItemInfo(menu,ret,FALSE,&info);
				key.ReleaseBuffer();
			}
			pInfo->OnContextMenu(key,ret);
		}
		menuKeys.RemoveAll();
		DestroyMenu(menu);
	}
}

void CLeftTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT fl;
	HTREEITEM item=HitTest(point,&fl);
	if(item)
	{
		m_click=point;
		SelectItem(item);
	}
}

void CLeftTree::OnDestroy() 
{
	HTREEITEM item=GetNextItem(NULL,TVGN_ROOT);
	while(item)
	{
		CTreeItemInfo* pInfo=(CTreeItemInfo*)GetItemData(item);
		if(pInfo)
			delete pInfo;
		item=GetNextItem(item,TVGN_NEXT);
	}
	CTreeCtrl::OnDestroy();
}

void CLeftTree::RemoveChilds(CTreeItemInfo *pInfo)
{
	HTREEITEM item=CTreeItemInfo::GetItemFromData(pInfo);
	item=GetNextItem(item,TVGN_CHILD);
	while(item)
	{
		CTreeItemInfo* pItem=(CTreeItemInfo*)GetItemData(item);
		if(pItem) delete pItem;
		HTREEITEM delItem=item;
		item=GetNextItem(item,TVGN_NEXT);
		DeleteItem(delItem);
	}
}

void CLeftTree::InsertScript(CScripting *pScript)
{
	HTREEITEM item=GetNextItem(NULL,TVGN_ROOT);
	while(item)
	{
		CItemScripts* pScripts=(CItemScripts*)GetItemData(item);
		if(pScripts->m_DisplayName=="Скрипты")
		{
			pScripts->InsertScript(pScript);
			return;
		}
		item=GetNextItem(item,TVGN_NEXT);
	}
}

void CLeftTree::RemoveScript(CScripting *pScript)
{
	HTREEITEM item=GetNextItem(NULL,TVGN_ROOT);
	while(item)
	{
		CItemScripts* pScripts=(CItemScripts*)GetItemData(item);
		if(pScripts->m_DisplayName=="Скрипты")
		{
			item=GetNextItem(item,TVGN_CHILD);
			while(item)
			{
				CItemScript* pS=(CItemScript*)GetItemData(item);
				if(pS->m_pScript==pScript)
				{
					delete pS;
					DeleteItem(item);
					return;
				}
				item=GetNextItem(item,TVGN_NEXT);
			}
			return;
		}
		item=GetNextItem(item,TVGN_NEXT);
	}
}

void CLeftTree::InsertPlugin(void *pInfo)
{
	CPluginManager::Info* pPlugin=(CPluginManager::Info*)pInfo;
	HTREEITEM item=GetNextItem(NULL,TVGN_ROOT);
	while(item)
	{
		CTreeItemInfo* pPlugins=(CTreeItemInfo*)GetItemData(item);
		if(pPlugins->m_DisplayName=="Плагины")
		{
			CPluginItem* pItem=new CPluginItem(pPlugin);
			pItem->InsertInTree(item);
			CPluginItem::CreatePluginPage(pPlugin);
			return;
		}
		item=GetNextItem(item,TVGN_NEXT);
	}
}
