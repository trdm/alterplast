//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueToolBar.h"
#include "ValueMenu.h"
#include "../MainFrm.h"
#include "../EditForm/DynControl.h"
#include "OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



extern CImageList *GetImageList();
extern CMainFrame *pMainFrame;
extern CString csProfileName;
extern TMenuItemArray aAllMenuItems;

void AssignToMenuID(CMyMenuItem &item,int nId);
int GetUniqueMenuID();
extern CMyMenuItem GetElementFromValues(CValue **p);


CMethods CValueToolBar::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueToolBar,CValue);

CValueToolBar::CValueToolBar()
{
	nType=100;
	m_pToolBar=0;
}

CValueToolBar::~CValueToolBar()
{
//	if(m_pToolBar)
//		delete m_pToolBar;
}


//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CValueToolBar::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CValueToolBar::GetCount,{"CountItem","КоличествоЭлементов",""}},
	{CValueToolBar::AddButton,{"Add","Добавить","Добавить(НомКартинки,ИмяФункции,ПараметрФункции,Текст,Подсказка,СтрокаСост)"}},
	{CValueToolBar::RemoveButton,{"Remove","Удалить","Удалить(Индекс)"}},
	{CValueToolBar::ShowWindow,{"Visible","Видимость","Видимость(Флаг)"}},
	{CValueToolBar::GetMainTool,{"GetMainTool","ПолучитьГлавнуюПанель","ПолучитьГлавнуюПанель()"}},
	{CValueToolBar::GetEditTool,{"GetFormTool","ПолучитьПанельФормы","ПолучитьПанельФормы()"}},
	{CValueToolBar::GetModuleTool,{"GetModuleTool","ПолучитьПанельМодуля","ПолучитьПанельМодуля()"}},
	{CValueToolBar::Name,{"ToolName","ИмяПанели","ИмяПанели(Стр) - задание имени панели перед методом Добавить"}},
	{CValueToolBar::GetImage,{"GetImage","ПолучитьКартинку","ПолучитьКартинку() - возвращает номер картинки элемента"}},


};
 

#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CValueToolBar::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM);
}


CValue CValueToolBar::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CValueToolBar::GetString()
{
	return "Панель инструментов";
}

//////////////////////////////////////////////////////////////////////
void CValueToolBar::Create()
{
	if(pMainFrame)
	if(!m_pToolBar)
	{
		static int count=0;
		count++;
		if(m_csName.IsEmpty())
			m_csName.Format("Панель инструментов %d",count);
		m_pToolBar=new CExtToolControlBar();
		if( !m_pToolBar->Create(
				m_csName,
				pMainFrame,
				MENU_FINISH_ID+count
				)
			)
		{
			AfxMessageBox("Failed to create toolbar\n");
			return;      // fail to create
		}

		m_pToolBar->EnableDocking(CBRS_ALIGN_ANY);
		pMainFrame->DockControlBar(m_pToolBar);



		CWinApp * pApp = ::AfxGetApp();
		CExtControlBar::ProfileBarStateLoad(
			pMainFrame,
			pApp->m_pszRegistryKey,
			csProfileName,
			"Save",
			&pMainFrame->m_dataFrameWP
			);

	}
}

//////////////////////////////////////////////////////////////////////
void CValueToolBar::Attach(class CExtToolControlBar	*m_pTool)
{
	m_pToolBar=m_pTool;
}



//////////////////////////////////////////////////////////////////////
//Пользовательские методы 
//////////////////////////////////////////////////////////////////////

