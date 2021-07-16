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

#if !defined(AFX_MYDLG_H__B90982E8_B955_4733_9D60_18EC9067E669__INCLUDED2_)
#define AFX_MYDLG_H__B90982E8_B955_4733_9D60_18EC9067E669__INCLUDED2_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WS_EX_LAYERED    
#define WS_EX_LAYERED			0x00080000
#endif
#ifndef ULW_ALPHA    
#define ULW_ALPHA				0x00000002    
#endif
#ifndef NOFADE    
#define NOFADE					255
#endif


class CDragDialog : public CDialog
{
public:
	CDragDialog(UINT, CWnd*);
	void SetDragButtons(BOOL bDragLeft, BOOL bDragRight);
	void KeepDialogInScreen(BOOL bKeepDialogInScreen);
	BOOL ActivateDrag();
	BOOL DeactivateDrag();
	void UpdateScreenMetrics();
	void GetDialogBackToScreen();
	BOOL SetDragCursor(UINT nID);
	void UseDragCursor(BOOL bUseDragCursor);
	BOOL SetFadeWhileDragging(int nFade);
protected:
	void FadeDialog(BOOL bFade);
	void DrawDragCursor();
	CWnd * m_pCapture;
	int m_nFadeWhileDragging;
	int m_nOldMouseX;
	int m_nOldMouseY;
	int m_nScreenX;
	int m_nScreenY;
	BOOL m_bDragLeft;
	BOOL m_bDragRight;
	BOOL m_bKeepDialogInScreen;
	BOOL m_bUseDragCursor;
	BOOL m_bDrag;
	HCURSOR	m_hCursor;
	//{{AFX_VIRTUAL(CDragDrop)
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CDragDrop)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// SetLayeredWindowAttributes function prototype
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
(
	HWND hwnd,	
	COLORREF crKey,	
	BYTE bAlpha,	
	DWORD dwFlags
);

//{{AFX_INSERT_LOCATION}}
#endif