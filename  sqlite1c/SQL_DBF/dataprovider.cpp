// dataprovider.cpp
#include "StdAfx.h"
#include "dataprovider.h"
#include "valuework.hpp"

BL_INIT_CONTEXT(SQLiteDataProvider);

const char idParamName[]		= "@sqlite_data_provider_id";
const char rowCountParamName[]	= "@sqlite_data_provider_rowcount";
const char keyParamPattern[]	= "@sqlite_data_provider_key";
const char trimPattern[]		= " \t\r\n";

int SQLiteDataRow::GetFieldCount()
{
	return m_fCount;
}

int SQLiteDataRow::GetFieldIndexOf(LPCSTR szFieldName)
{
	return m_pParent->findField(szFieldName);
}

LPCSTR SQLiteDataRow::GetFieldName(int nIndex)
{
	return m_pParent->fieldName(nIndex)	;
}

DataType SQLiteDataRow::GetFieldType(int nIndex)
{
	switch(m_pValues[nIndex].type)
	{
	case typeNumber:
		return dtNumeric;
	case typeDate:
		return dtDate;
	default:
		return dtText;
	}
}

void SQLiteDataRow::FormatField(int nIndex, CString& strValue)
{
	CV7DataRow::FormatValue(m_pValues[nIndex], strValue);
}

UINT SQLiteDataRow::GetRowIndex()
{
	return 0;
}

const CValue& SQLiteDataRow::GetValue(int nIndex) const
{
	return m_pValues[nIndex];
}

SQLiteDataRow::SQLiteDataRow(SQLiteDataProvider* pParent)
{
	m_pParent = pParent;
	m_fCount = m_pParent->fieldsCount();
	m_pValues = (CValue*)new char[sizeof(CValue) * m_fCount];
}

SQLiteDataRow::SQLiteDataRow(DWORD fc)
{
	m_pParent = NULL;
	m_fCount = fc;
	m_pValues = (CValue*)new char[sizeof(CValue) * m_fCount];
	for(DWORD i = 0; i < m_fCount; i++)
		m_pValues[i].CValue::CValue();
}

SQLiteDataRow::~SQLiteDataRow()
{
	CValue* pValue = m_pValues;
	for(DWORD c = m_fCount; c--; pValue++)
		pValue->CValue::~CValue();
	delete [] (char*) m_pValues;
}

void SQLiteDataProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
	clearRows();
	SQLiteQuery* pQuery = getQuery(nRowsCount > 0 ? (pRowFrom ? qDown : qTop) : (pRowFrom ? qUp : qBottom));
	bindKeyValues(pQuery, static_cast<SQLiteDataRow*>(pRowFrom));
	pQuery->setSqlParam(rowCountParamName, nRowsCount < 0 ? -nRowsCount : nRowsCount, 0);
	DataProviderResultLoader executor(this, &m_rows);
	executor.execute(pQuery);
}

CDataRow* SQLiteDataProvider::Fetch()
{
	if(m_rows.GetSize())
	{
		CDataRow* pRow = (SQLiteDataRow*)m_rows[0];
		m_rows.RemoveAt(0);
		return pRow;
	}
	return NULL;
}

BOOL SQLiteDataProvider::RefreshRow(CDataRow* pRowFrom)
{
	if(m_debug)
		DoMsgLine("Поставщие данных SQLite: обновить строку");
	if(!pRowFrom)
		return FALSE;
	SQLiteDataRow* pRow = static_cast<SQLiteDataRow*>(pRowFrom);
	SQLiteQuery* pQuery = getQuery(qCurRow);
	bindKeyValues(pQuery, pRow);
	
	RowsArray tempDst;
	DataProviderResultLoader executor(this, &tempDst);
	executor.execute(pQuery);
	if(tempDst.GetSize() != 1)
		return FALSE;
	
	SQLiteDataRow* pNewRow = tempDst[0];
	
	CValue* pOldValues = pRow->m_pValues;
	pRow->m_pValues = pNewRow->m_pValues;
	pNewRow->m_pValues = pOldValues;
	pNewRow->m_fCount = pRow->m_fCount;
	pRow->m_fCount = fieldsCount();
	
	delete pNewRow;
	return TRUE;
}

