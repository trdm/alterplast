// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_METEDATADOCUMENT_H__73AD0FFD_6ABF_4547_9E4A_44F5FCD621CB__INCLUDED_)
#define AFX_METEDATADOCUMENT_H__73AD0FFD_6ABF_4547_9E4A_44F5FCD621CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MetedataDocument.h : header file
//
//#include "MetaData.h"
//#include "MetadataTree.h"


/////////////////////////////////////////////////////////////////////////////
// CMetedataDocument document

class CMetedataDocument : public CDocument
{
protected:
	CMetedataDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMetedataDocument)

	void SetModifiedFlag(BOOL bFlag);
// Attributes
public:


// Operations
public:
	class CMetadataTree *GetView()
	{
	   POSITION pos = GetFirstViewPosition();
	   return (CMetadataTree*)GetNextView(pos);
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMetedataDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL SaveModified();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMetedataDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnFileSave();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMetedataDocument)
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METEDATADOCUMENT_H__73AD0FFD_6ABF_4547_9E4A_44F5FCD621CB__INCLUDED_)
