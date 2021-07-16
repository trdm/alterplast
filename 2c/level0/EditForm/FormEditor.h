// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
#if !defined(AFX_FORMEDITOR_H__E1B94053_BC3F_4EB4_8824_064F062D8F85__INCLUDED_)
#define AFX_FORMEDITOR_H__E1B94053_BC3F_4EB4_8824_064F062D8F85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormEditor.h : header file
//
#include "FormDialog.h"
#include "LayerSelect.h"
#include "ChangeOrder.h"

/////////////////////////////////////////////////////////////////////////////
// CFormEditor view

#define CBaseClass CView //CFormView
class CFormEditor : public CBaseClass
{
protected:
	CFormEditor();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormEditor)


	enum { IDD = IDD_FORMUNIT };



public:
	CMultiRectTracker *m_FormSelector;

	//поддержка работы со слоями (аля 1С 7.7)
	CLayerSelect m_Layers;
	CArray <CChangeOrderItem,CChangeOrderItem&> m_aChangeOrder;
	
	//Методы:
	CString GetStrLayers();
	CString GetLayerName();
	BOOL IsVisibleLayer(CString csName);
	void InvalidateSelector();
	void ClearData(); 


	//поддержка свойств
	void ShowProperty(bool bShow);


	void LoadData(int bCheckNew=0);
	void GetWindowText(CString &csText);
	void SetWindowText(CString csText);
	CValue AllElements;

//Поддержка Undo+Redo
	void SetModifiedFlag(int bSet=1);
	CArray <CString,CString> listStrUndo;
	int nCurPosUndo;

	void EndSelectMode(WPARAM wParam, LPARAM lParam);
	void MessageShowProperty(WPARAM wParam, LPARAM lParam);

// Attributes
public:
	CFormDialog *m_pDialog;
	CToolBar* m_pToolBox;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormEditor)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	//{{AFX_MSG(CFormEditor)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnDynbuttonedit();
	afx_msg void OnDynbuttoneditmulti();
	afx_msg void OnDynbuttonlabel();
	afx_msg void OnDynbuttongroup();
	afx_msg void OnDynbuttondate();
	afx_msg void OnDynbuttoncombo();
	afx_msg void OnDynbuttoncheck();
	afx_msg void OnDynbuttonradio();
	afx_msg void OnDynbutton();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelectAll();
	afx_msg void OnEditGrid();
	afx_msg void OnEditAllignGrid();
	afx_msg void OnUpdateEditGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditAllignGrid(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDynbuttongrid();
	afx_msg void OnDynbuttontree();
	afx_msg void OnDynbuttonleftalign();
	afx_msg void OnUpdateDynbuttonleftalign(CCmdUI* pCmdUI);
	afx_msg void OnDynbuttonrightalign();
	afx_msg void OnDynbuttontopalign();
	afx_msg void OnDynbuttonbottomalign();
	afx_msg void OnDynbuttonmakesamesize();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnButtonprogress();
	afx_msg void OnButtonslider();
	afx_msg void OnDynbuttonmakesamewidth();
	afx_msg void OnDynbuttonmakesameheight();
	afx_msg void OnCnterinviewHoriz();
	afx_msg void OnUpdateCnterinviewHoriz(CCmdUI* pCmdUI);
	afx_msg void OnCnterinviewVert();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButtonformobject();
	afx_msg void OnButtonTab();
	afx_msg void OnBringToFront();
	afx_msg void OnUpdateBringToFront(CCmdUI* pCmdUI);
	afx_msg void OnSendToBack();
	afx_msg void OnLayers();
	afx_msg void OnChangeOrder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEDITOR_H__E1B94053_BC3F_4EB4_8824_064F062D8F85__INCLUDED_)