BOOL SQLiteDataProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
	CValue	*pVals1 = static_cast<SQLiteDataRow*>(pRow1)->m_pValues,
			*pVals2 = static_cast<SQLiteDataRow*>(pRow2)->m_pValues;

	for(DWORD idx = 0, midx = keyFieldsCount(); idx < midx ; idx++)
	{
		if(pVals1[idx] != pVals2[idx])
			return FALSE;
	}
	return TRUE;
}

BOOL SQLiteDataProvider::GetRowCount(UINT* pCount)
{
	return FALSE;	
}

DataType SQLiteDataProvider::GetQuickSearchType(LPCSTR szFieldName)
{
	field_info* pFI;
	if(m_fieldByName.Lookup(szFieldName, pFI) && 0 != (pFI->flags & field_info::qsPossible))
		return dtText;
	return dtUndefined;//dtText;
}

void SQLiteDataProvider::QuickSearch(stQuickSearchPattern& QSPattern)
{
	field_info* pFI;
	if(QSPattern.dataType != dtText || !m_fieldByName.Lookup(QSPattern.szFieldName, pFI)
		|| 0 == (pFI->flags & field_info::qsPossible) || QSPattern.pTextData->IsEmpty())
	{
		QSPattern.pTextData->Empty();
		return;
	}
	SQLiteQuery* pQuery = m_queries[qQuickSearch];
	if(!pQuery)
		pQuery = m_queries[qQuickSearch] = m_pDataBase->newQuery();
	if(m_lastQSColumn != pFI)
	{
		m_QSColumn = -1;
		CString strSqlText("select\r\n"), strOrder(" order by\r\n");
		field_info* pFields = m_fieldsInfo;
		for(DWORD idx = 0; idx < keyFieldsCount(); idx++, pFields++)
		{
			strSqlText += pFields->expression + pFields->alias + ",\r\n";
			strOrder += pFields->expression + ",\r\n";
			if(pFields == pFI)
				m_QSColumn = idx;
		}
		if(-1 == m_QSColumn)
		{
			strSqlText += pFI->expression + pFI->alias;
			m_QSColumn = idx;
		}
		else
			strSqlText.GetBufferSetLength(strSqlText.GetLength() - 3);
		strSqlText += m_from;
		if(!m_where.IsEmpty())
			strSqlText = strSqlText + " where\r\n" + m_where + "\r\n";
		strOrder.GetBufferSetLength(strOrder.GetLength() - 3);
		strSqlText += strOrder;

		pQuery->setDebug(m_debug);
		pQuery->prepare(strSqlText);
		m_lastQSColumn = pFI;
	}

	m_sqlParams.applyParamsToQuery(pQuery, -1);
	
	SQLiteDataRow qsRow(keyFieldsCount());
	QuickSearchResultLoader qsloader(*QSPattern.pTextData, qsRow.m_pValues, keyFieldsCount(), m_QSColumn);
	int maxSymb = qsloader.find(pQuery);

	QSPattern.pTextData->GetBufferSetLength(maxSymb);
	if(maxSymb)
		SetCurrentRow(&qsRow);
}

void SQLiteDataProvider::OnAttach()
{
	ResetData();
}

CV7DataRow* SQLiteDataProvider::GetV7DataRow(CDataRow* pDataRow) const
{
	return static_cast<SQLiteDataRow*>(pDataRow);
}

void SQLiteDataProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
	if(idFieldPos() >= 0)
		value = static_cast<SQLiteDataRow*>(pDataRow)->m_pValues[idFieldPos()];
	else
		value.Reset();
}

int modificatorByType(typesOfFields t)
{
	switch(t)
	{
	case ttReference:
	case ttDocument:
	case ttEnum:
	case ttAccount:
	case ttCalendar:
		return 1;
	case ttUndefine:
		return -1;
		break;
	}
	return 0;
}

