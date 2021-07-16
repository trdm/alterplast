// ===================================================================================
// 					Class Implementation : COXSizeDockBar
// ===================================================================================

// Header file : OXSizeDockBar.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxole.h>
#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif

#include "OXSplitterRect.h"  // used class specification
#include "OXRectTracker.h"   // used class specification
#include "OXSizeCtrlBar.h"   // used class specification
#include "OXSizeToolBar.h"   // used class specification

#include "OXSizeDockBar.h"  // this class specification

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXSizeDockBar, CDockBar)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

/////////////////////////////////////////////////////////////////////////////
// CDockBar layout

// helper which can acts on any array of windows
CControlBar* GetDockedControlBar(int nPos, const CPtrArray& arrBars)
	{
	CControlBar* pResult = (CControlBar*)arrBars[nPos];
	if (HIWORD(pResult) == 0)
		return NULL;
	if(pResult!=NULL && !::IsWindow(pResult->m_hWnd))
		return NULL;
	return pResult;
	}

COXSizeDockBar::COXSizeDockBar()
	: m_hcurSizeNS(NULL),
	m_hcurSizeWE(NULL)
	{
    m_pSplitCapture = NULL;
    m_hcurLast = NULL;
    m_LayoutSize.cx = 0xffff;       // some stupid values to force automatic resize
    m_LayoutSize.cy = 0xffff;
	m_CountBars = 0;
	}


COXSizeDockBar::~COXSizeDockBar()
	{
    DeleteSplitterRects();          // delete any outstanding splitter rects
	}


BEGIN_MESSAGE_MAP(COXSizeDockBar, CDockBar)
//{{AFX_MSG_MAP(COXSizeDockBar)
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
END_MESSAGE_MAP()


#ifdef _DEBUG
void DumpArrayBars(CDumpContext& dc, const CPtrArray& arrBars)
	{
	for (int nPos = 0; nPos < arrBars.GetSize(); nPos++)
	{
		LPVOID pVoid = arrBars[nPos];
		dc << _T("   [") << nPos << _T("]") << pVoid;
		CControlBar* pBar = ::GetDockedControlBar(nPos, arrBars);
		if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()))
			{
			CString strTitle;
			pBar->GetWindowText(strTitle);
			dc << _T(" ") << strTitle;
			if (!pBar->IsVisible())
				dc << _T(" hidden");
			}				
		dc << _T("\n");			
		}
	}


void COXSizeDockBar::Dump( CDumpContext& dc ) const
	{
	CDockBar::Dump(dc);
	DumpArrayBars(dc, m_arrBars);

	// now go through the splitter array.
	int nDepth = dc.GetDepth();
	dc.SetDepth(1);
	m_SplitArr.Dump(dc);		
	dc.SetDepth(nDepth);
	}	

#endif

/////////////////////////////////////////////////////////////////////////////
// COXSizeDockBar message handlers

