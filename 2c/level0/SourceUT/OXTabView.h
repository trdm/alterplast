// ==========================================================================
//						   Class Specification : 
//							COXTabViewContainer
// ==========================================================================

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
                          
// //////////////////////////////////////////////////////////////////////////

/*

For most applications it's not enough to use only one window to provide its all output. 
There are different solutions for this problem like splitters or docking windows. 
But they usually have common inconvenience: all windows are shown at the same time.
They take a precious screen space while could be rarely used. 

COXShortcutBar control can be used in order to show a number of child windows while
keeping active (fully displayed) only one at a time. But COXShortcutBar was primarily 
designed to show icons and have a set of notifications that make sense only while
using such controls as treeview or listview.

Very good example of how the problem could be resolved can be found in Developer Studio
IDE. For instance "Output" window (with "Build", "Debug", "Find in Files..." panes) or 
"Result List" window (with "Search", "Lookup", "See Also" and "History" panes). We call 
them TabViews.

TabViews can be a good alternative for splitter window when you need to have more than 
one view per document. Also TabViews can be used within docking window and used as a 
container for associated windows that usually implemented as dialog bars.

So we designed new class that implements TabViews: COXTabViewContainer.

COXTabViewContainer is easy to use. If you previously worked with splitter windows,
the implementation of TabViews will be familiar to you. 

Here is the list of steps that should be taken in order to deploy TabViews in 
your application:

	First Case:		COXTabViewContainer will be used as a container for document view(s).

		1)	Embed a COXTabViewContainer member variable in the parent frame (main frame
			window for SDI application, MDIChild window for MDI application).
		2)	Override the parent frame's CFrameWnd::OnCreateClient member function.
		3)	From within the overridden OnCreateClient, call the Create member function 
			of COXTabViewContainer. In this function you have to specify the parent 
			window and optionally you can specify the initial rectangle, window styles
			and window ID.
		4)	After COXTabViewContainer window was successfully created you can populate
			it with window objects using AddPage or InsertPage function. If you are
			inserting view object you have to specify runtime class and context 
			information in order not to break document/view architecture. If you are
			adding window object that is not a document view then you have to create 
			it before adding to COXTabViewContainer window. In AddPage or InsertPage
			functions you can specify text that will be used as page title in tab button.

	Example:

	BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
	{
		// TODO: Add your specialized code here and/or call the base class

		UNREFERENCED_PARAMETER(lpcs);

		if(!m_TabViewContainer.Create(this))
			return FALSE;

		if(!m_TabViewContainer.AddPage(pContext->m_pNewViewClass,
			pContext,_T("Primary View")))
			return FALSE;
		if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CMyView2),
			pContext,_T("View2")))
			return FALSE;
		
		m_TabViewContainer.SetActivePageIndex(0);
	
		return TRUE;
	}




	Second Case:	COXTabViewContainer will be used as a container for windows within
					control bar.

		1)	Create your own CControlBar-derived class (you can use our 
			COXSizeControlBar as parent class if you need sizable docking windows).
			Let's call CMyControlBar.
		2)	Embed a COXTabViewContainer member variable in this class.
		3)	Override  CMyControlBar::OnCreate member function.
		4)	From within the overridden OnCreate, call the Create member function 
			of COXTabViewContainer. In this function you have to specify the parent 
			window and optionally you can specify the initial rectangle, window styles
			and window ID.
		5)	After COXTabViewContainer window was successfully created you can populate
			it with window objects using AddPage or InsertPage function. You have to 
			create window object before adding it to COXTabViewContainer. In AddPage or 
			InsertPage functions you can specify text that will be used as page title 
			in tab button.
		6)	Override  CMyControlBar::OnSize member function and resize in it 
			COXTabViewContainer object as appropriate


	Example:

	int CMyControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		if (COXSizeControlBar::OnCreate(lpCreateStruct) == -1)
			return -1;

		if(!m_TabViewContainer.Create(this,rect))
			return -1;

		// edit control
		if(!edit.Create(WS_CHILD|ES_MULTILINE|ES_AUTOHSCROLL|
			ES_AUTOVSCROLL|WS_HSCROLL|WS_VSCROLL,CRect(0,0,0,0),
			&m_TabViewContainer,1))
			return -1;
		m_TabViewContainer.AddPage(&edit,_T("Edit"));

		// list box
		if(!listBox.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL,
			CRect(0,0,0,0),&m_TabViewContainer,2))
			return -1;
		m_TabViewContainer.AddPage(&listBox,_T("ListBox"));

		// list control
		if(!listCtrl.Create(WS_CHILD|LVS_REPORT,
			CRect(0,0,0,0),&m_TabViewContainer,3))
			return -1;
		m_TabViewContainer.AddPage(&listCtrl,_T("List"));

		// tree control
		if(!treeCtrl.Create(WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,
			CRect(0,0,0,0),&m_TabViewContainer,4))
			return -1;
		m_TabViewContainer.AddPage(&treeCtrl,_T("Tree"));

		m_TabViewContainer.SetActivePageIndex(0);
	
		return 0;					
	}



The most challenging problem was to support scrolling functionality for windows
that would be used as COXTabViewContainer pages. Unfortunately, different windows
object that support scrolling (CEdit, CTreeCtrl, CListBox, CListCtrl, CRichEditCtrl,
CEditView, CTreeView, CListView, CScrollView to name just the standrad ones)
implement it in a different way and anyway the handling of scrolling happens
internally in these objects so in order to control it we should have provided 
derivations for all of above mentioned classes. Luckily we managed to resolve this
problem in different way so it doesn't require any efforts from programmer's side.
Actually, the only thing you have to do is to declare the class of window object 
that is going to be used as COXTabViewContainer page in a specific way.

If you declare you class as follows:
	class CMyView : public CEditView

now you have to do that in the following way:
	class CMyView : public COXTabViewPage<CEditView>


or if you don't derive your own class and just use an object of existing one, e.g.:

	CEdit m_edit;

instead you have to define it as:

	COXTabViewPage<CEdit> m_edit;


COXTabViewPage is internal template based Ultimate Toolbox class that was designed 
specifically to provide smooth integration of scrolling functionality for any
window within COXTabViewContainer object. All the details of implementation is hidden 
for different type of windows is hidden. There is no any public functions that you 
should specifically use. 

But there is one limitation. The base class that is used for derivation has to have 
default constructor. Some classes doesn't have it (e.g. CFormView). You can resolve 
this problem through using intermediate class that will be derived from the one 
that doesn't have default constructor and implement it. Then you just derive your 
window  object class that will be used as COXTabViewContainer page from this 
intermediate class. Out of standard only CFormView doesn't have default constructor.
Below you will find the steps that should be taken in order to derive classes from 
CFormView class.

1)	Use Class Wizard to build CFormView derived class on the base of dialog template 
	as you usually do (let's call it CMyFormView)
2)	CMyFormView will have default constructor which uses CFormView constructor with 
	dialog ID specified.
3)	define another class that will be used as view in your application:
	
		class CMyFormTabView : public COXTabViewPage<CMyFormView>
		{
		protected: // create from serialization only
		DECLARE_DYNCREATE(CMyFormView)
		};




The steps that should be taken in order to implement COXTabViewContainer in CControlBar
derived window can be applied in general case too. We just decided to show it using 
CControlBar derived window because we feel like it's going to be used as parent window
for COXTabViewContainer in most cases.

Above we described the process of creating and populating of COXTabViewContainer object.
In most cases that would be all the code you need. For those who need to change 
dynamically the contents of COXTabViewContainer object we provide a set of the 
following functions.

In order to remove any page at run time you have to use DeletePage function.

To access scroll bar objects that we use internally in order to provide scrolling
functionality for COXTabViewContainer pages you have to call GetHorzScrollBar and
GetVertScrollBar functions.

To set/retrieve page title that is displayed in corresponding tab button use 
GetPageTitle and SetPageTitle functions.

To set/retrive active page index call GetActivePageIndex and SetActivepageIndex 
functions.

Refer to the class reference for list and description of all public functions.


Also take look at the following samples that can be found in .\samples\gui 
subdirectory of your ultimate Toolbox directory:

  TabViews			-	text editor with three panes: text editor, hex viewer, 
						list view with statistics on number of unique symbols 
						found in text.
  Dynamic TabViews	-	MDI application that shows how to add/remove pages
						dynamically

#include "OXTabView.h"


*/

