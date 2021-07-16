// ========================================================================================
// 							Class Implementation : 
//		COXFrameWndSizeDock & COXMDIFrameWndSizeDock & COXMDIChildWndSizeDock
// ========================================================================================

// Source file : OXFrameWndDock.cpp

// Copyright © Dundas Software Ltd. 1997 - 1998, All Rights Reserved
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"				// standard MFC include
#include "OXFrameWndDock.h"		// this class specification

#include "OXSzMiniDockFrmWnd.h"	// used class specification
#include "OXSizeDockBar.h"		// used class specification
#include "OXMDIFloatWnd.h"		// used class specification

#ifndef __OXMFCIMPL_H__
#include <..\src\afximpl.h>
#define __OXMFCIMPL_H__
#endif

#include "..\src\oleimpl2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#define REG_VERSION    		_T("Version")
#define REG_VERSION_NO		2		// current version of docking state

#ifdef _DEBUG
//#define _DEBUG_WNDPOS			// provide debug info on the window positioning algorithm
#endif

// dwMRCDockBarMap - table mapping standard ID's to styles
// Exists in MFC30.DLL, but not exported, so have to code it here
// renamed it 'cos there seems to be a difference between MFC4.0 and 4.1
static const DWORD dwMRCDockBarMap[4][2] =
	{
        { AFX_IDW_DOCKBAR_TOP,      CBRS_TOP    },
        { AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
        { AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
        { AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
	};

/////////////////////////////////////////////////////////////////////////////
// COXFrameWndSizeDock

IMPLEMENT_DYNCREATE(COXFrameWndSizeDock, CFrameWnd)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXFrameWndSizeDock::COXFrameWndSizeDock()
	{
	}

COXFrameWndSizeDock::~COXFrameWndSizeDock()
	{
	COXSizeControlBar::TidyUp();		// tidy up any outstanding control bars...
#ifdef _DEBUG
	CObArray arrWnd;
	GetFloatingBars(arrWnd);  // debug code to see what's still around !
#endif
	}


BEGIN_MESSAGE_MAP(COXFrameWndSizeDock, CFrameWnd)
//{{AFX_MSG_MAP(COXFrameWndSizeDock)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// dock bars will be created in the order specified by dwMRCDockBarMap
// this also controls which gets priority during layout
// this order can be changed by calling EnableDocking repetitively
// with the exact order of priority

// This is over-ridden primarily because we need to insert our own CDockBar class
// to handle the recalc layout, and this is the place they are created.
void COXFrameWndSizeDock::EnableDocking(DWORD dwDockStyle)
	{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
	
    m_pFloatingFrameClass = RUNTIME_CLASS(COXSizableMiniDockFrameWnd); // protected member
    for (int i = 0; i < 4; i++)
		{
		if (dwMRCDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)          // protected
			{
			CDockBar* pDock = (CDockBar*)GetControlBar(dwMRCDockBarMap[i][0]);
            if (pDock == NULL)
				{
				pDock = new COXSizeDockBar;
				if (!pDock->Create(this,
					WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE |
					dwMRCDockBarMap[i][1], dwMRCDockBarMap[i][0]))
                    {
					AfxThrowResourceException();
                    }
                }
			}
		}
	}

// although this is not a virtual function in MFC 4.0 and we can't guarantee the override
void COXFrameWndSizeDock::FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle)
	{
	CFrameWnd::FloatControlBar(pBar, point, dwStyle);
	pBar->SendMessage(WM_USER_AFTERFLOAT_MSG);		// force update of float style
	}

void COXFrameWndSizeDock::ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay)
	{
	CFrameWnd::ShowControlBar(pBar, bShow, bDelay);
	}


// Tiles the bars docked in the specified orientation
void COXFrameWndSizeDock::TileDockedBars(DWORD dwDockStyle)
	{
	for (int i = 0; i < 4; i++)
		{
		if (dwMRCDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)  //protected
			{
			COXSizeDockBar* pDock = (COXSizeDockBar*)GetControlBar(dwMRCDockBarMap[i][0]);
            // ASSERT(pDock == NULL || pDock->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
            if (pDock != NULL && (pDock->m_dwStyle && dwDockStyle == dwDockStyle))
				{
				pDock->TileDockedBars();
				}
			}
		}
	}


void COXFrameWndSizeDock::ArrangeFloatingBars(DWORD dwOrient)
	{
	CObArray arrWnd;
	GetFloatingBars(arrWnd);
	ArrangeWindows(arrWnd, dwOrient);
	}

void COXMDIFrameWndSizeDock::ArrangeFloatingBars(DWORD dwOrient)
	{
	CObArray arrWnd;
	GetFloatingBars(arrWnd);
	ASSERT (this->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	ASSERT(m_hWndMDIClient != NULL);
	// Use the MDI Client window - not the normal client area
	CWnd* pMDIClientWnd = CWnd::FromHandle(m_hWndMDIClient);
	ArrangeWindowsInWindow(pMDIClientWnd, arrWnd, dwOrient);
	
	// clear all the MOVED flags for sizeable windows...
	for (int i = arrWnd.GetUpperBound(); i >= 0; i--)
		{
        COXSizableMiniDockFrameWnd* pFloatFrame = (COXSizableMiniDockFrameWnd*)arrWnd[i]; 
        
		ASSERT(pFloatFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)));
		pFloatFrame->ModifyStyle(CBRS_MOVED_BY_USER, 0);
		}
	}


void COXMDIFrameWndSizeDock::ArrangeWindows(CObArray& arrWnd, DWORD dwOrient)
	{
	ASSERT (this->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	ASSERT(m_hWndMDIClient != NULL);
	// Use the MDI Client window - not the normal client area
	CWnd* pMDIClientWnd = CWnd::FromHandle(m_hWndMDIClient);
	ArrangeWindowsInWindow (pMDIClientWnd, arrWnd, dwOrient);
	}



void COXFrameWndSizeDock::ArrangeWindows(CObArray& arrWnd, DWORD dwOrient)
	{
	ArrangeWindowsInWindow (this, arrWnd, dwOrient);
	}


// Appends the floating bars, visible bars to an array
void COXFrameWndSizeDock::GetFloatingBars(CObArray& arrWnd)
	{
	CPtrList& listControlBars = m_listControlBars;
	
	POSITION pos = listControlBars.GetHeadPosition();
	while (pos != NULL)
		{
		CControlBar* pBar = (CControlBar*)listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if (!pBar->IsDockBar() && pBar->IsFloating() && pBar->IsVisible()) 	// not a dockbar and floating....
			{
			ASSERT(pBar->m_pDockBar != NULL);
			CWnd* pFloatFrame = ((CWnd*)pBar->m_pDockBar)->GetParent(); 
			ASSERT(pBar != NULL);
			arrWnd.Add(pFloatFrame);
			}
		}
	}


// Destroys the dynamic bars in an application
void COXFrameWndSizeDock::DestroyDynamicBars()
	{
	CPtrList& listControlBars = m_listControlBars;
	
	CObArray arrBars;
	COXSizeControlBar* pBar;
	
	// pass through the list and build an array of bars to destroy
	POSITION pos = listControlBars.GetHeadPosition();
	while (pos != NULL)
		{
		pBar = (COXSizeControlBar*)listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if (pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)) &&
			(pBar->m_Style & SZBARF_AUTOTIDY) == SZBARF_AUTOTIDY)
			arrBars.Add(pBar);
		}
	
	// now destroy these bars...
	for (int i = arrBars.GetUpperBound(); i >= 0; i--)
		{
		pBar = (COXSizeControlBar*)arrBars[i];
		pBar->DestroyWindow();
		}
	}


struct BarSizeSaveInfo 
	{
    CSize   FloatSize;          // floating size
	CSize	HorzDockSize;       // size when docked horizontally
    CSize   VertDockSize;       // size when docked vertically
	BOOL	bMDIFloating;		// floating in an MDI child window
	};


void COXFrameWndSizeDock::SaveSizeBarState(LPCTSTR pszProfileName)
	{
	DestroyDynamicBars();			// remove bars allocated dynamically
	// - we reload these at present
	
	CFrameWnd::SaveBarState(pszProfileName);	// save the raw states
#ifdef _VERBOSE_TRACE
	TRACE0("Loading Bar Sizes\n");
#endif
	SaveBarSizes(pszProfileName, TRUE);			// save additional info
	AfxGetApp()->WriteProfileInt(pszProfileName, REG_VERSION, REG_VERSION_NO);
	}



