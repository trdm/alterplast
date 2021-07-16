// ========================================================================================
// 					Class Implementation : COXSizeControlBar
// ========================================================================================

// Source file : OXSizeCtrlBar.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSizeCtrlBar.h"

//#include "OXDockCtrlBarRes.h"  // for some resource strings
#include "OXDragDockContext.h"
#include "OXFrameWndDock.h"
#include "OXMDIFloatWnd.h"
#include "OXSizeDockBar.h"  
#include "OXSzMiniDockFrmWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXSizeControlBar

IMPLEMENT_DYNAMIC(COXSizeControlBar, CControlBar)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
CObArray* COXSizeControlBar::m_parrAllocBars;	

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXSizeControlBar::COXSizeControlBar(int nStyle)
{
	m_Style=nStyle;						
    m_PrevSize=CSize(0xffff,0xffff);	// dummy values so WindowPosChanged will 
										// respond correctly
    m_bPrevFloating=3;					// neither TRUE not FALSE;
	
	m_FloatSize=CSize(0,0);				// size when floating
   	m_HorzDockSize=CSize(0,0);			// size when docked horizontal
	m_VertDockSize=CSize(0,0);			// size when docked vertical

   	m_SavedDockSize=CSize(0,0);			// size before maximizing

	m_FloatingPosition=CPoint(0,0);	
	m_dwAllowDockingState = 0;
	if (nStyle & SZBARF_AUTOTIDY)
	{
		if (m_parrAllocBars == NULL)
			m_parrAllocBars = new CObArray;
		
		m_parrAllocBars->Add(this);
	}

	m_rectGripper.SetRectEmpty();			// gripper rect
	m_rectCloseBtn.SetRectEmpty();			// close button rect
	m_rectResizeBtn.SetRectEmpty();		// restore button rect

	m_bDelayRecalcLayout=FALSE;

	m_pressedBtn=NONE;
	m_bMaximized=FALSE;

	if(!m_font.CreatePointFont(100,_T("Arial")))
	{
		AfxThrowResourceException();
		return;
	}
}


COXSizeControlBar::~COXSizeControlBar()
{
	// if the bar was created with this flag, then ensure it is deleted with it also.
	if (m_Style & SZBARF_AUTOTIDY)
	{
		int i;
		for (i = m_parrAllocBars->GetUpperBound(); i >= 0; i--)
			if ((*m_parrAllocBars)[i] == this)	
		{
			m_parrAllocBars->RemoveAt(i);
			break;
		}
		ASSERT(i >= 0);			// means we didn't delete this item from the list
	}
	
	// This loop of debug code checks that we don't have any other references in the array.
	// This happens if we changed the auto delete flag during the lifetime of the control bar.
#ifdef _DEBUG
	if (m_parrAllocBars != NULL)
	{
		for (int i = m_parrAllocBars->GetUpperBound(); i >= 0; i--)
			ASSERT ((*m_parrAllocBars)[i] != this);	
	}
#endif

	if(m_pDockContext!=NULL)
	{
		// delete the dock context here - in an attempt to call the correct destructor
		delete (COXDragDockContext*)m_pDockContext;
		m_pDockContext = NULL;
	}
}


void COXSizeControlBar::TidyUp()
	{
	// just need to delete the contents of the array - the destructor will remove the element
	// from the array. 
	if (m_parrAllocBars != NULL)
		{
		for (int i = m_parrAllocBars->GetUpperBound(); i >= 0; i--)
			{
			ASSERT((*m_parrAllocBars)[i]->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)));
			delete ((*m_parrAllocBars)[i]);
			}
		delete m_parrAllocBars;
		m_parrAllocBars = NULL;
		}
	}

