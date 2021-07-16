// HTMLHelpView.cpp : implementation file
//

#include "stdafx.h"
#include "HTMLHelpView.h"
#include "../Mainfrm.h"
#include "htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpView

IMPLEMENT_DYNCREATE(CHTMLHelpView, CView)

CHTMLHelpView::CHTMLHelpView()
{
	m_hHelpWindow = NULL;
}

CHTMLHelpView::~CHTMLHelpView()
{
}


BEGIN_MESSAGE_MAP(CHTMLHelpView, CView)
	//{{AFX_MSG_MAP(CHTMLHelpView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpView drawing

void CHTMLHelpView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	//Height of caption
	//so we can hide the inner window's caption
	int iTopHide = GetSystemMetrics(SM_CYCAPTION);
	iTopHide += (2 * GetSystemMetrics(SM_CYBORDER));

	if (!m_hHelpWindow)
	{
		//-------------------------------------------------

		//for multiple windows the window definition must be
		//different.
		//This is only needed if allowing multiple help windows
		char szUnique[10];
		itoa((int)this, szUnique, 10);		

		CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
		
		//name of the compiled help file plus ">" plus A unique Window Name
		CString cHelpFile = "MyCompiledHelpFile.chm>MyHelp";
		cHelpFile += szUnique;
		
		HH_WINTYPE cWinType;
		memset(&cWinType, 0, sizeof(cWinType));
		
		cWinType.cbStruct = sizeof(cWinType); //size of this structure
		cWinType.fUniCodeStrings = FALSE; //TRUE if all strings are in UNICODE
		
		//The same unique Window Name
		CString cType = "MyHelp";   
		cType += szUnique;
		cWinType.pszType = cType;   //Name of a type of window		

		cWinType.fsValidMembers = HHWIN_PARAM_TABPOS | HHWIN_PARAM_STYLES | 
			HHWIN_PARAM_RECT | HHWIN_PARAM_PROPERTIES | HHWIN_PROP_NODEF_STYLES 
			| HHWIN_PROP_NODEF_EXSTYLES;  //Bit flag of valid members (HHWIN_PARAM_)
		cWinType.fsWinProperties = 
			HHWIN_PROP_TRI_PANE | HHWIN_PROP_TAB_SEARCH | HHWIN_PROP_AUTO_SYNC;

		//Any Old Title Is Fine Here
		cWinType.pszCaption = "Help Window"; //Window title
		
		cWinType.dwStyles = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN; //Window styles
		
		cWinType.hwndCaller = GetSafeHwnd();        //who called this window

		RECT tClientRect;
		GetClientRect(&tClientRect);
		CopyRect(&cWinType.rcWindowPos, &tClientRect);
		cWinType.rcWindowPos.top -= iTopHide;
		cWinType.rcWindowPos.bottom -= iTopHide;

		cWinType.tabpos = HHWIN_NAVTAB_TOP;

		//A path to the index and table of contents files
		cWinType.pszToc	= "c:\\MyFiles\\toc.hhc";    //Location of the table of contents file
		cWinType.pszIndex = "c:\\MyFiles\\index.hhk";  //Location of the index file
   
		HtmlHelp(GetSafeHwnd(), NULL, HH_SET_WIN_TYPE, (long)&cWinType); 			
		m_hHelpWindow = HtmlHelp(GetSafeHwnd(), cHelpFile, HH_DISPLAY_TOC, NULL); 	

		//gets rid of thick frame
		//so inner window cannot be re-sized
		::SetWindowLong(m_hHelpWindow, GWL_STYLE, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN); 
	}
	else
	{
		//Caption needs to be hidden
		//If you take it out of styles the window acts weird
		//better to hide it...
		RECT tClientRect;
		GetClientRect(&tClientRect);
		tClientRect.top -= iTopHide;
		::SetWindowPos(m_hHelpWindow, NULL, tClientRect.left, tClientRect.top, 
			tClientRect.right - tClientRect.left, tClientRect.bottom - tClientRect.top,
			SWP_NOZORDER);
		//force redraw
		//::InvalidateRect(m_hHelpWindow, NULL, TRUE);
		//::UpdateWindow(m_hHelpWindow);
	}	
}



/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpView diagnostics

#ifdef _DEBUG
void CHTMLHelpView::AssertValid() const
{
	CView::AssertValid();
}

void CHTMLHelpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpView message handlers

void CHTMLHelpView::OnInitialUpdate() 
{	
	CView::OnInitialUpdate();	
}

