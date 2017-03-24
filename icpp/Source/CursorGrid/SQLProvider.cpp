/////////////////////////////////////////////////////////////////////
// SQLProvider.cpp : implementation of helper class for data providers
// used for CursorGridCtrl
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Copyright (c) 2005-2008. All Rights Reserved.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLProvider.h"
#include "../odbc/MetaNameParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CSelectListItem
//////////////////////////////////////////////////////////////////////

CSelectListItem::CSelectListItem()
{
    m_nFlags = AUTO_DELETE;
}

CSelectListItem::~CSelectListItem()
{
}

BOOL CSelectListItem::GetAutoDelete() const
{
    return m_nFlags & AUTO_DELETE;
}

void CSelectListItem::SetAutoDelete(BOOL bAutoDelete /*= TRUE*/)
{
    if (m_nFlags & (IS_ORDERKEY | IS_IDFIELD))
        return;

    if (bAutoDelete)
        m_nFlags |= AUTO_DELETE;
    else
        m_nFlags &= ~AUTO_DELETE;
}

LPCSTR CSelectListItem::GetExpression() const
{
    return m_strExpression;
}

BOOL CSelectListItem::IsOrderKey() const
{
    return m_nFlags & IS_ORDERKEY;
}

BOOL CSelectListItem::GetBackOrder() const
{
    return m_nFlags & BACK_ORDER;
}

int CSelectListItem::GetFieldIndex() const
{
    return m_nFieldIndex;
}

//////////////////////////////////////////////////////////////////////
// class CSelectList
//////////////////////////////////////////////////////////////////////

CSelectList::CSelectList()
{
}

CSelectList::~CSelectList()
{
    Clear();
}

int CSelectList::GetSize() const
{
    if (!m_pProvider) return 0;

    return m_pProvider->m_fields.GetSize();
}

CSelectListItem* CSelectList::Add(LPCSTR szFieldName)
{
    if (!m_pProvider) return NULL;

    CSelectListItem* pSelectListItem = m_pProvider->IncludeToFields(szFieldName);
    if (pSelectListItem)
        m_pProvider->OnFieldsChanged();
    return pSelectListItem;
}

CSelectListItem* CSelectList::Insert(int nIndex, LPCSTR szFieldName)
{
    if (!m_pProvider) return NULL;

    if (nIndex < 0 || nIndex > m_pProvider->m_fields.GetSize())
        return NULL;
    
    CSelectListItem* pSelectListItem = static_cast<CSelectListItem*>(GetItem(szFieldName));
    if (pSelectListItem)
        if (!(pSelectListItem->m_nFlags & IS_FIELD))
        {
            pSelectListItem->m_nFlags |= IS_FIELD;
            m_pProvider->m_fields.InsertAt(nIndex, pSelectListItem);
            pSelectListItem->m_nFieldIndex = nIndex;
            for (int i = nIndex + 1; i < m_pProvider->m_fields.GetSize(); i++)
                m_pProvider->m_fields[i]->m_nFieldIndex = i;
            m_pProvider->OnFieldsChanged();
        }
    return pSelectListItem;
}

CSelectListItem* CSelectList::GetAt(int nIndex) const
{
    if (!m_pProvider) return NULL;

    if (nIndex < 0 || nIndex > m_pProvider->m_fields.GetUpperBound())
        return NULL;
    return m_pProvider->m_fields[nIndex];
}

CSelectListItem* CSelectList::GetAt(LPCSTR szFieldName) const
{
    if (!m_pProvider) return NULL;

    CCollectionItem* pCollectionItem = GetItem(szFieldName);
    if (!pCollectionItem)
        return NULL;
    
    CSelectListItem* pSelectListItem = static_cast<CSelectListItem*>(pCollectionItem);
    if (!(pSelectListItem->m_nFlags & IS_FIELD))
        pSelectListItem = NULL;
    return pSelectListItem;
}

int CSelectList::IndexOf(LPCSTR szFieldName) const
{
    if (!m_pProvider) return -1;

    int nIndex = GetIndex(szFieldName);
    if (nIndex >= 0)
    {
        CSelectListItem* pSelectListItem = static_cast<CSelectListItem*>(GetItem(nIndex));
        if (pSelectListItem->m_nFlags & IS_FIELD)
            return pSelectListItem->m_nFieldIndex;
    }
    return -1;
}

int CSelectList::IndexOf(CSelectListItem* pSelectListItem) const
{
    if (!m_pProvider) return -1;

    if (pSelectListItem->m_nFlags & IS_FIELD)
        return pSelectListItem->m_nFieldIndex;
    else
        return -1;
}

