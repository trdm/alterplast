#ifndef AFX_PLUGINPAGEDOC_H
#define AFX_PLUGINPAGEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginPageDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginPageDoc document

class CPluginPageDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CPluginPageDoc)

// Attributes
public:
	CPluginPageDoc();           // protected constructor used by dynamic creation
	IDispatchPtr m_pPlugin;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginPageDoc)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPluginPageDoc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPluginPageDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