#ifndef _TABVIEW_H
#define _TABVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


#ifndef __AFXCMN_H__
#include <afxcmn.h>
#define __AFXCMN_H__
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#define __AFXEXT_H__
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#define __AFXPRIV_H__
#endif

#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif

#ifndef __AFXCVIEW_H__
#include <afxcview.h>		// MFC Common controls
#define __AFXCVIEW_H__
#endif

#ifndef __AFXRICH_H__
#include <afxrich.h>		
#define __AFXRICH_H__
#endif




typedef struct _tagPAGEINFO
{
	CWnd* pWnd;
	CString sTitle;
	BOOL bHasScrollHorz;
	BOOL bHasScrollVert;
	SCROLLINFO scrlInfoHorz;
	SCROLLINFO scrlInfoVert;

	// constructor
	_tagPAGEINFO()
	{
		pWnd=NULL;
		sTitle=_T("");
		bHasScrollHorz=FALSE;
		bHasScrollVert=FALSE;
	}

	// constructor
	_tagPAGEINFO(const _tagPAGEINFO& pi)
	{
		ASSERT(pi.pWnd!=NULL);
		pWnd=pi.pWnd;
		sTitle=pi.sTitle;
		bHasScrollHorz=pi.bHasScrollHorz;
		bHasScrollVert=pi.bHasScrollVert;
		scrlInfoHorz=pi.scrlInfoHorz;
		scrlInfoVert=pi.scrlInfoVert;
	}

	// copy constructor
	_tagPAGEINFO& operator=(const _tagPAGEINFO& pi)
	{
		ASSERT(pi.pWnd!=NULL);
		pWnd=pi.pWnd;
		sTitle=pi.sTitle;
		bHasScrollHorz=pi.bHasScrollHorz;
		bHasScrollVert=pi.bHasScrollVert;
		scrlInfoHorz=pi.scrlInfoHorz;
		scrlInfoVert=pi.scrlInfoVert;
		return *this;
	}

} PAGEINFO;

#define ID_TABVIEWCONTAINER_SIGN	0x3a7b4567

#define ID_SPLITTERWIDTH			6

#define ID_TABBTNOVERLAPSIZE		5
#define ID_TABBTNGAPSIZE			3

#define ID_MINSCROLLBARWIDTH		60
#define ID_INITABBTNAREAWIDTH		200
#define ID_SCROLLTABBTNAREASTEP		15

#define IDT_SCROLLPAGE_TIMER		122
#define ID_SCROLLPAGE_DELAY			200

#define IDT_CHECKSCROLLINFO_TIMER	123
#define ID_CHECKSCROLLINFO_PERIOD	1000


//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXTabViewContainer window

class OX_CLASS_DECL COXTabViewContainer : public CWnd
{
	DECLARE_DYNCREATE(COXTabViewContainer)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXTabViewContainer();

// Attributes
public:

	typedef enum _tagHITTEST
	{
		SCROLLBARHORZ=-1,
		SCROLLBARVERT=-2,
		SCRLSTARTBTN=-3,
		SCRLBACKWARDBTN=-4,
		SCRLFORWARDBTN=-5,
		SCRLENDBTN=-6,
		SPLITTER=-7,
		PAGE=-8,
		TABBTNAREA=-9,
		SIZEBAR=-10,
		NONE=-11
	} HITTEST;


protected:

	// rectangle for TabView Container
	//
	
	// scroll buttons rectangles
	CRect m_rectScrollToStartBtn;
	CRect m_rectScrollBackwardBtn;
	CRect m_rectScrollForwardBtn;
	CRect m_rectScrollToEndBtn;

	// tab button area origin
	int m_nTabBtnAreaOrigin;
	// the rectangle of the area that is covered by tab buttons
	CRect m_rectTabBtnArea;
	// tab buttons rectangles (coordinates relative to the top/left
	// of m_rectTabBtnArea + m_nTabBtnAreaOrigin)
	CArray<CRect,CRect&> m_arrTabBtnRects;
	// last saved width of tab buttons area
	int m_nLastTabBtnAreaWidth;

	// scroll bars rectangles
	CRect m_rectScrollBarHorz;
	CRect m_rectScrollBarVert;

	// rect for splitter
	CRect m_rectSplitter;

	// rect for size bar
	CRect m_rectSizeBar;

	// rect for page
	CRect m_rectPage;
	//
	/////////////////////////////////////////

	/////////////////////////////////////////
	// array of pages
	CArray<PAGEINFO,PAGEINFO> m_arrPages;
	// index of currently active page
	int m_nActivePageIndex;
	/////////////////////////////////////////

	// scroll style of the container (internal)
	DWORD m_dwScrollStyle;

	// internal array of unique IDs
	CArray<DWORD,DWORD> m_arrUniqueIDs;

	// scroll bars controls
	CScrollBar m_scrlBarHorz;
	CScrollBar m_scrlBarVert;

	// the ID of last scroll button that was pressed
	HITTEST m_nPressedScrlBtn;
	// flag that specifies if last pressed scroll button is still pressed 
	BOOL m_bIsScrlBtnPressed;
	// timer for tab buttons scrolling operations
	int m_nScrollPageTimer;

	// flag that specifies that splitter has been pressed
	BOOL m_bIsSplitterPressed;

	// fonts to draw text in tab buttons
	CFont m_fontTabBtnText;
	CFont m_fontActiveTabBtnText;

// Operations

public:
	// --- In  :	lpszClassName	-	ignored
	//				lpszWindowName	-	ignored
	//				dwStyle			-	The TabView container's style.
	//				rect			-	window rectangle
	//				pParentWnd		-	Pointer to the window that is the 
	//									TabView container's parent.
	//				nID				-	The TabView container's ID.
	//				pContext		-	ignored
	// --- Out : 
	// --- Returns:	TRUE if TabView container was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the TabView container. Implemented in order to 
	//				support dynamic creation of the object.
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{
		UNREFERENCED_PARAMETER(lpszClassName);
		UNREFERENCED_PARAMETER(lpszWindowName);
		UNREFERENCED_PARAMETER(pContext);
		return Create(pParentWnd,rect,dwStyle,nID);
	}

	// --- In  :	pParentWnd	-	Pointer to the window that is the 
	//								TabView container's parent.
	//				rect		-	window rectangle
	//				dwStyle		-	The TabView container's style.
	//				nID			-	The TabView container's ID.
	// --- Out : 
	// --- Returns:	TRUE if TabView container was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the TabView container
	virtual BOOL Create(CWnd* pParentWnd, CRect rect=CRect(0,0,0,0), 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, UINT nID=AFX_IDW_PANE_FIRST);


	// --- In  :	pClass		-	pointer to runtime class information of
	//								the new window to be added as new page
	//				pContext	-	pointer to context info (refer to the
	//								description of CCreateContext class)
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: TRUE if new page was successfully added, or FALSE otherwise
	// --- Effect : Adds new page to TabView container. Use this version of
	//				function if you have to add CView derived class which is
	//				part of document/view architecture of your application
	BOOL AddPage(CRuntimeClass* pClass, CCreateContext* pContext, 
		LPCTSTR lpszTitle=NULL) {
		return InsertPage(GetPageCount(),pClass,pContext,lpszTitle);
	}

	// --- In  :	pWnd		-	pointer to created window to be added 
	//								as new page
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: TRUE if new page was successfully added, or FALSE otherwise
	// --- Effect : Adds new page to TabView container. Use this version of
	//				function if you have to add any generic window into the 
	//				TabView container.
	BOOL AddPage(CWnd* pWnd, LPCTSTR lpszTitle=NULL) { 
		return InsertPage(GetPageCount(),pWnd,lpszTitle); 
	}

	// --- In  :	nIndex		-	zero-based index of the new page
	//				pClass		-	pointer to runtime class information of
	//								the new window to be added as new page
	//				pContext	-	pointer to context info (refer to the
	//								description of CCreateContext class)
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: TRUE if new page was successfully inserted, or FALSE otherwise
	// --- Effect : Inserts new page to TabView container. Use this version of
	//				function if you have to insert CView derived class which is
	//				part of document/view architecture of your application
	virtual BOOL InsertPage(const int nIndex, CRuntimeClass* pClass, 
		CCreateContext* pContext, LPCTSTR lpszTitle=NULL);

	// --- In  :	nIndex		-	zero-based index of the new page
	//				pWnd		-	pointer to created window to be inserted 
	//								as new page
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: TRUE if new page was successfully inserted, or FALSE otherwise
	// --- Effect : Inserts new page to TabView container. Use this version of
	//				function if you have to add any generic window into the 
	//				TabView container.
	virtual BOOL InsertPage(const int nIndex, CWnd* pWnd, LPCTSTR lpszTitle=NULL);