BOOL CSelectList::Remove(int nIndex)
{
    if (!m_pProvider) return FALSE;

    if (nIndex < 0 || nIndex > m_pProvider->m_fields.GetUpperBound())
        return FALSE;

    CSelectListItem* pSelectListItem = m_pProvider->m_fields[nIndex];
    if (pSelectListItem->m_nFlags & (IS_ORDERKEY | IS_IDFIELD))
        return FALSE;

    pSelectListItem->m_nFlags &= ~IS_FIELD;
    m_pProvider->m_fields.RemoveAt(nIndex);
    for (int i = nIndex; i < m_pProvider->m_fields.GetSize(); i++)
        m_pProvider->m_fields[i]->m_nFieldIndex = i;
    m_pProvider->OnFieldsChanged();
    return TRUE;
}

void CSelectList::RemoveAll()
{
    if (!m_pProvider) return;

    for (int i = 0; i < m_pProvider->m_fields.GetSize();)
    {
        CSelectListItem* pSelectListItem = m_pProvider->m_fields[i];
        if (!(pSelectListItem->m_nFlags & (IS_ORDERKEY | IS_IDFIELD)))
        {
            m_pProvider->m_fields.RemoveAt(pSelectListItem->m_nFieldIndex);
            pSelectListItem->m_nFlags &= ~IS_FIELD;
        }
        else
            i++;
    }
    m_pProvider->OnFieldsChanged();
}

void CSelectList::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
	CSelectListItem* pSelectListItem = static_cast<CSelectListItem*>(pItem);
	m_pProvider->ClearParams(pSelectListItem->m_params);
	pSelectListItem->Release();
}

//////////////////////////////////////////////////////////////////////
// class CSQLRow
//////////////////////////////////////////////////////////////////////

CSQLRow::CSQLRow(CSQLProvider* pSQLProvider) : m_pSQLProvider(pSQLProvider)
{
}

CSQLRow::~CSQLRow()
{
}

int CSQLRow::GetFieldCount()
{
    return m_pSQLProvider->m_fields.GetSize();
}

int CSQLRow::GetFieldIndexOf(LPCSTR szFieldName)
{
    CSelectListItem* pSelectListItem = m_pSQLProvider->m_pSelectList->GetAt(szFieldName);
    if (pSelectListItem)
        if (pSelectListItem->m_nFlags & IS_FIELD)
            return pSelectListItem->m_nFieldIndex;
    return -1;
}

LPCSTR CSQLRow::GetFieldName(int nIndex)
{
    return m_pSQLProvider->m_fields[nIndex]->GetName();
}

//////////////////////////////////////////////////////////////////////
// class CSQLProvider
//////////////////////////////////////////////////////////////////////

CSQLProvider::CSQLProvider()
{
    m_pIDField = NULL;
}

CSQLProvider::~CSQLProvider()
{
    ClearQuery();
    m_pSelectList->m_pProvider = NULL;
    m_pSelectList->Release();
}

void CSQLProvider::Init(QueryStyle queryStyle)
{
    m_QueryStyle = queryStyle;
    m_pSelectList = NewSelectList();
    m_pSelectList->m_pProvider = this;
}

BOOL CSQLProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
    if (m_pIDField)
    {
        if (!static_cast<CSQLRow*>(pRow1)->CompareFields(m_pIDField->m_nFieldIndex, static_cast<CSQLRow*>(pRow2)))
            return FALSE;
    }
    else
    {
        for (int i = 0; i < m_orderkey.GetSize(); i++)
        {
            CSelectListItem* pSelectListItem = m_orderkey[i];
            if (!static_cast<CSQLRow*>(pRow1)->CompareFields(pSelectListItem->m_nFieldIndex, static_cast<CSQLRow*>(pRow2)))
                return FALSE;
        }
    }
    return TRUE;
}

#define IsWordChar(ch) (::IsCharAlphaNumeric(ch) || ch == '_')

