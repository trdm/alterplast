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
#include <memory>
#include "../Utils1s/Context/ContextBaseFwd.h"
#include "TableFieldGetCtrl.h"

class CV7GridColumns;
class CV7CursorGridCtrl;
class CV7RowAppearance;
class CV7RowFormat;
class CCellAppearance;
class CGridCells;
class CV7RowData;
class CV7TableField;
class CV7SelectedRows;
class CV7FixedRows_head;
class CV7FixedRows_tail;
class CAddedField;
class CDataProviderScaner;

class CGetDoc7_friend : public CGetDoc7
{
	friend CAddedField;
	friend CV7CursorGridCtrl;
	friend CV7TableField;
};

#define GET_V7_COLOR(crColor) (crColor == CLR_DEFAULT ? -1 : crColor)
#define SET_V7_COLOR(vValue) (vValue == -1L ? CLR_DEFAULT : (LONG)vValue.GetNumeric())

#define GET_V7_TRP(trp) (trp == TRANSPARENT_DEFAULT ? -1 : trp)
#define SET_V7_TRP(vValue) (vValue == -1L ? TRANSPARENT_DEFAULT : (LONG)vValue.GetNumeric())

class CV7ImageList : public CImageList
{
public:
	BOOL CreateFrom(CPictureHolder7*, int nHeight);
	BOOL AddFrom(CPictureHolder7*);
	void Reset();
private:
	BOOL RenderBitmap(CBitmap*, CPictureHolder7*);
private:
	int m_nHeight;
};

class CV7GridColumn : public CMyContextBase, public CGridColumn
{
	friend class CV7CursorGridCtrl;
	friend class CV7GridColumns;
    friend class CGridCells;
	friend class CRowFormat;
	friend class CDataProviderScaner;
	friend class CTableFieldGetCtrl;
	DECLARE_DYNCREATE(CV7GridColumn)
public:
    CV7GridColumn();
    virtual ~CV7GridColumn();

    BOOL MethSetRowsPictures(CValue** ppValue);
    BOOL DefSetRowsPictures(int nParam, CValue* pValue)const;
	BOOL MethAddRowsPictures(CValue** ppValue);
	BOOL MethResetRowsPictures(CValue** ppValue);

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
	
	BOOL GetPropBackTransparency(CValue& Value)const {Value = GET_V7_TRP(GetBackTransparency()); return TRUE;};
	BOOL SetPropBackTransparency(CValue const& Value) {SetBackTransparency(SET_V7_TRP(Value)); return TRUE;};

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

	BOOL GetPropImageDataField(CValue& Value)const {Value = GetImageDataField(); return TRUE;};
	BOOL SetPropImageDataField(CValue const& Value) {SetImageDataField(Value.GetString()); return TRUE;};

	BOOL GetPropOrderMarker(CValue& Value)const {Value = GetOrderMarker(); return TRUE;};
    BOOL SetPropOrderMarker(CValue const& Value);

    BOOL GetPropChangeVisible(CValue& Value)const {Value = GetChangeVisible(); return TRUE;};
    BOOL SetPropChangeVisible(CValue const& Value) {SetChangeVisible(Value.GetNumeric()); return TRUE;};

	BOOL GetPropParameter(CValue& Value)const {Value = m_Value; return TRUE;};
	BOOL SetPropParameter(CValue const& Value) {m_Value = Value; return TRUE;};

	BOOL GetPropLocation(CValue& Value)const {Value = GetLocation(); return TRUE;};
	BOOL SetPropLocation(CValue const& Value) {SetLocation((ColumnLocation)(int)Value.GetNumeric()); return TRUE;};

	BOOL GetPropMergeCaption(CValue& Value)const {Value = GetMergeCaption(); return TRUE;};
	BOOL SetPropMergeCaption(CValue const& Value) {SetMergeCaption(Value.GetNumeric()); return TRUE;};

	BOOL GetPropWordBreak(CValue& Value)const {Value = GetWordBreak(); return TRUE;};
	BOOL SetPropWordBreak(CValue const& Value) {SetWordBreak(Value.GetNumeric()); return TRUE;};
	
	BOOL GetPropFormat(CValue& Value)const {Value = m_strFormat; return TRUE;};
	BOOL SetPropFormat(CValue const& Value) {m_strFormat = Value.GetString(); return TRUE;};

	BOOL GetPropRealWidth(CValue& Value)const {Value = GetRealWidth(); return TRUE;};

