/////////////////////////////////////////////////////////////////////
// CursorGridCtrl.cpp : implementation file
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Copyright (c) 2005-2008. All Rights Reserved.
// Based on CVirtualGridCtrl class written by Uri Mellshior, (c) 2002-2003.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CursorGridCtrl.h"
#include "MemDC.h"
#include "../Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_LEFT_TEXT_MARGIN 2
#define DEFAULT_RIGHT_TEXT_MARGIN 2
#define DEFAULT_TOP_TEXT_MARGIN 1
#define DEFAULT_BOTTOM_TEXT_MARGIN 3

#define AUTOWIDTH_TEXT_MARGIN_THRESHOLD 25
#define AUTOWIDTH_TEXT_MARGIN 5

#define TIP_TIMEOUT 5000
#define RESIZE_AREA_WIDTH 3
#define RESIZE_MIN_WIDTH 17
#define RESIZE_MAX_WIDTH 10000
#define QUICKSEARCH_TIMEOUT 900
#define TIMER_ID_UPDATE 0x00000001
#define COLOR_ALTERNATION RGB(0xF0, 0xF0, 0xF0)

#define CHECKBOX_PLACE_SIZE 16

#define COLUMN_HELPER_CX (16 + 1 + 1)
#define COLUMN_HELPER_CY (16 + 1 + 1)
#define COLUMN_HELPER_MARGIN 8
#define COLUMN_HELPER_MINROWWIDTH 70

#define TIMER_ID_AUTOHIDE 0x00000002
#define HELPER_DISPLAY_TIMEOUT 900

#define TIMER_ID_SCROLL 0x00000003
#define SCROLL_TIMEOUT 100

#define TIMER_ID_THUMBTRACK 0x00000004
#define THUMBTRACK_LINE_TIMEOUT_MIN 8
#define THUMBTRACK_LINE_TIMEOUT_MAX 256 + THUMBTRACK_LINE_TIMEOUT_MIN
#define THUMBTRACK_PAGE_TIMEOUT_MIN 8
#define THUMBTRACK_PAGE_TIMEOUT_MAX 256 + THUMBTRACK_PAGE_TIMEOUT_MIN
#define THUMBTRACK_LINE_THRESHOLD 2
#define THUMBTRACK_GAP_THRESHOLD 28
#define THUMBTRACK_PAGE_THRESHOLD 32

#define DRAG_SCROLL_FIELD_WIDTH 30
#define DRAG_SCROLL_DELAY 900

#define DEFAULT_SELECTION_PAGE 128

#define MAX_HEAD_ROWS 100
#define MAX_TAIL_ROWS 100

/////////////////////////////////////////////////////////////////////////////
// CGridColumn
/////////////////////////////////////////////////////////////////////////////

CGridColumn::CGridColumn()
{
    m_nMinWidth = 50;
    m_bVisible = TRUE;
    m_HTextAlignment = htaAuto;
    
    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
	m_trpBack = TRANSPARENT_DEFAULT;
    m_pFont = NULL;
    m_pHeaderFont = NULL;
    m_pRowsImages = NULL;
    m_nImageWidth = 0;
    m_bFixedWidth = FALSE;
    m_crHeaderText = CLR_DEFAULT;
    m_crHeaderBack = CLR_DEFAULT;
    m_OrderMarker = omNone;
    m_bChangeVisible = TRUE;
	
	m_Location = clNewColumn;
	m_nLineNumber = 1;
	m_nColumnLineCount = 1;
	m_nMinWidthMul = 0;
	m_nMinWidthDiv = 0;
	m_nFamily = 0;
	m_nWidthBeforeResize = 0;

	m_nFamilyFixedWidth = 0;
	m_nFamilyWidth = 0;
	m_nFamilyResizableWidth = 0;
	m_nFamilyResizeMinWidth = 0;

	m_bMergeCaption = FALSE;
	m_bWordBreak = FALSE;
	m_bMarkNegatives = FALSE;
}

CGridColumn::~CGridColumn()
{
}

void CGridColumn::UpdateColumns(BOOL bRefill /*= FALSE*/)
{
    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    if (pGridColumns)
    {
        pGridColumns->RecalcVisibles();
		CCursorGridCtrl* pGrid = pGridColumns->m_pGrid;
        if (pGrid && !pGrid->m_bUpdateReq)
        {
            if (bRefill)
			{
				pGrid->FillRows_head();
				pGrid->FillRows(pGrid->m_nTopRowIndex, pGrid->m_nVisibleRows);
				pGrid->FillRows_tail();
			}
            pGrid->Invalidate();
        }
    }
}

void CGridColumn::RedrawGrid()
{
    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    if (pGridColumns && m_bVisible)
        if (pGridColumns->m_pGrid)
            pGridColumns->m_pGrid->InvalidateRect(pGridColumns->m_pGrid->GetGridRect());
}

void CGridColumn::RedrawHeader()
{
    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    if (pGridColumns && m_bVisible)
        if (pGridColumns->m_pGrid)
            pGridColumns->m_pGrid->InvalidateRect(GetBoundsRect());
}

CRect CGridColumn::GetBoundsRect()
{
    CRect rect = m_rect;
    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    rect.OffsetRect(-pGridColumns->m_pGrid->GetOrigin(), 0);
    return rect;
}

int CGridColumn::GetWidth() const
{
    return m_nMinWidth;
}

void CGridColumn::SetWidth(int nWidth)
{
    if (nWidth < 0)
        nWidth = 0;
    if (nWidth > 10000)
        nWidth = 10000;
    if (m_nMinWidth == nWidth)
        return;
    m_nMinWidth = nWidth;
    UpdateColumns();
}

int CGridColumn::GetAutoWidth() const
{
	CGridColumns* pColumns = static_cast<CGridColumns*>(GetCollection());
	CCursorGridCtrl* pGrid = pColumns->m_pGrid;
	int nWidth = 0;
	if (pGrid)
		nWidth = pGrid->GetColumnAutoWidth(const_cast<CGridColumn*>(this));

	return nWidth;
}

CString CGridColumn::GetCaption() const
{
    return m_strCaption;
}

void CGridColumn::SetCaption(LPCTSTR szCaption)
{
    if (!m_strCaption.Compare(szCaption))
        return;
    m_strCaption = szCaption;
    RedrawHeader();
}

BOOL CGridColumn::GetVisible() const
{
    return m_bVisible;
}

void CGridColumn::SetVisible(BOOL bVisible)
{
    if (m_bVisible == bVisible)
        return;
    m_bVisible = bVisible;

    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    if (pGridColumns)
    {
        if (m_bVisible)
            pGridColumns->QueryDataField(this);
        else
            pGridColumns->ReleaseDataField(this);
    }
    UpdateColumns(bVisible);
}

HorizTextAlignment CGridColumn::GetHTextAlignment() const
{
    return m_HTextAlignment;
}

void CGridColumn::SetHTextAlignment(HorizTextAlignment textAlignment)
{
    if (m_HTextAlignment == textAlignment)
        return;
    m_HTextAlignment = textAlignment;
    RedrawGrid();
}

COLORREF CGridColumn::GetTextColor() const
{
    return m_crText;
}

void CGridColumn::SetTextColor(COLORREF crTextColor)
{
    if (m_crText == crTextColor)
        return;
    m_crText = crTextColor;
    RedrawGrid();
}

COLORREF CGridColumn::GetBackColor() const
{
	return m_crBack;
}

void CGridColumn::SetBackColor(COLORREF crBackColor)
{
	if (m_crBack == crBackColor)
		return;
	m_crBack = crBackColor;
	RedrawGrid();
}

WORD CGridColumn::GetBackTransparency() const
{
	return m_trpBack;
}

void CGridColumn::SetBackTransparency(WORD trp)
{
	if (m_trpBack == trp)
		return;
	m_trpBack = trp;
	RedrawGrid();
}

CFont* CGridColumn::GetFont() const
{
    return m_pFont;
}

void CGridColumn::SetFont(CFont* pFont)
{
    if (m_pFont == pFont)
        return;
    m_pFont = pFont;
    RedrawGrid();
}

CFont* CGridColumn::GetHeaderFont() const
{
    return m_pHeaderFont;
}

void CGridColumn::SetHeaderFont(CFont* pFont)
{
    if (m_pHeaderFont == pFont)
        return;
    m_pHeaderFont = pFont;
    RedrawHeader();
}

CImageList* CGridColumn::GetRowsImages() const
{
    return m_pRowsImages;
}

void CGridColumn::SetRowsImages(CImageList* pRowsImages)
{
    if (m_pRowsImages == pRowsImages)
        return;
    m_pRowsImages = pRowsImages;
    if (m_pRowsImages)
    {
        IMAGEINFO imageInfo;
        if (m_pRowsImages->GetImageInfo(0, &imageInfo))
            m_nImageWidth = imageInfo.rcImage.right - imageInfo.rcImage.left;
        else
            m_nImageWidth = 0;
    }
    RedrawGrid();
}

BOOL CGridColumn::GetFixedWidth() const
{
    return m_bFixedWidth;
}

void CGridColumn::SetFixedWidth(BOOL bFixedWidth /*= TRUE*/)
{
    if (m_bFixedWidth == bFixedWidth)
        return;
    m_bFixedWidth = bFixedWidth;
    UpdateColumns();
}

COLORREF CGridColumn::GetHeaderTextColor() const
{
    return m_crHeaderText;
}

void CGridColumn::SetHeaderTextColor(COLORREF crTextColor)
{
    if (m_crHeaderText == crTextColor)
        return;
    m_crHeaderText = crTextColor;
    RedrawHeader();
}

COLORREF CGridColumn::GetHeaderBackColor() const
{
    return m_crHeaderBack;
}

void CGridColumn::SetHeaderBackColor(COLORREF crBackColor)
{
    if (m_crHeaderBack == crBackColor)
        return;
    m_crHeaderBack = crBackColor;
    RedrawHeader();
}

void CGridColumn::aux_SetDataField(CString& csDataField, LPCSTR szDataField)
{
	if (!_stricmp(csDataField, szDataField))
		return;

	CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());

	if (m_bVisible && pGridColumns)
		pGridColumns->ReleaseDataField(this);

	csDataField = szDataField;

	if (m_bVisible && pGridColumns)
		pGridColumns->QueryDataField(this);
}

OrderMarker CGridColumn::GetOrderMarker() const
{
    return m_OrderMarker;
}

void CGridColumn::SetOrderMarker(OrderMarker orderMarker)
{
    if (m_OrderMarker == orderMarker)
        return;
    m_OrderMarker = orderMarker;
    RedrawHeader();
}

BOOL CGridColumn::GetChangeVisible() const
{
    return m_bChangeVisible;
}

void CGridColumn::SetChangeVisible(BOOL bChangeVisible /*= TRUE*/)
{
    if (m_bChangeVisible == bChangeVisible)
        return;
    m_bChangeVisible = bChangeVisible;
    if (!m_bVisible)
    {
        CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
        if (pGridColumns && pGridColumns->m_pGrid)
        {
            if (pGridColumns->m_pGrid->m_ColumnHelper.IsDisplay())
                pGridColumns->m_pGrid->m_ColumnHelper.Show();
        }
    }
}

ColumnLocation CGridColumn::GetLocation() const
{
	return m_Location;
}
void CGridColumn::SetLocation(ColumnLocation Location)
{
	if (m_Location == Location)
		return;
	m_Location = Location;
	UpdateColumns();
}

BOOL CGridColumn::GetMergeCaption() const
{
	return m_bMergeCaption;
}

void CGridColumn::SetMergeCaption(BOOL bMergeCaption)
{
	if (m_bMergeCaption == bMergeCaption)
		return;
	m_bMergeCaption = bMergeCaption;
	RedrawGrid();
}

BOOL CGridColumn::GetWordBreak() const
{
	return m_bWordBreak;
}

void CGridColumn::SetWordBreak(BOOL bWordBreak)
{
	if (m_bWordBreak == bWordBreak)
		return;
	m_bWordBreak = bWordBreak;
	RedrawGrid();
}

BOOL CGridColumn::GetMarkNegatives() const
{
	return m_bMarkNegatives;
}

void CGridColumn::SetMarkNegatives(BOOL bMark)
{
	if (m_bMarkNegatives == bMark)
		return;
	m_bMarkNegatives = bMark;
	RedrawGrid();
}

/////////////////////////////////////////////////////////////////////////////
// CGridColumns
/////////////////////////////////////////////////////////////////////////////

CGridColumns::CGridColumns()
{
    m_pGrid = NULL;
    m_nMinWidth = 0;
    m_nResizableMinWidth = 0;
}

CGridColumns::~CGridColumns()
{
    Clear();
}

void CGridColumns::OnAdd(CCollectionItem* pItem)
{
    CGridColumn* pGridColumn = static_cast<CGridColumn*>(pItem);
    QueryDataField(pGridColumn);
    RecalcVisibles();
	m_pGrid->FillRows_head();
    m_pGrid->FillRows(m_pGrid->m_nTopRowIndex, m_pGrid->m_nVisibleRows);
	m_pGrid->FillRows_tail();
}

void CGridColumns::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
    CGridColumn* pGridColumn = static_cast<CGridColumn*>(pItem);
    ReleaseDataField(pGridColumn);
    pGridColumn->Release();

    if (nIndexWas != -1)
    {
        for (int i = 0; i < m_pGrid->m_rows.GetSize(); i++)
        {
            CGridRow* pGridRow = m_pGrid->m_rows[i];
			if (nIndexWas < pGridRow->m_cells.GetSize())
			{
				delete pGridRow->m_cells[nIndexWas];
				pGridRow->m_cells.RemoveAt(nIndexWas);
			}
        }
		for (int i = 0; i < m_pGrid->m_rows_head.GetSize(); i++)
		{
			CGridRow* pGridRow = m_pGrid->m_rows_head[i];
			if (nIndexWas < pGridRow->m_cells.GetSize())
			{
				delete pGridRow->m_cells[nIndexWas];
				pGridRow->m_cells.RemoveAt(nIndexWas);
			}
		}
		for (int i = 0; i < m_pGrid->m_rows_tail.GetSize(); i++)
		{
			CGridRow* pGridRow = m_pGrid->m_rows_tail[i];
			if (nIndexWas < pGridRow->m_cells.GetSize())
			{
				delete pGridRow->m_cells[nIndexWas];
				pGridRow->m_cells.RemoveAt(nIndexWas);
			}
		}
        if (nIndexWas > GetUpperBound() && nIndexWas == m_pGrid->m_nCurrentColumn)
            m_pGrid->m_nCurrentColumn = -1;
        RecalcVisibles();
    }
}

void CGridColumns::OnRename(CCollectionItem* pItem)
{
}

int CGridColumns::GetSize() const
{
    return GetCount();
}

CGridColumn* CGridColumns::Add(LPCSTR szName)
{
    CGridColumn* pGridColumn = static_cast<CGridColumn*>(GetItem(szName));
	if (!pGridColumn)
	{
		pGridColumn = NewColumn();
		pGridColumn->SetName(szName);
		CCollection::Add(pGridColumn);
	}
    return pGridColumn;
}

CGridColumn* CGridColumns::Insert(int nIndex, LPCSTR szName)
{
    if (nIndex < 0 || nIndex > GetCount())
        return NULL;

	CGridColumn* pGridColumn = static_cast<CGridColumn*>(GetItem(szName));
	if (!pGridColumn)
	{
		pGridColumn = NewColumn();
		pGridColumn->SetName(szName);
		CCollection::Insert(nIndex, pGridColumn);
	}
    return pGridColumn;
}

CGridColumn* CGridColumns::GetAt(int nIndex) const
{
    if (nIndex < 0 || nIndex > GetUpperBound())
        return NULL;
    else
        return static_cast<CGridColumn*>(GetItem(nIndex));
}

CGridColumn* CGridColumns::GetAt(LPCSTR szName) const
{
    CCollectionItem* pCollectionItem = GetItem(szName);
    if (pCollectionItem)
        return static_cast<CGridColumn*>(pCollectionItem);
    else
        return NULL;
}

int CGridColumns::IndexOf(LPCSTR szName) const
{
    return GetIndex(szName);
}

int CGridColumns::IndexOf(CGridColumn* pColumn) const
{
    return GetIndex(pColumn);
}

BOOL CGridColumns::Remove(int nIndex)
{
    if (nIndex < 0 || nIndex > GetUpperBound())
        return FALSE;

    CCollection::Remove(nIndex);
    return TRUE;
}

void CGridColumns::RecalcVisibles()
{
	if (m_pGrid)
	{
		m_pGrid->RecalcColumnsLineCount();
		m_pGrid->RecalcColumnsWidthMulDiv();
	}

    m_nMinWidth = 0;
    m_nResizableMinWidth = 0;

	int nCurFamily = -1;
	
    for (int i = 0; i < GetCount(); i++)
    {
        CGridColumn* pColumn = static_cast<CGridColumn*>(GetItem(i));
        if (pColumn->m_bVisible)
        {
			if (nCurFamily != pColumn->m_nFamily)
			{
				nCurFamily = pColumn->m_nFamily;
				m_nMinWidth += pColumn->m_nFamilyWidth;
				m_nResizableMinWidth += pColumn->m_nFamilyResizableWidth;
			}
        }
    };

    if (m_pGrid)
        m_pGrid->ColumnsChanged();
}

void CGridColumns::RemoveAll()
{
    Clear();
    for (int i = 0; i < m_pGrid->m_rows.GetSize(); i++)
        m_pGrid->m_rows[i]->m_cells.RemoveAll();
    
    m_nMinWidth = 0;
    m_nResizableMinWidth = 0;
    m_pGrid->m_nCurrentColumn = -1;
    m_pGrid->Invalidate();
}

void CGridColumns::QueryDataField(CGridColumn* pColumn) const
{
	if (m_pGrid)
	{
		QueryDataField(pColumn, m_pGrid->m_pProvider);
		QueryDataField(pColumn, m_pGrid->m_pProvider_head);
		QueryDataField(pColumn, m_pGrid->m_pProvider_tail);
	}
}

void CGridColumns::QueryDataField(CGridColumn* pColumn, CDataProvider* pProvider) const
{
	if (pProvider)
	{
		if (!pColumn->m_strDataField.IsEmpty())
			pProvider->QueryField(pColumn->m_strDataField);
		if (!pColumn->m_strImageDataField.IsEmpty())
			pProvider->QueryField(pColumn->m_strImageDataField);
	}
}

void CGridColumns::ReleaseDataField(CGridColumn* pColumn) const
{
    if (m_pGrid)
	{
		ReleaseDataField(pColumn, m_pGrid->m_pProvider);
		ReleaseDataField(pColumn, m_pGrid->m_pProvider_head);
		ReleaseDataField(pColumn, m_pGrid->m_pProvider_tail);
	}
}

void CGridColumns::ReleaseDataField(CGridColumn* pColumn, CDataProvider* pProvider) const
{
	if (pProvider)
	{
		if (!pColumn->m_strDataField.IsEmpty())
			pProvider->ReleaseField(pColumn->m_strDataField);
		if (!pColumn->m_strImageDataField.IsEmpty())
			pProvider->ReleaseField(pColumn->m_strImageDataField);
	}
}

BOOL CGridColumns::Move(int nIndex, int nOffset)
{
    if (!m_pGrid)
        return TRUE;
    
    if (nIndex < 0 || nIndex >= GetCount() || nOffset == 0)
        return FALSE;
    int nNewIndex = nIndex + nOffset;
    if (nNewIndex < 0 || nNewIndex >= GetCount())
        return FALSE;
    CCollection::Move(nIndex, nOffset);
	{
		for (int i = 0; i < m_pGrid->m_rows.GetSize(); i++)
			m_pGrid->m_rows[i]->Move(nIndex, nOffset);
	}
	{
		for (int i = 0; i < m_pGrid->m_rows_head.GetSize(); i++)
			m_pGrid->m_rows_head[i]->Move(nIndex, nOffset);
	}
	{
		for (int i = 0; i < m_pGrid->m_rows_tail.GetSize(); i++)
			m_pGrid->m_rows_tail[i]->Move(nIndex, nOffset);
	}
	RecalcVisibles();
    return TRUE;
}

int CGridColumns::GetVisibleWidth() const
{
	int nWidth = 0;
	for (int i = 0; i < GetSize(); i++)
	{
		CGridColumn* pGridColumn = GetAt(i);
		if ( pGridColumn->m_bVisible && pGridColumn->m_nLineNumber==1 )
			nWidth += pGridColumn->m_rect.Width();
	}
	return nWidth;
}

/////////////////////////////////////////////////////////////////////////////
// CGridItem
/////////////////////////////////////////////////////////////////////////////

#define CGI_TEXT                0x0001
#define CGI_IMAGE               0x0002
#define CGI_CHECKBOX            0x0004
#define CGI_HINT                0x0100
#define CGI_SYSIMAGE            0x0200
#define CGI_PICMASK             CGI_IMAGE|CGI_CHECKBOX|CGI_SYSIMAGE

#define CGI_TEXT_AUTO           0x0000
#define CGI_TEXT_LEFT           0x0008
#define CGI_TEXT_RIGHT          0x0010
#define CGI_TEXT_CENTER         0x0018
#define CGI_TEXT_H_ALIGN        0x0018

#define CGI_CHECKBOX_UNCHECKED  0x0000
#define CGI_CHECKBOX_CHECKED    0x0020
#define CGI_CHECKBOX_UNDEFINED  0x0040
#define CGI_CHECKBOX_STATE      0x0060

#define CGI_TEXT_WORDBREAK      0x0080

CGridItem::CGridItem(CGridColumn* pGridColumn) : m_pGridColumn(pGridColumn)
{
    m_wFlagsMask = 0;

    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
	m_trpBack = TRANSPARENT_DEFAULT;
    m_pFont = NULL;
	m_bIsNegative = FALSE;
	m_nNoTextOffset = 0;
}

CGridItem::~CGridItem()
{
}

CGridColumn* CGridItem::GetColumn() const
{
    return m_pGridColumn;
}

BOOL CGridItem::GetShowText() const
{
    return (m_wFlagsMask & CGI_TEXT) != 0;
}

void CGridItem::SetShowText(BOOL bEnable /*= TRUE*/)
{
    if (bEnable)
        m_wFlagsMask |= CGI_TEXT;
    else
        m_wFlagsMask &= ~CGI_TEXT;
}

BOOL CGridItem::GetShowImage() const
{
	return (m_wFlagsMask & CGI_IMAGE) != 0;
}

void CGridItem::SetShowImage(BOOL bEnable /*= TRUE*/)
{
	if (bEnable)
		m_wFlagsMask |= CGI_IMAGE;
	else
		m_wFlagsMask &= ~CGI_IMAGE;
}

BOOL CGridItem::GetShowSysImage() const
{
	return (m_wFlagsMask & CGI_SYSIMAGE) != 0;
}

void CGridItem::SetShowSysImage(BOOL bEnable /*= TRUE*/)
{
	if (bEnable)
		m_wFlagsMask |= CGI_SYSIMAGE;
	else
		m_wFlagsMask &= ~CGI_SYSIMAGE;
}

BOOL CGridItem::GetShowCheckBox() const
{
    return (m_wFlagsMask & CGI_CHECKBOX) != 0;
}

void CGridItem::SetShowCheckBox(BOOL bEnable /*= TRUE*/)
{
    if (bEnable)
        m_wFlagsMask |= CGI_CHECKBOX;
    else
        m_wFlagsMask &= ~CGI_CHECKBOX;
}

LPCSTR CGridItem::GetText() const
{
    return m_strText;
}

void CGridItem::SetText(LPCSTR szText)
{
    m_strText = szText;
}

HorizTextAlignment CGridItem::GetTextHAlign() const
{
    return (HorizTextAlignment)((m_wFlagsMask & CGI_TEXT_H_ALIGN) >> 3);
}

void CGridItem::SetTextHAlign(HorizTextAlignment horizTextAlignment)
{
    m_wFlagsMask &= ~CGI_TEXT_H_ALIGN;
    m_wFlagsMask |= (horizTextAlignment << 3);
}

void CGridItem::InitImageRefs()
{
	if (!m_pImageNumbers)
		m_pImageNumbers.reset(NewImageRefs());
	if (!m_pSysImageNumbers)
		m_pSysImageNumbers.reset(NewImageRefs());
}

int CGridItem::GetImageIndex(BOOL bSys /*= FALSE*/) const
{
	IMAGEREFS* p = bSys ? m_pSysImageNumbers.get() : m_pImageNumbers.get();

	if (p->empty())
		return -1;
	else
		return p->operator[](0);
}

int CGridItem::GetImageIndex(CString& csImageIndex, BOOL bSys /*= FALSE*/) const
{
	IMAGEREFS* p = bSys ? m_pSysImageNumbers.get() : m_pImageNumbers.get();

	csImageIndex.Empty();
	if (p->empty())
		return -1;
	else if (p->GetSize() == 1)
		return p->operator[](0);
	else
	{
		IMAGEREFS::iterator end = p->end();
		for (IMAGEREFS::iterator i = p->begin(); i != end; ++i)
		{
			CString cs;
			cs.Format("%i,", *i);
			csImageIndex += cs;
		}
		csImageIndex.TrimRight(',');
		return -2;
	}
}

void CGridItem::SetImageIndex(int nImageIndex, BOOL bSys /*= FALSE*/)
{
	IMAGEREFS* p = bSys ? m_pSysImageNumbers.get() : m_pImageNumbers.get();

	p->clear();
	if (nImageIndex >= 0)
		p->push_back(nImageIndex);
}

void CGridItem::SetImageIndex(LPCSTR szImageIndex, BOOL bSys /*= FALSE*/)
{
	IMAGEREFS* p = bSys ? m_pSysImageNumbers.get() : m_pImageNumbers.get();

	LPCSTR szDelims = ",";
	p->clear();

	CString csImageIndex(szImageIndex);
	csImageIndex.TrimRight();
	csImageIndex.TrimLeft();
	LPSTR szBuf = csImageIndex.GetBuffer(0);
	LPSTR szToken = strtok(szBuf, szDelims);
	while (szToken)
	{
		CString csToken(szToken);
		csToken.TrimRight();
		csToken.TrimLeft();
		if (!csToken.IsEmpty())
		{
			int nIndex = atoi((LPCSTR)csToken);
			if (nIndex >= 0)
				p->push_back(nIndex);
		}
		szToken = strtok(NULL, szDelims);
	}
	csImageIndex.ReleaseBuffer(0);
}

CGridItem::IMAGEREFS* CGridItem::GetImageRefs(BOOL bSys /*= FALSE*/)
{
	IMAGEREFS* p = bSys ? m_pSysImageNumbers.get() : m_pImageNumbers.get();

	return p;
}

int CGridItem::GetCheckValue() const
{
    return (m_wFlagsMask & CGI_CHECKBOX_STATE) >> 5;
}

void CGridItem::SetCheckValue(int nCheckValue)
{
    m_wFlagsMask &= ~CGI_CHECKBOX_STATE;
    m_wFlagsMask |= (nCheckValue << 5);
}

COLORREF CGridItem::GetTextColor() const
{
    return m_crText;
}

void CGridItem::SetTextColor(COLORREF crTextColor)
{
    m_crText = crTextColor;
}

COLORREF CGridItem::GetBackColor() const
{
    return m_crBack;
}

void CGridItem::SetBackColor(COLORREF crBackColor)
{
    m_crBack = crBackColor;
}

WORD CGridItem::GetBackTransparency() const
{
	return m_trpBack;
}

void CGridItem::SetBackTransparency(WORD trp)
{
	m_trpBack = trp;
}

CFont* CGridItem::GetFont() const
{
    return m_pFont;
}

void CGridItem::SetFont(CFont* pFont)
{
    m_pFont = pFont;
}

BOOL CGridItem::GetWordBreak() const
{
	return (m_wFlagsMask & CGI_TEXT_WORDBREAK) != 0;
}

void CGridItem::SetWordBreak(BOOL bWordBreak /*= TRUE*/)
{
	if (bWordBreak)
		m_wFlagsMask |= CGI_TEXT_WORDBREAK;
	else
		m_wFlagsMask &= ~CGI_TEXT_WORDBREAK;
}

BOOL CGridItem::GetIsNegative() const
{
	return m_bIsNegative;
}
void CGridItem::SetIsNegative(BOOL bIs)
{
	m_bIsNegative = bIs;
}

BOOL CGridItem::GetShowHint() const
{
	return (m_wFlagsMask & CGI_HINT) != 0;
}

void CGridItem::SetShowHint(BOOL bEnable /*= TRUE*/)
{
	if (bEnable)
		m_wFlagsMask |= CGI_HINT;
	else
		m_wFlagsMask &= ~CGI_HINT;
}

LPCSTR CGridItem::GetHint() const
{
	return m_strHint;
}

void CGridItem::SetHint(LPCSTR szHint)
{
	m_strHint = szHint;
}

/////////////////////////////////////////////////////////////////////////////
// CGridRow
/////////////////////////////////////////////////////////////////////////////

CGridRow::CGridRow(CGridColumns* pColumns, CDataRowPtr const& pDataRow, RegionType rType)
    : m_pColumns(pColumns), m_pDataRow(pDataRow), m_rType(rType)
{
    m_bIsCurrent = false;

    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
	m_trpBack = TRANSPARENT_DEFAULT;
    m_pFont = NULL;
}

CGridRow::~CGridRow()
{
    for (int i = 0; i < m_cells.GetSize(); i++)
        delete m_cells[i];
    
    if (m_bIsCurrent)
    {
        m_pColumns->m_pGrid->m_nCurrentDataRowIndex = -1;
        m_pColumns->m_pGrid->m_pCurrentDataRowOutOfWindow = m_pDataRow;
    }
}

CGridItem* CGridRow::NewItem(CGridColumn* pGridColumn)
{
	CGridItem* pItem = new CGridItem(pGridColumn);
	pItem->InitImageRefs();

	return pItem;
}

BOOL CGridRow::CleanUp()
{
    if (!m_cells.GetSize())
        return FALSE;

    for (int i = 0; i < m_cells.GetSize(); i++)
        delete m_cells[i];
    m_cells.RemoveAll();
    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
	m_trpBack = TRANSPARENT_DEFAULT;
    m_pFont = NULL;

    return TRUE;
}

void CGridRow::CreateItems()
{
	CleanUp();
	for (int i = 0; i < m_pColumns->GetSize(); i++)
		m_cells.Add(NewItem(m_pColumns->GetAt(i)));
}

void CGridRow::FillItems()
{
	CreateItems();

    for (int i = 0; i < m_cells.GetSize(); i++)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
        if (!pColumn->m_bVisible)
            continue;

		if (!pColumn->m_strDataField.IsEmpty())
		{
			int nIndex = m_pDataRow->GetFieldIndexOf(pColumn->m_strDataField);
			if (nIndex != -1)
			{
				CGridItem* pGridItem = m_cells[i];
				HorizTextAlignment HAlign = htaAuto;
				DataType FieldDataType = m_pDataRow->GetFieldType(nIndex);

				if (GetGrid()->FormatField(GetDataRow(), nIndex, pColumn, pGridItem->m_strText))
				{
					pGridItem->SetShowText();
					HAlign = FieldDataType == dtNumeric ? htaRight : htaLeft;
				}
				else
				{
					switch (FieldDataType)
					{
					case dtUndefined:
						if (!pGridItem->GetShowText())
						{
							pGridItem->SetShowText();
							m_pDataRow->FormatField(nIndex, pGridItem->m_strText);
						}
						HAlign = htaLeft;
						break;
					case dtText:
						if (!pGridItem->GetShowText())
						{
							pGridItem->SetShowText();
							m_pDataRow->FormatField(nIndex, pGridItem->m_strText);
							pGridItem->m_strText.TrimRight();
						}
						HAlign = htaLeft;
						break;
					case dtNumeric:
						if (!pGridItem->GetShowText())
						{
							pGridItem->SetShowText();
							m_pDataRow->FormatField(nIndex, pGridItem->m_strText);
						}
						HAlign = htaRight;
						break;
					case dtDate:
						if (!pGridItem->GetShowText())
						{
							pGridItem->SetShowText();
							m_pDataRow->FormatField(nIndex, pGridItem->m_strText);
						}
						HAlign = htaLeft;
						break;
					case dtTrue:
						pGridItem->SetShowCheckBox();
						pGridItem->SetCheckValue(1);
						break;
					case dtFalse:
						pGridItem->SetShowCheckBox();
						pGridItem->SetCheckValue(0);
						break;
					}
				}
				if (pGridItem->GetShowText())
				{
					CString str(pGridItem->m_strText);
					str.TrimLeft();
					pGridItem->SetIsNegative(!str.Left(1).Compare("-"));
				}

				if (pGridItem->GetTextHAlign() == htaAuto)
				{
					if (pColumn->m_HTextAlignment != htaAuto)
						pGridItem->SetTextHAlign(pColumn->m_HTextAlignment);
					else
						pGridItem->SetTextHAlign(HAlign);
				}
				pGridItem->SetWordBreak(pColumn->m_bWordBreak);
			}
		}
		if (!pColumn->m_strImageDataField.IsEmpty())
		{
			int nIndex = m_pDataRow->GetFieldIndexOf(pColumn->m_strImageDataField);
			if (nIndex != -1)
			{
				CGridItem* pGridItem = m_cells[i];
				if (!pGridItem->GetShowImage())
				{
					pGridItem->SetShowImage();
					CString csImageText;
					m_pDataRow->FormatField(nIndex, csImageText);
					pGridItem->SetImageIndex(csImageText);
				}
			}
		}
	}
 	OnFillItems();
}

CCursorGridCtrl* CGridRow::GetGrid() const
{
    return m_pColumns->m_pGrid;
}

CDataRowPtr const& CGridRow::GetDataRowPtr() const
{
	return m_pDataRow;
}

CDataRow* CGridRow::GetDataRow() const
{
    return m_pDataRow.get();
}

CGridItem* CGridRow::GetGridItem(int nIndex) const
{
    return m_cells[nIndex];
}

CGridItem* CGridRow::GetGridItem(CGridColumn* pColumn) const
{
	int nColIndex = m_pColumns->GetIndex(pColumn);
	if (nColIndex != -1)
		return GetGridItem(nColIndex);
	else
		return NULL;
}

COLORREF CGridRow::GetTextColor() const
{
    return m_crText;
}

void CGridRow::SetTextColor(COLORREF crTextColor)
{
    m_crText = crTextColor;
}