BOOL CSQLProvider::RemoveComments(CString& strQT)
{
    LPCSTR pQT = strQT;
    char ch;
    bool fStrConst = false;
    LPCSTR szComment1 = NULL, szComment2 = NULL;
    while (ch = *pQT++)
    {
        if (fStrConst && ch == '\'')
        {
            fStrConst = false;
            continue;
        }
        if (szComment1)
        {
            if (ch == '\n' || *pQT == 0)
            {
                int CurPos = szComment1 - strQT;
                strQT.Delete(CurPos, pQT - szComment1);
                pQT = strQT;
                pQT += CurPos;
                szComment1 = NULL;
            }
            continue;
        }
        if (szComment2)
        {
            if (ch == '*' && *pQT == '/')
            {
                int CurPos = szComment2 - strQT;
                strQT.Delete(CurPos, pQT - szComment2 + 2);
                pQT = strQT;
                pQT += CurPos;
                szComment2 = NULL;
            }
            continue;
        }

        if (ch == '-' && *pQT == '-')
            szComment1 = pQT - 1;
        else if (ch == '/' && *pQT == '*')
            szComment2 = pQT - 1;
        else if (ch == '\'')
            fStrConst = true;
    }

    if (fStrConst)
    {
        m_strLastError = "Не завершена строковая константа.";
        return FALSE;
    }
    if (szComment2)
    {
        m_strLastError = "Не завершен комментарий.";
        return FALSE;
    }
    return TRUE;
}

LPCSTR szTrimmedSimbols = " \n\r\t";

BOOL CSQLProvider::ParseQuery(CString& strQueryText)
{
    ClearFields();
    m_orderkey.RemoveAll();
    m_strFrom.Empty();
    m_strWhere.Empty();

    enum ParseStage {psSelect, psFrom, psWhere, psOther} ps = psSelect;
    LPCSTR pQT = strQueryText;
    char ch;
    LPCSTR szKeyword = "select";
    LPCSTR szCurKW = szKeyword;
    bool fWhiteSpace = true, fStrConst = false;
    int nBrLevelR = 0, nBrLevelSQ = 0;
    LPCSTR szStart;

    while (ch = *pQT++)
    {
        if (fStrConst)
        {
            if (ch == '\'')
                fStrConst = false;
            continue;
        }

        if (IsWordChar(ch))
        {
            if (fWhiteSpace && !nBrLevelR && !nBrLevelSQ)
            {
                if ((ch | 0x20) == *szCurKW)
                    szCurKW++;
                else
                {
                    szCurKW = szKeyword;
                    fWhiteSpace = false;
                }
            }
        }
        else
        {
            if (!*szCurKW)
            {
                switch (ps)
                {
                case psSelect:
                    ps = psFrom;
                    szKeyword = "from";
                    szStart = pQT - 1;
                    break;
                case psFrom:
					{
						szKeyword = "where";
						ps = psWhere;
						CString cs(szStart, pQT - szStart - 5);
						if (!ParseSelectListItem(cs))
							return FALSE;

						szStart = pQT - 1;
					}
                    break;
                case psWhere:
                    {
                        int nLen = pQT - szStart - 6;
                        if (nLen > 0)
                            strncpy(m_strFrom.GetBufferSetLength(nLen), szStart, nLen);
                        m_strWhere = pQT;
                        ps  = psOther;
                    }
                    break;
                }
            }

            szCurKW = szKeyword;
            fWhiteSpace = true;
            switch (ch)
            {
            case '\'':
                fStrConst = true;
                break;
            case '(':
                nBrLevelR++;
                break;
            case ')':
                if (!nBrLevelR)
                {
                    m_strLastError = "Ошибка в запросе.";
                    return FALSE;
                }
                nBrLevelR--;
                break;
            case '[':
                nBrLevelSQ++;
                break;
            case ']':
                if (!nBrLevelSQ)
                {
                    m_strLastError = "Ошибка в запросе.";
                    return FALSE;
                }
                nBrLevelSQ--;
                break;
            case ',':
                if (ps == psFrom && !nBrLevelR && !nBrLevelSQ)
                {
					CString cs(szStart, pQT - szStart - 1);
                    if (!ParseSelectListItem(cs))
                        return FALSE;
                    szStart = pQT;
                }
                break;
            }
        }
    }

    if (nBrLevelR || nBrLevelSQ)
    {
        m_strLastError = "Не закрыты скобки.";
        return FALSE;
    }
    else
        switch (ps)
        {
            case psSelect:
                m_strLastError = "Не найдено ключевое слово ""select"".";
                return FALSE;
                break;
            case psFrom:
                m_strLastError = "Не найдено ключевое слово ""from"".";
                return FALSE;
                break;
            case psWhere:
                m_strFrom = szStart;
                break;
        }

    if (!m_pSelectList->GetCount())
    {
        m_strLastError = "Список полей пуст.";
        return FALSE;
    }

    m_strFrom.TrimLeft(szTrimmedSimbols);
    m_strFrom.TrimRight(szTrimmedSimbols);
    if (m_strFrom.IsEmpty())
    {
        m_strLastError = "Не указаны источники данных в предложении ""from"".";
        return FALSE;
    }
    m_strWhere.TrimLeft(szTrimmedSimbols);
    m_strWhere.TrimRight(szTrimmedSimbols);
    
    if (!ParseSQLParams(m_strFrom, m_FromParams))
        return FALSE;

    if (!ParseSQLParams(m_strWhere, m_WhereParams))
        return FALSE;

    return TRUE;
}