void COXFrameWndSizeDock::LoadSizeBarState(LPCTSTR pszProfileName)
{
	// check the registry version. If it doesn't match, do nothing...
	// this prevents us trying to load registry info from a previous version, that
	// is unlikely to make sense to the code below.
	if (AfxGetApp()->GetProfileInt(pszProfileName, REG_VERSION, 0) != 
		REG_VERSION_NO)
	{
		WriteProfileString(pszProfileName, NULL, NULL);		// this deletes this key from the registry
		return;
	}

	RecalcLayout();
	UpdateWindow();

	////////
	// remove TBSTYLE_FLAT style out of CToolBar
	CDockState state;
	state.LoadState(pszProfileName);

//	m_sizeLogical.cx=0;
//	m_sizeLogical.cy=0;

	CObArray arrFlatBars;
	for (int nIndex=0; nIndex<state.m_arrBarInfo.GetSize(); nIndex++)
	{
		CControlBarInfo* pInfo=(CControlBarInfo*)state.m_arrBarInfo[nIndex];
		ASSERT(pInfo!=NULL);
		pInfo->m_pBar=GetControlBar(pInfo->m_nBarID);
		if (pInfo->m_pBar!=NULL)
		{
			if(pInfo->m_pBar->IsKindOf(RUNTIME_CLASS(CToolBar)) && 
				pInfo->m_pBar->GetStyle()&TBSTYLE_FLAT)
			{
					arrFlatBars.Add((CObject*)pInfo->m_pBar);
					pInfo->m_pBar->ModifyStyle(TBSTYLE_FLAT,0);
			}
		}
	}
	////////

	SetDockState(state);

	////////
	// set TBSTYLE_FLAT style for CToolBar
	for (nIndex=0; nIndex<arrFlatBars.GetSize(); nIndex++)
	{
		((CToolBar*)arrFlatBars.GetAt(nIndex))->ModifyStyle(0,TBSTYLE_FLAT);
	}
	////////

#ifdef _VERBOSE_TRACE
	TRACE(_T("Loading Bar Sizes\n"));
#endif
	SaveBarSizes(pszProfileName, FALSE);		// load the sizes back

	// Clear the dockbars' hidden lists to prevent interference with recalc layout
	for (int i = 0; i < 4; i++)
	{
		COXSizeDockBar* pDock = 
			(COXSizeDockBar*)GetControlBar(dwMRCDockBarMap[i][0]);
		if (pDock != NULL)
		{
			ASSERT(pDock->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
			pDock->m_arrHiddenBars.RemoveAll();
		}
	}

	RecalcLayout();
	UpdateWindow();
}


// Saves all the sizeable bars info
// uses the "ID" of the bar as a key. The bar will already exist on a 
// load, so this seems safe enough
void COXFrameWndSizeDock::SaveBarSizes(LPCTSTR pszSection, BOOL bSave)
{
	struct BarSizeSaveInfo BSI;
	COXSizeControlBar* pBar;
	TCHAR szBarId[20] = _T("BarSize_");

	CPtrArray arrFloatingBars;

	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		pBar = (COXSizeControlBar*)m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);

		if (pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)))
		{
			UINT nID = pBar->GetDlgCtrlID();
			_itot(nID, szBarId + 8, 10);
			
			if (bSave)
			{
				BSI.VertDockSize 	= pBar->m_VertDockSize;
				BSI.HorzDockSize	= pBar->m_HorzDockSize;
				BSI.FloatSize 		= pBar->m_FloatSize;
				BSI.bMDIFloating = FALSE;
				// if floating in a MDI Float window.
				CFrameWnd* pBarFrame = pBar->GetDockingFrame();
				if (pBarFrame != NULL && 
					pBarFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
				{
					ASSERT(pBar->IsFloating());
					BSI.bMDIFloating = TRUE;
				}

				AfxGetApp()->WriteProfileBinary(pszSection, szBarId, 
					(LPBYTE)&BSI, sizeof BSI);
			}
			else
			{
				BarSizeSaveInfo* pBSI;
				UINT nBytesRead;
				if (AfxGetApp()->GetProfileBinary(pszSection, szBarId, 
					(LPBYTE*)&pBSI, &nBytesRead))
				{
					pBar->m_VertDockSize	= pBSI->VertDockSize;
					pBar->m_HorzDockSize	= pBSI->HorzDockSize;
					pBar->m_FloatSize 		= pBSI->FloatSize;
					
					// Now have to set the actual window size. The reason for 
					// this is that the Adjustment algorithm looks at actual 
					// window rect sizes, so it doesn't have to worry about 
					// borders etc.
					CSize NewSize = pBar->CalcFixedLayout(FALSE, 
						(pBar->m_dwStyle & CBRS_ORIENT_HORZ) != 0);
					pBar->SetWindowPos(0, 0, 0, NewSize.cx, NewSize.cy, 
						SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOZORDER|SWP_NOMOVE);
					if (pBar->IsFloating())
					{
						if (pBSI->bMDIFloating)	// floating in an MDI frame - do the float
						{
							// have to cast to COXMDIFrameWndSizeDock - as this is a CFrameWnd function
							ASSERT(this->IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock)));
							arrFloatingBars.Add(pBar);
						}
						else
						{
							CFrameWnd* pFrame = pBar->GetParentFrame();
							if (pFrame != NULL)
							{
								// Clear the dockbars' hidden lists to prevent 
								// interference with recalc layout
								for (int i = 0; i < 4; i++)
								{
									COXSizeDockBar* pDock = 
										(COXSizeDockBar*)GetControlBar(dwMRCDockBarMap[i][0]);
									if(pDock!=NULL)
									{
										ASSERT(pDock->
											IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
										pDock->m_arrHiddenBars.RemoveAll();
									}
								}
								pFrame->RecalcLayout();
							}
						}
					}

					delete [](LPBYTE)pBSI;
				}
#ifdef _VERBOSE_TRACE
				TRACE(_T("Bar ID=%d, Floating(%d,%d), HorzDocked(%d,%d), VertDocked(%d.%d)\n"),
					nID,BSI.FloatSize.cx,	BSI.FloatSize.cy,
					BSI.VertDockSize.cx, BSI.VertDockSize.cy,
					BSI.HorzDockSize.cx, BSI.HorzDockSize.cy);
#endif

			}
			
#ifdef _VERBOSE_TRACE
			CString strTitle;
			pBar->GetWindowText(strTitle);
			TRACE(_T("%s '%s' ID=%d Float(%d,%d) Horz(%d,%d) Vert(%d,%d)\n"),
				LPCTSTR(pBar->GetRuntimeClass()->m_lpszClassName),
					LPCTSTR(strTitle), nID,
						pBar->m_FloatSize.cx, pBar->m_FloatSize.cy,
							pBar->m_HorzDockSize.cx,  pBar->m_HorzDockSize.cy,
								pBar->m_VertDockSize.cx,  pBar->m_VertDockSize.cy);
#endif
		}
	}

	// Clear the dockbars' hidden lists to prevent interference with recalc layout
	for (int i = 0; i < 4; i++)
	{
		COXSizeDockBar* pDock = (COXSizeDockBar*)GetControlBar(dwMRCDockBarMap[i][0]);
		if(pDock!=NULL)
		{
			ASSERT(pDock->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
			pDock->m_arrHiddenBars.RemoveAll();
		}
	}

	// recalc the layout - so we end up with a meaningful set of bars
	RecalcLayout();			

	if (!bSave)
	{
		for (int i = 0; i < arrFloatingBars.GetSize(); i++)
		{
			pBar = (COXSizeControlBar*)arrFloatingBars[i];
			ASSERT(pBar->m_pDockContext != NULL);
			((COXMDIFrameWndSizeDock*)this)->FloatControlBarInMDIChild(pBar, 
				pBar->m_pDockContext->m_ptMRUFloatPos, CBRS_ALIGN_TOP, 
				pBar->IsWindowVisible());
		}
	}
}


void COXFrameWndSizeDock::SetDockState(const CDockState& state)
{
	// first pass through barinfo's sets the m_pBar member correctly
	// creating floating frames if necessary
	for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_bFloating)
		{
			// need to create floating frame to match
			CMiniDockFrameWnd* pDockFrame = CreateFloatingFrame(
				pInfo->m_bHorz ? CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT);
			ASSERT(pDockFrame != NULL);
			CRect rect(pInfo->m_pointPos, CSize(10, 10));
			pDockFrame->CalcWindowRect(&rect);
			pDockFrame->SetWindowPos(NULL, rect.left, rect.top, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
			CDockBar* pDockBar =
				(CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
			ASSERT(pDockBar != NULL);
			ASSERT_KINDOF(CDockBar, pDockBar);
			pInfo->m_pBar = pDockBar;
		}
		else // regular dock bar or toolbar
		{
			pInfo->m_pBar = GetControlBar(pInfo->m_nBarID);
		}
		if(pInfo->m_pBar!=NULL)
			pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	}

	// the second pass will actually dock all of the control bars and
	//  set everything correctly
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_pBar != NULL)
			pInfo->m_pBar->SetBarInfo(pInfo, this);
	}

	// last pass shows all the floating windows that were previously shown
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		pInfo->m_pBar=GetControlBar(pInfo->m_nBarID);
		if(pInfo->m_pBar==NULL) //toolbar id's probably changed
		{
			TRACE(_T("Unable to restore bar state - id has probably changed\n"));
		}
		if (pInfo->m_pBar!=NULL && pInfo->m_bFloating)
		{
			CFrameWnd* pFrameWnd = pInfo->m_pBar->GetParentFrame();
			CDockBar* pDockBar = (CDockBar*)pInfo->m_pBar;
			ASSERT_KINDOF(CDockBar, pDockBar);
			if (pDockBar->GetDockedVisibleCount() > 0)
			{
				pFrameWnd->RecalcLayout();
				pFrameWnd->ShowWindow(SW_SHOWNA);
			}

		}
	}
	DelayRecalcLayout();
}

void COXFrameWndSizeDock::GetDockState(CDockState& state) const
{
	state.Clear(); //make sure dockstate is empty
	// get state info for each bar
	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		CControlBarInfo* pInfo = new CControlBarInfo;
		pBar->GetBarInfo(pInfo);
		state.m_arrBarInfo.Add(pInfo);
	}
}


// private class - don't want this exported !
class OX_CLASS_DECL CWndSpaceElt : public CObject
{
	DECLARE_DYNAMIC(CWndSpaceElt);
	
	public:
		WORD ht;
		WORD wd;
};

IMPLEMENT_DYNAMIC(CWndSpaceElt, CObject);


// helper function to find list position
void PositionInSpcList(CWnd* pWnd, CObList& SpcList, DWORD dwOrient,
					   CWnd* pParentWnd, CSize& ParentSize, HDWP hDwp)	
	{
	
	CRect WndRect;
	pWnd->GetWindowRect(&WndRect);		// external dimensions of the window
	CSize WndSize;
	WndSize = WndRect.Size();			// size of rectangle
   	POSITION pos;
	
#ifdef _DEBUG
	CString strTitle;
	pWnd->GetWindowText(strTitle);
#ifdef _DEBUG_WNDPOS
	TRACE(_T("Inserting Window: %s, cx = %d, cy = %d\n"), LPCTSTR(strTitle), WndSize.cx, WndSize.cy);
#endif
	pos = SpcList.GetHeadPosition();
	int nTotalHeightBefore = 0;
	while (pos != NULL)
		{
		CWndSpaceElt* pSpcElt = (CWndSpaceElt*)SpcList.GetNext(pos);
		ASSERT(pSpcElt != NULL);
		
#ifdef _DEBUG_WNDPOS
		TRACE(_T("    ht= %d w=%d\n"), pSpcElt->ht, pSpcElt->wd);
#endif
		nTotalHeightBefore += pSpcElt->ht;
		}

	ASSERT(nTotalHeightBefore == ParentSize.cy);		
#endif
	
	int nHt = WndSize.cy;			// height of window....
	int nWd = WndSize.cx;			// width of window (used below)
	int nHtLeft;
	
	int nCurY = 0;					// current Y position of scan
	int nMinX = 0xffff;				// minimum X position found so far;
	int nMinY = 0xffff;				// again should be ok...
	
	POSITION MinListPos = NULL;		// position in the list with this minimum X;
	
	pos = SpcList.GetHeadPosition();
	while (pos != NULL)
		{
		POSITION posCurrent = pos;
		CWndSpaceElt* pSpcElt = (CWndSpaceElt*)SpcList.GetNext(pos);
		ASSERT(pSpcElt != NULL);
		ASSERT_VALID(pSpcElt);
	
		// if we inserted in this position, what would the width be ? 
		// Set nHtLeft, ThisPosX accordingly....
		nHtLeft = nHt;
		int nThisX = 0;
		POSITION posLoop = posCurrent;
		while (posLoop != NULL) 
			{
			CWndSpaceElt* pLoopSpcElt = (CWndSpaceElt*)SpcList.GetNext(posLoop);
			nThisX = __max(nThisX, pLoopSpcElt->wd);
			if (nThisX > nMinX)
				break;		// give up if we're already beyond the current minimum
			nHtLeft -= pLoopSpcElt->ht;
			if (nHtLeft <= 0)
				{
				if (nThisX < nMinX)
					{
					nMinX = nThisX;
					MinListPos = posCurrent;	// acutually the position after the current index in the list
					nMinY = nCurY;
					}
				break;
				}
			}
		
		nCurY += pSpcElt->ht;	// update current Y position.
		}
	
	
	if (MinListPos == NULL || nMinX > ParentSize.cx)	// window wouldn't fit anywhere in the window cleanly, 
		{
#ifdef 	_DEBUG_WNDPOS
		TRACE(_T("No insert position found\n"));
#endif
		return;				// ignore this for now
		}
	
	ASSERT(MinListPos != NULL && nMinX < 0xffff);
	
	// work out the new position for the window
	// Might want to delay window positioning in future
	CPoint WndPt;
	WndPt.x = ((dwOrient & CBRS_ARRANGE_LEFT == CBRS_ARRANGE_LEFT) ?  nMinX : ParentSize.cx - nMinX - WndSize.cx);
	WndPt.y = ((dwOrient & CBRS_ARRANGE_TOP == CBRS_ARRANGE_TOP)  ?  nMinY : ParentSize.cy - nMinY - WndSize.cy);
	ASSERT(WndPt.y >= -1);
	
#ifdef _DEBUG_WNDPOS
	TRACE(_T("Positioning at: (%d, %d) nMinY=%d, nMinX=%d\n"), WndPt.x, WndPt.y, nMinY, nMinX);
#endif

	// if not child of requested window, convert co-ords to Screen
	if ((pWnd->GetStyle() & WS_POPUP == WS_POPUP) || pWnd->GetParent() != pParentWnd)
		pParentWnd->ClientToScreen(&WndPt);
	
	
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	// attempt to optimize by only moving windows that have changed position...
	if (rcWnd.TopLeft() != WndPt || rcWnd.Size() != WndSize)
		{
		if (hDwp == NULL)
			pWnd->SetWindowPos(NULL, WndPt.x, WndPt.y, WndSize.cx, WndSize.cy,
			SWP_NOSIZE | SWP_NOZORDER);
		else
			::DeferWindowPos(hDwp, pWnd->m_hWnd, NULL, WndPt.x,  WndPt.y, WndSize.cx, WndSize.cy,
			SWP_NOSIZE | SWP_NOZORDER);
		}
	
	// now update the SpcList.
	nHtLeft = nHt;
	ASSERT(nHt > 0);
	ASSERT(MinListPos != NULL);	// can't actually happen
	CWndSpaceElt * pSpcElt;
	POSITION InsertPos = NULL;
	while (pos != NULL)
		{
		POSITION Oldpos = pos;
		pSpcElt = (CWndSpaceElt*)SpcList.GetNext(pos);

		ASSERT_VALID(pSpcElt);
		if (pSpcElt->ht > nHtLeft)
			{		
			ASSERT(pSpcElt->ht >= nHtLeft);
			pSpcElt->ht = (WORD)(pSpcElt->ht - (WORD)nHtLeft);			
			nHtLeft = 0;
			InsertPos = Oldpos;		// position to insert before
			break;
			}
		nHtLeft -= pSpcElt->ht;
		
		CWndSpaceElt* pOldElt = (CWndSpaceElt*)SpcList.GetAt(Oldpos);
		ASSERT(pOldElt != NULL && pOldElt->IsKindOf(RUNTIME_CLASS(CWndSpaceElt)));
		SpcList.RemoveAt(Oldpos);				// remove that element
		
		ASSERT(pSpcElt != NULL && pSpcElt->IsKindOf(RUNTIME_CLASS(CWndSpaceElt)));
		delete pOldElt;
		}
	ASSERT(nHtLeft == 0);
	
	// should now be looking at the element we need to shrink...
	// NB: If pos = NULL then we removed to the end of the list...
	pSpcElt = new CWndSpaceElt;
	pSpcElt->wd = (WORD)(nMinX + nWd);
	pSpcElt->ht = (WORD)nHt;
	if (InsertPos == NULL)
		SpcList.AddTail(pSpcElt);
	else
		SpcList.InsertBefore(InsertPos, pSpcElt);

#ifdef _DEBUG
#ifdef _DEBUG_WNDPOS
	TRACE0("After insert:\n");
#endif
	pos = SpcList.GetHeadPosition();
	int nTotalHeightAfter = 0;
	while (pos != NULL)
		{
		CWndSpaceElt* pSpcElt = (CWndSpaceElt*)SpcList.GetNext(pos);
		ASSERT(pSpcElt != NULL);
		ASSERT_VALID(pSpcElt);
		nTotalHeightAfter += pSpcElt->ht;
#ifdef _DEBUG_WNDPOS
		TRACE2("    ht= %d w=%d\n", pSpcElt->ht, pSpcElt->wd);
#endif
		}
	
	ASSERT(nTotalHeightAfter == ParentSize.cy);		
	ASSERT(nTotalHeightBefore == nTotalHeightAfter);
#endif
	}