COLORREF CGridRow::GetBackColor() const
{
    return m_crBack;
}

void CGridRow::SetBackColor(COLORREF crBackColor)
{
    m_crBack = crBackColor;
}

WORD CGridRow::GetBackTransparency() const
{
	return m_trpBack;
}

void CGridRow::SetBackTransparency(WORD trp)
{
	m_trpBack = trp;
}

CFont* CGridRow::GetFont() const
{
    return m_pFont;
}

void CGridRow::SetFont(CFont* pFont)
{
    m_pFont = pFont;
}

void CGridRow::Move(int nIndex, int nOffset)
{
    if (!m_cells.GetSize())
        return;
    
    CGridItem* pItem = m_cells[nIndex];
    m_cells.RemoveAt(nIndex);
    m_cells.InsertAt(nIndex + nOffset, pItem);
}

/////////////////////////////////////////////////////////////////////////////
// CDataRowState
/////////////////////////////////////////////////////////////////////////////

CDataRowState::CDataRowState(CCursorGridCtrl* pGrid)
{
	CSelectedRows* pSelectedRows = &pGrid->m_SelectedRows;

	m_pDataRow = pGrid->GetCurrentDataRowPtr();
	m_nDataRowIndex = pGrid->m_nCurrentDataRowIndex;
	m_nDataRowDirection = pGrid->m_nCurrentDataRowDirection;
	m_nSelectDirection = pSelectedRows->GetDirection();
	m_nSize = pSelectedRows->GetSize();
	if (m_pDataRow)
	{
		m_bIsRemoved = pSelectedRows->IsRemoved(m_pDataRow.get());
		m_bIsCurrent = pSelectedRows->IsCurrent(m_pDataRow.get());
	}
	else
	{
		m_bIsRemoved = FALSE;
		m_bIsCurrent = FALSE;
	}
}

CDataRowState::~CDataRowState()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSelectedRows
/////////////////////////////////////////////////////////////////////////////

CSelectedRows::CSelectedRows() : m_pProvider(NULL), m_nDirection(0)
{
}

CSelectedRows::~CSelectedRows()
{
	SetDataProvider(NULL);
}

void CSelectedRows::SetDataProvider(CDataProvider* pProvider)
{
	RemoveAll();
	m_pProvider = pProvider;
}

BOOL CSelectedRows::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
	return m_pProvider && m_pProvider->CompareRows(pRow1, pRow2);
}

BOOL CSelectedRows::IsSelected(CDataRow* pDataRow) const
{
	for (int i = 0; i < m_rows.size(); i++)
	{
		if (CompareRows(pDataRow, m_rows[i].get()))
			return TRUE;
	}

	return FALSE;
}

BOOL CSelectedRows::IsRemoved(CDataRow* pDataRow) const
{
	return m_pRemovedRow && CompareRows(pDataRow, m_pRemovedRow.get());
}

BOOL CSelectedRows::IsCurrent(CDataRow* pDataRow) const
{
	return m_pCurrentRow && CompareRows(pDataRow, m_pCurrentRow.get());
}

void CSelectedRows::Add(CDataRowPtr const& pDataRow, BOOL bCheckIsSelected)
{
	if (!bCheckIsSelected || !IsSelected(pDataRow.get()))
		m_rows.push_back(pDataRow);

	SetRemovedRow(CDataRowPtr());
}

BOOL CSelectedRows::Remove(CDataRowPtr const& pDataRow)
{
	BOOL bSuccess = FALSE;

	for (int i = m_rows.size() - 1; i >= 0; i--)
	{
		if (CompareRows(pDataRow.get(), m_rows[i].get()))
		{
			m_rows.erase(m_rows.begin() + i);
			bSuccess = TRUE;
		}
	}

	if (bSuccess)
		SetRemovedRow(pDataRow);

	return bSuccess;
}

BOOL CSelectedRows::Toggle(CDataRowPtr const& pDataRow)
{
	BOOL bResult = FALSE;
	if (!Remove(pDataRow))
	{
		Add(pDataRow, FALSE);
		bResult = TRUE;
	}
	return bResult;
}

void CSelectedRows::RemoveAll()
{
	m_rows.swap(std::vector<CDataRowPtr>());
	SetRemovedRow(CDataRowPtr());
	SetCurrentRow(CDataRowPtr());
	SetDirection(0);
}

void CSelectedRows::SetRemovedRow(CDataRowPtr const& pDataRow)
{
	m_pRemovedRow = pDataRow;
}

void CSelectedRows::SetCurrentRow(CDataRowPtr const& pDataRow)
{
	m_pCurrentRow = pDataRow;
}

int CSelectedRows::GetSize() const
{
	return m_rows.size();
}

CDataRow* CSelectedRows::GetAt(int nIndex) const
{
	CDataRow* pDataRow = NULL;
	if (nIndex >= 0 && nIndex < m_rows.size())
		pDataRow = m_rows[nIndex].get();

	return pDataRow;
}

//////////////////////////////////////////////////////////////////////////
// CThumbTracker
//////////////////////////////////////////////////////////////////////////

void CThumbTracker::Track(int nPos)
{
	m_nTrackPos = nPos;
	if (!m_bTracking)
	{
		m_bTracking = TRUE;
		SetTimer(CalcTimeout(0));
	}
}

void CThumbTracker::Reset()
{
	if (m_bTracking)
	{
		m_bTracking = FALSE;
		KillTimer();
	}
}

void CThumbTracker::SetCurrentPos(int nPos)
{
	if (!m_bTracking)
		m_nStartPos = nPos;
}

void CThumbTracker::OnTimer()
{
	int nDelta = GetDelta();
	int nTimeout = CalcTimeout(nDelta);
	if (nTimeout != m_nTimeout)
	{
		KillTimer();
		SetTimer(nTimeout);
	}

	if (nDelta > THUMBTRACK_PAGE_THRESHOLD)
		m_pGrid->ScrollDown(m_pGrid->GetPageSize());
	else if (nDelta > THUMBTRACK_GAP_THRESHOLD);
	else if (nDelta > THUMBTRACK_LINE_THRESHOLD)
		m_pGrid->ScrollDown(1);
	else if (nDelta < -THUMBTRACK_PAGE_THRESHOLD)
		m_pGrid->ScrollUp(m_pGrid->GetPageSize());
	else if (nDelta < -THUMBTRACK_GAP_THRESHOLD);
	else if (nDelta < -THUMBTRACK_LINE_THRESHOLD)
		m_pGrid->ScrollUp(1);
}

void CThumbTracker::SetTimer(int nTimeout)
{
	m_nTimeout = nTimeout;
	m_pGrid->SetTimer(TIMER_ID_THUMBTRACK, nTimeout, NULL);
}

void CThumbTracker::KillTimer() const
{
	m_pGrid->KillTimer(TIMER_ID_THUMBTRACK);
}

#define GET_MIDDLE(n, x, y, mx, my) ((y)-(n))*((my)-(mx))/((y)-(x))+(mx)
int CThumbTracker::CalcTimeout(int nDelta) const
{
	nDelta = min(abs(nDelta), 50);
	if (nDelta < THUMBTRACK_LINE_THRESHOLD)
		return THUMBTRACK_LINE_TIMEOUT_MAX;
	else if (nDelta < THUMBTRACK_GAP_THRESHOLD)
		return GET_MIDDLE(nDelta, THUMBTRACK_LINE_THRESHOLD, THUMBTRACK_GAP_THRESHOLD, THUMBTRACK_LINE_TIMEOUT_MIN, THUMBTRACK_LINE_TIMEOUT_MAX);
	else if (nDelta < THUMBTRACK_PAGE_THRESHOLD)
		return THUMBTRACK_LINE_TIMEOUT_MIN;
	else
		return GET_MIDDLE(nDelta, THUMBTRACK_PAGE_THRESHOLD, 50, THUMBTRACK_PAGE_TIMEOUT_MIN, THUMBTRACK_PAGE_TIMEOUT_MAX);
}

int CThumbTracker::GetDelta()
{
	int nDelta = m_nTrackPos - m_nStartPos;
	if (nDelta > 50)
		m_nStartPos = m_nTrackPos - 50;
	else if (nDelta < -50)
		m_nStartPos = m_nTrackPos + 50;
	return nDelta;
}

/////////////////////////////////////////////////////////////////////////////
// CGridTip window
/////////////////////////////////////////////////////////////////////////////

CGridTip::CGridTip(CCursorGridCtrl* pGrid): m_pGrid(pGrid)
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    if(!(::GetClassInfo(hInst, GRIDTIP_CLASSNAME, &wndcls)))
    {
        wndcls.style			= CS_SAVEBITS;
        wndcls.lpfnWndProc		= ::DefWindowProc;
        wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
        wndcls.hInstance		= hInst;
        wndcls.hIcon			= NULL;
        wndcls.hCursor			= LoadCursor( hInst, IDC_ARROW );
        wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK +1);
        wndcls.lpszMenuName		= NULL;
        wndcls.lpszClassName	= GRIDTIP_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
            AfxThrowResourceException();
    }
	m_dwTipedID = 0;

    m_bCreated = FALSE;
    m_crTextColor = CLR_DEFAULT;
    m_crBackColor = CLR_DEFAULT;
}

CGridTip::~CGridTip()
{
    if (m_bCreated)
        DestroyWindow();
}

BEGIN_MESSAGE_MAP(CGridTip, CWnd)
	//{{AFX_MSG_MAP(CGridTip)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGridTip::DestroyWindow() 
{
    m_bCreated = FALSE;
    return CWnd::DestroyWindow();
}

void CGridTip::Show(const CString& strText, LPRECT pTextRect, DWORD dwTipID,
                    CFont* pFont, BOOL bWordBreak, BOOL bByCursor, BOOL bForceHint)
{
    if (!m_bCreated)
    {
        ASSERT_VALID(m_pParentWnd);
        m_bCreated = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, GRIDTIP_CLASSNAME, NULL, WS_POPUP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL);
        if (!m_bCreated)
            return;
    }

    if (m_dwTipedID == dwTipID)
        return;
    else
        m_dwTipedID = dwTipID;

    if (strText.IsEmpty())
        return;

    CRect TextRect(pTextRect);

    if (TextRect.IsRectEmpty())
        return;

    if (IsWindowVisible())
        return;

    if (GetFocus() == NULL)
        return;

    m_rectHover = TextRect;
    m_pGrid->ClientToScreen(m_rectHover);

    CClientDC dc(this);
    CFont *pOldFont;
    pOldFont = dc.SelectObject(pFont);

	int nOutWidth = TextRect.Width() - m_pGrid->GetLeftTextMargin() - m_pGrid->GetRightTextMargin();
	int nOutHeight = TextRect.Height()- m_pGrid->GetTopTextMargin() - m_pGrid->GetBottomTextMargin();
	CRect OutRect(0,0, nOutWidth, nOutHeight);

	UINT nFlags = DT_CALCRECT | DT_NOPREFIX | DT_NOCLIP;
	if (bWordBreak)
		nFlags |= DT_WORDBREAK;
	dc.DrawText(strText, OutRect, nFlags);
	CSize TextSize = OutRect.Size();

    if (bForceHint || TextSize.cx > nOutWidth || TextSize.cy > nOutHeight)
    {
        CRect rectDisplay;
        int nWidth = TextSize.cx + m_pGrid->GetLeftTextMargin() + m_pGrid->GetRightTextMargin() - 1;
        int nHeight = TextSize.cy + m_pGrid->GetTopTextMargin() + m_pGrid->GetBottomTextMargin() - 1;

        if (bByCursor)
        {
            CPoint point;
            ::GetCursorPos(&point);
            point.Offset(0, ::GetSystemMetrics(SM_CYCURSOR) / 2);
            rectDisplay.left = point.x;
            rectDisplay.top = point.y;
        }
        else
        {
            rectDisplay = TextRect;
            m_pGrid->ClientToScreen(rectDisplay);
        }

		CGridColumn* pGridColumn = NULL;
		CDataRow* pDataRow = NULL;
		int nRow;
		RegionType rType = m_pGrid->TestRegionType(m_pt, nRow);
		int nColIndex = m_pGrid->HitTestColumn(m_pt, rType, nRow);
		if (nColIndex != -1)
			pGridColumn = m_pGrid->GetColumns()->GetAt(nColIndex);
		if (rType != rtHeader)
			pDataRow = m_pGrid->GetGridRow(rType, nRow)->GetDataRow();

		BOOL bDoShow = m_pGrid->OnShowHint(pDataRow, pGridColumn, rType, rectDisplay.TopLeft(), strText);

		if (bDoShow)
		{
			rectDisplay.right = rectDisplay.left + nWidth;
			rectDisplay.bottom = rectDisplay.top + nHeight;

			rectDisplay.InflateRect(1, 1);

			SetWindowPos(&wndTop, rectDisplay.left, rectDisplay.top, rectDisplay.Width(), rectDisplay.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);

			if (m_crBackColor != CLR_DEFAULT)
			{
				CBrush backBrush(m_crBackColor);
				CBrush* pOldBrush = dc.SelectObject(&backBrush);
				CRect rect;
				dc.GetClipBox(&rect);
				dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),  PATCOPY);
				dc.SelectObject(pOldBrush);
			}
	        
			CBrush BorderBrush;
			BorderBrush.CreateSolidBrush(m_pGrid->GetLinesColor());
	        
			CRect rect;
			dc.GetClipBox(&rect);
			dc.FrameRect(rect, &BorderBrush);

			if (m_crTextColor != CLR_DEFAULT)
				dc.SetTextColor(m_crTextColor);

			dc.SetBkMode(TRANSPARENT);
	        
			//dc.TextOut(m_pGrid->GetLeftTextMargin() + 1, m_pGrid->GetTopTextMargin() + 1, strText);
			UINT nFlags = DT_NOPREFIX | DT_NOCLIP;
			if (bWordBreak)
				nFlags |= DT_WORDBREAK;
			OutRect.OffsetRect(m_pGrid->GetLeftTextMargin() + 1 - OutRect.left, m_pGrid->GetTopTextMargin() + 1 - OutRect.top);
			dc.DrawText(strText,OutRect, nFlags);

			SetCapture();

			SetTimer(TIMER_ID_AUTOHIDE, TIP_TIMEOUT, NULL);
		}
    }

    dc.SelectObject(pOldFont);
}

void CGridTip::Hide()
{
    if (!::IsWindow(GetSafeHwnd()))
        return;

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
        ReleaseCapture();

    if (IsWindowVisible())
    {
        BOOL bSuccess = KillTimer(TIMER_ID_AUTOHIDE);
        ASSERT(bSuccess);
    }

    ShowWindow(SW_HIDE);
}

void CGridTip::OnMouseMove(UINT nFlags, CPoint point) 
{
    ClientToScreen(&point);
    if (!m_rectHover.PtInRect(point)) 
    {
        Hide();
		m_dwTipedID = 0;
        
        CWnd *pWnd = WindowFromPoint(point);
        if (pWnd == this) 
            pWnd = m_pGrid;
        
        int hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0, MAKELONG(point.x,point.y));
        
        if (hittest == HTCLIENT)
        {
            pWnd->ScreenToClient(&point);
            pWnd->PostMessage(WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y));
        }
        else
            pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y));
    }
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

BOOL CGridTip::PreTranslateMessage(MSG* pMsg) 
{
    CWnd *pWnd;
	int hittest;
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
		POINTS pts = MAKEPOINTS( pMsg->lParam );
		POINT  point;
		point.x = pts.x;
		point.y = pts.y;

		ClientToScreen( &point );
        Hide();

		pWnd = WindowFromPoint( point );
		if (!pWnd)
			return CWnd::PreTranslateMessage(pMsg);

		if( pWnd->GetSafeHwnd() == GetSafeHwnd()) 
			pWnd = m_pGrid;

		hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

		if (hittest == HTCLIENT) 
		{
			pWnd->ScreenToClient( &point );
		}
		else 
		{
			switch (pMsg->message)
            {
			case WM_LBUTTONDOWN: 
				pMsg->message = WM_NCLBUTTONDOWN;
				break;
			case WM_RBUTTONDOWN: 
				pMsg->message = WM_NCRBUTTONDOWN;
				break;
			case WM_MBUTTONDOWN: 
				pMsg->message = WM_NCMBUTTONDOWN;
				break;
			}
			pMsg->wParam = hittest;
		}
		pWnd->GetParent()->PostMessage(WM_MOUSEACTIVATE, (WPARAM)pWnd->GetParentOwner()->GetSafeHwnd(), MAKELPARAM(hittest, MA_ACTIVATE));
        pMsg->lParam = MAKELONG(point.x,point.y);
        pWnd->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
		}
	case WM_TIMER:
        Hide();
		return TRUE;
    }
    
    if(GetFocus() == NULL)
    {
        Hide();
        return TRUE;
    }
    return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CQSEdit window
/////////////////////////////////////////////////////////////////////////////

/*
// CQSEdit::CQSEdit()
// {
// }
// 
// CQSEdit::~CQSEdit()
// {
// }
// 
// BEGIN_MESSAGE_MAP(CQSEdit, CEdit)
// 	//{{AFX_MSG_MAP(CQSEdit)
// 	ON_WM_KILLFOCUS()
// 	ON_WM_KEYDOWN()
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// BOOL CQSEdit::PreTranslateMessage(MSG* pMsg)
// {
//     return CEdit::PreTranslateMessage(pMsg);
// }
// 
// void CQSEdit::OnKillFocus(CWnd* pNewWnd)
// {
// }
// 
// void CQSEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
// {
//     CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
// }
*/

/////////////////////////////////////////////////////////////////////////////
// CQuickSearch window
/////////////////////////////////////////////////////////////////////////////

CQuickSearch::CQuickSearch(CCursorGridCtrl* pGrid) : m_pGrid(pGrid)
{
    HINSTANCE hInstance = AfxGetInstanceHandle();
    WNDCLASS wc;
    
    if (!(::GetClassInfo(hInstance, QUICKSEARCH_CLASSNAME, &wc)))
    {
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wc.hIcon = NULL;
        wc.hInstance = hInstance;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.lpszClassName = QUICKSEARCH_CLASSNAME;
        wc.lpszMenuName = NULL;
        wc.style = CS_SAVEBITS;
        
        if (!AfxRegisterClass(&wc))
        {
            AfxThrowResourceException();
            return;
        }
    }
    m_bIsWorking = FALSE;
    m_nTimeout = QUICKSEARCH_TIMEOUT;
}

CQuickSearch::~CQuickSearch()
{
    if (::IsWindow(GetSafeHwnd()))
        DestroyWindow();
}

BEGIN_MESSAGE_MAP(CQuickSearch, CWnd)
	//{{AFX_MSG_MAP(CQuickSearch)
	ON_WM_PAINT()
	ON_WM_TIMER()
    ON_WM_NCACTIVATE()
    ON_EN_KILLFOCUS(100, OnEditKillFocus)
//	ON_EN_CHANGE(100, OnEditChange)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CQuickSearch::IsWorking() const
{
    return m_bIsWorking;
}

void CQuickSearch::Hide()
{
    if (!::IsWindow(GetSafeHwnd()))
        return;
    m_bIsWorking = FALSE;
    m_pGrid->SetFocus();
    DestroyWindow();
}

void CQuickSearch::Show(LPRECT pRect, LPCSTR szFieldName, LPCSTR szInitStr)
{
    Hide();
    
    BOOL bSuccess;
    ASSERT_VALID(m_pParentWnd);

    bSuccess = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, QUICKSEARCH_CLASSNAME, NULL, WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, NULL);

    ASSERT(bSuccess);

    if (!bSuccess)
        return;

    m_bIsWorking = TRUE;
    m_strFieldName = szFieldName;

    CRect rectDisplay(pRect);
    m_pGrid->ClientToScreen(rectDisplay);
    rectDisplay.left--;

    CClientDC dc(this);
    dc.SelectStockObject(ANSI_VAR_FONT);
    CSize TextSize = dc.GetTextExtent("j^");
    rectDisplay.bottom = 1 + rectDisplay.top + m_pGrid->GetTopTextMargin() + TextSize.cy + m_pGrid->GetBottomTextMargin();

    CRect rectEdit;
    rectEdit.left = 1 + m_pGrid->GetLeftTextMargin();
    rectEdit.top = 1 + m_pGrid->GetTopTextMargin();
    rectEdit.right = rectEdit.left + rectDisplay.Width() - m_pGrid->GetLeftTextMargin() - m_pGrid->GetRightTextMargin();
    rectEdit.bottom = rectEdit.top + rectDisplay.Height() - m_pGrid->GetTopTextMargin() - m_pGrid->GetBottomTextMargin();
    bSuccess = m_edit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, rectEdit, this, 100);
    CFont font;
    font.CreateStockObject(ANSI_VAR_FONT);
    m_edit.SetFont(&font);
    m_edit.SetWindowText(szInitStr);
    m_edit.SetSel(1, 1);
    m_edit.SetFocus();
    
    SetWindowPos(&wndTop, rectDisplay.left, rectDisplay.top, rectDisplay.Width(), rectDisplay.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
//    m_bIsDirty = TRUE;
	m_strPattern ="";
    if (m_nTimeout)
        SetTimer(1, m_nTimeout, NULL);
}

void CQuickSearch::OnPaint() 
{
	CPaintDC dc(this);

    CRect rectClient;
    GetClientRect(rectClient);

    CBrush BorderBrush;
    BorderBrush.CreateSolidBrush(m_pGrid->GetLinesColor());
    dc.FrameRect(rectClient, &BorderBrush);
}

BOOL CQuickSearch::IsDirty()
{
	CString str;
	m_edit.GetWindowText(str);

	return (m_strPattern != str);
}

BOOL CQuickSearch::PreTranslateMessage(MSG* pMsg) 
{
    switch (pMsg->message)
    {
    case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			Hide();
			m_pGrid->OnQuickSearchEnd(qsrCancel, FALSE);
			return TRUE;
		case VK_UP:
		case VK_DOWN:
        case VK_PRIOR:
        case VK_NEXT:
//         case VK_HOME:
//         case VK_END:
        case VK_F5:
		case VK_TAB:
			m_pGrid->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			Hide();
			m_pGrid->OnQuickSearchEnd(qsrCancel, FALSE);
			return TRUE;
		case VK_RETURN:
			if (IsDirty())
			{
				CString strBeforeSearch, strAfterSearch;
				m_edit.GetWindowText(strBeforeSearch);
				Search();
				m_edit.GetWindowText(strAfterSearch);
				if (strBeforeSearch == strAfterSearch)
				{
					Hide();
					m_pGrid->OnQuickSearchEnd(qsrOK, TRUE);
				}
			}
			else
			{
// 				m_pGrid->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
				Hide();
				m_pGrid->OnQuickSearchEnd(qsrOK, FALSE);
			}
			return TRUE;
        default:
            if (m_nTimeout)
            {
                KillTimer(1);
                SetTimer(1, m_nTimeout, NULL);
            }
		}
        break;
    }
    return CWnd::PreTranslateMessage(pMsg);
}

void CQuickSearch::OnTimer(UINT nIDEvent)
{
	if (IsDirty())
		Search();
}

void CQuickSearch::Search()
{
    if (m_nTimeout)
        KillTimer(1);
//    m_bIsDirty = FALSE;
    m_edit.GetWindowText(m_strPattern);
    if (m_strPattern.IsEmpty())
        return;

	if (m_pGrid->OnQuickSearch(m_strPattern))
	{
		stQuickSearchPattern QSPattern;
		QSPattern.szFieldName = m_strFieldName;
		QSPattern.dataType = dtText;
		QSPattern.pTextData = &m_strPattern;
		m_pGrid->m_pProvider->QuickSearch(QSPattern);
/*
		int nStart, nEnd;
		m_edit.GetSel(nStart, nEnd);
		m_edit.SetWindowText(strPattern);
		m_edit.SetSel(nStart, nEnd);
*/
	}
	int nLast = m_strPattern.GetLength();
	m_edit.SetWindowText(m_strPattern);
	m_edit.SetSel(nLast, nLast);
}

BOOL CQuickSearch::OnNcActivate(BOOL bActive)
{
    AfxGetMainWnd()->SendMessage(WM_NCACTIVATE, TRUE, 0);
    return TRUE;
}

void CQuickSearch::OnEditKillFocus()
{
	if (m_bIsWorking)
	{
		Hide();
		m_pGrid->OnQuickSearchEnd(qsrCancel, FALSE);
	}
}
/*
void CQuickSearch::OnEditChange()
{
    m_bIsDirty = TRUE;
}
*/

void CQuickSearch::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

void CQuickSearch::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}
/////////////////////////////////////////////////////////////////////////////
// CReposColumnWnd window
/////////////////////////////////////////////////////////////////////////////

CReposColumnWnd::CReposColumnWnd(CCursorGridCtrl* pGrid): m_pGrid(pGrid)
{
    HINSTANCE hInstance = AfxGetInstanceHandle();
    WNDCLASS wc;
    
    if (!(::GetClassInfo(hInstance, REPOSCOLUMN_CLASSNAME, &wc)))
    {
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = NULL;
        wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wc.hIcon = NULL;
        wc.hInstance = hInstance;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.lpszClassName = REPOSCOLUMN_CLASSNAME;
        wc.lpszMenuName = NULL;
        wc.style = CS_SAVEBITS;
        
        if (!AfxRegisterClass(&wc))
        {
            AfxThrowResourceException();
            return;
        }
    }
    m_bArrowsAreDrawn = FALSE;
}

CReposColumnWnd::~CReposColumnWnd()
{
}

BEGIN_MESSAGE_MAP(CReposColumnWnd, CWnd)
	//{{AFX_MSG_MAP(CReposColumnWnd)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CReposColumnWnd::OnMouseColumnMove(CPoint point)
{
	//процедура определения значения свойства "Положение" при перетаскивании колонки
	//вызывается перед сдвигом колонки в коллекции

	int nColAIndex;
	if (m_nShift < 0)
		nColAIndex = m_nCapturedIndex + m_nShift - 1;
	else
		nColAIndex = m_nCapturedIndex + m_nShift;
	int nColBIndex = m_pGrid->GetNextColumn(nColAIndex, 1);
	
	int nColCIndex = m_pGrid->GetNextColumn(m_nCapturedIndex, 1);

	CGridColumn *pColA = m_pGrid->m_pColumns->GetAt(nColAIndex); //будущая колонка "слева"
	CGridColumn *pColB = m_pGrid->m_pColumns->GetAt(nColBIndex); //будущая колонка "справа"
	CGridColumn *pColC = m_pGrid->m_pColumns->GetAt(nColCIndex); //текущая колонка "справа"

	if (pColC)
	{
		if (m_pCapturedColumn->m_Location != clSomeColumn)
		{
			if (pColC->m_Location == clSomeColumn)
			{
				pColC->m_Location = m_pCapturedColumn->m_Location;
			}
			else if (pColC->m_Location == clNextLine)
			{
				pColC->m_Location = m_pCapturedColumn->m_Location;
			}
		}
	}

	if (pColB)
	{
		if (pColB->m_Location == clSomeColumn)
		{
			m_pCapturedColumn->m_Location = clSomeColumn;
		}
		else if (pColB->m_Location == clNextLine)
		{
			m_pCapturedColumn->m_Location = clSomeColumn;
		}
	}
}

void CReposColumnWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    HideArrows();
    ReleaseCapture();
    DestroyWindow();
    if (m_nShift && m_pGrid->OnColumnMove(m_pCapturedColumn, m_nShift))
	{
		OnMouseColumnMove(point);
        m_pGrid->m_pColumns->Move(m_nCapturedIndex, m_nShift);
	}

    if (m_bChangeVisible && m_pGrid->OnColumnVisibilityChange(m_pCapturedColumn, !m_pCapturedColumn->m_bVisible))
        m_pCapturedColumn->SetVisible(!m_pCapturedColumn->m_bVisible);
    
    m_pGrid->m_CaptureType = CCursorGridCtrl::ctNone;
    m_pGrid->m_ColumnHelper.Show();
    m_pGrid->SetFocus();
}

void CReposColumnWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    ClientToScreen(&point);
    SetWindowPos(NULL, point.x - m_Offset.cx, point.y - m_Offset.cy, 0, 0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);

    int nShift = 0;
    BOOL bChangeVisible = FALSE;

    m_pGrid->ScreenToClient(&point);

	int nHitColIndex = m_pGrid->HitTestHeader(point);

	LPCSTR szCursor = IDC_NO;
    if (nHitColIndex == -1)
    {
        if (m_pCapturedColumn->m_bVisible)
        {
            if (m_pCapturedColumn->m_bChangeVisible)
            {
                CRect rectHelper;
                m_pGrid->m_ColumnHelper.GetWindowRect(rectHelper);
                m_pGrid->ScreenToClient(rectHelper);
                if (rectHelper.PtInRect(point))
                {
                    szCursor = IDC_ARROW;
                    bChangeVisible = TRUE;
                }
            }
        }
        else
        {
            CRect rectHeader = m_pGrid->GetHeaderRect();
            if (rectHeader.PtInRect(point))
            {
                szCursor = IDC_ARROW;
                bChangeVisible = TRUE;
            }
        }
    }
    else
    {
        if (m_pGrid->m_bChangePositionOfColumns)
        {
            szCursor = IDC_ARROW;
            if (m_pGrid->DisplayColumn(m_pGrid->m_pColumns->GetAt(nHitColIndex)))
                nHitColIndex = m_pGrid->HitTestHeader(point);
        }
        else
        {
            if (!m_pCapturedColumn->m_bVisible)
            {
				CRect rectHeader = m_pGrid->GetHeaderRect();
				if (rectHeader.PtInRect(point))
                {
                    szCursor = IDC_ARROW;
                    bChangeVisible = TRUE;
                }
            }
            nHitColIndex = -1;
        }
    }
    SetCursor(AfxGetApp()->LoadStandardCursor(szCursor));

    if (nHitColIndex != -1 && nHitColIndex != m_nCapturedIndex)
    {
        CRect rectHitCol = m_pGrid->GetColumnClipRect(m_pGrid->m_pColumns->GetAt(nHitColIndex));
        if (nHitColIndex < m_nCapturedIndex)
        {
            if (point.x > rectHitCol.left + rectHitCol.Width() / 2)
            {
                nHitColIndex = m_pGrid->GetNextColumn(nHitColIndex, 1);
                if (nHitColIndex > m_nCapturedIndex || nHitColIndex == -1)
                    nHitColIndex = m_nCapturedIndex;
            }
        }
        else
        {
            if (point.x < rectHitCol.left + rectHitCol.Width() / 2)
            {
                nHitColIndex = m_pGrid->GetNextColumn(nHitColIndex, -1);
                if (nHitColIndex < m_nCapturedIndex || nHitColIndex == -1)
                    nHitColIndex = m_nCapturedIndex;
            }
        }
        nShift = nHitColIndex - m_nCapturedIndex;
        if (!m_pCapturedColumn->m_bVisible)
            bChangeVisible = TRUE;
    }

    DrawArrows(nShift, bChangeVisible);
    CWnd::OnMouseMove(nFlags, point);
}

void CReposColumnWnd::DrawArrows(int nShift, BOOL bChangeVisible)
{
    CPoint point;
	int nHeight = m_pGrid->m_nRowHeight;

    CGridColumn* pColumn = m_pGrid->m_pColumns->GetAt(m_nCapturedIndex + nShift);
    if (!pColumn->m_bVisible)
    {
        int nIndex = m_pGrid->GetNextColumn(m_nCapturedIndex, -1);
        if (nIndex == -1)
        {
            nIndex = m_pGrid->GetNextColumn(m_nCapturedIndex, 1);
            if (nIndex == -1)
            {
                CRect rect;
                m_pGrid->GetClientRectEx(rect);
                point.x = rect.left - 8;
                point.y = rect.top - 16;
            }
            else
            {
                CGridColumn* pNextColumn = m_pGrid->m_pColumns->GetAt(nIndex);
                CRect rect = m_pGrid->GetColumnClipRect(pNextColumn);
				m_pGrid->SetVerticalLocationOfRect(rect, pNextColumn);
                point.x = rect.left - 8;
                point.y = rect.top - 16;
				nHeight = rect.Height();
            }
        }
        else
        {
            CGridColumn* pNextColumn = m_pGrid->m_pColumns->GetAt(nIndex);
            CRect rect = m_pGrid->GetColumnClipRect(pNextColumn);
			m_pGrid->SetVerticalLocationOfRect(rect, pNextColumn);
            point.x = rect.right - 8;
            point.y = rect.top - 16;
			nHeight = rect.Height();
        }
    }
    else
    {
        CRect rect = m_pGrid->GetColumnClipRect(pColumn);
		m_pGrid->SetVerticalLocationOfRect(rect, pColumn);
        if (nShift > 0)
            point.x = rect.right - 8;
        else
            point.x = rect.left - 8;
        point.y = rect.top - 16;
		nHeight = rect.Height();
    }

    if (m_pCapturedColumn->m_bVisible && nShift || !m_pCapturedColumn->m_bVisible && bChangeVisible)
    {
        m_pGrid->ClientToScreen(&point);
        m_pGrid->GetParent()->ScreenToClient(&point);
        
        if (nShift != m_nShift)
            HideArrows();

        if (!m_bArrowsAreDrawn)
        {
            m_bArrowsAreDrawn = TRUE;
            m_ptLastArrows = point;
        }

        CDC* pDC = m_pGrid->GetParent()->GetDC();
        m_pGrid->m_ilInternalImages.Draw(pDC, iiTopReposArrow, point, ILD_NORMAL);
        point.Offset(0, 16 + nHeight);
        m_pGrid->m_ilInternalImages.Draw(pDC, iiBottomReposArrow, point, ILD_NORMAL);
        m_pGrid->GetParent()->ReleaseDC(pDC);
    }
    else
        HideArrows();

	CRect WindowRect;
	GetWindowRect(&WindowRect);
	if (WindowRect.Height() != nHeight)
	{
		m_Offset.cy = MulDiv(m_Offset.cy, nHeight, WindowRect.Height());
		SetWindowPos(NULL, WindowRect.left, WindowRect.top, WindowRect.Width(), nHeight,
			SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	}

    m_nShift = nShift;
    m_bChangeVisible = bChangeVisible;
}

void CReposColumnWnd::HideArrows()
{
    if (!m_bArrowsAreDrawn)
        return;

    CRect rect(m_ptLastArrows, CSize(16, 16 + 16 + m_pGrid->m_nRowHeight));
    m_pGrid->GetParent()->InvalidateRect(rect);

    m_bArrowsAreDrawn = FALSE;
}

void CReposColumnWnd::OnPaint()
{
    CPaintDC dc(this);
    
    CRect rect;
    GetClientRect(rect);
    CFont* pOldFont = dc.SelectObject(&m_pGrid->m_font);
    if (m_pGrid->m_HeadersStyle == hsFlat)
    {
        rect.left++;
        rect.top++;
        m_pGrid->DrawHeader(&dc, m_pCapturedColumn, rect, rect);
        rect.left--;
        rect.top--;
        CBrush BorderBrush;
        BorderBrush.CreateSolidBrush(m_pGrid->GetLinesColor());
        dc.FrameRect(rect, &BorderBrush);
    }
    else
        m_pGrid->DrawHeader(&dc, m_pCapturedColumn, rect, rect);
    dc.SelectObject(pOldFont);
}

BOOL CReposColumnWnd::OnNcActivate(BOOL bActive)
{
    AfxGetMainWnd()->SendMessage(WM_NCACTIVATE, TRUE, 0);
    return TRUE;
}

void CReposColumnWnd::Show(CGridColumn* pColumn, CSize& Offset)
{
    BOOL bSuccess;

    m_pCapturedColumn = pColumn;
    m_Offset = Offset;
    m_nCapturedIndex = m_pGrid->m_pColumns->IndexOf(m_pCapturedColumn);
	
    int cx, cy;
    if (m_pCapturedColumn->m_bVisible)
    {
        cx = m_pCapturedColumn->m_rect.Width();
        cy = m_pCapturedColumn->m_rect.Height();

        m_pGrid->m_ColumnHelper.Show();
    }
    else
    {
        cx = m_pCapturedColumn->m_nMinWidth;
        if (cx < COLUMN_HELPER_MINROWWIDTH - 1)
            cx = COLUMN_HELPER_MINROWWIDTH - 1;
        cy = m_pGrid->m_nSingleLineHeight;
    }

    if (m_pGrid->m_HeadersStyle == hsFlat)
    {
        cx++;
        cy++;
    }
    CPoint point;
    GetCursorPos(&point);

    bSuccess = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, REPOSCOLUMN_CLASSNAME, NULL, WS_POPUP|WS_VISIBLE,
        point.x - cx/2, point.y - cy/2, cx, cy,
        NULL, NULL, NULL);

    ASSERT(bSuccess);
    if (!bSuccess)
        return;
    SetCapture();
}

