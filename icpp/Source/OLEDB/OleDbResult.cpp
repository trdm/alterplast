#include "StdAfx.h"
#include <cstdlib>
#include "OleDbResult.h"
#include "../Utils/ComScopedPtr.h"
#include "../SQL/MetaDataWork.h"

COLEDBFldsInf::COLEDBFldsInf() : 
	m_nType1C(CType(0)), 
	m_Index(0), 
	m_strName(""), 
	m_eExtType1C(eToOther), 
	m_Flags (0),
	m_wType (DBTYPE_EMPTY) {}

namespace OleDb {

size_t const BLOCK_SIZE = 1024;

CResult::CResult (IRowset *pRowset) :
	m_pRowset (pRowset), m_cCols (0), m_cbMaxRowSize (0), m_HasKindField (false),
	m_TempBuffer (BLOCK_SIZE)
{
	if (IsEmpty ()) return;

	Binding ();
	CreateAccessor ();
}
	
void CResult::GetErrorDescription (CString& strError)
{
	HRESULT hr;
	
	IErrorInfo* pIErrorInfo;
	
	try
	{
		hr = ::GetErrorInfo(0, &pIErrorInfo);
		if(SUCCEEDED(hr))
		{
			BSTR errDescr;
			pIErrorInfo->GetDescription(&errDescr);
			
			strError += ": ";
			strError += errDescr;
			
			::SysFreeString(errDescr);
		}
		pIErrorInfo->Release();
	}
	catch(...)
	{
		strError += ": Undefined critical error!";
	}
}

ULONG CResult::AddOffset(ULONG nCurrent, ULONG nAdd)
{
	struct foobar
	{
		char foo;
		long bar;
	};
	
	ULONG nAlign = offsetof(foobar, bar);
	
	return nCurrent + nAdd + (nAdd % nAlign);
}

void CResult::Binding ()
{
	HRESULT hr;
	CString strErr;

	CComPtr<IColumnsInfo> spIColumnsInfo;
	
	hr = m_pRowset->QueryInterface(IID_IColumnsInfo, (void**)&spIColumnsInfo);
	if(FAILED(hr))
	{
		strErr = "FAILED! IRowset::QueryInterface()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return;
	}

	Utils::ComScopedPtr_t<OLECHAR> pColumnStrings;
	Utils::ComScopedPtr_t<DBCOLUMNINFO> pColumnsInfo;

	hr = spIColumnsInfo->GetColumnInfo(&m_cCols, &pColumnsInfo, &pColumnStrings);
	if(FAILED(hr))
	{
		strErr = "FAILED! IColumnsInfo::GetColumnInfo()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return;
	}

	ULONG nDataOffset = 0;
	ULONG nLengthOffset;
	ULONG nStatusOffset;

	m_DbObjects.resize (m_cCols);
	m_DBBindings.resize (m_cCols);
	m_ColumnsInfo.resize (m_cCols);

	Utils::UnicodeToStringConverter_t StringConverter;

	for(ULONG nCol = 0; nCol < m_cCols; ++nCol)
	{
		// заполняем мою структуру
		//
		m_ColumnsInfo[nCol].m_Index	 = nCol;
		m_ColumnsInfo[nCol].m_strName = StringConverter.Convert (
			pColumnsInfo[nCol].pwszName);
		m_ColumnsInfo[nCol].m_wType	 = pColumnsInfo[nCol].wType;
		m_ColumnsInfo[nCol].m_nType1C = GetType1C(&m_ColumnsInfo [nCol]);

		// Для небольшой оптимизации: чтобы не искать при любом виде запроса колонку с названием "XXX_вид"/"XXX_kind"
		//
		if((m_ColumnsInfo[nCol].m_strName.Find("_kind") >= 0) ||
			(m_ColumnsInfo[nCol].m_strName.Find("_вид") >= 0))
			m_HasKindField = true;

		//
		// нужно различать BLOB колонки от обычных.
		// MSDN говорит, что "text" and "ntext" (its for mssql) - 
		// это DBCOLUMNFLAGS_ISLONG = true or Maximum column size > 4,000 characters
		//
		
		if(pColumnsInfo[nCol].dwFlags & DBCOLUMNFLAGS_ISLONG)
		{
			m_ColumnsInfo[nCol].SetIsLong (true);

			m_DBBindings[nCol].pObject	 = &m_DbObjects [nCol];
			DBOBJECT* pLocDBObject		 = m_DBBindings[nCol].pObject;

			pLocDBObject->dwFlags		 = STGM_READ;
			pLocDBObject->iid			 = IID_ISequentialStream;

			m_DBBindings[nCol].wType		 = DBTYPE_IUNKNOWN;
			m_DBBindings[nCol].cbMaxLen	 = sizeof(ISequentialStream*);
		}
		else
		{
			m_ColumnsInfo[nCol].SetIsLong (false);
			
			m_DBBindings[nCol].pObject	 = NULL;

			switch(pColumnsInfo[nCol].wType)
			{
				case DBTYPE_STR:
				case DBTYPE_WSTR:
					{
						m_DBBindings[nCol].wType = pColumnsInfo[nCol].wType;
						m_DBBindings[nCol].cbMaxLen	 = pColumnsInfo[nCol].ulColumnSize +
							sizeof(char);
					}
					break;
				
				case DBTYPE_DBDATE:
				case DBTYPE_DBTIMESTAMP:
					{
						m_DBBindings[nCol].wType		 = pColumnsInfo[nCol].wType;
						m_DBBindings[nCol].cbMaxLen	 = pColumnsInfo[nCol].ulColumnSize;
					}
					break;
				
				default:
					{
						m_DBBindings[nCol].wType		 = DBTYPE_STR;
						m_DBBindings[nCol].cbMaxLen	 = 100;
					}
					break;
			};
		}

		nLengthOffset	 = AddOffset(nDataOffset, m_DBBindings[nCol].cbMaxLen);
		nStatusOffset	 = AddOffset(nLengthOffset, sizeof(ULONG));

		m_DBBindings[nCol].iOrdinal	 = pColumnsInfo[nCol].iOrdinal;
		m_DBBindings[nCol].obValue	 = nDataOffset;
		m_DBBindings[nCol].obLength	 = nLengthOffset;
		m_DBBindings[nCol].obStatus	 = nStatusOffset;
		m_DBBindings[nCol].pTypeInfo	 = NULL;
		m_DBBindings[nCol].pBindExt	 = NULL;
		m_DBBindings[nCol].dwPart	 = DBPART_VALUE | DBPART_STATUS;
		m_DBBindings[nCol].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
		m_DBBindings[nCol].eParamIO	 = DBPARAMIO_NOTPARAM;
		m_DBBindings[nCol].dwFlags	 = 0;
		m_DBBindings[nCol].bPrecision = pColumnsInfo[nCol].bPrecision;
		m_DBBindings[nCol].bScale	 = pColumnsInfo[nCol].bScale;

		nDataOffset = AddOffset(nStatusOffset, sizeof(DBSTATUS));

		int nTypeCode = m_ColumnsInfo[nCol].m_nType1C.GetTypeCode();
		
		switch(nTypeCode)
		{
			case ENUM_TYPE_1C:
			case REFERENCE_TYPE_1C:
			case CALENDAR_TYPE_1C:
			case CALCULATIONKIND_TYPE_1C:
			case ACCOUNT_TYPE_1C:
			case SUBCONTOKIND_TYPE_1C:
			case CHART_OF_ACC_TYPE_1C:
			case UNDEFINE_TYPE_1C:
			case DOCUMENT_TYPE_1C:
				m_ColumnsInfo[nCol].SetIsType1s (true);
				break;
			
			default:
				m_ColumnsInfo[nCol].SetIsType1s (false);
				break;
		}
	}
	
	m_cbMaxRowSize = nDataOffset;
	m_RowBuffer.resize (m_cbMaxRowSize);
}

CType CResult::GetType1C (COLEDBFldsInf* fInfo) const
{
	CDataBase7 *pDataBase = CDataBase7::GetDefault();
	if(pDataBase->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
	{
		int nPos = -1;
		CType rt(0);
		CString strName = fInfo->m_strName;
		
		nPos = strName.Find(TypeNamePrefix);
		if(nPos >= 0)
		{
			CString Type1C((LPCSTR)strName + nPos);
			Type1C.Replace(TypeNamePrefix, "");
			Type1C.Replace(szDotSubst, ".");

			CMetaDataWork::SetTypeAndKind(rt, Type1C);
			
			strName				 = strName.Left(nPos);
			fInfo->m_strName	 = strName;
			
			if(!Type1C.CollateNoCase("Субконто") || !Type1C.CollateNoCase("Subconto"))
				fInfo->m_eExtType1C = eToSb;
			
			else if(!Type1C.CollateNoCase("Документ") || !Type1C.CollateNoCase("Document"))
				fInfo->m_eExtType1C = eToDoc;
			
			else
				fInfo->m_eExtType1C = eToOther;
			
			return rt;
		}
	}
	
	switch(fInfo->m_wType)
	{
	case DBTYPE_STR:
	case DBTYPE_WSTR:
	case DBTYPE_DBTIME:
		return CType(2);
		break;
		
	case DBTYPE_NUMERIC:
	case DBTYPE_I2:
	case DBTYPE_I4:
	case DBTYPE_R4:
	case DBTYPE_R8:
	case DBTYPE_CY:
	case DBTYPE_DECIMAL:
	case DBTYPE_UI1:
	case DBTYPE_BOOL:
	case DBTYPE_I1:
	case DBTYPE_UI2:
	case DBTYPE_UI4:
	case DBTYPE_I8:
	case DBTYPE_UI8:
		return CType(1); // DmitrO говорит, что тип Число с Длина(0).Точность(0) - нормально
		break;
		
	case DBTYPE_DBDATE:
	case DBTYPE_DBTIMESTAMP:
		return CType(3);
		break;
		
	default:
		return CType(0);
		break;
	};
}

void CResult::CreateAccessor ()
{
	CString strErr;
	
	HRESULT hr = m_pRowset->QueryInterface(IID_IAccessor, (void**)&m_pAccessor);
	if(FAILED(hr))
	{
		strErr = "FAILED! IUnknown::QueryInterface()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return;
	}
	
	std::vector<DBBINDSTATUS> DBBindStatus;
	DBBindStatus.resize (m_cCols);

	HACCESSOR hAccessor;
	hr = m_pAccessor->CreateAccessor (DBACCESSOR_ROWDATA, m_cCols,
		&m_DBBindings [0], 0, &hAccessor, &DBBindStatus [0]);
	if(FAILED(hr))
	{
		strErr = "FAILED! IAccessor::CreateAccessor()";
		
		switch(hr)
		{
		case DB_E_BADACCESSORFLAGS:
			strErr += " [One or more accessor flags were invalid]";
			break;
			
		case DB_E_BYREFACCESSORNOTSUPPORTED:
			strErr += " [Reference accessors are not supported by this provider]";
			break;
			
		case DB_E_ERRORSOCCURRED:
			strErr += " [Multiple-step OLE DB operation generated errors]";
			break;
			
		case DB_E_NOTREENTRANT:
			strErr += " [Consumer's event handler called a non-reentrant method in the provider]";
			break;
			
		case DB_E_NULLACCESSORNOTSUPPORTED:
			strErr += " [Null accessors are not supported by this provider]";
			break;
			
		default:
			break;
		}
		
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		
		return;
	}

	m_Accessor.Reset (m_pAccessor, hAccessor);
}

bool CResult::GetFieldValue (ULONG Field, CValue &Value)
{
	if (Field >= m_cCols) return false;

	Value.Reset();
	
	DWORD dwStatus = (ULONG)m_RowBuffer [m_DBBindings[Field].obStatus];
	if(dwStatus == DBSTATUS_S_ISNULL)
	{
		Value.SetType (m_ColumnsInfo[Field].m_nType1C);
		return true;
	}
	
	if(m_ColumnsInfo[Field].GetIsLong ())
	{
		ReadLongField (Field, Value);
		return true;
	}

	CType rt = m_ColumnsInfo[Field].m_nType1C;
	
	switch(m_ColumnsInfo[Field].m_wType)
	{
	case DBTYPE_NUMERIC:
	case DBTYPE_I2:
	case DBTYPE_I4:
	case DBTYPE_R4:
	case DBTYPE_R8:
	case DBTYPE_CY:
	case DBTYPE_DECIMAL:
	case DBTYPE_UI1:
	case DBTYPE_I1:
	case DBTYPE_UI2:
	case DBTYPE_UI4:
	case DBTYPE_I8:
	case DBTYPE_UI8:
		ReadNumericField (Field, Value);
		return true;
	case DBTYPE_BOOL:
		ReadBoolField (Field, Value);
		return true;
	case DBTYPE_DBDATE:
		ReadDateField (Field, Value);
		return true;
	case DBTYPE_DBTIMESTAMP:
		ReadTimestampField (Field, Value);
		return true;
	case DBTYPE_WSTR:
		ReadWideStringField (Field, Value);
		return true;
	default:
		{
			if (m_ColumnsInfo[Field].GetIsType1s ())
			{
				Read1SField(Field, Value);
				return true;
			}
			else
			{
				char* strVal = (char*)&m_RowBuffer[m_DBBindings[Field].obValue];
				
				Value.SetType(m_ColumnsInfo[Field].m_nType1C);
				Value = strVal;
				return true;
			}
		}
	};
}

void CResult::ReadLongField (ULONG Field, CValue &Value)
{
	CComPtr<ISequentialStream> pISequentialStream = (*((ISequentialStream**)
		(&m_RowBuffer [0] + m_DBBindings [Field].obValue)));
	
	CString strTemp;
	ULONG cRead;

	do
	{
		pISequentialStream->Read(&m_TempBuffer [0], BLOCK_SIZE - 1, &cRead);
		
		if(cRead > 0) {
			m_TempBuffer [cRead] = 0;
			strTemp += (const char *)&m_TempBuffer [0];
		}
	} while(cRead >= BLOCK_SIZE - 1);
	
	Value.SetType (m_ColumnsInfo[Field].m_nType1C);
	Value = strTemp;
}

void CResult::ReadNumericField (ULONG Field, CValue &Value)
{
	Value.SetType (m_ColumnsInfo [Field].m_nType1C);
	Value = m_TempNumeric.FromString((const char*)&m_RowBuffer [
		m_DBBindings[Field].obValue], NULL);
}

void CResult::ReadBoolField (ULONG Field, CValue &Value)
{
	char const *strVal = (char const *)&m_RowBuffer [
		m_DBBindings [Field].obValue];
	
	m_TempString = strVal;

	Value.SetType(m_ColumnsInfo[Field].m_nType1C);
	
	if(m_TempString.CompareNoCase("False") == 0) Value = 0L;
	else Value = 1L;
}

void CResult::ReadDateField (ULONG Field, CValue &Value)
{
	DBDATE *dbdateVal = (DBDATE*)&m_RowBuffer [
		m_DBBindings [Field].obValue];
	
	Value.SetType (m_ColumnsInfo[Field].m_nType1C);
	
	if(dbdateVal->year == 1753 && 
		dbdateVal->month == 1 && 
		dbdateVal->day == 1)
	{
		// скваль 2к
		//
		CDate dateVal(0, 0, 0);
		dateVal.m_DateNum = 0;
		
		Value = dateVal;
	}
	else if(dbdateVal->year == 1899 && 
		dbdateVal->month == 12 && 
		dbdateVal->day == 30)
	{
		// фоксовый драйвер/провайдер
		//
		CDate dateVal(0, 0, 0);
		dateVal.m_DateNum = 0;
		
		Value = dateVal;
	}
	else
	{
		CDate dateVal(dbdateVal->year, dbdateVal->month, dbdateVal->day);
		Value = dateVal;
	}
}

void CResult::ReadTimestampField (ULONG Field, CValue &Value)
{
	DBTIMESTAMP*dbdtVal = (DBTIMESTAMP*)&m_RowBuffer [
		m_DBBindings [Field].obValue];
	
	Value.SetType(m_ColumnsInfo [Field].m_nType1C);
	
	if(dbdtVal->year == 1753 && 
		dbdtVal->month == 1 && 
		dbdtVal->day == 1)
	{
		// скваль 2к
		//
		CDate dateVal(0, 0, 0);
		dateVal.m_DateNum = 0;
		
		Value = dateVal;
	}
	else if(dbdtVal->year == 1899 && 
		dbdtVal->month == 12 && 
		dbdtVal->day == 30)
	{
		// фоксовый драйвер
		//
		CDate dateVal(0, 0, 0);
		dateVal.m_DateNum = 0;
		
		Value = dateVal;
	}
	else
	{
		CDate dateVal(dbdtVal->year, dbdtVal->month, dbdtVal->day);
		Value = dateVal;
	}
}

void CResult::ReadWideStringField (ULONG Field, CValue &Value)
{
	wchar_t *wstrVal = (wchar_t*)&m_RowBuffer [
		m_DBBindings [Field].obValue];
	
	Value.SetType(m_ColumnsInfo[Field].m_nType1C);
	Value = m_StringConverter.Convert (wstrVal);
}

void CResult::Read1SField (ULONG Field, CValue &Value)
{
	CString strVal;
	CType LocalType(0);
	CType rt = m_ColumnsInfo[Field].m_nType1C;
	CMetaDataWork::TypeStringValue eType;

	if(rt.GetTypeID() == 0)
	{
		int nTypeCode = rt.GetTypeCode();
		
		switch(nTypeCode)
		{
		case ENUM_TYPE_1C:
		case REFERENCE_TYPE_1C:
		case CALENDAR_TYPE_1C:
		case ACCOUNT_TYPE_1C:
			{
				strVal	 = (char*)&m_RowBuffer [
					m_DBBindings [Field].obValue];
				eType = CMetaDataWork::LongString;
				LocalType = rt;
			}
			break;
		
		case CALCULATIONKIND_TYPE_1C:
			{
				strVal	 = (char*)&m_RowBuffer [
					m_DBBindings[Field].obValue];

				int nLen = strVal.GetLength();
				if (nLen == 13)
				{
					CString sLocVal(strVal.Mid(4, 6));
					sLocVal.TrimLeft();
					sLocVal.TrimRight();
					strVal = sLocVal;
				}

				eType = CMetaDataWork::ShortString;

				LocalType = rt;

				Value.SetType(rt);
				Value.SetObjID(CObjID(strtol(strVal, NULL, 36), CDBSign("   ")));
			}
			break;
		
		case DOCUMENT_TYPE_1C:
		case UNDEFINE_TYPE_1C:
			{
				ULONG nKindCol = 0;

				ExtTp1C eExtType1C = m_ColumnsInfo[Field].m_eExtType1C;
				
				if (m_HasKindField)
					nKindCol = FindKindCol (m_ColumnsInfo[Field].m_strName);

				if(!nKindCol)
				{
					strVal	 = (char*)&m_RowBuffer [
						m_DBBindings[Field].obValue];
					if(UNDEFINE_TYPE_1C == nTypeCode)
						eType= CMetaDataWork::VeryLongString;
					else
						eType= CMetaDataWork::LongString;

					LocalType= rt;
				}
				else
				{
					CString strValLoc	 = (char*)&m_RowBuffer [
						m_DBBindings[Field].obValue];
					CString strKindVal	 = (char*)&m_RowBuffer [
						m_DBBindings[nKindCol - 1].obValue];

					if(eToSb == eExtType1C)
					{
						CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
						if(pBuhDef)
						{
							CString strKindID(strKindVal);
							strKindID.TrimLeft();
							strKindID.TrimRight();
							
							long nKindID = strtol(strKindID, NULL, 36);
							
							CSbKindDef* pSbKindDef = pBuhDef->GetSbKindDefs()->GetItem(nKindID);
							if(pSbKindDef)
							{
								LocalType= CType(pSbKindDef->m_TypeCode, pSbKindDef->m_Kind);
								strVal	 = strValLoc;
								eType	 = pSbKindDef->m_Kind ? CMetaDataWork::ShortString : CMetaDataWork::LongString;
							}
						}
					}
					else // eToDoc
					{
						strVal	 = strKindVal + strValLoc;
						eType	 = CMetaDataWork::LongString;
						LocalType= rt;
					}
				}
			}
			break;
		
		default:
			{
				strVal	 = (char*)&m_RowBuffer [
					m_DBBindings[Field].obValue];
				eType	 = CMetaDataWork::ShortString;

				LocalType = rt;
			}
			break;
		}
	}
	else
	{
		strVal	 = (char*)&m_RowBuffer [m_DBBindings[Field].obValue];
		eType	 = CMetaDataWork::ShortString;

		LocalType = rt;
	}
	
	if(CALCULATIONKIND_TYPE_1C != rt.GetTypeCode())
		CMetaDataWork::Make1C_ValueFromString(Value, LocalType, strVal, eType);
}

ULONG CResult::FindKindCol (const CString& strColName) const
{
	CString tmpNameRus(strColName);
	tmpNameRus += "_вид";
	
	CString tmpNameEng(strColName);
	tmpNameEng += "_kind";
	
	for(ULONG cCol = 0; cCol < m_cCols; cCol++)
	{
		int nRus = tmpNameRus.CompareNoCase(m_ColumnsInfo[cCol].m_strName);
		int nEng = tmpNameEng.CompareNoCase(m_ColumnsInfo[cCol].m_strName);
		
		if((0 == nRus) || (0 == nEng))
			return cCol + 1;
	}
	
	return 0;
}

ULONG CResult::GetNumFields() const
{
	return m_cCols;
}

CString const &CResult::GetFieldName (ULONG Field) const
{
	ASSERT (Field < m_cCols);
	return m_ColumnsInfo[Field].m_strName;
}

CType const &CResult::GetFieldType (ULONG Field) const
{
	ASSERT (Field < m_cCols);
	return m_ColumnsInfo[Field].m_nType1C;
}

bool CResult::IsEmpty () const
{
	return m_pRowset == NULL;
}

CResult::~CResult ()
{
	IRowset *pRowset = m_pRowset;

	UINT Refs;
	if (pRowset) {
		Refs = pRowset->AddRef();
		Refs = pRowset->Release();
	}
}

CResultSequentialAccess::CResultSequentialAccess (IRowset *pRowset, UINT RowsChunk) :
CResult (pRowset), m_RowsChunk (RowsChunk), m_RowsHolder (pRowset, RowsChunk)
{
}

bool CResultSequentialAccess::GetNextRow ()
{
	HROW hRow = m_RowsHolder.GetRow ();

	if (!hRow) {
		ULONG cRowsObtained = 0;
		HROW* phRow = m_RowsHolder.Get ();
		ASSERT (phRow);
		
		HRESULT hr = m_pRowset->GetNextRows (NULL, 0, m_RowsChunk, &cRowsObtained,
			&phRow);
		if(FAILED(hr))
		{
			CString strErr = "FAILED! IAccessor::GetNextRows()";
			GetErrorDescription(strErr);
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return false;
		}
		
		if(cRowsObtained == 0) return false;
		m_RowsHolder.SetObtainedRows (cRowsObtained);
		hRow = m_RowsHolder.GetRow ();
		if (!hRow) return false;
	}

	HRESULT hr = m_pRowset->GetData (hRow, m_Accessor.Get(), &m_RowBuffer [0]);
	if(FAILED(hr))
	{
		CString strErr = "FAILED! IRowset::GetData()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return false;
	}
	
	return true;
}

CResultDirectAccess::CResultDirectAccess (IRowset *pRowset) :
CResult (pRowset)
{
}

bool CResultDirectAccess::GetRow (ULONG Row)
{
	ULONG cRowsObtained = 0;
	RowsHolder_t RowsHolder (m_pRowset, 1);
	HROW* phRow = RowsHolder.Get ();
	
	HRESULT hr = m_pRowset->RestartPosition (DB_NULL_HCHAPTER);
	if(FAILED(hr))
	{
		CString strErr = "FAILED! IRowset::RestartPosition()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return false;
	}

	hr = m_pRowset->GetNextRows (DB_NULL_HCHAPTER, Row, 1, &cRowsObtained, &phRow);
	if(FAILED(hr))
	{
		CString strErr = "FAILED! IAccessor::GetNextRows()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return false;
	}
	
	if(cRowsObtained == 0) return false;
	RowsHolder.SetObtainedRows (cRowsObtained);
	HROW hRow = RowsHolder.GetRow ();
	if (!hRow) return false;
	
	hr = m_pRowset->GetData (hRow, m_Accessor.Get (), &m_RowBuffer [0]);
	if(FAILED(hr))
	{
		CString strErr = "FAILED! IRowset::GetData()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return false;
	}
	
	return true;
}

} // namespace OleDb
