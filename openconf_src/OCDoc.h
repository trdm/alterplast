#ifndef AFX_OCDOC_H
#define AFX_OCDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OCDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COCDoc document
#include "OCView.h"
#include "resource.h"
#include "scriptmaneger.h"
#include "pluginitem.h"

class COCDoc : public CDocument
{
protected:
	COCDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COCDoc)

// Attributes
public:
	static void OnCreateConfigWindow(CWnd* pWnd)
	{
		static HICON hIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1));
		LOG("Add OpenConf tab");
		CConfigSvcImpl::m_pTheService->AddConfigTab("OpenConf",hIcon,RUNTIME_CLASS(COCDoc),RUNTIME_CLASS(COCView));
		LOG("Try create Plugin pages...");
		CPluginItem::CreateConfigPages();
		LOG("Create Plugin pages success");
	}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCDoc)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COCDoc();

	// Generated message map functions
protected:
	//{{AFX_MSG(COCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OCDOC_H__5194DD21_E968_11D7_8D5E_F54993C54103__INCLUDED_)
