#include "stdafx.h"
#include "..\resource.h"
#include "WindowManager.h"
#include "ViewManager.h"
#include "PopupMenu.h"
#include "..\MainFrm.h"          // TODO: include your main window frame header file here.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Globals

// Please change this to a more useful logo text for your application
//static TCHAR szLogoString[] = _T("Based on codes by I. Apollonio, I. Zhakov, et al.");
static TCHAR szLogoString[] = _T("VTOOLS.RU");

// Helpers for saving/restoring window state

static TCHAR szSection[]     = _T("Settings");
static TCHAR szChildWinPos[] = _T("TVChildWinState");
static TCHAR szLogoFont[]    = _T("TVLogoFont");
static TCHAR szLogoColor[]   = _T("TVLogoColor");
static TCHAR szWindowPos[]   = _T("TVWindowPos");
static TCHAR szDispType[]    = _T("TVDispType");
static TCHAR szFileName[]    = _T("TVFileName");
static TCHAR szBackColor[]   = _T("TVBackColor");
static TCHAR szBkBitmap[]    = _T("TVBkBitmap");
static TCHAR szFormat[]      = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");

static BOOL ReadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString(szSection, szWindowPos);
	if (strBuffer.IsEmpty())
		return FALSE;
	
	WINDOWPLACEMENT wp;
	
	int nRead = _stscanf(strBuffer, szFormat,
		&wp.flags, &wp.showCmd,
		&wp.ptMinPosition.x, &wp.ptMinPosition.y,
		&wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
		&wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
		&wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);
	
	if (nRead != 10)
		return FALSE;
	
	wp.length = sizeof(WINDOWPLACEMENT);
	*pwp = wp;
	return TRUE;
}

static void WriteWindowPlacement(LPWINDOWPLACEMENT pwp)
	// write a window placement to settings section of app's ini file
{
	TCHAR szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];
	
	wsprintf(szBuffer, szFormat,
		pwp->flags, pwp->showCmd,
		pwp->ptMinPosition.x, pwp->ptMinPosition.y,
		pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
		pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
		pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString(szSection, szWindowPos, szBuffer);
}

///////////////////////////////////////////////////////////
// CDocumentList: class implementation

CDocumentList::CDocumentList()
{
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	
	// Get the first document template position and iterate through the document template
	POSITION posDocTemplate = pApp->GetFirstDocTemplatePosition();
	while (posDocTemplate != NULL)
	{
		// For each document template object found ...
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posDocTemplate);
		ASSERT_VALID(pDocTemplate);
		ASSERT_KINDOF(CDocTemplate, pDocTemplate);
		
		// ...iterate through the template's document list
		POSITION posDocument = pDocTemplate->GetFirstDocPosition();
		while (posDocument != NULL)
		{
			// And for each document object found...
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDocument);
			ASSERT_VALID(pDoc);
			ASSERT_KINDOF(CDocument, pDoc);
			
			// ...add the document pointer to the list
			AddTail(pDoc);
		}
	}
	
	// Finally, set the position of the first list member as the current 
	m_CurPosInDocList = GetHeadPosition();
}

CDocumentList::~CDocumentList()
{
	// Out of scope or deleted, remove all document templates...
	RemoveAll();
	//...set the current position to NULL.
	m_CurPosInDocList = NULL;
}

