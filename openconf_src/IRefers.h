// IRefers.h: interface for the CIRefers class.
#if !defined(AFX_IREFERS_H__DE8EE969_BA9B_428D_B925_8926546D39E0__INCLUDED_)
#define AFX_IREFERS_H__DE8EE969_BA9B_428D_B925_8926546D39E0__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIRefers : public CDispImpl<IMetaDataObjArray, MetaDataObjArray>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaDataObjArray)
	END_COM_MAP()
	CIRefers(CPtrArray* pArr):m_pArr(pArr){}
	virtual ~CIRefers(){delete m_pArr;}
	
	STDMETHOD(get_Item)(VARIANT idx,IMetaDataObj **pVal);
	STDMETHOD(get_Count)(long *pVal);
	
	CPtrArray* m_pArr;
};

#endif // !defined(AFX_IREFERS_H__DE8EE969_BA9B_428D_B925_8926546D39E0__INCLUDED_)
