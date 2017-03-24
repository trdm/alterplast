// IMetaDataObjProps.h: interface for the CIMetaDataObjProps class.
#if !defined(AFX_IMETADATAOBJPROPS_H__1B2C0F68_FCBE_4C1B_9E00_C4C1F19E0C9F__INCLUDED_)
#define AFX_IMETADATAOBJPROPS_H__1B2C0F68_FCBE_4C1B_9E00_C4C1F19E0C9F__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaDataObjProps : public CDispImpl<IMetaDataObjProps,MetaDataObjProps>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaDataObjProps)
	END_COM_MAP()

	CIMetaDataObjProps(CMetaDataObj* pObj):m_pObj(pObj){}
	STDMETHOD(get_item)(VARIANT idx,BSTR *pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Name)(VARIANT idx,VARIANT_BOOL Rus,BSTR *pVal);
	
	CMetaDataObj* m_pObj;
	int GetNum(VARIANT& idx);
};

#endif // !defined(AFX_IMETADATAOBJPROPS_H__1B2C0F68_FCBE_4C1B_9E00_C4C1F19E0C9F__INCLUDED_)