BOOL CSQLProvider::ParseSQLParams(CString& strText, CSQLParamArray& params)
{
    LPCSTR pQT = strText;
    char ch;
    bool fStrConst = false;
    LPCSTR pParamName = NULL;

    while (true)
    {
        ch = *pQT++;

        if (pParamName)
        {
            if (!IsWordChar(ch))
            {
                CString strParamName(pParamName, pQT - pParamName - 1);
                CStringList ParamList;
                if (ch == '(')
                {
                    int nProcessed;
                    if (!CMetaNameParser::DefineList(pQT - 1, ParamList, nProcessed))
                    {
                        m_strLastError.Format("Ошибка определения параметра \"%s\"", strParamName.operator LPCTSTR());
                        return FALSE;
                    }
                    pQT += nProcessed - 1;
                }

                int nStart = pParamName - strText.operator LPCSTR() - 1;

				CString strLabel;
                CSQLQueryParam* pSQLQueryParam = NewSQLQueryParam(strParamName, ParamList, strLabel);
                if (pSQLQueryParam)
                    params.Add(pSQLQueryParam);
                else
                    return FALSE;

                strText.Delete(nStart, pQT - pParamName + 1);
				strText.Insert(nStart, strLabel);
                pQT = strText.operator LPCSTR() + nStart + strLabel.GetLength();
                pParamName = NULL;
				
            }
            continue;
        }
        
        if (!ch) break;

        if (fStrConst)
        {
            if (ch == '\'')
                fStrConst = false;
            continue;
        }
        
        if (ch == '\'')
            fStrConst = true;
        else if (ch == '?')
            pParamName = pQT;
    }
    return TRUE;
}

BOOL CSQLProvider::FindKeyword(LPCSTR szText, LPCSTR szKeyword, int& nResult)
{
    nResult = -1;

    LPCSTR pQT = szText;
    char ch;
    LPCSTR szCurKW = szKeyword;
    bool fWhiteSpace = true, fStrConst = false;
    int nBrLevelR = 0, nBrLevelSQ = 0;

    while (ch = *pQT++)
    {
        if (fStrConst)
        {
            if (ch == '\'')
                fStrConst = false;
            continue;
        }

        if (IsWordChar(ch))
        {
            if (fWhiteSpace && !nBrLevelR && !nBrLevelSQ)
            {
                if ((ch | 0x20) == *szCurKW)
                    szCurKW++;
                else
                {
                    szCurKW = szKeyword;
                    fWhiteSpace = false;
                }
            }
        }
        else
        {
            if (!*szCurKW)
            {
                nResult = pQT - szText - 1;
                break;
            }

            szCurKW = szKeyword;
            fWhiteSpace = true;
            switch (ch)
            {
            case '\'':
                fStrConst = true;
                break;
            case '(':
                nBrLevelR++;
                break;
            case ')':
                if (!nBrLevelR)
                {
                    m_strLastError = "Ошибка в запросе.";
                    return FALSE;
                }
                nBrLevelR--;
                break;
            case '[':
                nBrLevelSQ++;
                break;
            case ']':
                if (!nBrLevelSQ)
                {
                    m_strLastError = "Ошибка в запросе.";
                    return FALSE;
                }
                nBrLevelSQ--;
                break;
            }
        }
    }

    if (nBrLevelR || nBrLevelSQ)
    {
        m_strLastError = "Не закрыты скобки.";
        return FALSE;
    }
    if (nResult == -1 && !*szCurKW)
        nResult = pQT - szText - 1;

    return TRUE;
}

