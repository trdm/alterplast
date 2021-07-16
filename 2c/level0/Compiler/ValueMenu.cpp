// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueMenu.cpp: implementation of the CValueMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueMenu.h"
#include "../MainFrm.h"

#include "OutToDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CMainFrame *pMainFrame;
TMenuItemArray aAllMenuItems;
int nMenuId=MENU_START_ID;
extern CRunContext *AfxGetCurrentRunContext();


int GetUniqueMenuID()
{
	nMenuId++;
	if(nMenuId>MENU_FINISH_ID)
		Error("Количество элементов меню превысило максимально допустимое значение");
	return nMenuId;
}

void AssignToMenuID(CMyMenuItem &item,int nId)
{
	int n=nId-MENU_START_ID;
	aAllMenuItems.SetAtGrow(n,item);
}



CMyMenuItem GetElementFromValues(CValue **p)
{
	CMyMenuItem element;
	element.csName=p[0]->GetString();
	if(!element.csName.IsEmpty())
	{
		if(p[1]->GetTypeString()=="Меню"||p[1]->GetTypeString()=="Menu")//вложенное меню
		{
			element.nType=MENU_POPUP;
			element.vObject=*p[1];
		}
		else
		if(p[1]->GetType()==TYPE_STRING)//вызов функции модуля
		{
			element.nType=MENU_FUNCTION;
			element.pRun=0;
			if(AfxGetCurrentRunContext())
			{
				element.pRun=AfxGetCurrentRunContext()->pProcUnit;
			}
			element.sFunction=p[1]->GetString();
			element.vData=*p[2];
		}
		else
		if(p[1]->GetType()>=100)//вызов метода
		{
			element.nType=MENU_OBJECT;
			element.vObject=*p[1];
			element.sFunction=p[2]->GetString();
			element.vData=*p[3];
		}
	}
	return element;
}



CMethods CValueMenu::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueMenu,CValue);

CValueMenu::CValueMenu()
{
	nType=100;
}

CValueMenu::~CValueMenu()
{

}


//******************************
//Работа как агрегатного объекта
//******************************
void CValueMenu::PrepareNames(void)
{
	SEngRus aMethods[]=
	{
		{"ShowMenu","ПоказатьМеню","ПоказатьМеню(стрЗаголовок,номПоложение) - отображение ранее созданной структуры меню на экране"},
		{"AddMenu","ДобавитьМеню","ДобавитьМеню(стрЗаголовок,стрФункция)"},
		{"DeleteMenu","УдалитьМеню","УдалитьМеню() - удалить самый правый столбец главного меню."},
		{"LoadMenu","ЗагрузитьМеню","ЗагрузитьМеню() - загружает исходное главное меню (которое грузится по умолчанию)"},
	
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

CValue CValueMenu::Method(int iName,CValue **p)
{
	if(!pMainFrame)
		Error("Не было инициализации главного окна!");
	CValue Ret;
	switch(iName)
	{
		case enAddMenu:
			{
				CMyMenuItem element=GetElementFromValues(p);
				aMenu.Add(element);
				break;
			}
		case enShowMenu:
			{
					CString csMenuName=p[0]->GetString();
					CMenu *pMenuBar = pMainFrame->GetMenu();
					//CMenu *pMenuBar = pMainFrame->m_wndMenuBar.GetMenu();
					
					if(!pMenuBar)
						return 0;
					UINT Count=pMenuBar->GetMenuItemCount();
					
					UINT nIndex=-1;
					if(p[1]->nType)
					{
						nIndex=p[1]->GetNumber()-1;
                    }

					CMenu *pMenu = new CMenu;
					VERIFY(pMenu->CreatePopupMenu());
					VERIFY(pMenuBar->InsertMenu(nIndex, MF_BYPOSITION | MF_POPUP, 
						(UINT)pMenu->m_hMenu, csMenuName));
					
					for(int i=0;i<aMenu.GetSize();i++)
					{
						int nID=GetUniqueMenuID();
						AssignToMenuID(aMenu[i],nID);

						if(aMenu[i].nType==MENU_SEPARATOR)
							pMenu->AppendMenu(MF_SEPARATOR);
						else
						if(aMenu[i].nType==MENU_FUNCTION||aMenu[i].nType==MENU_OBJECT)
							pMenu->AppendMenu(MF_STRING | MF_ENABLED, nID, aMenu[i].csName);


					}
					//g_CmdManager->UpdateFromMenu("csProfileName",pMenu->GetSafeHmenu());
					//g_CmdManager->ProfileWndAdd( "sProfileName", pMenu->m_hMenu);

					pMainFrame->DrawMenuBar();
					//pMainFrame->m_wndMenuBar.UpdateMenuBar();
				
				break;
			}
			case enDeleteMenu:
			{
					CMenu *pMenuBar = pMainFrame->GetMenu();
				    //CMenu *pMenuBar = pMainFrame->m_wndMenuBar.GetMenu();
					
					if(!pMenuBar)
						return 0;
					
					UINT Count=pMenuBar->GetMenuItemCount();
					pMenuBar->DeleteMenu(Count-1, MF_BYPOSITION );	
										
					pMainFrame->DrawMenuBar();
					//pMainFrame->m_wndMenuBar.DrawMenuBar();
				
				
				break;
			}
			case enLoadMenu:
			{
					CMenu *pMenuBar = pMainFrame->GetMenu();
				    //CMenu *pMenuBar = pMainFrame->m_wndMenuBar.GetMenu();
					if(!pMenuBar)
						return 0;				
					pMenuBar->DestroyMenu();
					pMenuBar->LoadMenu(IDR_MAINFRAME);	
					pMainFrame->SetMenu(pMenuBar);					
					pMainFrame->DrawMenuBar();
					//pMainFrame->m_wndMenuBar.DrawMenuBar();
				
				
				break;
			}

	}
	return Ret;
}
