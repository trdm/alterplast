// PluginItem.cpp: implementation of the CPluginItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "PluginItem.h"
#include "Pluginmacros.h"
#include "ocview.h"
#include "Pluginpagedoc.h"
#include "Pluginpageview.h"

CPluginItem::~CPluginItem(){}
HICON GetIcon(CPluginManager::Info* pInfo)
{
	HICON hIcon;
	UINT ret=ExtractIconEx(pInfo->path,pInfo->IconNum,NULL,&hIcon,1);
	if(!ret && pInfo->IconNum)
		ret=ExtractIconEx(pInfo->path,0,NULL,&hIcon,1);
	return ret?hIcon:NULL;
}
CPluginItem::CPluginItem(CPluginManager::Info* pInfo):CTreeItemInfo(pInfo->name,ITREE_SCRIPTS_PLUGIN),m_pInfo(pInfo)
{
	HICON hIcon=GetIcon(pInfo);
	if(hIcon)
	{
		m_IconCollapsed=COCView::m_ImageList.Add(hIcon);
		m_IconExpanded=m_IconCollapsed;
		DestroyIcon(hIcon);
	}
}


HTREEITEM CPluginItem::InsertInTree(HTREEITEM pParent)
{
	HTREEITEM me=CTreeItemInfo::InsertInTree(pParent);
	if(m_pInfo->pPlugin!=NULL)
	{
		if(m_pInfo->ShowMacros)
		{
			CStringArray macros;
			CDWordArray dispIDs;
			ExtractMacrosFromDispatch(m_pInfo->pPlugin,macros,dispIDs);
			for(int i=0;i<macros.GetSize();i++)
			{
				CPluginMacros* pMacros=new CPluginMacros(macros[i],dispIDs[i]);
				pMacros->InsertInTree(me);
			}
		}
	}
	return me;
}

void CPluginItem::InvokeMacros(DISPID dispID)
{
	if(m_pInfo->pPlugin)
	{
		DISPPARAMS none={0,0,0,0};
		m_pInfo->pPlugin->Invoke(dispID,IID_NULL,0,DISPATCH_METHOD,&none,NULL,NULL,NULL);
	}
}

void CPluginItem::OnAction()
{
}

void CPluginItem::OnSelect()
{
	COCView::m_pView->CreatePluginControl(this);
}

void CPluginItem::CreateConfigPages()
{
	CPluginManager::Info* pInfo;
	CMapStringToPtr& map=CPluginManager::GetPM()->m_Plugins;
	for(POSITION pos=map.GetStartPosition();pos;)
	{
		CString key;
		map.GetNextAssoc(pos,key,(void*&)pInfo);
		CreatePluginPage(pInfo);
	}
}

void CPluginItem::GetContextMenu(CString& list)
{
	if(m_pInfo->pPlugin!=NULL)
	{
		IConfigPluginPtr pPlugin=m_pInfo->pPlugin;
		if(pPlugin!=NULL)
		{
			VARIANT varResult;
			VariantInit(&varResult);
			try{
				HRESULT hr=pPlugin->GetPluginCaps(capGetMenu,&varResult);
				if(S_OK==hr)
				{
					if(!(varResult.vt==VT_EMPTY || varResult.vt==VT_ERROR))
					{
						_bstr_t bstrRet=varResult;
						list=(LPCTSTR)bstrRet;
					}
				}
			}
			catch(_com_error err)
			{
				DoMsgLine(err.ErrorMessage(),mmRedErr);
			}
			catch(...)
			{
				DoMsgLine("Error GetMenu from plugin",mmRedErr);
			}
			VariantClear(&varResult);
		}
	}
}

void CPluginItem::OnContextMenu(LPCTSTR name,int pos)
{
	if(m_pInfo->pPlugin!=NULL)
	{
		IConfigPluginPtr pPlugin=m_pInfo->pPlugin;
		if(pPlugin!=NULL)
		{
			VARIANT varResult;
			VariantInit(&varResult);
			VARIANT varParam;
			varParam.vt=VT_BSTR;
			varParam.bstrVal=CString(name).AllocSysString();
			try{
				HRESULT hr=pPlugin->DoAction(paMenu,varParam,&varResult);
			}
			catch(_com_error err)
			{
				DoMsgLine(err.ErrorMessage(),mmRedErr);
			}
			catch(...)
			{
				DoMsgLine("Error DoAction from plugin",mmRedErr);
			}
			VariantClear(&varResult);
			VariantClear(&varParam);
		}
	}
}

void CPluginItem::CreatePluginPage(CPluginManager::Info* pInfo)
{
	IOleControlPtr pCtrl=pInfo->pPlugin;
	if(pCtrl!=NULL)
	{
		if(pInfo->OpenIn==1)
		{
			SECTabWndBase* pWnd=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
			if(!pWnd)
			{
				DoMsgLine("Ќе удалось создать страницу контрола.",mmRedErr);
				return;
			}
			CCreateContext ctx;
			ctx.m_pCurrentDoc=(CDocument*)RUNTIME_CLASS(CPluginPageDoc)->CreateObject();
			((CPluginPageDoc*)ctx.m_pCurrentDoc)->m_pPlugin=pInfo->pPlugin;
			ctx.m_pCurrentFrame=NULL;
			ctx.m_pLastView=NULL;
			ctx.m_pNewDocTemplate=NULL;
			ctx.m_pNewViewClass=RUNTIME_CLASS(CPluginPageView);
			if(!pWnd->SECTabWndBase::AddTab(ctx.m_pNewViewClass,pInfo->name,&ctx,0))
				return;
			HICON hIcon=GetIcon(pInfo);
			if(hIcon)
				pWnd->SECTabWndBase::SetTabIcon(pWnd->GetTabCount()-1,hIcon);
		}
	}
}