int __cdecl CompareWndRect(const void* elem1, const void* elem2)
	{
	CRect rect1;
	CRect rect2;
	CWnd* pWnd1 = *((CWnd**)elem1);
	CWnd* pWnd2 = *((CWnd**)elem2);
	pWnd1->GetWindowRect(&rect1);
	pWnd2->GetWindowRect(&rect2);
	// array will be sorted into increasing order, so want the larger rectangles first.
	CSize size1 = rect1.Size();
	CSize size2 = rect2.Size();
	return ((size2.cx * size2.cy) - (size1.cx * size1.cy));
	}	


// Arranges the windows within the rectangle of another window.
void ArrangeWindowsInWindow (CWnd* pParentWnd, CObArray& arrWnd, DWORD dwOrient)
	{
	if (arrWnd.GetSize() == 0)			// no windows to size.. do nothing
		return;

	CRect ClientRect;
	pParentWnd->GetClientRect(&ClientRect);
	
	CSize ParentSize = ClientRect.Size();
	if (ParentSize.cy == 0)
		return;							// no height => not much we can do	
	
	CObList SpcList;					// list used to keep track of window spacing
	
	// add initial Arrange rectangle to the list;	
	CWndSpaceElt* pSpcElt = new CWndSpaceElt;
	pSpcElt->wd = 0;
	pSpcElt->ht = (WORD)ClientRect.Height();
	SpcList.AddTail(pSpcElt); 
	
	// sort array of window positions by size so that we position the largest windows first.
	// this improves the results quite a bit
	CObject ** pArrData = arrWnd.GetData();
	ASSERT(pArrData != NULL);		// shouldn't be NULL as array is non-empty, but check anyway	
	qsort(pArrData, arrWnd.GetSize(), sizeof(CObject *), CompareWndRect); 	

	HDWP hDWP = BeginDeferWindowPos(arrWnd.GetSize());	   // defer window moves to save on refresh
	
	// iterate thru all the windows in the list looking for a position to put it
	for (int nWndNo = 0; nWndNo < arrWnd.GetSize(); nWndNo++)
		{
		CWnd* pWnd = (CWnd*)arrWnd[nWndNo];
		ASSERT(pWnd != NULL);
		ASSERT_VALID(pWnd);
		PositionInSpcList(pWnd, SpcList, dwOrient, pParentWnd, ParentSize, hDWP);
		} 
	
	if (hDWP != NULL)
		::EndDeferWindowPos(hDWP);		// move the windows
	
	// Remove elements from the SpcList;
	while (!SpcList.IsEmpty())
		{
		CWndSpaceElt* pElt = (CWndSpaceElt*)SpcList.GetTail();
		delete pElt;
		SpcList.RemoveTail();
		}
	}


////////////////////////////////////////////////////////////////////////////////
// Diagnostics
#ifdef _DEBUG
void COXFrameWndSizeDock::AssertValid() const
	{
	CFrameWnd::AssertValid();
	}

void COXFrameWndSizeDock::Dump(CDumpContext& dc) const
	{
	CFrameWnd::Dump(dc);
	}
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// COXMDIFrameWndSizeDock frame
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(COXMDIFrameWndSizeDock, CMDIFrameWnd)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXMDIFrameWndSizeDock::COXMDIFrameWndSizeDock()
{
	m_pActiveDockChild=NULL;		

	m_bBeingDestroyed=FALSE;
}

COXMDIFrameWndSizeDock::~COXMDIFrameWndSizeDock()
{
	COXSizeControlBar::TidyUp();		// tidy up any outstanding control bars...
#ifdef _DEBUG
	CObArray arrWnd;
	GetFloatingBars(arrWnd);  // debug code to see what's still around !
#endif
}


BEGIN_MESSAGE_MAP(COXMDIFrameWndSizeDock, CMDIFrameWnd)
//{{AFX_MSG_MAP(COXMDIFrameWndSizeDock)
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
END_MESSAGE_MAP()


void COXMDIFrameWndSizeDock::FloatControlBarInMDIChild(CControlBar* pBar, CPoint point, DWORD dwStyle, BOOL bVisible /*=TRUE*/)
// float a control bar in an MDI Child window
// pBar = bar to float
// point = position in screen co-ordinates
{
	ASSERT(pBar != NULL);
	
	// point is in screen co-ords - map to client
	::ScreenToClient(m_hWndMDIClient, &point);

	// clip to client MDI client rectangle - ensures it's going to be visible
	CRect rcCtrlBar;
	pBar->GetClientRect(&rcCtrlBar);
	CRect rcMDIClient;
	::GetClientRect(m_hWndMDIClient, &rcMDIClient);
	point.x = __min(point.x, rcMDIClient.right - rcCtrlBar.Width());
	point.x = __max(point.x, rcMDIClient.left);		
	point.y = __min(point.y, rcMDIClient.bottom  - rcCtrlBar.Height());
	point.y = __max(point.y, rcMDIClient.top);		
	
	
	// If the bar is already floating in an MDI child, then just move it
	// MFC has a similar optimization for CMiniDockFrameWnd 
	if (pBar->m_pDockSite != NULL && pBar->m_pDockBar != NULL)
		{
		CDockBar* pDockBar = pBar->m_pDockBar;
		ASSERT(pDockBar->IsKindOf(RUNTIME_CLASS(CDockBar)));
		CFrameWnd* pDockFrame = (CFrameWnd*)pDockBar->GetParent();
		ASSERT(pDockFrame != NULL);
		ASSERT(pDockFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
		if (pDockFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
			{
			// already a floating as an MDI child, so just move it.
			if (pDockBar->m_bFloating && pDockBar->GetDockedCount() == 1 &&
				(dwStyle & pDockBar->m_dwStyle & CBRS_ALIGN_ANY) == CBRS_ALIGN_ANY)
				{
				pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0,
					SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
				return;
				}
			}
		}
	
	// Create a COXMDIFloatWnd, and dock the bar into it.
	COXMDIFloatWnd* pDockFrame = (COXMDIFloatWnd*)(RUNTIME_CLASS(COXMDIFloatWnd))->CreateObject();
	ASSERT(pDockFrame != NULL);
	if (!pDockFrame->Create(this, dwStyle))
		AfxThrowResourceException();
	
	pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	
	if (pDockFrame->m_hWndOwner == NULL)
		pDockFrame->m_hWndOwner = pBar->m_hWnd;
	
	// Gets the dockbar created by the COXMDIFloatWnd
	CDockBar* pDockBar = (CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	ASSERT(pDockBar != NULL);
	ASSERT(pDockBar->IsKindOf(RUNTIME_CLASS(CDockBar)));
	
	ASSERT(pBar->m_pDockSite == this);
	// if this assertion occurred it is because the parent of pBar was not
	//  initially this CFrameWnd when pBar's OnCreate was called
	// (this control bar should have been created with a different
	//  parent initially)
	
	pDockBar->DockControlBar(pBar);
	pDockFrame->RecalcLayout();

	if(bVisible)
	{
		pDockFrame->ShowWindow(SW_SHOWNA);
		pDockFrame->UpdateWindow(); 
	}
	
	::SendMessage(m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
}	

// removes the control bar from an MDI floating window, and then floats the bar
void COXMDIFrameWndSizeDock::UnFloatInMDIChild(CControlBar* pBar, CPoint point, DWORD dwStyle)
	{
	ASSERT(pBar != NULL);
	ASSERT(pBar->IsFloating());
	COXMDIFloatWnd * pFloatFrame = (COXMDIFloatWnd *)pBar->GetParentFrame();
	ASSERT(pFloatFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)));
	
	// point at which to float is ignored at present - use the co-ordinates of the current frame
	CRect rcMDIFloat;
	pFloatFrame->GetWindowRect(&rcMDIFloat);
	point = rcMDIFloat.TopLeft();

	// This is basically the code from MFC's CFrameWnd::FloatControlBar(), with the
	// test to avoid destroying/creating the floating frame window removed. 
	// Tried explicitly removing the control bar, but this doesn't work as it destroys the
	// CMDIFloatWnd, which in turn kills the child control bar. So need to create the floating
	// frame first, and then dock into this.  
	CMiniDockFrameWnd* pDockFrame = CreateFloatingFrame(dwStyle);
	ASSERT(pDockFrame != NULL);
	pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	if (pDockFrame->m_hWndOwner == NULL)
		pDockFrame->m_hWndOwner = pBar->m_hWnd;
	
	CDockBar* pDockBar = (CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	ASSERT(pDockBar != NULL);
	ASSERT(pDockBar->IsKindOf(RUNTIME_CLASS(CDockBar)));
	
	ASSERT(pBar->m_pDockSite == this);
	// if this assertion occurred it is because the parent of pBar was not
	//  initially this CFrameWnd when pBar's OnCreate was called
	// (this control bar should have been created with a different
	//  parent initially)
	
	pDockBar->DockControlBar(pBar);
	pDockFrame->RecalcLayout();
	pDockFrame->ShowWindow(SW_SHOWNA);
	pDockFrame->UpdateWindow();

	::SendMessage(m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
	}


// if control bar supplied, then set just resize it
void ForceLayoutAdjust(CControlBar* pBar)
	{
	CDockBar* pDockBar;
	ASSERT(pBar != NULL);
	pDockBar = pBar->m_pDockBar;
	if (pDockBar!= NULL && pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)))
		((COXSizeDockBar*)pDockBar)->m_CountBars = 0;			
	}

#ifdef _DEBUG
void COXMDIFrameWndSizeDock::AssertValid() const
	{
	CMDIFrameWnd::AssertValid();
	}

void COXMDIFrameWndSizeDock::Dump(CDumpContext& dc) const
	{
	CMDIFrameWnd::Dump(dc);
	dc << "\nCOXMDIFrameWndSizeDock - dockbars";
	}
#endif


/////////////////////////////////////////////////////////////

void COXMDIFrameWndSizeDock::HookActivation()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(m_hWndMDIClient!=NULL);

	CWnd* pActiveWnd=MDIGetActive();

	CWnd* pMDIClient=CWnd::FromHandle(m_hWndMDIClient);
	CWnd* pChild=pMDIClient->GetWindow(GW_CHILD);
	while(pChild!=NULL)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)))
		{
			BOOL bActive=(pActiveWnd==pChild) ? TRUE : FALSE;
			((COXMDIChildWndSizeDock*)pChild)->SendMessage(WM_NCACTIVATE,bActive);
		}
		pChild=pChild->GetNextWindow();
	}

	CView* pView;
	pView=((COXMDIChildWndSizeDock*)pActiveWnd)->GetActiveView();
	if(pView!=NULL)
	{
		BOOL bSetHack=FALSE;
		DWORD dwStyle=pView->GetStyle();
		CFrameWnd* pFrame=pView->GetParentFrame();
		if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) && 
			!((dwStyle&WS_VISIBLE)==0))
		{
			::SetWindowLong(pView->GetSafeHwnd(),GWL_STYLE,dwStyle&~WS_VISIBLE);
			if(pView->SetParent(this)!=NULL)
			{
				bSetHack=TRUE;
			}
		}
		