	const CString& GetFormat()const {return m_strFormat;};

	BOOL GetPropMarkNegatives(CValue& Value)const {Value = GetMarkNegatives(); return TRUE;};
	BOOL SetPropMarkNegatives(CValue const& Value) {SetMarkNegatives(Value.GetNumeric()); return TRUE;};

protected:
    virtual void Release() {DecrRef();};

	DECLARE_MY_CONTEXT()
private:
    CBLPtr<CBLFont> m_pFont;
    CBLPtr<CBLFont> m_pHeaderFont;
	CBLPtr<CCellAppearance> m_pCellAppearance;
	CBLPtr<CCellFormat> m_pCellFormat;
    CV7ImageList m_RowsImages;
	CValue m_Value;
	CString m_strFormat;
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
	typedef CSimpleCollection<IMAGEREFS, scf_Modify | scf_Insert | scf_Move> V7IMAGEREFS;

	CV7GridItem(CGridColumn* pGridColumn);
    virtual ~CV7GridItem();
protected:
	virtual IMAGEREFS* NewImageRefs() const { return new V7IMAGEREFS("ИндексыКартинок"); }
    virtual BOOL DrawImage(CDC* pDC, CRect& rect);
private:
    CBLPtr<CBLFont> m_pFont;
    CBLPtr<CPictureContext> m_pPicture;
};

class CV7GridRow : public CGridRow
{
	friend class CV7RowAppearance;
	friend class CV7CursorGridCtrl;
public:
    CV7GridRow(CGridColumns* pColumns, CDataRowPtr const& pDataRow, RegionType rt);
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
		m_pWnd.reset(new CWnd);
		m_pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
	}

private:
	std::auto_ptr<CWnd> m_pWnd;
};

class CAddedField
{
private:
	CAddedField(CAddedField const&);
	CAddedField& operator=(CAddedField const&);
public:
	CAddedField(CGetDoc7* pDoc, CValue const& vValue, int nLen, int nPrec, LPCSTR szCodeName, DWORD nFlags);
	virtual ~CAddedField();

	CGetDoc7* GetGetDoc7() const { return m_pDoc; }
	CGetField* GetGetField() const { return m_pField; }

	virtual BOOL OnDestroy(BOOL bCheckValue) const;
	virtual void GetValue(CValue& RetVal) const;

private:
	void RegisterProp() const;
	void UnregisterProp() const;
	LPCSTR GetPropID() const;

	CValue const* GetPValue() const;
	void GetString(CString& csVal) const;

	static void ShiftPCode(CBLModule7* pMod, DWORD nFrom, int nShift);
//	void ShiftPCode(int nShift) const;

private:
	CGetDoc7* m_pDoc;
	CGetField* m_pField;
};

class CV7GridCellEdit : public CGridCellEdit
{
private:
	CV7GridCellEdit(CV7GridCellEdit const&);
	CV7GridCellEdit& operator=(CV7GridCellEdit const&);

public:
	CV7GridCellEdit(CCursorGridCtrl* pGrid, RegionType rType, CGridColumn* pColumn, size_t nRowIndex)
		: CGridCellEdit(pGrid, rType, pColumn, nRowIndex) {}
	virtual ~CV7GridCellEdit() {}

	virtual BOOL OnDestroy(BOOL bCheckValue) const = 0;
	virtual void GetValue(CValue& RetVal) const = 0;
};

class CV7NativeCellEdit : public CV7GridCellEdit, public CAddedField
{
private:
	CV7NativeCellEdit(CV7NativeCellEdit const&);
	CV7NativeCellEdit& operator =(CV7NativeCellEdit const&);

public:
	CV7NativeCellEdit(CV7CursorGridCtrl* pGrid, RegionType rType, CGridColumn* pColumn, size_t nRowIndex, CValue const& vValue, int nLen, int nPrec, LPCSTR szCodeName, DWORD nFlags);
	virtual ~CV7NativeCellEdit();

	virtual BOOL OnDestroy(BOOL bCheckValue) const { return CAddedField::OnDestroy(bCheckValue); }
	virtual void GetValue(CValue& RetVal) const { CAddedField::GetValue(RetVal); }
};

class CV7CursorGridCtrl : public CCursorGridCtrl, public IV7ControlContext
{
    friend class CV7GridColumns;
    friend class CV7GridRow;
    friend class CV7GridItem;
	friend class CDataProviderScaner;
	friend class CTableFieldGetCtrl;
	friend class CRowFormat;
    DECLARE_DYNCREATE(CV7CursorGridCtrl)