CDataRow* SQLiteDataProvider::BuildRowByValue(const CValue& value)
{
	if(idFieldPos() < 0)
		return NULL;
	SQLiteQuery* pQuery = getQuery(qID);
	pQuery->setSqlParam(idParamName, value, modificatorByType(fieldInfo(idFieldPos()).type));
	
	RowsArray tempDst;
	DataProviderResultLoader executor(this, &tempDst);
	executor.execute(pQuery);
	if(tempDst.GetSize() != 1)
		return NULL;
	return (SQLiteDataRow*)tempDst[0];
}

SQLiteDataProvider::SQLiteDataProvider()
{
	m_pDataBase = NULL;
	m_fieldsInfo = NULL;
	m_fieldsInfoCount = 0;
	memset(m_queries, 0, sizeof(m_queries));
	m_debug = FALSE;
	m_lastQSColumn = NULL;
	m_queryFieldsNames = NULL;
	m_keyParamNames = NULL;
}

BOOL SQLiteDataProvider::SetDataBase(CValue** ppParams)
{
	clearAll();
	if(ppParams[0]->IsEmpty())
	{
		m_pDataBase = new CSLDataBase;
		m_pDataBase->open(":memory:");
	}
	else
	{
		if(ppParams[0]->type != 100 || !safeDynCast(ppParams[0]->m_Context, m_pDataBase))
			CBLModule::RaiseExtRuntimeError("Переданный параметр не база данных SQLite", FALSE);
		m_pDataBase->m_RefCount++;
	}
	ResetData();
	return TRUE;
}

void removeComments(CString& strQuery)
{
	enum {sNone, sQuote, sLiteral, sMinus, sDiv, sRemark, sMultRem, sMultRemMult};
	DWORD state = sNone;
	LPSTR pWrite = strQuery.GetBufferSetLength(strQuery.GetLength());
	LPCSTR pRead = pWrite, pStart = pRead;
	for(;; pRead++)
	{
		DWORD s = (DWORD)(BYTE)*pRead;
		switch(state)
		{
		case sNone:
			if('\'' == s)
				state = sQuote;
			else if('[' == s)
				state = sLiteral;
			else if('-' == s)
			{
				state = sMinus;
				continue;	// Пока символ не копируем
			}
			else if('/' == s)
			{
				state = sDiv;
				continue;	// Пока символ не копируем
			}
			break;
		case sQuote: // литерал в ' '
			if('\'' == s)
				state = sNone;
			break;
		case sLiteral:
			if(']' == s)
				state = sNone;
		    break;
		case sMinus:	//	'-'
			if('-' == s)
			{
				state = sRemark;
				continue;
			}
			// Это был не коммент, докопируем минус
			*pWrite++ = '-';
			if('/' == s)
			{
				state = sDiv;
				continue;
			}
			state = sNone;
		    break;
		case sDiv:	// '/'
			if('*' == s)
			{
				state = sMultRem;
				continue;
			}
			*pWrite++ = '/';
			if('-' == s)
			{
				state = sMinus;
				continue;
			}
			state = sNone;
			break;
		case sRemark:	// '--'
			if('\r' != s && 0 != s)
				continue;
			*pWrite++ = ' ';	// Заменим коммент пробелом, символ \r скопируем
			state = sNone;
			break;
		case sMultRem:	// '/*'
			if(0 != s)
			{
				if('*' == s)
					state = sMultRemMult;
				continue;
			}
		    break;
		case sMultRemMult:	// '/*  *'
			if(0 != s)
			{
				if('/' == s)
				{
					state = sNone;
					*pWrite++ = ' ';
				}
				else if('*' != s)
					state = sMultRem;
				continue;
			}
		    break;
		}
		if(!s)
			break;
		if(pRead > pWrite)
			*pWrite = s;
		pWrite++;
	}
	if(pRead > pWrite + 1)	// Комменты были
		strQuery.GetBufferSetLength(pWrite - pStart);
	strQuery.TrimLeft(trimPattern);
}

inline BOOL isWordSymbol(DWORD s)
{
	return (s >= 'A' && s <='Z') || (s >= 'a' && s <='z') || s > 0x80 || s == '_' || (s >= '0' && s <= '9');
}

