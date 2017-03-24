// ItemScripts.h: interface for the CItemScripts class.
#ifndef ITEMSCRIPTS_H
#define ITEMSCRIPTS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "treeiteminfo.h"

class CItemScripts : public  CTreeItemInfo  
{
public:
	void InsertScript(CScripting* pScript);
	void OnAction();
	CItemScripts():CTreeItemInfo("Скрипты",ITREE_SCRIPTS_FOLDER){}
	virtual ~CItemScripts();
	virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	virtual void OnSelect();
	virtual void GetContextMenu(CString& list);
	virtual void OnContextMenu(LPCTSTR txt,int pos);
};

#endif // !defined(AFX_ITEMSCRIPTS_H__7BBBFE16_AEC5_4ADB_949F_D98AAD39BF08__INCLUDED_)