CDocument* CDocumentList::GetNextDocument()
{
	if (m_CurPosInDocList == NULL)
		return NULL;
	
	CDocument* pDoc = GetNext(m_CurPosInDocList);
	ASSERT_VALID(pDoc);
	ASSERT_KINDOF(CDocument, pDoc);
	
	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CWindowDlg dialog

CWindowDlg::CWindowDlg(CMDIFrameWnd * pMDIFrame)
	: CDialog(CWindowDlg::IDD, pMDIFrame)
{
	m_pMDIFrame = pMDIFrame;
	//{{AFX_DATA_INIT(CWindowDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWindowDlg::CWindowDlg(CMDIFrameWnd* pMDIFrame, CWnd* pParentWnd)
	: CDialog(CWindowDlg::IDD, pParentWnd)
{
	m_pMDIFrame = pMDIFrame;
	//{{AFX_DATA_INIT(CWindowDlg)
	//}}AFX_DATA_INIT
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowDlg)
	DDX_Control(pDX, IDC_WINDOWLIST_LIST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialog)
	//{{AFX_MSG_MAP(CWindowDlg)
	ON_BN_CLICKED(IDC_WINDOWLIST_CLOSE, OnClose)
	ON_LBN_SELCHANGE(IDC_WINDOWLIST_LIST, OnSelChange)
	ON_BN_CLICKED(IDC_WINDOWLIST_SAVE, OnSave)
	ON_BN_CLICKED(IDC_WINDOWLIST_ACTIVATE, OnActivate)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_WINDOWLIST_TILEHORZ, OnTileHorz)
	ON_BN_CLICKED(IDC_WINDOWLIST_MINIMIZE, OnMinimize)
	ON_BN_CLICKED(IDC_WINDOWLIST_TILEVERT, OnTileVert)
	ON_BN_CLICKED(IDC_WINDOWLIST_CASCADE, OnCascade)
	ON_LBN_DBLCLK(IDC_WINDOWLIST_LIST, OnActivate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowDlg message handlers
				 
BOOL CWindowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	FillWindowList(); 	
	SelActive();
	UpdateButtons();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWindowDlg::OnClose() 
{
	int	nItems = m_wndList.GetCount();
	if (nItems != LB_ERR && nItems > 0)
	{
		HWND hMDIClient = m_pMDIFrame->m_hWndMDIClient;
		
		m_wndList.SetRedraw(FALSE);
		
		for (int i = nItems - 1; i >= 0; i--)
		{
			if (m_wndList.GetSel(i) > 0)
			{					   
				HWND hWnd = reinterpret_cast<HWND>(m_wndList.GetItemData(i));
				::SendMessage(hWnd, WM_CLOSE, static_cast<WPARAM>(0), 
					static_cast <LPARAM>(0));
				if (::GetParent(hWnd) == hMDIClient) 
					break;
			}				  
		}
		m_wndList.SetRedraw(TRUE);
	}
	FillWindowList();
	SelActive();
	UpdateButtons();
}

void CWindowDlg::OnSelChange() 
{
	UpdateButtons();
}
// Enables/Disables states of buttons
void CWindowDlg::UpdateButtons()
{						   
	int	nSel = m_wndList.GetSelCount();
	
	GetDlgItem(IDC_WINDOWLIST_CLOSE)->EnableWindow(nSel > 0);	
	GetDlgItem(IDC_WINDOWLIST_SAVE)->EnableWindow(nSel > 0);
	GetDlgItem(IDC_WINDOWLIST_TILEHORZ)->EnableWindow(nSel >= 2);
	GetDlgItem(IDC_WINDOWLIST_TILEVERT)->EnableWindow(nSel >= 2);
	GetDlgItem(IDC_WINDOWLIST_CASCADE)->EnableWindow(nSel >= 2);
	GetDlgItem(IDC_WINDOWLIST_MINIMIZE)->EnableWindow(nSel > 0);
	
	GetDlgItem(IDC_WINDOWLIST_ACTIVATE)->EnableWindow(nSel == 1);
}

// Selects currently active window in listbox
void CWindowDlg::SelActive()
{
	int	nItems = m_wndList.GetCount();
	if (nItems != LB_ERR && nItems > 0)
	{
		m_wndList.SetRedraw(FALSE);
		m_wndList.SelItemRange(FALSE, 0, nItems - 1);
		
		HWND hwndActive = reinterpret_cast<HWND>(::SendMessage(m_pMDIFrame->m_hWndMDIClient,
			WM_MDIGETACTIVE, 0, 0));
		
		for (int i = 0; i < nItems; i++) 
		{
			if ((HWND) m_wndList.GetItemData(i) == hwndActive)  
			{
				m_wndList.SetSel(i);
				break;
			}
		}
		m_wndList.SetRedraw(TRUE);
	}
}

// Saves selected documents
void CWindowDlg::OnSave() 
{
	int	nItems = m_wndList.GetCount();
	if (nItems != LB_ERR && nItems > 0)
	{
		for (int i = 0; i < nItems; i++)
		{
			if (m_wndList.GetSel(i) > 0)
			{
				HWND	   hWnd   = reinterpret_cast<HWND>(m_wndList.GetItemData(i));
				CWnd*	   pWnd   = CWnd::FromHandle(hWnd);
				CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
				if (pFrame != NULL)
				{
					CDocument* pDoc = pFrame->GetActiveDocument();
					if (pDoc != NULL) 
						pDoc->SaveModified();
				}
			}
		}
	}
	
	FillWindowList();
	SelActive();
	UpdateButtons();
}

void CWindowDlg::OnActivate() 
{
	if (m_wndList.GetSelCount() == 1)
	{
		int	index;	
		if (m_wndList.GetSelItems(1, &index) == 1)
		{						 
			DWORD dw = m_wndList.GetItemData(index);
			if (dw != LB_ERR)
			{
				WINDOWPLACEMENT	wndpl;
				wndpl.length = sizeof(WINDOWPLACEMENT);
				::GetWindowPlacement(reinterpret_cast<HWND>(dw), &wndpl);
				if (wndpl.showCmd == SW_SHOWMINIMIZED) 
					::ShowWindow(reinterpret_cast<HWND>(dw), SW_RESTORE);
				::SendMessage(m_pMDIFrame->m_hWndMDIClient, WM_MDIACTIVATE, 
					static_cast<WPARAM>(dw), 0);
				EndDialog(IDOK);
			}
		}
	}
}

// Refresh windows list
void CWindowDlg::FillWindowList(void)
{
	m_wndList.SetRedraw(FALSE);
	m_wndList.ResetContent();
	HWND hwndT;
	hwndT = ::GetWindow(m_pMDIFrame->m_hWndMDIClient, GW_CHILD);
	while (hwndT != NULL)
	{
		TCHAR szWndTitle[_MAX_PATH];
		::GetWindowText(hwndT, szWndTitle, sizeof(szWndTitle)/sizeof(szWndTitle[0]));
		
		int index = m_wndList.AddString(szWndTitle);
		m_wndList.SetItemData(index, reinterpret_cast<DWORD>(hwndT));
		hwndT = ::GetWindow(hwndT, GW_HWNDNEXT);
	}
	m_wndList.SetRedraw(TRUE);
}

// Draws listbox item
void CWindowDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS) 
{
	if (nIDCtl == IDC_WINDOWLIST_LIST)
	{
		if (lpDIS->itemID == LB_ERR) 
			return; 
		
		HBRUSH	brBackground;
		RECT	rcTemp = lpDIS->rcItem; 
		HDC		hDC    = lpDIS->hDC;
		
		COLORREF	clText; 
		
		if (lpDIS->itemState & ODS_SELECTED)  
		{ 
			brBackground = GetSysColorBrush(COLOR_HIGHLIGHT); 
			clText       = GetSysColor(COLOR_HIGHLIGHTTEXT); 
		} 
		else 
		{ 
			brBackground = GetSysColorBrush(COLOR_WINDOW); 
			clText       = GetSysColor(COLOR_WINDOWTEXT); 
		} 
		
		if (lpDIS->itemAction &(ODA_DRAWENTIRE | ODA_SELECT)) 	
			FillRect(hDC, &rcTemp, brBackground); 
		
		int		 OldBkMode = ::SetBkMode(hDC, TRANSPARENT); 
		COLORREF clOldText = ::SetTextColor(hDC, clText); 
		
		TCHAR szBuf[1024];
		::SendMessage(lpDIS->hwndItem, LB_GETTEXT, static_cast<WPARAM>(lpDIS->itemID), 
			reinterpret_cast<LPARAM>(szBuf));		
		
		int h = rcTemp.bottom - rcTemp.top;
		rcTemp.left += h + 4;
		DrawText(hDC, szBuf, -1, &rcTemp, DT_LEFT | DT_VCENTER |	
			DT_NOPREFIX | DT_SINGLELINE);
		
		HICON hIcon = reinterpret_cast<HICON>
			(::GetClassLong(reinterpret_cast<HWND>(lpDIS->itemData), GCL_HICONSM));
		/*AfxGetApp()->LoadStandardIcon(IDI_HAND); */
		//(HICON) ::SendMessage((HWND) lpDIS->itemData,WM_GETICON,(WPARAM)ICON_BIG,(LPARAM) 0);
		rcTemp.left = lpDIS->rcItem.left;
		::DrawIconEx(hDC, rcTemp.left + 2, rcTemp.top, hIcon, h, h, 0, 0, DI_NORMAL);		
		
		::SetTextColor(hDC, clOldText);
		::SetBkMode(hDC, OldBkMode);
		
		if (lpDIS->itemAction & ODA_FOCUS)   
			DrawFocusRect(hDC, &lpDIS->rcItem); 
		return;		
	}
	CDialog::OnDrawItem(nIDCtl, lpDIS);
}

void CWindowDlg::MDIMessage(UINT uMsg, WPARAM flag)
{
	int	nItems = m_wndList.GetCount();
	if (nItems != LB_ERR && nItems > 0)
	{
		HWND hMDIClient = m_pMDIFrame->m_hWndMDIClient;
		::LockWindowUpdate(hMDIClient);
		for (int i = nItems - 1; i >= 0; i--)
		{
			HWND hWnd = reinterpret_cast<HWND>(m_wndList.GetItemData(i));
			if (m_wndList.GetSel(i) > 0)	
				::ShowWindow(hWnd, SW_RESTORE);
			else						
				::ShowWindow(hWnd, SW_MINIMIZE);
		}
		::SendMessage(hMDIClient, uMsg, flag, 0);	
		::LockWindowUpdate(NULL);
	}
}

void CWindowDlg::OnTileHorz() 
{
	MDIMessage(WM_MDITILE, MDITILE_HORIZONTAL);	
}

void CWindowDlg::OnTileVert() 
{
	MDIMessage(WM_MDITILE, MDITILE_VERTICAL);	
}

void CWindowDlg::OnMinimize() 
{
	int	nItems = m_wndList.GetCount();
	if (nItems != LB_ERR && nItems > 0)
	{
		m_wndList.SetRedraw(FALSE);
		
		for (int i = nItems - 1; i >= 0; i--)
		{
			if (m_wndList.GetSel(i) > 0)
			{
				HWND hWnd = reinterpret_cast < HWND>(m_wndList.GetItemData(i));
				::ShowWindow(hWnd, SW_MINIMIZE);
			}
		}
		m_wndList.SetRedraw(TRUE);
	}
	
	FillWindowList();
	SelActive();
	UpdateButtons();
}

void CWindowDlg::OnCascade() 
{
	MDIMessage(WM_MDICASCADE, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CMDIClient

CMDIClient::CMDIClient() : m_sizeClient(0, 0), m_bFullScreen(FALSE), 
	m_bFirstTime(TRUE), m_bBkBitmap(TRUE), m_pwndControlBars(NULL)
{
	
	m_hMenuWindow         = 0;
	m_pMDIFrame           = NULL;
	m_bMaxChild           = FALSE;
	m_pwndFullScrnToolBar = NULL;
	m_strWindows.LoadString(IDS_WINDOW_MANAGE);

/*	m_hMenuWindow = 0;
	m_strWindows.LoadString(IDS_WINDOW_MANAGE);
	m_pMDIFrame = NULL;
	m_crBkColor           = GetSysColor(COLOR_DESKTOP);
	m_crLogoColor         = GetSysColor(COLOR_BTNFACE);

	m_bMaxChild           = FALSE;
	m_pwndFullScrnToolBar = NULL;
*/
	// Do defaults settings
	Defaults();
	
////////////////

	// Create the default font, Times New Roman is most common
//	CWindowDC wndDC(CWnd::GetDesktopWindow());
//	int nFontSize = -MulDiv(18, wndDC.GetDeviceCaps(LOGPIXELSY), 72);
//	m_fontLogo.CreateFont(nFontSize, 
//		0, 0, 0, 
//		FW_BOLD, 
//		FALSE, FALSE, FALSE,
//		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//		FIXED_PITCH | FF_ROMAN, 
//		_T("Times New Roman"));


	// Now do the registry stuff here...
	CWinApp* pApp = AfxGetApp();
	m_bMaxChild = pApp->GetProfileInt(szSection, szChildWinPos, m_bMaxChild);
	// Load the logo font
	{
		BOOL bSuccess = FALSE;
		LOGFONT* plf  = NULL;
		UINT dwSize   = sizeof(LOGFONT);
		bSuccess = pApp->GetProfileBinary(szSection, szLogoFont, (BYTE**)&plf, &dwSize);
		
		if (bSuccess)
		{
			m_fontLogo.Detach();
			m_fontLogo.CreateFontIndirect(plf);
		}
		delete plf;
	}
	m_bBkBitmap   =	pApp->GetProfileInt(szSection, szBkBitmap, (int)m_bBkBitmap);
	m_crLogoColor = pApp->GetProfileInt(szSection, szLogoColor, (int)m_crLogoColor);
	m_crBkColor   = pApp->GetProfileInt(szSection, szBackColor, (int)m_crBkColor);
	SetBkColor(m_crBkColor);
	m_enuDispType = (DispType)pApp->GetProfileInt(szSection, szDispType, (int)m_enuDispType);
	SetDispType(m_enuDispType);
	if (m_strFileName.IsEmpty())
	{
		m_strFileName = pApp->GetProfileString(szSection, szFileName, (LPCTSTR)m_strFileName);
		SetBitmap(m_strFileName);
	}
}

CMDIClient::~CMDIClient()
{
	if (m_pwndControlBars != NULL)
	{
		m_pwndControlBars->RemoveAll();
		delete m_pwndControlBars;
		m_pwndControlBars = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	//{{AFX_MSG_MAP(CMDIClient)
	ON_MESSAGE(WM_MDIREFRESHMENU, OnRefreshMenu)
	ON_MESSAGE(WM_MDISETMENU, OnSetMenu)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_WINDOW_MANAGE, OnWindowManage)
	ON_COMMAND(ID_WINDOW_NEXT, OnWindowNext)
	ON_COMMAND(ID_WINDOW_PREVIOUS, OnWindowPrevious)
	ON_COMMAND(ID_WINDOW_SAVE_ALL, OnWindowSaveAll)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	ON_COMMAND(ID_WINDOW_CLOSE, OnWindowClose)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_COMMAND(ID_VIEW_VIEWTAB, OnTabView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWTAB, OnUpdateTabView)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MANAGE, OnUpdateWindowManage)
	ON_WM_PAINT()
	ON_MESSAGE(WM_MDICREATE, OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY, OnMDIDestroy)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEXT, OnUpdateWindowManage)	     // Just cheating!!!
	ON_UPDATE_COMMAND_UI(ID_WINDOW_PREVIOUS, OnUpdateWindowManage)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SAVE_ALL, OnUpdateWindowManage)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE_ALL, OnUpdateWindowManage)
