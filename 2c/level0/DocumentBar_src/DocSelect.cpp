// This file was originally named SwitcherWnd.cpp
//
// Programmed by: JIMMY BRUSH (Kathy007@email.msn.com)
// 
// Legal:
//
// THIS CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
// You may use and distribute this code provided that you do not
// remove this title header and that you do not charge money for
// it. If you want to update the code, feel free to do so, as long
// as you *mark your changes* in code AND in the revision log below
// (and send it to me ;)
//
//
//
// Version: 1.0 revision 1
//
// Revision Log:
//
// SUN MAR 14 1999 - JIMMY BRUSH -  Finished Writing version 1.0
// MON MAR 15 1999 - JIMMY BRUSH -  Fixed RemoveButton to correctly decrement selected button
//									Added CMemDC by Keith Rule
//									Fixed up Animation a bit
//
//
//
// In the next version \ Wish List:
//
// 1. Tool Tips in CSwitcherButton
// 2. Support for more buttons than can display (with the up/down button at the right)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifications by Yogesh Jagota.
// All my modifications in the existing code...
//	 a) either start with 'Yogesh Jagota' and end with 'End Yogesh Jagota',
//	 b) or, are commented in the end of line with 'Yogesh Jagota',
//
//						/************************
// Everything after the  * All the following... * box is added by me.
//						 ************************/
//
//  Sn. Description											Date
// ==== =================================================== ===============
//   1.	Renamed the file and header to DocSelect and class 
//		to CDocSelector (Reason: the purpose of the program 
//		now was to provide a document selector, not a 
//		switcher window.)									06th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 2. Totally changed the class to turn the base class to 
//		CControlBar.										06th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 3. Now passing full path as tooltip, and filename as 
//		the button text. (assuming that a filename is 
//		passed to the function.)							07th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 4. Removed a feature, that when user clicks on the 
//		empty part of DocSelector, the current selection is
//		removed. The code to do this was defined in 
//		OnLButtonDown. If you like to add this feature, add
//		handler for OnLButtonUp and replace it with :-
//
//		void CDocSelector::OnLButtonUp(UINT nFlags, CPoint point) 
//		{
//			CWnd::OnLButtonUp(nFlags, point);
//
//			if (m_iSelectedButton > -1)
//				GetButtonFromID(m_iSelectedButton)->Unselect();
//
//			m_iSelectedButton = -1;
//		}													08th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 5. Now buttons outside screen range will not be drawn.	08th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 6. Added a Menu button in the end of the CDocSelector
//		to show buttons outside screen range.				08th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 7. Now the user has to pass just a pointer to the view
//		instead of filename and all that unwanted stuff is
//		handled by AddButton.								09th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 8. Added a OnTimer handler. This reduces the overload 
//		for the user to change the state of buttons in 
//		OnActivate and OnDeactivate. The only things, the 
//		user has to do now are 1) to create the button in
//		OnInitialUpdate and 2) to destroy it in OnDestroy.	09th Mar, 2000
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../resource.h"
#include "docselect.h"
#include "switcherbutton.h"
#include "MenuButton.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MINWIDTH		60


// Yogesh Jagota
#define DS_HEIGHT				22		// Default is 22.
#define DS_WIDTH				180		// Default is 110.
#define DS_LEFT_MARGIN			2		// Default is 2.
#define DS_MENU_BUTTON_WIDTH	12		// Default is 12.
#define DS_MENU_BUTTON_ID		20001	// Default is 20001.
#define DS_TIMER_ID				1		// Default is 1.

#define DS_SEPERATER_MARGIN		1		
		// Default is 1. Please don't modify this. 
		// although it will work nicely if you do, 
		// but it will easily destroy the looks of
		// the document bar.

// End Yogesh Jagota

/////////////////////////////////////////////////////////////////////////////
// CDocSelector

CDocSelector::CDocSelector()
{
	m_cxAvailable		= 0;
	m_iButtonWidth		= DS_WIDTH;
	m_iNextButtonStart	= DS_LEFT_MARGIN;
	m_iSelectedButton	= -1;
	m_nDisplayedButtons	= 0; // Yogesh Jagota
}

CDocSelector::~CDocSelector()
{
}