// most of this code is copied from MFC CDockBar - with additional comments to help
// my understanding of it. The basic idea is that our DockBar is being asked how big
// it is. To find out, it looks at the bars inside it
CSize COXSizeDockBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	
	CSize sizeFixed = CControlBar::CalcFixedLayout(bStretch, bHorz);
	
    // ID for this bar...used to set MRU docked position
	UINT uDockBarID = ::GetWindowLong(m_hWnd, GWL_ID);

	// prepare for layout
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = m_bLayoutQuery ? NULL : 
		::BeginDeferWindowPos(m_arrBars.GetSize());
	
	CPoint pt(-afxData.cxBorder2, -afxData.cyBorder2);
	
	// get max size
	CSize sizeMax;
	if (!m_rectLayout.IsRectEmpty())
		sizeMax = m_rectLayout.Size();
	else
	{
		CRect rectFrame;
		CFrameWnd* pFrame = GetParentFrame();
		pFrame->GetClientRect(rectFrame);
		pFrame->ClientToScreen(rectFrame);

		for(int nID = AFX_IDW_CONTROLBAR_FIRST; nID<=AFX_IDW_CONTROLBAR_LAST; nID++)
		{
			CControlBar* pBar=m_pDockSite->GetControlBar(nID);

			if(pBar!=NULL && pBar->IsDockBar() && 
				pBar->IsVisible() && !pBar->IsFloating())
			{
				CRect rectBar;
				pBar->GetWindowRect(rectBar);
				if(pBar->GetStyle() & CBRS_TOP)
				{
					rectFrame.top=__max(rectFrame.top,rectBar.bottom);
				}
				else if(pBar->GetStyle() & CBRS_BOTTOM)
				{
					rectFrame.bottom=__min(rectFrame.bottom,rectBar.top);
				}
			}
		}

		sizeMax = rectFrame.Size();
	}

	// true if we should draw a bar for this row
    BOOL bDrawBarForRow = FALSE;        
	
    DeleteSplitterRects();          // clear the splitter rects
    int nWidth = 0;
    int nFirstSplitterInRow = 0;
    int nFirstPaneInRow = 0;
    BOOL bFirstPaneInRow = TRUE;
	
	int nCountSizeBars = 0;
	BOOL bLastWasResizable=FALSE;

	BOOL bWrapped = FALSE;

	// layout all the control bars
	int nLastVisSep = -1;
    for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		void* pVoid = m_arrBars[nPos];
		CControlBar* pBar = GetDockedControlBar(nPos);
        if (pVoid != NULL)
		{
            if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && pBar->IsVisible())
			{
				BOOL bSomeToolBar = (BOOL) (((CControlBar*)pBar)->
					IsKindOf(RUNTIME_CLASS(COXSizeToolBar)));
				
                if (bFirstPaneInRow)
				{
                    bFirstPaneInRow = FALSE;        
					// remember where the first pane in the row is..
                    nFirstPaneInRow = nPos;
					
                    if (m_dwStyle & (CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT))
					{
                        bDrawBarForRow = IsRowSizeable(nPos) && !bSomeToolBar;
					}
					
                    if (bDrawBarForRow)
					{
                        if (bHorz)
						{
                            AddSplitterRect(SPLITTER_HORZ, pt.x, pt.y, 0 ,
								pt.y + CY_SPLIT, nPos);   // width set at end
                            pt.y += CY_SPLIT;
						}
                        else
						{
                            AddSplitterRect(SPLITTER_VERT, pt.x, pt.y, 
								pt.x + CX_SPLIT, 0, nPos);   // height set at end
                            pt.x += CX_SPLIT;
						}
                        bDrawBarForRow = FALSE;
					}
				}

				// draw a bar between 2 previous
				if (bLastWasResizable && IsSizeable(pBar)&!bSomeToolBar)  
				{
                    if (bHorz)
					{
						// width set at end of row
                        AddSplitterRect(SPLITTER_VERT, pt.x, pt.y, 
							pt.x + CX_SPLIT, 0, nPos); 
                        pt.x += CX_SPLIT;
					}
                    else
					{
						// width set at end of row
                        AddSplitterRect(SPLITTER_HORZ, pt.x, pt.y, 0, 
							pt.y + CY_SPLIT, nPos); 
                        pt.y += CY_SPLIT;
					}
				}
				
				// if the bar is sizeable, then we'll need a sizer after it
                if (IsSizeable(pBar))     
				{
                    bDrawBarForRow|=!bSomeToolBar;
				}
				
                // get ideal rect for bar to be docked
				DWORD dwMode = 0;
				if ((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) &&
					(pBar->m_dwStyle & CBRS_FLOATING))
					dwMode |= LM_HORZ | LM_MRUWIDTH;
				else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
					dwMode |= LM_HORZ | LM_HORZDOCK;
				else
					dwMode |=  LM_VERTDOCK;

				CSize sizeBar = pBar->CalcDynamicLayout(-1, dwMode);
                CRect rect(pt, sizeBar);

                // get current rect for bar to be docked
                CRect rectBar;
                pBar->GetWindowRect(&rectBar);
                ScreenToClient(&rectBar);

				if (bHorz)
				{
					if(IsSizeable(pBar) && !bSomeToolBar)
					{
						// If ControlBar has been wrapped, then left justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(-(rect.left + afxData.cxBorder2), 0);
						}
						else if(rect.left>=sizeMax.cx-afxData.cxBorder2 || 
							rectBar.right<=rectBar.left || rectBar.right<=0)
						{
							if(nFirstPaneInRow==nPos)
							{
								m_arrBars.InsertAt(nPos+1, (CObject*)NULL);
								AdjustRowSizes(nPos,sizeMax.cx,m_arrBars);
								AdjustRowSizes(nPos+2,sizeMax.cx,m_arrBars);
								pBar = NULL; 
								pVoid = NULL;
								bWrapped = TRUE;
								nPos--;
							}
							else
							{
								m_arrBars.InsertAt(nPos, (CObject*)NULL);
								AdjustRowSizes(nPos+1,sizeMax.cx,m_arrBars);
								pBar = NULL; 
								pVoid = NULL;
								bWrapped = TRUE;
							}
						}
					}
					else
					{
						// Offset Calculated Rect out to Actual
						if (rectBar.left > rect.left && !m_bFloating)
						{
							if(!bLastWasResizable && nCountSizeBars==0)
								rect.OffsetRect(rectBar.left - rect.left, 0);
						}

						// If ControlBar goes off the right, then right justify
						if (rect.right > sizeMax.cx && !m_bFloating)
						{
							int x = rect.Width();// - afxData.cxBorder2;
							x = max(sizeMax.cx - x, pt.x);
							rect.OffsetRect(x - rect.left, 0);
						}

						// If ControlBar has been wrapped, then left justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(-(rect.left + afxData.cxBorder2), 0);
						}
						// If ControlBar is completely invisible, then wrap it
						else if ((rect.left >= (sizeMax.cx - afxData.cxBorder2)) &&
							(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
						{
							m_arrBars.InsertAt(nPos, (CObject*)NULL);
							pBar = NULL; pVoid = NULL;
							bWrapped = TRUE;
						}
					}

					if (!bWrapped)
					{
						if (rect != rectBar)
						{
							if (!m_bLayoutQuery &&
								!(pBar->m_dwStyle & CBRS_FLOATING))
							{
								pBar->m_pDockContext->m_rectMRUDockPos = rect;
							}
							AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
						}
						pt.x = rect.left + sizeBar.cx - afxData.cxBorder2;
						nWidth = max(nWidth, sizeBar.cy);
					}
				}
                else
				{
					if(IsSizeable(pBar) && !bSomeToolBar)
					{
						// If ControlBar has been wrapped, then top justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(0, -(rect.top + afxData.cyBorder2));
						}
						else if(rect.top>=sizeMax.cy-afxData.cyBorder2 || 
							rectBar.bottom<=rectBar.top || rectBar.bottom<=0)
						{
							if(nFirstPaneInRow==nPos)
							{
								m_arrBars.InsertAt(nPos+1, (CObject*)NULL);
								AdjustRowSizes(nPos,sizeMax.cy,m_arrBars);
								AdjustRowSizes(nPos+2,sizeMax.cy,m_arrBars);
								pBar = NULL; 
								pVoid = NULL;
								bWrapped = TRUE;
								nPos--;
							}
							else
							{
								m_arrBars.InsertAt(nPos, (CObject*)NULL);
								AdjustRowSizes(nPos+1,sizeMax.cy,m_arrBars);
								pBar = NULL; 
								pVoid = NULL;
								bWrapped = TRUE;
							}
						}
					}
					else
					{
						// Offset Calculated Rect out to Actual
						if (rectBar.top > rect.top && !m_bFloating)
						{
							if(!bLastWasResizable && nCountSizeBars==0)
								rect.OffsetRect(0, rectBar.top - rect.top);
						}

						// If ControlBar goes off the bottom, then bottom justify
						if (rect.bottom > sizeMax.cy && !m_bFloating)
						{
							int y = rect.Height();// - afxData.cyBorder2;
							y = max(sizeMax.cy - y, pt.y);
							rect.OffsetRect(0, y - rect.top);
						}

						// If ControlBar has been wrapped, then top justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(0, -(rect.top + afxData.cyBorder2));
						}
						// If ControlBar is completely invisible, then wrap it
						else if ((rect.top >= (sizeMax.cy - afxData.cyBorder2)) &&
							(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
						{
							m_arrBars.InsertAt(nPos, (CObject*)NULL);
							pBar = NULL; pVoid = NULL;
							bWrapped = TRUE;
						}
					}

					if (!bWrapped)
					{
						if (rect != rectBar)
						{
							if (!m_bLayoutQuery &&
								!(pBar->m_dwStyle & CBRS_FLOATING))
							{
								pBar->m_pDockContext->m_rectMRUDockPos = rect;
							}
							AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
						}
						pt.y = rect.top + sizeBar.cy - afxData.cyBorder2;
						nWidth = max(nWidth, sizeBar.cx);
					}
				}
				
				/////////////////////////

				if(pBar!=NULL)
				{
					// repositioned the bar, so update the MRU dock position.
					CDockContext* pDockContext = pBar->m_pDockContext;
					ASSERT(pDockContext != NULL);
					if (pDockContext != NULL)
					{
						pDockContext->m_rectMRUDockPos = rect;
						pDockContext->m_uMRUDockID = uDockBarID;
					}			

					// handle any delay/show hide for the bar
					pBar->RecalcDelayShow(&layout);

					bLastWasResizable=IsSizeable(pBar)&!bSomeToolBar;
				}
			}
		}
		else
		{
			nCountSizeBars=0;
			// calculate the number of sizable bars in the row
			for (int nPos2 = nPos + 1; nPos2 < m_arrBars.GetSize(); nPos2++) 
			{
				void* pVoid = m_arrBars[nPos2];
				CControlBar* pBar = GetDockedControlBar(nPos2);
				if (pVoid == NULL)
					break;  // end of row

				if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
					pBar->IsVisible())
				{
					if (IsSizeable(pBar) && !((CControlBar*)pBar)->
						IsKindOf(RUNTIME_CLASS(COXSizeToolBar)))
					{
						nCountSizeBars++;
					}
				}
			}
			////////////////////////////////////

			if (!bFirstPaneInRow)	// FALSE if we've hit anything....
			{
				// end of row because pBar == NULL
				if (bHorz)
				{
					pt.y += nWidth - afxData.cyBorder2;
					sizeFixed.cx = __max(sizeFixed.cx, pt.x);
					sizeFixed.cy = __max(sizeFixed.cy, pt.y);
					pt.x = -afxData.cxBorder2;
					SetSplitterSizeInRange(nFirstSplitterInRow, SPLITTER_VERT, pt.y);
				}
				else
				{
					pt.x += nWidth - afxData.cxBorder2;
					sizeFixed.cx = __max(sizeFixed.cx, pt.x);
					sizeFixed.cy = __max(sizeFixed.cy, pt.y);
					pt.y = -afxData.cyBorder2;
					SetSplitterSizeInRange(nFirstSplitterInRow, SPLITTER_HORZ, pt.x);
				}
				nLastVisSep = nPos;			// record separator for last vis position
			}
		
			nFirstSplitterInRow = __max(m_SplitArr.GetSize(), 0);
			nWidth = 0;
			bFirstPaneInRow = TRUE;
			bLastWasResizable=FALSE;
		}
	}

	// special case when bars are at top or left.
	// use of nFirstPaneInRow (nPos where row started) so that sizing code can cope ok

	if (nFirstPaneInRow != 0 && bDrawBarForRow && 
		(m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT)))   // there is at least one pane.
	{
        ASSERT(nLastVisSep != -1);
		if (m_dwStyle & CBRS_ALIGN_TOP)
		{
            AddSplitterRect(SPLITTER_HORZ, pt.x, pt.y, 0 , 
				pt.y + CY_SPLIT, nLastVisSep);
			sizeFixed.cy += CY_SPLIT;
		}
		else
		{
            AddSplitterRect(SPLITTER_VERT, pt.x, pt.y, 
				pt.x + CX_SPLIT, 0, nLastVisSep);
			sizeFixed.cx += CX_SPLIT - 1;
		}
	}

	if (!m_bLayoutQuery)
	{
		// move and resize all the windows at once!
		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
	}

	// Finally go back and set the size of the bars between the rows
	if (bHorz)
		SetSplitterSizeInRange(0, SPLITTER_HORZ, sizeFixed.cx); // set widths of inter-rows
	else
		SetSplitterSizeInRange(0, SPLITTER_VERT, sizeFixed.cy); // set heights of inte-rcolumns

	// adjust size for borders on the dock bar itself
	CRect rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	if ((!bStretch || !bHorz) && sizeFixed.cx != 0)
		sizeFixed.cx += -rect.right + rect.left;
	if ((!bStretch || bHorz) && sizeFixed.cy != 0)
		sizeFixed.cy += -rect.bottom + rect.top;

	return sizeFixed;
}



