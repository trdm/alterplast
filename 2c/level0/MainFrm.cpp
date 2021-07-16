//Авторские права - VTOOLS.RU (info@vtools.ru)
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Enterprise.h"
#include "Resource.h"

#include "MainFrm.h"
#include "Compiler/ValueMenu.h"
#include "EditForm/FormEditor.h"
#include "MetadataTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OpenForm.h"

 
CMessageBar *pMessageWindow=0;//для вызова из языка
extern CStatusBar *pStatusBar;
extern CMainFrame *pMainFrame;
CDebugMessageBar *pDebugMessageBar=0;
CStackBar *pDebugStackBar=0;
extern BOOL bStopRunProgram;

CString csProfileName;
CImageList *GetImageList();

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
CStatusBar *pStatusBar=NULL;//для вызова из языка
CMainFrame *pMainFrame;
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_VIEW_LIKE_OFFICE_2K, OnViewLikeOffice2k)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_OFFICE_2K, OnUpdateViewLikeOffice2k)
	ON_COMMAND(ID_VIEW_LIKE_OFFICE_XP, OnViewLikeOfficeXp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_OFFICE_XP, OnUpdateViewLikeOfficeXp)
	ON_COMMAND(ID_PAUSE_DEBUG, OnPauseDebug)
	ON_UPDATE_COMMAND_UI(ID_PAUSE_DEBUG, OnUpdatePauseDebug)
	ON_UPDATE_COMMAND_UI(ID_STOP_PROGRAM, OnUpdateStopDebug)
	ON_COMMAND(ID_STOP_PROGRAM, OnStopProgram)
	ON_UPDATE_COMMAND_UI(ID_RUN_DEBUG, OnUpdateRunDebug)
	ON_COMMAND(ID_RUN_DEBUG, OnRunDebug)
	ON_COMMAND(ID_NEXT_DEBUG_OVER, OnNextDebugOver)
	ON_COMMAND(ID_NEXT_DEBUG, OnNextDebug)
	ON_COMMAND(ID_POST_SHOW, OnPostShow)
	ON_UPDATE_COMMAND_UI(ID_NEXT_DEBUG, OnUpdateRunDebug)
	ON_UPDATE_COMMAND_UI(ID_NEXT_DEBUG_OVER, OnUpdateRunDebug)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP



	ON_UPDATE_COMMAND_UI(IDS_DOC_SELECTOR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDS_DOC_SELECTOR, OnBarCheck)

	ON_UPDATE_COMMAND_UI(IDC_MAIN_MENU, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDC_MAIN_MENU, OnBarCheck)
	
	ON_UPDATE_COMMAND_UI(ID_VIEWMESSAGEWINDOW, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEWMESSAGEWINDOW, OnBarCheck)

	ON_UPDATE_COMMAND_UI(ID_VIEWDEBUGMESSAGEWINDOW, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEWDEBUGMESSAGEWINDOW, OnBarCheck)

	ON_UPDATE_COMMAND_UI(ID_VIEW_STACK, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STACK, OnBarCheck)
	

	ON_UPDATE_COMMAND_UI(ID_PROPERTYBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_PROPERTYBAR, OnBarCheck)
	

	ON_COMMAND(ID_ENTERPRISE, CEnterpriseApp::OnButtonEnterprise)
	ON_UPDATE_COMMAND_UI(ID_ENTERPRISE, CEnterpriseApp::OnUpdateButtonEnterprise)
	ON_COMMAND(ID_CONFIG, CEnterpriseApp::OnConfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG, CEnterpriseApp::OnUpdateConfig)

	ON_COMMAND_RANGE(MENU_START_ID, MENU_FINISH_ID, OnSelectMenu)


	ON_COMMAND_EX(IDR_MAIN_TOOLBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_MAIN_TOOLBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_MODULE, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_MODULE, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_EDITBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_EDITBAR, OnUpdateControlBarMenu)


	//пользовательские панели инструментов, создаваемые из языка
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+1, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+2, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+3, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+4, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+5, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+6, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+7, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+8, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+9, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(MENU_FINISH_ID+10,OnUpdateControlBarMenu)
	ON_COMMAND_EX(MENU_FINISH_ID+1, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+2, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+3, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+4, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+5, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+6, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+7, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+8, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+9, OnBarCheck)
	ON_COMMAND_EX(MENU_FINISH_ID+10,OnBarCheck)

