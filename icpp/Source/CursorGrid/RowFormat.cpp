#include "StdAfx.h"
#include "RowFormat.h"
#include "V7CursorGridCtrl.h"

//////////////////////////////////////////////////////////////////////////
// class CCellFormat
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CCellFormat)
	BL_METH("SetText", "УстановитьТекст", 1, &MethSetText, NULL, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CCellFormat)
	BL_PROP("Name", "Имя", &GetPropName, NULL)
	BL_PROP("ShowText", "ОтображатьТекст", &GetPropShowText, &SetPropShowText)
	BL_PROP("Text", "Текст", &GetPropText, &SetPropText)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CCellFormat, "CellFormat", "ФорматЯчейки", 0, NULL, NULL, -1);

CCellFormat::CCellFormat()
{
}

CCellFormat::~CCellFormat()
{
}

LPCSTR CCellFormat::GetName() const
{
//	if (m_pColumn)
//		return m_pColumn->GetName();
//	else
		return "";
}

BOOL CCellFormat::GetPropName(CValue& Value)const
{
	Value = GetName();
	return TRUE;
}

BOOL CCellFormat::MethSetText(CValue** ppValue)
{
	m_strText = ppValue[0]->GetString();
	m_bShowText = TRUE;
	return TRUE;
}

BOOL CCellFormat::GetPropShowText(CValue& Value)const
{
	Value = m_bShowText;
	return TRUE;
}

BOOL CCellFormat::SetPropShowText(CValue const& Value)
{
	m_bShowText = Value.GetNumeric();
	return TRUE;
}

BOOL CCellFormat::GetPropText(CValue& Value)const
{
	Value = m_strText;
	return TRUE;
}

BOOL CCellFormat::SetPropText(CValue const& Value)
{
	m_strText = Value.GetString();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// class CRowFormat
//////////////////////////////////////////////////////////////////////////
CRowFormat::CRowFormat() : m_pColumns(NULL)
{
}

CRowFormat::~CRowFormat()
{
}

int CRowFormat::GetSize() const
{
	if (m_pColumns)
		return m_pColumns->GetSize();
	else
		return 0;
}

CCellFormat* CRowFormat::GetAt(int nIndex) const
{
	CGridColumn* pGridColumn = m_pColumns->GetAt(nIndex);
	
	if (pGridColumn)
		return static_cast<CV7GridColumn*>(pGridColumn)->m_pCellFormat;
	else
		return NULL;
}

CCellFormat* CRowFormat::GetAt(LPCSTR szName) const
{
	CGridColumn* pGridColumn = m_pColumns->GetAt(szName);
	
	if (pGridColumn)
		return static_cast<CV7GridColumn*>(pGridColumn)->m_pCellFormat;
	else
		return NULL;
}

int CRowFormat::IndexOf(LPCSTR szName) const
{
	return m_pColumns->IndexOf(szName);
}

int CRowFormat::IndexOf(CCellFormat* pItemType) const
{
	return m_pColumns->IndexOf(pItemType->GetName());
}

void CRowFormat::SetColumns(CGridColumns* pColumns)
{
	m_pColumns = pColumns;
};

void CRowFormat::Format(const CDataRowPtr &pDataRow)
{
	if (pDataRow == m_pDataRow)
		return;
	m_pDataRow = pDataRow;

	if (m_pDataRow)
	{
		CV7CursorGridCtrl* pV7Grid = static_cast<CV7CursorGridCtrl*>(m_pColumns->GetGrid());

		for (int i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			if (pColumn->GetVisible())
			{
				const CString& strField = pColumn->GetDataFieldStr();
				if (!strField.IsEmpty())
				{
					int nFldIndex = m_pDataRow->GetFieldIndexOf(strField);
					if (nFldIndex > -1)
					{
						CString& strText = static_cast<CV7GridColumn*>(pColumn)->m_pCellFormat->m_strText;

						if (!pV7Grid->FormatField(GetDataRow(), nFldIndex, pColumn, strText))
						{
							m_pDataRow->FormatField(nFldIndex, strText);
						}
					}
				}
			}
		}
		pV7Grid->OnRowFind(m_pDataRow);
	}
	else
	{
		for (int i = 0; i < m_pColumns->GetSize(); i++)
		{
			CGridColumn* pColumn = m_pColumns->GetAt(i);
			static_cast<CV7GridColumn*>(pColumn)->m_pCellFormat->m_strText.Empty();
		}
	}
}

CDataRow* CRowFormat::GetDataRow()
{
	return m_pDataRow.get();
}



//////////////////////////////////////////////////////////////////////////
// class CV7RowFormat
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CV7RowFormat, CBLContext)
