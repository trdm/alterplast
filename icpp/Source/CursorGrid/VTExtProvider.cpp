
#include "StdAfx.h"
#include "VTExtProvider.h"
#include "../System/compare.h"

//////////////////////////////////////////////////////////////////////////
// class CVTExtDataRow
//////////////////////////////////////////////////////////////////////////

CVTExtDataRow::CVTExtDataRow(CVTExtended* pVT, UINT nRowIndex)
: m_pVT(pVT), m_nRowIndex(nRowIndex), m_vRowIndex(nRowIndex + 1)
{
}

CVTExtDataRow::~CVTExtDataRow()
{
}

//CDataRow
int CVTExtDataRow::GetFieldCount()
{
	return m_pVT->GetColumnsCount() + 1; //+ НомерСтроки
}

int CVTExtDataRow::GetFieldIndexOf(LPCSTR szFieldName)
{
	int nIndex;
	if (CVTExtProvider::IsRowNumberField(szFieldName))
		nIndex = 0;
	else
	{
		nIndex = m_pVT->ColumnNumber(szFieldName, true);
		if (nIndex != -1)
			++nIndex;
	}
	return nIndex;
}

LPCSTR CVTExtDataRow::GetFieldName(int nIndex)
{
	if (nIndex)
		return m_pVT->ColumnName(nIndex - 1);
	else
		return "НомерСтроки";
}

DataType CVTExtDataRow::GetFieldType(int nIndex)
{
 	const CValue& val = GetValue(nIndex);
 	return val.GetTypeCode() == NUMBER_TYPE_1C ? dtNumeric : dtText;
}

void CVTExtDataRow::FormatField(int nIndex, CString& strValue)
{
	if (nIndex)
		if (m_nRowIndex < m_pVT->GetRowsCount())
			FormatValue(m_pVT->GetValue(m_nRowIndex, nIndex - 1), strValue);
		else
			strValue.Empty();
	else
		strValue = m_vRowIndex.Format();
}

UINT CVTExtDataRow::GetRowIndex()
{
	return m_nRowIndex;
}

//CV7DataRow
const CValue& CVTExtDataRow::GetValue(int nIndex) const
{
	if (nIndex)
		return m_pVT->GetValue(m_nRowIndex, nIndex - 1);
	else
		return m_vRowIndex;
}

//////////////////////////////////////////////////////////////////////////
// class CVTExtProvider
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CVTExtProvider)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CVTExtProvider)
	BL_PROP("IndexedTable", "ИндексированнаяТаблица", &GetPropIndexedTable, &SetPropIndexedTable)
	BL_PROP("Index", "Индекс", &GetPropIndex, &SetPropIndex)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CVTExtProvider, "VTExtProvider", "ПоставщикДанныхИТ", 1, NULL, NULL, -1);

CVTExtProvider::CVTExtProvider() : m_pIndex(NULL)
{
}

CVTExtProvider::~CVTExtProvider()
{
}

// BOOL CVTExtProvider::FindRow(CVTExtIterator* pIterator, int nRowIndex)
// {
// 	pIterator->First();
// 	while (pIterator->Next())
// 		if (pIterator->CurrentRowIndex() == nRowIndex)
// 			return TRUE;
// 
// 	return FALSE;
// }

BOOL CVTExtProvider::FindRow(CVTExtIterator* pIterator, int nRowIndex)
{
	if (nRowIndex < 0 || m_pVT->GetRowsCount() <= nRowIndex)
		return FALSE;

	// simple iterator check
	CVTExtRow* pKeyRow = m_pVT->GetRow(nRowIndex);
	if (!m_pIndex->IsIndexed())
	{
		pIterator->SetPosition(NULL, 0, pKeyRow, nRowIndex);
		return TRUE;
	}

	CVTIndexRecord IdxRec;
	IdxRec.IndexFields = &m_pIndex->IndexFields;
	IdxRec.Row = pKeyRow;

	// filter check
	if (m_pIndex->pFilter && !m_pIndex->pFilter->CheckRow(pKeyRow))
		return FALSE;

	CVTExtIndexTreeNode* node = m_pIndex->Search(IdxRec);
	if(node)
	{
		int nFoundRow = node->ID, nFoundIndexInNode = 0;
		pIterator->SetPosition(node, nFoundIndexInNode, m_pVT->GetRow(nFoundRow), nFoundRow);
		if (nFoundRow == nRowIndex)
			return TRUE;
		while (pIterator->Next())
			if (pIterator->CurrentRowIndex() == nRowIndex)
				return TRUE;
	}
	return FALSE;
}

CVTExtRow* CVTExtProvider::GetRow(int nRowIndex)
{
	if (!m_pVT)
		return NULL;

	auto_ptr<CVTExtIterator> pIterator(m_pIndex->CreateIterator());
	CVTExtRow* pRow = FindRow(pIterator.get(), nRowIndex) ? pIterator->CurrentRow() : NULL;
	return pRow;
}

BOOL CVTExtProvider::IsRowNumberField(LPCSTR szFieldName)
{
	return !FastCompareNoCase.Compare(szFieldName, "НомерСтроки") || !FastCompareNoCase.Compare(szFieldName, "RowNumber");
}

void CVTExtProvider::ReleaseIterator()
{
	m_pIterator.reset(NULL);
}

// CBLContext
BOOL CVTExtProvider::GetPropIndexedTable(CValue& Value) const
{
	Value.AssignContext(m_pVT);
	return TRUE;
}