/////////////////////////////////////////////////////////////////////////////
// CColumnHelper window
/////////////////////////////////////////////////////////////////////////////

CColumnHelper::CColumnHelper(CCursorGridCtrl* pGrid): m_pGrid(pGrid)
{
    HINSTANCE hInstance = AfxGetInstanceHandle();
    WNDCLASS wc;
    
    if (!(::GetClassInfo(hInstance, COLUMNHELPER_CLASSNAME, &wc)))
    {
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = NULL;
        wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wc.hIcon = NULL;
        wc.hInstance = hInstance;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.lpszClassName = COLUMNHELPER_CLASSNAME;
        wc.lpszMenuName = NULL;
        wc.style = CS_SAVEBITS;
        
        if (!AfxRegisterClass(&wc))
        {
            AfxThrowResourceException();
            return;
        }
    }
    m_bDisplay = FALSE;
}

CColumnHelper::~CColumnHelper()
{
}

BEGIN_MESSAGE_MAP(CColumnHelper, CWnd)
	//{{AFX_MSG_MAP(CColumnHelper)
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColumnHelper::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    lpncsp->rgrc[0].top += COLUMN_HELPER_CX;
}

void CColumnHelper::OnNcPaint() 
{
    CWindowDC dc(this);
    CRect rectWindow;
    GetWindowRect(rectWindow);
    CRect rect;
    rect.left = rectWindow.Width() - COLUMN_HELPER_CX;
    rect.right = rectWindow.Width();
    rect.top = 0;
    rect.bottom = COLUMN_HELPER_CY;
    
    dc.FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

    m_pGrid->m_ilInternalImages.Draw(&dc, iiHelperIcon, CPoint(rect.left + 1, rect.top + 1), ILD_NORMAL);

    CBrush brush;
    brush.CreateSolidBrush(m_pGrid->GetLinesColor());
    dc.FrameRect(rect, &brush);

    if (m_columns.GetSize())
    {
        dc.MoveTo(rectWindow.Width(), COLUMN_HELPER_CY - 1);
        CPen pen;
        pen.CreatePen(PS_SOLID, 1, m_pGrid->GetLinesColor());
        CPen* pOldPen = dc.SelectObject(&pen);
        dc.LineTo(rectWindow.Width() - GetColumnWidth(m_columns[0]) - 1, COLUMN_HELPER_CY - 1);
        dc.SelectObject(pOldPen);
    }
}

void CColumnHelper::OnPaint()
{
    CPaintDC dc(this);
	
    CRect rectHeader;
    GetClientRect(rectHeader);
    rectHeader.bottom = m_pGrid->m_nSingleLineHeight;

    CBrush brush;
    brush.CreateSolidBrush(m_pGrid->GetLinesColor());

    CFont* OldFont = dc.SelectObject(&m_pGrid->m_font);
    for (int i = 0; i < m_columns.GetSize(); i++)
    {
        CGridColumn* pColumn = m_columns[i];
        rectHeader.left = rectHeader.right - GetColumnWidth(pColumn) + 1;
        m_pGrid->DrawHeader(&dc, pColumn, rectHeader, rectHeader);

        CRect rectFrame(rectHeader);
        rectFrame.left--;
        rectFrame.top--;
        dc.FrameRect(rectFrame, &brush);

        rectHeader.OffsetRect(0, m_pGrid->m_nSingleLineHeight);
    }
    dc.SelectObject(OldFont);
}

void CColumnHelper::OnLButtonDown(UINT nFlags, CPoint point)
{
    int nIndex = point.y / m_pGrid->m_nSingleLineHeight;
    CGridColumn* pColumn = m_columns[nIndex];
    CRect rect;
    GetClientRect(rect);
	CSize size(point.x - (rect.Width() - GetColumnWidth(pColumn)), point.y % m_pGrid->m_nSingleLineHeight);
    m_pGrid->m_ReposColumnWnd.Show(pColumn, size);
    m_pGrid->m_CaptureType = CCursorGridCtrl::ctMoveColumn;
    CWnd::OnLButtonDown(nFlags, point);
}

void CColumnHelper::Show()
{
    BOOL bSuccess;

    if (!m_hWnd)
    {
        bSuccess = CreateEx(0, COLUMNHELPER_CLASSNAME, NULL, WS_CHILD,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            m_pGrid->m_hWnd, NULL, NULL);
    }

    int cx = COLUMN_HELPER_CX;
    int cy = COLUMN_HELPER_CY;
    m_columns.RemoveAll();
    int i;
    for (i = 0; i < m_pGrid->m_pColumns->GetSize(); i++)
    {
        CGridColumn* pColumn = m_pGrid->m_pColumns->GetAt(i);
        if (!pColumn->m_bVisible && pColumn->m_bChangeVisible)
        {
            m_columns.Add(pColumn);
            int nWidth = GetColumnWidth(pColumn);

            if (nWidth > cx)
                cx = nWidth;
            cy += m_pGrid->m_nSingleLineHeight;
        }
    }

    CRect rect = m_pGrid->GetGridRect();
    int x = rect.right - cx - COLUMN_HELPER_MARGIN;
    int y = rect.top + COLUMN_HELPER_MARGIN;
    SetWindowPos(NULL, x, y, cx, cy, SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOZORDER);

    CRgn rgnWindow;
    bSuccess = rgnWindow.CreateRectRgn(cx - COLUMN_HELPER_CX, 0, cx, COLUMN_HELPER_CY);

    int CurY = COLUMN_HELPER_CY;
    for (i = 0; i < m_columns.GetSize(); i++)
    {
        CRgn rgn;
        bSuccess = rgn.CreateRectRgn(cx - GetColumnWidth(m_columns[i]), CurY - 1, cx, CurY + m_pGrid->m_nSingleLineHeight);
        int nRetVal = rgnWindow.CombineRgn(&rgnWindow, &rgn, RGN_OR);

        CurY += m_pGrid->m_nSingleLineHeight;
    }

    bSuccess = SetWindowRgn(rgnWindow, FALSE);
    ShowWindow(SW_NORMAL);
    m_bDisplay = TRUE;
}

void CColumnHelper::Hide()
{
    if (!m_hWnd)
        return;
    ShowWindow(SW_HIDE);
    m_bDisplay = FALSE;
}

int CColumnHelper::GetColumnWidth(CGridColumn* pColumn)
{
    int nWidth = pColumn->m_nMinWidth + 1;
    if (nWidth < COLUMN_HELPER_MINROWWIDTH)
        nWidth = COLUMN_HELPER_MINROWWIDTH;
    return nWidth;
}

BOOL CColumnHelper::IsDisplay()
{
    return m_bDisplay;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellEdit class
/////////////////////////////////////////////////////////////////////////////

CDataRow* CGridCellEdit::GetDataRow() const
{
	return m_pDataRow.get();
// 	CDataRow* pDataRow = NULL;
// 	if (m_pGrid)
// 	{
// 		size_t nRowIndex = m_pGrid->GetVisibleRowIndex(m_rType, m_pDataRow.get());
// 		CGridRow* pGridRow = m_pGrid->GetGridRow(m_rType, nRowIndex);
// 		if (pGridRow)
// 			pDataRow = pGridRow->GetDataRow();
// 	}
// 	return pDataRow;
}

CRect CGridCellEdit::SuggestWindowRect() const
{
	CRect rect(0, 0, 0, 0);
	if (m_pGrid)
	{
		int nRowIndex = m_pGrid->GetVisibleRowIndex(m_rType, m_pDataRow.get());
		if (nRowIndex != -1)
		{
			rect = m_pGrid->GetRowRect(m_rType, nRowIndex);
			rect.IntersectRect(rect, m_pGrid->GetColumnClipRect(m_pColumn));
			m_pGrid->SetVerticalLocationOfRect(rect,m_pColumn);

			if (rect.right)
				--rect.right;

			if (rect.bottom)
				--rect.bottom;

			if (m_rType != rtHeader)
			{
				CGridItem* pItem = m_pGrid->GetGridRow(m_rType, nRowIndex)->GetGridItem(m_pColumn);
				if (pItem)
					rect.left += pItem->GetNoTextOffset();
			}
		}
	}

	return rect;
}

void CGridCellEdit::Reposition() const
{
	if (m_pWnd)
	{
		CRect rect = SuggestWindowRect();
		m_pWnd->MoveWindow(rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCursorGridCtrl window
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCursorGridCtrl, CWnd)

CCursorGridCtrl::CCursorGridCtrl(): m_Tip(this), m_QuickSearch(this), m_ReposColumnWnd(this), m_ColumnHelper(this),
    m_DropTarget(this), m_CommandQueue(this), m_ThumbTracker(this)
{
    HINSTANCE hInstance = AfxGetInstanceHandle();
    WNDCLASS wc;
    
    if (!(::GetClassInfo(hInstance, CURSOR_GRID_CLASS_NAME, &wc)))
    {
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wc.hIcon = NULL;
        wc.hInstance = hInstance;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.lpszClassName = CURSOR_GRID_CLASS_NAME;
        wc.lpszMenuName = NULL;
        wc.style = /*CS_HREDRAW | CS_VREDRAW |*/ CS_DBLCLKS;
        
        if (!AfxRegisterClass(&wc))
        {
            AfxThrowResourceException();
            return;
        }
    }

    m_nTopRowIndex = 0;
    m_pProvider = NULL;
	m_pProvider_head = NULL;
	m_pProvider_tail = NULL;
    m_bShowHeader = TRUE;
    m_pCapturedColumn = NULL;
    m_CaptureType = ctNone;
    m_VScrollerPresent = spAuto;
    m_nCurrentColumn = -1;
    m_nCurrentDataRowIndex = -1;
	m_nCurrentDataRowDirection = 0;
//     m_pCurrentDataRowOutOfWindow = NULL;
    m_RowSelectMode = rsmCell;
    m_SelectMode = smSingleLine;
    m_bHorizontalLines = TRUE;
    m_bVerticalLines = TRUE;
    m_StartView = svFromBegin;
    m_bAutoDelete = FALSE;
    m_HeadersStyle = hs3D;
	m_BorderStyle = bs3D;
	m_ActiveHeaderStyle = ahsSelected;

	BOOL bSuccess;
	bSuccess = m_ilInternalImages.Attach(ImageList_LoadBitmap(_Module.m_hInst, (LPCTSTR)IDB_CG_IMAGES, 16, 0, COLOR_TRANSPARENT));
    ASSERT(bSuccess);

	// temporarily here
	CBitmap bmap;
//  HBITMAP hb = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_1C_1216));
	HBITMAP hb = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(1216));
	ASSERT(hb);
	bSuccess = bmap.Attach(hb);
	ASSERT(bSuccess);
	BITMAP bm;
	bmap.GetBitmap(&bm);
	if (bm.bmPlanes == 1 && bm.bmBitsPixel == 1)
	{
		size_t const nCount = bm.bmHeight / bm.bmWidth;
		size_t const nFullSize = bm.bmWidthBytes * bm.bmHeight;
		bm.bmHeight = bm.bmWidth;
		size_t const nPatternSize = bm.bmWidthBytes * bm.bmHeight;

		std::vector<BYTE> vBits;
		vBits.resize(nFullSize);
		bm.bmBits = &vBits[0];
		bmap.GetBitmapBits(nFullSize, bm.bmBits);

		for (int i = 0; i < nCount; ++i)
		{
			CBitmapPtr pattern(new CBitmap);
			pattern->CreateBitmapIndirect(&bm);
			m_Patterns.push_back(pattern);
			bm.bmBits = (BYTE*)bm.bmBits + nPatternSize;
		}
	}

    m_nLastLParam = ULONG_MAX;
    m_nRefreshTimeout = 0;
    m_crGridLines = CLR_DEFAULT;
	m_crInactiveGridLines = CLR_DEFAULT;
    m_bChangePositionOfColumns = TRUE;
    m_crText = CLR_DEFAULT;
	m_crNegatives = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
	m_crText_head = CLR_DEFAULT;
	m_crBack_head = CLR_DEFAULT;
	m_crText_tail = CLR_DEFAULT;
	m_crBack_tail = CLR_DEFAULT;
    m_crSelectionText = CLR_DEFAULT;
    m_crSelectionBack = CLR_DEFAULT;
    m_crSelectionText2 = CLR_DEFAULT;
    m_crSelectionBack2 = CLR_DEFAULT;
    m_crAlternationRowBack = CLR_DEFAULT;

	m_trpBack = TRANSPARENT_DEFAULT;
	m_trpBack_head = TRANSPARENT_DEFAULT;
	m_trpBack_tail = TRANSPARENT_DEFAULT;
	m_trpSelectionBack = TRANSPARENT_DEFAULT;
	m_trpSelectionBack2 = TRANSPARENT_DEFAULT;
	m_trpAlternationRowBack = TRANSPARENT_DEFAULT;

	m_crHeadersText = CLR_DEFAULT;
	m_crHeadersBack = CLR_DEFAULT;
	m_crGridBorder = CLR_DEFAULT;
    m_bUseAlternationRowColor = FALSE;
    m_bEvenRow = TRUE;
	m_bEnableDrag = FALSE;
	m_bEnableDrop = FALSE;

	m_nRowHeight = 0;
	m_nVisibleRows = 1;

	m_nLeftTextMargin = DEFAULT_LEFT_TEXT_MARGIN;
	m_nRightTextMargin = DEFAULT_RIGHT_TEXT_MARGIN;
	m_nTopTextMargin = DEFAULT_TOP_TEXT_MARGIN;
	m_nBottomTextMargin = DEFAULT_BOTTOM_TEXT_MARGIN;
	m_nFixedLeft = 0;
	m_nFixedRight = 0;
	m_nFixedLeftEx = 0;
	m_nFixedRightEx = 0;

	m_nDrawLocks = 0;
	m_bDrawReq = FALSE;
	m_nUpdateLocks = 0;
	m_bUpdateReq = FALSE;
	m_nCommandQueueLocks = 0;

	m_ptContextMenuPosition = CPoint(0,0);
	m_pEdit = NULL;
	m_nEditLock = 0;

	m_nColumnsLineCount = 1;
	m_nSingleLineHeight = 0;
	m_bSelOnDblClk = TRUE;

	m_pSysImages = NULL;
	m_nSysImageWidth = 0;
	ResetBackText();
}

BOOL CCursorGridCtrl::Create(const RECT& rect, CWnd *pParentWnd, UINT nID, BOOL bAutoDelete, BOOL bVisible, BOOL bDisabled)
{
    BOOL bSuccess;
    m_bAutoDelete = bAutoDelete;

	DWORD dwStyle = WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN;
	if (bVisible)
		dwStyle |= WS_VISIBLE;
	if (bDisabled)
		dwStyle |= WS_DISABLED;
	
	bSuccess = CWnd::Create(CURSOR_GRID_CLASS_NAME, NULL, dwStyle, rect, pParentWnd, nID);
	if (bSuccess)
		SetTimer(TIMER_ID_AUTOHIDE, HELPER_DISPLAY_TIMEOUT, NULL);

	return bSuccess;
}

CCursorGridCtrl::~CCursorGridCtrl()
{
    ClearRows();
    if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
        m_pCurrentDataRowOutOfWindow.reset();

	if (m_pProvider)
        m_pProvider->SetControl(NULL);

	if (m_pProvider_head)
		m_pProvider_head->SetControl(NULL);

	if (m_pProvider_tail)
		m_pProvider_tail->SetControl(NULL);

	m_pColumns->Release();
}

BEGIN_MESSAGE_MAP(CCursorGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CCursorGridCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_SYSKEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_NCDESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(DP_DATA_CHANGE, OnDataChange)
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCursorGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
    switch (pMsg->message)
    {
	case WM_COMMAND:
		GetParent()->SendMessage(WM_COMMAND, pMsg->wParam, pMsg->lParam);
		return TRUE;

	case WM_KEYUP:
	case WM_CHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		if (OnKey(pMsg))
			return TRUE;
		break;

	case WM_KEYDOWN:
		if (OnKey(pMsg))
			return TRUE;

        switch (pMsg->wParam)
        {
        case VK_TAB:
            {
				if (!IS_CTRL_PRESSED())
				{
					if (CloseEdit(TRUE) && !GetEdit())
					{
						int nIndex = GetNextColumn(m_nCurrentColumn, IS_SHIFT_PRESSED() ? -1 : 1);
						if (nIndex != -1)
						{
							SetCurrentCell(m_nCurrentDataRowIndex, nIndex);
							return TRUE;
						}
					}
					else
						return TRUE;
				}
			}
            break;
        case VK_RETURN:
			if (!IS_CTRL_PRESSED())
			{
				if (GetEdit())
				{
					CloseEdit(TRUE);
					return TRUE;
				}

				CDataRow* pDataRow = GetCurrentDataRow();
				if (pDataRow)
					OnSelection(pDataRow, m_pColumns->GetAt(m_nCurrentColumn), rtBody, ghtNA);
				return TRUE;
			}
			break;
		case VK_SPACE:
			if (IS_CTRL_PRESSED())
			{
				CDataRowState st(this);
				SelectRow(st);
				OnActivateCell();
				return TRUE;
			}
			break;
		case VK_ESCAPE:
			if (GetEdit())
			{
				CloseEdit(FALSE);
				return TRUE;
			}
			break;
        }
        break;

	case WM_LBUTTONDOWN:
        if (pMsg->hwnd == m_hWnd && pMsg->lParam == m_nLastLParam && ::GetTickCount() - m_dwLastTickCount <= ::GetDoubleClickTime())
        {
            PostMessage(WM_LBUTTONDBLCLK, pMsg->wParam, pMsg->lParam);
            return TRUE;
        }
        m_dwLastTickCount = ::GetTickCount();
        m_nLastLParam = pMsg->lParam;
        break;
    }
    return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CCursorGridCtrl message handlers

int CCursorGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitFont(GetDefaultFont());
	return 0;
}

CFont* CCursorGridCtrl::GetDefaultFont() const
{
	return GetParent()->GetFont();
}

void CCursorGridCtrl::InitFont(CFont* pFont)
{
    BOOL bSuccess;
    int nRetVal;

    LOGFONT lf;
    nRetVal = pFont->GetLogFont(&lf);
    ASSERT(nRetVal);

	m_font.DeleteObject();
    bSuccess = m_font.CreateFontIndirect(&lf);
    ASSERT(bSuccess);

    CDC *pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(pFont);

    m_nFontHeight = pDC->GetTextExtent("^j").cy;

    pDC->SelectObject(pOldFont);
    nRetVal = ReleaseDC(pDC);

    ASSERT(nRetVal);

	RecalcRowHeight();
}

void CCursorGridCtrl::PreSubclassWindow() 
{
    m_pColumns = NewColumns();
    m_pColumns->m_pGrid = this;

    BOOL bSuccess;
    
	if (::IsWindow(m_hWnd))
	{
// 		bSuccess = ModifyStyleEx(0, WS_EX_CLIENTEDGE);
// 		ASSERT(bSuccess);

		SetBorderStyle(m_BorderStyle);

		CRect rect;
		GetWindowRect(rect);
		CWnd* pParentWnd = GetParent();
		if (pParentWnd)
		{
			pParentWnd->ScreenToClient(rect);
			rect.InflateRect(1, 1);
			MoveWindow(rect);
			rect.DeflateRect(1, 1);
			MoveWindow(rect);

			if (!rect.IsRectNull())
				InitFont(pParentWnd->GetFont());
		}
	}
    
    CalcRowsParams();
}

void CCursorGridCtrl::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
    if (m_bAutoDelete)
        delete this;
}

void CCursorGridCtrl::OnPaint() 
{
	CPaintDC dc(this);

	if (m_nDrawLocks)
		m_bDrawReq = TRUE;
	else
	{
		CMemDC memDC(&dc);
		Paint(memDC);
		m_bDrawReq = FALSE;
	}
}

void CCursorGridCtrl::Paint(CDC *pDC)
{
    if (pDC->RectVisible(GetGridRect()))
    {
		PaintBackground(pDC);

		PaintCells(pDC);

        if (m_bHorizontalLines || m_bVerticalLines)
            PaintGridLines(pDC);
        
        if (m_pProvider)
            PaintError(pDC);

        if (m_DropTarget.m_bDragOn)
            PaintDropTarget(pDC);
    }
	
	if (m_bShowHeader && pDC->RectVisible(GetHeaderRect()))
		PaintHeaders(pDC);

	if (m_BorderStyle != bs3D)
	{
		CRect rect;
		CBrush br(GetBorderColor());
		GetClientRect(rect);
		pDC->FrameRect(rect, &br);
	}
}

void CCursorGridCtrl::PaintBackground(CDC *pDC)
{
	CRect rect = GetGridRect();
	pDC->FillSolidRect(&rect, GetBackColor());

	DrawBackImage(pDC);
	DrawBackText(pDC);
}

void CCursorGridCtrl::DrawBackText(CDC* pDC)
{
	if ( m_strBackText.IsEmpty() )
		return;
	
	CRect rect = GetGridRect(rtBodyNSpace);
	CFont* pOldFont = pDC->SelectObject(m_pBackTextFont ? m_pBackTextFont : &m_font);
	
	UINT nFlags = DT_NOPREFIX | DT_NOCLIP;
	if ( m_bBackTextWB )
		nFlags |= DT_WORDBREAK;

	switch( m_BackTextHA )
	{
	case hbaLeft:
		nFlags |= DT_LEFT;
		break;
	case hbaRight:
		nFlags |= DT_RIGHT;
		break;
	default:
		nFlags |= DT_CENTER;
		break;
	}

	if ( m_BackTextVA != vbaTop )
	{
		CRect CalcRect(rect);
		pDC->DrawText(m_strBackText, CalcRect, nFlags|DT_CALCRECT);
		if ( m_BackTextVA == vbaBotom )
			rect.OffsetRect(0, rect.Height() - CalcRect.Height());
		else
			rect.OffsetRect(0, (rect.Height() - CalcRect.Height()) / 2);
	}

	COLORREF crText;
	if (m_crBackText != CLR_DEFAULT)
		crText = m_crBackText;
	else if (m_crText != CLR_DEFAULT)
		crText = m_crText;
	else
		crText = ::GetSysColor(COLOR_WINDOWTEXT);
	pDC->SetTextColor(crText);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strBackText, rect, nFlags);
	
	pDC->SelectObject(pOldFont);
}

void CCursorGridCtrl::PaintDropTarget(CDC* pDC)
{
    CRect rect(m_DropTarget.m_rectTarget);
    CPoint point = pDC->GetWindowOrg();
    rect.OffsetRect(-point.x, -point.y);
    CSize size(2,2);
    pDC->DrawDragRect(rect, size, NULL, size);
}

void CCursorGridCtrl::PaintError(CDC* pDC)
{
    LPCSTR szError = m_pProvider->GetLastError();
    if (!szError)
        return;

    CString strError(szError);
    if (!strError.GetLength())
        return;

    CRect GridRect = GetGridRect();
    CRect rect = GridRect;
    int nWidth = rect.Width() - rect.Width() / 4;
    rect.left = (rect.Width() - nWidth) / 2;
    rect.right = rect.left + nWidth;

    CFont* pOldFont = pDC->SelectObject(&m_font);

    UINT nFlags = DT_LEFT | DT_NOPREFIX | DT_WORDBREAK;
    CRect TestTextRect(rect);
    TestTextRect.left += 1;
    TestTextRect.right -= 1;
    pDC->DrawText(strError, TestTextRect, nFlags | DT_CALCRECT);
    int nTextHeight = TestTextRect.Height();
    rect.top = GridRect.top + (GridRect.Height() - nTextHeight + 2) / 2;
    rect.bottom = rect.top + nTextHeight + 2;

    CBrush br(::GetSysColor(COLOR_INFOBK));
    CBrush* pOldBrush = pDC->SelectObject(&br);
    pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
    pDC->SelectObject(pOldBrush);
    
    pDC->SetBkMode(TRANSPARENT);
    rect.left += 1;
    rect.right -= 1;
    pDC->DrawText(strError, rect, nFlags);

    pDC->SelectObject(pOldFont);
}

int CCursorGridCtrl::GetHeaderHeight() const
{
	return m_nRowHeight;
}

void CCursorGridCtrl::GetClientRectEx(CRect& rect) const
{
	GetClientRect(rect);
 	if (m_BorderStyle != bs3D)
  		rect.DeflateRect(1, 1, 0, 0);
}

CRect CCursorGridCtrl::GetHeaderRect() const
{
    CRect rect;
    GetClientRectEx(rect);
	rect.bottom = rect.top;
    
    if (m_bShowHeader)
        rect.bottom += GetHeaderHeight();
    
    return rect;
}

CRect CCursorGridCtrl::GetGridRect(RegionType rType /*= rtNone*/) const
{
	CRect HeaderRect = GetHeaderRect();

	CRect rect;
	GetClientRectEx(rect);

	switch(rType)
	{
	case rtHeader:
		rect = HeaderRect;
		break;
	case rtHead:
		rect.top = HeaderRect.bottom;
		rect.bottom = rect.top + GetVisibleRows(rtHead) * m_nRowHeight;
		break;
	case rtBody:
		rect.top = HeaderRect.bottom + GetVisibleRows(rtHead) * m_nRowHeight;
		rect.bottom = rect.top + GetVisibleRows(rtBody) * m_nRowHeight;
		break;
	case rtBodyNSpace:
		rect.top = HeaderRect.bottom + GetVisibleRows(rtHead) * m_nRowHeight;
		rect.bottom = rect.bottom - GetVisibleRows(rtTail) * m_nRowHeight;
		break;
	case rtTail:
		rect.top = rect.bottom - GetVisibleRows(rtTail) * m_nRowHeight;
		break;
	default:
		rect.top = HeaderRect.bottom;
	}

	if (rect.top > rect.bottom)
		rect.top = rect.bottom;

	return rect;
}

CRect CCursorGridCtrl::GetRowRect(RegionType rType, int nRowIndex) const
{
	CRect rect = GetGridRect(rType);
	switch(rType)
	{
	case rtHead:
	case rtBody:
		{
			CRect tmp = rect;
			tmp.bottom = tmp.top + m_nRowHeight;
			tmp.OffsetRect(0, (nRowIndex - GetTopRowIndex(rType)) * m_nRowHeight);
			rect.IntersectRect(rect, tmp);
		}
		break;
	case rtTail:
		{
			CRect tmp = rect;
			tmp.top = tmp.bottom - m_nRowHeight;
			tmp.OffsetRect(0, (nRowIndex - m_rows_tail.GetSize() + 1) * m_nRowHeight);
			rect.IntersectRect(rect, tmp);
		}
		break;
	default:
		break;
	}
	return rect;
}

BOOL CCursorGridCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

CGridColumns* CCursorGridCtrl::GetColumns() const
{
    return m_pColumns;
}

int CCursorGridCtrl::GetOrigin()
{
    BOOL bSuccess;
    SCROLLINFO si;
    int Origin = 0;
    
    bSuccess = GetScrollInfo(SB_HORZ, &si);
    if (bSuccess)
        Origin = si.nPos;
    return Origin;
}

void CCursorGridCtrl::PaintHeaders(CDC *pDC)
{
    CFont* pOldFont;
    CRect HeaderRect = GetHeaderRect();

	if (m_pColumns->GetSize() == 0)
	{
		pDC->FillSolidRect(&HeaderRect, ::GetSysColor(COLOR_BTNFACE));
		if (m_HeadersStyle == hs3D)
			Paint3DFrame_raised(pDC, &HeaderRect, BF_RECT);
		else
			pDC->FillSolidRect(HeaderRect.left, HeaderRect.bottom - 1, HeaderRect.Width(), 1, GetLinesColor());
		return;
	}

    pOldFont = pDC->SelectObject(&m_font);
	int nStrokeCount = 0;
	LPRECT lpColumnHeaderClipRect = new RECT[m_pColumns->GetSize()];
	LPRECT lpCurColumnHeaderClipRect = lpColumnHeaderClipRect;

    for (int i = 0; i < m_pColumns->GetSize(); ++i)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible && (!pColumn->m_bMergeCaption || i==0))
		{
			CRect Rect, clipRect;
			CRect NextColRect, NextColClipRect;
			
			CRect tmpHeaderRect = HeaderRect;
			SetVerticalLocationOfRect(tmpHeaderRect, pColumn);

			Rect.IntersectRect(GetColumnBoundsRect(pColumn), tmpHeaderRect);
			clipRect.IntersectRect(GetColumnClipRect(pColumn), tmpHeaderRect);

			//+MergeCaption
			for (int j = i+1; j < m_pColumns->GetSize(); ++j)
			{
		        CGridColumn* pNextColumn = m_pColumns->GetAt(j);
				if (pNextColumn->m_bMergeCaption)
				{
					NextColRect.IntersectRect(GetColumnBoundsRect(pNextColumn), tmpHeaderRect);
					NextColClipRect.IntersectRect(GetColumnClipRect(pNextColumn), tmpHeaderRect);
					Rect.UnionRect(Rect, NextColRect);
					clipRect.UnionRect(clipRect, NextColClipRect);
				}
				else
				{
					break;
				}
			}
			//-MergeCaption
			
			if (clipRect.right > HeaderRect.left && clipRect.left < HeaderRect.right)
			{
				DrawHeader(pDC, pColumn, Rect, clipRect);
				nStrokeCount++;
				if (m_HeadersStyle == hsFlat)
				{
					*lpCurColumnHeaderClipRect++ = clipRect;
				}
			}
		}
    }

	pDC->SelectObject(pOldFont);

	if (m_HeadersStyle == hsFlat)
	{
		int nPointCount = nStrokeCount * 3;
		LPPOINT lpPoints = new POINT[nPointCount];
		LPDWORD lpPolyPoints = new DWORD[nStrokeCount];

		for (int i = 0; i < nStrokeCount; i ++)
			lpPolyPoints[i] = 3;

		LPPOINT lpCurPoints = lpPoints;
		lpCurColumnHeaderClipRect = lpColumnHeaderClipRect;

		for (int i = 0; i < nStrokeCount; i ++)
		{
			//линия снизу и справа от заголовка колонки
			RECT &clipRect = *lpCurColumnHeaderClipRect++;
			lpCurPoints[0].x = clipRect.right - 1;
			lpCurPoints[0].y = clipRect.top;
			lpCurPoints[1].x = clipRect.right - 1;
			lpCurPoints[1].y = clipRect.bottom - 1;
			lpCurPoints[2].x = clipRect.left - 1;
			lpCurPoints[2].y = clipRect.bottom - 1;
			lpCurPoints += 3;
		}

		BOOL bSuccess;
		CPen *pOldPen;
		CPen penGridLines;
		penGridLines.CreatePen(PS_SOLID, 1, GetBorderColor());
		pOldPen = pDC->SelectObject(&penGridLines);

		bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
		ASSERT(bSuccess);

		pOldPen = pDC->SelectObject(pOldPen);

		delete[] lpPoints;
		delete[] lpPolyPoints;
	}
	delete[] lpColumnHeaderClipRect;

}