BEGIN_MESSAGE_MAP(COXSizeControlBar, CControlBar)
//{{AFX_MSG_MAP(COXSizeControlBar)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOMMAND()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
	ON_COMMAND(ID_MRC_HIDE, OnHide)
	ON_COMMAND(ID_MRC_ALLOWDOCKING, OnToggleAllowDocking)
	ON_COMMAND(ID_MRC_MDIFLOAT,	OnFloatAsMDI)
	ON_MESSAGE(WM_ADDCONTEXTMENUITEMS, OnAddContextMenuItems)
	ON_MESSAGE(WM_USER_AFTERFLOAT_MSG, OnAfterFloatMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXSizeControlBar message handlers

CSize COXSizeControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
//#ifdef _VERBOSE_TRACE
	CString strTitle;
	GetWindowText(strTitle);
	TRACE(_T("CalcFixedLayout: '%s' Horz(%d,%d)\n"), LPCTSTR(strTitle), 
		m_HorzDockSize.cx, m_HorzDockSize.cy);
//#endif	
	CControlBar::CalcFixedLayout(bStretch, bHorz);
    if (IsFloating())
		return m_FloatSize;
	
    if (bHorz)
		return m_HorzDockSize;
	else
		return m_VertDockSize;
}


// need to supply this, or else we can't instantiate the class. Derived classes should
// subclass this if they need to update their gadgets using this interface
void COXSizeControlBar::OnUpdateCmdUI(CFrameWnd* /* pTarget */, BOOL /* bDisableIfNoHndler */)
	{
	}


// CWnd-style create - need ability to specific window class in order to prevent flicker
// during resizing. 
BOOL COXSizeControlBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							   DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
							   UINT nID, CCreateContext* pContext)
	{
    ASSERT(pParentWnd != NULL);
	
    // have to set the style here
#if _MFC_VER <= 0x0421
    m_dwStyle = dwStyle;
#else
    m_dwStyle = dwStyle&CBRS_ALL;
#endif
	
    CRect Rectx;
    Rectx = rect;
	
    // calculate a sensible default rectangle if that's what the user wanted...
    if (memcmp(&rect, &CFrameWnd::rectDefault, sizeof(RECT)) == 0)
		{
        pParentWnd->GetClientRect(&Rectx);
        CSize def;
        def.cx = Rectx.right / 2;
        def.cy = Rectx.bottom  / 4;
        Rectx.left = Rectx.right - def.cx;
        Rectx.top  = Rectx.bottom - def.cy;
		}
	
   	// the rectangle specifies the default floating size.
   	m_FloatSize = Rectx.Size();
	
	// set default values for the docked sizes, based on this size.
	m_HorzDockSize.cx = m_FloatSize.cx;
	m_HorzDockSize.cy = m_FloatSize.cy;	
	
	m_VertDockSize.cx = m_HorzDockSize.cy;
	m_VertDockSize.cy = m_HorzDockSize.cx;
	
    return CControlBar::Create(lpszClassName, lpszWindowName, dwStyle, 
		Rectx, pParentWnd, nID, pContext);
	}

BOOL COXSizeControlBar::Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, UINT nID,
								DWORD dwStyle, const RECT& rect)
	{
	return Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

void COXSizeControlBar::SetSizeDockStyle(DWORD dwStyle)
	{
	m_Style = dwStyle;
	}


// Largely a copy of CControlBar::EnableDocking() - but uses a different class for the
// m_pDockContext, to give us different (hopefully you'll think better) dragging 
// behaviour.
void COXSizeControlBar::EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);
	
    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
		m_pDockContext = new COXDragDockContext(this);
	
    // permanently wire the bar's owner to its current parent
	if (m_hWndOwner == NULL)
		m_hWndOwner = ::GetParent(m_hWnd);
}

// message handler. Force the parent of the control bar to update it's style
// after floating, otherwise we'll wait till an WM_NCHITTEST.
afx_msg LONG COXSizeControlBar::OnAfterFloatMessage(UINT /* wParam */, LONG /* lParam */)
{
	CWnd* pFrame = GetParentFrame();
	if (pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(COXSizableMiniDockFrameWnd)))
		((COXSizableMiniDockFrameWnd*)pFrame)->GetContainedBarType();
	
	return TRUE;			// message handled.
}

// paint the background of the window - probably want a style flag to turn this
// off as for many control bars it won't be required.
BOOL COXSizeControlBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->FillSolidRect(&rect,::GetSysColor(COLOR_BTNFACE)); 
    return TRUE;
}


// change the cursor 
BOOL COXSizeControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	if(pWnd==this)
	{
		HCURSOR hCursor;     // Load the predefined Windows standard cursor.
		hCursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		ASSERT(hCursor);
		::SetCursor(hCursor);
		return TRUE;
	}

	return FALSE;
}

