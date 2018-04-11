// RMenu.h: interface for the CRMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMENU_H__C7B2B46E_B25E_4FD6_80DE_F73DDCF6D9BC__INCLUDED_)
#define AFX_RMENU_H__C7B2B46E_B25E_4FD6_80DE_F73DDCF6D9BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\\MyContext\\mycontextbase.h"
#include "MenuItem.h"

class CMenuItem;

class CRMenu : public CMyContextBase
{
	DECLARE_DYNCREATE(CRMenu);

public:
	CRMenu();
	virtual ~CRMenu();

	BOOL funcAdd(CValue& RetVal, CValue **params);
	BOOL funcAddMenu(CValue& RetVal, CValue **params);
	BOOL funcAddSeparator(CValue& RetVal, CValue **params);
	BOOL funcDelete(CValue& RetVal, CValue **params);
	BOOL funcDeleteAll(CValue& RetVal, CValue **params);
	BOOL funcGet(CValue& RetVal, CValue **params);
	BOOL funcGetSystem(CValue& RetVal, CValue **params);
	BOOL funcDeleteSystem(CValue& RetVal, CValue **params);
	BOOL funcCount(CValue& RetVal, CValue **params);
	BOOL funcSendMessage(CValue& RetVal, CValue **params);
	
	BOOL funcRedrawMenu(CValue& RetVal, CValue **params);
	BOOL funcLoadInterface(CValue& RetVal, CValue **params);
	BOOL funcGetList(CValue& RetVal, CValue **params);
	BOOL funcTest(CValue& RetVal, CValue **params);

	BOOL NoDefaultValue(int nParam, CValue* param) const;
	BOOL DefaultValue2(int nParam, CValue* param) const; // первые два параметра обязательные

	RDECLARE_MY_CONTEXT;

	CMenuItem* m_pMenuItem;

private:
	typedef struct _LoadedItemData {
		CString Code, Tips;
	} LoadedItemData;

	void LoadMenuFromMD(IStream* pStream, CMenuItem* pParentMenuItem, CMap<WORD, WORD, LoadedItemData, LoadedItemData>& mapCmdID2Command, CString strID);
	void LoadSubInterface(CString strSubIntName, CString& strTheInterfaceStrName, CItemList& lst, IStoragePtr, CList<CString, CString>& lstExclCommands);
};

#endif // !defined(AFX_RMENU_H__C7B2B46E_B25E_4FD6_80DE_F73DDCF6D9BC__INCLUDED_)
