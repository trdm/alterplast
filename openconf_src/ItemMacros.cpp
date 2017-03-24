// ItemMacros.cpp: implementation of the CItemMacros class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "ocview.h"
#include "itemscript.h"
#include "ItemMacros.h"

//////////////////////////////////////////////////////////////////////

CItemMacros::~CItemMacros()
{
}
/*
HTREEITEM CItemMacros::InsertInTree(HTREEITEM pParent)
{
}
*/

void CItemMacros::OnAction()
{
	/*IDispatch* pv77=NULL;
	CLSID cls77;
	CLSIDFromProgID(L"V77.Application",&cls77);
	HRESULT hr=CoCreateInstance(cls77,NULL,CLSCTX_LOCAL_SERVER,IID_IDispatch,(void**)&pv77);
	if(!pv77)
		return;
	DISPID dispID;
	DISPPARAMS args={0,0,0,0};
	VARIANT vars[3];	// Параметры для вызова Initialize
	// Получаем RMTrade (сразу в vars[2])
	BSTR rmTrade=L"RMTrade";
	hr=pv77->GetIDsOfNames(IID_NULL,&rmTrade,1,0,&dispID);
	hr=pv77->Invoke(dispID,IID_NULL,0,DISPATCH_PROPERTYGET,&args,&vars[2],NULL,NULL);
	// Вызываем Initialize
	BSTR init=L"Initialize";
	hr=pv77->GetIDsOfNames(IID_NULL,&init,1,0,&dispID);
	args.cArgs=3;
	args.rgvarg=vars;
	vars[0]=_variant_t("NO_SPLASH_SHOW");
	vars[1]=_variant_t("/DC:\\test");
	hr=pv77->Invoke(dispID,IID_NULL,0,DISPATCH_METHOD,&args,NULL,NULL,NULL);
	pv77->Release();
	return;
	*/
	CItemScript* pScript=(CItemScript*)GetParentItem();
	if(pScript)
		pScript->InvokeMacros(m_dispID);
}

void CItemMacros::OnSelect()
{
	CTreeItemInfo::GetParentItem()->OnSelect();
}
