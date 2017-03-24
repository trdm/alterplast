// Plugin.cpp : Implementation of CPlugin

#include "stdafx.h"
#include "Telepat.h"
#include "Plugin.h"
#include "mycmdtarget.h"
#include "option.h"
#include "paramwnd.h"
#include "txttrap.h"
#include "codejump.h"
#include "parser.h"
#include "StringParser.h"

/////////////////////////////////////////////////////////////////////////////
// CPlugin
CPlugin* CPlugin::m_pPlugin=NULL;
IConfiguratorPtr pConfig;

STDMETHODIMP CPlugin::Init(IConfigurator * pConfigurator, BSTR * PluginName)
{
	if (PluginName == NULL)
		return E_POINTER;
	CString name="Телепат";
	pConfig=pConfigurator;
	*PluginName=name.AllocSysString();
	return S_OK;
}

STDMETHODIMP CPlugin::Done()
{
	pConfSvc->RemoveCmdTarget(CMyCmdTarget::m_pTarget);
	pConfig=NULL;
	return S_OK;
}

LRESULT CPlugin::OnInitDialog(UINT msg, WPARAM wp, LPARAM lp,BOOL& h)
{
	int lng=COption::m_pOption->GetLanguage();
	::SendMessage(GetDlgItem(IDC_CHECK1),BM_SETCHECK,(lng & 2)?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK2),BM_SETCHECK,(lng & 1)?1:0,0);

	lng=COption::m_pOption->GetComponents();
	::SendMessage(GetDlgItem(IDC_CHECK3),BM_SETCHECK,(lng & 1)?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK4),BM_SETCHECK,(lng & 2)?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK5),BM_SETCHECK,(lng & 4)?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK15),BM_SETCHECK,(lng & 8)?1:0,0);

	::SendMessage(GetDlgItem(IDC_CHECK6),BM_SETCHECK,COption::m_pOption->GetUseEnumProc(),0);
	::SendMessage(GetDlgItem(IDC_CHECK7),BM_SETCHECK,COption::m_pOption->GetSortMethods(),0);
	::SendMessage(GetDlgItem(IDC_CHECK8),BM_SETCHECK,COption::m_pOption->GetFilterMethods(),0);

	CParamWnd* pParam=CParamWnd::GetParamWnd();
	::SendMessage(GetDlgItem(IDC_CHECK9),BM_SETCHECK,pParam->GetQuickOption()?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK10),BM_SETCHECK,pParam->GetHintMethodOption()?1:0,0);
	::SendMessage(GetDlgItem(IDC_CHECK11),BM_SETCHECK,pParam->GetHintParamOption()?1:0,0);

	::SendMessage(GetDlgItem(IDC_EDIT2),EM_SETLIMITTEXT,1,0);
	SetDlgItemInt(IDC_EDIT2,COption::m_pOption->GetACompleteSymbols(),FALSE);

	::SendMessage(GetDlgItem(IDC_CHECK12),BM_SETCHECK,COption::m_pOption->GetDisableTemplate() & 1,0);
	::SendMessage(GetDlgItem(IDC_CHECK13),BM_SETCHECK,COption::m_pOption->GetDisableTemplate() & 2,0);

	::SendMessage(GetDlgItem(IDC_CHECK14),BM_SETCHECK,COption::m_pOption->GetAddTemplate(),0);

	h=TRUE;
	InitToolTips();
	return TRUE;
}

void CPlugin::SetLang(int lang,HWND ctrl,int otherLang,WORD otherID)
{
	int set=::SendMessage(ctrl,BM_GETCHECK,0,0);
	int lng=COption::m_pOption->GetLanguage();
	if(set)
		lng|=lang;
	else
		lng&=~lang;
	if(!lng)
	{
		lng=otherLang;
		::SendMessage(GetDlgItem(otherID),BM_SETCHECK,1,0);
	}
	COption::m_pOption->SetLanguage(lng);
}

LRESULT CPlugin::OnRussian(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetLang(2,hWndCtl,1,IDC_CHECK2);
	return 0;
}

LRESULT CPlugin::OnEnglish(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetLang(1,hWndCtl,2,IDC_CHECK1);
	return 0;
}

