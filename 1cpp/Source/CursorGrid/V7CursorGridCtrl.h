//////////////////////////////////////////////////////////////////////
// V7CursorGridCtrl.h: interfaces for CursorGrid control for V7
// (the Table field) classes.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CursorGridCtrl.h"
#include "../mycontextbase.h"
#include "../BLCollection.h"
#include "V7DataProvider.h"
#include "../Font.h"
#include "../ValueCollection.h"
#include "../CtrlEvents.h"
#include "../V7Control.h"
#include "../MethodsWrapper.h"

class CV7GridColumns;
class CV7CursorGridCtrl;
class CV7RowAppearance;
class CCellAppearance;
class CGridCells;
class CV7RowData;
class CV7TableField;
class CV7SelectedRows;
class CV7FixedRows_head;
class CV7FixedRows_tail;

#define GET_V7_COLOR(crColor) (crColor == CLR_DEFAULT ? -1 : crColor)
#define SET_V7_COLOR(vValue) (vValue == -1L ? CLR_DEFAULT : (LONG)vValue.GetNumeric())

class CV7GridColumn : public CMyContextBase, public CGridColumn
{
    friend class CV7GridColumns;
    friend class CGridCells;
	DECLARE_DYNCREATE(CV7GridColumn)
public:
    CV7GridColumn();
    virtual ~CV7GridColumn();

    BOOL MethSetRowsPictures(CValue** ppValue);
    BOOL DefSetRowsPictures(int nParam, CValue* pValue)const;

    BOOL GetPropName(CValue& Value)const {Value = GetName(); return TRUE;};
    BOOL SetPropName(CValue const& Value) {SetName(Value.GetString()); return TRUE;};

    BOOL GetPropCaption(CValue& Value)const {Value = GetCaption(); return TRUE;};
    BOOL SetPropCaption(CValue const& Value) {SetCaption(Value.GetString()); return TRUE;};

    BOOL GetPropVisible(CValue& Value)const {Value = GetVisible(); return TRUE;};
    BOOL SetPropVisible(CValue const& Value) {SetVisible(Value.GetNumeric()); return TRUE;};

    BOOL GetPropWidth(CValue& Value)const {Value = GetWidth(); return TRUE;};
    BOOL SetPropWidth(CValue const& Value) {SetWidth(Value.GetNumeric()); return TRUE;};

    BOOL GetPropAutoWidth(CValue& Value)const {Value = GetAutoWidth(); return TRUE;};

    BOOL GetPropHorizontalAlignment(CValue& Value)const {Value = GetHTextAlignment(); return TRUE;};
    BOOL SetPropHorizontalAlignment(CValue const& Value);

    BOOL GetPropTextColor(CValue& Value)const {Value = GET_V7_COLOR(GetTextColor()); return TRUE;};
    BOOL SetPropTextColor(CValue const& Value) {SetTextColor(SET_V7_COLOR(Value)); return TRUE;};

    BOOL GetPropBackColor(CValue& Value)const {Value = GET_V7_COLOR(GetBackColor()); return TRUE;};
    BOOL SetPropBackColor(CValue const& Value) {SetBackColor(SET_V7_COLOR(Value)); return TRUE;};

    BOOL GetPropFont(CValue& Value)const {Value.AssignContext(m_pFont); return TRUE;};
    BOOL SetPropFont(CValue const& Value);

    BOOL GetPropHeaderFont(CValue& Value)const {Value.AssignContext(m_pHeaderFont); return TRUE;};
    BOOL SetPropHeaderFont(CValue const& Value);

    BOOL GetPropFixedWidth(CValue& Value)const {Value = !GetFixedWidth(); return TRUE;};
    BOOL SetPropFixedWidth(CValue const& Value) {SetFixedWidth(Value.GetNumeric() == 0); return TRUE;};

    BOOL GetPropHeaderTextColor(CValue& Value)const {Value = GET_V7_COLOR(GetHeaderTextColor()); return TRUE;};
    BOOL SetPropHeaderTextColor(CValue const& Value) {SetHeaderTextColor(SET_V7_COLOR(Value)); return TRUE;};

