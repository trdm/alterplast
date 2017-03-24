// OCMenu.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "OCMenu.h"
#include "resource.h"
#include "scriptmaneger.h"
#include "pluginmanager.h"
#include "helper.h"
#include "macrosdlg.h"
#include "Initializer.h"
#include "IConfig.h"
#include "FormScript.h"
#include "hackvirtual.h"
#include "icfgwindow.h"
#include "idlehandler.h"
#include "SECToolBarMgr.h"
#include "option.h"
#include "customcmddlg1.h"
#include "hotkeydlg.h"
#include "PanelManager.h"

CStringArray COCMenu::m_RecentEdit;		// Список последних редактируемых форм
CStringArray COCMenu::m_RecentRun;		// Список последних запускаемых форм

static CTrapVirtual trapGetMsgString;	// Перехват CFrameWnd::GetMessageString
static BOOL IsFormAvalable;				// Доступна ли fm20.dll
static CTrapSwap trapChildActivate;		// Перехват CChildFrame7::OnMDIActivate
static CTrapSwap trapFrameNeedTTT;		// Перехват CChildFrame7::OnToolTipNeedText
static AFX_PMSG pGetToolTipText;		// Реальный адрес MainFrame::OnToolTipText
static CMapPtrToPtr mapCmdTargets;		// Дополнительные CCmdTarget для обработки командных событий
static CTrapVirtual trapOnCmdMsg;		// Перехват OnCmdMsg
static CTrapVirtual trapPreTranslate;	// Перехват CFrameWnd::PreTranslateMessage
static HWND hMainWnd;

class CMyBkEndUI:public CBkEndUI
{
public:
	CMyBkEndUI()
	{
		oldVtable=*(DWORD*)pBkEndUI;
		*(DWORD*)pBkEndUI=*(DWORD*)this;
	}
	~CMyBkEndUI()
	{
		*(DWORD*)pBkEndUI=oldVtable;
		int cnt=strings.GetSize();
		for(int i=0;i<cnt;i++)
			DoMsgLine(strings[i],(MessageMarker)markers[i]);
		strings.RemoveAll();
		markers.RemoveAll();
	}
	virtual void DoMessageLine(char const *text,enum MessageMarker mark)
	{
		strings.Add(text);
		markers.Add((DWORD)mark);
	}
	static CStringArray strings;
	static CDWordArray  markers;
	DWORD oldVtable;
};
CStringArray CMyBkEndUI::strings;
CDWordArray  CMyBkEndUI::markers;

// Эта msgmap будет внедрена в MainFrame.
// те msgmap основного окна и COCMenu меняются местами
BEGIN_MESSAGE_MAP(COCMenu,CFrameWnd)
	//{{AFX_MSG_MAP(COCMenu)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_INITMENUPOPUP()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_COMMAND(ID_SETUP_PANELS, OnSetupPanels)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipTextMy)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipTextMy)
	ON_COMMAND_RANGE(ID_LOAD_SCRIPT,ID_SET_HOTKEYS,OnMenu)
	ON_COMMAND_RANGE(ID_CUSTOMCMD_FIRST,ID_CUSTOMCMD_LAST,OnCmdMacros)
	ON_COMMAND_RANGE(ID_HOTKEY_FIRST,ID_HOTKEY_LAST,OnHotKeyCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CUSTOMCMD_FIRST,ID_CUSTOMCMD_LAST,OnUpdateCustomCmd)
	ON_COMMAND_RANGE(IDR_PANEL_MENU_1,IDR_PANEL_MENU_16,OnShowHidePanels)
	//ON_COMMAND_RANGE(ID_OCFORM_LASTEDIT,ID_OCFORM_LASTEDIT+9,OnRecentEdit)
	//ON_COMMAND_RANGE(ID_OCFORM_LASTRUN,ID_OCFORM_LASTRUN+9,OnRecentRun)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_OCFORM_NEW,ID_OCFORM_RUN,OnUpdateForm)