void CCursorGridCtrl::PaintCells(CDC *pDC)
{
	int i;
	struct stColInfo
	{
		int nIndex;
		CRect rect;
		CRect clipRect;
		CGridColumn* pColumn;
	} *pColInfo = new stColInfo[m_pColumns->GetSize()];
	stColInfo* pCurInfo = pColInfo;

	//HEAD
	CRect rowRect = GetGridRect(rtHead);
	rowRect.bottom = rowRect.top + m_nRowHeight;

	for (i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			CRect clipRect = GetColumnClipRect(pColumn);
			if (clipRect.right > rowRect.left && clipRect.left < rowRect.right)
			{
				pCurInfo->rect = GetColumnBoundsRect(pColumn);
				pCurInfo->clipRect.IntersectRect(clipRect, rowRect);

				SetVerticalLocationOfRect(pCurInfo->clipRect, pColumn);
				pCurInfo->rect.top = pCurInfo->clipRect.top;
				pCurInfo->rect.bottom = pCurInfo->clipRect.bottom;

				pCurInfo->nIndex = i;
				pCurInfo->pColumn = pColumn;
				pCurInfo++;
			}
		}
	}
	stColInfo* pEndInfo = pCurInfo;

	COLORREF crText, crBack;
	WORD trpBack;
	CFont* pFont;

	BOOL bIsFocused = IsFocused();
	for (i = 0; i < GetFullVisibleRows(rtHead); i++)
	{
		if (pDC->RectVisible(rowRect))
		{
			CGridRow* pGridRow = m_rows_head[i];
			CDataRow* pDataRow = pGridRow->GetDataRow();
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				CGridItem* pGridItem = pGridRow->m_cells[pCurInfo->nIndex];
				pFont = EvalCellFont(pGridItem, pGridRow);
				EvalCellColors_head(pGridItem, pGridRow, crText, crBack, trpBack);
				DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridItem, crText, crBack, trpBack, pFont);
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
		}
		else
		{
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
		}
		rowRect.OffsetRect(0, m_nRowHeight);
	}

	//BODY
	rowRect = GetGridRect(rtBody);
	rowRect.bottom = rowRect.top + m_nRowHeight;
	for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
	{
		pCurInfo->rect.top = rowRect.top;
		SetVerticalLocationOfRect(pCurInfo->rect, pCurInfo->pColumn);
		pCurInfo->clipRect.top = pCurInfo->rect.top;
		pCurInfo->clipRect.bottom = pCurInfo->rect.bottom;
	}

	int nTopRowIndex = GetTopRowIndex(rtBody);
	for (i = nTopRowIndex; i < nTopRowIndex + GetFullVisibleRows(rtBody); i++)
	{
		if (pDC->RectVisible(rowRect))
		{
			CGridRow* pGridRow = m_rows[i];
			CDataRow* pDataRow = pGridRow->GetDataRow();
			BOOL bIsSelected = m_SelectedRows.IsSelected(pDataRow);
			BOOL bIsCurrent = pGridRow->m_bIsCurrent;
			if (bIsCurrent)
				bIsCurrent = !m_SelectedRows.IsRemoved(pDataRow);

			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				BOOL bSelected = FALSE;
				BOOL bAlterSelected = FALSE;

				if (m_RowSelectMode == rsmCell)
				{
					if (m_nCurrentColumn == pCurInfo->nIndex)
						bSelected = bIsSelected || bIsCurrent;
				}
				else if (m_RowSelectMode == rsmRow)
					bSelected = bIsSelected || bIsCurrent;
				else if (m_RowSelectMode == rsmBoth)
				{
					bAlterSelected = bIsSelected || bIsCurrent;
					if (m_nCurrentColumn == pCurInfo->nIndex)
						bSelected = bAlterSelected;
				}


				BOOL bAlterBack = FALSE;
				if (m_bUseAlternationRowColor)
				{
					if (m_bEvenRow)
						bAlterBack = !(i & 1);
					else
						bAlterBack = (i & 1);
				}

				CGridItem* pGridItem = pGridRow->m_cells[pCurInfo->nIndex];
				pFont = EvalCellFont(pGridItem, pGridRow);
				EvalCellColors(pGridItem, pGridRow, bSelected, bAlterSelected, bAlterBack, crText, crBack, trpBack);
				DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridRow->m_cells[pCurInfo->nIndex], crText, crBack, trpBack, pFont);

				if (bIsFocused && m_RowSelectMode != rsmRow && pGridRow->m_bIsCurrent && m_nCurrentColumn == pCurInfo->nIndex)
				{
					CRect Rect;
					Rect.IntersectRect(&pCurInfo->rect, &pCurInfo->clipRect);
					if (m_bVerticalLines)
						Rect.right--;
					if (m_bHorizontalLines)
						Rect.bottom--;
					pDC->DrawFocusRect(Rect);
				}
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
			if (bIsFocused && m_RowSelectMode == rsmRow && pGridRow->m_bIsCurrent)
			{
				CRect Rect(rowRect);
				if (m_bVerticalLines)
					Rect.right--;
				if (m_bHorizontalLines)
					Rect.bottom--;
				pDC->DrawFocusRect(Rect);
			}
		}
		else
		{
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
		}
		rowRect.OffsetRect(0, m_nRowHeight);
	}

	//TAIL
	rowRect = GetGridRect(rtTail);
	rowRect.bottom = rowRect.top + m_nRowHeight;
	for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
	{
		pCurInfo->rect.top = rowRect.top;
		SetVerticalLocationOfRect(pCurInfo->rect, pCurInfo->pColumn);
		pCurInfo->clipRect.top = pCurInfo->rect.top;
		pCurInfo->clipRect.bottom = pCurInfo->rect.bottom;
	}

	nTopRowIndex = m_rows_tail.GetSize() - GetFullVisibleRows(rtTail);
	for (i = nTopRowIndex; i < m_rows_tail.GetSize(); i++)
	{
		if (pDC->RectVisible(rowRect))
		{
			CGridRow* pGridRow = m_rows_tail[i];
			CDataRow* pDataRow = pGridRow->GetDataRow();
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				CGridItem* pGridItem = pGridRow->m_cells[pCurInfo->nIndex];
				pFont = EvalCellFont(pGridItem, pGridRow);
				EvalCellColors_tail(pGridItem, pGridRow, crText, crBack, trpBack);
				DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridItem, crText, crBack, trpBack, pFont);
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
		}
		else
		{
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
			}
		}
		rowRect.OffsetRect(0, m_nRowHeight);
	}

	//фокусная рамка при отсутствии вывода
	if ((!m_pColumns->GetSize() || m_rows.GetSize() == 0) && IsFocused())
	{
		CRect rect = GetGridRect(rtBody);
		rect.bottom = rect.top + m_nRowHeight - 1;
		pDC->DrawFocusRect(rect);
	}

	delete[] pColInfo;
}

void CCursorGridCtrl::EvalCellColors(CGridItem* pGridItem, CGridRow* pGridRow, BOOL bSelected, BOOL bAlterSelected, BOOL bAlterBack, COLORREF& crText, COLORREF& crBack, WORD& trpBack)
{
	if (pGridItem->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_crBack;
	else if (pGridRow->m_crBack != CLR_DEFAULT)
		crBack = pGridRow->m_crBack;
	else if (pGridItem->m_pGridColumn->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_pGridColumn->m_crBack;
	else if (bAlterBack)
		crBack = m_crAlternationRowBack == CLR_DEFAULT ? COLOR_ALTERNATION : m_crAlternationRowBack;
	else if (m_crBack != CLR_DEFAULT)
		crBack = m_crBack;
	else
		crBack = ::GetSysColor(COLOR_WINDOW);

	if (pGridItem->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridItem->m_trpBack;
	else if (pGridRow->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridRow->m_trpBack;
	else if (pGridItem->m_pGridColumn->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridItem->m_pGridColumn->m_trpBack;
	else
	{
		if (m_trpBack != TRANSPARENT_DEFAULT)
			trpBack = m_trpBack;
		else
			trpBack = 0x00;
		if (bAlterBack && m_trpAlternationRowBack != TRANSPARENT_DEFAULT)
			trpBack = m_trpAlternationRowBack;
	}

	if (pGridItem->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_crText;
	else if (pGridItem->m_pGridColumn->m_bMarkNegatives && pGridItem->m_bIsNegative)
		if (m_crNegatives != CLR_DEFAULT)
			crText = m_crNegatives;
		else
			crText = 0xFF;
	else if (pGridRow->m_crText != CLR_DEFAULT)
		crText = pGridRow->m_crText;
	else if (pGridItem->m_pGridColumn->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_pGridColumn->m_crText;
	else if (m_crText != CLR_DEFAULT)
		crText = m_crText;
	else
		crText = ::GetSysColor(COLOR_WINDOWTEXT);

	if (bAlterSelected)
	{
		if (m_crSelectionBack2 == CLR_DEFAULT)
			crBack = aux_ShiftColor(crBack, -32);
		else if (m_crSelectionBack2 != COLOR_TRANSPARENT)
			crBack = m_crSelectionBack2;

		if (m_trpSelectionBack2 == TRANSPARENT_DEFAULT)
			trpBack = 0x00;
		else
			trpBack = m_trpSelectionBack2;

		if (m_crSelectionText2 != CLR_DEFAULT && m_crSelectionText2 != COLOR_TRANSPARENT)
			crText = m_crSelectionText2;
	}
	if (bSelected && (!GetEdit() || m_RowSelectMode == rsmRow))
	{
		if (m_crSelectionBack == CLR_DEFAULT)
			crBack = ::GetSysColor(COLOR_HIGHLIGHT);
		else if (m_crSelectionBack != COLOR_TRANSPARENT)
			crBack = m_crSelectionBack;

		if (m_trpSelectionBack == TRANSPARENT_DEFAULT)
			trpBack = 0x00;
		else
			trpBack = m_trpSelectionBack;

		if (m_crSelectionText == CLR_DEFAULT)
			crText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		else if (m_crSelectionText != COLOR_TRANSPARENT)
			crText = m_crSelectionText;
	}
}

void CCursorGridCtrl::EvalCellColors_head(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack, WORD& trpBack)
{
	if (pGridItem->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_crBack;
	else if (pGridRow->m_crBack != CLR_DEFAULT)
		crBack = pGridRow->m_crBack;
	else if (m_crBack_head != CLR_DEFAULT)
		crBack = m_crBack_head;
	else
		crBack = ::GetSysColor(COLOR_BTNFACE);

	if (pGridItem->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridItem->m_trpBack;
	else if (pGridRow->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridRow->m_trpBack;
	else if (m_trpBack_head != TRANSPARENT_DEFAULT)
		trpBack = m_trpBack_head;
//	else if (m_trpBack != TRANSPARENT_DEFAULT)
//		trpBack = m_trpBack;
	else
		trpBack = 0x00;

	if (pGridItem->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_crText;
	else if (pGridItem->m_pGridColumn->m_bMarkNegatives && pGridItem->m_bIsNegative)
		if (m_crNegatives != CLR_DEFAULT)
			crText = m_crNegatives;
		else
			crText = 0xFF;
	else if (pGridRow->m_crText != CLR_DEFAULT)
		crText = pGridRow->m_crText;
	else if (m_crText_head != CLR_DEFAULT)
		crText = m_crText_head;
	else
		crText = ::GetSysColor(COLOR_BTNTEXT);
}

void CCursorGridCtrl::EvalCellColors_tail(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack, WORD& trpBack)
{
	if (pGridItem->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_crBack;
	else if (pGridRow->m_crBack != CLR_DEFAULT)
		crBack = pGridRow->m_crBack;
	else if (m_crBack_tail != CLR_DEFAULT)
		crBack = m_crBack_tail;
	else
		crBack = ::GetSysColor(COLOR_BTNFACE);

	if (pGridItem->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridItem->m_trpBack;
	else if (pGridRow->m_trpBack != TRANSPARENT_DEFAULT)
		trpBack = pGridRow->m_trpBack;
	else if (m_trpBack_tail != TRANSPARENT_DEFAULT)
		trpBack = m_trpBack_tail;
//	else if (m_trpBack != TRANSPARENT_DEFAULT)
//		trpBack = m_trpBack;
	else
		trpBack = 0x00;

	if (pGridItem->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_crText;
	else if (pGridItem->m_pGridColumn->m_bMarkNegatives && pGridItem->m_bIsNegative)
		if (m_crNegatives != CLR_DEFAULT)
			crText = m_crNegatives;
		else
			crText = 0xFF;
	else if (pGridRow->m_crText != CLR_DEFAULT)
		crText = pGridRow->m_crText;
	else if (m_crText_tail != CLR_DEFAULT)
		crText = m_crText_tail;
	else
		crText = ::GetSysColor(COLOR_BTNTEXT);
}

CFont* CCursorGridCtrl::EvalCellFont(CGridItem* pGridItem, CGridRow* pGridRow)
{
	CFont* pFont = &m_font;
	if (pGridItem->m_pFont)
		pFont = pGridItem->m_pFont;
	else if (pGridRow->m_pFont)
		pFont = pGridRow->m_pFont;
	else if (pGridItem->m_pGridColumn->m_pFont)
		pFont = pGridItem->m_pGridColumn->m_pFont;

	return pFont;
}

void CCursorGridCtrl::DrawCell(CDC* pDC, LPRECT pRect, LPRECT pClipRect, CGridItem* pGridItem, COLORREF crText, COLORREF crBack, WORD trpBack, CFont* pFont)
{
    CRect rect(pRect);
    if (!rect.Width())
        return;
    
    int nRetVal;

	CRect FillRect;
	FillRect.IntersectRect(rect, pClipRect);

	if (trpBack == 0x00)
		pDC->FillSolidRect(FillRect, crBack);
	else if (trpBack < 0xFF)
	{
		CDC dc2;
		dc2.CreateCompatibleDC(pDC);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, FillRect.Width(), FillRect.Height());
		CBitmap *pOldBitmap = dc2.SelectObject(&bitmap);
		dc2.FillSolidRect(0, 0, FillRect.Width(), FillRect.Height(), crBack);
		
		BLENDFUNCTION blend;
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.AlphaFormat = 0;
		blend.SourceConstantAlpha = 0xFF-trpBack;
		::AlphaBlend(*pDC, FillRect.left, FillRect.top, FillRect.Width(), FillRect.Height(),
			dc2, 0, 0, FillRect.Width(), FillRect.Height(), blend);
	}
	
	pDC->SetTextColor(crText);
    
	CPoint point = pDC->GetWindowOrg();
	CRect rectClip(FillRect);
	rectClip.OffsetRect(-point.x, -point.y);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rectClip);
	pDC->SelectClipRgn(&rgn);

	pGridItem->m_nNoTextOffset = rect.left;

	if (pGridItem->m_wFlagsMask & CGI_SYSIMAGE && rect.Width())
	{
		if (!pGridItem->m_pSysImageNumbers->empty())
		{
			if (m_pSysImages)
			{
				for(CGridItem::IMAGEREFS::iterator i = pGridItem->m_pSysImageNumbers->begin(); i != pGridItem->m_pSysImageNumbers->end(); ++i)
				{
					int nImageNumber = *i;
					if (nImageNumber < m_pSysImages->GetImageCount())
					{
						m_pSysImages->Draw(pDC, nImageNumber, CPoint(rect.left, rect.top), ILD_NORMAL);
						rect.left += m_nSysImageWidth;
					}
				}
			}
		}
	}

	if (pGridItem->m_wFlagsMask & CGI_CHECKBOX && rect.Width())
    {
        int nImageIndex;
        switch (pGridItem->m_wFlagsMask & CGI_CHECKBOX_STATE)
        {
        case CGI_CHECKBOX_UNCHECKED:
			nImageIndex = m_nSingleLineHeight > CHECKBOX_PLACE_SIZE ? iiCheckBoxUnchecked : iiSmallCheckBoxUnckecked;
			break;
        case CGI_CHECKBOX_CHECKED:
			nImageIndex = m_nSingleLineHeight > CHECKBOX_PLACE_SIZE ? iiCheckBoxChecked : iiSmallCheckBoxChecked;
			break;
        default:
			nImageIndex = m_nSingleLineHeight > CHECKBOX_PLACE_SIZE ? iiCheckBoxUndefined : iiSmallCheckBoxUndefined;
			break;
        }
        m_ilInternalImages.Draw(pDC, nImageIndex, rect.TopLeft(), ILD_NORMAL);
        rect.left += CHECKBOX_PLACE_SIZE;
    }

	if (pGridItem->m_wFlagsMask & CGI_IMAGE && rect.Width())
	{
		int nImageWidth = 0;
		if (pGridItem->m_pImageNumbers->empty())
		{
			CRect ImageRect(rect);
			if (pGridItem->DrawImage(pDC, ImageRect))
			{
				nImageWidth = ImageRect.Width();
				pGridItem->m_pGridColumn->m_nImageWidth = nImageWidth;
				rect.left += nImageWidth;
			}
		}
		else
		{
			CImageList* pRowsImages = pGridItem->m_pGridColumn->m_pRowsImages;
			if (pRowsImages)
			{
				nImageWidth = pGridItem->m_pGridColumn->m_nImageWidth;
				for(CGridItem::IMAGEREFS::iterator i = pGridItem->m_pImageNumbers->begin(); i != pGridItem->m_pImageNumbers->end(); ++i)
				{
					int nImageNumber = *i;
					if (nImageNumber < pRowsImages->GetImageCount())
					{
						pRowsImages->Draw(pDC, nImageNumber, CPoint(rect.left, rect.top), ILD_NORMAL);
						rect.left += nImageWidth;
					}
				}
			}
		}
	}

	pGridItem->m_nNoTextOffset = rect.left - pGridItem->m_nNoTextOffset;

    if (pGridItem->m_wFlagsMask & CGI_TEXT && pGridItem->m_strText.GetLength() && rect.Width())
    {
        CFont* pOldFont = pDC->SelectObject(pFont);

		DRAWTEXTPARAMS dtp;
        dtp.cbSize = sizeof(DRAWTEXTPARAMS);
        dtp.iLeftMargin = GetLeftTextMargin();
        dtp.iRightMargin = GetRightTextMargin();

        UINT nFlags = DT_NOPREFIX | DT_NOCLIP;

        WORD wHAlign = pGridItem->m_wFlagsMask & CGI_TEXT_H_ALIGN;
        if (wHAlign == CGI_TEXT_LEFT)
            nFlags |= DT_LEFT | DT_END_ELLIPSIS;
        else if (wHAlign == CGI_TEXT_RIGHT)
        {
            nFlags |= DT_RIGHT | DT_END_ELLIPSIS;
            dtp.iRightMargin++; //one pixel for vertical line
        }
        else if (wHAlign == CGI_TEXT_CENTER)
            nFlags |= DT_CENTER;
        else //default is left
            nFlags |= DT_LEFT | DT_END_ELLIPSIS;

		if (pGridItem->m_wFlagsMask & CGI_TEXT_WORDBREAK)
			nFlags |= DT_WORDBREAK;

        char arTextBuffer[1024];
        int nLen = pGridItem->m_strText.GetLength();
        if (nLen < 1024)
            strcpy(arTextBuffer, pGridItem->m_strText);
        else
        {
            nLen = 1023;
            strncpy(arTextBuffer, pGridItem->m_strText, nLen);
            arTextBuffer[nLen] = 0;
        }

        rect.OffsetRect(0, GetTopTextMargin());
        pDC->SetBkMode(TRANSPARENT);
        nRetVal = ::DrawTextEx(
            pDC->m_hDC,
            arTextBuffer,
            lstrlen(arTextBuffer),
            rect,
            nFlags,
            &dtp
            );

		pDC->SelectObject(pOldFont);
    }

	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();
}

void CCursorGridCtrl::DrawHeader(CDC *pDC, CGridColumn* pColumn, LPRECT pRect, LPRECT pClipRect)
{
    int nRetVal;
    COLORREF cr;

	CGridColumn* pActiveColumn = GetCurrentColumn();
	if (m_ActiveHeaderStyle == ahsSelected)
	{
		if (pActiveColumn->m_bMergeCaption) //объединенный заголовок
		{
			//поиск активного заголовка - первого из группы объединенных
			for (int i = m_pColumns->IndexOf(pActiveColumn)-1; i >= 0; --i)
			{
				CGridColumn* pProbeActiveColumn = m_pColumns->GetAt(i);
				if (!pProbeActiveColumn->m_bMergeCaption || i==0)
				{
					pActiveColumn = pProbeActiveColumn;
					break;
				}
			}
		}
	}

    if (pColumn->m_crHeaderBack != CLR_DEFAULT)
        cr = pColumn->m_crHeaderBack;
    else
	{
		if (m_crHeadersBack != CLR_DEFAULT)
			cr = m_crHeadersBack;
		else
			cr = GetSysColor(COLOR_BTNFACE);
		if (m_HeadersStyle != hs3D && m_ActiveHeaderStyle == ahsSelected && pColumn == pActiveColumn)
			cr = aux_ShiftColor(cr, -32);
	}

	CRect FillRect;
	FillRect.IntersectRect(pRect, pClipRect);

    pDC->FillSolidRect(FillRect, cr);

    if (pColumn->m_crHeaderText != CLR_DEFAULT)
        cr = pColumn->m_crHeaderText;
	else if (m_crHeadersText != CLR_DEFAULT)
		cr = m_crHeadersText;
	else
		cr = COLOR_WINDOWTEXT;

    pDC->SetTextColor(cr);

    CRect rect(pRect);
    if (pColumn->m_OrderMarker != omNone)
        rect.right -= 16;
    
	CRect ClipRect(FillRect);
	CPoint point = pDC->GetWindowOrg();
	ClipRect.OffsetRect(-point.x, -point.y);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(ClipRect);
	pDC->SelectClipRgn(&rgn);

    if (rect.Width() && pColumn->m_strCaption.GetLength())
    {
        CFont* pOldFont = NULL;
        if (pColumn->m_pHeaderFont)
            pOldFont = pDC->SelectObject(pColumn->m_pHeaderFont);

        DRAWTEXTPARAMS dtp;
        dtp.cbSize = sizeof(DRAWTEXTPARAMS);
        dtp.iLeftMargin = GetLeftTextMargin();
        dtp.iRightMargin = GetRightTextMargin();

        UINT nFlags = DT_END_ELLIPSIS | DT_NOPREFIX | DT_LEFT | DT_NOCLIP;
		if (pColumn->m_bWordBreak)
			nFlags |= DT_WORDBREAK;

        char arTextBuffer[1024];
        int nLen = pColumn->m_strCaption.GetLength();
        if (nLen < 1024)
		{
			strcpy(arTextBuffer, pColumn->m_strCaption);
		}
        else
        {
            nLen = 1023;
            strncpy(arTextBuffer, pColumn->m_strCaption, nLen);
            arTextBuffer[nLen] = 0;
        }
        
        rect.OffsetRect(0, GetTopTextMargin());
        pDC->SetBkMode(TRANSPARENT);
        nRetVal = ::DrawTextEx(
            pDC->m_hDC,
            arTextBuffer,
            lstrlen(arTextBuffer),
            rect,
            nFlags,
            &dtp
            );

        if (pOldFont)
            pDC->SelectObject(pOldFont);
    }

    if (pColumn->m_OrderMarker != omNone)
    {
        int nImageIndex;
        switch (pColumn->m_OrderMarker)
        {
        case omAsc: nImageIndex = iiOrderAsc; break;
        case omDesc: nImageIndex = iiOrderDesc; break;
        case omAsc2: nImageIndex = iiOrderAsc2; break;
        case omDesc2: nImageIndex = iiOrderDesc2; break;
        case omAsc3: nImageIndex = iiOrderAsc3; break;
        case omDesc3: nImageIndex = iiOrderDesc3; break;
        }
        m_ilInternalImages.Draw(pDC, nImageIndex, CPoint(pRect->right - 16, pRect->top), ILD_NORMAL);
    }
	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();

	if (m_HeadersStyle == hs3D)
	{
		if (m_ActiveHeaderStyle == ahsSelected && pColumn == pActiveColumn)
			Paint3DFrame_sunken(pDC, FillRect, BF_RECT);
		else
			Paint3DFrame_raised(pDC, FillRect, BF_RECT);
	}
}

void CCursorGridCtrl::Paint3DFrame_raised( CDC *pDC, LPRECT lpRect, UINT nSideFlags )
{
	CRect rect(*lpRect);
	BOOL bSuccess;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDOUTER, nSideFlags & (~BF_TOPLEFT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_BOTTOM)
		rect.bottom--;
	if (nSideFlags & BF_RIGHT)
		rect.right--;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDINNER, nSideFlags & (~BF_BOTTOMRIGHT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_TOP)
		rect.top++;
	if (nSideFlags & BF_LEFT)
		rect.left++;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDINNER, nSideFlags & (~BF_TOPLEFT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_BOTTOM)
		rect.bottom--;
	if (nSideFlags & BF_RIGHT)
		rect.right--;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDOUTER, nSideFlags & (~BF_BOTTOMRIGHT));
	ASSERT(bSuccess);
}

void CCursorGridCtrl::Paint3DFrame_sunken( CDC *pDC, LPRECT lpRect, UINT nSideFlags )
{
 	CRect rect(*lpRect);
 	CBrush br(GetLinesColor());
	pDC->FrameRect(rect, &br);
 	return;

// 	CRect rect(*lpRect);
// 	BOOL bSuccess;
// 
// 	bSuccess = pDC->DrawEdge(rect, BDR_SUNKENOUTER, nSideFlags);
// 	ASSERT(bSuccess);
// 
//   	if (nSideFlags & BF_RIGHT)
//   		rect.right--;
// 
// 	bSuccess = pDC->DrawEdge(rect, BDR_SUNKENINNER, nSideFlags);
// 	ASSERT(bSuccess);
}

void CCursorGridCtrl::RecalcFixedEx()
{
	m_nFixedLeftEx = 0;
	m_nFixedRightEx = 0;
	
	int i = 0;
	if (m_nFixedLeft)
	{
		for (; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->m_bVisible)
			{
				if (pColumn->m_nFamily >= m_nFixedLeft)
					break;
				m_nFixedLeftEx++;
			}
		}
	}
	
	if (m_nFixedRight)
	{
		int nCurFamily = -1;
		int nFixedRight = m_nFixedRight + 1;
		for (int j = m_pColumns->GetUpperBound(); j >= i; j--)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(j);
			if (pColumn->m_bVisible)
			{
				if (nCurFamily != pColumn->m_nFamily)
				{
					nCurFamily = pColumn->m_nFamily;
					nFixedRight--;
					if (!nFixedRight)
						break;
				}
				m_nFixedRightEx++;
			}
		}
	}
}

void CCursorGridCtrl::RecalcColumnsLineCount()
{
	//Первый проход
	//пересчет максимального количества линий колонок в строке
	//и определение номера начальной линии и семейства для каждой колонки
	//Семейство - это совокупность колонок от clNewColumn включительно до следующей clNewColumn
	m_nColumnsLineCount = 1;
	int nLineNumber = 1;
	int nFamily = -1;

	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn->m_Location != clNewColumn && nFamily == -1)
				pColumn->m_Location = clNewColumn;
			
			switch(pColumn->m_Location)
			{
			case clNewColumn:
				nLineNumber = 1;
				nFamily++;
				break;
			case clNextLine:
				nLineNumber++;
				if (m_nColumnsLineCount < nLineNumber)
					m_nColumnsLineCount = nLineNumber;
				break;
			case clSomeColumn:
			default:
				break;
			}
			pColumn->m_nLineNumber = nLineNumber;
			pColumn->m_nFamily = nFamily;
		}
	}
	RecalcRowHeight();
	RecalcFixedEx();

	//Второй проход
	//определение количества линий занимаемых каждой колонкой
	int nStartLineIndex = 0;
	int nLinesLeft = m_nColumnsLineCount;

	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			pColumn->m_nColumnLineCount = 1;
			switch(pColumn->m_Location)
			{
			case clNewColumn:
				for (int j = nStartLineIndex; j < i; j++)
				{
					CGridColumn* pPrevColumn = m_pColumns->GetAt(j);
					if (pPrevColumn->m_bVisible)
						pPrevColumn->m_nColumnLineCount = nLinesLeft;
				}
				nStartLineIndex = i;
				nLinesLeft = m_nColumnsLineCount;
				break;
			case clNextLine:
				nStartLineIndex = i;
				nLinesLeft--;
				break;
			case clSomeColumn:
			default:
				break;
			}
		}
	}
	for (int j = nStartLineIndex; j < m_pColumns->GetSize(); j++)
	{
		CGridColumn* pPrevColumn = m_pColumns->GetAt(j);
		if (pPrevColumn->m_bVisible)
			pPrevColumn->m_nColumnLineCount = nLinesLeft;
	}
}

void CCursorGridCtrl::RecalcColumnsWidthMulDiv()
{
	CGridColumn** ppCol = new CGridColumn*[m_pColumns->GetSize()];
	CGridColumn** ppCurCol = ppCol;
	CGridColumn** ppNewCol = ppCol;
	CGridColumn** ppSomeCol = ppCol;

	int nWidthMul = 0;
	int nMaxWidthMul = 0;
	int nWidthDiv = 0;

	int nFixedWidthSum = 0;

	int nMaxFamilyResizeMinWidth = 0;
	int nFamilyResizeMinWidth = 0;

	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			*ppCurCol = pColumn;
			switch(pColumn->m_Location)
			{
			case clNewColumn:
				for (;ppSomeCol < ppCurCol; ppSomeCol++)
				{
					CGridColumn* pCol = *ppSomeCol;
					pCol->m_nMinWidthDiv = nWidthDiv;
					pCol->m_nFamilyFixedWidth = nFixedWidthSum;
				}
				nFixedWidthSum = 0;

				nWidthDiv = pColumn->m_nMinWidth;
				
				nMaxFamilyResizeMinWidth = max(nMaxFamilyResizeMinWidth, nFamilyResizeMinWidth);

				int nFixW = 0;
				int nMaxFixW = 0;
				CGridColumn** ppNewCol2 = ppNewCol;
				for (;ppNewCol < ppCurCol; ppNewCol++)
				{
					CGridColumn* pCol = *ppNewCol;
					nMaxFixW = max(nMaxFixW, nFixW);
					if (pCol->m_Location == clNextLine)
						nFixW = 0;
					if (pCol->m_bFixedWidth)
					{
						pCol->m_nMinWidthMul = nMaxWidthMul;
						nFixW += MulDiv(pCol->m_nMinWidth, pCol->m_nMinWidthMul, pCol->m_nMinWidthDiv);
					}
					else
					{
						pCol->m_nMinWidthMul = nMaxWidthMul;
					}
					pCol->m_nFamilyWidth = nMaxWidthMul;
					pCol->m_nFamilyResizableWidth = pCol->m_nFamilyWidth;
					pCol->m_nFamilyResizeMinWidth = nMaxFamilyResizeMinWidth;
				}

				nMaxFixW = max(nMaxFixW, nFixW);
				for (;ppNewCol2 < ppCurCol; ppNewCol2++)
				{
					if (nMaxFixW == (*ppNewCol2)->m_nFamilyResizableWidth)
					{
						(*ppNewCol2)->m_nFamilyResizableWidth = 0;
					}
				}

				nWidthMul = pColumn->m_nMinWidth;
				nMaxWidthMul = pColumn->m_nMinWidth;

				nMaxFamilyResizeMinWidth = 0;
				nFamilyResizeMinWidth = 0;

				break;
			case clNextLine:
				for (;ppSomeCol < ppCurCol; ppSomeCol++)
				{
					CGridColumn* pCol = *ppSomeCol;
					pCol->m_nMinWidthDiv = nWidthDiv;
					pCol->m_nFamilyFixedWidth = nFixedWidthSum;
				}
				nFixedWidthSum = 0;

				nWidthDiv = pColumn->m_nMinWidth;

				nWidthMul = pColumn->m_nMinWidth;
				nMaxWidthMul = max(nMaxWidthMul, nWidthMul);

				nMaxFamilyResizeMinWidth = max(nMaxFamilyResizeMinWidth, nFamilyResizeMinWidth);
				nFamilyResizeMinWidth = 0;

				break;
			case clSomeColumn:
			default:
				nWidthDiv += pColumn->m_nMinWidth;
				nWidthMul += pColumn->m_nMinWidth;
				nMaxWidthMul = max(nMaxWidthMul, nWidthMul);
				break;
			}
			if (pColumn->m_bFixedWidth)
			{
				nFixedWidthSum += pColumn->m_nMinWidth;
				nFamilyResizeMinWidth += pColumn->m_nMinWidth;
			}
			else
			{
				nFamilyResizeMinWidth += RESIZE_MIN_WIDTH;
			}
			ppCurCol++;
		}
	}

	for (;ppSomeCol < ppCurCol; ppSomeCol++)
	{
		CGridColumn* pCol = *ppSomeCol;
		pCol->m_nMinWidthDiv = nWidthDiv;
		pCol->m_nFamilyFixedWidth = nFixedWidthSum;
	}
	nFixedWidthSum = 0;

	nMaxFamilyResizeMinWidth = max(nMaxFamilyResizeMinWidth, nFamilyResizeMinWidth);

	int nFixW = 0;
	int nMaxFixW = 0;
	CGridColumn** ppNewCol2 = ppNewCol;
	for (;ppNewCol < ppCurCol; ppNewCol++)
	{
		CGridColumn* pCol = *ppNewCol;
		nMaxFixW = max(nMaxFixW, nFixW);
		if (pCol->m_Location == clNextLine)
			nFixW = 0;
		if (pCol->m_bFixedWidth)
		{
			pCol->m_nMinWidthMul = nMaxWidthMul;
			nFixW += MulDiv(pCol->m_nMinWidth, pCol->m_nMinWidthMul, pCol->m_nMinWidthDiv);
		}
		else
		{
			pCol->m_nMinWidthMul = nMaxWidthMul;
		}
		pCol->m_nFamilyWidth = nMaxWidthMul;
		pCol->m_nFamilyResizableWidth = pCol->m_nFamilyWidth;
		pCol->m_nFamilyResizeMinWidth = nMaxFamilyResizeMinWidth;
	}

	nMaxFixW = max(nMaxFixW, nFixW);
	for (;ppNewCol2 < ppCurCol; ppNewCol2++)
	{
		if (nMaxFixW == (*ppNewCol2)->m_nFamilyResizableWidth)
		{
			(*ppNewCol2)->m_nFamilyResizableWidth = 0;
		}
	}

	delete[] ppCol;
}

void CCursorGridCtrl::RecalcColumnsWidth()
{
	RecalcColumnsWidthMulDiv();

    CRect ClientRect;
    GetClientRectEx(ClientRect);
    int nGridWidth;
    if (ClientRect.Width() < m_pColumns->m_nMinWidth)
        nGridWidth = m_pColumns->m_nResizableMinWidth;
    else
        nGridWidth = ClientRect.Width() - (m_pColumns->m_nMinWidth - m_pColumns->m_nResizableMinWidth);
	

	CRect rect = GetHeaderRect();
	int nLeftOffset = rect.left;
	rect.right = nLeftOffset;
	rect.bottom = GetHeaderHeight();

	int nLeftOffsetNewCol = rect.left;
	
	//Массив для хранения указателей на колонки
	//крайние правые колонки в линиях выше нижней
	//для выравнивания ширин по нижней строке
	CGridColumn** ppColInEndLineArr = new CGridColumn*[m_pColumns->GetSize()];
	CGridColumn** ppColInEndLine = ppColInEndLineArr;

	CGridColumn* pLastColumn = NULL;
    for (int i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
        if (pColumn->m_bVisible)
        {
			switch(pColumn->m_Location)
			{
			case clNewColumn:
				//лево смещаем на ширину предыдущей колонки
				rect.left += rect.Width();
				nLeftOffsetNewCol = rect.left;

				if (pLastColumn)
					while(ppColInEndLine > ppColInEndLineArr)
						(*--ppColInEndLine)->m_rect.right = pLastColumn->m_rect.right;

				break;
			case clNextLine:
				//лево возвращаем
				rect.left = nLeftOffsetNewCol;

				if (pLastColumn)
					*ppColInEndLine++ = pLastColumn;

			    break;
			case clSomeColumn:
			default:
				//лево смещаем на ширину предыдущей колонки
				rect.left += rect.Width();
			    break;
			}
			//право смещаем на ширину текущей колонки
			if (pColumn->m_nFamilyResizableWidth==0 || pColumn->m_bFixedWidth)
			{
				int nColWidth = pColumn->m_nMinWidth;
				if (pColumn->m_nFamilyFixedWidth == pColumn->m_nMinWidthDiv || !pColumn->m_bFixedWidth)
				{
					nColWidth = MulDiv(nColWidth, pColumn->m_nMinWidthMul, pColumn->m_nMinWidthDiv);
					if (!pColumn->m_bFixedWidth)
					{
						nColWidth += MulDiv(pColumn->m_nFamilyFixedWidth, pColumn->m_nMinWidthMul, pColumn->m_nMinWidthDiv)-pColumn->m_nFamilyFixedWidth;
					}
				}
				rect.right = rect.left + nColWidth;
			}
			else
			{
				int nColWidth = MulDiv(pColumn->m_nMinWidth, pColumn->m_nMinWidthMul, pColumn->m_nMinWidthDiv);
				
				//прибавка от фиксированных колонок этой линии,...
				int nAddWidth = MulDiv(pColumn->m_nFamilyFixedWidth, pColumn->m_nMinWidthMul, pColumn->m_nMinWidthDiv) - pColumn->m_nFamilyFixedWidth;
				//...распределенная между не фикс.колонками этой линии
				nAddWidth = MulDiv(nAddWidth, pColumn->m_nMinWidth, pColumn->m_nMinWidthDiv-pColumn->m_nFamilyFixedWidth);
				nColWidth += nAddWidth;

				//общее уширение
				int nDx = nGridWidth - m_pColumns->m_nResizableMinWidth;
				
				//уширение приходящееся на семейство
				//int nDxF = MulDiv(nDx, pColumn->m_nFamilyWidth, m_pColumns->m_nMinWidth);
				////уширение приходящееся на семейство вариант 2
				int nDxF = MulDiv(nDx, pColumn->m_nFamilyResizableWidth, m_pColumns->m_nResizableMinWidth);

				//часть уширения семества, которой может воспользоваться эта колонка в линии
				int nDxC = MulDiv(nDxF, pColumn->m_nMinWidth, pColumn->m_nMinWidthDiv - pColumn->m_nFamilyFixedWidth);

				nColWidth = nColWidth + nDxC;
				rect.right = rect.left + nColWidth;
			}

            pColumn->m_rect = rect;
			SetVerticalLocationOfRect(pColumn->m_rect, pColumn);

            pLastColumn = pColumn;
        }
    }
    if (pLastColumn)
    {
		//выравнивание крайней правой колонки по ширине ТП
		//- исправление погрешностей округления при целочисленном пересчете ширин
		if (ClientRect.Width() < m_pColumns->m_nMinWidth)
			pLastColumn->m_rect.right = nLeftOffset + m_pColumns->m_nMinWidth;
		else
			pLastColumn->m_rect.right = ClientRect.right;

		while(ppColInEndLine > ppColInEndLineArr)
			(*--ppColInEndLine)->m_rect.right = pLastColumn->m_rect.right;
    }

	delete[] ppColInEndLineArr;

    SetHScrollInfo();
    Invalidate();
}

void CCursorGridCtrl::RecalcRowHeight()
{
	m_nSingleLineHeight = m_nFontHeight + GetTopTextMargin() + GetBottomTextMargin();
	m_nRowHeight = m_nSingleLineHeight * m_nColumnsLineCount;
    CalcRowsParams();
}

void CCursorGridCtrl::SetHScrollInfo()
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.nMin = 0;
    si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;

	int nFixedWidth = GetFixedWidth_left() + GetFixedWidth_right();
    int nPageWidth =  GetGridRect().Width() - nFixedWidth + 1;
	int nColumnsWidth = m_pColumns->GetVisibleWidth() - nFixedWidth;

    if (nPageWidth > 0 && nPageWidth < nColumnsWidth)
    {
        si.nMax = nColumnsWidth;
        si.nPos = GetOrigin();
        si.nPage = nPageWidth;
        SetScrollInfo(SB_HORZ, &si);
        ShowScrollBar(SB_HORZ);
    }
    else
    {
        si.nMax = 0;
        si.nPos = 0;
        si.nPage = 0;
        SetScrollInfo(SB_HORZ, &si);
        ShowScrollBar(SB_HORZ, FALSE);
    }
}

void CCursorGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    RecalcColumnsWidth();
    CalcRowsParams();
	RepositionEdit();
}

