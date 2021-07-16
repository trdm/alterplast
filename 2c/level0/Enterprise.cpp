/*///////////////////////////////////////////////////////////////////////////
Интегрированная платформа автоматизации предприятия
Авторские права: VTOOLS.RU (Юрий Иванов, info@vtools.ru) 2002-2004г.
Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ.  Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU - http://gnu.org.ru/gpl.html
///////////////////////////////////////////////////////////////////////////*/

#include "stdafx.h"
#include "Enterprise.h"
#include "Resource.h"
#include "DialogUnit.h"
#include "FormFrame.h"
#include "Compiler/ValueMenu.h"
#include "Compiler/ValueMeta.h"
#include "Compiler/ValueModule.h"
#include "Compiler/ValueText.h"
#include "Compiler/ValueToolBar.h"
#include "Compiler/ValueRange.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "InputDialog.h"

#include "MetadataTree.h"
#include "Compiler/CompileModule.h"
#include "Compiler/ProcUnit.h"
#include "Compiler/ValueArray.h"
#include "Compiler/ValueListLayers.h"
#include "MetedataDocument.h"
#include "FormViewConfig.h"

#include "ModuleView.h"
#include "ModuleDoc.h"

#include "CREDITS/CREDITS.h"

#include "ModuleManager.h"

#include "EditForm/FormEditor.h"
#include "FormUnit.h"
#include "DocUnit.h"


#include "Compiler/ValueFileSystem.h"
#include "Compiler/ValueGrid.h"

#include "help_view/MyHelpDoc.h"
#include "help_view/MyHelpView.h"

#include "GridCtrl_src/View/GridView.h"
#include "Compiler/ValueGridExt.h"
#include "OpenForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define MIN_PLUG_VER 2//МИНИМАЛЬНАЯ ВЕРСИЯ ПЛАГИНОВ
#define CUR_PLUG_VER PLUG_VER//ТЕКУЩАЯ ВЕРСИЯ ПЛАГИНОВ


bool FileExist(CString sFileName);


//Глобальные переменные программы
CString m_csConfigPath;//путь к файлу конфигурации
CModuleManager m_RunModuleManager;//выполняемая среда
int nReturnStatus=1;//Статус возврата
int afxAppRunMode=ENTERPRISE_MODE;
CString csCurrentExeDir;
CString csIBDir;
CString csTitleWnd;
CString csTitleWndOrig;
CString csUserPath;//каталог пользователя для записи настроек
CString csUserName;
CString csUserPassword;
int bSingleMode=0;//Монопольный режим

CDefList glDefList;//глобальный массив определений

BOOL bViewLibObjects=0;//показывать в дереве метаданных объекты из системных библиотек

CWnd *afxModalParent=0;
CWnd* GetModalParent()
{
	return afxModalParent;
};

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CEnterpriseApp, CWinApp)
	//{{AFX_MSG_MAP(CEnterpriseApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_ENTERPRISE, OnButtonEnterprise)
	ON_UPDATE_COMMAND_UI(ID_ENTERPRISE, OnUpdateButtonEnterprise)
	ON_COMMAND(ID_CONFIG, OnConfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG, OnUpdateConfig)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_OPENCONFIG, OnOpenconfig)
	ON_UPDATE_COMMAND_UI(ID_OPENCONFIG, OnUpdateOpenconfig)
	ON_COMMAND(ID_ALL_FIND, OnAllFind)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterpriseApp construction
CEnterpriseApp::CEnterpriseApp()
{
}


