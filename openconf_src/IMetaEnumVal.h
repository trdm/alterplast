// IMetaEnumVal.h: interface for the CIMetaEnumVal class.
#if !defined(AFX_IMETAENUMVAL_H__35F789CD_44A8_42E6_943B_174290F9ADED__INCLUDED_)
#define AFX_IMETAENUMVAL_H__35F789CD_44A8_42E6_943B_174290F9ADED__INCLUDED_
#include "MetaDataObjDerived.h"

class CIMetaEnumVal : public CMetaDataObjDerived<CEnumValDef,IMetaEnumVal, MetaEnumVal>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaEnumVal)
	END_COM_MAP()
	CIMetaEnumVal(CIMetaDataObj* pObj):CMetaDataObjDerived<CEnumValDef,IMetaEnumVal, MetaEnumVal>(pObj){}
	virtual ~CIMetaEnumVal();
	STDMETHOD(get_Title)(BSTR *pVal);
	STDMETHOD(put_Title)(BSTR newVal);
};

#endif // !defined(AFX_IMETAENUMVAL_H__35F789CD_44A8_42E6_943B_174290F9ADED__INCLUDED_)