BOOL CCursorGridCtrl::HScrollLine(SCROLLINFO& si, int nDirection)
{
	int nPrevPos = si.nPos;

	if (nDirection < 0)
	{
		int nLeft = si.nPos;
		for (int i = 0; i < m_pColumns->GetSize(); i++)
		{
			LPRECT pRect = m_pColumns->GetAt(i)->m_rect;
			if (pRect->right >= nLeft)
			{
				si.nPos += pRect->left - nLeft;
				break;
			}
		}
	}
	else if (nDirection > 0)
	{
		CRect ClientRect;
		GetClientRectEx(ClientRect);
        int nRight = si.nPos + ClientRect.right;
        for (int i = m_pColumns->GetUpperBound(); i >= 0; i--)
        {
            LPRECT pRect = m_pColumns->GetAt(i)->m_rect;
            if (pRect->left <= nRight)
            {
                si.nPos += pRect->right - nRight;
                break;
            }
        }
	}
	if (nPrevPos != si.nPos)
	{
		SetScrollInfo(SB_HORZ, &si);
		Invalidate();
		return TRUE;
	}
	else
		return FALSE;

}

void CCursorGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this) 
		SetFocus();

	if (!GetEdit())
	{
		SCROLLINFO si;
		CRect rect;
		BOOL bSuccess;

		m_Tip.Hide();

		GetClientRectEx(rect);

		bSuccess = GetScrollInfo(SB_HORZ, &si);
		ASSERT(bSuccess);

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;

		switch (nSBCode)
		{
		case SB_LINELEFT:
			HScrollLine(si, -1);
			break;
		case SB_LINERIGHT:
			HScrollLine(si, 1);
			break;
		case SB_PAGELEFT:
			si.nPos -= rect.right;
			if (si.nPos < 0)
				si.nPos = 0;
			bSuccess = SetScrollInfo(SB_HORZ, &si);
			if (!bSuccess)
				break;
			Invalidate();
			break;
		case SB_PAGERIGHT:
			si.nPos += rect.right;
			bSuccess = SetScrollInfo(SB_HORZ, &si);
			if (!bSuccess)
				break;
			Invalidate();
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			bSuccess = GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
			ASSERT(bSuccess);
			si.fMask = SIF_POS;
			si.nPos = si.nTrackPos;
			bSuccess = SetScrollInfo(SB_HORZ, &si);
			if (!bSuccess)
				break;
			Invalidate();
			break;
		default:
			break;
		}
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

UINT CCursorGridCtrl::OnNcHitTest(CPoint point) 
{
    m_hitTest = point;
    ScreenToClient(&m_hitTest);
	
	return CWnd::OnNcHitTest(point);
}

BOOL CCursorGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (nHitTest == HTCLIENT && m_bShowHeader)
    {
        CRect rect(GetHeaderRect());
        
        if (rect.PtInRect(m_hitTest) && GetResizeHeaderAt(m_hitTest))
        {
            HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(AFX_IDC_HSPLITBAR), RT_GROUP_CURSOR);
            HCURSOR hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
			::SetCursor(hCursor); 
            return TRUE;
        }
    }
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

CGridColumn* CCursorGridCtrl::GetResizeHeaderAt(CPoint point)
{
    CRect rect;
    for (int i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pGridColumn = m_pColumns->GetAt(i);
        if (pGridColumn->m_bVisible)
        {
			rect = GetColumnClipRect(pGridColumn);
			SetVerticalLocationOfRect(rect, pGridColumn);

			rect.left = rect.right - RESIZE_AREA_WIDTH;
			rect.right += RESIZE_AREA_WIDTH;
			if (rect.PtInRect(point))
			{
				int j = GetNextColumn(i,1);
				if (j == -1)
				{
					if (pGridColumn->m_nFamilyResizableWidth)
						return pGridColumn;
				}
				else
				{
					CGridColumn* pNextGridColumn = m_pColumns->GetAt(j);
					if (pGridColumn->m_nFamilyResizableWidth)
					{
						if (pNextGridColumn->m_Location != clSomeColumn || !pGridColumn->m_bFixedWidth && !pNextGridColumn->m_bFixedWidth)
							return pGridColumn;
					}
					else
					{
						if (pNextGridColumn->m_Location == clSomeColumn && !pGridColumn->m_bFixedWidth && !pNextGridColumn->m_bFixedWidth)
							return pGridColumn;
					}
				}
				break;
			}
        }
    }

    return NULL;
}

int CCursorGridCtrl::HitTestColumn(CPoint point, RegionType rType, int nRowIndex/*=-1*/)
{
    for (int i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pGridColumn = m_pColumns->GetAt(i);
        if (pGridColumn->m_bVisible)
		{
			if (rType == rtHeader)
			{
				CRect rect = GetColumnClipRect(pGridColumn);
				SetVerticalLocationOfRect(rect, pGridColumn);
				if (rect.PtInRect(point))
					return i;
			}
			else
			{
				//CRect rect = GetColumnClipRect(pGridColumn);
				//rect.IntersectRect(rect, GetCellBounds(rType, pGridColumn, nRowIndex));
				CRect rect = GetCellBounds(rType, pGridColumn, nRowIndex);
				if (rect.PtInRect(point))
					return i;
			}
		}
    }
    return -1;
}

int CCursorGridCtrl::HitTestHeader(CPoint point)
{
	return GetHeaderRect().PtInRect(point) ? HitTestColumn(point, rtHeader) : -1;
}

CGridColumn* CCursorGridCtrl::PrepareColResize(CGridColumn* pCapturedColumn)
{
	int nIndex = m_pColumns->IndexOf(pCapturedColumn);
	nIndex = GetNextColumn(nIndex, 1);
	if (nIndex != -1)
		m_CapturedLocation = m_pColumns->GetAt(nIndex)->m_Location;
	else
		m_CapturedLocation = clNewColumn;

	if (m_CapturedLocation != clSomeColumn)
	{
		for (int i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->m_bVisible)
				pColumn->m_nWidthBeforeResize = pColumn->m_rect.Width();
		}

		int nIndex = m_pColumns->IndexOf(pCapturedColumn);
		while (pCapturedColumn && pCapturedColumn->m_Location != clNewColumn)
		{
			nIndex = GetNextColumn(nIndex, -1);
			pCapturedColumn = m_pColumns->GetAt(nIndex);
		}
		if (pCapturedColumn)
			m_nCapturedColumn_width = FamilyWidth(pCapturedColumn->m_nFamily);
	}
	return pCapturedColumn;
}

void CCursorGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) 
		SetFocus();

	if (!GetEdit())
	{
		m_Tip.Hide();
		m_hitMouseButton = point;

		int nColumn, nRow, nImageIndex;
		RegionType rType;

		GridHitTest ht = HitTest(point, nColumn, nRow, rType, nImageIndex);

		switch(ht)
		{
		case ghtHeader:
			m_pCapturedColumn = GetResizeHeaderAt(point);

			if (m_pCapturedColumn)
			{
				m_CaptureType = ctResizeColumn;
				m_pCapturedColumn = PrepareColResize(m_pCapturedColumn);
				if (m_pCapturedColumn)
				{
					CRect bounds = GetColumnBoundsRect(m_pCapturedColumn);
					CRect clip = GetColumnClipRect(m_pCapturedColumn);
					m_nCapturedColumn_left = bounds.left - (bounds.right - clip.right);
				}
			}
			else
			{
				m_pCapturedColumn = m_pColumns->GetAt(nColumn);
				m_CaptureType = ctClickHeader;
			}

			if (m_pCapturedColumn)
				SetCapture();

			break;
		case ghtCell:
		case ghtCheckBox:
		case ghtImage:
		case ghtSysImage:
			if (rType == rtBody)
			{
				if (nFlags & MK_CONTROL)
				{
					CCommandQueueLock CQLock(this);
					CDataRowState st(this);
					SetCurrentCell(nRow, nColumn, TRUE);
					SelectRow(st);
				}
				else if (nFlags & MK_SHIFT)
				{
					CCommandQueueLock CQLock(this);
					CDataRowState st(this);
					SetCurrentCell(nRow, nColumn);
					SelectRows(st);
				}
				else
				{
					CCommandQueueLock CQLock(this);
					SetCurrentCell(nRow, nColumn);
					ClearSelection(GetCurrentDataRow());
				}

				if (m_bEnableDrag)
					m_CaptureType = ctDragDelay;
				else
				{
					m_CaptureType = ctScroll;
					SetTimer(TIMER_ID_SCROLL, SCROLL_TIMEOUT, NULL);
				}
				SetCapture();
			}
			else
			{
				if (m_bEnableDrag)
				{
					m_CaptureType = ctDragDelay;
					SetCapture();
				}
			}
			
			if ( ht == ghtCheckBox )
			{
				m_bSelOnDblClk = FALSE;
				OnClickCheckBox(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType);
			}
			else if ( ht == ghtImage )
			{
				m_bSelOnDblClk = TRUE;
				OnClickImage(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), nImageIndex, rType);
			}
			else if ( ht == ghtSysImage )
			{
				m_bSelOnDblClk = FALSE;
				OnClickSysImage(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), nImageIndex, rType);
			}

			break;
		}
	}
    CWnd::OnLButtonDown(nFlags, point);
}

unsigned CCursorGridCtrl::GetTopRowIndex(RegionType rType) const
{
	int nTopRowIndex = 0;
	if (rType == rtBody)
		nTopRowIndex = m_nTopRowIndex;

	return nTopRowIndex;
}

RegionType CCursorGridCtrl::TestRegionType(CPoint point , int& nRowIndex) const
{
	RegionType rType = rtHeader;
	CRect rect = GetGridRect(rType);

	// PtInRect() abandoned intentionally (VSCROLL case)
	if (point.y < rect.top || point.y >= rect.bottom)
	{
		rType = rtHead;
		rect = GetGridRect(rType);
		if (point.y < rect.top || point.y >= rect.bottom)
		{
			rType = rtBody;
			rect = GetGridRect(rType);
			if (point.y < rect.top || point.y >= rect.bottom)
			{
				rType = rtTail;
				rect = GetGridRect(rType);
				if (point.y < rect.top || point.y >= rect.bottom)
					rType = rtNone;
			}
		}
	}
	if (rType != rtNone)
		nRowIndex = GetTopRowIndex(rType) + (point.y - rect.top) / m_nRowHeight;
	else
		nRowIndex = -1;

	return rType;
}

GridHitTest CCursorGridCtrl::HitTestCheckBoxAndImage(CPoint point, int nColumn, int& nRowIndex, RegionType const rType, int& nImageIndex)
{
	GridHitTest ht = ghtSpace;
	nImageIndex = 0;

	CGridRow* pGridRow = GetGridRow(rType, nRowIndex);
	if (!pGridRow)
		nRowIndex = -1;
	else if (nColumn != -1)
	{
		ht = ghtCell;

		CGridItem* pGridItem = pGridRow->m_cells[nColumn];
		WORD wMask = pGridItem->m_wFlagsMask;

		if ( wMask & CGI_PICMASK )
		{
			CGridColumn* pColumn = m_pColumns->GetAt(nColumn);
			CRect clipRect = GetColumnClipRect(pColumn);
			CRect boundsRect = GetColumnBoundsRect(pColumn);
			CPoint ptTL = boundsRect.TopLeft();
			CRect RegionRect = GetGridRect(rType);
			SetVerticalLocationOfRect(RegionRect, pColumn);
			ptTL.y += RegionRect.top + (nRowIndex - GetTopRowIndex(rType)) * m_nRowHeight;
			int nCellHeight = RegionRect.Height();

			if (wMask & CGI_SYSIMAGE)
			{
				if (!pGridItem->m_pSysImageNumbers->empty())
				{
					if (m_pSysImages)
					{
						for(CGridItem::IMAGEREFS::iterator i = pGridItem->m_pSysImageNumbers->begin(); i != pGridItem->m_pSysImageNumbers->end(); ++i)
						{
							nImageIndex = *i;
							if (nImageIndex < m_pSysImages->GetImageCount())
							{
								CRect rect(ptTL, CSize(m_nSysImageWidth, nCellHeight));
								rect.IntersectRect(rect, clipRect);
								if (rect.PtInRect(point))
									return ghtSysImage;
								else
									ptTL.Offset(m_nSysImageWidth, 0);
							}
						}
					}
				}
			}

			if (wMask & CGI_CHECKBOX)
			{
				CRect rect(ptTL, CSize(13, 13));
				rect.OffsetRect(2, 2);
				rect.IntersectRect(rect, clipRect);
				if (rect.PtInRect(point))
					return ghtCheckBox;

				ptTL.Offset(CHECKBOX_PLACE_SIZE, 0); //смещение на ширину знакоместа флажка
			}
			if (wMask & CGI_IMAGE)
			{
				if (pGridItem->m_pImageNumbers->empty())
				{
					nImageIndex = -1;
					CRect rect(ptTL, CSize(pColumn->m_nImageWidth, nCellHeight));
					rect.IntersectRect(rect, clipRect);
					if (rect.PtInRect(point))
						return ghtImage;
					else
						ptTL.Offset(pColumn->m_nImageWidth, 0);
				}
				else
				{
					CImageList* pRowsImages = pColumn->m_pRowsImages;
					if (pRowsImages)
					{
						for(CGridItem::IMAGEREFS::iterator i = pGridItem->m_pImageNumbers->begin(); i != pGridItem->m_pImageNumbers->end(); ++i)
						{
							nImageIndex = *i;
							if (nImageIndex < pRowsImages->GetImageCount())
							{
								CRect rect(ptTL, CSize(pColumn->m_nImageWidth, nCellHeight));
								rect.IntersectRect(rect, clipRect);
								if (rect.PtInRect(point))
									return ghtImage;
								else
									ptTL.Offset(pColumn->m_nImageWidth, 0);
							}
						}
					}
				}
			}
		}
	}
	return ht;
}

GridHitTest CCursorGridCtrl::HitTest(CPoint point, int &nColumn, int &nRowIndex, RegionType& rType, int& nImageIndex)
{
	GridHitTest ht;

	rType = TestRegionType(point, nRowIndex);
	nColumn = HitTestColumn(point, rType, nRowIndex);

	switch(rType)
	{
	case rtHeader:
		ht = ghtHeader;
		break;
	case rtBody:
	case rtHead:
	case rtTail:
		ht = HitTestCheckBoxAndImage(point, nColumn, nRowIndex, rType, nImageIndex);
		break;
	default:
		ht = ghtSpace;
	}

	return ht;
}

void CCursorGridCtrl::ScrollCurrentCell(int xDirection, int yDirection)
{
	if (xDirection)
	{
		int nIndex = GetNextColumn(m_nCurrentColumn, xDirection < 0 ? -1 : 1);
		if (nIndex != -1)
			SetCurrentCell(m_nCurrentDataRowIndex, nIndex);
	}
	
	if (yDirection < 0)
	{
		if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
			Reset(m_pCurrentDataRowOutOfWindow.get());
		
		if (m_nCurrentDataRowIndex != -1)
		{
			if (m_nCurrentDataRowIndex > m_nTopRowIndex && m_nCurrentDataRowIndex < m_nTopRowIndex + m_nVisibleRows)
				SetCurrentCell(m_nCurrentDataRowIndex - 1, m_nCurrentColumn);
			else
			{
				int nScroll = m_nTopRowIndex - m_nCurrentDataRowIndex + 1;
				if (nScroll > 0)
				{
					ScrollUp(nScroll);
					SetCurrentCell(m_nTopRowIndex, m_nCurrentColumn);
// 					RedrawWindow(GetGridRect());
				}
				else
				{
					nScroll += GetPageSize() - 1;
					if (nScroll < 0)
						ScrollDown(-nScroll);
					SetCurrentCell(m_nTopRowIndex + GetPageSize() - 1, m_nCurrentColumn);
// 					RedrawWindow(GetGridRect());
				}
				Invalidate();
			}
		}
	}
	else if (yDirection > 0)
	{
		if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
			Reset(m_pCurrentDataRowOutOfWindow.get());
		
		if (m_nCurrentDataRowIndex != -1)
		{
			if (m_nCurrentDataRowIndex >= m_nTopRowIndex && m_nCurrentDataRowIndex < m_nTopRowIndex + GetPageSize() - 1)
			{
				if (m_nCurrentDataRowIndex < m_rows.GetUpperBound())
					SetCurrentCell(m_nCurrentDataRowIndex + 1, m_nCurrentColumn);
			}
			else
			{
				int nScroll = m_nCurrentDataRowIndex - (m_nTopRowIndex + GetPageSize()) + 2;
				if (nScroll > 0)
				{
					ScrollDown(nScroll);
					SetCurrentCell(m_nTopRowIndex + GetPageSize() - 1, m_nCurrentColumn);
// 					RedrawWindow(GetGridRect());
				}
				else
				{
					nScroll = m_nTopRowIndex - m_nCurrentDataRowIndex - 1;
					if (nScroll > 0)
						ScrollUp(nScroll);
					SetCurrentCell(m_nTopRowIndex, m_nCurrentColumn);
// 					RedrawWindow(GetGridRect());
				}
				Invalidate();
			}
		}
	}
}

void CCursorGridCtrl::SelectRow(CDataRowState& st)
{
	if (m_pProvider && m_SelectMode == smMultiLine)
	{
		CDataRowPtr pPrevRow(st.m_pDataRow);
		if (pPrevRow)
		{
			CDataRowPtr pPrevDataRow;
			if (st.m_nSize == 0 && !st.m_bIsRemoved)
			{
				m_SelectedRows.Add(pPrevRow);
				pPrevDataRow = pPrevRow;
			}

			CDataRowPtr pDataRow = GetCurrentDataRowPtr();
			if (pDataRow != pPrevDataRow)
				m_SelectedRows.Toggle(pDataRow);

			m_SelectedRows.SetCurrentRow(pDataRow);
			m_SelectedRows.SetDirection(0);
		}
	}
	InvalidateRect(GetGridRect());
}

// nDirection is used only when prev row is out of the window
// and it is impossible to determine prev row direction (home/end case)
void CCursorGridCtrl::SelectRows(CDataRowState& st, int nDirection, BOOL bOneRow)
{
	if (m_pProvider && m_SelectMode == smMultiLine)
	{
		CDataRowPtr pPrevDataRow = st.m_pDataRow;
		if (pPrevDataRow)
		{
			int nPrevRowDirection = st.m_nDataRowDirection;
			int nPrevSelectDirection = st.m_nSelectDirection;
			BOOL bIsRemoved = st.m_bIsRemoved;
			BOOL bIsCurrent = st.m_bIsCurrent;

			int nPrevRowIndex = -1; // st.m_nDataRowIndex;
			for (int i = m_nTopRowIndex; i < m_rows.GetSize(); i++)
			{
				if (m_pProvider->CompareRows(m_rows[i]->GetDataRow(), pPrevDataRow.get()))
				{
					nPrevRowIndex = i;
					break;
				}
			}

			CDataRowPtr pCurrentDataRow = GetCurrentDataRowPtr();

			if (m_pProvider->CompareRows(pCurrentDataRow.get(), pPrevDataRow.get()))
			{
				m_SelectedRows.Toggle(pCurrentDataRow);
				nDirection = -nPrevSelectDirection;
			}
			else if (bOneRow)
			{
				BOOL bSkip = bIsRemoved || (bIsCurrent && (nDirection == nPrevSelectDirection || nPrevSelectDirection == 0));
				if (bSkip)
					m_SelectedRows.Toggle(pCurrentDataRow);
				else
				{
					if (m_SelectedRows.Toggle(pPrevDataRow))
						m_SelectedRows.Toggle(pCurrentDataRow);
					else
						nDirection = -nDirection;
				}
			}
			else if (nPrevRowIndex != -1)
			{
				int n1, n2;
				if (nPrevRowIndex < m_nCurrentDataRowIndex)
				{
					nDirection = 1;
					int nSkip = bIsRemoved || (bIsCurrent && (nDirection == nPrevSelectDirection || nPrevSelectDirection == 0)) ? 1 : 0;
					n1 = nPrevRowIndex + nSkip;
					n2 = m_nCurrentDataRowIndex;
				}
				else
				{
					nDirection = -1;
					int nSkip = bIsRemoved || (bIsCurrent && (nDirection == nPrevSelectDirection || nPrevSelectDirection == 0)) ? 1 : 0;
					n1 = m_nCurrentDataRowIndex;
					n2 = nPrevRowIndex - nSkip;
				}
				for (int i = n1; i <= n2; i++)
					m_SelectedRows.Toggle(m_rows[i]->GetDataRowPtr());
			}
			else
			{
				if (!nDirection)
					nDirection = -nPrevRowDirection;
				int nPage = DEFAULT_SELECTION_PAGE;
				int nCount = DEFAULT_SELECTION_PAGE * nDirection;
				BOOL bSkip = bIsRemoved || (bIsCurrent && (nDirection == nPrevSelectDirection || nPrevSelectDirection == 0));

				if (!bSkip)
					m_SelectedRows.Toggle(pPrevDataRow);

				m_pProvider->QueryRows(pPrevDataRow.get(), nCount, nPage);
				int nExpected = nPage;
				CDataRowPtr pDataRow;
				while (pDataRow.reset(m_pProvider->Fetch()), pDataRow)
				{
					nExpected--;

					m_SelectedRows.Toggle(pDataRow);
					if (m_pProvider->CompareRows(pDataRow.get(), pCurrentDataRow.get()))
						break;

					if (!nExpected)
					{
						nPage *= 2;
						nCount *= 2;
						nExpected = nPage;
						m_pProvider->QueryRows(pDataRow.get(), nCount, nPage);
					}
				}
				while (nExpected && (pDataRow.reset(m_pProvider->Fetch()), pDataRow))
					nExpected--;
			}
			m_SelectedRows.SetRemovedRow(CDataRowPtr());
			m_SelectedRows.SetCurrentRow(pCurrentDataRow);
			m_SelectedRows.SetDirection(nDirection);
		}
	}
    InvalidateRect(GetGridRect());
}

void CCursorGridCtrl::ClearSelection(CDataRow* pCurrentRow)
{
	if (!pCurrentRow || !m_SelectedRows.IsSelected(pCurrentRow))
	{
		m_SelectedRows.RemoveAll();
		InvalidateRect(GetGridRect());
	}
}

unsigned CCursorGridCtrl::GetVisibleRows(RegionType rType) const
{
	unsigned nHeadRows = min(m_rows_head.GetSize(), m_nAllVisibleRows);
	unsigned nTailRows = min(m_rows_tail.GetSize(), m_nAllVisibleRows - nHeadRows);
	unsigned nRows;
	switch(rType)
	{
	case rtNone:
		nRows = m_nAllVisibleRows;
		break;
	case rtHead:
		nRows = nHeadRows;
		break;
	case rtTail:
		nRows = nTailRows;
		break;
	case rtBody:
		nRows = m_nAllVisibleRows - nHeadRows - nTailRows;
		break;
	}
	return nRows;
}

unsigned CCursorGridCtrl::GetFullVisibleRows(RegionType rType) const
{
	unsigned nRows;
	switch(rType)
	{
	case rtNone:
		nRows = GetVisibleRows(rtHead) + GetVisibleRows(rtTail) + GetFullVisibleRows(rtBody);
		break;
	case rtHead:
		nRows = GetVisibleRows(rtHead);
		break;
	case rtTail:
		nRows = GetVisibleRows(rtTail);
		break;
	case rtBody:
		nRows = min(GetVisibleRows(rtBody), m_rows.GetSize() - m_nTopRowIndex);
		break;
	}
	return nRows;
}

CGridRow* CCursorGridCtrl::GetGridRow(RegionType rType, int nRowIndex) const
{
	CGridRow* pGridRow = NULL;
	if (nRowIndex >= 0)
	{
		switch(rType)
		{
		case rtHead:
			if (nRowIndex < m_rows_head.GetSize())
				pGridRow = m_rows_head[nRowIndex];
			break;
		case rtTail:
			if (nRowIndex < m_rows_tail.GetSize())
				pGridRow = m_rows_tail[nRowIndex];
			break;
		case rtBody:
			if (nRowIndex < m_rows.GetSize())
				pGridRow = m_rows[nRowIndex];
			break;
		}
	}
	return pGridRow;
}

void CCursorGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CaptureType prevCaptureType = m_CaptureType;
	if (m_CaptureType != ctNone)
	{
		ReleaseCapture();
		m_CaptureType = ctNone;
	}

	switch (prevCaptureType)
    {
    case ctResizeColumn:
		EndColResize();
		OnColumnWidthChange();
        break;
    case ctClickHeader:
        {
            int nColumnIndex = HitTestColumn(point, rtHeader);
            if (nColumnIndex != -1 && m_pColumns->GetAt(nColumnIndex) == m_pCapturedColumn)
                OnHeaderSelection(m_pCapturedColumn);
        }
        break;
	case ctScroll:
        KillTimer(TIMER_ID_SCROLL);
		break;
    }
	CWnd::OnLButtonUp(nFlags, point);
}

void CCursorGridCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (m_CaptureType != ctNone)
    {
        ReleaseCapture();
        m_CaptureType = ctNone;
    }
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CCursorGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
    switch (m_CaptureType)
    {
    case ctResizeColumn:
		{
			int nNewWidth = point.x - m_nCapturedColumn_left;
			StepColResize(nNewWidth);
		}
        break;
    case ctClickHeader:
        {
            int dx = point.x - m_hitMouseButton.x;
            if (dx < 0)
                dx = -dx;
            int dy = point.y - m_hitMouseButton.y;
            if (dy < 0)
                dy = -dy;
            if (dx > RESIZE_AREA_WIDTH || dy > RESIZE_AREA_WIDTH)
            {
                if (m_pCapturedColumn && (m_bChangePositionOfColumns || m_pCapturedColumn->m_bChangeVisible))
                {
                    m_Tip.Hide();
                    CRect rect = GetColumnBoundsRect(m_pCapturedColumn);
					SetVerticalLocationOfRect(rect, m_pCapturedColumn);
					CSize size(m_hitMouseButton.x - rect.left, m_hitMouseButton.y - rect.top);
                    m_ReposColumnWnd.Show(m_pCapturedColumn, size);
                    m_CaptureType = ctMoveColumn;
                }
                else
                    m_CaptureType = ctNone;
            }
        }
        break;
    case ctMoveColumn:
        break;
    case ctDragDelay:
        {
            CRect rect(m_hitMouseButton, m_hitMouseButton);
            rect.InflateRect(RESIZE_AREA_WIDTH, RESIZE_AREA_WIDTH);
            if (!rect.PtInRect(point))
            {
                m_CaptureType = ctNone;
                ReleaseCapture();
				InvalidateRect(GetGridRect());

				int nColumn, nRow, nImageIndex;
				GridHitTest ht = HitTest(m_hitMouseButton, nColumn, nRow, m_DragRegionType, nImageIndex);
				if (ht == ghtCell || ht == ghtImage)
				{
					m_pDragRow = GetGridRow(m_DragRegionType, nRow)->GetDataRowPtr();
					m_pDragColumn = m_pColumns->GetAt(nColumn);
					OnBeginDrag(m_pDragRow.get(), m_pDragColumn, m_DragRegionType);
				}
            }
        }
        break;
	case ctScroll:
		{
			CRect rect = GetGridRect(rtBody);
			if (rect.PtInRect(point))
			{
				int nColumn, nRow, nImageIndex;
				RegionType rType;
				GridHitTest ht = HitTest(point, nColumn, nRow, rType, nImageIndex);
				if (ht == ghtCell || ht == ghtImage)
					SetCurrentCell(nRow, nColumn);
			}
			else
			{
				int xDirection = point.x < rect.TopLeft().x ? -1 : point.x > rect.BottomRight().x ? 1 : 0;
				int yDirection = point.y < rect.TopLeft().y ? -1 : point.y > rect.BottomRight().y ? 1 : 0;
				ScrollCurrentCell(xDirection, yDirection);
			}
		}
		break;
    default:
        if (!m_ColumnHelper.IsDisplay())
            ShowHint(point);
        break;
    }
	CWnd::OnMouseMove(nFlags, point);
}

