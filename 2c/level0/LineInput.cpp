//Авторские права - VTOOLS.RU (info@vtools.ru)
// LineInput.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "LineInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineInput dialog


CLineInput::CLineInput(CWnd* pParent /*=NULL*/)
	: CDialog(CLineInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineInput)
	DDX_Control(pDX, IDC_EDIT1, m_Line);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineInput, CDialog)
	//{{AFX_MSG_MAP(CLineInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineInput message handlers
static CString csLastLine;
void CLineInput::OnOK() 
{
	// TODO: Add extra validation here

	
	m_Line.GetWindowText(csLastLine);
	nLine=atoi(csLastLine);

	CDialog::OnOK();
}

BOOL CLineInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

//	if(csLastLine.IsEmpty())
	csLastLine="";
	m_Line.SetWindowText(csLastLine);
	m_Line.SetFocus();
	
	return FALSE;
}