BOOL CSQLProvider::ParseSelectListItem(CString& strText)
{
    strText.TrimLeft(szTrimmedSimbols);
    strText.TrimRight(szTrimmedSimbols);
    
    if (strText.IsEmpty())
    {
        m_strLastError = "Недопустимое задание поля.";
        return FALSE;
    }

    int nPos;
    if (!FindKeyword(strText, "as", nPos))
        return FALSE;

    if (nPos == -1)
    {
        m_strLastError = "В определении поля не найдено ключевое слово ""as"".";
        return FALSE;
    }

    LPCSTR szName = strText;
    szName += nPos;
    CString strName(szName);
    strName.TrimLeft(szTrimmedSimbols);
    CString strExpr = strText.Left(nPos - 2);
    strExpr.TrimRight(szTrimmedSimbols);

    CSelectListItem* pSelectListItem = m_pSelectList->NewSelectListItem();
    pSelectListItem->SetName(strName);
    if (!ParseSQLParams(strExpr, pSelectListItem->m_params))
    {
        delete pSelectListItem;
        return FALSE;
    }
    pSelectListItem->m_strExpression = strExpr;

    BOOL bSuccess = OnAddToSelectList(pSelectListItem);
    if (bSuccess)
        static_cast<CCollection*>(m_pSelectList)->Add(pSelectListItem);
    
    return bSuccess;
}

BOOL CSQLProvider::SetQuery(LPCSTR szQueryText)
{
    ClearQuery();
    CString strQT = szQueryText;
    if (!RemoveComments(strQT))
        return FALSE;
    
    return ParseQuery(strQT);
}

void CSQLProvider::ClearFields()
{
	m_pIDField = NULL;
    static_cast<CCollection*>(m_pSelectList)->Clear();
    m_fields.RemoveAll();
}

BOOL CSQLProvider::SetOrderKey(LPCSTR szOrderKey)
{
    CString str(szOrderKey);
    str.TrimLeft(szTrimmedSimbols);
    str.TrimRight(szTrimmedSimbols);

    LPSTR szOK = str.LockBuffer();
    for (int i = 0; i < m_orderkey.GetSize(); i++)
        m_orderkey[i]->m_nFlags &= ~(IS_ORDERKEY | BACK_ORDER);
    m_orderkey.RemoveAll();

    for (int nLen = str.GetLength(); nLen > 0;)
    {
        LPSTR szCurItem = strtok(szOK, ",");
        int nItemLen = strlen(szCurItem);
        nLen -= nItemLen;
        szOK += nItemLen;
        if (nLen > 0)
        {
            nLen--;
            szOK++;
        }

        LPSTR szCurWord = strtok(szCurItem, szTrimmedSimbols);
		if (!szCurWord)
		{
            m_strLastError = "Недопустимое задание ключа порядка: не указано поле данных.";
            return FALSE;
		}

        CSelectListItem* pSelectListItem;
        for (int n = 0; szCurWord; szCurWord = strtok(NULL, szTrimmedSimbols), n++)
        {
            if (n > 1)
            {
                m_strLastError.Format("Недопустимое задание ключа порядка (%s).", szCurWord);
                return FALSE;
            }
            CString str(szCurWord);
            str.TrimLeft(szTrimmedSimbols);

            if (n == 0)
            {
                pSelectListItem = IncludeToFields(str);
                if (!pSelectListItem)
                {
                    m_strLastError.Format("Неизвестное поле ""%s"".", str.operator LPCTSTR());
                    return FALSE;
                }
                else
                {
                    if (pSelectListItem->m_params.GetSize())
                    {
                        m_strLastError.Format("Выражения полей ключа порядка не могут иметь SQL параметров (%s).", str.operator LPCTSTR());
                        return FALSE;
                    }
                    pSelectListItem->m_nFlags |= IS_ORDERKEY;
                    m_orderkey.Add(pSelectListItem);
                }
            }
            else //if (n == 1)
            {
                if (!str.CompareNoCase("desc"))
                    pSelectListItem->m_nFlags |= BACK_ORDER;
                else if (!str.CompareNoCase("asc"))
                {}
                else
                {
                    m_strLastError = "Недопустимое направление упорядочивания.";
                    return FALSE;
                }
            }
        }
    }

    if (!m_orderkey.GetSize())
    {
        m_strLastError = "Ключ порядка не может быть пустым.";
        return FALSE;
    }
	
	ReorderFields();
    OnFieldsChanged();
	ResetData();
    return TRUE;
}

