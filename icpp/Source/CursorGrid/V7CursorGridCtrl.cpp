#include "StdAfx.h"
#include "V7CursorGridCtrl.h"
#include "TableFieldGetCtrl.h"
#include "../Utils1s/Context/ContextBase.h"
#include "../Utils1s/Interfaces.h"
#include "../Keyboard/V7Keyboard.h"


//////////////////////////////////////////////////////////////////////////
// class CV7GridColumn
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7GridColumn)
	BL_METH("SetRowsPictures", "УстановитьКартинкиСтрок", 2, &MethSetRowsPictures, NULL, &DefSetRowsPictures)
	BL_METH("AddRowsPictures", "ДобавитьКартинкиСтрок", 1, &MethAddRowsPictures, NULL, NULL)
	BL_METH("ResetRowsPictures", "СброситьКартинкиСтрок", 0, &MethResetRowsPictures, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7GridColumn)
    BL_PROP("Name", "Имя", &GetPropName, &SetPropName)
    BL_PROP("Caption", "Заголовок", &GetPropCaption, &SetPropCaption)
    BL_PROP("Visible", "Видимость", &GetPropVisible, &SetPropVisible)
    BL_PROP("Width", "Ширина", &GetPropWidth, &SetPropWidth)
    BL_PROP("AutoWidth", "АвтоШирина", &GetPropAutoWidth, NULL)
    BL_PROP("HorizontalAlignment", "ГоризонтальноеВыравнивание", &GetPropHorizontalAlignment, &SetPropHorizontalAlignment)
    BL_PROP("ИзменениеРазмера", "SizeChange", &GetPropFixedWidth, &SetPropFixedWidth)
    BL_PROP("TextColor", "ЦветТекста", &GetPropTextColor, &SetPropTextColor)
	BL_PROP("BackColor", "ЦветФона", &GetPropBackColor, &SetPropBackColor)
	BL_PROP("BackTransparency", "ПрозрачностьФона", &GetPropBackTransparency, &SetPropBackTransparency)
    BL_PROP("Font", "Шрифт", &GetPropFont, &SetPropFont)
    BL_PROP("HeaderFont", "ШрифтЗаголовка", &GetPropHeaderFont, &SetPropHeaderFont)
    BL_PROP("HeaderTextColor", "ЦветТекстаЗаголовка", &GetPropHeaderTextColor, &SetPropHeaderTextColor)
    BL_PROP("HeaderBackColor", "ЦветФонаЗаголовка", &GetPropHeaderBackColor, &SetPropHeaderBackColor)
    BL_PROP("DataField", "Данные", &GetPropDataField, &SetPropDataField)
    BL_PROP("PictureDataField", "ДанныеКартинки", &GetPropImageDataField, &SetPropImageDataField)
    BL_PROP("OrderMarker", "ЗнакУпорядочивания", &GetPropOrderMarker, &SetPropOrderMarker)
    BL_PROP("ChangeVisible", "ИзменятьВидимость", &GetPropChangeVisible, &SetPropChangeVisible)
    BL_PROP("Parameter", "Параметр", &GetPropParameter, &SetPropParameter)
    BL_PROP("Location", "Положение", &GetPropLocation, &SetPropLocation)
    BL_PROP("MergeCaption", "ОбъединитьЗаголовок", &GetPropMergeCaption, &SetPropMergeCaption)
    BL_PROP("WordBreak", "ПереноситьПоСловам", &GetPropWordBreak, &SetPropWordBreak)
    BL_PROP("Format", "Формат", &GetPropFormat, &SetPropFormat)
    BL_PROP("RealWidth", "РеалШирина", &GetPropRealWidth, NULL)
	BL_PROP("MarkNegatives", "ВыделятьОтрицательные", &GetPropMarkNegatives, &SetPropMarkNegatives)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7GridColumn, "TableBoxColumn", "КолонкаТабличногоПоля", 0, NULL, NULL, -1);

