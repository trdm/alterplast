// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_MetadataTree_H__98576F73_ED41_444F_8084_BAF324681721__INCLUDED_)
#define AFX_MetadataTree_H__98576F73_ED41_444F_8084_BAF324681721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MetadataTree.h : header file
//

#define MODE_INSERT		1
#define MODE_REPLACE	2
#define MODE_SAVE		3



#include "Metadata.h"
#include "MetedataDocument.h"
#include "ModuleManager.h"
//#include "FormObjectDoc.h"
#include "DocList.h"


typedef CArray<class CCopyElement,class CCopyElement&> CCopyArray;
//Поддержка операций копирования
class CCopyElement
{
public:
	CCopyElement()
	{
		pObj=0;
	};
	CObjectData data;
	CMetaObject *pObj;

	CCopyArray aChildList;
};

class CDocList;
/////////////////////////////////////////////////////////////////////////////
// CMetadataTree view
class CMetadataTree : public CTreeView
{
protected:
	CMetadataTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMetadataTree)
// Attributes
public:
	CMap <HTREEITEM,HTREEITEM,CDocList*,CDocList*> metaDocList;//список видимых объектов (документов конфигурации)
	CMap <HTREEITEM,HTREEITEM,CMetaObject*,CMetaObject*> buferMetaObj;


	//типы объекта
	CMap <HTREEITEM,HTREEITEM,CObjectData,CObjectData> dataMetaObj;


//	CMapStringToOb docObjList;//список видимых объектов
//	CMapStringToPtr metaObjList;//список метаданных объектов
	BOOL bIsClose;

	//CMetadata m_Metadata;
	//CString csPathName;

	HTREEITEM treeMODULES;
	HTREEITEM treeFORMS;
	HTREEITEM treeMAKETS;
	HTREEITEM treeOBJECTS;
	HTREEITEM TreeMetadata;
	HTREEITEM TreeGlobalModule;
	HTREEITEM TreeReadOnly;

	HTREEITEM hTrash;
	HTREEITEM hTrashDelete;

	HTREEITEM hDocuments;

	BOOL IsTrash(HTREEITEM hItem);

	//для дин. меню:
	CArray <HTREEITEM,HTREEITEM> aMenuItem;
	
//	CSimpleProperty m_SimpleProperty2;//свойства объектов


// Operations
	void OnAllFind();
	void AllFind(CString csFind,int nCase,int nWorld);

	void SaveOpenModulesState(int nMode=1);//0-записать пустой файл,1-записать состояния окон
	void LoadOpenModulesState();

	void CheckModified();
	int EditModule(CString csObjName,int nLine,BOOL bSetRunLine=0);
	void EditModule(CString csObjName);
	HTREEITEM InsertFromCopyList(CCopyElement &cElement,HTREEITEM hParent,HTREEITEM hInsertAfter = TVI_LAST);
	void AddToCopyList(CCopyArray &aChildList,HTREEITEM hParent);
	void EndDrag() ;
	CDocument* Edit();
	BOOL IsEditable(HTREEITEM CurItem0=0);
	BOOL IsGroup(HTREEITEM CurItem0=0);
	BOOL IsType(UINT nPropert,HTREEITEM CurItem0=0,BOOL bNotCheckGroup=0);
	void Remove(HTREEITEM CurItem=0);
	void SaveReplaceForm(int nMode,CString csFile="");
	void GetAllData();
	void NewItem(int nShowItem,CString csNewName="",HTREEITEM hParent=0,int nMode=0);
	BOOL SaveMetadata();
	HTREEITEM GetParentForNew();
	CString GetNewName(HTREEITEM hParent,CString csPrefix="");
	CString GetNewName(HTREEITEM hParent);
	HTREEITEM AddObjectItem(CString csName,HTREEITEM hParent,BOOL nNew=0);
	CString GetPath(HTREEITEM CurItem);
	CMetaObject *GetMetaObject(HTREEITEM CurItem0=0);
	CDocument* OpenForm(CMetaObject *pObj,CString csClassName,CString csName);
	void RemoveFromList(CMetaObject *pObj,CString csName);
	void Clear();
	void InsertSystemElements();


	void LoadMetaFolder(CValue Val,HTREEITEM hParentItem);
	void LoadObjectList();
	void AddMenuInfo(CMenu* pPopup,HTREEITEM hParentItem,int &nPos);
	void GetFormArray(CStringArray &aList,HTREEITEM hParentItem=0);

	
	void ShowProperty(bool bShow);
	BOOL Save();
	BOOL Load(int nReload=0);
	class CMetedataDocument *GetDocument()
	{
		return (CMetedataDocument *)CTreeView::GetDocument();
	};

	void OnSelectMenu(UINT nID);

	BOOL IsCanView(CString csName);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMetadataTree)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMetadataTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	afx_msg void OnModuleOpen();
protected:
	//{{AFX_MSG(CMetadataTree)
	afx_msg void OnNew();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEdit();
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRemove();
	afx_msg void OnProperty();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnInsertForm();
	afx_msg void OnSaveform();
	afx_msg void OnReplaceform();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnModuleConfig();
	afx_msg void OnUpdateModuleConfig(CCmdUI* pCmdUI);
	afx_msg void OnModuleRun();
	afx_msg void OnUpdateModuleRun(CCmdUI* pCmdUI);
	afx_msg void OnOpenconfig();
	afx_msg void OnNewFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MetadataTree_H__98576F73_ED41_444F_8084_BAF324681721__INCLUDED_)