END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,               // status line indicator 
	ID_EDIT_INDICATOR_POSITION,	 
	ID_EDIT_INDICATOR_COL,
	ID_EDIT_INDICATOR_CRLF,
	ID_INDICATOR_OVR,	
	ID_EDIT_INDICATOR_READ,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	pMainFrame=this;
	nCurAppRunMode=0;


	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;

}

CMainFrame::~CMainFrame()
{
}
/*
static UINT g_statBasicCommands[] =
{
	ID_VIEWMESSAGEWINDOW,
	ID_PROPERTYBAR,
	IDC_MAIN_MENU,
	IDR_MAIN_TOOLBAR,
	IDR_MODULE,
	IDR_EDITBAR,
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_FILE_NEW,
	ID_VIEW_TOOLBAR,
	ID_VIEW_STATUS_BAR,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
	ID_VIEW_LIKE_OFFICE_2K,
	ID_VIEW_LIKE_OFFICE_XP,
	MENU_START_ID,
	MENU_START_ID+1,
	MENU_START_ID+2,
	MENU_START_ID+3,
	MENU_START_ID+4,
	MENU_START_ID+5,
	MENU_START_ID+6,
	MENU_START_ID+7,
	MENU_START_ID+8,
	MENU_START_ID+9,
	0, // end of list
};
*/