// Normally a CControlBar would just pass most of these messages through to
// the parent. We want to handle them properly though - again may be this should
// be a behaviour flag
LRESULT COXSizeControlBar::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
    ASSERT_VALID(this);
	
    // We need to ensure WM_COMMAND and other messages get through to the derived class.
    // Primarily done so we receive notifications from child windows. The default CControlBar
    // code routes messsages through to the parent. This means WM_COMMANDs, etc make their
    // way to a FrameWnd eventually. This is needed for toolbar's, dialog bars, etc, but isn't
    // very useful if we want to put controls on a COXSizeControlBar and process them 
	// locally
	
    // In case any of these messages are actually needed by the owner window, we check to see
    // if CWnd would handle them first. If not, then we pass them through to the owner window,
    // as CControlBar would.
	
    switch (nMsg)
		{
		case WM_COMMAND:
			if (OnCommand(wParam, lParam))          // post normal commands....
				return 1L; // command handled
			break;
			
		}
	return CControlBar::WindowProc(nMsg, wParam, lParam);
	}


// This handler is used to notify sizeable bars if their size has
// changed, or if they are docked/undocked.
void COXSizeControlBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
    CControlBar::OnWindowPosChanged(lpwndpos);
	
    CSize NewSize(lpwndpos->cx, lpwndpos->cy);

	// This is meant to return "floating" if we're not docked yet...
    BOOL bFloating = IsProbablyFloating();
	
    int Flags = (NewSize == m_PrevSize ? 0 : 1);
    Flags |= (bFloating == m_bPrevFloating ? 0 : 2);
    if (Flags != 0)
	{
		m_PrevSize = NewSize;
        m_bPrevFloating = bFloating;
        OnSizedOrDocked(NewSize.cx, NewSize.cy, bFloating, Flags);
	}

	RecalcLayout();

	if(m_bMaximized)
		SetMaximized(FALSE);
}


// override this function to respond to a redraw as a result of a
// resize or docked/undocked notification
void COXSizeControlBar::OnSizedOrDocked(int /* cx */, int /* cy */, BOOL /* bFloating */, int /* flags */)
{
}


BOOL COXSizeControlBar::IsProbablyFloating()
{ 
	// used to check the dock bar status, but this has problems when we 
	// docking/undocking - so check the actual bar style instead
	return (m_pDockBar == NULL || (GetBarStyle() & CBRS_FLOATING));
}


LONG COXSizeControlBar::OnAddContextMenuItems(WPARAM /* wParam */, LPARAM lParam)
{
	HMENU hMenu = (HMENU)lParam;		// handle of menu.
	CMenu Menu;
	Menu.Attach(hMenu);
	
	DWORD dwDockStyle = m_dwDockStyle & CBRS_ALIGN_ANY;
	DWORD style;
	CString strMenu;
	
	BOOL bMDIFloating = FALSE;
	CFrameWnd* pParentFrame = GetParentFrame();
	if (IsFloating())
	{
		if (pParentFrame != NULL && pParentFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
		{
			bMDIFloating = TRUE;
		}
	}
	style = (bMDIFloating ? MF_STRING | MF_CHECKED : MF_STRING);
	
	VERIFY(strMenu.LoadString(ID_MRC_CLEAR));
	Menu.AppendMenu(MF_STRING, ID_MRC_CLEAR, strMenu);
	Menu.AppendMenu(MF_SEPARATOR);
	// if allowed - add the float as MDI floating window
	if ((m_Style&SZBARF_ALLOW_MDI_FLOAT)!=0 && m_pDockContext!=NULL)
	{	
		VERIFY(strMenu.LoadString(ID_MRC_MDIFLOAT));
		Menu.AppendMenu(style, ID_MRC_MDIFLOAT, strMenu);
	}
	
	if (!bMDIFloating && (dwDockStyle != 0 || m_dwAllowDockingState != 0))	// ie docking is actually allowed ...
	{
		DWORD style = (dwDockStyle != 0 ? MF_STRING | MF_CHECKED : MF_STRING);
		VERIFY(strMenu.LoadString(ID_MRC_ALLOWDOCKING));
		Menu.AppendMenu(style, ID_MRC_ALLOWDOCKING, strMenu);
	}
	VERIFY(strMenu.LoadString(ID_MRC_HIDE));
	Menu.AppendMenu(MF_STRING, ID_MRC_HIDE, strMenu);
	
	Menu.Detach();	// detatch MFC object
	return TRUE;
}
void COXSizeControlBar::OnHide()
{
	CFrameWnd* pParentFrameWnd = GetParentFrame();
	BOOL bMDIFloating = (IsFloating() && pParentFrameWnd != NULL && pParentFrameWnd->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)));
	if (bMDIFloating)
		// Have to activate anothet MDIchildframe Wnd
	{
		((COXMDIFloatWnd*)pParentFrameWnd)->ShowControlBar(this, FALSE, FALSE);
		CFrameWnd* pTopParentFrameWnd = pParentFrameWnd->GetTopLevelFrame();
		if (pTopParentFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
			((CMDIFrameWnd*)pTopParentFrameWnd)->MDINext();
	}
	else
		pParentFrameWnd->ShowControlBar(this, FALSE, FALSE);
}


