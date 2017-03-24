// Scripting.h: interface for the CScripting class.
#ifndef AFX_SCRIPTING_H
#define AFX_SCRIPTING_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemScript;
class CISelfScript;

class CScripting:public IActiveScriptSite,
	public IActiveScriptSiteWindow,
	public IActiveScriptSiteDebug
{
public:
	bool ReloadScript();
	LPCTSTR GetCurrentState();
	bool ExtractEngineAndName(LPCTSTR& text,CString& EngineName,CString& Name);
	bool CreateScript(LPCTSTR text);
	bool Unload();
	int ReportError(IActiveScriptError *pse,bool bDebug=false);
	CScripting();
	virtual ~CScripting();
	// IActiveScriptSite
	STDMETHOD(GetLCID)(LCID *plcid)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask,IUnknown **ppiunkItem,ITypeInfo **ppti);
	STDMETHOD(GetDocVersionString)(BSTR *pszVersion)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
	{
		return S_OK;
	}
	STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState);
	STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror);
	STDMETHOD(OnEnterScript)(void)
	{
		m_pActiveScript=this;
		m_EnterCount++;
		return S_OK;
	}
	STDMETHOD(OnLeaveScript)(void);
	// IActiveScriptSiteWindow
	STDMETHOD(GetWindow)(HWND* phWnd)
	{
		*phWnd=AfxGetMainWnd()->m_hWnd;
		return S_OK;
	}
	STDMETHOD(EnableModeless)(BOOL)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void **ppvObject)
	{
		if(!ppvObject)
			return E_INVALIDARG;
		if(riid==IID_IActiveScriptSite)
			*ppvObject=(IActiveScriptSite*)this;
		else if(riid==IID_IActiveScriptSiteWindow)
			*ppvObject=(IActiveScriptSiteWindow*)this;
		else if(riid==IID_IActiveScriptSiteDebug)
			*ppvObject=(IActiveScriptSiteDebug*)this;
		else if(riid==IID_IUnknown)
			*ppvObject=this;
		else
			return E_NOINTERFACE;
		m_dwRef++;
		return S_OK;
	}
	virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_dwRef;}
	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if(!--m_dwRef)
			delete this;
		return m_dwRef;
	}
	bool LoadScript(const CString file);
	long m_dwRef;
	CString m_ScriptName;
	CString m_EngineName;
	CString m_FileName;
	long m_EnterCount;
	int  m_DelayUnReload;
	CItemScript* m_pViewItem;
	CISelfScript* m_pSelf;
	//CFormScript* m_pForm;
	IActiveScriptPtr m_pEngine;
	static CScripting* m_pActiveScript;
	static CMapStringToString m_mapEngineNames;
	
	IDebugDocumentHelper* m_pDebugDocumentHelper;

	STDMETHOD(GetDocumentContextFromPosition)(
		DWORD dwSourceContext,
		ULONG uCharacterOffset,
		ULONG uNumChars,
		IDebugDocumentContext **ppsc);
	STDMETHOD(GetApplication)(IDebugApplication **ppda);
	STDMETHOD(GetRootApplicationNode)(IDebugApplicationNode **ppdanRoot);
	STDMETHOD(OnScriptErrorDebug)(IActiveScriptErrorDebug *pErrorDebug,
		BOOL *pfEnterDebugger,
		BOOL *pfCallOnScriptErrorWhenContinuing);

};
#endif
