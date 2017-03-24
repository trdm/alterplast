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

#define COLUMN_HELPER_CX (16 + 1 + 1)
#define COLUMN_HELPER_CY (16 + 1 + 1)
#define COLUMN_HELPER_MARGIN 8
#define COLUMN_HELPER_MINROWWIDTH 70

#define TIMER_ID_AUTOHIDE 0x00000002
#define HELPER_DISPLAY_TIMEOUT 900

#define TIMER_ID_SCROLL 0x00000003
#define SCROLL_TIMEOUT 100

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
    m_pFont = NULL;
    m_pHeaderFont = NULL;
    m_pRowsImages = NULL;
    m_nImageWidth = 0;
    m_bFixedWidth = FALSE;
    m_crHeaderText = CLR_DEFAULT;
    m_crHeaderBack = CLR_DEFAULT;
    m_OrderMarker = omNone;
    m_bChangeVisible = TRUE;
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
        if (pGridColumns->m_pGrid)
        {
            CCursorGridCtrl* pGrid = pGridColumns->m_pGrid;
            if (bRefill)
                pGrid->FillRows(pGrid->m_nTopRowIndex, pGrid->m_nVisibleRows);
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
    if (m_nMinWidth < 0)
        m_nMinWidth = 0;
    if (m_nMinWidth > 10000)
        m_nMinWidth = 10000;
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

LPCSTR CGridColumn::GetDataField() const
{
    return m_strDataField;
}

void CGridColumn::SetDataField(LPCSTR szDataField)
{
    if (!stricmp(m_strDataField, szDataField))
        return;

    CGridColumns* pGridColumns = static_cast<CGridColumns*>(GetCollection());
    
    if (m_bVisible && pGridColumns)
        pGridColumns->ReleaseDataField(this);

    m_strDataField = szDataField;

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
    m_pGrid->FillRows(m_pGrid->m_nTopRowIndex, m_pGrid->m_nVisibleRows);
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
            delete pGridRow->m_cells[nIndexWas];
            pGridRow->m_cells.RemoveAt(nIndexWas);
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
    CGridColumn* pGridColumn = NewColumn();
    pGridColumn->SetName(szName);
    CCollection::Add(pGridColumn);
    return pGridColumn;
}

CGridColumn* CGridColumns::Insert(int nIndex, LPCSTR szName)
{
    if (nIndex < 0 || nIndex > GetCount())
        return NULL;

    CGridColumn* pGridColumn = NewColumn();
    pGridColumn->SetName(szName);
    CCollection::Insert(nIndex, pGridColumn);
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
    m_nMinWidth = 0;
    m_nResizableMinWidth = 0;
    for (int i = 0; i < GetCount(); i++)
    {
        CGridColumn* pColumn = static_cast<CGridColumn*>(GetItem(i));
        if (pColumn->m_bVisible)
        {
            m_nMinWidth += pColumn->m_nMinWidth;
            if (!pColumn->m_bFixedWidth)
                m_nResizableMinWidth += pColumn->m_nMinWidth;
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
	if (pProvider && !pColumn->m_strDataField.IsEmpty())
		pProvider->QueryField(pColumn->m_strDataField);
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
	if (pProvider && !pColumn->m_strDataField.IsEmpty())
		pProvider->ReleaseField(pColumn->m_strDataField);
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
    m_pGrid->RecalcColumnsWidth();
    return TRUE;
}

int CGridColumns::GetVisibleWidth() const
{
	int nWidth = 0;
	for (int i = 0; i < GetSize(); i++)
	{
		CGridColumn* pGridColumn = GetAt(i);
		if (pGridColumn->m_bVisible)
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

#define CGI_TEXT_H_ALIGN        0x0018
#define CGI_TEXT_AUTO           0x0000
#define CGI_TEXT_LEFT           0x0008
#define CGI_TEXT_RIGHT          0x0010
#define CGI_TEXT_CENTER         0x0018

#define CGI_CHECKBOX_STATE      0x0060
#define CGI_CHECKBOX_UNCHECKED  0x0000
#define CGI_CHECKBOX_CHECKED    0x0020
#define CGI_CHECKBOX_UNDEFINED  0x0040

CGridItem::CGridItem(CGridColumn* pGridColumn) : m_pGridColumn(pGridColumn)
{
    m_wFlagsMask = 0;
    m_nImageNumber = -1;

    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
    m_pFont = NULL;
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
    return m_wFlagsMask & CGI_TEXT != 0;
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
    return m_wFlagsMask & CGI_IMAGE != 0;
}

void CGridItem::SetShowImage(BOOL bEnable /*= TRUE*/)
{
    if (bEnable)
        m_wFlagsMask |= CGI_IMAGE;
    else
        m_wFlagsMask &= ~CGI_IMAGE;
}

BOOL CGridItem::GetShowCheckBox() const
{
    return m_wFlagsMask & CGI_CHECKBOX != 0;
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

int CGridItem::GetImageIndex() const
{
    return m_nImageNumber;
}

void CGridItem::SetImageIndex(int nImageIndex)
{
    m_nImageNumber = nImageIndex;
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

CFont* CGridItem::GetFont() const
{
    return m_pFont;
}

void CGridItem::SetFont(CFont* pFont)
{
    m_pFont = pFont;
}

/////////////////////////////////////////////////////////////////////////////
// CGridRow
/////////////////////////////////////////////////////////////////////////////

CGridRow::CGridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rType)
    : m_pColumns(pColumns), m_pDataRow(pDataRow), m_rType(rType)
{
    m_bIsCurrent = false;

    m_crText = CLR_DEFAULT;
    m_crBack = CLR_DEFAULT;
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
    else
        m_pDataRow->Release();
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
    m_pFont = NULL;

    return TRUE;
}

void CGridRow::FillItems()
{
    int i;
    CleanUp();
    for (i = 0; i < m_pColumns->GetSize(); i++)
        m_cells.Add(NewItem(m_pColumns->GetAt(i)));

    for (i = 0; i < m_cells.GetSize(); i++)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
        if (!pColumn->m_bVisible)
            continue;
        if (pColumn->m_strDataField.IsEmpty())
            continue;
        int nIndex = m_pDataRow->GetFieldIndexOf(pColumn->m_strDataField);
        if (nIndex == -1)
            continue;

        CGridItem* pGridItem = m_cells[i];
        HorizTextAlignment HAlign = htaAuto;
        switch (m_pDataRow->GetFieldType(nIndex))
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
        if (pGridItem->GetTextHAlign() == htaAuto)
        {
            if (pColumn->m_HTextAlignment != htaAuto)
                pGridItem->SetTextHAlign(pColumn->m_HTextAlignment);
            else
                pGridItem->SetTextHAlign(HAlign);
        }
    }
	OnFillItems();
}

CCursorGridCtrl* CGridRow::GetGrid() const
{
    return m_pColumns->m_pGrid;
}

CDataRow* CGridRow::GetDataRow() const
{
    return m_pDataRow;
}

CGridItem* CGridRow::GetGridItem(int nIndex) const
{
    return m_cells[nIndex];
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

	m_pDataRow = pGrid->GetCurrentDataRow();
	m_nDataRowIndex = pGrid->m_nCurrentDataRowIndex;
	m_nDataRowDirection = pGrid->m_nCurrentDataRowDirection;
	m_nSelectDirection = pSelectedRows->GetDirection();
	m_nSize = pSelectedRows->GetSize();
	if (m_pDataRow)
	{
		m_pDataRow->AddRef();
		m_bIsRemoved = pSelectedRows->IsRemoved(m_pDataRow);
		m_bIsCurrent = pSelectedRows->IsCurrent(m_pDataRow);
	}
	else
	{
		m_bIsRemoved = FALSE;
		m_bIsCurrent = FALSE;
	}
}

CDataRowState::~CDataRowState()
{
	if (m_pDataRow)
		m_pDataRow->Release();
}

/////////////////////////////////////////////////////////////////////////////
// CSelectedRows
/////////////////////////////////////////////////////////////////////////////

CSelectedRows::CSelectedRows() : m_pProvider(NULL), m_pRemovedRow(NULL), m_pCurrentRow(NULL), m_nDirection(0)
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

BOOL CSelectedRows::IsSelected(CDataRow* pDataRow) const
{
	for (int i = 0; i < m_rows.GetSize(); i++)
	{
		if (m_pProvider->CompareRows(pDataRow, m_rows[i]))
			return TRUE;
	}

	return FALSE;
}

BOOL CSelectedRows::IsRemoved(CDataRow* pDataRow) const
{
	BOOL bSuccess = FALSE;
	
	if (m_pRemovedRow)
		bSuccess = m_pProvider->CompareRows(pDataRow, m_pRemovedRow);

	return bSuccess;
}

BOOL CSelectedRows::IsCurrent(CDataRow* pDataRow) const
{
	BOOL bSuccess = FALSE;
	
	if (m_pCurrentRow)
		bSuccess = m_pProvider->CompareRows(pDataRow, m_pCurrentRow);

	return bSuccess;
}

void CSelectedRows::Add(CDataRow* pDataRow, BOOL bCheckIsSelected)
{
	if (!bCheckIsSelected || !IsSelected(pDataRow))
	{
		pDataRow->AddRef();
		m_rows.Add(pDataRow);
	}
	SetRemovedRow(NULL);
}

BOOL CSelectedRows::Remove(CDataRow* pDataRow)
{
	BOOL bSuccess = FALSE;

	for (int i = m_rows.GetUpperBound(); i >= 0; i--)
	{
		CDataRow* pSelectedRow = m_rows[i];
		if (m_pProvider->CompareRows(pDataRow, pSelectedRow))
		{
			pSelectedRow->Release();
			m_rows.RemoveAt(i);
			bSuccess = TRUE;
		}
	}

	if (bSuccess)
		SetRemovedRow(pDataRow);

	return bSuccess;
}

void CSelectedRows::Toggle(CDataRow* pDataRow)
{
	if (!Remove(pDataRow))
		Add(pDataRow, FALSE);
}

void CSelectedRows::RemoveAll()
{
	for (int i = 0; i < m_rows.GetSize(); i++)
		m_rows[i]->Release();

	m_rows.RemoveAll();
	SetRemovedRow(NULL);
	SetCurrentRow(NULL);
	SetDirection(0);
}

void CSelectedRows::SetRemovedRow(CDataRow* pDataRow)
{
	if (m_pRemovedRow)
		m_pRemovedRow->Release();

	m_pRemovedRow = pDataRow;

	if (m_pRemovedRow)
		m_pRemovedRow->AddRef();
}

void CSelectedRows::SetCurrentRow(CDataRow* pDataRow)
{
	if (m_pCurrentRow)
		m_pCurrentRow->Release();

	m_pCurrentRow = pDataRow;

	if (m_pCurrentRow)
		m_pCurrentRow->AddRef();
}

int CSelectedRows::GetSize() const
{
	return m_rows.GetSize();
}

CDataRow* CSelectedRows::GetAt(int nIndex) const
{
	CDataRow* pDataRow = NULL;
	if (nIndex >= 0 && nIndex < m_rows.GetSize())
		pDataRow = m_rows[nIndex];

	return pDataRow;
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
                    CFont* pFont, BOOL bByCursor)
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
    CSize size = dc.GetTextExtent(strText);

    if (size.cx > TextRect.Width() - m_pGrid->GetLeftTextMargin() - m_pGrid->GetRightTextMargin() - 1)
    {
//        dc.SelectStockObject(ANSI_VAR_FONT);

        CRect rectDisplay;
        
        CSize TextSize = dc.GetTextExtent(strText);
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
        dc.TextOut(m_pGrid->GetLeftTextMargin() + 1, m_pGrid->GetTopTextMargin() + 1, strText);

        SetCapture();

        SetTimer(TIMER_ID_AUTOHIDE, TIP_TIMEOUT, NULL);
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

void CReposColumnWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    HideArrows();
    ReleaseCapture();
    DestroyWindow();
    if (m_nShift && m_pGrid->OnColumnMove(m_pCapturedColumn, m_nShift))
        m_pGrid->m_pColumns->Move(m_nCapturedIndex, m_nShift);

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
                m_pGrid->GetClientRect(rect);
                point.x = rect.left - 8;
                point.y = rect.top - 16;
            }
            else
            {
                CRect rect = m_pGrid->GetColumnClipRect(m_pGrid->m_pColumns->GetAt(nIndex));
                point.x = rect.left - 8;
                point.y = rect.top - 16;
            }
        }
        else
        {
            CRect rect = m_pGrid->GetColumnClipRect(m_pGrid->m_pColumns->GetAt(nIndex));
            point.x = rect.right - 8;
            point.y = rect.top - 16;
        }
    }
    else
    {
        CRect rect = m_pGrid->GetColumnClipRect(pColumn);
        if (nShift > 0)
            point.x = rect.right - 8;
        else
            point.x = rect.left - 8;
        point.y = rect.top - 16;
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
        point.Offset(0, 16 + m_pGrid->m_nRowHeight);
        m_pGrid->m_ilInternalImages.Draw(pDC, iiBottomReposArrow, point, ILD_NORMAL);
        m_pGrid->GetParent()->ReleaseDC(pDC);
    }
    else
        HideArrows();

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
        cy = m_pGrid->m_nRowHeight;
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
    rectHeader.bottom = m_pGrid->m_nRowHeight;

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

        rectHeader.OffsetRect(0, m_pGrid->m_nRowHeight);
    }
    dc.SelectObject(OldFont);
}

void CColumnHelper::OnLButtonDown(UINT nFlags, CPoint point)
{
    int nIndex = point.y / m_pGrid->m_nRowHeight;
    CGridColumn* pColumn = m_columns[nIndex];
    CRect rect;
    GetClientRect(rect);
	CSize size(point.x - (rect.Width() - GetColumnWidth(pColumn)), point.y % m_pGrid->m_nRowHeight);
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
            cy += m_pGrid->m_nRowHeight;
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
        bSuccess = rgn.CreateRectRgn(cx - GetColumnWidth(m_columns[i]), CurY - 1, cx, CurY + m_pGrid->m_nRowHeight);
        int nRetVal = rgnWindow.CombineRgn(&rgnWindow, &rgn, RGN_OR);

        CurY += m_pGrid->m_nRowHeight;
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
// CCursorGridCtrl window
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCursorGridCtrl, CWnd)

CCursorGridCtrl::CCursorGridCtrl(): m_Tip(this), m_QuickSearch(this), m_ReposColumnWnd(this), m_ColumnHelper(this),
    m_DropTarget(this)
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
    m_pCurrentDataRowOutOfWindow = NULL;
    m_RowSelectMode = rsmCell;
    m_SelectMode = smSingleLine;
    m_bHorizontalLines = TRUE;
    m_bVerticalLines = TRUE;
    m_StartView = svFromBegin;
    m_bAutoDelete = FALSE;
    m_HeadersStyle = hs3D;
	m_ActiveHeaderStyle = ahsSelected;
    BOOL bSuccess;
    bSuccess = m_ilInternalImages.Attach(ImageList_LoadBitmap(_Module.m_hInst, (LPCTSTR)IDB_CG_IMAGES, 16, 0, RGB(0x00, 0x80, 0x80)));
    ASSERT(bSuccess);
    m_nLastLParam = ULONG_MAX;
    m_nRefreshTimeout = 0;
    m_crGridLines = CLR_DEFAULT;
    m_bChangePositionOfColumns = TRUE;
    m_crText = CLR_DEFAULT;
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

	m_nDrawLocks = 0;
	m_bDrawReq = FALSE;
	m_nUpdateLocks = 0;
	m_bUpdateReq = FALSE;

	m_ptContextMenuPosition = CPoint(0,0);
}