void COXSizeDockBar::AddSplitterRect(int type, int x1, int y1, int x2, int y2, int nPos)
{
    COXSplitterRect* pSplit = new COXSplitterRect(type, CRect(x1, y1, x2, y2));
    pSplit->m_nPos = nPos;
    ASSERT( pSplit != NULL);
    m_SplitArr.Add(pSplit);
}


// helper function: Sets the length of all COXSplitterRects in the range 
// (start->end of array) with the specified type. Used at the end of a row 
// to set all the heights to the calculated width.
void COXSizeDockBar::SetSplitterSizeInRange(int start, int type, int length)
{
    ASSERT(type == SPLITTER_VERT || type == SPLITTER_HORZ);
    ASSERT(start >= 0 && start <= m_SplitArr.GetSize());
	
    for (int i = m_SplitArr.GetUpperBound(); i >= start; i--)
	{
        COXSplitterRect* pItem = (COXSplitterRect*)m_SplitArr[i];
        if (pItem->m_type == type)
		{
            if (type == SPLITTER_VERT)
                pItem->m_rect.bottom = length;
            else
                pItem->m_rect.right = length;
		}
	}
}


void COXSizeDockBar::DeleteSplitterRects()
{
    for (int i = m_SplitArr.GetUpperBound(); i >= 0 ; i--)
        delete (m_SplitArr[i]);
    m_SplitArr.RemoveAll();
}


void COXSizeDockBar::OnPaint()
{
    CPaintDC dc(this); // device context for painting
	
    for (int i = m_SplitArr.GetUpperBound(); i >= 0; i--)
        ((COXSplitterRect*)(m_SplitArr[i]))->Draw(&dc);
}




COXSplitterRect* COXSizeDockBar::HitTest(CPoint pt)
{
	for (int i = m_SplitArr.GetUpperBound(); i >= 0; i--)
	{
		COXSplitterRect* pSplit = GetSplitter(i);
        if (pSplit->m_rect.PtInRect(pt))
			return(pSplit);
	}
    return NULL;
}


BOOL COXSizeDockBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT && pWnd == this) // && !m_bTracking)
        return TRUE;    // we will handle it in the mouse move
	
    return CDockBar::OnSetCursor(pWnd, nHitTest, message);
}



// if already positioned on NULL, go back one further
int COXSizeDockBar::StartPosOfRow(int nPos)
{
    if (nPos > 0)
	{
        if (m_arrBars[nPos] == NULL)
            nPos --;
        while (nPos >= 0)
		{
            if (m_arrBars[nPos] == NULL)
                return (nPos + 1);
            nPos --;
		}
	}
	return 0;
}


// returns strart of previous row.
// This function includes logic to cope with nPos beyond the end of the array.
int COXSizeDockBar::StartPosOfPreviousRow(int nPos)
{
    ASSERT (nPos > 0);
    if (nPos >= m_arrBars.GetUpperBound())
        return (StartPosOfRow(nPos));
    else
        return StartPosOfRow(nPos - 1);
}



// Go through control bars in the row.
// returns the number of sizeable bars in the row, and the total space they currently
// take up
void COXSizeDockBar::GetRowSizeInfo(int nPos, ROWSIZEINFO* pRZI, 
									const CPtrArray& arrBars)
{
	BOOL bHorz = IsBarHorizontal();
	
    // zero all the fields
    memset (pRZI, 0, sizeof (ROWSIZEINFO));

	BOOL bLastWasResizable=FALSE;
    while (nPos <= arrBars.GetUpperBound())
	{
		CRect rect;
        void* pVoid = arrBars[nPos];
		if (pVoid == NULL)
		{
			break;                  // end of the row
		}
		CControlBar* pBar=::GetDockedControlBar(nPos, arrBars);
		if (pBar!= NULL && ::IsWindow(pBar->GetSafeHwnd()) && pBar->IsVisible())
		{	
			// add a splitter size
			if((IsSizeable(pBar) && 
				!pBar->IsKindOf(RUNTIME_CLASS(COXSizeToolBar))))
			{
//				pRZI->nFlexWidth += (bHorz ? afxData.cxBorder2 : afxData.cyBorder2);
				if(bLastWasResizable)
					pRZI->nFixedWidth += (bHorz ? CX_SPLIT : CY_SPLIT);     
			}
			else
			{
//				pRZI->nFixedWidth += (bHorz ? afxData.cxBorder2 : afxData.cyBorder2);
			}
			
            pBar->GetWindowRect(&rect);
            int nWidth = __max(0, (bHorz ? rect.Width() : rect.Height()) );
            int nHeight = __max(0, (bHorz ? rect.Height() : rect.Width()) );
			
            pRZI->nTotalBars ++;
            if(nHeight > pRZI->nMaxHeight)
                pRZI->nMaxHeight = nHeight;
			
            if(IsSizeable(pBar))
			{
                pRZI->nFlexBars ++;
                pRZI->nFlexWidth += nWidth;
			}
            else
			{
                pRZI->nFixedWidth += nWidth;
                if (nHeight > pRZI->nMaxFixedHeight)
					pRZI->nMaxFixedHeight = nHeight;
			}
			bLastWasResizable=IsSizeable(pBar) && 
				!pBar->IsKindOf(RUNTIME_CLASS(COXSizeToolBar));
		}
        nPos++;
	}

   	pRZI->nTotalWidth = pRZI->nFixedWidth + pRZI->nFlexWidth;
	return;
}