CV7GridColumn::CV7GridColumn()
{
    m_pCellAppearance.Create();
	m_pCellFormat.Create();
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

	BOOL bSuccess = m_RowsImages.CreateFrom(&PictureHolder7, ppValue[1]->GetNumeric());
	if (bSuccess)
		SetRowsImages(&m_RowsImages);
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

BOOL CV7GridColumn::MethAddRowsPictures(CValue** ppValue)
{
	if (ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C || strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext"))
	{
		RuntimeError("Недопустимое значение первого параметра.", 0);
		return FALSE;
	}
	CPictureHolder7& PictureHolder7 = static_cast<CPictureContext*>(ppValue[0]->GetContext())->m_PictureHolder7;
	return m_RowsImages.AddFrom(&PictureHolder7);
}

BOOL CV7GridColumn::MethResetRowsPictures(CValue** ppValue)
{
	m_RowsImages.Reset();
	SetRowsImages(NULL);
	return TRUE;
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
	if (m_pPicture)
		return CV7CursorGridCtrl::DrawPicture(m_pPicture, pDC, rect);
	else
		return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// class CV7GridRow
//////////////////////////////////////////////////////////////////////////

CV7GridRow::CV7GridRow(CGridColumns* pColumns, CDataRowPtr const& pDataRow, RegionType rt)
    : CGridRow(pColumns, pDataRow, rt)
{
    m_pFont.Create();
}

CV7GridRow::~CV7GridRow()
{
}

CGridItem* CV7GridRow::NewItem(CGridColumn* pGridColumn)
{
    CGridItem* pItem = new CV7GridItem(pGridColumn);
	pItem->InitImageRefs();

	return pItem;
}

void CV7GridRow::OnFillItems()
{
    static_cast<CV7CursorGridCtrl*>(GetGrid())->OnFillItems(this);
}

//////////////////////////////////////////////////////////////////////////
// class CV7NativeCellEdit
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// class CV7CursorGridCtrl
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CV7CursorGridCtrl, CCursorGridCtrl)

// static objects
CMapPtrToPtr CV7CursorGridCtrl::m_map_Grid2Doc;
CTrapSwap CV7CursorGridCtrl::m_trOnCloseDocument;
CTrapSwap CV7CursorGridCtrl::m_trOnActivateCtrl;
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
	geClickPicture,
	geClickSysPicture,
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
	geOnEditValue,
	geOnCancelEditValue,
	geAfterEditValue,
	geOnKeyDown,
	geOnKeyUp,
	geOnSysKeyDown,
	geOnSysKeyUp,
	geOnChar,
	geOnShowHint,
	geOnFind,
	geOnFindDialog,
	geOnRowFormat,
	geOnClipboard,
	geOnActivate,
	geOnDeactivate,
	geLastEvent
};

stCtrlEvents CV7CursorGridCtrlEvents[] = 
{
	{"ПриВыводеСтроки", "RowOut", 3},
    {"Выбор", "Selection", 4},
    {"ПриАктивизацииСтроки", "ActivateRow", 0},
    {"ПриАктивизацииКолонки", "ActivateColumn", 0},
    {"ПриАктивизацииЯчейки", "ActivateCell", 0},
    {"ПриВыбореЗаголовка", "HeaderSelection", 1},
    {"ПриКонтекстномМеню", "ContextMenu", 3},
	{"ПриВыбореФлажка", "ClickCheckBox", 4},
	{"ПриВыбореКартинки", "ClickPicture", 5},
	{"ПриВыбореСистемнойКартинки", "ClickSysPicture", 5},
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
	{"ПриРедактированииЗначения", "OnEditValue", 5},
	{"ПриОтменеРедактированияЗначения", "OnCancelEditValue", 3},
	{"ПослеРедактированияЗначения", "AfterEditValue", 3},

	{"ПриНажатииКлавиши", "OnKeyDown", 4},
	{"ПриОтжатииКлавиши", "OnKeyUp", 4},
	{"ПриНажатииСистемнойКлавиши", "OnSysKeyDown", 4},
	{"ПриОтжатииСистемнойКлавиши", "OnSysKeyUp", 4},
	{"ПриВводеСимвола", "OnChar", 4},

	{"ПриВыводеПодсказки", "OnShowHint", 7},
	{"ПриПоиске", "OnFind", 2},
	{"ПриДиалогеПоиска", "OnFindDialog", 2},
	{"ПриФорматированииСтроки", "OnRowFormat", 2},
	{"ПриКомандеБуфераОбмена", "OnClipboard", 3},

    {"ПриАктивизации", "OnActivate", 0},
    {"ПриДеактивизации", "OnDeactivate", 0},
};

CBLContext* CV7CursorGridCtrl::GetControlContext() const
{
	CBLContext* pCont = EventManager.GetDerivedObject();
	if (!pCont)
		pCont = EventManager.GetCtrlObject();

	return pCont;
}

CV7CursorGridCtrl::CV7CursorGridCtrl()
{
	m_bOnDestroyFired = FALSE;
	m_bOnActivateCtrl = FALSE;

	m_pFont.Create();
    m_pV7RowAppearance.Create();
	m_pV7RowAppearance_head.Create();
	m_pV7RowAppearance_tail.Create();
    m_pCurrentData.Create();
	m_pV7SelectedRows.Create();
	m_pV7FixedRows_head.Create();
	m_pV7FixedRows_tail.Create();
	m_pV7RowFormat.Create();

	m_BackPicHA = hbaCenter;
	m_BackPicVA = vbaCenter;
	m_crBackPicTransparentColor = CLR_DEFAULT;
	m_bBackPicRepeat = FALSE;
}

CV7CursorGridCtrl::~CV7CursorGridCtrl()
{
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
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CV7CursorGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	if (CloseEdit(TRUE) && !GetEdit())
	{
		CCursorGridCtrl::OnSetFocus(pOldWnd);
		m_pDoc->SetActiveCtl(GetDlgCtrlID());
	}

	if( EventManager.IsAppointed(geOnActivate) )
	{
		EventManager.Invoke0(geOnActivate);
	}
}

void CV7CursorGridCtrl::OnKillFocus(CWnd* pOldWnd) 
{
	if (CloseEdit(TRUE) && !GetEdit())
	{
		CCursorGridCtrl::OnKillFocus(pOldWnd);
	}

	if( EventManager.IsAppointed(geOnDeactivate) )
	{
		EventManager.Invoke0(geOnDeactivate);
	}
}

CGridColumns* CV7CursorGridCtrl::NewColumns()
{
    CV7GridColumns* pV7GridColumns = new CV7GridColumns;
    m_pV7RowAppearance->m_pV7GridCells->m_pColumns = pV7GridColumns;
	m_pV7RowAppearance_head->m_pV7GridCells->m_pColumns = pV7GridColumns;
	m_pV7RowAppearance_tail->m_pV7GridCells->m_pColumns = pV7GridColumns;
	m_pV7RowFormat->SetColumns(pV7GridColumns);
    return pV7GridColumns;
}

CBLFont* CV7CursorGridCtrl::GetBLFont() const
{
	if (!m_pFont->operator CFont*())
		m_pFont->Attach(GetFont());

    return m_pFont;
}

void CV7CursorGridCtrl::SetBLFont(CBLFont* pFont)
{
	m_pFont = pFont;
 	SetFont(*m_pFont);
}

CDataProvider* CV7CursorGridCtrl::ProviderCheckedDowncast(CBLContext* pProvider)
{
	CDataProvider* pDataProvider = NULL;
	if (pProvider && pProvider->IsKindOf(RUNTIME_CLASS(CComponentClass)))
		static_cast<IUserClassContext*>(pProvider)->GetBaseClass(&pDataProvider);
	else
	{
		try
		{
			pDataProvider = dynamic_cast<CDataProvider*>(pProvider);
		}
		catch (NON_RTTI_OBJECT const&)
		{
			RuntimeError("Значение не является поставщиком данных!");
		}
	}

	return pDataProvider;
}

CBLContext* CV7CursorGridCtrl::GetBLDataProvider() const
{
    return m_pProvider;
}

void CV7CursorGridCtrl::SetBLDataProvider(CBLContext* pProvider)
{
	CDataProvider* pDataProvider = ProviderCheckedDowncast(pProvider);
    CV7DataProvider* pV7DataProvider = NULL;

	SetDataProvider(NULL);

	if (pDataProvider)
		pV7DataProvider = dynamic_cast<CV7DataProvider*>(pDataProvider);
	else
	{
		Utils1s::Context::CContextBase *pUtils1sContext =
			safe_dynamic_cast<Utils1s::Context::CContextBase *> (pProvider);

		if (pUtils1sContext)
		{
			GetProvidersFromUtils1sContext (pUtils1sContext, pDataProvider,
				pV7DataProvider);
		}
	}
    m_pProvider = pProvider;

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
	CDataProvider* pDataProvider = ProviderCheckedDowncast(pProvider);
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
	CDataProvider* pDataProvider = ProviderCheckedDowncast(pProvider);
	SetDataProvider_tail(NULL);

	CV7DataProvider* pV7DataProvider = NULL;
	pV7DataProvider = dynamic_cast<CV7DataProvider*>(pProvider);
	m_pV7RowAppearance_tail->m_pV7DataProvider = pV7DataProvider;

	m_pProvider_tail = pProvider;
	SetDataProvider_tail(pDataProvider);
}

CGridRow* CV7CursorGridCtrl::NewGridRow(CGridColumns* pColumns, CDataRowPtr const& pDataRow, RegionType rt)
{
    return new CV7GridRow(pColumns, pDataRow, rt);
}

BOOL CV7CursorGridCtrl::FormatField(CDataRow* pDataRow, int nIndex, CGridColumn* pColumn, CString &strValue)
{
	CV7GridColumn* pV7GridColumn = static_cast<CV7GridColumn*>(pColumn);
	const CString& strFormat = pV7GridColumn->GetFormat();
	if (strFormat.IsEmpty())
		return FALSE;

	CBLModule* pModule = m_pDoc->m_pBLModule;
	if (!pModule)
		return FALSE;

	CV7DataRow* pV7DataRow = dynamic_cast<CV7DataRow*>(pDataRow);
	const CValue &val = pV7DataRow->GetValue(nIndex);
	strValue = pModule->FormatValue(val, strFormat);
	if ( dtNumeric == pDataRow->GetFieldType(nIndex) )
		strValue.TrimLeft();
	return TRUE;
}

void CV7CursorGridCtrl::PreInit(CGetDoc7* pDoc)
{
	m_pDoc = pDoc;
}

BOOL CV7CursorGridCtrl::Init(CV7TableField* pTableField, CControlID* pControlID, CBLContext* pCtrlObject)
{
    pTableField->m_pGrid = this;

	EventManager.SetEvents(CV7CursorGridCtrlEvents, geLastEvent,
        pControlID, (CBLModule7*)(((char*)m_pDoc) + 0x4E0), pTableField,
        pCtrlObject);

	if (!m_trOnCloseDocument.IsTrapped())
		m_trOnCloseDocument.SetTrapByName("basic.dll", "?OnCloseDocument@CGetDoc7@@UAEXXZ", &CV7CursorGridCtrl::OnCloseDocument);

	if (!m_trOnActivateCtrl.IsTrapped())
		m_trOnActivateCtrl.SetTrapByName("basic.dll", "?OnActivateCtrl@CGetDoc7@@MAEXI@Z", &CV7CursorGridCtrl::OnActivateCtrl);

	m_map_Grid2Doc.SetAt(this, m_pDoc);

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

void CV7CursorGridCtrl::EditValue(CValue** ppValues)
{
	if (!CloseEdit(FALSE))
		RuntimeError("Ошибка при начале редактирования значения (возможно, вложенный вызов)");

	CValue const& vRow = *ppValues[0];
	CValue const& vColumn = *ppValues[1];
	CValue const& vRegionType =*ppValues[2];
	CValue const& vValue = *ppValues[3];
	CValue const& vLen = *ppValues[4];
	CValue const& vPrec = *ppValues[5];
	CValue const& vID = *ppValues[6];
	CValue const& vFlags = *ppValues[7];

	RegionType rType = (RegionType)vRegionType.GetNumeric().operator long();
	if (rType < rtNone || rType > rtTail)
		RuntimeError("Неверно задан тип региона '%i'!", rType);

	if (!IS_BLTYPE(vColumn, CV7GridColumn))
		RuntimeError("Неверно задана колонка '%s'!", (LPCSTR)vColumn.GetString());
	CV7GridColumn* pColumn = static_cast<CV7GridColumn*>(vColumn.GetContext());
	if (rType == rtBody || GetColumnClipRect(pColumn).Width() == 0)
		SetCurrentColumn(pColumn);

	int nRowIndex = 0;
	if (rType != rtHeader)
	{
		CV7RowAppearance* pV7RowAppearance = GetV7RowAppearance(rType);
		CV7DataProvider* pProvider = pV7RowAppearance->m_pV7DataProvider;
		if (!pProvider)
			RuntimeError("Поставщик данных региона %i не определен!", rType);

		CDataRowPtr pDataRow(pProvider->BuildRowByValue(vRow));
		if (!pDataRow)
			RuntimeError("Неверно задана строка '%s'!", (LPCSTR)vRow.GetString());

		nRowIndex = GetVisibleRowIndex(rType, pDataRow.get());
		if (nRowIndex == -1 && rType == rtBody)
		{
			ActivateRow(pDataRow.get());
			nRowIndex = GetVisibleRowIndex(rType, pDataRow.get());
		}
		if (nRowIndex == -1)
			RuntimeError("Невозможно спозиционироваться на строке '%s'!", (LPCSTR)vRow.GetString());
	}
	else
	{
		if (!GetShowHeader())
			RuntimeError("Заголовки не отображаются. Редактирование невозможно!");
	}

	int nLen = (long)vLen.GetNumeric();
	int nPrec = (long)vPrec.GetNumeric();

	CString const& csID = vID.GetString();
	if (!csID.IsEmpty())
	{
		CGetField* pField = m_pDoc->GetField((LPCSTR)csID);
		if (pField)
			RuntimeError("Элемент управления с идентификатором '%s' уже определен!", (LPCSTR)csID);
	}
	
	DWORD nFlags = 0;
	switch (vFlags.GetTypeCode())
	{
	case NUMBER_TYPE_1C:
		{
			nFlags = (long)vFlags.GetNumeric();
		}
		break;
	case STRING_TYPE_1C:
		{
			LPSTR endptr;
			LPCSTR buf = vFlags.GetString().operator LPCSTR();
			nFlags = strtol(buf, &endptr, 0);
		}
		break;
	}

	SetEdit(new CV7NativeCellEdit(this, rType, pColumn, nRowIndex, vValue, nLen, nPrec, csID, nFlags));
}

BOOL CV7CursorGridCtrl::EndEditValue(BOOL bFireEvent)
{
	return CloseEdit(bFireEvent);
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
        CDataRowPtr pDataRow(m_pV7RowAppearance->m_pV7DataProvider->BuildRowByValue(value));
        if (pDataRow)
            ActivateRow(pDataRow.get());
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

// return: TRUE: edit closed successfully (or was closed already)
BOOL CV7CursorGridCtrl::CloseEdit(BOOL bFireEvent)
{
	CV7GridCellEdit* pEdit = GetEdit();
	boost::shared_ptr<CSimpleLock> CloseEditLock(CreateEditLock());
	if (pEdit)
	{
		if (!pEdit->OnDestroy(bFireEvent))
			return FALSE;

		if (EventManager.IsAppointed(geOnEditValue) ||
			EventManager.IsAppointed(geOnCancelEditValue) ||
			EventManager.IsAppointed(geAfterEditValue))
		{
			RegionType rType = pEdit->GetRegionType();
			CGridColumn* pColumn = pEdit->GetGridColumn();
			CDataRow* pDataRow = pEdit->GetDataRow();

			CValue vRow, vColumn, vRegionType = (long)rType;
			GetRowValue(pDataRow, rType, vRow);
			vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

			if (bFireEvent && EventManager.IsAppointed(geOnEditValue))
			{
				CValue vValue, vFSO = 1L;
				pEdit->GetValue(vValue);

				CUpdateLock UpdateLock = CreateUpdateLock();
				EventManager.Invoke5(geOnEditValue, vRow, vColumn, vRegionType, vValue, vFSO);
				if (!vFSO.GetNumeric())
					return FALSE;
			}

			if (!bFireEvent && EventManager.IsAppointed(geOnCancelEditValue))
			{
				CUpdateLock UpdateLock = CreateUpdateLock();
				EventManager.Invoke3(geOnCancelEditValue, vRow, vColumn, vRegionType);
			}

			SetEdit(NULL);
			delete pEdit;

			// в случае отмены событие принципиально не генерируется - небезопасно
			if (bFireEvent && EventManager.IsAppointed(geAfterEditValue))
			{
				// можно открыть новое редактирование
				CloseEditLock.reset();

				CUpdateLock UpdateLock = CreateUpdateLock();
				EventManager.Invoke3(geAfterEditValue, vRow, vColumn, vRegionType);
			}
		}
	}
	return TRUE;
}

//Events
void CV7CursorGridCtrl::OnChangeCurrentData(CDataRow* pDataRow)
{
    m_pCurrentData->m_pDataRow = pDataRow;
    if (m_pV7RowAppearance->m_pV7DataProvider)
        m_pCurrentData->m_pV7DataRow = m_pV7RowAppearance->m_pV7DataProvider->GetV7DataRow(pDataRow);
}

void CV7CursorGridCtrl::OnSelection(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, GridHitTest htArea)
{
	if (EventManager.IsAppointed(geSelection))
	{
		CValue vValue, vColumn, vArea;
		CValue vRegionType((long)rType);
		switch(htArea)
		{
		case ghtCell:
			vArea = 0L;
			break;
		case ghtImage:
			vArea = 1;
		    break;
		case ghtCheckBox:
			vArea = 2;
			break;
		case ghtSysImage:
			vArea = 3;
			break;
		default:
			vArea = -1;
		    break;
		}

		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CTempWndHelper wh = CreateTempWnd();
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke4(geSelection, vValue, vColumn, vRegionType, vArea);
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
//		Наличие CUpdateLock приводит к отложенному позиционированию ТП строке при вызове из меню
// 		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke3(geContextMenu, vRegionType, vColumn, vValue);
	}
}

void CV7CursorGridCtrl::OnClickCheckBox(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType)
{
	if (EventManager.IsAppointed(geClickCheckBox))
	{
		CValue vValue, vColumn;
		CValue vRegionType((long)rType);
		CValue vSelOnDblClk(m_bSelOnDblClk);

		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke4(geClickCheckBox, vValue, vColumn, vRegionType, vSelOnDblClk);
		m_bSelOnDblClk = vSelOnDblClk.GetNumeric();
	}
}

void CV7CursorGridCtrl::OnClickImage(CDataRow* pDataRow, CGridColumn* pColumn, int nPicIndex, RegionType rType)
{
	if (EventManager.IsAppointed(geClickPicture))
	{
		CValue vValue, vColumn;
		CValue vIndex(nPicIndex);
		CValue vRegionType((long)rType);
		CValue vSelOnDblClk(m_bSelOnDblClk);

		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke(geClickPicture, &vValue, &vColumn, &vIndex, &vRegionType, &vSelOnDblClk);
		m_bSelOnDblClk = vSelOnDblClk.GetNumeric();
	}
}

void CV7CursorGridCtrl::OnClickSysImage(CDataRow* pDataRow, CGridColumn* pColumn, int nPicIndex, RegionType rType)
{
	if (EventManager.IsAppointed(geClickSysPicture))
	{
		CValue vValue, vColumn;
		CValue vIndex(nPicIndex);
		CValue vRegionType((long)rType);
		CValue vSelOnDblClk(m_bSelOnDblClk);

		GetRowValue(pDataRow, rType, vValue);
		vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke(geClickSysPicture, &vValue, &vColumn, &vIndex, &vRegionType, &vSelOnDblClk);
		m_bSelOnDblClk = vSelOnDblClk.GetNumeric();
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
		if (!CloseEdit(TRUE))
			CloseEdit(FALSE);
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
	reinterpret_cast<CGetDoc7*>(this)->CGetDoc7::OnCloseDocument();

	if (!m_map_Grid2Doc.IsEmpty())
		m_trOnCloseDocument.Swap();
}

void CV7CursorGridCtrl::OnActivateCtrl(UINT nCtrl)
{
	POSITION pos = m_map_Grid2Doc.GetStartPosition();
	while (pos)
	{
		CV7CursorGridCtrl* pGrid;
		CGetDoc7* pDoc;

		m_map_Grid2Doc.GetNextAssoc(pos, (void *&)pGrid, (void *&)pDoc);
		if (pDoc == (CGetDoc7*)this)
		{
			CSimpleLock OnActivateCtrlLock(m_bOnActivateCtrl);
			pGrid->CloseEdit(TRUE);
		}
	}
	m_trOnActivateCtrl.Swap();
	reinterpret_cast<CGetDoc7_friend*>(this)->CGetDoc7_friend::OnActivateCtrl(nCtrl);

	if (!m_map_Grid2Doc.IsEmpty())
 		m_trOnActivateCtrl.Swap();
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

void CV7CursorGridCtrl::OnRowFind(CDataRowPtr const& ptrDataRow)
{
	if (EventManager.IsAppointed(geOnRowFormat))
	{
		if (m_pV7RowFormat)
		{
			CValue vRowFormat;
			vRowFormat.AssignContext(m_pV7RowFormat);

			CV7RowData RowData(ptrDataRow.get(), GetDataProvider(rtBody));
			CValue vRowData;
			vRowData.AssignContext(&RowData);

			CUpdateLock UpdateLock = CreateUpdateLock();
			EventManager.Invoke2(geOnRowFormat, vRowFormat, vRowData);
		}
	}
	else if (EventManager.IsAppointed(geRowOut))
	{
		CGridColumns* pColumns = GetColumns();
		CV7GridRow GridRow(pColumns, ptrDataRow, rtBody);
		
		GridRow.CreateItems();
		for (int i = 0; i < pColumns->GetSize(); i++)
		{
			CV7GridColumn* pV7Column = static_cast<CV7GridColumn*>(pColumns->GetAt(i));
			GridRow.GetGridItem(i)->SetText(pV7Column->m_pCellFormat->GetText());
		}
		//GridRow.FillFromArray(pItems);
		GridRow.OnFillItems();

		for (int i = 0; i < pColumns->GetSize(); i++)
		{
			CV7GridColumn* pV7Column = static_cast<CV7GridColumn*>(pColumns->GetAt(i));
			pV7Column->m_pCellFormat->SetText(GridRow.GetGridItem(i)->GetText());
		}
	}
}

BOOL CV7CursorGridCtrl::OnFind(CV7GlobalFindData* pV7GlobalFindData)
{
	if (EventManager.IsAppointed(geOnFind))
	{
		CValue vGlobalFindData;
		vGlobalFindData.AssignContext(pV7GlobalFindData);
		CValue vFSP(1);

		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke2(geOnFind, vGlobalFindData, vFSP);

		return vFSP.GetNumeric().operator long();
	}
	return TRUE;
}

BOOL CV7CursorGridCtrl::OnFindDialog(CV7GlobalFindData* pV7GlobalFindData)
{
	if (EventManager.IsAppointed(geOnFindDialog))
	{
		CValue vGlobalFindData;
		vGlobalFindData.AssignContext(pV7GlobalFindData);
		CValue vFSP(1);

		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke2(geOnFindDialog, vGlobalFindData, vFSP);

		return vFSP.GetNumeric().operator long();
	}
	return TRUE;
}

BOOL CV7CursorGridCtrl::OnClipboard(UINT dwCommandID, CString& str)
{
	if (EventManager.IsAppointed(geOnClipboard))
	{
		CValue vFSP(1);
		CValue vText(str);
		CValue vCommand;
		
		switch(dwCommandID)
		{
		case ID_EDIT_COPY:
			vCommand = 0L;
			break;
		case ID_EDIT_PASTE:
			vCommand = 1;
			break;
		case ID_MEM_SAV:
			vCommand = 2;
			break;
		case ID_MEM_ADD:
			vCommand = 3;
			break;
		case ID_MEM_SUB:
			vCommand = 4;
			break;
		default:
			vCommand = -1;
		}

		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke3(geOnClipboard, vCommand, vText, vFSP);
		str = vText.Format();

		return vFSP.GetNumeric().operator long();
	}
	return TRUE;
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
		CValue vData = (LPCSTR)str;
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

void CV7CursorGridCtrl::GetProvidersFromUtils1sContext (
	Utils1s::Context::CContextBase *pContext,
	CDataProvider *&pProvider, CV7DataProvider *&pV7Provider)
{
	boost::shared_ptr<CDataProvider> pTempProvider;
	if (pContext->QueryInterface (Utils1s::Interfaces::CURSOR_GRID_DATA_PROVIDER,
		&pTempProvider)) {

		ASSERT (pTempProvider);
		pProvider = pTempProvider.get ();
	}

	boost::shared_ptr<CV7DataProvider> pTempV7Provider;
	if (pContext->QueryInterface (Utils1s::Interfaces::CURSOR_GRID_V7_DATA_PROVIDER,
		&pTempV7Provider)) {

		ASSERT (pTempV7Provider);
		pV7Provider = pTempV7Provider.get ();
	}
} // CV7CursorGridCtrl::GetProvidersFromUtils1sContext

CFont* CV7CursorGridCtrl::GetDefaultFont() const
{
	return &m_pDoc->m_Font;
}

BOOL CV7CursorGridCtrl::OnKey(MSG* pMsg)
{
	if (GetEdit())
		return FALSE;

	V7CursorGridCtrlEvents geEvent;
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		geEvent = geOnKeyDown;
		break;
	case WM_KEYUP:
		geEvent = geOnKeyUp;
		break;
	case WM_CHAR:
		geEvent = geOnChar;
		break;
	case WM_SYSKEYDOWN:
		geEvent = geOnSysKeyDown;
		break;
	case WM_SYSKEYUP:
		geEvent = geOnSysKeyUp;
		break;
	default:
		return FALSE;
	}

	if (EventManager.IsAppointed(geEvent))
	{
		CValue vWParam((DWORD)pMsg->wParam);

		CValue vLParam;
		if (pMsg->lParam & 0x80000000)
		{
			//У CNumeric и CValue нет ни оператора ни конструктора от unsigned long
			//Поэтому при присваивании значений >= 0x80000000 получается отрицательное число
			//Поэтому нужен финт
			CNumeric num = (double)pMsg->lParam;
			/*
			DWORD dw = (DWORD)pMsg->lParam;
			CNumeric num = (int)(dw/2);
			num = num * CNumeric(2);
			num = num + CNumeric((int)(dw&1));
			*/
			vLParam = num;
		}
		else
			vLParam = pMsg->lParam;


		CValue vKeyboard;
		vKeyboard.AssignContext(CV7Keyboard::GetV7Keyboard());
		CValue vFSP(1);

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		EventManager.Invoke4(geEvent, vWParam, vLParam, vKeyboard, vFSP);
		if (vFSP.GetNumeric() == 0)
			return TRUE;
/*
		CValue vVirtKey(pMsg->wParam);
		CValue vScanCode(LOBYTE(HIWORD(pMsg->lParam)));
		CValue vRepeat(pMsg->lParam&0x40000000 ? 1 : 0);
		CValue vFSP(1);
		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		EventManager.Invoke4(geOnKeyDown, vVirtKey, vScanCode, vRepeat, vFSP);
		if (vFSP.GetNumeric() == 0)
			return TRUE;
*/
	}
	return FALSE;
}

BOOL CV7CursorGridCtrl::OnShowHint(CDataRow* pDataRow, CGridColumn* pColumn, RegionType rType, CPoint pt, const CString &strHint)
{
	if (EventManager.IsAppointed(geOnShowHint))
	{
		CValue vRegionType;
		CValue vRow, vColumn;
		if (pDataRow)
			GetRowValue(pDataRow, rType, vRow);
		if (pColumn)
			vColumn.AssignContext(static_cast<CV7GridColumn*>(pColumn));
		vRegionType = (long)rType;

		CValue vHint(strHint);
		CValue vFSP(1);
		CValue vX(pt.x), vY(pt.y);

		CExecBatchHelper ExecBatchHelper(m_pDoc, FALSE);
		CUpdateLock UpdateLock = CreateUpdateLock();
		EventManager.Invoke(geOnShowHint, &vHint, &vFSP, &vX, &vY, &vRow, &vColumn, &vRegionType);
		
		//strHint = vHint.GetString();
		//не знаю нужна ли возможность изменять текст подсказки в собитии

		return vFSP.GetNumeric();
	}
	return TRUE;
}

void CV7CursorGridCtrl::DrawBackImage(CDC* pDC)
{
	if (!m_pBackPicture)
		return;

	CPictureHolder7 &pic = m_pBackPicture->m_PictureHolder7;
	if (pic.IsEmpty())
		return;

	CRect rect = GetGridRect(rtBodyNSpace);

	int nStepXCnt = 1;
	int nStepYCnt = 1;
	int nStepXSize = 0;
	int nStepYSize = 0;
	int nRetX = 0;

	if (pic.m_DrawMode == CPictureHolder7::dmOriginalSize)
	{
		CSize sz = pic.GetSize();
		int nOffsetXEx = 0, nOffsetYEx = 0;
		if (m_bBackPicRepeat)
		{
			nStepXCnt = rect.Width() / sz.cx + 2;
			nStepYCnt = rect.Height() / sz.cy + 2;
			nStepXSize = sz.cx;
			nStepYSize = sz.cy;
			nOffsetXEx = sz.cx * (nStepXCnt/2);
			nOffsetYEx = sz.cy * (nStepYCnt/2);
		}

		int nOffsetX, nOffsetY;
		switch( m_BackPicVA )
		{
		case vbaTop:
			nOffsetY = 0;
			break;
		case vbaBotom:
			nOffsetY = rect.Height() - sz.cy * nStepYCnt;
			break;
		default:
			nOffsetY = (rect.Height() - sz.cy) / 2 - nOffsetYEx;
		    break;
		}

		switch( m_BackPicHA )
		{
		case hbaLeft:
			nOffsetX = 0;
			break;
		case hbaRight:
			nOffsetX = rect.Width() - sz.cx * nStepXCnt;
			break;
		default:
			nOffsetX = (rect.Width() - sz.cx) / 2 - nOffsetXEx;
			break;
		}

		nRetX = nStepXSize * nStepXCnt;
	
		rect.OffsetRect(nOffsetX, nOffsetY);
		rect.right = rect.left + sz.cx;
		rect.bottom = rect.top + sz.cy;
	}

	int nPicType = pic.GetType();
	if (nPicType != 1) //ico, ...
	{
		while(nStepYCnt--)
		{
			for(int nX = nStepXCnt; nX; nX--)
			{
				pic.Render(*pDC, rect, 0, NULL);
				rect.OffsetRect(nStepXSize, 0);
			}
			rect.OffsetRect(-nRetX, nStepYSize);
		}
	}
	else //bmp, jpg, gif, ...
	{
		CDC dc2;
		if (!dc2.CreateCompatibleDC(pDC))
			return;
		
		CBitmap bmp;
		if (!bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			return;
		CBitmap* pOldBmp = dc2.SelectObject(&bmp);

		CRect rect2(0, 0, rect.Width(), rect.Height());
		dc2.FillSolidRect(&rect2, GetBackColor());
		pic.Render(dc2, rect2, 0, NULL);

		while(nStepYCnt--)
		{
			for(int nX = nStepXCnt; nX; nX--)
			{
				if (m_crBackPicTransparentColor == CLR_DEFAULT)
					pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc2, 0, 0, SRCCOPY);
				else
					::TransparentBlt(*pDC, rect.left, rect.top, rect.Width(), rect.Height(), dc2, 0, 0, rect.Width(), rect.Height(), m_crBackPicTransparentColor);

				rect.OffsetRect(nStepXSize, 0);
			}
			rect.OffsetRect(-nRetX, nStepYSize);
		}
		
		dc2.SelectObject(pOldBmp);
	}
}

BOOL CV7CursorGridCtrl::DrawPicture(CPictureContext* pPicture, CDC* pDC, CRect& rect)
{
	CSize size = pPicture->m_PictureHolder7.GetSize();
	switch (pPicture->m_PictureHolder7.m_DrawMode)
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
	pPicture->m_PictureHolder7.Render(*pDC, rect, 0, NULL);
	return TRUE;
}

void CV7CursorGridCtrl::SetBackTextBLFont(CBLFont* pFont)
{
	m_pBackTextFont = pFont;
	SetBackTextFont(*m_pBackTextFont);
}


//////////////////////////////////////////////////////////////////////////
// class CV7TableField
//////////////////////////////////////////////////////////////////////////

#undef BL_PROC
#define BL_PROC BL_METH_PROC
#undef BL_FUNC
#define BL_FUNC BL_METH_FUNC

BEGIN_BL_METH_MAP(CV7TableField)
    BL_METH("RefreshRows", "ОбновитьСтроки", 0, &MethRefreshRows, NULL, NULL)
    BL_METH("RedrawRows", "ПерерисоватьСтроки", 0, &MethRedrawRows, NULL, NULL)
    BL_METH("SetTextMargins", "УстановитьОтступы", 4, &MethSetTextMargins, NULL, &defSetTextMargins)
    BL_METH("GetCurrentCellBounds", "ПолучитьГраницыТекущейЯчейки", 5, NULL, &funcGetCurrentCellBounds, NULL)
    BL_PROC("AutoWidth", "АвтоШирина", 0, &procAutoWidth)
	BL_FUNC("EditValue", "РедактироватьЗначение", 8, &funcEditValue)
	BL_FUNC("EndEditValue", "ЗавершитьРедактированиеЗначения", 0, &funcEndEditValue)
	BL_FUNC("CancelEditValue", "ОтменитьРедактированиеЗначения", 0, &funcCancelEditValue)
	BL_METH("SetSysPictures", "УстановитьСистемныеКартинки", 2, &MethSetSysPictures, NULL, &DefSetSysPictures)
	BL_METH("AddSysPictures", "ДобавитьСистемныеКартинки", 1, &MethAddSysPictures, NULL, NULL)
	BL_METH("ResetSysPictures", "СброситьСистемныеКартинки", 0, &MethResetSysPictures, NULL, NULL)
	
	BL_METH("SetBackgroundPicture", "УстановитьФоновуюКартинку", 5, &procSetBackPicture, NULL, &defSetBackPicture)
	BL_PROC("ResetBackgroundPicture", "СброситьФоновуюКартинку", 0, &procResetBackPicture)
	
	BL_METH("SetBackgroundText", "УстановитьФоновыйТекст", 6, &procSetBackText, NULL, &defSetBackText)
	BL_PROC("ResetBackgroundText", "СброситьФоновыйТекст", 0, &procResetBackText)

END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7TableField)
    BL_PROP("Columns", "Колонки", &GetPropColumns, NULL)
    BL_PROP("DataProvider", "ПоставщикДанных", &GetPropProvider, &SetPropProvider)
    BL_PROP("VScroller", "ВертСкроллер", &GetPropVScroller, &SetPropVScroller)
    BL_PROP("StartView", "НачальноеПредставление", &GetPropStartView, &SetPropStartView)
    BL_PROP("RowSelectMode", "РежимВыделенияСтроки", &GetPropRowSelectMode, &SetPropRowSelectMode)
    BL_PROP("SelectMode", "РежимВыделения", &GetPropSelectMode, &SetPropSelectMode)
    BL_PROP("VerticalLines", "ВертикальныеЛинии", &GetPropVerticalLines, &SetPropVerticalLines)
    BL_PROP("HorizontalLines", "ГоризонтальныеЛинии", &GetPropHorizontalLines, &SetPropHorizontalLines)
    BL_PROP("HeadersStyle", "СтильЗаголовков", &GetPropHeadersStyle, &SetPropHeadersStyle)
	BL_PROP("ActiveHeaderStyle", "СтильАктивногоЗаголовка", &GetPropActiveHeaderStyle, &SetPropActiveHeaderStyle)
	BL_PROP("BorderStyle", "СтильРамки", &GetPropBorderStyle, &SetPropBorderStyle)
    BL_PROP("CurrentColumn", "ТекущаяКолонка", &GetPropCurrentColumn, &SetPropCurrentColumn)
    BL_PROP("CurrentData", "ТекущиеДанные", &GetPropCurrentData, NULL)
    BL_PROP("SelectedRows", "ВыделенныеСтроки", &GetPropSelectedRows, NULL)
    BL_PROP("Font", "Шрифт", &GetPropFont, &SetPropFont)
    BL_PROP("RefreshTimeout", "ТаймаутОбновления", &GetPropRefreshTimeout, &SetPropRefreshTimeout)
    BL_PROP("QuickSearchTimeout", "ТаймаутБыстрогоПоиска", &GetPropQuickSearchTimeout, &SetPropQuickSearchTimeout)
    BL_PROP("ChangePositionOfColumns", "ИзменятьПозициюКолонок", &GetPropChangePositionOfColumns, &SetPropChangePositionOfColumns)
	BL_PROP("LinesColor", "ЦветЛиний", &GetPropLinesColor, &SetPropLinesColor)
	BL_PROP("InactiveRowsLinesColor", "ЦветЛинийНеактивныхСтрок", &GetPropInactiveLinesColor, &SetPropInactiveLinesColor)
    BL_PROP("TextColor", "ЦветТекста", &GetPropTextColor, &SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", &GetPropBackColor, &SetPropBackColor)
	BL_PROP("CellBackTransparency", "ПрозрачностьФонаЯчеек", &GetPropBackTransparency, &SetPropBackTransparency)
	BL_PROP("SelectionCellBackTransparency", "ПрозрачностьФонаЯчеекВыделения", &GetPropSelectionBackTransparency, &SetPropSelectionBackTransparency)
	BL_PROP("SelectionCellBackTransparency2", "ПрозрачностьФонаЯчеекВыделения2", &GetPropSelectionBackTransparency2, &SetPropSelectionBackTransparency2)
	BL_PROP("AlternationRowCellBackTransparency", "ПрозрачностьФонаЯчеекЧередованияСтрок", &GetPropAlternationRowBackTransparency, &SetPropAlternationRowBackTransparency)
	BL_PROP("SelectionTextColor", "ЦветТекстаВыделения", &GetPropSelectionTextColor, &SetPropSelectionTextColor)
    BL_PROP("SelectionBackColor", "ЦветФонаВыделения", &GetPropSelectionBackColor, &SetPropSelectionBackColor)
    BL_PROP("SelectionTextColor2", "ЦветТекстаВыделения2", &GetPropSelectionTextColor2, &SetPropSelectionTextColor2)
    BL_PROP("SelectionBackColor2", "ЦветФонаВыделения2", &GetPropSelectionBackColor2, &SetPropSelectionBackColor2)
    BL_PROP("AlternationRowBackColor", "ЦветФонаЧередованияСтрок", &GetPropAlternationRowBackColor, &SetPropAlternationRowBackColor)
    BL_PROP("UseAlternationRowColor", "ЧередованиеЦветовСтрок", &GetPropUseAlternationRowColor, &SetPropUseAlternationRowColor)
    BL_PROP("CurrentRow", "ТекущаяСтрока", &GetPropCurrentRow, &SetPropCurrentRow)
    BL_PROP("РазрешитьНачалоПеретаскивания", "EnableStartDrag", &GetPropEnableStartDrag, &SetPropEnableStartDrag)
    BL_PROP("РазрешитьПеретаскивание", "EnableDrag", &GetPropEnableDrag, &SetPropEnableDrag)
    BL_PROP("ShowHeaders", "ОтображатьЗаголовки", &GetPropShowHeaders, &SetPropShowHeaders)
	BL_PROP("FixedLeft", "ФиксацияСлева", &GetPropFixedLeft, &SetPropFixedLeft)
	BL_PROP("FixedRight", "ФиксацияСправа", &GetPropFixedRight, &SetPropFixedRight)
	BL_PROP("Header", "Шапка", &GetPropFixedRows_head, NULL)
	BL_PROP("Footer", "Подвал", &GetPropFixedRows_tail, NULL)
	BL_PROP("NegativesColor", "ЦветОтрицательных", &GetPropNegativesColor, &SetPropNegativesColor)
	BL_PROP("HeadersTextColor", "ЦветТекстаЗаголовков", &GetPropHeadersTextColor, &SetPropHeadersTextColor)
	BL_PROP("HeadersBackColor", "ЦветФонаЗаголовков", &GetPropHeadersBackColor, &SetPropHeadersBackColor)
	BL_PROP("BorderColor", "ЦветРамки", &GetPropBorderColor, &SetPropBorderColor)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7TableField, "TableBox", "ТабличноеПоле", 1, NULL, NULL, -1);

CV7TableField::CV7TableField()
{
    m_pGrid = NULL;
}

CV7TableField::~CV7TableField()
{

}

//CV7Control
BOOL CV7TableField::CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC)
{
    if (pGetField == NULL || pGetField->GetCtrlInfo()->m_CtrlType != 3)
    {
        Msg("Недопустимый атрибут формы.");
        return FALSE;
    }
	
    CControlID* pControlID = pGetField->GetCtrlInfo();
	
	CGetCtrl* pCtrl = pDoc->GetCtrl(pControlID->m_CtrlID);
	if (pCtrl)
	{
		//Замена таблицы вирт.членов Ctrl кнопки на таблицу вирт.членов Ctrl ТП
		CTableFieldGetCtrl TFCtrl;
		*(DWORD*)pCtrl = *(DWORD*)&TFCtrl;
	}
	pDoc->m_Flag123 = TRUE; //От этого флага зависит включение подменю Поиск и Значение в меню Действия.

	//Добавление акселераторов
	ACCEL arrAcc[] = {
		{FCONTROL|FVIRTKEY, 'F', ID_EDIT_FIND},
		{FCONTROL|FVIRTKEY, VK_INSERT, ID_EDIT_COPY},
		{FSHIFT|FVIRTKEY, VK_INSERT, ID_EDIT_PASTE},
	};
	#define SIZE_OF_ARRAY(arr) sizeof arr / sizeof arr[0]
	HACCEL hAddAcc = ::CreateAcceleratorTable(arrAcc, SIZE_OF_ARRAY(arrAcc));
	
	((CGetDoc7_friend*)pDoc)->GetDefaultAccelerator();
	((CGetDoc7_friend*)pDoc)->m_AccelTable7.Merge(hAddAcc, SIZE_OF_ARRAY(arrAcc));
	
	::DestroyAcceleratorTable(hAddAcc);

	CRect rect;
    CWnd* pControlWnd = pDoc->GetFieldWnd(pGetField);
    pControlWnd->GetWindowRect(&rect);
    pParent->ScreenToClient(rect);

    CV7CursorGridCtrl* pV7CursorGridCtrl = new CV7CursorGridCtrl();
	pV7CursorGridCtrl->PreInit(pDoc);
    BOOL bSuccess = pV7CursorGridCtrl->Create(rect, pParent, pControlID->m_CtrlID, TRUE, pGetField->m_Visible, pGetField->GetReadOnly());
    if (bSuccess)
	{
		bSuccess = pV7CursorGridCtrl->SetWindowPos(pControlWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		bSuccess = pV7CursorGridCtrl->Init(this, pControlID, pUDC);
	}
    pControlWnd->DestroyWindow();
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

BOOL CV7TableField::MethSetSysPictures(CValue** ppValue)
{
	if (ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C || strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext"))
	{
		RuntimeError("Недопустимое значение первого параметра.", 0);
		return FALSE;
	}
	CPictureHolder7& PictureHolder7 = static_cast<CPictureContext*>(ppValue[0]->GetContext())->m_PictureHolder7;
	if (m_pGrid)
	{
		BOOL bSuccess = m_pGrid->GetSysImages().CreateFrom(&PictureHolder7, ppValue[1]->GetNumeric());
		if (bSuccess)
			m_pGrid->SetSysImages(&m_pGrid->GetSysImages());
		return bSuccess;
	}
	return TRUE;
}

BOOL CV7TableField::DefSetSysPictures(int nParam, CValue* pValue)const
{
	pValue->Reset();
	return TRUE;
}

BOOL CV7TableField::MethAddSysPictures(CValue** ppValue)
{
	if (ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C || strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CPictureContext"))
	{
		RuntimeError("Недопустимое значение первого параметра.", 0);
		return FALSE;
	}
	CPictureHolder7& PictureHolder7 = static_cast<CPictureContext*>(ppValue[0]->GetContext())->m_PictureHolder7;
	if (m_pGrid)
	{
		return m_pGrid->GetSysImages().AddFrom(&PictureHolder7);
	}
	return TRUE;
}

BOOL CV7TableField::MethResetSysPictures(CValue** ppValue)
{
	if (m_pGrid)
	{
		m_pGrid->GetSysImages().Reset();
		m_pGrid->SetSysImages(NULL);
	}
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

BOOL CV7TableField::funcEditValue(CValue& RetVal, CValue** ppValues)
{
	if (m_pGrid)
		m_pGrid->EditValue(ppValues);
	return TRUE;
}

BOOL CV7TableField::funcEndEditValue(CValue& RetVal, CValue** ppValues)
{
	if (m_pGrid)
	{
		LONG nResult = m_pGrid->EndEditValue(TRUE) ? 1 : 0;
		RetVal = nResult;
	}
	return TRUE;
}

BOOL CV7TableField::funcCancelEditValue(CValue& RetVal, CValue** ppValues)
{
	if (m_pGrid)
	{
		LONG nResult = m_pGrid->EndEditValue(FALSE) ? 1 : 0;
		RetVal = nResult;
	}
	return TRUE;
}

BOOL CV7TableField::procSetBackPicture(CValue** ppValues)
{
	if (!m_pGrid)
		return FALSE;

	CPictureContext* pPicCont = (CPictureContext*)ppValues[0]->GetContext();
	if ( ppValues[0]->GetTypeCode() == AGREGATE_TYPE_1C && pPicCont && !strcmp(pPicCont->GetRuntimeClass()->m_lpszClassName, "CPictureContext") )
		m_pGrid->SetBackPicture(pPicCont);

	if ( ppValues[1]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackPicHA((HBackAlignment)ppValues[1]->GetNumeric().operator long());

	if ( ppValues[2]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackPicVA((VBackAlignment)ppValues[2]->GetNumeric().operator long());

	if ( ppValues[3]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackPicRepeat(ppValues[3]->GetNumeric());

	if ( ppValues[4]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
	{
		CValue &val = *ppValues[4];
		m_pGrid->SetBackPicTransparentColor(SET_V7_COLOR(val));
	}

	m_pGrid->Invalidate();

	return TRUE;
}
BOOL CV7TableField::defSetBackPicture(int nParam, CValue* pValue)const
{
	pValue->Reset();
	pValue->SetTypeCode(UNSPECIFIED_TYPE_1C);
	return TRUE;
}
BOOL CV7TableField::procResetBackPicture(CValue** ppValues)
{
	if (m_pGrid)
	{
		m_pGrid->SetBackPicture(NULL);
		m_pGrid->SetBackPicHA(hbaCenter);
		m_pGrid->SetBackPicVA(vbaCenter);
	}
	return TRUE;
}

BOOL CV7TableField::procSetBackText(CValue** ppValues)
{
	if (!m_pGrid)
		return FALSE;
	
	if ( ppValues[0]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackText(ppValues[0]->GetString());

	if ( ppValues[1]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
	{
		CValue &val = *ppValues[1];
		CHECK_BLTYPE(val, CBLFont);
		m_pGrid->SetBackTextBLFont(static_cast<CBLFont*>(val.GetContext()));
	}

	if ( ppValues[2]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
	{
		CValue &val = *ppValues[2];
		m_pGrid->SetBackTextColor(SET_V7_COLOR(val));
	}

	if ( ppValues[3]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackTextHA((HBackAlignment)ppValues[3]->GetNumeric().operator long());

	if ( ppValues[4]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackTextVA((VBackAlignment)ppValues[4]->GetNumeric().operator long());

	if ( ppValues[5]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
		m_pGrid->SetBackTextWB(ppValues[5]->GetNumeric());

	return TRUE;
}
BOOL CV7TableField::defSetBackText(int nParam, CValue* pValue)const
{
	pValue->Reset();
	pValue->SetTypeCode(UNSPECIFIED_TYPE_1C);
	return TRUE;
}
BOOL CV7TableField::procResetBackText(CValue** ppValues)
{
	if (m_pGrid)
		m_pGrid->ResetBackText();
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

BOOL CV7TableField::GetPropBorderStyle(CValue& Value)const
{
	if (m_pGrid)
		Value = m_pGrid->GetBorderStyle();
	return TRUE;
}

BOOL CV7TableField::SetPropBorderStyle(CValue const& Value)
{
	if (m_pGrid)
	{
		GET_ENUM_PROP(BorderStyle, bs3D, bsFlat);
		m_pGrid->SetBorderStyle(_enum);
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

BOOL CV7TableField::GetPropInactiveLinesColor(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_COLOR(m_pGrid->GetInactiveGridLinesColor());
	return TRUE;
}

BOOL CV7TableField::SetPropInactiveLinesColor(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetInactiveGridLinesColor(SET_V7_COLOR(Value));
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
        Value = GET_V7_COLOR(m_pGrid->GetGridBackColor());
    return TRUE;
}

BOOL CV7TableField::SetPropBackColor(CValue const& Value)
{
    if (m_pGrid)
        m_pGrid->SetGridBackColor(SET_V7_COLOR(Value));
    return TRUE;
}

BOOL CV7TableField::GetPropBackTransparency(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_TRP(m_pGrid->GetBackTransparency());
	return TRUE;
}

BOOL CV7TableField::SetPropBackTransparency(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetBackTransparency(SET_V7_TRP(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropSelectionBackTransparency(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_TRP(m_pGrid->GetSelectionBackTransparency());
	return TRUE;
}

BOOL CV7TableField::SetPropSelectionBackTransparency(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetSelectionBackTransparency(SET_V7_TRP(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropSelectionBackTransparency2(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_TRP(m_pGrid->GetSelectionBackTransparency2());
	return TRUE;
}

BOOL CV7TableField::SetPropSelectionBackTransparency2(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetSelectionBackTransparency2(SET_V7_TRP(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropAlternationRowBackTransparency(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_TRP(m_pGrid->GetAlternationRowBackTransparency());
	return TRUE;
}

BOOL CV7TableField::SetPropAlternationRowBackTransparency(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetAlternationRowBackTransparency(SET_V7_TRP(Value));
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

BOOL CV7TableField::GetPropNegativesColor(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_COLOR(m_pGrid->GetNegativesColor());
	return TRUE;
}

BOOL CV7TableField::SetPropNegativesColor(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetNegativesColor(SET_V7_COLOR(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropHeadersTextColor(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_COLOR(m_pGrid->GetHeadersTextColor());
	return TRUE;
}

BOOL CV7TableField::SetPropHeadersTextColor(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetHeadersTextColor(SET_V7_COLOR(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropHeadersBackColor(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_COLOR(m_pGrid->GetHeadersBackColor());
	return TRUE;
}

BOOL CV7TableField::SetPropHeadersBackColor(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetHeadersBackColor(SET_V7_COLOR(Value));
	return TRUE;
}

BOOL CV7TableField::GetPropBorderColor(CValue& Value)const
{
	if (m_pGrid)
		Value = GET_V7_COLOR(m_pGrid->GetGridBorderColor());
	return TRUE;
}

BOOL CV7TableField::SetPropBorderColor(CValue const& Value)
{
	if (m_pGrid)
		m_pGrid->SetGridBorderColor(SET_V7_COLOR(Value));
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// class CV7RowData
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CV7RowData, CBLContext)

CV7RowData::CV7RowData() : m_pDataRow(NULL)//, m_pV7DataRow(NULL)
{
}

CV7RowData::CV7RowData(CDataRow* pDataRow, CDataProvider* pDataProvider) : m_pDataRow(pDataRow)
{
	if (pDataProvider)
	{
		CV7DataProvider* pV7Provider = dynamic_cast<CV7DataProvider*>(pDataProvider);
		m_pV7DataRow = pV7Provider->GetV7DataRow(pDataRow);
	}
	else
		m_pV7DataRow = NULL;
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
	BL_METH("SetText", "УстановитьТекст", 1, &MethSetText, NULL, NULL)
	BL_METH("SetPicture", "УстановитьКартинку", 1, &MethSetImage, NULL, NULL)
	BL_METH("SetSysPicture", "УстановитьСистемнуюКартинку", 1, &MethSetSysImage, NULL, NULL)
	BL_METH("SetCheck", "УстановитьФлажок", 1, &MethSetCheckValue, NULL, NULL)
	BL_METH("SetHint", "УстановитьПодсказку", 1, &MethSetHint, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CCellAppearance)
	BL_PROP("Name", "Имя", &GetPropName, NULL)
	BL_PROP("ShowText", "ОтображатьТекст", &GetPropShowText, &SetPropShowText)
	BL_PROP("ShowPicture", "ОтображатьКартинку", &GetPropShowImage, &SetPropShowImage)
	BL_PROP("ShowSysPicture", "ОтображатьСистемнуюКартинку", &GetPropShowSysImage, &SetPropShowSysImage)
	BL_PROP("ShowCheck", "ОтображатьФлажок", &GetPropShowCheckBox, &SetPropShowCheckBox)
	BL_PROP("HorizontalAlignment", "ГоризонтальноеВыравнивание", &GetPropHorizontalAlignment, &SetPropHorizontalAlignment)
	BL_PROP("PictureIndex", "ИндексКартинки", &GetPropImageIndex, &SetPropImageIndex)
	BL_PROP("Pictures", "Картинки", &GetPropImageRefs, NULL)
	BL_PROP("SysPictureIndex", "ИндексСистемнойКартинки", &GetPropSysImageIndex, &SetPropSysImageIndex)
	BL_PROP("SysPictures", "СистемныеКартинки", &GetPropSysImageRefs, NULL)
	BL_PROP("CheckValue", "ЗначениеФлажка", &GetPropCheckValue, &SetPropCheckValue)
	BL_PROP("Text", "Текст", &GetPropText, &SetPropText)
	BL_PROP("TextColor", "ЦветТекста", &GetPropTextColor, &SetPropTextColor)
	BL_PROP("BackColor", "ЦветФона", &GetPropBackColor, &SetPropBackColor)
	BL_PROP("BackTransparency", "ПрозрачностьФона", &GetPropBackTransparency, &SetPropBackTransparency)
	BL_PROP("Font", "Шрифт", &GetPropFont, &SetPropFont)
	BL_PROP("WordBreak", "ПереноситьПоСловам", &GetPropWordBreak, &SetPropWordBreak)
	BL_PROP("ShowHint", "ОтображатьПодсказку", &GetPropShowHint, &SetPropShowHint)
	BL_PROP("Hint", "Подсказка", &GetPropHint, &SetPropHint)
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
		else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
			m_pGridItem->SetImageIndex((LPCSTR)ppValue[0]->GetString());
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

BOOL CCellAppearance::MethSetSysImage(CValue** ppValue)
{
	if (m_pGridItem)
	{
		m_pGridItem->SetShowSysImage();
		if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
			m_pGridItem->SetImageIndex(ppValue[0]->GetNumeric(), TRUE);
		else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
			m_pGridItem->SetImageIndex((LPCSTR)ppValue[0]->GetString(), TRUE);
		else
			RuntimeError("Недопустимое значение первого параметра.", 0);
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

BOOL CCellAppearance::MethSetHint(CValue** ppValue)
{
	if (m_pGridItem)
	{
		m_pGridItem->SetShowHint();
		m_pGridItem->SetHint(ppValue[0]->GetString());
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

BOOL CCellAppearance::GetPropShowSysImage(CValue& Value)const
{
	if (m_pGridItem)
		Value = m_pGridItem->GetShowSysImage();
	return TRUE;
}

BOOL CCellAppearance::SetPropShowSysImage(CValue const& Value)
{
	if (m_pGridItem)
		m_pGridItem->SetShowSysImage(Value.GetNumeric());
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
	{
		int nIndex = m_pGridItem->GetImageIndex(Value.m_String);
		if (nIndex >= -1)
			Value = nIndex;
		else
			Value.SetTypeCode(STRING_TYPE_1C);
	}
	return TRUE;
}

BOOL CCellAppearance::SetPropImageIndex(CValue const& Value)
{
	if (m_pGridItem)
	{
		switch (Value.GetTypeCode())
		{
		case NUMBER_TYPE_1C:
			m_pGridItem->SetImageIndex(Value.GetNumeric());
			break;
		case STRING_TYPE_1C:
			m_pGridItem->SetImageIndex((LPCSTR)Value.GetString());
			break;
		default:
			RuntimeError("Неверный тип аргумента!");
		}
	}
	return TRUE;
}

BOOL CCellAppearance::GetPropImageRefs(CValue& Value)const
{
	if (m_pGridItem)
		Value.AssignContext(static_cast<CV7GridItem::V7IMAGEREFS*>(m_pGridItem->GetImageRefs()));
	return TRUE;
}

BOOL CCellAppearance::GetPropSysImageIndex(CValue& Value)const
{
	if (m_pGridItem)
	{
		int nIndex = m_pGridItem->GetImageIndex(Value.m_String, TRUE);
		if (nIndex >= -1)
			Value = nIndex;
		else
			Value.SetTypeCode(STRING_TYPE_1C);
	}
	return TRUE;
}

BOOL CCellAppearance::SetPropSysImageIndex(CValue const& Value)
{
	if (m_pGridItem)
	{
		switch (Value.GetTypeCode())
		{
		case NUMBER_TYPE_1C:
			m_pGridItem->SetImageIndex(Value.GetNumeric(), TRUE);
			break;
		case STRING_TYPE_1C:
			m_pGridItem->SetImageIndex((LPCSTR)Value.GetString(), TRUE);
			break;
		default:
			RuntimeError("Неверный тип аргумента!");
		}
	}
	return TRUE;
}

BOOL CCellAppearance::GetPropSysImageRefs(CValue& Value)const
{
	if (m_pGridItem)
		Value.AssignContext(static_cast<CV7GridItem::V7IMAGEREFS*>(m_pGridItem->GetImageRefs(TRUE)));
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
        Value = m_pGridItem->GetText();
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

BOOL CCellAppearance::GetPropBackTransparency(CValue& Value)const
{
	if (m_pGridItem)
		Value = GET_V7_TRP(m_pGridItem->GetBackTransparency());
	return TRUE;
}

BOOL CCellAppearance::SetPropBackTransparency(CValue const& Value)
{
	if (m_pGridItem)
		m_pGridItem->SetBackTransparency(SET_V7_TRP(Value));
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

BOOL CCellAppearance::GetPropWordBreak(CValue& Value)const
{
	if (m_pGridItem)
		Value = m_pGridItem->GetWordBreak();
	return TRUE;
}

BOOL CCellAppearance::SetPropWordBreak(CValue const& Value)
{
	if (m_pGridItem)
		m_pGridItem->SetWordBreak(Value.GetNumeric());
	return TRUE;
}

BOOL CCellAppearance::GetPropShowHint(CValue& Value)const
{
	if (m_pGridItem)
		Value = m_pGridItem->GetShowHint();
	return TRUE;
}

BOOL CCellAppearance::SetPropShowHint(CValue const& Value)
{
	if (m_pGridItem)
		m_pGridItem->SetShowHint(Value.GetNumeric());
	return TRUE;
}

BOOL CCellAppearance::GetPropHint(CValue& Value)const
{
	if (m_pGridItem)
		Value = m_pGridItem->GetHint();
	return TRUE;
}

BOOL CCellAppearance::SetPropHint(CValue const& Value)
{
	if (m_pGridItem)
		m_pGridItem->SetHint(Value.GetString());
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
    BL_PROP("RowData", "ДанныеСтроки", &GetPropRowData, NULL)
    BL_PROP("Cells", "Ячейки", &GetPropCells, NULL)
    BL_PROP("TextColor", "ЦветТекста", &GetPropTextColor, &SetPropTextColor)
    BL_PROP("BackColor", "ЦветФона", &GetPropBackColor, &SetPropBackColor)
	BL_PROP("BackTransparency", "ПрозрачностьФона", &GetPropBackTransparency, &SetPropBackTransparency)
    BL_PROP("Font", "Шрифт", &GetPropFont, &SetPropFont)
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

BOOL CV7RowAppearance::GetPropBackTransparency(CValue& Value)const
{
	if (m_pV7GridRow)
		Value = GET_V7_TRP(m_pV7GridRow->GetBackTransparency());
	return TRUE;
}

BOOL CV7RowAppearance::SetPropBackTransparency(CValue const& Value)
{
	if (m_pV7GridRow)
		m_pV7GridRow->SetBackTransparency(SET_V7_TRP(Value));
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
    BL_METH_FUNC("Count",			"Количество",	0,		&funcCount)
    BL_METH_PROC("Clear",			"Очистить",		0,		&procClear)
    BL_METH_FUNC("Get",				"Получить",		1,		&funcGet)
    BL_METH_PROC("Add",				"Добавить",		1,		&procAdd)
    BL_METH_PROC("Delete",			"Удалить",		1,		&procDelete)
    BL_METH_FUNC("Contains",		"Содержит",		1,		&funcContains)
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
 	if (m_pSelectedRows->IsRemoved(pDataRow) || m_pSelectedRows->IsSelected(pDataRow))
//	if (m_pSelectedRows->GetSize() || m_pSelectedRows->IsRemoved(pDataRow))
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

	CDataRowPtr pDataRow(m_pV7DataProvider->BuildRowByValue(*ppValues[0]));
	if (pDataRow)
	{
		m_pSelectedRows->Add(pDataRow);
		m_pGrid->Invalidate();
	}
	return TRUE;
}

BOOL CV7SelectedRows::procDelete(CValue** ppValues)
{
	aux_Check();

	CDataRowPtr pDataRow(m_pV7DataProvider->BuildRowByValue(*ppValues[0]));
	if (pDataRow)
	{
		m_pSelectedRows->Remove(pDataRow);
		m_pGrid->Invalidate();
	}
	return TRUE;
}

BOOL CV7SelectedRows::funcContains(CValue& RetVal, CValue** ppValues)
{
	aux_Check();

	BOOL bSuccess = FALSE;
	CDataRowPtr pDataRow(m_pV7DataProvider->BuildRowByValue(*ppValues[0]));
	if (pDataRow)
	{
		CDataRow* pCurrentDataRow = aux_GetCurrentDataRow();
		if (pCurrentDataRow)
		{
			CDataProvider* pProvider = dynamic_cast<CDataProvider*>(m_pV7DataProvider);
			bSuccess = pProvider->CompareRows(pDataRow.get(), pCurrentDataRow);
		}
		if (!bSuccess)
			bSuccess = m_pSelectedRows->IsSelected(pDataRow.get());
	}

	RetVal = bSuccess ? 1L : 0L;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CV7FixedRows
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7FixedRows)
 	BL_METH_PROC("RefreshRows", "ОбновитьСтроки", 0, &procRefreshRows)
	BL_METH_PROC("RedrawRows", "ПерерисоватьСтроки", 0, &procRedrawRows)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7FixedRows)
	BL_PROP("DataProvider", "ПоставщикДанных", &propGetDataProvider, &propSetDataProvider)
	BL_PROP("TextColor", "ЦветТекста", &propGetTextColor, &propSetTextColor)
	BL_PROP("BackColor", "ЦветФона", &propGetBackColor, &propSetBackColor)
	BL_PROP("BackTransparency", "ПрозрачностьФона", &propGetBackTransparency, &propSetBackTransparency)
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

BOOL CV7FixedRows::propGetBackTransparency(CValue& Value) const
{
	aux_Check();
	Value = GET_V7_TRP(GetBackTransparency());

	return TRUE;
}

BOOL CV7FixedRows::propSetBackTransparency(CValue const& Value)
{
	aux_Check();
	SetBackTransparency(SET_V7_TRP(Value));

	return TRUE;
}

BOOL CV7FixedRows::procRefreshRows( CValue** ppValue )
{
	aux_Check();
	RefreshRows();

	return TRUE;
}

BOOL CV7FixedRows::procRedrawRows( CValue** ppValue )
{
	aux_Check();
	RedrawRows();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CAddedField class
//////////////////////////////////////////////////////////////////////////

CAddedField::CAddedField(CGetDoc7* pDoc, CValue const& vValue, int nLen, int nPrec, LPCSTR szCodeName, DWORD nFlags)
: m_pDoc(pDoc), m_pField(NULL)
{
	CControlList* pControlList = &pDoc->m_ControlList;
	CControlID* pTailControlID = pControlList->GetTail();
	DWORD nControlID = pTailControlID->GetCtrlID() + 1;
	CString csCodeName(szCodeName);

	CControlID* pControlID = new CControlID;
	pControlID->SetCtrlID(nControlID);
	pControlID->SetCodeName(csCodeName);
	pControlID->SetTitle("");
	pControlID->SetCtrlType(5);
	pControlID->SetLongID(0);
	pControlID->SetToolTip("");
	pControlID->SetHelpString("");
	pControlID->SetFormul("");
	pControlID->SetStyle(WS_CHILD);
	pControlID->SetExStyle(0);
	pControlID->SetDependParam(nFlags);
//	pControlID->SetPlace(rect); //??

	// 	LOGFONTA font;
	// 	m_pGrid->m_font.GetLogFont(&font);
	// 	pControlID->SetFont(&font);

	pControlList->AddTail(pControlID);

	CGetFieldsArray* pFieldsArray = &pDoc->m_GetFieldsArray;
	m_pField = pFieldsArray->NewItem();
	m_pField->SetCtrlID(nControlID);
	m_pField->SetDoc(pDoc);
	m_pField->SetInTable(0);
	m_pField->SetCtrlInfo(pControlID);

	((CGetDoc7_friend*)pDoc)->InitStandartFields(m_pField, pControlID);

	CType type = vValue;
	type.m_length = nLen;
	type.m_prec = nPrec;
	m_pField->SetInternalValue(type);

	CValue* pVal = m_pField->GetPValue();
	*pVal = vValue;

	m_pField->TranslateType();
	m_pField->UpdateMask();

	CFormAllCtrlsContext* pFormAll = pDoc->GetFormContext()->m_pForm;
	pFormAll->m_GetFieldsArray.Add(m_pField);
	DWORD dwSize = pFormAll->m_GetFieldsArray.GetSize();
	if (pFormAll->m_Data1)
	{
		CFormCtrlContext** pBuf = new CFormCtrlContext*[dwSize];
		std::copy(pFormAll->m_Data1, pFormAll->m_Data1 + dwSize - 1, pBuf);
		pBuf[dwSize - 1] = NULL;
		delete[] pFormAll->m_Data1;
		pFormAll->m_Data1 = pBuf;
	}

	// 	POSITION aPOS = pDoc->GetFirstViewPosition();
	// 	CFormView* pView = (CFormView*)(pDoc->GetNextView(aPOS));
	CFormView* pView = (CFormView*)pDoc->GetForm();
	pDoc->LayoutControl(pView, m_pField);

	pDoc->InitFieldsState();
	pDoc->CorrectActiveCtrl();

	pDoc->UpdateData(0, pControlID->m_CtrlID);
	//(*m_pField->GetPValue()).Reset();
	//pDoc->UpdateData(1, pControlID->m_CtrlID);

	//CGetExchange exch(pDoc, 1, pControlID->m_CtrlID, NULL);
	//DX_Text(&exch, pControlID->m_CtrlID, *m_pField->GetPValue());

	pDoc->m_GetFieldsArray2.Add(m_pField);
	RegisterProp();
}

CAddedField::~CAddedField()
{
	UnregisterProp();
	CGetFieldsArray* pFieldsArray = &m_pDoc->m_GetFieldsArray;
//	CGetField* pField = pFieldsArray->GetAt(pFieldsArray->GetSize() - 1);
	CWnd* pWnd = m_pDoc->GetFieldWnd(m_pField);
	pWnd->DestroyWindow();

	CFormAllCtrlsContext* pFormAll = m_pDoc->m_pFormContext->m_pForm;
	int nFormAllIdx = pFormAll->m_GetFieldsArray.GetSize() - 1;
	pFormAll->m_GetFieldsArray.RemoveAt(nFormAllIdx);
	CFormCtrlContext*& pFormCtrl = pFormAll->m_Data1[nFormAllIdx];
	if (pFormCtrl)
	{
		pFormCtrl->DecrRef();
		pFormCtrl = NULL;
	}

	m_pDoc->m_GetFieldsArray2.RemoveAt(m_pDoc->m_GetFieldsArray2.GetSize() - 1);

	pFieldsArray->RemoveAt(pFieldsArray->GetSize() - 1);
	delete m_pField;

	CControlList* pControlList = &m_pDoc->m_ControlList;
	pControlList->RemoveTail();

	CDocEditGet* pDocEditGet = static_cast<CDocEditGet*>(m_pDoc->m_CtrlArray.GetAt(m_pDoc->m_CtrlArray.GetSize() - 1));
	m_pDoc->m_CtrlArray.RemoveAt(m_pDoc->m_CtrlArray.GetSize() - 1);
	delete pDocEditGet;
}
// return: TRUE: safe to destroy
// function MUST be called before DTOR
BOOL CAddedField::OnDestroy(BOOL bCheckValue) const
{
	if (bCheckValue)
	{
// 		CString csVal;
// 		GetString(csVal);
// 		return m_pField->CheckLinkValue(csVal, TRUE);

		UINT nID = m_pField->GetCtrlID();
		CGetCtrl* pGetCtrl = m_pDoc->GetCtrl(nID);
		return pGetCtrl->DataExchange(TRUE);
	}
	else
		return TRUE;
}

void CAddedField::GetValue(CValue& RetVal) const
{
	CValue const* pVal = GetPValue();
	pVal ? RetVal = *pVal : RetVal.Reset();
}


void CAddedField::RegisterProp() const
{
	CString csID(GetPropID());
	if (!csID.IsEmpty())
	{
		CFormContext* pForm = m_pDoc->GetFormContext();
		int nFormCtrlIdx = pForm->m_GetFieldsArray.Add(m_pField);
		pForm->m_DWordArray.Add(nFormCtrlIdx);

		int* pBuf = new int[nFormCtrlIdx + 1];
		std::copy(pForm->m_pRefreshFlagsArray, pForm->m_pRefreshFlagsArray + nFormCtrlIdx, pBuf);
		pBuf[nFormCtrlIdx] = 0;
		delete[] pForm->m_pRefreshFlagsArray;
		pForm->m_pRefreshFlagsArray = pBuf;

		CGroupContext* pCont = m_pDoc->GetGroupContext();
		pCont->Accept();
		int nFrom = pCont->FindProp(csID);
		ShiftPCode(m_pDoc->m_pBLModule, nFrom, 1);
	}
}

void CAddedField::UnregisterProp() const
{
	CString csID(GetPropID());
	if (!csID.IsEmpty())
	{
		CGroupContext* pCont = m_pDoc->GetGroupContext();
		int nFrom = pCont->FindProp(csID);
		ShiftPCode(m_pDoc->m_pBLModule, nFrom, -1);

		CFormContext* pForm = m_pDoc->GetFormContext();
		int nFormCtrlIdx = pForm->m_GetFieldsArray.GetSize() - 1;
		pForm->m_GetFieldsArray.RemoveAt(nFormCtrlIdx);
		pForm->m_DWordArray.RemoveAt(nFormCtrlIdx );
		pForm->m_pRefreshFlagsArray[nFormCtrlIdx] = 0;

		pCont->Accept();
	}
}

LPCSTR CAddedField::GetPropID() const
{
	CControlID* pControlID = m_pField->GetCtrlInfo();
	return pControlID->GetCodeName();
}

CValue const* CAddedField::GetPValue() const
{
	CValue const* pVal = NULL;
	CControlID* pControlID = m_pField->GetCtrlInfo();
	CGetExchange exch(m_pDoc, 1, pControlID->m_CtrlID, NULL);
	((CGetDoc7_friend*)m_pDoc)->DX_Text(&exch, pControlID->m_CtrlID, *m_pField->GetPValue());
	//  		pDoc->UpdateData(1, m_pField->m_CtrlID);
	pVal = m_pField->GetPValue();

	return pVal;
}

void CAddedField::GetString(CString& csVal) const
{
	CControlID* pControlID = m_pField->GetCtrlInfo();
	CGetExchange exch(m_pDoc, 1, pControlID->m_CtrlID, NULL);
	((CGetDoc7_friend*)m_pDoc)->DX_Text(&exch, pControlID->m_CtrlID, csVal);
}

void CAddedField::ShiftPCode(CBLModule7* pMod, DWORD nFrom, int nShift)
{
	CCompiledModule* pCompiled = pMod->GetInternalData()->pCompiledModule;
	CDWordArray& PCode = pCompiled->PCode;
	size_t nSize = PCode.GetSize();
	nFrom |= 0x0E000000;
	DWORD dwPrevCode = 0;
	for (int i = 0; i < nSize; ++i)
	{
		DWORD dwCode = PCode[i];
		if (dwPrevCode != 0x02000001 && dwCode >= nFrom && dwCode < 0x0EFFFFFF)
			PCode.SetAt(i,  dwCode + nShift);
		dwPrevCode = dwCode;
	}
};
/*
void CAddedField::ShiftPCode(int nShift) const
{
	CGroupContext* pCont = m_pDoc->GetGroupContext();
	CFormContext* pForm = m_pDoc->GetFormContext();

	DWORD nFrom = 0;
	for(int i = 0; i != pCont->GetSize(); ++i)
	{
		CBLContext* pContAt = pCont->GetAt(i)->m_pContext;
		nFrom += pContAt->GetNProps();
		if (pContAt == pForm)
			break;
	}

	CBLModule7* pMod = m_pDoc->m_pBLModule;
	ShiftPCode(pMod, nFrom - 1, nShift);
}
*/

//////////////////////////////////////////////////////////////////////////
// CV7NativeCellEdit class
//////////////////////////////////////////////////////////////////////////

CV7NativeCellEdit::CV7NativeCellEdit(CV7CursorGridCtrl* pGrid, RegionType rType, CGridColumn* pColumn, size_t nRowIndex, CValue const& vValue, int nLen, int nPrec, LPCSTR szCodeName, DWORD nFlags)
: CV7GridCellEdit(pGrid, rType, pColumn, nRowIndex), CAddedField(pGrid->GetGetDoc7(), vValue, nLen, nPrec, szCodeName, nFlags)
{
	CGetDoc7* pDoc = GetGetDoc7();
	CGetField* pField = GetGetField();

	CWnd* pWnd = pDoc->GetFieldWnd(pField);
	SetWnd(pWnd);

	pWnd->SetParent(GetGrid());

	CRect rect = SuggestWindowRect();
	pWnd->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

	pDoc->SetActiveCtl(pField->m_CtrlID);
}

CV7NativeCellEdit::~CV7NativeCellEdit()
{
	CV7CursorGridCtrl* pGrid = static_cast<CV7CursorGridCtrl*>(GetGrid());
 	CGetDoc7* pDoc = GetGetDoc7();

   	if ((!pDoc->m_bModalMode || !pGrid->GetOnDestroyFired()) && !pGrid->GetOnActivateCtrl())
	{
		pDoc->SetActiveCtl(pGrid->GetDlgCtrlID());
		pGrid->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
// CV7ImageList class
//////////////////////////////////////////////////////////////////////////

BOOL CV7ImageList::RenderBitmap(CBitmap* pBitmap, CPictureHolder7* pPicHolder)
{
	CDC dc;
	CDC DesktopDC;
	DesktopDC.Attach(::GetDC(NULL));
	if (dc.CreateCompatibleDC(&DesktopDC))
	{
		CSize size = pPicHolder->GetSize();
		size.cy = m_nHeight;
		if (pBitmap->CreateCompatibleBitmap(&DesktopDC, size.cx, size.cy))
		{
			CBitmap* pOldbitmap = dc.SelectObject(pBitmap);
			dc.FillSolidRect(0, 0, size.cx, size.cy, COLOR_TRANSPARENT);
			CRect rect(CPoint(0,0), pPicHolder->GetSize());
			pPicHolder->Render(dc, rect, 1, NULL);
			dc.SelectObject(pOldbitmap);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CV7ImageList::CreateFrom(CPictureHolder7* pPicHolder, int nHeight)
{
	m_nHeight = nHeight ? nHeight : pPicHolder->GetSize().cy;

	CBitmap bitmap;
	BOOL bSuccess = RenderBitmap(&bitmap, pPicHolder);

	if (bSuccess)
		if (Create(m_nHeight, m_nHeight, ILC_MASK|ILC_COLOR24, 2, 0))
			bSuccess = Add(&bitmap, COLOR_TRANSPARENT) == 0;
	return bSuccess;
}

BOOL CV7ImageList::AddFrom(CPictureHolder7* pPicHolder)
{
	CBitmap bitmap;
	BOOL bSuccess = RenderBitmap(&bitmap, pPicHolder);
	if (bSuccess)
		bSuccess = Add(&bitmap, COLOR_TRANSPARENT) == 0;
	return bSuccess;
}

void CV7ImageList::Reset()
{
	DeleteImageList();
}
