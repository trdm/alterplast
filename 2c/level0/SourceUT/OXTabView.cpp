// ==========================================================================
// 							   Class Implementation : 
//								COXTabViewContainer 
// ==========================================================================

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
                          
// //////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "OXTabView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COXTabViewContainer* PASCAL GetParentTabViewContainer(const CWnd* pWnd,
													  const BOOL bOnlyActive/*=TRUE*/)
{
#ifdef _AFXDLL
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#endif

	ASSERT(pWnd!=NULL);
	HWND hWndParent=::GetParent(pWnd->GetSafeHwnd());
	if(hWndParent==NULL)
		return NULL;

	COXTabViewContainer* pContainer=
		(COXTabViewContainer*)CWnd::FromHandlePermanent(hWndParent);
	if(pContainer!=NULL)
	{
		ASSERT(::IsWindow(pContainer->m_hWnd));
		if(::IsWindow(pContainer->m_hWnd))
		{
			if(::GetWindowLong(pContainer->m_hWnd,GWL_USERDATA)==
				ID_TABVIEWCONTAINER_SIGN)
			{
				if(!bOnlyActive || pContainer->IsActivePage(pWnd))
					return pContainer;
			}
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXTabViewContainer
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COXTabViewContainer,CWnd)

COXTabViewContainer::COXTabViewContainer()
{
	EmptyRects();
	m_nLastTabBtnAreaWidth=-1;
	m_nTabBtnAreaOrigin=0;

	m_arrUniqueIDs.Add(1);
	m_nActivePageIndex=-1;

	m_nPressedScrlBtn=NONE;
	m_bIsScrlBtnPressed=FALSE;
	m_nScrollPageTimer=-1;

	m_bIsSplitterPressed=FALSE;

	m_nLastTabBtnAreaWidth=ID_INITABBTNAREAWIDTH;

	if((HFONT)m_fontTabBtnText==NULL)
		m_fontTabBtnText.CreatePointFont(80,_T("MS Sans Serif"));
	if((HFONT)m_fontActiveTabBtnText==NULL && (HFONT)m_fontTabBtnText!=NULL)
	{
		LOGFONT lf;
		if(m_fontTabBtnText.GetLogFont(&lf)!=0)
		{
			lf.lfWeight=FW_BOLD;
			m_fontActiveTabBtnText.CreateFontIndirect(&lf);
		}
	}

	ASSERT((HFONT)m_fontTabBtnText!=NULL && (HFONT)m_fontActiveTabBtnText!=NULL);
}

COXTabViewContainer::~COXTabViewContainer()
{
}

BEGIN_MESSAGE_MAP(COXTabViewContainer, CWnd)
	//{{AFX_MSG_MAP(COXTabViewContainer)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CANCELMODE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL COXTabViewContainer::Create(CWnd* pParentWnd, CRect rect/*=CRect(0,0,0,0)*/,
								 DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
								 UINT nID/*=AFX_IDW_PANE_FIRST*/)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(nID != 0);

	// the Windows scroll bar styles bits turn on the smart scrollbars
	DWORD dwCreateStyle=dwStyle&~(WS_HSCROLL|WS_VSCROLL);
	dwCreateStyle&=~WS_BORDER;

	dwCreateStyle|=WS_CHILD;

	// define our own window class 
	WNDCLASS wndClass;
	wndClass.style=CS_DBLCLKS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=0; 
    wndClass.hCursor=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1); 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("TabViewContainer");
	
	if(!AfxRegisterClass(&wndClass))
		return FALSE;

	if (!CreateEx(WS_EX_CLIENTEDGE,wndClass.lpszClassName,NULL,
		dwCreateStyle,rect.left,rect.top,rect.Width(),rect.Height(),
		pParentWnd->m_hWnd,(HMENU)nID,NULL))
	{
		return FALSE;       // create invisible
	}

	// remove WS_EX_CLIENTEDGE style from parent window
	pParentWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);

	// sign 
	::SetWindowLong(GetSafeHwnd(),GWL_USERDATA,ID_TABVIEWCONTAINER_SIGN);

	SetScrollStyle(0,TRUE);

	CalcLayout();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COXTabViewContainer command routing

BOOL COXTabViewContainer::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (CWnd::OnCommand(wParam,lParam))
		return TRUE;

	// route commands from the container to the parent frame window
	if(GetParentFrame()!=NULL)
		return GetParentFrame()->SendMessage(WM_COMMAND,wParam,lParam);
	else
		return FALSE;
}

BOOL COXTabViewContainer::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (CWnd::OnNotify(wParam,lParam,pResult))
		return TRUE;

	// route commands from the container to the parent frame window

	if(GetParentFrame()!=NULL)
	{
		*pResult=GetParentFrame()->SendMessage(WM_NOTIFY,wParam,lParam);
		return TRUE;
	}
	else
	{
		*pResult=0;
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////

void COXTabViewContainer::OnDestroy() 
{
	if(m_nScrollPageTimer!=-1)
	{
		KillTimer(m_nScrollPageTimer);
		m_nScrollPageTimer=-1;
	}

	CWnd::OnDestroy();
}


void COXTabViewContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType!=SIZE_MINIMIZED && cx>0 && cy>0)
	{
		CalcLayout();

		UpdateScrollSizes();

		CWnd* pWnd=GetActivePage();
		if(pWnd!=NULL)
		{
			ASSERT(::IsWindow(pWnd->m_hWnd));
			if(::IsWindow(pWnd->m_hWnd))
				pWnd->MoveWindow(m_rectPage);
		}

		RedrawContainer();
	}

}


void COXTabViewContainer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CWnd* pWnd=GetActivePage();
	if(pWnd!=NULL)
	{
		CRect rect;
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		if(m_rectPage!=rect && !rect.IsRectEmpty())
		{
			CRgn rgnInTheory;
			CRgn rgnInReality;
			if(rgnInTheory.CreateRectRgnIndirect(m_rectPage) &&
				rgnInReality.CreateRectRgnIndirect(rect))
			{
				if(rgnInTheory.CombineRgn(&rgnInTheory,&rgnInReality,
					RGN_DIFF)!=ERROR)
				{
					CBrush* pBrush=NULL;
					CBrush brush;
					HBRUSH hBrush=(HBRUSH)::GetClassLong(pWnd->m_hWnd,
						GCL_HBRBACKGROUND);
					if(hBrush==NULL)
					{
						if(brush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
							pBrush=&brush;
						else
							pBrush=dc.GetCurrentBrush();
					}
					else
					{
						pBrush=CBrush::FromHandle(hBrush);
					}
					
					if(pBrush!=NULL)
						dc.FillRgn(&rgnInTheory,pBrush);
				}
			}
		}
	}

	DrawScrollButtons(&dc);
	DrawSplitter(&dc);
	DrawSizeBar(&dc);
	DrawTabBtnArea(&dc);

	// Do not call CWnd::OnPaint() for painting messages
}