    BOOL GetPropHeaderBackColor(CValue& Value)const {Value = GET_V7_COLOR(GetHeaderBackColor()); return TRUE;};
    BOOL SetPropHeaderBackColor(CValue const& Value) {SetHeaderBackColor(SET_V7_COLOR(Value)); return TRUE;};

    BOOL GetPropDataField(CValue& Value)const {Value = GetDataField(); return TRUE;};
    BOOL SetPropDataField(CValue const& Value) {SetDataField(Value.GetString()); return TRUE;};

    BOOL GetPropOrderMarker(CValue& Value)const {Value = GetOrderMarker(); return TRUE;};
    BOOL SetPropOrderMarker(CValue const& Value);

    BOOL GetPropChangeVisible(CValue& Value)const {Value = GetChangeVisible(); return TRUE;};
    BOOL SetPropChangeVisible(CValue const& Value) {SetChangeVisible(Value.GetNumeric()); return TRUE;};

protected:
    virtual void Release() {DecrRef();};

	DECLARE_MY_CONTEXT()
private:
    CBLPtr<CBLFont> m_pFont;
    CBLPtr<CBLFont> m_pHeaderFont;
    CBLPtr<CCellAppearance> m_pCellAppearance;
    CImageList m_RowsImages;
};

class CV7GridColumns : public CBLCollection<CGridColumns, CV7GridColumn, RUNTIME_CLASS(CV7GridColumn),
 BLC_MODIFED | BLC_INSERT | BLC_MOVE>
{
    DECLARE_DYNAMIC(CV7GridColumns)
public:
    CV7GridColumns();
    virtual ~CV7GridColumns();
protected:
    virtual void Release();
    virtual CGridColumn* NewColumn();
public:
    virtual char const* 	GetTypeString(void)const {return "КолонкиТабличногоПоля";};
};

class CV7GridItem : public CGridItem
{
    friend class CCellAppearance;
public:
    CV7GridItem(CGridColumn* pGridColumn);
    virtual ~CV7GridItem();
protected:
    virtual BOOL DrawImage(CDC* pDC, CRect& rect);
private:
    CBLPtr<CBLFont> m_pFont;
    CBLPtr<CPictureContext> m_pPicture;
};

class CV7GridRow : public CGridRow
{
    friend class CV7RowAppearance;
public:
    CV7GridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rt);
    virtual ~CV7GridRow();
protected:
    virtual CGridItem* NewItem(CGridColumn* pGridColumn);
    virtual void OnFillItems();
private:
    CBLPtr<CBLFont> m_pFont;
};

class CTempWndHelper
{
public:
	CTempWndHelper() : m_pWnd(NULL) {};
	~CTempWndHelper() {};

	void Create(CWnd* pParentWnd, CRect rect, DWORD nID)
	{
		m_pWnd = auto_ptr<CWnd>(new CWnd);
		m_pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
	}

private:
	auto_ptr<CWnd> m_pWnd;
};

class CV7CursorGridCtrl : public CCursorGridCtrl
{
    friend class CV7GridColumns;
    friend class CV7GridRow;
    DECLARE_DYNCREATE(CV7CursorGridCtrl)
public:
    CV7CursorGridCtrl();
    virtual ~CV7CursorGridCtrl();

    BOOL Init(CV7TableField* pTableField, CControlID* pControlID, CGetDoc7* pDoc, CBLContext* pCtrlObject);
	CTempWndHelper CreateTempWnd();

    CBLContext* GetBLDataProvider() const;
    void SetBLDataProvider(CBLContext* pProvider);

	CBLContext* GetBLDataProvider_head() const;
	void SetBLDataProvider_head(CBLContext* pProvider);

	CBLContext* GetBLDataProvider_tail() const;
	void SetBLDataProvider_tail(CBLContext* pProvider);

	CBLFont* GetBLFont() const;
    void SetBLFont(CBLFont* pFont);

    CV7RowData* GetRowData() const;
	CV7SelectedRows* GetV7SelectedRows() const;

	CV7FixedRows_head* GetV7FixedRows_head() const;
	CV7FixedRows_tail* GetV7FixedRows_tail() const;