void splitQuery(CString& strQuery, CStringArray& fields, CString& strFrom, CString& strWhere)
{
	removeComments(strQuery);
	LPCSTR ptr = strQuery;
	ptr += sizeof("select") - 1;
	
	// Будем выделять поля
	// Поля разделяются либо , либо from вне скобок строк литералов
	DWORD parentheses = 0, inLiterals = 0;
	LPCSTR pStartOfField = ptr, pStartOfFrom = NULL;
	LPCSTR keyword = "from";
	
	for(;;)
	{
		DWORD s = (DWORD)(BYTE)*ptr++;
		if(!s)
			break;
		if(inLiterals)
		{
			if( (1 == inLiterals && '\'' == s) ||
				(2 == inLiterals && ']' == s))
				inLiterals = 0;
		}
		else if('\'' == s)
			inLiterals = 1;
		else if('[' == s)
			inLiterals = 2;
		else if('(' == s)
			parentheses++;
		else if(')' == s)
			parentheses--;
		else if(0 == parentheses)
		{
			if(!pStartOfFrom && ',' == s)
			{
				fields.Add(CString(pStartOfField, ptr - pStartOfField - 1));
				pStartOfField = ptr;
			}
			else
			{
				// Проверим ключевое слово
				LPCSTR pKeyword = keyword;
				DWORD k;
				for(;;)
				{
					k = (DWORD)(BYTE)*pKeyword++;
					if(k != (s | 0x20) || !s)
						break;
					s = (DWORD)(BYTE)*ptr++;
				}
				if(isWordSymbol(s))
				{
					do 
					{
						s = (DWORD)(BYTE)*ptr++;
					}while(isWordSymbol(s));
					ptr--;
				}
				else if(0 == k)
				{
					if(!pStartOfFrom)
					{
						pStartOfFrom = ptr - 1;
						keyword = "where";
						fields.Add(CString(pStartOfField, ptr - pStartOfField - sizeof("from")));
					}
					else
					{
						DWORD fromLen = ptr - pStartOfFrom - sizeof("where");
						memcpy(strFrom.GetBufferSetLength(fromLen), pStartOfFrom, fromLen);
						strWhere = ptr - 1;
						return;
					}
				}
			}
		}
	}
	strFrom = pStartOfFrom;
}

