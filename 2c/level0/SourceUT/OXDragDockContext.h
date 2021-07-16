// ===================================================================================
// 					Class Specification : COXDragDockContext
// ===================================================================================

// Header file : OXDragDockContext.h

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CDockContext

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// This replaces the MFC CDockContext class. It provides the "dragging" user interface,
// and includes some extra features that try to indicate exactly where a bar will
// dock. 
//
// The gateway MFC gives us to work with is:
// 1) CControlBar::EnableDocking() creates the dock context for the control bar - so it's
//    important to use the COX...Frame.. implementation of EnableDocking().
//
// 2) Other than the constructor/destructor, the only function access outside this class 
//    is to StartDrag(CPoint pt), called when we want to start dragging the control bar.
//     
// The other important tie-ins are the call to Dock/Float control bar which we need to handle
// with the same interpretation of parameters as the existing MFC implementation.

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __DRAGDOCKCTXT_H__
#define __DRAGDOCKCTXT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxpriv.h>


class COXSizeDockBar;

class OX_CLASS_DECL COXDragDockContext : public CDockContext
{
// Data members -------------------------------------------------------------
public:

    CRect			m_rectDragDock;        // rectangle indicating where we'll dock

protected:

	CRect			m_rectDragHorzAlone;		
	CRect			m_rectDragVertAlone;
	COXSizeDockBar*	m_pTargetDockBar;
	CPoint			m_ptStart;

private :

// Member functions ---------------------------------------------------------
public:

    COXDragDockContext(CControlBar* pBar);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state

    virtual void StartDrag(CPoint pt);
	// --- In  : pt : point where dragging starts
	// --- Out : 
	// --- Returns :
	// --- Effect : only thing called externally

	virtual void ToggleDocking();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : called to toggle docking

    void Move(CPoint pt);
	// --- In  : pt : point to where window has moved
	// --- Out : 
	// --- Returns :
	// --- Effect : called when mouse has moved

    void EndDrag();            
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : drop window

    void CancelDrag();  
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : drag cancelled

    void OnKey(int nChar, BOOL bDown);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : keyboardhit

    BOOL Track();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : called when window is dragged

    void DrawFocusRect(BOOL bRemoveRect = FALSE);
	// --- In  : bRemoveRect : whether or not to erase the previous focus rect
	// --- Out : 
	// --- Returns :
	// --- Effect : draws a rectangle to indicate focus

    void UpdateState(BOOL* pFlag, BOOL bNewValue);
	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : Update the focus state of the dragged window

    DWORD CanDock();
	// --- In  :
	// --- Out : 
	// --- Returns :whether or not the window can be docked
	// --- Effect : 

    CDockBar* GetDockBar();
	// --- In  :
	// --- Out : 
	// --- Returns : the dockbar where docking will take place
	// --- Effect : 

    virtual ~COXDragDockContext();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:

private:	

};

#endif // __DRAGDOCKCTXT_H__