    void GetCurrentRow(CValue& value);
    void SetCurrentRow(const CValue& value);

	CGetDoc7* GetGetDoc7() const { return m_pDoc; };
protected:
	//{{AFX_MSG(CV7CursorGridCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    virtual CGridColumns* NewColumns();
    virtual CGridRow* NewGridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rt);

	CV7RowAppearance* GetV7RowAppearance(RegionType rType) const;
	void GetRowValue(CDataRow* pDataRow, RegionType rType, CValue& val) const;

	//Events
    virtual void OnChangeCurrentData(CDataRow* pDataRow);
    virtual void OnSelection(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
    virtual void OnActivateRow();
    virtual void OnActivateColumn();
    virtual void OnActivateCell();
    virtual void OnHeaderSelection(CGridColumn* pColumn);
	virtual void OnShowContextMenu(RegionType rType, CGridColumn* pColumn, CDataRow* pDataRow);
    virtual void OnClickCheckBox(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
	virtual void OnBeginDrag(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
    virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, DropTargetView& dropTargetView);
    virtual void OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
	virtual void OnDestroy();
	virtual BOOL OnQuickSearchStart(DataType dt);
	virtual BOOL OnQuickSearch(CString& str);
	virtual void OnQuickSearchEnd(QuickSearchResult result, BOOL bPositionChanged);
	virtual BOOL OnColumnMove(CGridColumn* pColumn, int nShift);
	virtual BOOL OnColumnVisibilityChange(CGridColumn* pColumn, BOOL bNewVisible);
	virtual void OnColumnWidthChange();
	void OnCloseDocument(void);
	void OnFillItems(CV7GridRow* pV7GridRow);
    
private:
    CBLPtr<CV7RowAppearance> m_pV7RowAppearance;
	CBLPtr<CV7RowAppearance> m_pV7RowAppearance_head;
	CBLPtr<CV7RowAppearance> m_pV7RowAppearance_tail;
    CBLPtr<CV7RowData> m_pCurrentData;
    CBLPtr<CV7SelectedRows> m_pV7SelectedRows;
	CBLPtr<CV7FixedRows_head> m_pV7FixedRows_head;
	CBLPtr<CV7FixedRows_tail> m_pV7FixedRows_tail;

    CGetDoc7* m_pDoc;
	static CMapPtrToPtr m_map_Grid2Doc;
	static CTrapSwap m_trOnCloseDocument;
	static BOOL m_bOnCloseDocument_trapped;
    
    CBLPtr<CBLContext> m_pProvider;
	CBLPtr<CBLContext> m_pProvider_head;
	CBLPtr<CBLContext> m_pProvider_tail;
    CBLPtr<CBLFont> m_pFont;

    CCtrlEventManager EventManager;
	BOOL m_bOnDestroyFired;
};

class CV7TableField : public CMyContextBase, public CV7Control
{
    friend class CV7CursorGridCtrl;
    DECLARE_DYNCREATE(CV7TableField)
public:
    CV7TableField();
    virtual ~CV7TableField();
    //static BOOL CreateControl(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CValue& vControl);

    //CV7Control
    virtual BOOL CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC);

    BOOL MethRefreshRows(CValue** ppValues);
    BOOL MethRedrawRows(CValue** ppValues);

	BOOL defSetTextMargins(int nParam, CValue* pValue)const;
	BOOL MethSetTextMargins(CValue** ppValues);

	BOOL funcGetCurrentCellBounds(CValue& RetVal, CValue** ppValues);
	BOOL procAutoWidth(CValue** ppValues);

    BOOL GetPropColumns(CValue& Value)const;

    BOOL GetPropProvider(CValue& Value)const;
    BOOL SetPropProvider(CValue const& Value);

	BOOL GetPropVScroller(CValue& Value)const;
    BOOL SetPropVScroller(CValue const& Value);

    BOOL GetPropStartView(CValue& Value)const;
    BOOL SetPropStartView(CValue const& Value);

    BOOL GetPropRowSelectMode(CValue& Value)const;
    BOOL SetPropRowSelectMode(CValue const& Value);

    BOOL GetPropSelectMode(CValue& Value)const;
    BOOL SetPropSelectMode(CValue const& Value);

    BOOL GetPropVerticalLines(CValue& Value)const;
    BOOL SetPropVerticalLines(CValue const& Value);

    BOOL GetPropHorizontalLines(CValue& Value)const;
    BOOL SetPropHorizontalLines(CValue const& Value);

    BOOL GetPropHeadersStyle(CValue& Value)const;
    BOOL SetPropHeadersStyle(CValue const& Value);

    BOOL GetPropActiveHeaderStyle(CValue& Value)const;
    BOOL SetPropActiveHeaderStyle(CValue const& Value);

    BOOL GetPropCurrentColumn(CValue& Value)const;
    BOOL SetPropCurrentColumn(CValue const& Value);

    BOOL GetPropCurrentData(CValue& Value)const;

    BOOL GetPropSelectedRows(CValue& Value)const;

    BOOL GetPropFont(CValue& Value)const;
    BOOL SetPropFont(CValue const& Value);

    BOOL GetPropRefreshTimeout(CValue& Value)const;
    BOOL SetPropRefreshTimeout(CValue const& Value);

    BOOL GetPropQuickSearchTimeout(CValue& Value)const;
    BOOL SetPropQuickSearchTimeout(CValue const& Value);

    BOOL GetPropChangePositionOfColumns(CValue& Value)const;
    BOOL SetPropChangePositionOfColumns(CValue const& Value);

    BOOL GetPropLinesColor(CValue& Value)const;
    BOOL SetPropLinesColor(CValue const& Value);

    BOOL GetPropTextColor(CValue& Value)const;
    BOOL SetPropTextColor(CValue const& Value);

	BOOL GetPropBackColor(CValue& Value)const;
	BOOL SetPropBackColor(CValue const& Value);

	BOOL GetPropSelectionTextColor(CValue& Value)const;
    BOOL SetPropSelectionTextColor(CValue const& Value);

    BOOL GetPropSelectionBackColor(CValue& Value)const;
    BOOL SetPropSelectionBackColor(CValue const& Value);

    BOOL GetPropSelectionTextColor2(CValue& Value)const;
    BOOL SetPropSelectionTextColor2(CValue const& Value);

    BOOL GetPropSelectionBackColor2(CValue& Value)const;
    BOOL SetPropSelectionBackColor2(CValue const& Value);

    BOOL GetPropAlternationRowBackColor(CValue& Value)const;
    BOOL SetPropAlternationRowBackColor(CValue const& Value);

    BOOL GetPropUseAlternationRowColor(CValue& Value)const;
    BOOL SetPropUseAlternationRowColor(CValue const& Value);

    BOOL GetPropCurrentRow(CValue& Value)const;
    BOOL SetPropCurrentRow(CValue const& Value);

    BOOL GetPropEnableStartDrag(CValue& Value)const;
    BOOL SetPropEnableStartDrag(CValue const& Value);

    BOOL GetPropEnableDrag(CValue& Value)const;
    BOOL SetPropEnableDrag(CValue const& Value);

    BOOL GetPropShowHeaders(CValue& Value)const;
    BOOL SetPropShowHeaders(CValue const& Value);

	BOOL GetPropFixedLeft(CValue& Value)const;
	BOOL SetPropFixedLeft(CValue const& Value);
	BOOL GetPropFixedRight(CValue& Value)const;
	BOOL SetPropFixedRight(CValue const& Value);

	BOOL GetPropFixedRows_head(CValue& Value) const;
	BOOL GetPropFixedRows_tail(CValue& Value) const;

	DECLARE_MY_CONTEXT()
private:
    CV7CursorGridCtrl* m_pGrid;
};

