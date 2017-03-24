// metadataobjarray.h
#ifndef METADATAOBJARRAY_H
#define METADATAOBJARRAY_H
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaDataObjArray: public CDispImpl<IMetaDataObjArray,MetaDataObjArray>
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaDataObjArray)
	END_COM_MAP()

	CIMetaDataObjArray(CMetaDataObjArray* pObjArray)
	{
		m_pArray=pObjArray;
		m_curpos=-1;
	}
	STDMETHOD(get_Item)(VARIANT idx,IMetaDataObj **pVal);
	STDMETHOD(get_Count)(long *pVal)
	{
		try{
		*pVal=m_pArray->GetNItems();
		}
		catch(...)
		{
			return E_FAIL;
		}
		return S_OK;
	}
	STDMETHOD(get__NewEnum)(IUnknown **pVal)
	{
		return E_NOTIMPL;
	}
	CMetaDataObjArray* m_pArray;
	long m_curpos;
};

#endif