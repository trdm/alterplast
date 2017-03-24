// SECToolBarMgr.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "SECToolBarMgr.h"
#include "resource.h"
#include "option.h"
#include "scriptmaneger.h"
#include "pluginmanager.h"
#include "macrosdlg.h"

CTrapSwap		SECToolBarMgr::m_TrapCmdPage;
CPtrArray		SECToolBarMgr::m_CustomCmd;
CMapPtrToPtr	SECToolBarMgr::m_mapIDs;

void SECToolBarMgr::Init()
{
	m_TrapCmdPage.SetTrap("Frame.dll","?Initialize@SECToolBarCmdPage@@IAEXXZ",(AFX_PMSG)&SECToolBarMgr::OnInitCmdPage);
	LoadToolBar(g_hInst,128,"OpenConf",4);
	for(UINT i=ID_CUSTOMCMD_FIRST;i<=ID_CUSTOMCMD_LAST;i++)
	{
		SCustomCmd* pCmd=GetCustomCmd(i);
		pCmd->Load();
	}
}

void SECToolBarMgr::Done()
{
	int GroupCount=m_CustomCmd.GetSize();
	for(int i=0;i<GroupCount;i++)
	{
		SCustomCmdGroup* pGroup=(SCustomCmdGroup*)m_CustomCmd[i];
		if(pGroup->ids)
			delete [] pGroup->ids;
		if(pGroup->pCmds)
			delete [] pGroup->pCmds;
		delete pGroup;
	}
}

struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	//WORD aItems[wItemCount]

	WORD* items()
		{ return (WORD*)(this+1); }
};

void SECToolBarMgr::LoadToolBar(HINSTANCE hInst,UINT nID,LPCTSTR lpszName,int count)
{
	SECToolBarMgr* tlb=(SECToolBarMgr*)CTBManager::GetSECManager();
	if(!tlb)
		return;
	// Загружаем тулбар
	HRSRC hRsrc = ::FindResource(hInst, (LPCTSTR) nID, RT_TOOLBAR);
	if (hRsrc == NULL)
		return;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return;
	// Заводим новую группу
	SCustomCmdGroup* pCustomGroup=new SCustomCmdGroup;
	pCustomGroup->name=lpszName;
	pCustomGroup->count=pData->wItemCount;
	pCustomGroup->pCmds=new SCustomCmd[pData->wItemCount];
	m_CustomCmd.Add(pCustomGroup);

	UINT* pItems = new UINT[pData->wItemCount];
	int CmdCount=0;
	for(int i=0;i<pData->wItemCount;i++)
	{
		pItems[i] = pData->items()[i];
		if(pItems[i])
		{
			CmdCount++;
			m_mapIDs[(void*)pItems[i]]=&pCustomGroup->pCmds[i];
			pCustomGroup->pCmds[i].id=pItems[i];
			LoadString(hInst,pItems[i],pCustomGroup->pCmds[i].descr.GetBuffer(256),256);
			pCustomGroup->pCmds[i].descr.ReleaseBuffer();
		}
	}
	pCustomGroup->ids=pItems;
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	// Теперь надо слить битмапы и дописать идшники
	HDC hScreen=GetDC(GetDesktopWindow());
	HDC hMemDC=CreateCompatibleDC(hScreen);
	// Получим размер текущей битмапы
	BITMAP bmp;
	GetObject(tlb->m_bmp1,sizeof(bmp),&bmp);
	CSize oldSize(bmp.bmWidth,bmp.bmHeight);
	// Теперь загрузим битмапу
	HBITMAP hMyBitmap=LoadBitmap(hInst,(LPCTSTR)nID);
	// Создадим новую битмапу нужного размера
	HBITMAP hBmp=CreateCompatibleBitmap(hScreen,oldSize.cx+tlb->m_width*CmdCount,oldSize.cy);
	// Теперь будем в нее рисовать.
	SelectObject(hMemDC,hBmp);
	// А рисовать будем отсюда
	HDC hdcPaintBmp=CreateCompatibleDC(hScreen);
	SelectObject(hdcPaintBmp,tlb->m_bmp1);
	BitBlt(hMemDC,0,0,oldSize.cx,oldSize.cy,hdcPaintBmp,0,0,SRCCOPY);
	
	SelectObject(hdcPaintBmp,hMyBitmap);
	BitBlt(hMemDC,oldSize.cx,0,tlb->m_width*CmdCount,oldSize.cy,hdcPaintBmp,0,0,SRCCOPY);

	DeleteDC(hMemDC);
	DeleteObject(hMyBitmap);
	DeleteDC(hdcPaintBmp);
	ReleaseDC(GetDesktopWindow(),hScreen);
	// И заменяем битмапу на новую
	DeleteObject(tlb->m_bmp1.Detach());
	tlb->m_bmp1.Attach(hBmp);

	UINT* newIDs=new UINT[tlb->m_count1+CmdCount];
	memcpy(newIDs,tlb->m_cmdIDs,sizeof(UINT)*tlb->m_count1);
	for(i=0;i<pData->wItemCount;i++)
	{
		if(pItems[i])
			newIDs[tlb->m_count1++]=pItems[i];
	}
	delete [] tlb->m_cmdIDs;
	tlb->m_cmdIDs=newIDs;

	static UINT tlbID=AFX_IDW_TOOLBAR+19;
	tlb->DefineDefaultToolBar(tlbID++,lpszName,count,pItems,CBRS_ALIGN_ANY,
		AFX_IDW_DOCKBAR_TOP,0,TRUE,TRUE);
}

