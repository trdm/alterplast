// ===================================================================================
// 							Class Specification :
//		COXFrameWndSizeDock & COXMDIFrameWndSizeDock & COXMDIChildWndSizeDock
//				COXSizeViewBar & COXSizeBarWrapper & COXDockDocument
// ===================================================================================

// Header file : OXFrameWndDock.h

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CFrameWnd / CMDIFrameWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
//	The 2 classes COXFrameWndSizeDock & COXMDIFrameWndSizeDock provide the frame window
//	functionality necessary to support sizeable control bars. If you wish to use sizeable
//	control bars, you should derive your main frame class (CMainFrame) from them
//	(COXMDIFrameWndSizeDock if an MDI app, COXFrameWndSizeDock if an SDI app).
//	The two classes have almost identical functions to work with control bars.
//
//
// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

/*

We introduce Docking Views Framework now. To accomplish this task there were added 
a lot of new functions to COXMDIFrameWndSizeDock and COXMDIChildWndSizeDock classes
and built new three new classes: COXSizeViewBar, COXSizeBarWrapper and COXDockDocument.

To implement Docking Views in your MDI application generally you have to take next steps:

1)	Your mainframe window have to be derived from COXMDIFrameWndSizeDock
	instead of CMDIFrameWnd;
2)	Your MDIChild window have to be derived from COXMDIChildWndSizeDock
	instead of CMDIChildWnd;
3)	Your document have to be derived from COXDockDocument instead of CDocument;
4)	Make sure you call EnableDocking() in your CMainFrame::OnCreate() function;
5)	In the MDIChild system menu you will find "Dock Window" item. If you choose it,
	MDIChild window will set to dockable state and docked to appropriate side of
	the mainframe window. To undock window: right click over caption; system menu
	will be displayed; "Dock Window" item will be checked; choose it.



By default MDIChild window will be created with CBRS_ALIGN_ANY style that means it could 
be docked to any side of mainframe window. If you want to set your own docking style
you have to derive from COXMDIChildWndSizeDock your own CChildWnd class and set in 
constructor m_dwDockStyle variable to the style you need. 


At the moment we are not providing any functionality to change the docking style at 
run-time and currently we are not supporting COLEDocument derivation. Also Docking Views 
state cannot be saved/loaded to/from registry at this stage. If MDIChild window 
has any child windows other than CView they won't be displayed in Dockable state.


All three new classes are created as internal classes to provide docking functionality
and will be probably changed in the future. COXMDIChildWndSizeDock uses COXSizeViewBar
as helper class to dock views. And COXSizeViewBar uses COXSizeBarWrapper and 
COXSizableMiniDockFrameWnd to display views while docked or floating correspondingly.


Also there were added lots of functions to documented COXMDIFrameWndSizeDock and 
COXMDIChildWndSizeDock classes. Although some of them are public they are not supposed 
to be used explicitly in your applications and only provided here as internal functions
to be called by different classes in Docking Framework.


Below you will find some useful properties and methods that you can use in your
derivation of above described classes to control Docking Framework.

COXMDIFrameWndSizeDock:

	BOOL IsDockable(CWnd* pWnd);
	// --- In  :	pWnd	-	pointer to a child window
	// --- Out :	
	// --- Returns: TRUE if pWnd is currently in Dockable state (Dockable MDIChild), 
	//				or FALSE otherwise
	// --- Effect :


COXMDIChildWndSizeDock:

	// message ID that is sent to window when user want to toggle 
	// MDIChild window state (dockable/undockable)
	static UINT m_nDockMessageID;

	// string resource used as text of menu item to toggle the MDIChild window state 
	CString m_sDockMenuItem;

	// docking style, could be:
	//
	//	CBRS_ALIGN_TOP		Allows docking at the top of the client area.
	//	CBRS_ALIGN_BOTTOM	Allows docking at the bottom of the client area.
	//	CBRS_ALIGN_LEFT		Allows docking on the left side of the client area.
	//	CBRS_ALIGN_RIGHT	Allows docking on the right side of the client area.
	//	CBRS_ALIGN_ANY		Allows docking on any side of the client area.
	//	CBRS_FLOAT_MULTI	Allows multiple control bars to be floated in a 
	//						single mini-frame window.
	//	0					(that is, indicating no flags), the control bar will not dock.
	//
	// By default CBRS_ALIGN_ANY is set
	DWORD m_dwDockStyle;

	BOOL MakeItDockable(BOOL bDockable);
	// --- In  :	bDockable	- 	TRUE if MDIChild window is to be docked,
	//								FALSE if MDIChild window is to be undocked
	// --- Out : 
	// --- Returns: TRUE if succeeds, FALSE otherwise
	// --- Effect : Dock/Undock MDIChild window
	inline BOOL IsDockable() { return m_bDockable; }
	// --- Returns: TRUE if MDIChild window is in Dockable state,
	//				or FALSE otherwise

	BOOL IsDockableFrame();
	// --- Returns: TRUE if MDIFrame window is derived from COXMDIFrameWndSizeDock,
	//				or FALSE otherwise
	CMDIChildWnd* MDIGetActive(BOOL* pbMaximized=NULL);
	// --- In  :	pbMaximized	- A pointer to a BOOL return value. Set to TRUE on 
	//				return if the window is maximized; otherwise FALSE	
	// --- Returns: active MDIChild window (takes into account Dockable MDIChild window)

*/


