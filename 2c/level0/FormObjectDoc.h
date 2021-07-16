// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_FORMOBJECTDOC_H__80E7A99D_F814_46A7_B292_80E91490966E__INCLUDED_)
#define AFX_FORMOBJECTDOC_H__80E7A99D_F814_46A7_B292_80E91490966E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormObjectDoc.h : header file
//

#include "DocList.h"
/////////////////////////////////////////////////////////////////////////////
// CFormObjectDoc document

class CFormObjectDoc : public CMetaDocument
{
protected:
	CFormObjectDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormObjectDoc)

// Attributes
public:
//	bool bCanCloseFrame;

// Operations
public:
	class CFormObject *GetView()
	{
	   POSITION pos = GetFirstViewPosition();
	   return (class CFormObject*)GetNextView(pos);
	};
	void OnChangedViewList();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormObjectDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormObjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFormObjectDoc)
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMOBJECTDOC_H__80E7A99D_F814_46A7_B292_80E91490966E__INCLUDED_)
