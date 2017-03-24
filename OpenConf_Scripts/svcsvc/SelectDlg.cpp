// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SelectDlg.h"
#include <afxcmn.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog

static CRect curRect(0,0,0,0);

CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	m_list=NULL;
	//{{AFX_DATA_INIT(CSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(100, OnDblclkList1)
	ON_NOTIFY(TVN_SELCHANGED, 100, OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, 100, OnDblclkTree1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers
static bool IsMark;
static void InsertInTree(CTreeCtrl* tree,HTREEITEM parent,LPCTSTR& ptr,int& level)
{
	int newLevel;
	while(*ptr)
	{
		LPCTSTR pStart=ptr;
		bool check=false,expand=false;
		while(*ptr && *ptr!='\r' && *ptr!='\n')
		{
			if(*ptr=='|')
			{
				if(ptr[1]=='|')
					ptr++;
				else
					break;
			}
			ptr++;
		}
		CString name(pStart,ptr-pStart);
		name.Replace("||","|");
		name.TrimRight();
		if(*ptr=='|')
		{
			ptr++;
			while(*ptr && *ptr!='\r' && *ptr!='\n' && *ptr!='|')
			{
				if(IsMark && (*ptr=='c' || *ptr=='C'))
					check=true;
				else if(*ptr=='e' || *ptr=='e')
					expand=true;
				ptr++;
			}
		}
		while(*ptr && (*ptr=='\r' || *ptr=='\n'))
			ptr++;
		HTREEITEM item=tree->InsertItem(name,parent);
		while(*ptr && (*ptr=='\r' || *ptr=='\n'))
			ptr++;
		if(*ptr)
		{
			newLevel=0;
			while(*ptr && *ptr=='\t')
				*ptr++,newLevel++;
		}
		else
			newLevel=level;
		if(newLevel>level)
			InsertInTree(tree,item,ptr,newLevel);
		if(check)
			tree->SetItemData(item,1);
		if(expand)
			tree->Expand(item,TVE_EXPAND);
		if(newLevel<level)
			break;
	}
	level=newLevel;
}

static HICON hIcon;
BOOL CALLBACK EnumThreadWndProc(
  HWND hwnd,      // handle to window
  LPARAM lParam   // application-defined value
)
{
	char buf[200];
	GetClassName(hwnd,buf,200);
	WNDCLASSEX wc;
	if(GetClassInfoEx(GetModuleHandle(NULL),buf,&wc))
		hIcon=wc.hIconSm;
	return hIcon==NULL;
}

static SetTreeCheck(CTreeCtrl* tree,HTREEITEM item)
{
 	if(item!=TVI_ROOT && tree->GetItemData(item))
		tree->SetCheck(item,TRUE);
	HTREEITEM child=tree->GetNextItem(item,TVGN_CHILD);
	while(child)
	{
		SetTreeCheck(tree,child);
		child=tree->GetNextItem(child,TVGN_NEXT);
	}
}

BOOL CSelectDlg::OnInitDialog() 
{
	if(hIcon)
		SendMessage(WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
	CDialog::OnInitDialog();
	GetWindowRect(m_rcMin);
	ScreenToClient(m_rcMin);

	SetWindowText(m_caption);
	CRect rc,rcbtn;
	GetClientRect(rc);
	GetDlgItem(IDOK)->GetWindowRect(rcbtn);
	ScreenToClient(&rcbtn);
	m_btnHeight=rc.bottom-rcbtn.top;
	m_okLeft=rc.right-rcbtn.left;

	GetDlgItem(IDCANCEL)->GetWindowRect(rcbtn);
	ScreenToClient(&rcbtn);
	m_cancelLeft=rc.right-rcbtn.left;

	rc.SetRectEmpty();

	if(0==m_mode || 1==m_mode)
	{
		CListBox* pList;
		DWORD style=WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|WS_TABSTOP|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|WS_VSCROLL|WS_HSCROLL;
		if(m_mode)
		{
			pList=new CCheckListBox;
			style|=LBS_OWNERDRAWFIXED;
		}
		else
			pList=new CListBox;
		pList->CreateEx(WS_EX_CLIENTEDGE,"LISTBOX",NULL,style,rc,this,100);
		m_list=pList;
		LPCTSTR pStart=m_vals,pRead=pStart;
		do{
			while(!(*pRead==0 || *pRead=='\r' || *pRead=='\n'))
				pRead++;
			if(pRead>pStart)
			{
				CString val=CString(pStart,pRead-pStart);
				val.TrimLeft();
				val.TrimRight();
				bool check=false;
				if(m_mode)
				{
					if(val.Right(2)=="|c")
					{
						check=true;
						val=val.Left(val.GetLength()-2);
					}
				}
				int pos=pList->AddString(val);
				if(check)
					((CCheckListBox*)pList)->SetCheck(pos,1);
			}
			while(*pRead=='\r' || *pRead=='\n')
				pRead++;
			if(!*pRead)
				break;
			pStart=pRead;
		}while(1);
		pList->SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),1);
		pList->SetCurSel(0);
	}
	else
	{
		CTreeCtrl* pTree=new CTreeCtrl;
		DWORD styles=TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_DISABLEDRAGDROP;
		if(m_mode>3)
		{
			IsMark=true;
			styles|=TVS_CHECKBOXES;
		}
		else
			IsMark=false;
		pTree->CreateEx(WS_EX_CLIENTEDGE,WC_TREEVIEW,NULL,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|styles,rc,this,100);
		LPCTSTR ptr=m_vals;
		int level=0;
		InsertInTree(pTree,TVI_ROOT,ptr,level);
		m_list=pTree;
	}
	m_list->SetFocus();
	if(!curRect.left && !curRect.right)
		GetWindowRect(curRect);
	MoveWindow(curRect);
	OnSize(0,0,0);
	return FALSE;
}