void CCursorGridCtrl::ShowHint(CPoint point)
{
    CRect HeaderRect = GetHeaderRect();

	int nRow;
	RegionType rType = TestRegionType(point, nRow);
    int nColIndex = HitTestColumn(point, rType, nRow);

    if (nColIndex != -1)
    {
        CString strTipText;
        CRect TipRect;
        DWORD dwTipedObject = 0;

        CGridColumn* pGridColumn = m_pColumns->GetAt(nColIndex);
        BOOL bByCursor = FALSE;
		BOOL bWordBreak = FALSE;
		BOOL bForceHint = FALSE;
        CFont* pFont = &m_font;
		if (rType == rtHeader)
        {
	        TipRect.IntersectRect(GetColumnBoundsRect(pGridColumn), HeaderRect);
			SetVerticalLocationOfRect(TipRect, pGridColumn);

            TipRect.left += RESIZE_AREA_WIDTH;
            if (pGridColumn->m_OrderMarker != omNone)
                TipRect.right -= 16;
            TipRect.right -= RESIZE_AREA_WIDTH;
            strTipText = pGridColumn->GetCaption();
            dwTipedObject = (DWORD)pGridColumn;
            bByCursor = TRUE;
            if (pGridColumn->m_pHeaderFont)
                pFont = pGridColumn->m_pHeaderFont;
			bWordBreak = pGridColumn->m_bWordBreak;
        }
        else
        {
			HeaderRect.bottom = HeaderRect.top + GetHeaderHeight();
			TipRect.IntersectRect(GetColumnBoundsRect(pGridColumn), HeaderRect);
			int TipRect_top = TipRect.top;
			SetVerticalLocationOfRect(TipRect, pGridColumn);

			CGridRow* pGridRow = GetGridRow(rType, nRow);
			nRow -= GetTopRowIndex(rType);
			if (pGridRow)
			{
				CGridItem* pGridItem = pGridRow->m_cells[nColIndex];

				if (pGridItem->m_wFlagsMask & CGI_HINT)
				{
					strTipText = pGridItem->m_strHint;
					bForceHint = TRUE;
				}
				else
				{
					strTipText = pGridItem->m_strText;
				}

				TipRect.OffsetRect(0, GetGridRect(rType).top + nRow * m_nRowHeight - TipRect_top);
				TipRect.left += pGridItem->m_nNoTextOffset;

				dwTipedObject = (DWORD)pGridItem;
				bByCursor = FALSE;
				if (pGridItem->m_pFont)
					pFont = pGridItem->m_pFont;
				else if (pGridRow->m_pFont)
					pFont = pGridRow->m_pFont;
				else if (pGridItem->m_pGridColumn->m_pFont)
					pFont = pGridItem->m_pGridColumn->m_pFont;
				bWordBreak = pGridItem->m_wFlagsMask & CGI_TEXT_WORDBREAK;
			}
		}

        if (!TipRect.PtInRect(point))
            return;

        if (dwTipedObject)
		{
			m_Tip.m_pt = point;
    	    m_Tip.Show(strTipText, TipRect, dwTipedObject, pFont, bWordBreak, bByCursor, bForceHint);
		}
    }
}

int CCursorGridCtrl::GetColumnAutoWidth(CGridColumn* pGridColumn)
{
	int nWidth = 0;
	int nColIndex = m_pColumns->IndexOf(pGridColumn);

	CTypedPtrArray<CPtrArray, CGridRow*> rows;
	for (int i = 0; i < GetFullVisibleRows(rtHead); i++)
		rows.Add(m_rows_head[i]);

	for (int i = 0; i < GetFullVisibleRows(rtTail); i++)
		rows.Add(m_rows_tail[i]);

	for (int i = 0; i < GetFullVisibleRows(rtBody); i++)
		rows.Add(m_rows[m_nTopRowIndex + i]);

	for (int i = 0; i < rows.GetSize(); i++)
	{
		CClientDC dc(this);
		CGridRow* pGridRow = rows[i];
		CGridItem* pGridItem = pGridRow->m_cells[nColIndex];
		int nItemWidth = 0;
		
		nItemWidth += pGridItem->m_nNoTextOffset;
		
		CFont* pFont = &m_font;
		if (pGridItem->m_pFont)
			pFont = pGridItem->m_pFont;
		else if (pGridRow->m_pFont)
			pFont = pGridRow->m_pFont;
		else if (pGridItem->m_pGridColumn->m_pFont)
			pFont = pGridItem->m_pGridColumn->m_pFont;

		CString csText = pGridItem->m_strText;
		csText.TrimRight();
		if (!csText.IsEmpty())
		{
			CFont *pOldFont;
			pOldFont = dc.SelectObject(pFont);

			CRect OutRect;
			UINT nFlags = DT_CALCRECT | DT_NOPREFIX | DT_NOCLIP;
			dc.DrawText(csText, OutRect, nFlags);
			CSize size = OutRect.Size();
			//CSize size = dc.GetTextExtent(csText);
			dc.SelectObject(pOldFont);

			nItemWidth += size.cx;
			nWidth = max(nWidth, nItemWidth);
		}
	}
	rows.RemoveAll();

	nWidth += GetLeftTextMargin() + GetRightTextMargin() + 1;
	if (nWidth >= AUTOWIDTH_TEXT_MARGIN_THRESHOLD)
		nWidth += AUTOWIDTH_TEXT_MARGIN;

	nWidth = max(nWidth, RESIZE_MIN_WIDTH);
	return nWidth;
}

int CCursorGridCtrl::FamilyWidth(int nFamily, int &nCaptWidth)
{
	int nWidth = 0;
	nCaptWidth = 0;
	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn->m_nFamily == nFamily && pColumn->m_nLineNumber == 1)
			{
				nWidth += pColumn->m_rect.Width();
				nCaptWidth += pColumn->m_nWidthBeforeResize;
			}
			else if (pColumn->m_nFamily > nFamily)
				break;
		}
	}
	return nWidth;
}
int CCursorGridCtrl::FamilyWidth(int nFamily)
{
	int nCaptWidth;
	return FamilyWidth(nFamily, nCaptWidth);
}

int CCursorGridCtrl::FamilyResize(int nStartIndex, int nFamily, int nOldWidth, int nNewWidth, int nOffset/* = 0*/)
{
	int nFamilyRight = 0;
	int nLastRight = 0;
	CGridColumn* pLastColumn = NULL;

	for (int i = nStartIndex; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn->m_nFamily > nFamily)
				break;
			else if (pColumn->m_nFamily == nFamily)
			{
				switch(pColumn->m_Location)
				{
				case clNewColumn:
					pColumn->m_rect.left += nOffset;
					nFamilyRight = pColumn->m_rect.left + nNewWidth;
					break;
				case clNextLine:
					pColumn->m_rect.left += nOffset;
					if (pLastColumn)
						pLastColumn->m_rect.right = nFamilyRight;
					break;
				case clSomeColumn:
				default:
					pColumn->m_rect.left = nLastRight;
					break;
				}

				if (pColumn->m_bFixedWidth)
					pColumn->m_rect.right = pColumn->m_rect.left + pColumn->m_nWidthBeforeResize;
				else
				{
					int nAddWidth = MulDiv(pColumn->m_nFamilyFixedWidth, pColumn->m_nMinWidth, pColumn->m_nMinWidthDiv-pColumn->m_nFamilyFixedWidth);
					nAddWidth = MulDiv(nAddWidth, nNewWidth, nOldWidth) - nAddWidth;

					pColumn->m_rect.right = pColumn->m_rect.left + MulDiv(pColumn->m_nWidthBeforeResize, nNewWidth, nOldWidth) + nAddWidth;
				}
				
				nLastRight = pColumn->m_rect.right;
				pLastColumn = pColumn;
			}
		}
	}
	if (pLastColumn)
		pLastColumn->m_rect.right = nFamilyRight;

	return nFamilyRight;
}

void CCursorGridCtrl::StepColResize(int nNewWidth)
{
	if (nNewWidth < RESIZE_MIN_WIDTH)
		nNewWidth = RESIZE_MIN_WIDTH;
	if (nNewWidth > RESIZE_MAX_WIDTH)
		nNewWidth = RESIZE_MAX_WIDTH;

	if (m_CapturedLocation == clSomeColumn)
	{
		if (nNewWidth == m_pCapturedColumn->m_rect.Width())
			return;

		int dx = nNewWidth - m_pCapturedColumn->m_rect.Width();
		for (int i = m_pColumns->IndexOf(m_pCapturedColumn) + 1; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (pGridColumn->m_bFixedWidth)
					break;

				int nWidth = pGridColumn->m_rect.Width();
				nWidth -= dx;
				if (nWidth < RESIZE_MIN_WIDTH)
					nWidth = RESIZE_MIN_WIDTH;
				pGridColumn->m_rect.left = pGridColumn->m_rect.right - nWidth;
				m_pCapturedColumn->m_rect.right = pGridColumn->m_rect.left;
				break;
			}
		}
	}
	else if (IS_SHIFT_PRESSED())
	{	
		int nCapturedColumnFamilyWidth = FamilyWidth(m_pCapturedColumn->m_nFamily);
		if (nNewWidth == nCapturedColumnFamilyWidth)
			return;

		int dx = nNewWidth - nCapturedColumnFamilyWidth;
		if (dx < 0)
		{
			int xav = GetGridRect().Width() - m_pColumns->GetVisibleWidth();
			dx = max(dx, xav);
		}
		else
			dx = min(dx, GetPageRect().Width() - RESIZE_MIN_WIDTH);
		if (dx == 0)
			return;
		
		int nWidth = nCapturedColumnFamilyWidth + dx;
		if (nWidth < m_pCapturedColumn->m_nFamilyResizeMinWidth)
		{
			dx += m_pCapturedColumn->m_nFamilyResizeMinWidth - nWidth;
			nWidth = m_pCapturedColumn->m_nFamilyResizeMinWidth;
		}

		for (int i = m_pColumns->GetUpperBound(); i >= 0; i--)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->m_bVisible)
			{
				if (pColumn->m_nFamily > m_pCapturedColumn->m_nFamily)
					pColumn->m_rect.OffsetRect(dx, 0);
				else if (pColumn->m_nFamily == m_pCapturedColumn->m_nFamily)
				{
					FamilyResize(m_pColumns->IndexOf(m_pCapturedColumn),
						m_pCapturedColumn->m_nFamily,
						m_nCapturedColumn_width,
						nWidth);
				}
			}
		}

		m_pColumns->m_nMinWidth = 0;
		for (int i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				pGridColumn->m_nMinWidth = pGridColumn->m_rect.Width();
				if (pGridColumn->m_nLineNumber == 1)
					m_pColumns->m_nMinWidth += pGridColumn->m_nMinWidth;
			}
		}
		RecalcColumnsWidthMulDiv();
		SetHScrollInfo();
	}
	else if (IS_CTRL_PRESSED())
	{
		if (nNewWidth < m_pCapturedColumn->m_nFamilyResizeMinWidth)
			nNewWidth = m_pCapturedColumn->m_nFamilyResizeMinWidth;

		int nCapturedColumnIndex = m_pColumns->IndexOf(m_pCapturedColumn);
		for (int i = nCapturedColumnIndex + 1; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (pGridColumn->m_nFamily > m_pCapturedColumn->m_nFamily)
				{
					if (!pGridColumn->m_nFamilyResizableWidth)
						break;
						
					int nCapturedColumnFamilyWidth = FamilyWidth(m_pCapturedColumn->m_nFamily);
					int nDx = nNewWidth - nCapturedColumnFamilyWidth;

					int nCaptWidth;
					int nFamilyWidth = FamilyWidth(pGridColumn->m_nFamily, nCaptWidth);
					int nWidth = nFamilyWidth - nDx;
					
					if (nWidth < pGridColumn->m_nFamilyResizeMinWidth)
					{
						nDx -= pGridColumn->m_nFamilyResizeMinWidth - nWidth;
						nNewWidth -= pGridColumn->m_nFamilyResizeMinWidth - nWidth;
						nWidth = pGridColumn->m_nFamilyResizeMinWidth;
					}

					FamilyResize(i, pGridColumn->m_nFamily, nCaptWidth, nWidth, nDx);

					FamilyResize(nCapturedColumnIndex,
						m_pCapturedColumn->m_nFamily,
						m_nCapturedColumn_width, nNewWidth);
					break;
				}
			}            
		}        
	}
	else
	{
		if (nNewWidth < m_pCapturedColumn->m_nFamilyResizeMinWidth)
			nNewWidth = m_pCapturedColumn->m_nFamilyResizeMinWidth;

		CRect page = GetPageRect(); //клиентская часть минус фиксированные колонки слева/справа
		CRect clip = GetColumnClipRect(m_pCapturedColumn);
		int dx = nNewWidth - m_pCapturedColumn->m_rect.Width();
		if (dx < 0 && clip.right == page.right)
		{
			if (page.Width() <= RESIZE_MIN_WIDTH)
				return;
		}
		else if (dx > 0 && clip.right == page.left)
		{
			if (page.Width() <= RESIZE_MIN_WIDTH)
				return;
		}

		int i;
		int nResizeWidth = 0;
		int CurX;
		int nLastFamily = -1;
		//перебор колонок справа
		for (i = m_pColumns->GetUpperBound(); i >= 0; i--)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (pGridColumn->m_nFamily == m_pCapturedColumn->m_nFamily)
					break;
				if (pGridColumn->m_nFamily != nLastFamily)
				{
					if (!nResizeWidth)
						nResizeWidth = CurX = pGridColumn->m_rect.right; //общая щирина ресайзинга = правая граница самой правой колонки
					
					nLastFamily = pGridColumn->m_nFamily;
				}
			}
		}

		if (!nResizeWidth)
			return;

		int nNewResizeWidth = nResizeWidth - (m_pCapturedColumn->m_rect.left + nNewWidth); //новая ширина справа до фикс.кол или до конца
		int nRight = m_pCapturedColumn->m_rect.left + FamilyWidth(m_pCapturedColumn->m_nFamily);
		nResizeWidth -= nRight; //текущая ширина справа до фикс.кол или до конца

		nLastFamily = -1;
		int nOffset = 0;
		//второй перебор колонок справа
		for (i = m_pColumns->GetUpperBound(); i >= 0; i--)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (pGridColumn->m_nFamily == m_pCapturedColumn->m_nFamily)
				{
					FamilyResize(m_pColumns->IndexOf(m_pCapturedColumn),
						m_pCapturedColumn->m_nFamily,
						m_nCapturedColumn_width, CurX - m_pCapturedColumn->m_rect.left);
					break;
				}
				if (pGridColumn->m_nFamily != nLastFamily)
				{
					int nCaptWidth;
					int nFamilyWidth = FamilyWidth(pGridColumn->m_nFamily, nCaptWidth);
					
					int nDx = MulDiv(nFamilyWidth, nNewResizeWidth, nResizeWidth) - nFamilyWidth;
					int nWidth = nFamilyWidth + MulDiv(nDx, pGridColumn->m_nFamilyResizableWidth, pGridColumn->m_nFamilyWidth);

					if (nWidth < pGridColumn->m_nFamilyResizeMinWidth)
						nWidth = pGridColumn->m_nFamilyResizeMinWidth;

					nOffset += nFamilyWidth-nWidth;
					FamilyResize(0, pGridColumn->m_nFamily, nCaptWidth, nWidth, nOffset);
					CurX -= nWidth;//уменьшаем остаток от общей ширины на новую ширину колонки

					nLastFamily = pGridColumn->m_nFamily;
				}
			}
		}
	}
	Invalidate();
}

void CCursorGridCtrl::EndColResize()
{
	if (m_CapturedLocation != clSomeColumn)
	{
		int i, nTotalWidth = 0;
		int nCurFamily = -1;
		int nTotalMinWidth = m_pColumns->m_nMinWidth;

		for (i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (nCurFamily != pGridColumn->m_nFamily)
				{
					nCurFamily = pGridColumn->m_nFamily;
					if (pGridColumn->m_nFamilyResizableWidth)
					{
						nTotalWidth += FamilyWidth(nCurFamily);
					}
					else
						nTotalMinWidth -= pGridColumn->m_nFamilyWidth;
				}
			}
		}

		for (i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (!pGridColumn->m_bFixedWidth && pGridColumn->m_nFamilyResizableWidth)
				{
					if (!pGridColumn->m_nFamilyFixedWidth)
					{
						pGridColumn->m_nMinWidth = MulDiv(pGridColumn->m_rect.Width(), nTotalMinWidth, nTotalWidth);
					}
					else
					{
						int nSubWidth = MulDiv(pGridColumn->m_nFamilyFixedWidth, nTotalWidth, nTotalMinWidth) - pGridColumn->m_nFamilyFixedWidth;
						pGridColumn->m_nMinWidth = MulDiv(pGridColumn->m_rect.Width() - nSubWidth, nTotalMinWidth, nTotalWidth);
						int nResizeMinWidth = MulDiv(RESIZE_MIN_WIDTH, nTotalWidth, nTotalMinWidth);
						if (pGridColumn->m_nMinWidth < nResizeMinWidth)
							pGridColumn->m_nMinWidth = nResizeMinWidth;
					}
				}
			}
		}
		RecalcColumnsWidthMulDiv();

		nCurFamily = -1;
		m_pColumns->m_nResizableMinWidth = 0;
		m_pColumns->m_nMinWidth = 0;
		for (i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(i);
			if (pGridColumn->m_bVisible)
			{
				if (nCurFamily != pGridColumn->m_nFamily)
				{
					nCurFamily = pGridColumn->m_nFamily;
					m_pColumns->m_nResizableMinWidth += pGridColumn->m_nFamilyResizableWidth;
					m_pColumns->m_nMinWidth += pGridColumn->m_nFamilyWidth;
				}
			}
		}
		SetHScrollInfo();
	}
	else
	{
		int i = m_pColumns->IndexOf(m_pCapturedColumn);
		int j = GetNextColumn(i,1);
		if (j != -1)
		{
			CGridColumn* pNextGridColumn = m_pColumns->GetAt(j);
			int nTotalWidth = m_pCapturedColumn->m_rect.Width() + pNextGridColumn->m_rect.Width();
			int nTotalMinWidth = m_pCapturedColumn->m_nMinWidth + pNextGridColumn->m_nMinWidth;
			m_pCapturedColumn->m_nMinWidth = MulDiv(m_pCapturedColumn->m_rect.Width(), nTotalMinWidth, nTotalWidth);
			pNextGridColumn->m_nMinWidth = nTotalMinWidth - m_pCapturedColumn->m_nMinWidth;
			RecalcColumnsWidthMulDiv();
		}
	}
	Invalidate();
}

void CCursorGridCtrl::CalcRowsParams()
{
    if (m_nRowHeight == 0)
        return;

	int nRowsHeight = GetGridRect().Height();
	m_nAllVisibleRows = nRowsHeight / m_nRowHeight;
	m_bLastRowNotFull = nRowsHeight % m_nRowHeight > 0;

	int nCorr = m_rows_tail.GetSize() == 0 ? 1 : 0;
	if (m_bLastRowNotFull)
	{
		if (m_rows_tail.GetSize() == 0 || m_rows_head.GetSize() > m_nAllVisibleRows)
			m_nAllVisibleRows++;
		else
			m_bLastRowNotFull = FALSE;
	}

	int nVisChange = m_nVisibleRows;
	m_nVisibleRows = GetVisibleRows(rtBody);
	if (m_nVisibleRows == 0)
		m_nVisibleRows = 1;

    if (!m_pProvider)
        return;

    nVisChange -= m_nVisibleRows;
    if (nVisChange < 0)
    {
        int nGetRows = m_nTopRowIndex + m_nVisibleRows - m_rows.GetSize();
        if (nGetRows > 0 && !m_bIsEOF)
            GetRowsDown(nGetRows);

        FillRows(m_nTopRowIndex + m_nVisibleRows + nVisChange, -nVisChange);
        SetVScrollInfo();

        if (m_bIsEOF && nGetRows > nCorr)
            ScrollUp(nGetRows - nCorr);
    }
    else if (nVisChange > 0)
    {
        int nScroll = m_nCurrentDataRowIndex - m_nTopRowIndex - m_nVisibleRows + 1 + nCorr;
        nScroll = min(nScroll, m_nCurrentDataRowIndex - m_nTopRowIndex);
        if (nScroll > 0)
        {
            if (m_bIsEOF && m_nCurrentDataRowIndex == m_rows.GetUpperBound())
            {
				m_nTopRowIndex += nScroll;
				InvalidateRect(GetGridRect());
				SetVScrollInfo();
            }
            else
                ScrollDown(nScroll);
        }
    }
    else
        SetVScrollInfo();
}

void CCursorGridCtrl::PaintGridLines(CDC *pDC, RegionType rType)
{
	int nFullVisibleRows = GetFullVisibleRows(rType);
	if (!nFullVisibleRows)
		return;

	CRect& rect = GetGridRect(rType);
	if (rect.IsRectEmpty())
		return;

	int nBegRowIndex, nEndRowIndex;
	int nCurRowIndex = -1;

	COLORREF crInactiveLines;
	BOOL bEachRow;
	if (rType == rtBody)
	{
		crInactiveLines = GetInactiveLinesColor();
		bEachRow = TRUE;
		nCurRowIndex = m_nCurrentDataRowIndex - m_nTopRowIndex;
	}
	else
	{
		crInactiveLines = GetLinesColor();
		bEachRow = TRUE;
	}
	

	int nBegHOffset = rect.top;
	if (m_BorderStyle != bs3D)
		nBegHOffset--;

	if (bEachRow)
	{
		nBegRowIndex = 0;
		nEndRowIndex = nFullVisibleRows;
	}
	else
	{
		nBegRowIndex = nCurRowIndex;
		nEndRowIndex = nCurRowIndex;
		if (nFullVisibleRows > nBegRowIndex)
			nEndRowIndex++;
		nBegHOffset += nBegRowIndex * m_nRowHeight;
	}

	
	int nVLineBottom;
	if (m_bHorizontalLines)
		//если горизонтальные линии есть, то вертикальные линии на высоту видимых строк
		nVLineBottom = rect.top + nFullVisibleRows * m_nRowHeight;
	else
		//если горизонтальных линий нет, то вертикальные линии на всю высоту
		nVLineBottom = rect.bottom;


	int nMaxStrokeCount = 0; //Максимально возможное количество штрихов
	int nMaxStrokeCountInAct = 0; //Максимально возможное количество штрихов для неактивных строк
	if (m_bVerticalLines)
	{
		nMaxStrokeCount += m_pColumns->GetSize(); //Тип линий A
		nMaxStrokeCount      += m_pColumns->GetSize() * (nEndRowIndex-nBegRowIndex); //Тип линий B
		nMaxStrokeCountInAct += m_pColumns->GetSize() * (nEndRowIndex-nBegRowIndex); //Тип линий B
	}
	if (m_bHorizontalLines)
	{
		nMaxStrokeCount      += m_pColumns->GetSize() * (nEndRowIndex-nBegRowIndex); //Тип линий С
		nMaxStrokeCountInAct += m_pColumns->GetSize() * (nEndRowIndex-nBegRowIndex); //Тип линий С
		nMaxStrokeCount += nFullVisibleRows + 1;//Тип линий D
	}

	LPPOINT lpPoints = new POINT[nMaxStrokeCount * 2]; //по две точки на штрих
	LPDWORD lpPolyPoints = new DWORD[nMaxStrokeCount];
	int nPointIndex = 0;
	int nStrokeCount = 0; //Точное количество штрихов

	LPPOINT lpPointsInAct = new POINT[nMaxStrokeCountInAct * 2]; //по две точки на штрих
	LPDWORD lpPolyPointsInAct = new DWORD[nMaxStrokeCountInAct];
	int nPointIndexInAct = 0;
	int nStrokeCountInAct = 0; //Точное количество штрихов

	CGridColumn* pLastColumn = NULL;
	CRect LastRect;

	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			//CRect Rect = GetColumnBoundsRect(pColumn);
			CRect Rect = GetColumnClipRect(pColumn);
			SetVerticalLocationOfRect(Rect, pColumn);

			switch(pColumn->m_Location)
			{
			case clNewColumn:
				if (pLastColumn && m_bVerticalLines)
				{
					//Тип линий A
					//РАЗЛИНОВКА ГРИДА
					//Вертикальная линия на заданную высоту справа предыдущей колонки
					int nRight = LastRect.right - 1;
					if(nRight > rect.left && nRight < rect.right)
					{
						lpPoints[nPointIndex].x   = nRight;
						lpPoints[nPointIndex++].y = rect.top;
						lpPoints[nPointIndex].x   = nRight;
						lpPoints[nPointIndex++].y = nVLineBottom;
						lpPolyPoints[nStrokeCount++] = 2;
					}
				}
				break;
			case clNextLine:
				break;
			case clSomeColumn:
			default:
				if (m_bVerticalLines)
				{
					int nLeft = Rect.left - 1;
					if(nLeft > rect.left && nLeft < rect.right)
					{
						int nHOffset = nBegHOffset;
						for (int i = nBegRowIndex; i < nEndRowIndex; i++)
						{
							//Тип линий B
							//РАЗЛИНОВКА СТРОКИ
							//Вертикальная линия слева ячейки
							if (i == nCurRowIndex)
							{
								lpPoints[nPointIndex].x   = nLeft;
								lpPoints[nPointIndex++].y = Rect.top + nHOffset;
								lpPoints[nPointIndex].x   = nLeft;
								lpPoints[nPointIndex++].y = Rect.bottom + nHOffset - 1;
								lpPolyPoints[nStrokeCount++] = 2;
							}
							else
							{
								lpPointsInAct[nPointIndexInAct].x   = nLeft;
								lpPointsInAct[nPointIndexInAct++].y = Rect.top + nHOffset;
								lpPointsInAct[nPointIndexInAct].x   = nLeft;
								lpPointsInAct[nPointIndexInAct++].y = Rect.bottom + nHOffset - 1;
								lpPolyPointsInAct[nStrokeCountInAct++] = 2;
							}
							nHOffset += m_nRowHeight;
						}
					}
				}
			}

			if (m_bHorizontalLines)
			{
				if(Rect.left < rect.right && Rect.right > rect.left)
				{
					if (pColumn->m_nLineNumber + pColumn->m_nColumnLineCount <= m_nColumnsLineCount)
					{//нижняя граница ячейки НЕ совпадает с нижней границей строки
						int nHOffset = nBegHOffset - 1;
						for (int i = nBegRowIndex; i < nEndRowIndex; i++)
						{
							//Тип линий C
							//РАЗЛИНОВКА СТРОКИ
							//Горизонтальная линия снизу ячейки
							if (i == nCurRowIndex)
							{
								lpPoints[nPointIndex].x   = Rect.left - 1;
								lpPoints[nPointIndex++].y = Rect.bottom + nHOffset;
								lpPoints[nPointIndex].x   = Rect.right - 1;
								lpPoints[nPointIndex++].y = Rect.bottom + nHOffset;
								lpPolyPoints[nStrokeCount++] = 2;
							}
							else
							{
								lpPointsInAct[nPointIndexInAct].x   = Rect.left - 1;
								lpPointsInAct[nPointIndexInAct++].y = Rect.bottom + nHOffset;
								lpPointsInAct[nPointIndexInAct].x   = Rect.right - 1;
								lpPointsInAct[nPointIndexInAct++].y = Rect.bottom + nHOffset;
								lpPolyPointsInAct[nStrokeCountInAct++] = 2;
							}
							nHOffset += m_nRowHeight;
						}
					}
				}
			}

			pLastColumn = pColumn;
			LastRect = Rect;
		}
	}
	if (pLastColumn && m_bVerticalLines)
	{
		//Тип линий A
		//РАЗЛИНОВКА ГРИДА
		//Вертикальная линия на заданную высоту справа предыдущей колонки
		int nRight = LastRect.right - 1;
		if(nRight > rect.left && nRight < rect.right)
		{
			lpPoints[nPointIndex].x   = nRight;
			lpPoints[nPointIndex++].y = rect.top;
			lpPoints[nPointIndex].x   = nRight;
			lpPoints[nPointIndex++].y = nVLineBottom;
			lpPolyPoints[nStrokeCount++] = 2;
		}
	}

	if (m_bHorizontalLines)
	{
		int nLeft = rect.left;
		int nRight = rect.right;
		int y = rect.top - 1;
		for (int i = 0; i < nFullVisibleRows+1; i++)
		{
			//Тип линий D
			//РАЗЛИНОВКА ГРИДА
			//Горизонтальная линия под строкой
			lpPoints[nPointIndex].x   = nLeft;
			lpPoints[nPointIndex++].y = y;
			lpPoints[nPointIndex].x   = nRight;
			lpPoints[nPointIndex++].y = y;
			lpPolyPoints[nStrokeCount++] = 2;
			y += m_nRowHeight;
		}
	}

	CPen* pOldPen;
	CPen  penLines, penInAct;
	penLines.CreatePen(PS_SOLID, 1, GetLinesColor());
	penInAct.CreatePen(PS_SOLID, 1, crInactiveLines);

	pOldPen = pDC->SelectObject(&penInAct);
	pDC->PolyPolyline(lpPointsInAct, lpPolyPointsInAct, nStrokeCountInAct);

	pDC->SelectObject(&penLines);
	pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);

	pDC->SelectObject(pOldPen);

	delete[] lpPoints;
	delete[] lpPolyPoints;
	delete[] lpPointsInAct;
	delete[] lpPolyPointsInAct;
}

void CCursorGridCtrl::PaintGridLines(CDC *pDC)
{
	PaintGridLines(pDC, rtHead);
	PaintGridLines(pDC, rtBody);
	PaintGridLines(pDC, rtTail);
}

void CCursorGridCtrl::ClearRows_head()
{
	CDrawLock DrawLock = CreateDrawLock();

	for (int i = 0; i < m_rows_head.GetSize(); i++)
		delete m_rows_head[i];
	m_rows_head.RemoveAll();
}

void CCursorGridCtrl::ClearRows_tail()
{
	CDrawLock DrawLock = CreateDrawLock();

	for (int i = 0; i < m_rows_tail.GetSize(); i++)
		delete m_rows_tail[i];
	m_rows_tail.RemoveAll();
}

void CCursorGridCtrl::ClearRows()
{
	CDrawLock DrawLock = CreateDrawLock();

	OnChangeCurrentData(NULL);

    for (int i = 0; i < m_rows.GetSize(); i++)
        delete m_rows[i];
    m_rows.RemoveAll();
	m_nTopRowIndex = 0;

	m_bIsBOF = TRUE;
    m_bIsEOF = TRUE;
}

void CCursorGridCtrl::QueryFields(CDataProvider* pProvider)
{
	for (int i = 0; i < m_pColumns->GetCount(); i++)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		m_pColumns->QueryDataField(pColumn, pProvider);

		if (!pColumn->GetVisible())
			m_pColumns->ReleaseDataField(pColumn, pProvider);
	}
}

CDataProvider* CCursorGridCtrl::GetDataProvider(RegionType rType) const
{
	CDataProvider* pProvider;
	switch(rType)
	{
	case rtBody:
		pProvider = m_pProvider;
		break;
	case rtHead:
		pProvider = m_pProvider_head;
		break;
	case rtTail:
		pProvider = m_pProvider_tail;
		break;
	default:
		pProvider = NULL;
	}
    return pProvider;
}

void CCursorGridCtrl::SetDataProvider(CDataProvider* pProvider)
{
    ClearRows();
    if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
        m_pCurrentDataRowOutOfWindow.reset();

    if (m_pProvider)
        m_pProvider->SetControl(NULL);

    m_pProvider = pProvider;
	m_SelectedRows.SetDataProvider(m_pProvider);

    if (!m_pProvider)
    {
		if (m_hWnd)
			Invalidate();
        return;
    }

	QueryFields(m_pProvider);
	m_pProvider->SetRegionType(rtBody);
    m_pProvider->SetControl(*this);
}

void CCursorGridCtrl::SetDataProvider_head(CDataProvider* pProvider)
{
	ClearRows_head();

	if (m_pProvider_head)
		m_pProvider_head->SetControl(NULL);

	m_pProvider_head = pProvider;
	if (!m_pProvider_head)
	{
		if (m_hWnd)
		{
			Invalidate();
			CalcRowsParams();
		}
	}
	else
	{
		QueryFields(m_pProvider_head);
		m_pProvider_head->SetRegionType(rtHead);
		m_pProvider_head->SetControl(*this);
	}
}

void CCursorGridCtrl::SetDataProvider_tail(CDataProvider* pProvider)
{
	ClearRows_tail();

	if (m_pProvider_tail)
		m_pProvider_tail->SetControl(NULL);

	m_pProvider_tail = pProvider;
	if (!m_pProvider_tail)
	{
		if (m_hWnd)
		{
			Invalidate();
			CalcRowsParams();
		}
	}
	else
	{
		QueryFields(m_pProvider_tail);
		m_pProvider_tail->SetRegionType(rtTail);
		m_pProvider_tail->SetControl(*this);
	}
}

void CCursorGridCtrl::Update()
{
	if (m_nUpdateLocks)
	{
		m_bUpdateReq = TRUE;
		return;
	}

	CUpdateLock UpdateLock = CreateUpdateLock();
	CDrawLock DrawLock = CreateDrawLock();

	if (m_pProvider)
	{
		BOOL bReset = FALSE;

		if (m_DropTarget.m_bDragOn)
		{
			m_DropTarget.ReleaseDropOverRow();
			CRect rect;
			rect.SetRectEmpty();
			m_DropTarget.DrawDragRect(rect);
		}

		if (m_rows.GetSize())
		{
			if (m_nTopRowIndex & 1)
				m_bEvenRow = !m_bEvenRow;

			int i;
			for (i = 0; i < m_nTopRowIndex; i++)
				delete m_rows[i];
			m_rows.RemoveAt(0, m_nTopRowIndex);

			if (m_nCurrentDataRowIndex != -1)
				m_nCurrentDataRowIndex -= m_nTopRowIndex;
			m_nTopRowIndex = 0;

			for (i = 1; i < m_rows.GetSize(); i++)
				delete m_rows[i];
			m_rows.RemoveAt(1, m_rows.GetUpperBound());

			if (m_pProvider->RefreshRow(m_rows[0]->GetDataRow()))
			{
				GetRowsDown(m_nVisibleRows - 1);
				if (m_nCurrentDataRowIndex == -1)
				{
					if (!m_pProvider->RefreshRow(m_pCurrentDataRowOutOfWindow.get()))
						SetCurrentCell(0, m_nCurrentColumn);
				}
				else
					OnChangeCurrentData(m_rows[m_nCurrentDataRowIndex]->GetDataRow());
			}
			else
			{
				BOOL bTopIsCurrent = m_nCurrentDataRowIndex == 0;
				GetRowsDown(m_nVisibleRows);
				delete m_rows[0];
				m_rows.RemoveAt(0);
				if (m_nCurrentDataRowIndex != -1)
					m_nCurrentDataRowIndex--;

				if (m_rows.GetSize())
				{
					if (bTopIsCurrent)
						SetCurrentCell(0, m_nCurrentColumn);
					else
					{
						if (m_nCurrentDataRowIndex == -1)
						{
							if (!m_pProvider->RefreshRow(m_pCurrentDataRowOutOfWindow.get()))
								SetCurrentCell(0, m_nCurrentColumn);
							else
								OnChangeCurrentData(m_pCurrentDataRowOutOfWindow.get());
						}
						else
							OnChangeCurrentData(m_rows[m_nCurrentDataRowIndex]->GetDataRow());
					}
				}
				else
					bReset = TRUE;
			}

			if (m_bIsBOF && !bReset)
				GetRowsUp(1);

		}
		else
			bReset = TRUE;

		if (bReset)
			Reset();
		else
		{
			FillRows(m_nTopRowIndex, m_nVisibleRows);
			SetVScrollInfo();
			Invalidate();
		}
	}
	m_bUpdateReq = FALSE;
}