// Adjusts the sizes of all the bars on a dockbar to fit a new size
BOOL COXSizeDockBar::AdjustAllRowSizes(int nNewSize)
{
    BOOL bAdjusted = FALSE;
    int nPos = 0;
    while (nPos < m_arrBars.GetSize())
	{
        CControlBar* pBar = (CControlBar*)m_arrBars[nPos];
        if (pBar == NULL)
		{                                               
			// skip over NULLs
			nPos++;
            continue;
		}
        // adjust the sizes on a row
        bAdjusted |= AdjustRowSizes(nPos, nNewSize, m_arrBars);
		// skip to end of row
        while (m_arrBars[nPos] != NULL)                 
			nPos++;
	}
    return bAdjusted;
}


// Adjusts the size of a row - returns TRUE if any changes were made
BOOL COXSizeDockBar::AdjustRowSizes(int nPos, int nNewSize, CPtrArray& arrBars)
{
	BOOL bHorz = IsBarHorizontal();

    ROWSIZEINFO RZI;
    GetRowSizeInfo(nPos, &RZI, arrBars);
    if (RZI.nFlexBars == 0)
		return FALSE;                   // no flexible bars - nothing to do !
	
	// prepare for layout
	int nTotalSize = (nNewSize - RZI.nFixedWidth)+
		(bHorz ? afxData.cxBorder2 : afxData.cyBorder2)*RZI.nTotalBars;
    int nSizeRemaining = nTotalSize;
	int nSize=0;

	// have to apply this size change to the bars on this row. Note: This will work
    // by setting the docked size of the controls bars directly. 
	// Then RecalcLayout will do the rest.
    int nCountFlexBars = 0;
    while (TRUE)
	{
		void* pVoid = arrBars[nPos];
		if (pVoid == NULL)
            break;          // end of the row, stop
		// note:slight abuse of cast
		COXSizeControlBar* pBar = 
			(COXSizeControlBar*)::GetDockedControlBar(nPos, arrBars);	
        
        if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
			pBar->IsVisible() && IsSizeable(pBar))
		{
			CRect rect;
            pBar->GetWindowRect(&rect);
            int nWidth = (bHorz ? rect.Width() : rect.Height());
///			int nWidth = (bHorz ? pBar->m_HorzDockSize.cx : pBar->m_VertDockSize.cy);
			nCountFlexBars ++;
			if(RZI.nFlexWidth==0)
				nSize = nSizeRemaining;
			else
			{
				nSize = (int)((float)(((nWidth * nTotalSize)) / RZI.nFlexWidth) + 
					(float)0.5);
			}

#ifdef _VERBOSE_TRACE
			CString strTitle;
			pBar->GetWindowText(strTitle);
			TRACE2("New Size : %d on %s\n", nSize, strTitle);
#endif			

			nSizeRemaining -= nSize;

			if (bHorz)
			{
				pBar->m_HorzDockSize.cx = nSize;
                pBar->m_HorzDockSize.cy = RZI.nMaxHeight;
				SetWindowSize(pBar, pBar->m_HorzDockSize);
			}
            else
			{
                pBar->m_VertDockSize.cy = nSize;
				pBar->m_VertDockSize.cx = RZI.nMaxHeight;
				SetWindowSize(pBar, pBar->m_VertDockSize);
			}

		}
        nPos++;
	}

	return TRUE;
}


// Adjusts the sizes of all the bars on a dockbar to fit a new size
void COXSizeDockBar::TileDockedBars()
{
    int nPos = 0;
    while (nPos < m_arrBars.GetSize())
	{
        CControlBar* pBar = (CControlBar*)m_arrBars[nPos];
        if (pBar == NULL)
		{                                               // skip over NULLs
            nPos ++;
            continue;
		}
        TileDockedBarsRow(nPos);                                        // adjust the sizes on a row
        while (m_arrBars[nPos] != NULL)                         // skip to end of row
            nPos++;
	}
    return;
}


// Tiles the docked bars:
void COXSizeDockBar::TileDockedBarsRow(int nPos)
	{
    BOOL bHorz = IsBarHorizontal();
	
    ROWSIZEINFO RZI;
    GetRowSizeInfo(nPos, &RZI, m_arrBars);
    if (RZI.nFlexBars == 0)
		return;                         // no flexible bars - nothing to do !
	
    int nNewSize = (bHorz ? m_LayoutSize.cx : m_LayoutSize.cy);
    int nTotalSize = __max(0, nNewSize - RZI.nFixedWidth);
    int nNewWidth = nTotalSize / RZI.nFlexBars;
	
    int nCountFlexBars = 0;
    while(TRUE)
		{
		void* pVoid = m_arrBars[nPos];    
		if (pVoid == NULL)
			break;          // end of the row, stop
		COXSizeControlBar* pBar = (COXSizeControlBar*)GetDockedControlBar(nPos); // note:slight abuse of cast
        
        if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
			IsSizeable(pBar) && pBar->IsVisible())
			{
            nCountFlexBars ++;
            if (nCountFlexBars == RZI.nFlexBars)    // last bar adjust size change
				{
                nNewWidth = nTotalSize - ((RZI.nFlexBars - 1) * nNewWidth);
				}
            if (bHorz)
				{
                pBar->m_HorzDockSize.cx = nNewWidth;
                pBar->m_HorzDockSize.cy = RZI.nMaxHeight;
				}
            else
				{
                pBar->m_VertDockSize.cy = nNewWidth;
                pBar->m_VertDockSize.cx = RZI.nMaxHeight;
				}
			}
        nPos++;
		}
	}

// Returns TRUE if BAR is in m_arrInvisibleBars
BOOL COXSizeDockBar::WasBarHidden(CControlBar* pBar)
{
	for (int i= 0; i < m_arrHiddenBars.GetSize(); i++)
	{							
		if (m_arrHiddenBars[i] == pBar)
			return TRUE;
	}
	return FALSE;
}


