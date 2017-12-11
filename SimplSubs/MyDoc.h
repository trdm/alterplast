#if !defined(AFX_MYDOC_H__BA1C1C6D_577F_4E4F_98E3_39968A86741E__INCLUDED_)
#define AFX_MYDOC_H__BA1C1C6D_577F_4E4F_98E3_39968A86741E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDoc document

class CMyDoc : public CDocument
{
protected:
	CMyDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyDoc)

// Attributes
public:
	CMapStringToString m_MDChildNames;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateTreeMD (CTreeCtrl * nTreCtrl);
	virtual ~CMyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDOC_H__BA1C1C6D_577F_4E4F_98E3_39968A86741E__INCLUDED_)
