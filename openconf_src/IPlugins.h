// IPlugins.h: interface for the CIPlugins class.
#if !defined(AFX_IPLUGINS_H__90E4DA4F_B24A_41C3_9F1F_497602465BC4__INCLUDED_)
#define AFX_IPLUGINS_H__90E4DA4F_B24A_41C3_9F1F_497602465BC4__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"
#include "pluginmanager.h"
typedef CPluginManager::Info *PINFO; 


class CIPlugins : public CDispImpl<IPlugins,Plugins>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IPlugins)
	END_COM_MAP()

	static void Done();
	CIPlugins();
	virtual ~CIPlugins();
	// Interface
	STDMETHOD(get_item)(VARIANT Idx,IDispatch **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(LoadPlugin)(BSTR From);
	STDMETHOD(get_Path)(VARIANT idx,BSTR *pVal);
	STDMETHOD(get_Name)(VARIANT idx,BSTR *pVal);
	// Implements
	static CIPlugins* m_pPlugins;
	PINFO GetIdx(VARIANT& idx);
};

#endif // !defined(AFX_IPLUGINS_H__90E4DA4F_B24A_41C3_9F1F_497602465BC4__INCLUDED_)
