#if !defined(AFX_CHANGEORDER_H__100466E4_1EF8_4A47_BC45_2CC046903D68__INCLUDED_)
#define AFX_CHANGEORDER_H__100466E4_1EF8_4A47_BC45_2CC046903D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeOrder.h : header file
//
#include "../GridCtrl_src/GridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CChangeOrder dialog
struct CChangeOrderItem
{
public:
	CChangeOrderItem()
	{
	};
	CString csLayer;
	CString csCapture;
	CString csType;
	CString csId;
	CDynControl* pControl;
	
};
class CChangeOrder : public CDialog
{
// Construction
public:
	CChangeOrder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeOrder)
	enum { IDD = IDD_CHANGEORDER };
	CButton	m_Cancel;
	//}}AFX_DATA
	int nCancel;
	
	CArray <CChangeOrderItem,CChangeOrderItem&> aList;

	class CFormEditor *pEditor;//обратная связь с редактором форм для выдачи сообщений по изменению состава слоев

	CToolBar m_ToolBar;
	CGridCtrl m_Grid;


	void ReLoadGrid();
	void AttachEditor(class CFormEditor *);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeOrder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetType(int nType);
	
	// Generated message map functions
	//{{AFX_MSG(CChangeOrder)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpElement();
	afx_msg void OnDownElement();
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEORDER_H__100466E4_1EF8_4A47_BC45_2CC046903D68__INCLUDED_)
