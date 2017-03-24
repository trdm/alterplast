// HotKeyDlg.cpp : implementation file
#include "stdafx.h"
#include "configsvcimpl.h"
#include "HotKeyDlg.h"
#include "option.h"
#include "macrosdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CHotKeyDlg dialog
CHotKeyDlg::SHKeyInfo* CHotKeyDlg::m_hkeyInfo=NULL;
HACCEL CHotKeyDlg::m_hMacrosAccel=NULL;
CHotKeyDlg* CHotKeyDlg::m_ActiveDlg=NULL;
bool CHotKeyDlg::m_CancelHotkey;

static CString GetAccelString(DWORD code)
{
	CString line;
	if(code & FCONTROL)
		line+="Ctrl + ";
	if(code & FALT)
		line+="Alt + ";
	if(code & FSHIFT)
		line+="Shift + ";
	char name[20];
	GetKeyNameText((MapVirtualKey(code>>16,0) & 0xFF)<<16,name,20);
	line+=name;
	return line;
}

void CHotKeyDlg::RunHKEy(UINT cmd)
{
	cmd-=ID_HOTKEY_FIRST;
	RunMacros(m_hkeyInfo->moduls[cmd],m_hkeyInfo->macros[cmd],m_hkeyInfo->IsScript[cmd]!=NULL);
}


CHotKeyDlg::CHotKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHotKeyDlg::IDD, pParent)
{
	m_curInfo=NULL;
	//{{AFX_DATA_INIT(CHotKeyDlg)
	//}}AFX_DATA_INIT
}


void CHotKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotKeyDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_HOTKEY1, m_hkBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHotKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CHotKeyDlg)
	ON_WM_DESTROY()
	ON_COMMAND(ID_DEL_CCMD, OnDelHotKey)
	ON_COMMAND(ID_DOWN_CCMD, OnDownHotKey)
	ON_COMMAND(ID_MACROS_CCMD, OnMacrosHotKey)
	ON_COMMAND(ID_NEW_CCMD, OnNewHotKey)
	ON_COMMAND(ID_UP_CCMD, OnUpHotKey)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnAssign)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_HOTKEY1, OnChangeHotKey)
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipText)
END_MESSAGE_MAP()

void CHotKeyDlg::Init()
{
	CreateAccel(LoadHotKeys());
}

CHotKeyDlg::SHKeyInfo* CHotKeyDlg::LoadHotKeys()
{
	SHKeyInfo* pInfo=new SHKeyInfo;
	COption& opt=COption::GetOptions();
	for(int i=ID_HOTKEY_FIRST;i<=ID_HOTKEY_LAST;i++)
	{
		CString path;
		path.Format("HotKeys\\%i\\",i-ID_HOTKEY_FIRST);
		if(opt.Exist(path))
		{
			pInfo->IsScript.Add((void*)opt.GetInt(path+"IsScript"));
			pInfo->moduls.Add(opt.GetString(path+"modul"));
			pInfo->macros.Add(opt.GetString(path+"macros"));
			pInfo->codes.Add((void*)opt.GetInt(path+"code"));
		}
		else
			break;
	}
	return pInfo;
}

void CHotKeyDlg::CreateAccel(SHKeyInfo *pInfo)
{
	int size=pInfo->codes.GetSize();
	// Copy old accel table
	if(m_hMacrosAccel)
		DestroyAcceleratorTable(m_hMacrosAccel);
	ACCEL* pNewAccel=new ACCEL[size];
	for(int i=0;i<size;i++)
	{
		DWORD code=(DWORD)pInfo->codes[i];
		pNewAccel[i].cmd=ID_HOTKEY_FIRST+i;
		pNewAccel[i].fVirt=(code & 0xFFFF)|FVIRTKEY;
		pNewAccel[i].key=code>>16;
	}
	m_hMacrosAccel=CreateAcceleratorTable(pNewAccel,size);
	delete [] pNewAccel;
	m_hkeyInfo=pInfo;
}

BOOL CHotKeyDlg::OnInitDialog() 
{
	m_ActiveDlg=this;
	CDialog::OnInitDialog();
	m_tb.CreateEx(this,TBSTYLE_FLAT,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC);
	m_tb.LoadToolBar(IDR_CUSTOMCMD_DLG_TB);
	m_tb.GetToolBarCtrl().DeleteButton(2);
	RepositionBars(AFX_IDW_TOOLBAR,AFX_IDW_TOOLBAR,0);
	CRect rc;
	m_list.GetClientRect(rc);
	m_list.InsertColumn(0,"Клавиша",LVCFMT_LEFT,115,0);
	m_list.InsertColumn(1,"Макрос",LVCFMT_LEFT,rc.right-115,1);
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	FillList();
	if(m_list.GetItemCount())
		m_list.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	SetEnabledButtons();
	m_haccel=LoadAccelerators(g_hInst,(LPCTSTR)IDR_CCMD);
	m_hkBox.SendMessage(HKM_SETRULES,HKCOMB_S|HKCOMB_NONE,HOTKEYF_ALT);
	AfxSetResourceHandle(h1CResource);
	return TRUE;
}

