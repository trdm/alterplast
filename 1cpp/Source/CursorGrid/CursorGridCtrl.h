/////////////////////////////////////////////////////////////////////
// CursorGrid.h : interface for CursorGrid control.
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Copyright (c) 2005-2008. All Rights Reserved.
// Based on CVirtualGridCtrl class written by Uri Mellshior, (c) 2002-2003.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURSORGRIDCTRL_H__0F1BED66_89DD_46FC_860E_1C863A61C957__INCLUDED_)
#define AFX_CURSORGRIDCTRL_H__0F1BED66_89DD_46FC_860E_1C863A61C957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "commons.h"
#include "DataProvider.h"

#define IS_SHIFT_PRESSED()	((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT) * 8 - 1))) != 0)
#define IS_CTRL_PRESSED()	((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT) * 8 - 1))) != 0)
#define IS_TAB_PRESSED()	((GetKeyState(VK_TAB) & (1 << (sizeof(SHORT) * 8 - 1))) != 0)

#define CURSOR_GRID_CLASS_NAME			_T("CursorGrid")

enum RowSelectMode {rsmCell, rsmRow, rsmBoth};
enum SelectMode {smSingleLine, smMultiLine};
enum ScrollerPresent {spAuto, spAlways, spNever};
enum StartView {svFromBegin, svFromEnd};
enum HorizTextAlignment {htaAuto, htaLeft, htaRight, htaCenter};
enum HeadersStyle {hs3D, hsFlat};
enum ActiveHeaderStyle {ahsNone, ahsSelected};
enum InternalImages
{
    iiCheckBoxUnchecked, iiCheckBoxChecked, iiCheckBoxUndefined,
    iiOrderAsc, iiOrderDesc, iiOrderAsc2, iiOrderDesc2, iiOrderAsc3, iiOrderDesc3,
    iiBottomReposArrow, iiTopReposArrow, iiHelperIcon,
	iiSmallCheckBoxUnckecked, iiSmallCheckBoxChecked, iiSmallCheckBoxUndefined
};
enum OrderMarker {omNone, omAsc, omDesc, omAsc2, omDesc2, omAsc3, omDesc3};
enum GridHitTest {ghtHeader, ghtCell, ghtCheckBox, ghtSpace};
enum DropTargetView {dtvNone, dtvCell, dtvRow, dtvColumn};
enum QuickSearchResult {qsrCancel, qsrOK};

class CGridColumn;
class CGridColumns;
class CGridItem;
class CGridRow;
class CCursorGridCtrl;
class CReposColumnWnd;
class CReposPlaceWnd;

/////////////////////////////////////////////////////////////////////////////
// CGridColumn
/////////////////////////////////////////////////////////////////////////////

class CGridColumn : public CCollectionItem
{
    friend class CCursorGridCtrl;
    friend class CGridColumns;
    friend class CGridRow;
    friend class CGridItem;
    friend class CReposColumnWnd;
    friend class CReposPlaceWnd;
    friend class CColumnHelper;
    friend class CCGDropTarget;
public:
    CGridColumn();
    virtual ~CGridColumn();
    
    int GetWidth() const;
    void SetWidth(int nWidth);

	int GetAutoWidth() const;

    CString GetCaption() const;
    void SetCaption(LPCTSTR szCaption);
    
    BOOL GetVisible() const;
    void SetVisible(BOOL bVisible = TRUE);

    BOOL GetFixedWidth() const;
    void SetFixedWidth(BOOL bFixedWidth = TRUE);

    CFont* GetHeaderFont() const;
    void SetHeaderFont(CFont* pFont);
    
    COLORREF GetHeaderTextColor() const;
    void SetHeaderTextColor(COLORREF crTextColor);

    COLORREF GetHeaderBackColor() const;
    void SetHeaderBackColor(COLORREF crBackColor);

    HorizTextAlignment GetHTextAlignment() const;
    void SetHTextAlignment(HorizTextAlignment textAlignment);

    COLORREF GetTextColor() const;
    void SetTextColor(COLORREF crTextColor);

    COLORREF GetBackColor() const;
    void SetBackColor(COLORREF crBackColor);

    CFont* GetFont() const;
    void SetFont(CFont* pFont);
    
    CImageList* GetRowsImages() const;
    void SetRowsImages(CImageList* pRowsImages);
    
    LPCSTR GetDataField() const;
    void SetDataField(LPCSTR szDataField);
    
