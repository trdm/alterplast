/////////////////////////////////////////////////////////////////////////////
// DropEdit.h : header file for SECDropEdit - subclassed edit with little
//			    drop down button.
//
// Stingray Software Extension Classes
// Copyright (C) 1996-1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Don			20/01/97
//


#ifndef __DROPEDIT_H__
#define __DROPEDIT_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


/////////////////////////////////////////////////////////////////////////////
// SECDropEdit window
//
// Adds a combo like drop down button to an edit control
//

/////////////////////////////
// AutoDuck tag block block for SECDropEdit 

//@doc SECDropEdit
//@mdata CRect    | SECDropEdit | m_btnRect | The window rect of the button 
// (in our parents client coordinates)
//@xref <c SECDropEdit>

//@doc SECDropEdit
//@mdata CBitmap  | SECDropEdit | m_bmp | Bitmap drawn on face of button
//@xref <c SECDropEdit>

//@doc SECDropEdit
//@mdata LPCTSTR  | SECDropEdit | m_lpszBmpName | filename of the bitmap
//@xref <c SECDropEdit>

//@doc SECDropEdit
//@mdata WORD     | SECDropEdit | m_wState | Current state
//@xref <c SECDropEdit>

//@doc SECDropEdit
//@mdata WORD     | SECDropEdit | m_wLeftBorder | Left hand Gap between bitmap and border
//@xref <c SECDropEdit>

//@doc SECDropEdit
//@mdata WORD     | SECDropEdit | m_wRightBorder | Right hand Gap between bitmap and border
//@xref <c SECDropEdit>

// adinsert AutoDuck insertion point for SECDropEdit 
//@doc SECDropEdit 
//@class The SECDropEdit class adds a combo-like drop-down button to an 
// edit control.
//@comm This class is used with SECCurrencyEdit and Format to implement an edit 
// control for entering and displaying custom-formatted currency data.
//
// See the CURRENCY sample in the \OT\SAMPLES\STANDARD\CONTROLS\CURRENCY 
// directory for a demonstration of this class.
//
//@base public | CEdit
//#include "../Edit/amsEdit.h"
#define CPARENTEDIT CEdit//CAMSDateEdit//CAMSMaskedEdit//CEdit
class SECDropEdit : public CPARENTEDIT//CEdit
{
	DECLARE_DYNAMIC(SECDropEdit)

	//@access Creation/Initialization
public:
	//@cmember
	/* Constructs a SECDropEdit object.*/
	SECDropEdit();

	//@cmember
	/* Creates a window and attaches it to the drop edit object.*/
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	
	//@cmember
	/* Creates a child window with extended attributes and */
	// attaches it to the drop edit object.
	virtual BOOL CreateEx(DWORD dwExStyle,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//@cmember
	/* Attaches an existing edit control to the drop edit object.*/
	BOOL AttachEdit(int nCtlID, CWnd* pParentWnd);

	//@access Attributes
public:
	
	//@cmember
	/* Manipulates the alignment mode (within the edit control).*/
	void SetRightAlign(BOOL bRightAlign = TRUE);
	
	//@cmember
	/* Indicates whether drop-down button is right-aligned within the edit control.*/
	BOOL IsRightAligned() const;

	//@cmember
	/* Manipulates the border settings.*/
	void SetBorder(WORD wLeftBorder, WORD wRightBorder);
	
	//@cmember
	/* Retrieves the border settings.*/
	DWORD GetBorders() const;

public:
	
	//@access Operations

	// Methods to set/get the bitmap on the button
	//@cmember
	/* Sets bitmap on the button.*/
	BOOL SetBitmap(UINT nBmpID);
	
	//@cmember
	/* Sets bitmap on the button.*/
	BOOL SetBitmap(LPCTSTR lpszBmpName);
	
	//@cmember
	/* Removes the button.*/
	BOOL SetBitmap();						// Removes button
	
	//@cmember
	/* Sets bitmap on the button.*/
	HBITMAP GetBitmap() const;

	//@cmember
	/* Forces a redraw of the button.*/
	void InvalidateBtn();

protected:

	//@access Overridable
 
	//@cmember
	/* Called when the button has been clicked.*/
	virtual void OnClicked();

	//@cmember
	/* Draws the button.*/
	virtual void DrawBtn(CDC& dc, CRect r);

	//@access Overrides

	//@cmember
	/* Called when the size and position of the client area needs */
    // to be calculated.
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	
	//@cmember
	/* Called when the user presses the left mouse button while the */
	// cursor is within a nonclient area of the CWnd object. 
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	
	//@cmember
	/* Called when the user releases teh left mouse button.*/
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	//@cmember
	/* Called after the CWnd object has been moved.*/
	afx_msg void OnMove(int x, int y);

	//@cmember
	/* Called when the mouse cursor moves. */
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	//@cmember
	/* Called every time the mouse is moved.*/
	afx_msg UINT OnNcHitTest(CPoint point);
	
	//@cmember
	/* Called when the nonclient area needs to be painted.*/
	afx_msg void OnNcPaint();
	
	//@cmember
	/* Called to inform CWnd to cancel any internal mode. */
	afx_msg void OnCancelMode();
	
	//@cmember
	/* Called when a change is made in the system color setting. */
	afx_msg void OnSysColorChange();
	
	//@cmember
	/* Called when an application changes the enabled state of */
	// the CWnd object. 
	afx_msg void OnEnable(BOOL bEnable);
	
	//@cmember
	/* Called when a nonsystem key is pressed.*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	//@cmember
	/* Called when a system key is depressed.*/
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:

	//@cmember
	/* Determines whether the given point (in screen or client */
	// coordinates) is a hit on the button.
	virtual BOOL HitTestBtn(CPoint point, BOOL bClient = FALSE) const;

// Implementation
public:
	virtual ~SECDropEdit();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:
	// Implementation, data members.
	//@cmember
	/* The window rect of the button (in our*/
	// parents client coordinates)
	CRect   m_btnRect;

	//@cmember
	/* Bitmap drawn on face of button*/
	CBitmap m_bmp;

	//@cmember
	/* Resource ID of above bitmap*/
	LPCTSTR m_lpszBmpName;

	//@cmember
	/* Current state*/
	WORD    m_wState;

	//@cmember
	/* Left hand Gap between bitmap and border*/
	WORD    m_wLeftBorder;

	//@cmember
	/* Right hand Gap between bitmap and border*/
	WORD    m_wRightBorder;

	// Bit settings in m_wState
	enum States
	{
		Pressed    = 0x0001,		// Button is currently down	
		Capture    = 0x0002,		// We are tracking a left click on the button
		RightAlign = 0x0004			// The button is right aligned within the edit
	};

	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

// Function to take the given rectange in the given dc and draw it disabled.
void SECDrawDisabled(CDC& dc, int x, int y, int nWidth, int nHeight);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __DROPEDIT_H__