void COXSizeControlBar::OnToggleAllowDocking()
{
	if ((m_dwDockStyle & CBRS_ALIGN_ANY) != 0)
	{											   // docking currently allowed - disable it
		m_dwAllowDockingState = m_dwDockStyle & CBRS_ALIGN_ANY;	// save previous state
		if (!IsFloating())
		{					// if docked, then force it to be floating...
			ASSERT(m_pDockContext != NULL);
			m_pDockContext->ToggleDocking();
		}
		EnableDocking(0);				// disable docking
	}
	else
	{
		EnableDocking (m_dwAllowDockingState);	// re-enable docking...
	}
}

void COXSizeControlBar::OnFloatAsMDI()
{
	ASSERT(m_Style & SZBARF_ALLOW_MDI_FLOAT);		// must have specified this
	
	COXMDIFrameWndSizeDock* pFrame = (COXMDIFrameWndSizeDock*)AfxGetMainWnd();
	ASSERT(pFrame != NULL);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock)));
	ASSERT(m_pDockContext!=NULL);
	
	CFrameWnd* pParentFrame = GetParentFrame();
	BOOL bMDIFloating = (IsFloating() && pParentFrame != NULL && pParentFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)));
	
	if (bMDIFloating)
	{
		pFrame->UnFloatInMDIChild(this, m_pDockContext->m_ptMRUFloatPos);
	}
	else
	{
		pFrame->FloatControlBarInMDIChild(this, m_pDockContext->m_ptMRUFloatPos);
	}		
}


// Now run off WM_CONTEXTMENU: if user wants standard handling, then let him have it
void COXSizeControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_Style & SZBARF_STDMOUSECLICKS)
	{
		CMenu menu;
		if (menu.CreatePopupMenu())
		{
			OnAddContextMenuItems(0, (LPARAM)menu.m_hMenu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			
		}
	} 
	else
		CControlBar::OnContextMenu(pWnd, point);
}

void COXSizeControlBar::OnNcCalcSize(BOOL bCalcValidRects, 
									 NCCALCSIZE_PARAMS* lpncsp)
{
	CControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect rect=lpncsp->rgrc[0];
	rect.DeflateRect(ID_CONTAINER_GAP,ID_CONTAINER_GAP);
	
	if(IsGripper() && !(m_dwStyle & CBRS_FLOATING))
	{
		if(m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			rect.DeflateRect(ID_CONTAINER_GAP,0,0,0);
			rect.left+=ID_BUTTON_SIDE;
			if(rect.left>rect.right)
				rect.left=rect.right;
		} 
		else 
		{
			rect.DeflateRect(0,ID_CONTAINER_GAP,0,0);
			rect.top+=ID_BUTTON_SIDE;
			if(rect.top>rect.bottom)
				rect.top=rect.bottom;
		}
		m_bDelayRecalcLayout=TRUE;
	}

	lpncsp->rgrc[0]=rect;
}


void COXSizeControlBar::OnNcPaint() 
{
	EraseNonClient();
}

void COXSizeControlBar::OnPaint() 
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
}

