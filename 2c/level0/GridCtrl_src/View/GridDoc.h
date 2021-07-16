// GridDoc.h : interface of the CGridDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GridDoc_H__19E18FBD_DFF6_11D1_8CE7_000000000000__INCLUDED_)
#define AFX_GridDoc_H__19E18FBD_DFF6_11D1_8CE7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "../../DocList.h"
#include "GridCtrlExt.h"
#include "undo.h"

class CGridDoc : public CMetaDocument, public CUndo
{
protected: // create from serialization only
	CGridDoc();
	DECLARE_DYNCREATE(CGridDoc)

// Attributes
public:
	CGridCtrlExt *m_pGridCtrl;
	//BOOL	bDelete;

	CGridCtrlExt* GetGrid(){return m_pGridCtrl;};
	CString Buffer;


	CView *GetView()
	{
	   POSITION pos = GetFirstViewPosition();
	   return GetNextView(pos);
	};
	virtual void DeleteContents();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
//	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GridDoc_H__19E18FBD_DFF6_11D1_8CE7_000000000000__INCLUDED_)
