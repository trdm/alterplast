// IMDWindow.h: interface for the CIMDWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMDWINDOW_H__3AF08CE2_E9E6_45B1_9E1D_99DD4FAFD0FE__INCLUDED_)
#define AFX_IMDWINDOW_H__3AF08CE2_E9E6_45B1_9E1D_99DD4FAFD0FE__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIMDWindow : public CDispImpl<IMDWindow, MDWindow>  
{
public:
	STDMETHOD(GetSelected)(/*[out, retval]*/ BSTR* Path);
	static void Init();
	STDMETHOD(get_hWnd)(/*[out, retval]*/ long *pVal);
	STDMETHOD(DoAction)(/*[in]*/ BSTR PathInTree, /*[in, optional, defaultvalue(mdaEdit)]*/ mdActions Action);
	STDMETHOD(get_ActiveTab)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ActiveTab)(/*[in]*/ long newVal);
	STDMETHOD(get_TabName)(/*[in]*/ long idx, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_TabCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(Activate)();
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMDWindow)
	END_COM_MAP()
	CIMDWindow();
	virtual ~CIMDWindow();

	static CIMDWindow* m_pMDWindow;

};

#endif // !defined(AFX_IMDWINDOW_H__3AF08CE2_E9E6_45B1_9E1D_99DD4FAFD0FE__INCLUDED_)
