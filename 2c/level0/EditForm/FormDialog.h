// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// FormDialog.h : header file
//

#if !defined(AFX_FormDialog_H__02909904_6F6D_454E_827B_F94A9F490C48__INCLUDED_)
#define AFX_FormDialog_H__02909904_6F6D_454E_827B_F94A9F490C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "DynDialogEx.h"
#include "DynControl.h"



void swap(long &a,long &b);

#define	WM_ENDSELECT	WM_USER+1
#define	WM_ADDCONTROL	WM_USER+2
#define	WM_SHOWPROPERTY	WM_USER+3
#define	WM_POSTUPDATE	WM_USER+4

#define MIN_SIZE	7//минималная сторона диалогового элемента

#include "MultiRectTracker.h"

/////////////////////////////////////////////////////////////////////////////
// CFormDialog dialog

#define CDIALOG CDialog//CDynDialogEx

class CFormDialog : public CDIALOG
{
// Construction
public:
	CFormDialog(CWnd* pParent = NULL);	// standard constructor
	~CFormDialog();


	//поддержка работы со свойствами
	CDocument *m_pDoc;
	CDocument *GetDocument(){return m_pDoc;};
	class CFormEditor *m_pView;
	class CFormEditor *GetView(){return m_pView;};

	void OnUpdate();
	void ShowProperty(int nShow);
	//свойства самой формы:
	CString m_csName;
	CString m_csValue;
	CString csAlias;
	CString csComment;
	bool	bCanResize;
	int		m_nIcon;

	BOOL bReadOnly;

	CString csCurrentObjectName;


	//поддержка ввода новых элементов
	int nCurrentMode;
	CDynControl *AddControl(int nType,CRect &Rect,bool bGridAllign=1);
	CDynControl *AddControl(int nType,CPoint point);
	CDynControl *AddControl(CValue ObjectVal);
	void ReloadWindows();


	//Мультивыделение
	CMultiRectTracker *m_pTracker;

	CArray <HWND,HWND> aWindowsList;
	CArray <CDynControl*,CDynControl*> aControlList;
	
	void ClearData();


	CPoint ptContextMenu;

	//Сетка
	bool	m_bGrid;
	bool	m_bUseGrid;//привязка к сетке
	int		m_nGridX;
	int		m_nGridY;
	void SetGridSize(int nX, int nY) { m_nGridX = nX; m_nGridY = nY; }

	//Методы:
	void AutoSetDlgItem();
	void BringToFront(int nMode=1);
	CWnd* FindCurrentWindow(CPoint point);
	void EndSelectMode(WPARAM wParam, LPARAM lParam);
	void SelectItem(CWnd* pWnd,int nMode);//0-Remove+Add+Move,1-Add+Remove,2-Add

	void OnAddControl(WPARAM wParam, LPARAM lParam);
	void MoveSelectedWindows(int dx=0,int dy=0,int bResize=0);
	void UseGrid(CRect *pRect);
	CDynControl* GetCurrenElement();
	CDynControl* GetNextElement(int nPrev=0);

	int GetCountSelectedList();
	void GroupsSelectedList(int nMode);
	void OnAddColumn();
	void CopyColumn(int nCol1,int nCol2);


	//Запись
	void SaveAllToValue(CValue &Elements);

	afx_msg void PostUpdate();


//////////////////////////////////////////////////////////////////////////
//EDIT_FORM
//////////////////////////////////////////////////////////////////////////
	static LRESULT CALLBACK PREDblSubclassFunction(
		CWnd* pWnd, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, bool &bDoReturn);
	static bool CALLBACK AllowSubclassCallBack(HWND hWnd);

	CWnd   *m_pEditedCtrl;
//////////////////////////////////////////////////////////////////////////



// Dialog Data
	//{{AFX_DATA(CFormDialog)
	enum { IDD = IDD_EDITDIALOG_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFormDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRemove();
	afx_msg void OnProperty();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPropColumn();
	afx_msg void OnDelColumn();
	afx_msg void OnLeftColumn();
	afx_msg void OnRightColumn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FormDialog_H__02909904_6F6D_454E_827B_F94A9F490C48__INCLUDED_)
