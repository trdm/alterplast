
#include "StdAfx.h"
#include "V7ODBCProvider.h"
#include "../ODBC/MetaNameParser.h"
#include "../struct.h"

#define MIN_ROWCOUNT 20

extern CDataBase7* pDataBase7;
LPCSTR ExtTypeNames[] =
{
    "MSSQL",
    "VFP",
    "MySQL",
    "Oracle",
};

//////////////////////////////////////////////////////////////////////////
// class CV7SelectListItem
//////////////////////////////////////////////////////////////////////////

BEGIN_BL_METH_MAP(CV7SelectListItem)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7SelectListItem)
    BL_PROP("Name", "Имя", GetPropName, NULL)
    BL_PROP("AutoDelete", "АвтоУдаление", GetPropAutoDelete, SetPropAutoDelete)
    BL_PROP("QuickSearchType", "ТипБыстрогоПоиска", GetPropQuickSearchType, SetPropQuickSearchType)
    BL_PROP("QuickSearchCondition", "УсловиеБыстрогоПоиска", GetPropQuickSearchCondition, SetPropQuickSearchCondition)
    BL_PROP("Typification", "Типизация", GetPropType, SetPropType)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7SelectListItem, "ODBCDataField", "ПолеДанныхODBC", 0, NULL, NULL, -1);

CV7SelectListItem::CV7SelectListItem(): m_rt(-1)
{
    m_AddType = eStandart;
    m_QSType = dtUndefined;
}

CV7SelectListItem::~CV7SelectListItem()
{
}

BOOL CV7SelectListItem::GetPropName(CValue& Value) const
{
    Value = GetName();
    return TRUE;
}

BOOL CV7SelectListItem::GetPropAutoDelete(CValue& Value) const
{
    Value = GetAutoDelete();
    return TRUE;
}

BOOL CV7SelectListItem::SetPropAutoDelete(CValue const& Value)
{
    SetAutoDelete(Value.GetNumeric());
    return TRUE;
}

BOOL CV7SelectListItem::GetPropQuickSearchType(CValue& Value) const
{
    Value = m_QSType;
    return TRUE;
}

BOOL CV7SelectListItem::SetPropQuickSearchType(CValue const& Value)
{
    GET_ENUM_PROP(DataType, dtUndefined, dtText);
    m_QSType = _enum;
    return TRUE;
}

BOOL CV7SelectListItem::GetPropQuickSearchCondition(CValue& Value) const
{
    Value = m_strCondition;
    return TRUE;
}

BOOL CV7SelectListItem::SetPropQuickSearchCondition(CValue const& Value)
{
    m_strCondition = Value.GetString();
    return TRUE;
}

// возврат строки вида 'Справочник.Номенклатура', без скобок и знака $
BOOL CV7SelectListItem::GetPropType(CValue& Value) const
{
	switch (m_rt.GetTypeCode())
	{
	case (WORD)-1:
		if (!m_BLCTypeName.IsEmpty())
		{
			Value = "Класс."+m_BLCTypeName;
			return TRUE;
		}
		break;
	case UNDEFINE_TYPE_1C:
		if (m_AddType == eSbKind)
		{
			Value = "Субконто";
			return TRUE;
		}
	default:
		{
			CString str = static_cast<CType>(m_rt).GetTypeTitle();
			LONG tid = static_cast<CType>(m_rt).GetTypeID();
			if (tid)
			{
				str += ".";
				str += pMetaDataCont->FindObject(tid)->GetCode();
			}
			Value = str;
		}
		break;
	}
    return TRUE;
}

// типизация строкой вида 'Справочник.Номенклатура', без скобок, знака $ и наименования колонки
BOOL CV7SelectListItem::SetPropType(CValue const& Value)
{
	CString strName = Value.GetString();

	if (strName.Find('$') < 0)
		strName.Insert(0, '$');

	if (!CODBCRecordset::ParseV7FieldType(strName, m_rt, m_AddType, m_BLCTypeName))
	{
		RuntimeError("Недопустимая типизация для поля %s.", strName);
		return FALSE;
	}
    return TRUE;
}
 
BOOL CV7SelectListItem::IsTypeEqual(const CType& type) const
{
	if (m_rt.GetTypeCode() != m_rt.GetTypeCode())
		return FALSE;
	if (!m_rt.GetTypeID())
		return TRUE;
	return m_rt.GetTypeID() == type.GetTypeID();
}

//////////////////////////////////////////////////////////////////////////
// class CV7SelectList
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CV7SelectList, CBLContext)

//////////////////////////////////////////////////////////////////////////
// class CV7ODBCDataRow
//////////////////////////////////////////////////////////////////////////

CV7ODBCDataRow::CV7ODBCDataRow(CV7ODBCProvider* pV7ODBCProvider) : CSQLRow(pV7ODBCProvider)
{
    int nCount = CSQLRow::GetFieldCount();
    m_values.SetSize(nCount);
    for (int i = 0; i < nCount; i++)
        m_values[i] = new CValue;
}

CV7ODBCDataRow::~CV7ODBCDataRow()
{
    for (int i = 0; i < m_values.GetSize(); i++)
        delete m_values[i];
}

//CDataRow
DataType CV7ODBCDataRow::GetFieldType(int nIndex)
{
    switch (m_values[nIndex]->GetTypeCode())
    {
    case NUMBER_TYPE_1C:
        return dtNumeric;
    case DATE_TYPE_1C:
        return dtDate;
    }
    return dtText;
}

