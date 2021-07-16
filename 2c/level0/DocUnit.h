//Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_DOCUNIT_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_)
#define AFX_DOCUNIT_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DocUnit.h : header file
//
#include "DocList.h"
/////////////////////////////////////////////////////////////////////////////
// ИсполняемаяФорма - объект-переключатель для запуска форм в конфигураторе режиме исполнения
// CDocUnit document

class CDocUnit : public CMetaDocument
{
public:
	CDocUnit();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDocUnit)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocUnit)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDocUnit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDocUnit)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCUNIT_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_)