static void SetComponents(HWND ctrl,int cmp)
{
	int set=::SendMessage(ctrl,BM_GETCHECK,0,0);
	int lng=COption::m_pOption->GetComponents();
	if(set)
		lng|=cmp;
	else
		lng&=~cmp;
	COption::m_pOption->SetComponents(lng);
}

LRESULT CPlugin::OnTrade(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetComponents(hWndCtl,1);
	return 0;
}

LRESULT CPlugin::OnBuh(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetComponents(hWndCtl,2);
	return 0;
}

LRESULT CPlugin::OnCalc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetComponents(hWndCtl,4);
	return 0;
}

LRESULT CPlugin::OnDistrdb(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetComponents(hWndCtl,8);
	return 0;
}

LRESULT CPlugin::OnUseStdEnumProc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	COption::m_pOption->SetUseEnumProc(::SendMessage(hWndCtl,BM_GETCHECK,0,0)!=0);
	return 0;
}

LRESULT CPlugin::OnSortMethDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	COption::m_pOption->SetSortMethods(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?1:0);
	return 0;
}

LRESULT CPlugin::OnFiltrMethDlg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	COption::m_pOption->SetFilterMethods(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?1:0);
	return 0;
}

LRESULT CPlugin::OnParamQuick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	CParamWnd::GetParamWnd()->SetQuickOption(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?true:false);
	return 0;
}

LRESULT CPlugin::OnParamHintMethod(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	CParamWnd::GetParamWnd()->SetHintMethodOption(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?true:false);
	return 0;
}

LRESULT CPlugin::OnParamHintParam(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	CParamWnd::GetParamWnd()->SetHintParamOption(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?true:false);
	return 0;
}

LRESULT CPlugin::OnChangeACSymb(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	COption::m_pOption->SetACompleteSymbols(GetDlgItemInt(IDC_EDIT2,NULL,FALSE));
	return 0;
}

LRESULT CPlugin::OnDisableTemplate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	int f=(wID==IDC_CHECK13?2:1);
	int tmpl=COption::m_pOption->GetDisableTemplate();
	if(SendMessage(hWndCtl,BM_GETCHECK,0,0))
		tmpl|=f;
	else
		tmpl&=~f;
	COption::m_pOption->SetDisableTemplate(tmpl);
	return 0;
}

LRESULT CPlugin::OnAddTemplate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled=TRUE;
	COption::m_pOption->SetAddTemplate(::SendMessage(hWndCtl,BM_GETCHECK,0,0)?1:0);
	return 0;
}

bool CPlugin::FireOnTemplate(LPCTSTR name, CString &text)
{
	int nConnectionIndex;
	CString tName=name;
	BSTR TemplateName=tName.AllocSysString();

	VARIANT vText;
	vText.vt=VT_BSTR;
	vText.bstrVal=text.AllocSysString();

	VARIANT Cancel;
	Cancel.vt=VT_BOOL;
	Cancel.boolVal=VARIANT_FALSE;

	VARIANT pvars[3];
	pvars[2].vt=VT_BSTR;
	pvars[2].bstrVal = TemplateName;
	pvars[1].vt=VT_VARIANT|VT_BYREF;
	pvars[1].pvarVal= &vText;
	pvars[0].vt=VT_VARIANT|VT_BYREF;
	pvars[0].pvarVal = &Cancel;
	DISPPARAMS disp = { pvars, NULL, 3, 0 };
	int nConnections = m_vec.GetSize();
	bool retVal=true;
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			HRESULT hr=pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			if(hr==S_OK)
			{
				text=(LPCTSTR)_bstr_t(vText.bstrVal);
				if(Cancel.boolVal==VARIANT_TRUE)
					retVal=false;
			}
		}
	}
	SysFreeString(vText.bstrVal);
	SysFreeString(TemplateName);
	return retVal;
}

STDMETHODIMP CPlugin::get_Components(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetComponents();
	return S_OK;
}

STDMETHODIMP CPlugin::put_Components(long newVal)
{
	if(newVal<0 || newVal>15)
		return E_INVALIDARG;
	COption::m_pOption->SetComponents(newVal);
	return S_OK;
}