BOOL SQLiteDataProvider::SetQueryText(CValue** ppParams)
{
	if(!m_pDataBase)
		CBLModule::RaiseExtRuntimeError("Неустановлена база данных", FALSE);
	CString strQueryText = ppParams[0]->GetString();
	CString strKeyField = ppParams[1]->GetString();
	CString strIDField = ppParams[2]->GetString();

	if(strKeyField.IsEmpty())
		CBLModule::RaiseExtRuntimeError("Не заданы ключевые поля", FALSE);
	CStringArray fieldNames;
	CArray<typesOfFields, typesOfFields> types;
	DWORD fieldsCount;
	// Для начала проверим, что запрос синтаксически правилен
	{
		auto_ptr<SQLiteQuery> qTest(m_pDataBase->newQuery());
		m_metaParser.processSql(strQueryText);
		qTest->prepare(strQueryText);
		// Получим имена и типы колонок запроса
		qTest->getFields(fieldNames, types);
		fieldsCount = fieldNames.GetSize();
		if(!fieldsCount)
			CBLModule::RaiseExtRuntimeError("Передан запрос не на выборку", FALSE);
	}
	// Теперь надо найти номера ключевого и ид поля.
	CDWordArray newKeyFields;
	int newIDField = -1;
	{
		CStringArray strKeyFields;
		SplitStr2Array(strKeyField, strKeyFields, ',');
		CNoCaseMap<int> namesToPos;
		for(DWORD idx = 0; idx < fieldsCount; idx++)
			namesToPos[fieldNames[idx]] = idx;
		for(idx = 0; idx < strKeyFields.GetSize(); idx++)
		{
			CString keyName = strKeyFields[idx];
			keyName.TrimLeft(trimPattern);
			keyName.TrimRight(trimPattern);
			SQLiteQuery::typeField(keyName);
			int pos;
			if(!namesToPos.Lookup(keyName, pos))
			{
				CString msg;
				msg.Format("Ключевое поле '%s' не найдено в составе полей запроса", keyName);
				CBLModule::RaiseExtRuntimeError(msg, FALSE);
			}
			for(DWORD k = 0; k < newKeyFields.GetSize(); k++)
			{
				if(newKeyFields[k] == pos)
				{
					CString msg;
					msg.Format("Ключевое поле '%s' повторяется в составе ключевых полей", keyName);
					CBLModule::RaiseExtRuntimeError(msg, FALSE);
				}
			}
			newKeyFields.Add(pos);
		}
		if(!newKeyFields.GetSize())
			CBLModule::RaiseExtRuntimeError("Не задано ни одного ключевого поля", FALSE);
		if(!strIDField.IsEmpty())
		{
			strIDField.TrimLeft(trimPattern);
			strIDField.TrimRight(trimPattern);
			SQLiteQuery::typeField(strIDField);
			if(!namesToPos.Lookup(strIDField, newIDField))
			{
				CString msg;
				msg.Format("Идентификационное поле '%s' не найдено в составе полей запроса", strIDField);
				CBLModule::RaiseExtRuntimeError(msg, FALSE);
			}
		}
	}
	// Разобъем запрос на поля, источник и условия
	CStringArray fieldsExpressions, fieldAliases;
	CString from, where;
	splitQuery(strQueryText, fieldsExpressions, from, where);
	// Каждое поле надо разделить на выражение и алиас
	for(DWORD idx = 0, midx = fieldsExpressions.GetSize(); idx < midx; idx++)
	{
		CString expr = fieldsExpressions[idx], alias;
		expr.TrimLeft(trimPattern);
		expr.TrimRight(trimPattern);
		LPCSTR start = expr, ptr = start + expr.GetLength() - 1;
		if(*ptr == ']')
		{
			while(*--ptr != '[');
			LPCSTR pAs = --ptr;
			while(pAs > start + 2 && (*pAs == ' ' || *pAs == '\t' || *pAs == '\r' || *pAs == '\n'))
				pAs--;
			if(pAs[0] | 0x20 == 's' && pAs[-1] | 0x20 == 'a' &&
				(pAs[-2] == ' ' || pAs[-2] == '\t' || pAs[-2] == '\n'))
				ptr = pAs - 1;
			alias = ptr;
			expr.GetBufferSetLength(ptr - start);
		}
		fieldsExpressions[idx] = expr;
		fieldAliases.Add(alias);
	}

	m_metaParser.reset();
	
	CNoCaseMap<int> usedFields;
	if(m_fieldsInfo)
	{
		for(DWORD idx = 0; idx < fieldsInfoCount(); idx++)
		{
			if(0 != (m_fieldsInfo[idx].flags & field_info::inUse))
				usedFields[m_fieldsInfo[idx].nameOfQueryColumn] = 0;
		}
	}

	clearQueriesData();

	// Заполним fieldsInfo
	m_fieldsInfoCount = fieldsCount;
	m_fieldsInfo = new field_info[fieldsCount];
	field_info* pFI = m_fieldsInfo;

	// Инфу о полях уложим так:
	// Сначала ключевые поля в нужном порядке, затем ид-поле, если оно есть
	// затем остальные поля. Это нужно для того, что бы при изменении состава
	// колонок запроса ключевые и ид поля оставались на тех же позициях в запросе.
	// Сначала разместим ключевые поля
	m_keyFieldCount = newKeyFields.GetSize();
	m_keyParamNames = new CString[m_keyFieldCount];
	int nIDFieldAdded = -1;
	for(idx = 0; idx < m_keyFieldCount; idx++)
	{
		DWORD pos = newKeyFields[idx];
		pFI->type = types[pos];
		pFI->expression = fieldsExpressions[pos];
		pFI->alias = fieldAliases[pos];
		pFI->nameOfQueryColumn = fieldNames[pos];
		pFI->flags = field_info::keyField | field_info::inUse;
		m_fieldByName[pFI->nameOfQueryColumn] = pFI;
		fieldsExpressions[pos].Empty();
		m_keyParamNames[idx].Format("%s%i", keyParamPattern, idx);	// Имена SQL-параметров для ключей
		pFI++;
		if(newIDField == pos)
			nIDFieldAdded = idx;
	}
	// Теперь ид-поле, если оно есть и еще не добавлено
	if(newIDField >= 0 && nIDFieldAdded < 0)
	{
		pFI->type = types[newIDField];
		pFI->expression = fieldsExpressions[newIDField];
		pFI->alias = fieldAliases[newIDField];
		pFI->nameOfQueryColumn = fieldNames[newIDField];
		pFI->flags = field_info::idField | field_info::inUse;
		m_fieldByName[pFI->nameOfQueryColumn] = pFI;
		fieldsExpressions[newIDField].Empty();
		pFI++;
		nIDFieldAdded = idx;
	}
	// Теперь остальные поля
	for(idx = 0; idx < fieldsCount ; idx++)
	{
		if(!fieldsExpressions[idx].IsEmpty())
		{
			pFI->type = types[idx];
			pFI->expression = fieldsExpressions[idx];
			pFI->alias = fieldAliases[idx];
			pFI->nameOfQueryColumn = fieldNames[idx];
			pFI->flags = 0;
			m_fieldByName[pFI->nameOfQueryColumn] = pFI;
			pFI++;
		}
	}
	m_idFieldPos = nIDFieldAdded;

	from.TrimLeft(trimPattern);
	from.TrimRight(trimPattern);
	where.TrimLeft(trimPattern);
	where.TrimRight(trimPattern);

	m_from.Format("\r\n from\r\n%s\r\n ", from);
	if(!where.IsEmpty())
		m_where.Format("(%s)", where);

	if(usedFields.GetCount())
	{
		for(DWORD idx = 0; idx < fieldsInfoCount(); idx++)
		{
			int i;
			if(usedFields.Lookup(m_fieldsInfo[idx].nameOfQueryColumn, i))
				m_fieldsInfo[idx].flags |= field_info::inUse;
		}
	}
	ResetData();
	return TRUE;

}