void CCursorGridCtrl::Update_head()
{
	CDrawLock DrawLock = CreateDrawLock();
	ClearRows_head();

	if (m_pProvider_head)
	{
		m_pProvider_head->QueryRows(NULL, MAX_HEAD_ROWS, MAX_HEAD_ROWS);
		CDataRow* pDataRow;
		CDataRowPtr ptrDataRow;
		while (pDataRow = m_pProvider_head->Fetch())
		{
			ptrDataRow.reset(pDataRow);
			CGridRow* pGridRow = NewGridRow(m_pColumns, ptrDataRow, rtHead);
			pGridRow->FillItems();
			m_rows_head.Add(pGridRow);
		}
	}
	CalcRowsParams();
}

void CCursorGridCtrl::Update_tail()
{
	CDrawLock DrawLock = CreateDrawLock();
	ClearRows_tail();

	if (m_pProvider_tail)
	{
		m_pProvider_tail->QueryRows(NULL, MAX_TAIL_ROWS, MAX_TAIL_ROWS);
		CDataRow* pDataRow;
		CDataRowPtr ptrDataRow;
		while (pDataRow = m_pProvider_tail->Fetch())
		{
			ptrDataRow.reset(pDataRow);
			CGridRow* pGridRow = NewGridRow(m_pColumns, ptrDataRow, rtTail);
			pGridRow->FillItems();
			m_rows_tail.Add(pGridRow);
		}
	}
	CalcRowsParams();
}

int CCursorGridCtrl::GetVisibleRowIndex(RegionType rType, CDataRow* pDataRow)
{
	if (rType == rtHeader)
		 return m_bShowHeader ? 0 : -1;

	int nTopRowIndex = GetTopRowIndex(rType);
	int nVisibleRows = GetFullVisibleRows(rType);
	for (int i = nTopRowIndex; i < nTopRowIndex + nVisibleRows; ++i)
	{
		CDataProvider* pProvider = GetDataProvider(rType);
		if (pProvider->CompareRows(GetGridRow(rType, i)->GetDataRow(), pDataRow))
			return i;
	}
	return -1;
}

void CCursorGridCtrl::ActivateRow(CDataRow* pActRow)
{
    for (int i = m_nTopRowIndex; i < m_nTopRowIndex + m_nVisibleRows && i < m_rows.GetSize(); i++)
    {
        if (m_pProvider->CompareRows(m_rows[i]->GetDataRow(), pActRow))
        {
            SetCurrentCell(i, m_nCurrentColumn);
            return;
        }
    }
    Reset(pActRow);
}

void CCursorGridCtrl::Reset(CDataRow* pActRow)
{
	CDrawLock DrawLock = CreateDrawLock();
    ClearRows();

//    CDataRowPtr pCurrentRowToDel(m_pCurrentDataRowOutOfWindow);
    m_nCurrentDataRowIndex = -1;
	m_nCurrentDataRowDirection = 0;
    m_pCurrentDataRowOutOfWindow.reset();

    if (pActRow)
    {
        int nCount = m_nVisibleRows;
        m_pProvider->QueryRows(pActRow, nCount, m_nVisibleRows);
		CDataRow* pDataRow;
		CDataRowPtr ptrDataRow;
        while (pDataRow = m_pProvider->Fetch())
        {
			ptrDataRow.reset(pDataRow);
            m_rows.Add(NewGridRow(m_pColumns, ptrDataRow, rtBody));
            nCount--;
        }
        m_bIsEOF = nCount > 0;
    
 		m_nTopRowIndex = -1;
        nCount = min(m_nVisibleRows / 2, m_rows.GetSize());
        GetRowsUp(m_nVisibleRows - nCount);

        int nCurrentRow;
        if (m_nTopRowIndex != -1)
        {
            nCurrentRow = m_nTopRowIndex;
            m_nTopRowIndex = nCurrentRow - m_nVisibleRows / 2;
            if (m_nTopRowIndex + m_nVisibleRows > m_rows.GetSize())
                m_nTopRowIndex = m_rows.GetSize() - GetPageSize();
            if (m_nTopRowIndex < 0)
                m_nTopRowIndex = 0;
        }
        else
        {
            nCurrentRow = 0;
			m_nTopRowIndex = 0;
        }
        FillRows(m_nTopRowIndex, m_nVisibleRows);
        SetVScrollInfo();
        SetCurrentCell(nCurrentRow, m_nCurrentColumn);
    }
    else
    {
        int nCurrentRowIndex;
        if (m_StartView == svFromBegin)
        {
            m_bIsBOF = TRUE;
            m_bIsEOF = FALSE;
            GetRowsDown(m_nVisibleRows);
            nCurrentRowIndex = 0;
        }
        else //svFromEnd
        {
            m_bIsBOF = FALSE;
            m_bIsEOF = TRUE;
            GetRowsUp(m_nVisibleRows);
            m_nTopRowIndex = m_nTopRowIndex - GetPageSize();
            if (m_nTopRowIndex < 0)
                m_nTopRowIndex = 0;
            nCurrentRowIndex = m_rows.GetUpperBound();
        }

        FillRows(m_nTopRowIndex, m_nVisibleRows);

        m_nCurrentColumn = -1;
        ColumnsChanged();
        SetCurrentCell(nCurrentRowIndex, m_nCurrentColumn);
        SetVScrollInfo();
        Invalidate();
    }
}

LRESULT CCursorGridCtrl::OnDataChange(WPARAM wparam, LPARAM lparam)
{
	BYTE nEvent = LOBYTE(wparam);
	RegionType rType = (RegionType)HIBYTE(wparam);

    switch (nEvent)
    {
    case DP_RESET:
		switch(rType)
		{
		case rtBody:
			Reset((CDataRow*)lparam);
			break;
		case rtHead:
			Update_head();
			break;
		case rtTail:
			Update_tail();
			break;
		}
        break;
    case DP_FIELDS:
		switch(rType)
		{
		case rtBody:
			Update();
			break;
		case rtHead:
			Update_head();
			break;
		case rtTail:
			Update_tail();
			break;
		}
        break;
    case DP_CURROW:
		if (rType == rtBody)
			ActivateRow((CDataRow*)lparam);
        break;
    }
    return 0;
}

void CCursorGridCtrl::SetVScrollInfo()
{
	m_ThumbTracker.SetCurrentPos(0);

	if (m_VScrollerPresent == spNever)
    {
        ShowScrollBar(SB_VERT, FALSE);
        return;
    }

    if (!m_pProvider)
        return;

    BOOL bIsBegin = m_bIsBOF && m_nTopRowIndex == 0;
    BOOL bIsEnd = m_bIsEOF && m_nTopRowIndex + GetPageSize() >= m_rows.GetSize();

    if (m_VScrollerPresent == spAuto && bIsBegin && bIsEnd)
        ShowScrollBar(SB_VERT, FALSE);
    else
    {
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.nMin = 0;

        UINT nRowCount;
        if (m_pProvider->GetRowCount(&nRowCount))
        {
            if (!m_rows.GetSize())
                return;
            si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			si.nMax = --nRowCount ? nRowCount : 1;
            si.nPos = m_rows[m_nTopRowIndex]->m_pDataRow->GetRowIndex();
			si.nPage = GetPageSize();
        }
        else
        {
            si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
            si.nMax = 100;
            if (bIsBegin)
                si.nPos = 0;
            else if(bIsEnd)
                si.nPos = 100;
            else
                si.nPos = 50;
			si.nPage = 0;
        }

        BOOL bSuccess = SetScrollInfo(SB_VERT, &si);
        ASSERT(bSuccess);
        ShowScrollBar(SB_VERT, TRUE);

		m_ThumbTracker.SetCurrentPos(si.nPos);
    }
}

ScrollerPresent CCursorGridCtrl::GetVScroller() const
{
    return m_VScrollerPresent;
}

void CCursorGridCtrl::SetVScroller(ScrollerPresent scrollerPresent)
{
    if (m_VScrollerPresent == scrollerPresent)
        return;
    m_VScrollerPresent = scrollerPresent;
    SetVScrollInfo();
}

void CCursorGridCtrl::GetRowsDown(int nCount)
{
	if (m_nCurrentDataRowIndex != -1)
		m_nCurrentDataRowDirection = -1;

    while (m_nTopRowIndex > m_nVisibleRows - 1)
    {
        delete m_rows[0];
        m_rows.RemoveAt(0);
        if (m_nCurrentDataRowIndex != -1)
            m_nCurrentDataRowIndex--;
        m_nTopRowIndex--;
        m_bIsBOF = FALSE;
        m_bEvenRow = !m_bEvenRow;
    }

    for (int i = m_nTopRowIndex - 1; i >= 0; i--)
    {
        if (!m_rows[i]->CleanUp())
            break;
    }

    CDataRow* pRowFrom;
    if (m_rows.GetSize())
        pRowFrom = m_rows[m_rows.GetUpperBound()]->GetDataRow();
    else
        pRowFrom = NULL;

    m_pProvider->QueryRows(pRowFrom, nCount, m_nVisibleRows);

	CDataRow* pDataRow;
	CDataRowPtr ptrDataRow;
    while (pDataRow = m_pProvider->Fetch())
    {
		ptrDataRow.reset(pDataRow);
        m_rows.Add(NewGridRow(ptrDataRow, m_rows.GetSize()));
        nCount--;
    }
    m_bIsEOF = nCount > 0;
}

void CCursorGridCtrl::GetRowsUp(int nCount)
{
	if (m_nCurrentDataRowIndex != -1)
		m_nCurrentDataRowDirection = 1;

    while (m_rows.GetSize() - (m_nTopRowIndex + m_nVisibleRows) > m_nVisibleRows - 1)
    {
        delete m_rows[m_rows.GetUpperBound()];
        m_rows.RemoveAt(m_rows.GetUpperBound());
        m_bIsEOF = FALSE;
    }

    for (int i = m_nTopRowIndex + m_nVisibleRows; i < m_rows.GetSize(); i++)
    {
        if (!m_rows[i]->CleanUp())
            break;
    }

    CDataRow* pRowFrom;
    if (m_rows.GetSize())
        pRowFrom = m_rows[0]->GetDataRow();
    else
        pRowFrom = NULL;

    m_pProvider->QueryRows(pRowFrom, -nCount, m_nVisibleRows);

	CDataRow* pDataRow;
	CDataRowPtr ptrDataRow;
    while (pDataRow = m_pProvider->Fetch())
    {
		ptrDataRow.reset(pDataRow);
        if (m_nCurrentDataRowIndex != -1)
            m_nCurrentDataRowIndex++;
        m_rows.InsertAt(0, NewGridRow(ptrDataRow, 0));
        nCount--;
        m_nTopRowIndex++;
        m_bEvenRow = !m_bEvenRow;
    }
	m_bIsBOF = nCount > 0;
}

CGridRow* CCursorGridCtrl::NewGridRow(CDataRowPtr const& pDataRow, int nNewRowIndex)
{
    CGridRow* pGridRow = NewGridRow(m_pColumns, pDataRow, rtBody);
    if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
    {
        if (m_pProvider->CompareRows(pDataRow.get(), m_pCurrentDataRowOutOfWindow.get()))
        {
            m_pCurrentDataRowOutOfWindow.reset();
            m_nCurrentDataRowIndex = nNewRowIndex;
			m_nCurrentDataRowDirection = 0;
            pGridRow->m_bIsCurrent = true;
            OnChangeCurrentData(pDataRow.get());
        }
    }
    return pGridRow;
}

int CCursorGridCtrl::GetFixedWidth_left() const
{
	int nWidth = 0;
	for (int i = 0, nFixed = m_nFixedLeftEx; nFixed && i < m_pColumns->GetSize(); ++i)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			if (pGridColumn->m_nLineNumber == 1)
				nWidth += pGridColumn->m_rect.Width();
			--nFixed;
		}
	}
	return nWidth;
}

int CCursorGridCtrl::GetFixedWidth_right() const
{
	int nWidth = 0;
	for (int i = m_pColumns->GetUpperBound(), nFixed = m_nFixedRightEx; nFixed && i >= 0; --i)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			if (pGridColumn->m_nLineNumber == 1)
				nWidth += pGridColumn->m_rect.Width();
			--nFixed;
		}
	}
	return nWidth;
}

CRect CCursorGridCtrl::GetPageRect() const
{
	CRect rect;
	GetClientRectEx(rect);
	rect.left += GetFixedWidth_left();
	rect.right -= GetFixedWidth_right();

	if (rect.right < rect.left)
		rect.right = rect.left;

	return rect;
}

CRect CCursorGridCtrl::GetColumnBoundsRect(CGridColumn* pGridColumn)
{
	CRect ClientRect;
	GetClientRectEx(ClientRect);
	CRect rect(ClientRect.left, ClientRect.top, ClientRect.left, ClientRect.top);
	BOOL bFound = FALSE;
	int i = 0, j = m_pColumns->GetUpperBound();

	for (int nFixedLeft = m_nFixedLeftEx; nFixedLeft && i <= j; ++i)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn == pGridColumn)
			{
				rect = pColumn->m_rect;
				bFound = TRUE;
				break;
			}
			--nFixedLeft;
		}
	}
	if (!bFound)
	{
		for (int nFixedRight = m_nFixedRightEx; nFixedRight && i <=j; --j)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(j);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					rect = pColumn->m_rect;
					rect.OffsetRect(ClientRect.Width() - m_pColumns->GetVisibleWidth(), 0);
					bFound = TRUE;
					break;
				}
				--nFixedRight;
			}
		}
	}
	if (!bFound)
	{
		if (pGridColumn->m_bVisible)
		{
			rect = pGridColumn->m_rect;
			rect.OffsetRect(-GetOrigin(), 0);
		}
		/*
		for (; i <= j; ++i)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					rect = pColumn->m_rect;
					rect.OffsetRect(-GetOrigin(), 0);
					break;
				}
			}
		}
		*/
	}
	rect.top = ClientRect.top;
	rect.bottom = ClientRect.bottom;
	return rect;
}

CRect CCursorGridCtrl::GetColumnClipRect(CGridColumn* pGridColumn)
{
	CRect ClientRect;
	GetClientRectEx(ClientRect);
	CRect rect(ClientRect.left, ClientRect.top, ClientRect.left, ClientRect.top);
	BOOL bFound = FALSE;
	int i = 0, j = m_pColumns->GetUpperBound();

	for (int nFixedLeft = m_nFixedLeftEx; nFixedLeft && i <= j; ++i)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn == pGridColumn)
			{
				rect.IntersectRect(pColumn->m_rect, ClientRect);
				bFound = TRUE;
				break;
			}
			--nFixedLeft;
		}
	}
	if (!bFound)
	{
		for (int nFixedRight = m_nFixedRightEx; nFixedRight && i <=j; --j)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(j);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					CRect Rect = pColumn->m_rect;
					Rect.OffsetRect(ClientRect.Width() - m_pColumns->GetVisibleWidth(), 0);
					Rect.left = max(Rect.left, ClientRect.left + GetFixedWidth_left());
					rect.IntersectRect(Rect, ClientRect);
					bFound = TRUE;
					break;
				}
				--nFixedRight;
			}
		}
	}
	if (!bFound)
	{
		if (pGridColumn->m_bVisible)
		{
			CRect Rect = pGridColumn->m_rect;
			Rect.OffsetRect(-GetOrigin(), 0);
			rect.IntersectRect(Rect, GetPageRect());
		}
		/*
		for (; i <= j; ++i)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					CRect Rect = pColumn->m_rect;
					Rect.OffsetRect(-GetOrigin(), 0);
					rect.IntersectRect(Rect, GetPageRect());
					break;
				}
			}
		}
		*/
	}
	rect.top = ClientRect.top;
	rect.bottom = ClientRect.bottom;
	return rect;
}

void CCursorGridCtrl::RepositionEdit() const
{
	CGridCellEdit* pEdit = GetEdit();
	if (pEdit)
		pEdit->Reposition();
}

void CCursorGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this) 
		SetFocus();

	if (!GetEdit())
	{
		m_Tip.Hide();

		UINT nRowCount;

		switch (nSBCode)
		{
		case SB_LINEDOWN:
			ScrollDown(1);
			break;
		case SB_LINEUP:
			ScrollUp(1);
			break;
		case SB_PAGEDOWN:
			ScrollDown(GetPageSize());
			break;
		case SB_PAGEUP:
			ScrollUp(GetPageSize());
			break;
		case SB_THUMBPOSITION:
			if (m_pProvider && !m_pProvider->GetRowCount(&nRowCount))
			{
				m_ThumbTracker.Reset();

				if (nPos == 0)
					ToHome();
				else if (nPos == 100)
					ToEnd();
				else
					SetVScrollInfo();
			}
			break;
		case SB_THUMBTRACK:
			if (m_pProvider && m_pProvider->GetRowCount(&nRowCount))
			{
				if (!m_rows.GetSize())
					break;

				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
				UINT nIndex = m_rows[m_nTopRowIndex]->m_pDataRow->GetRowIndex();
				int dy = si.nTrackPos - nIndex;
				if (dy > 0)
					ScrollDown(dy);
				else
					ScrollUp(-dy);
			}
			else
				m_ThumbTracker.Track(nPos);
			break;
		}
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCursorGridCtrl::ScrollDown(int nPageSize)
{
    int nGetRows = m_nTopRowIndex + m_nVisibleRows + nPageSize - m_rows.GetUpperBound();
    if (nGetRows > 0 && !m_bIsEOF)
        GetRowsDown(nGetRows);
    if (m_nTopRowIndex + m_nVisibleRows <= m_rows.GetUpperBound())
    {
        FillRows(m_nTopRowIndex + m_nVisibleRows, nPageSize);

        int nScroll = m_rows.GetSize() - (m_nTopRowIndex + GetPageSize());
        if (nScroll > nPageSize)
            nScroll = nPageSize;
        m_nTopRowIndex += nScroll;

        InvalidateRect(GetGridRect());
        SetVScrollInfo();
		return TRUE;
    }
    else if (m_bIsEOF && m_nTopRowIndex + GetPageSize() <= m_rows.GetUpperBound() && m_nTopRowIndex < m_rows.GetUpperBound())
    {
        m_nTopRowIndex++;
        InvalidateRect(GetGridRect());
        SetVScrollInfo();
		return TRUE;
    }
	return FALSE;
}

BOOL CCursorGridCtrl::ScrollUp(int nPageSize)
{
    int nGetRows = nPageSize - m_nTopRowIndex;
    if (nGetRows > 0 && !m_bIsBOF)
    {
        GetRowsUp(nGetRows);
        if (m_nTopRowIndex == 0)
        {
            SetVScrollInfo();
            return FALSE;
        }
    }
    if (m_nTopRowIndex > 0)
    {
        int nFirst = m_nTopRowIndex - nPageSize;
        if (nFirst < 0)
            nFirst = 0;

        FillRows(nFirst, m_nTopRowIndex - nFirst);
        m_nTopRowIndex = nFirst;

        InvalidateRect(GetGridRect());
        SetVScrollInfo();
		return TRUE;
    }
	return FALSE;
}

BOOL CCursorGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (GetEdit())
		return TRUE;

    m_Tip.Hide();
    
    UINT nScroll;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nScroll, 0);

    if (zDelta < 0)
        ScrollDown(nScroll);
    else
        ScrollUp(nScroll);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CCursorGridCtrl::SetCurrentCell(int nRowIndex, int nColumnIndex, BOOL bForced)
{
    if (!bForced && m_nCurrentDataRowIndex == nRowIndex && m_nCurrentColumn == nColumnIndex)
		return;

	BOOL bRowActivated = FALSE;
	BOOL bColumnActivated = FALSE;

	if (m_nCurrentDataRowIndex != nRowIndex)
	{
		if (m_nCurrentDataRowIndex == -1)
			m_pCurrentDataRowOutOfWindow.reset();
		else
			m_rows[m_nCurrentDataRowIndex]->m_bIsCurrent = false;

		if (nRowIndex >= 0 && nRowIndex < m_rows.GetSize())
		{
			m_nCurrentDataRowIndex = nRowIndex;
			m_rows[nRowIndex]->m_bIsCurrent = true;
			OnChangeCurrentData(m_rows[nRowIndex]->GetDataRow());
			bRowActivated = TRUE;
		}
		else
		{
			m_nCurrentDataRowIndex = -1;
			OnChangeCurrentData(NULL);
		}
	}

	if (nColumnIndex != m_nCurrentColumn)
	{
		m_nCurrentColumn = nColumnIndex;
		DisplayColumn(m_pColumns->GetAt(nColumnIndex));
		bColumnActivated = TRUE;
	}

	if (m_nCurrentDataRowIndex == m_nTopRowIndex + m_nVisibleRows - 1 && m_bLastRowNotFull)
		ScrollDown(1);

	m_SelectedRows.SetRemovedRow(CDataRowPtr());
	m_SelectedRows.SetCurrentRow(CDataRowPtr());
	m_SelectedRows.SetDirection(0);

	Invalidate();

	CCommandQueueLock CQLock(this);
	if (bRowActivated)
		m_CommandQueue.Add(&CCursorGridCtrl::OnActivateRow);

	if (bColumnActivated)
		m_CommandQueue.Add(&CCursorGridCtrl::OnActivateColumn);

	m_CommandQueue.Add(&CCursorGridCtrl::OnActivateCell);
}

BOOL CCursorGridCtrl::DisplayColumn(CGridColumn* pColumn)
{
	BOOL bShifted = FALSE;

	CRect rect = GetColumnBoundsRect(pColumn);
	CRect clip = GetColumnClipRect(pColumn);
	CRect page = GetPageRect();
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ, &si);
	if (clip.Width() > 0)
	{
		int pos = clip.left - rect.left;
		if (pos > 0)
		{
			si.nPos -= pos;
			SetScrollInfo(SB_HORZ, &si);
			Invalidate();
			bShifted = TRUE;
		}
		else
		{
			pos = rect.right - clip.right;
			if (pos > 0)
			{
				int pos_alt = rect.left - page.left;
				si.nPos += min(pos, pos_alt);
				SetScrollInfo(SB_HORZ, &si);
				Invalidate();
				bShifted = TRUE;
			}
		}
	}
	else
	{
		int pos = rect.left - page.left;
		int pos_alt = rect.right - page.right;
		if (pos > 0)
			si.nPos += min(pos, pos_alt);
		else
			si.nPos += max(pos, pos_alt);

		SetScrollInfo(SB_HORZ, &si);
		Invalidate();
		bShifted = TRUE;
	}
	return bShifted;
}

int CCursorGridCtrl::GetNextColumn(int nIndex, int nDirection)
{
    for (int i = nIndex + nDirection; i >= 0 && i < m_pColumns->GetSize(); i += nDirection)
    {
        if (m_pColumns->GetAt(i)->m_bVisible)
            return i;
    }
    return -1;
 }

void CCursorGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (!(nFlags & (1 << 14)))
        m_Tip.Hide();

	BOOL bSHIFT = IS_SHIFT_PRESSED();
	BOOL bCTRL = IS_CTRL_PRESSED();
    
	switch (nChar)
    {
    case VK_LEFT:
		ScrollCurrentCell(-1, 0);
        break;
    case VK_RIGHT:
		ScrollCurrentCell(1, 0);
        break;
    case VK_UP:
		if (!bCTRL)
		{
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
			ScrollCurrentCell(0, -1);
			if (bSHIFT)
				SelectRows(st, -1 , TRUE);
		}
        break;
    case VK_DOWN:
		if (!bCTRL)
		{
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
			ScrollCurrentCell(0, 1);
			if (bSHIFT)
				SelectRows(st, 1, TRUE);
		}
        break;
    case VK_PRIOR:
        {
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
            if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
                Reset(m_pCurrentDataRowOutOfWindow.get());

            if (m_nCurrentDataRowIndex != -1)
            {
                int nScroll = m_nTopRowIndex + GetPageSize() - 1 - m_nCurrentDataRowIndex;
                if (nScroll > 0)
                    ScrollUp(nScroll);
                SetCurrentCell(m_nTopRowIndex, m_nCurrentColumn);
				if (bSHIFT)
					SelectRows(st, -1);
				else
					RedrawWindow(GetGridRect());
            }
        }
        break;
    case VK_NEXT:
        {
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
            if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
                Reset(m_pCurrentDataRowOutOfWindow.get());

            if (m_nCurrentDataRowIndex != -1)
            {
                int nScroll = m_nCurrentDataRowIndex - m_nTopRowIndex;
                if (nScroll > 0)
                    ScrollDown(nScroll);
                int nIndex = m_nTopRowIndex + GetPageSize() - 1;
                if (nIndex > m_rows.GetUpperBound())
                    nIndex = m_rows.GetUpperBound();
                SetCurrentCell(nIndex, m_nCurrentColumn);
				if (bSHIFT)
					SelectRows(st, 1);
				else
					RedrawWindow(GetGridRect());
            }
        }
        break;
    case VK_HOME:
        {
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
            if (m_bIsBOF)
            {
                int nScroll = m_nTopRowIndex;
                if (nScroll > 0)
                    ScrollUp(nScroll);
                if (m_rows.GetSize())
                    SetCurrentCell(m_nTopRowIndex, m_nCurrentColumn);
            }
            else
                ToHome();

			if (bSHIFT)
				SelectRows(st, -1);
        }
        break;
    case VK_END:
        {
			CCommandQueueLock CQLock(this);
			CDataRowState st(this);
            if (m_bIsEOF)
            {
                int nScroll = m_rows.GetUpperBound() - (m_nTopRowIndex + GetPageSize() - 2);
                if (nScroll > 0)
                    ScrollDown(nScroll);
                if (m_rows.GetSize())
                    SetCurrentCell(m_rows.GetUpperBound(), m_nCurrentColumn);
            }
            else
                ToEnd();

			if (bSHIFT)
				SelectRows(st, 1);
        }
        break;
    case VK_F5:
		if (bSHIFT)
		{
			m_SelectedRows.RemoveAll();
			InvalidateRect(GetGridRect());
		}
        Update();
        break;
    }
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCursorGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_CONTROL)
        //m_ColumnHelper.Hide()
    {};
}

UINT CCursorGridCtrl::OnGetDlgCode()
{
	return DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTCHARS;
}

int CCursorGridCtrl::GetPageSize()
{
    int nPageSize = m_nVisibleRows;
    if (m_bLastRowNotFull)
        nPageSize--;
    return nPageSize;
}

void CCursorGridCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    m_Tip.Hide();
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CCursorGridCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) 
		SetFocus();

	if (!GetEdit())
	{
		m_Tip.Hide();
		m_hitMouseButton = point;

		int nColumn, nRow, nImageIndex;
		RegionType rType;

		switch(HitTest(point, nColumn, nRow, rType, nImageIndex))
		{
		case ghtCell:
		case ghtImage:
			if (rType == rtBody)
			{
				CCommandQueueLock CQLock(this);
				SetCurrentCell(nRow, nColumn);
				ClearSelection(GetCurrentDataRow());

				if (m_bEnableDrag)
				{
					m_CaptureType = ctDragDelay;
					SetCapture();
				}
			}
			break;
		case ghtCheckBox:
		case ghtSysImage:
			if (rType == rtBody)
				SetCurrentCell(nRow, nColumn);
			break;
		}
	}
    CWnd::OnRButtonDown(nFlags, point);
}

void CCursorGridCtrl::ToHome()
{
    ClearRows();

    m_nTopRowIndex = 0;
    m_bIsBOF = TRUE;
    GetRowsDown(m_nVisibleRows);
    FillRows(0, m_nVisibleRows);
    SetCurrentCell(0, m_nCurrentColumn);
    SetVScrollInfo();
}

void CCursorGridCtrl::ToEnd()
{
    ClearRows();
    m_bIsEOF = TRUE;
    GetRowsUp(GetPageSize());
    m_nTopRowIndex = m_rows.GetSize() - GetPageSize();
    if (m_nTopRowIndex < 0)
        m_nTopRowIndex = 0;
    FillRows(m_nTopRowIndex, m_nVisibleRows);
    SetCurrentCell(m_rows.GetUpperBound(), m_nCurrentColumn);
    SetVScrollInfo();
}

BOOL CCursorGridCtrl::IsFocused()
{
    return ::GetFocus() == GetSafeHwnd();
}

void CCursorGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	InvalidateRect(GetGridRect());
}

void CCursorGridCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
    m_Tip.Hide();
	InvalidateRect(GetGridRect());
}

void CCursorGridCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (!bShow)
		CloseEdit(FALSE);
}

void CCursorGridCtrl::ColumnsChanged()
{
    RecalcColumnsWidth();
    
    if (m_nCurrentColumn == -1)
        m_nCurrentColumn = GetNextColumn(m_nCurrentColumn, 1);
    else
    {
        if (!m_pColumns->GetAt(m_nCurrentColumn)->m_bVisible)
            m_nCurrentColumn = GetNextColumn(m_nCurrentColumn, 1);
    }
}

StartView CCursorGridCtrl::GetStartView() const
{
    return m_StartView;
}

void CCursorGridCtrl::SetStartView(StartView startView)
{
    m_StartView = startView;
}

RowSelectMode CCursorGridCtrl::GetRowSelectMode() const
{
    return m_RowSelectMode;
}

void CCursorGridCtrl::SetRowSelectMode(RowSelectMode rowSelectMode)
{
    if (m_RowSelectMode == rowSelectMode)
        return;
    m_RowSelectMode = rowSelectMode;
    InvalidateRect(GetGridRect());
}

SelectMode CCursorGridCtrl::GetSelectMode() const
{
    return m_SelectMode;
}

void CCursorGridCtrl::SetSelectMode(SelectMode selectMode)
{
    if (m_SelectMode != selectMode)
	{
		m_SelectMode = selectMode;
		m_SelectedRows.RemoveAll();
		InvalidateRect(GetGridRect());
	}
}

BOOL CCursorGridCtrl::GetVerticalLines() const
{
    return m_bVerticalLines;
}

void CCursorGridCtrl::SetVerticalLines(BOOL bVerticalLines /*= TRUE*/)
{
    if (bVerticalLines == m_bVerticalLines)
        return;
    m_bVerticalLines = bVerticalLines;
    InvalidateRect(GetGridRect());
}

BOOL CCursorGridCtrl::GetHorizontalLines() const
{
    return m_bHorizontalLines;
}

void CCursorGridCtrl::SetHorizontalLines(BOOL bHorizontalLines /*= TRUE*/)
{
    if (bHorizontalLines == m_bHorizontalLines)
        return;
    m_bHorizontalLines = bHorizontalLines;
    InvalidateRect(GetGridRect());
}

HeadersStyle CCursorGridCtrl::GetHeadersStyle() const
{
    return m_HeadersStyle;
}

void CCursorGridCtrl::SetHeadersStyle(HeadersStyle headersStyle)
{
    if (headersStyle == m_HeadersStyle)
        return;

    m_HeadersStyle = headersStyle;
    InvalidateRect(GetHeaderRect());
}

ActiveHeaderStyle CCursorGridCtrl::GetActiveHeaderStyle() const
{
	return m_ActiveHeaderStyle;
}

void CCursorGridCtrl::SetActiveHeaderStyle(ActiveHeaderStyle style)
{
	if (style == m_ActiveHeaderStyle)
		return;

	m_ActiveHeaderStyle = style;
	InvalidateRect(GetHeaderRect());
}

BorderStyle CCursorGridCtrl::GetBorderStyle() const
{
	return m_BorderStyle;
}

void CCursorGridCtrl::SetBorderStyle(BorderStyle style)
{
	DWORD dwAdd = 0;
	DWORD dwRemove = 0;
	(style == bs3D ? dwAdd : dwRemove) = WS_EX_CLIENTEDGE;
	m_BorderStyle = style;
	ModifyStyleEx(dwRemove, dwAdd, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

CGridColumn* CCursorGridCtrl::GetCurrentColumn() const
{
    if (m_nCurrentColumn == -1)
        return NULL;
    else
        return m_pColumns->GetAt(m_nCurrentColumn);
}

void CCursorGridCtrl::SetCurrentColumn(CGridColumn* pColumn)
{
    int nIndex = m_pColumns->IndexOf(pColumn);
    if (nIndex < 0 || nIndex == m_nCurrentColumn)
        return;
    SetCurrentCell(m_nCurrentDataRowIndex, nIndex);
}

void CCursorGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!GetEdit())
	{
		int nColumn, nRow, nImageIndex;
		RegionType rType;

		GridHitTest ht = HitTest(point, nColumn, nRow, rType, nImageIndex);
                         
		switch(ht)
		{
		case ghtHeader:
			m_pCapturedColumn = GetResizeHeaderAt(point);
			if (m_pCapturedColumn)
			{
				if (IS_SHIFT_PRESSED())
					AutoWidth();
				else
				{
					m_pCapturedColumn = PrepareColResize(m_pCapturedColumn);
					int nWidth = 0;
					if (m_CapturedLocation == clSomeColumn)
					{
						nWidth = GetColumnAutoWidth(m_pCapturedColumn);
					}
					else
					{
						int nLineWidth = 0;
						int nLineNumber = 0;

						for (int i = m_pColumns->IndexOf(m_pCapturedColumn); i < m_pColumns->GetSize(); i++)
						{
							CGridColumn* pColumn = m_pColumns->GetAt(i);
							if (pColumn->m_bVisible)
							{
								if (pColumn->m_nFamily != m_pCapturedColumn->m_nFamily)
									break;

								if (pColumn->m_nLineNumber != nLineNumber)
								{
									nLineNumber = pColumn->m_nLineNumber;
									nWidth = max(nWidth, nLineWidth);
									nLineWidth = 0;
								}
								nLineWidth += GetColumnAutoWidth(pColumn);
							}
						}
						nWidth = max(nWidth, nLineWidth);
					}
					if (m_pCapturedColumn && nWidth)
					{
						StepColResize(nWidth);
						EndColResize();
					}
				}
				OnColumnWidthChange();
			}
			else
				OnSelection(NULL, m_pColumns->GetAt(nColumn), rType, ht);
			break;
		case ghtCell:
			if ((rType != rtBody) || (GetGridRow(rType, nRow)->GetDataRow() == GetCurrentDataRow()))
				OnSelection(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType, ht);
			break;
		case ghtCheckBox:
			if ( m_bSelOnDblClk )
				if ((rType != rtBody) || (GetGridRow(rType, nRow)->GetDataRow() == GetCurrentDataRow()))
					OnSelection(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType, ht);
			else
				OnClickCheckBox(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType);
			break;
		case ghtImage:
			if ( m_bSelOnDblClk )
				if ((rType != rtBody) || (GetGridRow(rType, nRow)->GetDataRow() == GetCurrentDataRow()))
					OnSelection(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType, ht);
			else
				OnClickImage(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), nImageIndex, rType);
			break;
		case ghtSysImage:
			if ( m_bSelOnDblClk )
				if ((rType != rtBody) || (GetGridRow(rType, nRow)->GetDataRow() == GetCurrentDataRow()))
					OnSelection(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType, ht);
			else
				OnClickSysImage(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), nImageIndex, rType);
			break;
		}
	}
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CCursorGridCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (GetEdit())
		return;

	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		if (GetCurrentCellBounds(rect))
		{
			point.x = rect.left;
			point.y = (rect.top + rect.bottom) / 2;
			ClientToScreen(&point);
		}
		else
			return;
	}

	m_ptContextMenuPosition = point;
	ScreenToClient(&point);

	int nColumn, nRow, nImageIndex;
	RegionType rType;

	HitTest(point, nColumn, nRow, rType, nImageIndex);
	CGridRow* pGridRow = GetGridRow(rType, nRow);

	CDataRow* pDataRow = !pGridRow ? NULL : pGridRow->GetDataRow();
	CGridColumn* pColumn = nColumn == -1 ? NULL : m_pColumns->GetAt(nColumn);

	OnShowContextMenu(rType, pColumn, pDataRow);
	m_ptContextMenuPosition = CPoint(0,0);
}