/*		if(pActiveWnd->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)) && 
		((COXMDIChildWndSizeDock*)pActiveWnd)->IsBeingDestroyed())
		{
			SetActiveView(NULL);
		}
		else
		{
			SetActiveView(pView);
		}
*/

//		SetActiveView(NULL);
		SetActiveView(pView);

		if(bSetHack)
		{
			pView->SetParent(&(((COXMDIChildWndSizeDock*)pActiveWnd)->m_dockBar));
			::SetWindowLong(pView->GetSafeHwnd(),GWL_STYLE,dwStyle);
		}
	}

/*	CView* pView=GetActiveView();
	if(pView!=NULL)
	{
		BOOL bSetHack=FALSE;
		DWORD dwStyle=pView->GetStyle();
		CFrameWnd* pFrame=pView->GetParentFrame();
		if(!((dwStyle&WS_VISIBLE)==0))
		{
			::SetWindowLong(pView->GetSafeHwnd(),GWL_STYLE,dwStyle&~WS_VISIBLE);
			if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
			{
				pView->SetParent(pFrame);
			}
			else
			{
				pView->SetParent(pActiveWnd);
			}
			bSetHack=TRUE;
		}
		
		((COXMDIChildWndSizeDock*)pActiveWnd)->SetActiveView(pView);
		SetActiveView(pView);

		if(bSetHack)
		{
			pView->SetParent(&(((COXMDIChildWndSizeDock*)pActiveWnd)->m_dockBar));
			::SetWindowLong(pView->GetSafeHwnd(),GWL_STYLE,dwStyle);
		}
	}*/

	OnUpdateFrameTitle(TRUE);
}

BOOL COXMDIFrameWndSizeDock::IsDockable(CWnd* pWnd)
{
	if(pWnd->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)))
	{
		return ((COXMDIChildWndSizeDock*)pWnd)->IsDockable();
	}
	return FALSE;
}

BOOL COXMDIFrameWndSizeDock::IsFrontChildMaximized()
{
	BOOL bMaximized=FALSE;
	CMDIChildWnd* pChild=CMDIFrameWnd::MDIGetActive(&bMaximized);
	bMaximized=pChild==NULL ? FALSE : bMaximized;
	return bMaximized;
}

void COXMDIFrameWndSizeDock::SetActiveChild(CWnd* pWnd)
{
	if(pWnd==NULL || !::IsWindow(pWnd->m_hWnd))
	{
		m_pActiveDockChild=NULL;		
	}
	else
	{
		ASSERT_KINDOF(COXMDIChildWndSizeDock,pWnd);
		m_pActiveDockChild=(COXMDIChildWndSizeDock*)pWnd;		
	}
}

COXMDIChildWndSizeDock* COXMDIFrameWndSizeDock::GetActiveChild() 
{ 
	if(m_pActiveDockChild!=NULL)
	{
		if(!::IsWindow(m_pActiveDockChild->m_hWnd))
		{
			m_pActiveDockChild=NULL;
		}
	}
	return m_pActiveDockChild; 
}

/////////////////////////////////////////////////////////////

void COXMDIFrameWndSizeDock::MDIActivate(CWnd* pWndActivate)
{ 
	ASSERT(::IsWindow(m_hWnd));

	if(IsDockable(pWndActivate))
	{
		SetActiveChild(pWndActivate);
		TRACE(_T("COXMDIFrameWndSizeDock::MDIActivate::Dockable\n"));
	}
	else
	{
		SetActiveChild(NULL);
		CMDIFrameWnd::MDIActivate(pWndActivate);
		TRACE(_T("COXMDIFrameWndSizeDock::MDIActivate::Non-Dockable\n"));
	}
	HookActivation();
}

void COXMDIFrameWndSizeDock::MDIMaximize(CWnd* pWnd)
{
	ASSERT(::IsWindow(m_hWnd));

	if(!IsDockable(pWnd))
	{
		CMDIFrameWnd::MDIMaximize(pWnd);
	}
}

void COXMDIFrameWndSizeDock::MDIRestore(CWnd* pWnd)
{ 
	ASSERT(::IsWindow(m_hWnd)); 

	if(!IsDockable(pWnd))
	{
		CMDIFrameWnd::MDIRestore(pWnd);
	}
}

CMDIChildWnd* COXMDIFrameWndSizeDock::MDIGetActive(BOOL* pbMaximized) const
{
	ASSERT(::IsWindow(m_hWndMDIClient));

	CMDIChildWnd* pActiveWnd;
	if(m_pActiveDockChild==NULL)
	{
		pActiveWnd=CMDIFrameWnd::MDIGetActive(pbMaximized);
	}
	else
	{
		if(pbMaximized!=NULL)
		{
			*pbMaximized=FALSE;
		}
		pActiveWnd=(CMDIChildWnd*)m_pActiveDockChild;
	}

	if(pActiveWnd!=NULL)
	{
		if(!::IsWindow(pActiveWnd->m_hWnd))
		{
			pActiveWnd=NULL;
		}
	}

	return pActiveWnd;
}

BOOL COXMDIFrameWndSizeDock::IsChild(const CWnd* pWnd)
{
	if(AfxGetMainWnd()==this && pWnd!=NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CFrameWnd* pFrame=pWnd->GetParentFrame();
		if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		{
			return TRUE;
		}
	}
	return CMDIFrameWnd::IsChild(pWnd);
}

BOOL COXMDIFrameWndSizeDock::OnBarCheck(UINT nID)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);

	if(CFrameWnd::OnBarCheck(nID))
	{
		CControlBar* pBar = GetControlBar(nID);
		if(pBar && pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)))
		{
			if(((COXSizeControlBar*)pBar)->m_pDockSite != NULL && 
				((COXSizeControlBar*)pBar)->m_pDockBar != NULL)
			{
				if(MDIGetActive() == pBar->GetParentFrame() && 
					!pBar->IsWindowVisible())
				{
					MDINext();
				}
				::SendMessage(m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
			}
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////

LRESULT COXMDIFrameWndSizeDock::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(nMsg==WM_SETTEXT)
	{
		CMDIChildWnd* pActiveChild=MDIGetActive();
		if(pActiveChild!=NULL && IsDockable(pActiveChild))
		{
			return CWnd::DefWindowProc(nMsg, wParam, lParam);
		}
	}

	return CMDIFrameWnd::DefWindowProc(nMsg, wParam, lParam);
}

BOOL COXMDIFrameWndSizeDock::PreTranslateMessage(MSG* pMsg)
{
	// check for special cancel modes for ComboBoxes
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
		AfxCancelModes(pMsg->hwnd);    // filter clicks

	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

#ifndef _AFX_NO_OLE_SUPPORT
	// allow hook to consume message
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnPreTranslateMessage(pMsg))
		return TRUE;
#endif

	CMDIChildWnd* pActiveChild = MDIGetActive();

	// current active child gets first crack at it
	if (pActiveChild != NULL && pActiveChild->PreTranslateMessage(pMsg))
		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators for frame and any children
		if (m_hAccelTable != NULL &&
			::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}

		// special processing for MDI accelerators last
		// and only if it is not in SDI mode (print preview)
		if (GetActiveView() == NULL)
		{
			if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
			{
				// the MDICLIENT window may translate it
				if (::TranslateMDISysAccel(m_hWndMDIClient, pMsg))
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL COXMDIFrameWndSizeDock::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	SetActiveView(NULL);
	TRACE(_T("MainWindow destroyed\n"));
	return CMDIFrameWnd::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// Special UI processing depending on current active child

void COXMDIFrameWndSizeDock::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

#ifndef _AFX_NO_OLE_SUPPORT
	// allow hook to set the title (used for OLE support)
	if (m_pNotifyHook != NULL && m_pNotifyHook->OnUpdateFrameTitle())
		return;
#endif

	CMDIChildWnd* pActiveChild=MDIGetActive();
	if(pActiveChild!=NULL && ::IsWindow(pActiveChild->m_hWnd))
	{
		CDocument* pDocument = GetActiveDocument();
		if(bAddToTitle && (pActiveChild->GetStyle() & WS_MAXIMIZE) == 0 && 
			(pDocument != NULL || 
			(pDocument = pActiveChild->GetActiveDocument()) != NULL))
		{
			UpdateFrameTitleForDocument(pDocument->GetTitle());
		}
		else
		{
			UpdateFrameTitleForDocument(NULL);
		}
	}
	else
	{
		UpdateFrameTitleForDocument(NULL);
	}
}

BOOL COXMDIFrameWndSizeDock::OnCmdMsg(UINT nID, int nCode, void* pExtra, 
									  AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Print Preview
	if (m_lpfnCloseProc != NULL)
		return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if (CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	SetActiveView(NULL);

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Smarts for updating the window menu based on the current child

void COXMDIFrameWndSizeDock::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	CMDIChildWnd* pActiveWnd = MDIGetActive();
	if (pActiveWnd != NULL)
	{
		// let child update the menu bar
		pActiveWnd->OnUpdateFrameMenu(TRUE, pActiveWnd, hMenuAlt);
	}
	else
	{
		// no child active, so have to update it ourselves
		//  (we can't send it to a child window, since pActiveWnd is NULL)
		if (hMenuAlt == NULL)
			hMenuAlt = m_hMenuDefault;  // use default
		::SendMessage(m_hWndMDIClient, WM_MDISETMENU, (WPARAM)hMenuAlt, NULL);
	}
}

CFrameWnd* COXMDIFrameWndSizeDock::GetActiveFrame()
{
	CMDIChildWnd* pActiveChild=NULL;
	pActiveChild = MDIGetActive();
	if (pActiveChild == NULL)
		return this;
	return pActiveChild;
}

/////////////////////////////////////////////////////////////////////

BOOL COXMDIFrameWndSizeDock::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// send to MDI child first - will be re-sent through OnCmdMsg later
	CMDIChildWnd* pActiveChild = MDIGetActive();
	
	if (pActiveChild != NULL && AfxCallWndProc(pActiveChild,
	  pActiveChild->m_hWnd, WM_COMMAND, wParam, lParam) != 0)
		return TRUE; // handled by child

	if (CFrameWnd::OnCommand(wParam, lParam))
		return TRUE; // handled through normal mechanism (MDI child or frame)

	HWND hWndCtrl = (HWND)lParam;

	ASSERT(AFX_IDM_FIRST_MDICHILD == 0xFF00);
	if (hWndCtrl == NULL && (LOWORD(wParam) & 0xf000) == 0xf000)
	{
		// menu or accelerator within range of MDI children
		// default frame proc will handle it
		DefWindowProc(WM_COMMAND, wParam, lParam);
		return TRUE;
	}

	return FALSE;   // not handled
}

/////////////////////////////////////////////////////////////////////////////
// Standard MDI Commands

// Two function for all standard MDI "Window" commands
void COXMDIFrameWndSizeDock::OnUpdateMDIWindowCmd(CCmdUI* pCmdUI)
{
	ASSERT(m_hWndMDIClient != NULL);
	pCmdUI->Enable(MDIGetActive() != NULL);
}

void COXMDIFrameWndSizeDock::OnWindowNew()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	CDocument* pDocument;
	if (pActiveChild == NULL ||
	  (pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE0("Warning: No active document for WindowNew command.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return;     // command failed
	}

	// otherwise we have a new frame !
	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDocument);
	HookActivation();
}

LRESULT COXMDIFrameWndSizeDock::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0 && IsTracking())
		lParam = HID_BASE_COMMAND+m_nIDTracking;

	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild != NULL && AfxCallWndProc(pActiveChild,
	  pActiveChild->m_hWnd, WM_COMMANDHELP, wParam, lParam) != 0)
	{
		// handled by child
		return TRUE;
	}

	if (CFrameWnd::OnCommandHelp(wParam, lParam))
	{
		// handled by our base
		return TRUE;
	}

	if (lParam != 0)
	{
		AfxGetApp()->WinHelp(lParam);
		return TRUE;
	}
	return FALSE;
}

