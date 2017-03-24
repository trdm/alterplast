// metadata.h
#ifndef METADATA_H
#define METADATA_H
#include "interfaces.h"
#include "dispimpl.h"

class CIMetaData:public CDispImpl<IMetaData,MetaData>
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IMetaData)
	END_COM_MAP()

	CIMetaData();
	// Interface methods
	STDMETHOD(get_TaskDef)(ITaskDef **pVal);
	STDMETHOD(get_Modified)(short *pVal);
	STDMETHOD(FindObject)(long id,IMetaDataObj **pObj);
	STDMETHOD(RefreshMDWnd)();
	STDMETHOD(SaveMDToFile)(BSTR path,VARIANT_BOOL FireEvent,VARIANT_BOOL *retVal);
	STDMETHOD(Describe)(BSTR *pRet);
	STDMETHOD(get_LongCopyright)(BSTR *pVal);
	//STDMETHOD(put_LongCopyright)(BSTR newVal);
	STDMETHOD(get_ShortCopyright)(BSTR *pVal);
	//STDMETHOD(put_ShortCopyright)(BSTR newVal);
	STDMETHOD(SaveMMDS)(BSTR Path);
	STDMETHOD(get_Migration)(long id,IMigration **pVal);
	STDMETHOD(get_Constants)(IConstants **pVal);
	STDMETHOD(TestRefers)(long ID,VARIANT_BOOL FindAll,IMetaDataObjArray **pRetArr);
	STDMETHOD(get_Enums)(IMetaEnums **pVal);

	// Implements
	static CString GetFullName(long id);
	void SetMDModified();
	void SetDocModified();
	CMultiDocTemplate* m_pTemplateOfMetaData;
	CMultiDocTemplate* m_pTemplateMetaStream;
	bool m_isMDModified;
	static CMetaDataCont* m_pMetaData;
	static CIMetaData* m_pOneMetaData;
	static void Init(){new CIMetaData;}
};

#endif