	// --- In  :	pWnd		-	pointer to the page to be deleted
	//				bDestroy	-	flag that specifies if window has to be 
	//								destroyed
	// --- Out : 
	// --- Returns: TRUE if the specified page was successfully deleted, 
	//				or FALSE otherwise
	// --- Effect : Deletes existing page from TabView container.
	virtual BOOL DeletePage(const CWnd* pWnd, const BOOL bDestroy=TRUE) {
		ASSERT(pWnd!=NULL);
		ASSERT(IsPage(pWnd));
		int nIndex=-1;
		if(FindPage(pWnd,nIndex))
			return DeletePage(nIndex,bDestroy);
		return FALSE;
	}
	
	// --- In  :	nIndex		-	zero-based index of the page to be deleted
	//				bDestroy	-	flag that specifies if window has to be 
	//								destroyed
	// --- Out : 
	// --- Returns: TRUE if the specified page was successfully deleted, 
	//				or FALSE otherwise
	// --- Effect : Deletes existing page from TabView container.
	virtual BOOL DeletePage(const int nIndex, const BOOL bDestroy=TRUE);


	// --- In  :	nIndex		-	zero-based index of the page to be retrieved
	// --- Out : 
	// --- Returns: pointer to the corresponding page, or NULL if out of range
	//				index was specified
	// --- Effect : Retrieves pointer to the page with specified index
	inline CWnd* GetPage(const int nIndex) const {
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
			return m_arrPages[nIndex].pWnd;
		return NULL;
	}


	// --- In  :	pWnd		-	pointer to the page which title
	//								is to be retrieved
	// --- Out : 
	// --- Returns: title of the corresponding page
	// --- Effect : Retrieves title of the specified page
	inline CString GetPageTitle(const CWnd* pWnd) const {
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
			return _T("");
		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
			return _T("");
		return GetPageTitle(nIndex);
	}

	// --- In  :	nIndex		-	zero-based index of the page which title
	//								is to be retrieved
	// --- Out : 
	// --- Returns: title of the corresponding page
	// --- Effect : Retrieves title of the page with specified index
	inline CString GetPageTitle(const int nIndex) const {
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
			return m_arrPages[nIndex].sTitle;
		return _T("");
	}


	// --- In  :	pWnd		-	pointer to the page which title
	//								is to be set
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the title of the specified page
	inline BOOL SetPageTitle(const CWnd* pWnd, LPCTSTR lpszTitle) {
		ASSERT(pWnd!=NULL);
		if(pWnd==NULL)
			return FALSE;
		int nIndex=-1;
		if(!FindPage(pWnd,nIndex) || nIndex==-1)
			return FALSE;
		return SetPageTitle(nIndex,lpszTitle);
	}

	// --- In  :	nIndex		-	zero-based index of the page which title
	//								is to be set
	//				lpszTitle	-	text that will be used as page title in
	//								tab button
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets title of the page with specified index
	inline BOOL SetPageTitle(const int nIndex, LPCTSTR lpszTitle) {
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(nIndex>=0 && nIndex<GetPageCount())
		{
			PAGEINFO& pi=m_arrPages[nIndex];
			if(pi.sTitle.Compare(lpszTitle)!=0)
			{
				pi.sTitle=lpszTitle;
				CalcTabBtnRects();
				CRect rect=m_arrTabBtnRects[nIndex];
				if(rect.left<m_rectTabBtnArea.right &&
					rect.right>m_rectTabBtnArea.right)
				{
					RedrawTabBtnArea();
				}
			}
			return TRUE;
		}
		return FALSE;
	}


	// --- In  :	
	// --- Out : 
	// --- Returns: number of pages in the Tab View container
	// --- Effect : Retrieves the number of pages in the Tab View container
	inline int GetPageCount() const { return m_arrPages.GetSize(); }


	// --- In  :	pTestWnd	-	pointer to the window to be tested
	//								as Tab View container's page
	//				hTestWnd	-	handle of the window to be tested
	//								as Tab View container's page
	// --- Out :	nIndex		-	reference to the variable where zero-based 
	//								index of the found page will be saved
	// --- Returns: TRUE if specified window is Tab View container's page,
	//				or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is Tab View container's page and if it is TRUE then
	//				save the index of found page into nIndex
	inline BOOL FindPage(const CWnd* pTestWnd, int& nIndex) const {
		ASSERT(pTestWnd!=NULL);
		if(pTestWnd!=NULL)
			return FindPage(pTestWnd->m_hWnd,nIndex);
		return FALSE;
	}
	inline BOOL FindPage(const HWND hTestWnd, int& nIndex) const {
		BOOL bResult=FALSE;
		for(nIndex=0; nIndex<GetPageCount(); nIndex++)
		{
			if(m_arrPages[nIndex].pWnd->m_hWnd==hTestWnd)
			{
				bResult=TRUE;
				break;
			}
		}
		return bResult;
	}


	// --- In  :	pTestWnd	-	pointer to the window to be tested
	//								as Tab View container's page
	//				hTestWnd	-	handle of the window to be tested
	//								as Tab View container's page
	// --- Out :	
	// --- Returns: TRUE if specified window is Tab View container's page,
	//				or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is Tab View container's page
	inline BOOL IsPage(const HWND hTestWnd) const {
		int nIndex=-1;
		return FindPage(hTestWnd,nIndex);
	}
	inline BOOL IsPage(const CWnd* pTestWnd) const {
		int nIndex=-1;
		return FindPage(pTestWnd,nIndex);
	}


