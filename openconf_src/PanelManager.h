// PanelManager.h: interface for the CPanelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELMANAGER_H__7B768523_A48E_4B53_9A58_0FA98719D9B3__INCLUDED_)
#define AFX_PANELMANAGER_H__7B768523_A48E_4B53_9A58_0FA98719D9B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PluginManager.h"


struct SPanelInfo
{
	CString name;
	CView* pView;
	CStringArray tabs;
};
struct STabInfo
{
	CRuntimeClass* pWndClass;
	HICON hIcon;
};

class CPanelManager  
{
public:
	static void Setup();
	static void ProcessMenu(CMenu* pMenu);
	static bool IsPluginPresent(LPCTSTR szName);
	static void Init();
	CPanelManager();
	virtual ~CPanelManager();

	static SPanelInfo m_panels[16];
	static CMapStringToPtr m_rtcPanels;
};

#endif // !defined(AFX_PANELMANAGER_H__7B768523_A48E_4B53_9A58_0FA98719D9B3__INCLUDED_)