#ifndef __FRAMESIZEDOCK_H__
#define __FRAMESIZEDOCK_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxole.h>
#include "OXSizeCtrlBar.h"		// used class specification
#include "OXSzMiniDockFrmWnd.h"

// Window arangement positions
#define CBRS_ARRANGE_TOPLEFT		1
#define CBRS_ARRANGE_TOPRIGHT		2
#define CBRS_ARRANGE_BOTTOMLEFT		4
#define CBRS_ARRANGE_BOTTOMRIGHT	8

#define CBRS_ARRANGE_LEFT  			( CBRS_ARRANGE_TOPLEFT  | CBRS_ARRANGE_BOTTOMLEFT )
#define CBRS_ARRANGE_TOP			( CBRS_ARRANGE_TOPRIGHT | CBRS_ARRANGE_TOPLEFT )

#define WM_ACTIVATEVIEWBAR			(WM_USER + 3)

// special constants to define the source of activation of the view
#define ID_SOURCE_MDICHILD			0x00000001
#define ID_SOURCE_BARWRAPPER		0x00000002
#define ID_SOURCE_MINIDOCK			0x00000003

#ifdef _OXIE4PATCH
#define WM_DOCKCHILDWND				(WM_USER + 4)
#endif // _OXIE4PATCH


class OX_CLASS_DECL COXMDIChildWndSizeDock;
class OX_CLASS_DECL COXSizeViewBar;
class OX_CLASS_DECL COXMDIFrameWndSizeDock;

void ArrangeWindowsInWindow(CWnd* pParentWnd, CObArray& arrWnd, DWORD dwOrient);
void ForceLayoutAdjust(CControlBar* pBar);

class OX_CLASS_DECL COXFrameWndSizeDock : public CFrameWnd
{
DECLARE_DYNCREATE(COXFrameWndSizeDock)

// Data members -------------------------------------------------------------
public:

protected:

private :

// Member functions ---------------------------------------------------------
public:

	void SetDockState(const CDockState& state);
	void GetDockState(CDockState& state) const;

	// --- In  : dwDockStyle : specifies which sides of the frame window control bars can be docked
	//						   to. It can be a combination of  CBRS_ALIGN_TOP, CBRS_ALIGN_BOTTOM,
	//						   CBRS_ALIGN_LEFT,  CBRS_ALIGN_RIGHT or CBRS_ALIGN_ANY.
	//						   Note that CBRS_FLOAT_MULTI is not supported.
	// --- Out : 
	// --- Returns : 
	// --- Effect : This function is equivalent to CFrameWnd:EnableDocking(), but should be called
	//				if your frame window is to support sizeable docking.
	//				NB: CFrameWnd::EnableDocking() is NOT virtual, so you must ensure you do not
	//					inadvertently call it for a COXFrameWndSizeDock or COXMDIFrameWndSizeDock
	//					object.
    void EnableDocking(DWORD dwDockStyle);


	// --- In  : dwDockStyle : specifies which sides of the frame window, the control bars
	//						   should be tiled for. It can be a combination of  CBRS_ALIGN_TOP,
	//						   CBRS_ALIGN_BOTTOM, CBRS_ALIGN_LEFT, CBRS_ALIGN_RIGHT or CBRS_ALIGN_ANY.
	// --- Out : 
	// --- Returns : 
	// --- Effect : Tiles the sizeable control bars within a frame so they take up
	//				equal size. You should call RecalcLayout() after calling this function.
    void TileDockedBars(DWORD dwDockStyle = CBRS_ALIGN_ANY);