void COXMDIFrameWndSizeDock::OnClose()
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	if(m_bBeingDestroyed)
	{
		return;
	}

	m_bBeingDestroyed=TRUE;


	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if(!pApp->SaveAllModified())
		{
			m_bBeingDestroyed=FALSE;
			return;     // don't close it
		}
		else
		{
			if(pApp->m_pDocManager != NULL)
			{
				POSITION pos=pApp->m_pDocManager->GetFirstDocTemplatePosition();
				while(pos!=NULL)
				{
					CDocTemplate* pTemplate=pApp->m_pDocManager->GetNextDocTemplate(pos);
					ASSERT_KINDOF(CDocTemplate, pTemplate);

					POSITION posDoc=pTemplate->GetFirstDocPosition();
					while (posDoc!=NULL)
					{
						CDocument* pDoc=pTemplate->GetNextDoc(posDoc);
						pDoc->SetModifiedFlag(FALSE);
					}
				}
			}
		}

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		UndockAllViews();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// then destroy the window
	DestroyWindow();
}

void COXMDIFrameWndSizeDock::OnActivate(UINT nState, CWnd* pWndOther, 
										BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

/*	CMDIChildWnd* pActiveWnd=MDIGetActive();
	if(pActiveWnd!=NULL && 
		pActiveWnd->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)))
	{
		if(::IsWindow(((COXMDIChildWndSizeDock*)pActiveWnd)->
			m_dockBar.GetSafeHwnd()) && 
			((COXMDIChildWndSizeDock*)pActiveWnd)->
			m_dockBar.IsWindowVisible())
		{
			if(nState==WA_INACTIVE)
			{
				CControlBar* pBar=&((COXMDIChildWndSizeDock*)pActiveWnd)->m_dockBar;
				if(pBar!=NULL)
				{
					ASSERT_KINDOF(CControlBar, pBar);
					ASSERT(pBar->m_pDockContext != NULL);
					if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
						((COXSizeViewBar*)pBar)->m_pView->SetFocus();
				}
			}
		}
	}*/
}

void COXMDIFrameWndSizeDock::UndockAllViews()
{
	CWnd* pMDIClient=CWnd::FromHandle(m_hWndMDIClient);
	CWnd* pChild=pMDIClient->GetWindow(GW_CHILD);
	while(pChild!=NULL)
	{
		if(IsDockable(pChild))
		{
			if(::IsWindow(((COXMDIChildWndSizeDock*)pChild)->m_dockBar.m_hWnd))
			{
				((COXMDIChildWndSizeDock*)pChild)->m_dockBar.DetachMDIChild(FALSE);
			}
		}
		pChild=pChild->GetNextWindow();
	}
}

void COXMDIFrameWndSizeDock::SaveSizeBarState(LPCTSTR pszProfileName)
{
	// remove bars allocated dynamically
	((COXFrameWndSizeDock*)this)->DestroyDynamicBars();			
	// - we reload these at present
	
	SaveBarState(pszProfileName);	// save the raw states
#ifdef _VERBOSE_TRACE
	TRACE0("Loading Bar Sizes\n");
#endif
	// save additional info
	((COXFrameWndSizeDock*)this)->SaveBarSizes(pszProfileName, TRUE);			
	AfxGetApp()->WriteProfileInt(pszProfileName, REG_VERSION, REG_VERSION_NO);
}

void COXMDIFrameWndSizeDock::SaveBarState(LPCTSTR lpszProfileName) const
{
	CDockState state;
	GetDockState(state);
	state.SaveState(lpszProfileName);
}

void COXMDIFrameWndSizeDock::GetDockState(CDockState& state) const
{
	state.Clear(); //make sure dockstate is empty
	// get state info for each bar
	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if(!pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			CControlBarInfo* pInfo = new CControlBarInfo;
			pBar->GetBarInfo(pInfo);
			state.m_arrBarInfo.Add(pInfo);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// COXMDIChildWndSizeDock frame
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifdef _OXIE4PATCH
UINT COXMDIChildWndSizeDock::m_nDockMessageID=WM_DOCKCHILDWND;
#else
UINT COXMDIChildWndSizeDock::m_nDockMessageID=RegisterWindowMessage(_T("_DOCK_MDICHILD_"));
#endif // _OXIE4PATCH

IMPLEMENT_DYNCREATE(COXMDIChildWndSizeDock, CMDIChildWnd)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXMDIChildWndSizeDock::COXMDIChildWndSizeDock()
{
	m_bDockable=FALSE;	
	m_sDockMenuItem=_T("Dock window");

	m_dwDockStyle=CBRS_ALIGN_ANY;

	m_bBeingDestroyed=FALSE;
}

COXMDIChildWndSizeDock::~COXMDIChildWndSizeDock()
{
}

BEGIN_MESSAGE_MAP(COXMDIChildWndSizeDock, CMDIChildWnd)
//{{AFX_MSG_MAP(COXMDIChildWndSizeDock)
	ON_WM_MDIACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
	ON_WM_PARENTNOTIFY()
	ON_WM_SYSCOMMAND()
	ON_WM_NCRBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_STYLECHANGING()
	ON_MESSAGE(WM_SETTEXT,OnSetText)
#ifdef _OXIE4PATCH
	ON_COMMAND(WM_DOCKCHILDWND,OnMakeItDockable)
#else
	ON_COMMAND(m_nDockMessageID,OnMakeItDockable)
#endif // _OXIE4PATCH
END_MESSAGE_MAP()

BOOL COXMDIChildWndSizeDock::MakeItDockable(BOOL bDockable)
{
	if(!IsDockableFrame())
	{
		return FALSE;
	}

	if(bDockable==m_bDockable)
	{
		return TRUE;
	}

	BOOL bResult;
	if(bDockable)
	{
		bResult=m_dockBar.AttachMDIChild(this);
	}
	else
	{
		ASSERT(::IsWindow(m_dockBar.m_hWnd));
		bResult=m_dockBar.DetachMDIChild();
		RecalcLayout();
	}

	m_bDockable=bDockable;	

	return bResult;
}

BOOL COXMDIChildWndSizeDock::IsDockableFrame()
{
	ASSERT(::IsWindow(m_hWnd));
	CMDIFrameWnd* pMDIFrame=GetMDIFrame();
	return pMDIFrame->IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock));
}

////////////////////////////////////////////////////////////

BOOL COXMDIChildWndSizeDock::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect, CMDIFrameWnd* pParentWnd,
	CCreateContext* pContext)
{
	if (pParentWnd == NULL)
	{
		CWnd* pMainWnd = AfxGetThread()->m_pMainWnd;
		ASSERT(pMainWnd != NULL);
		ASSERT_KINDOF(CMDIFrameWnd, pMainWnd);
		pParentWnd = (CMDIFrameWnd*)pMainWnd;
	}
	ASSERT(::IsWindow(pParentWnd->m_hWndMDIClient));

	// insure correct window positioning
	pParentWnd->RecalcLayout();

	// first copy into a CREATESTRUCT for PreCreate
	CREATESTRUCT cs;
	cs.dwExStyle = 0L;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = rect.left;
	cs.y = rect.top;
	cs.cx = rect.right - rect.left;
	cs.cy = rect.bottom - rect.top;
	cs.hwndParent = pParentWnd->m_hWnd;
	cs.hMenu = NULL;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = (LPVOID)pContext;

	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}
	// extended style must be zero for MDI Children (except under Win4)
	ASSERT(afxData.bWin4 || cs.dwExStyle == 0);
	ASSERT(cs.hwndParent == pParentWnd->m_hWnd);    // must not change

	// now copy into a MDICREATESTRUCT for real create
	MDICREATESTRUCT mcs;
	mcs.szClass = cs.lpszClass;
	mcs.szTitle = cs.lpszName;
	mcs.hOwner = cs.hInstance;
	mcs.x = cs.x;
	mcs.y = cs.y;
	mcs.cx = cs.cx;
	mcs.cy = cs.cy;
	mcs.style = cs.style & ~(WS_MAXIMIZE | WS_VISIBLE);
	mcs.lParam = (LONG)cs.lpCreateParams;

	// create the window through the MDICLIENT window
	AfxHookWindowCreate(this);
	HWND hWnd = (HWND)::SendMessage(pParentWnd->m_hWndMDIClient,
		WM_MDICREATE, 0, (LPARAM)&mcs);
	if (!AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if MDICREATE fails too soon

	if (hWnd == NULL)
		return FALSE;

	// special handling of visibility (always created invisible)
	if (cs.style & WS_VISIBLE)
	{
		// place the window on top in z-order before showing it
		::BringWindowToTop(hWnd);

		// show it as specified
		if (cs.style & WS_MINIMIZE)
			ShowWindow(SW_SHOWMINIMIZED);
		else if (cs.style & WS_MAXIMIZE)
			ShowWindow(SW_SHOWMAXIMIZED);
		else
			ShowWindow(SW_SHOWNORMAL);

		// make sure it is active (visibility == activation)
		if(IsDockableFrame())
		{
			((COXMDIFrameWndSizeDock*)pParentWnd)->MDIActivate(this);
		}
		else
		{
			pParentWnd->MDIActivate(this);
		}

		// refresh MDI Window menu
		::SendMessage(pParentWnd->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);
	}

#ifdef _MAC
	// force the size box to be visible
	ModifyStyleEx(0, WS_EX_FORCESIZEBOX);

	// if the window is already visible, we need to redraw the area covered by
	// the sizebox so that the sizebox is drawn
	if (IsWindowVisible())
	{
		RECT rc;
		GetClientRect(&rc);
		rc.left = rc.right - (GetSystemMetrics(SM_CXVSCROLL) - 1);
		rc.top = rc.bottom - (GetSystemMetrics(SM_CYHSCROLL) - 1);
		RedrawWindow(&rc, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}
#endif

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(m_bDockable ? MF_CHECKED : MF_UNCHECKED, 
			m_nDockMessageID, m_sDockMenuItem);

		ASSERT(hWnd == m_hWnd);
	}
	return TRUE;
}

////////////////////////////////////////////////////////////

CMDIChildWnd* COXMDIChildWndSizeDock::MDIGetActive(BOOL* pbMaximized)
{
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	CMDIChildWnd* pActiveWnd;
	if(IsDockableFrame())
	{
		pActiveWnd=((COXMDIFrameWndSizeDock*)pFrameWnd)->MDIGetActive(pbMaximized);
	}
	else
	{
		pActiveWnd=pFrameWnd->MDIGetActive(pbMaximized);
	}

	if(pActiveWnd!=NULL && !::IsWindow(pActiveWnd->m_hWnd))
	{
		pActiveWnd=NULL;
	}

	return pActiveWnd;
}

void COXMDIChildWndSizeDock::MDIDestroy()
{ 
	ASSERT(::IsWindow(m_hWnd));
	CMDIChildWnd::MDIDestroy();
}