BEGIN_MESSAGE_MAP(CDocSelector, CControlBar)
	//{{AFX_MSG_MAP(CDocSelector)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SWM_SELCHANGE, OnSelChange)
	ON_MESSAGE(SWM_UNSELECT, OnUnselect)
	ON_MESSAGE(ID_DS_POPUP_BUTTON, OnDSPopup)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDocSelector message handlers
BOOL CDocSelector::AddButton( CWnd* wnd, WORD wIcon ,HICON hIcon )
{
	CRect rect;
	GetClientRect(&rect);

	// Yogesh Jagota
	CDocument* pDoc = ((CView *) wnd)->GetDocument();

	CString sPath = pDoc->GetPathName();
	CString sFileName;
	
	if ( sPath.IsEmpty() )
		sPath = pDoc->GetTitle();

	// Can be only the filename, like Noname01
	if ( sPath.Find( _T('\\') ) != -1 )
		sFileName = sPath.Mid( sPath.ReverseFind( _T('\\') ) + 1 );
	else
		sFileName = sPath;

	// If there is no space left to display extra buttons...
	int nNewSize = ( ( rect.Width() - DS_MENU_BUTTON_WIDTH ) 
		/ ( m_Buttons.GetSize() + 1 ) ) - DS_SEPERATER_MARGIN;

	bool bShowButton = true;
	if ( nNewSize <= MINWIDTH )
	{
		// Check if the menu button is displayed, if not, display it...
		m_btnMenuButton.ShowWindow( SW_SHOW );

		// Don't show the button...
		bShowButton = false;
	}
	else
		m_nDisplayedButtons++;

	// End Yogesh Jagota

	CSwitcherButton* newbutton = new CSwitcherButton();

	 // Yogesh Jagota
	newbutton->m_AttachedView = wnd;

	// I am saving the frame to make life easier in case
	// of activation in selection process...
	CMDIFrameWnd *pFrame = (CMDIFrameWnd *)AfxGetMainWnd();
	CWnd * pWnd = pFrame->GetWindow( GW_CHILD );
	ASSERT (pWnd);
	pWnd = pWnd->GetWindow( GW_CHILD );
	while (pWnd)
	{
		if ( ((CFrameWnd *)pWnd)->GetActiveView() == (CView*)wnd )
			newbutton->m_FrameWnd = (CFrameWnd *)pWnd;

		pWnd = pWnd->GetWindow( GW_HWNDNEXT );
	}

	// Load the icon....
	if ( hIcon)
	{
		newbutton->m_iIcon=hIcon;
		newbutton->m_iIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	}
	
	else

	if ( wIcon != -1 )
		newbutton->m_iIcon = ::LoadIcon( AfxGetInstanceHandle(), 
			MAKEINTRESOURCE( wIcon ) );
	// End Yogesh Jagota

	newbutton->m_iID = m_Buttons.GetSize();
	if (!newbutton->DoCreate(this, m_iNextButtonStart, 
		rect.top + 3, m_iButtonWidth, DS_HEIGHT - 2, 
		sFileName, sPath )) // Yogesh Jagota
	{
		return FALSE;
	}

	if ( !bShowButton )
		newbutton->ShowWindow( SW_HIDE );

	m_Buttons.Add((void*)newbutton);
	m_iNextButtonStart += m_iButtonWidth + DS_SEPERATER_MARGIN;

	if (m_iNextButtonStart - DS_SEPERATER_MARGIN > rect.Width())
	{
		// this loop makes a neat little animation
		int newsize = ((rect.Width() - DS_MENU_BUTTON_WIDTH ) 
			/ (m_Buttons.GetSize())) - DS_SEPERATER_MARGIN;

//		Yogesh Jagota. Removed animation because did'nt liked it.
//		Remove if you want animated addition or removal of buttons.
//
//		register int y;
//		for (y = m_iButtonWidth; y >= newsize; y-=3)
//		{
//			ResizeButtons(y);
//			Sleep(15);
//		}
//
//		if (y != newsize)
//			/* ResizeButtons(newsize); */ <- this is the next line...
//		End Yogesh Jagota

		ResizeButtons(newsize);

		if (m_iButtonWidth < MINWIDTH)
		{
			// the smallest allowable button size has been reached...
			// in this version, we can't handle this
			ASSERT(0);
			return FALSE;
		}
	}
	else
		m_btnMenuButton.ShowWindow( SW_HIDE );

	// Yogesh Jagota
	if ( m_iSelectedButton != -1 )
		((CSwitcherButton *) m_Buttons.GetAt( m_iSelectedButton ))->Unselect();

	m_iSelectedButton = newbutton->m_iID;
	((CSwitcherButton *) m_Buttons.GetAt( m_iSelectedButton ))->Select();
	// End Yogesh Jagota

	return TRUE;
}

