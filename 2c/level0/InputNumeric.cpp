// InputNumeric.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "InputNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputNumeric dialog


CInputNumeric::CInputNumeric(CWnd* pParent /*=NULL*/)
	: CDialog(CInputNumeric::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputNumeric)
	csValue = _T("");
	//}}AFX_DATA_INIT
}



BEGIN_MESSAGE_MAP(CInputNumeric, CDialog)
	//{{AFX_MSG_MAP(CInputNumeric)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDITNUMERIC, OnChangeEditnumeric)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputNumeric message handlers
void CInputNumeric::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputNumeric)
	DDX_Control(pDX, IDC_EDITNUMERIC, CtrlEdit);
	DDX_Text(pDX, IDC_EDITNUMERIC, csValue);
	//}}AFX_DATA_MAP
}

BOOL CInputNumeric::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(nTimer)
		SetTimer(10000,nTimer*1000,0);

	if(csTitle.IsEmpty())
		csTitle="¬ведите число";
	SetWindowText(csTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputNumeric::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==10000)
	{
		nTimer=-1;
		
		CDialog::OnCancel();
	}
	CDialog::OnTimer(nIDEvent);
}



void CInputNumeric::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	KillTimer(10000);
}

void CInputNumeric::OnChangeEditnumeric() 
{
	
	CString csData;
	CtrlEdit.GetWindowText(csData);
	if(!CheckText(csData))
	{
		DWORD dwSelection=CtrlEdit.GetSel();
		CtrlEdit.SetWindowText(csValue);
		CtrlEdit.SetSel(dwSelection-0x00010001);
	}
	else
		csValue=csData;
	
}

int CInputNumeric::CheckText(CString Str)
{
	Str.TrimLeft();
	Str.TrimRight();
	
	int nLen=Str.GetLength();
	if(nLen>nLength)
		return 0;
	int nPos=Str.Find(".");
	if(nPos>=0)
		if(nLen-nPos-1>nPrec)
			return 0;
	if(CString(String(Number(CValue(Str))))==Str)
		return 1;
	else
		if(Str.Right(1)=="0"||Str.Right(1)==".")
		{
			CString LeftPartStr=Str.Left(nLen-1);
			if (CheckText(LeftPartStr))
				return 1;
			else
				return 0;
		}
		if(Str.Left(1)=="0")
		{
			CString RightPartStr=Str.Right(nLen-1);
			if (CheckText(RightPartStr))
				return 1;
			else
				return 0;
		}
		if(Str.Left(2)=="-0")
		{
			CString RightPartStr=Str.Right(nLen-2);
			if (CheckText(RightPartStr))
				return 1;
			else
				return 0;
		}
		return 0;
	return 1;

}


