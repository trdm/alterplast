// MetaDataObjDerived.h: interface for the CMetaDataObjDerived class.
#if !defined(AFX_METADATAOBJDERIVED_H__722445B8_CC21_4860_8F9D_EB83C4B3165D__INCLUDED_)
#define AFX_METADATAOBJDERIVED_H__722445B8_CC21_4860_8F9D_EB83C4B3165D__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"
#include "metadataobj.h"

template<class M,class T,class C=T>
class CMetaDataObjDerived:public CDispImpl<T,C>
{
public:
	CMetaDataObjDerived(CIMetaDataObj* pObj):CDispImpl<T,C>(),m_pMdo(pObj){}
	~CMetaDataObjDerived(){delete m_pMdo;}
	M* GetMD()
	{
		if(m_pMdo->IsValid())
			return (M*)m_pMdo->m_pObj;
		SetError(E_FAIL,"Объект с id=%i не найден. Возможно, он был удален.",m_pMdo->m_id);
		return NULL;
	}
	// IUnknown implements
	STDMETHOD(QueryInterface)(REFIID iid,void** pObj)
	{
		if(iid==IID_IMetaDataObj)
		{
			*pObj=this;
			return S_OK;
		}
		return CDispImpl<T,C>::QueryInterface(iid,pObj);
	}
	ULONG STDMETHODCALLTYPE AddRef(){return CDispImpl<T,C>::AddRef();}
	ULONG STDMETHODCALLTYPE Release(){return CDispImpl<T,C>::Release();}
	// IMetaDataObj implements
	STDMETHOD(get_WhatIsIt)(TypeOfMetaDataObj *pVal){return m_pMdo->get_WhatIsIt(pVal);}
	STDMETHOD(get_ID)(long *pVal){return m_pMdo->get_ID(pVal);}
	STDMETHOD(get_Name)(BSTR *pVal){return m_pMdo->get_Name(pVal);}
	STDMETHOD(put_Name)(BSTR newVal){return m_pMdo->put_Name(newVal);}
	STDMETHOD(get_Present)(BSTR *pVal){return m_pMdo->get_Present(pVal);}
	STDMETHOD(put_Present)(BSTR newVal){return m_pMdo->put_Present(newVal);}
	STDMETHOD(get_Descr)(BSTR *pVal){return m_pMdo->get_Descr(pVal);}
	STDMETHOD(put_Descr)(BSTR newVal){return m_pMdo->put_Descr(newVal);}
	STDMETHOD(IsValid)(VARIANT_BOOL *pRet){return m_pMdo->IsValid(pRet);}
	STDMETHOD(get_FullName)(BSTR *pVal){return m_pMdo->get_FullName(pVal);}
	STDMETHOD(get_Props)(IMetaDataObjProps **pVal){return m_pMdo->get_Props(pVal);}
	STDMETHOD(get_Childs)(IMetaDataObjChilds **pVal){return m_pMdo->get_Childs(pVal);}
	STDMETHOD(get_Type)(IType1C **pVal){return m_pMdo->get_Type(pVal);}
	STDMETHOD(put_Type)(IType1C *newVal){return m_pMdo->put_Type(newVal);}
	STDMETHOD(DescribeObject)(VARIANT_BOOL WithChilds,BSTR* pRetVap){return m_pMdo->DescribeObject(WithChilds,pRetVap);}
	STDMETHOD(get_Parent)(IMetaDataObj **pVal){return m_pMdo->get_Parent(pVal);}
	CIMetaDataObj *m_pMdo;
};

#endif // !defined(AFX_METADATAOBJDERIVED_H__722445B8_CC21_4860_8F9D_EB83C4B3165D__INCLUDED_)
