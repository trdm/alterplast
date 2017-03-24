#if !defined(AFX_LSTCT_H__D2093597_5A85_4579_8FF8_C11837FFAB20__INCLUDED_)
#define AFX_LSTCT_H__D2093597_5A85_4579_8FF8_C11837FFAB20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LstCt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLstCt window

class CLstCt : public CListCtrl
{
// Construction
public:
	CLstCt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLstCt)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLstCt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLstCt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLstCt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSTCT_H__D2093597_5A85_4579_8FF8_C11837FFAB20__INCLUDED_)
