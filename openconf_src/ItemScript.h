// ItemScript.h: interface for the CItemScript class.
#ifndef ITEMSCRIPT_H
#define ITEMSCRIPT_H
#include "treeiteminfo.h"
#include "scripting.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemScript : public  CTreeItemInfo  
{
public:
	void FillMacros();
	void OnSelect();
	void InvokeMacros(DISPID dispID);
	CItemScript(CScripting* pScript):CTreeItemInfo(pScript->m_ScriptName,ITREE_SCRIPTS_SCRIPT),m_pScript(pScript)
	{
		SHFILEINFO  shfi;
		if (SHGetFileInfo (pScript->m_FileName,0,&shfi,sizeof (SHFILEINFO),SHGFI_ICON|SHGFI_SMALLICON))
		{
			m_IconCollapsed=COCView::m_ImageList.Add(shfi.hIcon);
			m_IconExpanded=m_IconCollapsed;
			DestroyIcon(shfi.hIcon);
		}
	}
	virtual ~CItemScript();
	virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	virtual void GetContextMenu(CString& list);
	virtual void OnContextMenu(LPCTSTR name,int pos);
	virtual void OnAction();
	CScripting* m_pScript;
};

#endif