	// --- In  : dwOrient specifies the orientation of the arrangement.
	//				It can be one of:
	//					CBRS_ARRANGE_TOPLEFT
	//					CBRS_ARRANGE_TOPRIGHT
	//					CBRS_ARRANGE_BOTTOMLEFT
	//					CBRS_ARRANGE_BOTTOMRIGHT
	// --- Out : 
	// --- Returns : 
	// --- Effect : Arranges the floating control bars within a frame.
	void ArrangeFloatingBars(DWORD dwOrient);


	// --- In  : arrWnd : array with window pointers
	//			 dwOrient specifies the orientation of the arrangement.
	//				It can be one of:
	//					CBRS_ARRANGE_TOPLEFT
	//					CBRS_ARRANGE_TOPRIGHT
	//					CBRS_ARRANGE_BOTTOMLEFT
	//					CBRS_ARRANGE_BOTTOMRIGHT
	// --- Out : 
	// --- Returns : 
	// --- Effect : Arranges the windows pointed at in arrWnd, within a frame.
	void ArrangeWindows(CObArray& arrWnd, DWORD dwOrient);


	// --- In  : 
	// --- Out : arrWnd : arrary with all floating controlbars at that moment
	// --- Returns : 
	// --- Effect : gets all floating controlbars
	void GetFloatingBars(CObArray& arrWnd);
	

	// --- In  : pszProfileName : specifies the key to use within the registry/ini.
	// --- Out : 
	// --- Returns : 
	// --- Effect : Loads the saved state of the bars from the Registry/ini, including sizes.
	//				This function calls CFrameWnd::LoadBarState() to do most of it’s work. 
	void LoadSizeBarState(LPCTSTR pszProfileName);

	// --- In  : pszProfileName : specifies the key to use within the registry/ini.
	// --- Out : 
	// --- Returns : 
	// --- Effect : Saves the state of the bars to the Registry/ini, including sizes.
	//				This function calls CFrameWnd::SaveBarState() to do most of it’s work. 
	//				Notes:
	//					1. This function destroys any windows constructed with the SZBARF_AUTOTIDY
	//						flag. CControlBars created with this flag will not generally be around
	//						next time in.
	//					2. Positions and sizes are saved in a binary format, based on the ID of the
	//						control bars. You should ensure all your CControlBars have unique ID’s.
	void SaveSizeBarState(LPCTSTR pszProfileName);

	
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : destroys all controlbars managed by this frame window
	void DestroyDynamicBars();

	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : floats controlbar managed by this frame window
	void FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP);

	void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif // _DEBUG

    COXFrameWndSizeDock();           

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
    virtual ~COXFrameWndSizeDock();


protected:
    // Generated message map functions
    //{{AFX_MSG(COXFrameWndSizeDock)
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

//private:	
	void SaveBarSizes(LPCTSTR pszSection, BOOL bSave);

	friend COXMDIFrameWndSizeDock;
};


// COXMDIFrameWndSizeDock provides Docking Views functionality
class OX_CLASS_DECL COXMDIFrameWndSizeDock : public CMDIFrameWnd
{
DECLARE_DYNCREATE(COXMDIFrameWndSizeDock)

// Data members -------------------------------------------------------------
public:

protected:
	// inner var to save the active Docking MDIChild window
	COXMDIChildWndSizeDock* m_pActiveDockChild;		

	// inner var that is set to TRUE when destroying of the window is in process
	BOOL m_bBeingDestroyed;

private :

// Member functions ---------------------------------------------------------
public:

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
    void EnableDocking(DWORD dwDockStyle)
                { ((COXFrameWndSizeDock*)this)->EnableDocking(dwDockStyle); };

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
    void TileDockedBars(DWORD dwDockStyle = CBRS_ALIGN_ANY)
                { ((COXFrameWndSizeDock*)this)->TileDockedBars(dwDockStyle); };

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
	void ArrangeFloatingBars(DWORD dwOrient);

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
	void ArrangeWindows(CObArray& arrWnd, DWORD dwOrient);
		
	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
	void GetFloatingBars(CObArray & arrWnd)
                { ((COXFrameWndSizeDock*)this)->GetFloatingBars(arrWnd); };

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
	void LoadSizeBarState(LPCTSTR pszProfileName)
				{ ((COXFrameWndSizeDock*)this)->LoadSizeBarState(pszProfileName); };
			
