#if !defined(AFX_SECTIONS_H__34DC760A_FFD9_409D_9D15_2B7F8E2B32D5__INCLUDED_)
#define AFX_SECTIONS_H__34DC760A_FFD9_409D_9D15_2B7F8E2B32D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sections.h : header file
//
#include "Moxcel.h"
#include "GridCtrlExt.h"
#include "../../Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCalcEdit window



/////////////////////////////////////////////////////////////////////////////
// CSections dialog
class CSections : public CDialog
{
// Construction
public:
	CSections(CWnd* pParent = NULL);   // standard constructor
	Create();

// Dialog Data
	//{{AFX_DATA(CSections)
	enum { IDD = IDD_DIALOG_SECTIONS };
	CListCtrl	m_TreeList;
	//}}AFX_DATA

	//секции:
	//CArray <CSection,CSection> *pHorizSection;
	//CArray <CSection,CSection> *pVertSection;
	CGridCtrlExt *pGrid;
	HTREEITEM hHorizSection;
	HTREEITEM hVertSection;

	//void Load(CArray <CSection,CSection> *pHorizSection2,CArray <CSection,CSection> *pVertSection2);
	void Load(CGridCtrlExt *pGrid);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSections)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSections)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnMycancel();
	afx_msg void OnEndlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndSubItemLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginSubItemLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONS_H__34DC760A_FFD9_409D_9D15_2B7F8E2B32D5__INCLUDED_)