END_MESSAGE_MAP()

BOOL CMDIClient::SubclassMDIClient(CMDIFrameWnd* pMDIFrameWnd, 
								   CViewManager* pViewManager, UINT uID)
{
	// Subclass the MDI client window
    VERIFY(this->SubclassWindow(pMDIFrameWnd->m_hWndMDIClient));
	if (pViewManager == NULL)
		m_pViewManager = new CViewManager;
	else
		m_pViewManager = pViewManager;
    m_pViewManager->CreateViewManager(pMDIFrameWnd, uID);
	m_pMDIFrame    = reinterpret_cast<CMyMainFrameClass*>(pMDIFrameWnd); // TODO
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIClient message handlers

void CMDIClient::OnTabView() 
{
	ASSERT(m_pMDIFrame != NULL);
	ASSERT(m_pViewManager != NULL);
	m_pMDIFrame->ShowControlBar(m_pViewManager,
		(m_pViewManager->GetStyle() & WS_VISIBLE) == 0, FALSE);
}

void CMDIClient::OnUpdateTabView(CCmdUI* pCmdUI) 
{
	ASSERT(m_pMDIFrame != NULL);
	ASSERT(m_pViewManager != NULL);
	pCmdUI->SetCheck((m_pViewManager->GetStyle() & WS_VISIBLE) != 0);
}

LRESULT CMDIClient::OnSetMenu(WPARAM wParam, LPARAM lParam)
{
	//	LRESULT lResult = Default(); // This does not prevent level 4 warning...
	LRESULT lResult = DefWindowProc(WM_MDISETMENU, wParam, lParam);
	
	// Remember Window submenu handle
	m_hMenuWindow = reinterpret_cast < HMENU>(lParam);
	
	// Refresh window submenu
	SendMessage(WM_MDIREFRESHMENU);
	return lResult;
}

LRESULT CMDIClient::OnRefreshMenu(WPARAM wParam, LPARAM lParam)
{
	//	LRESULT lResult = Default();  // This does not prevent level 4 warning...
	LRESULT lResult = DefWindowProc(WM_MDIREFRESHMENU, wParam, lParam);
	
	HMENU hMenu = m_hMenuWindow;
	
	if (hMenu != NULL)
	{
		UINT uState = ::GetMenuState(hMenu, AFX_IDM_FIRST_MDICHILD + 9, MF_BYCOMMAND);
		if (uState == 0xFFFFFFFF) 
			uState = 0;
		
		// Remove old MDI 'Windows...' command
		while (::RemoveMenu(hMenu, AFX_IDM_FIRST_MDICHILD + 9, MF_BYCOMMAND))
			;
		
		// Remove 'Windows' command
		while (::RemoveMenu(hMenu, ID_WINDOW_MANAGE, MF_BYCOMMAND)) 
			;
		
		::AppendMenu(hMenu, MF_STRING | uState, ID_WINDOW_MANAGE, m_strWindows);
	}
	
	return lResult;
}

void CMDIClient::OnWindowManage()
{    
	ManageWindows(m_pMDIFrame);
}


void CMDIClient::OnUpdateWindowManage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pViewManager->GetWindowNum());
}