void CheckWindow(CWnd &m_Wnd)
{
	if(!AfxGetMainWnd())
		return;
	CRect Rect0;
	AfxGetMainWnd()->GetWindowRect(Rect0);

	CRect Rect;
	m_Wnd.GetWindowRect(Rect);
	if(Rect.left<Rect0.left)
		Rect.left=0;
	if(Rect.top<Rect0.top)
		Rect.top=0;
	if(Rect.left>Rect0.right)
		Rect.left=0;
	if(Rect.top>Rect0.bottom)
		Rect.top=0;
	m_Wnd.MoveWindow(Rect);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	nCurAppRunMode=afxAppRunMode;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	



	CExtCmdManager::g_bDisableCmdIfNoHandler=0;


	UINT nMainToolBarID=IDR_MAINFRAME;
	if(nCurAppRunMode==CONFIG_MODE)
	{
		SetTitle("2С:Конфигуратор");
		nMainToolBarID=IDR_CONFIGTOOLBAR;
	}

	csProfileName=GetTitle()+"N05";
#ifdef _DEBUG
	csProfileName+="D";
#endif



	CWinApp * pApp = ::AfxGetApp();


	g_CmdManager->ProfileSetup(
		csProfileName,
		GetSafeHwnd()
		);
	

	//Меню
/*	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if( !m_wndMenuBar.Create("Главное меню",
							 this,
							 IDC_MAIN_MENU))
	{
		TRACE0( _T( "Failed to create menubar\n" ) );
		return -1;
	}
*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	pStatusBar=&m_wndStatusBar;


	//Главная панель
	if( !m_ToolMain.Create(
			_T("Общая панель инструментов"),
			this,
			IDR_MAIN_TOOLBAR
			)
		||
		!m_ToolMain.LoadToolBar(nMainToolBarID)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//Панель инструментов модуля
	if( !m_ToolModule.Create(
			_T("Панель инструментов модуля"),
			this,
			IDR_MODULE
			)
		||
		!m_ToolModule.LoadToolBar(IDR_MODULE)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//Панель инструментов формы
	if(nCurAppRunMode==CONFIG_MODE)
	{
		if( !m_ToolEdit.Create(
				_T("Панель инструментов формы"),
				this,
				IDR_EDITBAR
				)
			||
			!m_ToolEdit.LoadToolBar(IDR_EDITBAR)
			)
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
	}

	//Создаем контейнер окна сообщений
	if(	!m_ParentMessageBar.Create(
			_T("Окно сообщений"),
			this,
			ID_VIEWMESSAGEWINDOW
			)
		)
	{
		TRACE0("Failed to create m_ParentMessageBar\n");
		return -1;		// fail to create
	}
	//Создаем само окно сообщений
	if( !m_MessageBar.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL
				|ES_MULTILINE|ES_LEFT|ES_WANTRETURN,

			CRect(0,0,0,0),
			&m_ParentMessageBar,
			m_ParentMessageBar.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_MessageBar\n");
		return -1;		// fail to create
	}
	m_MessageBar.Init();





	if(nCurAppRunMode==CONFIG_MODE)
	{
		//Создаем контейнер отладочного окна
		if(	!m_ParentDebugMessageBar.Create(
				_T("Окно отладки"),
				this,
				ID_VIEWDEBUGMESSAGEWINDOW
				)
			) 
		{
			TRACE0("Failed to create m_ParentDebugMessageBar\n");
			return -1;		// fail to create
		}
		if( !m_DebugMessageBar.Create(
				WS_VISIBLE|WS_CHILD|LVS_SHAREIMAGELISTS,
				CRect(0,50,50,200),
				&m_ParentDebugMessageBar,
				m_ParentDebugMessageBar.GetDlgCtrlID()
				)
			)
		{
			TRACE0("Failed to create m_DebugMessageBar\n");
			return -1;		// fail to create
		}
		m_DebugMessageBar.Init();
		pDebugMessageBar=&m_DebugMessageBar;

		
		//Создаем контейнер окна стека вызова
		if(	!m_ParentStackBar.Create(
				_T("Стек вызова"),
				this,
				ID_VIEW_STACK
				)
			)
		{
			TRACE0("Failed to create m_ParentStackBar\n");
			return -1;		// fail to create
		}
		if( !m_StackBar.Create(
				WS_VISIBLE|WS_CHILD|LVS_SHAREIMAGELISTS,
				CRect(0,0,50,200),
				&m_ParentStackBar,
				m_ParentStackBar.GetDlgCtrlID()
				)
			)
		{
			TRACE0("Failed to create m_StackBar\n");
			return -1;		// fail to create
		}

		m_StackBar.Init();
		pDebugStackBar=&m_StackBar;
		


		//Создаем контейнер окна свойств
		if(	!m_ParentPropertyBar.Create(
				_T("Окно свойств"),
				this,
				ID_PROPERTYBAR
				)
			)
		{
			TRACE0("Failed to create m_ParentPropertyBar\n");
			return -1;		// fail to create
		}
		//Создаем окно свойств
		if( !m_PropertyBar.Create(
			CRect(0,0,50,50),
			&m_ParentPropertyBar,
			IDC_OI,
			0x50010000
				)
			)
		{
			TRACE0("Failed to create m_PropertyBar\n");
			return -1;		// fail to create
		}
		m_PropertyBar.SetBorderStyle(CObjectInspector::bsSingle);
		m_PropertyBar.SetImageList(GetImageList());
	}





	if( !m_ComboHelpSearch.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
								  CRect( 0, 0, 200, 480 ),
								  &m_ToolMain,
								  ID_HELP_SEARCH_COMBO) || 
		!m_ToolMain.SetButtonCtrl(                                           
										m_ToolMain.CommandToIndex(
										m_ComboHelpSearch.GetDlgCtrlID()) ,
										&m_ComboHelpSearch) )
	{
		TRACE0( "Failed to create help search combobox\n" );
		return -1;
	}
			
	m_ComboHelpSearch.SetItemHeight( -1, 16 );

	m_ComboHelpSearch.SetFont( &g_PaintManager->m_FontNormal );
	g_CmdManager->CmdGetPtr(csProfileName,m_ComboHelpSearch.GetDlgCtrlID())-> m_sMenuText
							   = _T( "Search help system" );



	m_wndDocSelector.Create(NULL, "Панель окон", WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | 
							CBRS_TOP, CRect(0,0,0,0), this, IDS_DOC_SELECTOR);
	m_wndDocSelector.SetBarStyle( CBRS_ALIGN_BOTTOM);

	
	


    //m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ToolMain.EnableDocking(CBRS_ALIGN_ANY);
	m_ToolModule.EnableDocking(CBRS_ALIGN_ANY);
	if(nCurAppRunMode==CONFIG_MODE)
		m_ToolEdit.EnableDocking(CBRS_ALIGN_ANY);
	m_ParentMessageBar.EnableDocking(CBRS_ALIGN_ANY);

	if(nCurAppRunMode==CONFIG_MODE)
	{
		m_ParentDebugMessageBar.EnableDocking(CBRS_ALIGN_ANY);
		m_ParentStackBar.EnableDocking(CBRS_ALIGN_ANY);
		m_ParentPropertyBar.EnableDocking(CBRS_ALIGN_ANY);
	}


	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}


	//DockControlBar(&m_wndMenuBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_ToolMain);
	DockControlBar(&m_ToolModule,AFX_IDW_DOCKBAR_BOTTOM);
	if(nCurAppRunMode==CONFIG_MODE)
		DockControlBar(&m_ToolEdit,AFX_IDW_DOCKBAR_BOTTOM);


	RecalcLayout();

	m_ParentMessageBar.SetInitDesiredSizeHorizontal( CSize(80,80) );
	m_ParentMessageBar.DockControlBar(AFX_IDW_DOCKBAR_BOTTOM,1,this,false);

	if(nCurAppRunMode==CONFIG_MODE)
	{
		m_ParentDebugMessageBar.SetInitDesiredSizeHorizontal( CSize(80,80) );
		m_ParentDebugMessageBar.DockControlBar(AFX_IDW_DOCKBAR_BOTTOM,2,this,false);

		m_ParentStackBar.SetInitDesiredSizeHorizontal( CSize(80,80) );
		m_ParentStackBar.DockControlBar(AFX_IDW_DOCKBAR_RIGHT,1,this,false);

		m_ParentPropertyBar.SetInitDesiredSizeVertical( CSize(180,80) );
		m_ParentPropertyBar.DockControlBar(AFX_IDW_DOCKBAR_RIGHT,1,this,false);

//		m_ParentPropertyBar.SetInitDesiredSizeHorizontal( CSize(180,80) );
//		m_ParentPropertyBar.SetInitDesiredSizeFloating( CSize(380,380) );
//		m_ParentPropertyBar.FloatControlBar();

//		m_ParentPropertyBar.AutoHideModeGet();
	}







/*	g_CmdManager->SetBasicCommands(
	csProfileName,
	g_statBasicCommands);
*/
		
	RecalcLayout();
	
	CExtControlBar::ProfileBarStateLoad(
		this,
		pApp->m_pszRegistryKey,
		csProfileName,
		"Save",
		&m_dataFrameWP
		);


	//Восстанавливаем стиль
	CString csStyle=AfxGetApp()->GetProfileString(csProfileName, "Style");
	//if(csStyle!="XP")
	//	OnViewLikeOffice2k();
	OnViewLikeOffice2k();


	//передача ошибок в окно сообщений конфигуратора
	//if(nCurAppRunMode==CONFIG_MODE)
	//или отладка
	MyDDE.Create(MAKEINTRESOURCE(IDD_MYDATAEXCHANGE_DIALOG),this);


	CheckWindow(m_ToolMain);
	CheckWindow(m_ToolModule);
	if(nCurAppRunMode==CONFIG_MODE)
	{
		CheckWindow(m_ToolEdit);
		CheckWindow(m_ParentDebugMessageBar);
		CheckWindow(m_ParentStackBar);
		CheckWindow(m_ParentPropertyBar);
	}
	CheckWindow(m_ParentMessageBar);


	//**************************
	// Install the tab view here
	//VERIFY(m_MDIClient.SubclassMDIClient(this));

//	m_ToolMain.InsertButton();



	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL afxExitMode=0;
void CMainFrame::OnClose() 
{
	if(AfxGetModuleManager())
		if(!AfxGetModuleManager()->IsCanDestroy())
			return;//программа не разрешила закрывать

	if(AfxGetMetadataTree())
		if(AfxGetMetadataTree()->GetParent())
		{
			afxExitMode=1;
			AfxGetMetadataTree()->SaveOpenModulesState();
			//AfxGetMetadataTree()->GetParent()->BringWindowToTop();
		}

	CMDIChildWnd* pChild;
	while(pChild=MDIGetActive())
	{
		pChild->SendMessage(WM_CLOSE);
		if(pChild==MDIGetActive())
			return;//была нажата кнопка Отмена
	}


	afxExitMode=0;
	pMessageWindow=0;
	pStatusBar=0;

  //  m_MDIClient.SaveMainFrameState();

	CMDIFrameWnd::OnClose();
}





void CMainFrame::OnEditCopy() 
{
	CWnd *pW=GetFocus();
	if(pW)
	{
//		if(CString(pW->GetRuntimeClass()->m_lpszClassName)=="CFormEditor")
//			((CFormEditor*)pW)->OnEditCopy();
//		else
		if(!pW->SendMessage(WM_COMMAND,ID_EDIT_COPY))
			if(!pW->SendMessage(WM_COPY))
				pW->SendMessage(WM_COMMAND,WM_COPY);
			
	}
}

void CMainFrame::OnEditPaste() 
{
	CWnd *pW=GetFocus();
	if(pW)
	{
/*		if(CString(pW->GetRuntimeClass()->m_lpszClassName)=="CFormEditor")
			((CFormEditor*)pW)->OnEditPaste();
		else
*/
		if(!pW->SendMessage(WM_COMMAND,ID_EDIT_PASTE))
			if(!pW->SendMessage(WM_PASTE))
				pW->SendMessage(WM_COMMAND,WM_PASTE);
	}
}

void CMainFrame::OnEditCut() 
{
	CWnd *pW=GetFocus();
	if(pW)
	{
/*		if(CString(pW->GetRuntimeClass()->m_lpszClassName)=="CFormEditor")
			((CFormEditor*)pW)->OnEditCut();
		else
*/
		if(!pW->SendMessage(WM_COMMAND,ID_EDIT_CUT))
			if(!pW->SendMessage(WM_CUT))
				pW->SendMessage(WM_COMMAND,WM_CUT);
	}
}


void CMainFrame::OnSelectMenu(UINT nID)
{
	int nMenu=nID-MENU_START_ID;
	if(nMenu<0||nMenu>=aAllMenuItems.GetSize())
	{
		Message(String("Ошибка: идентификатор обработки меню не найден"));
		return;
	}
	
	try
	{

		CMyMenuItem element=aAllMenuItems[nMenu];
		if(element.nType==MENU_FUNCTION)
		{
			if(element.pRun)
			{
				int nRes=element.pRun->FindFunction(element.sFunction,0);
				if(nRes>=0)
					element.pRun->CallFunction(element.sFunction,element.vData);
				else
					OpenFormExt(element.sFunction,CValue());
			}
		}
		else
		if(element.nType==MENU_OBJECT)
			element.vObject.CallFunction(element.sFunction,&element.vData);
	}
	catch(CTranslateError *)
	{
		DoError();
	};
}

BOOL CMainFrame::DestroyWindow() 
{
	ShowControlBar(&m_ParentMessageBar,0,1);	
	if(nCurAppRunMode==CONFIG_MODE)
		ShowControlBar(&m_ParentPropertyBar,0,1);	


	CWinApp * pApp = ::AfxGetApp();
	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			csProfileName,
			"Save"
			)
		);
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
/*	
	//Сохраняем стиль
	CString Str="2K";
	if(g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		Str="XP";
	pApp->WriteProfileString(csProfileName, "Style", Str);
*/
	bStopRunProgram=1;

	return CMDIFrameWnd::DestroyWindow();
	//exit(0);
	//return 1;
}