void COXTabViewContainer::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_nPressedScrlBtn!=NONE)
	{
		int hitTest=HitTest(point);
		// send corresponding messages
		if(((int)m_nPressedScrlBtn==hitTest && !m_bIsScrlBtnPressed) ||
			((int)m_nPressedScrlBtn!=hitTest && m_bIsScrlBtnPressed))
		{
			m_bIsScrlBtnPressed=!m_bIsScrlBtnPressed;
			RedrawScrollButton(m_nPressedScrlBtn);
		}
	}
	else if(m_bIsSplitterPressed)
	{
		CPoint ptMoved=point;
		if(ptMoved.x>m_rectPage.right-ID_MINSCROLLBARWIDTH+ID_SPLITTERWIDTH/2)
			ptMoved.x=m_rectPage.right-ID_MINSCROLLBARWIDTH+ID_SPLITTERWIDTH/2;
		if(ptMoved.x<m_rectScrollToEndBtn.right+ID_SPLITTERWIDTH/2)
			ptMoved.x=m_rectScrollToEndBtn.right+ID_SPLITTERWIDTH/2;

		int nOldSplitterLeft=m_rectSplitter.left;
		m_rectSplitter.left=ptMoved.x-ID_SPLITTERWIDTH/2;
		if(nOldSplitterLeft!=m_rectSplitter.left)
		{
			m_rectSplitter.right=m_rectSplitter.left+ID_SPLITTERWIDTH;
			m_rectScrollBarHorz.left=m_rectSplitter.right;

			m_nLastTabBtnAreaWidth=m_rectSplitter.left-m_rectScrollToEndBtn.right;

			m_rectTabBtnArea.right+=m_rectSplitter.left-nOldSplitterLeft;
			if(m_rectTabBtnArea.right<m_rectTabBtnArea.left)
				m_rectTabBtnArea.right=m_rectTabBtnArea.left;

			if(::IsWindow(m_scrlBarHorz.GetSafeHwnd()))
				m_scrlBarHorz.MoveWindow(m_rectScrollBarHorz);
			RedrawSplitter();

			if(nOldSplitterLeft<m_rectSplitter.left)
			{	
				CRect rect=m_rectSplitter;
				rect.left=nOldSplitterLeft;
				rect.right=m_rectSplitter.left;
				RedrawWindow(rect);

				if(m_nTabBtnAreaOrigin<0)
				{
					ASSERT(GetPageCount()==m_arrTabBtnRects.GetSize());
					rect=m_arrTabBtnRects[GetPageCount()-1];
					rect+=m_rectTabBtnArea.TopLeft();
					if(rect.right+m_nTabBtnAreaOrigin<=m_rectTabBtnArea.right)
					{
						m_nTabBtnAreaOrigin+=
							m_rectSplitter.left-nOldSplitterLeft;
						m_nTabBtnAreaOrigin=
							m_nTabBtnAreaOrigin>0 ? 0 : m_nTabBtnAreaOrigin;
						RedrawTabBtnArea();
					}
				}

			}		
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


void COXTabViewContainer::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);

	int hitTest=HitTest(point);
	if(hitTest>=0 && hitTest<GetPageCount() && hitTest!=GetActivePageIndex())
		SetActivePageIndex(hitTest);
	else
		StartTracking(point);
}


void COXTabViewContainer::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	StopTracking(point);

	CWnd::OnLButtonUp(nFlags,point);
}


void COXTabViewContainer::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if((int)nIDEvent==m_nScrollPageTimer)
	{
		if(m_nPressedScrlBtn!=NONE && m_bIsScrlBtnPressed)
			ScrollPage(m_nPressedScrlBtn);
	}

	CWnd::OnTimer(nIDEvent);
}


BOOL COXTabViewContainer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	int hitTest=HitTest(point);
	if(m_bIsSplitterPressed || hitTest==SPLITTER)
	{
		HCURSOR hCursor=AfxGetApp()->
			LoadCursor(MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
		if(hCursor==NULL)
			hCursor=::LoadCursor(NULL,IDC_SIZEWE);
		SetCursor(hCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL COXTabViewContainer::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	if(GetPageCount()==0)
	{
		CRect rect;
		pDC->GetClipBox(rect);
		pDC->FillSolidRect(rect,::GetSysColor(COLOR_BTNFACE));
	} 
	
	return TRUE;
}


void COXTabViewContainer::OnCancelMode() 
{
	CWnd::OnCancelMode();
	
	// TODO: Add your message handler code here

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	StopTracking(point);
}


void COXTabViewContainer::OnHScroll(UINT nSBCode, UINT nPos, 
									CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	ASSERT(pScrollBar==GetHorzScrollBar());
	
	CWnd* pWnd=GetActivePage();
	ASSERT(pWnd!=NULL);
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		pWnd->SendMessage(WM_HSCROLL,MAKEWPARAM(nSBCode,nPos),
			(LPARAM)pScrollBar->m_hWnd);
	}
}

void COXTabViewContainer::OnVScroll(UINT nSBCode, UINT nPos, 
									CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	ASSERT(pScrollBar==GetVertScrollBar());
	
	CWnd* pWnd=GetActivePage();
	ASSERT(pWnd!=NULL);
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		pWnd->SendMessage(WM_VSCROLL,MAKEWPARAM(nSBCode,nPos),
			(LPARAM)pScrollBar->m_hWnd);
	}
}

void COXTabViewContainer::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	UNREFERENCED_PARAMETER(uFlags);
	UNREFERENCED_PARAMETER(lpszSection);

	CalcLayout();

	UpdateScrollSizes();

	CWnd* pWnd=GetActivePage();
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		if(::IsWindow(pWnd->m_hWnd))
			pWnd->MoveWindow(m_rectPage);
	}

	RedrawContainer();

	CWnd::OnSettingChange(uFlags, lpszSection);
}


//////////////////////////////////////////////////////////////////

BOOL COXTabViewContainer::SaveState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp=AfxGetApp();
	if(pApp==NULL)
		return FALSE;

	BOOL bResult=TRUE;
	bResult&=pApp->WriteProfileInt(lpszProfileName,
		_T("TabButtonsAreaWidth"),m_nLastTabBtnAreaWidth);

	return bResult;
}