void CDocSelector::OnDestroy() 
{
	KillTimer( DS_TIMER_ID ); // Yogesh Jagota
	
	RemoveAll();
	CControlBar::OnDestroy();
}

void CDocSelector::RemoveAll()
{
	// remove the first button over and over
	int size = m_Buttons.GetSize();
	for (int x = 1; x <= size; x++)
	{
		CSwitcherButton* btn = (CSwitcherButton*)m_Buttons.GetAt(0);
		delete btn;
		m_Buttons.RemoveAt(0);
	}

	m_iNextButtonStart	= 0;
	m_iButtonWidth		= DS_WIDTH;
	m_iSelectedButton	= -1;
	m_nDisplayedButtons = 0; // Yogesh Jagota
}

void CDocSelector::ResizeButtons(int NewSize)
{
//	Yogesh Jagota
//	if (NewSize < MINWIDTH || NewSize > DS_WIDTH)
//		return;
//	End Yogesh Jagota

	// Yogesh Jagota
	// We are having good space to draw the buttons...
	if ( NewSize > DS_WIDTH )
	{
		m_btnMenuButton.ShowWindow( SW_HIDE );
		return;
	}

	// Oh no! we are not...
	if ( NewSize < MINWIDTH )
	{
		m_btnMenuButton.ShowWindow( SW_SHOW );
		return;
	}

	CRect rRect;
	GetClientRect( &rRect );

	int nShow = SW_HIDE;
	// End Yogesh Jagota

	m_iButtonWidth = NewSize;
	m_iNextButtonStart = DS_LEFT_MARGIN;

	m_nDisplayedButtons = 0; // Yogesh Jagota
	for (register int x = 0; x < m_Buttons.GetSize(); x++)
	{
		CSwitcherButton* btn = (CSwitcherButton*)m_Buttons.GetAt(x);
		btn->SetWindowPos(NULL, m_iNextButtonStart, 3, 
			m_iButtonWidth, DS_HEIGHT - 2, SWP_NOZORDER|SWP_NOCOPYBITS);

		m_iNextButtonStart += m_iButtonWidth + DS_SEPERATER_MARGIN;

		// Yogesh Jagota
		btn->ShowWindow( SW_SHOW );

		if ( m_iNextButtonStart > ( rRect.Width() - DS_MENU_BUTTON_WIDTH ) )
		{
			for ( int i = x; i < m_Buttons.GetSize(); i++)
			{
				CSwitcherButton* btn = ( CSwitcherButton *) m_Buttons.GetAt( i );

				btn->ShowWindow( SW_HIDE );
			}

			nShow = SW_SHOW;
			break;
		}

		m_nDisplayedButtons++;
		// End Yogesh Jagota
	}

	RedrawWindow();

	m_btnMenuButton.ShowWindow( nShow ); // Yogesh Jagota
}

void CDocSelector::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	
	// Yogesh Jagota
	CRect rc;
	GetWindowRect(&rc);
	
	m_cxAvailable = rc.Width();

	ScreenToClient(&rc);

	rc.top		+= 3;
	rc.bottom	-= 1;
	rc.right	-= 2;
	rc.left		= ( rc.right - DS_MENU_BUTTON_WIDTH ) - 1;

	m_btnMenuButton.MoveWindow( &rc );
	// End Yogesh Jagota

	if (m_Buttons.GetSize() == 0)
		return;

	CRect rect;
	GetClientRect(&rect);

