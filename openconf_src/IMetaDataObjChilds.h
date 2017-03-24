// IMetaDataObjChilds.h: interface for the CIMetaDataObjChilds class.
#if !defined(AFX_IMETADATAOBJCHILDS_H__D5AE69E0_4016_485D_9BB1_7926810BD436__INCLUDED_)
#define AFX_IMETADATAOBJCHILDS_H__D5AE69E0_4016_485D_9BB1_7926810BD436__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaDataObjChilds : public CDispImpl<IMetaDataObjChilds,MetaDataObjChilds>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaDataObjChilds)
	END_COM_MAP()
	CIMetaDataObjChilds(CMetaDataObj* pObj):m_pObj(pObj){}

	STDMETHOD(get_item)(VARIANT idx,IMetaDataObjArray **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_Name)(VARIANT idx,VARIANT_BOOL Rus,BSTR *pVal);
	
	int GetNum(VARIANT& idx);
	CMetaDataObj* m_pObj;
};

#endif // !defined(AFX_IMETADATAOBJCHILDS_H__D5AE69E0_4016_485D_9BB1_7926810BD436__INCLUDED_)
