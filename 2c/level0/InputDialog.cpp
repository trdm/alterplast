//Авторские права - VTOOLS.RU (info@vtools.ru)
// InputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "InputDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDialog dialog


CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	nMultiLine=0;
	nLimit=0;
	nTimer=0;
}


void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDialog)
	DDX_Control(pDX, IDC_EDIT1, m_String);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialog)
	//{{AFX_MSG_MAP(CInputDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDialog message handlers

void CInputDialog::OnOK() 
{
	m_String.GetWindowText(csString);
	
	CDialog::OnOK();
}

BOOL CInputDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(nLimit)
		m_String.SetLimitText(nLimit);
	m_String.SetWindowText(csString);

	if(nTimer)
		SetTimer(10000,nTimer*1000,0);

	if(csTitle.IsEmpty())
		csTitle="Введите строку";
	SetWindowText(csTitle);
	m_String.SetFocus();
	
	if(!nMultiLine)
	{
		CRect Rect;
		m_String.GetWindowRect(Rect);
		ScreenToClient(Rect);
		Rect.bottom=Rect.top+24;
		m_String.SetWindowPos(&wndTop,Rect.left,Rect.top,Rect.Width(),Rect.Height(),SWP_NOMOVE);

		::SetWindowLong(m_String.m_hWnd, GWL_STYLE, ::GetWindowLong(m_String.m_hWnd, GWL_STYLE) ^ (ES_WANTRETURN | ES_MULTILINE | WS_VSCROLL ));
		::SetWindowLong(m_String.m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_String.m_hWnd, GWL_EXSTYLE) ^ (ES_WANTRETURN | ES_MULTILINE ));
		

	}
	
	return FALSE;
}

void CInputDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==10000)
	{
		nTimer=-1;
		KillTimer(10000);
		CDialog::OnCancel();
	}
	CDialog::OnTimer(nIDEvent);
}
