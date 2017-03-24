// IConfig.h: interface for the CIConfig class.
#ifndef AFX_ICONFIG_H
#define AFX_ICONFIG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "interfaces.h"
#include "dispimpl.h"

class CIConfigEvents:public IConnectionPoint
{
public:
	void FireDialogChange(CDocument* pDoc);
	static void Done();
	void OnQuit();
	bool OnQueryQuit();
	void FireFileSave(CDocument* pDoc);
	void FireActivateMainWnd(bool bActive);
	void FireActivateMDIWnd(ICfgWindow* pWnd,bool bActive);
	void FireOnIdle();
	void FireTimer(long id);
	CIConfigEvents():m_dwRef(2),m_cook(1){}
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID iid,void** pObj)
	{
		if(iid==IID_IConnectionPoint || iid==IID_IUnknown)
		{
			*pObj=this;
			m_dwRef++;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_dwRef;
	}
	ULONG STDMETHODCALLTYPE Release()
	{
		return --m_dwRef;
	}
	// IConnectionPoint
	STDMETHOD(GetConnectionInterface)(IID *pIID)
	{
		*pIID=DIID_IConfigEvents;
		return S_OK;
	}
	STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer **ppCPC)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Advise)(IUnknown *pUnkSink,DWORD *pdwCookie)
	{
		IConfigEvents* pInt=NULL;
		pUnkSink->QueryInterface(__uuidof(IConfigEvents),(void**)&pInt);
		if(!pInt)
			return CONNECT_E_CANNOTCONNECT;
		m_Advised[(void*)m_cook]=pInt;
		*pdwCookie=m_cook++;
		return S_OK;
	}
	STDMETHOD(Unadvise)(DWORD dwCookie)
	{
		IUnknown* pUnk;
		if(m_Advised.Lookup((void*)dwCookie,(void*&)pUnk))
		{
			m_Advised.RemoveKey((void*)dwCookie);
			pUnk->Release();
		}
		return S_OK;
	}
	STDMETHOD(EnumConnections)(IEnumConnections **ppEnum)
	{
		return E_NOTIMPL;
	}
    void FireEventNonParam(DISPID idEvent);
	void FireOpenConfigWnd();
	void FireAllPluginsInit();
	void FireMDSaved(CString fName);
	int FireDoModal(CDialog*);
	int FireFileDialog(bool bSaved,CString lpTitle,CString lpFilter,CString& fName);
	int FireMsgBox(CString text,UINT styles);

	long m_dwRef;
	CMapPtrToPtr m_Advised;
	long m_cook;
	static CIConfigEvents m_ConfigEvents;
};

class CIConfig: public CDispImpl<IConfigurator,Configurator>,
	public IConnectionPointContainer
{
public:
	STDMETHOD(get_CancelHotKey)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_CancelHotKey)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_MDWnd)(/*[out, retval]*/ IMDWindow* *pVal);
	STDMETHOD(get_AppProps)(/*[in]*/ AppProperties idx, /*[out, retval]*/ BSTR *pVal);
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IConfigurator)
	END_COM_MAP()

	DISPIMPL_STD()
	CIConfig();
	virtual ~CIConfig();
	STDMETHOD(Message)(BSTR,MsgLineMarkers);
    STDMETHOD(get_IBDir)(BSTR *pVal);
    STDMETHOD(get_BinDir)(BSTR *pVal);
	STDMETHOD(Status)(BSTR text);
	STDMETHOD(get_CommandLine)(BSTR *pVal);
	STDMETHOD(get_IBName)(BSTR *pVal);
	STDMETHOD(put_IBName)(BSTR newVal);
	STDMETHOD(OpenOLEForm)(BSTR progID,BSTR title,IDispatch** ppDisp);
	STDMETHOD(get_Version)(long *pVal);
	STDMETHOD(get_MetaData)(IMetaData **pVal);
	STDMETHOD(SendCommand)(ConfigCommands cmd);
	STDMETHOD(IsConfigWndOpen)(VARIANT_BOOL *pResult);
	STDMETHOD(get_Plugins)(IPlugins **pVal);
	STDMETHOD(get_Scripts)(IScripts **pVal);
	STDMETHOD(get_Windows)(ICfgWindows **pVal);
	STDMETHOD(Quit)(VARIANT_BOOL PromptSave);
	STDMETHOD(LogMessageWrite)(EventCategories EventCategory,BSTR Message,BSTR strEventType,BSTR Event);
	STDMETHOD(Run1CApp)(RunAppModes mode);
	STDMETHOD(get_Documents)(ICfgDocuments **pVal);
	STDMETHOD(get_CfgTimer)(ICfgTimer **pVal);
	
	virtual HRESULT STDMETHODCALLTYPE EnumConnectionPoints( 
        /* [out] */ IEnumConnectionPoints __RPC_FAR *__RPC_FAR *ppEnum)
	{
		return E_NOTIMPL;
	}
    
    virtual HRESULT STDMETHODCALLTYPE FindConnectionPoint( 
        /* [in] */ REFIID riid,
        /* [out] */ IConnectionPoint **ppCP)
	{
		*ppCP=&CIConfigEvents::m_ConfigEvents;
		CIConfigEvents::m_ConfigEvents.AddRef();
		return S_OK;
	}
	virtual HRESULT AdditionalQI(REFIID riid,void **ppvObject)
	{
		if(riid==IID_IConnectionPointContainer)
		{
			*ppvObject=(IConnectionPointContainer*)this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
};

_COM_SMARTPTR_TYPEDEF(IConfigPlugin,__uuidof(IConfigPlugin));

#endif