class CV7RowData : public CValueCollection
{
    friend class CV7RowAppearance;
    friend class CV7CursorGridCtrl;
    DECLARE_DYNCREATE(CV7RowData)
public:
    CV7RowData();
    virtual ~CV7RowData();

public:
    virtual char const* 	GetTypeString(void)const;

    virtual int				GetNProps(void)const;
	virtual int				FindProp(char const* szName)const;
	virtual char const* 	GetPropName(int nPropIndex, int nAlias)const;
	virtual int				GetPropVal(int nPropIndex, class CValue& Value)const;
	virtual int				SetPropVal(int nPropIndex, class CValue const& Value);
    
private:
    CDataRow* m_pDataRow;
    CV7DataRow* m_pV7DataRow;
};


class CCellAppearance : public CMyContextBase
{
    friend class CV7RowAppearance;
    friend class CV7GridColumns;
    friend class CGridCells;
    DECLARE_DYNCREATE(CCellAppearance)
public:
    CCellAppearance();
    virtual ~CCellAppearance();

    LPCSTR GetName() const;

    BOOL GetPropName(CValue& Value)const;

    BOOL MethSetText(CValue** ppValue);
    BOOL MethSetImage(CValue** ppValue);
    BOOL MethSetCheckValue(CValue** ppValue);