void COXMDIChildWndSizeDock::MDIActivate()
{ 
	ASSERT(::IsWindow(m_hWnd));

	if(IsDockableFrame())
	{
		COXMDIFrameWndSizeDock* pMDIFrame=(COXMDIFrameWndSizeDock*)GetMDIFrame();
		pMDIFrame->MDIActivate(this);
	}
	else
	{
		CMDIChildWnd::MDIActivate();
	}
}

void COXMDIChildWndSizeDock::MDIMaximize()
{ 
	ASSERT(::IsWindow(m_hWnd)); 

	if(!IsDockable())
	{
		CMDIChildWnd::MDIMaximize();
	}
}

void COXMDIChildWndSizeDock::MDIRestore()
{ 
	ASSERT(::IsWindow(m_hWnd)); 

	if(!IsDockable())
	{
		CMDIChildWnd::MDIRestore();
	}
}

BOOL COXMDIChildWndSizeDock::OnCmdMsg(UINT nID, int nCode, void* pExtra,
									  AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(IsDockable())
	{
		if(::IsWindow(m_dockBar.m_hWnd))
		{
			if(m_dockBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		}
	}

	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void COXMDIChildWndSizeDock::ActivateFrame(int nCmdShow)
{
	BOOL bVisibleThen = (GetStyle() & WS_VISIBLE) != 0;
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	ASSERT_VALID(pFrameWnd);

	// determine default show command
	if (nCmdShow == -1)
	{
		// get maximized state of frame window (previously active child)
		BOOL bMaximized;
		if(IsDockableFrame())
		{
			((COXMDIFrameWndSizeDock*)pFrameWnd)->MDIGetActive(&bMaximized);
		}
		else
		{
			pFrameWnd->MDIGetActive(&bMaximized);
		}

		// convert show command based on current style
		DWORD dwStyle = GetStyle();
		if (bMaximized || (dwStyle & WS_MAXIMIZE))
			nCmdShow = SW_SHOWMAXIMIZED;
		else if (dwStyle & WS_MINIMIZE)
			nCmdShow = SW_SHOWMINIMIZED;
	}

	// finally, show the window
	CFrameWnd::ActivateFrame(nCmdShow);

	// update the Window menu to reflect new child window
	CMDIFrameWnd* pFrame = GetMDIFrame();
	::SendMessage(pFrame->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);

	// Note: Update the m_bPseudoInactive flag.  This is used to handle the
	//  last MDI child getting hidden.  Windows provides no way to deactivate
	//  an MDI child window.

	BOOL bVisibleNow = (GetStyle() & WS_VISIBLE) != 0;
	if (bVisibleNow == bVisibleThen)
		return;

	////////////////////////////////////
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// probably have to rewrite
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	////////////////////////////////////
	if (!bVisibleNow)
	{
		// get current active window according to Windows MDI
		HWND hWnd = (HWND)::SendMessage(pFrameWnd->m_hWndMDIClient,
			WM_MDIGETACTIVE, 0, 0);
		if (hWnd != m_hWnd)
		{
			// not active any more -- window must have been deactivated
			ASSERT(!m_bPseudoInactive);
			return;
		}

		// check next window
		ASSERT(hWnd != NULL);
		pFrameWnd->MDINext();

		// see if it has been deactivated now...
		hWnd = (HWND)::SendMessage(pFrameWnd->m_hWndMDIClient,
			WM_MDIGETACTIVE, 0, 0);
		if (hWnd == m_hWnd)
		{
			// still active -- fake deactivate it
			ASSERT(hWnd != NULL);
			OnMDIActivate(FALSE, NULL, this);
			m_bPseudoInactive = TRUE;   // so MDIGetActive returns NULL
		}
	}
	else if (m_bPseudoInactive)
	{
		// if state transitioned from not visible to visible, but
		//  was pseudo deactivated -- send activate notify now
		OnMDIActivate(TRUE, this, NULL);
		ASSERT(!m_bPseudoInactive); // should get set in OnMDIActivate!
	}
}

BOOL COXMDIChildWndSizeDock::PreTranslateMessage(MSG* pMsg)
{
	// check for special cancel modes for combo boxes
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_NCLBUTTONDOWN)
		AfxCancelModes(pMsg->hwnd);    // filter clicks

	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// we can't call 'CFrameWnd::PreTranslate' since it will translate
	//  accelerators in the context of the MDI Child - but since MDI Child
	//  windows don't have menus this doesn't work properly.  MDI Child
	//  accelerators must be translated in context of their MDI Frame.

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// use document specific accelerator table over m_hAccelTable
		HACCEL hAccel = GetDefaultAccelerator();
		return hAccel != NULL &&
		   ::TranslateAccelerator(GetMDIFrame()->m_hWnd, hAccel, pMsg);
	}
	return FALSE;
}

void COXMDIChildWndSizeDock::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==m_nDockMessageID)
	{
		OnMakeItDockable();
	}
	else
	{
		CMDIChildWnd::OnSysCommand(nID, lParam);
	}
}

BOOL COXMDIChildWndSizeDock::UpdateClientEdge(LPRECT lpRect)
{
	if (!afxData.bWin4)
		return FALSE;

	// only adjust for active MDI child window
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	CMDIChildWnd* pChild=MDIGetActive();
	if (pChild == NULL || pChild == this)
	{
		// need to adjust the client edge style as max/restore happens
		DWORD dwStyle = ::GetWindowLong(pFrameWnd->m_hWndMDIClient, GWL_EXSTYLE);
		DWORD dwNewStyle = dwStyle;
		if (pChild != NULL && !(GetExStyle() & WS_EX_CLIENTEDGE) &&
		  (GetStyle() & WS_MAXIMIZE))
			dwNewStyle &= ~(WS_EX_CLIENTEDGE);
		else
			dwNewStyle |= WS_EX_CLIENTEDGE;

		if (dwStyle != dwNewStyle)
		{
			// SetWindowPos will not move invalid bits
			::RedrawWindow(pFrameWnd->m_hWndMDIClient, NULL, NULL,
				RDW_INVALIDATE | RDW_ALLCHILDREN);

			// remove/add WS_EX_CLIENTEDGE to MDI client area
			::SetWindowLong(pFrameWnd->m_hWndMDIClient, GWL_EXSTYLE, dwNewStyle);
			::SetWindowPos(pFrameWnd->m_hWndMDIClient, NULL, 0, 0, 0, 0,
				SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE |
				SWP_NOZORDER | SWP_NOCOPYBITS);

			// return new client area
			if (lpRect != NULL)
				::GetClientRect(pFrameWnd->m_hWndMDIClient, lpRect);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXMDIChildWndSizeDock::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_hWnd == NULL)
		return FALSE;

	m_bBeingDestroyed=TRUE;

	if(::IsWindow(m_dockBar.m_hWnd) && !m_dockBar.IsBeingDestroyed())
	{
		m_dockBar.DestroyWindow();
	} 

	// avoid changing the caption during the destroy message(s)
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	HWND hWndFrame = pFrameWnd->m_hWnd;
	ASSERT(::IsWindow(hWndFrame));
	DWORD dwStyle = SetWindowLong(hWndFrame, GWL_STYLE,
		GetWindowLong(hWndFrame, GWL_STYLE) & ~FWS_ADDTOTITLE);

	ASSERT(m_pViewActive==NULL || ::IsWindow(m_pViewActive->m_hWnd));

	pFrameWnd->SetActiveView(NULL);
	MDIDestroy();

	if (::IsWindow(hWndFrame))
	{
		ASSERT(hWndFrame == pFrameWnd->m_hWnd);
		SetWindowLong(hWndFrame, GWL_STYLE, dwStyle);
		pFrameWnd->OnUpdateFrameTitle(TRUE);
	}

	TRACE(_T("ChildWindow destroyed\n"));
	return TRUE;
}

void COXMDIChildWndSizeDock::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, 
										   CWnd* pDeactivateWnd) 
{
	if(MDIGetActive()!=this)
	{
		MDIActivate();
		return;
	}

	CMDIChildWnd::OnMDIActivate(bActivate,pActivateWnd,pDeactivateWnd);
	
	// TODO: Add your message handler code here
	if(m_bDockable)
	{
		if(::IsWindow(m_dockBar.m_hWnd))
		{
			m_dockBar.SendMessage(WM_ACTIVATEVIEWBAR,bActivate,ID_SOURCE_MDICHILD);
		}
	}
}

BOOL COXMDIChildWndSizeDock::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default

	// I've got some doubts
	if(bActive && MDIGetActive()!=this)
	{
		return FALSE;
	}
	else
	{
		if(m_bDockable)
		{
			if(::IsWindow(m_dockBar.m_hWnd))
			{
				m_dockBar.SendMessage(WM_ACTIVATEVIEWBAR,bActive,ID_SOURCE_MDICHILD);
			}
		}
		else
		{
			return CMDIChildWnd::OnNcActivate(bActive);
		}
	}


	return CMDIChildWnd::OnNcActivate(bActive);
}

void COXMDIChildWndSizeDock::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	CMDIChildWnd::OnNcRButtonDown(nHitTest,point);

	if(nHitTest==HTCAPTION && GetSystemMenu(FALSE)==NULL)
	{
		OnContextMenu(this,point);
	}
}

int COXMDIChildWndSizeDock::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	int nResult = CFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
/*	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // frame does not want to activate
*/
	// activate this window if necessary
	CMDIFrameWnd* pFrameWnd = GetMDIFrame();
	ASSERT_VALID(pFrameWnd);
	CMDIChildWnd* pActive = MDIGetActive();
	if (pActive != this)
	{
		MDIActivate();
	}
	else
	{
		if(IsDockableFrame())
		{
			((COXMDIFrameWndSizeDock*)pFrameWnd)->HookActivation();
		}
	}

	return nResult;
}

void COXMDIChildWndSizeDock::OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	if(m_bDockable && nStyleType&GWL_STYLE && lpStyleStruct->styleNew&WS_VISIBLE)
	{
		lpStyleStruct->styleNew&=~WS_VISIBLE;
	}
}

LONG COXMDIChildWndSizeDock::OnSetText(UINT wParam, LONG lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_bDockable)
	{
		if(::IsWindow(m_dockBar.m_hWnd))
		{
			m_dockBar.SendMessage(WM_SETTEXT,wParam,lParam);
		}
	}
	return Default();
}