    OrderMarker GetOrderMarker() const;
    void SetOrderMarker(OrderMarker orderMarker);

    BOOL GetChangeVisible() const;
    void SetChangeVisible(BOOL bChangeVisible = TRUE);

protected:
    virtual void Release() {delete this;};
private:
    //Common
    int m_nMinWidth;
    CString m_strCaption;
    BOOL m_bVisible;
    BOOL m_bChangeVisible;

    //Internal
    CRect m_rect;
    CString m_strDataField;

    //Header
    COLORREF m_crHeaderText;
    COLORREF m_crHeaderBack;
    CFont* m_pHeaderFont;
    OrderMarker m_OrderMarker;

    //Cells
    HorizTextAlignment m_HTextAlignment;
    BOOL m_bFixedWidth;
    COLORREF m_crText;
    COLORREF m_crBack;
    CFont* m_pFont;
    CImageList* m_pRowsImages;
    int m_nImageWidth;

    CRect GetBoundsRect();
    void RedrawHeader();
    void RedrawGrid();
    void UpdateColumns(BOOL bRefill = FALSE);
};

/////////////////////////////////////////////////////////////////////////////
// CGridColumns
/////////////////////////////////////////////////////////////////////////////

class CGridColumns : private CCollection
{
    friend class CCursorGridCtrl;
    friend class CGridColumn;
    friend class CGridRow;
public:
    CGridColumns();
    virtual ~CGridColumns();

    int GetSize() const;
    CGridColumn* Add(LPCSTR szName);
    CGridColumn* Insert(int nIndex, LPCSTR szName);
    CGridColumn* GetAt(int nIndex) const;
    CGridColumn* GetAt(LPCSTR szName) const;
    int IndexOf(LPCSTR szName) const;
    int IndexOf(CGridColumn* pColumn) const;
    BOOL Remove(int nIndex);
    void RemoveAll();
    BOOL Move(int nIndex, int nOffset);
	int GetVisibleWidth() const;
protected:
    virtual void Release() {delete this;};
    virtual CGridColumn* NewColumn() {return new CGridColumn();};
    virtual void OnAdd(CCollectionItem* pItem);
    virtual void OnRemove(CCollectionItem* pItem, int nIndexWas);
    virtual void OnRename(CCollectionItem* pItem);

private:
    CCursorGridCtrl* m_pGrid;
    int m_nMinWidth;
    int m_nResizableMinWidth;

    void RecalcVisibles();
	void QueryDataField(CGridColumn* pColumn) const;
	void QueryDataField(CGridColumn* pColumn, CDataProvider* pProvider) const;
	void ReleaseDataField(CGridColumn* pColumn) const;
	void ReleaseDataField(CGridColumn* pColumn, CDataProvider* pProvider) const;
};

/////////////////////////////////////////////////////////////////////////////
// CGridItem
/////////////////////////////////////////////////////////////////////////////

class CGridItem
{
    friend class CCursorGridCtrl;
    friend class CGridRow;
public:
    CGridItem(CGridColumn* pGridColumn);
    virtual ~CGridItem();
    
    CGridColumn* GetColumn() const;

    BOOL GetShowText() const;
    void SetShowText(BOOL bEnable = TRUE);

    BOOL GetShowImage() const;
    void SetShowImage(BOOL bEnable = TRUE);

    BOOL GetShowCheckBox() const;
    void SetShowCheckBox(BOOL bEnable = TRUE);

    LPCSTR GetText() const;
    void SetText(LPCSTR szText);

    HorizTextAlignment GetTextHAlign() const;
    void SetTextHAlign(HorizTextAlignment horizTextAlignment);

    int GetImageIndex() const;
    void SetImageIndex(int nImageIndex);

    int GetCheckValue() const;
    void SetCheckValue(int nCheckValue);

    COLORREF GetTextColor() const;
    void SetTextColor(COLORREF crTextColor);

    COLORREF GetBackColor() const;
    void SetBackColor(COLORREF crBackColor);

    CFont* GetFont() const;
    void SetFont(CFont* pFont);
protected:
    virtual BOOL DrawImage(CDC* pDC, CRect& rect) {return FALSE;};
private:
    CGridColumn* m_pGridColumn;
    WORD m_wFlagsMask;
    short m_nImageNumber;
    CString m_strText;
    COLORREF m_crText;
    COLORREF m_crBack;
    CFont* m_pFont;
};

