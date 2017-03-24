// ITextDoc.h: interface for the CITextDoc class.
#if !defined(AFX_ITEXTDOC_H__99E6A405_0802_4BF4_BCC2_8CD5AFBC9392__INCLUDED_)
#define AFX_ITEXTDOC_H__99E6A405_0802_4BF4_BCC2_8CD5AFBC9392__INCLUDED_
#include "icfgdoc.h"

class CITextDoc : public CCfgDocDerived<ITextDoc, TextDoc>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ITextDoc)
	END_COM_MAP()

	CITextDoc(CDocument* pDoc);
	virtual ~CITextDoc();
	STDMETHOD(SaveToFile)(BSTR FileName,VARIANT_BOOL *bSucces);
	STDMETHOD(LoadFromFile)(BSTR FileName,VARIANT_BOOL *bSuccess);
	STDMETHOD(get_Text)(BSTR *pVal);
	STDMETHOD(put_Text)(BSTR newVal);
	STDMETHOD(get_LineCount)(long *pVal);
	STDMETHOD(get_LineLen)(long LineNum,long *pVal);
	STDMETHOD(get_IsModule)(VARIANT_BOOL *pVal);
	STDMETHOD(put_IsModule)(VARIANT_BOOL newVal);
	STDMETHOD(get_ReadOnly)(VARIANT_BOOL *pVal);
	STDMETHOD(put_ReadOnly)(VARIANT_BOOL newVal);
	STDMETHOD(get_Range)(long StartLine,VARIANT StartCol,VARIANT EndLine,VARIANT EndCol,BSTR *pVal);
	STDMETHOD(put_Range)(long StartLine,VARIANT StartCol,VARIANT EndLine,VARIANT EndCol,BSTR newVal);
	STDMETHOD(get_SelStartLine)(long *pVal);
	STDMETHOD(get_SelStartCol)(long *pVal);
	STDMETHOD(get_SelEndLine)(long *pVal);
	STDMETHOD(get_SelEndCol)(long *pVal);
	STDMETHOD(get_CurrentWord)(BSTR *pVal);
	STDMETHOD(MoveCaret)(long LineStart,long ColStart,VARIANT LineEnd,VARIANT ColEnd);
	
	STDMETHOD(get_BookMark)(long LineNum,VARIANT_BOOL *pVal);
	STDMETHOD(put_BookMark)(long LineNum,VARIANT_BOOL newVal);
	STDMETHOD(NextBookmark)(long StartLine,long *pNextBookmark);
	STDMETHOD(PrevBookmark)(long StartLine,long *pRet);
	STDMETHOD(ClearAllBookMark)();
	STDMETHOD(get_CanUndo)(VARIANT_BOOL *pVal);
	STDMETHOD(get_CanRedo)(VARIANT_BOOL *pVal);
	STDMETHOD(Undo)();
	STDMETHOD(Redo)();
	STDMETHOD(Cut)();
	STDMETHOD(Copy)();
	STDMETHOD(Paste)();
	STDMETHOD(CommentSel)();
	STDMETHOD(UncommentSel)();
	STDMETHOD(FormatSel)();

	CTextEditor* GetView(CTextDocument* pDoc)
	{
		if(pDoc)
		{
			POSITION pos=pDoc->GetFirstViewPosition();
			if(pos)
				return (CTextEditor*)pDoc->GetNextView(pos);
		}
		SetError(E_FAIL,"Данная операция допустима только при открытом документе");
		return NULL;
	}
	CTextDocument* GetTD(){return (CTextDocument*) m_doc.GetDocument();}
	bool GetSelection(long num,long* res);
};

#endif // !defined(AFX_ITEXTDOC_H__99E6A405_0802_4BF4_BCC2_8CD5AFBC9392__INCLUDED_)