void COXSizeControlBar::EraseNonClient()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);
    CRect rectClient;
    GetClientRect(rectClient);
    CRect rectWindow;
    GetWindowRect(rectWindow);
    ScreenToClient(rectWindow);
	CSize sizeOffset(-rectWindow.left, -rectWindow.top);
	rectClient+=sizeOffset;

    // draw borders in non-client area
    rectWindow+=sizeOffset;
    DrawBorders(&dc, rectWindow);

    // erase parts not drawn
    dc.ExcludeClipRect(rectClient);
    dc.IntersectClipRect(rectWindow);
//	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);
	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillSolidRect(&rect,::GetSysColor(COLOR_BTNFACE)); 

	RecalcLayout();

	if(IsGripper())
		DrawGripper(&dc);
	if(IsCloseBtn())
		DrawCloseBtn(&dc);
	if(IsResizeBtn())
		DrawResizeBtn(&dc);
}

void COXSizeControlBar::DrawGripper(CDC* pDC)
{
    if(!IsGripper() || (m_dwStyle & CBRS_FLOATING))
		return;

    CRect rectWindow;
    GetWindowRect(rectWindow);
    ScreenToClient(rectWindow);
	CRect gripper=m_rectGripper;
    ScreenToClient(gripper);
	gripper.OffsetRect(-rectWindow.left, -rectWindow.top);
	gripper.DeflateRect(2,2);

	BOOL bHorz=(m_dwStyle&CBRS_ORIENT_HORZ)==0;

	CString sBarTitle;
	GetWindowText(sBarTitle);
	CRect rectText=CRect(0,0,0,0);

	//paint text
	if(IsCaption() && !sBarTitle.IsEmpty())
	{
		rectText=gripper;
		if(!bHorz)
			rectText.bottom-=ID_TEXT_OFFSET;
		else
			rectText.left+=ID_TEXT_OFFSET;

		COLORREF oldColor=pDC->SetTextColor(::GetSysColor(COLOR_ACTIVECAPTION));

		CFont* pOldFont=NULL;
		CFont fontVert;
		if(!bHorz)
		{
			// setup font for vertically oriented mode
			LOGFONT lf;
			VERIFY(m_font.GetLogFont(&lf));
			lf.lfEscapement=900;
			lf.lfOrientation=900;
			VERIFY(fontVert.CreateFontIndirect(&lf));
			pOldFont=pDC->SelectObject(&fontVert);
		}
		else
			pOldFont=pDC->SelectObject((CFont*)&m_font);

		CRect rectHelper=rectText;
		if(!bHorz)
		{
			// adjust rectangle to display vertical text
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
		}

		// calculate the rect to display text in
		UINT nFormat=DT_LEFT|DT_SINGLELINE;
		pDC->DrawText(sBarTitle,&rectText,nFormat|DT_CALCRECT);
		if(!bHorz)
		{
			if(rectText.Width()>rectHelper.Height())
				rectText.right=rectText.left+rectHelper.Height();
			rectHelper=rectText;

			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
			rectHelper=rectText;
		}
		else
		{
			if(rectText.Width()>rectHelper.Width())
				rectText.right=rectText.left+rectHelper.Width();
			rectHelper=rectText;
		}

		// adjust coordinates
		if(!bHorz)
		{
			rectText.left+=(gripper.Width()-rectHelper.Width())/2+
				(gripper.Width()-rectHelper.Width())%2;
			rectText.right=rectText.left+rectHelper.Width();
		}
		else
		{
			rectText.top+=(gripper.Height()-rectHelper.Height())/2+
				(gripper.Height()-rectHelper.Height())%2;
			rectText.bottom=rectText.top+rectHelper.Height();
		}

		if(!bHorz)
		{
			rectText.bottom=gripper.bottom-ID_TEXT_OFFSET;
			rectText.top=rectText.bottom-rectHelper.Height();
		}
		else
		{
			rectText.left=rectText.left;
			rectText.right=rectText.left+rectHelper.Width();
		}

		rectHelper=rectText;
		if(!bHorz)
		{
			rectHelper.bottom+=rectText.Width();
			rectHelper.right+=rectText.Height();
		}
		// draw text
		pDC->DrawText(sBarTitle,&rectHelper,DT_BOTTOM|DT_LEFT|
			DT_SINGLELINE|DT_END_ELLIPSIS);

		pDC->SetTextColor(oldColor);
		if(pOldFont!=NULL)
			pDC->SelectObject(pOldFont);
	}


	// paint the gripper
	if(!bHorz) 
	{
		if(!rectText.IsRectEmpty())
			gripper.bottom=rectText.top-ID_TEXTGRIPPER_MARGIN;
		
		// gripper at left
		if(gripper.bottom>gripper.top && gripper.Height()>=ID_GRIPPER_MINSIZE)
		{
			gripper.right = gripper.left + 3;
			pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
				::GetSysColor(COLOR_BTNSHADOW));
			gripper.OffsetRect(4, 0);
			pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
				::GetSysColor(COLOR_BTNSHADOW));
			TRACE(_T("DrawGripper: left=%u, top=%u, right=%u, bottom=%u\n"),
				gripper.left,gripper.top,gripper.right,gripper.bottom);
		}
	}
	else 
	{
		if(!rectText.IsRectEmpty())
			gripper.left=rectText.right+ID_TEXTGRIPPER_MARGIN;

		// gripper at top
		if(gripper.right>gripper.left && gripper.Width()>=ID_GRIPPER_MINSIZE)
		{
			gripper.bottom = gripper.top + 3;
			pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
				::GetSysColor(COLOR_BTNSHADOW));
			gripper.OffsetRect(0, 4);
			pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
				::GetSysColor(COLOR_BTNSHADOW));
			TRACE(_T("DrawGripper: left=%u, top=%u, right=%u, bottom=%u\n"),
				gripper.left,gripper.top,gripper.right,gripper.bottom);
		}
	}
}