void CV7ODBCDataRow::FormatField(int nIndex, CString& strValue)
{
    FormatValue(*m_values[nIndex], strValue);
}

//CSQLRow

void CV7ODBCDataRow::SQLFormatField(int nIndex, CString& strValue)
{
    SQLFormatValue(*m_values[nIndex], strValue);
}

BOOL CV7ODBCDataRow::CompareFields(int nIndex, CSQLRow* pRow) const
{
    return *m_values[nIndex] == *static_cast<CV7ODBCDataRow*>(pRow)->m_values[nIndex];
}

//CV7DataRow

CValue const& CV7ODBCDataRow::GetValue(int nIndex) const
{
    return *m_values[nIndex];
}

void CV7ODBCDataRow::SQLFormatValue(const CValue &value, CString& strValue)
{
    bool bIsStrLiteral;

    int nModificator;
    switch (value.GetTypeCode())
    {
    case DATE_TYPE_1C:
        nModificator = 2;
        break;
    default:
        nModificator = 0;
    }

	CString strSQLType;
    CMetaNameParser::CValueToDBValue(value, nModificator, strValue, bIsStrLiteral, strSQLType);
    
    if (value.GetTypeCode() == STRING_TYPE_1C)
        strValue.Replace("'", "''");

    if (bIsStrLiteral)
    {
        strValue.Insert(0, '\'');
        strValue += '\'';
    }
}

//////////////////////////////////////////////////////////////////////////
// class CQueryParams
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CQueryParams, CBLContext)

CQueryParams::CQueryParams() : CValueCollection(TRUE)
{
}

CQueryParams::~CQueryParams()
{
    for (int i = 0; i < m_values.GetCount(); i++)
        delete m_values[i];
}

char const* CQueryParams::GetTypeString(void)const
{
    return "ПараметрыЗапроса";
}

int	CQueryParams::GetNProps(void)const
{
    return m_values.GetCount();
}

int	CQueryParams::FindProp(char const* szName)const
{
    return m_values.GetIndex(szName);
}

char const* CQueryParams::GetPropName(int nPropIndex, int nAlias)const
{
    return m_values[nPropIndex]->GetName();
}

int	CQueryParams::GetPropVal(int nPropIndex, class CValue& Value)const
{
    Value = static_cast<CItem*>(m_values[nPropIndex])->m_Value;
    return TRUE;
}

int	CQueryParams::SetPropVal(int nPropIndex, class CValue const& Value)
{
    static_cast<CItem*>(m_values[nPropIndex])->m_Value = Value;
    return TRUE;
}

CValue* CQueryParams::AddOrGet(LPCSTR szName)
{
    CCollectionItem* pItem = m_values[szName];
    if (!pItem)
    {
        pItem = new CItem;
        pItem->SetName(szName);
        m_values.Add(pItem);
    }
    return &static_cast<CItem*>(pItem)->m_Value;
}

void CQueryParams::Clear()
{
    for (int i = 0; i < m_values.GetCount(); i++)
        delete m_values[i];
    m_values.Clear();
}

//////////////////////////////////////////////////////////////////////////
// class CProviderRS
//////////////////////////////////////////////////////////////////////////

CBLPtr<CMetaDataWork> CProviderRS::m_pMDW;

CProviderRS::CProviderRS(CV7ODBCProvider* pProvider) : m_pProvider(pProvider)
{
    if (!m_pMDW)
        m_pMDW.Create();
    if (pProvider->m_pDatabase)
        SetDatabase(pProvider->m_pDatabase);
    m_nRows = 0;
}

BOOL CProviderRS::TypifyField(int nIndex, CDBField& field)
{
    CV7SelectListItem* pItem = static_cast<CV7SelectListItem*>(m_pProvider->GetSelectList()->GetAt(nIndex));
    field.m_rt = pItem->m_rt;
    field.m_AddType = pItem->m_AddType;
    field.m_BLCTypeName = pItem->m_BLCTypeName;
    return TRUE;
}

BOOL CProviderRS::InternalMapFields() const
{
    return FALSE;
}

int CProviderRS::GetFieldIndex(const char* szFieldName) const
{
    return m_pProvider->GetSelectList()->IndexOf(szFieldName);
}

CBLContext* CProviderRS::GetVCtorParam()
{
    return m_pMDW;
}

void CProviderRS::Init()
{
    Close();
    m_nRows = 0;
    m_params.RemoveAll();
}

void FormatSQLType(SQLSMALLINT nSQLType, SQLSMALLINT nScale, SQLUINTEGER nPrecision, CString& strResult)
{
	switch(nSQLType)
	{
	case SQL_CHAR:           //1
		strResult.Format("char(%d)",nPrecision);
		break;          
	case SQL_NUMERIC:        //2
		strResult.Format("numeric(%d,%d)",nPrecision,nScale);
		break;          
	case SQL_DECIMAL:        //3
		strResult.Format("decimal(%d,%d)",nPrecision,nScale);
		break;          
	case SQL_INTEGER:        //4
		strResult = "int";
		break;          
	case SQL_SMALLINT:       //5
		strResult = "smallint";
		break;          
	case SQL_FLOAT:          //6
		strResult.Format("float(%d)",nPrecision);
		break;          
	case SQL_REAL:           //7
		strResult.Format("real(%d)",nPrecision);
		break;          
	case SQL_DOUBLE:         //8
		strResult = "float(53)";
		break;          
	case SQL_DATETIME:       //9
		strResult = "datetime";
		break;          
	case SQL_VARCHAR:       //12
		strResult.Format("varchar(%d)",nPrecision);
		break;          
	case SQL_TYPE_DATE:     //91
	case SQL_TYPE_TIME:     //92
	case SQL_TYPE_TIMESTAMP://93
		strResult = "datetime";
		break;          
	case SQL_TIMESTAMP:     //11
		strResult = "timestamp";
		break;          
	case SQL_LONGVARCHAR:   //-1
		strResult = "text";
		break;          
	case SQL_BINARY:        //-2
		strResult.Format("binary(%d)",nPrecision);
		break;          
	case SQL_VARBINARY:     //-3
		strResult.Format("varbinary(%d)",nPrecision);
		break;          
	case SQL_LONGVARBINARY: //-4
		strResult = "image";
		break;          
	case SQL_BIGINT:        //-5
		strResult = "bigint";
		break;          
	case SQL_TINYINT:       //-6
		strResult = "tinyint";
		break;          
	case SQL_BIT:           //-7
		strResult = "bit";
		break;          
	case SQL_GUID:          //-11
		strResult = "uniqueidentifier";
		break;          
	case SQL_UNKNOWN_TYPE:  //0
		strResult = "char";
		break;          
	default:
		strResult = "";
	}
}

