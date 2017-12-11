// SubsViewMain.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SubsViewMain.h"
#include "SubsView.h"

#include <AFXPRIV.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubsViewMain

IMPLEMENT_DYNCREATE(CSubsViewMain, CFrameWnd)

CSubsViewMain::CSubsViewMain()
{
}

CSubsViewMain::~CSubsViewMain()
{
}


BEGIN_MESSAGE_MAP(CSubsViewMain, CFrameWnd)
	//{{AFX_MSG_MAP(CSubsViewMain)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubsViewMain message handlers

BOOL CSubsViewMain::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CSubsView *pview;

	// Create a context.
	CCreateContext context;
	pContext = &context;

	// Assign custom view.
	pContext->m_pNewViewClass = RUNTIME_CLASS(CSubsView);

	// Create the view.
	pview = (CSubsView *) CreateView(pContext, AFX_IDW_PANE_FIRST);
	if (pview == NULL)
		return FALSE;

	// Notify the view.
	pview->SendMessage(WM_INITIALUPDATE);
	SetActiveView(pview, FALSE);
	return TRUE;
}


int CSubsViewMain::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		/*| CBRS_GRIPPER */| CBRS_TOOLTIPS | CBRS_FLYBY /*| CBRS_SIZE_DYNAMIC*/) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetBorders(0, 0, 0, 0);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);	
	return 0;
}



BOOL CSubsViewMain::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.dwExStyle&=~WS_EX_CLIENTEDGE;	
	cs.style&=~WS_BORDER;	
	cs.style&=~DS_3DLOOK;	
	return CFrameWnd::PreCreateWindow(cs);
}