// WM_SIZEPARENT message is sent from CFrameWnd::RepositionBars() to tell the 
// dockbar to calculate it's size. The only reason for intercepting this was 
// to actually find out the size the dockbar is taking up in the layout, 
// so we can opt to re-layout a row to fit the desired size. There might well 
// be a better way of doing this.
LRESULT COXSizeDockBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	
	BOOL bHorz = IsBarHorizontal();
	
    CRect LayRect;
    LayRect.CopyRect(&lpLayout->rect);
    CSize LaySize = LayRect.Size();  // maximum size available
	int nLayoutWidth = bHorz ? LaySize.cx : LaySize.cy;
	BOOL bLayoutWidthChanged=
		(nLayoutWidth != (bHorz ? m_LayoutSize.cx : m_LayoutSize.cy));
	m_LayoutSize = LaySize;
	
	// Attempt to detect bars that have changed state from Hidden->Visible. For these we attempt
	// to adjust the other (previously visible) bars on the row so that the newly shown bars
	// restore their previous size.
	CPtrArray	arrVisibleBarsInRow;		// Bars visible in the row (ones we can shrink)
	int nWidthNeeded = 0;
	for (int i = 0; i < m_arrBars.GetSize(); i++)
	{
		if (m_arrBars[i] == NULL)
		{
			ROWSIZEINFO RZI;
			if (arrVisibleBarsInRow.GetSize() != 0 && nWidthNeeded != 0)
			{
				arrVisibleBarsInRow.Add(NULL);
				
				GetRowSizeInfo(0, &RZI, arrVisibleBarsInRow);
				int nNewWidth = __max(0, RZI.nTotalWidth - nWidthNeeded);
				AdjustRowSizes(0, nNewWidth, arrVisibleBarsInRow);
			}
			nWidthNeeded = 0;
			arrVisibleBarsInRow.RemoveAll();
		}
		else
		{
			CControlBar* pBar = GetDockedControlBar(i);
			if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()))
			{
				if (pBar->IsVisible())
				{
					if (WasBarHidden(pBar))
					{
						TRACE0("Bar hidden->visible\n");
						CRect rect;
						pBar->GetWindowRect(&rect);
						nWidthNeeded += (bHorz ? rect.Width() : rect.Height());
					}
					else
					{
						arrVisibleBarsInRow.Add(pBar);		// Track visible bars in this row that we can shrink
					}
				}
			}
		}
	}
	
	// construct new array of bars that are hidden in this dockbar
	m_arrHiddenBars.RemoveAll();
	for (i = 0; i < m_arrBars.GetSize(); i++)
	{
		CControlBar* pBar = GetDockedControlBar(i);
		if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && ! pBar->IsVisible())
			m_arrHiddenBars.Add(pBar);
	}
	
	int nCheckSum = CheckSumBars();
	
    // any other changes and we size the bars to fit the layout width
	if (bLayoutWidthChanged || nCheckSum != m_CountBars)
	{
        AdjustAllRowSizes(nLayoutWidth);
        m_CountBars = nCheckSum;
		// force redraw of the dock bar - seems a bit of a sledgehammer
		InvalidateRect(NULL);   
		// force redraw of the dock bar - seems a bit of a sledgehammer
//		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);   
	}
	
	// set m_bLayoutQuery to TRUE if lpLayout->hDWP == NULL
	BOOL bLayoutQuery = m_bLayoutQuery;
    m_bLayoutQuery = (lpLayout->hDWP == NULL);
	LRESULT lResult = CControlBar::OnSizeParent(wParam, lParam);
    // restore m_bLayoutQuery
    m_bLayoutQuery = bLayoutQuery;
	
    return lResult;

//	return CDockBar::OnSizeParent(wParam,lParam);
}



// Simple checksum for bars. Designed to spot the case when a bars moves within
// a dockrow.
int COXSizeDockBar::CheckSumBars() const
{
	int nCount = 0;         // total no of bars
    int nCheckSum = 0;      // XOR, power of 2 checksum
    for (int i = 0; i < m_arrBars.GetSize(); i++)
	{
		if (m_arrBars[i] == NULL)
			nCheckSum *= 2;
		else
		{        
			CControlBar* pBar = GetDockedControlBar(i);
			ASSERT(pBar == NULL || pBar->IsKindOf(RUNTIME_CLASS(CControlBar)));
			if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && pBar->IsVisible())
			{
				nCheckSum++;
				if(IsSizeable(pBar) && !pBar->IsKindOf(RUNTIME_CLASS(COXSizeToolBar)))
					nCheckSum+=i;
                nCount++;
			}
		}
	}
	// LSB = actual no of dockbars (limited to 256 !)
	// bits 8-31 = checksum based on layout of rows.
    return ((nCheckSum << 8) | (nCount & 0xff));
}


// Adjust sizes for specified newly added bar.
void COXSizeDockBar::AdjustForNewBar(CControlBar* pNewBar)		
{
	int nPos = FindBar(pNewBar);
	ASSERT(nPos != -1);			// bar should have been found.
	
	// Go back to start of row.
	while (m_arrBars[nPos] != NULL)
		nPos--;
	
	nPos++;
	
	// create an array for the bars on the row, that aren't this one
	CPtrArray arrOtherBarsInRow;
	while (nPos < m_arrBars.GetSize() && m_arrBars[nPos] != NULL)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		if (pBar != pNewBar)
			arrOtherBarsInRow.Add(pBar);
		nPos++;
	}
	
	ROWSIZEINFO RZI;
	arrOtherBarsInRow.Add(NULL);
	GetRowSizeInfo(0, &RZI, arrOtherBarsInRow);
	CRect rcNewBar;
	pNewBar->GetWindowRect(&rcNewBar);
	int nWidthNeeded = (IsBarHorizontal() ? rcNewBar.Width() : 
		rcNewBar.Height());
	if(IsSizeable(pNewBar) && RZI.nFlexBars>0)
		nWidthNeeded+=(IsBarHorizontal() ? CX_SPLIT : CY_SPLIT);
	int nNewWidth = __max(0, RZI.nTotalWidth - nWidthNeeded);

	AdjustRowSizes(0, nNewWidth, arrOtherBarsInRow);
}

// Hit test the mouse position - and set cursor accordingly
COXSplitterRect* COXSizeDockBar::SetHitCursor(CPoint pt)
	{
	// Set up the split cursors here. This guarantees the app is around
    if (m_hcurSizeWE == NULL)
		{ 
		m_hcurSizeWE = AfxGetApp()->LoadCursor(AFX_IDC_HSPLITBAR);
		if (m_hcurSizeWE == NULL)
			m_hcurSizeWE = ::LoadCursor(NULL, IDC_SIZEWE);
		}			
	
    if (m_hcurSizeNS == NULL)
		{
		m_hcurSizeNS = AfxGetApp()->LoadCursor(AFX_IDC_VSPLITBAR);
		if (m_hcurSizeNS == NULL)
			m_hcurSizeNS = ::LoadCursor(NULL, IDC_SIZENS);
		}
	
	
    HCURSOR hcurNew;
    COXSplitterRect* pSplit = HitTest(pt);
    if (pSplit != NULL)
		hcurNew = (pSplit->m_type == SPLITTER_VERT ? m_hcurSizeWE : m_hcurSizeNS);
    else
        hcurNew = afxData.hcurArrow;
	
    ::SetCursor(hcurNew);
    return pSplit;
	}