void CV7ODBCSQLParam::FormatValue(CString& strValue)
{
	switch(m_pValue->GetTypeCode())
	{
	case UNDEFINE_TYPE_1C:
		if (m_nSQLType == SQL_CHAR && m_nPrecision == 23)
			strValue.Format("'U%-22s'", "");
		else if (m_nSQLType == SQL_CHAR)
			strValue = "''";
		break;
	case NUMBER_TYPE_1C:
		if (m_nSQLType == SQL_CHAR && m_nPrecision == 23)
			strValue.Format("'N%22s'", m_pValue->GetString());
		else if (m_nSQLType == SQL_CHAR || m_nSQLType == SQL_VARCHAR)
			strValue.Format("'%s'", m_pValue->GetString());
		else
			strValue = m_pValue->GetString();
		break;
	case STRING_TYPE_1C:
		if (m_nSQLType == SQL_CHAR && m_nPrecision == 23 && m_pValue->GetString().GetLength() == 22)
			strValue.Format("'S%-22s'", m_pValue->GetString());
		else if (m_nSQLType == SQL_CHAR || m_nSQLType == SQL_VARCHAR)
			strValue.Format("'%s'", m_pValue->GetString());
		else
			strValue = m_pValue->GetString();
		break;
	case DATE_TYPE_1C:
		{
			CDate date(m_pValue->GetDate());
			if (m_nSQLType == SQL_CHAR && m_nPrecision == 23)
				strValue.Format("D%-22s", date.Format(df_yyyyMMdd, NULL));
			else
				strValue.Format("{d '%04u-%02u-%02u'}", date.GetYear(), date.GetMonth(), date.GetMonthDay());
		}
		break;
	case ENUM_TYPE_1C:
	case REFERENCE_TYPE_1C:
	case DOCUMENT_TYPE_1C:
	case ACCOUNT_TYPE_1C:
		if (m_nSQLType == SQL_CHAR || m_nSQLType == SQL_VARCHAR)
		{
			if (m_nPrecision == 9)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::ShortString));
			else if (m_nPrecision == 13)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::LongString));
			else if (m_nPrecision == 23)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::VeryLongString));
		}
		break;
	case CALENDAR_TYPE_1C:
	case CALCULATIONKIND_TYPE_1C:
		if (m_nSQLType == SQL_CHAR || m_nSQLType == SQL_VARCHAR)
		{
			if (m_nPrecision == 13)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::LongString));
			else if (m_nPrecision == 23)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::VeryLongString));
		}
		break;
	case SUBCONTOKIND_TYPE_1C:
	case CHART_OF_ACC_TYPE_1C:
		if (m_nSQLType == SQL_CHAR || m_nSQLType == SQL_VARCHAR)
		{
			if (m_nPrecision == 9)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::ShortString));
			else if (m_nPrecision == 23)
				strValue.Format("'%s'", CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::VeryLongString));
		}
		break;
	}
}

void SpecialReplace(CString& str, LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	LPSTR pBuf = str.LockBuffer();
	LPSTR pStart = pBuf;
	LPCTSTR pChFind = lpszOld;
	int nIndex = 0;
	BOOL bIsStrConst = FALSE;
	for (; *pBuf; pBuf++)
	{
		if (*pBuf == '\'')
		{
			bIsStrConst = !bIsStrConst;
			if (bIsStrConst)
			{
				nIndex = 0;
				pChFind = lpszOld;
				continue;
			}
		}
		if (bIsStrConst)
			continue;

		if (*pBuf == *pChFind)
		{
			if (!nIndex)
				nIndex = pBuf - pStart;

			pChFind++;
			if (!*pChFind)
			{
				str.UnlockBuffer();
				str.Delete(nIndex, pChFind - lpszOld);
				str.Insert(nIndex, lpszNew);
				pBuf = str.LockBuffer();
				pStart = pBuf;
				pBuf += nIndex;
				nIndex = 0;
				pChFind = lpszOld;
			}
		}
		else if (nIndex)
		{
			nIndex = 0;
			pChFind = lpszOld;
		}
	}
	str.UnlockBuffer();
}

