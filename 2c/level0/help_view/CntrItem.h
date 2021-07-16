// CntrItem.h : interface of the CTestForMyHelpDocCntrItem class
//

#if !defined(AFX_CNTRITEM_H__2197F2EA_4AAE_486A_9D90_8EB190B8FC9E__INCLUDED_)
#define AFX_CNTRITEM_H__2197F2EA_4AAE_486A_9D90_8EB190B8FC9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyHelpDoc;
class CMyHelpView;

class CTestForMyHelpDocCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTestForMyHelpDocCntrItem)

// Constructors
public:
	CTestForMyHelpDocCntrItem(REOBJECT* preo = NULL, CMyHelpDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CMyHelpDoc* GetDocument()
		{ return (CMyHelpDoc*)CRichEditCntrItem::GetDocument(); }
	CMyHelpView* GetActiveView()
		{ return (CMyHelpView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestForMyHelpDocCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CTestForMyHelpDocCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__2197F2EA_4AAE_486A_9D90_8EB190B8FC9E__INCLUDED_)