	// IV7ControlContext
	CBLContext* GetControlContext() const;

public:
	// CCursorGridCtrl
    CV7CursorGridCtrl();
    virtual ~CV7CursorGridCtrl();

	void PreInit(CGetDoc7* pDoc);
    BOOL Init(CV7TableField* pTableField, CControlID* pControlID, CBLContext* pCtrlObject);
	CTempWndHelper CreateTempWnd();

	void EditValue(CValue** ppValues);
	BOOL EndEditValue(BOOL bFireEvent);

    CBLContext* GetBLDataProvider() const;
    void SetBLDataProvider(CBLContext* pProvider);

	CBLContext* GetBLDataProvider_head() const;
	void SetBLDataProvider_head(CBLContext* pProvider);

	CBLContext* GetBLDataProvider_tail() const;
	void SetBLDataProvider_tail(CBLContext* pProvider);

	CBLFont* GetBLFont() const;
	void SetBLFont(CBLFont* pFont);
	
	void SetBackPicture(CPictureContext* pPicCont){ m_pBackPicture = pPicCont; };
	void SetBackPicVA(VBackAlignment va){ m_BackPicVA = va; };
	void SetBackPicHA(HBackAlignment ha){ m_BackPicHA = ha; };
	void SetBackPicTransparentColor(COLORREF cr){ m_crBackPicTransparentColor = cr; };
	void SetBackPicRepeat(BOOL bRepeat){ m_bBackPicRepeat = bRepeat; };
	void SetBackTextBLFont(CBLFont* pFont);

    CV7RowData* GetRowData() const;
	CV7SelectedRows* GetV7SelectedRows() const;

	CV7FixedRows_head* GetV7FixedRows_head() const;
	CV7FixedRows_tail* GetV7FixedRows_tail() const;

    void GetCurrentRow(CValue& value);
    void SetCurrentRow(const CValue& value);

	CGetDoc7* GetGetDoc7() const { return m_pDoc; };
	BOOL GetOnDestroyFired() const { return m_bOnDestroyFired; };
	BOOL GetOnActivateCtrl() const { return m_bOnActivateCtrl; };
	CV7ImageList& GetSysImages() const { return m_SysImages; };

protected:
	//{{AFX_MSG(CV7CursorGridCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    virtual CGridColumns* NewColumns();
    virtual CGridRow* NewGridRow(CGridColumns* pColumns, CDataRowPtr const& pDataRow, RegionType rt);

	virtual BOOL FormatField(CDataRow* pDataRow, int nIndex, CGridColumn*, CString&);

	CV7RowAppearance* GetV7RowAppearance(RegionType rType) const;
	void GetRowValue(CDataRow* pDataRow, RegionType rType, CValue& val) const;

	virtual BOOL CloseEdit(BOOL bFireEvent);
	virtual CV7GridCellEdit* GetEdit() const { return static_cast<CV7GridCellEdit*>(CCursorGridCtrl::GetEdit()); }

	// Events
    virtual void OnChangeCurrentData(CDataRow* pDataRow);
    virtual void OnSelection(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, GridHitTest htArea);
    virtual void OnActivateRow();
    virtual void OnActivateColumn();
    virtual void OnActivateCell();
    virtual void OnHeaderSelection(CGridColumn* pColumn);
	virtual void OnShowContextMenu(RegionType rType, CGridColumn* pColumn, CDataRow* pDataRow);
    virtual void OnClickCheckBox(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType);
	virtual void OnClickImage(CDataRow* pDataRow, CGridColumn* pColumn, int nPicIndex, RegionType rType);
	virtual void OnClickSysImage(CDataRow* pDataRow, CGridColumn* pColumn, int nPicIndex, RegionType rType);
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
	void OnActivateCtrl(UINT nCtrl);
	void OnFillItems(CV7GridRow* pV7GridRow);
	void OnRowFind(CDataRowPtr const& );
	BOOL OnFind(CV7GlobalFindData*);
	BOOL OnFindDialog(CV7GlobalFindData*);
	BOOL OnClipboard(UINT dwCommandID, CString& str);
    
	virtual BOOL OnKey(MSG* pMsg);    
	virtual BOOL OnShowHint(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, CPoint ptShow, const CString &strHint);
private:
	static CDataProvider* ProviderCheckedDowncast(CBLContext* pProvider);
	void GetProvidersFromUtils1sContext (Utils1s::Context::CContextBase *pContext,
		CDataProvider *&pProvider, CV7DataProvider *&pV7Provider);
	virtual CFont* GetDefaultFont() const;
	