BOOL CSQLProvider::BuildQuery(CString& strQT, CSQLRow* pRowFrom, int nRowsCount, CSQLParamMap& params)
{
    if (m_strFrom.IsEmpty())
    {
        m_strLastError = "Не указан текст запроса.";
        return FALSE;
    }

    strQT.Empty();
    params.RemoveAll();

    if (m_QueryStyle == qsOracle && nRowsCount)
        strQT = "select * from ( select";
	else
        strQT = "select";
    
    if (nRowsCount && (m_QueryStyle == qsMSSQL || m_QueryStyle == qsVFP))
        AddLimitRowCount(strQT, nRowsCount);
    
    strQT += "\r\n";

    
    CString strTmp;

    int i;

    for (i = 0; i < m_fields.GetSize(); i++)
    {
        CSelectListItem* pSelectListItem = m_fields[i];
        
        if (i)
            strQT += ",\r\n";

        strQT += pSelectListItem->m_strExpression;
        strQT += " as ";
        strQT += pSelectListItem->GetName();

        params.Append(pSelectListItem->m_params);
    }

    CString strWhere;
    CSQLParamArray OKparams;
    CString strOrder;
    for (i = 0; i < m_orderkey.GetSize(); i++)
    {
        CSelectListItem* pSelectListItem = m_orderkey[i];

        if (nRowsCount)
        {
            if (i)
                strOrder += ", ";

            BOOL bBackOrder = pSelectListItem->m_nFlags & BACK_ORDER;
            if (nRowsCount < 0)
                bBackOrder = !bBackOrder;

            if (pRowFrom)
            {
                if (!strWhere.IsEmpty())
                    strWhere += " or ";

                strWhere += pSelectListItem->m_strExpression;

                if (bBackOrder)
                    strWhere += " < ";
                else
                    strWhere += " > ";

                CSQLQueryParam* pSQLQueryParam = GetOrderKeyParam(i);
				if (pSQLQueryParam)
					OKparams.Add(GetOrderKeyParam(i));

				if (pSQLQueryParam && GetQueryStyle() == qsMSSQL)
					strTmp = pSQLQueryParam->GetName();
				else
					pRowFrom->SQLFormatField(pSelectListItem->m_nFieldIndex, strTmp);
                strWhere += strTmp;

                for (int j = 0; j < i; j++)
                {
                    CSelectListItem* pSelectListItem = m_orderkey[j];
                    
                    strWhere += " and ";
                    strWhere += pSelectListItem->m_strExpression;
                    strWhere += " = ";
                    
					pSQLQueryParam = GetOrderKeyParam(j);
					if (pSQLQueryParam && GetQueryStyle() == qsMSSQL)
						strTmp = pSQLQueryParam->GetName();
					else
						pRowFrom->SQLFormatField(pSelectListItem->m_nFieldIndex, strTmp);
                    strWhere += strTmp;
                }
            }

            strOrder += pSelectListItem->m_strExpression;
            if (bBackOrder)
                strOrder += " desc";
        }
        else
        {
            if (i)
                strWhere += " and ";
            strWhere += pSelectListItem->m_strExpression;
            strWhere += " = ";

            CSQLQueryParam* pSQLQueryParam = GetOrderKeyParam(i);
			if (pSQLQueryParam)
				OKparams.Add(GetOrderKeyParam(i));

			if (pSQLQueryParam && GetQueryStyle() == qsMSSQL)
				strTmp = pSQLQueryParam->GetName();
			else
				pRowFrom->SQLFormatField(pSelectListItem->m_nFieldIndex, strTmp);
            strWhere += strTmp;
        }
    }
	params.Append(OKparams);

    strQT += "\r\nfrom ";
    strQT += m_strFrom;
    params.Append(m_FromParams);
    strQT += "\r\n";

    if (strWhere.IsEmpty())
    {
        if (!m_strWhere.IsEmpty())
        {
            strQT += "where ";
            strQT += m_strWhere;
            params.Append(m_WhereParams);
        }
    }
    else
    {
        if (m_strWhere.IsEmpty())
        {
            strQT += "where ";
            strQT += strWhere;
        }
        else
        {
            strQT += "where (";
            strQT += strWhere;
            strQT += ") and (";
            strQT += m_strWhere;
            params.Append(m_WhereParams);
            strQT += ')';
        }
    }

    if (nRowsCount)
    {
        strQT += "\r\norder by ";
        strQT += strOrder;
    }
    
    if (nRowsCount && (m_QueryStyle == qsMySQL || m_QueryStyle == qsOracle))
        AddLimitRowCount(strQT, nRowsCount);

    return TRUE;
}

void CSQLProvider::AddLimitRowCount(CString& strQT, int nCount)
{
    switch (m_QueryStyle)
    {
    case qsMSSQL:
    case qsVFP:
        strQT += " top ";
        break;
    case qsMySQL:
        strQT += "\r\nlimit ";
        break;
    case qsOracle:
        strQT += "\r\n)\r\nwhere rownum <= ";
        break;
    }

    char arLimit[12];
    itoa(nCount > 0 ? nCount : -nCount, arLimit, 10);
    strQT += arLimit;
}

