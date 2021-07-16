// FindTextDialog.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FindTextDialog.h"
#include "SyntaxColor\editreg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString csUserPath;//каталог пользователя для записи настроек
#define FILE_FLIST_NAME csUserPath+"\\find_text_all.lst"
CValue ValueFromFile(CString csFileName);
void ValueToFile(CValue *Val,CString csFileName);

/////////////////////////////////////////////////////////////////////////////
// CFindTextDialog dialog


CFindTextDialog::CFindTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTextDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindTextDialog)
	m_csFind = _T("");
	m_bMatchCase = FALSE;
	//}}AFX_DATA_INIT
}


void CFindTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTextDialog)
	DDX_Text(pDX, IDC_EDIT1, m_csFind);
	DDX_Check(pDX, IDC_CHECK1, m_bMatchCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindTextDialog, CDialog)
	//{{AFX_MSG_MAP(CFindTextDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindTextDialog message handlers

BOOL CFindTextDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LoadState();

	UpdateData(0);

	ASSERT(GetDlgItem(IDC_EDIT1));
	GetDlgItem(IDC_EDIT1)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindTextDialog::OnOK() 
{
	UpdateData(1);

	SaveState();
	
	CDialog::OnOK();
}

void CFindTextDialog::SaveState()
{
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, m_bMatchCase);
	//pApp->WriteProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, m_bWholeWord);
	pApp->WriteProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, m_csFind);
}

void CFindTextDialog::LoadState()
{
	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	m_bMatchCase = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_MATCH_CASE, FALSE);
	//m_bWholeWord = pApp->GetProfileInt(REG_FIND_SUBKEY, REG_WHOLE_WORD, FALSE);
	m_csFind = pApp->GetProfileString(REG_FIND_SUBKEY, REG_FIND_WHAT, _T(""));
}