	// --- In  :	pTestWnd	-	pointer to the window to be tested as 
	//								currently active Tab View container's page
	//				hTestWnd	-	handle of the window to be tested as 
	//								currently active Tab View container's page
	// --- Out :	
	// --- Returns: TRUE if specified window is currently active Tab View 
	//				container's page, or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the specified
	//				window is currently active Tab View container's page
	inline BOOL IsActivePage(const HWND hTestWnd) const {
		int nIndex=-1;
		if(FindPage(hTestWnd,nIndex) && GetActivePageIndex()==nIndex)
			return TRUE;
		return FALSE;
	}
	inline BOOL IsActivePage(const CWnd* pTestWnd) const {
		int nIndex=-1;
		if(FindPage(pTestWnd,nIndex) && GetActivePageIndex()==nIndex)
			return TRUE;
		return FALSE;
	}


	// --- In  :	
	// --- Out :	
	// --- Returns: index of currently active Tab View container's page
	// --- Effect : Retrieves the index of currently active Tab View 
	//				container's page
	inline int GetActivePageIndex() const { return m_nActivePageIndex; }

	// --- In  :	
	// --- Out :	
	// --- Returns: pointer to currently active Tab View container's page
	// --- Effect : Retrieves the pointer to currently active Tab View 
	//				container's page
	inline CWnd* GetActivePage() const { 
		if(m_nActivePageIndex>=0 && m_nActivePageIndex<GetPageCount())
			return m_arrPages[m_nActivePageIndex].pWnd;
		return NULL;
	}


	// --- In  :	pWnd		-	pointer to the page to be set as active
	// --- Out : 
	// --- Returns: TRUE is specified page was successfully set as active
	// --- Effect : Sets the specified page as active one
	inline BOOL SetActivePage(const CWnd* pWnd) {
		ASSERT(pWnd!=NULL);
		if(pWnd!=NULL)
		{
			int nIndex=-1;
			if(FindPage(pWnd,nIndex))
				return SetActivePageIndex(nIndex);
		}
		return FALSE;
	}

	// --- In  :	nIndex		-	index of the page to be set as active
	// --- Out : 
	// --- Returns: TRUE is page with specified index was successfully set 
	//				as active
	// --- Effect : Sets the page with specified index as active one
	virtual BOOL SetActivePageIndex(const int nIndex);


