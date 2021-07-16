// ViewManager.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"       // main symbols
#include "ViewManager.h"
#include "PopupMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIEWTAB           1005
#define ID_DEFDOCTIPS        _T("A new unsaved file")
#define ID_DEFCAPTION        _T("Open File Tabs Bar")
#define ID_TABHEIGHT         26

//
// This function adds a title entry to a popup menu
//
void AddMenuTitle(CMenu* popup, LPCSTR title)
{
    // insert a separator item at the top
    popup->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, title);

    // insert title item
    // note: item is not selectable (disabled) but not grayed
    popup->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_DISABLED, 0, title);

    SetMenuDefaultItem(popup->m_hMenu, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CVViewManager

IMPLEMENT_DYNAMIC(CViewManager, CCBaseControlBar)

CViewManager::CViewManager()
{
	m_sizeDefault   = CSize(400, 32);    // set fixed size
	m_sizeMRU       = CSize(400, 32);
    m_sizeMin       = CSize( 32, 32);    //TODO--
    m_sizeFloat     = CSize(400, 30);   //TODO--
	m_bClosing      = FALSE;
	m_nLMargin      = 10;
	m_nDockID       = 0;
	m_bWin2000      = TRUE;
}

CViewManager::~CViewManager()
{
}

BEGIN_MESSAGE_MAP(CViewManager, CCBaseControlBar)
	//{{AFX_MSG_MAP(CViewManager)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnViewManagerToolTip)
END_MESSAGE_MAP()

void CViewManager::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CMDIFrameWnd* pFrame = static_cast<CMDIFrameWnd*>(AfxGetApp()->m_pMainWnd);
	if (pFrame == NULL)
		return;        // this is not meant for us...
	
	// Get the active MDI child window.
	CMDIChildWnd* pChild = static_cast<CMDIChildWnd*>(pFrame->GetActiveFrame());
	// Get the active view attached to the active MDI child window.
	CView* pActiveView = reinterpret_cast<CView*>(pChild->GetActiveView());
    if (pActiveView == NULL) //...Is there a view anyway?
	{
		//...since there is no view hide the tab control, otherwise it looks...guess!
		m_ViewTabCtrl.ShowWindow(SW_HIDE);
		return;
	}
	else
	{
		//...we might have hidden the tab control, show it now
		if (!m_ViewTabCtrl.IsWindowVisible())
			m_ViewTabCtrl.ShowWindow(SW_SHOW);
	}
	
	// Now, we go...
	int iSel = -1;
    if (pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
    {
		CString strWin;
		// scan the TabCtrl Tabs
        for (int nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount() ; ++nTabs)
        {
			TCHAR szLabel[_MAX_PATH];
			
            CMDIChildWnd* pViewAt = GetTabInfo(nTabs, szLabel);
            ASSERT_KINDOF(CMDIChildWnd, pViewAt);
			
			// Note that we don't use GetDocument()->GetTitle() here as it doesn't include
			// the :n suffix when a Doc is open in multiple views. eg. test.cpp:1
            pViewAt->GetWindowText(strWin);
			
			// TRACE( "OnUpdateCmdUI() strWin = '%s', tci.pszText = '%s'\n", strWin, tci.pszText );
			
			// ...if there is window title change since the last update set the new title
            if (szLabel != strWin)
                SetViewName(strWin, pViewAt);
			
			// ...find the active view from the view list
            if (pViewAt == pChild)
                iSel = nTabs;
        }
    }
	
	m_ViewTabCtrl.SetCurSel(iSel);   // set the tab for the active view
	
	// Be polite! update the dialog controls added to the CControlBar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


////////////////////////////////////////////////////////////////////////////
// CViewManager	operations

CMDIChildWnd* CViewManager::GetTabInfo(int nTab, TCHAR* pszLabel /* = NULL */) const
{
	TCITEM tci;
    tci.mask = TCIF_PARAM | (pszLabel != NULL ? TCIF_TEXT : 0x00);
    tci.pszText    = pszLabel;
    tci.cchTextMax = _MAX_PATH - 1;
    VERIFY(m_ViewTabCtrl.GetItem(nTab, &tci));

    CMDIChildWnd* pView = reinterpret_cast<CMDIChildWnd*>(tci.lParam);
    ASSERT(pView != NULL);
    ASSERT_KINDOF(CMDIChildWnd, pView);
    return pView;
}

void CViewManager::AddView(const TCHAR* csName, CMDIChildWnd* pView)
{
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
	if (!m_bClosing && m_ViewTabCtrl.GetSafeHwnd()) 
	{
		CString cs(csName);
		
		if (m_ViewTabCtrl.GetSafeHwnd())
		{
			TCITEM tci;
			tci.mask    = TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE;
			tci.pszText = cs.LockBuffer();
			tci.lParam  = reinterpret_cast<LPARAM>(pView);
			tci.iImage  = 0;                       //TODO
			m_ViewTabCtrl.InsertItem(m_ViewTabCtrl.GetItemCount(), &tci);
			cs.UnlockBuffer();
		}
	}
}

void CViewManager::RemoveView(CMDIChildWnd* pView)
{
    if (!m_bClosing)
    {
        ASSERT(pView->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))); 

        if (m_ViewTabCtrl.GetSafeHwnd())
        {
            for (int nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
            {
                TCITEM tci;
                tci.mask = TCIF_PARAM;
                m_ViewTabCtrl.GetItem(nTabs, &tci);
                if (tci.lParam == reinterpret_cast<LPARAM>(pView))
                {
                    m_ViewTabCtrl.DeleteItem(nTabs);
                    break;
                }
            }
        }
    }
}

void CViewManager::RemoveAll()
{
}

void CViewManager::SetViewName(const TCHAR* cs, CMDIChildWnd* pView)
{
    if (!m_bClosing)
    {
        ASSERT(pView->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))); 

        if (m_ViewTabCtrl.GetSafeHwnd())
        {
			CString csName(cs);
            for (int nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
            {
                TCITEM tci;
                tci.mask = TCIF_PARAM;
                m_ViewTabCtrl.GetItem(nTabs, &tci);
                if (tci.lParam == reinterpret_cast<LPARAM>(pView))
                {
                    tci.mask = TCIF_PARAM | TCIF_TEXT;
                    tci.pszText = csName.LockBuffer();
                    m_ViewTabCtrl.SetItem(nTabs, &tci);
                    csName.UnlockBuffer();
                    m_ViewTabCtrl.Invalidate();
                    break;
                }
            }
        }
    }
}