void COXSizeDockBar::OnMouseMove(UINT nFlags, CPoint point)
{
    SetHitCursor(point);
    CDockBar::OnMouseMove(nFlags, point);
}



void COXSizeDockBar::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
	if (m_pSplitCapture == NULL)
	{
		m_pSplitCapture = SetHitCursor(point);
		if (m_pSplitCapture != NULL)
		{
			StartTracking(point);
			m_pSplitCapture = NULL;
		}
	}
}


void COXSizeDockBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    CDockBar::OnLButtonUp(nFlags, point);
}


void COXSizeDockBar::StartTracking(CPoint pt)
{
    ASSERT(m_pSplitCapture != NULL);
	
    // Some organizational flags: helps to cut down the cases
    BOOL bHorz      = IsBarHorizontal();
    BOOL bVertSplitter = (m_pSplitCapture->m_type == SPLITTER_VERT);
    BOOL bRowDivider = ((!bVertSplitter) && bHorz) || (bVertSplitter && (!bHorz));
    int nPos = m_pSplitCapture->m_nPos;
	
    COXRectTracker MvRect;
	
    // attempt to clip move rect by current layout size of the dockbar
    CRect LayoutRect(CPoint(0,0), m_LayoutSize);
    MvRect.m_rect = m_pSplitCapture->m_rect;
	
   	// Shrink the splitter rectangle to end up with a solid bar
	if (bVertSplitter)
		MvRect.m_rect.InflateRect(-2, 0);
	else
		MvRect.m_rect.InflateRect(0, -2);

	MvRect.m_rect.IntersectRect(MvRect.m_rect, LayoutRect);
    ASSERT(!(MvRect.m_rect.IsRectEmpty()));
    
    // get main window - all dragging is done relative to this window.
	// this should be the frame window.....
	CWnd* pClipWnd = GetParentFrame();
	
    if (bVertSplitter)
		MvRect.m_nStyle |= RectTracker_OnlyMoveHorz;      // allow horizontal movement
    else
		MvRect.m_nStyle |= RectTracker_OnlyMoveVert;      // allow vertical movement
	
    // workout a limiting rectangle; - very dependent on orientation. Eventually may need to work
    // out the fixed size of the windows beyond the current splitter, so it could get nasty.
    // for now just use the client area of the window
    ROWSIZEINFO RZI;
	::ZeroMemory((void*)&RZI,sizeof(RZI));
    CRect LimitRect;
    pClipWnd->GetClientRect(&LimitRect);
    pClipWnd->ClientToScreen(&LimitRect);
    ScreenToClient(&LimitRect);			// map to co-ords of pWnd
	
#ifdef _VERBOSE_TRACE
	Dump(afxDump);
#endif

    if (bRowDivider)
		{
		if (m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT))             // apply to previous row for top/bottom
		{
			nPos = StartPosOfPreviousRow(nPos);
            ASSERT(nPos != 0);
		}
       	GetRowSizeInfo(nPos, &RZI, m_arrBars);             // get the row information:
       	switch (m_dwStyle & CBRS_ALIGN_ANY)
		{
			case CBRS_ALIGN_BOTTOM:
                LimitRect.bottom = __min(LimitRect.bottom, MvRect.m_rect.top + (RZI.nMaxHeight - RZI.nMaxFixedHeight));
                break;
            case CBRS_ALIGN_TOP:
				LimitRect.top = __max(LimitRect.top, MvRect.m_rect.top - (RZI.nMaxHeight - RZI.nMaxFixedHeight));
				break;
            case CBRS_ALIGN_LEFT:
                LimitRect.left = __max(LimitRect.left, MvRect.m_rect.left - (RZI.nMaxHeight - RZI.nMaxFixedHeight));
                break;
            case CBRS_ALIGN_RIGHT:
                LimitRect.right = __max(LimitRect.right, MvRect.m_rect.left + (RZI.nMaxHeight - RZI.nMaxFixedHeight));
                break;
            default:
                ASSERT(FALSE);
		}
	}
	else
	{
        // How far can we go to down/right
        int nFlexToRight, nFlexToLeft;
        int nDownRight = ShrinkRowToRight(nPos, 16000, FALSE, &nFlexToRight);
        int nUpLeft = ShrinkRowToLeft(nPos - 1, 16000, FALSE, &nFlexToLeft);
		
        if ((nFlexToRight + nFlexToLeft) <= 1 )  // only 1 flex bar in the array - no movement !
		{
            nDownRight = 0;
            nUpLeft = 0;
		}
		
        if (bHorz)
		{
			LimitRect.left = __max(LimitRect.left, MvRect.m_rect.left - nUpLeft);
            LimitRect.right = __min(LimitRect.right, MvRect.m_rect.left + nDownRight);
		}
        else
		{
            LimitRect.top = __max(LimitRect.top , MvRect.m_rect.top - nUpLeft);
            LimitRect.bottom = __min(LimitRect.bottom, MvRect.m_rect.top + nDownRight);
		}
	}
	
    // Now enter the COXSplitterRect's modal track function
    MvRect.m_LimitRect = LimitRect;
    if (!MvRect.TrackFromHitTest (HTCAPTION, this, pt, pClipWnd))
		return;
	
	// Workout the size change cause by the drag:
	int nSizeChange;
    if (m_pSplitCapture->m_type == SPLITTER_VERT)
		nSizeChange = MvRect.m_rect.left - MvRect.m_OrigRect.left;
	else
        nSizeChange = MvRect.m_rect.top - MvRect.m_OrigRect.top;
    if (nSizeChange == 0)
        return;
	
    // COXSplitterRect::m_nPos is the pane position that the splitter was created at.
    // For a row divider: this is the pane that immediately starts the next row
    // For a column divider: this is the pane that is to the right of it.
    // special case will be needed for the splitter used at the end of a left/top aligned
    // dockbar.
    int nSizeMoved;
    if (bRowDivider)
	{
        if (m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT))             // apply to previous row for top/bottom
		{
			nSizeChange = -nSizeChange;             // reverse polarity of change
		}
		
        int nNewHeight = __max(RZI.nMaxFixedHeight, RZI.nMaxHeight - nSizeChange);
		
        // go along the rows applying size change to each bar in turn....
        while (nPos < m_arrBars.GetSize())	// need to check size now
		{
            void* pVoid = m_arrBars[nPos];
			if (pVoid == NULL)
                break;
            COXSizeControlBar* pBar = (COXSizeControlBar*)GetDockedControlBar(nPos);
            // should check for visible ???
			if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
				pBar->IsVisible() && IsSizeable(pBar))
			{
                if (bHorz)
                   	pBar->m_HorzDockSize.cy = nNewHeight;
                else
					pBar->m_VertDockSize.cx = nNewHeight;
	
			}
            nPos ++;
		}
		
	}
    else
	{
        if (nSizeChange < 0)
		{
			// move to left/up
			nSizeMoved = ShrinkRowToLeft(nPos - 1, - nSizeChange, TRUE);
            ShrinkRowToRight(nPos, - nSizeMoved, TRUE);
		}
        else
		{
			// move to right/down
			nSizeMoved = ShrinkRowToRight(nPos, nSizeChange, TRUE);
            ShrinkRowToLeft(nPos - 1, - nSizeMoved, TRUE);
		}
		
	}
    // reposition the bars..
    InvalidateRect(NULL);
    //((CFrameWnd*)AfxGetMainWnd())->RecalcLayout();
    ASSERT(pClipWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
    ((CFrameWnd*)pClipWnd)->RecalcLayout();
    return;
}


