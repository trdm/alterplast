// IConstants.h: interface for the CIConstants class.
#if !defined(AFX_ICONSTANTS_H__13FA7DCC_7A28_4227_9C6C_E95E0E0B802A__INCLUDED_)
#define AFX_ICONSTANTS_H__13FA7DCC_7A28_4227_9C6C_E95E0E0B802A__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIConstants : public CDispImpl<IConstants,Constants>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IConstants)
	END_COM_MAP()
	CIConstants();
	STDMETHOD(get_item)(VARIANT idx,IConstDef **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Name)(VARIANT idx,BSTR *pVal);
	STDMETHOD(Add)(BSTR name,BSTR Present,IConstDef **pRetVal);
	STDMETHOD(Remove)(VARIANT idx);
	
	CConstDef* GetDef(VARIANT& idx);
};

#endif // !defined(AFX_ICONSTANTS_H__13FA7DCC_7A28_4227_9C6C_E95E0E0B802A__INCLUDED_)