	virtual void DrawBackImage(CDC*);
	
	static BOOL DrawPicture(CPictureContext*, CDC*, CRect&);

	CBLPtr<CV7RowAppearance> m_pV7RowAppearance;
	CBLPtr<CV7RowAppearance> m_pV7RowAppearance_head;
	CBLPtr<CV7RowAppearance> m_pV7RowAppearance_tail;
    CBLPtr<CV7RowData> m_pCurrentData;
    CBLPtr<CV7SelectedRows> m_pV7SelectedRows;
	CBLPtr<CV7FixedRows_head> m_pV7FixedRows_head;
	CBLPtr<CV7FixedRows_tail> m_pV7FixedRows_tail;
	
	CBLPtr<CV7RowFormat> m_pV7RowFormat;
	
    CGetDoc7* m_pDoc;
	static CMapPtrToPtr m_map_Grid2Doc;
	static CTrapSwap m_trOnCloseDocument;
	static CTrapSwap m_trOnActivateCtrl;
    
    CBLPtr<CBLContext> m_pProvider;
	CBLPtr<CBLContext> m_pProvider_head;
	CBLPtr<CBLContext> m_pProvider_tail;
    CBLPtr<CBLFont> m_pFont;

    CCtrlEventManager EventManager;
	BOOL m_bOnDestroyFired;
	UINT m_bOnActivateCtrl;
	
	CV7ImageList m_SysImages;

	CBLPtr<CPictureContext> m_pBackPicture;
	VBackAlignment m_BackPicVA;
	HBackAlignment m_BackPicHA;
	COLORREF m_crBackPicTransparentColor;
	BOOL m_bBackPicRepeat;
	CBLPtr<CBLFont> m_pBackTextFont;
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

	BOOL MethSetSysPictures(CValue** ppValue);
	BOOL DefSetSysPictures(int nParam, CValue* pValue)const;
	BOOL MethAddSysPictures(CValue** ppValue);
	BOOL MethResetSysPictures(CValue** ppValue);

	BOOL defSetTextMargins(int nParam, CValue* pValue)const;
	BOOL MethSetTextMargins(CValue** ppValues);

	BOOL funcGetCurrentCellBounds(CValue& RetVal, CValue** ppValues);
	BOOL procAutoWidth(CValue** ppValues);

	BOOL funcEditValue(CValue& RetVal, CValue** ppValues);
	BOOL funcEndEditValue(CValue& RetVal, CValue** ppValues);
	BOOL funcCancelEditValue(CValue& RetVal, CValue** ppValues);

	BOOL procSetBackPicture(CValue** ppValues);
	BOOL defSetBackPicture(int nParam, CValue* pValue)const;
	BOOL procSetBackText(CValue** ppValues);
	BOOL defSetBackText(int nParam, CValue* pValue)const;
	BOOL procResetBackPicture(CValue** ppValues);
	BOOL procResetBackText(CValue** ppValues);

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

	BOOL GetPropBorderStyle(CValue& Value)const;
	BOOL SetPropBorderStyle(CValue const& Value);

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

	BOOL GetPropInactiveLinesColor(CValue& Value)const;
	BOOL SetPropInactiveLinesColor(CValue const& Value);

    BOOL GetPropTextColor(CValue& Value)const;
    BOOL SetPropTextColor(CValue const& Value);

	BOOL GetPropBackColor(CValue& Value)const;
	BOOL SetPropBackColor(CValue const& Value);
	
	BOOL GetPropBackTransparency(CValue& Value)const;
	BOOL SetPropBackTransparency(CValue const& Value);
	BOOL GetPropSelectionBackTransparency(CValue& Value)const;
	BOOL SetPropSelectionBackTransparency(CValue const& Value);
	BOOL GetPropSelectionBackTransparency2(CValue& Value)const;
	BOOL SetPropSelectionBackTransparency2(CValue const& Value);
	BOOL GetPropAlternationRowBackTransparency(CValue& Value)const;
	BOOL SetPropAlternationRowBackTransparency(CValue const& Value);

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
	
	BOOL GetPropNegativesColor(CValue& Value)const;
	BOOL SetPropNegativesColor(CValue const& Value);

	BOOL GetPropHeadersTextColor(CValue& Value)const;
	BOOL SetPropHeadersTextColor(CValue const& Value);

