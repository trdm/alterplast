// amsWnd.h : implementation of the CAMSWnd class
// Created by: Alvaro Mendez - 04/09/2002

#include <stdafx.h>
#include "amsWnd.h"


///////////////////////////////////////////////////////////////////////////////
// CAMSWnd

// Saves the given window's position and size to the app's profile.
void CAMSWnd::Save(CWnd* pWnd, LPCTSTR szWindowName /*= NULL*/)
{	
	ASSERT(IsWindow(pWnd->GetSafeHwnd()));

	// Set registry section
	CString strSection = szWindowName;
	if (strSection.IsEmpty())
		pWnd->GetWindowText(strSection);

	if (strSection.Find('\\') < 0)
		strSection = _T("Windows\\") + strSection;

	WINDOWPLACEMENT wp;
	pWnd->GetWindowPlacement(&wp);
	
	// Save everything
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(strSection, _T("X"), wp.rcNormalPosition.left);	
	pApp->WriteProfileInt(strSection, _T("Y"), wp.rcNormalPosition.top);	
	pApp->WriteProfileInt(strSection, _T("Width"), wp.rcNormalPosition.right - wp.rcNormalPosition.left);	
	pApp->WriteProfileInt(strSection, _T("Height"), wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);	
	pApp->WriteProfileInt(strSection, _T("State"), wp.showCmd);	
}

// Restores and sets the given window's position and/or size from the app's profile
void CAMSWnd::Restore(CWnd* pWnd, LPCTSTR szWindowName /*= NULL*/, unsigned uFlags /*= CAMSWnd::Position*/)
{	
	ASSERT(IsWindow(pWnd->GetSafeHwnd()));

	// Set registry section
	CString strSection = szWindowName;
	if (strSection.IsEmpty())
		pWnd->GetWindowText(strSection);

	if (strSection.Find('\\') < 0)
		strSection = _T("Windows\\") + strSection;

	unsigned uSWPflags = SWP_NOZORDER | ((uFlags & NoRedraw) ? SWP_NOREDRAW : 0);
	const int nInvalidValue = -1717171717;
	CWinApp* pApp = AfxGetApp();

	// Get position from registry
	int nX = pApp->GetProfileInt(strSection, _T("X"), nInvalidValue);	
	int nY = pApp->GetProfileInt(strSection, _T("Y"), nInvalidValue);	

	if (!(uFlags & CAMSWnd::Position) || (nX == nInvalidValue && nY == nInvalidValue))
		uSWPflags |= SWP_NOMOVE;

	// Get size from registry
	int nWidth = pApp->GetProfileInt(strSection, _T("Width"), nInvalidValue);	
	int nHeight = pApp->GetProfileInt(strSection, _T("Height"), nInvalidValue);	

	if (!(uFlags & CAMSWnd::Size) || (nWidth == nInvalidValue && nHeight == nInvalidValue))
		uSWPflags |= SWP_NOSIZE;

	// Set it!
	pWnd->SetWindowPos(NULL, nX, nY, nWidth, nHeight, uSWPflags);

	int nState = pApp->GetProfileInt(strSection, _T("State"), nInvalidValue);	
	if ((uFlags & CAMSWnd::State) && (nState != nInvalidValue))
	{
		WINDOWPLACEMENT wp;
		pWnd->GetWindowPlacement(&wp);

		// If the state was relevant, refresh the window with it
		wp.showCmd = nState;
		pWnd->SetWindowPlacement(&wp);
	}
}

