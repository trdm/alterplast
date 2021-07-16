// Авторские права - VTOOLS.RU (info@vtools.ru)
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
#define AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SyntaxColor/CCrystalTextBuffer.h"
#include "DocList.h"
//#include "FormObjectDoc.h"

#define DEFAULT_FORM_MODULE			1
#define DEFAULT_OBJ_RUN_MODULE		2
#define DEFAULT_OBJ_CONF_MODULE		3
#define DEFAULT_OBJ_COMMON_MODULE	4

class CModuleDoc : public CMetaDocument
{
protected: // create from serialization only
	CModuleDoc();
	DECLARE_DYNCREATE(CModuleDoc)

//Атрибуты:
public:
	int nCanDefualtType;


//Методы:


	static CString GetDefaultText(int nTypeText);
	void GetAllData(CString &csModule,CString &csForm,CString &csDescription);
	void SetDefaultText();
//	void OnSaveData();
	void Run(bool bRun);
	void SetText(CString csStr);
	CString GetText();
	CView *GetView()
	{
	   POSITION pos = GetFirstViewPosition();
	   return GetNextView(pos);
	};
	int GetViewCount() ;
	class CFormEditor *GetForm();
	class CMyCodeView *GetModule();

	void SetModifiedFlag( BOOL bModified = TRUE );
	CString GetModulePath();

	class CSampleTextBuffer : public CCrystalTextBuffer
	{
		private:
			CModuleDoc *m_pOwnerDoc;
		public:
			CSampleTextBuffer(CModuleDoc *pDoc) { m_pOwnerDoc = pDoc; };
			virtual void SetModified(BOOL bModified = TRUE)
				{ m_pOwnerDoc->SetModifiedFlag(bModified); };
	};
	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	CSampleTextBuffer m_xTextBuffer;
	LOGFONT m_lf;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
//	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModuleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CModuleDoc)
	afx_msg void OnReadOnly();
	afx_msg void OnUpdateReadOnly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEnterprise(CCmdUI* pCmdUI);
	afx_msg void OnButtonCompile();
	afx_msg void OnButtonRun();
	afx_msg void OnUpdateButtonRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConnect2c(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEDOC_H__B1B69ED1_9FCE_11D2_8CA4_0080ADB8683C__INCLUDED_)
