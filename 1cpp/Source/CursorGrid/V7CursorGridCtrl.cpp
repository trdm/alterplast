
#include "StdAfx.h"
#include "V7CursorGridCtrl.h"


//////////////////////////////////////////////////////////////////////////
// class CV7GridColumn
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7GridColumn)
    BL_METH("SetRowsPictures", "УстановитьКартинкиСтрок", 2, MethSetRowsPictures, NULL, DefSetRowsPictures)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7GridColumn)
    BL_PROP("Name", "Имя", GetPropName, SetPropName)
    BL_PROP("Caption", "Заголовок", GetPropCaption, SetPropCaption)
    BL_PROP("Visible", "Видимость", GetPropVisible, SetPropVisible)
    BL_PROP("Width", "Ширина", GetPropWidth, SetPropWidth)
    BL_PROP("AutoWidth", "АвтоШирина", GetPropAutoWidth, NULL)
    BL_PROP("HorizontalAlignment", "ГоризонтальноеВыравнивание", GetPropHorizontalAlignment, SetPropHorizontalAlignment)
    BL_PROP("ИзменениеРазмера", "SizeChange", GetPropFixedWidth, SetPropFixedWidth)
    BL_PROP("TextColor", "ЦветТекста", GetPropTextColor, SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", GetPropBackColor, SetPropBackColor)
    BL_PROP("Font", "Шрифт", GetPropFont, SetPropFont)
    BL_PROP("HeaderFont", "ШрифтЗаголовка", GetPropHeaderFont, SetPropHeaderFont)
    BL_PROP("HeaderTextColor", "ЦветТекстаЗаголовка", GetPropHeaderTextColor, SetPropHeaderTextColor)
    BL_PROP("HeaderBackColor", "ЦветФонаЗаголовка", GetPropHeaderBackColor, SetPropHeaderBackColor)
    BL_PROP("DataField", "Данные", GetPropDataField, SetPropDataField)
    BL_PROP("OrderMarker", "ЗнакУпорядочивания", GetPropOrderMarker, SetPropOrderMarker)
    BL_PROP("ChangeVisible", "ИзменятьВидимость", GetPropChangeVisible, SetPropChangeVisible)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7GridColumn, "TableBoxColumn", "КолонкаТабличногоПоля", 0, NULL, NULL, -1);

CV7GridColumn::CV7GridColumn()
{
    m_pCellAppearance.Create();
    m_pFont.Create();
    m_pHeaderFont.Create();
}

CV7GridColumn::~CV7GridColumn()
{
}

BOOL CV7GridColumn::SetPropHorizontalAlignment(CValue const& Value)
{
    GET_ENUM_PROP(HorizTextAlignment, htaAuto, htaCenter);
    SetHTextAlignment(_enum);
    return TRUE;
}

BOOL CV7GridColumn::SetPropFont(CValue const& Value)
{
    CHECK_BLTYPE(Value, CBLFont);
    m_pFont = static_cast<CBLFont*>(Value.GetContext());
    SetFont(*m_pFont);
    return TRUE;
}

BOOL CV7GridColumn::SetPropHeaderFont(CValue const& Value)
{
    CHECK_BLTYPE(Value, CBLFont);
    m_pHeaderFont = static_cast<CBLFont*>(Value.GetContext());
    SetHeaderFont(*m_pHeaderFont);
    return TRUE;
}

BOOL CV7GridColumn::MethSetRowsPictures(CValue** ppValue)
{
	if (ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C || strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext"))
	{
		RuntimeError("Недопустимое значение первого параметра.", 0);
		return FALSE;
	}
	CPictureHolder7& PictureHolder7 = static_cast<CPictureContext*>(ppValue[0]->GetContext())->m_PictureHolder7;

	BOOL bSuccess = FALSE;
	CDC dc;
	CDC DesktopDC;
	DesktopDC.Attach(::GetDC(NULL));
	if (dc.CreateCompatibleDC(&DesktopDC))
	{
		CSize size = PictureHolder7.GetSize();
		if (ppValue[1]->GetTypeCode() != UNDEFINE_TYPE_1C)
			size.cy = ppValue[1]->GetNumeric();
		CBitmap bitmap;
		if (bitmap.CreateCompatibleBitmap(&DesktopDC, size.cx, size.cy))
		{
			CBitmap* pOldbitmap = dc.SelectObject(&bitmap);
			CRect rect(CPoint(0,0), PictureHolder7.GetSize());
			dc.FillSolidRect(0,0, size.cx, size.cy, RGB(0x00, 0x80, 0x80));
			PictureHolder7.Render(dc, rect, 1, NULL);
			dc.SelectObject(pOldbitmap);
			if (m_RowsImages.Create(size.cy, size.cy, ILC_MASK|ILC_COLOR24, 2, 0))
			{
				bSuccess = m_RowsImages.Add(&bitmap, RGB(0x00, 0x80, 0x80)) == 0;
				int i = m_RowsImages.GetImageCount();
				if (bSuccess)
					SetRowsImages(&m_RowsImages);
			}

		}
	}
	return bSuccess;
}

BOOL CV7GridColumn::DefSetRowsPictures(int nParam, CValue* pValue)const
{
    if (nParam == 1)
    {
        pValue->Reset();
        return TRUE;
    }
    else
        return FALSE;
}

BOOL CV7GridColumn::SetPropOrderMarker(CValue const& Value)
{
    GET_ENUM_PROP(OrderMarker, omNone, omDesc3);
    SetOrderMarker(_enum);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CV7GridColumns
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CV7GridColumns, CBLContext)

CV7GridColumns::CV7GridColumns()
{
}

CV7GridColumns::~CV7GridColumns()
{
}

void CV7GridColumns::Release()
{
    DecrRef();
}

CGridColumn* CV7GridColumns::NewColumn()
{
    return new CV7GridColumn;
}

//////////////////////////////////////////////////////////////////////////
// class CV7GridItem
//////////////////////////////////////////////////////////////////////////

CV7GridItem::CV7GridItem(CGridColumn* pGridColumn) : CGridItem(pGridColumn)
{
    m_pFont.Create();
}

CV7GridItem::~CV7GridItem()
{
}

BOOL CV7GridItem::DrawImage(CDC* pDC, CRect& rect)
{
    if (!m_pPicture)
        return FALSE;
    //CPictureHolder7 CBLContext
    CSize size = m_pPicture->m_PictureHolder7.GetSize();
    switch (m_pPicture->m_PictureHolder7.m_DrawMode)
    {
    case CPictureHolder7::dmStretch:
        if (size.cy > rect.Height())
            rect.right = rect.left + MulDiv(rect.Height(), size.cx, size.cy);
        else if (size.cx > rect.Width())
            rect.bottom = rect.top + MulDiv(rect.Width(), size.cy, size.cx);
        else
        {
            rect.bottom = rect.top + size.cy;
            rect.right = rect.left + size.cx;
        }
        break;
    case CPictureHolder7::dmOriginalSize:
        rect.right = rect.left + size.cx;
        rect.bottom = rect.top + size.cy;
        break;
    case CPictureHolder7::dmKeepingOriginalParity:
        if (size.cx > size.cy)
        {
            int nWidth = MulDiv(rect.Height(), size.cx, size.cy);
            if (nWidth < rect.Width())
                rect.right = rect.left + nWidth;
        }
        else
        {
            int nHeight = MulDiv(rect.Width(), size.cy, size.cx);
            if (nHeight < rect.Height())
                rect.bottom = rect.top + nHeight;
        }
        break;
    default:
        return FALSE;
    }
    m_pPicture->m_PictureHolder7.Render(*pDC, rect, 0, NULL);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CV7GridRow
//////////////////////////////////////////////////////////////////////////

CV7GridRow::CV7GridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rt)
    : CGridRow(pColumns, pDataRow, rt)
{
    m_pFont.Create();
}

CV7GridRow::~CV7GridRow()
{
}

CGridItem* CV7GridRow::NewItem(CGridColumn* pGridColumn)
{
    return new CV7GridItem(pGridColumn);
}

void CV7GridRow::OnFillItems()
{
    static_cast<CV7CursorGridCtrl*>(GetGrid())->OnFillItems(this);
}

//////////////////////////////////////////////////////////////////////////
// class CV7CursorGridCtrl
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CV7CursorGridCtrl, CCursorGridCtrl)

