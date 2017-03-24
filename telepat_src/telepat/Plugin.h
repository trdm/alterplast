// Plugin.h : Declaration of the CPlugin

#ifndef __PLUGIN_H_
#define __PLUGIN_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "Telepat.h"
#include "telepatCP.h"


/////////////////////////////////////////////////////////////////////////////
// CPlugin
class ATL_NO_VTABLE CPlugin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IPlugin, &IID_IPlugin, &LIBID_TELEPATLib>,
	public CComCompositeControl<CPlugin>,
	public IPersistStreamInitImpl<CPlugin>,
	public IOleControlImpl<CPlugin>,
	public IOleObjectImpl<CPlugin>,
	public IOleInPlaceActiveObjectImpl<CPlugin>,
	public IViewObjectExImpl<CPlugin>,
	public IOleInPlaceObjectWindowlessImpl<CPlugin>,
	public CComCoClass<CPlugin, &CLSID_Plugin>,
	public IConfigPlugin,
	public CProxyDTelepatEvents< CPlugin >,
	public IConnectionPointContainerImpl<CPlugin>,
	public IProvideClassInfo
{
public:
	static CPlugin* m_pPlugin;
	CPlugin()
	{
		m_pPlugin=this;
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PLUGIN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPlugin)
	COM_INTERFACE_ENTRY(IPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IConfigPlugin)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_PROP_MAP(CPlugin)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CPlugin)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	CHAIN_MSG_MAP(CComCompositeControl<CPlugin>)
	COMMAND_HANDLER(IDC_CHECK1, BN_CLICKED, OnRussian)
	COMMAND_HANDLER(IDC_CHECK2, BN_CLICKED, OnEnglish)
	COMMAND_HANDLER(IDC_CHECK3, BN_CLICKED, OnTrade)
	COMMAND_HANDLER(IDC_CHECK4, BN_CLICKED, OnBuh)
	COMMAND_HANDLER(IDC_CHECK5, BN_CLICKED, OnCalc)
	COMMAND_HANDLER(IDC_CHECK15,BN_CLICKED, OnDistrdb)
	COMMAND_HANDLER(IDC_CHECK6, BN_CLICKED, OnUseStdEnumProc)
	COMMAND_HANDLER(IDC_CHECK7, BN_CLICKED, OnSortMethDlg)
	COMMAND_HANDLER(IDC_CHECK8, BN_CLICKED, OnFiltrMethDlg)

	COMMAND_HANDLER(IDC_CHECK9, BN_CLICKED, OnParamQuick)
	COMMAND_HANDLER(IDC_CHECK10, BN_CLICKED, OnParamHintMethod)
	COMMAND_HANDLER(IDC_CHECK11, BN_CLICKED, OnParamHintParam)
	COMMAND_HANDLER(IDC_EDIT2, EN_CHANGE, OnChangeACSymb)
	COMMAND_HANDLER(IDC_CHECK12, BN_CLICKED, OnDisableTemplate)
	COMMAND_HANDLER(IDC_CHECK13, BN_CLICKED, OnDisableTemplate)
	COMMAND_HANDLER(IDC_CHECK14, BN_CLICKED, OnAddTemplate)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CPlugin)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CPlugin>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IPlugin
public:
	LRESULT OnInitDialog(UINT msg,WPARAM wp,LPARAM lp,BOOL& h);
	STDMETHOD(GetClassInfo)(ITypeInfo** ppTI)
	{
		if(!ppTI)
			return E_POINTER;
		HRESULT hr=E_FAIL;
		ITypeLib* pLib=NULL;
		LoadRegTypeLib(LIBID_TELEPATLib,1,0,NULL,&pLib);
		if(pLib)
		{
			hr=pLib->GetTypeInfoOfGuid(CLSID_Plugin,ppTI);
			pLib->Release();
		}
		return hr;		
	}


	enum { IDD = IDD_PLUGIN };
// IConfigPlugin
	STDMETHOD(Init)(IConfigurator * pConfigurator, BSTR * PluginName);
	STDMETHOD(Done)();
	STDMETHOD(GetPluginCaps)(PluginCaps capNum, VARIANT * pResult)
	{
		if (pResult == NULL)
			return E_POINTER;
		return E_NOTIMPL;
	}
	STDMETHOD(DoAction)(PluginAction Action, VARIANT Param, VARIANT * pResult)
	{
		if (pResult == NULL)
			return E_POINTER;
		return E_NOTIMPL;
	}
	void SetLang(int lang,HWND ctrl,int otherLang,WORD otherID);
	LRESULT OnRussian(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnglish(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnTrade(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBuh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDistrdb(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUseStdEnumProc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSortMethDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFiltrMethDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnParamQuick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnParamHintMethod(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnParamHintParam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeACSymb(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDisableTemplate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAddTemplate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
public :
	STDMETHOD(ReReadWords)();
	void FireMemberList(long line, long col,CStringArray& arr);
	STDMETHOD(GetMethodText)(/*[in]*/ long Line, /*[in]*/ long Col, /*[out, retval]*/ BSTR* pRetVal);
	STDMETHOD(Jump)(/*[in]*/ long Line, /*[in, optional, defaultvalue(-1)]*/ long Col, /*[in, optional, defaultvalue(-1)]*/ long LineEnd, /*[in, optional, defaultvalue(-1)]*/ long ColEnd, /*[in, optional, defaultvalue("")]*/ BSTR ModuleName);
	void InitToolTips();
	STDMETHOD(get_AddTemplate)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AddTemplate)(/*[in]*/ VARIANT_BOOL newVal);
	void FireOnMenu(CString cmd);
	CString GetCustomMenu();
	void FireInsert(long type, CString name, CString& text);
	STDMETHOD(ConvertTemplate)(/*[in]*/ BSTR TemplateText, VARIANT TemplateName,/*[out, retval]*/ BSTR* Result);
	STDMETHOD(get_DisableTemplateInRemString)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_DisableTemplateInRemString)(/*[in]*/ long newVal);
	STDMETHOD(get_AutoActiveCountSymbols)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_AutoActiveCountSymbols)(/*[in]*/ short newVal);
	STDMETHOD(get_ParamInfoAddParamDescr)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ParamInfoAddParamDescr)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ParamInfoAddMethDescr)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ParamInfoAddMethDescr)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_AutoParamInfo)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AutoParamInfo)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_FilterMethodDlg)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_FilterMethodDlg)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_NoOrderMethodDlg)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_NoOrderMethodDlg)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_UseStdMethodDlg)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_UseStdMethodDlg)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Language)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Language)(/*[in]*/ long newVal);
	bool FireOnTemplate(LPCTSTR name,CString& text);
	STDMETHOD(get_Components)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Components)(/*[in]*/ long newVal);

BEGIN_CONNECTION_POINT_MAP(CPlugin)
	CONNECTION_POINT_ENTRY(DIID_DTelepatEvents)
END_CONNECTION_POINT_MAP()

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif //__PLUGIN_H_