void SQLiteDataProvider::clearRows()
{
	// Это на случай, если ТП недофетчило строки, полученные запросом
	SQLiteDataRow** ppRows = (SQLiteDataRow**)m_rows.GetData();
	for(DWORD c = m_rows.GetSize(); c--; )
		delete *ppRows++;
	m_rows.RemoveAll();
}

void SQLiteDataProvider::clearQueriesData()
{
	clearRows();
	for(DWORD idx = 0; idx < qLast; idx++)
	{
		delete m_queries[idx];
		m_queries[idx] = 0;
	}
	m_sqlParams.dirtyQueries = 0xFFFFFFFF;
	
	m_keyFieldCount = 0;
	m_countOfColumns = 0;
	delete [] m_queryFieldsNames;	// Просто имена колонок текущего запроса
	m_queryFieldsNames = NULL;
	delete [] m_keyParamNames;
	m_keyParamNames = NULL;
	m_idFieldPos = -1;				// Номер ид-поля в списке всех полей
	m_fieldByName.RemoveAll();
	m_select.Empty();
	m_from.Empty();
	m_where.Empty();
	m_lastQSColumn = NULL;
	
	delete [] m_fieldsInfo;		// Массив с описанием всех полей запроса
	m_fieldsInfoCount = 0;
}

void SQLiteDataProvider::clearAll()
{
	clearQueriesData();
	if(m_pDataBase)
	{
		m_pDataBase->DecrRef();
		m_pDataBase = NULL;
	}
}

