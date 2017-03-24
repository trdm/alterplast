// ITableDoc.h: interface for the CITableDoc class.
#if !defined(AFX_ITABLEDOC_H__A3D358A2_7A7C_11D8_9FCD_D6268C95B908__INCLUDED_)
#define AFX_ITABLEDOC_H__A3D358A2_7A7C_11D8_9FCD_D6268C95B908__INCLUDED_
#include "icfgdoc.h"

class CITableDoc : public CCfgDocDerived<ITableDoc ,TableDoc>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(ITableDoc)
	END_COM_MAP()

	CITableDoc(CDocument* pDoc);
	virtual ~CITableDoc();
	STDMETHOD(SaveToFile)(BSTR FileName,VARIANT_BOOL *bSucces);
	STDMETHOD(LoadFromFile)(BSTR FileName,VARIANT_BOOL *bSuccess);
	STDMETHOD(SaveToFileAs)(BSTR FileName,TableSaveTypes type,VARIANT_BOOL *bSucces);

	CSheetDoc* GetDoc(){return (CSheetDoc*)m_doc.GetDocument();}
	CSheetView* GetView(CSheetDoc* pDoc)
	{
		if(pDoc)
		{
			POSITION pos=pDoc->GetFirstViewPosition();
			if(pos)
				return (CSheetView*)pDoc->GetNextView(pos);
		}
		SetError(E_FAIL,"Данная операция допустима только при открытом документе");
		return NULL;
	}
};

#endif // !defined(AFX_ITABLEDOC_H__A3D358A2_7A7C_11D8_9FCD_D6268C95B908__INCLUDED_)
