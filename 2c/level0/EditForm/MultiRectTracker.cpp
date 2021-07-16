// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// MultiRectTracker.cpp: implementation of the CMultiRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "enterprise.h"
#include "MultiRectTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMultiRectTracker CMultiRectTracker CMultiRectTracker CMultiRectTracker CMultiRectTracker
/////////////////////////////////////////////////////////////////////////////
HBRUSH CMultiRectTracker::_afxHatchBrush;
HPEN CMultiRectTracker::_afxDottedPen;
void CMultiRectTracker::AdjustRect( int nHandle, LPRECT lpRect )
{
	if(bNotMove)
	{
		if(lpRect->left!=0||lpRect->top!=0)
		{
			lpRect->left=0;
			lpRect->top=0;
		}
	}
	if(m_bUseGrid)
	{
		if(m_nGridX>0)
		{
			lpRect->left=int(lpRect->left/m_nGridX)*m_nGridX;
			lpRect->right=int(lpRect->right/m_nGridX)*m_nGridX;
		}
		if(m_nGridY>0)
		{
			lpRect->top=int(lpRect->top/m_nGridY)*m_nGridY;
			lpRect->bottom=int(lpRect->bottom/m_nGridY)*m_nGridY;
		}
	}

};


void CMultiRectTracker::Construct()
{
	// do one-time initialization if necessary
	static BOOL bInitialized;
	m_nHandleSize=5;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
		ASSERT(offsetof(RECT, top) > offsetof(RECT, left));
		ASSERT(offsetof(RECT, right) > offsetof(RECT, top));
		ASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));
		
		if (_afxHatchBrush == NULL)
		{
			// create the hatch pattern + bitmap
			WORD hatchPattern[8];
			WORD wPattern = 0x1111;
			for (int i = 0; i < 4; i++)
			{
				hatchPattern[i] = wPattern;
				hatchPattern[i+4] = wPattern;
				wPattern <<= 1;
			}
			HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, &hatchPattern);
			
			// create black hatched brush
			_afxHatchBrush = CreatePatternBrush(hatchBitmap);
			DeleteObject(hatchBitmap);
		}
		
		if (_afxDottedPen == NULL)
		{
			// create black dotted pen
			_afxDottedPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 150));
		}
	}
};

void CMultiRectTracker::Draw(CDC* pDC)
{
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	CRect _m_rect=m_rect;
	for(int i=0;i<aList.GetSize();i++)
	{
		bool bResize=0;
		if(aWindows[i]==pCurWnd)
			bResize=1;
		m_rect=aList[i];
		Draw(pDC,bResize);
	}
	m_rect=_m_rect;
}
void CMultiRectTracker::Draw(CDC* pDC,bool bResize)
{
	VERIFY(pDC->SaveDC() != 0);
	// set initial DC state

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CRect rectTrue;
	GetTrueRect(&rectTrue);

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxDottedPen));
		else
			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(OPAQUE);
		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, PATINVERT);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, PATINVERT);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), PATINVERT);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), PATINVERT);
	}

	// draw resize handles
	if(bResize)
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
				pDC->FillSolidRect(rect, RGB(0, 0, 0));
			}
		}
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}