	BOOL GetPropHeadersBackColor(CValue& Value)const;
	BOOL SetPropHeadersBackColor(CValue const& Value);

	BOOL GetPropBorderColor(CValue& Value)const;
	BOOL SetPropBorderColor(CValue const& Value);

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
	CV7RowData(CDataRow*, CDataProvider*);
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
	BOOL MethSetSysImage(CValue** ppValue);
    BOOL MethSetCheckValue(CValue** ppValue);
	BOOL MethSetHint(CValue** ppValue);

    BOOL GetPropShowText(CValue& Value)const;
    BOOL SetPropShowText(CValue const& Value);

	BOOL GetPropShowImage(CValue& Value)const;
	BOOL SetPropShowImage(CValue const& Value);

	BOOL GetPropShowSysImage(CValue& Value)const;
	BOOL SetPropShowSysImage(CValue const& Value);

    BOOL GetPropShowCheckBox(CValue& Value)const;
    BOOL SetPropShowCheckBox(CValue const& Value);

	BOOL GetPropShowHint(CValue& Value)const;
	BOOL SetPropShowHint(CValue const& Value);

    BOOL GetPropHorizontalAlignment(CValue& Value)const;
    BOOL SetPropHorizontalAlignment(CValue const& Value);

	BOOL GetPropImageIndex(CValue& Value)const;
	BOOL SetPropImageIndex(CValue const& Value);

	BOOL GetPropImageRefs(CValue& Value)const;

	BOOL GetPropSysImageIndex(CValue& Value)const;
	BOOL SetPropSysImageIndex(CValue const& Value);

	BOOL GetPropSysImageRefs(CValue& Value)const;

    BOOL GetPropCheckValue(CValue& Value)const;
    BOOL SetPropCheckValue(CValue const& Value);

    BOOL GetPropText(CValue& Value)const;
    BOOL SetPropText(CValue const& Value);

    BOOL GetPropTextColor(CValue& Value)const;
    BOOL SetPropTextColor(CValue const& Value);

    BOOL GetPropBackColor(CValue& Value)const;
    BOOL SetPropBackColor(CValue const& Value);

	BOOL GetPropBackTransparency(CValue& Value)const;
	BOOL SetPropBackTransparency(CValue const& Value);

    BOOL GetPropFont(CValue& Value)const;
    BOOL SetPropFont(CValue const& Value);
	
	BOOL GetPropWordBreak(CValue& Value)const;
	BOOL SetPropWordBreak(CValue const& Value);

	BOOL GetPropHint(CValue& Value)const;
	BOOL SetPropHint(CValue const& Value);

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

	BOOL GetPropBackTransparency(CValue& Value)const;
	BOOL SetPropBackTransparency(CValue const& Value);

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
	
	BOOL propGetBackTransparency(CValue& Value)const;
	BOOL propSetBackTransparency(CValue const& Value);

	BOOL procRefreshRows(CValue** ppValue);
	BOOL procRedrawRows(CValue** ppValue);
protected:
	virtual CBLContext* GetBLDataProvider() const { return NULL; };
	virtual void SetBLDataProvider(CBLContext* pCont) const {};

	virtual COLORREF GetTextColor() const { return CLR_DEFAULT; };
	virtual void SetTextColor(COLORREF crText) const {};

	virtual COLORREF GetBackColor() const { return CLR_DEFAULT; };
	virtual void SetBackColor(COLORREF crBack) const {};

	virtual WORD GetBackTransparency() const { return TRANSPARENT_DEFAULT; };
	virtual void SetBackTransparency(WORD) const {};

	virtual void RefreshRows() const {};
	virtual void RedrawRows() const {};
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

	virtual WORD GetBackTransparency() const { return m_pGrid->GetBackTransparency_head(); };
	virtual void SetBackTransparency(WORD trp) const { m_pGrid->SetBackTransparency_head(trp); };

	virtual void RefreshRows() const { m_pGrid->RefreshRows_head(); };
	virtual void RedrawRows() const { m_pGrid->RedrawRows_head(); };
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

	virtual WORD GetBackTransparency() const { return m_pGrid->GetBackTransparency_tail(); };
	virtual void SetBackTransparency(WORD trp) const { m_pGrid->SetBackTransparency_tail(trp); };

	virtual void RefreshRows() const { m_pGrid->RefreshRows_tail(); };
	virtual void RedrawRows() const { m_pGrid->RedrawRows_tail(); };
};
