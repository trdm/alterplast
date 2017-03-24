// ICfgWindow.h: interface for the CICfgWindow class.
#if !defined(AFX_ICFGWINDOW_H__E3074982_DD0C_4C71_B5B2_20C3D9D7026F__INCLUDED_)
#define AFX_ICFGWINDOW_H__E3074982_DD0C_4C71_B5B2_20C3D9D7026F__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CICfgWindow : public CDispImpl<ICfgWindow,CfgWindow>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ICfgWindow)
	END_COM_MAP()

	CICfgWindow(HWND hWnd):m_hWnd(hWnd){}
	virtual ~CICfgWindow();

	STDMETHOD(get_hWnd)(long *pVal);
	STDMETHOD(get_Caption)(BSTR *pVal);
	STDMETHOD(put_Caption)(BSTR newVal);
	STDMETHOD(get_Document)(ICfgDoc **pVal);
	STDMETHOD(Close)();
	STDMETHOD(get_Maximized)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Maximized)(VARIANT_BOOL newVal);
	STDMETHOD(get_Minimized)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Minimized)(VARIANT_BOOL newVal);
	STDMETHOD(get_Top)(long *pVal);
	STDMETHOD(get_Left)(long *pVal);
	STDMETHOD(get_Bottom)(long *pVal);
	STDMETHOD(get_Right)(long *pVal);
	STDMETHOD(Move)(long left,long top,long right,long bottom);
	STDMETHOD(get_ClientWidth)(long *pVal);
	STDMETHOD(get_ClientHeight)(long *pVal);
        

	bool IsValid()
	{
		if(!IsWindow(m_hWnd))
		{
			SetError(E_FAIL,"Окно с HWND=%08X не существует.",m_hWnd);
			return false;
		}
		return true;
	}

	
	HWND m_hWnd;
};
/*template<class T,class C=T>
class CWndDerived:public CDispImpl<T,C>
{
public:
	CWndDerived(HWND hWnd):m_pWnd(hWnd){}
	// IUnknown implements
	STDMETHOD(QueryInterface)(REFIID iid,void** pObj)
	{
		if(iid==IID_ICfgWindow)
		{
			*pObj=this;
			return S_OK;
		}
		return CDispImpl<T,C>::QueryInterface(iid,pObj);
	}
	ULONG STDMETHODCALLTYPE AddRef(){return CDispImpl<T,C>::AddRef();}
	ULONG STDMETHODCALLTYPE Release(){return CDispImpl<T,C>::Release();}
	// ICfgWindow Impl
	STDMETHOD(get_hWnd)(long* pVal){return m_pWnd.get_hWnd(pVal);}
	STDMETHOD(get_Caption)(BSTR *pVal){return m_pWnd.get_Caption(pVal);}
	STDMETHOD(put_Caption)(BSTR newVal){return m_pWnd.put_Caption(newVal);}
	STDMETHOD(get_Document)(ICfgDoc **pVal){return m_pWnd.get_Document(pVal);}

	CICfgWindow m_pWnd;
};
*/
#endif