LRESULT CCursorGridCtrl::OnGetFont(WPARAM wparam, LPARAM lparam)
{
    return (LRESULT)m_font.operator HFONT();
}

LRESULT CCursorGridCtrl::OnSetFont(WPARAM wparam, LPARAM lparam)
{
    HFONT hFont = (HFONT)wparam;
    BOOL fRedraw = lparam;

    if (hFont)
        InitFont(CFont::FromHandle(hFont));
    else
        InitFont(GetParent()->GetFont());

    if (fRedraw)
        Invalidate();
    
    return 0;
}

void CCursorGridCtrl::FillRows(int nFirst, int nCount)
{
    if (nFirst < 0)
        return;
    int nEnd = nFirst + nCount;
    if (nEnd > m_rows.GetSize())
        nEnd = m_rows.GetSize();
    if (nEnd <= 0)
        return;

	CDrawLock DrawLock = CreateDrawLock();

    for (int i = nFirst; i < nEnd; i++)
        m_rows[i]->FillItems();
}

void CCursorGridCtrl::FillRows_head()
{
	int nSize = m_rows_head.GetSize();
	for (int i = 0; i < nSize; ++i)
		m_rows_head[i]->FillItems();
}

void CCursorGridCtrl::FillRows_tail()
{
	int nSize = m_rows_tail.GetSize();
	for (int i = 0; i < nSize; ++i)
		m_rows_tail[i]->FillItems();
}

void CCursorGridCtrl::RefreshRows()
{
	Update();
}

void CCursorGridCtrl::RefreshRows_head()
{
	Update_head();
}

void CCursorGridCtrl::RefreshRows_tail()
{
	Update_tail();
}

void CCursorGridCtrl::RedrawRows()
{
	FillRows(m_nTopRowIndex, m_nVisibleRows);
    SetVScrollInfo();
    Invalidate();
}

void CCursorGridCtrl::RedrawRows_head()
{
	FillRows_head();
}

void CCursorGridCtrl::RedrawRows_tail()
{
	FillRows_tail();
}

void CCursorGridCtrl::SetTextMargins(int nLeft, int nTop, int nRight, int nBottom)
{
	if (nLeft >= 0)
		m_nLeftTextMargin = nLeft;

	if (nTop >= 0)
		m_nTopTextMargin = nTop;

	if (nRight >= 0)
		m_nRightTextMargin = nRight;

	if (nBottom >= 0)
		m_nBottomTextMargin = nBottom;

	RecalcRowHeight();

    SetVScrollInfo();
    Invalidate();
}

USHORT CCursorGridCtrl::GetRefreshTimeout() const
{
    return m_nRefreshTimeout;
}

void CCursorGridCtrl::SetRefreshTimeout(USHORT nTimeout)
{
    if (m_nRefreshTimeout == nTimeout)
        return;
    m_nRefreshTimeout = nTimeout;
    UINT nRetVal;
    if (m_nRefreshTimeout)
        nRetVal = SetTimer(TIMER_ID_UPDATE, m_nRefreshTimeout * 1000, NULL);
    else
        nRetVal = KillTimer(TIMER_ID_UPDATE);
}

void CCursorGridCtrl::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent)
    {
    case TIMER_ID_UPDATE:
		Update();
        break;
    case TIMER_ID_AUTOHIDE:
        {
            CPoint point;
            ::GetCursorPos(&point);
            ScreenToClient(&point);
            CRect rect;
            GetClientRectEx(rect);
            if (m_ColumnHelper.IsDisplay())
            {
                CRect rectHelper;
                m_ColumnHelper.GetWindowRect(rectHelper);
                ScreenToClient(rectHelper);
                rect.left = rectHelper.left;
                rect.bottom = rectHelper.bottom;
                if (!rect.PtInRect(point) && m_CaptureType != ctMoveColumn)
                    m_ColumnHelper.Hide();
            }
            else if (m_bShowHeader)
            {
                rect.left = rect.right - 16;
				rect.bottom = rect.top + m_nRowHeight;
				//rect.top = rect.top + m_nRowHeight - m_nSingleLineHeight;
				//rect.bottom = rect.top + m_nSingleLineHeight;

                if (rect.PtInRect(point))
                {
                    m_Tip.Hide();
                    m_ColumnHelper.Show();
                }
            }
        }
        break;
	case TIMER_ID_SCROLL:
		{
            CPoint point;
            ::GetCursorPos(&point);
            ScreenToClient(&point);

			CRect rect = GetGridRect(rtBody);
			if (!rect.PtInRect(point))
			{
				int xDirection = point.x < rect.TopLeft().x ? -1 : point.x > rect.BottomRight().x ? 1 : 0;
				int yDirection = point.y < rect.TopLeft().y ? -1 : point.y > rect.BottomRight().y ? 1 : 0;
				ScrollCurrentCell(xDirection, yDirection);
			}
		}
		break;
	case TIMER_ID_THUMBTRACK:
		m_ThumbTracker.OnTimer();
		break;
    }
}

void CCursorGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!m_pProvider)
        return;
    if (m_nCurrentColumn == -1)
        return;

    CGridColumn* pColumn = m_pColumns->GetAt(m_nCurrentColumn);

    DataType dataType = m_pProvider->GetQuickSearchType(pColumn->m_strDataField);
    switch (dataType)
    {
    case dtText:
        break;
    default:
        return;
    }

	if (OnQuickSearchStart(dataType))
	{
		CRect rect(pColumn->GetBoundsRect());
		CRect rectClient;
		GetClientRectEx(rectClient);
		int nHeight = rect.Height();
		rect.top = rectClient.Height();
		if (m_BorderStyle != hs3D || m_rows_tail.GetSize())
			--rect.top;
		rect.bottom = rect.top + nHeight;
		char arText[] = {nChar, 0};
		m_QuickSearch.Show(rect, pColumn->m_strDataField, arText);
	}
}

USHORT CCursorGridCtrl::GetQuickSearchTimeout() const
{
    return m_QuickSearch.m_nTimeout;
}

void CCursorGridCtrl::SetQuickSearchTimeout(USHORT nTimeout)
{
    m_QuickSearch.m_nTimeout = nTimeout;
}

BOOL CCursorGridCtrl::GetChangePositionOfColumns() const
{
    return m_bChangePositionOfColumns;
}

void CCursorGridCtrl::SetChangePositionOfColumns(BOOL bChangePositionOfColumns /*= TRUE*/)
{
    m_bChangePositionOfColumns = bChangePositionOfColumns;
}

COLORREF CCursorGridCtrl::GetLinesColor()
{
	if (m_crGridLines == CLR_DEFAULT)
		return ::GetSysColor(COLOR_BTNSHADOW);
	else
		return m_crGridLines;
}

COLORREF CCursorGridCtrl::GetBorderColor()
{
	if (m_crGridBorder == CLR_DEFAULT)
		return GetLinesColor();
	else
		return m_crGridBorder;
}

COLORREF CCursorGridCtrl::GetGridLinesColor() const
{
	return m_crGridLines;
}

void CCursorGridCtrl::SetGridLinesColor(COLORREF crGridLines)
{
	if (m_crGridLines == crGridLines)
		return;
	m_crGridLines = crGridLines;
	Invalidate();
}

COLORREF CCursorGridCtrl::GetInactiveLinesColor()
{
	if (m_crInactiveGridLines == CLR_DEFAULT)
		return ::GetSysColor(COLOR_BTNFACE);
	else
		return m_crInactiveGridLines;
}

COLORREF CCursorGridCtrl::GetInactiveGridLinesColor() const
{
	return m_crInactiveGridLines;
}

void CCursorGridCtrl::SetInactiveGridLinesColor(COLORREF crInactiveGridLines)
{
	if (m_crInactiveGridLines == crInactiveGridLines)
		return;
	m_crInactiveGridLines = crInactiveGridLines;
	Invalidate();
}

COLORREF CCursorGridCtrl::GetTextColor() const
{
    return m_crText;
}

void CCursorGridCtrl::SetTextColor(COLORREF crTextColor)
{
    if (m_crText == crTextColor)
        return;
    m_crText = crTextColor;
    Invalidate();
}

COLORREF CCursorGridCtrl::GetBackColor()
{
	if (m_crBack == CLR_DEFAULT)
		return ::GetSysColor(COLOR_WINDOW);
	else
		return m_crBack;
}

COLORREF CCursorGridCtrl::GetGridBackColor() const
{
    return m_crBack;
}

void CCursorGridCtrl::SetGridBackColor(COLORREF crBackColor)
{
    if (m_crBack == crBackColor)
        return;
    m_crBack = crBackColor;
    Invalidate();
}

WORD CCursorGridCtrl::GetBackTransparency() const
{
	return m_trpBack;
}

void CCursorGridCtrl::SetBackTransparency(WORD trp)
{
	if (m_trpBack == trp)
		return;
	m_trpBack = trp;
	Invalidate();
}

WORD CCursorGridCtrl::GetSelectionBackTransparency() const
{
	return m_trpSelectionBack;
}

void CCursorGridCtrl::SetSelectionBackTransparency(WORD trp)
{
	if (m_trpSelectionBack == trp)
		return;
	m_trpSelectionBack = trp;
	Invalidate();
}

WORD CCursorGridCtrl::GetSelectionBackTransparency2() const
{
	return m_trpSelectionBack2;
}

void CCursorGridCtrl::SetSelectionBackTransparency2(WORD trp)
{
	if (m_trpSelectionBack2 == trp)
		return;
	m_trpSelectionBack2 = trp;
	Invalidate();
}

WORD CCursorGridCtrl::GetAlternationRowBackTransparency() const
{
	return m_trpAlternationRowBack;
}

void CCursorGridCtrl::SetAlternationRowBackTransparency(WORD trp)
{
	if (m_trpAlternationRowBack == trp)
		return;
	m_trpAlternationRowBack = trp;
	Invalidate();
}

COLORREF CCursorGridCtrl::GetTextColor_head() const
{
	return m_crText_head;
}

void CCursorGridCtrl::SetTextColor_head(COLORREF crTextColor)
{
	if (m_crText_head != crTextColor)
	{
		m_crText_head = crTextColor;
		InvalidateRect(GetGridRect());
	}
}

COLORREF CCursorGridCtrl::GetBackColor_head() const
{
	return m_crBack_head;
}

void CCursorGridCtrl::SetBackColor_head(COLORREF crBackColor)
{
	if (m_crBack_head != crBackColor)
	{
		m_crBack_head = crBackColor;
		InvalidateRect(GetGridRect());
	}
}

WORD CCursorGridCtrl::GetBackTransparency_head() const
{
	return m_trpBack_head;
}

void CCursorGridCtrl::SetBackTransparency_head(WORD trp)
{
	if (m_trpBack_head != trp)
	{
		m_trpBack_head = trp;
		InvalidateRect(GetGridRect());
	}
}

COLORREF CCursorGridCtrl::GetTextColor_tail() const
{
	return m_crText_tail;
}

void CCursorGridCtrl::SetTextColor_tail(COLORREF crTextColor)
{
	if (m_crText_tail != crTextColor)
	{
		m_crText_tail = crTextColor;
		InvalidateRect(GetGridRect());
	}
}

COLORREF CCursorGridCtrl::GetBackColor_tail() const
{
	return m_crBack_tail;
}

void CCursorGridCtrl::SetBackColor_tail(COLORREF crBackColor)
{
	if (m_crBack_tail != crBackColor)
	{
		m_crBack_tail = crBackColor;
		InvalidateRect(GetGridRect());
	}
}

WORD CCursorGridCtrl::GetBackTransparency_tail() const
{
	return m_trpBack_tail;
}

void CCursorGridCtrl::SetBackTransparency_tail(WORD trp)
{
	if (m_trpBack_tail != trp)
	{
		m_trpBack_tail = trp;
		InvalidateRect(GetGridRect());
	}
}

COLORREF CCursorGridCtrl::GetSelectionTextColor() const
{
    return m_crSelectionText;
}

void CCursorGridCtrl::SetSelectionTextColor(COLORREF crTextColor)
{
    if (m_crSelectionText == crTextColor)
        return;
    m_crSelectionText = crTextColor;
    Invalidate();
}

COLORREF CCursorGridCtrl::GetSelectionBackColor() const
{
    return m_crSelectionBack;
}

void CCursorGridCtrl::SetSelectionBackColor(COLORREF crBackColor)
{
    if (m_crSelectionBack == crBackColor)
        return;
    m_crSelectionBack = crBackColor;
    Invalidate();
}

COLORREF CCursorGridCtrl::GetSelectionTextColor2() const
{
    return m_crSelectionText2;
}

void CCursorGridCtrl::SetSelectionTextColor2(COLORREF crTextColor)
{
    if (m_crSelectionText2 == crTextColor)
        return;
    m_crSelectionText2 = crTextColor;
    Invalidate();
}

COLORREF CCursorGridCtrl::GetSelectionBackColor2() const
{
    return m_crSelectionBack2;
}

void CCursorGridCtrl::SetSelectionBackColor2(COLORREF crBackColor)
{
    if (m_crSelectionBack2 == crBackColor)
        return;
    m_crSelectionBack2 = crBackColor;
    Invalidate();
}

COLORREF CCursorGridCtrl::GetAlternationRowBackColor() const
{
    return m_crAlternationRowBack;
}

void CCursorGridCtrl::SetAlternationRowBackColor(COLORREF crBackColor)
{
    if (m_crAlternationRowBack == crBackColor)
        return;
    m_crAlternationRowBack = crBackColor;
	InvalidateRect(GetGridRect());
}

BOOL CCursorGridCtrl::GetUseAlternationRowColor() const
{
    return m_bUseAlternationRowColor;
}

void CCursorGridCtrl::SetUseAlternationRowColor(BOOL bUse /*= TRUE*/)
{
    if (m_bUseAlternationRowColor == bUse)
        return;
    m_bUseAlternationRowColor = bUse;
    Invalidate();
}

void CCursorGridCtrl::OnBeginDrag(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
	CString cs;
	pDataRow->FormatField(m_pColumns->GetIndex(pColumn), cs);
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, cs.GetLength() + 1);
	memcpy(::GlobalLock(hGlobal), LPCSTR(cs), cs.GetLength() + 1);
	::GlobalUnlock(hGlobal);

	COleDataSource DS;
	DS.CacheGlobalData(CF_TEXT, hGlobal);
	DS.DoDragDrop(DROPEFFECT_COPY);

	::GlobalFree(hGlobal);
}

CPoint CCursorGridCtrl::GetContextMenuPosition() const
{
	return m_ptContextMenuPosition;
}

BOOL CCursorGridCtrl::GetEnableDrag() const
{
	return m_bEnableDrag;
}

void CCursorGridCtrl::SetEnableDrag(BOOL bEnableDrag /*= TRUE*/)
{
	m_bEnableDrag = bEnableDrag;
}

BOOL CCursorGridCtrl::GetEnableDrop() const
{
	return m_bEnableDrop;
}

void CCursorGridCtrl::SetEnableDrop(BOOL bEnableDrop /*= TRUE*/)
{
    if (m_bEnableDrop == bEnableDrop)
        return;

	m_bEnableDrop = bEnableDrop;

    if (m_bEnableDrop)
        m_DropTarget.Enable();
    else
        m_DropTarget.Disable();
}

DropTargetView CCursorGridCtrl::GetSelfDropTargetView() const
{
    return m_DropTarget.m_SelfDropTargetView;
}

void CCursorGridCtrl::SetSelfDropTargetView(DropTargetView dropTargetView)
{
    m_DropTarget.m_SelfDropTargetView = dropTargetView;
}

BOOL CCursorGridCtrl::GetShowHeader() const
{
	return m_bShowHeader;
}

void CCursorGridCtrl::SetShowHeader(BOOL bShowHeader)
{
	m_bShowHeader = bShowHeader;
	CalcRowsParams();
	Invalidate();
}

UINT CCursorGridCtrl::GetFixedLeft() const
{
	return m_nFixedLeft;
}

void CCursorGridCtrl::SetFixedLeft(int nFixed)
{
	if (nFixed >= 0 && nFixed != m_nFixedLeft)
	{
		m_nFixedLeft = nFixed;
		RecalcFixedEx();
		Invalidate();
	}
}

UINT CCursorGridCtrl::GetFixedRight() const
{
	return m_nFixedRight;
}

void CCursorGridCtrl::SetFixedRight(int nFixed)
{
	if (nFixed >= 0 && nFixed != m_nFixedRight)
	{
		m_nFixedRight = nFixed;
		RecalcFixedEx();
		Invalidate();
	}
}

void CCursorGridCtrl::SetVerticalLocationOfRect(CRect &rect, CGridColumn* pGridColumn)
{
	rect.top += MulDiv(m_nRowHeight, (pGridColumn->m_nLineNumber-1), m_nColumnsLineCount);
	rect.bottom = rect.top + MulDiv(m_nRowHeight, pGridColumn->m_nColumnLineCount, m_nColumnsLineCount);
}

CRect CCursorGridCtrl::GetCellBounds(RegionType rType, CGridColumn* pGridColumn, int nRowIndex)
{
	CRect rectRegion = GetGridRect(rType);
	CRect rect = GetColumnClipRect(pGridColumn);
	rect.IntersectRect(rect, rectRegion);
	
	SetVerticalLocationOfRect(rect, pGridColumn);

	rect.OffsetRect(0, (nRowIndex - GetTopRowIndex(rType)) * m_nRowHeight);
	rect.IntersectRect(rect, rectRegion);

	return rect;
}

BOOL CCursorGridCtrl::GetCurrentCellBounds(CRect &rect)
{
	BOOL bSuccess = FALSE;
	if (m_nCurrentColumn != -1) 
	{ 
		if (m_nCurrentDataRowIndex != -1 && m_nCurrentDataRowIndex >= m_nTopRowIndex && m_nCurrentDataRowIndex < m_nTopRowIndex + m_nVisibleRows)
		{
			CGridColumn* pGridColumn = m_pColumns->GetAt(m_nCurrentColumn);
			rect = GetCellBounds(rtBody, pGridColumn, m_nCurrentDataRowIndex);
			bSuccess = !rect.IsRectEmpty();
		}
	} 
	return bSuccess; 
}

CDataRowPtr const& CCursorGridCtrl::GetCurrentDataRowPtr() const
{
	if (m_nCurrentDataRowIndex == -1)
		return m_pCurrentDataRowOutOfWindow;

	else
		return m_rows[m_nCurrentDataRowIndex]->GetDataRowPtr();
}

CDataRow* CCursorGridCtrl::GetCurrentDataRow() const
{
	return GetCurrentDataRowPtr().get();
}

CSelectedRows* CCursorGridCtrl::GetSelectedRows()
{
	return &m_SelectedRows;
}

void CCursorGridCtrl::AutoWidth()
{
	for (int i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (!pGridColumn->m_bFixedWidth)
		{
			int nWidth = GetColumnAutoWidth(pGridColumn);
			pGridColumn->SetWidth(nWidth);
		}
	}
}

COLORREF CCursorGridCtrl::aux_ShiftColor(COLORREF cr, int nShift)
{
	int r = GetRValue(cr);
	int g = GetGValue(cr);
	int b = GetBValue(cr);

	if (nShift < 0)
		// darker
		cr =RGB(max(r + nShift, 0), max(g + nShift, 0), max(b + nShift, 0));
	else
		// lighter
		cr =RGB(min(r + nShift, 0xFF), min(g + nShift, 0xFF), min(b + nShift, 0xFF));

	return cr;
}

COLORREF CCursorGridCtrl::GetNegativesColor() const
{
	return m_crNegatives;
}

void CCursorGridCtrl::SetNegativesColor(COLORREF crColor)
{
	if (m_crNegatives == crColor)
		return;
	m_crNegatives = crColor;
	Invalidate();
}

CImageList* CCursorGridCtrl::GetSysImages() const
{
	return m_pSysImages;
}

void CCursorGridCtrl::SetSysImages(CImageList* pSysImages)
{
	if (m_pSysImages == pSysImages)
		return;
	m_pSysImages = pSysImages;
	if (m_pSysImages)
	{
		IMAGEINFO imageInfo;
		if (m_pSysImages->GetImageInfo(0, &imageInfo))
			m_nSysImageWidth = imageInfo.rcImage.right - imageInfo.rcImage.left;
		else
			m_nSysImageWidth = 0;
	}
	InvalidateRect(GetGridRect());
}

COLORREF CCursorGridCtrl::GetHeadersTextColor() const
{
	return m_crHeadersText;
}

void CCursorGridCtrl::SetHeadersTextColor(COLORREF cr)
{
	if (m_crHeadersText == cr)
		return;
	m_crHeadersText = cr;
	InvalidateRect(GetHeaderRect());
}

COLORREF CCursorGridCtrl::GetHeadersBackColor() const
{
	return m_crHeadersBack;
}

void CCursorGridCtrl::SetHeadersBackColor(COLORREF cr)
{
	if (m_crHeadersBack == cr)
		return;
	m_crHeadersBack = cr;
	InvalidateRect(GetHeaderRect());
}

COLORREF CCursorGridCtrl::GetGridBorderColor() const
{
	return m_crGridBorder;
}

void CCursorGridCtrl::SetGridBorderColor(COLORREF cr)
{
	if (m_crGridBorder == cr)
		return;
	m_crGridBorder = cr;
	Invalidate();
}

void CCursorGridCtrl::SetBackText(CString str)
{
	if (m_strBackText == str)
		return;
	m_strBackText = str;
	Invalidate();
}
void CCursorGridCtrl::SetBackTextColor(COLORREF cr)
{
	if (m_crBackText == cr)
		return;
	m_crBackText = cr;
	Invalidate();
}
void CCursorGridCtrl::SetBackTextHA(HBackAlignment ha)
{
	if (m_BackTextHA == ha)
		return;
	m_BackTextHA = ha;
	Invalidate();
}
void CCursorGridCtrl::SetBackTextVA(VBackAlignment va)
{
	if (m_BackTextVA == va)
		return;
	m_BackTextVA = va;
	Invalidate();
}
void CCursorGridCtrl::SetBackTextWB(BOOL bWB)
{
	if (m_bBackTextWB == bWB)
		return;
	m_bBackTextWB = bWB;
	Invalidate();
}
void CCursorGridCtrl::SetBackTextFont(CFont* pFont)
{
	if (m_pBackTextFont == pFont)
		return;
	m_pBackTextFont = pFont;
	Invalidate();
}
void CCursorGridCtrl::ResetBackText()
{
	m_strBackText.Empty();
	m_BackTextHA = hbaCenter;
	m_BackTextVA = vbaCenter;
	m_bBackTextWB = TRUE;
	m_crBackText = CLR_DEFAULT;
	m_pBackTextFont = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CCGDropTarget
/////////////////////////////////////////////////////////////////////////////

CCGDropTarget::CCGDropTarget(CCursorGridCtrl* pGrid) : m_pGrid(pGrid)
{
	m_pDropOverColumn = NULL;
// 	m_pDropOverRow = NULL;
	m_DropOverRegionType = rtNone;
    m_bDragOn = FALSE;
    m_dwDropEffect = DROPEFFECT_NONE;
    m_SelfDropTargetView = dtvNone;
	m_bScrollDelay = FALSE;
}

CCGDropTarget::~CCGDropTarget()
{
	ReleaseDropOverRow();
}

void CCGDropTarget::Enable()
{
    Register(m_pGrid);
}

void CCGDropTarget::Disable()
{
    Revoke();
}

DROPEFFECT CCGDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	m_bDragOn = TRUE;
	m_dwKeyState = dwKeyState;
	return OnDragOver(pWnd, pDataObject, dwKeyState, point);
}

void CCGDropTarget::OnDragLeave(CWnd* pWnd)
{
    m_bDragOn = FALSE;
    m_pDropOverColumn = NULL;
	m_bScrollDelay = FALSE;
	ReleaseDropOverRow();

    CRect rect;
    rect.SetRectEmpty();
    DrawDragRect(rect);
}

DROPEFFECT CCGDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CGridColumn* pColumn = NULL;
    CDataRowPtr pDataRow;
    BOOL bDropEnable = FALSE;

	int nColumn, nRow, nImageIndex;
	RegionType rType;

	CRect ClientRect;
	m_pGrid->GetClientRectEx(ClientRect);
	if (!ClientRect.PtInRect(point))
		m_dwDropEffect = DROPEFFECT_NONE;
	else
	{
		switch (m_pGrid->HitTest(point, nColumn, nRow, rType, nImageIndex))
		{
		case ghtCell:
		case ghtCheckBox:
		case ghtImage:
		case ghtSysImage:
		case ghtSpace:
			if (nColumn != -1)
				pColumn = m_pGrid->m_pColumns->GetAt(nColumn);

			if (nRow != -1)
				pDataRow = m_pGrid->GetGridRow(rType, nRow)->GetDataRowPtr();

			bDropEnable = TRUE;
			break;
		default:
			m_dwDropEffect = DROPEFFECT_NONE;
		}
	}

	if (m_pDropOverColumn != pColumn || !CompareRows(m_pDropOverRow.get(), m_DropOverRegionType, pDataRow.get(), rType))
	{
		SetDropOverRow(pDataRow, rType);
		m_pDropOverColumn = pColumn;

		DropTargetView dropTargetView = m_SelfDropTargetView == dtvNone ? dtvCell : m_SelfDropTargetView;

		if (bDropEnable)
        {
            switch (m_SelfDropTargetView)
            {
            case dtvCell:
				if (m_pDropOverColumn == m_pGrid->m_pDragColumn
					&& CompareRows(m_pDropOverRow.get(), m_DropOverRegionType, m_pGrid->m_pDragRow.get(), m_pGrid->m_DragRegionType))
				{
					dropTargetView = dtvNone;
					bDropEnable = FALSE;
					m_dwDropEffect = DROPEFFECT_NONE;
				}
                break;
            case dtvRow:
                if (m_pGrid->m_nCurrentDataRowIndex != -1)
					if (CompareRows(m_pDropOverRow.get(), m_DropOverRegionType, m_pGrid->m_pDragRow.get(), m_pGrid->m_DragRegionType))
                    {
                        dropTargetView = dtvNone;
                        bDropEnable = FALSE;
                        m_dwDropEffect = DROPEFFECT_NONE;
                    }
                break;
            case dtvColumn:
				if (m_pDropOverColumn == m_pGrid->m_pDragColumn)
                {
                    dropTargetView = dtvNone;
                    bDropEnable = FALSE;
                    m_dwDropEffect = DROPEFFECT_NONE;
                }
                break;
            }

            if (bDropEnable)
			{
    	        m_dwDropEffect = m_pGrid->OnDragOver(pDataObject, dwKeyState, m_pDropOverRow.get(), m_pDropOverColumn, m_DropOverRegionType, dropTargetView);
				ResetScroll();
			}
        }

		CRect rect;
		nRow -= m_pGrid->GetTopRowIndex(rType);
		switch (dropTargetView)
		{
		case dtvCell:
			if (m_pDropOverColumn && m_pDropOverRow)
			{
				rect.IntersectRect(m_pGrid->GetHeaderRect(), m_pGrid->GetColumnClipRect(m_pDropOverColumn));
				rect.right--;
				rect.bottom--;
				rect.OffsetRect(0, m_pGrid->GetGridRect(rType).top + nRow * m_pGrid->m_nRowHeight);
			}
			else
				rect.SetRectEmpty();
			break;
		case dtvRow:
			if (m_pDropOverRow)
			{
				rect = m_pGrid->GetGridRect(rType);
				rect.bottom = rect.top + m_pGrid->m_nRowHeight - 1;
				rect.OffsetRect(0, nRow * m_pGrid->m_nRowHeight);
			}
			else
				rect.SetRectEmpty();
			break;
		case dtvColumn:
			if (m_pDropOverColumn)
			{
 				CRect GridRect = m_pGrid->GetGridRect();
				rect = m_pGrid->GetColumnClipRect(m_pDropOverColumn);
 				rect.top = GridRect.top;
 				rect.bottom = GridRect.bottom;
				rect.right--;
				rect.bottom--;
			}
			else
				rect.SetRectEmpty();
			break;
		default:
			rect.SetRectEmpty();
		}
		DrawDragRect(rect);
	}
    return m_dwDropEffect;
}

DROPEFFECT CCGDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
    CRect rect;
    rect.SetRectEmpty();
    DrawDragRect(rect);

	if (m_dwKeyState & MK_RBUTTON)
	{
		CMenu menu;
		menu.CreatePopupMenu();
		if (dropList & DROPEFFECT_MOVE)
			menu.AppendMenu(MF_STRING, DROPEFFECT_MOVE, "Перенести");
		if (dropList & DROPEFFECT_COPY)
			menu.AppendMenu(MF_STRING, DROPEFFECT_COPY, "Скопировать");
		if (dropList & DROPEFFECT_LINK)
			menu.AppendMenu(MF_STRING, DROPEFFECT_LINK, "Связать");

		if (menu.GetMenuItemCount() == 0)
			dropDefault = DROPEFFECT_NONE;
		else
		{
			menu.AppendMenu(MF_SEPARATOR, -1, "");
			menu.AppendMenu(MF_STRING, DROPEFFECT_NONE, "Отмена");

			menu.SetDefaultItem(dropDefault);
			m_pGrid->ClientToScreen(&point);

			dropDefault = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, point.x, point.y, m_pGrid);
		}
	}

    m_pGrid->OnDrop(pDataObject, dropDefault, m_pDropOverRow.get(), m_pDropOverColumn, m_DropOverRegionType);
    
	ReleaseDropOverRow();
    m_pDropOverColumn = NULL;

    return dropDefault;
}

void CCGDropTarget::DrawDragRect(CRect& rect)
{
    m_rectTarget = rect;
    m_pGrid->InvalidateRect(m_pGrid->GetGridRect());
}

BOOL CCGDropTarget::CompareRows(CDataRow* pRow1, RegionType rType1, CDataRow* pRow2, RegionType rType2)
{
	BOOL bEQ = FALSE;
	if (pRow1 && pRow2)
	{
		if (rType1 == rType2)
		{
			CDataProvider* pProvider = m_pGrid->GetDataProvider(rType1);
			bEQ = pProvider->CompareRows(pRow1, pRow2);
		}
	}
	else
		bEQ = !pRow1 && !pRow2;

	return bEQ;
}

DROPEFFECT CCGDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	CRect rectWindow = m_pGrid->GetGridRect();
// 	m_pGrid->GetWindowRect(rectWindow);
// 	m_pGrid->ClientToScreen(&point);
	
	int nVScroll = 0;
	int nHScroll = 0;

	if (point.y <= rectWindow.top + DRAG_SCROLL_FIELD_WIDTH)
		nVScroll = 1;
	else if (point.y >= rectWindow.bottom - DRAG_SCROLL_FIELD_WIDTH)
		nVScroll = -1;

	if (point.x <= rectWindow.left + DRAG_SCROLL_FIELD_WIDTH)
		nHScroll = -1;
	else if (point.x >= rectWindow.right - DRAG_SCROLL_FIELD_WIDTH)
		nHScroll = 1;

	if (nVScroll || nHScroll)
	{
		if (m_bScrollDelay)
		{
			if (::GetTickCount() >= m_dwScrollStart)
			{
				BOOL bScrolled = FALSE;

				if (nVScroll > 0)
					bScrolled = m_pGrid->ScrollUp(nVScroll);
				else if (nVScroll < 0)
					bScrolled = m_pGrid->ScrollDown(-nVScroll);

				if (nHScroll)
				{
					SCROLLINFO si;
					m_pGrid->GetScrollInfo(SB_HORZ, &si);
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS;
					bScrolled |= m_pGrid->HScrollLine(si, nHScroll);
				}

				if (bScrolled)
				{
					ReleaseDropOverRow();
					m_pDropOverColumn = NULL;
					CRect rect;
					rect.SetRectEmpty();
					DrawDragRect(rect);
					return DROPEFFECT_SCROLL;
				}
			}
		}
		else
		{
			ResetScroll();
			m_bScrollDelay = TRUE;
		}
	}
	else
		m_bScrollDelay = FALSE;

	return DROPEFFECT_NONE;
}

void CCGDropTarget::ResetScroll()
{
	m_dwScrollStart = ::GetTickCount() + DRAG_SCROLL_DELAY;
}

void CCGDropTarget::SetDropOverRow(CDataRowPtr const& pRow, RegionType rType)
{
	m_pDropOverRow = pRow;
	m_DropOverRegionType = rType;
}
