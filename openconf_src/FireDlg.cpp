// FireDlg.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "FireDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CFireDlg dialog


CFireDlg::CFireDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFireDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFireDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFireDlg)
	DDX_Control(pDX, IDC_FIRE, m_fire);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFireDlg, CDialog)
	//{{AFX_MSG_MAP(CFireDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireDlg message handlers

BOOL CFireDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString fname=CConfigSvcImpl::m_pTheService->m_BinPath+"config.dll";
	DWORD z,size;
	size=GetFileVersionInfoSize((LPTSTR)(LPCTSTR)fname,&z);
	if(size)
	{
		static const char* EngNames[]={
			"FileVersion",
			"PrivateBuild",
		};
		char* pVersion=new char[size];
		GetFileVersionInfo((LPTSTR)(LPCTSTR)fname,0,size,pVersion);
		DWORD* translate;
		UINT len;
		VerQueryValue(pVersion,"\\VarFileInfo\\Translation",(void**)&translate,&len);
		CString blockName;
		blockName.Format("\\StringFileInfo\\%04x%04x\\",LOWORD(translate[0]),HIWORD(translate[0]));
		LPCTSTR valuebuf;
		for(int i=0;i<sizeof(EngNames)/sizeof(EngNames[0]);i++)
		{
			CString fdName=blockName+EngNames[i];
			VerQueryValue(pVersion,(char*)(LPCTSTR)(fdName),(void**)&valuebuf,&len);
			if(len)
			{
				m_version[i]=valuebuf;
			}
		}
		delete pVersion;
	}
	CString txt;
	GetDlgItem(IDC_VERSION)->GetWindowText(txt);
	CString newTxt;
	CString ins=m_version[0]+m_version[1];
	newTxt.Format(txt,ins);
	GetDlgItem(IDC_VERSION)->SetWindowText(newTxt);

	
	m_fire.InitFire(CFireWnd::red);
	int m_uTimerID = SetTimer(0x451,10,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFireDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);
	m_fire.RenderFlame();
	m_fire.PaintFlame();

	// Eat spurious WM_TIMER messages
	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}
