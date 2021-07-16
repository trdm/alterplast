// GridCellBase.cpp : implementation file
//
// MFC Grid Control - Main grid cell base class
//
// Provides the implementation for the base cell type of the
// grid control. No data is stored (except for state) but default
// implementations of drawing, printingetc provided. MUST be derived
// from to be used.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2000. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22
//
// History:
// Ken Bertelson - 12 Apr 2000 - Split CGridCell into CGridCell and CGridCellBase
// C Maunder     - 19 May 2000 - Fixed sort arrow drawing (Ivan Ilinov)
// C Maunder     - 29 Aug 2000 - operator= checks for NULL font before setting (Martin Richter)
// C Maunder     - 15 Oct 2000 - GetTextExtent fixed (Martin Richter)
// C Maunder     -  1 Jan 2001 - Added ValidateEdit
//
// NOTES: Each grid cell should take care of it's own drawing, though the Draw()
//        method takes an "erase background" paramter that is called if the grid
//        decides to draw the entire grid background in on hit. Certain ambient
//        properties such as the default font to use, and hints on how to draw
//        fixed cells should be fetched from the parent grid. The grid trusts the
//        cells will behave in a certain way, and the cells trust the grid will
//        supply accurate information.
//        
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCellBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellBase, CObject)




CString FuncFormat(CValue &v,CString &fmt);


/////////////////////////////////////////////////////////////////////////////
//MyDrawText