/////////////////////////////////////////////////////////////////////////////
// CGridRow
/////////////////////////////////////////////////////////////////////////////

class CGridRow
{
    friend class CGridColumns;
    friend class CCursorGridCtrl;
    friend class CCGDropTarget;
public:
    CGridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rType);
    virtual ~CGridRow();

    COLORREF GetTextColor() const;
    void SetTextColor(COLORREF crTextColor);

    COLORREF GetBackColor() const;
    void SetBackColor(COLORREF crBackColor);

    CFont* GetFont() const;
    void SetFont(CFont* pFont);
    
    CCursorGridCtrl* GetGrid() const;
    CDataRow* GetDataRow() const;
    CGridItem* GetGridItem(int nIndex) const;

 	RegionType GetRegionType() const { return m_rType; };
protected:
    virtual CGridItem* NewItem(CGridColumn* pGridColumn) {return new CGridItem(pGridColumn);};
    virtual void OnFillItems() {};

private:
    void FillItems();
    void Move(int nIndex, int nOffset);

    BOOL CleanUp();
    CGridColumns* m_pColumns;
    CTypedPtrArray<CPtrArray, CGridItem*> m_cells;
    CDataRow* m_pDataRow;
    bool m_bIsCurrent;
    
    COLORREF m_crText;
    COLORREF m_crBack;
    CFont* m_pFont;

	RegionType m_rType;
};

/////////////////////////////////////////////////////////////////////////////
// CDataRowState
// used in the row selection section to temporarily store state of the data row
/////////////////////////////////////////////////////////////////////////////
class CDataRowState
{
	friend class CCursorGridCtrl;

	CDataRowState(CCursorGridCtrl* pGrid);
	virtual ~CDataRowState();
private:
	CDataRow* m_pDataRow;
	int m_nDataRowIndex;
	int m_nDataRowDirection;
	int m_nSelectDirection;
	BOOL m_bIsRemoved;
	BOOL m_bIsCurrent;
	int m_nSize;
};

/////////////////////////////////////////////////////////////////////////////
// CSelectedRows
/////////////////////////////////////////////////////////////////////////////

class CSelectedRows
{
//     friend class CCursorGridCtrl;
public:
    CSelectedRows();
    virtual ~CSelectedRows();

	void SetDataProvider(CDataProvider* pProvider);

	BOOL IsSelected(CDataRow* pDataRow) const;
	BOOL IsRemoved(CDataRow* pDataRow) const;
	BOOL IsCurrent(CDataRow* pDataRow) const;
    void Add(CDataRow* pDataRow, BOOL bCheckIsSelected = TRUE);
    BOOL Remove(CDataRow* pDataRow);
	void Toggle(CDataRow* pDataRow);
	void RemoveAll();
	void SetRemovedRow(CDataRow* pDataRow);
	void SetCurrentRow(CDataRow* pDataRow);
	void SetDirection(int nDirection) {m_nDirection = nDirection;};
	int GetDirection() const {return m_nDirection;};
	int GetSize() const;
	CDataRow* GetAt(int nIndex) const;
// protected:
//     virtual void Release() { delete this; };
private:
	CDataProvider* m_pProvider;
    CTypedPtrArray<CPtrArray, CDataRow*> m_rows;
	CDataRow* m_pRemovedRow;
	CDataRow* m_pCurrentRow;
	int m_nDirection;
};

/////////////////////////////////////////////////////////////////////////////
// CGridTip window
/////////////////////////////////////////////////////////////////////////////

#define GRIDTIP_CLASSNAME _T("CursorGridTip")

class CGridTip : public CWnd
{
    friend class CCursorGridCtrl;
public:
    CGridTip(CCursorGridCtrl* pGrid);
    virtual ~CGridTip();

    void Show(const CString& strText, LPRECT pTextRect, DWORD dwTipID,
        CFont* pFont, BOOL bByCursor);
    void Hide();
    
