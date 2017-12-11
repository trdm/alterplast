// BaseView.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BaseView.h"
#include "mdsplitter.h"
#include "MyView.h"
#include "SubsViewMain.h"
#include "MyADO.h"

extern CMyADO* pAdo;

/*
#include "mdview.h"
#include "infopane.h"
#include "mddoc.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseView

IMPLEMENT_DYNCREATE(CBaseView, CFrameWnd)

CBaseView::CBaseView()
{
	pAdo = new CMyADO;
}

CBaseView::~CBaseView()
{
	if (pAdo !=NULL){ 
		delete pAdo;
	}
}


BEGIN_MESSAGE_MAP(CBaseView, CFrameWnd)
//BEGIN_MESSAGE_MAP(CBaseView, CCmdTarget)
	//{{AFX_MSG_MAP(CBaseView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CBaseView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	pAdo->Init();
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseView message handlers
BOOL CBaseView::OnCreateClient(LPCREATESTRUCT lpcs,CCreateContext* pContext)
{
	

	m_pSplitter=new CMDSplitter;
	/* 
	//можно поизмываться над настроками...
	WritePrivateProfileString("SimplSubs","SubPlacment","12","c:\\tttt.ini");
	int yyy = GetPrivateProfileInt("SimplSubs","SubPlacment",0,"c:\\tttt.ini");
	*/
	int iPanePos = pAdo->m_Settings.ShowSubPanePos;
	switch(iPanePos)
	{
	case 1: // снизу
	case 3: // сверху
		{
			//BOOL CreateStatic( CWnd* pParentWnd, int nRows, int nCols
			m_pSplitter->CreateStatic(this,2,1);
			m_pSplitter->SetRowInfo(0,400,20);
			m_pSplitter->SetRowInfo(1,400,20);
			
			// void SetRowInfo( int row, int cyIdeal, int cyMin );
			// virtual BOOL CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext );
			if (iPanePos == 1){ 
				m_pSplitter->CreateView(0,0,RUNTIME_CLASS(CMyView),CSize(250,100),pContext);
				m_pSplitter->CreateView(1,0,RUNTIME_CLASS(CSubsViewMain),CSize(200,100),pContext);
			} else {
				m_pSplitter->CreateView(1,0,RUNTIME_CLASS(CMyView),CSize(250,100),pContext);
				m_pSplitter->CreateView(0,0,RUNTIME_CLASS(CSubsViewMain),CSize(200,100),pContext);

			}
		}
		break;
	case 2: // слева....
	default: //0 = справа
		{
			// справа
			m_pSplitter->CreateStatic(this,1,2);
			m_pSplitter->SetColumnInfo(0,400,20);
			m_pSplitter->SetColumnInfo(1,100,20);
			
			// void SetRowInfo( int row, int cyIdeal, int cyMin );
			// virtual BOOL CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext );
			if (iPanePos ==0){ 
				m_pSplitter->CreateView(0,0,RUNTIME_CLASS(CMyView),CSize(250,20),pContext);
				m_pSplitter->CreateView(0,1,RUNTIME_CLASS(CSubsViewMain),CSize(200,20),pContext);
			} else {
				m_pSplitter->CreateView(0,1,RUNTIME_CLASS(CMyView),CSize(250,20),pContext);
				m_pSplitter->CreateView(0,0,RUNTIME_CLASS(CSubsViewMain),CSize(200,20),pContext);

			}

		}
	    break;
	}
	/*
		m_pSplitter->CreateStatic(this,1,2);
		m_pSplitter->SetColumnInfo(0,400,20);
		m_pSplitter->SetColumnInfo(1,100,20);
		
		// void SetRowInfo( int row, int cyIdeal, int cyMin );
		// virtual BOOL CreateView( int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext );
		m_pSplitter->CreateView(0,0,RUNTIME_CLASS(CMyView),CSize(250,20),pContext);
		m_pSplitter->CreateView(0,1,RUNTIME_CLASS(CSubsViewMain),CSize(200,20),pContext);
	*/

	return TRUE;
}


BOOL CBaseView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL ret=CFrameWnd::PreCreateWindow(cs);
	cs.dwExStyle&=~WS_EX_CLIENTEDGE;	
	cs.style&=~WS_BORDER;
	cs.style&=~WS_OVERLAPPED;
	return ret;
}
