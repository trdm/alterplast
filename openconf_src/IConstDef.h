// IConstDef.h: interface for the CIConstDef class.
#if !defined(AFX_ICONSTDEF_H__17A73E0F_A8D8_42C4_9711_3F4D3A0CFCE2__INCLUDED_)
#define AFX_ICONSTDEF_H__17A73E0F_A8D8_42C4_9711_3F4D3A0CFCE2__INCLUDED_
#include "MetaDataObjDerived.h"

class CIConstDef : public CMetaDataObjDerived<CConstDef,IConstDef,ConstDef>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IConstDef)
	END_COM_MAP()
	CIConstDef(CIMetaDataObj* pObj):CMetaDataObjDerived<CConstDef,IConstDef,ConstDef>(pObj){}
	STDMETHOD(get_Periodic)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Periodic)(VARIANT_BOOL newVal);
};

#endif // !defined(AFX_ICONSTDEF_H__17A73E0F_A8D8_42C4_9711_3F4D3A0CFCE2__INCLUDED_)
