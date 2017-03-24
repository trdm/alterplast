// PanelView.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "PanelView.h"
#include "PanelManager.h"
#include "resource.h"
#include "PluginManager.h"
#include "PluginPageDoc.h"
#include "PluginPageView.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelView

IMPLEMENT_DYNCREATE(CPanelView, CView)

CPanelView::CPanelView()
{
	m_pActiveWnd=NULL;
	m_pTab=NULL;
}

CPanelView::~CPanelView()
{
}


BEGIN_MESSAGE_MAP(CPanelView, CView)
	//{{AFX_MSG_MAP(CPanelView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE,99,OnSwitchTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelView drawing

void CPanelView::OnDraw(CDC* pDC){}
HICON GetIcon(CPluginManager::Info* pInfo);

void CPanelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	UINT id=CTemplate7::GetDocumentTemplateID(m_pDocument)-IDR_PANEL_MENU_1;
	if(id<16)
	{
		ModifyStyleEx(WS_EX_CLIENTEDGE,0);
		SPanelInfo* pInfo=CPanelManager::m_panels+id;
		pInfo->pView=this;

		HIMAGELIST hImgList=NULL;
		int iTabCnt=pInfo->tabs.GetSize();
		if(iTabCnt>1)
		{
			m_pTab=new CTabCtrl;
			m_pTab->Create(WS_CHILD|WS_VISIBLE|TCS_FOCUSNEVER|TCS_TABS|TCS_BOTTOM,CRect(0,0,0,0),this,99);
			m_pTab->SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT));
			hImgList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR32,0,1);
			m_pTab->SendMessage(TCM_SETIMAGELIST,0,(LPARAM)hImgList);
		}
		for(int i=0,page=0;i<iTabCnt;i++)
		{
			CString name=pInfo->tabs[i];
			CWnd* pCreatedWnd=NULL;
			HICON hIcon=NULL;
			CPluginManager::Info* pPlugin=CPluginManager::GetPM()->GetPluginByName(name);
			if(pPlugin)
			{
				CPluginPageDoc* pDoc=new CPluginPageDoc;
				pDoc->m_pPlugin=pPlugin->pPlugin;
				CCreateContext ctx;
				ctx.m_pCurrentDoc=pDoc;

				CPluginPageView* pView=new CPluginPageView;
				pView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
					CRect(0,0,0,0), this, 100+i,&ctx);
				if(pView->m_hWnd)
				{
					pView->OnInitialUpdate();
					pCreatedWnd=pView;
					hIcon=::GetIcon(pPlugin);
				}
			}
			else
			{
				STabInfo* pTab;
				CPanelManager::m_rtcPanels.Lookup(name,(void*&)pTab);
				if(pTab && pTab->pWndClass)
				{
					pCreatedWnd=(CWnd*)pTab->pWndClass->CreateObject();
					pCreatedWnd->Create(NULL,NULL,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,100);
					hIcon=pTab->hIcon;
					if(!pCreatedWnd->m_hWnd)
						pCreatedWnd=NULL;
				}
			}
			if(pCreatedWnd)
			{
				pCreatedWnd->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
				m_tabs.Add(pCreatedWnd);
				if(m_pTab)
				{
					int iIconNum=-1;
					if(hIcon)
						iIconNum=ImageList_AddIcon(hImgList,hIcon);
					m_pTab->InsertItem(page++,name,iIconNum);
				}
			}
		}
		m_pActiveWnd=(CWnd*)m_tabs[0];
		m_pActiveWnd->ShowWindow(SW_SHOW);
	}
	RepositionWnd();
}

void CPanelView::RepositionWnd()
{
	if(m_pActiveWnd)
	{
		CRect rc;
		GetClientRect(rc);
		if(m_pTab)
		{
			m_pTab->MoveWindow(rc);
			m_pTab->AdjustRect(FALSE,rc);
		}
		m_pActiveWnd->MoveWindow(rc);
		
	}
}

void CPanelView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	RepositionWnd();
}

void CPanelView::OnSwitchTab(NMHDR *phdr, LRESULT *pRes)
{
	*pRes=1;
	m_pActiveWnd->ShowWindow(SW_HIDE);
	m_pActiveWnd=(CWnd*)m_tabs[m_pTab->GetCurSel()];
	m_pActiveWnd->ShowWindow(SW_SHOW);
	m_pActiveWnd->SetFocus();
	RepositionWnd();
}

BOOL CPanelView::PreTranslateMessage(MSG* pMsg) 
{
	if(m_pTab)
	{
		if(pMsg->message==WM_KEYDOWN)
		{
			if(pMsg->wParam==VK_PRIOR || pMsg->wParam==VK_NEXT)
			{
				if(GetKeyState(VK_CONTROL) & 0x8000)
				{
					int nPage=m_pTab->GetCurSel();
					int Pages=m_tabs.GetSize();
					nPage=(Pages+nPage+(pMsg->wParam==VK_NEXT?1:-1))%Pages;
					m_pTab->SetCurSel(nPage);
					LRESULT res;
					OnSwitchTab(NULL,&res);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CPanelView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	if(m_pActiveWnd)
		m_pActiveWnd->SetFocus();
}
