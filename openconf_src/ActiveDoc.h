#if !defined(AFX_ACTIVEDOC_H__A86059D6_CD1A_46FA_80DD_C83588ACD008__INCLUDED_)
#define AFX_ACTIVEDOC_H__A86059D6_CD1A_46FA_80DD_C83588ACD008__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActiveDoc document

class CActiveDoc : public COleDocument
{
protected:
	DECLARE_DYNCREATE(CActiveDoc)

// Attributes
public:
	CActiveDoc();           // protected constructor used by dynamic creation
	static void Init();
	BOOL IsModified(){return FALSE;}

// Operations
public:
	static CMultiDocTemplate* m_pTemplate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnOpenPlugin(IDispatchPtr& pPlugin);
	static void OpenPlugin(IDispatchPtr& pPlugin);
	static IDispatch* OpenForm(BSTR progID,BSTR title);
	virtual ~CActiveDoc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CActiveDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEDOC_H__A86059D6_CD1A_46FA_80DD_C83588ACD008__INCLUDED_)