void CSelectDlg::DoSelect(BSTR caption, BSTR values, BSTR* ret,int mode)
{
	CString cap=(LPCTSTR)_bstr_t(caption),
		vals=(LPCTSTR)_bstr_t(values);
	*ret=NULL;
	if(vals.IsEmpty())
		return;
	if(cap.IsEmpty())
		cap="Выберите значение";
	hIcon=NULL;
	EnumThreadWindows(GetCurrentThreadId(),EnumThreadWndProc,0);

	CSelectDlg dlg;
	dlg.m_caption=cap;
	dlg.m_vals=vals;
	dlg.m_mode=mode;
	if(dlg.DoModal()==IDOK)
		*ret=dlg.m_ret.AllocSysString();
}

void CSelectDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	BOOL Enable=TRUE;
	if(m_mode==2)
	{
		HTREEITEM item=((CTreeCtrl*)m_list)->GetSelectedItem();
		if(((CTreeCtrl*)m_list)->GetNextItem(item,TVGN_CHILD))
			Enable=FALSE;
	}
	GetDlgItem(IDOK)->EnableWindow(Enable);
	*pResult = 0;
}


static void GetTreeSel(CTreeCtrl* tree,CString& ret,HTREEITEM item,CString parent)
{
	if(item!=TVI_ROOT && tree->GetCheck(item))
	{
		if(!ret.IsEmpty())
			ret+="\r\n";
		ret+=parent;
	}
	HTREEITEM child=tree->GetNextItem(item,TVGN_CHILD);
	while(child)
	{
		CString newParent=parent;
		if(!newParent.IsEmpty())
			newParent+="\\";
		newParent+=tree->GetItemText(child);
		GetTreeSel(tree,ret,child,newParent);
		child=tree->GetNextItem(child,TVGN_NEXT);
	}
}

void CSelectDlg::OnOK() 
{
	if(m_mode==0)
	{
		int sel=((CListBox*)m_list)->GetCurSel();
		((CListBox*)m_list)->GetText(sel,m_ret);
	}
	else if(m_mode==1)
	{
		CCheckListBox* pList=(CCheckListBox*)m_list;
		int count=pList->GetCount();
		for(int i=0;i<count;i++)
		{
			if(pList->GetCheck(i))
			{
				if(!m_ret.IsEmpty())
					m_ret+="\r\n";
				CString txt;
				pList->GetText(i,txt);
				m_ret+=txt;
			}
		}
	}
	else if(m_mode<4)
	{
		HTREEITEM item=((CTreeCtrl*)m_list)->GetSelectedItem();
		CString txt;
		while(item && item!=TVI_ROOT)
		{
			txt=((CTreeCtrl*)m_list)->GetItemText(item);
			if(!m_ret.IsEmpty())
				m_ret.Insert(0,"\\");
			m_ret.Insert(0,txt);
			item=((CTreeCtrl*)m_list)->GetNextItem(item,TVGN_PARENT);
		}
	}
	else
	{
		GetTreeSel((CTreeCtrl*)m_list,m_ret,TVI_ROOT,"");
	}
	CDialog::OnOK();
}

void CSelectDlg::OnDestroy() 
{
	GetWindowRect(curRect);
	if(m_list)
		delete m_list;
	CDialog::OnDestroy();
}

void CSelectDlg::OnSize(UINT nType, int cx, int cy) 
{
	if(m_list)
	{
		CRect rc;
		GetClientRect(rc);
		CWnd* pWnd=GetDlgItem(IDOK);
		pWnd->SetWindowPos(NULL,rc.right-m_okLeft,rc.bottom-m_btnHeight,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
		pWnd->Invalidate();
		pWnd->UpdateWindow();
		pWnd=GetDlgItem(IDCANCEL);
		pWnd->SetWindowPos(NULL,rc.right-m_cancelLeft,rc.bottom-m_btnHeight,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
		pWnd->Invalidate();
		pWnd->UpdateWindow();
		rc.bottom-=m_btnHeight+4;
		rc.left=3;
		rc.top=3;
		rc.right-=3;
		m_list->MoveWindow(rc);
	}
}

void CSelectDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x=250;
	lpMMI->ptMinTrackSize.y=300;
}

void CSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(IsMark && ((CTreeCtrl*)m_list)->GetImageList(TVSIL_STATE))
	{
		SetTreeCheck((CTreeCtrl*)m_list,TVI_ROOT);
		IsMark=false;
	}
	
}

void CSelectDlg::OnDblclkList1() 
{
	OnOK();
}

void CSelectDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	BOOL End=TRUE;
	if(m_mode==2)
	{
		HTREEITEM item=((CTreeCtrl*)m_list)->GetSelectedItem();
		if(((CTreeCtrl*)m_list)->GetNextItem(item,TVGN_CHILD))
			End=FALSE;
	}
	if(End)
		OnOK();
}