int CViewManager::GetWindowNum()
{
	return m_ViewTabCtrl.GetItemCount();
}

void CViewManager::OnActivateView(const BOOL bActivate, CMDIChildWnd* pView)
{
	if (bActivate)
	{
		if (m_ViewTabCtrl.GetSafeHwnd())
		{
			for (int nTabs = 0; nTabs < m_ViewTabCtrl.GetItemCount(); nTabs++)
			{
				TCITEM tci;
				tci.mask = TCIF_PARAM;
				m_ViewTabCtrl.GetItem(nTabs, &tci);
				if (tci.lParam == reinterpret_cast<LPARAM>(pView))
				{
					m_ViewTabCtrl.SetCurSel(nTabs);
					m_ViewTabCtrl.Invalidate();
					break;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CViewManager message handlers

void CViewManager::OnSize(UINT nType, int cx, int cy) 
{
	CCBaseControlBar::OnSize(nType, cx, cy);
	
	CRect rect, rcClient;
	if (m_ViewTabCtrl.GetSafeHwnd())
	{
		DWORD dwStyle = m_ViewTabCtrl.GetStyle();
		if (dwStyle & TCS_BOTTOM)
		{
			GetClientRect(rect);
			GetClientRect(rcClient);
			int nxOffset = ::GetSystemMetrics(SM_CXSIZEFRAME);
			m_ViewTabCtrl.SetWindowPos(&wndTop, rcClient.left + nxOffset + m_nLMargin, 
				rcClient.top, rect.Width() - nxOffset * 5 - m_nLMargin, 
				ID_TABHEIGHT, SWP_DEFERERASE);

			//m_ViewTabCtrl.SetItemSize(CSize(rect.Width() - nxOffset * 5 - m_nLMargin, rect.Height()));
			
			m_sizeMRU = CSize(cx, cy);
		}
		else
		{
			GetClientRect(rect);
			int nxOffset = ::GetSystemMetrics(SM_CXSIZEFRAME);
			m_ViewTabCtrl.SetWindowPos(&wndTop, nxOffset + m_nLMargin, 3, 
				rect.Width() - nxOffset * 5 - m_nLMargin, 
				ID_TABHEIGHT, SWP_DEFERERASE);
			
			//m_ViewTabCtrl.SetItemSize(CSize(rect.Width() - nxOffset * 5 - m_nLMargin, rect.Height()-3));
			m_sizeMRU = CSize(cx, cy);
		}
	}
}

int CViewManager::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCBaseControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_ViewTabImages.Create(16, 16, ILC_MASK, 5, 5);	

	m_ViewTabCtrl.Create(WS_CHILD | WS_VISIBLE | WS_EX_NOPARENTNOTIFY | 
//			TCS_BUTTONS	| TCS_FLATBUTTONS |		 // pLaY with this!
		TCS_TOOLTIPS | TCS_SINGLELINE | TCS_FOCUSNEVER | TCS_FORCELABELLEFT/* | TCS_OWNERDRAWFIXED*/, 
		CRect(0, 0, 0, 0), this, ID_VIEWTAB);

	TabCtrl_SetExtendedStyle(m_ViewTabCtrl.m_hWnd, TCS_EX_FLATSEPARATORS);

	m_ViewTabCtrl.SetImageList(&m_ViewTabImages);
	// Build the image list here
    HICON hIcon = AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);
//    HICON hIcon = AfxGetApp()->LoadIcon(IDR_DEMOTYPE);
	m_ViewTabImages.Add(hIcon);
	// Enable tooltips for all controls
	EnableToolTips(TRUE);
	
	return 0;
}

void CViewManager::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT  ptScreen = point;
	// Convert the mouse point to screen coordinates since that is what is used by
	// the TrackPopupMenu() function.
	ClientToScreen(&ptScreen);
	CPopupMenu cMenu;
	cMenu.CreatePopupMenu();
	
	cMenu.AppendMenu(MF_STRING, ID_VIEW_VIEWTAB, _T("Op&en File Tabs"));
	cMenu.AppendMenu(MF_STRING, ID_VIEW_FULLSCREEN, _T("F&ull Screen"));
	cMenu.AppendMenu(MF_SEPARATOR);
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_CLOSE_ALL, _T("C&lose All"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_SAVE_ALL, _T("&Save All"));
	cMenu.AppendMenu(MF_SEPARATOR);
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_NEW, _T("&New Windows"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_ARRANGE, _T("&Arrange Icons"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_CASCADE, _T("&Cascade Windows"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_TILE_HORZ, _T("Tile &Horizontally"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_TILE_VERT, _T("Tile &Vertically"));
	cMenu.AppendMenu(MF_SEPARATOR);
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_NEXT, _T("Ne&xt Window"));
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_PREVIOUS, _T("P&revious Window"));
	cMenu.AppendMenu(MF_SEPARATOR);
	cMenu.AppendMenu(MF_STRING, ID_WINDOW_MANAGE, _T("&Windows..."));

	cMenu.LoadToolBarResource(IDR_TABVIEW_MENU);
	cMenu.RemapMenu(&cMenu);
	cMenu.EnableMenuItems(&cMenu, AfxGetMainWnd());

    // insert a separator item at the top
    cMenu.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));

    // insert title item
    // note: item is not selectable (disabled) but not grayed
    cMenu.InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_DISABLED, 0, _T("Open File Tabs Bar"));

    ::SetMenuDefaultItem(cMenu.m_hMenu, 0, TRUE);
	
	cMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, 
		ptScreen.y, AfxGetMainWnd());
	
	cMenu.DestroyMenu();

	CCBaseControlBar::OnRButtonDown(nFlags, point);
}

