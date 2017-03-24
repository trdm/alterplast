// ICfgDoc.h: interface for the CICfgDoc class.
#if !defined(AFX_ICFGDOC_H__82051CA1_71FC_11D8_9FCD_E9053813DA08__INCLUDED_)
#define AFX_ICFGDOC_H__82051CA1_71FC_11D8_9FCD_E9053813DA08__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CICfgDoc : public CDispImpl<ICfgDoc,CfgDoc>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ICfgDoc)
	END_COM_MAP()

	CICfgDoc(CDocument* pDoc);
	virtual ~CICfgDoc();
	STDMETHOD(get_Type)(DocTypes *pVal);
	STDMETHOD(get_ID)(long *pVal);
	STDMETHOD(get_Kind)(BSTR *pVal);
	STDMETHOD(get_Path)(BSTR *pVal);
	STDMETHOD(Open)();
	STDMETHOD(get_IsOpen)(VARIANT_BOOL *pVal);
	STDMETHOD(Save)();
	STDMETHOD(get_Name)(BSTR *pVal);

	CDocument* GetDocument();
	CString m_path;
	CDocTemplate* m_pTemplate;
	CDocument* m_pDoc;
	static ICfgDoc* CreateDoc(CDocument* pDoc);
	static CString m_tempPath;
	static struct Init{
		Init()
		{
			HINSTANCE h7=GetModuleHandle("seven.dll");
			m_tempPath=*(CString*)GetProcAddress(h7,"?m_FullTempPath@CConfigCont@@2VCString@@A");
		}
	}m_init;
};

template <class T,class C=T>
class CCfgDocDerived:public CDispImpl<T,C>
{
public:
	CCfgDocDerived(CDocument* pDoc):CDispImpl<T,C>(),m_doc(pDoc){}
	// IUnknown implements
	STDMETHOD(QueryInterface)(REFIID iid,void** pObj)
	{
		if(iid==IID_ICfgDoc)
		{
			*pObj=this;
			return S_OK;
		}
		return CDispImpl<T,C>::QueryInterface(iid,pObj);
	}
	ULONG STDMETHODCALLTYPE AddRef(){return CDispImpl<T,C>::AddRef();}
	ULONG STDMETHODCALLTYPE Release(){return CDispImpl<T,C>::Release();}

	STDMETHOD(get_Type)(DocTypes *pVal){return m_doc.get_Type(pVal);}
	STDMETHOD(get_ID)(long *pVal){return m_doc.get_ID(pVal);}
	STDMETHOD(get_Kind)(BSTR *pVal){return m_doc.get_Kind(pVal);}
	STDMETHOD(get_Path)(BSTR *pVal){return m_doc.get_Path(pVal);}
	STDMETHOD(Open)(){return m_doc.Open();}
	STDMETHOD(get_IsOpen)(VARIANT_BOOL *pVal){return m_doc.get_IsOpen(pVal);}
	STDMETHOD(Save)(){return m_doc.Save();}
	STDMETHOD(get_Name)(BSTR *pVal){return m_doc.get_Name(pVal);}

	CICfgDoc m_doc;
};
#endif // !defined(AFX_ICFGDOC_H__82051CA1_71FC_11D8_9FCD_E9053813DA08__INCLUDED_)
