// RMenuItem.h: interface for the CRMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMENUITEM_H__811B37FC_D19D_4304_AA66_E35237775176__INCLUDED_)
#define AFX_RMENUITEM_H__811B37FC_D19D_4304_AA66_E35237775176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\\MyContext\\mycontextbase.h"
#include "MenuItem.h"
#include "RMenu.h"

class CMenuItem;

class CRMenuItem : public CMyContextBase  
{
	DECLARE_DYNCREATE(CRMenuItem);

public:
	CRMenuItem();
	virtual ~CRMenuItem();

	BOOL funcAdd(CValue& RetVal, CValue **params);
	BOOL funcAddMenu(CValue& RetVal, CValue **params);
	BOOL funcAddCommand(CValue& RetVal, CValue **params);
	BOOL funcAddSeparator(CValue& RetVal, CValue **params);
	BOOL funcDelete(CValue& RetVal, CValue **params);
	BOOL funcDeleteAll(CValue& RetVal, CValue **params);
	BOOL funcGet(CValue& RetVal, CValue **params);
	BOOL funcCount(CValue& RetVal, CValue **params);
	BOOL funcReadOnly(CValue& RetVal, CValue **params);
	BOOL funcCheck(CValue& RetVal, CValue **params);
	BOOL funcSetPicture(CValue& RetVal, CValue **params);
	BOOL funcGetCommandID(CValue& RetVal, CValue **params);
	
	BOOL getID(CValue& Value) const;
	BOOL getType(CValue& Value) const;
	BOOL getText(CValue& Value) const;
	BOOL setText(CValue const& Value);
	BOOL getScript(CValue& Value) const;
	BOOL setScript(CValue const& Value);
	BOOL getTips(CValue& Value) const;
	BOOL setTips(CValue const& Value);

	BOOL NoDefaultValue(int nParam, CValue* param) const;
	BOOL DefaultValue1(int nParam, CValue* param) const; // первый параметр обязательный
	BOOL DefaultValue2(int nParam, CValue* param) const; // первые два параметра обязательные
	BOOL DefaultValueBase(int nParam, CValue* param) const; // все параметры -1

	RDECLARE_MY_CONTEXT;

	CMenuItem* m_pMenuItem;
};

#endif // !defined(AFX_RMENUITEM_H__811B37FC_D19D_4304_AA66_E35237775176__INCLUDED_)