SQLiteQuery* SQLiteDataProvider::getQuery(qTypes type)
{
	SQLiteQuery* pQuery = m_queries[type];
	if(!pQuery)
	{
		auto_ptr<SQLiteQuery> query(m_pDataBase->newQuery());
		query->setDebug(m_debug);
		if(m_debug)
		{
			static const LPCSTR names[] = {"следющие записи", "предыдущие записи", "первые записи",
				"последние записи", "запись по id", "обновление строки"};
			DoMsgLine("Поставщие данных SQLite: Формирование запроса '%s'", mmNone, names[type]);
		}
		// Формируем текст запроса
		if(m_select.IsEmpty())	// Надо перестроить список полей
		{
			DWORD countOfFieldsInQuery = 0;
			for(DWORD idx = 0; idx < fieldsInfoCount(); idx++)
			{
				if(fieldInfo(idx).flags & field_info::inUse)
					countOfFieldsInQuery++;
			}
			if(countOfFieldsInQuery != m_countOfColumns)
			{
				m_countOfColumns = countOfFieldsInQuery;
				delete [] m_queryFieldsNames;
				m_queryFieldsNames = new LPCSTR[m_countOfColumns];
			}
			m_select = "select\r\n";
			DWORD pos = 0;
			for(idx = 0; idx < fieldsInfoCount(); idx++)
			{
				const field_info& fi = fieldInfo(idx);
				if(fi.flags & field_info::inUse)
				{
					m_select += fi.expression + fi.alias + ",\r\n";
					m_fieldsInfo[idx].posInQuery = pos;
					m_queryFieldsNames[pos] = fi.nameOfQueryColumn;
					pos++;
				}
				else
					m_fieldsInfo[idx].posInQuery = -1;
			}
			m_select.GetBufferSetLength(m_select.GetLength() - 3);
		}

		CString mainSqlQuery(m_select + m_from), strSql;
		if(qDown == type || qUp == type)
		{
			for(DWORD k = keyFieldsCount(); ;)
			{
				k--;
				strSql += mainSqlQuery + " where\r\n";
				if(!m_where.IsEmpty())
					strSql += m_where + "\r\n and \r\n";
				for(DWORD i = 0; i < k ; i++)
					strSql += fieldInfo(i).expression + " = " + keySQLParamName(i) + " and\r\n";
				
				strSql += fieldInfo(k).expression  + (qDown == type ? " > " : " < ") + keySQLParamName(k);
				if(k)
					strSql += "\r\n union all \r\n";
				else
					break;
			}
		}
		else
		{
			strSql = mainSqlQuery;
			if(qID == type)
			{
				strSql += " where\r\n";
				if(!m_where.IsEmpty())
					strSql += m_where + "\r\n and \r\n";
				strSql += fieldInfo(idFieldPos()).expression + " = " + idParamName;
			}
			else if(qCurRow == type)
			{
				strSql += " where\r\n";
				if(!m_where.IsEmpty())
					strSql += m_where + "\r\n and \r\n";
				for(DWORD idx = 0, midx = keyFieldsCount() -1 ; idx < midx ; idx++)
					strSql += fieldInfo(idx).expression + " = " + keySQLParamName(idx) + " and\r\n";
				strSql += fieldInfo(idx).expression + " = " + keySQLParamName(idx);
			}
			else
			{
				if(!m_where.IsEmpty())
					strSql = strSql + " where\r\n" + m_where;
			}
		}
		if(qID != type && qCurRow != type)
		{
			strSql += "\r\n order by \r\n";
			CString direction(qUp == type || qBottom == type ? " desc" : " asc");
			for(DWORD idx = 0, midx = keyFieldsCount(); idx < midx ; idx++)
			{
				strSql += fieldInfo(idx).expression + direction;
				if(idx < midx - 1)
					strSql += ",\r\n";
			}
			strSql = strSql + "\r\n limit " + rowCountParamName;
		}
		else
			strSql = strSql + "\r\n limit 1";
		query->prepare(strSql);
		pQuery = m_queries[type] = query.release();
	}
	m_sqlParams.applyParamsToQuery(pQuery, type);
	return pQuery;
}

void SQLiteDataProvider::bindKeyValues(SQLiteQuery* pQuery, SQLiteDataRow* pRow)
{
	if(!pRow)
		return;
	for(DWORD idx = 0, midx = keyFieldsCount(); idx < midx ; idx++)
	{
		pQuery->setSqlParam(CValue(keySQLParamName(idx)),
			pRow->m_pValues[idx], modificatorByType(fieldInfo(idx).type));
	}
}

void SQLiteDataProvider::QueryField(LPCSTR szFieldName)
{
	field_info* pFI;
	if(m_fieldByName.Lookup(szFieldName, pFI))
	{
		if(0 == (pFI->flags & field_info::inUse))
		{
			pFI->flags |= field_info::inUse;
			setColumnsChanged();
		}
	}
}

