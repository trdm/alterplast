// IDialogDoc.h: interface for the CIDialogDoc class.
#if !defined(AFX_IDIALOGDOC_H__B2E9B6A8_B05F_4009_9184_9FF215C616B4__INCLUDED_)
#define AFX_IDIALOGDOC_H__B2E9B6A8_B05F_4009_9184_9FF215C616B4__INCLUDED_
#include "icfgdoc.h"

class CIDialogDoc : public CCfgDocDerived<IDialogDoc, DialogDoc>  
{
public:
	STDMETHOD(get_ctrlType1C)(/*[in]*/ long idx, /*[out, retval]*/ IType1C* *pVal);
	STDMETHOD(put_ctrlType1C)(/*[in]*/ long idx, /*[in]*/ IType1C* newVal);
	STDMETHOD(get_formProp)(/*[in]*/ FormPropsIdx formPropIdx, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_formProp)(/*[in]*/ FormPropsIdx formPropIdx, /*[in]*/ VARIANT newVal);
	STDMETHOD(get_ctrlProp)(/*[in]*/ long ctrlIdx, /*[in]*/ CtrlPropsIdx propIdx, /*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_ctrlProp)(/*[in]*/ long ctrlIdx, /*[in]*/ CtrlPropsIdx propIdx, /*[in]*/ VARIANT newVal);
	STDMETHOD(ctrlIdx)(/*[in]*/ long UID, /*[out, retval]*/ long* pVal);
	STDMETHOD(ctrlUID)(/*[in]*/ long idx, /*[out, retval]*/ long* pVal);
	STDMETHOD(get_Selection)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Selection)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ctrlType)(/*[in]*/ long idx, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ctrlCount)(/*[out, retval]*/ long *pVal);
	static void Init();
	STDMETHOD(MoveLayer)(/*[in]*/ BSTR From, /*[in]*/ VARIANT_BOOL Down);
	STDMETHOD(DeleteLayer)(/*[in]*/ BSTR Name, /*[in, optional, defaultvalue(NULL)]*/ BSTR CopyControlsTo);
	STDMETHOD(get_ActiveLayer)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ActiveLayer)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LayerVisible)(/*[in]*/ BSTR Name, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_LayerVisible)(/*[in]*/ BSTR Name, /*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_LayerName)(/*[in]*/ long idx, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LayerName)(/*[in]*/ long idx, /*[in]*/ BSTR newVal);
	STDMETHOD(get_LayerCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(AddLayer)(/*[in]*/ BSTR Name,/*[in, optional,defaultvalue(-1)]*/ VARIANT_BOOL Visible);
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IDialogDoc)
	END_COM_MAP()

	bool LoadStream(CString& stream);
	bool GetStream(CString& stream);
	CIDialogDoc(CDocument* pDoc):CCfgDocDerived<IDialogDoc, DialogDoc>(pDoc){}
	virtual ~CIDialogDoc();
	STDMETHOD(SaveToFile)(BSTR FileName,VARIANT_BOOL *bSucces);
	STDMETHOD(LoadFromFile)(BSTR FileName,VARIANT_BOOL *bSuccess);
	STDMETHOD(get_Stream)(BSTR *pVal);
	STDMETHOD(put_Stream)(BSTR newVal);
	
	CEditDoc* GetDocument()
	{
		return (CEditDoc*)m_doc.GetDocument();
	}
};

#endif // !defined(AFX_IDIALOGDOC_H__B2E9B6A8_B05F_4009_9184_9FF215C616B4__INCLUDED_)