BOOL COXTabViewContainer::LoadState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp=AfxGetApp();
	if(pApp==NULL)
		return FALSE;

	m_nLastTabBtnAreaWidth=pApp->GetProfileInt(lpszProfileName,
		_T("TabButtonsAreaWidth"),m_nLastTabBtnAreaWidth);

	CalcLayout();
	UpdateScrollSizes();
	RedrawContainer();

	return TRUE;
}


BOOL COXTabViewContainer::InsertPage(const int nIndex, 
									 CRuntimeClass* pClass, 
									 CCreateContext* pContext,
									 LPCTSTR lpszTitle/*=NULL*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<=GetPageCount());
	ASSERT(pClass!=NULL);
	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pClass,sizeof(CRuntimeClass),FALSE));

	if(!(nIndex>=0 && nIndex<=GetPageCount()) || pClass==NULL)
		return FALSE;

	BOOL bSendInitialUpdate=FALSE;

	CCreateContext context;
	if(pContext==NULL)
	{
		// if no context specified, generate one from the currently active
		// view if possible
		CView* pOldView=(CView*)GetActivePage();
		if(pOldView!=NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame==NULL);
			context.m_pLastView=pOldView;
			context.m_pCurrentDoc=pOldView->GetDocument();
			if(context.m_pCurrentDoc!=NULL)
			{
				context.m_pNewDocTemplate=context.m_pCurrentDoc->
					GetDocTemplate();
			}
			bSendInitialUpdate=TRUE;
		}
		pContext=&context;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd=(CWnd*)pClass->CreateObject();
		if(pWnd==NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE(_T("COXTabViewContainer::InsertPage: Out of memory inserting new page\n"));
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd,pWnd);
	ASSERT(pWnd->m_hWnd==NULL);       // not yet created

	DWORD dwStyle=AFX_WS_DEFAULT_VIEW;
	if(afxData.bWin4)
		dwStyle&=~WS_BORDER;

	DWORD dwID=GetUniqueId();

	// Create with the right size
	if(!pWnd->Create(NULL,NULL,dwStyle,m_rectPage,this,dwID,pContext))
	{
		TRACE(_T("COXTabViewContainer::InsertPage: couldn't create new page\n"));
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	if(InsertPage(nIndex,pWnd,lpszTitle))
	{
		CWnd* pWnd=GetPage(nIndex);
		ASSERT(pWnd!=NULL);
		ASSERT(::IsWindow(pWnd->m_hWnd));
		if(bSendInitialUpdate && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// send initial notification message
			pWnd->SendMessage(WM_INITIALUPDATE);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL COXTabViewContainer::InsertPage(const int nIndex, CWnd* pWnd, 
									 LPCTSTR lpszTitle/*=NULL*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<=GetPageCount());
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if(!(nIndex>=0 && nIndex<=GetPageCount()) || pWnd==NULL ||
		!::IsWindow(pWnd->m_hWnd))
		return FALSE;

	PAGEINFO pi;
	pi.pWnd=pWnd;
	if(lpszTitle==NULL)
	{
		pWnd->GetWindowText(pi.sTitle);
		if(pi.sTitle.IsEmpty())
			pi.sTitle.Format(_T("%d"),nIndex);
	}
	else
		pi.sTitle=lpszTitle;

	m_arrPages.InsertAt(nIndex,pi);
	
	CalcTabBtnRects();

	SetActivePageIndex(nIndex);

	RedrawContainer();
	
	return TRUE;
}

BOOL COXTabViewContainer::DeletePage(const int nIndex, 
									 const BOOL bDestroy/*=TRUE*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<GetPageCount());

	if(nIndex>=0 && nIndex<GetPageCount())
	{
		// if active page is being deleted - activate next
		int nActivePage=GetActivePageIndex();
		if(nActivePage==nIndex)
		{
			if(nActivePage==GetPageCount()-1)
				SetActivePageIndex(0);
			else
				SetActivePageIndex(GetPageCount()>1 ? nActivePage+1 : 0);
		}
		CWnd* pWnd=GetPage(nIndex);
		ASSERT(pWnd!=NULL);
		m_arrUniqueIDs.Add(pWnd->GetDlgCtrlID());
		if(bDestroy && ::IsWindow(pWnd->m_hWnd))
			VERIFY(pWnd->DestroyWindow());

		m_arrPages.RemoveAt(nIndex);

		nActivePage=GetActivePageIndex();
		if(nActivePage>=nIndex)
			m_nActivePageIndex--;

		CalcTabBtnRects();

		RedrawContainer();
	
		return TRUE;
	}

	return FALSE;
}

BOOL COXTabViewContainer::SetActivePageIndex(const int nIndex)
{
	if(nIndex==m_nActivePageIndex)
		return TRUE;

	CWnd* pWndOld=GetActivePage();
	if(pWndOld!=NULL)
	{
		ASSERT(::IsWindow(pWndOld->m_hWnd));
		if(::IsWindow(pWndOld->m_hWnd))
		{
			UpdateScrollInfo();

/*			// allow active view to preserve focus before taking it away
			if(pWndOld->IsKindOf(RUNTIME_CLASS(CView)))
			{
				ASSERT_VALID(pWnd);
				CFrameWnd* pFrameWnd=GetParentFrame();
				ASSERT_VALID(pFrameWnd);
				((CView*)pWnd)->OnActivateFrame(WA_INACTIVE,pFrameWnd);
			}
*/
			pWndOld->ShowWindow(SW_HIDE);
			PAGEINFO pi=m_arrPages[GetActivePageIndex()];
			m_nActivePageIndex=-1;
			UINT nBar=(pi.bHasScrollHorz&pi.bHasScrollVert ? SB_BOTH : 
				(pi.bHasScrollHorz ? SB_HORZ : 
				(pi.bHasScrollVert ? SB_VERT : 0)));
			if(nBar!=0)
				pWndOld->ShowScrollBar(nBar,TRUE);
		}
	}

	m_nActivePageIndex=nIndex;
	if(m_nActivePageIndex>=0 && m_nActivePageIndex<GetPageCount())
	{
		EnsureTabBtnVisible(m_nActivePageIndex);

		CWnd* pWnd=GetPage(m_nActivePageIndex);
		if(pWnd!=NULL)
		{
			ASSERT(::IsWindow(pWnd->m_hWnd));
			if(::IsWindow(pWnd->m_hWnd))
			{
				IniScrollInfo();

				pWnd->ShowWindow(SW_SHOW);
				CRect rect;
				pWnd->GetWindowRect(rect);
				if(rect.Width()!=m_rectPage.Width() || 
					rect.Height()!=m_rectPage.Height())
					pWnd->MoveWindow(m_rectPage);
				else
					pWnd->SendMessage(WM_SIZE,SIZE_RESTORED,
						MAKELPARAM(rect.Width(),rect.Height()));

				// set the focus to the page
				CFrameWnd* pFrameWnd=(CFrameWnd*)GetParent();
				ASSERT(pFrameWnd!=NULL);
				if(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
				{
					if(pWnd->IsKindOf(RUNTIME_CLASS(CView)))
					{
						pFrameWnd->SetActiveView((CView*)pWnd);
					}
					else
					{
						if(pWndOld!=NULL && 
							pWndOld->IsKindOf(RUNTIME_CLASS(CView)))
						{
							pFrameWnd->SetActiveView(NULL);
						}
						pWnd->SetFocus();
					}
				}
				else
				{
					pWnd->SetFocus();
				}

				RedrawTabBtnArea();
			}
		}
	}
	else
		return FALSE;

	return TRUE;
}