	//{{AFX_VIRTUAL(CGridTip)
    protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

private:
    CCursorGridCtrl* m_pGrid;
    COLORREF m_crTextColor;
    COLORREF m_crBackColor;
    BOOL m_bCreated;
    CRect m_rectHover;
    DWORD m_dwTipedID;
    
protected:
	//{{AFX_MSG(CGridTip)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CQSEdit window
/////////////////////////////////////////////////////////////////////////////

/*
// class CQSEdit : public CEdit
// {
// public:
//     CQSEdit();
//     virtual ~CQSEdit();
// 
// 	//{{AFX_VIRTUAL(CQSEdit)
//     protected:
//     virtual BOOL PreTranslateMessage(MSG* pMsg);
// 	//}}AFX_VIRTUAL
// protected:
// 	//{{AFX_MSG(CQSEdit)
// 	afx_msg void OnKillFocus(CWnd* pNewWnd);
// 	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
// 	//}}AFX_MSG
// 	DECLARE_MESSAGE_MAP()
// 
// };
*/

/////////////////////////////////////////////////////////////////////////////
// CQuickSearch window
/////////////////////////////////////////////////////////////////////////////

#define QUICKSEARCH_CLASSNAME _T("QuickSearch")

class CQuickSearch : public CWnd
{
    friend class CCursorGridCtrl;
//     friend class CQSEdit;
public:
    CQuickSearch(CCursorGridCtrl* pGrid);
    virtual ~CQuickSearch();

    BOOL IsWorking() const;
    void Show(LPRECT pRect, LPCSTR szFieldName, LPCSTR szInitStr);
    void Hide();
    void Search();
	BOOL IsDirty();

	//{{AFX_VIRTUAL(CQuickSearch)
    protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CQuickSearch)
	afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnEditKillFocus();
	afx_msg void OnEditChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    CCursorGridCtrl* m_pGrid;
    BOOL m_bIsWorking;
    CEdit m_edit;
    CString m_strFieldName;
    USHORT m_nTimeout;

//    BOOL m_bIsDirty;
	CString m_strPattern;
};

/////////////////////////////////////////////////////////////////////////////
// CReposColumnWnd window
/////////////////////////////////////////////////////////////////////////////

#define REPOSCOLUMN_CLASSNAME _T("ReposColumn")

class CReposColumnWnd : public CWnd
{
    friend class CReposPlaceWnd;
public:
    CReposColumnWnd(CCursorGridCtrl* pGrid);
    virtual ~CReposColumnWnd();

    void Show(CGridColumn* pColumn, CSize& Offset);

	//{{AFX_VIRTUAL(CReposColumnWnd)
    protected:
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CReposColumnWnd)
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CCursorGridCtrl* m_pGrid;
    CSize m_Offset;
    CGridColumn* m_pCapturedColumn;
    int m_nCapturedIndex;
    int m_nShift;
    BOOL m_bChangeVisible;
    BOOL m_bArrowsAreDrawn;
    CPoint m_ptLastArrows;

    void DrawArrows(int nShift, BOOL bChangeVisible);
    void HideArrows();
};

/////////////////////////////////////////////////////////////////////////////
// CColumnHelper window
/////////////////////////////////////////////////////////////////////////////

#define COLUMNHELPER_CLASSNAME _T("ColumnHelper")

class CColumnHelper : public CWnd
{
public:
    CColumnHelper(CCursorGridCtrl* pGrid);
    ~CColumnHelper();

    BOOL IsDisplay();
    void Show();
    void Hide();
	//{{AFX_VIRTUAL(CColumnHelper)
    protected:
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CColumnHelper)
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
    afx_msg void OnNcPaint();
    afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    int GetColumnWidth(CGridColumn* pColumn);

    CCursorGridCtrl* m_pGrid;
    BOOL m_bDisplay;
    CTypedPtrArray<CPtrArray, CGridColumn*> m_columns;
};

/////////////////////////////////////////////////////////////////////////////
// CCGDropTarget
/////////////////////////////////////////////////////////////////////////////
class CCGDropTarget : public COleDropTarget
{
    friend class CCursorGridCtrl;
public:
    CCGDropTarget(CCursorGridCtrl* pGrid);
	~CCGDropTarget();
    void Enable();
    void Disable();
    virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave(CWnd* pWnd);
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point );
	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
private:
    void DrawDragRect(CRect& rect);
    BOOL CompareRows(CDataRow* pRow1, RegionType rType1, CDataRow* pRow2, RegionType rType2);
	void ResetScroll();
	void SetDropOverRow(CDataRow* pRow, RegionType rType);
	void ReleaseDropOverRow() { SetDropOverRow(NULL, rtNone); };

    CCursorGridCtrl* m_pGrid;
    DROPEFFECT m_dwDropEffect;
    CGridColumn* m_pDropOverColumn;
    CDataRow* m_pDropOverRow;
	RegionType m_DropOverRegionType;
    BOOL m_bDragOn;
    CRect m_rectTarget;
	DWORD m_dwKeyState;
    DropTargetView m_SelfDropTargetView;
	BOOL m_bScrollDelay;
	DWORD m_dwScrollStart;
};