END_MESSAGE_MAP()


COCMenu::COCMenu()
{
}

COCMenu::~COCMenu()
{
}

// Функция, возращающая базовую карту сообщений для основного окна
const AFX_MSGMAP* PASCAL COCMenu::_GetBaseWrap()
{
	return &COCMenu::messageMap;
}

static void ReCreateAccelTable(HACCEL& hAcc)
{
	int count=CopyAcceleratorTable(hAcc,NULL,0);
	ACCEL* pAcc=new ACCEL[count+1];
	CopyAcceleratorTable(hAcc,pAcc,count);
	DestroyAcceleratorTable(hAcc);
	pAcc[count].cmd=20002;
	pAcc[count].fVirt=FCONTROL|FVIRTKEY;
	pAcc[count].key='M';
	hAcc=CreateAcceleratorTable(pAcc,count+1);
	delete [] pAcc;
}

void BuildAccelerators()
{

}
static void MakeOwnerDraw(HMENU hMenu)
{
	CMenu menu;
	menu.Attach(hMenu);
	CMenu7::SetOwnerDraw(&menu, 1, 0);
	menu.Detach();
	return;
}

static WNDPROC oldMDIWndProc;

LRESULT MDIWndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg==WM_MDISETMENU)
	{
		try{
		HMENU hMenu=(HMENU)wParam;
		if(hMenu && IsMenu(hMenu))
		{
			char buf[40];
			MENUITEMINFO info;
			info.cbSize=sizeof(info);
			info.fMask=MIIM_TYPE;
			info.dwTypeData=buf;
			int cnt=GetMenuItemCount(hMenu);
			for(int i=0;i<cnt;i++)
			{
				info.cch=40;
				if(GetMenuItemInfo(hMenu,i,TRUE,&info))
				{
					if(!strcmp(buf,"OpenConf"))
						break;
					else if(!strcmp(buf,"&Конфигурация"))
					{
						HMENU hOpenConfMenu=LoadMenu(g_hInst,(LPCTSTR)IDR_OPENCONF);
						MakeOwnerDraw(hOpenConfMenu);
						InsertMenu(hMenu,i,MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT)hOpenConfMenu,"OpenConf");
						break;
					}
				}
			}
		}
		}catch(...)
		{
		}
	}
	return CallWindowProcA(oldMDIWndProc,hWnd,msg,wParam,lParam);
}

int COCMenu::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	ReCreateAccelTable(m_hAccelTable);

	CHotKeyDlg::Init();
	int ret;
	{
		CMyBkEndUI delayedbkendui;
		CPluginManager::Init();
		CPanelManager::Init();
		ret=(this->*(int (COCMenu::*)(LPCREATESTRUCT))FindRealHandler(WM_CREATE))(lpCreateStruct);
	}
	if(!ret)
	{
		CInitializer::InitOnCreateMainWnd();
		HWND mdi=::GetWindow(m_hWnd,GW_CHILD);
		oldMDIWndProc=(WNDPROC)::GetWindowLong(mdi,GWL_WNDPROC);
		::SetWindowLong(mdi,GWL_WNDPROC,(DWORD)MDIWndProc);
		trapPreTranslate.SetTrap(pMainApp,0x60/4,(AFX_PMSG)PreTranslateMessageWrap);
		hMainWnd=m_hWnd;
	}
	return ret;
}

CLSID MsFormClsid;

