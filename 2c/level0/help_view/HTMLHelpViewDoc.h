#if !defined(AFX_HTMLHELPVIEWDOC_H__B69FE254_05B2_11D2_BFD5_006008162563__INCLUDED_)
#define AFX_HTMLHELPVIEWDOC_H__B69FE254_05B2_11D2_BFD5_006008162563__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HTMLHelpViewDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpViewDoc document

class CHTMLHelpViewDoc : public CDocument
{
protected:
	CHTMLHelpViewDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHTMLHelpViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLHelpViewDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHTMLHelpViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHTMLHelpViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLHELPVIEWDOC_H__B69FE254_05B2_11D2_BFD5_006008162563__INCLUDED_)