/////////////////////////////////////////////////////////////////////////////
// CCursorGridCtrl window
/////////////////////////////////////////////////////////////////////////////

class CCursorGridCtrl : public CWnd
{
    friend class CGridColumn;
    friend class CGridColumns;
    friend class CGridRow;
    friend class CGridTip;
    friend class CQuickSearch;
    friend class CReposColumnWnd;
    friend class CReposPlaceWnd;
    friend class CColumnHelper;
    friend class CCGDropTarget;
	friend class CDataRowState;
    DECLARE_DYNCREATE(CCursorGridCtrl)

private:
	class CUpdateLock
	{
	private:
		CUpdateLock& operator=(CUpdateLock const&);
	public:
		CUpdateLock(CUpdateLock const& rhs): m_nCount(rhs.m_nCount), m_bReq(rhs.m_bReq), m_pGrid(rhs.m_pGrid){ ++m_nCount; }; // vc6 issue
		CUpdateLock(UINT& nCount, BOOL& bReq, CCursorGridCtrl* pGrid) : m_nCount(nCount), m_bReq(bReq), m_pGrid(pGrid) { ++m_nCount; };
		~CUpdateLock() { if (!--m_nCount && m_bReq) m_pGrid->Update(); };

		// 		UINT GetCount() const { return m_nCount; };
	private:
		UINT& m_nCount;
		BOOL& m_bReq;
		CCursorGridCtrl* m_pGrid;
	};

	class CDrawLock
	{
	private:
		CDrawLock& operator=(CDrawLock const&);
	public:
		CDrawLock(CDrawLock const& rhs): m_nCount(rhs.m_nCount), m_bReq(rhs.m_bReq), m_pWnd(rhs.m_pWnd){ ++m_nCount; }; // vc6 issue
		CDrawLock(UINT& nCount, BOOL& bReq, CWnd* pGrid) : m_nCount(nCount), m_bReq(bReq), m_pWnd(pGrid) { ++m_nCount; };
		~CDrawLock() { if (!--m_nCount && m_bReq) m_pWnd->Invalidate(); };

// 		UINT GetCount() const { return m_nCount; };
	private:
		UINT& m_nCount;
		BOOL& m_bReq;
		CWnd* m_pWnd;
	};

public:
	CCursorGridCtrl();
	virtual ~CCursorGridCtrl();

    BOOL Create(const RECT& rect, CWnd *pParentWnd, UINT nID, BOOL bAutoDelete = FALSE, BOOL bVisible = TRUE, BOOL bDisabled = FALSE);

    CGridColumns* GetColumns() const;
    void RefreshRows();
	void RefreshRows_head();
	void RefreshRows_tail();

	void RedrawRows();

    CGridColumn* GetCurrentColumn() const;
    void SetCurrentColumn(CGridColumn* pColumn);
    
    CDataProvider* GetDataProvider(RegionType rType) const;

	virtual void SetDataProvider(CDataProvider* pProvider);
	virtual void SetDataProvider_head(CDataProvider* pProvider);
	virtual void SetDataProvider_tail(CDataProvider* pProvider);

    ScrollerPresent GetVScroller() const;
    void SetVScroller(ScrollerPresent scrollerPresent);

    StartView GetStartView() const;
    void SetStartView(StartView startView);

    RowSelectMode GetRowSelectMode() const;
    void SetRowSelectMode(RowSelectMode rowSelectMode);

    SelectMode GetSelectMode() const;
    void SetSelectMode(SelectMode selectMode);

    BOOL GetVerticalLines() const;
    void SetVerticalLines(BOOL bVerticalLines = TRUE);

    BOOL GetHorizontalLines() const;
    void SetHorizontalLines(BOOL bHorizontalLines = TRUE);

    HeadersStyle GetHeadersStyle() const;
    void SetHeadersStyle(HeadersStyle headersStyle);

    ActiveHeaderStyle GetActiveHeaderStyle() const;
    void SetActiveHeaderStyle(ActiveHeaderStyle style);