void CMDIClient::ManageWindows(CMDIFrameWnd* pMDIFrame)
{
	CWindowDlg dlg(pMDIFrame);
	dlg.DoModal();
}

void CMDIClient::ManageWindows(CMDIFrameWnd* pMDIFrame, CWnd* pParentWnd)
{
	CWindowDlg dlg(pMDIFrame, pParentWnd);
	dlg.DoModal();
}

BOOL CMDIClient::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);	   // clear that level 4 warning...
    return TRUE;
}

void CMDIClient::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
    // if the app is just starting up, save the window
    // dimensions and get out
    if ((m_sizeClient.cx == 0) &&(m_sizeClient.cy == 0))
	{
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
        return;
	}
	
    // if the size hasn't changed, break and pass to default
    if ((m_sizeClient.cx == cx) &&(m_sizeClient.cy == cy))
    { 
        return;
    }
	
    // window size has changed so save new dimensions and force
    // entire background to redraw, including icon backgrounds
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
	
    RedrawWindow(NULL, NULL,
        RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_ALLCHILDREN);    
	
    return;                
}

void CMDIClient::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// A simple demo...
	POINT  ptScreen = point;
	// Convert the mouse point to screen coordinates since that is what is used by
	// the TrackPopupMenu() function.
	ClientToScreen(&ptScreen);
	CMenu* pMenuPopup;
	
	// Get a pointer to the application's File menu popup.
	// 1. AfxGetMainWnd() returns a pointer to the main frame window,
	// 2. GetMenu()       returns a pointer to the main menu of the application,
	// 3. GetSubMenu(0)   retrieves the submenu at the position 0(zero)--the File menu.
	pMenuPopup = reinterpret_cast<CPopupMenu*>(((AfxGetMainWnd())->GetMenu())->GetSubMenu(0));
	
	// Dract and track the floating menu
	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 
		AfxGetMainWnd(), // Use the 'this' pointer if you want the commands to 
		// be handler in the CMDIClient class instead
		NULL);
	
	// NOTE: This is an application menu, do not destroy it here!!!
	
	CWnd::OnRButtonDown(nFlags, point);
}