BOOL CCursorGridCtrl::Create(const RECT& rect, CWnd *pParentWnd, UINT nID, BOOL bAutoDelete /*= FALSE*/, BOOL bVisible /*= TRUE*/, BOOL bDisabled /*= FALSE*/)
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
    {
        m_pCurrentDataRowOutOfWindow->Release();
        m_pCurrentDataRowOutOfWindow = NULL;
    }
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

    case WM_KEYDOWN:
        switch (pMsg->wParam)
        {
        case VK_TAB:
            {
                if (!IS_CTRL_PRESSED())
                {
                    int nIndex = GetNextColumn(m_nCurrentColumn, IS_SHIFT_PRESSED() ? -1 : 1);
                    if (nIndex != -1)
                    {
                        SetCurrentCell(m_nCurrentDataRowIndex, nIndex);
                        return TRUE;
                    }
                }
            }
            break;
        case VK_RETURN:
			{
				CDataRow* pDataRow = GetCurrentDataRow();
				if (pDataRow)
					OnSelection(pDataRow, m_pColumns->GetAt(m_nCurrentColumn), rtBody);
			}
            return TRUE;
		case VK_SPACE:
			if (IS_CTRL_PRESSED())
			{
				CDataRowState st(this);
				SelectRow(st);
			}
			return TRUE;
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
	
	InitFont(GetParent()->GetFont());
	return 0;
}

void CCursorGridCtrl::InitFont(CFont* pFont)
{
    BOOL bSuccess;
    int nRetVal;

    LOGFONT lf;
    nRetVal = pFont->GetLogFont(&lf);
    ASSERT(nRetVal);
    bSuccess = m_font.CreateFontIndirect(&lf);
    ASSERT(bSuccess);

    CDC *pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(pFont);

    m_nFontHeight = pDC->GetTextExtent("^j").cy;

    pDC->SelectObject(pOldFont);
    nRetVal = ReleaseDC(pDC);

    ASSERT(nRetVal);

    m_nRowHeight = m_nFontHeight + GetTopTextMargin() + GetBottomTextMargin();
    RecalcColumnsWidth();
    CalcRowsParams();
}

