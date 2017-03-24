// ScriptManeger.h: interface for the CScriptManager class.
#ifndef AFX_SCRIPTMANEGER
#define AFX_SCRIPTMANEGER
#include "scripting.h"

class CScriptManager  
{
public:
	CScriptManager();
	virtual ~CScriptManager();
	static void InitScriptManeger();
	static void Done();
	static CScriptManager* GetScriptManager(){return m_pOneManager;}

	long GetScriptCount(){return m_ScriptsArray.GetSize();}
	CScripting* GetScriptByNum(long num);
	CScripting* GetScriptByPath(CString path);
	CScripting* GetScriptByName(CString name);

	void LoadScripts(const CString dir);
	CScripting* LoadScript(CString strFileName);
	void UnLoadScript(CScripting* pScript);
	bool RegisterScript(CScripting* pScript);
	bool UnRegisterScript(CScripting* pScript);
	bool ConnectScript(CScripting* pScript);
	void AddDelayed(CScripting* pScript);

	HRESULT GetItemInfo(CScripting* pScript,LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);
	// Поддержка отладки скриптов
	IProcessDebugManager* m_pProcDebugManager;
	IDebugApplication* m_pDebugApplication;
	DWORD m_dwDebugCookie;
	CPtrArray m_Delayed;
protected:
	CMapStringToPtr m_ScriptsByPath;
	CMapStringToPtr m_ScriptsByName;
	CPtrArray m_ScriptsArray;
	bool Prepare();
	static CScriptManager* m_pOneManager;
};

#endif
