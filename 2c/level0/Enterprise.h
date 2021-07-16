// Авторские права - VTOOLS.RU (info@vtools.ru)
// Enterprise.h : main header file for the Enterprise application
//

#if !defined(AFX_Enterprise_H__60B693B0_C732_464D_AE66_C2CFD04B93CD__INCLUDED_)
#define AFX_Enterprise_H__60B693B0_C732_464D_AE66_C2CFD04B93CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ReportManager.h"

/////////////////////////////////////////////////////////////////////////////
class CPluginServ{
	HMODULE mod;
	CPluginServ(){;};
	~CPluginServ(){;};
public:
	CPluginServ* Next;
	PLUGIN_INFO Info;

	static void Init();
	static void Done();
};
/////////////////////////////////////////////////////////////////////////////
// CEnterpriseApp:
// See Enterprise.cpp for the implementation of this class
//
class CEnterpriseApp : public CWinApp
{
public:
	CEnterpriseApp();
	~CEnterpriseApp();

	int Run();
	int MyRun();


	static CReportManager	*m_pSystemManager;
	CEdit m_wndSplash;

	void DeleteConfigName();
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnterpriseApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEnterpriseApp)
	afx_msg void OnAppAbout();
	afx_msg void OnButtonTest();
	afx_msg void OnReports();
	afx_msg void OnObrabotka();
	afx_msg void OnJurnaldoc();
	afx_msg void OnSubconto();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnEditCut();
	afx_msg void OnButtonEnterprise();
	afx_msg void OnUpdateButtonEnterprise(CCmdUI* pCmdUI);
	afx_msg void OnConfig();
	afx_msg void OnUpdateConfig(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnOpenconfig();
	afx_msg void OnUpdateOpenconfig(CCmdUI* pCmdUI);
	afx_msg void OnAllFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Enterprise_H__60B693B0_C732_464D_AE66_C2CFD04B93CD__INCLUDED_)
