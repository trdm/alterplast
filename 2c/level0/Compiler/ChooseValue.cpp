//Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ChooseValue.cpp : implementation file
//

#include "stdafx.h"
//#include "Dialog.h"
#include "ChooseValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImageList *GetImageList();

/////////////////////////////////////////////////////////////////////////////
// CChooseValue dialog


CChooseValue::CChooseValue(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseValue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_List.SetImageList(GetImageList());
}


void CChooseValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseValue)
	DDX_Control(pDX, IDC_CHOOSELIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseValue, CDialog)
	//{{AFX_MSG_MAP(CChooseValue)
	ON_LBN_DBLCLK(IDC_CHOOSELIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseValue message handlers

void CChooseValue::OnOK() 
{

	nRes=m_List.GetCurSel();

	/*
	for(int i=0;i<List.GetSize();i++)
	{
		//m_List.InsertString(-1,List[i]);
		if(nMode<0)
			ListCheck[i]=FALSE;
	}

	if(nMode<0)
	{
		int nCount = m_List.GetSelCount();
		CArray<int,int> aryListBoxSel;
		aryListBoxSel.SetSize(nCount);
		m_List.GetSelItems(nCount, aryListBoxSel.GetData());
		for(int i=0;i<nCount;i++)
		{
			int n=aryListBoxSel[i];
			ListCheck[n]=TRUE;
		}

	}
	*/
	
	CDialog::OnOK();
}

BOOL CChooseValue::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(nMode>=0)
		m_List.SetImageList();
		//m_List.ModifyStyle(LBS_NOREDRAW,LBS_OWNERDRAWFIXED| LBS_HASSTRINGS);
		
	m_List.SetFont(GetFont());
		//m_listbox.Create(LBS_OWNERDRAWFIXED |  LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP | WS_VSCROLL | WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_HASSTRINGS , p, this,IDC_LIST);
//LBS_MULTIPLESEL
//LBS_EXTENDEDSEL
	
	for(int i=0;i<List.GetSize();i++)
	{
		int nImage=0;
		
		if(nMode<0)
		{
			m_List.InsertString(-1,List[i]);
			m_List.SetItemImage(i,ListCheck[i]+18);
		}
		else
			m_List.InsertString(-1,List[i]);
	}

	
	if(nStart>=0&&nStart<List.GetSize())
		m_List.SetCurSel(nStart);
	SetWindowText(csTitle);
	m_List.SetFocus();
	return FALSE; 
}


void CChooseValue::OnDblclkList1() 
{
	if(nMode<0)
	{
		ListCheck[m_List.GetCurSel()]=1-ListCheck[m_List.GetCurSel()];
		m_List.SetItemImage(m_List.GetCurSel(),ListCheck[m_List.GetCurSel()]+18);
	}
	else
		OnOK();
}