void COCMenu::Init(HWND hWnd)
{
/* 
	Перехват msgmap основного окна.
	Для этого возьмем карту основного окна,
	свою карту, и переставим в них lpEntries.
	Также pfnGetBaseMap из карты основного окна
	перепишем в свою карту, а в основном окне
	вместо нее пропишем адрес функции, возращающую
	нашу карту.
	struct AFX_MSGMAP
	{
		const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)();
		const AFX_MSGMAP_ENTRY* lpEntries;
	};
*/
	COCMenu* pFrame=(COCMenu*)CWnd::FromHandlePermanent(hWnd);
	AFX_MSGMAP* pFrameMap=(AFX_MSGMAP*)pFrame->GetMessageMap();
	AFX_MSGMAP* pMyMap=(AFX_MSGMAP*)&messageMap;

	DWORD old;
	VirtualProtect(pFrameMap,sizeof(*pFrameMap),PAGE_EXECUTE_READWRITE,&old);
	VirtualProtect(pMyMap,sizeof(*pMyMap),PAGE_EXECUTE_READWRITE,&old);
	AFX_MSGMAP_ENTRY* pTemp=(AFX_MSGMAP_ENTRY*)pFrameMap->lpEntries;
	pFrameMap->lpEntries=pMyMap->lpEntries;
	pMyMap->lpEntries=pTemp;
	pMyMap->pfnGetBaseMap=pFrameMap->pfnGetBaseMap;
	pFrameMap->pfnGetBaseMap=_GetBaseWrap;

	IsFormAvalable=(S_OK==CLSIDFromProgID(L"Forms.Form.1",&MsFormClsid));

	trapChildActivate.SetTrap("frame.dll","?OnMDIActivate@CChildFrame7@@IAEXHPAVCWnd@@0@Z",(AFX_PMSG)&OnChildActivateWrap);
	trapFrameNeedTTT.SetTrap("frame.dll","?OnToolTipNeedText@CChildFrame7@@IAEXIPAUtagNMHDR@@PAJ@Z",(AFX_PMSG)OnToolTipTextMy1);
	trapGetMsgString.SetTrap(pFrame,0xCC/4,(AFX_PMSG)GetMsgString);
	// Найдем реальный адрес MainFrame::OnToolTipText
	{
		AFX_MSGMAP* pMap=(AFX_MSGMAP*)&messageMap;
		AFX_MSGMAP_ENTRY* pEntry;
		while(pMap)
		{
			pEntry=(AFX_MSGMAP_ENTRY*)pMap->lpEntries;
			while(pEntry->nMessage)
			{
				if(pEntry->nMessage==WM_NOTIFY && (pEntry->nCode==(WORD)TTN_NEEDTEXTA || pEntry->nCode==(WORD)TTN_NEEDTEXTW))
				{
					pGetToolTipText=pEntry->pfn;
					goto fnd;
				}
				pEntry++;
			}
			pMap=(AFX_MSGMAP*)pMap->pfnGetBaseMap();
		}
		fnd:;
	}
	trapOnCmdMsg.SetTrap(pFrame,5,(AFX_PMSG)OnCmdMsgMy);
}

// Функция ищет реальный обработчик сообщения в настоящей msgmap основного окна
AFX_PMSG COCMenu::FindRealHandler(UINT nMessage)
{
	AFX_MSGMAP* pMap=(AFX_MSGMAP*)&messageMap;
	AFX_MSGMAP_ENTRY* pEntry;
	while(pMap)
	{
		pEntry=(AFX_MSGMAP_ENTRY*)pMap->lpEntries;
		while(pEntry->nMessage)
		{
			if(pEntry->nMessage==nMessage)
				return pEntry->pfn;
			pEntry++;
		}
		pMap=(AFX_MSGMAP*)pMap->pfnGetBaseMap();
	}
	return NULL;
}