STDMETHODIMP CPlugin::get_Language(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetLanguage();
	return S_OK;
}

STDMETHODIMP CPlugin::put_Language(long newVal)
{
	if(newVal<1 || newVal>3)
		return E_INVALIDARG;
	COption::m_pOption->SetLanguage(newVal);
	return S_OK;
}

STDMETHODIMP CPlugin::get_UseStdMethodDlg(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetUseEnumProc()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_UseStdMethodDlg(VARIANT_BOOL newVal)
{
	COption::m_pOption->SetUseEnumProc(newVal==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CPlugin::get_NoOrderMethodDlg(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetSortMethods()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_NoOrderMethodDlg(VARIANT_BOOL newVal)
{
	COption::m_pOption->SetSortMethods(newVal==VARIANT_TRUE?1:0);
	return S_OK;
}

STDMETHODIMP CPlugin::get_FilterMethodDlg(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetFilterMethods()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_FilterMethodDlg(VARIANT_BOOL newVal)
{
	COption::m_pOption->SetFilterMethods(newVal==VARIANT_TRUE?1:0);
	return S_OK;
}

STDMETHODIMP CPlugin::get_AutoParamInfo(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CParamWnd::GetParamWnd()->GetQuickOption()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_AutoParamInfo(VARIANT_BOOL newVal)
{
	CParamWnd::GetParamWnd()->SetQuickOption(newVal==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CPlugin::get_ParamInfoAddMethDescr(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CParamWnd::GetParamWnd()->GetHintMethodOption()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_ParamInfoAddMethDescr(VARIANT_BOOL newVal)
{
	CParamWnd::GetParamWnd()->SetHintMethodOption(newVal==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CPlugin::get_ParamInfoAddParamDescr(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=CParamWnd::GetParamWnd()->GetHintParamOption()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_ParamInfoAddParamDescr(VARIANT_BOOL newVal)
{
	CParamWnd::GetParamWnd()->SetHintParamOption(newVal==VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CPlugin::get_AutoActiveCountSymbols(short *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetACompleteSymbols();
	return S_OK;
}

STDMETHODIMP CPlugin::put_AutoActiveCountSymbols(short newVal)
{
	COption::m_pOption->SetACompleteSymbols(newVal);
	return S_OK;
}

STDMETHODIMP CPlugin::get_DisableTemplateInRemString(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetDisableTemplate();
	return S_OK;
}

STDMETHODIMP CPlugin::put_DisableTemplateInRemString(long newVal)
{
	COption::m_pOption->SetDisableTemplate(newVal & 3);
	return S_OK;
}

STDMETHODIMP CPlugin::ConvertTemplate(BSTR TemplateText, VARIANT TemplateName,BSTR *Result)
{
	if(IsBadWritePtr(Result,sizeof(*Result)))
		return E_POINTER;
	CString tName="Телепат";
	if(TemplateName.vt!=VT_BSTR)
		VariantChangeType(&TemplateName,&TemplateName,0,VT_BSTR);
	if(TemplateName.vt==VT_BSTR)
		tName=(LPCTSTR)_bstr_t(TemplateName);
	CString tText=(LPCTSTR)_bstr_t(TemplateText);
	CPoint pt(0,0);
	int IsCaretSet=tText.Find("<?>");
	int param;
	CString res=CSTInsrtType::ConvertTemplate(tText,tName,pt,param);
	if(IsCaretSet>-1)
	{
		LPCTSTR start=res,ptr=start;
		int line=0,col=0;
		while(*ptr)
		{
			if(col==pt.x && line==pt.y)
				break;
			ptr++;
			if(*ptr=='\n')
			{
				line++;
				col=0;
			}
			else
				col++;
		}
		res.Insert(ptr-start,"<?>");
	}
	*Result=res.AllocSysString();
	return S_OK;
}

CString CPlugin::GetCustomMenu()
{
	int nConnectionIndex;
	CString retString;

	int nConnections = m_vec.GetSize();
	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			DISPPARAMS disp = { NULL, NULL, 0, 0 };
			VARIANT vRes;
			VariantInit(&vRes);
			HRESULT hr=pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &vRes, NULL, NULL);
			if(S_OK==hr)
			{
				if(vRes.vt!=VT_BSTR)
					VariantChangeType(&vRes,&vRes,0,VT_BSTR);
				if(vRes.vt==VT_BSTR)
				{
					CString txt=(LPCTSTR)_bstr_t(vRes.bstrVal);
					txt.TrimLeft(" \t\r\n");
					txt.TrimRight(" \t\r\n");
					if(!txt.IsEmpty())
					{
						if(!retString.IsEmpty())
							retString+="\r\n-\r\n";
						retString+=txt;
					}
				}
			}
			VariantClear(&vRes);
		}
	}
	return retString;
}

void CPlugin::FireOnMenu(CString cmd)
{
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	VARIANT varCmd;
	varCmd.vt=VT_BSTR;
	varCmd.bstrVal=cmd.AllocSysString();
	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			DISPPARAMS disp = { &varCmd, NULL, 1, 0 };
			pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
		}
	}
	VariantClear(&varCmd);
}

STDMETHODIMP CPlugin::get_AddTemplate(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=COption::m_pOption->GetAddTemplate()?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CPlugin::put_AddTemplate(VARIANT_BOOL newVal)
{
	COption::m_pOption->SetAddTemplate(newVal==VARIANT_TRUE?1:0);
	return S_OK;
}

void CPlugin::FireInsert(long type, CString name, CString &text)
{
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	VARIANT vText;
	vText.vt=VT_BSTR;
	vText.bstrVal=text.AllocSysString();

	VARIANT varArgs[3];
	varArgs[2].vt=VT_I4;
	varArgs[2].lVal=type;
	varArgs[1].vt=VT_BSTR;
	varArgs[1].bstrVal=name.AllocSysString();
	varArgs[0].vt=VT_VARIANT|VT_BYREF;
	varArgs[0].pvarVal=&vText;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			DISPPARAMS disp = {varArgs,NULL,3,0};
			if(S_OK==pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL))
			{
				if(vText.vt!=VT_BSTR)
					VariantChangeType(&vText,&vText,0,VT_BSTR);
				if(vText.vt==VT_BSTR)
					text=(LPCTSTR)_bstr_t(vText.bstrVal);
			}
		}
	}
	VariantClear(&vText);
}

DWORD GetOSVersion()
{
	OSVERSIONINFO info;
	info.dwOSVersionInfoSize=sizeof(info);
	GetVersionEx(&info);
	return info.dwMajorVersion*1000+info.dwMinorVersion;
}

DWORD GetShellVersion()
{
	HINSTANCE   hShell32;
	hShell32 = GetModuleHandle("shell32.dll");
	if(hShell32)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hShell32, "DllGetVersion");
		if(pDllGetVersion)
		{
			DLLVERSIONINFO    dvi;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			(*pDllGetVersion)(&dvi);
			return dvi.dwMajorVersion*1000+dvi.dwMinorVersion;
		}
	}
	return 0;
}

static HWND hToolTip=NULL;

void CPlugin::InitToolTips()
{
	// Инфо для тултипа
	static const struct{
		UINT id;
		LPCTSTR text;
	}tooltips[]={
		{IDC_CHECK3,	"Включить методы и объекты, связанные с компонентой \"Оперативный учет\""},
		{IDC_CHECK4,	"Включить методы и объекты, связанные с компонентой \"Бухгалтерсий учет\""},
		{IDC_CHECK5,	"Включить методы и объекты, связанные с компонентой \"Расчет\""},
		{IDC_CHECK15,	"Включить методы и объекты, связанные с компонентой \"УРБД\""},
		{IDC_CHECK1,	"Выводить в списке завершений русские названия методов"},
		{IDC_CHECK2,	"Выводить в списке завершений английские названия методов"},
		{IDC_CHECK6,	"При включенном флажке будет использоваться стандартный диалог \"Методы модуля\""},
		{IDC_CHECK7,	"При включенном флажке методы в списке будут выведены в порядке их следования в модуле"},
		{IDC_CHECK8,	"При отключенном флажке поиск методов производится по совпадению начальных символов, иначе - по вхождению подстроки"},
		{IDC_CHECK9,	"Автоматически выводить подсказку о параметрах при вставке имен методов, наборе '(' и ','"},
		{IDC_CHECK10,	"Включать в подсказку описание метода"},
		{IDC_CHECK11,	"Включать в подсказку описание текущего параметра"},
		{IDC_EDIT2,		"Количество символов в слове, после которых автоматически активируется список завершений. 0 - выводить список только вручную"},
		{IDC_CHECK12,	"Не обрабатывать шаблоны при наборе комментариев"},
		{IDC_CHECK13,	"Не обрабатывать шаблоны при наборе литералов (строки в кавычках)"},
		{IDC_CHECK14,	"Добавлять в список завершений автораскрываемые шаблоны"},
	};

	hToolTip=NULL;
	LPCTSTR toolClassName=TOOLTIPS_CLASS;
	DWORD tStyle=TTS_ALWAYSTIP;
	if(GetShellVersion()>5079)	// 5.80 и старше
		tStyle|=1<<6;	// TTS_BALLOON
	if(GetOSVersion()>5000)	// XP и старше
	{
		// Сделаем тултип с тенью
		static const char clName[]="T_ShadowToolTip";
		static bool IsClassRegister=false;
		if(!IsClassRegister)
		{
			WNDCLASSEX wc;
			wc.cbSize=sizeof(wc);
			if(GetClassInfoEx(NULL,TOOLTIPS_CLASS,&wc))
			{
				wc.style|=(1<<17);		// CS_DROPSHADOW
				wc.style&=~CS_GLOBALCLASS;
				wc.hInstance=hMyInst;
				wc.lpszClassName=clName;
				if(RegisterClassEx(&wc))
					IsClassRegister=true;
			}
		}
		if(IsClassRegister)
			toolClassName=clName;
	}
	hToolTip=CreateWindowEx(NULL,toolClassName,NULL,
			WS_POPUP|tStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			m_hWnd, NULL, hMyInst,NULL);
	
	if(!hToolTip)
		return;
	::SendMessage(hToolTip,TTM_SETDELAYTIME, TTDT_AUTOPOP,10000);
	::SendMessage(hToolTip,TTM_SETDELAYTIME, TTDT_INITIAL,100);
	::SendMessage(hToolTip,TTM_SETDELAYTIME, TTDT_RESHOW ,100);
	::SendMessage(hToolTip,TTM_SETMAXTIPWIDTH, 0, 250);
	::SendMessage(hToolTip,TTM_ACTIVATE, TRUE,0);
	// Загрузим инфу для тултипа
	TOOLINFO ti;
	ti.cbSize	=sizeof(ti);
	ti.uFlags	=TTF_IDISHWND|TTF_SUBCLASS;
	ti.hwnd		=m_hWnd;
	ti.hinst	=hMyInst;
	for(int i=0;i<sizeof(tooltips)/sizeof(tooltips[0]);i++)
	{
		HWND hControl=::GetDlgItem(m_hWnd,tooltips[i].id);
		ti.uId		=(UINT)hControl;
		ti.lpszText =(LPTSTR)tooltips[i].text;
		::SendMessage(hToolTip,TTM_ADDTOOL,0,(LPARAM)&ti);
	}
}

LRESULT CPlugin::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::DestroyWindow(hToolTip);
	return 0;
}

STDMETHODIMP CPlugin::Jump(long Line, long Col, long LineEnd, long ColEnd, BSTR ModuleName)
{
	CCodeJump* pJump=CCodeJump::GetCodeJump();
	CTxtTrap *pEditFrom=CTxtTrap::GetCurrentView(),*pEditTo=NULL;

	CString strModName=ModuleName;
	if(!strModName.IsEmpty())
	{
		ICfgDocumentsPtr docs;
		pConfig->get_Documents(&docs);
		if(docs!=NULL)
		{
			ICfgDocPtr doc;
			docs->get_item(ModuleName,&doc);
			if(doc!=NULL)
			{
				if(S_OK!=doc->Open())
					doc=NULL;
			}
			else
				docs->Open(ModuleName,&doc);
			if(doc!=NULL);
			{
				IWorkBookPtr wb=doc;
				if(wb!=NULL)
					wb->put_ActivePage(COleVariant(1L));
				pEditTo=CTxtTrap::GetCurrentView();
			}
		}
	}
	else
		pEditTo=pEditFrom;
	if(!pEditTo)
		return S_OK;
	CTextDocument *pDoc=pEditTo->GetDocument();
	if(!pDoc)
		return S_OK;
	int strCnt=pDoc->GetLineCount();
	if(Line<0 || Line>=strCnt)
		return S_OK;
	if(Col<0)
		Col=0;
	else if(Col>=pDoc->GetLineLength(Line))
	{
		Col=pDoc->GetLineLength(Line);
		if(Line!=strCnt-1)
			Col-=2;
	}
	if(LineEnd<0 || LineEnd>=strCnt)
		LineEnd=Line;

	if(ColEnd<0 || ColEnd>pDoc->GetLineLength(LineEnd))
		ColEnd=pDoc->GetLineLength(LineEnd);
	if(ColEnd==pDoc->GetLineLength(LineEnd))
	{
		if(LineEnd!=strCnt-1)
			ColEnd-=2;
	}
	if(pEditFrom)
	{
		pJump->SaveCurrentPos(pEditFrom);
		pJump->m_TopPos=pJump->m_CurPos++;
	}

	pEditTo->MoveCaret(CPoint(Col,Line),CPoint(ColEnd,LineEnd),1);
	return S_OK;
}

STDMETHODIMP CPlugin::GetMethodText(long Line, long Col, BSTR *pRetVal)
{
	if(IsBadWritePtr(pRetVal,sizeof(*pRetVal)))
		return E_POINTER;
	*pRetVal=NULL;
	CTxtTrap* pEditor=CTxtTrap::GetCurrentView();
	if(!pEditor)
		return S_OK;
	CParser* pParser=pEditor->GetMyParser();
	if(!pParser)
		return S_OK;
	if(Line<0)
		return S_OK;
	CTextDocument* pDoc=pEditor->GetDocument();
	if(Line>=pDoc->GetLineCount())
		Line=pDoc->GetLineCount()-1;
	pParser->GetMethodText(Line,Col);
	*pRetVal=pParser->m_textMethod.AllocSysString();
	return S_OK;
}

void CPlugin::FireMemberList(long line, long col, CStringArray &arr)
{
	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	VARIANT varArgs[2];
	//varArgs[2].vt=VT_BSTR|VT_BYREF;
	//varArgs[2].pbstrVal=&result;
	varArgs[1].vt=VT_I4;
	varArgs[1].lVal=line;
	varArgs[0].vt=VT_I4;
	varArgs[0].lVal=col;

	VARIANT result;

	
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		CMapStringToPtr mapAdd;
		if (pDispatch != NULL)
		{
			DISPPARAMS disp = { varArgs, NULL, 2, 0 };
			if(S_OK==pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &result, NULL, NULL))
			{
				if(result.vt!=VT_BSTR)
					VariantChangeType(&result,&result,0,VT_BSTR);
				if(result.vt==VT_BSTR)
				{
					CString strRes=result.bstrVal;
					LPCTSTR ptr=strRes,start=ptr;
					while(1)
					{
						if(*ptr=='\r' || *ptr=='\n' || *ptr=='\t' || !*ptr)
						{
							CString s(start,ptr-start);
							if(!s.IsEmpty())
							{
								if(!mapAdd[s])
								{
									arr.Add(s);
									mapAdd[s]=(void*)1;
								}
							}
							while(*ptr=='\r' || *ptr=='\n' || *ptr=='\t')
								ptr++;
							if(!*ptr)
								break;
							start=ptr;
						}
						ptr++;
					}
				}
				VariantClear(&result);
			}
		}
	}
	VariantClear(&varArgs[0]);
	VariantClear(&varArgs[1]);
}

STDMETHODIMP CPlugin::ReReadWords()
{
	CStringParser::ReadWords();
	return S_OK;
}