    USHORT GetRefreshTimeout() const;
    void SetRefreshTimeout(USHORT nTimeout);

    USHORT GetQuickSearchTimeout() const;
    void SetQuickSearchTimeout(USHORT nTimeout);

    BOOL GetChangePositionOfColumns() const;
    void SetChangePositionOfColumns(BOOL bChangePositionOfColumns = TRUE);

    COLORREF GetGridLinesColor() const;
    void SetGridLinesColor(COLORREF crGridLines);

    COLORREF GetTextColor() const;
    void SetTextColor(COLORREF crTextColor);

	COLORREF GetBackColor() const;
	void SetBackColor(COLORREF crBackColor);

	COLORREF GetTextColor_head() const;
	void SetTextColor_head(COLORREF crTextColor);

	COLORREF GetBackColor_head() const;
	void SetBackColor_head(COLORREF crBackColor);

	COLORREF GetTextColor_tail() const;
	void SetTextColor_tail(COLORREF crTextColor);

	COLORREF GetBackColor_tail() const;
	void SetBackColor_tail(COLORREF crBackColor);

	COLORREF GetSelectionTextColor() const;
    void SetSelectionTextColor(COLORREF crTextColor);

    COLORREF GetSelectionBackColor() const;
    void SetSelectionBackColor(COLORREF crBackColor);

    COLORREF GetSelectionTextColor2() const;
    void SetSelectionTextColor2(COLORREF crTextColor);

    COLORREF GetSelectionBackColor2() const;
    void SetSelectionBackColor2(COLORREF crBackColor);

    COLORREF GetAlternationRowBackColor() const;
    void SetAlternationRowBackColor(COLORREF crBackColor);

    BOOL GetUseAlternationRowColor() const;
    void SetUseAlternationRowColor(BOOL bUse = TRUE);
    
    void ActivateRow(CDataRow* pDataRow);

    CPoint GetContextMenuPosition() const;

    BOOL GetEnableDrag() const;
    void SetEnableDrag(BOOL bEnableDrag = TRUE);

    BOOL GetEnableDrop() const;
    void SetEnableDrop(BOOL bEnableDrop = TRUE);

    DropTargetView GetSelfDropTargetView() const;
    void SetSelfDropTargetView(DropTargetView dropTargetView);

    BOOL GetShowHeader() const;
    void SetShowHeader(BOOL bShowHeader);

	UINT GetFixedLeft() const;
	void SetFixedLeft(int nFixed);
	UINT GetFixedRight() const;
	void SetFixedRight(int nFixed);

	UINT GetLeftTextMargin() const {return m_nLeftTextMargin;};
	UINT GetRightTextMargin() const {return m_nRightTextMargin;};
	UINT GetTopTextMargin() const {return m_nTopTextMargin;};
	UINT GetBottomTextMargin() const {return m_nBottomTextMargin;};
	void SetTextMargins(int nLeft, int nTop, int nRight, int nBottom);

	BOOL GetCurrentCellBounds(CRect &rect);
	CDataRow* GetCurrentDataRow() const;

	int GetColumnAutoWidth(CGridColumn* pGridColumn);
	void AutoWidth();

protected:
	CSelectedRows* GetSelectedRows();
	static COLORREF aux_ShiftColor(COLORREF cr, int nShift);

