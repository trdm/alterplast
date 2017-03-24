
#include "StdAfx.h"
#include "VTProvider.h"

//////////////////////////////////////////////////////////////////////////
// class CVTRow
//////////////////////////////////////////////////////////////////////////

CVTRow::CVTRow(CValueTable* pVT, UINT nRowIndex): m_pVT(pVT), m_nRowIndex(nRowIndex)
{
    m_vRowIndex = nRowIndex + 1;
}

CVTRow::~CVTRow()
{
}

//CDataRow
int CVTRow::GetFieldCount()
{
    return m_pVT->GetColumnCount() + 1;
}

int CVTRow::GetFieldIndexOf(LPCSTR szFieldName)
{
    if (!stricmp(szFieldName, "НомерСтроки"))
        return 0;
    else
    {
        int nIndex = m_pVT->GetColumnIdx(szFieldName);
        if (nIndex != -1)
            nIndex++;
        return nIndex;
    }
}

LPCSTR CVTRow::GetFieldName(int nIndex)
{
    if (nIndex)
        return m_pVT->GetColumnCode(nIndex - 1);
    else
        return "НомерСтроки";
}

DataType CVTRow::GetFieldType(int nIndex)
{
    return dtText;
}

void CVTRow::FormatField(int nIndex, CString& strValue)
{
    if (nIndex)
        FormatValue(m_pVT->GetValue(nIndex - 1, m_nRowIndex), strValue);
    else
        strValue = m_vRowIndex.Format();
}

UINT CVTRow::GetRowIndex()
{
    return m_nRowIndex;
}

//CV7DataRow
const CValue& CVTRow::GetValue(int nIndex) const
{
    if (nIndex)
        return m_pVT->GetValue(nIndex - 1, m_nRowIndex);
    else
        return m_vRowIndex;
}

//////////////////////////////////////////////////////////////////////////
// class CVTProvider
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CVTProvider)
    BL_METH("SetValueTable", "УстТаблицуЗначений", 1, MethSetValueTable, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CVTProvider)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CVTProvider, "VTProvider", "ПоставщикДанныхТЗ", 1, NULL, NULL, -1);

CVTProvider::CVTProvider()
{
    m_pVTContextData = NULL;
    m_nCurrentRow = ULONG_MAX;
}

CVTProvider::~CVTProvider()
{
}

BOOL CVTProvider::MethSetValueTable(CValue** ppValue)
{
    m_pBLVT = NULL;
    m_pVTContextData = NULL;

    if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
    {
        CBLContext* pBLContext = ppValue[0]->GetContext();
        if (pBLContext->GetInternalData()->GetRuntimeClass() == RUNTIME_CLASS(CValueTableContextData))
        {
            m_pBLVT = pBLContext;
            m_pVTContextData = static_cast<CValueTableContextData*>(pBLContext->GetInternalData());
        }
    }
    ResetData();
    return TRUE;
}

//CDataProvider
void CVTProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
    m_nQueryRowsCount = nRowsCount;
    if (pRowFrom)
        m_nQueryRowIndex = static_cast<CVTRow*>(pRowFrom)->m_nRowIndex;
    else
        m_nQueryRowIndex = ULONG_MAX;
}

CDataRow* CVTProvider::Fetch()
{
    if (m_nQueryRowsCount == 0 || !m_pVTContextData)
        return NULL;

    CValueTable* pVT = m_pVTContextData->GetValueTable();
    if (!pVT->GetRowCount())
        return NULL;

    if (m_nQueryRowsCount > 0)
    {
        if (m_nQueryRowIndex == ULONG_MAX)
            m_nQueryRowIndex = 0;
        else if (m_nQueryRowIndex < pVT->GetRowCount() - 1)
            m_nQueryRowIndex++;
        else
            return NULL;

        m_nQueryRowsCount--;
    }
    else
    {
        if (m_nQueryRowIndex == ULONG_MAX)
            m_nQueryRowIndex = pVT->GetRowCount() - 1;
        else if (m_nQueryRowIndex > 0)
            m_nQueryRowIndex--;
        else
            return NULL;

        m_nQueryRowsCount++;
    }

    return new CVTRow(pVT, m_nQueryRowIndex);
}

BOOL CVTProvider::RefreshRow(CDataRow* pRowFrom)
{
    return static_cast<CVTRow*>(pRowFrom)->m_nRowIndex < m_pVTContextData->GetValueTable()->GetRowCount();
}

BOOL CVTProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
    return static_cast<CVTRow*>(pRow1)->m_nRowIndex == static_cast<CVTRow*>(pRow2)->m_nRowIndex;
}

BOOL CVTProvider::GetRowCount(UINT* pCount)
{
    if (pCount)
    {
        if (m_pVTContextData)
            *pCount = m_pVTContextData->GetValueTable()->GetRowCount();
        else
            *pCount = 0;
    }
    return TRUE;
}

DataType CVTProvider::GetQuickSearchType(LPCSTR szFieldName)
{
    if (!m_pVTContextData)
		return dtUndefined;
	if (*szFieldName == 0)
		return dtUndefined;

	CValueTable* pVT = m_pVTContextData->GetValueTable();
	DWORD nColumnIndex = pVT->GetColumnIdx(szFieldName);
	if (nColumnIndex != -1)
	{
		CVTColumn* pVTColumn = pVT->GetColumn(nColumnIndex);
		switch(pVTColumn->GetType().GetTypeCode())
		{
		case NUMBER_TYPE_1C:
			return dtNumeric;
		case DATE_TYPE_1C:
			return dtDate;
		default:
			return dtText;
		}
	}
	return dtUndefined;
}

void CVTProvider::QuickSearch(stQuickSearchPattern& QSPattern)
{
    if (QSPattern.dataType != dtText)
        return;

	CValueTable* pVT = m_pVTContextData->GetValueTable();
	DWORD nColumnIndex = pVT->GetColumnIdx(QSPattern.szFieldName);
	for (DWORD nCmpLen = QSPattern.pTextData->GetLength(); nCmpLen>0; nCmpLen--)
	{
		for (DWORD nRowIndex = 0; nRowIndex < pVT->GetRowCount(); nRowIndex++)
		{
			const CValue &val = pVT->GetValue(nColumnIndex, nRowIndex);
			if(strnicmp(val.Format(),*QSPattern.pTextData, nCmpLen) == 0)
			{
				CVTRow VTRow(pVT,nRowIndex);
				SetCurrentRow(&VTRow);
				QSPattern.pTextData->GetBufferSetLength(nCmpLen);
				return;
			}
		}
	}
	QSPattern.pTextData->Empty();
}

void CVTProvider::OnAttach()
{
    if (m_nCurrentRow != ULONG_MAX && m_pVTContextData)
    {
        CValueTable* pVT = m_pVTContextData->GetValueTable();
        CVTRow Row(pVT, m_nCurrentRow);
        ResetData(&Row);
    }
    else
        ResetData();
}

//CV7DataProvider
CV7DataRow* CVTProvider::GetV7DataRow(CDataRow* pDataRow)  const
{
    return static_cast<CVTRow*>(pDataRow);
}

void CVTProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
    value = static_cast<CVTRow*>(pDataRow)->m_vRowIndex;
}

CDataRow* CVTProvider::BuildRowByValue(const CValue& value)
{
    if (!m_pBLVT)
        return NULL;
    CValueTable* pVT = m_pVTContextData->GetValueTable();
    int nIndex = value.GetNumeric();
    if (nIndex <= 0 || nIndex > pVT->GetRowCount())
        return NULL;
    return new CVTRow(pVT, nIndex - 1);
}