BOOL CViewManager::OnViewManagerToolTip(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
    TCHITTESTINFO hti;
	TOOLTIPTEXT *pTTT = reinterpret_cast<TOOLTIPTEXT*>(pNMHDR);    
	UINT nID = pNMHDR->idFrom;

	// if there are some dialog controls, progress the tooltips
    if (pTTT->uFlags & TTF_IDISHWND)    
	{
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);        
		if (nID)        
		{
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst    = AfxGetResourceHandle();            
			return TRUE;
        }    
	} 

	// Now, address the view tab tooltips
    hti.pt = CPoint(GetMessagePos());
    m_ViewTabCtrl.ScreenToClient(&hti.pt);
    int nTab = m_ViewTabCtrl.HitTest(&hti);

	if (nTab >= 0)
	{
		CMDIChildWnd* pChild = GetTabInfo(nTab);
		// Get the active view attached to the active MDI child window.
		CView* pView = reinterpret_cast<CView*>(pChild->GetActiveView());
		ASSERT(pView != NULL);
		CDocument* pDoc = pView->GetDocument();
		ASSERT(pDoc != NULL);
		CString strTabText = pDoc->GetPathName();
		if (strTabText.IsEmpty()) //... the document is not yet saved?
			strTabText = ID_DEFDOCTIPS;

		lstrcpy(pTTT->lpszText, strTabText);
		return TRUE;
	}
	return FALSE;
}