// static objects
CMapPtrToPtr CV7CursorGridCtrl::m_map_Grid2Doc;
CTrapSwap CV7CursorGridCtrl::m_trOnCloseDocument;
BOOL CV7CursorGridCtrl::m_bOnCloseDocument_trapped = FALSE;

enum V7CursorGridCtrlEvents
{
    geRowOut,
    geSelection,
    geActivateRow,
    geActivateColumn,
    geActivateCell,
    geHeaderSelection,
    geContextMenu,
    geClickCheckBox,
    geDragStart,
    geDragEnd,
    geDragCheck,
    geDrag,
    geOnDestroy,
	geOnQuickSearchStart,
	geOnQuickSearch,
	geOnQuickSearchEnd,
	geOnColumnMove,
	geOnColumnVisibilityChange,
	geOnColumnWidthChange,
	
    geLastEvent
};

stCtrlEvents CV7CursorGridCtrlEvents[] = 
{
    {"ПриВыводеСтроки", "RowOut", 3},
    {"Выбор", "Selection", 3},
    {"ПриАктивизацииСтроки", "ActivateRow", 0},
    {"ПриАктивизацииКолонки", "ActivateColumn", 0},
    {"ПриАктивизацииЯчейки", "ActivateCell", 0},
    {"ПриВыбореЗаголовка", "HeaderSelection", 1},
    {"ПриКонтекстномМеню", "ContextMenu", 3},
    {"ПриВыбореФлажка", "ClickCheckBox", 3},
	{"НачалоПеретаскивания", "DragStart", 5},
    {"ОкончаниеПеретаскивания", "DragEnd", 4},
    {"ПроверкаПеретаскивания", "DragCheck", 6},
    {"Перетаскивание", "Drag", 5},
    {"ПриУничтожении", "OnDestroy", 0},
    {"ПриНачалеБыстрогоПоиска", "OnQuickSearchStart", 2},
    {"ПриБыстромПоиске", "OnQuickSearch", 3},
	{"ПриОкончанииБыстрогоПоиска", "OnQuickSearchEnd", 2},
	{"ПриПеремещенииКолонки", "OnColumnMove", 3},
	{"ПриИзмененииВидимостиКолонки", "OnColumnVisibilityChange", 3},
	{"ПриИзмененииШириныКолонки", "OnColumnWidthChange", 3},
};

CV7CursorGridCtrl::CV7CursorGridCtrl()
{
//	Msg("CV7CursorGridCtrl CTOR %p", this);
	m_bOnDestroyFired = FALSE;

	m_pFont.Create();
    m_pV7RowAppearance.Create();
	m_pV7RowAppearance_head.Create();
	m_pV7RowAppearance_tail.Create();
    m_pCurrentData.Create();
	m_pV7SelectedRows.Create();
	m_pV7FixedRows_head.Create();
	m_pV7FixedRows_tail.Create();
}

CV7CursorGridCtrl::~CV7CursorGridCtrl()
{
//	Msg("CV7CursorGridCtrl DTOR %p", this);
	OnDestroy();

	if (EventManager.GetCtrlObject())
        static_cast<CV7TableField*>(EventManager.GetCtrlObject())->m_pGrid = NULL;

    if (m_pProvider)
        SetDataProvider(NULL);

	if (m_pProvider_head)
		SetDataProvider_head(NULL);

	if (m_pProvider_tail)
		SetDataProvider_tail(NULL);

	m_map_Grid2Doc.RemoveKey(this);
}

BEGIN_MESSAGE_MAP(CV7CursorGridCtrl, CCursorGridCtrl)
	//{{AFX_MSG_MAP(CV7CursorGridCtrl)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CV7CursorGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CCursorGridCtrl::OnSetFocus(pOldWnd);
    m_pDoc->SetActiveCtl(GetDlgCtrlID());
}

CGridColumns* CV7CursorGridCtrl::NewColumns()
{
    CV7GridColumns* pV7GridColumns = new CV7GridColumns;
    m_pV7RowAppearance->m_pV7GridCells->m_pColumns = pV7GridColumns;
	m_pV7RowAppearance_head->m_pV7GridCells->m_pColumns = pV7GridColumns;
	m_pV7RowAppearance_tail->m_pV7GridCells->m_pColumns = pV7GridColumns;
    return pV7GridColumns;
}

CBLFont* CV7CursorGridCtrl::GetBLFont() const
{
    return m_pFont;
}

void CV7CursorGridCtrl::SetBLFont(CBLFont* pFont)
{
    m_pFont = pFont;
	SetFont(*m_pFont);
}

CBLContext* CV7CursorGridCtrl::GetBLDataProvider() const
{
    return m_pProvider;
}

void CV7CursorGridCtrl::SetBLDataProvider(CBLContext* pProvider)
{
    CDataProvider* pDataProvider = NULL;
    pDataProvider = dynamic_cast<CDataProvider*>(pProvider);

    SetDataProvider(NULL);

    m_pProvider = pProvider;

    CV7DataProvider* pV7DataProvider = NULL;
    pV7DataProvider = dynamic_cast<CV7DataProvider*>(pProvider);

    m_pV7RowAppearance->m_pV7DataProvider = pV7DataProvider;
	m_pV7SelectedRows->m_pV7DataProvider = pV7DataProvider;

    SetDataProvider(pDataProvider);
}

CBLContext* CV7CursorGridCtrl::GetBLDataProvider_head() const
{
	return m_pProvider_head;
}

void CV7CursorGridCtrl::SetBLDataProvider_head(CBLContext* pProvider)
{
	CDataProvider* pDataProvider = NULL;
	pDataProvider = dynamic_cast<CDataProvider*>(pProvider);

	SetDataProvider_head(NULL);

	CV7DataProvider* pV7DataProvider = NULL;
	pV7DataProvider = dynamic_cast<CV7DataProvider*>(pProvider);
	m_pV7RowAppearance_head->m_pV7DataProvider = pV7DataProvider;

	m_pProvider_head = pProvider;
	SetDataProvider_head(pDataProvider);
}

CBLContext* CV7CursorGridCtrl::GetBLDataProvider_tail() const
{
	return m_pProvider_tail;
}

void CV7CursorGridCtrl::SetBLDataProvider_tail(CBLContext* pProvider)
{
	CDataProvider* pDataProvider = NULL;
	pDataProvider = dynamic_cast<CDataProvider*>(pProvider);

	SetDataProvider_tail(NULL);

	CV7DataProvider* pV7DataProvider = NULL;
	pV7DataProvider = dynamic_cast<CV7DataProvider*>(pProvider);
	m_pV7RowAppearance_tail->m_pV7DataProvider = pV7DataProvider;

	m_pProvider_tail = pProvider;
	SetDataProvider_tail(pDataProvider);
}

CGridRow* CV7CursorGridCtrl::NewGridRow(CGridColumns* pColumns, CDataRow* pDataRow, RegionType rt)
{
    return new CV7GridRow(pColumns, pDataRow, rt);
}

BOOL CV7CursorGridCtrl::Init(CV7TableField* pTableField, CControlID* pControlID, CGetDoc7* pDoc, CBLContext* pCtrlObject)
{
    pTableField->m_pGrid = this;
    m_pDoc = pDoc;

    EventManager.SetEvents(CV7CursorGridCtrlEvents, geLastEvent,
        pControlID, (CBLModule7*)(((char*)pDoc) + 0x4E0), pTableField,
        pCtrlObject);

	if (!m_bOnCloseDocument_trapped)
	{
		m_trOnCloseDocument.SetTrapByName("basic.dll", "?OnCloseDocument@CGetDoc7@@UAEXXZ", &CV7CursorGridCtrl::OnCloseDocument);
		m_bOnCloseDocument_trapped = TRUE;
	}

	m_map_Grid2Doc.SetAt(this, pDoc);

	m_pV7SelectedRows->m_pGrid = this;
	m_pV7SelectedRows->m_pSelectedRows = GetSelectedRows();

	m_pV7FixedRows_head->m_pGrid = this;
	m_pV7FixedRows_tail->m_pGrid = this;

    return TRUE;
}

CTempWndHelper CV7CursorGridCtrl::CreateTempWnd()
{
	CTempWndHelper wh;

	CRect rect;
	if (GetCurrentCellBounds(rect))
	{
		DWORD nID = m_pDoc->GetActiveCtl()->m_uiID;
		wh.Create(this, rect, nID);
	}
	return wh;
}

CV7RowData* CV7CursorGridCtrl::GetRowData() const
{
    if (m_pCurrentData->m_pDataRow)
        return m_pCurrentData;
    else
        return NULL;
}