LRESULT CMDIClient::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd  = reinterpret_cast<HWND>(DefWindowProc(WM_MDICREATE, wParam, lParam));
	ASSERT(hWnd != NULL);
	CMDIChildWnd *pChild = static_cast<CMDIChildWnd*>(FromHandle(hWnd));
	if (pChild == NULL)
		return NULL;
	
	if (!m_bFullScreen && m_pViewManager->GetWindowNum() > 0)
	{
		CMDIChildWnd* pActiveChild = m_pMDIFrame->MDIGetActive();
		DWORD dwStyle = ::GetWindowLong(pActiveChild->m_hWnd, GWL_STYLE);
		m_bMaxChild   = (dwStyle & WS_MAXIMIZE) ? TRUE : FALSE;
	}
	
	// TODO-bug shows even if the document does not exits!!!
	if (m_bFirstTime && m_bMaxChild)
		m_pMDIFrame->MDIMaximize(pChild);
	else if (m_bMaxChild && m_pViewManager->GetWindowNum() == 0)
		m_pMDIFrame->MDIMaximize(pChild);
	
	m_pViewManager->AddView(_T(""), pChild);
	return (LRESULT) hWnd;
}

LRESULT CMDIClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	CMDIChildWnd* pChild = 
		static_cast<CMDIChildWnd*>(FromHandle(reinterpret_cast<HWND>(wParam)));
	ASSERT(pChild != NULL);
	
	// It will be useful to save the state of the last child window closed and
	// use this for displaying the next window...
	if (m_pViewManager->GetWindowNum() == 1)
	{
		DWORD dwStyle = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE);
		// Save the necessary options for later restoration process
		m_bMaxChild  =(dwStyle & WS_MAXIMIZE) ? TRUE : FALSE;
		AfxGetApp()->WriteProfileInt(szSection, szChildWinPos, m_bMaxChild);
	}
	
	m_pViewManager->RemoveView(pChild);
	return DefWindowProc(WM_MDIDESTROY,  wParam, lParam);
}

void CMDIClient::OnWindowNext() 
{
	m_pMDIFrame->MDINext();
}

void CMDIClient::OnWindowPrevious() 
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_pMDIFrame->m_hWndMDIClient, WM_MDINEXT, 0, 1); 
}

void CMDIClient::OnWindowSaveAll() 
{
	CDocumentList CurDocList;
	
	// save all modified documents, prompting the user if necessary for filename
	CDocument* pDoc = CurDocList.GetNextDocument();
	while (pDoc != NULL)
	{
		if (pDoc->IsModified())
			pDoc->DoFileSave();
		pDoc = CurDocList.GetNextDocument();  // VC++5.0 will not support this in the
		// while() statement -- Thanks Ivan Zhakov.
	}
}

void CMDIClient::OnWindowCloseAll() 
{
	CDocumentList CurDocList;
	
	// save all modified documents, prompting the user if necessary for filename 
	CDocument* pDoc = CurDocList.GetNextDocument();
	while (pDoc != NULL)
	{
		if (pDoc->IsModified())
			pDoc->DoFileSave();
		
		pDoc = CurDocList.GetNextDocument();  // VC++5.0 will not support this in the
		// while() statement -- Thanks Ivan Zhakov.
	}
	// now close all the open documents
	AfxGetApp()->CloseAllDocuments(FALSE);
}

void CMDIClient::OnWindowClose()
{
    CMDIChildWnd* pChild = m_pMDIFrame->MDIGetActive();
    if (pChild)
        pChild->SendMessage(WM_CLOSE);
}

void CMDIClient::OnViewFullscreen() 
{
	if (m_bFullScreen)
	{
		ShowControlBars();   // Display the hidden bars
		
		m_pMDIFrame->DockControlBar(m_pViewManager, AFX_IDW_DOCKBAR_TOP);
		
		FullScreenOff();
	}
	else 
	{
		// available only if there is an active doc
		CMDIChildWnd* pChild = m_pMDIFrame->MDIGetActive();
		DWORD dwStyle = 0;
		if (pChild != NULL)
		{
			dwStyle = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE);
		}
		// Save the necessary options for later restoration process
		m_bMaxChild = (dwStyle & WS_MAXIMIZE) ? TRUE : FALSE;
		m_pMDIFrame->GetWindowRect(&m_rcMainFrame);
		CRect rcBar;
		m_pViewManager->GetWindowRect(rcBar);
		int cxScrn   = ::GetSystemMetrics(SM_CXSCREEN);
		int cyScrn   = ::GetSystemMetrics(SM_CYSCREEN);
		
		// Float the tab control bar, and hide the toolbar and status bars
		m_pMDIFrame->FloatControlBar(m_pViewManager, 
			CPoint((cxScrn - rcBar.Width())/2, cyScrn - rcBar.Height() * 3));
		
		HideControlBars();   // Hide the bars
		
		FullScreenOn();
	}
}

void CMDIClient::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bFullScreen);
//	if (m_pViewManager->GetWindowNum() == 0 && !m_bFullScreen)
//		pCmdUI->Enable(m_pViewManager->GetWindowNum());
}

