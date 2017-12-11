// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SetupDlg.h"
#include "MyADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CMyADO* pAdo;

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog


CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	m_strCharTerminator = _T("");
	m_bEnableDDGroup = FALSE;
	m_bShowColComm = FALSE;
	m_bShowColID = FALSE;
	m_bShowColType = FALSE;
	m_bEnableFullPathDD = FALSE;
	m_bShow_ID = FALSE;
	//}}AFX_DATA_INIT
}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_CMB_PLACE_SUBS, m_Combo);
	DDX_Control(pDX, IDC_LST_ADD_DD_TEXT, m_LiistAddDDText);
	DDX_Control(pDX, IDC_STATIC_DD_GR_TERM, m_labelCharTerminator);
	DDX_Text(pDX, IDC_EDT_CHAR_TERMINATOR_DD, m_strCharTerminator);
	DDV_MaxChars(pDX, m_strCharTerminator, 4);
	DDX_Check(pDX, IDC_ENABBLE_GR_DD, m_bEnableDDGroup);
	DDX_Check(pDX, IDC_SHOW_COL_COMM, m_bShowColComm);
	DDX_Check(pDX, IDC_SHOW_COL_ID, m_bShowColID);
	DDX_Check(pDX, IDC_SHOW_COL_TYPE, m_bShowColType);
	DDX_Check(pDX, IDC_ENABBLE_FULL_PATH_DD, m_bEnableFullPathDD);
	DDX_Check(pDX, IDC_SHOW_ID_TO_TREE, m_bShow_ID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_BN_CLICKED(IDC_ENABBLE_FULL_PATH_DD, OnEnabbleFullPathDd)
	ON_BN_CLICKED(IDC_ENABBLE_GR_DD, OnEnabbleGrDd)
	ON_CBN_SELCHANGE(IDC_CMB_PLACE_SUBS, OnSelchangeCmbPlaceSubs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

BOOL CSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bEnableDDGroup = pAdo->m_Settings.EnableDDGroup;
	m_strCharTerminator = pAdo->m_Settings.strCharsTerminator;
	m_bShowColType = pAdo->m_Settings.ShowColType;
	m_bShowColComm = pAdo->m_Settings.ShowColComm;
	m_bShowColID = pAdo->m_Settings.ShowColID;
	m_bEnableFullPathDD = pAdo->m_Settings.EnableFullPathDD;
	//m_bShow_ID = pAdo->m_Settings.Show_ID;
	
	//0-справа, 1-снизу, 2-слева, 3-сверху
	int ics = pAdo->m_Settings.ShowSubPanePos;
	if (ics>=0 && ics<=3 && m_Combo.GetCount()>3){ 
		m_Combo.SetCurSel(pAdo->m_Settings.ShowSubPanePos);
	}


	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDlg::OnOK() 
{
	// TODO: Add extra validation here
	// Сохраним настройки
	UpdateData(TRUE);
	pAdo->m_Settings.EnableDDGroup = m_bEnableDDGroup?1:0;
	pAdo->m_Settings.strCharsTerminator = (LPCSTR) m_strCharTerminator;
	pAdo->m_Settings.ShowColType = m_bShowColType ?1:0;
	pAdo->m_Settings.ShowColComm = m_bShowColComm?1:0;
	pAdo->m_Settings.ShowColID = m_bShowColID ?1:0;
	pAdo->m_Settings.EnableFullPathDD = m_bEnableFullPathDD ?1:0;	
	pAdo->m_Settings.ShowSubPanePos = m_Combo.GetCurSel();
	pAdo->m_Settings.Show_ID = m_bShow_ID ?1:0;
	pAdo->SaveOrLoadSettings();

	
	CDialog::OnOK();
}


void CSetupDlg::OnEnabbleFullPathDd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bEnableDDGroup && m_bEnableFullPathDD){ 
		m_bEnableDDGroup = !m_bEnableDDGroup;
	}
	UpdateData(FALSE);
	
}

void CSetupDlg::OnEnabbleGrDd() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	if (m_bEnableDDGroup && m_bEnableFullPathDD){ 
		m_bEnableFullPathDD = !m_bEnableFullPathDD;
	}
	UpdateData(FALSE);

	
}

void CSetupDlg::OnSelchangeCmbPlaceSubs() 
{
	// TODO: Add your control notification handler code here
	//0-справа, 1-снизу, 2-слева, 3-сверху
	int iLBCurSel = m_Combo.GetCurSel();
	if (iLBCurSel != LB_ERR){ 
		//pAdo->m_Settings.ShowSubPanePos = iLBCurSel;
	}	
}
