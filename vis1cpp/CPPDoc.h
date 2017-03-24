#if !defined(AFX_CPPDOC_H__CB0C7001_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_)
#define AFX_CPPDOC_H__CB0C7001_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPPDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPPDoc document
CString PrintRTC(CObject*);
CString PrintRTC(CRuntimeClass*);

#include <boost\scoped_ptr.hpp>

#include ".\1cpp\MetaDataOfClasses.h"

class CCPPDoc : public CDocument
{
protected:
	CCPPDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCPPDoc)

// Attributes
private:
	boost::scoped_ptr<CMetaDataOfClasses> m_Meta;  

	// Operations
public:

	void UpdateMetadata();
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPPDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCPPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCPPDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPPDOC_H__CB0C7001_5A5C_11D7_8D5D_DF0D47561404__INCLUDED_)
