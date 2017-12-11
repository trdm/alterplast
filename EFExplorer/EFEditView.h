#if !defined(AFX_EFEDITVIEW_H__81DCCA30_AAE2_4EC5_A22E_0F40321EFE34__INCLUDED_)
#define AFX_EFEDITVIEW_H__81DCCA30_AAE2_4EC5_A22E_0F40321EFE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EFEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEFEditView view

class CEFEditView : public CEditView
{
protected:
	CEFEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEFEditView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFEditView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEFEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEFEditView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFEDITVIEW_H__81DCCA30_AAE2_4EC5_A22E_0F40321EFE34__INCLUDED_)