// amount to shrink row to left.
// nPos = current pane: nPos -1 = pane to go for:
// return value = amount of space we actually sized
// bApply: if TRUE, apply changes to bar sizes
int COXSizeDockBar::ShrinkRowToLeft(int nPos, int nOrigAmount, BOOL bApply, int* pnFlexBars)
	{
    ASSERT(nPos >= 0 && nPos <= m_arrBars.GetSize());
	
    int nAmount = nOrigAmount;
    int nFlexBars = 0;
    while (nPos >= 0)
		{
		if (m_arrBars[nPos] == NULL)
            break;
        COXSizeControlBar* pBar = (COXSizeControlBar*)GetDockedControlBar(nPos);
        if (pBar!= NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
			IsSizeable(pBar) && pBar->IsVisible())
			{
            nFlexBars ++;
            if (IsBarHorizontal())
				{
                if (pBar->m_HorzDockSize.cx >= nAmount)
					{
                    if (bApply)
                        pBar->m_HorzDockSize.cx -= nAmount;
                    nAmount = 0;
                    break;
					}
                else
					{
                    nAmount -= pBar->m_HorzDockSize.cx;
                    if (bApply)
                        pBar->m_HorzDockSize.cx = 0;
					}
				}
            else
				{
                if (pBar->m_VertDockSize.cy >= nAmount)
					{
                    if (bApply)
                        pBar->m_VertDockSize.cy -= nAmount;
                    nAmount = 0;
                    break;
					}
                else
					{
                    if (bApply)
                        pBar->m_VertDockSize.cy = 0;
                    nAmount -= pBar->m_VertDockSize.cy;
					}
				}
			}
        nPos--;
		}
	
    // return no of flexible components encountered (if pointer supplied)
    if (pnFlexBars != NULL)
        *pnFlexBars = nFlexBars;
	
    // reached left/top of row - return what size is still left to allocate
    return (nOrigAmount - nAmount);
	}


// amount to shrink row to right.
// nPos = current pane: nPos -1 = pane to go for:
// return value = amount of space we actually sized
int COXSizeDockBar::ShrinkRowToRight(int nPos, int nOrigAmount, BOOL bApply, int* pnFlexBars)
	{
    ASSERT(nPos >= 0 && nPos <= m_arrBars.GetSize());
    int nAmount = nOrigAmount;
    int nFlexBars = 0;
	
    COXSizeControlBar* pLastBar = NULL;
	
    while (nPos < m_arrBars.GetSize())
		{
        if (m_arrBars[nPos] == NULL)
			break;
		
		COXSizeControlBar* pBar = (COXSizeControlBar*)GetDockedControlBar(nPos);
        if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()))
			{				
			pLastBar = pBar;
			if (IsSizeable(pBar) && pBar->IsVisible())
				{
				nFlexBars ++;
				if (IsBarHorizontal())
					{
					if (pBar->m_HorzDockSize.cx >= nAmount)
						{
						if (bApply)
							pBar->m_HorzDockSize.cx -= nAmount;
						nAmount = 0;
						break;
						}
					else
						{
						nAmount -= pBar->m_HorzDockSize.cx;
						if (bApply)
							pBar->m_HorzDockSize.cx = 0;
						}
					}
				else		// Vertical
					{
					if (pBar->m_VertDockSize.cy >= nAmount)
						{
						if (bApply)
							pBar->m_VertDockSize.cy -= nAmount;
						nAmount = 0;
						break;
						}
					else
						{
						nAmount -= pBar->m_VertDockSize.cy;
						if (bApply)
							pBar->m_VertDockSize.cy = 0;
						}
					}
				
				}
			}
		nPos++;
		}
    // We've reached the end of the row. If we still have size left to find, the only way we can do it is if there
    // is a flexble area at the end of the control bars..
    if (nAmount > 0 && pLastBar != NULL)
		{
        int nSub;
        CRect rect;
        pLastBar->GetWindowRect(&rect);
        ScreenToClient(&rect);
        if (IsBarHorizontal())
            nSub = m_LayoutSize.cx - rect.right;
        else
            nSub = m_LayoutSize.cy - rect.bottom;
        nAmount -= __min(__max( 0, nSub), nAmount);
		}
	
    // return no of flexible components encountered (if pointer supplied)
    if (pnFlexBars != NULL)
        *pnFlexBars = nFlexBars;
	
    // return amount allocated
    return (nOrigAmount - nAmount);
	}



// returns the first bar in the array - NULL if none
// used by the simplistic floating size routine
COXSizeControlBar* COXSizeDockBar::GetFirstControlBar()
{
    // CMiniDockFrameWnd assumes that if there's only one bar, then it's at position 1
    // in the array
    // need to make a check for 0 sized array however
	if (m_arrBars.GetSize() > 1)
		return ((COXSizeControlBar*)GetDockedControlBar(1));
	else
		return NULL;
}


// returns TRUE if a CControlBar in the row is sizeable;
// intended to determine if the row should contain a splitter or not
BOOL COXSizeDockBar::IsRowSizeable(int nPos)
{
    ASSERT(nPos >= 0 && nPos < m_arrBars.GetSize());
    while (nPos < m_arrBars.GetSize())
	{
		if (m_arrBars[nPos] == NULL)
			break;
		CControlBar* pBar = GetDockedControlBar(nPos);
        if (pBar!= NULL && ::IsWindow(pBar->GetSafeHwnd()) && 
			IsSizeable(pBar)  && pBar->IsVisible())
			return TRUE;
        nPos++;
	}
    return FALSE;
}


// Essentially the same as CDockBar::Insert(). Returns the position in the 
// bar array that the object will be inserted.
// nPos = 0 => before first position... But will have to check if this dockbar
// is the same as the present one...(perhaps)
int COXSizeDockBar::TestInsertPosition(CControlBar* pBarIns, CRect rect)
	{
    CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
	// hang-on: Don't we want to work in client co-ords ???
	ScreenToClient(&ptMid);
	
	ASSERT_VALID(this);
	ASSERT(pBarIns != NULL);
	UNUSED(pBarIns);
	
	int nPos = 0;
	int nPosInsAfter = 0;
	int nWidth = 0;
	int nTotalWidth = 0;
	BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ ? TRUE : FALSE;
	
	for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
		{
		void* pVoid = m_arrBars[nPos];
		CControlBar* pBar = GetDockedControlBar(nPos);
		
		if (pVoid == NULL)
			{
			nTotalWidth += nWidth - afxData.cyBorder2;
			nWidth = 0;
			if ((bHorz ? ptMid.y : ptMid.x) < nTotalWidth)
				{
				if (nPos == 0) // ie in first section....
					return 0;  // indicate before first position....

				return nPosInsAfter+1;
				}
			nPosInsAfter = nPos;
			}
		else
			if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && pBar->IsVisible())	
			{
			CRect rectBar;
			pBar->GetWindowRect(&rectBar);
			ScreenToClient(&rectBar);
			nWidth = __max(nWidth,
				bHorz ? rectBar.Size().cy : rectBar.Size().cx - 1);
			//if (bHorz ? rect.left > rectBar.left : rect.top > rectBar.top)
			// don't need above test - only interested if it should go on the row or not...
			nPosInsAfter = nPos;
			}
		}
	
	return nPosInsAfter+1;
	}


