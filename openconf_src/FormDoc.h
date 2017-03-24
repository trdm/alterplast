// FormDoc.h : header file
#ifndef AFX_FORMDOC_H
#define AFX_FORMDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CFormDoc document

class CFormDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CFormDoc)

// Attributes
public:
	CFormDoc();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormDoc();
	// Generated message map functions
protected:
	//{{AFX_MSG(CFormDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