    BOOL GetPropShowText(CValue& Value)const;
    BOOL SetPropShowText(CValue const& Value);

    BOOL GetPropShowImage(CValue& Value)const;
    BOOL SetPropShowImage(CValue const& Value);

    BOOL GetPropShowCheckBox(CValue& Value)const;
    BOOL SetPropShowCheckBox(CValue const& Value);

    BOOL GetPropHorizontalAlignment(CValue& Value)const;
    BOOL SetPropHorizontalAlignment(CValue const& Value);

    BOOL GetPropImageIndex(CValue& Value)const;
    BOOL SetPropImageIndex(CValue const& Value);

    BOOL GetPropCheckValue(CValue& Value)const;
    BOOL SetPropCheckValue(CValue const& Value);

    BOOL GetPropText(CValue& Value)const;
    BOOL SetPropText(CValue const& Value);

    BOOL GetPropTextColor(CValue& Value)const;
    BOOL SetPropTextColor(CValue const& Value);

    BOOL GetPropBackColor(CValue& Value)const;
    BOOL SetPropBackColor(CValue const& Value);

    BOOL GetPropFont(CValue& Value)const;
    BOOL SetPropFont(CValue const& Value);

    DECLARE_MY_CONTEXT()
private:
    CV7GridItem* m_pGridItem;
};

class CGridCells
{
    friend class CV7RowAppearance;
    friend class CV7CursorGridCtrl;
public:
    CGridCells();
    ~CGridCells();
public:
    int GetSize() const;
    CCellAppearance* Add(LPCSTR szName) {return NULL;};
    CCellAppearance* Insert(int nIndex, LPCSTR szName) {return NULL;};
    CCellAppearance* GetAt(int nIndex) const;
    CCellAppearance* GetAt(LPCSTR szName) const;
    int IndexOf(LPCSTR szName) const;
    int IndexOf(CCellAppearance* pItemType) const;
    BOOL Remove(int nIndex) {return FALSE;};
    BOOL Move(int nIndex, int nOffset) {return FALSE;};
    void RemoveAll() {};

private:
    CGridColumns* m_pColumns;
    void Attach(CV7GridRow* pV7GridRow);
    void Detach();
};

class CV7GridCells : public CBLCollection<CGridCells, CCellAppearance, RUNTIME_CLASS(CCellAppearance), BLC_READONLY>
{
    DECLARE_DYNAMIC(CV7GridCells)
public:
    CV7GridCells() {};
    virtual ~CV7GridCells() {};
};

class CV7RowAppearance : public CMyContextBase
{
    friend class CV7CursorGridCtrl;
    DECLARE_DYNCREATE(CV7RowAppearance)
public:
    CV7RowAppearance();
    virtual ~CV7RowAppearance();
    void Attach(CV7GridRow* pV7GridRow);
    void Detach();

    BOOL GetPropRowData(CValue& Value)const;

    BOOL GetPropCells(CValue& Value)const;

    BOOL GetPropTextColor(CValue& Value)const;
    BOOL SetPropTextColor(CValue const& Value);

    BOOL GetPropBackColor(CValue& Value)const;
    BOOL SetPropBackColor(CValue const& Value);

