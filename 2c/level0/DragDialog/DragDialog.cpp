///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// (c) by Jochen Duell
// Schmiedgasse 1 || 91613 Marktbergel
//
// File:			DragDialog.cpp
// Description:		Contains a derivered DialogClass wich supports Drag&Drop.
//
// HowToDo:			Include the 'DragDialog.h' to your dialogs .h and .cpp file.
//					Take an existing dialog, the dialog is normaly derivered from CDialog
//					now search in the .h of you dialog for 'CDialog' an replace it with 'CDragDialog'
//					go to the .cpp and replace 'CDialog' at the MESSAGE_MAP and at the Constructor.
//					That's it! :)
//
// Functions:		void SetDragButtons(BOOL bDragLeft, BOOL bDragRight);
//						You can set the Mouse button witch drags the Dialog. (also booth)
//						TRUE means you can drag with this button.
//						Standard is TRUE FALSE (Only left button is activated for dragging)
//					void KeepDialogInScreen(BOOL bKeepDialogInScreen);
//						If you give by TRUE, the user can't move the dialog out of the screen.
//						Standard is FALSE;
//					void UpdateScreenMetrics();
//						Updates the variables for the Screen coordinates (x y)
//					void GetDialogBackToScreen();
//						If the user dragged the Dialog out of sight, the dialog will be returned.
//					BOOL ActivateDrag();
//						If you call this funktion the dialog sticks to the mousepointer.
//					BOOL DeactivateDrag();
//						Drag is deactivatet, till the user starts it again.
//					BOOL SetDragCursor(UINT nID);
//						Give by the ID of the Cursor wich sould be displayed instead of the
//						Mousepointer if user is dragging the dialog.
//					void UseDragCursor(BOOL bUseDragCursor);
//						When you want to display the standard-cursor while dragging, set to FALSE.
//						Standard is FALSE;
//					BOOL SetFadeWhileDragging(int nFade);
//						If the dialog schould have some 'look-through' effect while dragging,
//						set nFade to an value between 0 and 255. 0 means completely invisible
//						Set it to 255 or NOFADE, if you wouldn't have this effect.
//						Standard is NOFADE.
//						Attention: Funktion can only be used under Win2k/ME/XP or higher.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
// If you use my code, leave this text! Thanx
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Have a lot of fun with it :) and write me a Mail: JochenDuell@Web.de
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DragDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDragDialog::CDragDialog(UINT nID, CWnd* pParent) : CDialog(nID, pParent)
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
}

BEGIN_MESSAGE_MAP(CDragDialog, CDialog)
	//{{AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDragDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bDragLeft)
		ActivateDrag(); //if dragging is set to the left mousebutton, dragging will be activated
	CDialog::OnLButtonDown(nFlags, point);
}

void CDragDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bDragRight)
		ActivateDrag(); //if dragging is set to the right mousebutton, dragging will be activated
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CDragDialog::PreTranslateMessage(MSG* pMsg)
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CDragDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag) //If dragging is activated
	{
		WINDOWPLACEMENT placement;
		if (!GetWindowPlacement(&placement)) //Get placement of the dialog
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
		
		
		MoveWindow(nLeft, nTop, nWidth, nHeight, TRUE); //And finally move the window to new coordinates.
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDragDialog::SetDragButtons(BOOL bDragLeft, BOOL bDragRight) //Set the drag-mousebutton(s)
{
	m_bDragLeft = bDragLeft;
	m_bDragRight = bDragRight;
}

BOOL CDragDialog::ActivateDrag()
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

BOOL CDragDialog::DeactivateDrag()
{
	m_bDrag = FALSE; // Dragging is deactivated
	
	if (m_nFadeWhileDragging != NOFADE) //If fading was activated, draw the dialog normal
		FadeDialog(FALSE);
	if (!ReleaseCapture()) //Release Mousecapture
		return FALSE;
	return TRUE;
}

void CDragDialog::KeepDialogInScreen(BOOL bKeepDialogInScreen)
{
	m_bKeepDialogInScreen = bKeepDialogInScreen;
}

void CDragDialog::UpdateScreenMetrics()
{
	m_nScreenX = GetSystemMetrics(SM_CXSCREEN); // You get the width of the users monitor
	m_nScreenY = GetSystemMetrics(SM_CYSCREEN); // You get the height of the users monitor
}

void CDragDialog::GetDialogBackToScreen()
{
	SendMessage(DM_REPOSITION); //This fetchs the dialog back to the screen
}

void CDragDialog::UseDragCursor(BOOL bUseDragCursor)
{
	m_bUseDragCursor = bUseDragCursor;
}

BOOL CDragDialog::SetDragCursor(UINT nID)
{
	m_hCursor = AfxGetApp()->LoadCursor(nID); //Load the new dragcursor to an HCURSOR variable
	if (!m_hCursor)
		return FALSE;
	UseDragCursor(TRUE);
	return TRUE;
}

void CDragDialog::DrawDragCursor()
{
	if (m_hCursor)
		SetCursor(m_hCursor);
}

BOOL CDragDialog::SetFadeWhileDragging(int nFade)
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

void CDragDialog::FadeDialog(BOOL bFade)
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

