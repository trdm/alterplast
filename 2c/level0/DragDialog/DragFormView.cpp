// DragFormView.cpp: implementation of the CDragFormView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "enterprise.h"
#include "DragFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDragFormView, CFormView)

CDragFormView::CDragFormView(UINT nIDD)
	: CFormView(nIDD)
{
	m_nOldMouseX			= 0;
	m_nOldMouseY			= 0;
	m_nFadeWhileDragging	= NOFADE;
	m_bDrag					= FALSE;
	m_bDragLeft				= TRUE;
	m_bDragRight			= FALSE;
	m_bKeepDialogInScreen	= FALSE;
	m_bUseDragCursor		= FALSE;
	m_hCursor				= NULL;
	UpdateScreenMetrics();

	SetDragCursor(IDC_POINTER_DRAG);
//	m_hCursor = AfxGetApp()->LoadCursor(IDC_ARROW);
//	UseDragCursor(1);
}

CDragFormView::~CDragFormView()
{

}

BEGIN_MESSAGE_MAP(CDragFormView, CFormView)
	//{{AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDragFormView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bDragLeft)
		ActivateDrag(); //if dragging is set to the left mousebutton, dragging will be activated
	CFormView::OnLButtonDown(nFlags, point);
}

void CDragFormView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bDragRight)
		ActivateDrag(); //if dragging is set to the right mousebutton, dragging will be activated
	CFormView::OnRButtonDown(nFlags, point);
}

BOOL CDragFormView::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_LBUTTONUP:
		if(m_bDrag && m_bDragLeft)	//if leftmousebutton is up, the user is currently dragging and
									//the dragging is set to the left mousebutton
			DeactivateDrag();
		break;
	case WM_RBUTTONUP:
		if(m_bDrag && m_bDragRight)	//if rightmousebutton is up, the user is currently dragging and
									//the dragging is set to the right mousebutton
			DeactivateDrag();
		break;
	default:
		break;
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CDragFormView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag) //If dragging is activated
	{
		WINDOWPLACEMENT placement;
		if (!GetParentFrame()->GetWindowPlacement(&placement)) //Get placement of the dialog
			return;
		int nTop = placement.rcNormalPosition.top;
		int nBottom = placement.rcNormalPosition.bottom;
		int nLeft = placement.rcNormalPosition.left;
		int nRight = placement.rcNormalPosition.right;

		CPoint ptGlobPos;
		if (!GetCursorPos(&ptGlobPos)) //Get coordinates of the mouse (complete screen)
			return;
			
		int nDiffX = ptGlobPos.x - m_nOldMouseX;	//Calculate the difference between old 
													//and new X pos of the mouse.
		
		int nDiffY = ptGlobPos.y - m_nOldMouseY;	//Calculate the difference between old 
													//and new Y pos of the mouse.

		nLeft = nLeft + nDiffX; //Calculate the new windowposition
		nTop = nTop + nDiffY;
		nRight = nRight + nDiffX;
		nBottom = nBottom + nDiffY;

		int nWidth	= nRight-nLeft; //Calculate the width and height of the window
		int nHeight	= nBottom-nTop;
		
		if (m_bKeepDialogInScreen)	//If dialog should stay in screen the coordinates would be
									//corrected if necessary
		{
			if (nLeft<0)
				nLeft = 0;
			if (nRight>m_nScreenX)
				nLeft = m_nScreenX - nWidth;
			if (nTop<0)
				nTop = 0;
			if (nBottom>m_nScreenY)
				nTop = m_nScreenY - nHeight;
		}
		
		m_nOldMouseX = ptGlobPos.x; //Save actual Mousepos for the next call of OnMouseMove()
		m_nOldMouseY = ptGlobPos.y;
		
		
		GetParentFrame()->MoveWindow(nLeft, nTop, nWidth, nHeight, TRUE); //And finally move the window to new coordinates.
	}

	CFormView::OnMouseMove(nFlags, point);
}