void CSQLProvider::QueryField(LPCSTR szFieldName)
{
    m_pSelectList->Add(szFieldName);
}

void CSQLProvider::ReleaseField(LPCSTR szFieldName)
{
    CSelectListItem* pSelectListItem = m_pSelectList->GetAt(szFieldName);
    if (pSelectListItem)
    {
        if (pSelectListItem->m_nFlags & AUTO_DELETE)
            m_pSelectList->Remove(pSelectListItem->m_nFieldIndex);
    }
}

CSelectList* CSQLProvider::GetSelectList() const
{
    return m_pSelectList;
}

int CSQLProvider::GetOrderKeyFieldCount() const
{
    return m_orderkey.GetSize();
}

CSelectListItem* CSQLProvider::GetOrderKeyField(int nIndex) const
{
    return m_orderkey[nIndex];
}

BOOL CSQLProvider::BuildQuickSearchQuery(CString& strQT, CSelectListItem* pSearchItem, CSQLParamMap& params, LPCSTR szCondition, LPCSTR szSearchLabel)
{
    if (!m_orderkey.GetSize())
    {
		m_strLastError = "Не установлен ключ порядка.";
		return FALSE;
    }
	
    CString strOrder;
	
    if (m_QueryStyle == qsOracle)
		strQT = "select * from ( select";
	else
		strQT = "select";
	
    if (m_QueryStyle == qsMSSQL || m_QueryStyle == qsVFP)
		AddLimitRowCount(strQT, 1);
    strQT += "\r\n";
	
    for (int i = 0; i < m_fields.GetSize(); i++)
    {
		CSelectListItem* pItem = m_fields[i];
		if (i)
			strQT += ",\r\n";
		
		strQT += pItem->m_strExpression;
		params.Append(pItem->m_params);
		strQT += " as ";
		strQT += pItem->GetName();
    }
	
    for (int i = 0; i < m_orderkey.GetSize(); i++)
    {
		CSelectListItem* pItem = m_orderkey[i];
		if (i)
			strOrder += ", ";

		strOrder += pItem->m_strExpression;
    }

    strQT += "\r\nfrom ";
    strQT += m_strFrom;
    params.Append(m_FromParams);
	
    strQT += "\r\nwhere (";
    if (!m_strWhere.IsEmpty())
    {
		strQT += m_strWhere;
		params.Append(m_WhereParams);
		strQT += ") and (";
    }
    if (szCondition  && szCondition[0])
    {
		strQT += szCondition;
		strQT += ") and (";
    }
    strQT += pSearchItem->m_strExpression;
    params.Append(pSearchItem->m_params);
	
    strQT += " like ";
	strQT += szSearchLabel;
    strQT += ')';
	
    strQT += "\r\norder by ";
    strQT += strOrder;
	
    if (m_QueryStyle == qsMySQL || m_QueryStyle == qsOracle)
		AddLimitRowCount(strQT, 1);
	
    return TRUE;
} 

void CSQLProvider::ClearQuery()
{
    ClearFields();
    m_strFrom.Empty();
    ClearParams(m_FromParams);
    m_strWhere.Empty();
    ClearParams(m_WhereParams);
}

void CSQLProvider::ClearParams(CSQLParamArray& params)
{
    for (int i = 0; i < params.GetSize(); i++)
        delete params[i];
    params.RemoveAll();
}

BOOL CSQLProvider::SetIDField(LPCSTR szName)
{
	if (m_pIDField)
		m_pIDField->m_nFlags &= ~IS_IDFIELD;

	m_pIDField = NULL;
	
	if (!strlen(szName))
		return TRUE;

    m_pIDField = IncludeToFields(szName);
    if (m_pIDField)
    {
        m_pIDField->m_nFlags |= IS_IDFIELD;
        m_pIDField->m_nFlags &= ~AUTO_DELETE;

		ReorderFields();
        OnFieldsChanged();
        return TRUE;
    }
    else
    {
        m_strLastError = "Недопустимое указание поля.";
        return FALSE;
    }
}

CSelectListItem* CSQLProvider::GetIDField() const
{
    return m_pIDField;
}

