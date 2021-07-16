// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_FORMOBJECT_H__B95D5F30_D24B_461E_948A_A4F8BDD1DD2C__INCLUDED_)
#define AFX_FORMOBJECT_H__B95D5F30_D24B_461E_948A_A4F8BDD1DD2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormObject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormObject form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ZipArchive\ZipArchive.h"	
#include "MetaObject.h"

#include "DragDialog\DragFormView.h"




class CFormObject : public CDragFormView
{
protected:
	CFormObject();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormObject)

// Form Data
public:
	HICON m_hIcon;
	bool bCanModifing;

	void UpdateData();
	void Load(CZipArchive &m_zip);
	void OnUpdate();





	//{{AFX_DATA(CFormObject)
	enum { IDD = IDD_FORMOBJECT_FORM };
	CButton	m_Close;
	CButton	m_OK;
	CButton	m_Use;
	CEdit	m_Alias;
	CComboBox	m_Parent;
	CEdit	m_Name;
	CEdit	m_Comment;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	afx_msg void OnFileSave();
	afx_msg void OnModule();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormObject)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormObject();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormObject)
	afx_msg void OnChangeObjectname();
	afx_msg void OnCloseform();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	afx_msg void OnUse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMOBJECT_H__B95D5F30_D24B_461E_948A_A4F8BDD1DD2C__INCLUDED_)