    BOOL GetPropFont(CValue& Value)const;
    BOOL SetPropFont(CValue const& Value);

    
    DECLARE_MY_CONTEXT()
private:
    CV7DataProvider* m_pV7DataProvider;
    CV7GridRow* m_pV7GridRow;

    CBLPtr<CV7RowData> m_pV7RowData;
    CBLPtr<CV7GridCells> m_pV7GridCells;
};

class CV7SelectedRows : public CMyContextBase
{
    friend class CV7CursorGridCtrl;

    DECLARE_DYNCREATE(CV7SelectedRows)
public:
	CV7SelectedRows();
	virtual ~CV7SelectedRows();

	void aux_Check() const;
	CDataRow* aux_GetCurrentDataRow() const;

	BOOL funcCount(CValue& RetVal, CValue** ppValues);
	BOOL procClear(CValue** ppValue);
	BOOL funcGet(CValue& RetVal, CValue** ppValues);
	BOOL procAdd(CValue** ppValue);
	BOOL procDelete(CValue** ppValue);
	BOOL funcContains(CValue& RetVal, CValue** ppValues);
private:
	CSelectedRows* m_pSelectedRows;
	CV7DataProvider* m_pV7DataProvider;
	CV7CursorGridCtrl* m_pGrid;

	DECLARE_MY_CONTEXT()
};

class CV7FixedRows : public CMyContextBase
{
	friend class CV7CursorGridCtrl;

	DECLARE_DYNCREATE(CV7FixedRows)
public:
	CV7FixedRows();
	virtual ~CV7FixedRows();

	void aux_Check() const;

	BOOL propGetDataProvider(CValue& Value) const;
	BOOL propSetDataProvider(CValue const& Value);

	BOOL propGetTextColor(CValue& Value) const;
	BOOL propSetTextColor(CValue const& Value);

	BOOL propGetBackColor(CValue& Value) const;
	BOOL propSetBackColor(CValue const& Value);

	BOOL procRefreshRows(CValue** ppValue);
protected:
	virtual CBLContext* GetBLDataProvider() const { return NULL; };
	virtual void SetBLDataProvider(CBLContext* pCont) const {};

	virtual COLORREF GetTextColor() const { return CLR_DEFAULT; };
	virtual void SetTextColor(COLORREF crText) const {};

	virtual COLORREF GetBackColor() const { return CLR_DEFAULT; };
	virtual void SetBackColor(COLORREF crBack) const {};

	virtual void RefreshRows() const {};
protected:
	CV7CursorGridCtrl* m_pGrid;

	DECLARE_MY_CONTEXT()
};

class CV7FixedRows_head : public CV7FixedRows
{
public:
	virtual CBLContext* GetBLDataProvider() const { return m_pGrid->GetBLDataProvider_head(); };
	virtual void SetBLDataProvider(CBLContext* pCont) const { m_pGrid->SetBLDataProvider_head(pCont); };

	virtual COLORREF GetTextColor() const { return m_pGrid->GetTextColor_head(); };
	virtual void SetTextColor(COLORREF crText) const { m_pGrid->SetTextColor_head(crText); };

	virtual COLORREF GetBackColor() const { return m_pGrid->GetBackColor_head(); };
	virtual void SetBackColor(COLORREF crBack) const { m_pGrid->SetBackColor_head(crBack); };

	virtual void RefreshRows() const { m_pGrid->RefreshRows_head(); };
};

class CV7FixedRows_tail : public CV7FixedRows
{
public:
	virtual CBLContext* GetBLDataProvider() const { return m_pGrid->GetBLDataProvider_tail(); };
	virtual void SetBLDataProvider(CBLContext* pCont) const { m_pGrid->SetBLDataProvider_tail(pCont); };

	virtual COLORREF GetTextColor() const { return m_pGrid->GetTextColor_tail(); };
	virtual void SetTextColor(COLORREF crText) const { m_pGrid->SetTextColor_tail(crText); };

	virtual COLORREF GetBackColor() const { return m_pGrid->GetBackColor_tail(); };
	virtual void SetBackColor(COLORREF crBack) const { m_pGrid->SetBackColor_tail(crBack); };

	virtual void RefreshRows() const { m_pGrid->RefreshRows_tail(); };
};