void COXTabViewContainer::SetScrollStyle(const DWORD dwScrollStyle, 
										 const BOOL bForceToRebuild/*=FALSE*/) 
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(!bForceToRebuild && 
		m_dwScrollStyle==(dwScrollStyle&(WS_VSCROLL|WS_HSCROLL)))
		return; 

	m_dwScrollStyle=(dwScrollStyle&(WS_VSCROLL|WS_HSCROLL));

	if(::IsWindow(m_scrlBarHorz.GetSafeHwnd()))
		m_scrlBarHorz.DestroyWindow();
	if(::IsWindow(m_scrlBarVert.GetSafeHwnd()))
		m_scrlBarVert.DestroyWindow();

	VERIFY(m_scrlBarHorz.Create(SBS_HORZ|WS_CHILD|WS_VISIBLE|
		((dwScrollStyle&WS_HSCROLL) ? 0 : WS_DISABLED),
		m_rectScrollBarHorz,this,AFX_IDW_HSCROLL_FIRST));
	VERIFY(m_scrlBarVert.Create(SBS_VERT|WS_CHILD|WS_VISIBLE|
		((dwScrollStyle&WS_VSCROLL) ? 0 : WS_DISABLED),
		m_rectScrollBarVert,this,AFX_IDW_VSCROLL_FIRST));
}

void COXTabViewContainer::CalcLayout()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	EmptyRects();

	CRect rect;
	GetClientRect(rect);

	if(rect.IsRectEmpty())
		return;

	CSize szScrollBtn=CSize(::GetSystemMetrics(SM_CXHSCROLL), 
		::GetSystemMetrics(SM_CYHSCROLL));

	// scroll buttons
	//
	m_rectScrollToStartBtn=CRect(rect.left,rect.bottom-szScrollBtn.cy,
		rect.left+szScrollBtn.cx,rect.bottom);

	m_rectScrollBackwardBtn=m_rectScrollToStartBtn;
	m_rectScrollBackwardBtn.OffsetRect(m_rectScrollToStartBtn.Width(),0);

	m_rectScrollForwardBtn=m_rectScrollBackwardBtn;
	m_rectScrollForwardBtn.OffsetRect(m_rectScrollBackwardBtn.Width(),0);

	m_rectScrollToEndBtn=m_rectScrollForwardBtn;
	m_rectScrollToEndBtn.OffsetRect(m_rectScrollForwardBtn.Width(),0);
	//
	/////////////////////////////////////////////

	// page rect
	m_rectPage=rect;
	m_rectPage.right-=szScrollBtn.cx;
	m_rectPage.bottom-=szScrollBtn.cy;

	int nTabBtnAreaWidth=m_nLastTabBtnAreaWidth;
	if(m_rectPage.right<m_rectScrollToEndBtn.right+
		nTabBtnAreaWidth+ID_MINSCROLLBARWIDTH)
	{
		nTabBtnAreaWidth=m_rectPage.right-
			m_rectScrollToEndBtn.right-ID_MINSCROLLBARWIDTH;
	}

	// tab buttons area
	m_rectTabBtnArea=m_rectScrollToStartBtn;
	m_rectTabBtnArea.left=m_rectScrollToEndBtn.right;
	m_rectTabBtnArea.right=m_rectTabBtnArea.left+nTabBtnAreaWidth;
	if(m_rectTabBtnArea.right<m_rectTabBtnArea.left)
		m_rectTabBtnArea.right=m_rectTabBtnArea.left;

	// splitter rect
	m_rectSplitter=m_rectScrollToStartBtn;
	m_rectSplitter.left=m_rectTabBtnArea.right;
	m_rectSplitter.right=m_rectSplitter.left+ID_SPLITTERWIDTH;

	// horz scroll bar
	m_rectScrollBarHorz=m_rectScrollToStartBtn;
	m_rectScrollBarHorz.left=m_rectSplitter.right;
	m_rectScrollBarHorz.right=m_rectPage.right;

	// vert scroll bar
	m_rectScrollBarVert=m_rectPage;
	m_rectScrollBarVert.left=m_rectPage.right;
	m_rectScrollBarVert.right=rect.right;

	// size bar
	m_rectSizeBar=rect;
	m_rectSizeBar.top=m_rectScrollBarVert.bottom;
	m_rectSizeBar.left=m_rectScrollBarHorz.right;

}