BOOL CViewManager::CreateViewManager(CMDIFrameWnd *pMDIFrameWnd, UINT uID)
{
    if (!Create(ID_DEFCAPTION, pMDIFrameWnd, m_sizeDefault, uID))
	{
		TRACE0(_T("Failed to create Tab bar\n"));  
		return FALSE;      // fail to create
	}

	SetBarStyle(GetBarStyle() | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM/*CBRS_ALIGN_ANY*/);
 	pMDIFrameWnd->DockControlBar(this); 

	return TRUE;
}

BOOL CViewManager::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
                               CSize sizeDefault, UINT nID, DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT(!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

    m_dwStyle     = dwStyle & CBRS_ALL; // save the control bar styles
	m_sizeDefault = sizeDefault;    // set fixed size
	m_sizeMRU     = sizeDefault;
    // register and create the window - skip CControlBar::Create()
    CString strWndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL;
	dwStyle |= WS_CLIPCHILDREN | CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NORESIZE;
    if (!CWnd::Create(strWndclass, lpszWindowName, dwStyle,
        CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;

    return TRUE;
}


CSize CViewManager::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	
	if (bStretch) // the bar is stretched (is not the child of a dockbar)
		return CSize(bHorz ? 32767 : m_sizeDefault.cx, bHorz ? m_sizeDefault.cy : 32767);
	
	CClientDC dc(NULL);
	HFONT hFont    = reinterpret_cast<HFONT>(SendMessage(WM_GETFONT));
	HFONT hOldFont = NULL;
	if (hFont != NULL)
		hOldFont = reinterpret_cast<HFONT>(dc.SelectObject(hFont));
	TEXTMETRIC tm;
	VERIFY(dc.GetTextMetrics(&tm));
	if (hOldFont != NULL)
		dc.SelectObject(hOldFont);
	
	// get border information
	CSize size;
	CRect rcInside, rcWnd; 
	rcInside.SetRectEmpty();
	CalcInsideRect(rcInside, bHorz);
	GetParentFrame()->GetWindowRect(&rcWnd);
	size.cx = rcWnd.Width();
	size.cy = tm.tmHeight + tm.tmInternalLeading // - 1
		 + ::GetSystemMetrics(SM_CYBORDER) * 5 - rcInside.Height();

	return size;
}


CSize CViewManager::CalcDynamicLayout(int nLength, DWORD dwMode)
{
//	UNREFERENCED_PARAMETER(nLength);	   // clear that level 4 warning...

	if (dwMode & LM_HORZDOCK)
	{
		ASSERT(dwMode & LM_HORZ);
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
	}

/*	if (m_sizeMRU.cx > m_sizeDefault.cx)
		return m_sizeMRU;
	else
		return m_sizeDefault;
*/
    if (dwMode & LM_MRUWIDTH)
        return m_sizeFloat;

    if (dwMode & LM_COMMIT)
    {
        m_sizeFloat.cx = nLength;
        return m_sizeFloat;
    }

	// TODO-remove the hard coded values...
    if (dwMode & LM_LENGTHY)
        return CSize(m_sizeFloat.cx,
            m_sizeFloat.cy /*= m_sizeMRU.cy30*/); //max(m_sizeMin.cy, nLength));
    else
        return CSize(max(m_sizeMin.cx, nLength), m_sizeFloat.cy/*m_sizeMRU.cy*//*30*/);

}

void CViewManager::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_FRAMECHANGED;
	CCBaseControlBar::OnWindowPosChanging(lpwndpos);
	
	UINT nOldDockID = m_nDockID;
	m_nDockID = GetParent()->GetDlgCtrlID();
	if (nOldDockID == m_nDockID)
		return;
	else
	{
		DWORD dwStyle = m_ViewTabCtrl.GetStyle();
		switch(m_nDockID)
		{
		case AFX_IDW_DOCKBAR_TOP:
			if (dwStyle & TCS_BOTTOM)
			{
				DWORD dwNewStyle = dwStyle & ~TCS_BOTTOM;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
			}
			break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			if ((dwStyle & TCS_BOTTOM) == 0)
			{
				DWORD dwNewStyle = dwStyle | TCS_BOTTOM;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
			}
			break;
/*		case AFX_IDW_DOCKBAR_LEFT:
//			if ((dwStyle & TCS_RIGHT))
			{
				DWORD dwNewStyle = dwStyle | TCS_VERTICAL | TCS_MULTILINE;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
			}
			break;
		case AFX_IDW_DOCKBAR_RIGHT:
//			if ((dwStyle & TCS_RIGHT) == 0)
			{
				DWORD dwNewStyle = dwStyle | TCS_RIGHT;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
			}
			break;
*/		default:
				DWORD dwNewStyle = dwStyle & ~TCS_BOTTOM;
				m_ViewTabCtrl.ModifyStyle(dwStyle, dwNewStyle);
			break;
		}
	}
}

