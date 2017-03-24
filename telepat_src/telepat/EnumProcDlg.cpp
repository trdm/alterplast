// EnumProcDlg.cpp : implementation file
#include "stdafx.h"
#include "EnumProcDlg.h"
#include "txttrap.h"
#include "parser.h"
#include "telegroup.h"
#include "option.h"
#include "tentrydrawing.h"
#include "codejump.h"
static int listTop;

CEnumProcDlg::CEnumProcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnumProcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnumProcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CEnumProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnumProcDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnumProcDlg, CDialog)
	//{{AFX_MSG_MAP(CEnumProcDlg)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_SIZING()
	ON_COMMAND(ID_FILTER, OnFilter)
	ON_COMMAND(ID_SORT, OnSort)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumProcDlg message handlers

void CEnumProcDlg::ShowEnumProc(CTxtTrap *pEditor)
{
	CPoint ptCaretPos=pEditor->GetCaretPos();
	AfxSetResourceHandle(hMyInst);
	CEnumProcDlg dlg;
	dlg.m_pEditor=pEditor;
	dlg.m_CurLine=ptCaretPos.y;
	dlg.DoModal();
	AfxSetResourceHandle(h1CResource);
	if(dlg.m_CurLine>-1)
		CCodeJump::GetCodeJump()->JumpFromTo(pEditor,pEditor->GetDocument(),dlg.m_CurLine);
}

BOOL CEnumProcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_tb.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_TOOLTIPS);
	m_tb.LoadToolBar(IDR_TOOLBAR2);
	m_tb.SetBorders(2,2,2,2);
	RepositionBars(AFX_IDW_TOOLBAR,AFX_IDW_TOOLBAR,0);
	m_hAccel=LoadAccelerators(hMyInst,(LPCTSTR)IDR_ENUMPROC);

	m_list.GetClientRect(&m_list.m_ClientRect);

	int func,total;
	FillMethodList(m_array,m_pEditor,&m_list,m_sort,m_filtr,true,&func,&total);
	
	m_tb.GetToolBarCtrl().CheckButton(ID_SORT,m_sort==0);
	m_tb.GetToolBarCtrl().CheckButton(ID_FILTER,m_filtr);

	m_lock=false;
	m_CurLine=-1;
	CString text;
	text.Format("Процедур: %i Функций: %i Всего: %i",total-func,func,total);
	GetDlgItem(IDC_TOTAL)->SetWindowText(text);
	HICON hIcon=(HICON)CTEntryDrawing::m_iconList[IDI_GMMETEXP_];
	SetIcon(hIcon,FALSE);
	m_list.OnSelchange();
	return TRUE;
}

void CEnumProcDlg::MoveSel()
{
	int pos=m_list.GetCurSel();
	m_lock=true;
	CString txt;
	m_list.GetText(pos,txt);
	m_edit.SetWindowText(txt);
	m_edit.SetSel(0,-1);
	m_lock=false;
}

void CEnumProcDlg::OnOK()
{
	int pos=m_list.GetCurSel();
	if(pos>=0)
	{
		SProcInfo* pInfo=((CMethodEntry*)m_list.GetItemData(pos))->m_pInfo;
		m_CurLine=pInfo->start.line;
	}
	EndDialog(IDOK);
}

void CEnumProcDlg::OnChangeEdit()
{
	if(m_lock)
		return;
	CString txt;
	m_edit.GetWindowText(txt);
	if(m_filtr)
		FillList();
	else
	{
		int pos=m_list.FindString(-1,txt);
		if(pos>=0 && pos!=m_list.GetCurSel())
		{
			m_list.SetCurSel(pos);
			m_list.OnSelchange();
		}
	}
}

void CEnumProcDlg::OnDblclkList1() 
{
	OnOK();		
} 


BEGIN_MESSAGE_MAP(CFindEdit, CEdit)
	//{{AFX_MSG_MAP(CFindEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CFindEdit message handlers

void CFindEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_RETURN)
	{
		((CEnumProcDlg*)GetParent())->OnOK();
		return;
	}
	else if(nChar==VK_DOWN || nChar==VK_UP || nChar==VK_PRIOR || nChar==VK_NEXT)
	{
		CEnumProcDlg* dlg=(CEnumProcDlg*)GetParent();
		_AFX_THREAD_STATE* state=AfxGetThreadState();
		dlg->m_list.SendMessage(WM_KEYDOWN,state->m_lastSentMsg.wParam,
			state->m_lastSentMsg.lParam);
		if((nChar==VK_PRIOR || nChar==VK_NEXT) && (GetKeyState(VK_CONTROL) & (1<<16)))
		{
			int idx=nChar==VK_NEXT?dlg->m_list.GetCount()-1:0;
			dlg->m_list.SetCurSel(idx);
			dlg->m_list.SetTopIndex(idx);
		}
		dlg->MoveSel();
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEnumProcDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x=300;
	lpMMI->ptMinTrackSize.y=300;
	if(!m_list.m_hWnd)
		return;
	CRect rcList;
	m_list.GetWindowRect(rcList);
	ScreenToClient(rcList);
	rcList.bottom=rcList.top+12*CTEntryDrawing::m_EntryHeight+26;
	rcList.top=0;
	::AdjustWindowRect(&rcList,WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,FALSE);
	lpMMI->ptMinTrackSize.y=rcList.Height();
	
}

void CEnumProcDlg::OnDestroy() 
{
	CRect rc;
	GetWindowRect(rc);
	COption* opt=COption::m_pOption;
	CString path="EnumMethDlg\\";
	opt->SetInt(path+"top",rc.top);
	opt->SetInt(path+"left",rc.left);
	opt->SetInt(path+"bottom",rc.bottom);
	opt->SetInt(path+"right",rc.right);
	DestroyAcceleratorTable(m_hAccel);
	CDialog::OnDestroy();
}

void CEnumProcDlg::LayoutControls(CRect &newRect)
{
	CRect rcMsg,rcList,rcClient,rcEdit;
	GetClientRect(rcClient);

	m_edit.GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);
	rcEdit.right=rcClient.right;
	m_edit.MoveWindow(rcEdit);


	m_list.GetWindowRect(rcList);
	ScreenToClient(rcList);
	rcList.right=rcClient.right;
	rcList.bottom=rcClient.bottom-22;
	m_list.MoveWindow(rcList);
	m_list.GetWindowRect(rcList);
	ScreenToClient(rcList);
	int curSel=m_list.GetCurSel();
	if(curSel>-1)
	{
		CRect rcItem;
		m_list.GetItemRect(curSel,&rcItem);
		if(rcItem.top>rcList.Height())
			m_list.SetTopIndex(curSel);
		m_list.OnSelchange();
	}


	rcMsg.left=0;
	rcMsg.top=rcList.bottom+2;
	rcMsg.right=rcClient.right;
	rcMsg.bottom=rcMsg.top+20;
	GetDlgItem(IDC_TOTAL)->MoveWindow(rcMsg);
	rcClient.bottom=rcMsg.bottom;
	::AdjustWindowRect(&rcClient,WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,FALSE);
	newRect.bottom=newRect.top+rcClient.Height();
}

void CEnumProcDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	CRect rc;
	COption* opt=COption::m_pOption;
	CString path="EnumMethDlg\\";
	rc.top		= opt->GetInt(path+"top");
	rc.left		= opt->GetInt(path+"left");
	rc.bottom	= opt->GetInt(path+"bottom");
	rc.right	= opt->GetInt(path+"right");
	if(!rc.top && !rc.left && !rc.bottom && !rc.right)
		GetWindowRect(rc);
	MoveWindow(rc);
	LayoutControls(rc);
	MoveWindow(rc);
	CRect rcList;
	m_list.GetWindowRect(rcList);
	listTop=rcList.top-rc.top+30;
}

void CEnumProcDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(!IsWindowVisible())
		return;
	CRect rc;
	GetWindowRect(rc);
	int oldBottom=rc.bottom;
	LayoutControls(rc);
	if(rc.bottom!=oldBottom)
		MoveWindow(rc);
	m_list.OnSelchange();
}

void CEnumProcDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	m_list.OnSelchange();
}

void CEnumProcDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	if(fwSide==WMSZ_LEFT || fwSide==WMSZ_RIGHT)
		return;
	int h=pRect->bottom-pRect->top-listTop;
	h=h/CTEntryDrawing::m_EntryHeight*CTEntryDrawing::m_EntryHeight;
	h+=listTop;
	if(fwSide>=WMSZ_BOTTOM)
		pRect->bottom=pRect->top+h;
	else
		pRect->top=pRect->bottom-h;
}