void CMDIClient::FullScreenOn()
{
	// The full screen toolbar array! The array form was chosen just in case
	// you wish to add more buttons to the floating toolbar
	UINT uIDToolBar[] = 
	{
		ID_VIEW_FULLSCREEN
	};
	
	// 1. Get the child window and hide it to avoid flicker. 
	CMDIChildWnd* pChild = m_pMDIFrame->MDIGetActive();
	if (pChild != NULL)
		pChild->ShowWindow(SW_HIDE);
	
	// 2. Remove the caption of the main window and resize it to the full screen
	LONG dwStyle = ::GetWindowLong(m_pMDIFrame->m_hWnd, GWL_STYLE);
	dwStyle &= ~WS_CAPTION;
	::SetWindowLong(m_pMDIFrame->m_hWnd, GWL_STYLE, dwStyle);
	// Now resize the main window
	int cxScrn   = ::GetSystemMetrics(SM_CXSCREEN);
	int cyScrn   = ::GetSystemMetrics(SM_CYSCREEN);
	int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
	int cyBorder = ::GetSystemMetrics(SM_CYBORDER);
	m_pMDIFrame->SetWindowPos(NULL, -cxBorder, -cyBorder, cxScrn + cxBorder * 2, 
		cyScrn + cyBorder * 2, SWP_NOZORDER);
	
	// 3. Create the new full screen toolbar, for toggling on mode
	m_pwndFullScrnToolBar = new CToolBar;
	if (!m_pwndFullScrnToolBar->CreateEx(m_pMDIFrame, TBSTYLE_FLAT, WS_VISIBLE | CBRS_TOOLTIPS | 
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOATING) || 
		!m_pwndFullScrnToolBar->LoadToolBar(IDR_TABVIEW_MENU))
	{
		TRACE0(_T("Failed to create the full screen toolbar\n"));
		return;
	}
	m_pwndFullScrnToolBar->SetButtons(uIDToolBar, sizeof(uIDToolBar) / sizeof(UINT));
	//	TCHAR szButText[] = _T("Full Screen");
	//	m_pwndFullScrnToolBar->SetButtonText(0, szButText);  // TODO
	m_pwndFullScrnToolBar->EnableDocking(0);
	m_pwndFullScrnToolBar->SetWindowText(_T("Full Screen"));
	CRect rcTB;
	m_pwndFullScrnToolBar->GetItemRect(0, rcTB);
	// position the full screen toolbar at some convenient location
	m_pMDIFrame->FloatControlBar(m_pwndFullScrnToolBar, 
		CPoint(cxScrn - rcTB.Width() * 3, /*cyScrn - */rcTB.Height() * 3));
	
	// 4. Remove the caption of the child window and display it
	//	dwStyle  = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE);
	//	dwStyle &= ~WS_CAPTION; 
	//	::SetWindowLong(pChild->m_hWnd, GWL_STYLE, dwStyle); 
	//
	if (pChild != NULL)
		pChild->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMDIFrame->RecalcLayout();
	m_bFullScreen = TRUE;
}

void CMDIClient::FullScreenOff()
{
	if (m_pwndFullScrnToolBar != NULL)
	{
		m_pwndFullScrnToolBar->DestroyWindow();
		delete m_pwndFullScrnToolBar;
	}
	
	// Hidding and showing the child window may not be the best solution,
	// but it helps reducing the flicker
	CMDIChildWnd* pChild = m_pMDIFrame->MDIGetActive();
	if (pChild)
		pChild->ShowWindow(SW_HIDE);	
	
	LONG dwStyle = ::GetWindowLong(m_pMDIFrame->m_hWnd, GWL_STYLE);
	dwStyle |= WS_CAPTION;
	::SetWindowLong(m_pMDIFrame->m_hWnd, GWL_STYLE, dwStyle);
	
	m_pMDIFrame->MoveWindow(&m_rcMainFrame);
	m_pMDIFrame->RecalcLayout();
	
	if (pChild)
	{
		//		dwStyle  = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE); 
		//		dwStyle |= WS_CAPTION; 
		//		::SetWindowLong(pChild->m_hWnd, GWL_STYLE, dwStyle); 
		
		if (m_bMaxChild)
			m_pMDIFrame->MDIMaximize(pChild);
		else 
			m_pMDIFrame->MDIRestore(pChild);
		
		pChild->ShowWindow(SW_SHOW);
	}
	m_bFullScreen = FALSE;
}

BOOL CMDIClient::PreTranslateMessage(MSG* pMsg) 
{
	// When there is an escape key toggle the full screen mode
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bFullScreen)	
		OnViewFullscreen();
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CMDIClient::SaveMainFrameState()
{
	if (m_bFullScreen)
		return;
	
	CMDIChildWnd* pChild = m_pMDIFrame->MDIGetActive();
	if (pChild)
	{
		// It will be useful to save the state of the last child window closed and
		// use this for displaying the next window...
		DWORD dwStyle = ::GetWindowLong(pChild->m_hWnd, GWL_STYLE);
		// Save the necessary options for later restoration process
		m_bMaxChild  =(dwStyle & WS_MAXIMIZE) ? TRUE : FALSE;
		AfxGetApp()->WriteProfileInt(szSection, szChildWinPos, m_bMaxChild);
	}
	
	// before it is destroyed, save the position of the window
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	if (m_pMDIFrame->GetWindowPlacement(&wp))
	{
		wp.flags = 0;
		if (m_pMDIFrame->IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		// and write it to the registry
		WriteWindowPlacement(&wp);
	}
	m_pMDIFrame->SaveBarState(AfxGetApp()->m_pszProfileName);
}

void CMDIClient::RestoreMainFrameState(UINT nCmdShow)
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	if (!ReadWindowPlacement(&wp))
	{
		m_pMDIFrame->ShowWindow(nCmdShow);
		return;
	}
	if (nCmdShow != SW_SHOWNORMAL)
		wp.showCmd = nCmdShow;
	m_pMDIFrame->SetWindowPlacement(&wp);
	m_pMDIFrame->ShowWindow(wp.showCmd);
	m_pMDIFrame->LoadBarState(AfxGetApp()->m_pszProfileName);
}