void COCMenu::OnMenu(UINT cmd)
{
	switch(cmd){
	case ID_LOAD_SCRIPT:
		CScriptManager::GetScriptManager()->LoadScript("");
		break;
	case ID_LOAD_PLUGIN:
		CPluginManager::GetPM()->LoadPlugin("");
		break;
	case ID_INVOKE_MACROS:
		CMacrosDlg::InvokeMacros();
		break;
	case ID_OC_HELP:
		CHelper::ShowHelp();
		break;
	case ID_OC_ABOUT:		// О Программе
		CHelper::ShowAbout();
		break;
	case ID_OCFORM_NEW:
		{
			CFormScript* pForm=CFormScript::CreateForm("");
			break;
		}
	case ID_OCFORM_EDIT:
		{
			CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"ocf files|*.ocf||");
			dlg.DoModal();
			CFormScript::CreateForm(dlg.GetPathName());
			break;
		}
	case ID_CUSTOMIZE:
		CCustomCmdDlg::ShowDlg();
		break;
	case ID_SET_HOTKEYS:
		CHotKeyDlg::ShowDlg();
		break;
	};
}

void COCMenu::OnClose() 
{
	if(CIConfigEvents::m_ConfigEvents.OnQueryQuit())
		return;
	static AFX_PMSG pfn=FindRealHandler(WM_CLOSE);
	(this->*pfn)();
}

void COCMenu::OnDestroy() 
{
	CIConfigEvents::Done();
	AFX_PMSG pfn=FindRealHandler(WM_DESTROY);
	(this->*pfn)();
}


static void FillRecentMenu(CMenu* pMenu,CStringArray& list,UINT idx)
{
	int count=pMenu->GetMenuItemCount();
	for(int i=0;i<count;i++)
		pMenu->DeleteMenu(0,MF_BYPOSITION);
	for(i=0;i<list.GetSize();i++)
	{
		CString txt=list[i],item;
		txt.Replace(CConfigSvcImpl::m_pTheService->m_IBPath,"");
		txt.Replace(CConfigSvcImpl::m_pTheService->m_BinPath,"");
		item.Format("%i %s",i+1,txt);
		pMenu->AppendMenu(MF_STRING,idx++,item);
	}
}

void COCMenu::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	UINT id=pPopupMenu->GetMenuItemID(0);
	if(id==ID_OCFORM_LASTEDIT)
		FillRecentMenu(pPopupMenu,m_RecentEdit,ID_OCFORM_LASTEDIT);
	else if(id==ID_OCFORM_LASTRUN)
		FillRecentMenu(pPopupMenu,m_RecentRun,ID_OCFORM_LASTRUN);
	else if(id==ID_INVOKE_MACROS)
	{
		int cnt=pPopupMenu->GetMenuItemCount()-3;
		for(int i=0;i<cnt;i++)
			pPopupMenu->DeleteMenu(1,MF_BYPOSITION);
		int pos=1;
		for(UINT id=ID_CUSTOMCMD_FIRST;id<=ID_CUSTOMCMD_LAST;id++)
		{
			MENUITEMINFO info;
			info.cbSize=sizeof(info);
			info.fType=MFT_OWNERDRAW;
			info.fMask=MIIM_DATA|MIIM_ID|MIIM_TYPE;
			SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(id);
			if(pCmd && !pCmd->IsEmpty())
			{
				info.dwItemData=id;
				info.wID=id;
				::InsertMenuItem(pPopupMenu->m_hMenu,pos++,TRUE,&info);
				CMenu7::AddTempString(id,pCmd->descr,0);
			}
		}
	}
	static AFX_PMSG pFunc=FindRealHandler(WM_INITMENUPOPUP);
	(this->*(void (COCMenu::*)(CMenu*, UINT, BOOL))pFunc)(pPopupMenu,nIndex,bSysMenu);
	CPanelManager::ProcessMenu(pPopupMenu);
}

void COCMenu::OnRecentEdit(UINT cmd)
{
	DoMsgLine(m_RecentEdit[cmd-ID_OCFORM_LASTEDIT]);
}

void COCMenu::OnRecentRun(UINT cmd)
{
	DoMsgLine(m_RecentRun[cmd-ID_OCFORM_LASTRUN]);
}