void CProviderRS::InjectParamValues(CString& strQT, CV7ODBCDataRow* pRowFrom)
{
	if (!m_params.IsEmpty())
	{
		CString strTmp;
		if (m_pProvider->GetQueryStyle() == qsMSSQL)
		{
			CString strExecSqlParamsDeclare;
			CString strExecSqlParamsValues;

			int bFirst = TRUE;
			CString strParamName;
			POSITION pos = m_params.GetStartPosition();
			while(pos)
			{
				CV7ODBCSQLParam* pParam;
				m_params.GetNextAssoc(pos, strParamName, (void*&)pParam);

				if (bFirst)
					bFirst = FALSE;
				else
					strExecSqlParamsDeclare += ',';
				
				strExecSqlParamsDeclare += pParam->m_strLabel;
				strExecSqlParamsDeclare += ' ';

				FormatSQLType(pParam->m_nSQLType, pParam->m_nScale, pParam->m_nPrecision, strTmp);
				strExecSqlParamsDeclare += strTmp;

				if (pParam->m_bIsOK)
				{
					pRowFrom->SQLFormatValue(*pRowFrom->m_values[pParam->m_nFieldIndex], strTmp);
					strExecSqlParamsValues += ',';
					strExecSqlParamsValues += strTmp;
				}
				else
				{
					pParam->FormatValue(strTmp);
					strExecSqlParamsValues += ',';
					strExecSqlParamsValues += strTmp;
				}
			}

			strQT.Replace("'","''");
			strQT.Insert(0,"{call sp_executesql(N'\r\n");
			strQT += "\r\n'";
			strQT += ",N'";
			strQT += strExecSqlParamsDeclare;
			strQT += "'\r\n";
			strQT += strExecSqlParamsValues;
			strQT += ")}";
			//Msg("%s",strExecSqlParamsDeclare);
			//Msg("%s",strExecSqlParamsValues);
		}
		else
		{
			CString strParamName;
			POSITION pos = m_params.GetStartPosition();
			while(pos)
			{
				CV7ODBCSQLParam* pParam;
				m_params.GetNextAssoc(pos, strParamName, (void*&)pParam);
				if (!pParam->m_bIsOK)
				{
					CV7ODBCDataRow::SQLFormatValue(*pParam->m_pValue, strTmp);
					SpecialReplace(strQT, pParam->m_strLabel, strTmp);
				}
			}
		}
	}
	//Msg(strQT.LockBuffer());
	//strQT.UnlockBuffer();
}

BOOL CProviderRS::ExecuteQuery(CV7ODBCDataRow* pRowFrom, int nRowsCount)
{
    if (nRowsCount && m_nRows && nRowsCount != m_nRows)
        Init();

    CString strQT;
    if (!m_pProvider->BuildQuery(strQT, pRowFrom, nRowsCount, m_params))
        return FALSE;

    if (nRowsCount)
        m_nRows = nRowsCount;
    else
        m_nRows = 1;

    InjectParamValues(strQT, pRowFrom);

	if (m_pProvider->m_bDebug)
		Msg("%s", strQT);

	BOOL res = Open(strQT, FALSE, FALSE);

    if (res)
        m_pProvider->m_strLastError.Empty();
    else
    {
        m_pProvider->m_strLastError = GetLastError();
        res = m_pProvider->m_strLastError.IsEmpty();
		if (!res)
			Init();
    }

    return res;
}

//////////////////////////////////////////////////////////////////////////
// class CV7ODBCProvider
//////////////////////////////////////////////////////////////////////////
#undef BL_FUNC
#define BL_FUNC BL_METH_FUNC

BEGIN_BL_METH_MAP(CV7ODBCProvider)
    BL_METH("SetOrderKey", "УстКлючПорядка", 1, MethSetOrderKey, NULL, NULL)
    BL_METH("SetQueryText", "УстТекстЗапроса", 1, MethSetQueryText, NULL, NULL)
    BL_METH("SetTextParam", "УстановитьТекстовыйПараметр", 2, MethSetTextParam, NULL, NULL)
    BL_METH("Refresh", "Обновить", 1, MethRefresh, NULL, MethRefreshDefVal)
    BL_METH("SetIDField", "УстИДПоле", 1, MethSetIDField, NULL, NULL)
    BL_METH("Отладка", "Debug", 1, MethDebug, NULL, NULL)
    BL_FUNC("GetQueryText", "ПолучитьТекстЗапроса", 0, funcGetQueryText)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7ODBCProvider)
    BL_PROP("Fields", "Поля", GetPropFields, NULL)
    BL_PROP("Database", "БазаДанных", GetPropDatabase, SetPropDatabase)
    BL_PROP("Params", "Параметры", GetPropParams, NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7ODBCProvider, "ODBCDataProvider", "ПоставщикДанныхODBC", 0, InitTypes, NULL, -1);

CV7ODBCProvider::CV7ODBCProvider()
{
    m_pMNParser = NULL;
    m_pOKParams = NULL;

    m_pUpRS = NULL;
    m_pDownRS = NULL;
    m_pTopRS = NULL;
    m_pBottomRS = NULL;
    m_pRefreshRS = NULL;
    m_pRowByValueRS = NULL;

    m_IDFieldParam.m_bIsOK = FALSE;
    m_bIDFieldParamDefined = FALSE;

	m_bDebug = FALSE;
}

CV7ODBCProvider::~CV7ODBCProvider()
{
    if (m_pMNParser) delete m_pMNParser;
    if (m_pOKParams) delete[] m_pOKParams;
    
    if (m_pUpRS) delete m_pUpRS;
    if (m_pDownRS) delete m_pDownRS;
    if (m_pTopRS) delete m_pTopRS;
    if (m_pBottomRS) delete m_pBottomRS;
    if (m_pRefreshRS) delete m_pRefreshRS;
    if (m_pRowByValueRS) delete m_pRowByValueRS;
}

