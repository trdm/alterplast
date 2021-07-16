//Авторские права - gpl2c.ru (gpl2c@pochta.ru)
// InputDate.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "InputDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CInputDate dialog


CInputDate::CInputDate(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDate::IDD, pParent)
{
	//EnableAutomation();
	//{{AFX_DATA_INIT(CInputDate)
	m_Date2 = 0;
	//}}AFX_DATA_INIT
}

void CInputDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDate)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Date1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Date2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDate, CDialog)
	//{{AFX_MSG_MAP(CInputDate)
	ON_WM_TIMER()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputDate message handlers

BOOL CInputDate::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(csTitle.IsEmpty())
		csTitle="Введите дату";
	SetWindowText(csTitle);
	m_Date1.SetFocus();
	if(nTimer)
		SetTimer(10000,nTimer*1000,0); 
	return false;
	
}

void CInputDate::OnOK()
{
	
	
	CDialog::OnOK();
}

void CInputDate::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==10000)
		nTimer=-1;
	KillTimer(10000);
	CDialog::OnCancel();
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CInputDate::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME sTime;
	CString csDate1;
	m_Date1.GetWindowText(csDate1);
	CValue cvDate=Date(csDate1);
	int nYear,nMonth,nDay;
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear  =nYear;
	sTime.wMonth =nMonth;
	sTime.wDay   =nDay;
	sTime.wHour  =0;
	sTime.wMinute=0;
	sTime.wSecond=0;
	m_Date2  =CTime(sTime);
	*pResult = 0;
}