static void AddRecent(CStringArray& arr,CString file)
{
	int size=arr.GetSize();
	for(int i=0;i<size;i++)
	{
		if(!arr[i].CompareNoCase(file))
		{
			arr.RemoveAt(i);
			break;
		}
	}
	arr.InsertAt(0,file);
	if(arr.GetSize()>10)
		arr.RemoveAt(10);
}

void COCMenu::AddRecentEdit(CString file)
{
	AddRecent(m_RecentEdit,file);
}

void COCMenu::AddRecentRun(CString file)
{
	AddRecent(m_RecentRun,file);
}

void COCMenu::OnUpdateForm(CCmdUI *pCm)
{
	//pCm->Enable(IsFormAvalable);
	pCm->Enable(FALSE);
}

void COCMenu::OnCmdMacros(UINT id)
{
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(id);
	if(pCmd)
	{
		if(!pCmd->IsEmpty())
			pCmd->Run();
	}
}

void COCMenu::OnUpdateCustomCmd(CCmdUI *pcm)
{
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(pcm->m_nID);
	if(pCmd)
		pcm->Enable(!pCmd->IsEmpty());
}

// Массив с окнами, для которых было отложено событие
// ActivateWnd, из-за того, что в тот момент они были невидимы
static HWND hDelayedActivate[20];
// Отложенное порождение события для этих окон.
// Вызывается из OnIdle
static void DelayedActivate()
{
	for(int i=0;i<20;i++)
	{
		if(hDelayedActivate[i])
		{
			CICfgWindow* pWnd=new CICfgWindow(hDelayedActivate[i]);
			CIConfigEvents::m_ConfigEvents.FireActivateMDIWnd(pWnd,true);
			pWnd->Release();
			hDelayedActivate[i]=NULL;
		}
	}
	CIdleHandler::RemoveIdleHandler(DelayedActivate);
}

// Функция-перехватчик CChildFrame7::OnMDIActivate
// Служит для порождения события OnActivateWindow
void COCMenu::OnChildActivateWrap(BOOL bActive,CWnd* pActivate,CWnd* pDeActivate)
{
	// Для начала вызовем реальную функцию
	trapChildActivate.Swap();
	(this->*(void(COCMenu::*)(int,CWnd*,CWnd*))trapChildActivate.m_pFuncTrapped.pClassFunc)(bActive,pActivate,pDeActivate);
	trapChildActivate.Swap();
	
	if(bActive && !::IsWindowVisible(m_hWnd))
	{
		// Если окно активизируется, но еще невидимо, (те только создается)
		// то событие для него лучше пока не порождать, а подождать.
		// Запишем окно в список отложенных
		for(int i=0;i<20;i++)
		{
			if(!hDelayedActivate[i])
			{
				hDelayedActivate[i]=m_hWnd;
				CIdleHandler::SetIdleHandler(DelayedActivate);
				return;
			}
		}
	}
	else
	{
		CICfgWindow* pWnd=new CICfgWindow(m_hWnd);
		CIConfigEvents::m_ConfigEvents.FireActivateMDIWnd(pWnd,bActive!=FALSE);
		pWnd->Release();
	}
}

// Функция-перехватчик активации основного окна
void COCMenu::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	static AFX_PMSG ptr=FindRealHandler(WM_ACTIVATE);
	(this->*(void (COCMenu::*)(UINT, CWnd*, BOOL))ptr)(nState,pWndOther,bMinimized);
	CIConfigEvents::m_ConfigEvents.FireActivateMainWnd(nState!=WA_INACTIVE);
}

// Функция-перехватчик GetMessageString
void COCMenu::GetMsgString(UINT nID, CString &rMessage)
{
	SECToolBarMgr::SCustomCmd* pCmd=SECToolBarMgr::GetCustomCmd(nID);
	if(pCmd)
	{
		rMessage=pCmd->descr;
		if(nID>=ID_CUSTOMCMD_FIRST && nID<=ID_CUSTOMCMD_LAST)
		{
			if(pCmd->modul.IsEmpty())
				rMessage="Макрос не назначен";
			else
			{
				CString descr=pCmd->modul+"::"+pCmd->macros;
				if(descr!=rMessage)
					rMessage=rMessage+" ("+descr+")";
			}
		}
	}
	else
		(this->*(void (COCMenu::*)(UINT,CString&))trapGetMsgString.m_pRealAdress)(nID,rMessage);
}