// Changes the position and/or size of the given pWnd window by nPixels,
// The uFlags parameter determines what is being changed.
void CAMSWnd::ChangeBy(CWnd* pWnd, int nPixels, unsigned uFlags)
{
	ASSERT(IsWindow(pWnd->GetSafeHwnd()));
	ASSERT(uFlags);

	if (nPixels == 0)
		return;

	// Get window rectangle
	CRect rectWindow;
	pWnd->GetWindowRect(rectWindow);

	// Change position
	if (uFlags & CAMSWnd::Left) 
		rectWindow.left += nPixels;
	if (uFlags & CAMSWnd::Right) 
		rectWindow.right += nPixels;
	if (uFlags & CAMSWnd::Top) 
		rectWindow.top += nPixels;
	if (uFlags & CAMSWnd::Bottom) 
		rectWindow.bottom += nPixels;

	// Change size
	if (uFlags & CAMSWnd::Width) 
		rectWindow.right += nPixels;
	if (uFlags & CAMSWnd::Height) 
		rectWindow.bottom += nPixels;

	// Modify coordinates if it's a child window
	CWnd* pWndParent = pWnd->GetParent();
	if (pWndParent && pWndParent->IsChild(pWnd))
		pWndParent->ScreenToClient(rectWindow);

	// Do it!
	pWnd->MoveWindow(rectWindow, !(uFlags & NoRedraw));
}

// Aligns the given uCtrlToAlign based on the current position and/or size of the uCtrlToAlignAgainst.  Additionally the aligned
// control may be placed by nOffset pixels away from uCtrlToAlignAgainst.  
// The uFlags may be any combination of Left, Right, Top, Bottom, Width, or Height.
void CAMSWnd::AlignControl(CWnd* pWnd, unsigned uCtrlToAlign, unsigned uCtrlToAlignAgainst, unsigned uFlags, int nOffset /*= 0*/)
{
	ASSERT(pWnd);
	ASSERT(uFlags);

	// Get each control's window
	CWnd* pWndToAlign= pWnd->GetDlgItem(uCtrlToAlign);
	CWnd* pWndToAlignAgainst = pWnd->GetDlgItem(uCtrlToAlignAgainst);

	ASSERT(pWndToAlign);
	ASSERT(pWndToAlignAgainst);

	// Get each control's dimensions
	CRect rectToAlign, rectToAlignAgainst;
	pWndToAlign->GetWindowRect(rectToAlign);
	pWndToAlignAgainst->GetWindowRect(rectToAlignAgainst);
	unsigned uNoRedraw = (uFlags & CAMSWnd::NoRedraw);

	// Change each item individually
	if (uFlags & CAMSWnd::Left)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.left - rectToAlign.left) + nOffset, CAMSWnd::Left | uNoRedraw);

	if (uFlags & CAMSWnd::Top)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.top - rectToAlign.top) + nOffset, CAMSWnd::Top | uNoRedraw);

	if (uFlags & CAMSWnd::Right)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.right - rectToAlign.right) + nOffset, CAMSWnd::Right | uNoRedraw);

	if (uFlags & CAMSWnd::Bottom)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.bottom - rectToAlign.bottom) + nOffset, CAMSWnd::Bottom | uNoRedraw);

	pWndToAlign->GetWindowRect(rectToAlign);
	
	if (uFlags & CAMSWnd::Width)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.Width() - rectToAlign.Width()) + nOffset, CAMSWnd::Width | uNoRedraw);

	if (uFlags & CAMSWnd::Height)
		CAMSWnd::ChangeBy(pWndToAlign, (rectToAlignAgainst.Height() - rectToAlign.Height()) + nOffset, CAMSWnd::Height | uNoRedraw);
}

// Lines up the given set of sequentially numbered controls horizontally or vertically by the given number of pixels (nDistance).
// As a result every control ends up looking exactly like the first one but is separated by nDistance pixels below or to the right of it.
void CAMSWnd::OrganizeSequentialControls(CWnd* pWnd, unsigned uFirstCtrl, unsigned uLastCtrl, int nDistance, unsigned uFlags /*= CAMSWnd::Y*/)
{
	ASSERT(IsWindow(pWnd->GetSafeHwnd()));
	ASSERT(nDistance > 0);

	// Get the first control's rectangle
	CRect rectRef, rectCtrl;
	pWnd->GetDlgItem(uFirstCtrl)->GetWindowRect(rectRef);
	pWnd->ScreenToClient(rectRef);
	long& nIncrement = ((uFlags & CAMSWnd::Y)? rectCtrl.top : rectCtrl.left);
	BOOL bRepaint = !(uFlags & CAMSWnd::NoRedraw);

	// Loop thorugh every subsequent control and align it on the same column
	for (UINT iCtrl = uFirstCtrl + 1; iCtrl <= uLastCtrl; iCtrl++)
	{
		rectCtrl.left = rectRef.left;
		rectCtrl.top = rectRef.top;		
		nIncrement += (nDistance * (iCtrl - uFirstCtrl));

		rectCtrl.right = rectCtrl.left + rectRef.Width();
		rectCtrl.bottom = rectCtrl.top + rectRef.Height();		

		pWnd->GetDlgItem(iCtrl)->MoveWindow(rectCtrl, bRepaint);
	}
}

