// FormScript.h : header file
#ifndef AFX_FORMSCRIPT_H
#define AFX_FORMSCRIPT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CFormScript document
class CScripting;
class CFormHolder;

class CFormScript : public COleDocument
{
protected:
	DECLARE_DYNCREATE(CFormScript)

// Attributes
public:
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
	CFormScript();
	CFormHolder* m_pForm;
	static CMultiDocTemplate* m_pFormTemplate;

	ILockBytesPtr m_ilock;
	bool m_IsDesign;
	CString m_module;
	_UserFormPtr m_pUserForm;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormScript)
	public:
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL IsModified();
	bool OpenForm();
	static CFormScript* CreateForm(CString path);
	virtual ~CFormScript();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFormScript)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFormScript)
	//}}AFX_DISPATCH
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMSCRIPT_H__930A3097_F138_11D7_8D5E_B340FDE9FB02__INCLUDED_)