void COXMDIChildWndSizeDock::OnClose() 
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	if(m_bBeingDestroyed)
	{
		return;
	}

	m_bBeingDestroyed=TRUE;

	// Note: only queries the active document
	ASSERT(::IsWindow(m_pViewActive->m_hWnd));
	CDocument* pDocument = GetActiveDocument();
	if(pDocument != NULL)
	{
		if(!pDocument->CanCloseFrame(this))
		{
			// document can't close right now -- don't close it
			m_bBeingDestroyed=FALSE;
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if(pDocument!=NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// then destroy the window
	DestroyWindow();
}

void COXMDIChildWndSizeDock::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNREFERENCED_PARAMETER(pWnd);

	CMenu menu;
	if (menu.CreatePopupMenu())
	{
		OnAddContextMenuItems(menu.m_hMenu);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void COXMDIChildWndSizeDock::OnAddContextMenuItems(HMENU hMenu)
{
	CMenu Menu;
	Menu.Attach(hMenu);
	
	Menu.AppendMenu(m_bDockable ? MF_CHECKED : MF_UNCHECKED, 
		m_nDockMessageID, m_sDockMenuItem);

	Menu.Detach();	// detatch MFC object
}

void COXMDIChildWndSizeDock::OnMakeItDockable() 
{
	// TODO: Add your command handler code here
	MakeItDockable(!m_bDockable);
}

void COXMDIChildWndSizeDock::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// update our parent window first
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle && pDocument != NULL)
	{
		TCHAR szText[256+_MAX_PATH];
		lstrcpy(szText, pDocument->GetTitle());
		if (m_nWindow > 0)
			wsprintf(szText + lstrlen(szText), _T(":%d"), m_nWindow);

		// set title if changed, but don't remove completely
		AfxSetWindowText(m_hWnd, szText);

		if(m_bDockable)
		{
			if(::IsWindow(m_dockBar.m_hWnd))
			{
				m_dockBar.SetWindowText(szText);
			}
		}
	}
}

void COXMDIChildWndSizeDock::OnParentNotify(UINT message, LPARAM lParam)
{
	if(LOWORD(message)==WM_DESTROY)
	{
		CWnd* pWnd=FromHandle((HWND)lParam);
		if(pWnd->IsKindOf(RUNTIME_CLASS(CView)) && 
			!pWnd->IsKindOf(RUNTIME_CLASS(CPreviewView)))
		{
			SetActiveView(NULL);
			GetMDIFrame()->SetActiveView(NULL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// COXSizeViewBar frame
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(COXSizeViewBar, COXSizeControlBar)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXSizeViewBar::COXSizeViewBar(int nStyle)
	: COXSizeControlBar(nStyle)
{
	m_pChildWnd=NULL;
	m_pView=NULL;

	m_bBeingDestroyed=FALSE;
}

COXSizeViewBar::~COXSizeViewBar()
{
}


BEGIN_MESSAGE_MAP(COXSizeViewBar, COXSizeControlBar)
	//{{AFX_MSG_MAP(COXSizeViewBar)
	ON_WM_NCACTIVATE()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_ADDCONTEXTMENUITEMS, OnAddContextMenuItems)
	ON_MESSAGE(WM_ACTIVATEVIEWBAR,OnActivateViewBar)
END_MESSAGE_MAP()


void COXSizeViewBar::OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags)
{
	COXSizeControlBar::OnSizedOrDocked(cx, cy, bFloating, flags);

	if(::IsWindow(m_pChildWnd->m_hWnd) && m_pChildWnd->IsDockable())
	{
		ASSERT(::IsWindow(m_pView->m_hWnd));

		CRect rect;
		if(bFloating)
		{
			m_WrapperWnd.UnwrapView();
			GetDockingFrame()->RecalcLayout();
			GetClientRect(&rect);
		}
		else
		{
			m_WrapperWnd.WrapView(this,m_pView);
			m_WrapperWnd.GetClientRect(&rect);
		}
		m_pView->MoveWindow(&rect);

		if(m_pChildWnd==m_pChildWnd->MDIGetActive(&m_bMaximized) && m_pView!=NULL)
			m_pView->SetFocus();

	}
}

BOOL COXSizeViewBar::AttachMDIChild(COXMDIChildWndSizeDock* pWnd)
{
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)));
	m_pChildWnd=pWnd;
	m_pView=m_pChildWnd->GetActiveView();
	
	if(m_pView==NULL)
	{
		return FALSE;
	}

	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)m_pChildWnd->GetMDIFrame();
	ASSERT_KINDOF(CMDIFrameWnd, pFrame);

	BOOL bFirstTime=FALSE;
	if(!::IsWindow(m_hWnd))
	{
		if(!Create(pFrame,_T("")))   
		{
			return FALSE;
		}
		EnableDocking(m_pChildWnd->m_dwDockStyle);
		ShowWindow(SW_HIDE);

		bFirstTime=TRUE;
	}

	CString sWindowText;
	m_pChildWnd->GetWindowText(sWindowText);
	SetWindowText(sWindowText);

	CWnd* pPrevParent=m_pView->SetParent(this);
	ASSERT(pPrevParent!=NULL);

	m_bMaximized=FALSE;
	if(m_pChildWnd==m_pChildWnd->MDIGetActive(&m_bMaximized))
	{
		if(m_bMaximized && m_pChildWnd->IsDockableFrame())
		{
			((COXMDIFrameWndSizeDock*)pFrame)->MDINext();
			m_pChildWnd->ShowWindow(SW_HIDE);
			m_pChildWnd->MDIRestore();
			m_pChildWnd->ShowWindow(SW_HIDE);
		}

		BOOL bOldDockable=m_pChildWnd->m_bDockable;
		m_pChildWnd->m_bDockable=TRUE;
		m_pChildWnd->MDIActivate();
		m_pChildWnd->m_bDockable=bOldDockable;
	}

	m_pChildWnd->ShowWindow(SW_HIDE);

	if(bFirstTime)
	{
		ShowWindow(SW_SHOWNA);
		UINT nDockBarID=0;
		switch(m_pChildWnd->m_dwDockStyle)
		{
		case CBRS_ALIGN_TOP:
			{
				nDockBarID=AFX_IDW_DOCKBAR_TOP;
				break;
			}
		case CBRS_ALIGN_BOTTOM:
			{
				nDockBarID=AFX_IDW_DOCKBAR_BOTTOM;
				break;
			}
		case CBRS_ALIGN_LEFT:
			{
				nDockBarID=AFX_IDW_DOCKBAR_LEFT;
				break;
			}
		case CBRS_ALIGN_RIGHT:
			{
				nDockBarID=AFX_IDW_DOCKBAR_RIGHT;
				break;
			}
		default:
			{
				nDockBarID=0;
				break;
			}
		}
		pFrame->DockControlBar(this,nDockBarID);
	}
	else
	{
		pFrame->ShowControlBar(this, (GetStyle() & WS_VISIBLE) == 0, TRUE);
		pFrame->RecalcLayout();

		CRect rect;
		GetWindowRect(rect);
		BOOL bOldDockable=m_pChildWnd->m_bDockable;
		m_pChildWnd->m_bDockable=TRUE;
		OnSizedOrDocked(rect.Width(),rect.Height(),IsFloating(),2);
		m_pChildWnd->m_bDockable=bOldDockable;
	}

	// refresh MDI Window menu
	::SendMessage(pFrame->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);

	return TRUE;
}

BOOL COXSizeViewBar::DetachMDIChild(BOOL bRedraw)
{
	if(::IsWindow(m_pChildWnd->m_hWnd) && m_pChildWnd->IsDockable() &&
		m_pChildWnd->IsDockableFrame())
	{
		COXMDIFrameWndSizeDock* pFrame=(COXMDIFrameWndSizeDock*)m_pChildWnd->GetMDIFrame();
		ASSERT_KINDOF(COXMDIFrameWndSizeDock, pFrame);
		
		if(!IsFloating() && ::IsWindow(m_WrapperWnd.m_hWnd) && 
			!m_WrapperWnd.IsBeingDestroyed())
		{
			m_WrapperWnd.UnwrapView();
		}

		CWnd* pPrevParent=m_pView->SetParent(m_pChildWnd);
		ASSERT(pPrevParent!=NULL);

		pFrame->SetActiveView(NULL);
		m_pChildWnd->SetActiveView(m_pView);

		if(bRedraw)
		{
			pFrame->ShowControlBar(this, (GetStyle() & WS_VISIBLE) == 0, FALSE);
			pFrame->RecalcLayout();
		}

		if(m_pChildWnd==m_pChildWnd->MDIGetActive())
		{
			BOOL bOldDockable=m_pChildWnd->m_bDockable;
			m_pChildWnd->m_bDockable=FALSE;
			m_pChildWnd->MDIActivate();
			if(m_bMaximized || pFrame->IsFrontChildMaximized())
			{
				m_pChildWnd->MDIMaximize();
			}
			m_pChildWnd->m_bDockable=bOldDockable;

			CRect rect;
			m_pChildWnd->GetClientRect(rect);
			m_pView->MoveWindow(&rect);
		}

		if(bRedraw)
		{
			m_pChildWnd->ShowWindow(SW_SHOW);
		}

		// refresh MDI Window menu
		::SendMessage(pFrame->m_hWndMDIClient, WM_MDIREFRESHMENU, 0, 0);

		return TRUE;
	}

	return FALSE;
}

LONG COXSizeViewBar::OnActivateViewBar(UINT wParam, LONG lParam)
{
	BOOL bActive=wParam;

	switch(lParam)
	{
	case ID_SOURCE_MDICHILD:
		{
			if(::IsWindow(m_pChildWnd->m_hWnd) && m_pChildWnd->IsDockable())
			{
				if(!IsFloating())
				{
					if(::IsWindow(m_WrapperWnd.m_hWnd))
					{
						m_WrapperWnd.SendMessage(WM_NCACTIVATE,bActive);
					}
				}
				else
				{
					CFrameWnd* pFrame=GetParentFrame();
					if(pFrame!=NULL && 
						pFrame->IsKindOf(RUNTIME_CLASS(COXSizableMiniDockFrameWnd)))
					{
						pFrame->SendMessage(WM_NCACTIVATE,FALSE);
						pFrame->SendMessage(WM_FLOATSTATUS,
							bActive ? FS_ACTIVATE : FS_DEACTIVATE);
					}
				}
			}
			break;
		}
	case ID_SOURCE_BARWRAPPER:
	case ID_SOURCE_MINIDOCK:
		{
			if(::IsWindow(m_pChildWnd->m_hWnd) && m_pChildWnd->IsDockable())
			{
				if(bActive && m_pChildWnd->IsDockableFrame())
				{
					COXMDIFrameWndSizeDock* pFrame=
						(COXMDIFrameWndSizeDock*)m_pChildWnd->GetMDIFrame();
					if(pFrame->MDIGetActive()!=m_pChildWnd)
					{
						m_pChildWnd->MDIActivate();
					}
				}

				if(bActive && m_pChildWnd->IsDockable() && m_pView!=NULL && 
					::IsWindow(m_pView->GetSafeHwnd()))
					m_pView->SetFocus();
			}
			break;
		}
	}

	return TRUE;
}

BOOL COXSizeViewBar::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default

	return COXSizeControlBar::OnNcActivate(bActive);
}

void COXSizeViewBar::OnDestroy() 
{
	m_bBeingDestroyed=TRUE;

	COXSizeControlBar::OnDestroy();

	// TODO: Add your message handler code here
	
	if(::IsWindow(m_pChildWnd->m_hWnd) && m_pChildWnd->IsDockable())
	{
		DetachMDIChild(FALSE);
	}
}

void COXSizeViewBar::OnNcDestroy() 
{
	m_bBeingDestroyed=TRUE;

	COXSizeControlBar::OnNcDestroy();
	
	// TODO: Add your message handler code here
	
	if(::IsWindow(m_WrapperWnd.m_hWnd) && !m_WrapperWnd.IsBeingDestroyed())
	{
		m_WrapperWnd.DestroyWindow();
	}

	if(::IsWindow(m_pChildWnd->m_hWnd))
	{
		if(!m_pChildWnd->IsBeingDestroyed() && m_pChildWnd->IsDockable())
		{
			m_pChildWnd->PostMessage(WM_CLOSE);
		}
		else
		{
			m_pChildWnd->GetMDIFrame()->SetActiveView(NULL);
		}
	}
	TRACE(_T("ViewBarWindow destroyed\n"));
}

void COXSizeViewBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

LONG COXSizeViewBar::OnAddContextMenuItems(WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_pChildWnd->m_hWnd));

	UNREFERENCED_PARAMETER(wParam);

	m_pChildWnd->OnAddContextMenuItems((HMENU)lParam);

	return TRUE;
}

LONG COXSizeViewBar::OnSetText(UINT wParam, LONG lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(!IsFloating())
	{
		if(::IsWindow(m_WrapperWnd.m_hWnd))
		{
			m_WrapperWnd.SendMessage(WM_SETTEXT,wParam,lParam);
		}
	}
	else
	{
		CFrameWnd* pFrame=GetParentFrame();
		ASSERT_KINDOF(CMiniDockFrameWnd,pFrame);
		pFrame->SendMessage(WM_SETTEXT,wParam,lParam);
	}

	return Default();
}