void CCursorGridCtrl::PreSubclassWindow() 
{
    m_pColumns = NewColumns();
    m_pColumns->m_pGrid = this;

    BOOL bSuccess;
    
    if (::IsWindow(m_hWnd))
    {
        bSuccess = ModifyStyleEx(0, WS_EX_CLIENTEDGE);
        ASSERT(bSuccess);
        
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
    if (m_bShowHeader && pDC->RectVisible(GetHeaderRect()))
        PaintHeaders(pDC);

	CRect rect = GetGridRect();
    if (pDC->RectVisible(rect))
    {
		COLORREF crBack = m_crBack != CLR_DEFAULT ? m_crBack : ::GetSysColor(COLOR_WINDOW);
		CBrush br(crBack);
		CBrush* pOldBrush = pDC->SelectObject(&br);
		pDC->FillRect(&rect, &br);
		pDC->SelectObject(pOldBrush);

        PaintCells(pDC);

        if (m_bHorizontalLines || m_bVerticalLines)
            PaintGridLines(pDC);
        
        if (m_pProvider)
            PaintError(pDC);

        if (m_DropTarget.m_bDragOn)
            PaintDropTarget(pDC);
    }
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

CRect CCursorGridCtrl::GetHeaderRect() const
{
    CRect rect;
    
    GetClientRect(rect);
    rect.bottom = rect.top;
    
    if (m_bShowHeader)
        rect.bottom += GetHeaderHeight();
    
    return rect;
}

int CCursorGridCtrl::GetHeaderHeight() const
{
    return m_nFontHeight + GetTopTextMargin() + GetBottomTextMargin();
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
	int nStrokeCount = 1;

    for (int i = 0; i < m_pColumns->GetSize(); ++i)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			CRect Rect, clipRect;
			Rect.IntersectRect(GetColumnBoundsRect(pColumn), HeaderRect);
			clipRect.IntersectRect(GetColumnClipRect(pColumn), HeaderRect);
			if (clipRect.right > HeaderRect.left && clipRect.left < HeaderRect.right)
			{
				DrawHeader(pDC, pColumn, Rect, clipRect);
				nStrokeCount++;
			}
		}
    }

	pDC->SelectObject(pOldFont);
    
    if (m_HeadersStyle == hsFlat)
    {
        int nPointCount = nStrokeCount * 2;
        LPPOINT lpPoints = new POINT[nPointCount];
        LPDWORD lpPolyPoints = new DWORD[nStrokeCount];

        for (i = 0; i < nStrokeCount; i ++)
            lpPolyPoints[i] = 2;

        lpPoints[0].x = HeaderRect.left;
        lpPoints[0].y = HeaderRect.bottom - 1;
        lpPoints[1].x = HeaderRect.right;
        lpPoints[1].y = HeaderRect.bottom - 1;

        LPPOINT lpCurPoints = lpPoints + 2;

        for (int i = 0; i < m_pColumns->GetSize(); ++i)
        {
            CGridColumn* pColumn = m_pColumns->GetAt(i);
            if (pColumn->m_bVisible)
			{
				CRect clipRect = GetColumnClipRect(pColumn);
				if (clipRect.right > HeaderRect.left && clipRect.left < HeaderRect.right)
				{
					lpCurPoints[0].x = clipRect.right - 1;
					lpCurPoints[0].y = clipRect.top;
					lpCurPoints[1].x = clipRect.right - 1;
					lpCurPoints[1].y = clipRect.bottom;

					lpCurPoints += 2;
				}
			}
        }
    
        BOOL bSuccess;
        CPen *pOldPen;
        CPen penGridLines;
        penGridLines.CreatePen(PS_SOLID, 1, GetLinesColor());
        pOldPen = pDC->SelectObject(&penGridLines);
    
        bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
        ASSERT(bSuccess);
    
        pOldPen = pDC->SelectObject(pOldPen);

        delete[] lpPoints;
        delete[] lpPolyPoints;
    }
}