//Query rows interface
void CV7ODBCProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
    if (!m_pDatabase)
    {
        m_strLastError = "Не установлена база данных.";
        return;
    }

    int nCount = max(nPageSizeHint, MIN_ROWCOUNT);
    if (nRowsCount < 0)
        nCount = -nCount;

    CProviderRS** ppRS;

    if (pRowFrom)
    {
        if (nCount < 0)
            ppRS = &m_pUpRS;
        else
            ppRS = &m_pDownRS;
    }
    else
    {
        if (nCount < 0)
            ppRS = &m_pBottomRS;
        else
            ppRS = &m_pTopRS;
    }

    if (!*ppRS)
        m_pIteratedRS = *ppRS = new CProviderRS(this);
    else
        m_pIteratedRS = *ppRS;

    BOOL bSuccess = m_pIteratedRS->ExecuteQuery(static_cast<CV7ODBCDataRow*>(pRowFrom), nCount);
    if (m_bRSIsOKDeterminator = bSuccess && !m_pOKParams)
    {
        DefineSQLParams(m_pIteratedRS);
        m_bRSIsOKDeterminator = pRowFrom != NULL;
    }
}

CDataRow* CV7ODBCProvider::Fetch()
{
    if (!m_strLastError.IsEmpty() || !m_pIteratedRS->IsOpen())
        return NULL;

    if (m_pIteratedRS->IsEof())
    {
        if (m_bRSIsOKDeterminator)
            m_pIteratedRS->Init();
        return NULL;
    }

    CV7ODBCDataRow* pV7ODBCDataRow = new CV7ODBCDataRow(this);
    for (int i = 0; i < GetSelectList()->GetSize(); i++)
        m_pIteratedRS->operator[](i).GetValue(*pV7ODBCDataRow->m_values[i]);

    m_pIteratedRS->MoveNext();

    return pV7ODBCDataRow;
}

BOOL CV7ODBCProvider::RefreshRow(CDataRow* pRowFrom)
{
    if (!m_pDatabase)
    {
        m_strLastError = "Не установлена база данных.";
        return FALSE;
    }

    CV7ODBCDataRow* pRow = static_cast<CV7ODBCDataRow*>(pRowFrom);
    
    if (!m_pRefreshRS)
        m_pRefreshRS = new CProviderRS(this);

    if (!m_pRefreshRS->ExecuteQuery(pRow, 0))
        return FALSE;

    BOOL bRSIsOKDeterminator;
    if (bRSIsOKDeterminator = !m_pOKParams)
        DefineSQLParams(m_pRefreshRS);

    BOOL Ret = !m_pRefreshRS->IsEof();
    
    if (Ret)
    {
        int i;
        for (i = 0; i < pRow->m_values.GetSize(); i++)
            delete pRow->m_values[i];

        pRow->m_values.SetSize(GetSelectList()->GetSize());
        for (i = 0; i < GetSelectList()->GetSize(); i++)
        {
            CValue* pValue = new CValue;
            m_pRefreshRS->operator[](i).GetValue(*pValue);
            pRow->m_values[i] = pValue;
        }
        m_pRefreshRS->MoveNext();
    }

    if (bRSIsOKDeterminator)
        m_pRefreshRS->Init();

    return Ret;
}

void CV7ODBCProvider::DefineSQLParams(CProviderRS* pRS)
{
    if (!m_pOKParams)
    {
        m_pOKParams = new CV7ODBCSQLParam[GetOrderKeyFieldCount()];
        for (int i = 0; i < GetOrderKeyFieldCount(); i++)
        {
            CV7ODBCSQLParam& Param = m_pOKParams[i];
            Param.m_bIsOK = TRUE;
            Param.m_nFieldIndex = GetOrderKeyField(i)->GetFieldIndex();
            CDBField& DBField = pRS->operator[](Param.m_nFieldIndex);
            Param.m_nSQLType = DBField.m_nSQLType;
            Param.m_nPrecision = DBField.m_nPrecision;
            Param.m_nScale = DBField.m_nScale;
			Param.m_strLabel.Format("@OKParam_%d", i);
        }
    }
    CSelectListItem* pIDFieldItem = GetIDField();
    if (!m_bIDFieldParamDefined && pIDFieldItem)
    {
        CDBField& DBField = pRS->operator[](pIDFieldItem->GetFieldIndex());
        m_IDFieldParam.m_nSQLType = DBField.m_nSQLType;
        m_IDFieldParam.m_nPrecision = DBField.m_nPrecision;
        m_IDFieldParam.m_nScale = DBField.m_nScale;
		m_IDFieldParam.m_strLabel = "@IDFieldParam";
        m_bIDFieldParamDefined = TRUE;
    }
}

void CV7ODBCProvider::OnAttach()
{
}

//CSQLProvider

//CV7DataProvider
CV7DataRow* CV7ODBCProvider::GetV7DataRow(CDataRow* pDataRow) const
{
    return static_cast<CV7ODBCDataRow*>(pDataRow);
}

void CV7ODBCProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
    CSelectListItem* pIDField = GetIDField();
    if (pIDField)
        value = *static_cast<CV7ODBCDataRow*>(pDataRow)->m_values[pIDField->GetFieldIndex()];
    else
        value.Reset();
}