void CMultiRectTracker::DrawTrackerRect(
	LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CRect rect = *lpRect;
	rect.NormalizeRect();

	// convert to client coordinates
	if (pWndClipTo != NULL)
	{
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
/*		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
*/		{
			size.cx = CX_BORDER;
			size.cy = CY_BORDER;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
	{
		//проверяем - это перемещение или изменение размера
		if(TestRect.Height()==m_rect.Height()&&TestRect.Width()==m_rect.Width())
		{
			for(int i=0;i<aList.GetSize();i++)
			{
				CPoint delta;
				delta.x=rect.left-TestRect.left;
				delta.y=rect.top-TestRect.top;
				CRect rect1=aList[i];
				rect1.OffsetRect(delta);

				delta.x=m_rectLast.left-TestRect.left;
				delta.y=m_rectLast.top-TestRect.top;
				CRect rect2=aList[i];
				rect2.OffsetRect(delta);

				pDC->DrawDragRect(rect1, size, rect2, m_sizeLast);
			}
		}
		else
			pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast);
	}

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}

BOOL CMultiRectTracker::TrackHandle(int nHandle, CWnd* pWnd, CPoint point,
	CWnd* pWndClipTo)
{
	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= 8);   // handle 8 is inside the rect

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// save original width & height in pixels
	int nWidth = m_rect.Width();
	int nHeight = m_rect.Height();

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();
	CRect rectSave = m_rect;

	// find out what x/y coords we are supposed to modify
	int *px, *py;
	int xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ASSERT_VALID(pDrawDC);

	CRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			rectOld = m_rect;
			// handle resize cases (and part of move)
			if (px != NULL)
				*px = (int)(short)LOWORD(msg.lParam) - xDiff;
			if (py != NULL)
				*py = (int)(short)HIWORD(msg.lParam) - yDiff;

			if(bNotNegative)
			{
				for(int i=0;i<aList.GetSize();i++)
				{
					CPoint delta;
					delta.x=m_rect.left-TestRect.left;
					delta.y=m_rect.top-TestRect.top;
					if(delta.x+aList[i].left<0)
					{
						m_rect = rectOld;
						m_rect.left+=1;
						m_rect.right+=1;
						break;
					}
					if(delta.y+aList[i].top<0)
					{
						m_rect = rectOld;
						m_rect.top+=1;
						m_rect.bottom+=1;
						break;
					}
				}
			}

			// handle move case
			if (nHandle == hitMiddle)
			{
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
			}
			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
					DrawTrackerRect(&rectOld, pWndClipTo, pDrawDC, pWnd);
				}
				OnChangedRect(rectOld);
				if (msg.message != WM_LBUTTONUP)
					bMoved = TRUE;
			}
			if (m_bFinalErase)
				goto ExitLoop;

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
				DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
			m_rect = rectSave;
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}

BOOL CMultiRectTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	// perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;
	aOldList.Copy(aList);
	AdjustRect(0,&m_rect);
	TestRect=m_rect;
	BOOL bRes=TrackHandle(nHandle, pWnd, point, pWndClipTo);

	if(bRes)
	{
		//проверяем - это перемещение или изменение размера
		if(TestRect.Height()==m_rect.Height()&&TestRect.Width()==m_rect.Width())
		{
			for(int i=0;i<aList.GetSize();i++)
			{
				CPoint delta;
				delta.x=m_rect.left-TestRect.left;
				delta.y=m_rect.top-TestRect.top;
				aList[i].OffsetRect(delta);
			}
		}
	}

	return bRes;
}

void CMultiRectTracker::Init(CWnd *pWnd,LPCRECT lpSrcRect, int nStyle0,int bOnlyAdd)
{
	UINT nStyle=nStyle0;

	if(nStyle0==-1)
		nStyle=dottedLine  + resizeOutside + resizeInside + hatchedBorder;

	pCurWnd=pWnd;
	m_nStyle = nStyle;
	int nFind=-1;
	for(int i=0;i<aList.GetSize();i++)
	{
		if(pWnd==aWindows[i])
		{
			nFind=i;
			break;
		}
	}
	if(bOnlyAdd>=0 && (1==bOnlyAdd||IsCTRLpressed()||IsSHIFTpressed()))
	{
		if(nFind>=0)
		{
			aList.RemoveAt(nFind);
			aWindows.RemoveAt(nFind);
			if(aList.GetSize()>0)
			{
				m_rect.CopyRect(aList[aList.GetSize()-1]);
				pCurWnd=aWindows[aWindows.GetSize()-1];
			}
			if(1==bOnlyAdd)
				return;
		}
	}
	else
	{
		if(nFind>=0)//&&bOnlyAdd==1)
		{
			m_rect.CopyRect(lpSrcRect);
			return;
		}
		else
		{
			aList.RemoveAll();
			aWindows.RemoveAll();
		}
	}

	pCurWnd=pWnd;
	m_rect.CopyRect(lpSrcRect);
	aList.Add(m_rect);
	aWindows.Add(pWnd);
};


int CMultiRectTracker::HitTestHandles(CPoint point) const
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	rect.InflateRect(+5, +5);
	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

/////////////////////////////////////////////////////////////////////////////


