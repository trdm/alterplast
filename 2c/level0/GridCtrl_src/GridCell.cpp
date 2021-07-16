// GridCell.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for the "default" cell type of the
// grid control. Adds in cell editing.
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
// For use with CGridCtrl v2.20
//
// History:
// Eric Woodruff - 20 Feb 2000 - Added PrintCell() plus other minor changes
// Ken Bertelson - 12 Apr 2000 - Split CGridCell into CGridCell and CGridCellBase
// <kenbertelson@hotmail.com>
// C Maunder     - 17 Jun 2000 - Font handling optimsed, Added CGridDefaultCell
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "InPlaceEdit.h"
#include "GridCtrl.h"
#include "View/Moxcel.h"//поддержка формата 1С

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCell, CGridCellBase)
IMPLEMENT_DYNCREATE(CGridDefaultCell, CGridCell)


CValue GetTypeValue(CString Str);

/////////////////////////////////////////////////////////////////////////////
// GridCell

CGridCell::CGridCell()
{
	m_pEditWnd=0;
    m_plfFont = NULL;
	m_pCell=0;
//    m_nLeftSpace=0;
    Reset();
}

CGridCell::~CGridCell()
{
    delete m_plfFont;

 	delete m_pCell;
}

void CGridCell::SetCell(CCell *pValue)
{
	delete m_pCell;
	if(pValue)
	{
		m_pCell=new CCell;
		*m_pCell=*pValue;
	}
	else
	{
		m_pCell=0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// GridCell Attributes

void CGridCell::operator=(const CGridCell& cell)
{
    CGridCellBase::operator=(  cell);
	SetCell(cell.m_pCell);
}

void CGridCell::Reset()
{
    CGridCellBase::Reset();

	m_Value.Reset();
//    m_strText.Empty();
    m_nImage   = -1;
    m_pGrid    = NULL;
    m_bEditing = FALSE;
	if(m_pEditWnd)
		EndEdit();
    m_pEditWnd = NULL;

    m_nFormat = (DWORD)-1;       // Use default from CGridDefaultCell
    m_crBkClr = CLR_DEFAULT;     // Background colour (or CLR_DEFAULT)
    m_crFgClr = CLR_DEFAULT;     // Forground colour (or CLR_DEFAULT)
    m_nMargin = (UINT)-1;        // Use default from CGridDefaultCell

    delete m_plfFont;
    m_plfFont = NULL;            // Cell font
    m_lParam=NULL;

	m_nCoveredRow=0;
	m_nCoveredCol=0;
	pCoveredCell=0;

	delete m_pCell;
	m_pCell=0;
}

void CGridCell::SetFont(const LOGFONT* plf)
{
    if (plf == NULL)
    {
        delete m_plfFont;
        m_plfFont = NULL;
    }
    else
    {
        if (!m_plfFont)
            m_plfFont = new LOGFONT;
        if (m_plfFont)
            memcpy(m_plfFont, plf, sizeof(LOGFONT)); 
    }
}

LOGFONT* CGridCell::GetFont() const
{
    if (m_plfFont == NULL)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return NULL;

        return pDefaultCell->GetFont();
    }

    return m_plfFont; 
}

CFont* CGridCell::GetFontObject() const
{
    // If the default font is specified, use the default cell implementation
    if (m_plfFont == NULL)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return NULL;

        return pDefaultCell->GetFontObject();
    }
    else
    {
        static CFont Font;
        Font.DeleteObject();
        Font.CreateFontIndirect(m_plfFont);
        return &Font;
    }
}

DWORD CGridCell::GetFormat() const
{
    if (m_nFormat == (DWORD)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetFormat();
    }

    return m_nFormat; 
}

UINT CGridCell::GetMargin() const           
{
    if (m_nMargin == (UINT)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetMargin();
    }

    return m_nMargin; 
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Operations

BOOL CGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    if ( m_bEditing )
	{   
		if(m_pEditWnd)
			m_pEditWnd->SendMessage ( WM_CHAR, nChar );    
    }  
	else  
	{   
		CGridCtrl* pGrid = GetGrid();
		ASSERT(pGrid);
		ASSERT(nCol<pGrid->aColumns.GetSize());
		if(!pGrid->aColumns[nCol].bEnable)
			return FALSE;

		m_Value=pGrid->GetItemValue(nRow,nCol);

		if(m_Value.nType==0)
			m_Value=GetTypeValue(pGrid->aColumns[nCol].csType);
		if(m_Value.nType==0) 
			m_Value=String("");

		DWORD dwStyle = ES_LEFT;
		if (GetFormat() & DT_RIGHT)// || m_Value.nType==TYPE_NUMBER) 
			dwStyle = ES_RIGHT;
		else if (GetFormat() & DT_CENTER) 
			dwStyle = ES_CENTER;
		
		m_bEditing = TRUE;
		
		if(m_Value.nType>TYPE_DATE)
			nChar=VK_ENTER; 

		int nFormat=0;

		CCell *pExtCell=GetCell();
		if(pExtCell)
			nFormat=pExtCell->nHPosition;

		// InPlaceEdit auto-deletes itself
		m_pEditWnd = new CInPlaceEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetValue().GetString(), nChar, m_Value.GetType(),nFormat);
		if(!m_pEditWnd)
			return FALSE;
		if(m_Value.nType>TYPE_DATE)
		{
			CValue Val;
			CString Str;
			m_Value.SaveToString(Str);
			Val.LoadFromString(Str);
			((CInPlaceEdit*)m_pEditWnd)->SetValue(Val);

		}
		else
		{
			((CInPlaceEdit*)m_pEditWnd)->SetValue(m_Value);
		}
    }
    return TRUE;
}

