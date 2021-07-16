//Авторские права - VTOOLS.RU (info@vtools.ru)
// DialogUnit.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "DialogUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CDialogUnit dialog
#define CFormUnit CDialogUnit
#define CPARENT CDialog

#define VIRUALFUNCTION GridVirtualModeFunc1

#include "CommonUnit.cpp"



#undef CFormUnit
#undef CFormView
#undef VIRUALFUNCTION

void OutTime(int n);
void InitTime();


CDialogUnit::CDialogUnit(CWnd* pParent)
	: CDialog(CDialogUnit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogUnit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Init();
}




BOOL CDialogUnit::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_hIcon)
		m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	int nRes=1;


	nRes=Load();
	if(nRes)
	{
		nRes=CreateForm();
	}

	SetWindowText(csFormTitle);

	if(nRes)
		nRes=Run();

	CString csName=GetRuntimeClass()->m_lpszClassName;
	if("CMicroForm"==csName)//микроформу закрывать нельзя
	{ 
		nRes=1;
	}

	if(!nRes)
	{
		EndDialog(0);

		return FALSE;
	}

	//SetRedraw(1);
/*	for(int i=0;i<aControlList.GetSize();i++)
	{
		CDynControl *pControl=aControlList[i];
		if(pControl->GetWnd())
		{
			pControl->GetWnd()->SetRedraw(1);
			pControl->GetWnd()->Invalidate();
		}
	}
	Invalidate();
	*/

	//ShowWindow(1);

	if(pSetFocus)
		pSetFocus->SetFocus();


	return 0;  // return TRUE  unless you set the focus to a control
}


void CDialogUnit::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDialogUnit::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDialogUnit::OnClose() 
{
	if(bCanOpen)
	if(!IsCanDestroy())
		return;
	
	CDialog::OnClose();
}

