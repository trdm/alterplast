// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_FUNCTIONLIST_H__2DF6806E_B4FD_408C_B7FE_B9C051DE8F56__INCLUDED_)
#define AFX_FUNCTIONLIST_H__2DF6806E_B4FD_408C_B7FE_B9C051DE8F56__INCLUDED_

#include "GridCtrl_src/GridCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunctionList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFunctionList dialog

class CFunctionList : public CDialog
{
// Construction
public:
	CFunctionList(CWnd* pParent = NULL);   // standard constructor


	CGridCtrl m_Grid;
	CArray <CString,CString> aListName;
	CArray <UINT,UINT> aListImage;
	int nLine;
	int nPrevSort;


	void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	void SortGrid();
	int GetNumberInArray(int nRow);
	int GetNumberInGrid(int nIndex);
	void LoadGrid();

	void LoadState();
	void SaveState();

// Dialog Data
	//{{AFX_DATA(CFunctionList)
	enum { IDD = IDD_PROCEDURE_LIST };
	CButton	m_Sort;
	CButton	m_Cancel;
	CButton	m_OK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFunctionList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFunctionList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSort();
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONLIST_H__2DF6806E_B4FD_408C_B7FE_B9C051DE8F56__INCLUDED_)