void CEnumProcDlg::OnFilter() 
{
	COption::m_pOption->SetFilterMethods(m_filtr=1-m_filtr);
	FillList();
	m_tb.GetToolBarCtrl().CheckButton(ID_FILTER,m_filtr);
}

void CEnumProcDlg::OnSort() 
{
	COption::m_pOption->SetSortMethods(m_sort=1-m_sort);
	FillList();
	m_tb.GetToolBarCtrl().CheckButton(ID_SORT,m_sort==0);
}

void CEnumProcDlg::FillList()
{
	m_list.SendMessage(WM_SETREDRAW,0);
	m_list.ResetContent();
	int selection=0;
	int s=m_array.GetSize();

	CString txt;
	m_edit.GetWindowText(txt);
	if(m_filtr)
	{
		txt.MakeLower();
		CStringArray arr;
		SplitStrToArray(txt,arr,' ');
		int strCount=arr.GetSize();
		for(int i=0,count=0;i<s;i++)
		{
			CTeleEntry* pEntry=(CTeleEntry*)m_array[i];
			CString name=pEntry->m_Name;
			name.MakeLower();
			bool add=true;
			for(int k=0;k<strCount;k++)
			{
				if(name.Find(arr[k])<0)
				{
					add=false;
					break;
				}
			}
			if(add)
			{
				int idx;
				if(m_sort)
					idx=m_list.InsertString(count++,pEntry->m_Name);
				else
					idx=m_list.AddString(pEntry->m_Name);
				m_list.SetItemDataPtr(idx,pEntry);
			}
		}
	}
	else
	{
		for(int i=0,count=0;i<s;i++)
		{
			CTeleEntry* pEntry=(CTeleEntry*)m_array[i];
			int idx;
			if(m_sort)
				idx=m_list.InsertString(count++,pEntry->m_Name);
			else
				idx=m_list.AddString(pEntry->m_Name);
			m_list.SetItemDataPtr(idx,pEntry);
		}
		selection=m_list.FindString(-1,txt);
		if(selection<0)
			selection=0;
	}
	m_list.SetCurSel(selection);
	m_list.SetTopIndex(selection);
	m_list.SendMessage(WM_SETREDRAW,1);
	m_list.OnSelchange();
}

BOOL CEnumProcDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(TranslateAccelerator(m_hWnd,m_hAccel,pMsg))
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CEnumProcDlg::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	pTTT->szText[0]=0;
    if(pNMHDR->idFrom==ID_SORT || pNMHDR->idFrom==ID_FILTER)
	{
		static LPCTSTR tips[]={
			"Сортировать методы по алфавиту (Ctrl+S)",
			"Фильтровать методы по вхождению подстрок (Ctrl+F)",
		};
		strcpy(pTTT->szText,tips[pNMHDR->idFrom==ID_SORT?0:1]);
		*pResult=1;
		return TRUE;
	}
	*pResult=0;
	return FALSE;
}

void CEnumProcDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	m_list.m_pHint->Hide(nState!=WA_INACTIVE);
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CEnumProcDlg::FillMethodList(CPtrArray& arr,CTxtTrap *pEditor, CWnd *list, int &sort, int &filter,
			bool listbox, int* pFunc, int* total)
{
	CTxtTrap::m_ParsedDoc=pEditor->GetDocument();
	CTxtTrap::m_ParsedView=pEditor;
	
	CParser* pParser=pEditor->GetMyParser();
	pParser->m_flags=0;
	pParser->ParseModul();
	CProcCont* pCont=(CProcCont*)pParser->m_Conts[CParser::Methods];
	sort =COption::m_pOption->GetSortMethods();
	filter=COption::m_pOption->GetFilterMethods();
	CPoint ptCaretPos=pEditor->GetCaretPos();

	int func=pCont->FillListBox(list,sort,ptCaretPos.y,arr,listbox);
	if(pFunc)
		*pFunc=func;
	if(total)
		*total=pCont->GetCount();
	CTxtTrap::m_ParsedDoc=NULL;
	CTxtTrap::m_ParsedView=NULL;
}