	// --- In  : pBar : Points to the control bar to be floated in an MDI child frame.
	//			 point : Position in screen co-ordinates of the top-left corner of the control bar
	//			 dwStyle : Orientation of the control bar within the parent window.
	//					   Provided for consistency.
	//			 bVisible: 
	// --- Out : 
	// --- Returns : 
	// --- Effect : This function floats the specified control bar as an MDI Child window
	//				(as opposed to a mini-frame window) in the MDI client area (similar to
	//				the docking and non-docking views in the MS Visual C++ IDE).
	void FloatControlBarInMDIChild(CControlBar* pBar, CPoint point, 
		DWORD dwStyle = CBRS_ALIGN_TOP, BOOL bVisible = TRUE);

	// --- In  : pBar : Points to the control bar to be floated in a mini frame.
	//			 point : Position in screen co-ordinates of the top-left corner of the control bar
	//			 dwStyle : Orientation of the control bar within the parent window.
	//					   Provided for consistency.
	// --- Out : 
	// --- Returns : 
	// --- Effect : This function floats the specified control bar as a mini frame window
	void UnFloatInMDIChild(CControlBar* pBar, CPoint point, 
		DWORD dwStyle = CBRS_ALIGN_TOP);

	// --- In  : (See COXFrameWndSizeDock above)
	// --- Out : (See COXFrameWndSizeDock above)
	// --- Returns : (See COXFrameWndSizeDock above)
	// --- Effect : (See COXFrameWndSizeDock above)
	void FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP)
		{ ((COXFrameWndSizeDock*)this)->FloatControlBar(pBar, point, dwStyle); };

	// --- Effect :	Activate and deactivate child windows in a special way (takes into
	//				account Docking MDIChild windows)
	void HookActivation();

	// --- In  :	pWnd	-	pointer to a child window
	// --- Out :	
	// --- Returns: TRUE if pWnd is currently in Dockable state (Dockable MDIChild), 
	//				or FALSE otherwise
	// --- Effect :
	BOOL IsDockable(CWnd* pWnd);

	// --- Returns: TRUE if currently active undocked MDIChild window is maximized, 
	//				or FALSE otherwise 
	BOOL IsFrontChildMaximized();

	// --- Returns: TRUE if destroying of the window is in process, 
	//				or FALSE otherwise 
	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }

	// --- In  :	pWnd	-	pointer to a child window
	// --- Out :	
	// --- Returns: 
	// --- Effect : sets pWnd as active Dockable MDIChild window
	void SetActiveChild(CWnd* pWnd);

	// --- Returns: active Dockable MDIChild window
	COXMDIChildWndSizeDock* GetActiveChild();

// Operations
	void MDIActivate(CWnd* pWndActivate);
	CMDIChildWnd* MDIGetActive(BOOL* pbMaximized=NULL) const;
	void MDIMaximize(CWnd* pWnd);
	void MDIRestore(CWnd* pWnd);

	BOOL IsChild(const CWnd* pWnd);
	BOOL OnBarCheck(UINT nID);

	// --- In  : pszProfileName : specifies the key to use within the registry/ini.
	// --- Out : 
	// --- Returns : 
	// --- Effect : Saves the state of the bars to the Registry/ini, including sizes.
	//				This function calls CFrameWnd::SaveBarState() to do most of it’s work. 
	//				Notes:
	//					1. This function destroys any windows constructed with the SZBARF_AUTOTIDY
	//						flag. CControlBars created with this flag will not generally be around
	//						next time in.
	//					2. Positions and sizes are saved in a binary format, based on the ID of the
	//						control bars. You should ensure all your CControlBars have unique ID’s.
	void SaveSizeBarState(LPCTSTR pszProfileName);

	void SaveBarState(LPCTSTR lpszProfileName) const;
	void GetDockState(CDockState& state) const;

// Implementation

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COXMDIFrameWndSizeDock)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
	virtual CFrameWnd* GetActiveFrame();
	virtual BOOL DestroyWindow();
	protected:
	virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
    COXMDIFrameWndSizeDock();           // protected constructor used by dynamic creation
	virtual ~COXMDIFrameWndSizeDock();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// helper function to undock all child windows before destroy them
	void UndockAllViews();

    // Generated message map functions
    //{{AFX_MSG(COXMDIFrameWndSizeDock)
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg void OnWindowNew();
	afx_msg void OnUpdateMDIWindowCmd(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()

private:

};

