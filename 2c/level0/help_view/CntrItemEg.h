// CntrItem.h : interface of the CRichEgCntrItem class
//

#if !defined(AFX_CNTRITEM_H__9C6AB8E5_7C79_4E14_9EB5_76D38A6E761A__INCLUDED_)
#define AFX_CNTRITEM_H__9C6AB8E5_7C79_4E14_9EB5_76D38A6E761A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRichEgDoc;
class CRichEgView;

class CRichEgCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CRichEgCntrItem)

// Constructors
public:
	CRichEgCntrItem(REOBJECT* preo = NULL, CRichEgDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CRichEgDoc* GetDocument()
		{ return (CRichEgDoc*)CRichEditCntrItem::GetDocument(); }
	CRichEgView* GetActiveView()
		{ return (CRichEgView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEgCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CRichEgCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__9C6AB8E5_7C79_4E14_9EB5_76D38A6E761A__INCLUDED_)