void CMDIClient::OnPaint() 
{
	CPaintDC dc(this);
	CRect    rc;
	GetClientRect(rc);
	
	BOOL bDoBitBlt = TRUE;
    
	if (!(HBITMAP)m_bkBitmap)
		dc.FillRect(rc, &m_bkBrush);
	
	if ((HBITMAP)m_bkBitmap)
	{
		CDC* pDC;
		CDC memDC;
		CBitmap bmp; 
		
		BOOL bUseMemDC =(GetDispType() != DISPTILE) ? TRUE : FALSE;
		if (bUseMemDC)
		{
			if (GetDispType() != DISPSTRETCH)
				bmp.CreateCompatibleBitmap(&dc, rc.right, rc.bottom);
			else
				bmp.CreateCompatibleBitmap(&dc, m_sizImage.cx, m_sizImage.cy);
			memDC.CreateCompatibleDC(&dc);
			memDC.SelectObject(&bmp);
			pDC = &memDC;
		}
		else
			pDC = &dc;      
		
		switch (GetDispType())
		{
			case DISPTILE:
				{
					CPoint point;
					for (point.y = 0; point.y < rc.Height(); point.y += m_sizImage.cy)
						for (point.x = 0; point.x < rc.Width(); point.x += m_sizImage.cx)
							pDC->DrawState(point, m_sizImage, &m_bkBitmap, DST_BITMAP | DSS_NORMAL);
				}
				break;
				
			case DISPCENTER:
				{
					pDC->FillRect(rc, &m_bkBrush);
					CPoint point((rc.Width() - m_sizImage.cx) / 2, 
						(rc.Height() - m_sizImage.cy) / 2);
					pDC->DrawState(point, m_sizImage, &m_bkBitmap, DST_BITMAP | DSS_NORMAL);
				}
				break;
				
			case DISPSTRETCH:
				{
					memDC.DrawState(CPoint(0, 0), m_sizImage, &m_bkBitmap, DST_BITMAP | DSS_NORMAL);
					dc.SetStretchBltMode(COLORONCOLOR);
					dc.StretchBlt(0, 0, rc.right, rc.bottom, &memDC, 
						0, 0, m_sizImage.cx, m_sizImage.cy, SRCCOPY);
					bDoBitBlt = FALSE;
				}
				break;
		}
		
		// Now is the time to draw the logo text at the bottom
		if (!bDoBitBlt)	   // If not using memory DC, then grab a DC here
			pDC = &dc;

		PaintLogo(pDC);
		if ((bUseMemDC == TRUE) && bDoBitBlt)
			dc.BitBlt(0, 0, rc.right, rc.bottom, pDC, 0, 0, SRCCOPY);
   }
   else
   {
		CDC* pDC;
	   	pDC = &dc;
		PaintLogo(pDC);
   }
}

BOOL CMDIClient::SetBitmap(LPCTSTR lpszFileName, UINT uFlags)
{
	HANDLE hBitmap = ::LoadImage(AfxGetInstanceHandle(), lpszFileName,
		IMAGE_BITMAP, 0, 0, uFlags | LR_LOADFROMFILE);
	
	if (!hBitmap)    // There were some problems during loading the image
		return FALSE;
	
	m_bkBitmap.DeleteObject();
	m_bkBitmap.Attach((HBITMAP)hBitmap);
	if (IsWindow(m_hWnd))
		Invalidate();
	m_strFileName = lpszFileName;
	
	BITMAP bi;
	m_bkBitmap.GetBitmap(&bi);
	m_sizImage.cx = bi.bmWidth;
	m_sizImage.cy = bi.bmHeight;
	
	return TRUE;
}

BOOL CMDIClient::SetBitmap(UINT nBmpID, COLORMAP* pClrMap, int nCount)
{
	m_bkBitmap.DeleteObject();
	if (pClrMap == NULL)
	{
		if (m_bkBitmap.LoadBitmap(nBmpID) == FALSE)
			return FALSE;
	}
	else         
	{      
		if (m_bkBitmap.LoadMappedBitmap(nBmpID, 0, pClrMap, nCount) == FALSE)
			return FALSE;
	}
	
	BITMAP bi;
	m_bkBitmap.GetBitmap(&bi);
	m_sizImage.cx = bi.bmWidth;
	m_sizImage.cy = bi.bmHeight;
	
	if (IsWindow(m_hWnd))
		Invalidate();
	return TRUE;
}

BOOL CMDIClient::SetDefBitmap(UINT nBmpID, COLORMAP* pClrMap, int nCount)
{
	m_bkDefBitmap.DeleteObject();
	if (pClrMap == NULL)
	{
		if (m_bkDefBitmap.LoadBitmap(nBmpID) == FALSE)
			return FALSE;
	}
	else         
	{      
		if (m_bkDefBitmap.LoadMappedBitmap(nBmpID, 0, pClrMap, nCount) == FALSE)
			return FALSE;
	}
	
	BITMAP bi;
	m_bkDefBitmap.GetBitmap(&bi);
	m_sizDefImage.cx = bi.bmWidth;
	m_sizDefImage.cy = bi.bmHeight;
	
	if (IsWindow(m_hWnd))
		Invalidate();
	return TRUE;
}

