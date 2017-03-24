// module.h
#ifndef MODULE_H
#define MODULE_H
#include "interfaces.h"
#include "dispimpl.h"

class CIModule:public CDispImpl<IModule,Module>
{
public:
	static CIModule* GetModule(CString name);
	CTextDocument* GetDoc();
	DISPIMPL_STD();
	CIModule(ModuleKind kind,long id):m_kind(kind),m_id(id){}
	CIModule(ModuleKind kind,LPCTSTR file):m_kind(kind),m_FileName(file){}

	STDMETHOD(get_Text)(BSTR *pVal);
	STDMETHOD(put_Text)(BSTR newVal);
	STDMETHOD(get_LineCount)(long *pVal);
	STDMETHOD(get_Line)(long num,BSTR *pVal);
	STDMETHOD(put_Line)(long num,BSTR newVal);
	STDMETHOD(get_Name)(BSTR *pVal);
	STDMETHOD(Open)(void);
	STDMETHOD(get_MetaDataObj)(IMetaDataObj **pVal);
	STDMETHOD(get_Kind)(ModuleKind *pVal);

	long m_id;
	CString m_FileName;
	ModuleKind m_kind;
	static char* m_TypesOfModules[];
};
#endif