int MyDrawText(HDC hdc, CString szText, int nCount, LPRECT lpRect, UINT uFormat)
{
    SIZE size;
    int x=0, y=0, width=0, height=0;
    
    if(hdc == NULL)
        return FALSE;

	if(nCount<0)
		nCount=szText.GetLength();
    
    x = lpRect->left;
    y = lpRect->top;
    
    GetTextExtentPoint32(hdc, szText, nCount, &size);
    width = size.cx;
    height = size.cy;
    
    
    if(uFormat & DT_CALCRECT) {
        lpRect->right = x + width;
        lpRect->bottom = y + height;
        return height; // Ничего не рисуем 
    }
    
    if(uFormat & DT_CENTER)
        x +=((lpRect->right - lpRect->left) - width) / 2;
    else if(uFormat & DT_RIGHT)
        x +=((lpRect->right - lpRect->left) - width);
    
    if(uFormat & DT_VCENTER)
        y +=((lpRect->bottom - lpRect->top) - height) / 2;
    if(uFormat & DT_BOTTOM)
        y +=((lpRect->bottom-lpRect->top) - height);
    
    TextOut(hdc, x, y, (LPCTSTR)szText, nCount);
    
    return height;
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase

CGridCellBase::CGridCellBase()
{
    Reset();
}

CGridCellBase::~CGridCellBase()
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase Operations

void CGridCellBase::Reset()
{
    m_nState  = 0;
}

void CGridCellBase::operator=(const CGridCellBase& cell)
{
    SetGrid(cell.GetGrid());    // do first in case of dependencies

    SetText(cell.GetText());
    SetValue(cell.GetValue());
    SetImage(cell.GetImage());
    SetData(cell.GetData());
    SetState(cell.GetState());
    SetFormat(cell.GetFormat());
    SetTextClr(cell.GetTextClr());
    SetBackClr(cell.GetBackClr());
    SetFont(cell.IsDefaultFont()? NULL : cell.GetFont());
    SetMargin(cell.GetMargin());
//	SetLeftSpace(cell.GetLeftSpace());
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Attributes

// Returns a pointer to a cell that holds default values for this particular type of cell
CGridCellBase* CGridCellBase::GetDefaultCell() const
{
    if (GetGrid())
        return GetGrid()->GetDefaultCell(IsFixedRow(), IsFixedCol());
    return NULL;
}

//для W98
CPen *GetPen(int nNumber) 
{
	switch(nNumber)
	{
		case 0:
			{
				static CPen pen(PS_NULL,1,RGB(0,0,0));
				return &pen;
			}
		case 1:
			{
				static CPen pen(PS_DOT,1,RGB(0,0,0));
				return &pen;
			}
		case 2:
			{
				static CPen pen(PS_SOLID,1,RGB(0,0,0));
				return &pen;
			}
		case 3:
			{
				static CPen pen(PS_SOLID,2,RGB(0,0,0));
				return &pen;
			}
		case 4:
			{
				static CPen pen(PS_SOLID,2,RGB(0,0,0));
				return &pen;
			}
		case 5:
			{
				static CPen pen(PS_SOLID,1,RGB(0,0,0));
				return &pen;
			}
		case 6:
			{
				static CPen pen(PS_DASH,1,RGB(0,0,0));
				return &pen;
			}
		case 7:
			{
				static CPen pen(PS_DASHDOT,1,RGB(0,0,0));
				return &pen;
			}
		case 8:
			{
				static CPen pen(PS_SOLID,1,RGB(0,0,0));
				return &pen;
			}
		case 9:
			{
				static CPen pen(PS_DASH,2,RGB(0,0,0));
				return &pen;
			}
	}

	ASSERT(0);
	static CPen pen(PS_NULL,1,RGB(0,0,0));
	return &pen;
}

int IsWindows5();

inline void DrawRamka(CDC* pDC,int nColor,int nNumber,int x1,int y1,int x2,int y2)
{
	if(nNumber>0)
	{
		if(IsWindows5())
		{
			int nRamka=GetRamka(nNumber);
			CPen pen;
			pen.CreatePen(nRamka%10, nRamka/10, nColor);
			pDC->SelectObject(&pen);
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
		}
		else
		{
			pDC->SelectObject(GetPen(nNumber));
			pDC->MoveTo(x1, y1);
			pDC->LineTo(x2, y2);
		}
		//pen.DeleteObject();
	}
}

//int nCountCall=0;

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Operations
// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell() and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL CGridCellBase::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!pGrid || !pDC)
        return FALSE;

	CPen *pOldPen = pDC->GetCurrentPen();
	if(IsCovered())//поддержка объединенных ячеек
	{
		CGridCellBase *pCoveredCell=GetCoveredCell();
		if(pCoveredCell)//перекрытая ячейка
		{
			if(pDC->IsPrinting())
				return 1;//рисуем только один раз
			pCoveredCell->GetCoveredRowCol(nRow,nCol);
			return pCoveredCell->Draw(pDC,nRow,nCol,rect,bEraseBkgnd);
		}
		else
		{
			if(pDC->IsPrinting())
			{
				CRect rect2;
				pGrid->GetCellRect(nRow,nCol,rect2,0);
				rect.right=rect.left+rect2.Width();
				rect.bottom=rect.top+rect2.Height();
			}
			else
			{
				pGrid->GetCellRect(nRow,nCol,rect,0);
			}
		}
	}

    if( rect.Width() <= 0 || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;                             //  though cell is hidden

    int nSavedDC = pDC->SaveDC();
    pDC->SetBkMode(TRANSPARENT);

	BOOL nFixed=IsFixed();
	if(nFixed)
	{
		LOGFONT *pFont; 
		pFont=(LOGFONT *)GetFont();
		pFont->lfWeight = FW_BOLD;
		SetFont(pFont);
	}


    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
    CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
    if (!pDefaultCell)
        return FALSE;

    // Set up text and background colours
    COLORREF TextClr, TextBkClr;

    TextClr = (GetTextClr() == CLR_DEFAULT)? pDefaultCell->GetTextClr() : GetTextClr();
    if (GetBackClr() == CLR_DEFAULT)
        TextBkClr = pDefaultCell->GetBackClr();
    else
    {
		bEraseBkgnd = TRUE;
        TextBkClr = GetBackClr(); 
    }

	CCell *pExtCell=GetCell();
	if(pExtCell)
	{
		if(0xFFFFFF!=pExtCell->nBackgroundColor)
		{
			TextBkClr=pExtCell->nBackgroundColor;
			bEraseBkgnd = TRUE;
		}
		if(pExtCell->nFontColor)
			TextClr=pExtCell->nFontColor;
	}

    // Draw cell background and highlighting (if necessary)
	if(!pDC->IsPrinting())
    if ((IsFocused() || IsDropHighlighted() ))
    {
        // Always draw even in list mode so that we can tell where the
        // cursor is at.  Use the highlight colors though.
        if(GetState() & GVIS_SELECTED || pGrid->GetFrameFocusCell())
        {
            TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
            TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
            bEraseBkgnd = TRUE;
        }

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBkgnd)
        {
            TRY 
            {
                CBrush brush(TextBkClr);
                pDC->FillRect(rect, &brush);
            } 
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }

        // Don't adjust frame rect if no grid lines so that the
        // whole cell is enclosed.
        //if(pGrid->GetGridLines() != GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        if (pGrid->GetFrameFocusCell())
        {
                // Use same color as text to outline the cell so that it shows
                // up if the background is black.
            TRY 
            {
                CBrush brush(TextClr);
                pDC->FrameRect(rect, &brush);
            }
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }
        pDC->SetTextColor(TextClr);
		//rect.right++;
		//rect.bottom++;
		rect.DeflateRect(1,1);
    }
    else 
	if ((GetState() & GVIS_SELECTED))
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        rect.right--; rect.bottom--;
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    else
    {
        if (bEraseBkgnd)
        {
            rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
        pDC->SetTextColor(TextClr);
    }

    // Draw lines only when wanted
	if (nFixed)
	{
		static CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
			darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW));

		pDC->SelectObject(&lightpen);
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom);

		pDC->SelectObject(&darkpen);
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.right, rect.bottom);
		pDC->LineTo(rect.left, rect.bottom);

		if(!pGrid->IsExtMode)//разделительные линии аля 1С
		{
			rect.right-=2;
			pDC->MoveTo(rect.right, rect.top);
			pDC->LineTo(rect.right, rect.bottom);
		}

		rect.DeflateRect(1,1);
	}
	else
	{
		int nLineColor=pGrid->GetGridLineColor();
		if(nLineColor!=RGB(255,255,255))
		if(pGrid->GetGridLines() != GVL_NONE)
		{
			if(pGrid->IsExtMode)
			{
				static CPen pen(PS_SOLID, 0, nLineColor);
				pDC->SelectObject(&pen);
			}
			else
			{
				static CPen pen(PS_SOLID, 0, nLineColor);
				pDC->SelectObject(&pen);
			}

			rect.top-=1;
			pDC->MoveTo(rect.left, rect.bottom);
			pDC->LineTo(rect.right, rect.bottom);
			pDC->LineTo(rect.right, rect.top);
			rect.top+=1;


			if(!pGrid->IsExtMode)//разделительные линии аля 1С
			{
				rect.right-=2;
				pDC->MoveTo(rect.right, rect.top);
				pDC->LineTo(rect.right, rect.bottom);
			}
		}
	}

	//DrawText(pDC->m_hDC, GetText(), -1, rect, GetFormat() | DT_NOPREFIX);
	//return TRUE;

	CString StrText;
    int nFormat;
	CRect rect0=rect;

	BOOL bWasFont=0;
	//Рисование рамки и установка шрифта при наличии дополнительной информации
	{
	if(pExtCell)
	{
		StrText=pExtCell->csText;

		nFormat=0;
		//формат здесь свой...
		if(0==pExtCell->nHPosition)
			nFormat+=DT_LEFT;
		else
		if(1==pExtCell->nHPosition)
			nFormat+=DT_RIGHT;
		else
		if(3==pExtCell->nHPosition)
			nFormat+=DT_CENTER;

		if(0==pExtCell->nVPosition)
			nFormat+=DT_TOP;
		else
		if(1==pExtCell->nVPosition)
			nFormat+=DT_BOTTOM;
		else
		if(2==pExtCell->nVPosition)
			nFormat+=DT_VCENTER;

		//контроль ввода текста
		if(pGrid->IsExtMode)
		{
			if(!StrText.IsEmpty())
			if(0==pExtCell->nControl)
			if(nFormat&DT_RIGHT)//авторасширение влево
			{
				for(int n=nCol-1;n>=1;n--)
				{
					if(!pGrid->GetItemText(nRow,n).IsEmpty())
						break;
					rect.left-=pGrid->GetColumnWidth(n);

					rect.left=max(rect.left,pGrid->GetFixedColumnWidth());
				}
			}
			else
			if(!(nFormat&DT_CENTER))//авторасширение вправо
			{
				int nCount=pGrid->GetColumnCount();
				for(int n=nCol+1;n<nCount;n++)
				{
					if(!pGrid->GetItemText(nRow,n).IsEmpty())
						break;
					rect.right+=pGrid->GetColumnWidth(n);
				}
			}

			if(3==pExtCell->nControl)//Переносить
				nFormat = nFormat | DT_WORDBREAK;
		} 

		//Рисование рамки
		CRect rect2=rect0;
		rect2.left-=1;
		rect2.top-=1;

		int nColor=pExtCell->nRamkaColor;
		//нижняя
		DrawRamka(pDC,nColor,
			pExtCell->nRamkaD,
			rect2.left,rect2.bottom,rect2.right,rect2.bottom);

		//левая
		DrawRamka(pDC,nColor,
			pExtCell->nRamkaL,
			rect2.left,rect2.bottom,rect2.left,rect2.top);

		//верхняя
		DrawRamka(pDC,nColor,
			pExtCell->nRamkaU,
			rect2.left,rect2.top,rect2.right,rect2.top);

		//правая
		DrawRamka(pDC,nColor,
			pExtCell->nRamkaR,
			rect2.right,rect2.top,rect2.right,rect2.bottom);

		//Установка шрифта
		//if(0)
		if(pExtCell->nBold || pExtCell->nItalic || pExtCell->nUnderLine || pExtCell->nFontHeight)
		{

			LOGFONT *pFont; 
			pFont=(LOGFONT *)GetFont();
			if(pExtCell->nBold)
				pFont->lfWeight = FW_BOLD;
			else
				pFont->lfWeight = FW_NORMAL;
			pFont->lfItalic = pExtCell->nItalic;
			pFont->lfUnderline = pExtCell->nUnderLine;

			if(pExtCell->nFontHeight)
				pFont->lfHeight=16*pExtCell->nFontHeight/10;

			SetFont(pFont);
/*			static CFont SetFont;
			SetFont.DeleteObject();
			strcpy(pFont->lfFaceName,"Arial");
			SetFont.CreateFontIndirect(pFont);

			pDC->SelectObject(&SetFont);
			bWasFont=1;
*/
		}
	}
	else
	if(nRow<pGrid->GetRowCount())
	if(nCol<pGrid->aColumns.GetSize())
	{
		if(nRow>=pGrid->GetFixedRowCount())
		{
			CColumnDescription &Column=pGrid->aColumns[nCol];
			if(!Column.csFormule.IsEmpty())
			{
				pGrid->nDrawRow=nRow;
				pGrid->nDrawCol=nCol;
				CValue Val=CProcUnit::Eval(Column.csFormule,pGrid->pRunContext);
				pGrid->nDrawRow=0;
				pGrid->nDrawCol=0;
				if(!Column.bEnable || nFixed) 
				{
					SetValue(Val);
					if(pGrid->GetVirtualMode())
						pGrid->SetItemValue(nRow,nCol,Val);
				}
			}
		}

		
		CValue Val=GetValue();
		if(Val.GetType()==TYPE_NUMBER)
		{
			CColumnDescription &Column=pGrid->aColumns[nCol];
			int nLength=pGrid->aColumns[nCol].nLength;
			int nPrec=pGrid->aColumns[nCol].nPrec;
			if(nLength && nPrec)
			{
				if(nLength<=0)
					nLength=20;
				CString csFrm;
				csFrm.Format("Ч%d.%d",nLength,nPrec);
				StrText=FuncFormat(Val,csFrm);
				StrText.TrimLeft();
			}
			else
			{
				StrText.Format("%d",int(Val.GetNumber()));
			}
		}
		else
		{
			StrText=Val.GetString();
		}

		nFormat = GetFormat();
	}
	}

    // Draw Text and image
	CFont* pFont = GetFontObject();
	ASSERT(pFont);
	if (pFont)
	{
		pDC->SelectObject(pFont);
	}

    rect.DeflateRect(GetMargin(), 0);

	CImageList* pImageList=pGrid->GetImageList();
	int nImage=GetImage();
    if (pImageList && nImage >= 0)
    {
        IMAGEINFO Info;
        if (pImageList->GetImageInfo(nImage, &Info))
        {
            //  would like to use a clipping region but seems to have issue
            //  working with CMemDC directly.  Instead, don't display image
            //  if any part of it cut-off

            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top+1;

            if( nImageWidth + rect.left <= rect.right + (int)(2*GetMargin())
                && nImageHeight + rect.top <= rect.bottom + (int)(2*GetMargin())  )
            {
                pImageList->Draw(pDC, nImage, rect.TopLeft(), ILD_NORMAL);
            }
            rect.left += nImageWidth+GetMargin();
        }
    }

    // Draw sort arrow
    if (pGrid->GetSortColumn() == nCol && nRow == 0)
    {
        CSize size = pDC->GetTextExtent(_T("M"));
        int nOffset = 2;

        // Base the size of the triangle on the smaller of the column
        // height or text height with a slight offset top and bottom.
        // Otherwise, it can get drawn outside the bounds of the cell.
        size.cy -= (nOffset * 2);

        if (size.cy >= rect.Height())
            size.cy = rect.Height() - (nOffset * 2);

        size.cx = size.cy;      // Make the dimensions square

        // Kludge for vertical text
        BOOL bVertical = (GetFont()->lfEscapement == 900);

        // Only draw if it'll fit!
        if (size.cx + rect.left < rect.right + (int)(2*GetMargin()))
        {
            int nTriangleBase = rect.bottom - nOffset - size.cy;    // Triangle bottom right
            int nTriangleLeft;
            if (bVertical)
                nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
            else
                nTriangleLeft = rect.right - size.cx;               // Triangle RHS

            static CPen penShadow(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
            static CPen penLight(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
            if (pGrid->GetSortAscending())
            {
                // Draw triangle pointing upwards
                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);
                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + size.cy + 1);
                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);

                pDC->SelectObject(&penShadow);
                pDC->MoveTo( nTriangleLeft, nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase );
                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft, nTriangleBase + size.cy );
            }
            else
            {
                // Draw triangle pointing downwards
                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + size.cy + 1 );
                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + 1 );
    
                pDC->SelectObject(&penShadow);
                pDC->MoveTo( nTriangleLeft, nTriangleBase );
                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase );
                pDC->LineTo( nTriangleLeft, nTriangleBase );
            }
            
            if (!bVertical)
                rect.right -= size.cy;
        }
    }

	//nCountCall++;
	if(pGrid->IsExtMode)
	{
		if(nFixed)//принудительное рисование номеров строк и колонок
		{
			if(nRow!=0||nCol!=0)
			{
				CString Str;
				if(nRow==0)
					Str.Format("%d",nCol);
				else
					Str.Format("%d",nRow);
				int nFormat=DT_CENTER|DT_VCENTER|DT_NOPREFIX;// | DT_END_ELLIPSIS;
				DrawText(pDC->m_hDC, Str, -1, rect, nFormat);
		        //pDC->TextOut( rect.left, rect.top, Str );
			}
		}
		else
		{
			if(!StrText.IsEmpty())
			{
				//pDC->SelectObject(pFont);
				DrawText(pDC->m_hDC, StrText, -1, rect, nFormat | DT_NOPREFIX);
		        //pDC->TextOut( rect.left, rect.top, StrText );

			}
		}
	}
	else
	{
		// We want to see '&' characters so use DT_NOPREFIX
		DrawText(pDC->m_hDC, StrText, -1, rect, nFormat | DT_NOPREFIX);
	}


    pDC->SelectObject(pOldPen);
    pDC->RestoreDC(nSavedDC);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Mouse and Cursor events