BOOL CHotKeyDlg::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	pTTT->szText[0]=0;
    if(pNMHDR->idFrom>=ID_NEW_CCMD && pNMHDR->idFrom<=ID_DOWN_CCMD)
	{
		static LPCTSTR tips[]={
			"Добавить команду (Ins)",
			"Удалить команду (Del)",
			NULL,
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

BOOL CHotKeyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(!m_list.SendMessage(LVM_GETEDITCONTROL) && ::GetFocus()!=m_hkBox)
		if(TranslateAccelerator(m_hWnd,m_haccel,pMsg))
			return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CHotKeyDlg::ShowDlg()
{
	AfxSetResourceHandle(g_hInst);
	CHotKeyDlg dlg;
	dlg.DoModal();
}

void CHotKeyDlg::SetEnabledButtons()
{
	CToolBarCtrl& tc=m_tb.GetToolBarCtrl();
	int cnt=m_list.GetItemCount();
	int sel=GetListSel();
	m_hkBox.EnableWindow(sel>-1);
	m_enabled=0;
	if(cnt<ID_HOTKEY_LAST-ID_HOTKEY_FIRST+1)
		m_enabled|=1;
	if(sel>=0)
	{
		m_enabled|=6;//	8+4+2 123
		if(sel<cnt-1)
			m_enabled|=16;
		if(sel>0)
			m_enabled|=8;
	}
	for(sel=0;sel<5;sel++)
	{
		TBBUTTON tbb;
		tc.GetButton(sel,&tbb);
		tc.EnableButton(tbb.idCommand,m_enabled & (1<<sel));
	}
	OnChangeHotKey();
}

void CHotKeyDlg::OnDestroy() 
{
	DestroyAcceleratorTable(m_haccel);
	if(m_curInfo)
		delete m_curInfo;
	m_ActiveDlg=NULL;
	CDialog::OnDestroy();
}

int CHotKeyDlg::GetListSel()
{
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos)
		return m_list.GetNextSelectedItem(pos);
	return -1;
}

void CHotKeyDlg::OnDelHotKey() 
{
	int sel=GetListSel();
	if(sel<0)
		return;
	m_curInfo->Remove(sel);
	m_list.DeleteItem(sel);
	if(sel==m_list.GetItemCount())
		sel--;
	m_list.SetItemState(sel,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}


void CHotKeyDlg::SwapItems(int sel,int dif)
{
	m_curInfo->Swap(sel,sel+dif);
	m_list.DeleteItem(sel);
	sel+=dif;
	m_list.InsertItem(sel,GetAccelString((DWORD)m_curInfo->codes[sel]),0);
	CString txt;
	txt.Format("%s::%s",m_curInfo->moduls[sel],m_curInfo->macros[sel]);
	m_list.SetItem(sel,1,LVIF_TEXT,txt,0,0,0,0);
	m_list.SetItemState(sel,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}

void CHotKeyDlg::OnUpHotKey() 
{
	if(!(m_enabled & 8))
		return;
	SwapItems(GetListSel(),-1);
}

void CHotKeyDlg::OnDownHotKey() 
{
	if(!(m_enabled & 16))
		return;
	SwapItems(GetListSel(),1);
}

void CHotKeyDlg::OnMacrosHotKey() 
{
	int sel=GetListSel();
	if(sel<0)
		return;
	CMacrosDlg dlg;
	dlg.m_LastSelIsScript=m_curInfo->IsScript[sel]!=NULL;
	dlg.m_LastSelPath=m_curInfo->moduls[sel]+"::"+m_curInfo->macros[sel];
	if(dlg.DoModal()==IDCANCEL)
		return;
	bool IsScript;
	CString modul,macros;
	dlg.GetLastSel(IsScript,modul,macros);
	m_curInfo->IsScript[sel]=(void*)IsScript;
	m_curInfo->moduls[sel]=modul;
	m_curInfo->macros[sel]=macros;

	m_list.SetItem(sel,1,LVIF_TEXT,modul+"::"+macros,0,0,0,0);
	SetEnabledButtons();
}

void CHotKeyDlg::OnNewHotKey() 
{
	if(!(m_enabled & 1))
		return;
	CMacrosDlg dlg;
	if(dlg.DoModal()==IDCANCEL)
		return;
	bool IsScript;
	CString modul,macros;
	dlg.GetLastSel(IsScript,modul,macros);
	m_curInfo->codes.Add((void*)0);
	m_curInfo->IsScript.Add((void*)IsScript);
	m_curInfo->moduls.Add(modul);
	m_curInfo->macros.Add(macros);

	int cnt=m_list.GetItemCount();
	m_list.InsertItem(cnt,"",0);
	m_list.SetItem(cnt,1,LVIF_TEXT,modul+"::"+macros,0,0,0,0);
	m_list.SetItemState(cnt,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	SetEnabledButtons();
	m_hkBox.SetFocus();
}

void CHotKeyDlg::FillList()
{
	m_curInfo=new SHKeyInfo;
	int size=m_hkeyInfo->codes.GetSize();
	for(int i=0;i<size;i++)
	{
		DWORD code=(DWORD)m_hkeyInfo->codes[i];
		m_curInfo->codes.Add((void*)code);

		CString modul=m_hkeyInfo->moduls[i];
		m_curInfo->moduls.Add(modul);

		CString macros=m_hkeyInfo->macros[i];
		m_curInfo->macros.Add(macros);

		void* v=m_hkeyInfo->IsScript[i];
		m_curInfo->IsScript.Add(v);

		m_list.InsertItem(i,GetAccelString(code),0);
		m_list.SetItem(i,1,LVIF_TEXT,modul+"::"+macros,0,0,0,0);
	}
}

void CHotKeyDlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel=GetListSel();
	if(sel>-1)
	{
		DWORD code=(DWORD)m_curInfo->codes[sel];
		DWORD mod=0;
		if(code & FALT)
			mod|=HOTKEYF_ALT;
		if(code & FCONTROL)
			mod|=HOTKEYF_CONTROL;
		if(code & FSHIFT)
			mod|=HOTKEYF_SHIFT;
		m_hkBox.SetHotKey(code>>16,mod);
	}
	else
		m_hkBox.SetHotKey(0,0);
	SetEnabledButtons();
}

void CHotKeyDlg::OnChangeHotKey() 
{
	DWORD code=GetHotKeyCode();
	BOOL enable=TRUE;
	CString text="";
	if(code)
	{
		int s=m_curInfo->codes.GetSize();
		for(int i=0;i<s;i++)
		{
			if(code==(DWORD)m_curInfo->codes[i])
			{
				text.Format("Используется в %s::%s",m_curInfo->moduls[i],m_curInfo->macros[i]);
				enable=FALSE;
				break;
			}
		}
	}
	else
		enable=FALSE;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(enable);
	GetDlgItem(IDC_TEXT)->SetWindowText(text);
}

void CHotKeyDlg::OnAssign() 
{
	DWORD code=GetHotKeyCode();
	if(!code)
		return;
	int sel=GetListSel();
	if(sel<0)
		return;
	m_curInfo->codes[sel]=(void*)code;
	m_list.SetItem(sel,0,LVIF_TEXT,GetAccelString(code),0,0,0,0);
	SetEnabledButtons();
	m_list.SetFocus();
}

DWORD CHotKeyDlg::GetHotKeyCode()
{
	WORD vk,mod;
	DWORD code=0;
	m_hkBox.GetHotKey(vk,mod);
	if(vk)
	{
		code=vk<<16;
		if(mod & HOTKEYF_ALT)
			code|=FALT;
		if(mod & HOTKEYF_CONTROL)
			code|=FCONTROL;
		if(mod & HOTKEYF_SHIFT)
			code|=FSHIFT;
	}
	return code;
}

void CHotKeyDlg::OnOK() 
{
	int s=m_curInfo->codes.GetSize();
	for(int i=0;i<s;i++)
	{
		if(!m_curInfo->codes[i])
		{
			m_curInfo->Remove(i);
			i--,s--;
		}
	}
	COption& opt=COption::GetOptions();
	opt.Delete("HotKeys\\");
	for(i=0;i<s;i++)
	{
		CString path;
		path.Format("HotKeys\\%i\\",i);
		opt.SetInt(path+"IsScript",(int)m_curInfo->IsScript[i]);
		opt.SetString(path+"modul",m_curInfo->moduls[i]);
		opt.SetString(path+"macros",m_curInfo->macros[i]);
		opt.SetInt(path+"code",(int)m_curInfo->codes[i]);
	}
	delete m_hkeyInfo;
	CreateAccel(m_curInfo);
	m_curInfo=NULL;
	CDialog::OnOK();
}


void CHotKeyDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnMacrosHotKey();
	*pResult = 1;
}
