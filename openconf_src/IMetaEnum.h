// IMetaEnum.h: interface for the CIMetaEnum class.
#if !defined(AFX_IMETAENUM_H__51EE7B06_513D_42F4_8EA1_28F64BAC4639__INCLUDED_)
#define AFX_IMETAENUM_H__51EE7B06_513D_42F4_8EA1_28F64BAC4639__INCLUDED_
#include "MetaDataObjDerived.h"

class CIMetaEnum : public CMetaDataObjDerived<CEnumDef, IMetaEnum, MetaEnum>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaEnum)
	END_COM_MAP()

	CIMetaEnum(CIMetaDataObj* pObj):CMetaDataObjDerived<CEnumDef, IMetaEnum, MetaEnum>(pObj){}
	virtual ~CIMetaEnum();
	
	STDMETHOD(get_ValsCount)(long *pVal);
	STDMETHOD(get_EnumVal)(VARIANT idx,IMetaEnumVal **pVal);
	STDMETHOD(Add)(BSTR Name,IMetaEnumVal **pVal);
	STDMETHOD(Remove)(VARIANT idx);

	CEnumValDef* GetValDef(VARIANT& idx);
};

#endif // !defined(AFX_IMETAENUM_H__51EE7B06_513D_42F4_8EA1_28F64BAC4639__INCLUDED_)