void COXTabViewContainer::CalcTabBtnRects()
{
	m_arrTabBtnRects.RemoveAll();

	CString sTitle;
	int nLeftSide=0;
	CRect rect;

	CClientDC dc(this);
	ASSERT((HFONT)m_fontActiveTabBtnText!=NULL);
	if((HFONT)m_fontActiveTabBtnText!=NULL)
		dc.SelectObject(&m_fontActiveTabBtnText);
	
	for(int nIndex=0; nIndex<GetPageCount(); nIndex++)
	{
		rect=m_rectTabBtnArea;
		rect-=rect.TopLeft();
		rect.right=0xffff;

		sTitle=GetPageTitle(nIndex);

		dc.DrawText(sTitle,rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		rect.OffsetRect(nLeftSide,0);
		rect.right+=2*ID_TABBTNOVERLAPSIZE+2*ID_TABBTNGAPSIZE;
		
		m_arrTabBtnRects.Add(rect);

		nLeftSide=rect.right-ID_TABBTNOVERLAPSIZE;
	}
}

void COXTabViewContainer::EmptyRects()
{
	m_rectScrollForwardBtn.SetRectEmpty();
	m_rectScrollBackwardBtn.SetRectEmpty();
	m_rectScrollToStartBtn.SetRectEmpty();
	m_rectScrollToEndBtn.SetRectEmpty();

	m_rectTabBtnArea.SetRectEmpty();

	m_rectScrollBarHorz.SetRectEmpty();
	m_rectScrollBarVert.SetRectEmpty();

	m_rectSplitter.SetRectEmpty();

	m_rectSizeBar.SetRectEmpty();

	m_rectPage.SetRectEmpty();
}

void COXTabViewContainer::UpdateScrollSizes()
{
	if(::IsWindow(m_scrlBarHorz.GetSafeHwnd()))
		m_scrlBarHorz.MoveWindow(m_rectScrollBarHorz);
	if(::IsWindow(m_scrlBarVert.GetSafeHwnd()))
		m_scrlBarVert.MoveWindow(m_rectScrollBarVert);
}


void COXTabViewContainer::IniScrollInfo()
{
	CWnd* pWnd=GetActivePage();
	if(pWnd==NULL)
		return;

	PAGEINFO& pi=m_arrPages[GetActivePageIndex()];

	pi.bHasScrollHorz=((pWnd->GetStyle()&WS_HSCROLL)==WS_HSCROLL);
	pi.bHasScrollVert=((pWnd->GetStyle()&WS_VSCROLL)==WS_VSCROLL);

	if(pi.bHasScrollHorz)
	{
		pi.scrlInfoHorz.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_HORZ,&pi.scrlInfoHorz);
		pi.scrlInfoHorz.nMax-=(pi.bHasScrollVert ? 
			::GetSystemMetrics(SM_CXVSCROLL) : 0);
		if(pi.scrlInfoHorz.nMax==0 || pi.scrlInfoHorz.nPage==0)
			pi.bHasScrollHorz=FALSE;
	}

	if(pi.bHasScrollVert)
	{
		pi.scrlInfoVert.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_VERT,&pi.scrlInfoVert);
		pi.scrlInfoVert.nMax-=(pi.bHasScrollHorz ? 1 : 0);
		if(pi.scrlInfoVert.nMax==0 || pi.scrlInfoVert.nPage==0)
			pi.bHasScrollVert=FALSE;
	}

	DWORD dwStyle=(pi.bHasScrollHorz ? WS_HSCROLL : 0) | 
		(pi.bHasScrollVert ? WS_VSCROLL : 0);
	SetScrollStyle(dwStyle,TRUE);
	pWnd->ShowScrollBar(SB_BOTH,FALSE);
	pWnd->RedrawWindow();

	if(pi.bHasScrollHorz || 
		(pi.scrlInfoHorz.nMax>0 && pi.scrlInfoHorz.nPage>0 && 
		pi.scrlInfoHorz.nPos>0 && pi.scrlInfoHorz.nPos<=pi.scrlInfoHorz.nMax))
	{
		ASSERT(::IsWindow(m_scrlBarHorz.GetSafeHwnd()));
		m_scrlBarHorz.SetScrollInfo(&pi.scrlInfoHorz);
	}
	if(pi.bHasScrollVert || 
		(pi.scrlInfoVert.nMax>0 && pi.scrlInfoVert.nPage>0 && 
		pi.scrlInfoVert.nPos>0 && pi.scrlInfoVert.nPos<=pi.scrlInfoVert.nMax))
	{
		ASSERT(::IsWindow(m_scrlBarVert.GetSafeHwnd()));
		m_scrlBarVert.SetScrollInfo(&pi.scrlInfoVert);
	}

}


static BOOL bUpdatingScrollInfo=FALSE;
static BOOL bUpdatingScrollState=FALSE;

void COXTabViewContainer::UpdateScrollInfo()
{
	if(bUpdatingScrollInfo || bUpdatingScrollState)
		return;

	bUpdatingScrollInfo=TRUE;

	CWnd* pWnd=GetActivePage();
	if(pWnd==NULL || (pWnd->GetStyle()&WS_VISIBLE)!=WS_VISIBLE)
	{
		bUpdatingScrollInfo=FALSE;
		return;
	}

	PAGEINFO& pi=m_arrPages[GetActivePageIndex()];

	if(pi.bHasScrollHorz) 
	{
		pWnd->ModifyStyle(WS_VISIBLE,NULL);
		pWnd->ShowScrollBar(SB_HORZ,TRUE);
		pWnd->ModifyStyle(NULL,WS_HSCROLL,SWP_DRAWFRAME);

		pi.scrlInfoHorz.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_HORZ,&pi.scrlInfoHorz);

		ASSERT(::IsWindow(m_scrlBarHorz.GetSafeHwnd()));
		m_scrlBarHorz.SetScrollInfo(&pi.scrlInfoHorz);

		pWnd->ShowScrollBar(SB_HORZ,FALSE);
		pWnd->ModifyStyle(WS_HSCROLL,NULL,SWP_DRAWFRAME);
		pWnd->ModifyStyle(NULL,WS_VISIBLE);
	}
	else
	{
		pi.scrlInfoHorz.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_HORZ,&pi.scrlInfoHorz);
	}

	if(pi.bHasScrollVert)
	{
		pWnd->ModifyStyle(WS_VISIBLE,NULL);
		pWnd->ShowScrollBar(SB_VERT,TRUE);
		pWnd->ModifyStyle(NULL,WS_VSCROLL,SWP_DRAWFRAME);

		pi.scrlInfoVert.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_VERT,&pi.scrlInfoVert);

		ASSERT(::IsWindow(m_scrlBarVert.GetSafeHwnd()));
		m_scrlBarVert.SetScrollInfo(&pi.scrlInfoVert);

		pWnd->ShowScrollBar(SB_VERT,FALSE);
		pWnd->ModifyStyle(WS_VSCROLL,NULL,SWP_DRAWFRAME);
		pWnd->ModifyStyle(NULL,WS_VISIBLE);
	}
	else
	{
		pi.scrlInfoVert.cbSize=sizeof(SCROLLINFO);
		pWnd->GetScrollInfo(SB_VERT,&pi.scrlInfoVert);
	}

	bUpdatingScrollInfo=FALSE;
}