CV7SelectedRows* CV7CursorGridCtrl::GetV7SelectedRows() const
{
	return m_pV7SelectedRows;
}

CV7FixedRows_head* CV7CursorGridCtrl::GetV7FixedRows_head() const
{
	return m_pV7FixedRows_head;
}

CV7FixedRows_tail* CV7CursorGridCtrl::GetV7FixedRows_tail() const
{
	return m_pV7FixedRows_tail;
}

void CV7CursorGridCtrl::GetCurrentRow(CValue& value)
{
    if (m_pV7RowAppearance->m_pV7DataProvider && m_pCurrentData->m_pDataRow)
        m_pV7RowAppearance->m_pV7DataProvider->GetRowValue(m_pCurrentData->m_pDataRow, value);
}

void CV7CursorGridCtrl::SetCurrentRow(const CValue& value)
{
    if (m_pV7RowAppearance->m_pV7DataProvider)
    {
        CDataRow* pDataRow = m_pV7RowAppearance->m_pV7DataProvider->BuildRowByValue(value);
        if (pDataRow)
        {
            ActivateRow(pDataRow);
            pDataRow->Release();
        }
    }
}

CV7RowAppearance* CV7CursorGridCtrl::GetV7RowAppearance(RegionType rType) const
{
	CV7RowAppearance* pV7RowAppearance = NULL;
	switch (rType)
	{
	case rtBody:
		pV7RowAppearance = m_pV7RowAppearance;
		break;
	case rtHead:
		pV7RowAppearance = m_pV7RowAppearance_head;
		break;
	case rtTail:
		pV7RowAppearance = m_pV7RowAppearance_tail;
		break;
	}
	return pV7RowAppearance;
}

void CV7CursorGridCtrl::GetRowValue(CDataRow* pDataRow, RegionType rType, CValue& val) const
{
	CV7RowAppearance* pV7RowAppearance = GetV7RowAppearance(rType);
	if (pDataRow && pV7RowAppearance && pV7RowAppearance->m_pV7DataProvider)
		pV7RowAppearance->m_pV7DataProvider->GetRowValue(pDataRow, val);
	else
		val.Reset();
}

//Events
void CV7CursorGridCtrl::OnChangeCurrentData(CDataRow* pDataRow)
{
    m_pCurrentData->m_pDataRow = pDataRow;
    if (m_pV7RowAppearance->m_pV7DataProvider)
        m_pCurrentData->m_pV7DataRow = m_pV7RowAppearance->m_pV7DataProvider->GetV7DataRow(pDataRow);
}

void CV7CursorGridCtrl::OnSelection(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
	if (EventManager.IsAppointed(geSelection))
	{
		CValue vValue, vColumn, vRegionType = (long)rType;
		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CTempWndHelper wh = CreateTempWnd();
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke3(geSelection, vValue, vColumn, vRegionType);
	}
}

void CV7CursorGridCtrl::OnActivateRow()
{
    if (EventManager.IsAppointed(geActivateRow))
	{
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke0(geActivateRow);
	}
}

void CV7CursorGridCtrl::OnActivateColumn()
{
    if (EventManager.IsAppointed(geActivateColumn))
	{
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke0(geActivateColumn);
	}
}

void CV7CursorGridCtrl::OnActivateCell()
{
    if (EventManager.IsAppointed(geActivateCell))
	{
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke0(geActivateCell);
	}
}

void CV7CursorGridCtrl::OnHeaderSelection(CGridColumn* pColumn)
{
    if (EventManager.IsAppointed(geHeaderSelection))
    {
        CValue vColumn;
        vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke1(geHeaderSelection, vColumn);
    }
}

void CV7CursorGridCtrl::OnShowContextMenu(RegionType rType, CGridColumn* pColumn, CDataRow* pDataRow)
{
    if (EventManager.IsAppointed(geContextMenu))
	{
		CValue vValue, vColumn, vRegionType = (long)rType;
		GetRowValue(pDataRow, rType, vValue);
        vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CTempWndHelper wh = CreateTempWnd();
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke3(geContextMenu, vRegionType, vColumn, vValue);
	}
}

void CV7CursorGridCtrl::OnClickCheckBox(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
    if (EventManager.IsAppointed(geClickCheckBox))
	{
		CValue vValue, vColumn, vRegionType = (long)rType;
		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke3(geClickCheckBox, vValue, vColumn, vRegionType);
	}
}

#define VALUE_TO_DROPEFFECT(Value) (DROPEFFECT)(Value.GetNumeric().operator long()) & (DROPEFFECT_MOVE | DROPEFFECT_COPY | DROPEFFECT_LINK)

void CV7CursorGridCtrl::OnBeginDrag(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
	CValue vValue, vColumn, vRegionType = (long)rType;
	GetRowValue(pDataRow, rType, vValue);
	vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

	DROPEFFECT dropEffects = DROPEFFECT_COPY;
    DropTargetView SelfDropTargetView = dtvCell;

    if (EventManager.IsAppointed(geDragStart))
	{
		CValue vDropEffect = dropEffects;
        CValue vSelfDropTargetView = SelfDropTargetView;
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        if (!EventManager.Invoke(geDragStart, &vDropEffect, &vValue, &vColumn, &vRegionType, &vSelfDropTargetView))
			return;

		dropEffects = VALUE_TO_DROPEFFECT(vDropEffect);
		SelfDropTargetView = (DropTargetView)vSelfDropTargetView.GetNumeric().operator long();
        if (SelfDropTargetView < dtvNone || SelfDropTargetView > dtvColumn)
		{
			Msg("Недопустимое значение представления цели! (%i)", SelfDropTargetView);
			SelfDropTargetView = dtvNone;
		}
	}

	if (dropEffects == DROPEFFECT_NONE)
		return;

	CV7Control::BeginDrag(vValue);

	CString strDragValue(vValue.Format());

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, strDragValue.GetLength() + 1);
	memcpy(::GlobalLock(hGlobal), strDragValue, strDragValue.GetLength() + 1);
	::GlobalUnlock(hGlobal);

	COleDataSource DS;
	DS.CacheGlobalData(CF_TEXT, hGlobal);

    SetSelfDropTargetView(SelfDropTargetView);

	DROPEFFECT dropEffectRes = DS.DoDragDrop(dropEffects);

    SetSelfDropTargetView(dtvNone);

	CV7Control::EndDrag();

	::GlobalFree(hGlobal);

    if (EventManager.IsAppointed(geDragEnd))
	{
		CValue vDropEffect = dropEffectRes;
        EventManager.Invoke4(geDragEnd, vDropEffect, vValue, vColumn, vRegionType);
	}
}

DROPEFFECT CV7CursorGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, DropTargetView& dropTargetView)
{
	DROPEFFECT dropEffect = DROPEFFECT_NONE;

    if (EventManager.IsAppointed(geDragCheck))
	{

		CValue vDropEffect, vDragValue, vRow, vColumn, vDropTargetView;
		CValue vRegionType = (long)rType;

		vDropEffect = dropEffect;

		CV7Control::GetDragValue(vDragValue, pDataObject);

		GetRowValue(pDataRow, rType, vRow);

		if (pColumn)
			vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));
		
        vDropTargetView = dropTargetView;

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		if (!EventManager.Invoke(geDragCheck, &vDropEffect, &vDragValue, &vRow, &vColumn, &vRegionType, &vDropTargetView))
			return DROPEFFECT_NONE;

		dropEffect = VALUE_TO_DROPEFFECT(vDropEffect);
        dropTargetView = (DropTargetView)vDropTargetView.GetNumeric().operator long();
        if (dropTargetView < dtvNone || dropTargetView > dtvColumn)
		{
            Msg("Недопустимое значение представления цели! (%i)", dropTargetView);
			dropTargetView = dtvNone;
		}
	}
	return dropEffect;
}

void CV7CursorGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
    if (EventManager.IsAppointed(geDrag))
	{
		CValue vDropEffect, vDragValue, vRow, vColumn;
		CValue vRegionType = (long)rType;

		vDropEffect = dropEffect;

		CV7Control::GetDragValue(vDragValue, pDataObject);

		GetRowValue(pDataRow, rType, vRow);

		if (pColumn)
			vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));
		
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke(geDrag, &vDropEffect, &vDragValue, &vRow, &vColumn, &vRegionType);
	}
}