CEnterpriseApp::~CEnterpriseApp()
{

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEnterpriseApp object
CEnterpriseApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CEnterpriseApp initialization
#include "locale.h"
#include "EditDialog.h"

CReportManager *CEnterpriseApp::m_pSystemManager;
BOOL CEnterpriseApp::InitInstance()
{

	SetRegistryKey(_T("2C"));
	//заставка
	#define SAMPLE_TEXT_INTERVAL_MILSEC 1000 
	//m_wndSplash.LoadBitmap(IDB_BITMAP_SPLASH);
	//Sleep(700);

    AfxInitRichEdit();


	RegisterObject(RUNTIME_CLASS(CValueList),"СписокЗначений");
	RegisterObject(RUNTIME_CLASS(CValueList),"ValueList");

	RegisterObject(RUNTIME_CLASS(CValueTreeCtrl),"ДеревоЗначений");
	RegisterObject(RUNTIME_CLASS(CValueTreeCtrl),"ValueTreeCtrl");

	RegisterObject(RUNTIME_CLASS(CValueArray),"Массив");
	RegisterObject(RUNTIME_CLASS(CValueArray),"Array");
	RegisterObject(RUNTIME_CLASS(CValueArray),"Структура");

	RegisterObject(RUNTIME_CLASS(CValueMetadata),"ФайлМетаданных");
	RegisterObject(RUNTIME_CLASS(CValueMetadata),"MetadataFile");
	
	RegisterObject(RUNTIME_CLASS(CValueSQL),"ЗапросSQL");
	RegisterObject(RUNTIME_CLASS(CValueSQL),"QuerySQL");

	RegisterObject(RUNTIME_CLASS(CValueFileSystem),"ФС");
	RegisterObject(RUNTIME_CLASS(CValueFileSystem),"Файловая система");
	RegisterObject(RUNTIME_CLASS(CValueFileSystem),"FS");

	RegisterObject(RUNTIME_CLASS(CValueGrid),"!Grid");
	RegisterObject(RUNTIME_CLASS(CValueGrid),"Grid");
//	RegisterObject(RUNTIME_CLASS(CValueGrid),"Сетка");
	
	RegisterObject(RUNTIME_CLASS(CValueMenu),"Меню");
	RegisterObject(RUNTIME_CLASS(CValueMenu),"Menu");

	RegisterObject(RUNTIME_CLASS(CValueMeta),"Метаданные");
	RegisterObject(RUNTIME_CLASS(CValueMeta),"Metadata");
	
	RegisterObject(RUNTIME_CLASS(CValueModule),"Модуль");
	RegisterObject(RUNTIME_CLASS(CValueModule),"Module");
	
	RegisterObject(RUNTIME_CLASS(CValueText),"Текст");
	RegisterObject(RUNTIME_CLASS(CValueText),"Text");

	RegisterObject(RUNTIME_CLASS(CValueNumber0),"!Число");
	RegisterObject(RUNTIME_CLASS(CValueString0),"!Строка");
	RegisterObject(RUNTIME_CLASS(CValueDate0),"!Дата");
	
	RegisterObject(RUNTIME_CLASS(CValueNumber0),"Число");
	RegisterObject(RUNTIME_CLASS(CValueString0),"Строка");
	RegisterObject(RUNTIME_CLASS(CValueDate0),"Дата");

	//Служебное использование:
	RegisterObject(RUNTIME_CLASS(CValueControl),"!CValueControl");
	RegisterObject(RUNTIME_CLASS(CValueControl),"CValueControl");
	RegisterObject(RUNTIME_CLASS(CValueForm),"!CValueForm");
	RegisterObject(RUNTIME_CLASS(CValueForm),"CValueForm");

	RegisterObject(RUNTIME_CLASS(CValueListLayers),"Закладки");

	RegisterObject(RUNTIME_CLASS(CValueToolBar),"ПанельИнструментов");
	RegisterObject(RUNTIME_CLASS(CValueToolBar),"ToolBar");

	RegisterObject(RUNTIME_CLASS(CValueRange),"Область");
	RegisterObject(RUNTIME_CLASS(CValueRange),"Area");


	if (!AfxOleInit())
	{
		return FALSE;
	}

	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	csUserPath=".\\USERS";


	int nIsDDE=0;
	CString csRunProgram;
	int bUseNew=0;
	if(__argc>1)
	{
		for(int i=1;i<__argc;i++)
		{
			//AfxMessageBox(__argv[i]);

			CString csMode=__argv[i];
			csMode.MakeUpper();
			if(csMode=="/DDE")
			{
				nIsDDE=1;
				break;
			}
			else
			if(csMode=="/ENTERPRISE" || csMode=="ENTERPRISE")   //Режим Предприятие
				afxAppRunMode=ENTERPRISE_MODE;
			else
			if(csMode=="/CONFIG" || csMode=="CONFIG")           //Режим Конфигуратор
				afxAppRunMode=CONFIG_MODE;
			else
			if(csMode.Left(8)=="/DEFINE:")						//Общие константы уровня компиляции
			{
				CString Str=__argv[i];
				CString csName,csValue;
				Str=Str.Mid(8);

				int nIndex=Str.Find('=');
				if(nIndex>0)
				{
					csName=Str.Left(nIndex);
					csValue=Str.Mid(nIndex+1);
				}
				else
				{
					csName=Str;
				}
				glDefList.SetDef(csName,csValue);
			}
			else
			if(csMode.Left(2)=="/D")							//Каталог конфигурации
			{
				csIBDir=csMode.Mid(2);
				csIBDir.TrimRight();
				csIBDir.TrimLeft('"');
				csIBDir.TrimRight('"');
				csIBDir.TrimRight("\\");
				csIBDir.TrimRight();
			}
			else
			if(csMode.Left(6)=="/TITLE")						//Заголовок главного окна
			{
				CString csMode=__argv[i];
				csMode=csMode.Mid(6);
				csMode.TrimLeft('"');
				csMode.TrimRight('"');
				csTitleWnd=csMode;
				csTitleWndOrig=csMode;
			}
			else
			if(csMode.Right(3)==".FD")							//Запуск внешней формы
			{
				if(FileExist(csMode))
					csRunProgram=csMode;
			}
			else
			if(csMode.Left(2)=="/U")							//Каталог пользователя
			{
				CString csMode=__argv[i];
				csMode=csMode.Mid(2);
				csUserPath=csMode;
			}
			else
			if(csMode.Left(2)=="/N")							//Имя пользователя
			{
				CString csMode=__argv[i];
				csMode=csMode.Mid(2);
				csUserName=csMode;
			}
			else
			if(csMode.Left(2)=="/P")							//Пароль пользователя
			{
				CString csMode=__argv[i];
				csMode=csMode.Mid(2);
				csUserPassword=csMode;
			}
			else
			if(csMode.Left(2)=="/M")							//Монопольно
			{
				bSingleMode=1;
			}
			else
			if(csMode.Left(2)=="/L")							//Видимость библ. объектов
			{
				bViewLibObjects=1;
			}
			else
			if(csMode.Left(2)=="/F")							//Файл конфигурации
			{
				CString csMode=__argv[i];
				m_csConfigPath=csMode.Mid(2);

				if(!FileExist(m_csConfigPath))
				{
					MessageBox(0,CString("Файл: ")+m_csConfigPath+" не найден!","Ошибка!",MB_ICONEXCLAMATION);
					m_csConfigPath="";
				}
			}
			
		}
	}
	

	csCurrentExeDir=__argv[0];

	//корректировка каталога программы
	int nIndex=csCurrentExeDir.ReverseFind('\\');
	if(nIndex>0)
		csCurrentExeDir=csCurrentExeDir.Left(nIndex);
	else
	{
		GetCurrentDirectory(1024,csCurrentExeDir.GetBuffer(1024));
		csCurrentExeDir.ReleaseBuffer();
	}
	csCurrentExeDir.TrimRight('\\');
	
	//Загрузка плагинов
	CPluginServ::Init();


	if(nIsDDE==0)
	{
		CString csRun;
		if(csRunProgram.IsEmpty())
			csRun=csCurrentExeDir+"\\System\\dirselector.fd";
		else
			csRun=csRunProgram;

		if(csIBDir.IsEmpty())
		if(FileExist(csRun))//старт диалога
		{
			afxAppRunMode=START_MODE;
			CDialogUnit dlg;
			afxModalParent=&dlg;
			afxFormPath=csRun;
			dlg.DoModal();
			return TRUE;
		}

		if(!csIBDir.IsEmpty())
		if(!FileExist(csIBDir))
		{
			MessageBox(0,CString("Каталог ")+csIBDir+" не найден!","Ошибка!",MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	//m_wndSplash.Show(SAMPLE_TEXT_INTERVAL_MILSEC, m_pMainWnd);
	//корректировка каталога базы данных
	if(!(csIBDir.GetLength()>=2&&(csIBDir.Left(2)=="\\\\"||csIBDir.Mid(1,1)==":")))
	{
		csIBDir=csCurrentExeDir+"\\"+csIBDir;
	}
	SetCurrentDirectory(csIBDir);

	//определяем каталог пользователя
	if(csUserPath.Left(2)==".\\" || (csUserPath.Mid(1,1)!=":" && csUserPath.Mid(1,1)!="\\"))//задан путь относительно каталога базы данных
	{
		csUserPath=csIBDir+csUserPath.Mid(1);
	}
	if(!FileExist(csUserPath))
	{
		CreateDirectory(csUserPath,0);
	}

	//корректировка каталога пользователя
	if(!FileExist(csUserPath))
	{
		csUserPath=csIBDir+"\\USERS";
		if(!FileExist(csUserPath))
			CreateDirectory(csUserPath,0);
	}


	//для записи файлов
	m_pDocManager=new CDocManager();

	//специальный
	m_pSystemManager=	new CReportManager("Выбор вида документа",TRUE);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame();
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	//pMainFrame->RecalcLayout();


	if(!csTitleWnd.IsEmpty())
	{
		if(AfxGetMainWnd())
			AfxGetMainWnd()->SetWindowText(csTitleWnd);
		pMainFrame->SetTitle(csTitleWnd);
	}


	CMultiDocTemplate* pDocTemplate;
	if(afxAppRunMode==CONFIG_MODE)
	{
		pDocTemplate=new CMultiDocTemplate(
			IDR_FORM1C_TMPL,
			RUNTIME_CLASS(CModuleDoc),
			RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CFormEditor));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Форма","Форма",1,ICON_FORM);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

		pDocTemplate=new CMultiDocTemplate(
		IDR_SAMPLETYPE,
		RUNTIME_CLASS(CModuleDoc), 
		RUNTIME_CLASS(CMDIChildWnd),
		RUNTIME_CLASS(CModuleView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Модуль","Модуль",1,601);
		//pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;


		pDocTemplate=new CMultiDocTemplate(
		IDR_METADATATREE,
		RUNTIME_CLASS(CMetedataDocument), 
		RUNTIME_CLASS(CMDIChildWnd),
		RUNTIME_CLASS(CMetadataTree));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Метаданные","Метаданные",0);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

		pDocTemplate=new CMultiDocTemplate(
			1,//пусто
			RUNTIME_CLASS(CDocUnit),
			RUNTIME_CLASS(CFormFrame),
			RUNTIME_CLASS(CFormUnit));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"ИсполняемаяФорма","ИсполняемаяФорма",0);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;


		pDocTemplate=new CMultiDocTemplate(
			IDS_RTF_TYPE,
			RUNTIME_CLASS(CMyHelpDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CMyHelpView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Документация","Документация",1,394);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;


		pDocTemplate=new CMultiDocTemplate(
			IDR_ENTERPTYPE,
			RUNTIME_CLASS(CGridDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CGridView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Таблица","Таблица",1,ICON_MAKET);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;



	}
	else
	{
		pDocTemplate=new CMultiDocTemplate(
			IDR_FORM1C_TMPL,
			RUNTIME_CLASS(CDocUnit),
			RUNTIME_CLASS(CFormFrame),
		RUNTIME_CLASS(CFormUnit));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Форма","Форма",0);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

		pDocTemplate=new CMultiDocTemplate(
		IDR_SAMPLETYPE,
		RUNTIME_CLASS(CModuleDoc), 
		RUNTIME_CLASS(CMDIChildWnd),
		RUNTIME_CLASS(CModuleView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Модуль","Модуль",1,601);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

		pDocTemplate=new CMultiDocTemplate(
			IDS_RTF_TYPE,
			RUNTIME_CLASS(CMyHelpDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CMyHelpView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Документация","Документация",0);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

		pDocTemplate=new CMultiDocTemplate(
			IDR_ENTERPTYPE,
			RUNTIME_CLASS(CGridDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CGridView));
		m_pSystemManager->AddDocTemplate(pDocTemplate,"Таблица","Таблица",1,ICON_MAKET);
		pDocTemplate->m_hMenuShared=pMainFrame->m_hMenuDefault;

	}

	//регистрация Таблицы только если запуск с поддержкой MDI
	RegisterObject(RUNTIME_CLASS(CValueGridExt),"Таблица");


	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	// Enable DDE Execute open
	EnableShellOpen();
	m_pSystemManager->RegisterShellFileTypes(TRUE);


	// The main window has been initialized, so show and update it.
	m_nCmdShow=SW_SHOWMAXIMIZED;
	pMainFrame->PostMessage(WM_COMMAND,ID_POST_SHOW,0);
	
	pMainFrame->UpdateWindow();


	if(m_csConfigPath.IsEmpty())
		m_csConfigPath=csIBDir+"\\"+METADATA_FILENAME;


	if(!AfxGetMainWnd())
		return 0;

	if(afxAppRunMode==CONFIG_MODE)
	{
		OnOpenconfig();
	}


	if(!AfxGetMainWnd())
		return 0;


	if(afxAppRunMode==ENTERPRISE_MODE)
	{
		csIBDir.TrimRight("\\");
		if(FileExist(m_csConfigPath))
		{
			try
			{
				m_RunModuleManager.pMetadata->Load(m_csConfigPath);
				m_RunModuleManager.Load(1);//пользовательская среда
			}
			catch(CTranslateError *)
			{
				DoError();
			};
		}
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(!csRunProgram.IsEmpty())
	{
		cmdInfo.m_nShellCommand=CCommandLineInfo::FileOpen;
		cmdInfo.m_strFileName=csRunProgram;
		ProcessShellCommand(cmdInfo);
	}



	//	m_wndSplash.Hide();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEnterpriseApp message handlers

void CEnterpriseApp::OnButtonTest() 
{
}
/////////////////////////////////////////////////////////////////////////////
void CEnterpriseApp::OnFileNew() 
{
	m_pSystemManager->OnFileNew();
}
void CEnterpriseApp::OnFileOpen() 
{
	m_pSystemManager->OnFileOpen();
}

CDocument* CEnterpriseApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	CDocument* pDoc=m_pSystemManager->OpenDocumentFile(lpszFileName);
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		CView *pView=pDoc->GetNextView(pos);
		if(pView)
		if(!pView->IsWindowVisible())
		if(pView->GetParentFrame())
		{
			pView->GetParentFrame()->SendMessage(WM_CLOSE);
		}
	};

	return pDoc;
}



// App command to run the dialog
void CEnterpriseApp::OnAppAbout()
{
	CCredits aboutDlg;
	aboutDlg.DoModal();
}


int CEnterpriseApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}



void CEnterpriseApp::OnButtonEnterprise() 
{
	if(AfxGetMetadataTree())
		if(!AfxGetMetadataTree()->GetDocument()->SaveModified())
			return;

	SetCurrentDirectory(csCurrentExeDir);
	if (bSingleMode)
	ShellExecute(NULL,NULL,__argv[0],CString("/M")+" /N"+csUserName+" /P"+csUserPassword+" /d\""+csIBDir+"\""+" /title\""+csTitleWndOrig+"\"",NULL,SW_HIDE);
	else
	ShellExecute(NULL,NULL,__argv[0],CString("/enterprise")+" /N"+csUserName+" /P"+csUserPassword+" /d\""+csIBDir+"\""+" /title\""+csTitleWndOrig+"\"",NULL,SW_HIDE);
}

void CEnterpriseApp::OnUpdateButtonEnterprise(CCmdUI* pCmdUI) 
{
	static BOOL bFirst=1;

	if(bFirst)
	if(afxAppRunMode==ENTERPRISE_MODE)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
	bFirst=0;
}

void CEnterpriseApp::OnConfig() 
{
	SetCurrentDirectory(csCurrentExeDir);
	ShellExecute(NULL,NULL,__argv[0],CString("/config")+" /N"+csUserName+" /P"+csUserPassword+" /d\""+csIBDir+"\""+" /title\""+csTitleWndOrig+"\"",NULL,SW_HIDE);
}

void CEnterpriseApp::OnUpdateConfig(CCmdUI* pCmdUI) 
{
	static BOOL bFirst=1;

	if(bFirst)
	if(afxAppRunMode==CONFIG_MODE)
		pCmdUI->Enable(0);
	else
		pCmdUI->Enable(1);
	bFirst=0;
}

void CEnterpriseApp::OnHelp() 
{
	CDocument *pDoc=OpenFormMDI("Help");
	if(pDoc)
		pDoc->SetTitle("HTML Help");

}

void CEnterpriseApp::OnOpenconfig() 
{
	if(csIBDir.IsEmpty())
	{
		MessageBox(0,CString("Не определен каталог информационной базы!"),"Ошибка!",MB_ICONEXCLAMATION);
		return;
	} 

	try
	{
		if(!m_RunModuleManager.pTree)
		{
			m_RunModuleManager.pMetadata->Load(m_csConfigPath);
			m_RunModuleManager.Load(1);//конфигуратор
		}
		else//активизация окна
		{
		   CDocument *pTree=m_RunModuleManager.pTree;
		   if(pTree)
		   {
			   POSITION pos = pTree->GetFirstViewPosition();
			   CView *pView=pTree->GetNextView(pos);
			   if(pView->GetParentFrame())
				   pView->GetParentFrame()->BringWindowToTop();
		   }
		}
	}
	catch(CTranslateError *) 
	{
		if(!m_RunModuleManager.pTree)
		{
			m_RunModuleManager.pTree=OpenFormMDI("Метаданные",1);
			if(m_RunModuleManager.pTree)
				m_RunModuleManager.pTree->SetTitle(CONFIG_NAME);
		}
		DoError();
	};

}
void CEnterpriseApp::DeleteConfigName()
{
	if(m_pRecentFileList->GetSize())//убираем из списка MRU
	{
		CString Str=(*m_pRecentFileList)[0];
		int nIndex=Str.ReverseFind('\\');
		if(nIndex>0)
			Str=Str.Mid(nIndex+1);
		if(Str==CONFIG_NAME)
			m_pRecentFileList->Remove(0);
	}
}
void CEnterpriseApp::OnUpdateOpenconfig(CCmdUI* pCmdUI) 
{
	static BOOL bFirst=1;
	if(bFirst)
	pCmdUI->Enable(afxAppRunMode==CONFIG_MODE);
	bFirst=0;
}

BOOL CEnterpriseApp::DoPromptFileName(CString& fileName, UINT nIDSTitle,
		DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	return m_pSystemManager->DoPromptFileName(fileName, nIDSTitle,lFlags, bOpenFileDialog, pTemplate);
}
int nWaitCursor=1;
extern BOOL bDebugStopLine;
extern int nCurrentNumberStopContext;
BOOL CEnterpriseApp::OnIdle(LONG lCount) 
{
	if(nWaitCursor)
	{
		EndWaitCursor();
		nWaitCursor=0;
		CString Str;
		Str.LoadString(AFX_IDS_IDLEMESSAGE);
		SetStatus(Str);
	}

	bDebugStopLine=0;
	nCurrentNumberStopContext=0;

	return CWinApp::OnIdle(lCount);
}




/////////////////////////////////////////////////////////////////////////////

template <typename TTYPE> inline void TypeGetProcAddress(TTYPE &p,HMODULE hmod,LPCSTR lpProc)
{
	 p = (TTYPE) ::GetProcAddress(hmod,lpProc);
}


CPluginServ* ListPlugins=NULL;
void CPluginServ::Init()
{
	if (ListPlugins)return;
	CString StartPath=csCurrentExeDir;
	if('\\' != StartPath[StartPath.GetLength()-1] )StartPath+='\\';
	
	for(int nCount=1;nCount<=2;nCount++)
	{
		if(2==nCount)
			StartPath += "SYSTEM\\";
		
		SetCurrentDirectory(StartPath);

		WIN32_FIND_DATA fdata;
		HANDLE hfind=FindFirstFile(StartPath+"*.dll",&fdata);
		if(NULL==hfind || INVALID_HANDLE_VALUE==hfind) return;
		CPluginServ *NextServ=new CPluginServ;
		PLUGIN_INFO pinfo; 
		BOOL  (_cdecl  * fInfo)(PLUGIN_INFO*);
		BOOL  (_cdecl * fInit)(void);
		if(NextServ)
			do{
				HMODULE hmod=AfxLoadLibrary(StartPath +fdata.cFileName);
				if(NULL==hmod || INVALID_HANDLE_VALUE==hmod) continue;
				pinfo.cb=sizeof(PLUGIN_INFO);
				pinfo.VerNeed=CUR_PLUG_VER;
				
				TypeGetProcAddress(fInit,hmod,"InitPlugin" PLUGINFUNC_SUFFIX);
				TypeGetProcAddress(fInfo,hmod,"PluginInfo" PLUGINFUNC_SUFFIX);
				if (!fInit 
					|| !fInfo 
					|| !fInfo(&pinfo)
					|| pinfo.VerNeed < MIN_PLUG_VER
					|| !fInit()
					) { 
					AfxFreeLibrary(hmod);
					continue;
				}
				
				if(pinfo.csTitle.IsEmpty())
				{
					pinfo.csTitle=fdata.cFileName;
				}
				
				NextServ->Info=pinfo;
				NextServ->Next=ListPlugins;
				NextServ->mod=hmod;
				ListPlugins=NextServ;
				NextServ=new CPluginServ;
				
			}while( FindNextFile(hfind,&fdata) 
				&& NextServ);
			
			if(NextServ)delete NextServ;
			FindClose(hfind);
	}
}

void ViewError()
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );
}


void CPluginServ::Done()
{
	for(CPluginServ *NextServ,*CurServ=ListPlugins;CurServ;CurServ=NextServ){
			NextServ=CurServ->Next;
			int (__cdecl *fDone)(void);
			TypeGetProcAddress(fDone,CurServ->mod,"DonePlugin" PLUGINFUNC_SUFFIX);
			if(fDone)fDone();
			AfxFreeLibrary(CurServ->mod);
			delete CurServ;
	}
	ListPlugins=NULL;
}


/////////////////////////////////////////////////////////////////////////////









void CEnterpriseApp::OnAllFind() 
{
	if(AfxGetMetadataTree())
		AfxGetMetadataTree()->OnAllFind();
}


// Main running routine until application exits
int CEnterpriseApp::Run()
{
	if (m_pMainWnd == NULL && AfxOleGetUserCtrl())
	{
		// Not launched /Embedding or /Automation, but has no main window!
		TRACE0("Warning: m_pMainWnd is NULL in CWinApp::Run - quitting application.\n");
		AfxPostQuitMessage(0);
	}
	return MyRun();
}
// main running routine until thread exits
int CEnterpriseApp::MyRun()
{
	ASSERT_VALID(this);

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// acquire and dispatch messages until a WM_QUIT message is received.
	for (;;)
	{
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; // assume "no idle" state
		}

		// phase2: pump messages while available
		do
		{
			try
			{
			// pump message, but quit on WM_QUIT
			if (!PumpMessage())
				return ExitInstance();
			}
			catch(...)
			{
				OutToDebug("Произошла критическая ошибка программы\n");
				Message("Неопознанная ошибка. Рекомендуется завершить работу программы.");
				//AfxMessageBox("Неопознанная ошибка!");
				/*HWND hWnd=0;
				if(AfxGetMainWnd())
					hWnd=AfxGetMainWnd()->m_hWnd;
				int nRet=MessageBox(hWnd,"Неопознанная ошибка!\nЗавершить работу программы?","Внимание",MB_OKCANCEL|MB_ICONQUESTION);
				if(nRet==1)
					return ExitInstance();
				*/

			}

			// reset "no idle" state after pumping "normal" message
			if (IsIdleMessage(&m_msgCur))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}

	ASSERT(FALSE);  // not reachable
}