void COXTabViewContainer::UpdateScrollState()
{
	if(bUpdatingScrollState || bUpdatingScrollInfo)
		return;

	bUpdatingScrollState=TRUE;

	CWnd* pWnd=GetActivePage();
	if(pWnd==NULL || (pWnd->GetStyle()&WS_VISIBLE)!=WS_VISIBLE)
	{
		bUpdatingScrollState=FALSE;
		return;
	}

	PAGEINFO& pi=m_arrPages[GetActivePageIndex()];

	BOOL bHasScrollHorz=((pWnd->GetStyle()&WS_HSCROLL)==WS_HSCROLL);
	BOOL bHasScrollVert=((pWnd->GetStyle()&WS_VSCROLL)==WS_VSCROLL);

	if(bHasScrollHorz || bHasScrollVert)
	{
		if(bHasScrollHorz)
		{
			pi.bHasScrollHorz=bHasScrollHorz;
			pWnd->ModifyStyle(WS_HSCROLL,NULL,SWP_DRAWFRAME);
			pWnd->ShowScrollBar(SB_HORZ,FALSE);
		}
		if(bHasScrollVert)
		{
			pi.bHasScrollVert=bHasScrollVert;
			pWnd->ModifyStyle(WS_VSCROLL,NULL,SWP_DRAWFRAME);
			pWnd->ShowScrollBar(SB_VERT,FALSE);
		}
	}

	pWnd->ModifyStyle(WS_VISIBLE,NULL);
	pWnd->ShowScrollBar(SB_HORZ,TRUE);
	pWnd->ModifyStyle(NULL,WS_HSCROLL,SWP_DRAWFRAME);
	pWnd->ShowScrollBar(SB_VERT,TRUE);
	pWnd->ModifyStyle(NULL,WS_VSCROLL,SWP_DRAWFRAME);

	pi.scrlInfoHorz.cbSize=sizeof(SCROLLINFO);
	pWnd->GetScrollInfo(SB_HORZ,&pi.scrlInfoHorz);
	if(pi.bHasScrollHorz)
	{
		if(pi.scrlInfoHorz.nMax==0 || pi.scrlInfoHorz.nPage==0 || 
			pi.scrlInfoHorz.nMax<(int)pi.scrlInfoHorz.nPage)
			pi.bHasScrollHorz=FALSE;
	}
	else
	{
		if(pi.scrlInfoHorz.nMax>0 && pi.scrlInfoHorz.nPage>0 &&
			pi.scrlInfoHorz.nMax>=(int)pi.scrlInfoHorz.nPage)
			pi.bHasScrollHorz=TRUE;
	}


	pi.scrlInfoVert.cbSize=sizeof(SCROLLINFO);
	pWnd->GetScrollInfo(SB_VERT,&pi.scrlInfoVert);
	if(pi.bHasScrollVert)
	{
		if(pi.scrlInfoVert.nMax==0 || pi.scrlInfoVert.nPage==0 || 
			pi.scrlInfoVert.nMax<(int)pi.scrlInfoVert.nPage)
			pi.bHasScrollVert=FALSE;
	}
	else
	{
		if(pi.scrlInfoVert.nMax>0 && pi.scrlInfoVert.nPage>0 &&
			pi.scrlInfoVert.nMax>=(int)pi.scrlInfoVert.nPage)
			pi.bHasScrollVert=TRUE;
	}

	pWnd->ShowScrollBar(SB_HORZ,FALSE);
	pWnd->ModifyStyle(WS_HSCROLL,NULL,SWP_DRAWFRAME);
	pWnd->ShowScrollBar(SB_VERT,FALSE);
	pWnd->ModifyStyle(WS_VSCROLL,NULL,SWP_DRAWFRAME);
	pWnd->ModifyStyle(NULL,WS_VISIBLE);

	DWORD dwStyle=GetScrollStyle();
	dwStyle=(pi.bHasScrollHorz ? WS_HSCROLL : 0) | 
		(pi.bHasScrollVert ? WS_VSCROLL : 0);
	SetScrollStyle(dwStyle);

	bUpdatingScrollState=FALSE;
}


void COXTabViewContainer::DrawScrollButtons(CDC* pDC)
{
	ASSERT_VALID(pDC);

	// scroll to start button
	DrawButton(pDC,m_rectScrollToStartBtn,SCRLSTARTBTN);
	// scroll backward button
	DrawButton(pDC,m_rectScrollBackwardBtn,SCRLBACKWARDBTN);
	// scroll forward button
	DrawButton(pDC,m_rectScrollForwardBtn,SCRLFORWARDBTN);
	// scroll to end button
	DrawButton(pDC,m_rectScrollToEndBtn,SCRLENDBTN);
}

void COXTabViewContainer::DrawTabBtnArea(CDC* pDC)
{
	ASSERT(m_arrTabBtnRects.GetSize()==GetPageCount());

	pDC->IntersectClipRect(m_rectTabBtnArea);

	pDC->FillSolidRect(m_rectTabBtnArea,::GetSysColor(COLOR_BTNFACE));

	CPen pen(PS_SOLID,1,::GetSysColor(COLOR_WINDOWTEXT));
	CPen* pOldPen=pDC->SelectObject(&pen);
	pDC->MoveTo(m_rectTabBtnArea.left,m_rectTabBtnArea.top);
	pDC->LineTo(m_rectTabBtnArea.right,m_rectTabBtnArea.top);

	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush=pDC->SelectObject(&brush);

	CFont* pOldFont=NULL;
	ASSERT((HFONT)m_fontTabBtnText!=NULL);
	if((HFONT)m_fontTabBtnText!=NULL)
		pOldFont=pDC->SelectObject(&m_fontTabBtnText);

	for(int nIndex=0; nIndex<GetPageCount(); nIndex++)
	{
		if(nIndex!=GetActivePageIndex())
			DrawTabButton(pDC,nIndex);
	}


	if(GetActivePageIndex()!=-1)
	{
		CBrush brushActive;
		brushActive.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
		pDC->SelectObject(&brushActive);

		ASSERT((HFONT)m_fontActiveTabBtnText!=NULL);
		if((HFONT)m_fontActiveTabBtnText!=NULL)
			pDC->SelectObject(&m_fontActiveTabBtnText);

		DrawTabButton(pDC,GetActivePageIndex());
	}

	if(pOldFont!=NULL)
		pDC->SelectObject(pOldFont);

	if(pOldBrush!=NULL)
		pDC->SelectObject(pOldBrush);

	if(pOldPen!=NULL)
		pDC->SelectObject(pOldPen);
}

void COXTabViewContainer::DrawSplitter(CDC* pDC)
{
	ASSERT_VALID(pDC);

	CRect rect=m_rectSplitter;

	pDC->Draw3dRect(rect,::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_WINDOWFRAME));
	rect.DeflateRect(1,1);
	pDC->Draw3dRect(rect,::GetSysColor(COLOR_BTNHILIGHT),
		::GetSysColor(COLOR_BTNSHADOW));
	rect.DeflateRect(1,1);

	// fill the middle
	COLORREF clr=::GetSysColor(COLOR_BTNFACE);
#ifdef _MAC
	// just use white for interior if less than 16 colors
	if(pDC->GetDeviceCaps(NUMCOLORS)<16)
		clr=RGB(0xFF,0xFF,0xFF);
#endif
	pDC->FillSolidRect(rect,clr);
}