CSelectListItem* CSQLProvider::IncludeToFields(LPCSTR szName)
{
	CSelectListItem* pItem = static_cast<CSelectListItem*>(static_cast<CCollection*>(m_pSelectList)->GetItem(szName));
	if (pItem && !(pItem->m_nFlags & IS_FIELD))
	{
		pItem->m_nFlags |= IS_FIELD;
        pItem->m_nFieldIndex = m_fields.Add(pItem);
	}
	return pItem;
}

bool CSelectListItem::CSelectListItemLess::operator()(CSelectListItem* lhs, CSelectListItem* rhs)
{
	DWORD const lhsOrder = lhs->m_nFlags & IS_ORDERKEY;
	DWORD const rhsOrder = rhs->m_nFlags & IS_ORDERKEY;
	bool ret = rhsOrder < lhsOrder;
	if (!ret)
	{
		if (lhsOrder)
			ret = lhs->m_nFieldIndex < rhs->m_nFieldIndex;
		else if (!rhsOrder)
		{
			DWORD const lhsID = lhs->m_nFlags & IS_IDFIELD;
			DWORD const rhsID = rhs->m_nFlags & IS_IDFIELD;
			ret = rhsID < lhsID;
			if (!ret && lhsID == rhsID)
				ret = lhs->m_nFieldIndex < rhs->m_nFieldIndex;
		}
	}
	return ret;
}

void CSQLProvider::ReorderFields()
{
	CSelectListItem** begin = reinterpret_cast<CSelectListItem**>(m_fields.GetData());
	CSelectListItem** end = begin + m_fields.GetSize();

	if (begin != end)
	{
		std::sort(begin, end, CSelectListItem::CSelectListItemLess());
		CSelectListItem** pItem = begin;
		for (int i = 0; pItem != end; ++i)
			(*pItem++)->m_nFieldIndex = i;
	}
}

BOOL CSQLProvider::BuildByIDQuery(CString& strQT, CSQLParamMap& params, LPCSTR szIDText, CSQLQueryParam* pIDParam)
{
    if (!m_orderkey.GetSize())
    {
        m_strLastError = "Не установлен ключ порядка.";
        return FALSE;
    }

    if (!m_pIDField)
    {
        m_strLastError = "Не установлено идентификационное поле строки.";
        return FALSE;
    }

    strQT.Empty();
    params.RemoveAll();

    CString strOrder;

    if (m_QueryStyle == qsOracle)
        strQT = "select * from ( select";
	else
        strQT = "select";
    
    if (m_QueryStyle == qsMSSQL || m_QueryStyle == qsVFP)
        AddLimitRowCount(strQT, 1);
    strQT += "\r\n";

    for (int i = 0; i < m_fields.GetSize(); i++)
    {
        CSelectListItem* pItem = m_fields[i];
        if (i)
            strQT += ",\r\n";

        strQT += pItem->m_strExpression;
        params.Append(pItem->m_params);
        strQT += " as ";
        strQT += pItem->GetName();
    }

    for (int i = 0; i < m_orderkey.GetSize(); i++)
	{
        CSelectListItem* pItem = m_orderkey[i];
        if (i)
            strOrder += ", ";
        strOrder += pItem->m_strExpression;
	}

	strQT += "\r\nfrom ";
    strQT += m_strFrom;
    params.Append(m_FromParams);

    strQT += "\r\nwhere ";
    if (!m_strWhere.IsEmpty())
    {
        strQT += '(';
        strQT += m_strWhere;
        params.Append(m_WhereParams);
        strQT += ") and (";
    }
    strQT += m_pIDField->m_strExpression;
    params.Append(m_pIDField->m_params);

    strQT += " = ";
    strQT += szIDText;
    if (pIDParam)
        params.Add(pIDParam);

    if (!m_strWhere.IsEmpty())
        strQT += ')';

    strQT += "\r\norder by ";
    strQT += strOrder;

    if (m_QueryStyle == qsMySQL || m_QueryStyle == qsOracle)
        AddLimitRowCount(strQT, 1);

    return TRUE;
}

int CSQLProvider::GetQueryFieldCount() const
{
    return m_pSelectList->GetCount();
}

CSelectListItem* CSQLProvider::GetQueryField(int nIndex) const
{
    return static_cast<CSelectListItem*>(m_pSelectList->GetItem(nIndex));
}

CSelectListItem* CSQLProvider::GetQueryField(LPCSTR szName) const
{
    return static_cast<CSelectListItem*>(m_pSelectList->GetItem(szName));
}

int CSQLProvider::IndexOf(LPCSTR szName) const
{
    return m_pSelectList->IndexOf(szName);
}