BOOL CValueToolBar::AddButton(CValue &rez,CValue**p)
{
	Create();
	int nImage=p[0]->GetNumber();

	CMyMenuItem element=GetElementFromValues(p);
	if(nImage<0)
	{
		m_pToolBar->InsertButton();
		return TRUE;
	}

	UINT nID=0;

	//ищем кнопки с той же картинкой
	if(nImage)
	for(int i=0;i<m_pToolBar->GetButtonsCount();i++)
	{
		CExtBarButton * pButton=m_pToolBar->GetButton( i );
		UINT nStyle;
		if(pButton)
		{
			m_pToolBar->GetButtonInfo(i,nID,nStyle);
			if(nID>0 && nID<MENU_FINISH_ID)
			if(pButton->m_nData==nImage)
			{
				if(!m_pToolBar->RemoveButton(i))
					Error("Ошибка удаления элемента");
				break;
			}
		}
		nID=0;
	}


	//привязка к событиям (также как в объекте Меню)
	if(nID==0)
		nID=GetUniqueMenuID();
	AssignToMenuID(element,nID);


	CExtCmdItem item;
	item.m_nCmdID=nID;
	item.m_sToolbarText=p[3]->GetString();
	item.m_sTipTool=p[4]->GetString();
	item.m_sTipStatus=p[5]->GetString();


	CImageList *pImageList=GetImageList();
	if(!pImageList)
		return 1;
	HICON hIcon = pImageList->ExtractIcon( nImage );
	if(!hIcon)
		Error("Не обнаружена картинка с заданным номером");
	
	g_CmdManager->CmdSetup(csProfileName,item,1);
	g_CmdManager->CmdSetIcon(csProfileName,item.m_nCmdID,hIcon);

	BOOL bRes=m_pToolBar->InsertButton(-1,item.m_nCmdID,1,nImage);
	return TRUE;
};

BOOL CValueToolBar::GetCount(CValue &rez,CValue**p)
{
	Create();
	rez=m_pToolBar->GetButtonsCount();
	return TRUE;
};
BOOL CValueToolBar::ShowWindow(CValue &rez,CValue**p)
{
	Create();
	m_pToolBar->ShowWindow(p[0]->GetNumber());
	return TRUE;
};
BOOL CValueToolBar::RemoveButton(CValue &rez,CValue**p)
{
	Create();
	int nIndex=p[0]->GetNumber()-1;
	if(nIndex<0 || nIndex>=m_pToolBar->GetButtonsCount())
		Error("Индекс выходит за границы значений");

	m_pToolBar->RemoveButton(nIndex);
	return TRUE;
};
BOOL CValueToolBar::GetMainTool(CValue &rez,CValue**p)
{

	rez.CreateObject(GetTypeString());
	((CValueToolBar*)rez.pRef)->Attach(&pMainFrame->m_ToolMain);
	((CValueToolBar*)rez.pRef)->m_csName="Общая панель инструментов";
	return TRUE;
};

BOOL CValueToolBar::GetModuleTool(CValue &rez,CValue**p)
{

	rez.CreateObject(GetTypeString());
	((CValueToolBar*)rez.pRef)->Attach(&pMainFrame->m_ToolModule);
	((CValueToolBar*)rez.pRef)->m_csName="Панель инструментов модуля";
	return TRUE;
};

BOOL CValueToolBar::GetEditTool(CValue &rez,CValue**p)
{

	rez.CreateObject(GetTypeString());
	((CValueToolBar*)rez.pRef)->Attach(&pMainFrame->m_ToolEdit);
	((CValueToolBar*)rez.pRef)->m_csName="Панель инструментов формы";
	return TRUE;
};

BOOL CValueToolBar::Name(CValue &rez,CValue**p)
{
	rez=String(m_csName);
	m_csName=p[0]->GetString();
	return TRUE;
};

BOOL CValueToolBar::GetImage(CValue &rez,CValue**p)
{
	Create();
	int nIndex=p[0]->GetNumber()-1;
	if(nIndex<0 || nIndex>=m_pToolBar->GetButtonsCount())
		Error("Индекс выходит за границы значений");

	CExtBarButton * pButton=m_pToolBar->GetButton( nIndex );
	if(pButton)
	{
		rez=pButton->m_nData;
	}
	return TRUE;
};