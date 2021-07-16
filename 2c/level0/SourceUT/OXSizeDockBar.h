// ===================================================================================
// 					Class Specification : COXSizeDockBar
// ===================================================================================

// Header file : OXSizeDockBar.h

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CDockBar

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
//  This class replaces the standard MFC (UNDOCUMENTED) CDockBar class to handle
//	sizeable controlbars and provide the neccessary objects like an special CRectTracker
//  class, COXRectTracker, and a special CSplitterRect class, COXSplitterRect.
// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SIZEDOCKBAR_H__
#define __SIZEDOCKBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxpriv.h>


#define CX_SPLIT        8               // dimensions of splitter bars
#define CY_SPLIT        8
#define CX_BORDER       1
#define CY_BORDER       1

typedef struct _ROWSIZEINFO
{
    int nFlexWidth;                 // space taken up by flexible bars
    int nFixedWidth;                // space taken up by fixed size bars
    int nMaxHeight;                 // max height taken  (both flexible and fixed height bars)
    int nMaxFixedHeight;			// max height taken by a fixed size bar.
    int nFlexBars;                  // number of sized bars
    int nTotalBars;                 // total no of bars in the row.
	int nTotalWidth;				// fixed + flex width
} ROWSIZEINFO;

class COXSplitterRect;
class COXSizeControlBar;

class OX_CLASS_DECL COXSizeDockBar : public CDockBar
{
friend class COXDragDockContext;

DECLARE_DYNAMIC(COXSizeDockBar)

// Data members -------------------------------------------------------------
public:

    CObArray			m_SplitArr;             // array of COXSplitterRect's constructed by ReCalcLayout

    COXSplitterRect*	m_pSplitCapture;        // capture splitter rect (if any)
    HCURSOR				m_hcurLast;             // last cursor type
	HCURSOR				m_hcurSizeNS;
	HCURSOR				m_hcurSizeWE;

    // following items used to detect when DockBar has changed since last time, so we
    // can re-arrange the rows if parent resizes, or bars are docked/floated/hidden
    CSize				m_LayoutSize;			// size used in previous layout
    int					m_CountBars;			// no of bars for previous layout - need to go better than this..
	CPtrArray			m_arrHiddenBars;		// array of currently invisible bars

protected:

private :

// Member functions ---------------------------------------------------------
public:
	COXSizeDockBar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state

    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : See CControlBar::CalcFixedLayout
	//				It specially handles layouts, build with diffrent rows, due to the
	//				sizeable splitterfunctionality of this dockbar.

    void TileDockedBars();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Adjusts the sizes of all the bars on a dockbar to fit a new size

	void AdjustForNewBar(CControlBar* pBar);		
	// --- In  : pBar : bar to adjsut to
	// --- Out : 
	// --- Returns : 
	// --- Effect : Adjust sizes for specified newly added bar.

	BOOL WasBarHidden(CControlBar* pBar);
	// --- In  : pBar : bar to test visibility of
	// --- Out : 
	// --- Returns : 
	// --- Effect : Returns TRUE if BAR if bar is hidden

// Dragging
	int TestInsertPosition(CControlBar* pBarIns, CRect rect);
	// --- In  : pBar : bar to insert
	//			 rect : the size and position of pBar
	// --- Out : 
	// --- Returns : position of insertion
	// --- Effect : Essentially the same as CDockBar::Insert(). Returns the position in the 
	//				bar array where the bar will be inserted.

	int BarsOnThisRow(CControlBar* pBarIns, CRect rect);
	// --- In  : pBarIns : bar to insert
	//			 rect : the size and position of pBarIns
	// --- Out : 
	// --- Returns : bars on this row
	// --- Effect : returns no of bars that will be in the row (excluding the one to be	inserted)        

// Miscellaneous
    COXSizeControlBar* GetFirstControlBar();
	// --- In  :
	// --- Out : 
	// --- Returns : The first real controlbar (not a place holder) encountered on this dockbar
	// --- Effect :

    BOOL IsBarHorizontal() const { 
		return (((m_dwStyle & CBRS_ALIGN_TOP) == CBRS_ALIGN_TOP) || 
			((m_dwStyle & CBRS_ALIGN_BOTTOM) == CBRS_ALIGN_BOTTOM)); 
	}
	// --- In  :
	// --- Out : 
	// --- Returns : whether the bar is horizontal or not
	// --- Effect :

	inline BOOL SetWindowSize(CWnd * pWnd, CSize size) {
		return pWnd->SetWindowPos(NULL, 0, 0, size.cx, size.cy, 
			SWP_NOMOVE /* | SWP_NOREDRAW */ | SWP_NOZORDER);
	}

	void ResizeBar(COXSizeControlBar* pBar, const BOOL bMaximize);

	static BOOL IsSizeable(CControlBar* pBar) { 
		ASSERT(pBar!=NULL);
		return pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar));
	}

#ifdef _DEBUG	
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual ~COXSizeDockBar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COXSizeDockBar)
    //}}AFX_VIRTUAL

protected:

	// Splitter rectangles...
    COXSplitterRect* GetSplitter(int i)
                { return ((COXSplitterRect*)(m_SplitArr[i])); };
    void AddSplitterRect(int type, int x1, int y1, int x2, int y2, int nPos);
    void DeleteSplitterRects();
    void SetSplitterSizeInRange(int start, int type, int length);
    COXSplitterRect* SetHitCursor(CPoint pt);
    COXSplitterRect* HitTest(CPoint pt);
    void StartTracking(CPoint pt);

        // Navigating rows
    int StartPosOfRow(int nPos);
    int StartPosOfPreviousRow(int nPos);

        // Resizing rows
    BOOL IsRowSizeable(int nPos);

    void GetRowSizeInfo(int nPos,  ROWSIZEINFO* pRZI, const CPtrArray & arrBars);
    BOOL AdjustAllRowSizes(int nNewSize);
    BOOL AdjustRowSizes(int nPos, int nNewSize, CPtrArray & arrBars);
    void TileDockedBarsRow(int nPos);

    int ShrinkRowToLeft(int nPos, int nAmount, BOOL bApply, int* pnFlex = NULL);
    int ShrinkRowToRight(int nPos, int nAmount, BOOL bApply, int* pnFlex = NULL);
    int CheckSumBars() const;

    //{{AFX_MSG(COXSizeDockBar)
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    afx_msg LRESULT OnSizeParent(WPARAM, LPARAM);
    DECLARE_MESSAGE_MAP()


private :

};

#endif  // __SIZEDOCKBAR_H__