BOOL CVTExtProvider::SetPropIndexedTable(CValue const& Value)
{
	CVTExtended* pVT = NULL;

	if (Value.GetTypeCode() == AGREGATE_TYPE_1C)
		pVT = safe_dynamic_cast<CVTExtended*>(Value.GetContext());

	if (pVT != m_pVT)
	{
		m_pVT = pVT;
		m_pIndex = m_pVT ? m_pVT->DefaultIndex : NULL;
		ReleaseIterator();
		ResetData();
	}

	return TRUE;
}

BOOL CVTExtProvider::GetPropIndex(CValue& Value) const
{
	if (m_pVT)
		Value = m_pIndex->GetName();
	else
		RuntimeError("Индексированная таблица не установлена!");

	return TRUE;
}

BOOL CVTExtProvider::SetPropIndex(CValue const& Value)
{
	if (m_pVT)
	{
		CVTExtIndex* pIndex = m_pVT->GetIndex(const_cast<CValue*>(&Value));
		if (pIndex != m_pIndex)
		{
			m_pIndex = pIndex;
			ReleaseIterator();
			ResetData();
		}
	}
	else
		RuntimeError("Индексированная таблица не установлена!");

	return TRUE;
}

// CDataProvider
void CVTExtProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
	if (!m_pVT)
		return;

	m_nQueryRowsCount = nRowsCount;
	m_pIterator.reset(m_pIndex->CreateIterator());
	if (!pRowFrom || !FindRow(m_pIterator.get(), static_cast<CVTExtDataRow*>(pRowFrom)->m_nRowIndex))
		if (nRowsCount > 0)
			m_pIterator->First();
		else
			m_pIterator->Last();
}

CDataRow* CVTExtProvider::Fetch()
{
	if (!m_pVT || !m_nQueryRowsCount)
		return NULL;

	int nRowIndex = 0;
	if (m_nQueryRowsCount > 0)
	{
		if (m_pIterator->Next())
			nRowIndex = m_pIterator->CurrentRowIndex();
		else
			return NULL;

		--m_nQueryRowsCount;
	}
	else
	{
		if (m_pIterator->Prev())
			nRowIndex = m_pIterator->CurrentRowIndex();
		else
			return NULL;

		++m_nQueryRowsCount;
	}
	return new CVTExtDataRow(m_pVT, nRowIndex);
}

BOOL CVTExtProvider::RefreshRow(CDataRow* pRowFrom)
{
	return GetRow(static_cast<CVTExtDataRow*>(pRowFrom)->m_nRowIndex) != NULL;
}

BOOL CVTExtProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
	return static_cast<CVTExtDataRow*>(pRow1)->m_nRowIndex == static_cast<CVTExtDataRow*>(pRow2)->m_nRowIndex;
}

BOOL CVTExtProvider::GetRowCount(UINT* pCount)
{
	if (!m_pVT || m_pIndex->IsIndexed())
		return FALSE;

	if (pCount)
	{
		if (m_pVT)
			*pCount = m_pVT->GetRowsCount();
		else
			*pCount = 0;
	}
	return TRUE;
}

DataType CVTExtProvider::GetQuickSearchType(LPCSTR szFieldName)
{
	if (m_pVT && (IsRowNumberField(szFieldName) || m_pVT->ColumnNumber(szFieldName, true) != -1))
		return dtText;
	else
		return dtUndefined;
}

void CVTExtProvider::QuickSearch(stQuickSearchPattern& QSPattern)
{
	if (!m_pVT || QSPattern.dataType != dtText)
		return;

	if (IsRowNumberField(QSPattern.szFieldName))
	{
		int nRowIndex = atoi(*QSPattern.pTextData) - 1;
		if (GetRow(nRowIndex))
		{
			CVTExtDataRow VTRow(m_pVT, nRowIndex);
			SetCurrentRow(&VTRow);
			return;
		}
	}
	else
	{
		DWORD nColumnIndex = m_pVT->ColumnNumber(QSPattern.szFieldName);
		for (DWORD nCmpLen = QSPattern.pTextData->GetLength(); nCmpLen > 0; --nCmpLen)
		{
			std::auto_ptr<CVTExtIterator> pIterator(m_pIndex->CreateIterator());
			pIterator->First();
			while (pIterator->Next())
			{
				CVTExtRow* pRow = pIterator->CurrentRow();
				if (!FastCompareNoCase.Compare(QSPattern.pTextData->operator LPCSTR(), pRow->GetValue(nColumnIndex).Format(), nCmpLen))
				{
					CVTExtDataRow VTRow(m_pVT, pIterator->CurrentRowIndex());
					SetCurrentRow(&VTRow);
					QSPattern.pTextData->GetBufferSetLength(nCmpLen);
					return;
				}
			}
		}
	}
	QSPattern.pTextData->Empty();
}

void CVTExtProvider::OnAttach()
{
// 	if (m_pVT && m_pIndex && m_pIndex->GetCurrentRow() != -1)
// 	{
// 		CVTExtDataRow Row(m_pVT, m_pIndex->GetCurrentRow());
// 		ResetData(&Row);
// 	}
// 	else
// 		ResetData();
}

// CV7DataProvider
CV7DataRow* CVTExtProvider::GetV7DataRow(CDataRow* pDataRow)  const
{
	return static_cast<CVTExtDataRow*>(pDataRow);
}

void CVTExtProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
	value = static_cast<CVTExtDataRow*>(pDataRow)->m_vRowIndex;
}

CDataRow* CVTExtProvider::BuildRowByValue(const CValue& value)
{
	int nIndex = (int)value.GetNumeric() - 1;
	if (!m_pVT || !GetRow(nIndex))
		return NULL;

	return new CVTExtDataRow(m_pVT, nIndex);
}