// Not yet implemented
void CGridCellBase::OnMouseEnter()
{
    TRACE0("Mouse entered cell\n");
}

void CGridCellBase::OnMouseOver()
{
    //TRACE0("Mouse over cell\n");
}

// Not Yet Implemented
void CGridCellBase::OnMouseLeave()
{
    TRACE0("Mouse left cell\n");
}

void CGridCellBase::OnClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse Left btn up in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnClickDown( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse Left btn down in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnRClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse right-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnDblClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse double-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

// Return TRUE if you set the cursor
BOOL CGridCellBase::OnSetCursor()
{
#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase editing

void CGridCellBase::OnEndEdit() 
{
	ASSERT( FALSE); 
}

BOOL CGridCellBase::ValidateEdit(LPCTSTR str)
{
    UNUSED_ALWAYS(str);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Sizing

BOOL CGridCellBase::GetTextRect( LPRECT pRect)  // i/o:  i=dims of cell rect; o=dims of text rect
{
    if (GetImage() >= 0)
    {
        IMAGEINFO Info;

        CGridCtrl* pGrid = GetGrid();
        CImageList* pImageList = pGrid->GetImageList();
        
        if (pImageList->GetImageInfo( GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            pRect->left += nImageWidth + GetMargin();
        }
    }

    return TRUE;
}

// By default this uses the selected font (which is a bigger font)
CSize CGridCellBase::GetTextExtent(LPCTSTR szText, CDC* pDC /*= NULL*/)
{
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    BOOL bReleaseDC = FALSE;
    if (pDC == NULL)
    {
        pDC = pGrid->GetDC();
        if (!pDC) 
        {
            CGridDefaultCell* pDefCell = (CGridDefaultCell*) GetDefaultCell();
            ASSERT(pDefCell);
            return CSize(pDefCell->GetWidth(), pDefCell->GetHeight());
        }
        bReleaseDC = TRUE;
    }

    CFont *pOldFont = NULL,
          *pFont = GetFontObject();
    if (pFont)
        pOldFont = pDC->SelectObject(pFont);

    CSize size;
    int nFormat = GetFormat();

    // If the cell is a multiline cell, then use the width of the cell
    // to get the height
    if ((nFormat & DT_WORDBREAK) && !(nFormat & DT_SINGLELINE))
    {
        CString str = szText;
        int nMaxWidth = 0;
        while (TRUE)
        {
            int nPos = str.Find(_T('\n'));
            CString TempStr = (nPos < 0)? str : str.Left(nPos);
            int nTempWidth = pDC->GetTextExtent(TempStr).cx;
            if (nTempWidth > nMaxWidth)
                nMaxWidth = nTempWidth;

            if (nPos < 0)
                break;
            str = str.Mid(nPos + 1);    // Bug fix by Thomas Steinborn
        }
        
        CRect rect;
        rect.SetRect(0,0, nMaxWidth+1, 0);
        pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);
        size = rect.Size();
    }
    else
        size = pDC->GetTextExtent(szText, _tcslen(szText));

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    size.cx += (tm.tmOverhang);

    if (pOldFont)
        pDC->SelectObject(pOldFont);
    
    size += CSize(4*GetMargin(), 2*GetMargin());

    // Kludge for vertical text
    LOGFONT *pLF = GetFont();
    if (pLF->lfEscapement == 900 || pLF->lfEscapement == -900)
    {
        int nTemp = size.cx;
        size.cx = size.cy;
        size.cy = nTemp;
        size += CSize(0, 4*GetMargin());
    }
    
    if (bReleaseDC)
        pGrid->ReleaseDC(pDC);

    return size;
}

CSize CGridCellBase::GetCellExtent(CDC* pDC)
{
    CSize size = GetTextExtent(GetText(), pDC);
    CSize ImageSize(0,0);

    int nImage = GetImage();
    if (nImage >= 0) 
    {
        CGridCtrl* pGrid = GetGrid();
        ASSERT(pGrid);

        if (pGrid->GetImageList()) 
        {
            IMAGEINFO Info;
            if (pGrid->GetImageList()->GetImageInfo(nImage, &Info))
                ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
                                  Info.rcImage.bottom-Info.rcImage.top+1);
        }
    }
    
    return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}

// EFW - Added to print cells so that grids that use different colors are
// printed correctly.
BOOL CGridCellBase::PrintCell(CDC* pDC, int nRow, int nCol, CRect rect)
{
//	return Draw(pDC, nRow, nCol, rect);
#if defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return FALSE;
#else
    COLORREF crFG, crBG;
    GV_ITEM Item;

    CGridCtrl* pGrid = GetGrid();
    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0
        || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;           //  though cell is hidden

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if(pGrid->GetShadedPrintOut())
    {
        // Get the default cell implementation for this kind of cell. We use it if this cell
        // has anything marked as "default"
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return FALSE;

        // Use custom color if it doesn't match the default color and the
        // default grid background color.  If not, leave it alone.
        if(IsFixed())
            crBG = (GetBackClr() != CLR_DEFAULT) ? GetBackClr() : pDefaultCell->GetBackClr();
        else
            crBG = (GetBackClr() != CLR_DEFAULT && GetBackClr() != pDefaultCell->GetBackClr()) ?
                GetBackClr() : CLR_DEFAULT;

        // Use custom color if the background is different or if it doesn't
        // match the default color and the default grid text color.  If not,
        // use black to guarantee the text is visible.
        if(IsFixed())
            crFG = (GetBackClr() != CLR_DEFAULT) ? GetTextClr() : pDefaultCell->GetTextClr();
        else
            crFG = (GetBackClr() != CLR_DEFAULT ||
                (GetTextClr() != CLR_DEFAULT && GetTextClr() != pDefaultCell->GetTextClr())) ?
                    GetTextClr() : RGB(0, 0, 0);

        // If not printing on a color printer, adjust the foreground color
        // to a gray scale if the background color isn't used so that all
        // colors will be visible.  If not, some colors turn to solid black
        // or white when printed and may not show up.  This may be caused by
        // coarse dithering by the printer driver too (see image note below).
        if(pDC->GetDeviceCaps(NUMCOLORS) == 2 && crBG == CLR_DEFAULT)
            crFG = RGB(GetRValue(crFG) * 0.30, GetGValue(crFG) * 0.59,
                GetBValue(crFG) * 0.11);

        // Only erase the background if the color is not the default
        // grid background color.
        if(crBG != CLR_DEFAULT)
        {
            CBrush brush(crBG);
            rect.right++; rect.bottom++;
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
    }
    else
    {
        crBG = CLR_DEFAULT;
        crFG = RGB(0, 0, 0);
    }

    pDC->SetTextColor(crFG);

    CFont *pFont = GetFontObject();
    if (pFont)
        pDC->SelectObject(pFont);

    /*
    // ***************************************************
    // Disabled - if you need this functionality then you'll need to rewrite.
    // Create the appropriate font and select into DC.
    CFont Font;
    // Bold the fixed cells if not shading the print out.  Use italic
    // font it it is enabled.
    const LOGFONT* plfFont = GetFont();
    if(IsFixed() && !pGrid->GetShadedPrintOut())
    {
        Font.CreateFont(plfFont->lfHeight, 0, 0, 0, FW_BOLD, plfFont->lfItalic, 0, 0,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
#ifndef _WIN32_WCE
            PROOF_QUALITY,
#else
            DEFAULT_QUALITY,
#endif
            VARIABLE_PITCH | FF_SWISS, plfFont->lfFaceName);
    }
    else
        Font.CreateFontIndirect(plfFont);

    pDC->SelectObject(&Font);
    // ***************************************************
    */

    // Draw lines only when wanted on fixed cells.  Normal cell grid lines
    // are handled in OnPrint.
    if(/*pGrid->GetGridLines() != GVL_NONE && */IsFixed())
    {
        CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
             darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
            *pOldPen = pDC->GetCurrentPen();

        pDC->SelectObject(&lightpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.left, rect.top);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(&darkpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);

        rect.DeflateRect(1,1);
        pDC->SelectObject(pOldPen);
    }

    rect.DeflateRect(GetMargin(), 0);
//	if(!IsFixed())
//		rect.left+=GetLeftSpace();

    if(pGrid->GetImageList() && GetImage() >= 0)
    {
        // NOTE: If your printed images look like fuzzy garbage, check the
        //       settings on your printer driver.  If it's using coarse
        //       dithering and/or vector graphics, they may print wrong.
        //       Changing to fine dithering and raster graphics makes them
        //       print properly.  My HP 4L had that problem.

        IMAGEINFO Info;
        if(pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left;
            pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+GetMargin();
        }
    }

    // Draw without clipping so as not to lose text when printed for real
	// DT_NOCLIP removed 01.01.01. Slower, but who cares - we are printing!
    DrawText(pDC->m_hDC, GetText(), -1, rect,
		GetFormat() | /*DT_NOCLIP | */ DT_NOPREFIX);

    pDC->RestoreDC(nSavedDC);

    return TRUE;
#endif
}

/*****************************************************************************
Callable by derived classes, only
*****************************************************************************/
LRESULT CGridCellBase::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    CGridCtrl* pGrid = GetGrid();
    if( pGrid)
        return pGrid->SendMessageToParent(nRow, nCol, nMessage);
    else
        return 0;
}


BOOL CGridCellBase::GetCoveredRowCol(int &nRowChild,int &nColChild)
{
	//nRowChild,int &nColChild
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);
	if(!pGrid)
		return FALSE;

	int nStartRow=max(0,nRowChild-GetCoveredRow());
	int nStartCol=max(0,nColChild-GetCoveredCol());

	for(int nRow=nStartRow;nRow<=nRowChild;nRow++)
		for(int nCol=nStartCol;nCol<=nColChild;nCol++)
		{
			//CGridCellBase *p=pGrid->GetCell(nRow,nCol);
			//if(0==p->GetCoveredCell())
			if(this==pGrid->GetCell(nRow,nCol))
			{

				nRowChild=nRow;
				nColChild=nCol;
				return TRUE;
			}
		}

	SetCovered(0,0);
	//ASSERT(FALSE);//значит некорректные значения переурытых ячеек (не перехвачены все события - таких как вставка строк и т.п.)
	return FALSE;
}
