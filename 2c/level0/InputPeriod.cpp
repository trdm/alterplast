// InputPeriod.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "InputPeriod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InputPeriod dialog


CInputPeriod::CInputPeriod(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPeriod::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPeriod)
	m_CheckYear = FALSE;
	m_CheckQuart = FALSE;
	m_CheckMonth = FALSE;
	m_QuartData = _T("");
	m_MonthData = _T("");
	m_Date1 = 0;
	m_DateBeg = 0;
	m_DateEnd = 0;
	m_RadioInterval = -1;
	//}}AFX_DATA_INIT
}


void CInputPeriod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPeriod)
	DDX_Control(pDX, IDC_CHECK3, m_CheckMonthCtrl);
	DDX_Control(pDX, IDC_CHECK2, m_CheckQuartCtrl);
	DDX_Control(pDX, IDC_CHECK1, m_CheckYearCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_PeriodEditCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_MonthCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_QuartCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_DateEndCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DateBegCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Date1Ctrl);
	DDX_Check(pDX, IDC_CHECK1, m_CheckYear);
	DDX_Check(pDX, IDC_CHECK2, m_CheckQuart);
	DDX_Check(pDX, IDC_CHECK3, m_CheckMonth);
	DDX_CBString(pDX, IDC_COMBO1, m_QuartData);
	DDX_CBString(pDX, IDC_COMBO2, m_MonthData);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Date1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_DateBeg);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_DateEnd);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioInterval);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPeriod, CDialog)
	//{{AFX_MSG_MAP(CInputPeriod)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_CBN_SELENDOK(IDC_COMBO1, OnSelendokCombo1)
	ON_CBN_SELENDOK(IDC_COMBO2, OnSelendokCombo2)
	ON_NOTIFY(DTN_USERSTRING, IDC_DATETIMEPICKER1, OnUserstringDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnDatetimechangeDatetimepicker2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, OnDatetimechangeDatetimepicker3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPeriod message handlers

BOOL CInputPeriod::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(csTitle.IsEmpty())
		csTitle="Введите период";
	SetWindowText(csTitle);
	UpdatePeriod();
	m_MonthCtrl.SetCurSel(m_Date1.GetMonth()-1);
	m_QuartCtrl.SetCurSel(int((m_Date1.GetMonth()-1)/3));
	m_MonthCtrl.EnableWindow(FALSE);
	m_QuartCtrl.EnableWindow(FALSE);
	m_Date1Ctrl.EnableWindow(FALSE);
	m_CheckYearCtrl.EnableWindow(FALSE);
	m_CheckQuartCtrl.EnableWindow(FALSE);
	m_CheckMonthCtrl.EnableWindow(FALSE);
	sTime.wYear		=m_Date1.GetYear();
	sTime.wMonth	=m_Date1.GetMonth();
	sTime.wDay		=m_Date1.GetDay();
	sTime.wHour		=0;
	sTime.wMinute	=0;
	sTime.wSecond	=0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CInputPeriod::OnCheck1() 
{
	if(m_RadioInterval==2)
		m_DateBeg=m_Date1;
	else if(m_RadioInterval==1)
	{
		sTime.wMonth=m_MonthCtrl.GetCurSel()+1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	else if(m_RadioInterval==0)
	{
		sTime.wMonth=m_QuartCtrl.GetCurSel()*3+1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	if(m_CheckYearCtrl.GetCheck())
	{	
		m_CheckQuartCtrl.SetCheck(0);
		m_CheckMonthCtrl.SetCheck(0);
		sTime.wYear		=m_DateBeg.GetYear();
		sTime.wMonth	=1;
		sTime.wDay		=1;
		m_DateBeg=CTime(sTime);
	}
	
	UpdatePeriod();
}

void CInputPeriod::OnCheck2() 
{
	if(m_RadioInterval==2)
		m_DateBeg=m_Date1;
	else if(m_RadioInterval==1)
	{
		sTime.wMonth=m_MonthCtrl.GetCurSel()+1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	if(m_CheckQuartCtrl.GetCheck())
	{	
		m_CheckYearCtrl.SetCheck(0);
		m_CheckMonthCtrl.SetCheck(0);
		sTime.wYear		=m_DateBeg.GetYear();
		sTime.wMonth	=1+int((m_DateBeg.GetMonth()-1)/3)*3;
		sTime.wDay		=1;
		m_DateBeg=CTime(sTime);
	}
	
	UpdatePeriod();
}

void CInputPeriod::OnCheck3() 
{
	m_DateBeg=m_Date1;
	if(m_CheckMonthCtrl.GetCheck())
	{	
		m_CheckYearCtrl.SetCheck(0);
		m_CheckQuartCtrl.SetCheck(0);
		sTime.wYear		=m_DateBeg.GetYear();
		sTime.wMonth	=m_DateBeg.GetMonth();
		sTime.wDay		=1;
		m_DateBeg=CTime(sTime);
	}
	
	UpdatePeriod();
}

void CInputPeriod::OnRadio1() 
{
	m_RadioInterval=0;
	m_MonthCtrl.EnableWindow(FALSE);
	m_QuartCtrl.EnableWindow(TRUE);
	m_Date1Ctrl.EnableWindow(FALSE);
	m_DateBegCtrl.EnableWindow(FALSE);
	m_DateEndCtrl.EnableWindow(FALSE);
	m_CheckYearCtrl.EnableWindow(TRUE);
	m_CheckQuartCtrl.EnableWindow(FALSE);
	m_CheckMonthCtrl.EnableWindow(FALSE);
	sTime.wMonth=m_QuartCtrl.GetCurSel()*3+1;
	sTime.wDay=1;
	m_DateBeg=CTime(sTime);
	sTime.wMonth+=2;
	CValue cvLastDateOfMonth=AddMonth(Date(sTime.wYear,sTime.wMonth,1))-1;
	int nYear, nMonth, nDay;
	cvLastDateOfMonth.FromDate(nYear,nMonth,nDay);
	sTime.wDay=nDay;
	m_DateEnd=CTime(sTime);
	UpdatePeriod();
}

void CInputPeriod::OnRadio2() 
{
	m_RadioInterval=1;
	m_MonthCtrl.EnableWindow(TRUE);
	m_QuartCtrl.EnableWindow(FALSE);
	m_Date1Ctrl.EnableWindow(FALSE);
	m_DateBegCtrl.EnableWindow(FALSE);
	m_DateEndCtrl.EnableWindow(FALSE);
	m_CheckYearCtrl.EnableWindow(TRUE);
	m_CheckQuartCtrl.EnableWindow(TRUE);
	m_CheckMonthCtrl.EnableWindow(FALSE);
	sTime.wMonth=m_MonthCtrl.GetCurSel()+1;
	sTime.wDay=1;
	m_DateBeg=CTime(sTime);
	CValue cvLastDateOfMonth=AddMonth(Date(sTime.wYear,sTime.wMonth,1))-1;
	int nYear, nMonth, nDay;
	cvLastDateOfMonth.FromDate(nYear,nMonth,nDay);
	sTime.wDay=nDay;
	m_DateEnd=CTime(sTime);
	UpdatePeriod();
}

void CInputPeriod::OnRadio3() 
{
	m_RadioInterval=2;
	m_MonthCtrl.EnableWindow(FALSE);
	m_QuartCtrl.EnableWindow(FALSE);
	m_Date1Ctrl.EnableWindow(TRUE);
	m_DateBegCtrl.EnableWindow(FALSE);
	m_DateEndCtrl.EnableWindow(FALSE);
	m_CheckYearCtrl.EnableWindow(TRUE);
	m_CheckQuartCtrl.EnableWindow(TRUE);
	m_CheckMonthCtrl.EnableWindow(TRUE);
	m_DateBeg=m_Date1;
	m_DateEnd=m_Date1;
	UpdatePeriod();
}

void CInputPeriod::OnRadio4() 
{
	m_RadioInterval=3;
	m_CheckYearCtrl.SetCheck(0);
	m_CheckQuartCtrl.SetCheck(0);
	m_CheckMonthCtrl.SetCheck(0);

	m_MonthCtrl.EnableWindow(FALSE);
	m_QuartCtrl.EnableWindow(FALSE);
	m_Date1Ctrl.EnableWindow(FALSE);
	m_DateBegCtrl.EnableWindow(TRUE);
	m_DateEndCtrl.EnableWindow(TRUE);
	m_CheckYearCtrl.EnableWindow(FALSE);
	m_CheckQuartCtrl.EnableWindow(FALSE);
	m_CheckMonthCtrl.EnableWindow(FALSE);
	
	CString csDate1;
	m_DateBegCtrl.GetWindowText(csDate1);
	CValue cvDate=Date(csDate1);
	int nYear,nMonth,nDay;
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear =nYear;
	sTime.wMonth=nMonth;
	sTime.wDay  =nDay;
	m_DateBeg=CTime(sTime);

	m_DateEndCtrl.GetWindowText(csDate1);
	cvDate=Date(csDate1);
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear =nYear;
	sTime.wMonth=nMonth;
	sTime.wDay  =nDay;
	m_DateEnd=CTime(sTime);
	UpdatePeriod();
}

void CInputPeriod::UpdatePeriod()
{
	CString csDateBeg=CString(Date(m_DateBeg.GetYear(),m_DateBeg.GetMonth(),m_DateBeg.GetDay()));
	CString csDateEnd=CString(Date(m_DateEnd.GetYear(),m_DateEnd.GetMonth(),m_DateEnd.GetDay()));
	m_PeriodEditCtrl.SetWindowText("Выбранный период: "+csDateBeg+"-"+csDateEnd);
}

void CInputPeriod::OnSelendokCombo1() 
{
	// TODO: Add your control notification handler code here
	sTime.wMonth=m_QuartCtrl.GetCurSel()*3+1;
	sTime.wDay=1;
	m_DateBeg=CTime(sTime);
	sTime.wMonth+=2;
	CValue cvLastDateOfMonth=AddMonth(Date(sTime.wYear,sTime.wMonth,1))-1;
	int nYear, nMonth, nDay;
	cvLastDateOfMonth.FromDate(nYear,nMonth,nDay);
	sTime.wDay=nDay;
	m_DateEnd=CTime(sTime);
	if(m_CheckYearCtrl.GetCheck())
	{
		sTime.wMonth=1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	UpdatePeriod();
}

void CInputPeriod::OnSelendokCombo2() 
{
	// TODO: Add your control notification handler code here
	sTime.wMonth=m_MonthCtrl.GetCurSel()+1;
	sTime.wDay=1;
	m_DateBeg=CTime(sTime);
	CValue cvLastDateOfMonth=AddMonth(Date(sTime.wYear,sTime.wMonth,1))-1;
	int nYear, nMonth, nDay;
	cvLastDateOfMonth.FromDate(nYear,nMonth,nDay);
	sTime.wDay=nDay;
	m_DateEnd=CTime(sTime);
	if(m_CheckYearCtrl.GetCheck())
	{
		sTime.wMonth=1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	if(m_CheckQuartCtrl.GetCheck())
	{
		sTime.wMonth=1+int((m_DateBeg.GetMonth()-1)/3)*3;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	UpdatePeriod();
}

void CInputPeriod::OnUserstringDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CInputPeriod::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString csDate1;
	m_Date1Ctrl.GetWindowText(csDate1);
	CValue cvDate=Date(csDate1);
	int nYear,nMonth,nDay;
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear =nYear;
	sTime.wMonth=nMonth;
	sTime.wDay  =nDay;
	m_Date1  =CTime(sTime);
	m_DateBeg=CTime(sTime);
	m_DateEnd=CTime(sTime);
	if(m_CheckYearCtrl.GetCheck())
	{
		sTime.wMonth=1;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	if(m_CheckQuartCtrl.GetCheck())
	{
		sTime.wMonth=1+int((m_DateBeg.GetMonth()-1)/3)*3;
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	if(m_CheckMonthCtrl.GetCheck())
	{
		sTime.wDay=1;
		m_DateBeg=CTime(sTime);
	}
	UpdatePeriod();
	*pResult = 0;
}

void CInputPeriod::OnDatetimechangeDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString csDate1;
	m_DateBegCtrl.GetWindowText(csDate1);
	CValue cvDate=Date(csDate1);
	int nYear,nMonth,nDay;
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear =nYear;
	sTime.wMonth=nMonth;
	sTime.wDay  =nDay;
	
	UpdatePeriod();
	*pResult = 0;
}

void CInputPeriod::OnDatetimechangeDatetimepicker3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CString csDate1;
	m_DateEndCtrl.GetWindowText(csDate1);
	
	CValue cvDate=Date(csDate1);
	int nYear,nMonth,nDay;
	cvDate.FromDate(nYear,nMonth,nDay);
	sTime.wYear =nYear;
	sTime.wMonth=nMonth;
	sTime.wDay  =nDay;
	
	UpdatePeriod();
	*pResult = 0;
}

void CInputPeriod::OnOK() 
{
	// TODO: Add extra validation here
	m_DateBeg1=m_DateBeg;
	m_DateEnd1=m_DateEnd;

	CDialog::OnOK();
}