// Returns the number of pixels it takes to display the given text on the window's device context.
// If pFont is not NULL, it is temporarily selected into the window's device context.
int CAMSWnd::GetTextExtent(CWnd* pWnd, const CString& strText, CFont* pFont /*= NULL*/)
{
	ASSERT(IsWindow(pWnd->GetSafeHwnd()));

	int nExtent = 0;
	int nLen = strText.GetLength();

	if (nLen)
	{
		TEXTMETRIC tm;
		
		CDC* pDC = pWnd->GetDC();
		CFont* pFontOld = pDC->SelectObject(pFont ? pFont : pWnd->GetFont()); 
		pDC->GetTextMetrics(&tm);
		CSize size = pDC->GetTextExtent(strText, nLen);
		nExtent = size.cx + tm.tmAveCharWidth;
		
		pDC->SelectObject(pFontOld);
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}


///////////////////////////////////////////////////////////////////////////////
// CAMSWnd::FocusHolder

// Version 1: Constructs the object with a pointer to the window whose focus will be set later.
CAMSWnd::FocusHolder::FocusHolder(CWnd* pWnd) :
	m_pWnd(pWnd)
{
}

// Version 2: Constructs the object with a reference to the window whose focus will be set later.
CAMSWnd::FocusHolder::FocusHolder(CWnd& wnd) :
	m_pWnd(&wnd)
{
}

// Destroys the object and sets the focus to the window passed in construction.
CAMSWnd::FocusHolder::~FocusHolder()
{
	ASSERT(m_pWnd);
	if (::IsWindow(m_pWnd->m_hWnd))
		m_pWnd->SetFocus();
}


///////////////////////////////////////////////////////////////////////////////
// CAMSWnd::Disabler

// Version 1: Constructs the object with a pointer to the window to disable temporarily.
CAMSWnd::Disabler::Disabler(CWnd* pWnd) : 
	m_pWnd(pWnd) 
{ 
	ASSERT(m_pWnd);
	m_pWnd->EnableWindow(FALSE);	
}

// Version 2: Constructs the object with a reference to the window to disable temporarily.
CAMSWnd::Disabler::Disabler(CWnd& wnd) : 
	m_pWnd(&wnd) 
{ 
	m_pWnd->EnableWindow(FALSE);	
}

// Destroys the object and reenables the window.
CAMSWnd::Disabler::~Disabler() 
{ 
	if (::IsWindow(m_pWnd->m_hWnd))
		m_pWnd->EnableWindow(TRUE); 
}


///////////////////////////////////////////////////////////////////////////////
// CAMSWnd::Hook

// Map of windows which have been hooked and their corresponding Hook-derived objects.
CMap<HWND, HWND&, CAMSWnd::Hook*, CAMSWnd::Hook*&> CAMSWnd::Hook::m_mapHookedWindows;


// Constructs the object using the given pWnd object pointer.
CAMSWnd::Hook::Hook(CWnd* pWnd /*= NULL*/) :
	m_hWnd(NULL),
	m_pWndProcOld(NULL)
{
	if (pWnd)
		Open(pWnd);
}

// Destroys the object by closing the hook first.
CAMSWnd::Hook::~Hook()
{
	Close();
}

// Opens the hook for the given window by attaching self's HookedWindowProc to it
BOOL CAMSWnd::Hook::Open(CWnd* pWnd)
{
	ASSERT(pWnd);
	m_hWnd = pWnd->GetSafeHwnd();
	ASSERT(::IsWindow(m_hWnd));  // window must already exist

	if (!(m_pWndProcOld = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD)Hook::HookedWindowProc)))
		return FALSE;

	m_mapHookedWindows[m_hWnd] = this;
	return TRUE;
}

