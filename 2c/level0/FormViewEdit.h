#if !defined(AFX_FORMVIEWEDIT_H__51BF38F8_1AD8_446B_9B8C_8DF3E707E788__INCLUDED_)
#define AFX_FORMVIEWEDIT_H__51BF38F8_1AD8_446B_9B8C_8DF3E707E788__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormViewEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormViewEdit form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormViewEdit : public CFormView
{
protected:
	CFormViewEdit();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormViewEdit)

// Form Data
public:
	//{{AFX_DATA(CFormViewEdit)
	enum { IDD = IDD_FORM1C_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormViewEdit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormViewEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWEDIT_H__51BF38F8_1AD8_446B_9B8C_8DF3E707E788__INCLUDED_)