////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Class COXDockDocument is the main document data abstraction
// It overwrites virtual CDocument functions to support Docking Views functionality
class OX_CLASS_DECL COXDockDocument : public CDocument
{
	DECLARE_DYNAMIC(COXDockDocument)

protected:
// Data
	// inner var that is set to TRUE when destroying of the document is in process
	BOOL m_bBeingDestroyed;

public:
// Constructor
	COXDockDocument();

// Attributes
public:

// Operations
	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }
	// --- Returns: TRUE if destroying of the document is in process, 
	//				or FALSE otherwise 

// Overridables
	virtual void OnCloseDocument();

	// advanced overridables, closing down frame/doc, etc.
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);

// Implementation
protected:
	virtual ~COXDockDocument();

	// implementation helpers
	virtual void UpdateFrameCounts();

	CFrameWnd* GetParentFrame(CView* pView) const;

	// helper function to undock all document views before destroy it
	void UndockAllViews();

protected:
	// file menu commands
	//{{AFX_MSG(CDocument)
	afx_msg void OnFileClose();
	//}}AFX_MSG
	// mail enabling
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COXSizeBarWrapper window
// This class wraps a view while MDIChild 
class OX_CLASS_DECL COXSizeBarWrapper : public CWnd
{
// Construction
public:
	COXSizeBarWrapper();

// Data
protected:
	COXSizeViewBar* m_pParentWnd;
	CView* m_pView;

	BOOL m_bBeingDestroyed;

// Operations
public:
	BOOL Create(COXSizeViewBar* pParentWnd);

	void WrapView(COXSizeViewBar* pParentWnd, CView* pView);
	void UnwrapView();

	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSizeBarWrapper)
	public:
	//}}AFX_VIRTUAL
public:
	virtual ~COXSizeBarWrapper();

// implementation
protected:
	// Generated message map functions
protected:
	//{{AFX_MSG(COXSizeBarWrapper)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// This class provides docking functionality for MDIChild windows. 
// Operates like "Control Center" between MDIChild window and its represantation
// while it is dockable
class OX_CLASS_DECL COXSizeViewBar : public COXSizeControlBar
{
DECLARE_DYNCREATE(COXSizeViewBar)

// Data members -------------------------------------------------------------
public:

protected:
	COXMDIChildWndSizeDock* m_pChildWnd;
	CView* m_pView;

	COXSizeBarWrapper m_WrapperWnd;
	BOOL m_bMaximized;

	BOOL m_bBeingDestroyed;

private :

// Member functions ---------------------------------------------------------
public:

	COXSizeViewBar(int nStyle=SZBARF_STDMOUSECLICKS|SZBARF_NOCLOSEBTN|SZBARF_NOCAPTION);
	// --- In  : nStyle : See COXSizeControlBar::COXSizeViewBar for details
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state

    virtual void OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See COXSizeControlBar::OnSizedOrDocked for details

	virtual ~COXSizeViewBar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

	BOOL AttachMDIChild(COXMDIChildWndSizeDock* pWnd);
	BOOL DetachMDIChild(BOOL bRedraw=TRUE);

	BOOL QueryCloseView();

	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }

	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const { 
		UNREFERENCED_PARAMETER(pTI); 
		UNREFERENCED_PARAMETER(point); 
		return -1; 
	}
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	
protected:

    //{{AFX_MSG(COXSizeViewBar)
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);
	afx_msg LONG OnAddContextMenuItems(UINT wParam, LPARAM lParam);
	afx_msg LONG OnActivateViewBar(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

	friend COXMDIFrameWndSizeDock;
	friend COXDockDocument;
	friend COXSizableMiniDockFrameWnd;
	friend COXSizeBarWrapper;

private:

};

// COXMDIChildWndSizeDock supports docking functionality using
// COXSizeViewBar and have to be used together with COXFrameWndSizeDock
class OX_CLASS_DECL COXMDIChildWndSizeDock : public CMDIChildWnd
{
DECLARE_DYNCREATE(COXMDIChildWndSizeDock)

// Data members -------------------------------------------------------------
public:
	// message ID that is sent to window when user want to toggle 
	// MDIChild window state (dockable/undockable)
	static UINT m_nDockMessageID;

protected:
	COXSizeViewBar m_dockBar;
	BOOL m_bDockable;