CDataRow* CV7ODBCProvider::BuildRowByValue(const CValue& value)
{
    BOOL bSuccess;

    CV7ODBCDataRow* pRow = NULL;
    if (value.GetTypeCode() == AGREGATE_TYPE_1C && value.GetContext()->GetRuntimeClass() == RUNTIME_CLASS(CStruct))
    {
        CBLContext* pCont = value.GetContext();
        pRow = new CV7ODBCDataRow(this);
        for (int i = 0; i < GetOrderKeyFieldCount(); i++)
        {
            CSelectListItem* pItem = GetOrderKeyField(i);
            int nBLPropIndex = pCont->FindProp(pItem->GetName());
            if (nBLPropIndex == -1)
            {
                m_strLastError.Format("Не найдено свойство \"%s\"", pItem->GetName());
                delete pRow;
                return NULL;
            }
            pCont->GetPropVal(nBLPropIndex, *pRow->m_values[pItem->GetFieldIndex()]);
        }
        CV7SelectListItem* pIDItem = static_cast<CV7SelectListItem*>(GetIDField());
        if (pIDItem)
            *pRow->m_values[pIDItem->GetFieldIndex()] = value;
    }
    else if (GetIDField())
    {
        CV7SelectListItem* pIDItem = static_cast<CV7SelectListItem*>(GetIDField());
        if (pIDItem->IsTypeEqual(value))
        {
            if (!m_pRowByValueRS)
                m_pRowByValueRS = new CProviderRS(this);
            
            BOOL bInit = FALSE;

            CString strQT;
            if (m_bIDFieldParamDefined)
            {
                bSuccess = BuildByIDQuery(strQT, m_pRowByValueRS->m_params, m_IDFieldParam.m_strLabel, &m_IDFieldParam);
            }
            else
            {
                CString strIDText;
                CV7ODBCDataRow::SQLFormatValue(value, strIDText);
                bSuccess = BuildByIDQuery(strQT, m_pRowByValueRS->m_params, strIDText, NULL);
                bInit = TRUE;
            }
            if (!bSuccess)
                return NULL;

            m_pRowByValueRS->m_nRows = 1;

            if (m_bIDFieldParamDefined)
                m_IDFieldParam.m_pValue = &value;

			m_pRowByValueRS->InjectParamValues(strQT, NULL);

			if (m_bDebug)
				Msg("%s", strQT);

			bSuccess = m_pRowByValueRS->Open(strQT, FALSE, FALSE);

            m_strLastError = m_pRowByValueRS->GetLastError();
            if (!bSuccess || !m_strLastError.IsEmpty())
                return NULL;
            else
            {
                pRow = new CV7ODBCDataRow(this);
                for (int i = 0; i < GetOrderKeyFieldCount(); i++)
				{
					int nIndex = GetOrderKeyField(i)->GetFieldIndex();
					m_pRowByValueRS->operator[](nIndex).GetValue(*pRow->m_values[nIndex]);
				}
                *pRow->m_values[pIDItem->GetFieldIndex()] = value;
            }

            if (bInit)
                m_pRowByValueRS->Init();
        }
    }
    return pRow;
}

//CBLContext
void CV7ODBCProvider::InitTypes()
{
    CString str;
    for (int i = 0; i < qsLastStyle; i++)
    {
        str = m_myCtxBaseInfo->EngTypeString;
        str += '.';
        str += ExtTypeNames[i];
    	CBLContext::RegisterContextClass(RUNTIME_CLASS(CV7ODBCProvider), str, CType(100));
        str = m_myCtxBaseInfo->RusTypeString;
        str += '.';
        str += ExtTypeNames[i];
    	CBLContext::RegisterContextClass(RUNTIME_CLASS(CV7ODBCProvider), str, CType(100));
    }
}

void CV7ODBCProvider::InitObject(char const* szName)
{
    LPSTR szType = strchr(szName, '.');
    szType++;
    for (int i = 0; i < qsLastStyle; i++)
    {
        if (!stricmp(szType, ExtTypeNames[i]))
        {
            Init((QueryStyle)i);
			if (GetMetaData()->GetTaskDef()->GetDefaultLanguage() == 0)
				m_strType = m_myCtxBaseInfo->EngTypeString;
			else
				m_strType = m_myCtxBaseInfo->RusTypeString;
            m_strType += '.';
            m_strType += ExtTypeNames[i];
            break;
        }
    }
}

char const* CV7ODBCProvider::GetTypeString(void)const
{
    return m_strType;
}

BOOL CV7ODBCProvider::MethSetQueryText(CValue** ppValue)
{
    InitRecordsets(TRUE);
    m_bIDFieldParamDefined = FALSE;

    if (!m_pMNParser)
        m_pMNParser = new CMetaNameParser;

    m_pMNParser->SetParseSQLParam(false);
    m_pMNParser->SetQueryText(ppValue[0]->GetString());
    try
    {
        m_pMNParser->Parse();
    }
    catch (CMNPException* pMNPException)
    {
		CString s("Meta name parser: ");
		s += pMNPException->GetErrorDescr();
        pMNPException->Delete();
        RuntimeError(s);
    }

    BOOL bSuccess = SetQuery(m_pMNParser->GetQueryText());
    if (!bSuccess)
        RuntimeError(CSQLProvider::GetLastError());

    delete m_pMNParser;
    m_pMNParser = NULL;

    
    ResetData();
    return bSuccess;
}

BOOL CV7ODBCProvider::MethSetOrderKey(CValue** ppValue)
{
    InitRecordsets(TRUE);
    BOOL bSuccess = SetOrderKey(ppValue[0]->GetString());
    if (!bSuccess)
        CBLModule::RaiseExtRuntimeError(CSQLProvider::GetLastError(), 0);
    return bSuccess;
}

