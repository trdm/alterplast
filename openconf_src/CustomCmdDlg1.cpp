// CustomCmdDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "CustomCmdDlg1.h"
#include "sectoolbarmgr.h"

CCustomCmdDlg::CCustomCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomCmdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomCmdDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCustomCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomCmdDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomCmdDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomCmdDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_COMMAND(ID_DEL_CCMD, OnDelCcmd)
	ON_COMMAND(ID_EDIT_CCMD, OnEditCcmd)
	ON_COMMAND(ID_MACROS_CCMD, OnMacrosCcmd)
	ON_COMMAND(ID_NEW_CCMD, OnNewCcmd)
	ON_COMMAND(ID_UP_CCMD, OnUpCcmd)
	ON_COMMAND(ID_DOWN_CCMD, OnDownCcmd)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, OnEndlabeleditList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomCmdDlg message handlers

BOOL CCustomCmdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_tb.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC);
	m_tb.LoadToolBar(IDR_CUSTOMCMD_DLG_TB);
	RepositionBars(AFX_IDW_TOOLBAR,AFX_IDW_TOOLBAR,0);
	CRect rc;
	m_list.GetClientRect(rc);
	m_list.InsertColumn(0,"Название",LVCFMT_LEFT,rc.right/2,0);
	m_list.InsertColumn(1,"Макрос",LVCFMT_LEFT,rc.right/2,1);
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int item=0;
	for(UINT id=ID_CUSTOMCMD_FIRST;id<=ID_CUSTOMCMD_LAST;id++)
	{
		SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(id);
		if(pCmd && !pCmd->IsEmpty())
		{
			m_list.InsertItem(item,pCmd->descr,0);
			m_list.SetItem(item++,1,LVIF_TEXT,pCmd->modul+"::"+pCmd->macros,0,0,0,0);
		}
	}
	if(m_list.GetItemCount())
		m_list.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
	SetEnabledButtons();
	m_haccel=LoadAccelerators(g_hInst,(LPCTSTR)IDR_CCMD);
	return TRUE;
}

void CCustomCmdDlg::ShowDlg()
{
	AfxSetResourceHandle(g_hInst);
	CCustomCmdDlg dlg;
	dlg.DoModal();
	AfxSetResourceHandle(h1CResource);
}

void CCustomCmdDlg::SetEnabledButtons()
{
	CToolBarCtrl& tc=m_tb.GetToolBarCtrl();
	int cnt=m_list.GetItemCount();
	int sel=GetListSel();
	m_enabled=0;
	if(cnt<ID_CUSTOMCMD_LAST-ID_CUSTOMCMD_FIRST+1)
		m_enabled|=1;
	if(sel>=0)
	{
		m_enabled|=14;
		if(sel<cnt-1)
			m_enabled|=32;
		if(sel>0)
			m_enabled|=16;
	}
	for(sel=0;sel<6;sel++)
		tc.EnableButton(ID_NEW_CCMD+sel,m_enabled & (1<<sel));
}

void CCustomCmdDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	SetEnabledButtons();
	*pResult = 1;
}

BOOL CCustomCmdDlg::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	pTTT->szText[0]=0;
    if(pNMHDR->idFrom>=ID_NEW_CCMD && pNMHDR->idFrom<=ID_DOWN_CCMD)
	{
		static LPCTSTR tips[]={
			"Добавить команду (Ins)",
			"Удалить команду (Del)",
			"Редактировать название (F2, Click)",
			"Задать макрос (F5, DblClick)",
			"Переместить вверх (Ctrl+Вверх)",
			"Переместить вниз (Ctrl+Вниз",
		};
		strcpy(pTTT->szText,tips[pNMHDR->idFrom-ID_NEW_CCMD]);
		*pResult=1;
		return TRUE;
	}
	*pResult=0;
	return FALSE;
}

void CCustomCmdDlg::OnDelCcmd() 
{
	if(!(m_enabled & 2))
		return;
	int sel=GetListSel(),pos=sel;
	m_list.DeleteItem(sel);
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel);
	SECToolBarMgr::SCustomCmd* pCmdNext=NULL;
	for(;sel<ID_CUSTOMCMD_LAST-ID_CUSTOMCMD_FIRST;sel++)
	{
		pCmdNext=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel+1);
		pCmd->Copy(pCmdNext);
		pCmd=pCmdNext;
	}
	if(!pCmd->IsEmpty())
		pCmd->Empty();
	//m_list.SetItemState(pos?pos-1:0,LVIS_SELECTED,LVIS_SELECTED);
}

void CCustomCmdDlg::OnEditCcmd() 
{
	if(m_enabled & 4)
		m_list.SendMessage(LVM_EDITLABEL,GetListSel());
}

void CCustomCmdDlg::OnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	if(pDispInfo->item.pszText)
	{
		int sel=GetListSel();
		SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel);
		if(pCmd)
		{
			pCmd->descr=pDispInfo->item.pszText;
			pCmd->Save();
		}
	}
	*pResult = 1;
}

void CCustomCmdDlg::OnMacrosCcmd() 
{
	if(!(m_enabled & 8))
		return;
	int sel=GetListSel();
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel);
	if(pCmd)
	{
		if(pCmd->Select())
			m_list.SetItem(sel,1,LVIF_TEXT,pCmd->modul+"::"+pCmd->macros,0,0,0,0);
	}
}

void CCustomCmdDlg::OnNewCcmd() 
{
	if(!(m_enabled & 1))
		return;
	int cnt=m_list.GetItemCount();
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+cnt);
	if(pCmd)
	{
		pCmd->descr.Empty();
		if(!pCmd->Select())
			return;
		m_list.InsertItem(cnt,pCmd->descr,0);
		m_list.SetItem(cnt,1,LVIF_TEXT,pCmd->modul+"::"+pCmd->macros,0,0,0,0);
		m_list.SendMessage(LVM_EDITLABEL,cnt);
	}
}

static void SwapItems(CListCtrl& list,int sel,int dif)
{
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel);
	SECToolBarMgr::SCustomCmd* pCmdUp=SECToolBarMgr::GetCustomCmd(ID_CUSTOMCMD_FIRST+sel+dif);
	pCmd->Swap(pCmdUp);
	list.DeleteItem(sel);
	sel+=dif;
	list.InsertItem(sel,pCmdUp->descr,0);
	list.SetItem(sel,1,LVIF_TEXT,pCmdUp->modul+"::"+pCmdUp->macros,0,0,0,0);
	list.SetItemState(sel,LVIS_SELECTED,LVIS_SELECTED);
}

void CCustomCmdDlg::OnUpCcmd() 
{
	if(!(m_enabled & 16))
		return;
	SwapItems(m_list,GetListSel(),-1);
}

void CCustomCmdDlg::OnDownCcmd() 
{
	if(!(m_enabled & 32))
		return;
	SwapItems(m_list,GetListSel(),1);
}

BOOL CCustomCmdDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(!m_list.SendMessage(LVM_GETEDITCONTROL))
		if(TranslateAccelerator(m_hWnd,m_haccel,pMsg))
			return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CCustomCmdDlg::OnDestroy() 
{
	DestroyAcceleratorTable(m_haccel);
	CDialog::OnDestroy();
}

int CCustomCmdDlg::GetListSel()
{
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos)
		return m_list.GetNextSelectedItem(pos);
	return -1;
}

void CCustomCmdDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnMacrosCcmd();
	*pResult = 1;
}
