// RichEgDoc.h : interface of the CRichEgDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RICHEGDOC_H__90E9AC8F_C76C_45E4_BC5F_19675963C896__INCLUDED_)
#define AFX_RICHEGDOC_H__90E9AC8F_C76C_45E4_BC5F_19675963C896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRichEgDoc : public CRichEditDoc
{
protected: // create from serialization only
	CRichEgDoc();
	DECLARE_DYNCREATE(CRichEgDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEgDoc)
	public:
	//}}AFX_VIRTUAL
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;

// Implementation
public:
	virtual ~CRichEgDoc();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRichEgDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEGDOC_H__90E9AC8F_C76C_45E4_BC5F_19675963C896__INCLUDED_)