void CMDIClient::SetBkColor(COLORREF crValue)
{
	m_crBkColor = crValue;
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(m_crBkColor);
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::SetLogoColor(COLORREF crValue)
{
	m_crLogoColor = crValue;
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::SetDispType(CMDIClient::DispType enuDispType)
{
	m_enuDispType = enuDispType;      
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::SetLogoFont(CFont* pLogoFont)
{
	ASSERT(pLogoFont);

	LOGFONT lFont;
	memset(&lFont, 0, sizeof(LOGFONT));

	m_fontLogo.DeleteObject();

	pLogoFont->GetLogFont(&lFont);
	m_fontLogo.CreateFontIndirect(&lFont);

	SetFont(&m_fontLogo);
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::SetLogoFont(int nLogoWeight, BOOL bLogoItalic, BOOL bLogoUnderline)
{
	// Free any memory currently used by the fonts.
	m_fontLogo.DeleteObject();

	// Get the current font
	LOGFONT lFont;
	memset(&lFont, 0, sizeof(LOGFONT));
	CFont* pFont = GetFont();
	if (pFont)
		pFont->GetLogFont(&lFont);
	else 
	{
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 
			sizeof(NONCLIENTMETRICS), &ncm, 0));
		lFont = ncm.lfMessageFont; 
	}

	// Create the Logo font
	lFont.lfWeight    = static_cast<LONG>(nLogoWeight);
	lFont.lfItalic    = static_cast<BYTE>(bLogoItalic);
	lFont.lfUnderline = static_cast<BYTE>(bLogoUnderline);
	m_fontLogo.CreateFontIndirect(&lFont);

	SetFont(&m_fontLogo);
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::Defaults(BOOL bBkBitmap /*= TRUE*/, BOOL bDelBkBitmap /*= TRUE*/)
{
//	m_hMenuWindow         = 0;
//	m_pMDIFrame           = NULL;  
	m_bBkBitmap           = bBkBitmap;

	m_crBkColor           = GetSysColor(COLOR_APPWORKSPACE);
	m_crLogoColor         = GetSysColor(COLOR_BTNFACE);

//	m_bMaxChild           = FALSE;
//	m_pwndFullScrnToolBar = NULL;
	m_enuDispType         = DISPCENTER;
//	m_strWindows.LoadString(IDS_WINDOW_MANAGE);
	m_strFileName.Empty();      
	
	// Create the default font, Times New Roman is most common
	CWindowDC wndDC(CWnd::GetDesktopWindow());
	int nFontSize = -MulDiv(18, wndDC.GetDeviceCaps(LOGPIXELSY), 72);
	if (m_fontLogo.m_hObject != NULL)
		m_fontLogo.Detach();
	m_fontLogo.CreateFont(nFontSize, 
		0, 0, 0, 
		FW_BOLD, 
		FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_ROMAN, 
		_T("Times New Roman"));

	if (IsWindow(m_hWnd))
		SetFont(&m_fontLogo);

	SetBkColor(m_crBkColor);
	if (bDelBkBitmap)
	{
		m_bkBitmap.DeleteObject();
		m_sizImage.cx = m_sizImage.cy = 0;
		if (bBkBitmap && (HBITMAP)m_bkDefBitmap)
		{
			m_sizImage = m_sizDefImage;
//			m_bkBitmap& = CBitmap::FromHandle((HBITMAP)m_bkDefBitmap);
//			BITMAP* bmpDef = new BITMAP;
//			m_bkDefBitmap.GetBitmap(bmpDef);
//			VERIFY(m_bkBitmap.CreateBitmapIndirect(bmpDef));
//			delete bmpDef;
		}

	}
	if (IsWindow(m_hWnd))
		Invalidate();
}

void CMDIClient::OnDestroy() 
{
	CWnd::OnDestroy();

	CWinApp* pApp = AfxGetApp();
	if (m_fontLogo.m_hObject != NULL)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		m_fontLogo.GetLogFont(&lf);
		pApp->WriteProfileBinary(szSection, szLogoFont, (LPBYTE)&lf, sizeof(lf));
		m_fontLogo.Detach();
	}
	pApp->WriteProfileInt(szSection, szLogoColor, (int)m_crLogoColor);
	pApp->WriteProfileInt(szSection, szBackColor, (int)m_crBkColor);
	pApp->WriteProfileInt(szSection, szDispType, (int)m_enuDispType);
	pApp->WriteProfileInt(szSection, szBkBitmap, (int)m_bBkBitmap);
//	if (!m_strFileName.IsEmpty())
		pApp->WriteProfileString(szSection, szFileName, (LPCTSTR)m_strFileName);
	if (m_pViewManager != NULL)
	{
		delete m_pViewManager;
		m_pViewManager = NULL;
	}
}

void CMDIClient::PaintLogo(CDC *pDC)
{
	CRect      rcDataBox;
	CString    strLogo = szLogoString;
	TEXTMETRIC tm;
	
	pDC->SetBkMode(OPAQUE);	
	
	CFont* oldFont = pDC->SelectObject(&m_fontLogo);
	CRect st(0, 0, 0, 0);
	
	CSize sz = pDC->GetTextExtent(strLogo, strLogo.GetLength());
	// GetTextExtent calculates the size of the displayed logo
	// which depends on the device context....
	pDC->GetTextMetrics(&tm);
	
	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...
	GetClientRect(&rcDataBox);
	
	rcDataBox.left = rcDataBox.right  - sz.cx - tm.tmAveCharWidth / 2;
	rcDataBox.top  = rcDataBox.bottom - sz.cy - st.bottom - tm.tmHeight / 5;
	
	CRect rcSave = rcDataBox;		
	
	pDC->SetBkMode(TRANSPARENT);
	rcSave = rcDataBox;
	
	// shift logo box right, and print black...
	rcDataBox.left   += tm.tmAveCharWidth / 5;
	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	rcDataBox = rcSave;
	
	// shift logo box left and print white
	rcDataBox.left -= tm.tmAveCharWidth / 5;
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	// Restore original location and print in the button face color
	rcDataBox = rcSave;
	pDC->SetTextColor(m_crLogoColor);
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	// restore the original properties and release resources...
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);   
	pDC->SetBkMode(OPAQUE);	
}

	
void CMDIClient::HideControlBars() 
{
	m_pwndControlBars = new CArray<FSControlBars, FSControlBars&>;
	FSControlBars xControlBar;
	CArray<CControlBar*, CControlBar*> arrBars;
	reinterpret_cast<CMyMainFrameClass*>(m_pMDIFrame)->GetControlBarsEx(arrBars);
	for (int i = 0; i < arrBars.GetSize(); i++)
	{
		xControlBar.pControlBar = arrBars[i];
		// 
		xControlBar.bWasVisible = xControlBar.pControlBar->IsWindowVisible();
		m_pwndControlBars->Add(xControlBar);
		m_pMDIFrame->ShowControlBar(xControlBar.pControlBar, FALSE, FALSE);
	}
}

void CMDIClient::ShowControlBars() 
{
	FSControlBars xControlBar;
	// Iterate through all the control bar information...
	for (int i = 0; i < m_pwndControlBars->GetSize(); i++)
	{	
		// Obtain one at the given index...
		xControlBar = m_pwndControlBars->GetAt(i);
		// Restore the show/hide state before fullscreen view
		m_pMDIFrame->ShowControlBar(xControlBar.pControlBar, 
			xControlBar.bWasVisible, FALSE);
	}

	// Now, destroy the control bar information container
	if (m_pwndControlBars != NULL)
	{
		m_pwndControlBars->RemoveAll();
		delete m_pwndControlBars;
		m_pwndControlBars = NULL;
	}
}
