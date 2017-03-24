// ICfgDocuments.h: interface for the CICfgDocuments class.
#if !defined(AFX_ICFGDOCUMENTS_H__4A91A7A2_06D4_4B03_A730_8B00953FD388__INCLUDED_)
#define AFX_ICFGDOCUMENTS_H__4A91A7A2_06D4_4B03_A730_8B00953FD388__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CICfgDocuments : public CDispImpl<ICfgDocuments, CfgDocuments>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(CICfgDocuments)
		COM_INTERFACE_ENTRY(ICfgDocuments)
	END_COM_MAP()

	CICfgDocuments();
	virtual ~CICfgDocuments();

	STDMETHOD(get_item)(BSTR name,ICfgDoc **pVal);
	STDMETHOD(Open)(BSTR Path,ICfgDoc **pDoc);
	STDMETHOD(DocFromID)(long id,DocTypes type,BSTR Path,BSTR kind,ICfgDoc **pRetDoc);
	STDMETHOD(New)(DocTypes type,ICfgDoc **pDoc);
	STDMETHOD(EnumAllTypedObj)(IAllTypedObj **pRet);
	
	static CICfgDocuments* m_pDocuments;
	static CRuntimeClass* m_pUsefulRTC[10];
	static void Init();
	static const char* TypesOfModules[];
	static int NameToType(LPCTSTR name);
};

#endif // !defined(AFX_ICFGDOCUMENTS_H__4A91A7A2_06D4_4B03_A730_8B00953FD388__INCLUDED_)
