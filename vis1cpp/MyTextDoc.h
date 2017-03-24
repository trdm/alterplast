// MyTextDoc.h : header file
//

#if !defined(AFX_MYTEXTDOC_H__21B4BEE5_3459_4EFB_A66D_109D1061BE36__INCLUDED_)
#define AFX_MYTEXTDOC_H__21B4BEE5_3459_4EFB_A66D_109D1061BE36__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyTextDoc document

class CMyTextDoc;

class CDocumentInterceptor
{
	friend class CMyTextDoc; 
public:
	static void HookDoc(CDocument* pDoc, LPCSTR title, HICON hIcon=NULL);
	
	static CMyTextDoc* LookupDoc(LPCSTR strDocPath);
	static bool HaveUnsavedDocuments();
	static void SendSaveCommandToAllDocuments();
	
	// TODO artbear - к сожалению, в VC 6 не работает способ определения подобной функции-шаблона вне тела класса
	template<typename UnaryOp>
		static void ForEachDocument(UnaryOp op)
	{
		m_Hooked.ForEachValue(op);
	}
	
private:
	typedef	void (CDocument::*PFONCLOSE)();
	typedef	BOOL (CDocument::*PFDOFSAVE)();
	// 	typedef	BOOL (CDocument::*PFOnOpenDocument)(LPCTSTR lpszPathName);
	// 	typedef	BOOL (CDocument::*PFOnNewDocument)();
	
	union PtrConv{
		void* pV;
		PFONCLOSE pOnClose;
		PFDOFSAVE pDoSave;
		// 		PFOnOpenDocument pOnOpen;
		// 		PFOnNewDocument pOnNew;
	};
	
	static CIStringMap<DWORD*, DWORD*> m_pVtableMap;
	//static CTypedPtrMap<CMapStringToPtr, CString, void*>  m_OnCloseMap; 
	static CMapStringToPtr m_OnCloseMap;
	static CMapStringToPtr m_DoSaveMap;
	// 	static CMapStringToPtr m_OnOpenMap;
	// 	static CMapStringToPtr m_OnNewMap;
	
	static CIStringMap<CMyTextDoc*, CMyTextDoc*> m_Hooked;
	
 	static void RunOrig_OnCloseDocument(CMyTextDoc* pDoc);
 	static BOOL RunOrig_DoFileSave(CMyTextDoc* pDoc);
	// 	BOOL OnOpenDocumentMy(LPCTSTR lpszPathName);
	// 	BOOL OnNewDocumentMy();
};

// TODO данный класс не должен иметь собственных полей, используется только для перехвата - необходим рефакторинг для точного обозначения этого файла в коде
class CMyTextDoc : public CDocument
{
	friend class CDocumentInterceptor;
public:
	CMyTextDoc();           // protected constructor used by dynamic creation
	virtual ~CMyTextDoc();

protected:
	DECLARE_DYNCREATE(CMyTextDoc)

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTextDoc)
	public:
	//}}AFX_VIRTUAL

private:
 	void OnCloseDocumentMy();
 	BOOL DoFileSaveMy();
// 	BOOL OnOpenDocumentMy(LPCTSTR lpszPathName);
// 	BOOL OnNewDocumentMy();

	void ST(const CString& t){m_strTitle=t;}
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTextDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTEXTDOC_H__21B4BEE5_3459_4EFB_A66D_109D1061BE36__INCLUDED_)