	// --- In  :	
	// --- Out : 
	// --- Returns: pointer to the horizontal scroll bar control
	// --- Effect : Retrieves pointer to the horizontal scroll bar control
	inline CScrollBar* GetHorzScrollBar() { 
		if(::IsWindow(m_scrlBarHorz.GetSafeHwnd()))
			return &m_scrlBarHorz; 
		else
			return NULL; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: pointer to the vertical scroll bar control
	// --- Effect : Retrieves pointer to the vertical scroll bar control
	inline CScrollBar* GetVertScrollBar() { 
		if(::IsWindow(m_scrlBarVert.GetSafeHwnd()))
			return &m_scrlBarVert; 
		else
			return NULL; 
	}


	// --- In  :	lpszProfileName	-	name of hive in registry where
	//									all information about COXTabViewContainer
	//									will be saved. 
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Saves COXTabViewContainer state into registry
	BOOL SaveState(LPCTSTR lpszProfileName) ;

	// --- In  :	lpszProfileName	-	name of hive in registry where
	//									all information about COXTabViewContainer
	//									was saved. 
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Loads COXTabViewContainer state from registry
	BOOL LoadState(LPCTSTR lpszProfileName);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

protected:
	// sets the scroll style of the Tab View container. 
	// dwScrollStyle can be zero or any combination of WS_VSCROLL
	// and WS_HSCROLL styles
	virtual void SetScrollStyle(const DWORD dwScrollStyle, const BOOL bForceToRebuild=FALSE);
	// retrieves the scroll style of the Tab View container. 
	// It can be zero or any combination of WS_VSCROLL and WS_HSCROLL styles
	inline DWORD GetScrollStyle() const { return m_dwScrollStyle; }

	// initialize internal PAGEINFO structure with scroll info for currently
	// active page
	void IniScrollInfo();

	// calculates position of all Tab View container elements
	virtual void CalcLayout();
	// calculates position of all tab buttons
	virtual void CalcTabBtnRects();
	// resize scroll bar controls
	void UpdateScrollSizes();

	// retrieves unique ID for newly added page
	virtual DWORD GetUniqueId() { 
		int nCount=m_arrUniqueIDs.GetSize();
		ASSERT(nCount>0);
		DWORD dwUniqueID=m_arrUniqueIDs[nCount-1]; 
		if(nCount==1)
			m_arrUniqueIDs.SetAt(nCount-1,dwUniqueID+1);
		return dwUniqueID; 
	}

	// empties all internal position info of all Tab View container elements 
	void EmptyRects();

	// redraw the specified scroll button.
	void RedrawScrollButton(HITTEST hitTest);
	// redraw tab buttons area
	inline void RedrawTabBtnArea() { 
		ASSERT(::IsWindow(GetSafeHwnd()));
		RedrawWindow(m_rectTabBtnArea); 
	}
	// redraw splitter
	inline void RedrawSplitter() { 
		ASSERT(::IsWindow(GetSafeHwnd()));
		RedrawWindow(m_rectSplitter); 
	}
	// redraw all Tab View container elements 
	inline void RedrawContainer() {
		CRect rect=m_rectScrollToStartBtn;
		rect.right=m_rectSplitter.right;
		RedrawWindow(rect);
		RedrawWindow(m_rectSizeBar);
	}

	// the following virtual routines are responsible for drawing
	// corresponding Tab View container elements 
	virtual void DrawScrollButtons(CDC* pDC);
	virtual void DrawTabBtnArea(CDC* pDC);
	virtual void DrawSplitter(CDC* pDC);
	virtual void DrawSizeBar(CDC* pDC);
	/////////////////////////////////////////////////////////////

	// draws specified scroll button
	virtual void DrawButton(CDC* pDC, CRect rect, const HITTEST nButtonType) const;
	// draws tab button for the page with specified index
	virtual void DrawTabButton(CDC* pDC, const int nIndex) const;

	// starts handling "move splitter" or "scroll tab buttons" operation
	void StartTracking(const CPoint& point);
	// stops "move splitter" or "scroll tab buttons" operation
	void StopTracking(const CPoint& point);

	// the following test functions return the flag that specifies
	// if corresponding scroll tab button operation can be accomplished
	//
	inline BOOL CanScrollToStart() const { 
		ASSERT(m_nTabBtnAreaOrigin<=0);
		return (m_nTabBtnAreaOrigin<0 && 
			m_rectTabBtnArea.right>m_rectTabBtnArea.left); 
	}
	inline BOOL CanScrollBackward() const { return CanScrollToStart(); }
	inline BOOL CanScrollForward() const { return CanScrollToEnd(); }
	inline BOOL CanScrollToEnd() const {
		ASSERT(m_nTabBtnAreaOrigin<=0);
		if(GetPageCount()>0 && 
			m_rectTabBtnArea.right>m_rectTabBtnArea.left)
		{
			ASSERT(GetPageCount()==m_arrTabBtnRects.GetSize());
			CRect rect=m_arrTabBtnRects[GetPageCount()-1];
			rect+=m_rectTabBtnArea.TopLeft();
			return (rect.right+m_nTabBtnAreaOrigin>m_rectTabBtnArea.right);
		}
		return FALSE;
	}

	inline void EnsureTabBtnVisible(const int nIndex, 
		const BOOL bPartialOk=FALSE)
	{
		ASSERT(nIndex>=0 && nIndex<GetPageCount());
		if(m_rectTabBtnArea.Width()>0 && nIndex>=0 && nIndex<GetPageCount())
		{
			CRect rect=m_arrTabBtnRects[nIndex];
			rect+=m_rectTabBtnArea.TopLeft();
			rect.OffsetRect(m_nTabBtnAreaOrigin,0);
			if(rect.left>m_rectTabBtnArea.right || 
				rect.right<m_rectTabBtnArea.left ||
				(rect.left<m_rectTabBtnArea.left && 
				rect.right<m_rectTabBtnArea.right && !bPartialOk) ||
				(rect.left>m_rectTabBtnArea.left && 
				rect.right>m_rectTabBtnArea.right && !bPartialOk))
			{
				if(rect.left<m_rectTabBtnArea.left)
				{
					m_nTabBtnAreaOrigin+=m_rectTabBtnArea.left-rect.left;
					if(nIndex>0)
						m_nTabBtnAreaOrigin+=m_rectTabBtnArea.Width()/2<20 ?
							m_rectTabBtnArea.Width()/2 : 20;
				}
				else
				{
					m_nTabBtnAreaOrigin+=m_rectTabBtnArea.right-rect.right;
					if(nIndex<GetPageCount()-1)
						m_nTabBtnAreaOrigin-=m_rectTabBtnArea.Width()/2<20 ?
							m_rectTabBtnArea.Width()/2 : 20;
				}
				RedrawTabBtnArea();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////

// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXTabViewContainer();


	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Updates scroll info for currently active page
	void UpdateScrollInfo();

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Updates scroll state for currently active page
	void UpdateScrollState();


	// --- In  :	point	-	point in Tab View container client coordinates
	//							to be tested
	// --- Out : 
	// --- Returns:	if equals or more than zero then the return value specifies 
	//				the corresponding index of tab button, otherwise it can be
	//				one of the following value:
	//
	//				SCROLLBARHORZ	-	horizontal scroll bar control
	//				SCROLLBARVERT	-	vertical scroll bar control
	//				SCRLSTARTBTN	-	scroll button "scroll to start"
	//				SCRLBACKWARDBTN	-	scroll button "scroll backward"
	//				SCRLFORWARDBTN	-	scroll button "scroll forward"
	//				SCRLENDBTN		-	scroll button "scroll to end"
	//				SPLITTER		-	splitter
	//				PAGE			-	currently active page
	//				TABBTNAREA		-	tab buttons area
	//				SIZEBAR			-	sizebar
	//				NONE			-	out of Tab View container
	//
	// --- Effect : Retrieves the element of Tab View container over which
	//				the specified point is located
	int HitTest(const CPoint point) const;


	// --- In  :	nScrlBtn	-	scroll button identifier. it can be one 
	//								of the following:
	//
	//				SCRLSTARTBTN	-	scroll button "scroll to start"
	//				SCRLBACKWARDBTN	-	scroll button "scroll backward"
	//				SCRLFORWARDBTN	-	scroll button "scroll forward"
	//				SCRLENDBTN		-	scroll button "scroll to end"
	//
	// --- Out : 
	// --- Returns:	
	// --- Effect : Scroll tab btn area in the specified direction
	void ScrollPage(const HITTEST nScrlBtn);

protected:

	// special command routing to frame
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// Generated message map functions
protected:
	// overwrite standard handlers to overcome some problems with MFC
	// standard painting routine
	//{{AFX_MSG(COXTabViewContainer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCancelMode();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



///////////////////////////////////////////////////////////////////////////


// --- In  :	pWnd		-	pointer to valid window
//				bOnlyActive	-	if it is TRUE then the function will return
//								non-NULL value only if specified window 
//								is current active page of parent TabView 
//								container
// --- Out : 
// --- Returns:	pointer to parent TabView container, or NULL if parent of
//				the specified window is not TabView container
// --- Effect : Retrieves parent TabView container foor specified window
OX_API_DECL COXTabViewContainer* PASCAL 
GetParentTabViewContainer(const CWnd* pWnd, const BOOL bOnlyActive=TRUE);


///////////////////////////////////////////////////////////////////////////


template<class PARENTWND>
class OX_CLASS_DECL COXTabViewPage : public PARENTWND
{
public:
	// Contruction
	COXTabViewPage();

// Scrolling Functions
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
			// return sibling scrollbar control (or NULL if none)

protected:
	// timer for checking scroll info of currently active page
	int m_nCheckScrollInfoTimer;
	BOOL m_bCurrentlyScrolling;
	BOOL m_bHasInternalScrollBars;
	BOOL m_bNeedsInternalRedrawing;

	int m_nLastHorzPos;
	int m_nLastVertPos;

protected:
	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Handle all messages that go to the window
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	
};


template<class PARENTWND>
COXTabViewPage<PARENTWND>::COXTabViewPage()
{
	m_nCheckScrollInfoTimer=-1;
	m_bCurrentlyScrolling=FALSE;
	m_bHasInternalScrollBars=PARENTWND::IsKindOf(RUNTIME_CLASS(CEdit)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CEditView)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CListBox)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CTreeCtrl)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CTreeView)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CListView)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CRichEditCtrl)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CRichEditView));
	m_bNeedsInternalRedrawing=PARENTWND::IsKindOf(RUNTIME_CLASS(CEdit)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CEditView)) ||
		PARENTWND::IsKindOf(RUNTIME_CLASS(CListBox));

	m_nLastHorzPos=0;
	m_nLastVertPos=0;
}


