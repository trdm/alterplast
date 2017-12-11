#if !defined(AFX_EFTREEVIEW_H__58C53CF2_E9AE_48E8_B1BF_26BC4B58B5F6__INCLUDED_)
#define AFX_EFTREEVIEW_H__58C53CF2_E9AE_48E8_B1BF_26BC4B58B5F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EFTreeView.h : header file
//

#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CEFTreeView view

class CEFTreeView : public CTreeView
{
protected:
	CEFTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEFTreeView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEFTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEFTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
  CTreeCtrl* m_tree;
  CImageList m_imageList;
  CString m_IBDir;
  HTREEITEM MakeBranchOfTree(HTREEITEM hParentItem, HTREEITEM hAfterItem, CString Path, CString Name, int& CntItem, void* ExceptPath);
  void LoadDescrFile(CString fileName);
  void UpdateTree();
  void LoadCheckBoxState(int DlgItem, const char* ValueName);
  void SaveCheckBoxState(int DlgItem, const char* ValueName);
  void GroupByFirstSymbol(HTREEITEM hParentItem);
  long OpenDoc();
};

class CItemData
{
public:
  CItemData(CString Path, BOOL IsFolder, int Type):m_Path(Path)
                                                  ,m_IsFolder(IsFolder)
                                                  ,m_Type(Type){};
  ~CItemData(){};
  CString m_Path;
  BOOL m_IsFolder;
  int m_Type;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFTREEVIEW_H__58C53CF2_E9AE_48E8_B1BF_26BC4B58B5F6__INCLUDED_)