void COXTabViewContainer::DrawSizeBar(CDC* pDC)
{
	ASSERT_VALID(pDC);

	CWnd* pParentWnd=GetParent();
	ASSERT(pParentWnd!=NULL);
	ASSERT(::IsWindow(pParentWnd->m_hWnd));
	if(pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) && 
		(pParentWnd->GetStyle()&WS_THICKFRAME)==WS_THICKFRAME &&
		(pParentWnd->GetStyle()&WS_MAXIMIZE)!=WS_MAXIMIZE)
		pDC->DrawFrameControl(m_rectSizeBar,DFC_SCROLL,DFCS_SCROLLSIZEGRIP);
	else
		pDC->FillSolidRect(m_rectSizeBar,::GetSysColor(COLOR_BTNFACE));
}

void COXTabViewContainer::DrawButton(CDC* pDC, CRect rect, 
									 const HITTEST nButtonType) const
{
	ASSERT_VALID(pDC);

	pDC->FillSolidRect(rect,::GetSysColor(COLOR_BTNFACE));

	COLORREF clrTopLeft;
	COLORREF clrBottomRight;
	if(m_nPressedScrlBtn==nButtonType)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);
		if(rect.PtInRect(point))
		{
			clrBottomRight=::GetSysColor(COLOR_BTNFACE);
			clrTopLeft=::GetSysColor(COLOR_BTNFACE);
			rect.OffsetRect(1,1);
		}
		else
		{
			clrTopLeft=::GetSysColor(COLOR_BTNHILIGHT);
			clrBottomRight=::GetSysColor(COLOR_BTNSHADOW);
		}
	}
	else
	{
		clrTopLeft=::GetSysColor(COLOR_BTNHILIGHT);
		clrBottomRight=::GetSysColor(COLOR_BTNSHADOW);
	}

	pDC->Draw3dRect(rect,clrTopLeft,clrBottomRight);


	// draw arrows
	//
	rect.DeflateRect(2,2);
	CRect rectCopy=rect;

	POINT arrPoints[3];
	CRgn rgn;
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_WINDOWTEXT));

	switch(nButtonType)
	{
	// start
	case SCRLSTARTBTN:
		rectCopy.DeflateRect(rectCopy.Width()/4,rectCopy.Height()/6);
		rectCopy.left+=rectCopy.Width()/4-2;
		if(rectCopy.Height()%2==0)
			rectCopy.bottom+=1;
		rectCopy.right=rectCopy.left+2;
		pDC->FillSolidRect(rectCopy,::GetSysColor(COLOR_WINDOWTEXT));
	// backward
	case SCRLBACKWARDBTN:
		rectCopy=rect;
		rectCopy.DeflateRect(rectCopy.Width()/4,rectCopy.Height()/6);
		if(nButtonType==SCRLSTARTBTN)
			rectCopy.left+=rectCopy.Width()/4;
		else
			rectCopy.right-=rectCopy.Width()/4;
		if(rectCopy.Height()%2==0)
			rectCopy.bottom++;
		arrPoints[0].x=rectCopy.right;
		arrPoints[0].y=rectCopy.top-1;
		arrPoints[1].x=rectCopy.right;
		arrPoints[1].y=rectCopy.bottom;
		arrPoints[2].x=rectCopy.left;
		arrPoints[2].y=rectCopy.top+rectCopy.Height()/2;
		rgn.CreatePolygonRgn(arrPoints,3,WINDING);
		pDC->FillRgn(&rgn,&brush);
		break;
	// end
	case SCRLENDBTN:
		rectCopy.DeflateRect(rectCopy.Width()/4,rectCopy.Height()/6);
		rectCopy.right-=rectCopy.Width()/4-2;
		if(rectCopy.Height()%2==0)
			rectCopy.bottom+=1;
		rectCopy.left=rectCopy.right-2;
		pDC->FillSolidRect(rectCopy,::GetSysColor(COLOR_WINDOWTEXT));
	// forward
	case SCRLFORWARDBTN:
		rectCopy=rect;
		rectCopy.DeflateRect(rectCopy.Width()/4,rectCopy.Height()/6);
		if(nButtonType==SCRLENDBTN)
			rectCopy.right-=rectCopy.Width()/4;
		else
			rectCopy.left+=rectCopy.Width()/4;
		if(rectCopy.Height()%2==0)
			rectCopy.bottom++;
		arrPoints[0].x=rectCopy.right;
		arrPoints[0].y=rectCopy.top+rectCopy.Height()/2;
		arrPoints[1].x=rectCopy.left;
		arrPoints[1].y=rectCopy.top-1;
		arrPoints[2].x=rectCopy.left;
		arrPoints[2].y=rectCopy.bottom;
		rgn.CreatePolygonRgn(arrPoints,3,WINDING);
		pDC->FillRgn(&rgn,&brush);
		break;
	default:
		ASSERT(FALSE);
	}
}

void COXTabViewContainer::DrawTabButton(CDC* pDC, const int nIndex) const
{
	ASSERT(nIndex>=0 && nIndex<GetPageCount());

	CRect rect=m_arrTabBtnRects[nIndex];
	rect+=m_rectTabBtnArea.TopLeft();
	rect.OffsetRect(m_nTabBtnAreaOrigin,0);
	if(rect.right>m_rectTabBtnArea.left && rect.left<m_rectTabBtnArea.right)
	{
		rect.bottom=m_rectTabBtnArea.bottom;
		
		POINT arrPoints[4];
		arrPoints[0].x=rect.left;
		arrPoints[0].y=rect.top;
		arrPoints[1].x=rect.left+ID_TABBTNOVERLAPSIZE;
		arrPoints[1].y=rect.bottom;
		arrPoints[2].x=rect.right-ID_TABBTNOVERLAPSIZE;
		arrPoints[2].y=rect.bottom;
		arrPoints[3].x=rect.right;
		arrPoints[3].y=rect.top;

		CPen penTop(PS_SOLID,1,::GetSysColor(COLOR_WINDOW));
		CPen penBottom(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));
		CPen* pOldPen=NULL;
		if(nIndex==GetActivePageIndex())
		{
			pDC->Polygon(arrPoints,4);

			pOldPen=pDC->SelectObject(&penTop);
			arrPoints[0].x++;
			pDC->MoveTo(arrPoints[0]);
			pDC->LineTo(arrPoints[3]);

			pDC->SelectObject(&penBottom);
			arrPoints[1].y--;
			arrPoints[2].y--;
			pDC->MoveTo(arrPoints[1]);
			pDC->LineTo(arrPoints[2]);
		}
		else
		{
			pDC->Polygon(arrPoints,4);

			pOldPen=pDC->SelectObject(&penBottom);
			arrPoints[1].y--;
			arrPoints[2].y--;
			pDC->MoveTo(arrPoints[1]);
			pDC->LineTo(arrPoints[2]);
			arrPoints[2].x--;
			arrPoints[3].x--;
			arrPoints[3].y--;
			pDC->MoveTo(arrPoints[2]);
			pDC->LineTo(arrPoints[3]);

			pDC->SelectObject(&penTop);
			arrPoints[0].x++;
			arrPoints[1].x++;
			pDC->MoveTo(arrPoints[0]);
			pDC->LineTo(arrPoints[1]);
		}
		if(pOldPen!=NULL)
			pDC->SelectObject(pOldPen);

		CString sTitle=GetPageTitle(nIndex);
		if(!sTitle.IsEmpty())
		{
			COLORREF oldColor=pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(sTitle,rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			pDC->SetBkMode(nOldBkMode);
			pDC->SetTextColor(oldColor);
		}
	}
}