template<class PARENTWND>
CScrollBar* COXTabViewPage<PARENTWND>::GetScrollBarCtrl(int nBar) const
{
	COXTabViewContainer* pContainer=GetParentTabViewContainer(this);
	if(pContainer!=NULL)
	{
		ASSERT(nBar==SB_HORZ || nBar==SB_VERT);
		CScrollBar* pScrlBar=NULL;
		if(nBar==SB_HORZ)
		{
			if((GetStyle()&WS_HSCROLL)!=WS_HSCROLL)
				pScrlBar=pContainer->GetHorzScrollBar();
		}
		else if(nBar==SB_VERT)
		{
			if((GetStyle()&WS_VSCROLL)!=WS_VSCROLL)
				pScrlBar=pContainer->GetVertScrollBar();
		}

		if(pScrlBar!=NULL)
		{
			return pScrlBar;
		}
	}

	return PARENTWND::GetScrollBarCtrl(nBar);
}

template<class PARENTWND>
LRESULT COXTabViewPage<PARENTWND>::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#ifdef _AFXDLL
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#endif

	ASSERT_VALID(this);

	static BOOL bIgnoreResize=FALSE;

	switch(msg) 
	{
	case WM_CREATE:
		{
			LRESULT lResult=PARENTWND::WindowProc(msg,wp,lp);
			if(lResult==-1)
				return -1;

			if(m_bHasInternalScrollBars)
			{
				m_nCheckScrollInfoTimer=SetTimer(IDT_CHECKSCROLLINFO_TIMER,
					ID_CHECKSCROLLINFO_PERIOD,NULL);
				if(m_nCheckScrollInfoTimer==-1)
					return -1;
			}

			return 0;
		}

	case WM_DESTROY:
		{
			if(m_nCheckScrollInfoTimer!=-1)
			{
				KillTimer(m_nCheckScrollInfoTimer);
				m_nCheckScrollInfoTimer=-1;
			}
			break;
		}

	case WM_TIMER:
		{
			if((int)wp==m_nCheckScrollInfoTimer)
			{
				if(m_bCurrentlyScrolling)
					return 0;

				COXTabViewContainer* pContainer=GetParentTabViewContainer(this);
				if(pContainer!=NULL)
				{
					ASSERT(m_bHasInternalScrollBars);

					if(m_bNeedsInternalRedrawing)
					{
						pContainer->UpdateScrollState();
						pContainer->UpdateScrollInfo();
					}
					else
					{
						bIgnoreResize=TRUE;
						pContainer->UpdateScrollState();
						pContainer->UpdateScrollInfo();
						bIgnoreResize=FALSE;
					}
				}
				return 0;
			}
			break;
		}

	case WM_HSCROLL:
	case WM_VSCROLL:
		{
			if(LOWORD(wp)==SB_ENDSCROLL)
			{
				m_bCurrentlyScrolling=FALSE;
				m_nLastHorzPos=GetScrollPos(SB_HORZ);
				m_nLastVertPos=GetScrollPos(SB_VERT);
			}
			else
				m_bCurrentlyScrolling=TRUE;

			COXTabViewContainer* pContainer=GetParentTabViewContainer(this);
			if(pContainer!=NULL && m_bHasInternalScrollBars)
			{
				if(m_bNeedsInternalRedrawing)
				{
					PARENTWND::WindowProc(msg,wp,lp);
					pContainer->UpdateScrollInfo();
				}
				else 
				{
					if(LOWORD(wp)==SB_THUMBTRACK && 
						(PARENTWND::IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
						PARENTWND::IsKindOf(RUNTIME_CLASS(CListView))))
					{
						int nPos=HIWORD(wp);
						if(msg==WM_HSCROLL && nPos!=m_nLastHorzPos)
						{
							SendMessage(LVM_SCROLL,
								(nPos-m_nLastHorzPos),0);
							m_nLastHorzPos=GetScrollPos(SB_HORZ);
						}
						else if(msg==WM_VSCROLL && nPos!=m_nLastVertPos)
						{
							if(nPos>m_nLastVertPos)
							{
								for(int nIndex=0; nIndex<nPos-m_nLastVertPos;
									nIndex++)
								{
									SendMessage(msg,
										MAKEWPARAM(SB_LINEDOWN,0),lp);
								}
							}
							else
							{
								for(int nIndex=0; nIndex<m_nLastVertPos-nPos;
									nIndex++)
								{
									SendMessage(msg,
										MAKEWPARAM(SB_LINEUP,0),lp);
								}
							}
							m_nLastVertPos=GetScrollPos(SB_VERT);
						}
					}
					else
					{
						PARENTWND::WindowProc(msg,wp,lp);
						bIgnoreResize=TRUE;
						pContainer->UpdateScrollInfo();
						bIgnoreResize=FALSE;
					}
				}
			}
			else
			{
				if(pContainer!=NULL && PARENTWND::IsKindOf(RUNTIME_CLASS(CScrollView)))
				{
					((CScrollView*)this)->OnScroll((msg==WM_HSCROLL ? 
						MAKEWORD(LOWORD(wp),-1) : MAKEWORD(-1,LOWORD(wp))),HIWORD(wp));
				}
				else
				{
					PARENTWND::WindowProc(msg,wp,lp);
				}
			}
			return 0;
		}

	case WM_SIZE:
		{
			COXTabViewContainer* pContainer=GetParentTabViewContainer(this);
			if(pContainer!=NULL)
			{
				if(bIgnoreResize)
					return 0;

				if((GetStyle()&WS_VISIBLE)!=WS_VISIBLE)
					PARENTWND::WindowProc(msg,wp,lp);
				else if(PARENTWND::IsKindOf(RUNTIME_CLASS(CRichEditView)) ||
					PARENTWND::IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
				{
					if(!m_bCurrentlyScrolling)
					{
						bIgnoreResize=TRUE;
						pContainer->UpdateScrollState();
						pContainer->UpdateScrollInfo();
						ModifyStyle(WS_VISIBLE,NULL);
						PARENTWND::WindowProc(msg,wp,lp);
						ShowScrollBar(SB_HORZ,FALSE);
						ShowScrollBar(SB_VERT,FALSE);
						ModifyStyle(NULL,WS_VISIBLE);
						Invalidate();
						bIgnoreResize=FALSE;
					}
				}
				else if(m_bNeedsInternalRedrawing)
				{
					int nTopIndex=CB_ERR;
					if(PARENTWND::IsKindOf(RUNTIME_CLASS(CListBox)))
						nTopIndex=((CListBox*)this)->GetTopIndex();
				
					pContainer->UpdateScrollState();
					pContainer->UpdateScrollInfo();
					PARENTWND::WindowProc(msg,wp,lp);
					
					if(PARENTWND::IsKindOf(RUNTIME_CLASS(CListBox)))
					{
						if(((CListBox*)this)->GetTopIndex()!=nTopIndex)
							RedrawWindow();
					}
				}
				else if(!m_bHasInternalScrollBars)
				{
					ModifyStyle(WS_VISIBLE,NULL);
					pContainer->GetHorzScrollBar()->
						ModifyStyle(WS_VISIBLE,NULL);
					pContainer->GetVertScrollBar()->
						ModifyStyle(WS_VISIBLE,NULL);
					PARENTWND::WindowProc(msg,wp,lp);
					ShowScrollBar(SB_HORZ,FALSE);
					ShowScrollBar(SB_VERT,FALSE);
					pContainer->GetHorzScrollBar()->
						ModifyStyle(NULL,WS_VISIBLE);
					pContainer->GetVertScrollBar()->
						ModifyStyle(NULL,WS_VISIBLE);
					ModifyStyle(NULL,WS_VISIBLE);
				}
				else
				{
					CSize sizeScrollPos(-1,-1);
					if(PARENTWND::IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
					{
						sizeScrollPos.cx=pContainer->GetHorzScrollBar()->
							GetScrollPos();
						sizeScrollPos.cy=pContainer->GetVertScrollBar()->
							GetScrollPos();
					}
				
					bIgnoreResize=TRUE;
					pContainer->UpdateScrollState();
					pContainer->UpdateScrollInfo();
					ModifyStyle(WS_VISIBLE,NULL);
					PARENTWND::WindowProc(msg,wp,lp);
					ShowScrollBar(SB_HORZ,FALSE);
					bIgnoreResize=FALSE;
					ShowScrollBar(SB_VERT,FALSE);
					ModifyStyle(NULL,WS_VISIBLE);

					if(PARENTWND::IsKindOf(RUNTIME_CLASS(CTreeCtrl)))
					{
						if(sizeScrollPos.cx!=pContainer->GetHorzScrollBar()->
							GetScrollPos() || 
							sizeScrollPos.cy!=pContainer->GetVertScrollBar()->
							GetScrollPos())
						{
							RedrawWindow();
						}
					}
				}

				return 0;
			}
			break;
		}
	default:
		{
			if(m_bHasInternalScrollBars)
			{
				if(msg==WM_SYSCOMMAND || msg==WM_SHOWWINDOW ||
					(msg>=WM_KEYFIRST && msg<=WM_KEYLAST) || 
					(msg>=WM_SYSKEYFIRST && msg<=WM_SYSKEYLAST) || 
					(msg>=WM_MOUSEFIRST && msg<=WM_MOUSELAST && msg!=WM_MOUSEMOVE))
				{
					PostMessage(WM_TIMER,m_nCheckScrollInfoTimer);
				}
			}
			break;
		}
	}

	// I don't handle it: pass along
	return PARENTWND::WindowProc(msg,wp,lp);
}

#endif // _TABVIEW_H

///////////////////////////////////////////////////////////////////////////