	// string resource used as text of menu item to toggle the MDIChild window state 
	CString m_sDockMenuItem;

	// docking style, could be:
	//
	//	CBRS_ALIGN_TOP		Allows docking at the top of the client area.
	//	CBRS_ALIGN_BOTTOM	Allows docking at the bottom of the client area.
	//	CBRS_ALIGN_LEFT		Allows docking on the left side of the client area.
	//	CBRS_ALIGN_RIGHT	Allows docking on the right side of the client area.
	//	CBRS_ALIGN_ANY		Allows docking on any side of the client area.
	//	CBRS_FLOAT_MULTI	Allows multiple control bars to be floated in a 
	//						single mini-frame window.
	//	0					(that is, indicating no flags), the control bar will not dock.
	//
	// By default CBRS_ALIGN_ANY is set
	DWORD m_dwDockStyle;

	// inner var that is set to TRUE when destroying of the window is in process
	BOOL m_bBeingDestroyed;

private :

// Member functions ---------------------------------------------------------
public:

	virtual BOOL Create(LPCTSTR lpszClassName,
				LPCTSTR lpszWindowName,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
				const RECT& rect = rectDefault,
				CMDIFrameWnd* pParentWnd = NULL,
				CCreateContext* pContext = NULL);

    void EnableDocking(DWORD dwDockStyle)
                { ((COXFrameWndSizeDock*)this)->EnableDocking(dwDockStyle); };
    void TileDockedBars(DWORD dwDockStyle = CBRS_ALIGN_ANY)
                { ((COXFrameWndSizeDock*)this)->TileDockedBars(dwDockStyle); };

	void GetFloatingBars(CObArray & arrWnd)
                { ((COXFrameWndSizeDock*)this)->GetFloatingBars(arrWnd); };

	void DestroyDynamicBars()
                { ((COXFrameWndSizeDock*)this)->DestroyDynamicBars(); };

	void LoadSizeBarState(LPCTSTR pszProfileName)
				{ ((COXFrameWndSizeDock*)this)->LoadSizeBarState(pszProfileName); };
			
	void SaveSizeBarState(LPCTSTR pszProfileName)
				{ ((COXFrameWndSizeDock*)this)->SaveSizeBarState(pszProfileName); };

	//////////////////////////////////////
	BOOL MakeItDockable(BOOL bDockable);
	// --- In  :	bDockable	- 	TRUE if MDIChild window is to be docked,
	//								FALSE if MDIChild window is to be undocked
	// --- Out : 
	// --- Returns: TRUE if succeeds, FALSE otherwise
	// --- Effect : Dock/Undock MDIChild window
	inline BOOL IsDockable() { return m_bDockable; }
	// --- Returns: TRUE if MDIChild window is in Dockable state,
	//				or FALSE otherwise

	BOOL IsDockableFrame();
	// --- Returns: TRUE if MDIFrame window is derived from COXMDIFrameWndSizeDock,
	//				or FALSE otherwise
	CMDIChildWnd* MDIGetActive(BOOL* pbMaximized=NULL);
	// --- In  :	pbMaximized	- A pointer to a BOOL return value. Set to TRUE on 
	//				return if the window is maximized; otherwise FALSE	
	// --- Returns: active MDIChild window (takes into account Dockable MDIChild window)

	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }
	// --- Returns: TRUE if destroying of the document is in process, 
	//				or FALSE otherwise 
	//////////////////////////////////////

// Operations
	void MDIDestroy();
	void MDIActivate();
	void MDIMaximize();
	void MDIRestore();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COXMDIChildWndSizeDock)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

protected:
    COXMDIChildWndSizeDock();           // protected constructor used by dynamic creation

    virtual ~COXMDIChildWndSizeDock();

	BOOL UpdateClientEdge(LPRECT lpRect = NULL);
	
	virtual void OnAddContextMenuItems(HMENU hMenu);

    // Generated message map functions
    //{{AFX_MSG(COXMDIChildWndSizeDock)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd); 
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	afx_msg void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);
	afx_msg void OnMakeItDockable();
    DECLARE_MESSAGE_MAP()

	friend COXSizeViewBar;
	friend COXMDIFrameWndSizeDock;
	friend COXDockDocument;
	friend COXSizableMiniDockFrameWnd;

private:

};

#endif // __FRAMESIZEDOCK_H__