void CDragFormView::SetDragButtons(BOOL bDragLeft, BOOL bDragRight) //Set the drag-mousebutton(s)
{
	m_bDragLeft = bDragLeft;
	m_bDragRight = bDragRight;
}

BOOL CDragFormView::ActivateDrag()
{
	if (m_bKeepDialogInScreen) //If the dialog should stay in screen, we update the ScreenMetrics
		UpdateScreenMetrics();
	
	CPoint ptGlobPos;
	if (!GetCursorPos(&ptGlobPos)) //Get the mouseposition
		return FALSE;
	
	m_nOldMouseX = ptGlobPos.x;
	m_nOldMouseY = ptGlobPos.y;

	SetCapture(); //Get Mousecapture

	if (m_bUseDragCursor) //Should be used another mousecursor for dragging? Yes? DrawIt!
		DrawDragCursor();

	if (m_nFadeWhileDragging != NOFADE) //If fading is activated, fade the Dialog
		FadeDialog(TRUE);

	m_bDrag = TRUE; //Dragging is now activated
	return TRUE;
}

BOOL CDragFormView::DeactivateDrag()
{
	m_bDrag = FALSE; // Dragging is deactivated
	
	if (m_nFadeWhileDragging != NOFADE) //If fading was activated, draw the dialog normal
		FadeDialog(FALSE);
	if (!ReleaseCapture()) //Release Mousecapture
		return FALSE;
	return TRUE;
}

void CDragFormView::KeepDialogInScreen(BOOL bKeepDialogInScreen)
{
	m_bKeepDialogInScreen = bKeepDialogInScreen;
}

void CDragFormView::UpdateScreenMetrics()
{
	m_nScreenX = GetSystemMetrics(SM_CXSCREEN); // You get the width of the users monitor
	m_nScreenY = GetSystemMetrics(SM_CYSCREEN); // You get the height of the users monitor
}

void CDragFormView::GetDialogBackToScreen()
{
	SendMessage(DM_REPOSITION); //This fetchs the dialog back to the screen
}

void CDragFormView::UseDragCursor(BOOL bUseDragCursor)
{
	m_bUseDragCursor = bUseDragCursor;
}

BOOL CDragFormView::SetDragCursor(UINT nID)
{
	m_hCursor = AfxGetApp()->LoadCursor(nID); //Load the new dragcursor to an HCURSOR variable
	if (!m_hCursor)
		return FALSE;
	UseDragCursor(TRUE);
	return TRUE;
}

void CDragFormView::DrawDragCursor()
{
	if (m_hCursor)
		SetCursor(m_hCursor);
}

BOOL CDragFormView::SetFadeWhileDragging(int nFade)
{
	if ((LOBYTE(LOWORD(GetVersion()))) <= 4)
	{
		m_nFadeWhileDragging = NOFADE;
		return FALSE; // 'Cause Windows versions lower than 5 don't support fading
	}
	if (nFade < 0 || nFade > NOFADE)
		return FALSE; // You can only fade between 0 and 255
	m_nFadeWhileDragging = nFade;
	return TRUE;
}

void CDragFormView::FadeDialog(BOOL bFade)
{
	int nFade = m_nFadeWhileDragging;
	if (!bFade)
		nFade = NOFADE;

	lpfnSetLayeredWindowAttributes pSetLayeredWindowAttributes;
	pSetLayeredWindowAttributes=NULL;
	// Function for fading is in USER32.DLL
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	if (!hUser32)
		return;
	pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");    
	
	if (nFade >= NOFADE)
		SetWindowLong(*this, GWL_EXSTYLE, GetWindowLong(*this, GWL_EXSTYLE));
	else
		SetWindowLong(*this, GWL_EXSTYLE, GetWindowLong(*this, GWL_EXSTYLE) | WS_EX_LAYERED);
	// Call the function to fade the dialog.
	pSetLayeredWindowAttributes(*this, 0, nFade, ULW_ALPHA);
}

