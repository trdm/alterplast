// IScripts.h: interface for the CIScripts class.
#if !defined(AFX_ISCRIPTS_H__4020D9AE_CCE4_44CF_9E46_432AF92262FD__INCLUDED_)
#define AFX_ISCRIPTS_H__4020D9AE_CCE4_44CF_9E46_432AF92262FD__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"
class CScripting;

class CIScripts : public CDispImpl<IScripts,Scripts>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IScripts)
	END_COM_MAP()

	CIScripts();
	virtual ~CIScripts();
	STDMETHOD(get_item)(VARIANT Idx,IDispatch **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(Load)(BSTR Path,IDispatch **pRetVal);
	STDMETHOD(ReLoad)(VARIANT Idx);
	STDMETHOD(UnLoad)(VARIANT Idx);
	STDMETHOD(get_Path)(VARIANT idx,BSTR *pVal);
	STDMETHOD(get_Name)(VARIANT idx,BSTR *pVal);
	STDMETHOD(get_Engine)(VARIANT idx,BSTR *pVal);

	CScripting* GetScript(VARIANT& idx);
	static void Done();
	static CIScripts* m_pScripts;
};

#endif // !defined(AFX_ISCRIPTS_H__4020D9AE_CCE4_44CF_9E46_432AF92262FD__INCLUDED_)