SECToolBarMgr::SCustomCmd* SECToolBarMgr::GetCustomCmd(UINT id)
{
	if(id>=ID_LOAD_SCRIPT && id<=ID_CUSTOMIZE)
		return NULL;
	SCustomCmd* pCmd=NULL;
	m_mapIDs.Lookup((void*)id,(void*&)pCmd);
	return pCmd;
}

void SECToolBarMgr::MakeToolBar(CString strName,CString cmds,IDispatch* pDisp,LPCTSTR bmpRes,LPCTSTR modName)
{
}

void SECToolBarMgr::OnInitCmdPage()
{
	SECToolBarCmdPage* pPage=(SECToolBarCmdPage*)this;
	m_TrapCmdPage.Swap();
	(pPage->*m_TrapCmdPage.m_pFuncTrapped.pClassFunc)();
	m_TrapCmdPage.Swap();
	int GroupCount=m_CustomCmd.GetSize();
	for(int i=0;i<GroupCount;i++)
	{
		SCustomCmdGroup* pGroup=(SCustomCmdGroup*)m_CustomCmd[i];
		pPage->DefineBtnGroup(pGroup->name,pGroup->count,pGroup->ids);
	}
}

BOOL SECToolBarMgr::OnToolTipText(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	SCustomCmd* pCmd=GetCustomCmd(pNMHDR->idFrom);
	if(pCmd)
	{
		CString descr=pCmd->descr;
		if(nID>=ID_CUSTOMCMD_FIRST && nID<=ID_CUSTOMCMD_LAST)
		{
			if(pCmd->IsEmpty())
				descr="Макрос не назначен";
		}
		if(pNMHDR->code == TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText, descr, sizeof(pTTTA->szText)/sizeof(pTTTA->szText[0]));
		else
			_mbstowcsz(pTTTW->szText, descr, sizeof(pTTTW->szText)/sizeof(pTTTW->szText[0]));
		*pResult = 1;
		// bring the tooltip window above other popup windows
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
			SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
		return TRUE;    // message was handled
	}
	return FALSE;
}

void SECToolBarMgr::SCustomCmd::Load()
{
	CString appPath;
	COption& opt=COption::GetOptions();
	appPath.Format("HotMacros\\%i\\",id-ID_CUSTOMCMD_FIRST);
	if(opt.Exist(appPath))
	{
		modul=opt.GetString(appPath+"modul");
		macros=opt.GetString(appPath+"macros");
		descr=opt.GetString(appPath+"description");
		IsScript=opt.GetInt(appPath+"IsScript")!=0;
		if(descr.IsEmpty())
			descr=modul+"::"+macros;
	}
}

void SECToolBarMgr::SCustomCmd::Save()
{
	CString appPath;
	COption& opt=COption::GetOptions();
	appPath.Format("HotMacros\\%i\\",id-ID_CUSTOMCMD_FIRST);
	if(IsEmpty())
		opt.Delete(appPath);
	else
	{
		opt.SetString(appPath+"modul",modul);
		opt.SetString(appPath+"macros",macros);
		opt.SetString(appPath+"description",descr);
		opt.SetInt(appPath+"IsScript",IsScript);
	}
}

bool SECToolBarMgr::SCustomCmd::IsEmpty()
{
	return modul.IsEmpty()!=0;
}

void SECToolBarMgr::SCustomCmd::Run()
{
	RunMacros(modul,macros,IsScript);
}

bool SECToolBarMgr::SCustomCmd::Select()
{
	CMacrosDlg dlg;
	CMacrosDlg::m_LastSelIsScript=IsScript;
	CMacrosDlg::m_LastSelPath=modul+"::"+macros;
	if(dlg.DoModal()==IDCANCEL)
		return false;
	if(descr.IsEmpty())
		descr=dlg.m_LastSelPath;
	dlg.GetLastSel(IsScript,modul,macros);
	Save();
	return true;
}