// Now run off WM_CONTEXTMENU: if user wants standard handling, then let him have it
void COXSizeViewBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_Style & SZBARF_STDMOUSECLICKS)
	{
		CMenu menu;
		if (menu.CreatePopupMenu())
		{
			OnAddContextMenuItems(0, (LPARAM)menu.m_hMenu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	} 
	else
	{
		CControlBar::OnContextMenu(pWnd, point);
	}
}

BOOL COXSizeViewBar::QueryCloseView()
{
	if(::IsWindow(GetSafeHwnd()) && ::IsWindow(m_pView->GetSafeHwnd())
		 && ::IsWindow(m_pChildWnd->GetSafeHwnd()))	
	{
		CDocument* pDocument = m_pView->GetDocument();
		if(pDocument != NULL)
		{
			if(!pDocument->CanCloseFrame(m_pChildWnd))
			{
				// document can't close right now -- don't close it
				return FALSE;
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


COXSizeBarWrapper::COXSizeBarWrapper()
{
	m_pParentWnd=NULL;
	m_pView=NULL;

	m_bBeingDestroyed=FALSE;
}

COXSizeBarWrapper::~COXSizeBarWrapper()
{
}


BEGIN_MESSAGE_MAP(COXSizeBarWrapper, CWnd)
	//{{AFX_MSG_MAP(COXSizeBarWrapper)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COXSizeBarWrapper::Create(COXSizeViewBar* pParentWnd)
{
	ASSERT_VALID(pParentWnd);
    ASSERT(::IsWindow(pParentWnd->GetSafeHwnd()));

	m_pParentWnd=pParentWnd;

	WNDCLASS wndClass;
	wndClass.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=0; 
    wndClass.hCursor=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hbrBackground=0; 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("PopupBarControl");
	
	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	CRect rect;
	m_pParentWnd->GetClientRect(rect);
	rect.OffsetRect(-rect.TopLeft());
	CString sWindowText;
	m_pParentWnd->GetWindowText(sWindowText);

    if(!CWnd::CreateEx(WS_EX_TOOLWINDOW|WS_EX_WINDOWEDGE, wndClass.lpszClassName, 
		sWindowText, WS_CHILD|WS_BORDER|WS_DLGFRAME|WS_SYSMENU|WS_VISIBLE|WS_OVERLAPPED|0x00008000, 
		rect, m_pParentWnd, 0, NULL))
	{
        return FALSE;
	}

	SendMessage(WM_NCACTIVATE,TRUE);

    return TRUE;
}

void COXSizeBarWrapper::WrapView(COXSizeViewBar* pParentWnd, CView* pView)
{
	ASSERT(::IsWindow(pParentWnd->m_hWnd));
	ASSERT(::IsWindow(pView->m_hWnd));

	if(!::IsWindow(m_hWnd))
	{
	    if(!Create(pParentWnd))
		{
			AfxThrowMemoryException();
		}
	}

	m_pView=pView;

	CRect rect;
	pParentWnd->GetClientRect(rect);
	rect.OffsetRect(-rect.TopLeft());

	CWnd* pPrevParent=m_pView->SetParent(this);
	ASSERT(pPrevParent!=NULL);

	SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
	ShowWindow(SW_SHOW);
	UpdateWindow();
}

void COXSizeBarWrapper::UnwrapView()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(::IsWindow(m_pParentWnd->m_hWnd));
	if(::IsWindow(m_pView->m_hWnd))
	{
		CWnd* pPrevParent=m_pView->SetParent(m_pParentWnd);
		ASSERT(pPrevParent!=NULL);
		ShowWindow(SW_HIDE);
	}
}

///////////////////////////////////////////////////////////////

UINT COXSizeBarWrapper::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nHitTest=CWnd::OnNcHitTest(point);

	switch(nHitTest)
	{
	case HTCAPTION:
	case HTCLIENT:
		{
			ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
			ClientToScreen(&point);
			m_pParentWnd->ScreenToClient(&point);
			nHitTest=m_pParentWnd->SendMessage(WM_NCHITTEST,0, 
				MAKELONG(point.x,point.y));
		}
	}

	return nHitTest;
}


void COXSizeBarWrapper::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));

	ClientToScreen(&point);
	m_pParentWnd->ScreenToClient(&point);
	m_pParentWnd->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(point.x,point.y));
}

void COXSizeBarWrapper::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	m_pParentWnd->SendMessage(WM_LBUTTONDBLCLK,nFlags,MAKELONG(point.x,point.y));
}

int COXSizeBarWrapper::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // window does not want to activate

	// activate this window if necessary
	m_pParentWnd->SendMessage(WM_ACTIVATEVIEWBAR,TRUE,ID_SOURCE_BARWRAPPER);

	return nResult;
}

void COXSizeBarWrapper::OnDestroy() 
{
	m_bBeingDestroyed=TRUE;

	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if(::IsWindow(m_pParentWnd->GetSafeHwnd()) && !m_pParentWnd->IsBeingDestroyed())	
	{
		ASSERT_KINDOF(COXSizeViewBar,m_pParentWnd);
		if(!m_pParentWnd->IsFloating())
		{
			UnwrapView();
		}
	}
}

void COXSizeBarWrapper::OnNcDestroy() 
{
	m_bBeingDestroyed=TRUE;

	CWnd* pChild=GetWindow(GW_CHILD);
	ASSERT(pChild==NULL);

	CWnd::OnNcDestroy();
	
	// TODO: Add your message handler code here
	
	if(::IsWindow(m_pParentWnd->GetSafeHwnd()) && !m_pParentWnd->IsBeingDestroyed())	
	{
		ASSERT_KINDOF(COXSizeViewBar,m_pParentWnd);
		m_pParentWnd->DestroyWindow();
	}
	TRACE(_T("WrapperWindow destroyed\n"));
}


void COXSizeBarWrapper::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bBeingDestroyed=TRUE;

	if(::IsWindow(m_pParentWnd->GetSafeHwnd()) && !m_pParentWnd->IsBeingDestroyed())	
	{
		if(!m_pParentWnd->QueryCloseView())
		{
			// document can't close right now -- don't close it
			m_bBeingDestroyed=FALSE;
			return;
		}
	}

	CWnd::OnClose();
}

BOOL COXSizeBarWrapper::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(::IsWindow(m_pParentWnd->GetSafeHwnd()))	
	{
		COXMDIChildWndSizeDock* pMDIChild=m_pParentWnd->m_pChildWnd;
		if(::IsWindow(pMDIChild->m_hWnd))
		{
			if(pMDIChild->MDIGetActive()!=pMDIChild && bActive)
			{
				return FALSE;
			}
			else
			{
				return Default();
			}
		}
	}

	return CWnd::OnNcActivate(bActive);
}

/////////////////////////////////////////////////////////////////////////////
// CDocument

BEGIN_MESSAGE_MAP(COXDockDocument, CDocument)
	//{{AFX_MSG_MAP(CDocument)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocument construction/destruction

COXDockDocument::COXDockDocument()
{
	m_bBeingDestroyed=FALSE;
}

COXDockDocument::~COXDockDocument()
{
}

void COXDockDocument::OnFileClose()
{
	CWnd* pMainWnd=AfxGetMainWnd();
	if(pMainWnd->IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock)))
	{
		if(!SaveModified())
		{
			// document can't close right now -- don't close it
			return ;
		}
		else
		{
			SetModifiedFlag(FALSE);
		}

		OnCloseDocument();
	}
	else
	{
		CDocument::OnFileClose();
	}
}

BOOL COXDockDocument::CanCloseFrame(CFrameWnd* pFrameArg)
	// permission to close all views using this frame
	//  (at least one of our views must be in this frame)
{
	ASSERT_VALID(pFrameArg);
	UNREFERENCED_PARAMETER(pFrameArg);   // unused in release builds

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = GetParentFrame(pView);
		// assume frameless views are ok to close
		if (pFrame != NULL)
		{
			// assumes 1 document per frame
			ASSERT_VALID(pFrame);
			if (pFrame->m_nWindow > 0)
				return TRUE;        // more than one frame refering to us
		}
	}

	// otherwise only one frame that we know about
	if(SaveModified())
	{
		SetModifiedFlag(FALSE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void COXDockDocument::OnCloseDocument()
	// must close all views now (no prompting) - usually destroys this
{
	if(m_bBeingDestroyed)
	{
		return;
	}

	m_bBeingDestroyed=TRUE;

	CWnd* pMainWnd=AfxGetMainWnd();
	BOOL bDockableFramework=pMainWnd->
		IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock)) ? TRUE : FALSE;
	if(bDockableFramework)
	{
		// destroy all frames viewing this document
		// the last destroy may destroy us
		BOOL bAutoDelete = m_bAutoDelete;
		m_bAutoDelete = FALSE;  // don't destroy document while closing views

		UndockAllViews();

		while(!m_viewList.IsEmpty())
		{
			// get frame attached to the view
			CView* pView = (CView*)m_viewList.GetHead();
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = GetParentFrame(pView);
			ASSERT_VALID(pFrame);

			// and close it
			PreCloseFrame(pFrame);

			if(::IsWindow(pFrame->m_hWnd))
			{
				if(pFrame->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)))
				{
					COXMDIChildWndSizeDock* pMDIChild=(COXMDIChildWndSizeDock*)pFrame;
					pMDIChild->DestroyWindow();
				}
				else
				{
					pFrame->DestroyWindow();
				}
			}
		}
		m_bAutoDelete = bAutoDelete;

		// clean up contents of document before destroying the document itself
		DeleteContents();

		// delete the document if necessary
		if (m_bAutoDelete)
			delete this;

		((COXMDIFrameWndSizeDock*)pMainWnd)->RecalcLayout();
	}
	else
	{
		CDocument::OnCloseDocument();
	}
}

void COXDockDocument::UpdateFrameCounts()
	 // assumes 1 doc per frame
{
	// walk all frames of views (mark and sweep approach)
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = GetParentFrame(pView);
			if (pFrame != NULL)
				pFrame->m_nWindow = -1;     // unknown
		}
	}

	// now do it again counting the unique ones
	int nFrames = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = GetParentFrame(pView);
			if (pFrame != NULL && pFrame->m_nWindow == -1)
			{
				ASSERT_VALID(pFrame);
				// not yet counted (give it a 1 based number)
				pFrame->m_nWindow = ++nFrames;
			}
		}
	}

	// lastly walk the frames and update titles (assume same order)
	// go through frames updating the appropriate one
	int iFrame = 1;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = GetParentFrame(pView);
			if (pFrame != NULL && pFrame->m_nWindow == iFrame)
			{
				ASSERT_VALID(pFrame);
				if (nFrames == 1)
					pFrame->m_nWindow = 0;      // the only one of its kind
				pFrame->OnUpdateFrameTitle(TRUE);
				iFrame++;
			}
		}
	}
	ASSERT(iFrame == nFrames + 1);
}

CFrameWnd* COXDockDocument::GetParentFrame(CView* pView) const
{
	if (pView->GetSafeHwnd() == NULL) // no Window attached
	{
		return NULL;
	}

	ASSERT_VALID(pView);

	CWnd* pParentWnd = pView->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		if (pParentWnd->IsFrameWnd())
		{
			return (CFrameWnd*)pParentWnd;
		}
		else if (pParentWnd->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			if(::IsWindow(((COXSizeViewBar*)pParentWnd)->m_pChildWnd->m_hWnd))
			{
				return (CFrameWnd*)((COXSizeViewBar*)pParentWnd)->m_pChildWnd;
			}
			else
			{
				return NULL;
			}
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void COXDockDocument::UndockAllViews()
{
	POSITION pos=m_viewList.GetHeadPosition();
	while(pos!=NULL)
	{
		// get frame attached to the view
		CView* pView = (CView*)m_viewList.GetNext(pos);
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = GetParentFrame(pView);
		ASSERT_VALID(pFrame);

		if(::IsWindow(pFrame->m_hWnd) && 
			pFrame->IsKindOf(RUNTIME_CLASS(COXMDIChildWndSizeDock)))
		{
			COXMDIChildWndSizeDock* pMDIChild=(COXMDIChildWndSizeDock*)pFrame;
			if(pMDIChild->IsDockable())
			{
				if(::IsWindow(pMDIChild->m_dockBar.m_hWnd))
				{
					pMDIChild->m_dockBar.DetachMDIChild(FALSE);
				}
			}
		}
	}
}

IMPLEMENT_DYNAMIC(COXDockDocument, CDocument)