void SQLiteDataProvider::ReleaseField(LPCSTR szFieldName)
{
	field_info* pFI;
	if(m_fieldByName.Lookup(szFieldName, pFI))
	{
		if(0 != (pFI->flags & (field_info::keyField | field_info::idField | field_info::NoAutoDelete)))
			return;
		if(0 != (pFI->flags & field_info::inUse))
		{
			pFI->flags &= ~field_info::inUse;
			setColumnsChanged();
		}
	}
}

void SQLiteDataProvider::setFlagOnFields(const CString& strFields, int flag)
{
	CString notFound;
	CStringArray fields;
	SplitStr2Array(strFields, fields, ',');
	DWORD dNotFound = 0;
	for(DWORD idx = 0, midx = fields.GetSize(); idx < midx; idx++)
	{
		CString strField = fields[idx];
		strField.TrimLeft(trimPattern);
		strField.TrimRight(trimPattern);
		field_info* pFI;
		if(m_fieldByName.Lookup(strField, pFI))
			pFI->flags |= flag;
		else
		{
			dNotFound++;
			notFound += strField + ',';
		}
	}
	if(dNotFound)
	{
		notFound.GetBufferSetLength(notFound.GetLength() - 1);
		CString msg;
		msg.Format(dNotFound == 1 ? "Поле %s не найдено" : "Поля %s не найдены", notFound);
		CBLModule::RaiseExtRuntimeError(msg, FALSE);
	}
}

void SQLiteDataProvider::setColumnsChanged()
{
	if(m_select.IsEmpty())
		return;
	m_select.Empty();
	for(DWORD idx = 0; idx < qLast; idx++)
	{
		delete m_queries[idx];
		m_queries[idx] = NULL;
	}
	m_sqlParams.dirtyQueries = 0xFFFFFFFF;
	OnFieldsChanged();
}

BOOL SQLiteDataProvider::GetQueryText(CValue& retVal, CValue** ppParams)
{
	BOOL bAll = ppParams[0]->GetNumeric() != 0;
	CString text;
	if(fieldsInfoCount())
	{
		text = "select\r\n";
		for(DWORD idx = 0; idx < fieldsInfoCount(); idx++)
		{
			const field_info& fi = fieldInfo(idx);
			if(bAll || 0 != (fi.flags & field_info::inUse))
				text += fi.expression + fi.alias + ",\r\n";
		}
		text.GetBufferSetLength(text.GetLength() - 3);
		text += m_from;
		if(!m_where.IsEmpty())
			text = text + " where\r\n" + m_where;
	}
	retVal = text;
	return TRUE;
}

void DataProviderResultLoader::addValues(CValue** ppValues)
{
	SQLiteDataRow* pRow = new SQLiteDataRow(m_pProvider);
	CValue* pValDst = pRow->m_pValues;
	for(DWORD c = m_count; c--; pValDst++, ppValues++)
	{
		pValDst->CValue::CValue(**ppValues);
		pValDst->m_length = (*ppValues)->m_length;
		pValDst->m_prec = (*ppValues)->m_prec;
	}
	m_pDestination->Add(pRow);
}

void QuickSearchResultLoader::addValues(CValue** ppValues)
{
	CString str = ppValues[m_testField]->Format();
	if(ppValues[m_testField]->type == typeNumber)
		str.TrimLeft();
	DWORD len = 0;
	for(LPCSTR p1 = m_pattern, p2 = str, pStart = p1;;)
	{
		DWORD s1 = CNoCaseMapBase::m_lotable[ncm_symb::symbol(p1)];
		DWORD s2 = CNoCaseMapBase::m_lotable[ncm_symb::symbol(p2)];
		if(s1 != s2 || !s1)
			break;
		p1++;
		p2++;
		len++;
	}
	if(len > m_maxSymbols)
	{
		m_maxSymbols = len;
		for(DWORD i = 0; i < m_keysCount; i++)
			m_pBestKey[i] = *ppValues[i];
		if(len == m_pattern.GetLength())
			throw 1;
	}
}
