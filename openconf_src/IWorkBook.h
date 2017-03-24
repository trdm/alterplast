// IWorkBook.h: interface for the CIWorkBook class.
#if !defined(AFX_IWORKBOOK_H__E091E815_B915_496B_889F_CA85D0A15A20__INCLUDED_)
#define AFX_IWORKBOOK_H__E091E815_B915_496B_889F_CA85D0A15A20__INCLUDED_
#include "icfgdoc.h"

class CIWorkBook : public CCfgDocDerived<IWorkBook,WorkBook>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IWorkBook)
	END_COM_MAP()

	CIWorkBook(CDocument* pDoc);
	virtual ~CIWorkBook();

	STDMETHOD(get_CountPages)(long *pVal);
	STDMETHOD(get_NamePage)(VARIANT idx,BSTR *pVal);
	STDMETHOD(put_NamePage)(VARIANT idx,BSTR newVal);
	STDMETHOD(get_Page)(VARIANT idx,ICfgDoc **pVal);
	STDMETHOD(get_ActivePage)(VARIANT *pVal);
	STDMETHOD(put_ActivePage)(VARIANT newVal);
	STDMETHOD(AddPage)(BSTR Name,ICfgDoc **ppNewPage);
	STDMETHOD(RemovePage)(VARIANT idx);

	CWorkBookDoc* GetWB()
	{
		return (CWorkBookDoc*)m_doc.GetDocument();
	}
	int GetPageMinCount(CWorkBookDoc* pDoc);
	CDocument* GetPage(CWorkBookDoc* pDoc,VARIANT& idx,int& pos,CString* name=NULL);
};

#endif