BOOL CV7ODBCProvider::MethSetTextParam(CValue** ppValue)
{
    if (!m_pMNParser)
        m_pMNParser = new CMetaNameParser;
    m_pMNParser->SetParameter(ppValue[0]->GetString(), *ppValue[1]);

    CBLContext* pCont;
    int i;
    pCont = ppValue[1]->GetContext();
    i = ppValue[1]->LinkContext(1);
    pCont = ppValue[1]->GetContext();
    
    i = ppValue[1]->LinkContext(1);
    pCont = ppValue[1]->GetContext();

    return TRUE;
}

BOOL CV7ODBCProvider::MethRefresh(CValue** ppValue)
{
    CDataRow* pDataRow = NULL;
    if (ppValue[0]->GetTypeCode() != UNDEFINE_TYPE_1C)
        pDataRow = BuildRowByValue(*ppValue[0]);
    ResetData(pDataRow);
    return TRUE;
}

BOOL CV7ODBCProvider::MethRefreshDefVal(int nIndex, CValue* pValue) const
{
    pValue->Reset();
    return TRUE;
}

BOOL CV7ODBCProvider::MethSetIDField(CValue** ppValue)
{
    m_bIDFieldParamDefined = FALSE;
    if (m_pRowByValueRS) m_pRowByValueRS->Init();
    BOOL bSuccess = SetIDField(ppValue[0]->GetString());
    if (!bSuccess)
        CBLModule::RaiseExtRuntimeError(CSQLProvider::GetLastError(), 0);
    return bSuccess;
}

BOOL CV7ODBCProvider::MethDebug(CValue** ppValue)
{
	if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C
		&& ppValue[0]->GetNumeric().operator long() == 0)
		m_bDebug = FALSE;
	else
		m_bDebug = TRUE;

    return TRUE;
}

BOOL CV7ODBCProvider::funcGetQueryText(CValue& RetVal, CValue** ppValues)
{
	CString strQT;
	CSQLParamMap params;

	BuildQuery(strQT, NULL, 1, params);
	RetVal = strQT;

	return TRUE;
}

BOOL CV7ODBCProvider::GetPropFields(CValue& Value) const
{
    Value.AssignContext(static_cast<CV7SelectList*>(GetSelectList()));
    return TRUE;
}

BOOL CV7ODBCProvider::GetPropDatabase(CValue& Value) const
{
    if (m_pDatabase)
        Value.AssignContext(m_pDatabase);
    return TRUE;
}

BOOL CV7ODBCProvider::SetPropDatabase(CValue const& Value)
{
    FreeRecordsets();

    CHECK_BLTYPE(Value, C1CPP_ODBCDatabase);
    m_pDatabase = static_cast<C1CPP_ODBCDatabase*>(Value.GetContext());
    
    return TRUE;
}

BOOL CV7ODBCProvider::GetPropParams(CValue& Value) const
{
    Value.AssignContext(const_cast<CQueryParams*>(&m_QueryParams));
    return TRUE;
}

void CV7ODBCProvider::OnFieldsChanged()
{
    InitRecordsets();
    CSQLProvider::OnFieldsChanged();
}

BOOL CV7ODBCProvider::OnAddToSelectList(CSelectListItem* pSelectListItem)
{
    if (*pSelectListItem->GetName() == '[')
    {
        CV7SelectListItem* pItem = static_cast<CV7SelectListItem*>(pSelectListItem);
        CString strName(pItem->GetName());
        strName.TrimLeft('[');
        strName.TrimRight(']');
        
        if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
            if (strName.Replace(TypeNamePrefix, "$"))
                strName.Replace('_', '.');

        if (!CODBCRecordset::ParseV7FieldType(strName, pItem->m_rt, pItem->m_AddType, pItem->m_BLCTypeName))
        {
            m_strLastError.Format("Недопустимая типизация для поля %s.", strName);
            return FALSE;
        }
        pItem->SetName(strName);
    }

    return TRUE;
}

CSQLQueryParam* CV7ODBCProvider::NewSQLQueryParam(const CString& strName, const CStringList& params, CString& strLabel)
{
    SQLSMALLINT nSQLType = 0;
    SQLUINTEGER nPrecision = 0;
    SQLSMALLINT nScale = 0;

    POSITION pos = params.GetHeadPosition();
    if (!pos)
    {
        m_strLastError.Format("Не указан тип параметра \"%s\"", strName);
        return NULL;
    }
    nSQLType = atoi(params.GetNext(pos));
    if (pos)
    {
        nPrecision = atoi(params.GetNext(pos));
        if (pos)
        {
            nScale = atoi(params.GetNext(pos));
            if (pos)
            {
                m_strLastError.Format("Не допустимое задание параметра \"%s\"", strName);
                return NULL;
            }
        }
    }

    switch (nSQLType)
    {
    case 1: nSQLType = SQL_BIT; break;
    case 2: nSQLType = SQL_TINYINT; break;
    case 3: nSQLType = SQL_SMALLINT; break;
    case 4: nSQLType = SQL_INTEGER; break;
    case 5: nSQLType = SQL_REAL; break;
    case 6: nSQLType = SQL_FLOAT; break;
    case 7: nSQLType = SQL_DOUBLE; break;
    case 8: nSQLType = SQL_TYPE_DATE; break;
    case 9: nSQLType = SQL_TYPE_TIME; break;
    case 10: nSQLType = SQL_TYPE_TIMESTAMP; break;
    case 11: nSQLType = SQL_NUMERIC; break;
    case 12: nSQLType = SQL_DECIMAL; break;
    case 13: nSQLType = SQL_BIGINT; break;
    case 14: nSQLType = SQL_CHAR; break;
    case 15: nSQLType = SQL_VARCHAR; break;
    //case 16: nSQLType = SQL_LONGVARCHAR; break;
    case 17: nSQLType = SQL_BINARY; break;
    case 18: nSQLType = SQL_VARBINARY; break;
    //case 19: nSQLType = SQL_LONGVARBINARY; break;
    default:
        m_strLastError.Format("Не допустимый тип параметра \"%s\"", strName);
        return NULL;
    }

    CV7ODBCSQLParam* pParam = new CV7ODBCSQLParam;
    pParam->m_bIsOK = FALSE;
    pParam->m_nSQLType = nSQLType;
    pParam->m_nPrecision = nPrecision;
    pParam->m_nScale = nScale;
    pParam->m_pValue = m_QueryParams.AddOrGet(strName);
	pParam->m_strLabel.Format("@QParam_%s_%d_%d_%d", strName, nSQLType, nPrecision, nScale);
	strLabel = pParam->m_strLabel;
    return pParam;
}