void CViewManager::DoPaint(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	// clean background
	COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
	pDC->FillSolidRect(rect, clr);	

	// draw the gripper
	DrawGripper(pDC);
	// It is better to let the underlining control bar take the last shot
	CCBaseControlBar::DoPaint(pDC);
}

void CViewManager::DrawGripper(CDC* pDC)
{
	if( (m_dwStyle & CBRS_FLOATING) || m_dwDockStyle == 0 )
		return;

	COLORREF clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
	COLORREF clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);

	CRect rcGrip;					 
	GetWindowRect(&rcGrip);
	ScreenToClient(&rcGrip);
	rcGrip.OffsetRect(-rcGrip.left, -rcGrip.top);

	if(m_dwStyle & CBRS_ORIENT_HORZ) 
	{
		// gripper at left
		rcGrip.DeflateRect(4, 4);
		rcGrip.right = rcGrip.left + 3;
        pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
		if (!m_bWin2000)
		{
			rcGrip.OffsetRect(3, 0);
			pDC->Draw3dRect(rcGrip,	clrBtnHilight, clrBtnShadow);
		}
	}
	else 
	{
		// gripper at top
		rcGrip.DeflateRect(4, 4);
		rcGrip.bottom = rcGrip.top + 3;
		pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
		if (!m_bWin2000)
		{
			rcGrip.OffsetRect(0, 3);
			pDC->Draw3dRect(rcGrip, clrBtnHilight, clrBtnShadow);
		}
	}
}


void CViewManager::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	OnSize(0, lpwndpos->cx, lpwndpos->cy);
	CCBaseControlBar::OnWindowPosChanged(lpwndpos);
}
