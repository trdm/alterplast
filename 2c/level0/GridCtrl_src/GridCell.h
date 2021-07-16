/////////////////////////////////////////////////////////////////////////////
// GridCell.h : header file
//
// MFC Grid Control - Grid cell class header file
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
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGridCtrl;
#include "GridCellBase.h"

// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

CString GetCellString(CValue &Value);

class CGridCell : public CGridCellBase
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCell)

// Construction/Destruction
public:
    CGridCell();
    virtual ~CGridCell();

// Attributes
public:
    void operator=(const CGridCell& cell);

	CString &TextData()
	{
		m_Value.sData=m_Value.GetString();
		m_Value.nType=TYPE_STRING;
		return m_Value.sData;
	};
    virtual void  SetText(LPCTSTR szText);//        { m_Value.SetData(String(szText));};//m_strText = GetCellString(m_Value);}
    virtual void  SetValue(CValue &Val);//			 { m_Value = Val;};//m_strText = GetCellString(m_Value);}
    virtual void  SetData(CValue &Val)			 { m_Value.SetData(Val);};//m_strText=m_Value.GetString(); }
    virtual void  SetImage(int nImage)           { m_nImage = nImage;   }                        
    virtual void  SetData(LPARAM lParam)         { m_lParam = lParam;   }      
    virtual void  SetGrid(CGridCtrl* pGrid)      { m_pGrid = pGrid;     }                          
    // virtual void SetState(const DWORD nState);  -  use base class version   
    virtual void  SetFormat(DWORD nFormat)       { m_nFormat = nFormat; }                      
    virtual void  SetTextClr(COLORREF clr)       { m_crFgClr = clr;     }                          
    virtual void  SetBackClr(COLORREF clr)       { m_crBkClr = clr;     }                          
    virtual void  SetFont(const LOGFONT* plf);
    virtual void  SetMargin(UINT nMargin)        { m_nMargin = nMargin; }
    virtual CWnd* GetEditWnd() const             { return m_pEditWnd;   }
    virtual void  SetCoords( int /* nRow */, int /* nCol */) {}  // don't need to know the row and
	virtual void SetCell(class CCell *pValue);

	virtual void  SetCovered(int nCoveredRow,int nCoveredCol){pCoveredCell=0;m_nCoveredRow=nCoveredRow;m_nCoveredCol=nCoveredCol;};
	virtual void SetCoveredCell(CGridCellBase*p)
	{
		m_nCoveredRow=0;
		pCoveredCell=p;
	};
                                                                // column for base implementation

    virtual LPCTSTR     GetText() const;
    virtual CValue      GetValue()const;//	            { return m_Value; }
    virtual int         GetImage() const            { return m_nImage;  }
    virtual LPARAM      GetData() const             { return m_lParam;  }
    virtual CGridCtrl*  GetGrid() const             { return m_pGrid;   }
    // virtual DWORD    GetState() const - use base class
    virtual DWORD       GetFormat() const;
    virtual COLORREF    GetTextClr() const          { return m_crFgClr; } // TODO: change to use default cell
    virtual COLORREF    GetBackClr() const          { return m_crBkClr; }
    virtual LOGFONT*    GetFont() const;
    virtual CFont*      GetFontObject() const;
    virtual UINT        GetMargin() const;
	virtual class CCell *GetCell()	const{return m_pCell;};

	virtual BOOL IsCovered(){ return pCoveredCell || m_nCoveredRow>1 || m_nCoveredCol>1;}
	virtual int GetCoveredRow(){return m_nCoveredRow;};
	virtual int GetCoveredCol(){return m_nCoveredRow?m_nCoveredCol:0;};
	virtual CGridCellBase *GetCoveredCell()
	{
		if(m_nCoveredRow)
			return 0;
		else
			return (CGridCellBase *)(void*)pCoveredCell;
	};

    virtual BOOL        IsEditing() const           { return m_bEditing; }
    virtual BOOL        IsDefaultFont() const       { return (m_plfFont == NULL); }
    virtual void        Reset();

// editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
    virtual void OnEndEdit();

public:
#ifdef _DEBUG
    CString    m_strText;      // Cell text (or binary data if you wish...)
#endif
    CValue	   m_Value;      // Cell text (or binary data if you wish...)
    LPARAM     m_lParam;       // 32-bit value to associate with item
    int        m_nImage;       // Index of the list view item’s icon
    DWORD      m_nFormat;
    COLORREF   m_crFgClr;
    COLORREF   m_crBkClr;
    LOGFONT*   m_plfFont;
    UINT       m_nMargin;
	class CCell *m_pCell;
	int        m_nCoveredRow;
	union
	{
	int				m_nCoveredCol;
	CGridCellBase	*pCoveredCell;
	};

    bool       m_bEditing;     // Cell being edited?

    CGridCtrl* m_pGrid;        // Parent grid control
    CWnd*      m_pEditWnd;
};

// This class is for storing grid default values. It's a little heavy weight, so
// don't use it in bulk 
class CGridDefaultCell : public CGridCell
{
    DECLARE_DYNCREATE(CGridDefaultCell)

// Construction/Destruction
public:
    CGridDefaultCell();
    virtual ~CGridDefaultCell();

public:
    virtual DWORD GetStyle() const                      { return m_dwStyle;      }
    virtual void  SetStyle(DWORD dwStyle)               { m_dwStyle = dwStyle;   }
    virtual int   GetWidth() const                      { return m_Size.cx;      }
    virtual int   GetHeight() const                     { return m_Size.cy;      }
    virtual void  SetWidth(int nWidth)                  { m_Size.cx = nWidth;    }
    virtual void  SetHeight(int nHeight)                { m_Size.cy = nHeight;   }

    // Disable these properties
    virtual void     SetData(LPARAM /*lParam*/)             { ASSERT(FALSE);         }      
    virtual void     SetState(DWORD /*nState*/)             { ASSERT(FALSE);         }
    virtual DWORD    GetState() const                       { return CGridCell::GetState()|GVIS_READONLY; }
    virtual void     SetCoords( int /*row*/, int /*col*/)   { ASSERT(FALSE);         }
    virtual void     SetFont(const LOGFONT* /*plf*/);
    virtual LOGFONT* GetFont() const;   
    virtual CFont*   GetFontObject() const;
    virtual LPCTSTR	 GetText() const;
    //virtual void	 SetValue(CValue &Val);
    //virtual CValue	 GetValue()const;
protected:
    CSize m_Size;       // Default Size
    CFont m_Font;       // Cached font
    DWORD m_dwStyle;    // Cell Style - unused
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