int CMainFrame::DoCloseWnd(CExtControlBar &Box)//заданное Окно, которое требуется закрыть
{
	CWnd *pWnd=GetFocus();
	if(pWnd)
	{
		CWnd *pParent=pWnd->GetParent();
		if(pParent==&Box)//если текущее окно - заданное Окно, то не закрываем
		{
			CMDIChildWnd* pView=MDIGetActive();
			if(pView)
			{
				MDIActivate(pView);
				pView->SetFocus();
				return 0;
			}
		}
		//иначе, если открыто заданное Окно - закрываем
		if(Box.IsVisible())
		{
			ShowControlBar(&Box,0,1);	
			return 1;
		}
	}
	return -1;
}
BOOL CMainFrame::DoCloseMessageBox(MSG* pMsg) 
{
	CWnd *pWnd=GetFocus();
	int nChar= (int) pMsg->wParam;
	if(nChar==VK_ESCAPE) 
	if(pWnd)
	if(pWnd->GetParent())
	{
		int nRes;
		nRes=DoCloseWnd(m_ParentMessageBar);//Окно сообщений
		if(nRes!=-1) 
			return nRes;

		if(nCurAppRunMode==CONFIG_MODE)
			nRes=DoCloseWnd(m_ParentPropertyBar);//Окно свойств
		if(nRes!=-1) 
			return nRes;

		if(nCurAppRunMode==CONFIG_MODE)
			nRes=DoCloseWnd(m_ParentStackBar);//Окно стека вызовов
		if(nRes!=-1) 
			return nRes;

		if(nCurAppRunMode==CONFIG_MODE)
			nRes=DoCloseWnd(m_ParentDebugMessageBar);//Окно отладки
		if(nRes!=-1) 
			return nRes;

	}
	return FALSE;
}
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
/*	if(pMsg->message == WM_KEYDOWN)
	{
		CString Str;
		Str.Format("2 WM_KEYDOWN = %d",pMsg->wParam);

		Message(Str);
	}*/

	if(pMsg->message == WM_KEYDOWN)
	{
		CWnd *pWnd=GetFocus();
		int nChar= (int) pMsg->wParam;


		//перенаправляем сообщения по работе с буфером обмена (копирование/вставка)
		if(pWnd)
		{
			if (IsCTRLpressed())
			{
				switch (nChar)
				{
				case 'X':
					OnEditCut();
					return TRUE;
				case VK_INSERT:
				case 'C':
					OnEditCopy();
					return TRUE;
				case 'V':
					OnEditPaste();
					return TRUE;
				}
			}
			else
			if(IsSHIFTpressed()&&nChar==VK_INSERT)
			{
					OnEditPaste();
					return TRUE;
			}
			else
			{
				DoCloseMessageBox(pMsg);
			}
		}
	}

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}