int COXTabViewContainer::HitTest(const CPoint point) const
{
	CRect m_rectPage;

	if(m_rectSplitter.PtInRect(point))
		return SPLITTER;
	else if(m_rectScrollBarHorz.PtInRect(point))
		return SCROLLBARHORZ;
	else if(m_rectScrollBarVert.PtInRect(point))
		return SCROLLBARVERT;
	else if(m_rectScrollToStartBtn.PtInRect(point))
		return SCRLSTARTBTN;
	else if(m_rectScrollBackwardBtn.PtInRect(point))
		return SCRLBACKWARDBTN;
	else if(m_rectScrollForwardBtn.PtInRect(point))
		return SCRLFORWARDBTN;
	else if(m_rectScrollToEndBtn.PtInRect(point))
		return SCRLENDBTN;
	else if(m_rectTabBtnArea.PtInRect(point))
	{
		// test here for tab buttons
		for(int nIndex=0; nIndex<GetPageCount(); nIndex++)
		{
			CRect rect=m_arrTabBtnRects[nIndex];
			rect+=m_rectTabBtnArea.TopLeft();
			rect.OffsetRect(m_nTabBtnAreaOrigin,0);
			rect.bottom=m_rectTabBtnArea.bottom;
			if(rect.PtInRect(point))
				return nIndex;
		}

		return TABBTNAREA;
	}
	else if(m_rectSizeBar.PtInRect(point))
		return SIZEBAR;
	else if(m_rectPage.PtInRect(point))
		return PAGE;

	return NONE;
}

void COXTabViewContainer::RedrawScrollButton(HITTEST hitTest)
{
	switch(hitTest)
	{
	case SCRLSTARTBTN:
		RedrawWindow(m_rectScrollToStartBtn);
		break;
	case SCRLBACKWARDBTN:
		RedrawWindow(m_rectScrollBackwardBtn);
		break;
	case SCRLFORWARDBTN:
		RedrawWindow(m_rectScrollForwardBtn);
		break;
	case SCRLENDBTN:
		RedrawWindow(m_rectScrollToEndBtn);
		break;
	}
}

void COXTabViewContainer::ScrollPage(const HITTEST nScrlBtn)
{
	BOOL m_bHasScrolled=FALSE;

	switch(nScrlBtn)
	{
	case SCRLSTARTBTN:
		if(CanScrollToStart())
		{
			m_nTabBtnAreaOrigin=0;
			m_bHasScrolled=TRUE;
		}
		break;
	case SCRLBACKWARDBTN:
		if(CanScrollBackward())
		{
			m_nTabBtnAreaOrigin+=ID_SCROLLTABBTNAREASTEP;
			m_nTabBtnAreaOrigin=
				m_nTabBtnAreaOrigin>0 ? 0 : m_nTabBtnAreaOrigin;
			m_bHasScrolled=TRUE;
		}
		break;
	case SCRLFORWARDBTN:
		if(CanScrollForward())
		{
			m_nTabBtnAreaOrigin-=ID_SCROLLTABBTNAREASTEP;
			CRect rect=m_arrTabBtnRects[GetPageCount()-1];
			rect+=m_rectTabBtnArea.TopLeft();
			if(rect.right+m_nTabBtnAreaOrigin<m_rectTabBtnArea.right)
				m_nTabBtnAreaOrigin=m_rectTabBtnArea.right-rect.right;
			m_bHasScrolled=TRUE;
		}
		break;
	case SCRLENDBTN:
		if(CanScrollToEnd())
		{
			CRect rect=m_arrTabBtnRects[GetPageCount()-1];
			rect+=m_rectTabBtnArea.TopLeft();
			m_nTabBtnAreaOrigin=m_rectTabBtnArea.right-rect.right;
			m_bHasScrolled=TRUE;
		}
		break;
	default:
		ASSERT(FALSE);
	}

	if(m_bHasScrolled)
		RedrawTabBtnArea();
}


void COXTabViewContainer::StartTracking(const CPoint& point)
{
	ASSERT(m_nPressedScrlBtn==NONE);
	m_nPressedScrlBtn=NONE;

	int hitTest=HitTest(point);
	if(hitTest==SCRLSTARTBTN || hitTest==SCRLBACKWARDBTN || 
		hitTest==SCRLFORWARDBTN || hitTest==SCRLENDBTN)
	{
		SetCapture();
		m_nPressedScrlBtn=(HITTEST)hitTest;
		m_bIsScrlBtnPressed=TRUE;
		RedrawScrollButton(m_nPressedScrlBtn);
		ScrollPage(m_nPressedScrlBtn);
		m_nScrollPageTimer=SetTimer(IDT_SCROLLPAGE_TIMER,
			ID_SCROLLPAGE_DELAY,NULL);
	}
	else if(hitTest==SPLITTER)
	{
		SetCapture();
		m_bIsSplitterPressed=TRUE;
	}
}

	
void COXTabViewContainer::StopTracking(const CPoint& point)
{
	if(::GetCapture()==GetSafeHwnd())
		::ReleaseCapture();

	if(m_nPressedScrlBtn!=NONE)
	{
		int hitTest=HitTest(point);
		if((int)m_nPressedScrlBtn==hitTest && m_nScrollPageTimer!=-1)
		{
			KillTimer(m_nScrollPageTimer);
			m_nScrollPageTimer=-1;
		}

		HITTEST nOldPressedScrlBtn=m_nPressedScrlBtn;
		m_nPressedScrlBtn=NONE;
		m_bIsScrlBtnPressed=FALSE;
		RedrawScrollButton(nOldPressedScrlBtn);
	}
	else if(m_bIsSplitterPressed)
	{
		m_bIsSplitterPressed=FALSE;
	}
}