void CV7CursorGridCtrl::OnDestroy()
{
	if (!m_bOnDestroyFired)
	{
		m_bOnDestroyFired = TRUE;
		if (EventManager.IsAppointed(geOnDestroy))
		{
			CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
			CUpdateLock UpdateLock = CreateUpdateLock();
			EventManager.Invoke0(geOnDestroy);
		}
	}
}

void CV7CursorGridCtrl::OnCloseDocument()
{
	POSITION pos = m_map_Grid2Doc.GetStartPosition();
	while (pos)
	{
		CV7CursorGridCtrl* pGrid;
		CGetDoc7* pDoc;

		m_map_Grid2Doc.GetNextAssoc(pos, (void *&)pGrid, (void *&)pDoc);
		if (pDoc == (CGetDoc7*)this)
		{
			pGrid->OnDestroy();
			m_map_Grid2Doc.RemoveKey(pGrid);
		}
	}
	
	m_trOnCloseDocument.Swap();
	m_bOnCloseDocument_trapped = FALSE;
	reinterpret_cast<CGetDoc7*>(this)->CGetDoc7::OnCloseDocument();

	if (!m_map_Grid2Doc.IsEmpty())
	{
		m_trOnCloseDocument.Swap();
		m_bOnCloseDocument_trapped = TRUE;
	}
}

void CV7CursorGridCtrl::OnFillItems(CV7GridRow* pV7GridRow)
{
	if (EventManager.IsAppointed(geRowOut))
	{
		RegionType rType = pV7GridRow->GetRegionType();
		CV7RowAppearance* pV7RowAppearance = GetV7RowAppearance(rType);

		if (pV7RowAppearance)
		{
			pV7RowAppearance->Attach(pV7GridRow);

			CValue vRowAppearance, vRowData, vRegionType = (long)rType;
			vRowAppearance.AssignContext(pV7RowAppearance);
			vRowData.AssignContext(pV7RowAppearance->m_pV7RowData);
			CUpdateLock UpdateLock = CreateUpdateLock();
			EventManager.Invoke3(geRowOut, vRowAppearance, vRowData, vRegionType);

			pV7RowAppearance->Detach();
		}
	}
}

BOOL CV7CursorGridCtrl::OnQuickSearchStart(DataType dt)
{
	const V7CursorGridCtrlEvents nEvent = geOnQuickSearchStart;
	BOOL bRet = TRUE;

    if (EventManager.IsAppointed(nEvent))
	{
		CValue vDataType = (LONG)dt;
		CValue vFSO = 1L;

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke2(nEvent, vDataType, vFSO);

		bRet = (BOOL)vFSO.GetNumeric();
	}
	return bRet;
}

// dtText
BOOL CV7CursorGridCtrl::OnQuickSearch(CString& str)
{
	const V7CursorGridCtrlEvents nEvent = geOnQuickSearch;
	BOOL bRet = TRUE;

    if (EventManager.IsAppointed(nEvent))
	{
		CValue vDataType = (LONG)dtText;
		CValue vData = str;
		CValue vFSO = 1L;

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
        EventManager.Invoke3(nEvent, vDataType, vData, vFSO);

		str = vData.GetString();
		bRet = (BOOL)vFSO.GetNumeric();
	}
	return bRet;
}

void CV7CursorGridCtrl::OnQuickSearchEnd(QuickSearchResult result, BOOL bPositionChanged)
{
	const V7CursorGridCtrlEvents nEvent = geOnQuickSearchEnd;
	if (EventManager.IsAppointed(nEvent))
	{
		CValue vResult = (LONG)result;
		CValue vPositionChanged = bPositionChanged ? 1L : 0L;
		
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke2(nEvent, vResult, vPositionChanged);
	}
}

BOOL CV7CursorGridCtrl::OnColumnMove(CGridColumn* pColumn, int nShift)
{
	const V7CursorGridCtrlEvents nEvent = geOnColumnMove;
	BOOL bRet = TRUE;

	if (EventManager.IsAppointed(nEvent))
	{
		CValue vColumn;
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));
		CValue vShift = nShift;
		CValue vFSO = 1L;

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke3(nEvent, vColumn, vShift, vFSO);

		bRet = (BOOL)vFSO.GetNumeric();
	}
	return bRet;
}

BOOL CV7CursorGridCtrl::OnColumnVisibilityChange(CGridColumn* pColumn, BOOL bNewVisible)
{
	const V7CursorGridCtrlEvents nEvent = geOnColumnVisibilityChange;
	BOOL bRet = TRUE;

	if (EventManager.IsAppointed(nEvent))
	{
		CValue vColumn;
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));
		CValue vNewVisible = bNewVisible ? 1L : 0L;
		CValue vFSO = 1L;

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke3(nEvent, vColumn, vNewVisible, vFSO);

		bRet = (BOOL)vFSO.GetNumeric();
	}
	return bRet;
}

void CV7CursorGridCtrl::OnColumnWidthChange()
{
	const V7CursorGridCtrlEvents nEvent = geOnColumnWidthChange;
	BOOL bRet = TRUE;

	if (EventManager.IsAppointed(nEvent))
	{
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke0(nEvent);
	}
}

//////////////////////////////////////////////////////////////////////////
// class CV7TableField
//////////////////////////////////////////////////////////////////////////

#undef BL_PROC
#define BL_PROC BL_METH_PROC

