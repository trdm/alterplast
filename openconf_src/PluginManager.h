// PluginManager.h: interface for the CPluginManager class.
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "iconfig.h"

class CPluginManager  
{
public:
	void LoadPlugin(CString path);
	void UnloadPlugin(IDispatchPtr& ptr);
	static void Done();
	static void Init();
	struct Info{
		Info(LPCTSTR pPath,LPCTSTR n,HANDLE h):path(pPath),ShowMacros(true),OpenIn(0),IconNum(0),name(n),handle(h),pPlugin(NULL){CPluginManager::GetPM()->RegisterInfo(this);}
		Info(LPCTSTR pPath,LPCTSTR n,IDispatch* pP,HANDLE h):path(pPath),ShowMacros(true),OpenIn(0),IconNum(0),name(n),pPlugin(pP),handle(h){CPluginManager::GetPM()->RegisterInfo(this);}
		CString name;
		IDispatchPtr pPlugin;
		HANDLE handle;
		int IconNum;
		bool ShowMacros;
		int OpenIn;
		CString progID;
		CString path;
	};
	Info* GetPluginByName(CString name);
	Info* GetPluginByNum(long num);
	CPluginManager();
	virtual ~CPluginManager();
	static CPluginManager* GetPM();
	CMapStringToPtr m_Plugins;
	void RegisterInfo(Info* pInfo)
	{
		CString lo=pInfo->name;
		lo.MakeLower();
		m_Plugins[lo]=pInfo;
	}
	bool CheckName(LPCTSTR name)
	{
		CString n=name;
		n.MakeLower();
		void* d;
		return m_Plugins.Lookup(n,d)==FALSE;
	}
protected:
	Info* RegisterPlugin(LPCTSTR path,LPCTSTR name,HANDLE h);
	Info* RegisterDLL(LPCTSTR path,LPCTSTR name,HINSTANCE h);
	static CPluginManager* m_pPM;
};
typedef CPluginManager::Info PluginInfo;
#endif