CSQLQueryParam* CV7ODBCProvider::GetOrderKeyParam(int nIndex)
{
    if (m_pOKParams)
        return &m_pOKParams[nIndex];
    else
        return NULL;
}

DataType CV7ODBCProvider::GetQuickSearchType(LPCSTR szFieldName)
{
    CV7SelectListItem* pItem = static_cast<CV7SelectListItem*>(GetQueryField(szFieldName));
    if (pItem)
        return pItem->m_QSType;
    else
        return dtUndefined;
}

void CV7ODBCProvider::QuickSearch(stQuickSearchPattern& QSPattern)
{
    BOOL bSuccess;

    if (QSPattern.dataType != dtText)
    {
        QSPattern.pTextData->Empty();
        return;
    }

    CV7SelectListItem* pItem = static_cast<CV7SelectListItem*>(GetQueryField(QSPattern.szFieldName));
    if (!pItem)
    {
        QSPattern.pTextData->Empty();
        return;
    }

    CProviderRS rs(this);

	CValue val;
	CV7ODBCSQLParam* param = new CV7ODBCSQLParam;
	rs.m_params.Add(param);
	param->m_bIsOK = FALSE;
	param->m_nSQLType = SQL_VARCHAR;
    param->m_nPrecision = 100;
    param->m_nScale = 0;
	param->m_strLabel = "@QSParam";
	param->m_pValue = &val;

    CString strQT;
    if (!BuildQuickSearchQuery(strQT, pItem, rs.m_params, pItem->m_strCondition, param->m_strLabel))
    {
        QSPattern.pTextData->Empty();
        return;
    }

    bSuccess = rs.Prepare(strQT);
    
    if (!bSuccess)
    {
        QSPattern.pTextData->Empty();
        return;
    }

    int nBegin = 0, nCount = QSPattern.pTextData->GetLength(), nEnd;
    nEnd = nCount;
    bSuccess;
    while (nBegin != nEnd && nBegin + nCount)
    {
        CString strLike = QSPattern.pTextData->Left(nBegin + nCount);
        strLike += '%';
		val = strLike;
		
		CString strQTCopy(strQT);
		rs.InjectParamValues(strQTCopy, NULL);

		if (m_bDebug)
			Msg("%s", strQTCopy);

		bSuccess = rs.Open(strQTCopy, FALSE, FALSE);

        LPCSTR err = rs.GetLastError();
        if (err[0])
        {
            m_strLastError = err;
            break;
        }

        if (bSuccess)
        {
            nBegin += nCount;
            nCount = nEnd - nBegin;
            if (nCount == 1)
                break;
        }
        else
            nEnd = nBegin + nCount;

        nCount /= 2;
    }
    
    *QSPattern.pTextData = QSPattern.pTextData->Left(nBegin);
    if (bSuccess)
    {
        CV7ODBCDataRow Row(this);
        for (int i = 0; i < GetOrderKeyFieldCount(); i++)
		{
			int nIndex = GetOrderKeyField(i)->GetFieldIndex();
            rs[nIndex].GetValue(*Row.m_values[nIndex]);
		}

		SetCurrentRow(&Row);
    }
	
	delete param;
    rs.Close();
}

void CV7ODBCProvider::InitRecordsets(BOOL bInitOrderKey /*= FALSE*/)
{
    if (m_pUpRS) m_pUpRS->Init();
    if (m_pDownRS) m_pDownRS->Init();
    if (m_pTopRS) m_pTopRS->Init();
    if (m_pBottomRS) m_pBottomRS->Init();
    if (m_pRefreshRS) m_pRefreshRS->Init();
    if (m_pRowByValueRS) m_pRowByValueRS->Init();
    
    if (bInitOrderKey && m_pOKParams)
    {
        delete[] m_pOKParams;
        m_pOKParams = NULL;
    }
}

void CV7ODBCProvider::FreeRecordsets()
{
    delete m_pUpRS;
    m_pUpRS = NULL;
    delete m_pDownRS;
    m_pDownRS = NULL;
    delete m_pTopRS;
    m_pTopRS = NULL;
    delete m_pBottomRS;
    m_pBottomRS = NULL;
    delete m_pRefreshRS;
    m_pRefreshRS = NULL;
    delete m_pRowByValueRS;
    m_pRowByValueRS = NULL;
}
