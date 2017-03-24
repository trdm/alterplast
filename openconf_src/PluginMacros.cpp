// PluginMacros.cpp: implementation of the CPluginMacros class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "PluginMacros.h"
#include "Pluginitem.h"

void CPluginMacros::OnAction()
{
	CPluginItem* pPlugin=(CPluginItem*)GetParentItem();
	if(pPlugin)
		pPlugin->InvokeMacros(m_dispID);
}

void CPluginMacros::OnSelect()
{
	CTreeItemInfo::GetParentItem()->OnSelect();
}