void COXSizeControlBar::DrawCloseBtn(CDC* pDC) const
{
    if(!IsCloseBtn() || (m_dwStyle & CBRS_FLOATING))
		return;

	// draw close button
    CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	CRect btn=m_rectCloseBtn;
	ScreenToClient(btn);
	btn.OffsetRect(-rectWindow.left, -rectWindow.top);

	pDC->DrawFrameControl(btn,DFC_CAPTION,DFCS_CAPTIONCLOSE|
		(m_pressedBtn==CLOSEBTN ? DFCS_PUSHED : 0));
}


void COXSizeControlBar::DrawResizeBtn(CDC* pDC) const
{
    if(!IsResizeBtn() || (m_dwStyle & CBRS_FLOATING))
		return;

	// draw close button
    CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	CRect btn=m_rectResizeBtn;
	ScreenToClient(btn);
	btn.OffsetRect(-rectWindow.left, -rectWindow.top);

	pDC->DrawFrameControl(btn,DFC_CAPTION,
		(m_bMaximized ? DFCS_CAPTIONMIN : DFCS_CAPTIONMAX)|
		(m_pressedBtn==RESIZEBTN ? DFCS_PUSHED : 0)|
		(CanResize() ? 0 : DFCS_INACTIVE));
}

void COXSizeControlBar::RecalcLayout()
{
	CRect rectWindow;
	GetWindowRect(rectWindow);
	rectWindow.DeflateRect(ID_CONTAINER_GAP,ID_CONTAINER_GAP);
	m_rectGripper=rectWindow;
    
	if(IsGripper() && !(m_dwStyle & CBRS_FLOATING))
	{
		if(m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			m_rectGripper.right=m_rectGripper.left+ID_BUTTON_SIDE;
			
			if(IsCloseBtn())
			{
				m_rectCloseBtn=m_rectGripper;
				m_rectCloseBtn.bottom=m_rectCloseBtn.top+ID_BUTTON_SIDE;
				m_rectGripper.top+=ID_BUTTON_SIDE+ID_BUTTONS_GAP;
			}

			if(IsResizeBtn())
			{
				m_rectResizeBtn=m_rectGripper;
				m_rectResizeBtn.bottom=m_rectResizeBtn.top+ID_BUTTON_SIDE;
				m_rectGripper.top+=ID_BUTTON_SIDE+ID_BUTTONS_GAP;
			}
		} 
		else 
		{
			m_rectGripper.bottom=m_rectGripper.top+ID_BUTTON_SIDE;
			
			if(IsCloseBtn())
			{
				m_rectCloseBtn=m_rectGripper;
				m_rectCloseBtn.left=m_rectCloseBtn.right-ID_BUTTON_SIDE;
				m_rectGripper.right-=ID_BUTTON_SIDE+ID_BUTTONS_GAP;
			}

			if(IsResizeBtn())
			{
				m_rectResizeBtn=m_rectGripper;
				m_rectResizeBtn.left=m_rectResizeBtn.right-ID_BUTTON_SIDE;
				m_rectGripper.right-=ID_BUTTON_SIDE+ID_BUTTONS_GAP;
			}
		}
	}

	m_bDelayRecalcLayout=FALSE;
}

