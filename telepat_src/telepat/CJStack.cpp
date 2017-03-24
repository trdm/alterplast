// CJStack.cpp : implementation file
//

#include "stdafx.h"
#include "CJStack.h"
#include "codejump.h"


/////////////////////////////////////////////////////////////////////////////
// CCJStack dialog
void CCJStack::ShowStack()
{
	AfxSetResourceHandle(hMyInst);
	CCJStack dlg;
	dlg.DoModal();
	AfxSetResourceHandle(h1CResource);
	if(dlg.m_selpos>-1)
	{
		CCodeJump* jump=CCodeJump::GetCodeJump();
		jump->m_CurPos=jump->m_TopPos-dlg.m_selpos;
		jump->Jump(jump->m_CurPos);
	}
}


CCJStack::CCJStack(CWnd* pParent /*=NULL*/)
	: CDialog(CCJStack::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCJStack)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCJStack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCJStack)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCJStack, CDialog)
	//{{AFX_MSG_MAP(CCJStack)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJStack message handlers

void CCJStack::OnSelchangeList1() 
{
	int sel=m_list.GetCurSel();
	CWnd* pText=GetDlgItem(IDC_TEXT);
	pText->SetWindowText("");
	if(sel>-1)
	{
		CCodeJump::SBackState* state=(CCodeJump::SBackState*)m_list.GetItemDataPtr(sel);
		pText->SetWindowText(state->word);
	}
}

BOOL CCJStack::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hBG=CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_list.SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),MAKELPARAM(TRUE,0));
	CCodeJump* pJump=CCodeJump::GetCodeJump();
	for(int i=pJump->m_TopPos;i>=0;i--)
	{
		CCodeJump::SBackState* state=(CCodeJump::SBackState*)pJump->m_stack[i];
		CString pos;
		pos.Format("%s  -  Строка %i",state->title,state->point.y+1);
		int item=m_list.AddString(pos);
		m_list.SetItemDataPtr(item,state);
	}
	m_list.SetCurSel(pJump->m_TopPos-pJump->m_CurPos+1);
	OnSelchangeList1();
	m_selpos=-1;
	return TRUE;
}

void CCJStack::OnOK()
{
	m_selpos=m_list.GetCurSel();
	EndDialog(IDOK);
}

HBRUSH CCJStack::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(pWnd==this)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd!=&m_list)
		pDC->SetBkMode(TRANSPARENT);
	return m_hBG;
}

void CCJStack::OnDblclkList1() 
{
	OnOK();	
}

void CCJStack::OnDestroy() 
{
	CDialog::OnDestroy();
	DeleteObject(m_hBG);
}