void CMainFrame::OnViewLikeOffice2k() 
{
	if( !g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManager )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
}


void CMainFrame::OnUpdateViewLikeOffice2k(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
			? FALSE
			: TRUE
		);
}

void CMainFrame::OnViewLikeOfficeXp() 
{
	if( g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerXP )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
}

void CMainFrame::OnUpdateViewLikeOfficeXp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
			? TRUE
			: FALSE
		);
}

HWND Get2C();
void PauseDebug();
extern int nRunLine;
void StopProgram();
void RunDebug();
void NextDebug(int);
void NextDebugOver(int);

void CMainFrame::OnPauseDebug() 
{
	PauseDebug();
}

void CMainFrame::OnUpdatePauseDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(Get2C()!=0&&nRunLine==-1&&nCurAppRunMode==CONFIG_MODE);
}
void CMainFrame::OnUpdateStopDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(Get2C()!=0&&nCurAppRunMode==CONFIG_MODE);
}

void CMainFrame::OnStopProgram() 
{
	StopProgram();
}

void CMainFrame::OnUpdateRunDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(Get2C()!=0&&nRunLine>-1&&nCurAppRunMode==CONFIG_MODE);
}

void CMainFrame::OnRunDebug() 
{
	if(nRunLine>-1)
	{
		::RunDebug();
	}
}

void CMainFrame::OnNextDebugOver() 
{
	if(nRunLine>-1)
	{
		NextDebugOver(nRunLine);
		RunDebug();
	}
}

void CMainFrame::OnNextDebug() 
{
	if(nRunLine>-1)
	{
		NextDebug(nRunLine);
		RunDebug();
	}
}


void CMainFrame::OnPostShow() 
{
	ShowWindow(SW_SHOWMAXIMIZED);
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIFrameWnd::OnSetFocus(pOldWnd);
	

}

void CMainFrame::GetControlBarsEx(CArray<CControlBar*, CControlBar*>& arrBars)
{
	if (::IsWindow(m_ToolMain.m_hWnd))
		arrBars.Add(&m_ToolMain);      
	if (::IsWindow(m_wndStatusBar))
		arrBars.Add(&m_wndStatusBar); 

	//...more here
}