	virtual CGridColumns* NewColumns() {return new CGridColumns;};
    virtual CGridRow* NewGridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rType) {return new CGridRow(pColumns, pDataRow, rType);};

	//+ refactoring suggested
	int GetFixedWidth_left() const;
	int GetFixedWidth_right() const;
	CRect GetPageRect() const;
	CRect GetColumnBoundsRect(CGridColumn* pGridColumn);
	CRect GetColumnClipRect(CGridColumn* pGridColumn);
	//-

    //Events
    virtual void OnChangeCurrentData(CDataRow* pDataRow) {};
    virtual void OnSelection(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType) {};
    virtual void OnActivateRow() {};
    virtual void OnActivateColumn() {};
    virtual void OnActivateCell() {};
    virtual void OnHeaderSelection(CGridColumn* pColumn) {};
    virtual void OnShowContextMenu(RegionType rType, CGridColumn* pColumn, CDataRow* pDataRow) {};
    virtual void OnClickCheckBox(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType) {};
	virtual void OnBeginDrag(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, DropTargetView& dropTargetView) {return DROPEFFECT_MOVE;};
    virtual void OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType) {};
    virtual void OnDestroy() {};
	virtual BOOL OnQuickSearchStart(DataType dt) { return TRUE; };
	virtual BOOL OnQuickSearch(CString& str) { return TRUE; };
	virtual void OnQuickSearchEnd(QuickSearchResult result, BOOL bPositionChanged) {};
	virtual BOOL OnColumnMove(CGridColumn* pColumn, int nShift) { return TRUE; };
	virtual BOOL OnColumnVisibilityChange(CGridColumn* pColumn, BOOL bNewVisible) { return TRUE; };
	virtual void OnColumnWidthChange() {};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCursorGridCtrl)
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CCursorGridCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg LRESULT OnGetFont(WPARAM wparam, LPARAM lparam);
    afx_msg LRESULT OnSetFont(WPARAM wparam, LPARAM lparam);
    afx_msg LRESULT OnDataChange(WPARAM wparam, LPARAM lparam);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    BOOL m_bAutoDelete;
    void InitFont(CFont* pFont);
    void Paint(CDC *pDC);
    CRect GetHeaderRect() const;
    CRect GetGridRect(RegionType rType = rtNone) const;
    int GetHeaderHeight() const;
    void PaintHeaders(CDC *pDC);
	void PaintGridLines(CDC *pDC, CRect& rect, int nFullVisibleRows);
    void PaintGridLines(CDC *pDC);
	void PaintCells(CDC *pDC);
	void Paint3DFrame_raised(CDC *pDC, LPRECT lpRect, UINT nSideFlags);
	void Paint3DFrame_sunken(CDC *pDC, LPRECT lpRect, UINT nSideFlags);
    void DrawHeader(CDC *pDC, CGridColumn* pColumn, LPRECT pRect, LPRECT pClipRect);
	void EvalCellColors(CGridItem* pGridItem, CGridRow* pGridRow, BOOL bSelected, BOOL bAlterSelected, BOOL bAlterBack, COLORREF& crText, COLORREF& crBack);
	void EvalCellColors_head(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack);
	void EvalCellColors_tail(CGridItem* pGridItem, CGridRow* pGridRow, COLORREF& crText, COLORREF& crBack);
	CFont* EvalCellFont(CGridItem* pGridItem, CGridRow* pGridRow);
    void DrawCell(CDC* pDC, LPRECT pRect, LPRECT pClipRect, CGridItem* pGridItem, COLORREF crText, COLORREF crBack, CFont* pFont);
    void RecalcColumnsWidth();
    void SetHScrollInfo();
    CGridColumn* GetResizeHeaderAt(CPoint point);
    void StepColResize(int nNewWidth);
	void EndColResize();
    void CalcRowsParams();
public:
    void Update();
private:
	void Update_head();
	void Update_tail();
    void ClearRows();
	void ClearRows_head();
	void ClearRows_tail();
	void QueryFields(CDataProvider* pProvider);
	void SetVScrollInfo();
    void GetRowsUp(int nCount);
    void GetRowsDown(int nCount);
    BOOL ScrollDown(int nPageSize);
    BOOL ScrollUp(int nPageSize);
    int HitTestColumn(CPoint point);
	int HitTestHeader(CPoint point);
    void SetCurrentCell(int nRowIndex, int nColumnIndex);
    CGridRow* NewGridRow(CDataRow* pDataRow, int nNewRowIndex);
    int GetNextColumn(int nIndex, int nDirection);
    int GetPageSize();
    void ToHome();
    void ToEnd();
    BOOL IsFocused();
    int GetOrigin();
    void ColumnsChanged();
    void PaintError(CDC* pDC);
    void ShowHint(CPoint point);
    void FillRows(int nFirst, int nCount);
    void Reset(CDataRow* pActRow = NULL);
    BOOL DisplayColumn(CGridColumn* pColumn);
    BOOL ShowColumnHelper(CPoint point);
    COLORREF GetLinesColor();
	unsigned GetTopRowIndex(RegionType rType) const;
	RegionType TestRegionType(CPoint point, int& nRowIndex) const;
	GridHitTest HitTestCheckBox(CPoint point, int nColumn, int& nRowIndex, RegionType rType);
	GridHitTest HitTest(CPoint point, int& nColumn, int& nRowIndex, RegionType& rType);
    void SetCurrent(int nRow, int nColumn);
    BOOL HScrollLine(SCROLLINFO& si, int nDirection);
    void PaintDropTarget(CDC* pDC);
	void ScrollCurrentCell(int xDirection, int yDirection);
	void SelectRow(CDataRowState& st);
 	void SelectRows(CDataRowState& st, int nDirection = 0, BOOL bOneRow = FALSE);
	unsigned GetVisibleRows(RegionType rType) const;
	unsigned GetFullVisibleRows(RegionType rType) const; // visible & populated
	CGridRow* GetGridRow(RegionType rType, int nRowIndex) const;