void CCursorGridCtrl::PaintCells(CDC *pDC)
{
	CRect rect = GetGridRect(rtHead);
	rect.bottom = rect.top + m_nRowHeight;

    int i;
    struct stColInfo
    {
        int nIndex;
        CRect rect;
		CRect clipRect;
    } *pColInfo = new stColInfo[m_pColumns->GetSize()];
    
    stColInfo* pCurInfo = pColInfo;
    for (i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
        if (pColumn->m_bVisible)
		{
			CRect clipRect = GetColumnClipRect(pColumn);
			if (clipRect.right > rect.left && clipRect.left < rect.right)
			{
				CRect colRect = GetColumnBoundsRect(pColumn);
				colRect.top = rect.top;
				colRect.bottom = rect.bottom;

				pCurInfo->nIndex = i;
				pCurInfo->rect = colRect;
				pCurInfo->clipRect.IntersectRect(clipRect, rect);
				pCurInfo++;
			}
		}
    }
	stColInfo* pEndInfo = pCurInfo;
    
	COLORREF crText, crBack;
	CFont* pFont;

    BOOL bIsFocused = IsFocused();
	for (i = 0; i < GetFullVisibleRows(rtHead); i++)
	{
		if (pDC->RectVisible(rect))
		{
			CGridRow* pGridRow = m_rows_head[i];
			CDataRow* pDataRow = pGridRow->GetDataRow();
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				CGridItem* pGridItem = pGridRow->m_cells[pCurInfo->nIndex];
				pFont = EvalCellFont(pGridItem, pGridRow);
				EvalCellColors_head(pGridItem, pGridRow, crText, crBack);
				DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridItem, crText, crBack, pFont);
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
		rect.OffsetRect(0, m_nRowHeight);
	}

	rect = GetGridRect(rtBody);
	rect.bottom = rect.top + m_nRowHeight;
	for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
	{
		pCurInfo->rect.top = rect.top;
		pCurInfo->rect.bottom = rect.bottom;
		pCurInfo->clipRect.top = rect.top;
		pCurInfo->clipRect.bottom = rect.bottom;
	}

	int nTopRowIndex = GetTopRowIndex(rtBody);
	for (i = nTopRowIndex; i < nTopRowIndex + GetFullVisibleRows(rtBody); i++)
    {
        if (pDC->RectVisible(rect))
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

				if (bIsSelected)
					bSelected = bIsSelected;
				else if (m_nCurrentColumn == pCurInfo->nIndex)
					bSelected = bIsCurrent;
				else
				{
					if (m_RowSelectMode == rsmRow)
						bSelected = bIsCurrent;
					else if (m_RowSelectMode == rsmBoth)
						bAlterSelected = bIsCurrent;
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
				EvalCellColors(pGridItem, pGridRow, bSelected, bAlterSelected, bAlterBack, crText, crBack);
                DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridRow->m_cells[pCurInfo->nIndex], crText, crBack, pFont);

                if (bIsFocused && m_RowSelectMode != rsmRow && pGridRow->m_bIsCurrent && m_nCurrentColumn == pCurInfo->nIndex)
                {
                    CRect Rect;
					Rect.IntersectRect(&pCurInfo->rect, &pCurInfo->clipRect);
                    Rect.right--;
                    Rect.bottom--;
                    pDC->DrawFocusRect(Rect);
                }
                ::OffsetRect(&pCurInfo->rect, 0, m_nRowHeight);
				::OffsetRect(&pCurInfo->clipRect, 0, m_nRowHeight);
            }
            if (bIsFocused && m_RowSelectMode == rsmRow && pGridRow->m_bIsCurrent)
            {
                CRect Rect(rect);
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
        rect.OffsetRect(0, m_nRowHeight);
    }

	rect = GetGridRect(rtTail);
	rect.bottom = rect.top + m_nRowHeight;
	for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
	{
		pCurInfo->rect.top = rect.top;
		pCurInfo->rect.bottom = rect.bottom;
		pCurInfo->clipRect.top = rect.top;
		pCurInfo->clipRect.bottom = rect.bottom;
	}

	nTopRowIndex = m_rows_tail.GetSize() - GetFullVisibleRows(rtTail);
	for (i = nTopRowIndex; i < m_rows_tail.GetSize(); i++)
	{
		if (pDC->RectVisible(rect))
		{
			CGridRow* pGridRow = m_rows_tail[i];
			CDataRow* pDataRow = pGridRow->GetDataRow();
			for (pCurInfo = pColInfo; pCurInfo != pEndInfo; pCurInfo++)
			{
				CGridItem* pGridItem = pGridRow->m_cells[pCurInfo->nIndex];
				pFont = EvalCellFont(pGridItem, pGridRow);
				EvalCellColors_tail(pGridItem, pGridRow, crText, crBack);
				DrawCell(pDC, &pCurInfo->rect, &pCurInfo->clipRect, pGridItem, crText, crBack, pFont);
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
		rect.OffsetRect(0, m_nRowHeight);
	}

	if ((!m_pColumns->GetSize() || m_rows.GetSize() == 0) && IsFocused())
	{
		CRect rect = GetGridRect(rtBody);
		rect.bottom = rect.top + m_nRowHeight - 1;
		pDC->DrawFocusRect(rect);
	}

	delete[] pColInfo;
}

void CCursorGridCtrl::EvalCellColors(CGridItem* pGridItem, CGridRow* pGridRow, BOOL bSelected, BOOL bAlterSelected, BOOL bAlterBack, COLORREF& crText, COLORREF& crBack)
{
	if (bSelected)
	{
		if (m_crSelectionBack != CLR_DEFAULT)
			crBack = m_crSelectionBack;
		else
			crBack = ::GetSysColor(COLOR_HIGHLIGHT);

		if (m_crSelectionText != CLR_DEFAULT)
			crText = m_crSelectionText;
		else
			crText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else
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

		if (pGridItem->m_crText != CLR_DEFAULT)
			crText = pGridItem->m_crText;
		else if (pGridRow->m_crText != CLR_DEFAULT)
			crText = pGridRow->m_crText;
		else if (pGridItem->m_pGridColumn->m_crText != CLR_DEFAULT)
			crText = pGridItem->m_pGridColumn->m_crText;
		else if (m_crText != CLR_DEFAULT)
			crText = m_crText;
		else
			crText = ::GetSysColor(COLOR_WINDOWTEXT);

	}
	if (bAlterSelected)
	{
		if (m_crSelectionBack2 != CLR_DEFAULT)
			crBack = m_crSelectionBack2;
		else
			crBack = aux_ShiftColor(crBack, -32);

		if (m_crSelectionText2 != CLR_DEFAULT)
			crText = m_crSelectionText2;
	}
}

void CCursorGridCtrl::EvalCellColors_head(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack)
{
	if (pGridItem->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_crBack;
	else if (pGridRow->m_crBack != CLR_DEFAULT)
		crBack = pGridRow->m_crBack;
	else if (m_crBack_head != CLR_DEFAULT)
		crBack = m_crBack_head;
	else
		crBack = ::GetSysColor(COLOR_BTNFACE);

	if (pGridItem->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_crText;
	else if (pGridRow->m_crText != CLR_DEFAULT)
		crText = pGridRow->m_crText;
	else if (m_crText_head != CLR_DEFAULT)
		crText = m_crText_head;
	else
		crText = ::GetSysColor(COLOR_BTNTEXT);
}

void CCursorGridCtrl::EvalCellColors_tail(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack)
{
	if (pGridItem->m_crBack != CLR_DEFAULT)
		crBack = pGridItem->m_crBack;
	else if (pGridRow->m_crBack != CLR_DEFAULT)
		crBack = pGridRow->m_crBack;
	else if (m_crBack_tail != CLR_DEFAULT)
		crBack = m_crBack_tail;
	else
		crBack = ::GetSysColor(COLOR_BTNFACE);

	if (pGridItem->m_crText != CLR_DEFAULT)
		crText = pGridItem->m_crText;
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

void CCursorGridCtrl::DrawCell(CDC* pDC, LPRECT pRect, LPRECT pClipRect, CGridItem* pGridItem, COLORREF crText, COLORREF crBack, CFont* pFont)
{
    CRect rect(pRect);
    if (!rect.Width())
        return;
    
    int nRetVal;
    rect.right--;
    rect.bottom--;

	CRect FillRect;
	FillRect.IntersectRect(rect, pClipRect);
	pDC->FillSolidRect(FillRect, crBack);

	pDC->SetTextColor(crText);
    
	CPoint point = pDC->GetWindowOrg();
	CRect rectClip(FillRect);
	rectClip.OffsetRect(-point.x, -point.y);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rectClip);
	pDC->SelectClipRgn(&rgn);

    if (pGridItem->m_wFlagsMask & CGI_CHECKBOX)
    {
        int nImageIndex;
        switch (pGridItem->m_wFlagsMask & CGI_CHECKBOX_STATE)
        {
        case CGI_CHECKBOX_UNCHECKED: nImageIndex = m_nRowHeight > 16 ? iiCheckBoxUnchecked : iiSmallCheckBoxUnckecked; break;
        case CGI_CHECKBOX_CHECKED: nImageIndex = m_nRowHeight > 16 ? iiCheckBoxChecked : iiSmallCheckBoxChecked; break;
        default: nImageIndex = m_nRowHeight > 16 ? iiCheckBoxUndefined : iiSmallCheckBoxUndefined; break;
        }
        m_ilInternalImages.Draw(pDC, nImageIndex, rect.TopLeft(), ILD_NORMAL);
        rect.left += 16;
    }

    if (pGridItem->m_wFlagsMask & CGI_IMAGE && rect.Width())
    {
        int nImageWidth;
        int nImageNumber = pGridItem->m_nImageNumber;
        if (nImageNumber == -1)
        {
			CRect ImageRect(rect);
            if (pGridItem->DrawImage(pDC, ImageRect))
                nImageWidth = ImageRect.Width();
            else
                nImageWidth = 0;
            pGridItem->m_pGridColumn->m_nImageWidth = nImageWidth;
        }
        else
        {
            CImageList* pRowsImages = pGridItem->m_pGridColumn->m_pRowsImages;
            if (pRowsImages)
                if (nImageNumber < pRowsImages->GetImageCount())
                {
                    pRowsImages->Draw(pDC, nImageNumber, CPoint(rect.left, rect.top), ILD_NORMAL);
                    nImageWidth = pGridItem->m_pGridColumn->m_nImageWidth;
                }
        }
        rect.left += nImageWidth;
    }

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
    if (pColumn->m_crHeaderBack != CLR_DEFAULT)
        cr = pColumn->m_crHeaderBack;
    else
	{
		cr = GetSysColor(COLOR_BTNFACE);
		if (m_HeadersStyle != hs3D && m_ActiveHeaderStyle == ahsSelected && pColumn == GetCurrentColumn())
			cr = aux_ShiftColor(cr, -32);
	}

	CRect FillRect;
	FillRect.IntersectRect(pRect, pClipRect);

    pDC->FillSolidRect(FillRect, cr);

    if (pColumn->m_crHeaderText != CLR_DEFAULT)
        cr = pColumn->m_crHeaderText;
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

        char arTextBuffer[1024];
        int nLen = pColumn->m_strCaption.GetLength();
        if (nLen < 1024)
            strcpy(arTextBuffer, pColumn->m_strCaption);
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
		if (m_ActiveHeaderStyle == ahsSelected && pColumn == GetCurrentColumn())
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

void CCursorGridCtrl::RecalcColumnsWidth()
{
    CRect ClientRect;
    GetClientRect(ClientRect);
    int nGridWidth;
    if (ClientRect.Width() < m_pColumns->m_nMinWidth)
        nGridWidth = m_pColumns->m_nResizableMinWidth;
    else
        nGridWidth = ClientRect.Width() - (m_pColumns->m_nMinWidth - m_pColumns->m_nResizableMinWidth);

    
    CRect rect(0, 0, 0, GetHeaderHeight());
    CGridColumn* pLastColumn = NULL;
    for (int i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pColumn = m_pColumns->GetAt(i);
        if (pColumn->m_bVisible)
        {
            rect.left += rect.Width();
            if (pColumn->m_bFixedWidth)
                rect.right = rect.left + pColumn->m_nMinWidth;
            else
                rect.right = rect.left + MulDiv(pColumn->m_nMinWidth, nGridWidth, m_pColumns->m_nResizableMinWidth);
            pColumn->m_rect = rect;
            pLastColumn = pColumn;
        }
    }
    if (pLastColumn)
    {
        if (ClientRect.Width() < m_pColumns->m_nMinWidth)
            pLastColumn->m_rect.right = m_pColumns->m_nMinWidth;
        else
            pLastColumn->m_rect.right = ClientRect.right;
    }

    SetHScrollInfo();
    Invalidate();
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
		GetClientRect(ClientRect);
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
    SCROLLINFO si;
    CRect rect;
    BOOL bSuccess;

    m_Tip.Hide();
    
    GetClientRect(rect);
    
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
        if (pGridColumn->m_bVisible && !pGridColumn->m_bFixedWidth)
        {
            rect = GetColumnClipRect(pGridColumn);
            rect.left = rect.right - RESIZE_AREA_WIDTH;
            rect.right += RESIZE_AREA_WIDTH;
            if (rect.PtInRect(point))
                return pGridColumn;
        }
    }

    return NULL;
}

int CCursorGridCtrl::HitTestColumn(CPoint point)
{
    for (int i = 0; i < m_pColumns->GetSize(); i++)
    {
        CGridColumn* pGridColumn = m_pColumns->GetAt(i);
        if (pGridColumn->m_bVisible)
		{
			CRect rect = GetColumnClipRect(pGridColumn);
            if (rect.PtInRect(point))
                return i;
		}
    }
    return -1;
}

int CCursorGridCtrl::HitTestHeader(CPoint point)
{
	return GetHeaderRect().PtInRect(point) ? HitTestColumn(point) : -1;
}

void CCursorGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this) 
		SetFocus();

    m_Tip.Hide();
    m_hitMouseButton = point;

    int nColumn, nRow;
	RegionType rType;

	switch(HitTest(point, nColumn, nRow, rType))
    {
    case ghtHeader:
        m_pCapturedColumn = GetResizeHeaderAt(point);
        if (m_pCapturedColumn)
		{
            m_CaptureType = ctResizeColumn;
			CRect bounds = GetColumnBoundsRect(m_pCapturedColumn);
			CRect clip = GetColumnClipRect(m_pCapturedColumn);
			m_nCapturedColumn_left = bounds.left - (bounds.right - clip.right);
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
		if (rType == rtBody)
		{
			if (nFlags & MK_CONTROL)
			{
				CDataRowState st(this);
				SetCurrent(nRow, nColumn);
				SelectRow(st);
			}
			else if (nFlags & MK_SHIFT)
			{
				CDataRowState st(this);
				SetCurrent(nRow, nColumn);
				SelectRows(st);
			}
			else
			{
				SetCurrent(nRow, nColumn);
				m_SelectedRows.RemoveAll();
				InvalidateRect(GetGridRect());
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
        break;
    case ghtCheckBox:
		if (rType == rtBody)
			SetCurrent(nRow, nColumn);

		OnClickCheckBox(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType);
        break;
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

GridHitTest CCursorGridCtrl::HitTestCheckBox(CPoint point, int nColumn, int& nRowIndex, RegionType const rType)
{
	GridHitTest ht = ghtSpace;

	CGridRow* pGridRow = GetGridRow(rType, nRowIndex);
	if (!pGridRow)
		nRowIndex = -1;
	else if (nColumn != -1)
	{
		ht = ghtCell;
		if (pGridRow->m_cells[nColumn]->m_wFlagsMask & CGI_CHECKBOX)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(nColumn);
			CRect boundsRect = GetColumnBoundsRect(pColumn);
			CPoint ptTL = boundsRect.TopLeft();
			CRect RegionRect = GetGridRect(rType);
			ptTL.y += RegionRect.top + (nRowIndex - GetTopRowIndex(rType)) * m_nRowHeight;
			CRect rect(ptTL, CSize(14, 14));
			rect.OffsetRect(1, 1);
			CRect clipRect = GetColumnClipRect(pColumn);
			rect.IntersectRect(rect, clipRect);
			if (rect.PtInRect(point))
				ht = ghtCheckBox;
		}
	}
	return ht;
}

GridHitTest CCursorGridCtrl::HitTest(CPoint point, int &nColumn, int &nRowIndex, RegionType& rType)
{
	GridHitTest ht;

	nColumn = HitTestColumn(point);
	rType = TestRegionType(point, nRowIndex);

	switch(rType)
	{
	case rtHeader:
		ht = ghtHeader;
		break;
	case rtBody:
	case rtHead:
	case rtTail:
		ht = HitTestCheckBox(point, nColumn, nRowIndex, rType);
		break;
	default:
		ht = ghtSpace;
	}

	return ht;
}

void CCursorGridCtrl::SetCurrent(int nRow, int nColumn)
{
    SetCurrentCell(nRow, nColumn);
/*
    if (m_nCurrentDataRowIndex == m_nTopRowIndex + m_nVisibleRows - 1 && m_bLastRowNotFull)
        ScrollDown(1);
*/
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
			Reset(m_pCurrentDataRowOutOfWindow);
		
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
			Reset(m_pCurrentDataRowOutOfWindow);
		
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
	if (m_SelectMode == smMultiLine)
	{
		CDataRow* pPrevRow = st.m_pDataRow;
		CDataRow* pPrevDataRow = NULL;
		if (st.m_nSize == 0 && !st.m_bIsRemoved)
		{
			m_SelectedRows.Add(pPrevRow);
			pPrevDataRow = pPrevRow;
		}
		
		CDataRow* pDataRow = GetCurrentDataRow();
		if (pDataRow != pPrevDataRow)
			m_SelectedRows.Toggle(pDataRow);
		
		m_SelectedRows.SetCurrentRow(pDataRow);
		m_SelectedRows.SetDirection(0);
	}
    InvalidateRect(GetGridRect());
}

// nDirection is used only when prev row is out of the window
// and it is impossible to determine prev row direction (home/end case)
void CCursorGridCtrl::SelectRows(CDataRowState& st, int nDirection, BOOL bOneRow)
{
	if (m_SelectMode == smMultiLine)
	{
		CDataRow* pPrevDataRow = st.m_pDataRow;
		int nPrevRowDirection = st.m_nDataRowDirection;
		int nPrevSelectDirection = st.m_nSelectDirection;
		BOOL bIsRemoved = st.m_bIsRemoved;
		BOOL bIsCurrent = st.m_bIsCurrent;
		
		int nPrevRowIndex = -1; // st.m_nDataRowIndex;
		for (int i = m_nTopRowIndex; i < m_rows.GetSize(); i++)
		{
			if (m_pProvider->CompareRows(m_rows[i]->m_pDataRow, pPrevDataRow))
			{
				nPrevRowIndex = i;
				break;
			}
		}
		
		CDataRow* pCurrentDataRow = GetCurrentDataRow();
		
		if (m_pProvider->CompareRows(pCurrentDataRow, pPrevDataRow))
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
				m_SelectedRows.Toggle(pPrevDataRow);
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
				m_SelectedRows.Toggle(m_rows[i]->GetDataRow());
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
			
			m_pProvider->QueryRows(pPrevDataRow, nCount, nPage);
			int nExpected = nPage;
			while (CDataRow* pDataRow = m_pProvider->Fetch())
			{
				nExpected--;
				
				m_SelectedRows.Toggle(pDataRow);
				if (m_pProvider->CompareRows(pDataRow, pCurrentDataRow))
				{
					pDataRow->Release();
					break;
				}
				
				if (!nExpected)
				{
					nPage *= 2;
					nCount *= 2;
					nExpected = nPage;
					m_pProvider->QueryRows(pDataRow, nCount, nPage);
				}
				pDataRow->Release();
			}
			if (nExpected)
				while (CDataRow* pDataRow = m_pProvider->Fetch())
				{
					nExpected--;
					pDataRow->Release();
				}
		}
		m_SelectedRows.SetRemovedRow(NULL);
		m_SelectedRows.SetCurrentRow(pCurrentDataRow);
		m_SelectedRows.SetDirection(nDirection);
	}
    InvalidateRect(GetGridRect());
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
    switch (m_CaptureType)
    {
    case ctResizeColumn:
		EndColResize();
		OnColumnWidthChange();
        break;
    case ctClickHeader:
        {
            int nColumnIndex = HitTestColumn(point);
            if (nColumnIndex != -1 && m_pColumns->GetAt(nColumnIndex) == m_pCapturedColumn)
                OnHeaderSelection(m_pCapturedColumn);
        }
        break;
	case ctScroll:
        KillTimer(TIMER_ID_SCROLL);
		break;
    }

    if (m_CaptureType != ctNone)
    {
        ReleaseCapture();
        m_CaptureType = ctNone;
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
			CRect rect = GetColumnBoundsRect(m_pCapturedColumn);
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
                if (m_bChangePositionOfColumns || m_pCapturedColumn->m_bChangeVisible)
                {
                    m_Tip.Hide();
                    CRect rect = GetColumnBoundsRect(m_pCapturedColumn);
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

				int nColumn, nRow;
				if (HitTest(m_hitMouseButton, nColumn, nRow, m_DragRegionType) == ghtCell)
				{
					m_pDragRow = GetGridRow(m_DragRegionType, nRow)->GetDataRow();
					m_pDragColumn = m_pColumns->GetAt(nColumn);
					OnBeginDrag(m_pDragRow, m_pDragColumn, m_DragRegionType);
				}
            }
        }
        break;
	case ctScroll:
		{
			CRect rect = GetGridRect(rtBody);
			if (rect.PtInRect(point))
			{
				int nColumn, nRow;
				RegionType rType;
				if (HitTest(point, nColumn, nRow, rType) == ghtCell)
					SetCurrent(nRow, nColumn);
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
    int nColIndex = HitTestColumn(point);
    if (nColIndex != -1)
    {
        CString strTipText;
        CRect TipRect;
        DWORD dwTipedObject = 0;

        CGridColumn* pGridColumn = m_pColumns->GetAt(nColIndex);
        TipRect.IntersectRect(GetColumnBoundsRect(pGridColumn), HeaderRect);
        BOOL bByCursor;
        CFont* pFont = &m_font;
        if (HeaderRect.PtInRect(point))
        {
            TipRect.left += RESIZE_AREA_WIDTH;
            if (pGridColumn->m_OrderMarker != omNone)
                TipRect.right -= 16;
            TipRect.right -= RESIZE_AREA_WIDTH;
            strTipText = pGridColumn->GetCaption();
            dwTipedObject = (DWORD)pGridColumn;
            bByCursor = TRUE;
            if (pGridColumn->m_pHeaderFont)
                pFont = pGridColumn->m_pHeaderFont;
        }
        else
        {
			HeaderRect.bottom = HeaderRect.top + GetHeaderHeight();
			TipRect.IntersectRect(GetColumnBoundsRect(pGridColumn), HeaderRect);
            int nRow;
			RegionType rType = TestRegionType(point, nRow);
			CGridRow* pGridRow = GetGridRow(rType, nRow);
			nRow -= GetTopRowIndex(rType);
			if (pGridRow)
			{
				CGridItem* pGridItem = pGridRow->m_cells[nColIndex];
				strTipText = pGridItem->m_strText;
				TipRect.OffsetRect(0, GetGridRect(rType).top + nRow * m_nRowHeight);
				if (pGridItem->m_wFlagsMask & CGI_CHECKBOX)
					TipRect.left += 16;
				if (pGridItem->m_wFlagsMask & CGI_IMAGE)
					TipRect.left += pGridColumn->m_nImageWidth;
				dwTipedObject = (DWORD)pGridItem;
				bByCursor = FALSE;
				if (pGridItem->m_pFont)
					pFont = pGridItem->m_pFont;
				else if (pGridRow->m_pFont)
					pFont = pGridRow->m_pFont;
				else if (pGridItem->m_pGridColumn->m_pFont)
					pFont = pGridItem->m_pGridColumn->m_pFont;
			}
		}

        if (!TipRect.PtInRect(point))
            return;

        if (dwTipedObject)
    	    m_Tip.Show(strTipText, TipRect, dwTipedObject, pFont, bByCursor);
    }
}

int CCursorGridCtrl::GetColumnAutoWidth(CGridColumn* pGridColumn)
{
	int nWidth = 0;
	int nColIndex = m_pColumns->IndexOf(pGridColumn);

	CTypedPtrArray<CPtrArray, CGridRow*> rows;
	for (int i = 0; i < GetFullVisibleRows(rtHead); i++)
		rows.Add(m_rows_head[i]);

	for (i = 0; i < GetFullVisibleRows(rtTail); i++)
		rows.Add(m_rows_tail[i]);

	for (i = 0; i < GetFullVisibleRows(rtBody); i++)
		rows.Add(m_rows[m_nTopRowIndex + i]);

	for (i = 0; i < rows.GetSize(); i++)
	{
		CClientDC dc(this);
		CGridRow* pGridRow = rows[i];
		CGridItem* pGridItem = pGridRow->m_cells[nColIndex];
		int nItemWidth = 0;

		if (pGridItem->m_wFlagsMask & CGI_CHECKBOX)
			nItemWidth += 16;

		if (pGridItem->m_wFlagsMask & CGI_IMAGE)
			nItemWidth += pGridColumn->m_nImageWidth;
		
		CFont* pFont = &m_font;
		if (pGridItem->m_pFont)
			pFont = pGridItem->m_pFont;
		else if (pGridRow->m_pFont)
			pFont = pGridRow->m_pFont;
		else if (pGridItem->m_pGridColumn->m_pFont)
			pFont = pGridItem->m_pGridColumn->m_pFont;

		CString csText = pGridItem->m_strText;
		csText.TrimRight();

		CFont *pOldFont;
		pOldFont = dc.SelectObject(pFont);

		CSize size = dc.GetTextExtent(csText);
	    dc.SelectObject(pOldFont);

		nItemWidth += size.cx;
		nWidth = max(nWidth, nItemWidth);
	}
	rows.RemoveAll();

	nWidth += GetLeftTextMargin() + GetRightTextMargin() + 1;
	if (nWidth >= AUTOWIDTH_TEXT_MARGIN_THRESHOLD)
		nWidth += AUTOWIDTH_TEXT_MARGIN;

	nWidth = max(nWidth, RESIZE_MIN_WIDTH);
	return nWidth;
}

void CCursorGridCtrl::StepColResize(int nNewWidth)
{
    if (nNewWidth < RESIZE_MIN_WIDTH)
        nNewWidth = RESIZE_MIN_WIDTH;
    if (nNewWidth > RESIZE_MAX_WIDTH)
        nNewWidth = RESIZE_MAX_WIDTH;
    if (nNewWidth == m_pCapturedColumn->m_rect.Width())
        return;

    if (IS_SHIFT_PRESSED())
    {
        int dx = nNewWidth - m_pCapturedColumn->m_rect.Width();
		if (dx < 0)
		{
			int xav = GetGridRect().Width() - m_pColumns->GetVisibleWidth();
			dx = max(dx, xav);
		}
		else
		{
			dx = min(dx, GetPageRect().Width() - RESIZE_MIN_WIDTH);
		}
        for (int i = m_pColumns->GetUpperBound(); i >= 0; i--)
        {
            CGridColumn* pGridColumn = m_pColumns->GetAt(i);
            if (pGridColumn->m_bVisible)
            {
                if (pGridColumn == m_pCapturedColumn)
                {
                    pGridColumn->m_rect.right += dx;
                    break;
                }
                else
                    pGridColumn->m_rect.OffsetRect(dx, 0);
            }
        }

        m_pColumns->m_nMinWidth = 0;
        for (i = 0; i < m_pColumns->GetSize(); i++)
        {
            CGridColumn* pGridColumn = m_pColumns->GetAt(i);
            if (pGridColumn->m_bVisible)
            {
                pGridColumn->m_nMinWidth = pGridColumn->m_rect.Width();
                m_pColumns->m_nMinWidth += pGridColumn->m_nMinWidth;
            }
        }
		SetHScrollInfo();
    }
    else if (IS_CTRL_PRESSED())
    {
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
    else
    {
		CRect page = GetPageRect();
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
        for (i = m_pColumns->GetUpperBound(); i >= 0; i--)
        {
            CGridColumn* pGridColumn = m_pColumns->GetAt(i);
            if (pGridColumn == m_pCapturedColumn)
                break;
            if (pGridColumn->m_bVisible)
            {
                if (!nResizeWidth)
                    nResizeWidth = CurX = pGridColumn->m_rect.right;
                if (pGridColumn->m_bFixedWidth)
                    nResizeWidth -= pGridColumn->m_rect.Width();
            }
        }

        if (!nResizeWidth)
            return;
        
        int nNewResizeWidth = nResizeWidth - (m_pCapturedColumn->m_rect.left + nNewWidth);
        nResizeWidth -= m_pCapturedColumn->m_rect.right;
        
        for (i = m_pColumns->GetUpperBound(); i >= 0; i--)
        {
            CGridColumn* pGridColumn = m_pColumns->GetAt(i);
            if (pGridColumn == m_pCapturedColumn)
            {
                pGridColumn->m_rect.right = CurX;
                break;
            }

            if (pGridColumn->m_bVisible)
            {
                int nWidth;
                if (pGridColumn->m_bFixedWidth)
                    nWidth = pGridColumn->m_nMinWidth;
                else
                    nWidth = MulDiv(pGridColumn->m_rect.Width(), nNewResizeWidth, nResizeWidth);
                    if (nWidth < RESIZE_MIN_WIDTH)
                        nWidth = RESIZE_MIN_WIDTH;
                pGridColumn->m_rect.right = CurX;
                CurX -= nWidth;
                pGridColumn->m_rect.left = CurX;
            }
        }
		SetHScrollInfo();
    }
    Invalidate();
}

void CCursorGridCtrl::EndColResize()
{
	int i, nTotalWidth = 0, nTotalMinWidth = m_pColumns->m_nMinWidth;
	for (i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			if (pGridColumn->m_bFixedWidth)
				nTotalMinWidth -= pGridColumn->m_nMinWidth;
			else
				nTotalWidth += pGridColumn->m_rect.Width();
		}
	}
	m_pColumns->m_nResizableMinWidth = 0;
	for (i = 0; i < m_pColumns->GetSize(); i++)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			if (!pGridColumn->m_bFixedWidth)
			{
				pGridColumn->m_nMinWidth = MulDiv(pGridColumn->m_rect.Width(), nTotalMinWidth, nTotalWidth);
				m_pColumns->m_nResizableMinWidth += pGridColumn->m_nMinWidth;
			}
		}
	}
	SetHScrollInfo();
}

void CCursorGridCtrl::CalcRowsParams()
{
    if (m_nRowHeight == 0)
        return;

	int nRowsHeight = GetGridRect(rtNone).Height();
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

CRect CCursorGridCtrl::GetGridRect(RegionType rType) const
{
	CRect HeaderRect = GetHeaderRect();
    CRect rect;
	GetClientRect(rect);

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

void CCursorGridCtrl::PaintGridLines(CDC *pDC, CRect& rect, int nFullVisibleRows)
{
	int nStrokeCount;
	int nPointCount;
	int i;

	nStrokeCount = nFullVisibleRows;
	if (nStrokeCount)
		nStrokeCount++;

	if (m_bVerticalLines)
	{
		for (i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			CRect Rect = GetColumnClipRect(pColumn);

			if(Rect.right > rect.left && Rect.left < rect.right)
				nStrokeCount +=2 ;
		}
	}

	if (nStrokeCount)
	{
		nPointCount = nStrokeCount * 2;

		LPPOINT lpPoints = new POINT[nPointCount];
		LPDWORD lpPolyPoints = new DWORD[nStrokeCount];

		for (i = 0; i < nStrokeCount; i++)
			lpPolyPoints[i] = 2;

		//Horizontal
		int nLeft = rect.left;
		int nRight = rect.right;
		int y = rect.top - 1;

		int nIndex = 0;
		if (nFullVisibleRows)
		{
			lpPoints[nIndex].x = nLeft;
			lpPoints[nIndex++].y = y;
			lpPoints[nIndex].x = nRight;
			lpPoints[nIndex++].y = y;
		}

		for (i = 0; i < nFullVisibleRows; i++)
		{
			y += m_nRowHeight;
			lpPoints[nIndex].x = nLeft;
			lpPoints[nIndex++].y = y;
			lpPoints[nIndex].x = nRight;
			lpPoints[nIndex++].y = y;
		}

		//Vertical
		if (m_bVerticalLines)
		{
			int nTop = rect.top;
			int nBottom;
			if (m_bHorizontalLines)
				nBottom = y + 1;
			else
				nBottom = rect.bottom;

			for (i = 0; i < m_pColumns->GetSize(); i++)
			{
				CGridColumn* pColumn = m_pColumns->GetAt(i);
				CRect Rect = GetColumnClipRect(pColumn);

				if (Rect.right > rect.left && Rect.left < rect.right)
				{
					lpPoints[nIndex].x = Rect.left - 1;
					lpPoints[nIndex++].y = nTop;
					lpPoints[nIndex].x = Rect.left - 1;
					lpPoints[nIndex++].y = nBottom;

					lpPoints[nIndex].x = Rect.right - 1;
					lpPoints[nIndex++].y = nTop;
					lpPoints[nIndex].x = Rect.right - 1;
					lpPoints[nIndex++].y = nBottom;
				}
			}
		}

		BOOL bSuccess;
		CPen *pOldPen;
		CPen penGridLines;
		penGridLines.CreatePen(PS_SOLID, 1, GetLinesColor());
		pOldPen = pDC->SelectObject(&penGridLines);

		bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
		ASSERT(bSuccess);

		pOldPen = pDC->SelectObject(pOldPen);

		delete[] lpPoints;
		delete[] lpPolyPoints;
	}
}

void CCursorGridCtrl::PaintGridLines(CDC *pDC)
{
	int nRows = 0;

	CRect rect = GetGridRect(rtHead);
	if (m_bHorizontalLines)
		nRows = GetFullVisibleRows(rtHead);
	PaintGridLines(pDC, rect, nRows);

	rect = GetGridRect(rtBody);
	if (m_bHorizontalLines)
		nRows = GetFullVisibleRows(rtBody);
	PaintGridLines(pDC, rect, nRows);

	rect = GetGridRect(rtTail);
	if (m_bHorizontalLines)
		nRows = GetFullVisibleRows(rtTail);
	PaintGridLines(pDC, rect, nRows);
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
    {
        m_pCurrentDataRowOutOfWindow->Release();
        m_pCurrentDataRowOutOfWindow = NULL;
    }

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
			Invalidate();
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
			Invalidate();
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

			if (m_pProvider->RefreshRow(m_rows[0]->m_pDataRow))
			{
				GetRowsDown(m_nVisibleRows - 1);
				if (m_nCurrentDataRowIndex == -1)
				{
					if (!m_pProvider->RefreshRow(m_pCurrentDataRowOutOfWindow))
						SetCurrentCell(0, m_nCurrentColumn);
				}
				else
					OnChangeCurrentData(m_rows[m_nCurrentDataRowIndex]->m_pDataRow);
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
							if (!m_pProvider->RefreshRow(m_pCurrentDataRowOutOfWindow))
								SetCurrentCell(0, m_nCurrentColumn);
							else
								OnChangeCurrentData(m_pCurrentDataRowOutOfWindow);
						}
						else
							OnChangeCurrentData(m_rows[m_nCurrentDataRowIndex]->m_pDataRow);
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
		while (pDataRow = m_pProvider_head->Fetch())
		{
			CGridRow* pGridRow = NewGridRow(m_pColumns, pDataRow, rtHead);
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
		while (pDataRow = m_pProvider_tail->Fetch())
		{
			CGridRow* pGridRow = NewGridRow(m_pColumns, pDataRow, rtTail);
			pGridRow->FillItems();
			m_rows_tail.Add(pGridRow);
		}
	}
	CalcRowsParams();
}

void CCursorGridCtrl::ActivateRow(CDataRow* pActRow)
{
    for (int i = m_nTopRowIndex; i < m_nTopRowIndex + m_nVisibleRows && i < m_rows.GetSize(); i++)
    {
        if (m_pProvider->CompareRows(m_rows[i]->m_pDataRow, pActRow))
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

    CDataRow* pCurrentRowToDel = m_pCurrentDataRowOutOfWindow;
    m_nCurrentDataRowIndex = -1;
	m_nCurrentDataRowDirection = 0;
    m_pCurrentDataRowOutOfWindow = NULL;

    if (pActRow)
    {
        int nCount = m_nVisibleRows;
        m_pProvider->QueryRows(pActRow, nCount, m_nVisibleRows);
        CDataRow* pDataRow;
        while (pDataRow = m_pProvider->Fetch())
        {
            m_rows.Add(NewGridRow(m_pColumns, pDataRow, rtBody));
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

	if (pCurrentRowToDel)
		pCurrentRowToDel->Release();
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
            si.nMax = nRowCount - 1;
            si.nPos = m_rows[m_nTopRowIndex]->m_pDataRow->GetRowIndex();
            si.nPage = GetPageSize();
        }
        else
        {
            si.fMask = SIF_POS | SIF_RANGE;
            si.nMax = 100;
            if (bIsBegin)
                si.nPos = 0;
            else if(bIsEnd)
                si.nPos = 100;
            else
                si.nPos = 50;
        }

        BOOL bSuccess = SetScrollInfo(SB_VERT, &si);
        ASSERT(bSuccess);
        ShowScrollBar(SB_VERT, TRUE);
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
        pRowFrom = m_rows[m_rows.GetUpperBound()]->m_pDataRow;
    else
        pRowFrom = NULL;

    m_pProvider->QueryRows(pRowFrom, nCount, m_nVisibleRows);

    CDataRow* pDataRow;
    while (pDataRow = m_pProvider->Fetch())
    {
        m_rows.Add(NewGridRow(pDataRow, m_rows.GetSize()));
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
        pRowFrom = m_rows[0]->m_pDataRow;
    else
        pRowFrom = NULL;

    m_pProvider->QueryRows(pRowFrom, -nCount, m_nVisibleRows);

    CDataRow* pDataRow;
    while (pDataRow = m_pProvider->Fetch())
    {
        if (m_nCurrentDataRowIndex != -1)
            m_nCurrentDataRowIndex++;
        m_rows.InsertAt(0, NewGridRow(pDataRow, 0));
        nCount--;
        m_nTopRowIndex++;
        m_bEvenRow = !m_bEvenRow;
    }
	m_bIsBOF = nCount > 0;
}

CGridRow* CCursorGridCtrl::NewGridRow(CDataRow* pDataRow, int nNewRowIndex)
{
    CGridRow* pGridRow = NewGridRow(m_pColumns, pDataRow, rtBody);
    if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
    {
        if (m_pProvider->CompareRows(pDataRow, m_pCurrentDataRowOutOfWindow))
        {
            m_pCurrentDataRowOutOfWindow->Release();
            m_nCurrentDataRowIndex = nNewRowIndex;
			m_nCurrentDataRowDirection = 0;
            pGridRow->m_bIsCurrent = true;
            OnChangeCurrentData(pDataRow);
        }
    }
    return pGridRow;
}

int CCursorGridCtrl::GetFixedWidth_left() const
{
	int nWidth = 0;
	for (int i = 0, nFixed = m_nFixedLeft; nFixed && i < m_pColumns->GetSize(); ++i)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			nWidth += pGridColumn->m_rect.Width();
			--nFixed;
		}
	}
	return nWidth;
}

int CCursorGridCtrl::GetFixedWidth_right() const
{
	int nWidth = 0;
	for (int i = m_pColumns->GetUpperBound(), nFixed = m_nFixedRight; nFixed && i >= 0; --i)
	{
		CGridColumn* pGridColumn = m_pColumns->GetAt(i);
		if (pGridColumn->m_bVisible)
		{
			nWidth += pGridColumn->m_rect.Width();
			--nFixed;
		}
	}
	return nWidth;
}

CRect CCursorGridCtrl::GetPageRect() const
{
	CRect rect;
	GetClientRect(rect);
	rect.left += GetFixedWidth_left();
	rect.right -= GetFixedWidth_right();

	if (rect.right < rect.left)
		rect.right = rect.left;

	return rect;
}

CRect CCursorGridCtrl::GetColumnBoundsRect(CGridColumn* pGridColumn)
{
	CRect clientRect;
	GetClientRect(clientRect);
	CRect rect(0, 0, 0, 0);
	BOOL bFound = FALSE;
	int i = 0, j = m_pColumns->GetUpperBound();

	for (int nFixedLeft = m_nFixedLeft; nFixedLeft && i <= j; ++i)
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
		for (int nFixedRight = m_nFixedRight; nFixedRight && i <=j; --j)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(j);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					rect = pColumn->m_rect;
					rect.OffsetRect(clientRect.Width() - m_pColumns->GetVisibleWidth(), 0);
					bFound = TRUE;
					break;
				}
				--nFixedRight;
			}
		}
	}
	if (!bFound)
	{
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
	}
	rect.top = clientRect.top;
	rect.bottom = clientRect.bottom;
	return rect;
}

CRect CCursorGridCtrl::GetColumnClipRect(CGridColumn* pGridColumn)
{
	CRect clientRect;
	GetClientRect(clientRect);
	CRect rect(0, 0, 0, 0);
	BOOL bFound = FALSE;
	int i = 0, j = m_pColumns->GetUpperBound();

	for (int nFixedLeft = m_nFixedLeft; nFixedLeft && i <= j; ++i)
	{
		CGridColumn* pColumn = m_pColumns->GetAt(i);
		if (pColumn->m_bVisible)
		{
			if (pColumn == pGridColumn)
			{
				rect.IntersectRect(pColumn->m_rect, clientRect);
				bFound = TRUE;
				break;
			}
			--nFixedLeft;
		}
	}
	if (!bFound)
	{
		for (int nFixedRight = m_nFixedRight; nFixedRight && i <=j; --j)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(j);
			if (pColumn->m_bVisible)
			{
				if (pColumn == pGridColumn)
				{
					CRect Rect = pColumn->m_rect;
					Rect.OffsetRect(clientRect.Width() - m_pColumns->GetVisibleWidth(), 0);
					Rect.left = max(Rect.left, clientRect.left + GetFixedWidth_left());
					rect.IntersectRect(Rect, clientRect);
					bFound = TRUE;
					break;
				}
				--nFixedRight;
			}
		}
	}
	if (!bFound)
	{
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
	}
	rect.top = clientRect.top;
	rect.bottom = clientRect.bottom;
	return rect;
}

void CCursorGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this) 
		SetFocus();
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
            if (nPos == 0)
                ToHome();
            else if (nPos == 100)
                ToEnd();
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
        break;
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
    m_Tip.Hide();
    
    UINT nScroll;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nScroll, 0);

    if (zDelta < 0)
        ScrollDown(nScroll);
    else
        ScrollUp(nScroll);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CCursorGridCtrl::SetCurrentCell(int nRowIndex, int nColumnIndex)
{
    if (m_nCurrentDataRowIndex == nRowIndex && m_nCurrentColumn == nColumnIndex)
        return;

	BOOL bRowActivated = FALSE;
	BOOL bColumnActivated = FALSE;

    if (m_nCurrentDataRowIndex != nRowIndex)
    {
        if (m_nCurrentDataRowIndex == -1)
		{
			if  (m_pCurrentDataRowOutOfWindow) 
			{
				m_pCurrentDataRowOutOfWindow->Release();
				m_pCurrentDataRowOutOfWindow = NULL;
			}
		}
		else
			m_rows[m_nCurrentDataRowIndex]->m_bIsCurrent = false;

        if (nRowIndex >= 0 && nRowIndex < m_rows.GetSize())
        {
            m_nCurrentDataRowIndex = nRowIndex;
            m_rows[nRowIndex]->m_bIsCurrent = true;
            OnChangeCurrentData(m_rows[nRowIndex]->m_pDataRow);
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

	m_SelectedRows.SetRemovedRow(NULL);
	m_SelectedRows.SetCurrentRow(NULL);
	m_SelectedRows.SetDirection(0);

	if (bRowActivated)
		OnActivateRow();

	if (bColumnActivated)
		OnActivateColumn();

    OnActivateCell();

    Invalidate();
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
    
	switch (nChar)
    {
    case VK_LEFT:
		ScrollCurrentCell(-1, 0);
        break;
    case VK_RIGHT:
		ScrollCurrentCell(1, 0);
        break;
    case VK_UP:
		{
			CDataRowState st(this);
			ScrollCurrentCell(0, -1);
			if (bSHIFT)
				SelectRows(st, -1 , TRUE);
		}
        break;
    case VK_DOWN:
		{
			CDataRowState st(this);
			ScrollCurrentCell(0, 1);
			if (bSHIFT)
				SelectRows(st, 1, TRUE);
		}
        break;
    case VK_PRIOR:
        {
			CDataRowState st(this);
            if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
                Reset(m_pCurrentDataRowOutOfWindow);

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
			CDataRowState st(this);
            if (m_nCurrentDataRowIndex == -1 && m_pCurrentDataRowOutOfWindow)
                Reset(m_pCurrentDataRowOutOfWindow);

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
    case VK_CONTROL:
        //m_ColumnHelper.Show();
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
    m_Tip.Hide();
	m_hitMouseButton = point;
	
    int nColumn, nRow;
	RegionType rType;

    switch(HitTest(point, nColumn, nRow, rType))
    {
    case ghtCell:
		if (rType == rtBody)
		{
			SetCurrent(nRow, nColumn);
			if (m_bEnableDrag)
			{
				m_CaptureType = ctDragDelay;
				SetCapture();
			}
		}
        break;
    case ghtCheckBox:
		if (rType == rtBody)
	        SetCurrent(nRow, nColumn);
        break;
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
    int nColumn, nRow;
	RegionType rType;

    switch(HitTest(point, nColumn, nRow, rType))
    {
    case ghtHeader:
        m_pCapturedColumn = GetResizeHeaderAt(point);
		if (m_pCapturedColumn)
		{
			if (IS_SHIFT_PRESSED())
				AutoWidth();
			else
			{
				StepColResize(GetColumnAutoWidth(m_pCapturedColumn));
				EndColResize();
			}
			OnColumnWidthChange();
		}
        break;
    case ghtCell:
        OnSelection(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType);
        break;
    case ghtCheckBox:
		if (rType == rtBody)
	        OnClickCheckBox(GetGridRow(rType, nRow)->GetDataRow(), m_pColumns->GetAt(nColumn), rType);
        break;
    }
    CWnd::OnLButtonDblClk(nFlags, point);
}

void CCursorGridCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
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

	int nColumn, nRow;
	RegionType rType;

	HitTest(point, nColumn, nRow, rType);
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

    m_nRowHeight = m_nFontHeight + GetTopTextMargin() + GetBottomTextMargin();
    RecalcColumnsWidth();
    CalcRowsParams();

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
            GetClientRect(rect);
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
            else
            {
                rect.left = rect.right - 16;
                rect.bottom = rect.top + m_nRowHeight;
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
		GetClientRect(rectClient);
		int nHeight = rect.Height();
		rect.top = rectClient.Height();
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

COLORREF CCursorGridCtrl::GetBackColor() const
{
    return m_crBack;
}

void CCursorGridCtrl::SetBackColor(COLORREF crBackColor)
{
    if (m_crBack == crBackColor)
        return;
    m_crBack = crBackColor;
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
		Invalidate();
	}
}

BOOL CCursorGridCtrl::GetCurrentCellBounds(CRect &rect)
{
	BOOL bSuccess = FALSE;
    if (m_nCurrentColumn != -1) 
    { 
           if (m_nCurrentDataRowIndex != -1 && m_nCurrentDataRowIndex >= m_nTopRowIndex && m_nCurrentDataRowIndex < m_nTopRowIndex + m_nVisibleRows)
           {
				CRect rectBody = GetGridRect(rtBody);

				CGridColumn* pColumn = GetCurrentColumn(); 
				rect = GetColumnClipRect(pColumn);
				rect.IntersectRect(rect, rectBody);
				rect.bottom = rect.top + m_nRowHeight;
				rect.OffsetRect(0, (m_nCurrentDataRowIndex - m_nTopRowIndex) * m_nRowHeight); 

				bSuccess = rect.IntersectRect(rect, rectBody); 
           }
     } 
     return bSuccess; 
}

CDataRow* CCursorGridCtrl::GetCurrentDataRow() const
{
	if (m_nCurrentDataRowIndex == -1)
		return m_pCurrentDataRowOutOfWindow;

	else
		return m_rows[m_nCurrentDataRowIndex]->GetDataRow();
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
/////////////////////////////////////////////////////////////////////////////
// CCGDropTarget
/////////////////////////////////////////////////////////////////////////////

CCGDropTarget::CCGDropTarget(CCursorGridCtrl* pGrid) : m_pGrid(pGrid)
{
	m_pDropOverColumn = NULL;
	m_pDropOverRow = NULL;
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
    CDataRow* pDataRow = NULL;
    BOOL bDropEnable = FALSE;

	int nColumn, nRow;
	RegionType rType;

	CRect ClientRect;
	m_pGrid->GetClientRect(ClientRect);
	if (!ClientRect.PtInRect(point))
		m_dwDropEffect = DROPEFFECT_NONE;
	else
	{
		switch (m_pGrid->HitTest(point, nColumn, nRow, rType))
		{
		case ghtCell:
		case ghtCheckBox:
		case ghtSpace:
			if (nColumn != -1)
				pColumn = m_pGrid->m_pColumns->GetAt(nColumn);

			if (nRow != -1)
				pDataRow = m_pGrid->GetGridRow(rType, nRow)->GetDataRow();

			bDropEnable = TRUE;
			break;
		default:
			m_dwDropEffect = DROPEFFECT_NONE;
		}
	}

	if (m_pDropOverColumn != pColumn || !CompareRows(m_pDropOverRow, m_DropOverRegionType, pDataRow, rType))
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
					&& CompareRows(m_pDropOverRow, m_DropOverRegionType, m_pGrid->m_pDragRow, m_pGrid->m_DragRegionType))
				{
					dropTargetView = dtvNone;
					bDropEnable = FALSE;
					m_dwDropEffect = DROPEFFECT_NONE;
				}
                break;
            case dtvRow:
                if (m_pGrid->m_nCurrentDataRowIndex != -1)
					if (CompareRows(m_pDropOverRow, m_DropOverRegionType, m_pGrid->m_pDragRow, m_pGrid->m_DragRegionType))
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
    	        m_dwDropEffect = m_pGrid->OnDragOver(pDataObject, dwKeyState, m_pDropOverRow, m_pDropOverColumn, m_DropOverRegionType, dropTargetView);
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

    m_pGrid->OnDrop(pDataObject, dropDefault, m_pDropOverRow, m_pDropOverColumn, m_DropOverRegionType);
    
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

void CCGDropTarget::SetDropOverRow(CDataRow* pRow, RegionType rType)
{
	if (m_pDropOverRow)
		m_pDropOverRow->Release();

	m_pDropOverRow = pRow;

	if (m_pDropOverRow)
		m_pDropOverRow->AddRef();

	m_DropOverRegionType = rType;
}
