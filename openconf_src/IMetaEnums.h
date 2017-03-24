// IMetaEnums.h: interface for the CIMetaEnums class.
#if !defined(AFX_IMETAENUMS_H__F4595823_D351_4889_8FC5_C4F2A3D3BFCF__INCLUDED_)
#define AFX_IMETAENUMS_H__F4595823_D351_4889_8FC5_C4F2A3D3BFCF__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaEnums : public CDispImpl<IMetaEnums ,MetaEnums>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaEnums)
	END_COM_MAP()
	CIMetaEnums();
	virtual ~CIMetaEnums();
	STDMETHOD(get_Enum)(VARIANT idx,IMetaEnum **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(Remove)(VARIANT idx);
	STDMETHOD(Add)(BSTR Name,IMetaEnum **pVal);

	CEnumDef* GetEnum(VARIANT& idx);
};

#endif // !defined(AFX_IMETAENUMS_H__F4595823_D351_4889_8FC5_C4F2A3D3BFCF__INCLUDED_)
