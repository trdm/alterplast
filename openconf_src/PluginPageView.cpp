// PluginPageView.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "PluginPageView.h"
#include "PluginPagedoc.h"
#include "ocview.h"

/////////////////////////////////////////////////////////////////////////////
// CPluginPageView

IMPLEMENT_DYNCREATE(CPluginPageView, CView)

CPluginPageView::CPluginPageView()
{
	m_pActiveObject=NULL;
}

CPluginPageView::~CPluginPageView()
{
}


BEGIN_MESSAGE_MAP(CPluginPageView, CView)
	//{{AFX_MSG_MAP(CPluginPageView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginPageView drawing

void CPluginPageView::OnDraw(CDC* pDC)
{
}

void CPluginPageView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	IOleControlPtr pCtrl=GetDoc()->m_pPlugin;
	if(pCtrl==NULL)
		return;
	COleControlSiteWrap* pSite = NULL;
	InitControlContainer();
	TRY
	{
		pSite = (COleControlSiteWrap*)afxOccManager->CreateSite(m_pCtrlCont);
	}
	END_TRY
	CFakeWnd* pWnd=(CFakeWnd*)new CWnd;
	pSite->m_pWndCtrl=pWnd;
	pWnd->SetSite(pSite);
	pCtrl->QueryInterface(IID_IOleObject,(void**)&pSite->m_pObject);
	pSite->m_pObject->SetClientSite(&pSite->m_xOleClientSite);
	pSite->m_pObject->QueryInterface(IID_IOleInPlaceObject,(LPVOID*)&pSite->m_pInPlaceObject);
	GetClientRect(pSite->m_rect);
	pSite->SetExtentEx();
	pSite->DoVerb(OLEIVERB_INPLACEACTIVATE);
	pSite->AttachWnd();
	m_pCtrlCont->m_siteMap.SetAt(pSite->m_hWnd, pSite);
	m_pActiveObject=pSite;
	//pWnd->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	OnSize(0,0,0);
}

void CPluginPageView::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	GetClientRect(rc);
	if(m_pActiveObject)
	{
		if(m_pActiveObject->m_pInPlaceObject && rc.bottom && rc.right)
		{
			//CRect rr(-2,-2,rc.right+2,rc.bottom+2);
			m_pActiveObject->m_rect=rc;//rr;
			try{
				m_pActiveObject->m_pInPlaceObject->SetObjectRects(rc,rc);
			}
			catch(...)
			{
			}
			
		}
	}
}

BOOL CPluginPageView::PreTranslateMessage(MSG* pMsg) 
{
	CWnd* pWnd=GetParent();
	while(pWnd)
	{
		if(pWnd->PreTranslateMessage(pMsg))
			return TRUE;
		if(pWnd->IsFrameWnd())
			break;
		pWnd=pWnd->GetParent();
	}
	if(pMainApp->m_pMainWnd->PreTranslateMessage(pMsg))
		return TRUE;
	return IsDialogMessage(pMsg);
}

void CPluginPageView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	if(m_pActiveObject)
	{
		m_pActiveObject->SetFocus();
	}
}
