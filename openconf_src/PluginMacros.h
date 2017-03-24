// PluginMacros.h: interface for the CPluginMacros class.
#ifndef PLUGINMACROS_H
#define PLUGINMACROS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeItemInfo.h"

class CPluginMacros : public CTreeItemInfo  
{
public:
	void OnSelect();
	CPluginMacros(LPCTSTR macros,DISPID dispID):CTreeItemInfo(macros,ITREE_SCRIPTS_MACROS),m_dispID(dispID){}
	virtual ~CPluginMacros(){}
	virtual void OnAction();
	DISPID m_dispID;
};

#endif
