// cfgwindows.h
#ifndef CFGWINDOWS_H
#define CFGWINDOWS_H
#include "interfaces.h"
#include "dispimpl.h"
class CICfgWindows:public CDispImpl<ICfgWindows,CfgWindows>
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ICfgWindows)
	END_COM_MAP()

	CICfgWindows();
	STDMETHOD(get_MainWnd)(ICfgWindow **pVal);
	STDMETHOD(get_ActiveWnd)(ICfgWindow **pVal);
	STDMETHOD(put_ActiveWnd)(ICfgWindow *newVal);
	STDMETHOD(get_FirstWnd)(ICfgWindow **pVal);
	STDMETHOD(get_NextWnd)(ICfgWindow *CurrentWnd,ICfgWindow **pVal);
	STDMETHOD(get_PanelVisible)(BSTR Caption,VARIANT_BOOL *pVal);
	STDMETHOD(put_PanelVisible)(BSTR Caption,VARIANT_BOOL newVal);

	static CICfgWindows* m_pCfgWindows;
	static void Init();
};
#endif