protected:
	CUpdateLock CreateUpdateLock() { return CUpdateLock(m_nUpdateLocks, m_bUpdateReq, this); };
private:
	CDrawLock CreateDrawLock() { return CDrawLock(m_nDrawLocks, m_bDrawReq, this); };

    CFont m_font;
    COLORREF m_crGridLines;
    CGridColumns* m_pColumns;
    BOOL m_bShowHeader;
    int m_nFontHeight;
    CPoint m_hitTest;
    CPoint m_hitMouseButton;
    CGridColumn* m_pCapturedColumn;
	int m_nCapturedColumn_left;
    enum {ctNone, ctResizeColumn, ctClickHeader, ctMoveColumn, ctDragDelay, ctScroll} m_CaptureType;
    ScrollerPresent m_VScrollerPresent;
    CDataProvider* m_pProvider;
	CDataProvider* m_pProvider_head;
	CDataProvider* m_pProvider_tail;
	int m_nAllVisibleRows; // all
    int m_nVisibleRows; // rtBody only
    BOOL m_bLastRowNotFull;
    int m_nRowHeight;
	CGridTip m_Tip;
    CTypedPtrArray<CPtrArray, CGridRow*> m_rows;
	CTypedPtrArray<CPtrArray, CGridRow*> m_rows_head;
	CTypedPtrArray<CPtrArray, CGridRow*> m_rows_tail;
	CSelectedRows m_SelectedRows;
    int m_nTopRowIndex;
    BOOL m_bIsBOF;
    BOOL m_bIsEOF;
    int m_nCurrentColumn;
    RowSelectMode m_RowSelectMode;
    SelectMode m_SelectMode;
    int m_nCurrentDataRowIndex;
	// possible direction of outbound row; valid only when m_nCurrentDataRowIndex == -1
    int m_nCurrentDataRowDirection;
    CDataRow* m_pCurrentDataRowOutOfWindow;
    BOOL m_bHorizontalLines;
    BOOL m_bVerticalLines;
    HeadersStyle m_HeadersStyle;
	ActiveHeaderStyle m_ActiveHeaderStyle;
    CImageList m_ilInternalImages;
    StartView m_StartView;
    DWORD m_dwLastTickCount;
    LPARAM m_nLastLParam;
    USHORT m_nRefreshTimeout;
    CQuickSearch m_QuickSearch;
    CReposColumnWnd m_ReposColumnWnd;
    CColumnHelper m_ColumnHelper;
    BOOL m_bChangePositionOfColumns;
    COLORREF m_crText;
    COLORREF m_crBack;
	COLORREF m_crText_head;
	COLORREF m_crBack_head;
	COLORREF m_crText_tail;
	COLORREF m_crBack_tail;
    COLORREF m_crSelectionText;
    COLORREF m_crSelectionBack;
    COLORREF m_crSelectionText2;
    COLORREF m_crSelectionBack2;
    COLORREF m_crAlternationRowBack;
    BOOL m_bUseAlternationRowColor;
    BOOL m_bEvenRow;
	BOOL m_bEnableDrag;
	BOOL m_bEnableDrop;
    CCGDropTarget m_DropTarget;
	CGridColumn* m_pDragColumn;
	CDataRow* m_pDragRow;
	RegionType m_DragRegionType;
	UINT m_nLeftTextMargin;
	UINT m_nTopTextMargin;
	UINT m_nRightTextMargin;
	UINT m_nBottomTextMargin;
	UINT m_nFixedLeft;
	UINT m_nFixedRight;
	UINT m_nDrawLocks;
	BOOL m_bDrawReq;
	UINT m_nUpdateLocks;
	BOOL m_bUpdateReq;
	CPoint m_ptContextMenuPosition;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURSORGRIDCTRL_H__0F1BED66_89DD_46FC_860E_1C863A61C957__INCLUDED_)
