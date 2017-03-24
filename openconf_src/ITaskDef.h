// ITaskDef.h: interface for the CITaskDef class.
#if!defined(AFX_ITASKDEF_H__9ADE5C13_8CFA_4F61_8339_6CA5288382EF__INCLUDED_)
#define AFX_ITASKDEF_H__9ADE5C13_8CFA_4F61_8339_6CA5288382EF__INCLUDED_
#include "MetaDataObjDerived.h"

class CITaskDef : public CMetaDataObjDerived<CTaskDef,ITaskDef,TaskDef>
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ITaskDef)
	END_COM_MAP()
	CITaskDef(CIMetaDataObj* pObj):CMetaDataObjDerived<CTaskDef,ITaskDef,TaskDef>(pObj){}

	STDMETHOD(get_Language)(long *pVal);
	STDMETHOD(put_Language)(long newVal);
	STDMETHOD(get_ImmediateDelete)(VARIANT_BOOL *pVal);
	STDMETHOD(put_ImmediateDelete)(VARIANT_BOOL newVal);
	STDMETHOD(get_Round15_2)(VARIANT_BOOL *pVal);
	STDMETHOD(put_Round15_2)(VARIANT_BOOL newVal);
};

#endif // !defined(AFX_ITASKDEF_H__9ADE5C13_8CFA_4F61_8339_6CA5288382EF__INCLUDED_)