BEGIN_BL_METH_MAP(CV7TableField)
    BL_METH("RefreshRows", "ОбновитьСтроки", 0, MethRefreshRows, NULL, NULL)
    BL_METH("RedrawRows", "ПерерисоватьСтроки", 0, MethRedrawRows, NULL, NULL)
    BL_METH("SetTextMargins", "УстановитьОтступы", 4, MethSetTextMargins, NULL, defSetTextMargins)
    BL_METH("GetCurrentCellBounds", "ПолучитьГраницыТекущейЯчейки", 5, NULL, funcGetCurrentCellBounds, NULL)
    BL_PROC("AutoWidth", "АвтоШирина", 0, procAutoWidth)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7TableField)
    BL_PROP("Columns", "Колонки", GetPropColumns, NULL)
    BL_PROP("DataProvider", "ПоставщикДанных", GetPropProvider, SetPropProvider)
    BL_PROP("VScroller", "ВертСкроллер", GetPropVScroller, SetPropVScroller)
    BL_PROP("StartView", "НачальноеПредставление", GetPropStartView, SetPropStartView)
    BL_PROP("RowSelectMode", "РежимВыделенияСтроки", GetPropRowSelectMode, SetPropRowSelectMode)
    BL_PROP("SelectMode", "РежимВыделения", GetPropSelectMode, SetPropSelectMode)
    BL_PROP("VerticalLines", "ВертикальныеЛинии", GetPropVerticalLines, SetPropVerticalLines)
    BL_PROP("HorizontalLines", "ГоризонтальныеЛинии", GetPropHorizontalLines, SetPropHorizontalLines)
    BL_PROP("HeadersStyle", "СтильЗаголовков", GetPropHeadersStyle, SetPropHeadersStyle)
    BL_PROP("ActiveHeaderStyle", "СтильАктивногоЗаголовка", GetPropActiveHeaderStyle, SetPropActiveHeaderStyle)
    BL_PROP("CurrentColumn", "ТекущаяКолонка", GetPropCurrentColumn, SetPropCurrentColumn)
    BL_PROP("CurrentData", "ТекущиеДанные", GetPropCurrentData, NULL)
    BL_PROP("SelectedRows", "ВыделенныеСтроки", GetPropSelectedRows, NULL)
    BL_PROP("Font", "Шрифт", GetPropFont, SetPropFont)
    BL_PROP("RefreshTimeout", "ТаймаутОбновления", GetPropRefreshTimeout, SetPropRefreshTimeout)
    BL_PROP("QuickSearchTimeout", "ТаймаутБыстрогоПоиска", GetPropQuickSearchTimeout, SetPropQuickSearchTimeout)
    BL_PROP("ChangePositionOfColumns", "ИзменятьПозициюКолонок", GetPropChangePositionOfColumns, SetPropChangePositionOfColumns)
    BL_PROP("LinesColor", "ЦветЛиний", GetPropLinesColor, SetPropLinesColor)
    BL_PROP("TextColor", "ЦветТекста", GetPropTextColor, SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", GetPropBackColor, SetPropBackColor)
    BL_PROP("SelectionTextColor", "ЦветТекстаВыделения", GetPropSelectionTextColor, SetPropSelectionTextColor)
    BL_PROP("SelectionBackColor", "ЦветФонаВыделения", GetPropSelectionBackColor, SetPropSelectionBackColor)
    BL_PROP("SelectionTextColor2", "ЦветТекстаВыделения2", GetPropSelectionTextColor2, SetPropSelectionTextColor2)
    BL_PROP("SelectionBackColor2", "ЦветФонаВыделения2", GetPropSelectionBackColor2, SetPropSelectionBackColor2)
    BL_PROP("AlternationRowBackColor", "ЦветФонаЧередованияСтрок", GetPropAlternationRowBackColor, SetPropAlternationRowBackColor)
    BL_PROP("UseAlternationRowColor", "ЧередованиеЦветовСтрок", GetPropUseAlternationRowColor, SetPropUseAlternationRowColor)
    BL_PROP("CurrentRow", "ТекущаяСтрока", GetPropCurrentRow, SetPropCurrentRow)
    BL_PROP("РазрешитьНачалоПеретаскивания", "EnableStartDrag", GetPropEnableStartDrag, SetPropEnableStartDrag)
    BL_PROP("РазрешитьПеретаскивание", "EnableDrag", GetPropEnableDrag, SetPropEnableDrag)
    BL_PROP("ShowHeaders", "ОтображатьЗаголовки", GetPropShowHeaders, SetPropShowHeaders)
	BL_PROP("FixedLeft", "ФиксацияСлева", GetPropFixedLeft, SetPropFixedLeft)
	BL_PROP("FixedRight", "ФиксацияСправа", GetPropFixedRight, SetPropFixedRight)
	BL_PROP("Header", "Шапка", GetPropFixedRows_head, NULL)
	BL_PROP("Footer", "Подвал", GetPropFixedRows_tail, NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7TableField, "TableBox", "ТабличноеПоле", 1, NULL, NULL, -1);

CV7TableField::CV7TableField()
{
//	Msg("CV7TableField::CTOR");
    m_pGrid = NULL;
}

CV7TableField::~CV7TableField()
{
//	Msg("CV7TableField::DTOR");
}

//CV7Control
BOOL CV7TableField::CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC)
{
//	Msg("CV7TableField::CreateControlWnd");
    if (pGetField == NULL || pGetField->GetCtrlInfo()->m_CtrlType != 3)
    {
        Msg("Недопустимый атрибут формы.");
        return FALSE;
    }
	
    CControlID* pControlID = pGetField->GetCtrlInfo();
    CRect rect;
    CWnd* pControlWnd = pDoc->GetFieldWnd(pGetField);
    pControlWnd->GetWindowRect(&rect);
    pParent->ScreenToClient(rect);

    CV7CursorGridCtrl* pV7CursorGridCtrl = new CV7CursorGridCtrl();
    BOOL bSuccess = pV7CursorGridCtrl->Create(rect, pParent, pControlID->m_CtrlID, TRUE, pGetField->m_Visible, pGetField->GetReadOnly());
    if (bSuccess)
    {
        bSuccess = pV7CursorGridCtrl->SetWindowPos(pControlWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        bSuccess = pV7CursorGridCtrl->Init(this, pControlID, pDoc, pUDC);
    }
    delete pControlWnd;
    return bSuccess;
}


BOOL CV7TableField::MethRefreshRows(CValue** ppValues)
{
    if (m_pGrid)
        m_pGrid->RefreshRows();
    return TRUE;
}

BOOL CV7TableField::MethRedrawRows(CValue** ppValues)
{
    if (m_pGrid)
        m_pGrid->RedrawRows();
    return TRUE;
}

BOOL CV7TableField::defSetTextMargins(int nParam, CValue* pValue)const
{
	switch (nParam)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		{
			*pValue = -1L;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CV7TableField::MethSetTextMargins(CValue** ppValues)
{
    if (m_pGrid)
	{
		int nLeft = -1, nTop = -1, nRight = -1, nBottom = -1;

		if (ppValues[0]->GetTypeCode() == NUMBER_TYPE_1C)
			nLeft = ppValues[0]->GetNumeric();

		if (ppValues[1]->GetTypeCode() == NUMBER_TYPE_1C)
			nTop = ppValues[1]->GetNumeric();

		if (ppValues[2]->GetTypeCode() == NUMBER_TYPE_1C)
			nRight = ppValues[2]->GetNumeric();

		if (ppValues[3]->GetTypeCode() == NUMBER_TYPE_1C)
			nBottom = ppValues[3]->GetNumeric();

		m_pGrid->SetTextMargins(nLeft, nTop, nRight, nBottom);
	}
    return TRUE;
}

BOOL CV7TableField::funcGetCurrentCellBounds(CValue& RetVal, CValue** ppValues)
{
	BOOL bSuccess = FALSE;
	if (m_pGrid)
	{
		CRect rect;
		if (bSuccess = m_pGrid->GetCurrentCellBounds(rect))
		{
			m_pGrid->ClientToScreen(&rect);

			if (ppValues[4]->GetTypeCode() == NUMBER_TYPE_1C &&
				(LONG)ppValues[4]->GetNumeric() == 1)
			{
				CGetDoc7* pDoc = m_pGrid->GetGetDoc7();
				CWnd* pWnd = NULL;
					if (pDoc && pDoc->ExistWindow())
					{
						POSITION aPOS = pDoc->GetFirstViewPosition();
						pWnd = pDoc->GetNextView(aPOS);
						
						if (!pWnd)
							pWnd = pDoc->GetFrame();
					}
					
					if (pWnd)
						pWnd->ScreenToClient(&rect);
					else
						bSuccess = FALSE;
			}
			
			*ppValues[0] = (LONG)rect.left;
			*ppValues[1] = (LONG)rect.top;
			*ppValues[2] = (LONG)rect.right;
			*ppValues[3] = (LONG)rect.bottom;
		}
	}

	RetVal = bSuccess ? 1L : 0L;

	return TRUE;
}

BOOL CV7TableField::procAutoWidth(CValue** ppValues)
{
    if (m_pGrid)
        m_pGrid->AutoWidth();
    return TRUE;
}

BOOL CV7TableField::GetPropColumns(CValue& Value)const
{
    if (m_pGrid)
        Value.AssignContext(static_cast<CV7GridColumns*>(m_pGrid->GetColumns()));
    return TRUE;
}

BOOL CV7TableField::GetPropProvider(CValue& Value)const
{
    if (m_pGrid)
    {
        if (m_pGrid->GetBLDataProvider())
            Value.AssignContext(m_pGrid->GetBLDataProvider());
        else
            Value.Reset();
    }
    return TRUE;
}

BOOL CV7TableField::SetPropProvider(CValue const& Value)
{
    if (m_pGrid)
    {
        if (Value.GetTypeCode() == AGREGATE_TYPE_1C)
            m_pGrid->SetBLDataProvider(Value.GetContext());
        else
            m_pGrid->SetBLDataProvider(NULL);
    }
    return TRUE;
}
BOOL CV7TableField::GetPropVScroller(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetVScroller();
    return TRUE;
}

BOOL CV7TableField::SetPropVScroller(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(ScrollerPresent, spAuto, spNever);
        m_pGrid->SetVScroller(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropStartView(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetStartView();
    return TRUE;
}

BOOL CV7TableField::SetPropStartView(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(StartView, svFromBegin, svFromEnd);
        m_pGrid->SetStartView(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropRowSelectMode(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetRowSelectMode();
    return TRUE;
}

BOOL CV7TableField::SetPropRowSelectMode(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(RowSelectMode, rsmCell, rsmBoth);
        m_pGrid->SetRowSelectMode(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropSelectMode(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetSelectMode();
    return TRUE;
}

BOOL CV7TableField::SetPropSelectMode(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(SelectMode, smSingleLine, smMultiLine);
        m_pGrid->SetSelectMode(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropVerticalLines(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetVerticalLines();
    return TRUE;
}

BOOL CV7TableField::SetPropVerticalLines(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetVerticalLines(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropHorizontalLines(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetHorizontalLines();
    return TRUE;
}

BOOL CV7TableField::SetPropHorizontalLines(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetHorizontalLines(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropHeadersStyle(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetHeadersStyle();
    return TRUE;
}

BOOL CV7TableField::SetPropHeadersStyle(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(HeadersStyle, hs3D, hsFlat);
        m_pGrid->SetHeadersStyle(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropActiveHeaderStyle(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetActiveHeaderStyle();
    return TRUE;
}

BOOL CV7TableField::SetPropActiveHeaderStyle(CValue const& Value)
{
    if (m_pGrid)
    {
        GET_ENUM_PROP(ActiveHeaderStyle, ahsNone, ahsSelected);
        m_pGrid->SetActiveHeaderStyle(_enum);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropCurrentColumn(CValue& Value)const
{
    if (m_pGrid)
    {
        CGridColumn* pGridColumn = m_pGrid->GetCurrentColumn();
        if (pGridColumn)
            Value.AssignContext(static_cast<CV7GridColumn*>(pGridColumn));
    }
    return TRUE;
}

BOOL CV7TableField::SetPropCurrentColumn(CValue const& Value)
{
    if (m_pGrid)
    {
        CHECK_BLTYPE(Value, CV7GridColumn);
        m_pGrid->SetCurrentColumn(static_cast<CV7GridColumn*>(Value.GetContext()));
    }
    return TRUE;
}

BOOL CV7TableField::GetPropCurrentData(CValue& Value)const
{
    if (m_pGrid)
    {
        CV7RowData* pV7RowData = m_pGrid->GetRowData();
        if (pV7RowData)
            Value.AssignContext(pV7RowData);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropSelectedRows(CValue& Value)const
{
    if (m_pGrid)
    {
		CV7SelectedRows* pV7SelectedRows = m_pGrid->GetV7SelectedRows();
		Value.AssignContext(pV7SelectedRows);
    }
    return TRUE;
}

BOOL CV7TableField::GetPropFont(CValue& Value)const
{
    if (m_pGrid)
        Value.AssignContext(m_pGrid->GetBLFont());
    return TRUE;
}

BOOL CV7TableField::SetPropFont(CValue const& Value)
{
    CHECK_BLTYPE(Value, CBLFont);
    if (m_pGrid)
        m_pGrid->SetBLFont(static_cast<CBLFont*>(Value.GetContext()));
    return TRUE;
}

BOOL CV7TableField::GetPropRefreshTimeout(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetRefreshTimeout();
    return TRUE;
}

BOOL CV7TableField::SetPropRefreshTimeout(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetRefreshTimeout(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropQuickSearchTimeout(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetQuickSearchTimeout();
    return TRUE;
}

BOOL CV7TableField::SetPropQuickSearchTimeout(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetQuickSearchTimeout(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropChangePositionOfColumns(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetChangePositionOfColumns();
    return TRUE;
}

BOOL CV7TableField::SetPropChangePositionOfColumns(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetChangePositionOfColumns(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropLinesColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetGridLinesColor());
    return TRUE;
}

BOOL CV7TableField::SetPropLinesColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetGridLinesColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropTextColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetTextColor());
    return TRUE;
}

BOOL CV7TableField::SetPropTextColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetTextColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropBackColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetBackColor());
    return TRUE;
}

BOOL CV7TableField::SetPropBackColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropSelectionTextColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetSelectionTextColor());
    return TRUE;
}

BOOL CV7TableField::SetPropSelectionTextColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetSelectionTextColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropSelectionBackColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetSelectionBackColor());
    return TRUE;
}

BOOL CV7TableField::SetPropSelectionBackColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetSelectionBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropSelectionTextColor2(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetSelectionTextColor2());
    return TRUE;
}

BOOL CV7TableField::SetPropSelectionTextColor2(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetSelectionTextColor2(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropSelectionBackColor2(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetSelectionBackColor2());
    return TRUE;
}

BOOL CV7TableField::SetPropSelectionBackColor2(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetSelectionBackColor2(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropAlternationRowBackColor(CValue& Value)const
{
    if (m_pGrid)
        Value = GET_V7_COLOR(m_pGrid->GetAlternationRowBackColor());
    return TRUE;
}

BOOL CV7TableField::SetPropAlternationRowBackColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetAlternationRowBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropUseAlternationRowColor(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetUseAlternationRowColor();
    return TRUE;
}

BOOL CV7TableField::SetPropUseAlternationRowColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetUseAlternationRowColor(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropCurrentRow(CValue& Value)const
{
    if (m_pGrid)
        m_pGrid->GetCurrentRow(Value);
    return TRUE;
}

BOOL CV7TableField::SetPropCurrentRow(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetCurrentRow(Value);
    return TRUE;
}

BOOL CV7TableField::GetPropEnableStartDrag(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetEnableDrag();
    return TRUE;
}
BOOL CV7TableField::SetPropEnableStartDrag(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetEnableDrag(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropEnableDrag(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetEnableDrop();
    return TRUE;
}

BOOL CV7TableField::SetPropEnableDrag(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetEnableDrop(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropShowHeaders(CValue& Value)const
{
    if (m_pGrid)
        Value = m_pGrid->GetShowHeader();
    return TRUE;
}

BOOL CV7TableField::SetPropShowHeaders(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetShowHeader(Value.GetNumeric());
    return TRUE;
}

BOOL CV7TableField::GetPropFixedLeft(CValue& Value) const
{
	if (m_pGrid)
		Value = m_pGrid->GetFixedLeft();
	return TRUE;
}

BOOL CV7TableField::SetPropFixedLeft(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetFixedLeft(Value.GetNumeric());
	return TRUE;
}

BOOL CV7TableField::GetPropFixedRight(CValue& Value) const
{
	if (m_pGrid)
		Value = m_pGrid->GetFixedRight();
	return TRUE;
}

BOOL CV7TableField::SetPropFixedRight(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetFixedRight(Value.GetNumeric());
	return TRUE;
}

BOOL CV7TableField::GetPropFixedRows_head(CValue& Value) const
{
	if (m_pGrid)
		Value.AssignContext(m_pGrid->GetV7FixedRows_head());
	else
		Value.Reset();

	return TRUE;
}

BOOL CV7TableField::GetPropFixedRows_tail(CValue& Value) const
{
	if (m_pGrid)
		Value.AssignContext(m_pGrid->GetV7FixedRows_tail());
	else
		Value.Reset();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// class CV7RowData
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CV7RowData, CBLContext)

CV7RowData::CV7RowData() : m_pDataRow(NULL)//, m_pV7DataRow(NULL)
{
}

CV7RowData::~CV7RowData()
{
}

char const* CV7RowData::GetTypeString(void)const
{
    return "ДанныеСтроки";
}

int	CV7RowData::GetNProps(void)const
{
    if (m_pDataRow)
        return m_pDataRow->GetFieldCount();
    else
        return 0;
}

int	CV7RowData::FindProp(char const* szName)const
{
    if (m_pDataRow)
        return m_pDataRow->GetFieldIndexOf(szName);
    else
        return -1;
}

char const* CV7RowData::GetPropName(int nPropIndex, int nAlias)const
{
    if (m_pDataRow)
        return m_pDataRow->GetFieldName(nPropIndex);
    else
        return NULL;
}

int	CV7RowData::GetPropVal(int nPropIndex, class CValue& Value)const
{
    if (m_pV7DataRow)
        Value = m_pV7DataRow->GetValue(nPropIndex);
    return TRUE;
}

int	CV7RowData::SetPropVal(int nPropIndex, class CValue const& Value)
{
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// class CCellAppearance
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CCellAppearance)
    BL_METH("SetText", "УстановитьТекст", 1, MethSetText, NULL, NULL)
    BL_METH("SetPicture", "УстановитьКартинку", 1, MethSetImage, NULL, NULL)
    BL_METH("SetCheck", "УстановитьФлажок", 1, MethSetCheckValue, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CCellAppearance)
    BL_PROP("Name", "Имя", GetPropName, NULL)
    BL_PROP("ShowText", "ОтображатьТекст", GetPropShowText, SetPropShowText)
    BL_PROP("ShowPicture", "ОтображатьКартинку", GetPropShowImage, SetPropShowImage)
    BL_PROP("ShowCheck", "ОтображатьФлажок", GetPropShowCheckBox, SetPropShowCheckBox)
    BL_PROP("HorizontalAlignment", "ГоризонтальноеВыравнивание", GetPropHorizontalAlignment, SetPropHorizontalAlignment)
    BL_PROP("PictureIndex", "ИндексКартинки", GetPropImageIndex, SetPropImageIndex)
    BL_PROP("CheckValue", "ЗначениеФлажка", GetPropCheckValue, SetPropCheckValue)
    BL_PROP("Text", "Текст", GetPropText, SetPropText)
    BL_PROP("TextColor", "ЦветТекста", GetPropTextColor, SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", GetPropBackColor, SetPropBackColor)
    BL_PROP("Font", "Шрифт", GetPropFont, SetPropFont)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CCellAppearance, "CellAppearance", "ОформлениеЯчейки", 0, NULL, NULL, -1);

CCellAppearance::CCellAppearance() : m_pGridItem(NULL)
{
}

CCellAppearance::~CCellAppearance()
{
}

LPCSTR CCellAppearance::GetName() const
{
    if (m_pGridItem)
        return m_pGridItem->GetColumn()->GetName();
    else
        return "";
}

BOOL CCellAppearance::MethSetText(CValue** ppValue)
{
    if (m_pGridItem)
    {
        m_pGridItem->SetShowText();
        m_pGridItem->SetText(ppValue[0]->GetString());
    }
    return TRUE;
}

BOOL CCellAppearance::MethSetImage(CValue** ppValue)
{
    if (m_pGridItem)
    {
        m_pGridItem->SetShowImage();
        if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
            m_pGridItem->SetImageIndex(ppValue[0]->GetNumeric());
        else if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C && !strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext"))
        {
            m_pGridItem->m_pPicture = static_cast<CPictureContext*>(ppValue[0]->GetContext());
            m_pGridItem->SetImageIndex(-1);
        }
        else
        {
            RuntimeError("Недопустимое значение первого параметра.", 0);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CCellAppearance::MethSetCheckValue(CValue** ppValue)
{
    if (m_pGridItem)
    {
        m_pGridItem->SetShowCheckBox();
        m_pGridItem->SetCheckValue(ppValue[0]->GetNumeric());
    }
    return TRUE;
}

BOOL CCellAppearance::GetPropName(CValue& Value)const
{
    Value = GetName();
    return TRUE;
}

BOOL CCellAppearance::GetPropShowText(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetShowText();
    return TRUE;
}

BOOL CCellAppearance::SetPropShowText(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetShowText(Value.GetNumeric());
    return TRUE;
}

BOOL CCellAppearance::GetPropShowImage(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetShowImage();
    return TRUE;
}

BOOL CCellAppearance::SetPropShowImage(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetShowImage(Value.GetNumeric());
    return TRUE;
}

BOOL CCellAppearance::GetPropShowCheckBox(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetShowCheckBox();
    return TRUE;
}

BOOL CCellAppearance::SetPropShowCheckBox(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetShowCheckBox(Value.GetNumeric());
    return TRUE;
}

BOOL CCellAppearance::GetPropHorizontalAlignment(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetTextHAlign();
    return TRUE;
}

BOOL CCellAppearance::SetPropHorizontalAlignment(CValue const& Value)
{
    if (m_pGridItem)
    {
        GET_ENUM_PROP(HorizTextAlignment, htaAuto, htaCenter);
        m_pGridItem->SetTextHAlign(_enum);
    }
    return TRUE;
}

BOOL CCellAppearance::GetPropImageIndex(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetImageIndex();
    return TRUE;
}

BOOL CCellAppearance::SetPropImageIndex(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetImageIndex(Value.GetNumeric());
    return TRUE;
}

BOOL CCellAppearance::GetPropCheckValue(CValue& Value)const
{
    if (m_pGridItem)
        Value = m_pGridItem->GetCheckValue();
    return TRUE;
}

BOOL CCellAppearance::SetPropCheckValue(CValue const& Value)
{
    int nCheckValue = Value.GetNumeric();
    if (nCheckValue < 0 || nCheckValue > 2)
    {
        RuntimeError("Недопустимое значение свойства.", 0);
        return FALSE;
    }
    if (m_pGridItem)
        m_pGridItem->SetCheckValue(nCheckValue);
    return TRUE;
}

BOOL CCellAppearance::GetPropText(CValue& Value)const
{
    if (m_pGridItem)
        m_pGridItem->GetText();
    return TRUE;
}

BOOL CCellAppearance::SetPropText(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetText(Value.GetString());
    return TRUE;
}

BOOL CCellAppearance::GetPropTextColor(CValue& Value)const
{
    if (m_pGridItem)
        Value = GET_V7_COLOR(m_pGridItem->GetTextColor());
    return TRUE;
}

BOOL CCellAppearance::SetPropTextColor(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetTextColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CCellAppearance::GetPropBackColor(CValue& Value)const
{
    if (m_pGridItem)
        Value = GET_V7_COLOR(m_pGridItem->GetBackColor());
    return TRUE;
}

BOOL CCellAppearance::SetPropBackColor(CValue const& Value)
{
    if (m_pGridItem)
        m_pGridItem->SetBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CCellAppearance::GetPropFont(CValue& Value)const
{
    if (m_pGridItem)
        Value.AssignContext(m_pGridItem->m_pFont);
    return TRUE;
}

BOOL CCellAppearance::SetPropFont(CValue const& Value)
{
    if (m_pGridItem)
    {
        CHECK_BLTYPE(Value, CBLFont);
        m_pGridItem->m_pFont = static_cast<CBLFont*>(Value.GetContext());
        m_pGridItem->SetFont(*m_pGridItem->m_pFont);
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CGridCells
//////////////////////////////////////////////////////////////////////////

CGridCells::CGridCells()
{
}

CGridCells::~CGridCells()
{
}

int CGridCells::GetSize() const
{
    if (m_pColumns)
        return m_pColumns->GetSize();
    else
        return 0;
}

CCellAppearance* CGridCells::GetAt(int nIndex) const
{
    CGridColumn* pGridColumn = m_pColumns->GetAt(nIndex);
    if (pGridColumn)
        return static_cast<CV7GridColumn*>(pGridColumn)->m_pCellAppearance;
    else
        return NULL;
}

CCellAppearance* CGridCells::GetAt(LPCSTR szName) const
{
    CGridColumn* pGridColumn = m_pColumns->GetAt(szName);
    if (pGridColumn)
        return static_cast<CV7GridColumn*>(pGridColumn)->m_pCellAppearance;
    else
        return NULL;
}

int CGridCells::IndexOf(LPCSTR szName) const
{
    return m_pColumns->IndexOf(szName);
}

int CGridCells::IndexOf(CCellAppearance* pItemType) const
{
    return m_pColumns->IndexOf(pItemType->GetName());
}

void CGridCells::Attach(CV7GridRow* pV7GridRow)
{
    for (int i = 0; i < m_pColumns->GetSize(); i++)
        static_cast<CV7GridColumn*>(m_pColumns->GetAt(i))->m_pCellAppearance->m_pGridItem = static_cast<CV7GridItem*>(pV7GridRow->GetGridItem(i));
}

void CGridCells::Detach()
{
    for (int i = 0; i < m_pColumns->GetSize(); i++)
        static_cast<CV7GridColumn*>(m_pColumns->GetAt(i))->m_pCellAppearance->m_pGridItem = NULL;
}

//////////////////////////////////////////////////////////////////////////
// class CV7GridCells
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CV7GridCells, CBLContext)

//////////////////////////////////////////////////////////////////////////
// class CV7RowAppearance
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7RowAppearance)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7RowAppearance)
    BL_PROP("RowData", "ДанныеСтроки", GetPropRowData, NULL)
    BL_PROP("Cells", "Ячейки", GetPropCells, NULL)
    BL_PROP("TextColor", "ЦветТекста", GetPropTextColor, SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", GetPropBackColor, SetPropBackColor)
    BL_PROP("Font", "Шрифт", GetPropFont, SetPropFont)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7RowAppearance, "RowAppearance", "ОформлениеСтроки", 0, NULL, NULL, -1);

CV7RowAppearance::CV7RowAppearance() : m_pV7DataProvider(NULL)
{
    m_pV7GridRow = NULL;
    m_pV7RowData.Create();
    m_pV7GridCells.Create();
}

CV7RowAppearance::~CV7RowAppearance()
{
    m_pV7GridCells->m_pColumns = NULL;
}

void CV7RowAppearance::Attach(CV7GridRow* pV7GridRow)
{
    if (m_pV7GridRow)
        Detach();

    m_pV7GridRow = pV7GridRow;
    m_pV7RowData->m_pDataRow = pV7GridRow->GetDataRow();
    if (m_pV7DataProvider)
        m_pV7RowData->m_pV7DataRow = m_pV7DataProvider->GetV7DataRow(pV7GridRow->GetDataRow());
    else
        m_pV7RowData->m_pV7DataRow = NULL;

	m_pV7GridCells->Attach(pV7GridRow);
}

void CV7RowAppearance::Detach()
{
    m_pV7GridCells->Detach();
    m_pV7RowData->m_pDataRow = NULL;
    m_pV7RowData->m_pV7DataRow = NULL;
    m_pV7GridRow = NULL;
}

BOOL CV7RowAppearance::GetPropRowData(CValue& Value)const
{
    Value.AssignContext(m_pV7RowData);
    return TRUE;
}

BOOL CV7RowAppearance::GetPropCells(CValue& Value)const
{
    Value.AssignContext(m_pV7GridCells);
    return TRUE;
}

BOOL CV7RowAppearance::GetPropTextColor(CValue& Value)const
{
    if (m_pV7GridRow)
        Value = GET_V7_COLOR(m_pV7GridRow->GetTextColor());
    return TRUE;
}

BOOL CV7RowAppearance::SetPropTextColor(CValue const& Value)
{
    if (m_pV7GridRow)
        m_pV7GridRow->SetTextColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7RowAppearance::GetPropBackColor(CValue& Value)const
{
    if (m_pV7GridRow)
        Value = GET_V7_COLOR(m_pV7GridRow->GetBackColor());
    return TRUE;
}

BOOL CV7RowAppearance::SetPropBackColor(CValue const& Value)
{
    if (m_pV7GridRow)
        m_pV7GridRow->SetBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7RowAppearance::GetPropFont(CValue& Value)const
{
    if (m_pV7GridRow)
        Value.AssignContext(m_pV7GridRow->m_pFont);
    return TRUE;
}

BOOL CV7RowAppearance::SetPropFont(CValue const& Value)
{
    if (m_pV7GridRow)
    {
        CHECK_BLTYPE(Value, CBLFont);
        m_pV7GridRow->m_pFont = static_cast<CBLFont*>(Value.GetContext());
        m_pV7GridRow->SetFont(*m_pV7GridRow->m_pFont);
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CV7SelectedRows
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7SelectedRows)
    BL_METH_FUNC("Count",			"Количество",	0,		funcCount)
    BL_METH_PROC("Clear",			"Очистить",		0,		procClear)
    BL_METH_FUNC("Get",				"Получить",		1,		funcGet)
    BL_METH_PROC("Add",				"Добавить",		1,		procAdd)
    BL_METH_PROC("Delete",			"Удалить",		1,		procDelete)
    BL_METH_FUNC("Contains",		"Содержит",		1,		funcContains)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7SelectedRows)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7SelectedRows, "SelectedRows", "ВыделенныеСтроки", 0, NULL, NULL, -1);

CV7SelectedRows::CV7SelectedRows() : m_pSelectedRows(NULL), m_pV7DataProvider(NULL), m_pGrid(NULL)
{
}

CV7SelectedRows::~CV7SelectedRows()
{
}

void CV7SelectedRows::aux_Check() const
{
	if (!m_pSelectedRows || !m_pGrid)
		RuntimeError("Табличное поле не инициализировано!");

	if (!m_pV7DataProvider)
		RuntimeError("Поставщик данных табличного поля не установлен!");
}

// selected data row only
CDataRow* CV7SelectedRows::aux_GetCurrentDataRow() const
{
	CDataRow* pDataRow = m_pGrid->GetCurrentDataRow();
// 	if (m_pSelectedRows->IsRemoved(pDataRow) || m_pSelectedRows->IsSelected(pDataRow))
	if (m_pSelectedRows->GetSize() || m_pSelectedRows->IsRemoved(pDataRow))
		pDataRow = NULL;

	return pDataRow;
}

BOOL CV7SelectedRows::funcCount(CValue& RetVal, CValue** ppValues)
{
	aux_Check();

	int nSize = m_pSelectedRows->GetSize();
	if (aux_GetCurrentDataRow())
		nSize++;

	RetVal = nSize;
	return TRUE;
}

BOOL CV7SelectedRows::procClear(CValue** ppValue)
{
	aux_Check();

	m_pSelectedRows->RemoveAll();
	m_pGrid->Invalidate();
    return TRUE;
}

BOOL CV7SelectedRows::funcGet(CValue& RetVal, CValue** ppValues)
{
	aux_Check();

	UINT nIndex = ppValues[0]->GetNumeric();
	CDataRow* pDataRow;
	
	if (nIndex == m_pSelectedRows->GetSize())
		pDataRow = aux_GetCurrentDataRow();
	else
		pDataRow = m_pSelectedRows->GetAt(nIndex);

	if (pDataRow)
		m_pV7DataProvider->GetRowValue(pDataRow, RetVal);
	else
		RuntimeError("Неверный индекс!");

	return TRUE;
}

BOOL CV7SelectedRows::procAdd(CValue** ppValues)
{
	aux_Check();

	CDataRow* pDataRow = m_pV7DataProvider->BuildRowByValue(*ppValues[0]);
	if (pDataRow)
	{
		m_pSelectedRows->Add(pDataRow);
		pDataRow->Release();
		m_pGrid->Invalidate();
	}
	return TRUE;
}

BOOL CV7SelectedRows::procDelete(CValue** ppValues)
{
	aux_Check();

	CDataRow* pDataRow = m_pV7DataProvider->BuildRowByValue(*ppValues[0]);
	if (pDataRow)
	{
		m_pSelectedRows->Remove(pDataRow);
		pDataRow->Release();
		m_pGrid->Invalidate();
	}
	return TRUE;
}

BOOL CV7SelectedRows::funcContains(CValue& RetVal, CValue** ppValues)
{
	aux_Check();

	BOOL bSuccess = FALSE;
	CDataRow* pDataRow = m_pV7DataProvider->BuildRowByValue(*ppValues[0]);
	if (pDataRow)
	{
		CDataRow* pCurrentDataRow = aux_GetCurrentDataRow();
		if (pCurrentDataRow)
		{
			CDataProvider* pProvider = dynamic_cast<CDataProvider*>(m_pV7DataProvider);
			bSuccess = pProvider->CompareRows(pDataRow, pCurrentDataRow);
		}
		if (!bSuccess)
			bSuccess = m_pSelectedRows->IsSelected(pDataRow);

		pDataRow->Release();
	}

	RetVal = bSuccess ? 1L : 0L;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CV7FixedRows
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7FixedRows)
 	BL_METH_PROC("RefreshRows", "ОбновитьСтроки", 0, procRefreshRows)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7FixedRows)
	BL_PROP("DataProvider", "ПоставщикДанных", propGetDataProvider, propSetDataProvider)
	BL_PROP("TextColor", "ЦветТекста", propGetTextColor, propSetTextColor)
	BL_PROP("BackColor", "ЦветФона", propGetBackColor, propSetBackColor)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7FixedRows, "FixedRows", "ФиксированныеСтроки", 0, NULL, NULL, -1);

CV7FixedRows::CV7FixedRows() : m_pGrid(NULL)
{
}

CV7FixedRows::~CV7FixedRows()
{
}

void CV7FixedRows::aux_Check() const
{
	if (!m_pGrid)
		RuntimeError("Табличное поле не инициализировано!");
}

BOOL CV7FixedRows::propGetDataProvider(CValue& Value) const
{
	aux_Check();
	Value.AssignContext(GetBLDataProvider());

	return TRUE;
}

BOOL CV7FixedRows::propSetDataProvider(CValue const& Value)
{
	aux_Check();
	CBLContext* pCont = Value.GetTypeCode() == AGREGATE_TYPE_1C ? Value.GetContext() : NULL;
	SetBLDataProvider(pCont);

	return TRUE;
}

BOOL CV7FixedRows::propGetTextColor(CValue& Value) const
{
	aux_Check();
	Value = GET_V7_COLOR(GetTextColor());

	return TRUE;
}

BOOL CV7FixedRows::propSetTextColor(CValue const& Value)
{
	aux_Check();
	SetTextColor(SET_V7_COLOR(Value));

	return TRUE;
}

BOOL CV7FixedRows::propGetBackColor(CValue& Value) const
{
	aux_Check();
	Value = GET_V7_COLOR(GetBackColor());

	return TRUE;
}

BOOL CV7FixedRows::propSetBackColor(CValue const& Value)
{
	aux_Check();
	SetBackColor(SET_V7_COLOR(Value));

	return TRUE;
}

BOOL CV7FixedRows::procRefreshRows( CValue** ppValue )
{
	aux_Check();
	RefreshRows();

	return TRUE;
}