//	Yogesh Jagota
//	int newsize = ((rect.Width() - DS_MENU_BUTTON_WIDTH ) 
//		/ (m_Buttons.GetSize())) - DS_SEPERATER_MARGIN;
//
//	if (newsize <= DS_WIDTH && m_iButtonWidth != newsize)
//		ResizeButtons(newsize);
//	End Yogesh Jagota

	// Yogesh Jagota
	int nSize	= m_Buttons.GetSize();
	int newsize = ( ( rect.Width() - 
		( DS_MENU_BUTTON_WIDTH + DS_LEFT_MARGIN ) ) -
		( nSize * DS_SEPERATER_MARGIN ) ) / nSize;

	if ( newsize < MINWIDTH )
		ResizeButtons( MINWIDTH );
	else if ( newsize > DS_WIDTH )
		ResizeButtons( DS_WIDTH );
	else if ( newsize != m_iButtonWidth )
		ResizeButtons( newsize );
	// End Yogesh Jagota
}

BOOL CDocSelector::RemoveButton(CWnd* wnd)
{
	// Yogesh Jagota
	int index = -1;

	for ( int i = 0; i < m_Buttons.GetSize(); i++ )
	{
		if ( GetButtonFromID( i )->m_AttachedView == wnd )
		{
			index = i;
			break;
		}
	}

	if ( index == -1 )
		return FALSE;
	// End Yogesh Jagota

	if (index >= m_Buttons.GetSize() || index < 0)
	{
		ASSERT(0); // out of range
		return FALSE;
	}

	if (m_iSelectedButton == index)
		m_iSelectedButton = -1;
	if (m_iSelectedButton > index)
		m_iSelectedButton -= 1;

	delete GetButtonFromID(index);
	m_Buttons.RemoveAt(index);

	for (register int x = index; x < m_Buttons.GetSize(); x++)
		GetButtonFromID(x)->m_iID -= 1;

	m_iNextButtonStart -= m_iButtonWidth + 1;

	if (m_iButtonWidth != DS_WIDTH)
	{
		// do that funky animation thing
		CRect rect;
		GetClientRect(&rect);
		int newsize = ((rect.Width() - DS_MENU_BUTTON_WIDTH ) 
			/ (m_Buttons.GetSize())) - DS_SEPERATER_MARGIN;

		if (newsize > DS_WIDTH) newsize = DS_WIDTH;


//		Yogesh Jagota. Removed animation because did'nt liked it.
//		Remove if you want animated addition or removal of buttons.
//
//		this loop is shortened to make the following loop....
//		if (newsize > m_iButtonWidth)
//		{
//			register int y;
//			for (y = m_iButtonWidth; y <= newsize; y+=3)
//			{
//				ResizeButtons(y);
//				Sleep(15);
//			}
//			if (y != newsize)
//				ResizeButtons(newsize);
//		}
//		End Yogesh Jagota

		if (newsize > m_iButtonWidth)
			ResizeButtons(newsize);
	}

	ResizeButtons(m_iButtonWidth);

	return TRUE;
}

CSwitcherButton* CDocSelector::GetButtonFromID(int id)
{
	return (CSwitcherButton*)m_Buttons.GetAt(id);
}

LRESULT CDocSelector::OnSelChange(WPARAM wParam, LPARAM)
{
	// Killing the timer, because timer can change the
	// selection in between the procedure...
	KillTimer( DS_TIMER_ID ); // Yogesh Jagota

	// sent when a button gets clicked
	CSwitcherButton* newsel = (CSwitcherButton*)wParam;

	// Yogesh Jagota
	CMDIFrameWnd *pFrame = (CMDIFrameWnd *)AfxGetMainWnd();
	//CWnd* Window = (CWnd*)newsel->m_FrameWnd;//Y.Ivanov
	CWnd* Window = (CWnd*)newsel->m_AttachedView->GetParentFrame();//Yuriy Ivanov
	pFrame->MDIActivate( Window );
	if(Window->IsIconic())
		Window->ShowWindow(SW_SHOWNORMAL);
	pFrame->Invalidate();

	// End Yogesh Jagota

	if (m_iSelectedButton > -1)
		GetButtonFromID(m_iSelectedButton)->Unselect();

	m_iSelectedButton = newsel->m_iID;

	// Reactivate the timer...
	SetTimer( DS_TIMER_ID, 10, NULL ); // Yogesh Jagota


	return 1;
}

LRESULT CDocSelector::OnUnselect(WPARAM, LPARAM)
{
	// sent when a button gets clicked when its selected
	// (which unselects it)
	m_iSelectedButton = -1;
	return 1;
}

/************************************************************************
 ************************************************************************
 *																		*
 * All following code is added by Yogesh Jagota							*
 *																		*
 ************************************************************************
 ************************************************************************/
