// PanelManager.cpp: implementation of the CPanelManager class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "PanelManager.h"
#include "Option.h"
#include "OCDoc.h"
#include "PanelView.h"
#include "PanelSetupDlg.h"

SPanelInfo CPanelManager::m_panels[16];
CMapStringToPtr CPanelManager::m_rtcPanels;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanelManager::CPanelManager()
{

}

CPanelManager::~CPanelManager()
{

}

void LoadPanels(SPanelInfo* pInfo)
{
	CMapStringToPtr panels;
	CMapStringToPtr mapAllTabs;
	void* fnd;

	COption& opt=COption::GetOptions();
	
	CString path,name,lname,lnamet;
	if(opt.Exist("Panels\\"))
	{
		for(int i=0;i<16;i++)
		{
			path.Format("Panels\\%i\\",i);
			if(opt.Exist(path))
			{
				lname=name=opt.GetString(path);
				lname.MakeLower();
				if(!panels.Lookup(lname,fnd))
				{
					pInfo->name=name;
					int j=0;
					while(1)
					{
						path.Format("Panels\\%i\\%i\\",i,j);
						if(opt.Exist(path))
						{
							lnamet=name=opt.GetString(path);
							lnamet.MakeLower();
							if(!mapAllTabs.Lookup(lnamet,fnd))
							{
								if(CPanelManager::IsPluginPresent(name))
									pInfo->tabs.Add(name);
							}
						}
						else
							break;
						j++;
					}
					if(pInfo->tabs.GetSize())
					{
						panels[lname]=(void*)1;
						pInfo++;
					}
					else
						pInfo->name.Empty();
				}
			}
			else
				break;
		}
	}
}

void CPanelManager::Init()
{
	LoadPanels(m_panels);
    CRuntimeClass* pFrameClass=(CRuntimeClass*)GetProcAddress(
        GetModuleHandle("frame.dll"),
        "?classCPopupFrame@CPopupFrame@@2UCRuntimeClass@@B");
    AfxSetResourceHandle(g_hInst);
	
	for(int i=0;i<16;i++)
	{
		if(m_panels[i].name.IsEmpty())
			break;
		UINT id=IDR_PANEL_MENU_1+i;
        //–егистрируем шаблон
        pMainApp->AddDocTemplate(new CTemplate7(
            id,
            RUNTIME_CLASS(COCDoc),
            pFrameClass,
            RUNTIME_CLASS(CPanelView)));

        //ƒобавл€ем описание своей панели управлени€ через ToolBarManager
        //это избавл€ет нас от хлопот по выводу и сохранению/восстановлению состо€ни€ панели
		GetToolBarManager()->FUNC3(id, (char*)(LPCTSTR)m_panels[i].name, id, id,
			NULL, 0xE81D, 0, 1, 0x3F800000, 0x96);
	}
	AfxSetResourceHandle(h1CResource);
}

void CPanelManager::ProcessMenu(CMenu* pMenu)
{
	if(!m_panels[0].pView)
		return;
	int cnt=pMenu->GetMenuItemCount();
	if(cnt)
	{
		UINT idLast=pMenu->GetMenuItemID(cnt-1);
		bool IsMyMenu=false;
		if(idLast==0x8045)	// ѕанели инструментов
		{
			IsMyMenu=true;
			cnt-=1;
			pMenu->InsertMenu(cnt-1,MF_BYPOSITION|MF_SEPARATOR);
		}
		else if(idLast==0xB02B)	// ѕеремещать в главном окне
		{
			IsMyMenu=true;
			pMenu->DeleteMenu(ID_MENUITEM32797,MF_BYCOMMAND);
			cnt=0;
			if(pMenu->GetMenuItemID(0))
				pMenu->InsertMenu(0,MF_BYPOSITION|MF_SEPARATOR);
		}
		if(IsMyMenu)
		{
			for(int i=0;i<16;i++)
			{
				if(!m_panels[i].pView)
					break;
				pMenu->InsertMenu(cnt++,MF_BYPOSITION|MF_STRING|(m_panels[i].pView->IsWindowVisible()?MF_CHECKED:0),
					IDR_PANEL_MENU_1+i,m_panels[i].name);
			}
		}
	}
}

bool CPanelManager::IsPluginPresent(LPCTSTR szName)
{
	CPluginManager::Info* pInfo=CPluginManager::GetPM()->GetPluginByName(szName);
	if(pInfo)
	{
		IOleControlPtr ctrl=pInfo->pPlugin;
		if(ctrl!=NULL)
		{
			pInfo->OpenIn=2;
			return true;
		}
		return false;
	}
	void* fnd;
	return m_rtcPanels.Lookup(szName,fnd)!=FALSE;
}

void CPanelManager::Setup()
{
	CPanelSetupDlg dlg;
	AfxSetResourceHandle(g_hInst);
	dlg.DoModal();
}