// returns no of bars that will be in the row (excluding the one to be inserted)
int COXSizeDockBar::BarsOnThisRow(CControlBar* pBarIns, CRect rect)
	{
	int nPos = TestInsertPosition(pBarIns, rect);
	
	// if inserting before the first row, or after the last row, then return 0
	// (there are no bars on this row).
	if (nPos == 0 ||nPos > m_arrBars.GetUpperBound())		// case if inserting before first bar in the array.
		return 0;		// return 0 to use the full size
	
	// go back to start of row.
	while (nPos != 0 && m_arrBars[nPos - 1] != 0)
		nPos --;
	
	int nCount = 0;
	while (TRUE)
		{
		void* pVoid = m_arrBars[nPos];
		CControlBar* pBar = GetDockedControlBar(nPos);
		if (pVoid == NULL)
			break;
		if (pBar!=NULL && ::IsWindow(pBar->GetSafeHwnd()) && pBar != pBarIns) 
			nCount++;
		nPos++;
		}
	return nCount;
	}


void COXSizeDockBar::ResizeBar(COXSizeControlBar* pBar, const BOOL bMaximize)
{
	ASSERT(pBar!=NULL);
	ASSERT(pBar->CanResize());

	CRect rect;
	pBar->GetWindowRect(rect);
	int nPos=TestInsertPosition(pBar,rect);
	// go back to start of row.
	while (nPos != 0 && m_arrBars[nPos - 1] != 0)
		nPos --;

	// Defines whether we should save the sizes of the other bars in the 
	// row/column or not
	//
	BOOL bSaveSize=TRUE;
	if(bMaximize)
	{
		int nPosCopy=nPos;
		while(TRUE)
		{
			void* pVoid = m_arrBars[nPos];    
			if (pVoid == NULL)
				break;          // end of the row, stop
			CControlBar* pControlBar = GetDockedControlBar(nPos); 

			if(pBar!=pControlBar && pControlBar!=NULL && 
				pControlBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)) && 
				::IsWindow(pControlBar->GetSafeHwnd()) && 
				IsSizeable(pControlBar) && pControlBar->IsVisible())
			{
				COXSizeControlBar* pSizeControlBar=(COXSizeControlBar*)pControlBar; 
				if(pSizeControlBar->IsMaximized())
				{
					bSaveSize=FALSE;
					break;
				}
			}
		    nPos++;
		}
		nPos=nPosCopy;
	}
	////////////////////////////////////////////////////////////////////////


    BOOL bHorz = IsBarHorizontal();
	
    int nMargin=0;

    while(TRUE)
	{
		void* pVoid = m_arrBars[nPos];    
		if (pVoid == NULL)
			break;          // end of the row, stop

		CControlBar* pControlBar = GetDockedControlBar(nPos); 

        if(pBar!=pControlBar && pControlBar!=NULL && 
			pControlBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)) && 
			::IsWindow(pControlBar->GetSafeHwnd()) && 
			IsSizeable(pControlBar) && pControlBar->IsVisible())
		{
			COXSizeControlBar* pSizeControlBar=(COXSizeControlBar*)pControlBar; 
            if(bHorz)
			{
				if(bMaximize)
				{
					if(bSaveSize)
					{
						pSizeControlBar->m_SavedDockSize=
							pSizeControlBar->m_HorzDockSize;
					}

					nMargin+=pSizeControlBar->m_HorzDockSize.cx-
						2*ID_CONTAINER_GAP-ID_BUTTON_SIDE;
					pSizeControlBar->m_HorzDockSize.cx=2*ID_CONTAINER_GAP+
						ID_BUTTON_SIDE;
				}
				else
				{
					pSizeControlBar->m_HorzDockSize=
						pSizeControlBar->m_SavedDockSize;
				}
			}
            else
			{
				if(bMaximize)
				{
					if(bSaveSize)
					{
						pSizeControlBar->m_SavedDockSize=
							pSizeControlBar->m_VertDockSize;
					}

					nMargin+=pSizeControlBar->m_VertDockSize.cy-
						2*ID_CONTAINER_GAP-ID_BUTTON_SIDE;
			        pSizeControlBar->m_VertDockSize.cy=2*ID_CONTAINER_GAP+
						ID_BUTTON_SIDE;
				}
				else
				{
					pSizeControlBar->m_VertDockSize=
						pSizeControlBar->m_SavedDockSize;
				}
			}
			pSizeControlBar->SetMaximized(FALSE);
		}
        nPos++;
	}

	if(bMaximize)
	{
		if(bHorz)
		{
			if(bSaveSize)
				pBar->m_SavedDockSize=pBar->m_HorzDockSize;
	        pBar->m_HorzDockSize.cx+=nMargin;
		}
		else
		{
			if(bSaveSize)
				pBar->m_SavedDockSize=pBar->m_VertDockSize;
			pBar->m_VertDockSize.cy+=nMargin;
		}
	}
	else
	{
		if(bHorz)
		{
			pBar->m_HorzDockSize=pBar->m_SavedDockSize;
		}
		else
		{
			pBar->m_VertDockSize=pBar->m_SavedDockSize;
		}
	}

	GetParentFrame()->RecalcLayout();

	pBar->SetMaximized(bMaximize);
}



// Helper functions
int FindInArray(const CPtrArray& arrBars, int nStartIndex, void* pFind)
	{
	while (nStartIndex < arrBars.GetUpperBound())	
		{
		if (arrBars[nStartIndex] == pFind)
			return nStartIndex;
		if (arrBars[nStartIndex] == NULL)
			break;		
		nStartIndex++;
		}
	return -1;
	}

void* FindInArray(void* pFindId, void** pArray)
	{
	while (*pArray != NULL)
		{
		if (*pArray == pFindId)
			return pArray;
		pArray++;
		}
	return NULL;
	}	


#ifdef _DEBUG
// DEBUG only helper function  
CString GetBarTitles(const CPtrArray& arrBars, int nPos)  
	{
	CString strMsg, strTitle;
	while (arrBars[nPos] != 0)
		{
		CControlBar* pBar = ::GetDockedControlBar(nPos, arrBars);
		pBar->GetWindowText(strTitle);
		strMsg += strTitle;
		strMsg += ",";
		nPos ++;
		}
	return strMsg;	
	}
#endif