int CDocSelector::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CControlBar::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	
	ModifyStyleEx(0, WS_EX_WINDOWEDGE );

	if ( !m_btnMenuButton.Create( NULL, WS_VISIBLE | BS_BITMAP |
		BS_OWNERDRAW | BS_CENTER | BS_VCENTER,
		CRect( 0, 0, 0, 0 ), this, DS_MENU_BUTTON_ID ) )
	{
		return -1;
	}

	SetTimer( DS_TIMER_ID, 10, NULL );

	OnTimer( DS_TIMER_ID );

	return 0;
}

LRESULT CDocSelector::OnDSPopup( WPARAM, LPARAM )
{
	m_btnMenuButton.SetState( TRUE );

	CMenu pPopup;
	CString strText;
	UINT nFlags;

	pPopup.CreatePopupMenu();

	for ( int i = m_nDisplayedButtons; i < m_Buttons.GetSize(); i++ )
	{
		((CSwitcherButton *) m_Buttons.GetAt( i ))->GetWindowText( strText );

		if ( m_iSelectedButton == i )
			nFlags = MF_CHECKED | MF_STRING;
		else
			nFlags = MF_STRING;

		pPopup.AppendMenu( nFlags, i + 1, strText );
	}

	CRect rRect;
	m_btnMenuButton.GetWindowRect( &rRect );

	int nChoice = ::TrackPopupMenu( pPopup.GetSafeHmenu(),
		TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rRect.right, rRect.top, 0, this->GetSafeHwnd(), &rRect );

	if ( nChoice != 0 ) 
	{
		nChoice--;

		if ( m_iSelectedButton != nChoice )
			OnSelChange( (WPARAM)m_Buttons.GetAt( nChoice ),0 );
	}

	m_btnMenuButton.SetState( FALSE );
	return 1;
}

CSize CDocSelector::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	// Get border size (values will be negative)
	CRect rBorder;
	rBorder.SetRectEmpty();
	CalcInsideRect( rBorder, bHorz );

	// The Y value is the sum of the calculated height from DrawText,
	// plus the top and bottom border.
	CSize size;
	size.cx = 32767;
	size.cy = DS_HEIGHT;
	size.cy += ( -rBorder.Height() );

	return size;
}

void CDocSelector::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler )
{
	CCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;		// there's only one thing to update
	state.m_nID = AFX_IDW_STATUS_BAR;

	// allow the statusbar itself to have update handlers
	if ( CControlBar::OnCmdMsg( state.m_nID, CN_UPDATE_COMMAND_UI, 
		&state, NULL ) )
		return;

	// allow target (owner) to handle the remaining updates
	state.DoUpdate( pTarget, FALSE );
}

BOOL CDocSelector::OnEraseBkgnd( CDC* pDC ) 
{
	CRect rect;
	GetClientRect( rect );
	pDC->FillSolidRect( rect, GetSysColor( COLOR_MENU ) );

	return CControlBar::OnEraseBkgnd( pDC );
}

void CDocSelector::OnPaint() 
{
	CPaintDC dc( this ); // device context for painting

	CRect r;
	GetClientRect( r );

	// Draw Seperator line from others controls.
	dc.Draw3dRect( r.left, r.top, r.right, r.top + 2, 
		GetSysColor( COLOR_3DSHADOW ), GetSysColor( COLOR_3DHILIGHT ) );
}

void CDocSelector::OnTimer(UINT nIDEvent) 
{
	for ( int i = 0; i < m_Buttons.GetSize(); i++ )
	{
		CSwitcherButton* wndButton = 
			(CSwitcherButton *) m_Buttons.GetAt( i );

		// May be the user has destroyed the view...
		if ( wndButton->m_AttachedView )
		{
			if ( GetFocus() == wndButton->m_AttachedView )
			{
				if ( wndButton->m_nState != SWITCHBUTTON_SELECTED )
				{
					wndButton->Select();

					m_iSelectedButton = i;
				}
			}
			else
			{
				if ( wndButton->m_nState == SWITCHBUTTON_SELECTED )
					wndButton->Unselect();
			}
		}
		// Yes, he has, critical condition..., break...
		else
			break;
	}

	CControlBar::OnTimer(nIDEvent);
}