void CGridCell::EndEdit()
{
    if (m_pEditWnd)
	{
        ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
		m_pEditWnd=0;
	}
}

void CGridCell::OnEndEdit()
{
    m_bEditing = FALSE;
    m_pEditWnd = NULL;
}

LPCTSTR CGridCell::GetText() const
{ 
	CCell *pExtCell=GetCell();
	if(pExtCell)
	{
		return pExtCell->csText;
	}

#ifdef _DEBUG
	((CGridCell*)this)->m_strText=m_Value.GetString();
	return m_strText;
#endif

	if(m_Value.nType==TYPE_STRING)
		return m_Value.sData;
	else
	{
		if(m_Value.GetType()==TYPE_NUMBER)
			if(m_Value.GetNumber()==0)
				return _T("");
		return m_Value.GetString();
	}
};

void  CGridCell::SetText(LPCTSTR szText)
{ 
	CCell *pExtCell=GetCell();
	if(pExtCell)
	{
		pExtCell->csText=szText;
	}
	else
	{
		m_Value.SetData(String(szText));
	}
};
void  CGridCell::SetValue(CValue &Val)
{ 
	CCell *pExtCell=GetCell();
	if(pExtCell)
	{
		pExtCell->csText=Val.GetString();
	}
	else
	{
		m_Value = Val;
	}
};
CValue CGridCell::GetValue()const	            
{ 
	CCell *pExtCell=GetCell();
	if(pExtCell)
	{
		return String(pExtCell->csText);
	}
	else
	{
		return m_Value; 
	}
}



/////////////////////////////////////////////////////////////////////////////
// CGridDefaultCell

CGridDefaultCell::CGridDefaultCell() 
{
#ifdef _WIN32_WCE
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
#else
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX | DT_END_ELLIPSIS;
#endif
    m_crFgClr = CLR_DEFAULT;
    m_crBkClr = CLR_DEFAULT;
    m_Size    = CSize(30,10);
    m_dwStyle = 0;

#ifdef _WIN32_WCE
    LOGFONT lf;
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
    SetFont(&lf);
#else // not CE
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    SetFont(&(ncm.lfMessageFont));
#endif

	m_nCoveredRow=-1;
	m_nCoveredCol=-1;
	m_Value.Reset();
}

CGridDefaultCell::~CGridDefaultCell()
{
    m_Font.DeleteObject(); 
}

void CGridDefaultCell::SetFont(const LOGFONT* plf)
{
    ASSERT(plf);

    if (!plf) return;

    m_Font.DeleteObject();
    m_Font.CreateFontIndirect(plf);

    CGridCell::SetFont(plf);

    // Get the font size and hence the default cell size
    CDC* pDC = CDC::FromHandle(::GetDC(NULL));
    if (pDC)
    {
        CFont* pOldFont = pDC->SelectObject(&m_Font);

        SetMargin(pDC->GetTextExtent(_T(" "), 1).cx);
        m_Size = pDC->GetTextExtent(_T(" XXXXXXXXXXXX "), 14);
        m_Size.cy = (m_Size.cy * 3) / 2;

        pDC->SelectObject(pOldFont);
        ReleaseDC(NULL, pDC->GetSafeHdc());
    }
    else
    {
        SetMargin(3);
        m_Size = CSize(40,16);
    }
}

LOGFONT* CGridDefaultCell::GetFont() const
{
    ASSERT(m_plfFont);  // This is the default - it CAN'T be NULL!
    return m_plfFont;
}

CFont* CGridDefaultCell::GetFontObject() const
{
    ASSERT(m_Font.GetSafeHandle());
    return (CFont*) &m_Font; 
}

LPCTSTR CGridDefaultCell::GetText() const
{ 
	return _T("");
};