// Перехват текста тултипов для пользовательских команд
typedef BOOL(CCmdTarget::*POnTTNT)(UINT,NMHDR*,LRESULT*);
// Функция-перехватчик OnToolTipText основного окна
// Вызывается для тулбаров, когда не открыто ни одного mdi окна
BOOL COCMenu::OnToolTipTextMy(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	if(SECToolBarMgr::OnToolTipText(nID,pNMHDR,pResult))
		return TRUE;
	return (this->*(POnTTNT)pGetToolTipText)(nID,pNMHDR,pResult);
}

// Функция-перехватчик CChildFrame7::OnToolTipNeedText
BOOL COCMenu::OnToolTipTextMy1(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	if(SECToolBarMgr::OnToolTipText(nID,pNMHDR,pResult))
		return TRUE;
	trapFrameNeedTTT.Swap();
	BOOL ret=(this->*(POnTTNT)trapFrameNeedTTT.m_pFuncTrapped.pClassFunc)(nID,pNMHDR,pResult);
	trapFrameNeedTTT.Swap();
	return ret;
}

// Функция-перехватчик CMainFrame::OnCmdMsg
// Для добавления дополнительных роутингов командных сообщений
typedef BOOL (COCMenu::*POnCmdMsg)(UINT,int,void*,AFX_CMDHANDLERINFO*);
BOOL COCMenu::OnCmdMsgMy(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
	for(POSITION pos=mapCmdTargets.GetStartPosition();pos;)
	{
		CCmdTarget* pTarget;
		void* val;
		mapCmdTargets.GetNextAssoc(pos,(void*&)pTarget,val);
		try{
			if(pTarget->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
				return TRUE;
		}catch(...)
		{
		}
	}
	return (this->*(POnCmdMsg)trapOnCmdMsg.m_pRealAdress)(nID,nCode,pExtra,pHandlerInfo);	
}

void COCMenu::AddCmdTarget(CCmdTarget *pTarget)
{
	mapCmdTargets[pTarget]=(void*)1;
}

void COCMenu::RemoveCmdTarget(CCmdTarget *pTarget)
{
	mapCmdTargets.RemoveKey(pTarget);
}

void COCMenu::OnHotKeyCommand(UINT id)
{
	CHotKeyDlg::RunHKEy(id);
}

BOOL COCMenu::PreTranslateMessageWrap(MSG* pMsg) 
{
	if(CHotKeyDlg::m_ActiveDlg)
		return CHotKeyDlg::m_ActiveDlg->PreTranslateMessage(pMsg);
	if(CHotKeyDlg::m_hMacrosAccel)
	{
		CHotKeyDlg::m_CancelHotkey=false;
		if(TranslateAccelerator(hMainWnd,CHotKeyDlg::m_hMacrosAccel,pMsg) && CHotKeyDlg::m_CancelHotkey==false)
			return TRUE;
	}
	return (this->*(BOOL(COCMenu::*)(MSG*))trapPreTranslate.m_pRealAdress)(pMsg);
}

SECControlBar* FindMyPanel(LPCTSTR Caption);
void SetPanelVisible(SECControlBar* pBar,bool visible);

void COCMenu::OnShowHidePanels(UINT id)
{
	id-=IDR_PANEL_MENU_1;
	SetPanelVisible(FindMyPanel(CPanelManager::m_panels[id].name),!CPanelManager::m_panels[id].pView->IsWindowVisible());
}

void COCMenu::OnSetupPanels() 
{
	CPanelManager::Setup();
}