UINT COXSizeControlBar::OnNcHitTest(CPoint point) 
{
    if((m_dwStyle & CBRS_FLOATING))
        return CControlBar::OnNcHitTest(point);

    CRect rectWindow;
    GetWindowRect(rectWindow);
    if (rectWindow.PtInRect(point))
        return HTCLIENT;
    else
        return CControlBar::OnNcHitTest(point);
}

void COXSizeControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(!IsFloating())
	{
		CPoint ptTest=point;
		ClientToScreen(&ptTest);
		// handle mouse click over close, restore buttons
		//
		m_pressedBtn=NONE;

		if(m_rectCloseBtn.PtInRect(ptTest))
		{
			SetCapture();
			m_pressedBtn=CLOSEBTN;
			RedrawCloseBtn();
			return;
		}
		else if(m_rectResizeBtn.PtInRect(ptTest))
		{
			if(CanResize())
			{
				SetCapture();
				m_pressedBtn=RESIZEBTN;
				RedrawResizeBtn();
			}
			return;
		}
	}

	CControlBar::OnLButtonDown(nFlags, point);
}

void COXSizeControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	SIZEBARBTN pressedBtn=m_pressedBtn;
	m_pressedBtn=NONE;
	if(pressedBtn==CLOSEBTN)
		RedrawCloseBtn();
	else if(pressedBtn==RESIZEBTN)
		RedrawResizeBtn();

	if(::GetCapture()!=GetSafeHwnd())
	{
		CControlBar::OnLButtonUp(nFlags,point);
		return;
	}

	::ReleaseCapture();

	CPoint ptTest=point;
	ClientToScreen(&ptTest);
	
	if(pressedBtn==CLOSEBTN && m_rectCloseBtn.PtInRect(ptTest))
	{
		// close the bar
		GetDockingFrame()->ShowControlBar(this,FALSE,FALSE);
	}
	else if(m_pDockBar!=NULL && pressedBtn==RESIZEBTN && m_rectResizeBtn.PtInRect(ptTest))
	{
		// restore the bar
		ASSERT(m_pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
		((COXSizeDockBar*)m_pDockBar)->ResizeBar(this,!m_bMaximized);
	}

}


void COXSizeControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(::GetCapture()==GetSafeHwnd())
	{
		CPoint ptTest=point;
		ClientToScreen(&ptTest);

		SIZEBARBTN pressedBtn=NONE;
		if(m_rectCloseBtn.PtInRect(ptTest))
			pressedBtn=CLOSEBTN;
		else if(m_rectResizeBtn.PtInRect(ptTest) && CanResize())
			pressedBtn=RESIZEBTN;

		if(m_pressedBtn!=pressedBtn)
		{
			m_pressedBtn=pressedBtn;
			RedrawCloseBtn();
			RedrawResizeBtn();
		}
	}

	CControlBar::OnMouseMove(nFlags, point);
}


void COXSizeControlBar::RedrawCloseBtn()
{
	CRect rect=m_rectCloseBtn;
	ScreenToClient(rect);
	RedrawWindow(rect,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}

void COXSizeControlBar::RedrawResizeBtn()
{
	CRect rect=m_rectResizeBtn;
	ScreenToClient(rect);
	RedrawWindow(rect,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}

BOOL COXSizeControlBar::CanResize() const
{
	BOOL bCanResize=FALSE;
	if(m_pDockBar!=NULL && m_pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)))
	{
		CRect rect;
		GetWindowRect(rect);
		if(((COXSizeDockBar*)m_pDockBar)->
			BarsOnThisRow((CControlBar*)this,rect)>0)
		{
			bCanResize=TRUE;
		}
	}

	return bCanResize;
}

