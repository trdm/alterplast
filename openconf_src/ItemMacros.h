// ItemMacros.h: interface for the CItemMacros class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMACROS_H__1BBF8F8E_5F30_4D95_AEF4_4A5CB571B4D8__INCLUDED_)
#define AFX_ITEMMACROS_H__1BBF8F8E_5F30_4D95_AEF4_4A5CB571B4D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemMacros : public  CTreeItemInfo  
{
public:
	void OnSelect();
	CItemMacros(LPCTSTR name,DISPID dispID):CTreeItemInfo(name,ITREE_SCRIPTS_MACROS),m_dispID(dispID){}
	virtual ~CItemMacros();
	//virtual HTREEITEM InsertInTree(HTREEITEM pParent);
	virtual void OnAction();
	DISPID m_dispID;
};

#endif // !defined(AFX_ITEMMACROS_H__1BBF8F8E_5F30_4D95_AEF4_4A5CB571B4D8__INCLUDED_)