// Closes the hook and restores the old window procedure
void CAMSWnd::Hook::Close()
{
	if (!IsOpen())
		return;

	if (::IsWindow(m_hWnd))
		::SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD)m_pWndProcOld);

	m_mapHookedWindows.RemoveKey(m_hWnd);
	
	m_hWnd = NULL;
	m_pWndProcOld = NULL;
}

// Returns true if the hook has been opened
BOOL CAMSWnd::Hook::IsOpen() const
{
	return (m_hWnd && m_pWndProcOld);
}

// Returns a temporary CWnd pointer for the window which has been hooked
CWnd* CAMSWnd::Hook::GetWindow() const
{
	return CWnd::FromHandle(m_hWnd);
}

// Returns the handle of the window being hooked (if any)
HWND CAMSWnd::Hook::GetHwnd() const
{
	return m_hWnd;
}

// The window procedure through which the messages will be routed
LRESULT CALLBACK CAMSWnd::Hook::HookedWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	
	// Set up MFC message state just in case anyone wants it
	// This is just like AfxCallWindowProc, but we can't use that because
	// a CAMSWnd::Hook is not a CWnd.
	//
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	MSG  oldMsg = curMsg;   // save for nesting
	curMsg.hwnd	= hWnd;
	curMsg.message = message;
	curMsg.wParam = wParam;
	curMsg.lParam = lParam;
	
	// Get hook object for this window from the map
	Hook* pHook = NULL;
	VERIFY(m_mapHookedWindows.Lookup(hWnd, pHook));

	// pass to msg hook
	LRESULT result = pHook->WindowProc(message, wParam, lParam);

	curMsg = oldMsg;			// pop state
	return result;
}


LRESULT CAMSWnd::Hook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(m_pWndProcOld, m_hWnd, message, wParam, lParam);
}

LRESULT CAMSWnd::Hook::Default()
{
	// MFC stores current MSG in thread state
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	
	// Note: must explicitly call CAMSWnd::Hook::WindowProc to avoid infinite
	// recursion on virtual function
	return Hook::WindowProc(curMsg.message, curMsg.wParam, curMsg.lParam);
}


///////////////////////////////////////////////////////////////////////////////
// CAMSWnd::PlacementHook

// Constructs the object with no flags set
CAMSWnd::PlacementHook::PlacementHook() :
	m_uFlags(0)
{
}

// Hooks the given window and sets the flag to restore both the position and size 
// when the window is shown.
void CAMSWnd::PlacementHook::SetLastPositionAndSize(CWnd* pWnd, LPCTSTR szWindowName /*= NULL*/)
{
	Open(pWnd, szWindowName);
	m_uFlags = Both;
}

// Hooks the given window and sets the flag to restore the position when the window is shown.
void CAMSWnd::PlacementHook::SetLastPosition(CWnd* pWnd, LPCTSTR szWindowName /*= NULL*/)
{
	Open(pWnd, szWindowName);
	m_uFlags = Position;
}

// Hooks the given window and sets the flag to restore the size when the window is shown.
void CAMSWnd::PlacementHook::SetLastSize(CWnd* pWnd, LPCTSTR szWindowName /*= NULL*/)
{
	Open(pWnd, szWindowName);
	m_uFlags = Size;
}

// Hooks the given window and stores the window name to use for saving/restoring in the app's profile.
void CAMSWnd::PlacementHook::Open(CWnd* pWnd, LPCTSTR szWindowName)
{
	VERIFY(Hook::Open(pWnd));
	
	if (szWindowName && *szWindowName)
		m_strWindowName = szWindowName;
	else
		pWnd->GetWindowText(m_strWindowName);	
}

// Handles the window's show/destroy messages to restore/save the window's position and/or size.
LRESULT CAMSWnd::PlacementHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SHOWWINDOW:
			if (wParam)
				CAMSWnd::Restore(GetWindow(), m_strWindowName, m_uFlags);
			break;

		case WM_DESTROY:
			CAMSWnd::Save(GetWindow(), m_strWindowName);
			break;
	}

	return Hook::WindowProc(message, wParam, lParam);
}
