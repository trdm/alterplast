// InputStrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InputStrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputStrDlg dialog


CInputStrDlg::CInputStrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputStrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputStrDlg)
	m_StringInput = _T("");
	m_iMaxLen =128;
	//}}AFX_DATA_INIT
}


void CInputStrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputStrDlg)
	DDX_Text(pDX, IDC_EDIT_INPUT, m_StringInput);
	//}}AFX_DATA_MAP
	if (m_iMaxLen!=0){ 
		DDV_MaxChars(pDX, m_StringInput, m_iMaxLen);
	} else {
		DDV_MaxChars(pDX, m_StringInput, 128);
	}
}


BEGIN_MESSAGE_MAP(CInputStrDlg, CDialog)
	//{{AFX_MSG_MAP(CInputStrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputStrDlg message handlers

BOOL CInputStrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (!m_StringCaption.IsEmpty()){ 
		SetWindowText(m_StringCaption);
	}
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputStrDlg::SetOptions(CString nsInputStr, int n_iMaxLen, CString nsCaption)
{
	m_StringInput = nsInputStr;
	m_iMaxLen = n_iMaxLen;
	m_StringCaption = nsCaption;
}


CString CInputStrDlg::GetInput()
{
	CString nStr = (LPCSTR)m_StringInput;
	return nStr;
}
