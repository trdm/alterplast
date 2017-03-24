// metadataobj.h
#ifndef METADATAOBJ_H
#define METADATAOBJ_H
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaDataObj:public CDispImpl<IMetaDataObj,MetaDataObj>
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaDataObj)
	END_COM_MAP()
	CIMetaDataObj(CMetaDataObj* pObj);
	~CIMetaDataObj();
	long IsValid();
	//
	STDMETHOD(get_WhatIsIt)(TypeOfMetaDataObj *pVal);
	STDMETHOD(get_ID)(long *pVal);
	STDMETHOD(get_Name)(BSTR *pVal);
	STDMETHOD(put_Name)(BSTR newVal);
	STDMETHOD(get_Present)(BSTR *pVal);
	STDMETHOD(put_Present)(BSTR newVal);
	STDMETHOD(get_Descr)(BSTR *pVal);
	STDMETHOD(put_Descr)(BSTR newVal);
	STDMETHOD(IsValid)(VARIANT_BOOL *pRet);
	STDMETHOD(get_FullName)(BSTR *pVal);
	STDMETHOD(get_Props)(IMetaDataObjProps **pVal);
	STDMETHOD(get_Childs)(IMetaDataObjChilds **pVal);
	STDMETHOD(get_Type)(IType1C **pVal);
	STDMETHOD(put_Type)(IType1C *newVal);
	STDMETHOD(DescribeObject)(VARIANT_BOOL WithChilds,BSTR* pRetVap);
	STDMETHOD(get_Parent)(IMetaDataObj **pVal);

	static CIMetaDataObj* GetObj(CMetaDataObj* pObj);
	static CIMetaDataObj* GetObj(long id);
	static CMapPtrToPtr m_mapObjToIObj;

	CMetaDataObj* m_pObj;
	long m_id;
};
#define CHECK_VALID() if(!IsValid()) return SetError(E_FAIL,"Объект Метаданных ID=%i неактуален. Возможно, он был удален",m_id)
#endif