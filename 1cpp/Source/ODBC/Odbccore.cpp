//  MODULE:   odbccore.cpp
//
//	AUTHOR: Carlos Antollini 
//
//  mailto: cantollini@hotmail.com
//
//	Date: 08/21/2001
//
//	Version 1.11
// 

#include "stdafx.h"
#include "odbccore.h"
//#include "global.h"
#include "..\\blptr.h"
#include "..\\GUID.h"
#include "..\\SQL\\MetaDataWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMetaDataCont * pMetaDataCont;
extern CDataBase7 * pDataBase7;
extern CIBSet NativeIBSet;

int CODBCRecordset::pm_named = 0;
int CODBCRecordset::pm_pos   = 1;

#define FREE_POINTER(p) if (p) { free(p); p=NULL;};

BOOL Check(RETCODE nRetCode)
{
	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	//case SQL_NO_DATA_FOUND:
//	case SQL_NEED_DATA:
		return TRUE;
	}

	return FALSE;
}

short GetDefaultFieldType(short nSQLType)
{
	short nFieldType = 0;

	switch (nSQLType)
	{
	case SQL_BIT:
		nFieldType = SQL_C_BIT;
		break;

	case SQL_TINYINT:
		nFieldType = SQL_C_UTINYINT;
		break;

	case SQL_SMALLINT:
		nFieldType = SQL_C_SSHORT;
		break;

	case SQL_INTEGER:
		nFieldType = SQL_C_SLONG;
		break;

	case SQL_REAL:
		nFieldType = SQL_C_FLOAT;
		break;

	case SQL_FLOAT:
	case SQL_DOUBLE:
  case SQL_NUMERIC:
		nFieldType = SQL_C_DOUBLE;
		break;

	case SQL_TYPE_DATE:
	case SQL_TYPE_TIMESTAMP:
		nFieldType = SQL_C_TIMESTAMP;
		break;

	case SQL_DECIMAL:
	case SQL_BIGINT:
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
		nFieldType = SQL_C_CHAR;
		break;

	case SQL_WCHAR:
	case SQL_WVARCHAR:
	case SQL_WLONGVARCHAR:
		nFieldType = SQL_C_CHAR;
		break;

	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		nFieldType = SQL_C_BINARY;
		break;
//DmitrO
	case SQL_TYPE_TIME:
        nFieldType = SQL_C_TYPE_TIME;
		break;
//
	default:
		ASSERT(FALSE);
	}

	return nFieldType;
}

/////////////////////////////////////////////////////////////
//
// CODBCDatabase Class
//

void CODBCDatabase::SQLAlloc()
{
	if (!m_bIsAttached)
	{
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
		SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
		SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
	}
}

void CODBCDatabase::SQLFree()
{
	if (!m_bIsAttached)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	}
	m_hDbc = NULL;
	m_hEnv = NULL;
}

CODBCDatabase::CODBCDatabase()
{
	m_bIsAttached = FALSE;
	m_hDbc = NULL;
	m_hEnv = NULL;
	m_lConnectionTimeout = 0;
	m_lLoginTimeout = 5;
	m_bIsConnected = FALSE;
//	m_pCurrent = 0;
//DmitrO
    memset(m_DriverODBCVer, ' ', 5);
    m_DriverODBCVer[5] = 0;
//
//trad
    m_pMetaDataCont = pMetaDataCont;
    m_pCurrentIBSet = &NativeIBSet;
//
}

CODBCDatabase::CODBCDatabase(SQLHDBC hDbc)
{
	m_bIsAttached = TRUE;
	m_hDbc = hDbc;
	m_hEnv = NULL;
	m_lConnectionTimeout = 0;
	m_lLoginTimeout = 5;
	m_bIsConnected = FALSE;
//	m_pCurrent = 0;
//DmitrO
    memset(m_DriverODBCVer, ' ', 5);
    m_DriverODBCVer[5] = 0;
//
//trad
    m_pMetaDataCont = pMetaDataCont;
    m_pCurrentIBSet = &NativeIBSet;
//
}

BOOL CODBCDatabase::Attach(SQLHDBC hDbc)
{
	Close();
	m_bIsAttached = TRUE;
	m_hDbc = hDbc;
	m_bIsConnected = m_hDbc != NULL;
//DmitrO
    GetDriverInfo();
//
	return m_bIsConnected;
}

CODBCDatabase::~CODBCDatabase()
{
	Close();
	m_lConnectionTimeout = 0;
	m_lLoginTimeout = 0;
}

void CODBCDatabase::Close()
{
	if (m_bIsConnected)
	{
//		if (m_pCurrent) m_pCurrent->Close(TRUE); // закрыть текущий набор
//		m_pCurrent = 0;
		m_bIsConnected = FALSE;
	}
	if(m_hDbc == NULL)
  {
    m_bIsAttached = FALSE;
		return;
  }
	if (!m_bIsAttached)
  {    
    m_bIsAttached = FALSE;
		SQLDisconnect(m_hDbc);
  }
	SQLFree();
    CMetaDataWork::UnloadMD(m_pMetaDataCont);
    SetIBSet(&NativeIBSet);
}

void CODBCDatabase::SetIBSet(CIBSet* pIBSet)
{
    if (m_pCurrentIBSet && m_pCurrentIBSet != &NativeIBSet)
        delete m_pCurrentIBSet;
    m_pCurrentIBSet = pIBSet;
}

BOOL CODBCDatabase::Open(const char* szDSN,CHAR* szUser, CHAR* szPass)
{
	Close(); // закрыть если открыто
	SQLAlloc();

	SQLRETURN ret;

	//if (!GlobalData::m_bDBF)
	{
		if(m_lConnectionTimeout > 0)
			SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)m_lConnectionTimeout, 0);
			
		
		SQLSetConnectAttr(m_hDbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)m_lLoginTimeout, 0);
	}

	ret = SQLConnect(m_hDbc, 
					(SQLCHAR*)szDSN, 
					sizeof(szDSN), 
					(SQLCHAR*)szUser, 
					sizeof(szUser), 
					(SQLCHAR*)szPass, 
					sizeof(szPass));

    m_bIsConnected = Check(ret);
//DmitrO
    GetDriverInfo();
	return m_bIsConnected;
//
}

void CODBCDatabase::SetConnectionTimeout(LONG nSeconds)
{
	if(m_hDbc)
		SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)nSeconds, 0);

	m_lConnectionTimeout = nSeconds;
}

BOOL CODBCDatabase::DriverConnect(const char* szConnStr, CHAR* szConnStrOut, HWND hWnd, enum drvCompletion drvConn)
{
	Close(); // закрыть если открыто
	SQLAlloc();

	m_sLastError.Empty();

	SQLRETURN ret;
	SQLSMALLINT pcbConnStrOut;

	if(drvConn == sqlPrompt && hWnd == NULL)
		return FALSE;

	//if (!GlobalData::m_bDBF)
	{
		if(m_lConnectionTimeout > 0)
			SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)m_lConnectionTimeout, 0);
		
		SQLSetConnectAttr(m_hDbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)m_lLoginTimeout, 0);
	}
	
	ret = SQLDriverConnect(m_hDbc,
							hWnd, 
							(SQLCHAR*)szConnStr, 
							SQL_NTS, 
							(SQLCHAR*)szConnStrOut,
							sizeof(szConnStrOut), 
							&pcbConnStrOut, 
							(SQLUSMALLINT)drvConn);
  
  if (SQL_INVALID_HANDLE == ret)
    pBkEndUI->DoMessageLine("SQL_INVALID_HANDLE", mmExclamation);	
	
	m_bIsConnected = Check(ret);
//DmitrO
    GetDriverInfo();
	return m_bIsConnected;
//
}

void CODBCDatabase::SetReadOnly(BOOL bReadOnly)
{
	SQLSetConnectAttr(m_hDbc, SQL_ATTR_ACCESS_MODE, (SQLPOINTER)(bReadOnly? SQL_MODE_READ_ONLY : SQL_MODE_READ_WRITE), 0);
}

LONG CODBCDatabase::GetConnectionTimeout()
{
	LONG nSeconds;

	SQLGetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, &nSeconds, NULL, 0);

	return nSeconds;
}

const char* CODBCDatabase::GetLastError()
{
	if (m_sLastError.IsEmpty())
	{
		SQLCHAR state[6];
		SQLINTEGER native;
		CString sMsg;
		SQLCHAR* pchMsg = (SQLCHAR*)sMsg.GetBufferSetLength(256);
		SQLSMALLINT nLen;

		if (Check(SQLGetDiagRec(
			SQL_HANDLE_DBC,
			m_hDbc,
			1,
			state,
			&native,
			pchMsg,
			256,
			&nLen)))
		{
			m_sLastError.Format("State %5s, native %u, message %s", state, native, pchMsg);
		}
	}
	return m_sLastError;
}
/*
void CODBCDatabase::SetCurrent(CODBCRecordset* pCurrent)
{
	//if (m_pCurrent) m_pCurrent->Close();
//	m_pCurrent = pCurrent;
}

void CODBCDatabase::ResetCurrent()
{
//	m_pCurrent = 0;
}
*/
void CODBCDatabase::GetDriverInfo()
{
    if (m_bIsConnected)
    {
        SQLSMALLINT Len;
        SQLGetInfo(m_hDbc, SQL_DRIVER_ODBC_VER, m_DriverODBCVer, 6, &Len);
        char AtrBuf[4];
        SQLGetInfo(m_hDbc, SQL_NEED_LONG_DATA_LEN, AtrBuf, 4, &Len);
        m_NeedLongDataLen = AtrBuf[0] == 'Y';
	};
}

/////////////////////////////////////////////////////////////
//
// CODBCRecordset Class
//
BOOL CODBCRecordset::AllocStmt()
{
  BOOL ret = FALSE;
  BOOL ret2 = TRUE;  

  ret |= Check(SQLAllocHandle(SQL_HANDLE_STMT, m_pDb->m_hDbc, &m_hStmt));

  if (ret && m_isSetType)
  {
    ret2 |= Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_SCROLLABLE,SQL_NTS)) && 
    Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_TYPE,(SQLPOINTER)m_CursorType,SQL_NTS));
    m_isSetType = false;
  }
  if (ret && m_isSetCursorsScrollable)
  {
    ret2 |= Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)m_CursorScrollable,SQL_NTS));
    m_isSetCursorsScrollable = false;
  }
  if(ret && m_isSetCursorsSensitivity)
  {
    ret2 |= Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SENSITIVITY,(SQLPOINTER)m_CursorSensitivity,SQL_NTS));
    m_isSetCursorsSensitivity = false;
  }
  if (ret && m_isSetCursorConcurrency)
  {
    ret2 |= Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CONCURRENCY, (SQLPOINTER)m_CursorConcurrency,SQL_NTS));
    m_isSetCursorConcurrency = false;
  }
  
//DmitrO
  if (ret)
      ret2 |= Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_QUERY_TIMEOUT,(SQLPOINTER)m_QueryTimeout,SQL_NTS));
  m_IsQueryTimeoutExpired = false;
//
	return ret && ret2;
}

void CODBCRecordset::Fixup()
{
    for (int i = 0; i < m_nResultCols; i++)
    {
        CDBField& field = m_rgFields[i];
        if (field.IsLongSQLType() && !m_bIsEOF && !m_bIsBOF)
            field.GetLongData(i + 1);
        field.Fixup();
    }
	if (m_nResultCols > 0 && m_bIsEOF)
		FixupParams();
}

void CODBCRecordset::FixupParams()
{
	for (int i=0;i<m_aParams.GetSize();i++)
	{
		CQueryParam& param = m_aParams[i];
		if (param.m_nIOtype != SQL_PARAM_INPUT)
		{
			param.Fixup();
		}
	}
}

CODBCRecordset::CODBCRecordset()
{
	m_pDb = 0;
	m_hStmt = NULL;
	m_bIsEOF = FALSE;
	m_bIsBOF = FALSE;
	m_bIsOpen = FALSE;
	m_rgFields = NULL;
	m_nResultCols = 0;
	m_nRowsAffected = 0;
	//
	m_nRowsetSize = 0;
	m_nRowSize = 0;
	m_pRowstatus = NULL;
	m_pRowset = NULL;
	m_pOffsets = NULL;
  m_isSetType = false;
  m_isSetCursorsScrollable = false;
  m_isSetCursorsSensitivity = false;
//DmitrO
  m_isSetCursorConcurrency = false;
//
  m_nRowsetSizeForParams = 0;
  m_nAllocRowSizeForParams = 0;
//DmitrO
  m_QueryTimeout = 0;
  m_pBlobBuffer = NULL;
//
}

CODBCRecordset::CODBCRecordset(CODBCDatabase* pDb)
{
	m_hStmt = NULL;
	m_bIsEOF = FALSE;
	m_bIsBOF = FALSE;
	m_bIsOpen = FALSE;
	m_rgFields = NULL;
	m_nResultCols = 0;
	m_nRowsAffected = 0;
	//
	m_nRowsetSize = 0;
	m_nRowSize = 0;
	m_pRowstatus = NULL;
	m_pRowset = NULL;
	m_pOffsets = NULL;
  m_isSetType = false;
  m_isSetCursorsScrollable = false;
  m_isSetCursorsSensitivity = false;
  m_isSetCursorConcurrency = false;

  m_nRowsetSizeForParams = 0;
  m_nAllocRowSizeForParams = 0;
//DmitrO
  m_QueryTimeout = 0;
  m_pBlobBuffer = NULL;
//

	SetDatabase(pDb);
}

void CODBCRecordset::SetDatabase(CODBCDatabase* pDb)
{
	if (m_pDb != pDb)
	{
		//if (pDb) pDb->SetCurrent(this); // закрыть другой набор этого подключения
		//if (m_bIsOpen) m_pDb->SetCurrent(0); // закрыть себя в другом подключении
		m_pDb = pDb;
        if (m_pDb)
            m_nDriverODBCMajorVer = atoi(m_pDb->GetDriverODBCVer());
	}
}

const char* CODBCRecordset::GetLastError()
{
	SQLCHAR state[5];
	memset(state,' ',5);
	SQLINTEGER native=0;
	CString sMsg;
	SQLCHAR* pchMsg = (SQLCHAR*)sMsg.GetBufferSetLength(256);
	SQLSMALLINT nLen;

	if (Check(SQLGetDiagRec(
		SQL_HANDLE_STMT,
		m_hStmt,
		1,
		state,
		&native,
		pchMsg,
		256,
		&nLen)))
	{
		m_sLastError.Format("State %5s, native %u, message %s",state,native,pchMsg);

//DmitrO
        m_IsQueryTimeoutExpired = memcmp(state, "HYT00", 5) == 0;
//
	}
	return m_sLastError;
}

void CODBCRecordset::SetCursorsTypes(SQLUINTEGER CursorType)
{
  m_isSetType = true;
  m_CursorType = CursorType;
}
void CODBCRecordset::SetCursorsScrollable(SQLUINTEGER CursorScrollable)
{
  m_isSetCursorsScrollable = true;
  m_CursorScrollable = CursorScrollable;
}

void CODBCRecordset::SetCursorsSensitivity(SQLUINTEGER CursorSensitivity)
{
  m_isSetCursorsSensitivity = true;
  m_CursorSensitivity = CursorSensitivity;
}

void CODBCRecordset::SetCursorConcurrency(SQLUINTEGER CursorConcurrency)
{
  m_isSetCursorConcurrency = true;
  m_CursorConcurrency = CursorConcurrency;
}

BOOL CODBCRecordset::Open(const char *szSqlStr,BOOL bScroll,BOOL bPrep)
{
	if (!bPrep)
	{
		m_sLastError.Empty();
		if (m_bIsOpen) 
    {
      
      Close();
    }

		if (!AllocStmt())
		{
			return FALSE;
		}
	}
	else
	{
		if (m_bIsOpen && m_hStmt)
		{
			if (!Check(SQLFreeStmt(m_hStmt, SQL_CLOSE)))
			{
				return  FALSE;
			}
		}
	}

	if (!BindParams())
	{
		return  FALSE;
	}

	SQLRETURN ret;
	if (bScroll)
	{
		if (!Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_SCROLLABLE,SQL_NTS)))
			return FALSE;
	}

	if (!bPrep)
	{
		ret = SQLExecDirect(m_hStmt, (SQLCHAR*)szSqlStr, SQL_NTS);
	}
	else
	{
		ret = SQLExecute(m_hStmt);
	}

    ret = ProcessDataAtExec(ret);

	if(Check(ret))
	{
		m_bIsOpen = CreateFields();
	    if (m_bIsOpen)
		{
            if (m_nResultCols == 0)
            {
        	    SQLINTEGER nRowCount;
    		    SQLRowCount(m_hStmt, &nRowCount);
                m_nRowsAffected = nRowCount;
			FixupParams();
                return FALSE; //for backward compatibility
		}
            else
      return MoveNext();
		}

		return m_bIsOpen;
	}

	if (ret == SQL_NO_DATA_FOUND)
        m_nRowsAffected = 0;

	return FALSE;
}

SQLINTEGER CODBCRecordset::Execute(const char* szSqlStr,BOOL bPrep)
{
	if (!bPrep)
	{
		m_sLastError.Empty();
		if (m_bIsOpen) 
    {

      Close();
    }

		if (!AllocStmt())
		{
			return FALSE;
		}
	}
	else
	{
		if (m_bIsOpen && m_hStmt)
		{
			if (!Check(SQLCancel(m_hStmt)))
			{
				return  FALSE;
			}
		}
	}

	SQLRETURN ret;
	SQLINTEGER nRowCount;

	if (!BindParams())
	{
		return  FALSE;
	}
	if (!bPrep)
	{
		ret = SQLExecDirect(m_hStmt, (SQLCHAR*)szSqlStr, SQL_NTS);
	}
	else
	{    
		ret = SQLExecute(m_hStmt);
	}

    ret = ProcessDataAtExec(ret);

	if (ret == SQL_NO_DATA_FOUND)
    {
		m_nRowsAffected = 0;
        return TRUE;
    };

	if (Check(ret))
	{
		SQLRowCount(m_hStmt, &nRowCount);
		m_nRowsAffected = nRowCount;
		FixupParams();
		m_bIsOpen = TRUE;
		return TRUE;
	}
  else
    return FALSE;
}

SQLRETURN CODBCRecordset::ProcessDataAtExec(SQLRETURN ret)
{
    while (ret == SQL_NEED_DATA)
    {
        SQLPOINTER ParamValue;
        ret = SQLParamData(m_hStmt, &ParamValue);
        if (ret == SQL_NEED_DATA)
        {
            int n;
            void* pValuePtr;

            if (m_nRowsetSizeForParams > 0)
            {
                pValuePtr = ParamValue;
                n = 0;
                int offset = ((unsigned char*)ParamValue - m_pRowset) % m_nRowSize;
                for (int i = 0; i < m_nRowsetSize; i++)
                    if (m_pOffsets[i] == offset)
                    {
                        n = i + 1;
                        break;
                    }
                if (!n)
                    break;
            }
            else
            {
                n = (int)ParamValue;
                pValuePtr = NULL;
            }

            CQueryParam &param = m_aParams[n-1];
            if (!param.PutLongData(n, pValuePtr))
                break;
        }
    };
    return ret;
}

BOOL CODBCRecordset::Prepare(const char* szSqlStr)
{
	m_sLastError.Empty();
	if (m_bIsOpen) 
  {
    
    Close();
  }

	if (!AllocStmt())
	{
		return FALSE;
	}
	return Check(SQLPrepare(m_hStmt,(UCHAR*)szSqlStr,SQL_NTS));
}

int CODBCRecordset::GetFieldIndex(const char *szFieldName) const
{
	/*for (int i=0;i < m_nResultCols; i++)
	{
		if (stricmp(szFieldName,m_rgFields[i].m_strName) == 0)
			return i;
	}*/
  int ret = -1;
  void *p;
  if (mapName_IndexFields.Lookup(szFieldName, (void*&)p))
    ret = (int)p;

	return ret;
}

BOOL CODBCRecordset::MoveFirst()
{
	SQLRETURN ret;
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_FIRST, 0);
		
	m_bIsBOF = Check(ret);
	if(m_bIsBOF)
		m_bIsEOF = FALSE;

	Fixup();

	return m_bIsBOF;
}

BOOL CODBCRecordset::MoveNext()
{
	SQLRETURN ret;
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_NEXT, 0);
	
	m_bIsEOF = ret == SQL_NO_DATA;
	m_bIsBOF = FALSE;

	if (m_nRowsetSize < 1) Fixup();

	return Check(ret);
}

BOOL CODBCRecordset::MovePrevious()
{
	SQLRETURN ret;
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_PRIOR, 0);

	m_bIsBOF = ret == SQL_NO_DATA;
	m_bIsEOF = FALSE;

	Fixup();

	return Check(ret);
}

BOOL CODBCRecordset::MoveLast()
{
	SQLRETURN ret;
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_LAST, 0);

	m_bIsEOF = Check(ret);
	if(m_bIsEOF)
		m_bIsBOF = FALSE;
	
	Fixup();

	return m_bIsEOF;
}

void CODBCRecordset::SetRowsetSize(int nRows)
{
	if (!m_bIsOpen && m_nRowsetSize != nRows)
	{
		if (m_pRowset != NULL) free(m_pRowset);
		if (m_pRowstatus != NULL) free(m_pRowstatus);
    m_pRowset    = NULL;
    m_pRowstatus = NULL;
		m_nRowsetSize = nRows;
		if (m_nRowsetSize > 1)
		{
			m_pRowstatus = (BYTE*)malloc(nRows*sizeof(SQLUSMALLINT));
		}
		
	}
}

BOOL CODBCRecordset::SetRowsetSizeForParams(int nRows)
{
	if (!m_bIsOpen && m_nRowsetSizeForParams != nRows)
	{
		if (m_pRowset != NULL) free(m_pRowset);
		if (m_pRowstatus != NULL) free(m_pRowstatus);
    m_pRowset    = NULL;
    m_pRowstatus = NULL;
		m_nRowsetSizeForParams = nRows;
		
    if (m_nRowsetSizeForParams > 1)
		{
			m_pRowstatus = (BYTE*)malloc(nRows*sizeof(SQLUSMALLINT));      
		}
//DmitrO
		return AllocateRowsetForParams();
//
	}
    return TRUE;
}

void CODBCRecordset::ResizeRowsetSizeForParams(int nRows)
{
  if (m_nRowsetSizeForParams != nRows)
  {
    SQLRETURN ret;
    if (m_nAllocRowSizeForParams > nRows)
    {
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (void*)nRows, 0);
      m_nRowsetSizeForParams = nRows;
    }
    else
    {
      if (m_pRowset != NULL) free(m_pRowset);
		  if (m_pRowstatus != NULL) free(m_pRowstatus);
      m_pRowset    = NULL;
      m_pRowstatus = NULL;

      m_pRowstatus = (BYTE*)malloc(nRows*sizeof(SQLUSMALLINT));
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_STATUS_PTR, m_pRowstatus, 0);      
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (void*)nRows, 0);
      m_nRowsetSizeForParams = nRows;
      if (Check(ret))
      {
        BuildRowsetForParams();
      }
    }
  }
}

void CODBCRecordset::Close(BOOL bDetachBase)
{
	/*
  m_isSetType = false;
  m_isSetCursorsScrollable = false;
  m_isSetCursorsSensitivity = false;
  */

  if (m_rgFields != NULL)
	{
		delete[] m_rgFields;
		m_rgFields = NULL;
	}
	m_aParams.SetSize(0);

	FREE_POINTER(m_pRowset);
	FREE_POINTER(m_pRowstatus);
	FREE_POINTER(m_pOffsets);

	m_nRowsetSize = 0;
	m_nRowSize = 0;
  m_nRowsetSizeForParams = 0;
  m_nAllocRowSizeForParams = 0;

	if(m_hStmt != NULL)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = NULL;
	}
	
	m_bIsEOF = FALSE;
	m_bIsBOF = FALSE;
	m_bIsOpen = FALSE;
	m_nResultCols = 0;
	if (bDetachBase && m_pDb)
	{
		//m_pDb->ResetCurrent();
		m_pDb = 0;
	}

    if (m_pBlobBuffer)
    {
        free(m_pBlobBuffer);
        m_pBlobBuffer = NULL;
    }
}

BOOL CODBCRecordset::CreateFields()
{
	// Cache the number of result columns
  if (m_bIsOpen)
    return TRUE;

	SQLRETURN ret = SQLNumResultCols(m_hStmt, &m_nResultCols);
	if (!Check(ret))
	{
		return FALSE;
	}

	// If there are no fields quit now
	if (m_nResultCols == 0)
		return m_bIsOpen = TRUE;

	// Allocate buffer and get the ODBC meta data
	if (m_rgFields) 
  {
    delete[] m_rgFields;    
  }

	m_rgFields = new CDBField[m_nResultCols];

    BOOL bToMap = InternalMapFields();
    if (bToMap)
    {
        mapName_IndexFields.RemoveAll();
        mapName_IndexFields.InitHashTable(static_cast<UINT>(m_nResultCols*1.2));
    }

	// Get the field info each field
	BOOL bRowset = m_nRowsetSize > 1;
	if (bRowset)
	{
		m_nRowSize = 0;
		if (m_pOffsets) free(m_pOffsets);
		m_pOffsets = (int*)malloc(m_nResultCols*sizeof(int));
	}

    for (WORD i = 0; i < m_nResultCols; i++)
	{
		CDBField& dbf = m_rgFields[i];
        dbf.m_pRS = this;
        
		if (!dbf.BindToColumn(i + 1, NULL))
			return FALSE;

        if (!TypifyField(i, dbf))
            return FALSE;
		
        if (bToMap)
            mapName_IndexFields[dbf.m_strName] = (void*)(i);

		if (bRowset)
		{
			m_pOffsets[i] = m_nRowSize;
			m_nRowSize += dbf.m_nBufSize;
			m_nRowSize += sizeof(SQLINTEGER);
		}

	}

	if (bRowset)
	{
		ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_BIND_TYPE, (void*)m_nRowSize, 0);
		ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_ARRAY_SIZE, (void*)m_nRowsetSize, 0);
		ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROW_STATUS_PTR, m_pRowstatus, 0);
		ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_ROWS_FETCHED_PTR, &m_nRowFetched, 0);

		if (!Check(ret)) return FALSE;

		if (m_pRowset) free(m_pRowset);
		m_pRowset = (BYTE*)malloc(m_nRowSize*m_nRowsetSize);
		BYTE* pRowset = m_pRowset;
		int offset = 0;
		for (WORD n = 1; n <= m_nResultCols; n++)
		{
			CDBField& dbf = m_rgFields[n-1];
			
			BYTE* pNew = pRowset + dbf.m_nBufSize;
			*(SQLINTEGER*)pNew = dbf.m_nLength;

			if (!dbf.BindToColumn(n, pRowset))
			{
				return FALSE;
			}
			pRowset = pNew + sizeof(SQLINTEGER);
		}
	}

    if (!PrepareFields())
        return FALSE;

	return TRUE;
}

BOOL CODBCRecordset::CreateFieldsMapped(short nBindCount,int aMap[])
{
	// Cache the number of result columns
	SWORD nResultCols,nField;
	SQLRETURN ret = SQLNumResultCols(m_hStmt, &nResultCols);
	if (!Check(ret))
	{
		return FALSE;
	}

	m_nResultCols = nResultCols;
	if (m_nResultCols == 0)
		return m_bIsOpen = TRUE;

	m_nResultCols = nBindCount;
	if (m_rgFields) 
  {    
    delete[] m_rgFields;
  }
	m_rgFields = new CDBField[m_nResultCols];
  mapName_IndexFields.RemoveAll();
  mapName_IndexFields.InitHashTable(static_cast<UINT>(m_nResultCols*1.2));

	for (WORD n = 0; n < nBindCount; n++)
	{
		nField = aMap[n];
		if (nField < 1 || nField > nResultCols)
		{
			return FALSE;
		}
        m_rgFields[n].m_pRS = this;
        
		if (!m_rgFields[n].BindToColumn(nField, NULL))
		{
			return FALSE;
		}
    mapName_IndexFields[m_rgFields[n].m_strName] = (void*)(n);
	}
	return TRUE;
}

SQLHSTMT CODBCRecordset::AttachRecordset(CODBCDatabase* pDb)
{
	//pDb->SetCurrent(this);
	m_pDb = pDb;
	if (m_pDb)
		m_nDriverODBCMajorVer = atoi(m_pDb->GetDriverODBCVer());

	/*if (!AllocStmt())
	{
		return NULL;
	}*/
	return m_hStmt;
}

BOOL CODBCRecordset::BindColumns(int nBindCount,int aMap[])
{
	if (nBindCount == -1)
	{
		return CreateFields();
	}
	return CreateFieldsMapped(nBindCount,aMap);
}

/*BOOL CODBCRecordset::BuildParamsSP(SPQualif& spq,int nParams,param_list* params)
{
	if (nParams > 0)
	{
		CODBCRecordset* pCurrent = m_pDb->GetCurrent();
		BOOL ok = FALSE;

		{
			CODBCRecordset rs;
			SQLHSTMT hstmt = rs.AttachRecordset(m_pDb);

			if (hstmt != NULL)
			{
				int aCols[] = {18,4,5,6,8,10,12};
				SQLUINTEGER bMeta = 1;
				SQLSetStmtAttr(hstmt,SQL_ATTR_METADATA_ID,&bMeta,0);
				if (Check(SQLProcedureColumns(
					hstmt,
					(UCHAR*)spq.spDb,SQL_NTS,
					(UCHAR*)spq.spOwner,SQL_NTS,
					(UCHAR*)spq.spName,SQL_NTS,
					(UCHAR*)"%",SQL_NTS)))
				{
					if (rs.BindColumns(7,aCols))
					{
						int nBinded = 0;

						sql_param* param;
						int nSpPos,nPos,paramPos;
						
						CString sParam;
						const char* pchParam;

						m_aParams.SetSize(nParams);
						while (rs.MoveNext())
						{
							nSpPos = nPos = 0;
							if (params->m_item()->m_mapped != 0)
							{
								nSpPos++; // если нет return code, то номер параметра ХП начинается с 1
							}
							
							paramPos = rs[0].AsInt();
							sParam = rs[1].AsString();
							pchParam = ((const char*)sParam) + 1;

							param_list* pnext = params;
							while (pnext)
							{
								param = pnext->m_item();
								if (param->m_name->m_type == pm_pos)
								{
									m_nBindType = pm_pos;
									if (paramPos == nSpPos)	break;
									nPos++;
								}
								else
								if (param->m_name->m_type == pm_named)
								{
									m_nBindType = pm_named;
									if (stricmp(param->m_name->m_name,pchParam)==0) break;
									nPos++;
								}
								nSpPos++;
								pnext = pnext->m_pnext();
							}
							if (nPos < nParams)
							{
								param->m_mapped = nPos;
								CQueryParam& param = m_aParams[nPos];
								param.m_strName = rs[1].AsString();
								param.m_nIOtype = rs[2].AsShort();
								param.m_nSQLType = rs[3].AsShort();
								param.m_nPrecision = rs[4].AsInt();
								param.m_nScale = rs[5].AsInt();
								param.m_nNullability = rs[6].AsInt();
								param.SetCType();
								nBinded++;
							}
							if (nBinded == nParams) break; // все параметры связаны
						}
						ok = nBinded == nParams;
					}
				}
			}
		}
		//m_sLastError = rs.GetLastError();
		m_pDb->SetCurrent(pCurrent);
		return ok;
	}
	return TRUE;
}*/

BOOL CODBCRecordset::BuildParams()
{
	m_aParams.SetSize(0);

	SQLSMALLINT type, scale, null;
	SQLUINTEGER prec;


	SQLSMALLINT pcount;
	if (Check(SQLNumParams(m_hStmt,&pcount)))
	{    
		m_aParams.SetSize(pcount);
		for (SQLSMALLINT i=0;i<pcount;++i)
		{
			if (Check(SQLDescribeParam(m_hStmt, i + 1, &type, &prec, &scale, &null)))
			{
				CQueryParam& param = m_aParams[i];
                param.m_pRS = this;
				param.m_nIOtype = SQL_PARAM_INPUT;
				param.m_nSQLType = type;
				param.m_nPrecision = prec;
				param.m_nScale = scale;
				param.m_nNullability = null;
				param.SetCType(false);
			}
			else 
      {
        /*SQLCHAR Sqlstate[5] = {0};
        SQLINTEGER NativeError;
        SQLCHAR MessageText[1000];
        SQLSMALLINT BufferLength = 1000;
        SQLSMALLINT TextLength;
        SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, 1, Sqlstate, &NativeError, MessageText, BufferLength, &TextLength);*/
        return FALSE;
      }
		}
	}
	else 
    return FALSE;

    return AllocateRowsetForParams();
}

//DmitrO
BOOL CODBCRecordset::AllocateRowsetForParams()
{
  if (m_nRowsetSizeForParams > 0)
  {
    SQLRETURN ret;
    int nSize = m_aParams.GetSize();
    if (nSize > 0)
    {
      if (m_pOffsets) free(m_pOffsets);
		    m_pOffsets = (int*)malloc(nSize*sizeof(int));
      
      m_nRowSize = 0;
      for (int i=0;i < nSize; ++i)
	    {
		    const CQueryParam& param = m_aParams[i];
			  m_pOffsets[i] = m_nRowSize;
			  m_nRowSize += param.m_nBufSize;
			  m_nRowSize += sizeof(SQLINTEGER);		    
      }
      // Set the SQL_ATTR_PARAM_BIND_TYPE statement attribute to use
      // column-wise binding.
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_BIND_TYPE, (void*)m_nRowSize, 0);
      // Specify the number of elements in each parameter array.
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMSET_SIZE, (void*)m_nRowsetSizeForParams, 0);
      // Specify an array in which to return the status of each set of
      // parameters.      
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAM_STATUS_PTR, m_pRowstatus, 0);      
      // Specify an SQLUINTEGER value in which to return the number of sets of
      // parameters processed.
      ret = SQLSetStmtAttr(m_hStmt, SQL_ATTR_PARAMS_PROCESSED_PTR, &m_nParamsProcessed, 0);

      if (!Check(ret)) 
      {
        free(m_pOffsets);
        m_pOffsets = NULL;
        return FALSE;
      }
      if (BuildRowsetForParams() == FALSE)
      {
        free(m_pOffsets);
        m_pOffsets = NULL;
        return FALSE;
      }
    }
  }
	return TRUE;
}
//

// nSQLType:
// SQL_PARAM_INPUT
// SQL_PARAM_OUTPUT
// SQL_PARAM_INPUT_OUTPUT

// IOType
// SQL_BIT
// SQL_TINYINT
// SQL_SMALLINT
// SQL_INTEGER
// SQL_REAL
// SQL_FLOAT
// SQL_DOUBLE
// SQL_DATE
// SQL_TIME
// SQL_TIMESTAMP
// SQL_NUMERIC
// SQL_DECIMAL
// SQL_BIGINT
// SQL_CHAR
// SQL_VARCHAR
// SQL_LONGVARCHAR
// SQL_BINARY
// SQL_VARBINARY
// SQL_LONGVARBINARY

BOOL CODBCRecordset::AddParam(const char* pname,int IOType,SWORD nSQLType, CType const& type,int nLen,SWORD nDec)
{

	int nBind;
	if (pname[0] == '@')
	{
		nBind = pm_named;
	}
	else
	{
		nBind = pm_pos;
	}
	if (ParamCount() == 0)
	{
		m_nBindType = nBind;
	}
	else
	  if (m_nBindType != nBind)
	  {
		  return FALSE;
	  }
	int nOldSize = m_aParams.GetSize();
	m_aParams.SetSize(nOldSize+1);
	CQueryParam& qp = m_aParams[nOldSize];
    qp.m_pRS = this;
	
  if (nBind == pm_named)	
		qp.m_strName = pname + 1;	
	else 
    qp.m_strName = pname;

	qp.m_nSQLType = nSQLType;
	qp.m_rt = type;
	qp.m_nPrecision = nLen;
	qp.m_nScale = nDec;
	qp.m_nIOtype = IOType;
	qp.SetCType(false);
	return TRUE;
}

void CODBCRecordset::SetParamOfRow(int nRow, int nNumParam, int nSize, void *pValue)
{
  BYTE *pCurRow = m_pRowset + ((nRow-1)*m_nRowSize);
  BYTE *pPosOfParam = pCurRow + m_pOffsets[nNumParam-1];
  
  const CQueryParam& param = m_aParams[nNumParam-1];  
  BYTE* pNew = pPosOfParam + param.m_nBufSize;
	*(SQLINTEGER*)pNew = nSize;
  memcpy(pPosOfParam, pValue, nSize);  
}

BOOL CODBCRecordset::BindParams()
{
  if (m_nRowsetSizeForParams == 0)
  {
	  for (int i=0;i<m_aParams.GetSize();i++)
	  {
		  CQueryParam& param = m_aParams[i];
		  if (param.m_nCType == SQL_UNKNOWN_TYPE)
		  {
			  return FALSE;
		  }    
		  if (m_nBindType == pm_pos)
		  {
			  if (!param.BindAsParameter(m_hStmt, i+1, m_pDb->GetNeedLongDataLen()))
			  {
				  return FALSE;
			  }
		  }
		  else
		  {
			  if (!param.BindAsNamedParameter(m_hStmt,i+1))
			  {
				  return FALSE;
			  }
		  }
	  }
  }
	return TRUE;
}

/*void CODBCRecordset::BindParamData(param_list* params)
{
	param_list* pnext = params;
	while (pnext)
	{
		sql_param* param = pnext->m_item();
		if (param->m_name->m_type != pm_const)
		{
			CDBField& qp = m_aParams[param->m_mapped];
			if (param->m_type)
			{
				qp.m_rt = param->m_type->m_v7;
			}
			if (param->m_name->m_type == pm_named)
			{
				if (!param->m_name->m_init.IsEmpty())
				{
					qp = param->m_name->m_init;
				}
			}
		}
		pnext = pnext->m_pnext();
	}
}*/

/*void CODBCRecordset::SetValue(CValue& rVal,CDBField& dbf,BYTE* pRow,BOOL bAgg)
{
	BYTE* pData = pRow;
	SQLINTEGER ind = *(SQLINTEGER*)(pData + sizeof(SQLINTEGER));

	if (ind == SQL_NULL_DATA)
	{
		rVal.Reset();
	}
	else
	{
		CType& rt = dbf.m_rt;
		switch( dbf.m_nCType ) {
		case	SQL_C_BIT:
				rVal = *(bool*)pData;
				break;

		case	SQL_C_UTINYINT:
				rVal = *(UCHAR*)pData;
				break;

		case	SQL_C_SSHORT:
				rVal = *(short*)pData;
				break;

		case	SQL_C_SLONG:
				rVal = *(long*)pData;
				break;

		case	SQL_C_FLOAT:
			{
				CNumeric flt = *(float*)pData;
				rVal = flt;
				break;
			}

		case	SQL_C_DOUBLE:
			{
				CNumeric dbl = *(double*)pData;
				rVal = dbl;
				break;
			}

		case	SQL_C_TIMESTAMP:
			{
				TIMESTAMP_STRUCT* pdate = (TIMESTAMP_STRUCT*)pData;
				CDate date(pdate->year,pdate->month,pdate->day);
				rVal = date;
				break;
			}

		case	SQL_C_CHAR:
			{
				int nType = rt.GetTypeCode();
				if (nType == WORD(-1))
				{
					nType = 2;
				}
	#if defined(TOYSQLDEMO) && !defined(FULL)
				rVal = (const char*)pData;
	#else
				if (!bAgg || (nType > 0 && nType < 10))
				{
					rVal = (const char*)pData;
				}
				else
				{
					//ValFromString((char*)pData,rt,rVal);
					//rVal.LinkContext(nType);
				}
	#endif
				break;
			}

		case	SQL_C_BINARY:
				rVal.Reset();
				break;
		default:
			ASSERT( FALSE );
		}
	}
}
*/
//
////////////////////////////////////////////////////////////////
//
//	CDBField implementation
//
////////////////////////////////////////////////////////////////

CDBField::CDBField():m_rt(WORD(-1))
{
	m_bIsNull = TRUE;
	m_nCType = SQL_UNKNOWN_TYPE;
	m_pstring = NULL;
    m_AddType = eStandart;
}

CDBField::~CDBField()
{
//DmitrO
	switch (m_nCType)
	{
      case SQL_C_CHAR:
		delete m_pstring;
        break;
      case SQL_C_BINARY:
        {
            if (m_nSQLType == SQL_LONGVARBINARY)
                if (m_pstream) m_pstream->Release();
            else
                delete m_pguid;
        }
        break;
      case SQL_C_TIMESTAMP:
		delete m_pdate;
        break;
      case SQL_C_NUMERIC:
		delete m_pnumeric;
        break;
      case SQL_C_TYPE_TIME:
		delete m_ptime;
        break;
    }
//
}

void CDBField::SetCType(bool NumericEnabled)
{
//DmitrO
  if (m_nSQLType == SQL_NUMERIC && NumericEnabled)
  {
      if (m_pRS->m_nDriverODBCMajorVer >= 3 && m_nScale <= 9)
          m_nCType = SQL_C_NUMERIC;
      else
          m_nCType = SQL_C_CHAR;
  }
  else
    m_nCType = GetDefaultFieldType(m_nSQLType);

	switch(m_nCType)
//
	{
	case SQL_C_BIT:
		m_nBufSize = sizeof(BOOL);
		break;

	case SQL_C_UTINYINT:
		m_nBufSize = sizeof(unsigned char);
		break;

	case SQL_C_SSHORT:
		m_nBufSize = sizeof(short);
		break;

	case SQL_C_SLONG:
		m_nBufSize = sizeof(long);
		break;

	case SQL_C_FLOAT:
		m_nBufSize = sizeof(float);
		break;

	case SQL_C_DOUBLE:
		m_nBufSize = sizeof(double);
		break;

	case SQL_C_TIMESTAMP:
		if (m_pdate != NULL)
			delete m_pdate;
		m_pdate = new TIMESTAMP_STRUCT;
		m_nBufSize = sizeof(TIMESTAMP_STRUCT);
		break;

	case SQL_C_CHAR:
		if (m_pstring != NULL)
			delete m_pstring;
		m_pstring = new CString;
//DmitrO
        if (m_nSQLType == SQL_LONGVARCHAR || m_nSQLType == SQL_WLONGVARCHAR)
        {
		    m_nBufSize = 0;
		    m_nLength = 0;
        }
        else
        {
            if (m_nSQLType == SQL_NUMERIC)
    		    m_nBufSize = m_nPrecision + 3; //терминатор, точка, знак
            else
	    	    m_nBufSize = m_nPrecision + 1; //терминатор

		    m_pstring->GetBufferSetLength(m_nBufSize);
		    m_pstring->LockBuffer();
		    m_nLength = SQL_NTS;
        }
		break;

	case SQL_C_BINARY:
        if (m_nSQLType == SQL_LONGVARBINARY)
        {
		    m_pstream = NULL;
		m_nBufSize = 0;
        }
        else
        {
    		if (m_pguid != NULL)
	    		delete m_pguid;
		    m_pguid = new GUID;
            m_nPrecision = sizeof(GUID);
		    m_nBufSize = sizeof(GUID);
        }
		break;
	case SQL_C_NUMERIC:
		if (m_pnumeric != NULL)
			delete m_pnumeric;
		m_pnumeric = new SQL_NUMERIC_STRUCT;
		m_nBufSize = sizeof(SQL_NUMERIC_STRUCT);
        m_pnumeric->precision = 0;
		m_nLength = SQL_NULL_DATA;
		break;
	case SQL_C_TYPE_TIME:
		if (m_ptime != NULL)
			delete m_ptime;
		m_ptime = new SQL_TIME_STRUCT;
		m_nBufSize = sizeof(SQL_TIME_STRUCT);
		m_nLength = SQL_NTS;
		break;
//
	}
}

#pragma optimize("t",on)

SQLPOINTER CDBField::GetBuffer()
{
	switch (m_nCType)
	{
	case SQL_C_BIT:
		return &m_boolVal;

	case SQL_C_UTINYINT:
		return &m_chVal;

	case SQL_C_SSHORT:
		return &m_iVal;

	case SQL_C_SLONG:
		return &m_lVal;

	case SQL_C_FLOAT:
		return &m_fltVal;

	case SQL_C_DOUBLE:
		return &m_dblVal;

	case SQL_C_TIMESTAMP:
		return m_pdate;

	case SQL_C_CHAR:
		return m_pstring->GetBuffer(0);

	case SQL_C_BINARY:
		return m_pguid;

//DmitrO
	case SQL_C_NUMERIC:
		return m_pnumeric;
	case SQL_C_TYPE_TIME:
		return m_ptime;
//
	default:
		//	Unknown data type
		ASSERT( FALSE );
	}
	return NULL;
}

void CDBField::Fixup()
{
	if (m_nLength == SQL_NULL_DATA)
	{
		m_bIsNull = TRUE;
		switch (m_nCType)
		{
		case SQL_C_BIT:
			m_boolVal = false;
			break;

		case SQL_C_UTINYINT:
			m_chVal = 0;
			break;

		case SQL_C_SSHORT:
			m_iVal = 0;
			break;

		case SQL_C_SLONG:
			m_lVal = 0;
			break;

		case SQL_C_FLOAT:
			m_fltVal = .0;
			break;

		case SQL_C_DOUBLE:
			m_dblVal = .0;
			break;

		case SQL_C_TIMESTAMP:
			m_pdate->year = 0;
			m_pdate->month = 0;
			m_pdate->day = 0;
			m_pdate->hour = 0;
			m_pdate->minute = 0;
			m_pdate->second = 0;
			m_pdate->fraction = 0;
			break;

		case SQL_C_CHAR:
			m_pstring->GetBufferSetLength(0);
			break;

//DmitrO
		case SQL_C_NUMERIC:
			m_pnumeric->precision = 0;
			break;

		case SQL_C_TYPE_TIME:
			m_ptime->hour = 0;
			m_ptime->minute = 0;
			m_ptime->second = 0;
			break;

		case SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			break;
//
		default:
			ASSERT(FALSE);
		}
	}
	else
	{
		m_bIsNull = FALSE;
		if (m_nCType == SQL_C_CHAR)
		{
			m_pstring->ReleaseBuffer();
			m_nLength = m_pstring->GetLength();
		}
	}
}

////////////////////////////////////////////////////////////////
bool	CDBField::AsBool()	const
{
	if( IsNull() )
		return	false;

	CString	cValue;
	switch( m_nCType ) {
	case	SQL_UNKNOWN_TYPE:
				return	false;

	case	SQL_C_BIT:
				return	(m_boolVal == TRUE);

	case	SQL_C_UTINYINT:
				return	(m_chVal == 'T' || m_chVal == '1');

	case	SQL_C_SSHORT:
				return	(m_iVal != 0);

	case	SQL_C_SLONG:
				return	(m_lVal != 0);

	case	SQL_C_FLOAT:
				return	(m_fltVal != 0.0);

	case	SQL_C_DOUBLE:
				return	(m_dblVal != 0.0);

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to bool
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				if( m_pstring->GetLength() < 1 )
					return	false;
				return	((*m_pstring)[0] == 'T' || (*m_pstring)[0] == '1');

	case	SQL_C_BINARY:
				//	Cannot convert long binary to bool
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num != 0;
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	false;
}

unsigned char	CDBField::AsChar()	const
{
	if( IsNull() )
		return	' ';

	switch( m_nCType ) {
	case	DBVT_NULL:
				return	' ';

	case	SQL_C_BIT:
				return	(m_boolVal) ? 'T' : 'F';

	case	SQL_C_UTINYINT:
				return	m_chVal;

	case	SQL_C_SSHORT:
				return	(unsigned char)m_iVal;

	case	SQL_C_SLONG:
				return	(unsigned char)m_lVal;

	case	SQL_C_FLOAT:
				return	(unsigned char)m_fltVal;

	case	SQL_C_DOUBLE:
				return	(unsigned char)m_dblVal;

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to unsigned char
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				if( m_pstring->GetLength() < 1 )
					return	' ';
				return	(unsigned char)((*m_pstring)[0]);

	case	SQL_C_BINARY:
				//	Cannot convert long binary to unsigned char
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num;
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	' ';
}

short	CDBField::AsShort()	const
{
	if( IsNull() )
		return	0;

	switch( m_nCType ) {
	case	DBVT_NULL:
				return	0;

	case	SQL_C_BIT:
				return	m_boolVal ? 1 : 0;

	case	SQL_C_UTINYINT:
				return	(short)m_chVal;

	case	SQL_C_SSHORT:
				return	m_iVal;

	case	SQL_C_SLONG:
				return	(short)m_lVal;

	case	SQL_C_FLOAT:
				return	(short)m_fltVal;

	case	SQL_C_DOUBLE:
				return	(short)m_dblVal;

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to short
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				return	(short)atoi( *m_pstring );

	case	SQL_C_BINARY:
				//	Cannot conver long binary to short
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num;
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	0;
}

int		CDBField::AsInt()	const
{
	return	AsLong();
}

long	CDBField::AsLong()	const
{
	if( IsNull() )
		return	0;

	switch( m_nCType ) {
	case	DBVT_NULL:
				return	0;

	case	SQL_C_BIT:
				return	m_boolVal ? 1 : 0;

	case	SQL_C_UTINYINT:
				return	(long)m_chVal;

	case	SQL_C_SSHORT:
				return	(long)m_iVal;

	case	SQL_C_SLONG:
				return	m_lVal;

	case	SQL_C_FLOAT:
				return	(long)m_fltVal;

	case	SQL_C_DOUBLE:
				return	(long)m_dblVal;

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to long
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				return	atol( *m_pstring );

	case	SQL_C_BINARY:
				//	Cannot conver long binary to long
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num;
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	0;
}

float	CDBField::AsFloat()	const
{
	if( IsNull() )
		return	0.0;

	switch( m_nCType ) {
	case	DBVT_NULL:
				return	0.0;

	case	SQL_C_BIT:
				return	(float)(m_boolVal ? 1.0 : 0.0);

	case	SQL_C_UTINYINT:
				return	(float)m_chVal;

	case	SQL_C_SSHORT:
				return	(float)m_iVal;

	case	SQL_C_SLONG:
				return	(float)m_lVal;

	case	SQL_C_FLOAT:
				return	m_fltVal;

	case	SQL_C_DOUBLE:
				return	(float)m_dblVal;

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to float
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				return	(float)atof( *m_pstring );

	case	SQL_C_BINARY:
				//	Cannot conver long binary to float
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num.GetDouble();
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	0.0;
}

double	CDBField::AsDouble()	const
{
	if( IsNull() )
		return	0.0;

	switch( m_nCType ) {
	case	DBVT_NULL:
				return	0.0;

	case	SQL_C_BIT:
				return	m_boolVal ? 1.0 : 0.0;

	case	SQL_C_UTINYINT:
				return	(double)m_chVal;

	case	SQL_C_SSHORT:
				return	(double)m_iVal;

	case	SQL_C_SLONG:
				return	(double)m_lVal;

	case	SQL_C_FLOAT:
				return	(double)m_fltVal;

	case	SQL_C_DOUBLE:
				return	m_dblVal;

	case	SQL_C_TIMESTAMP:
				//	Cannot convert date to double
				ASSERT( FALSE );
				break;

	case	SQL_C_CHAR:
				ASSERT( m_pstring != NULL );
				return	atof( *m_pstring );

	case	SQL_C_BINARY:
				//	Cannot conver long binary to double
				ASSERT( FALSE );
				break;
//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            return Num.GetDouble();
        }
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	0.0;
}

COleDateTime	CDBField::AsDate()	const
{
	COleDateTime	date;
	if( IsNull() ) {
		date.SetStatus( COleDateTime::null );
		return	date;
	}

	switch( m_nCType ) {
	case	DBVT_NULL:
			date.SetStatus( COleDateTime::null );
			return	date;

	case	SQL_C_BIT:
			date.SetStatus( COleDateTime::invalid );
			return	date;

	case	SQL_C_UTINYINT:
			date.SetStatus( COleDateTime::invalid );
			return	date;

	case	SQL_C_SSHORT:
			return	COleDateTime( (time_t)m_iVal );

	case	SQL_C_SLONG:
			return	COleDateTime( (time_t)m_lVal );

	case	SQL_C_FLOAT:
			return	COleDateTime( (time_t)m_fltVal );

	case	SQL_C_DOUBLE:
			return	COleDateTime( (time_t)m_dblVal );

	case	SQL_C_TIMESTAMP:
			ASSERT( m_pdate != NULL );
			return	COleDateTime(	m_pdate->year, m_pdate->month, m_pdate->day,
									m_pdate->hour, m_pdate->minute, m_pdate->second );

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			date.ParseDateTime( *m_pstring );
			return	date;

	case	SQL_C_BINARY:
			//	Cannot conver long binary to date
			ASSERT( FALSE );
			break;
//DmitrO
    case	SQL_C_NUMERIC:
			ASSERT( FALSE );
			break;
    case	SQL_C_TYPE_TIME:
			ASSERT( FALSE );
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	date.SetStatus( COleDateTime::invalid );
	return	date;
}

CString		CDBField::AsString()	const
{
	CString	cValue;

	switch( m_nCType ) {
	case	DBVT_NULL:
			return	cValue;

	case	SQL_C_BIT:
			return	CString( m_boolVal ? "T" : "F" );

	case	SQL_C_UTINYINT:
			return	CString( m_chVal );

	case	SQL_C_SSHORT:
			cValue.Format( "%hd", m_iVal );
			return	cValue;

	case	SQL_C_SLONG:
			cValue.Format( "%ld", m_lVal );
			return	cValue;

	case	SQL_C_FLOAT:
			cValue.Format( "%f", m_fltVal );
			return	cValue;

	case	SQL_C_DOUBLE:
			cValue.Format( "%f", m_dblVal );
			return	cValue;

	case	SQL_C_TIMESTAMP:
		{
			ASSERT( m_pdate != NULL );
			COleDateTime	date( m_pdate->year, m_pdate->month, m_pdate->day,
								  m_pdate->hour, m_pdate->minute, m_pdate->second );
			return	date.Format();
		}
	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			return	*m_pstring;

//DmitrO
    case	SQL_C_NUMERIC:
        {
            CSQLNumeric Num;
            Num.Load(m_pnumeric);
            char* pStr = cValue.GetBufferSetLength(m_pnumeric->precision + 2);
            Num.Convert(pStr, m_pnumeric->precision + 2, m_pnumeric->scale);
            cValue.ReleaseBuffer();
            return cValue;
        }
    case	SQL_C_TYPE_TIME:
        {
            cValue.Format("%02hu:%02hu:%02hu", m_ptime->hour, m_ptime->minute, m_ptime->second);
            return cValue;
        }
			break;
//
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	cValue;
}

//DmitrO
CNumeric CDBField::AsNumeric() const
{
    CSQLNumeric Num;
    
	switch( m_nCType ) {
    case	DBVT_NULL:
        return	Num;

    case	SQL_C_BIT:
        if (m_boolVal)
            static_cast<CNumeric>(Num) = 1;
        return Num;

	case	SQL_C_UTINYINT:
		return	static_cast<CNumeric>(Num) = m_chVal;

	case	SQL_C_SSHORT:
		return	static_cast<CNumeric>(Num) = m_iVal;

	case	SQL_C_SLONG:
		return	static_cast<CNumeric>(Num) = m_lVal;

	case	SQL_C_FLOAT:
		return	static_cast<CNumeric>(Num) = m_fltVal;

	case	SQL_C_DOUBLE:
		return	static_cast<CNumeric>(Num) = m_dblVal;

	case	SQL_C_TIMESTAMP:
		//	Cannot convert date to double
		ASSERT( FALSE );
		break;

	case	SQL_C_CHAR:
		ASSERT( m_pstring != NULL );
        Num.FromString(*m_pstring, NULL);
		return Num;

	case	SQL_C_BINARY:
		//	Cannot conver long binary to double
		ASSERT( FALSE );
		break;

	case	SQL_C_NUMERIC:
        Num.Load(m_pnumeric);
        return Num;
	case	SQL_C_TYPE_TIME:
		ASSERT( FALSE );
		break;
	}
	//	Undefined data type
	ASSERT( FALSE );
    return Num;
}

const TIMESTAMP_STRUCT*	CDBField::AsTimeStamp()	const
{
    ASSERT(m_nCType == SQL_C_TIMESTAMP);
    return m_pdate;
}

IStream* CDBField::AsIStream() const
{
    ASSERT(m_nCType == SQL_C_BINARY);
    if (m_nCType == SQL_C_BINARY)
        return m_pstream;
    else
        return NULL;
}

const GUID* CDBField::AsGUID() const
{
    ASSERT(m_nCType == SQL_C_BINARY);
    if (m_nCType == SQL_C_BINARY)
        return m_pguid;
    else
        return NULL;
}

#define ROWSET_IS_NULL(pData) (*((SQLINTEGER*)(pData + m_nBufSize)) == SQL_NULL_DATA)

void CDBField::GetValue(CValue& value, BYTE* pData /*= NULL*/) const
{
    switch(m_nCType)
    {
        case DBVT_NULL:
            value.Reset();
            break;
        case SQL_C_BIT:
            if (pData)
                value = ROWSET_IS_NULL(pData) || !(*(bool*)pData) ? 0 : 1;
            else
                value = m_boolVal ? 1 : 0;
            break;
        case SQL_C_UTINYINT:
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    value = 0L;
                else
                    value = *(UCHAR*)pData;
            }
            else
                value = m_chVal;
            break;
        case SQL_C_SSHORT:
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    value = 0L;
                else
                    value = *(short*)pData;
            }
            else
                value = m_iVal;
            break;
        case SQL_C_SLONG:
          {
            long nVal;
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    nVal = 0;
                else
                    nVal = *(long*)pData;
            }
            else
                nVal = m_lVal;

			switch (m_AddType)
			{
			case eDocKindAsStr:
				{
					CMetaDataObj* pObj = m_pRS->m_pDb->m_pMetaDataCont->m_DocDefs.GetItem(nVal);
					value = pObj ? pObj->GetCode() : "";
				}
				break;
			case eDocKindPresentAsStr:
				{
					CMetaDataObj* pObj = m_pRS->m_pDb->m_pMetaDataCont->m_DocDefs.GetItem(nVal);
					value = pObj ? pObj->GetRealPresent() : "";
				}
				break;
			default:
				if (m_rt.GetTypeCode() == CALCULATIONKIND_TYPE_1C || m_rt.GetTypeCode() == SUBCONTOKIND_TYPE_1C)
				{ //Specially for sql database format
					value.SetType(m_rt);    		
					value.SetObjID(CObjID(nVal,CDBSign("   ")));
				}
				else
					value = nVal;
			}
          }
          break;
        case SQL_C_FLOAT:
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    value = 0L;
                else
                    value = CNumeric(*(float*)pData);
            }
            else
    			value = CNumeric(m_fltVal);
            break;
        case SQL_C_DOUBLE:
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    value = 0L;
                else
                    value = CNumeric(*(double*)pData);
            }
            else
    			value = CNumeric(m_dblVal);
            break;
        case SQL_C_TIMESTAMP:
          {
            TIMESTAMP_STRUCT* pdate;
            BOOL bIsNull;
            if (pData)
            {
                bIsNull = ROWSET_IS_NULL(pData);
                pdate = (TIMESTAMP_STRUCT*)pData;
            }
            else
            {
                bIsNull = m_bIsNull;
                pdate = m_pdate;
            }

            if (m_rt.GetTypeCode() == STRING_TYPE_1C)
            {
                CString s;
                if (!bIsNull)
                    s.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", pdate->year, pdate->month, pdate->day,
                        pdate->hour, pdate->minute, pdate->second,
                        pdate->fraction/1000000);
                value = s;
            }
            else
            {
                if (!bIsNull)
                    value = CDate(pdate->year, pdate->month, pdate->day);
                else
                {
                    value.Reset();
                    value.SetType(CType(DATE_TYPE_1C));
                }
            }
          }
            break;
        case SQL_C_CHAR:
          {
            LPCSTR pstring;
            if (pData)
            {
                if (ROWSET_IS_NULL(pData))
                    *pData = 0;
                pstring = (LPCSTR)pData;
            }
            else
                pstring = *m_pstring;

            if (m_nSQLType == SQL_NUMERIC || m_rt.GetTypeCode() == NUMBER_TYPE_1C)
            {
                CNumeric Num;
                Num.FromString(pstring, NULL);
                value = Num;
            }
            else
            {
                if (m_AddType == eStandart)
                {
                    switch (m_rt.GetTypeCode())
                    {
                    case UNDEFINE_TYPE_1C:
                        CMetaDataWork::Make1C_ValueFromString(value, m_rt, pstring, CMetaDataWork::VeryLongString);
                        break;
                    case DATE_TYPE_1C:
						{
							CDate date(0,0,0);
							date.GetFormatted(df_yyyyMMdd, pstring);
							value = date;
						}
                        break;
                    case ENUM_TYPE_1C:
                    case REFERENCE_TYPE_1C:
                    case DOCUMENT_TYPE_1C:
                    case CALENDAR_TYPE_1C:
                    case ACCOUNT_TYPE_1C:
                        CMetaDataWork::Make1C_ValueFromString(value, m_rt, pstring,
                            m_rt.GetTypeID() ? CMetaDataWork::ShortString : CMetaDataWork::LongString);
                        break;
                    case CHART_OF_ACC_TYPE_1C:
                        CMetaDataWork::Make1C_ValueFromString(value, m_rt, pstring, CMetaDataWork::ShortString);
                        break;
                    case CALCULATIONKIND_TYPE_1C:
                    case SUBCONTOKIND_TYPE_1C:
                        if (m_nPrecision == 4) //Specially for dbf database format
                        {
                            value.SetType(m_rt);
                            value.SetObjID(CObjID(strtol(pstring, NULL, 36), CDBSign("   ")));
                        }
                        else
                            CMetaDataWork::Make1C_ValueFromString(value, m_rt, pstring,
                            m_rt.GetTypeCode() == CALCULATIONKIND_TYPE_1C ? CMetaDataWork::LongString : CMetaDataWork::ShortString);
                        break;
                    default:
                        value = pstring;
                    }
                }
				else if (m_AddType == eDocKindAsStr)
				{
					LONG nVal = strtol(pstring, NULL, 36);
					CMetaDataObj* pObj = m_pRS->m_pDb->m_pMetaDataCont->m_DocDefs.GetItem(nVal);
					value = pObj ? pObj->GetCode() : "";
				}
				else if (m_AddType == eDocKindPresentAsStr)
				{
					LONG nVal = strtol(pstring, NULL, 36);
					CMetaDataObj* pObj = m_pRS->m_pDb->m_pMetaDataCont->m_DocDefs.GetItem(nVal);
					value = pObj ? pObj->GetRealPresent() : "";
				}
                else if (m_AddFieldIndex >= 0)
                {
                    CDBField &KindField = m_pRS->m_rgFields[m_AddFieldIndex];
                    long KindID;
                    if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_SQL)
                        KindID = KindField.AsLong();
                    else
                        KindID = strtol(KindField.AsString(), NULL, 36);

                    if (m_AddType == eDocKind)
                        CMetaDataWork::Make1C_ValueFromString(value, CType(DOCUMENT_TYPE_1C, KindID), pstring, CMetaDataWork::ShortString);
                    else //eSbKind
                    {
                        CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
                        if (pBuhDef)
                        {
                            CSbKindDef* pSbKindDef = pBuhDef->GetSbKindDefs()->GetItem(KindID);
                            if (pSbKindDef)
                                CMetaDataWork::Make1C_ValueFromString(value, CType(pSbKindDef->m_TypeCode, pSbKindDef->m_Kind),
                                    pstring, pSbKindDef->m_Kind ? CMetaDataWork::ShortString : CMetaDataWork::LongString);
							else
								value.Reset();
                        }
                    }
                }
            }
          }
            break;
        case SQL_C_BINARY:
          {
            GUID* pguid;
            BOOL bIsNull;
            if (pData)
            {
                bIsNull = ROWSET_IS_NULL(pData);
                pguid = (GUID*)pData;
            }
            else
            {
                bIsNull = m_bIsNull;
                pguid = m_pguid;
            }

            if (m_nSQLType == SQL_BINARY && m_nPrecision == sizeof(GUID))
            {
                CBLPtr<CGUID> pGUID;
                pGUID.Create();
                if (!bIsNull)
                    pGUID->Set(pguid);
                value.AssignContext(pGUID);
            }
            else
            {
                CBLPtr<CBinaryData> pBinaryData;
                pBinaryData.Create();
                if (!bIsNull)
                {
                    if (pData)
                        pBinaryData->GetIStream()->Write(pData, *((SQLINTEGER*)(pData + m_nBufSize)), NULL);
                    else
                        pBinaryData->AttachToIStream(m_pstream);
                }
                value.AssignContext(pBinaryData);
            }
          }
            break;
        case SQL_C_NUMERIC:
          {
			  switch (m_rt.GetTypeCode())
			  {
			  case STRING_TYPE_1C:
				  value = AsString();
				  break;
			  default:
				  CSQLNumeric Num;
				  if (pData && !ROWSET_IS_NULL(pData))
					  Num.Load((SQL_NUMERIC_STRUCT*)pData);
				  else
					  Num.Load(m_pnumeric);
				  value = Num;
			  }
          }
            break;
        case SQL_C_TYPE_TIME:
          {
            CString s;
            if (pData && !ROWSET_IS_NULL(pData))
            {
                SQL_TIME_STRUCT* ptime = (SQL_TIME_STRUCT*)pData;
                s.Format("%02hu:%02hu:%02hu", ptime->hour, ptime->minute, ptime->second);
            }
            else
                s.Format("%02hu:%02hu:%02hu", m_ptime->hour, m_ptime->minute, m_ptime->second);
            value = s;
          }
			break;
	}

    if (m_BLCTypeName.GetLength())
    {
        int err = 0;
        CValue vBLCParam(value);
        if (value.CreateObject(m_BLCTypeName))
        {
            CBLContext* pCont = value.GetContext();
            if (pCont)
            {
                int nMeth = pCont->FindMethod("_SQLCreate");
                if (nMeth >= 0)
                {
                    if (pCont->GetNParams(nMeth) <= 2)
                    {
                        CValue vParam;
                        CBLContext* pParam = m_pRS->GetVCtorParam();
                        if (pParam)
                            vParam.AssignContext(pParam);
                        CValue* ppVal[2] = {&vBLCParam, &vParam};
                        if (!pCont->CallAsProc(nMeth, ppVal))
                            err = 5;
                    }
                    else
                        err = 4;
                }
                else
                    err = 3;
            }
            else
                err = 2;
        }
        else
            err = 1;

        if (err)
        {
            CString sErr;
            sErr.Format("ODBCRecordSet field:%s udf:%s error:", (LPCSTR)m_strName, (LPCSTR)m_BLCTypeName);
            switch(err)
            {
            case 1:
                sErr += "failed to create object";
                break;
            case 2:
                sErr += "wrong class name";
                break;
            case 3:
                sErr += "failed to find v_ctor (_SQLCreate)";
                break;
            case 4:
                sErr += "v_ctor params count must be <= 2";
                break;
            case 5:
                sErr += "failed to execute v_ctor";
                break;
            };
            pBkEndUI->DoMessageLine(sErr, mmBlackErr);
        }
    }
}
//
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//	Assignment operators
////////////////////////////////////////////////////////////////

CDBField& CDBField::operator =( const bool bVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = bVal;
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (bVal) ? 'T' : 'F';
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (bVal) ? 1 : 0;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (bVal) ? 1 : 0;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)((bVal) ? 1.0 : 0.0);
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)((bVal) ? 1.0 : 0.0);
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%c", (bVal) ? 'T' : 'F' );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const char chVal )
{
	return	operator =( (unsigned char) chVal );
}
CDBField& CDBField::operator =( const unsigned char chVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (chVal == 'T' || chVal == '1');
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)chVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)chVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)chVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)chVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)chVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%c", chVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const short sVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (sVal != 0);
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)sVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)sVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)sVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)sVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)sVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%hd", sVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const int iVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (iVal != 0);
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)iVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)iVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)iVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)iVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)iVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%d", iVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const long lVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (lVal != 0);
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)lVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)lVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)lVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)lVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)lVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%ld", lVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const float fltVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (fltVal != 0.0);
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)fltVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)fltVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)fltVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)fltVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)fltVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%f", fltVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const double dblVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (dblVal != 0.0);
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)dblVal;
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)dblVal;
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)dblVal;
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)dblVal;
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)dblVal;
			return	*this;

	case	SQL_C_TIMESTAMP:
			//	Cannot convert to datetime
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			m_pstring->Format( "%f", dblVal );
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const COleDateTime& dtVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
	case	SQL_C_UTINYINT:
	case	SQL_C_SSHORT:
	case	SQL_C_SLONG:
	case	SQL_C_FLOAT:
	case	SQL_C_DOUBLE:
			//	Cannot convert to the current data type
			ASSERT( FALSE );
			return	*this;

	case	SQL_C_TIMESTAMP:
			ASSERT( m_pdate != NULL );
			m_pdate->year	= dtVal.GetYear();
			m_pdate->month	= dtVal.GetMonth();
			m_pdate->day	= dtVal.GetDay();
			m_pdate->hour	= dtVal.GetHour();
			m_pdate->minute	= dtVal.GetMinute();
			m_pdate->second = dtVal.GetSecond();
			m_pdate->fraction = 0;
			return	*this;

	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			*m_pstring = dtVal.Format();
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const CString& cVal )
{
	return	operator =( (LPCTSTR)cVal );
}

CDBField& CDBField::operator =( const LPCTSTR szVal )
{
	switch( m_nCType ) {
	case	DBVT_NULL:
			//	Undefined data type
			ASSERT( FALSE );

	case	SQL_C_BIT:
			m_boolVal = (szVal != NULL || atoi( szVal ) != 0 );
			return	*this;

	case	SQL_C_UTINYINT:
			m_chVal = (unsigned char)szVal[0];
			return	*this;

	case	SQL_C_SSHORT:
			m_iVal = (short)atoi( szVal );
			return	*this;

	case	SQL_C_SLONG:
			m_lVal = (long)atol( szVal );
			return	*this;

	case	SQL_C_FLOAT:
			m_fltVal = (float)atof( szVal );
			return	*this;

	case	SQL_C_DOUBLE:
			m_dblVal = (double)atof( szVal );
			return	*this;

	case	SQL_C_TIMESTAMP:
		{
			ASSERT( m_pdate != NULL );
			COleDateTime	dt;
			dt.ParseDateTime( szVal );
			m_pdate->year	= dt.GetYear();
			m_pdate->month	= dt.GetMonth();
			m_pdate->day	= dt.GetDay();
			m_pdate->hour	= dt.GetHour();
			m_pdate->minute	= dt.GetMinute();
			m_pdate->second = dt.GetSecond();
			m_pdate->fraction = 0;
			return	*this;
		}

	case	SQL_C_TYPE_TIME:
        {
            char Buf[9];
            strncpy(Buf, szVal, 8);
            Buf[2] = 0;
            Buf[5] = 0;
            Buf[8] = 0;
            m_ptime->hour = atoi(Buf);
            m_ptime->minute = atoi(&Buf[3]);
            m_ptime->second = atoi(&Buf[6]);
			return	*this;
        }
	case	SQL_C_CHAR:
			ASSERT( m_pstring != NULL );
			*m_pstring = szVal;
			return	*this;

	case	SQL_C_BINARY:
            if (m_pstream)
            {
                m_pstream->Release();
                m_pstream = NULL;
            }
			return	*this;
	}
	//	Undefined data type
			ASSERT( FALSE );
			return	*this;
	}

CDBField& CDBField::operator =(IStream* pIStream )
{
    if (m_nCType == SQL_C_BINARY)
    {
        if (m_pstream)
            m_pstream->Release();
        m_pstream = pIStream;
        if (m_pstream)
            m_pstream->AddRef();
    }
    else
	    ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator =( const GUID* pGUID )
{
    if (m_nCType == SQL_C_BINARY)
    {
        memcpy(m_pguid, pGUID, sizeof(GUID));
    }
    else
	ASSERT( FALSE );
	return	*this;
}

CDBField& CDBField::operator = (const CValue& value)
{
    m_nLength = SQL_NTS;
    m_bIsNull = FALSE;
    switch (value.GetTypeCode())
    {
    case UNDEFINE_TYPE_1C:
        if (m_nCType == SQL_C_CHAR && m_nSQLType == SQL_CHAR && m_nPrecision == 23)
        {
            char* pStr = m_pstring->GetBufferSetLength(23);
            *pStr++ = 'U';
            memset(pStr, ' ', 23 - 1);
        }
        else
        {
            m_nLength = SQL_NULL_DATA;
            m_bIsNull = TRUE;
        }
        break;
    case NUMBER_TYPE_1C:
        {
            const CNumeric& Num = value.GetNumeric();
            switch (m_nCType)
            {
            case SQL_C_BIT:
                m_boolVal = (Num.operator long() != 0);
                break;
            case SQL_C_UTINYINT:
                m_chVal = Num.operator long();
                break;
            case SQL_C_SSHORT:
                m_iVal = Num.operator long();
                break;
	        case SQL_C_SLONG:
                m_lVal = Num.operator long();
                break;
	        case SQL_C_FLOAT:
                m_fltVal = Num.GetDouble();
                break;
	        case SQL_C_DOUBLE:
                m_dblVal = Num.GetDouble();
                break;
	        case SQL_C_CHAR:
                if (m_nSQLType == SQL_CHAR && m_nPrecision == 23)
                    m_pstring->Format("N%22s", (LPCSTR)value.GetString());
                break;
            }
        }
        break;
    case STRING_TYPE_1C:
        {
            switch (m_nCType)
            {
            case SQL_C_CHAR:
                if (m_nSQLType == SQL_CHAR && m_nPrecision == 23 && value.GetString().GetLength() == 22)
                    m_pstring->Format("S%-22s", (LPCSTR)value.GetString());
                else
                    m_pstring->operator = (value.GetString());
                break;
            case SQL_C_TYPE_TIME:
                {
                    char Buf[9];
                    strncpy(Buf, value.GetString(), 8);
                    Buf[2] = 0;
                    Buf[5] = 0;
                    Buf[8] = 0;
                    m_ptime->hour = atoi(Buf);
                    m_ptime->minute = atoi(&Buf[3]);
                    m_ptime->second = atoi(&Buf[6]);
                }
                break;
            }
        }
        break;
    case DATE_TYPE_1C:
        {
            CDate date(value.GetDate());
            switch (m_nCType)
            {
				case SQL_C_TIMESTAMP:
					m_pdate->year = date.GetYear();
					m_pdate->month = date.GetMonth();
					m_pdate->day = date.GetMonthDay();
					m_pdate->hour = 0;
					m_pdate->minute	= 0;
					m_pdate->second = 0;
					m_pdate->fraction = 0;
				break;
				case SQL_C_CHAR:
                    if (m_nSQLType == SQL_CHAR && m_nPrecision == 23)
                    {
                        char* pStr = m_pstring->GetBufferSetLength(23);
                        *pStr++ = 'D';
                        memcpy(pStr, date.Format(df_yyyyMMdd, NULL), 8);
                        pStr += 8;
                        memset(pStr, ' ', 23 - 8 - 1);
                    }
				break;
            };
        }
        break;
    case ENUM_TYPE_1C:
    case REFERENCE_TYPE_1C:
    case DOCUMENT_TYPE_1C:
    case ACCOUNT_TYPE_1C:
        if (m_nCType == SQL_C_CHAR)
        {
            if (m_nPrecision == 9)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::ShortString);
            else if (m_nPrecision == 13)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::LongString);
            else if (m_nPrecision == 23)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::VeryLongString);
        }
        break;
    case CALENDAR_TYPE_1C:
    case CALCULATIONKIND_TYPE_1C:
        if (m_nCType == SQL_C_CHAR)
        {
            if (m_nPrecision == 13)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::LongString);
            else if (m_nPrecision == 23)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::VeryLongString);
        }
        break;
    case SUBCONTOKIND_TYPE_1C:
    case CHART_OF_ACC_TYPE_1C:
        if (m_nCType == SQL_C_CHAR)
        {
            if (m_nPrecision == 9)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::ShortString);
            else if (m_nPrecision == 23)
                *m_pstring = CMetaDataWork::GetObjDBString(value, CMetaDataWork::VeryLongString);
        }
        break;
    case AGREGATE_TYPE_1C:
        if (m_nCType == SQL_C_BINARY)
        {
            CBLContext* pCont = value.GetContext();
            if (m_nSQLType == SQL_BINARY && m_nPrecision == sizeof(GUID))
            {
                if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CGUID))
                    static_cast<CGUID*>(pCont)->Get(m_pguid);
            }
            else
            {
                CBinaryData* pBinaryData = NULL;
                if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CBinaryData))
                    pBinaryData = static_cast<CBinaryData*>(pCont);
                else if (pCont->GetRuntimeClass() == RUNTIME_CLASS(CComponentClass))
                {
                    CValue vClassName("BinaryData"), vBD;
                    if (static_cast<CComponentClass*>(pCont)->GetBaseClass(vClassName, vBD))
                      pBinaryData = static_cast<CBinaryData*>(vBD.GetContext());
                }
                if (pBinaryData)
                    operator =(pBinaryData->GetIStream());
                else
                    operator =((IStream*)NULL);
            }
        }
        break;
    }
	return *this;
}

#pragma optimize("t",off)

BOOL CDBField::BindToColumn(int n, BYTE* pRowset)
{
	SQLRETURN ret;
	if (pRowset == NULL) // get field info
	{
		SWORD nActualLen;
		LPCSTR lpszFieldName = m_strName.GetBuffer(MAX_FNAME_LEN + 1);

		ret = SQLDescribeCol(m_pRS->m_hStmt, n,
			(UCHAR*)lpszFieldName, MAX_FNAME_LEN, &nActualLen,
			&m_nSQLType,
			&m_nPrecision,
			&m_nScale,
			&m_nNullability);

		SetCType(true);
		m_strName.ReleaseBuffer(nActualLen);

		if (!Check(ret))
			return FALSE;
	}

//DmitrO
    if (IsLongSQLType())
        return TRUE;
    
    SQLPOINTER p = NULL;
	if (m_pRS->m_nRowsetSize <= 1) // bind to field
	{
        p = GetBuffer();
		ret = SQLBindCol(m_pRS->m_hStmt,n,
			m_nCType,
			p,
			m_nBufSize,
			&m_nLength);
	}
	else if (pRowset) // bind to rowset
    {
        p = pRowset;
        ret = SQLBindCol(m_pRS->m_hStmt,n,
            m_nCType,
            pRowset,
            m_nBufSize,
            (long*)(pRowset + m_nBufSize));
    }
	if (!Check(ret))
	{
		return FALSE;
	}
    if (m_nCType == SQL_C_NUMERIC && (p))
    {
        SQLHDESC hdesc = NULL;
        ret = SQLGetStmtAttr(m_pRS->m_hStmt, SQL_ATTR_APP_ROW_DESC, &hdesc, 0, NULL);
        if (!Check(ret))
            return FALSE;
        ret = SQLSetDescField(hdesc, n, SQL_DESC_PRECISION, (SQLPOINTER) m_nPrecision, 0);
        if (!Check(ret))
            return FALSE;
        ret = SQLSetDescField(hdesc, n, SQL_DESC_SCALE, (SQLPOINTER) m_nScale, 0);
        if (!Check(ret))
            return FALSE;
        ret = SQLSetDescField(hdesc, n, SQL_DESC_DATA_PTR, p, 0);
        if (!Check(ret))
            return FALSE;
    };
//
	return TRUE;
}

void CQueryParam::SetDataAtExec(BOOL NeedLongDataLen, long DataLength)
{
    if (DataLength)
    {
        if (NeedLongDataLen)
            m_nLength = SQL_LEN_DATA_AT_EXEC(DataLength);
        else
            m_nLength = SQL_DATA_AT_EXEC;
    }
    else
        m_nLength = SQL_NULL_DATA;
    m_nPrecision = 0x7FFFFFFF;
}

BOOL CQueryParam::BindAsParameter(SQLHSTMT hStmt,int n, BOOL NeedLongDataLen)
{
	if (m_nIOtype == SQL_RETURN_VALUE) m_nIOtype = SQL_PARAM_OUTPUT;

    SQLPOINTER p;

    switch(m_nSQLType)
    {
    case SQL_LONGVARBINARY:
        {
            p = (SQLPOINTER)n;
            long DataLength;
            if (m_pstream)
            {
                LARGE_INTEGER Move;
                Move.QuadPart = 0;
                ULARGE_INTEGER Size;
                if (m_pstream->Seek(Move, STREAM_SEEK_END, &Size) != S_OK)
                    return FALSE;
                DataLength = Size.LowPart;
            }
            else
                DataLength = 0;

            SetDataAtExec(NeedLongDataLen, DataLength);
        }
        break;
    case SQL_WLONGVARCHAR:
    case SQL_LONGVARCHAR:
        {
            p = (SQLPOINTER)n;
            SetDataAtExec(NeedLongDataLen, m_pstring->GetLength());
        }
        break;
    default:
        p = GetBuffer();
        break;
    }

	return Check(SQLBindParameter(hStmt,n,m_nIOtype,
		m_nCType,
		m_nSQLType,
		m_nPrecision,
		m_nScale,
        p,
		m_nBufSize,
		&m_nLength));
}

BOOL CQueryParam::BindAsParameter(SQLHSTMT hStmt,int n, void *pBuffer, void *pInd)
{
  if (m_nIOtype == SQL_RETURN_VALUE) m_nIOtype = SQL_PARAM_OUTPUT;
	return Check(SQLBindParameter(hStmt,n,m_nIOtype,
		m_nCType,
		m_nSQLType,
		m_nPrecision,
		m_nScale,
    pBuffer,
		m_nBufSize,
		(long*)pInd));
}

BOOL CQueryParam::BindAsNamedParameter(SQLHSTMT hStmt,int n)
{
	if (!Check(SQLBindParameter(hStmt,n,m_nIOtype,
		m_nCType,
		m_nSQLType,
		m_nPrecision,
		m_nScale,GetBuffer(),
		m_nBufSize,
		&m_nLength)))
	{
		return FALSE;
	}
	SQLHDESC hIpd;
	if (!Check(SQLGetStmtAttr(hStmt, SQL_ATTR_IMP_PARAM_DESC, &hIpd, 0, 0)))
	{
		return FALSE;
	}
	if (!Check(SQLSetDescField(hIpd, n, SQL_DESC_NAME, (void*)(const char*)m_strName, SQL_NTS)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CQueryParam::GetBufferAndSize(void **pointer, int &size)
{
  BOOL ret = FALSE;
  if (m_nIOtype == SQL_PARAM_INPUT)
  {
    if (m_nCType == SQL_C_CHAR)
    {
      *pointer = GetBuffer();
      size = min(m_pstring->GetLength(), m_nBufSize);
      ret = TRUE;
    }
    else
    {      
      *pointer = GetBuffer();
      size = m_nBufSize;
      ret = TRUE;
    }          
  }
  return ret;
}

BOOL CODBCRecordset::BuildRowsetForParams()
{
  if (m_pRowset) free(m_pRowset);

	m_pRowset = (BYTE*)malloc(m_nRowSize*m_nRowsetSizeForParams);	
	m_nAllocRowSizeForParams = m_nRowsetSizeForParams;
    
  BYTE* pRow = m_pRowset;
  int nSize = m_aParams.GetSize();
  for (int i=0;i < nSize; ++i)
  {             
    CQueryParam& param = m_aParams[i];
    if (param.m_nCType == SQL_UNKNOWN_TYPE)
		{      
      free(m_pRowset);
      m_pRowset = NULL;  
			return FALSE;
		}    

    BYTE* pNew = pRow + param.m_nBufSize;
		//*(SQLINTEGER*)pNew = param.m_nLength;
    //*(SQLINTEGER*)pNew = SQL_NTS;
    //*(SQLINTEGER*)pNew = 0;    
    if (!param.BindAsParameter(m_hStmt,i+1, pRow, pNew))
    {      
      free(m_pRowset);
      m_pRowset = NULL;     
      return FALSE;
    }                        
		pRow = pNew + sizeof(SQLINTEGER);
  }
  return TRUE;
}

BOOL CODBCDatabase::ConfigDataSource(WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes, HWND hwndParent)
{
  return SQLConfigDataSource(hwndParent, fRequest, lpszDriver, lpszAttributes);
}

CString CODBCDatabase::GetDriverVer()
{
    CString ver;
    if (m_bIsConnected)
    {
        SQLSMALLINT Len;
        SQLGetInfo(m_hDbc, SQL_DRIVER_VER, ver.GetBufferSetLength(50), 50, &Len);
        ver.ReleaseBuffer(Len);
    }
    return ver;
}

void* CODBCRecordset::GetBlobBuffer()
{
    if (!m_pBlobBuffer)
        m_pBlobBuffer = malloc(BLOB_BUFFER_LEN);
    return m_pBlobBuffer;
}

BOOL CDBField::GetLongData(int n)
{
    SQLRETURN ret = SQL_SUCCESS_WITH_INFO;
    if (m_nSQLType == SQL_LONGVARBINARY)
    {
        if (m_pstream)
            m_pstream->Release();

//TODO: BLOB typification
        if (CreateStreamOnHGlobal(NULL, TRUE, &m_pstream) != S_OK)
        {
            m_pstream = NULL;
            return FALSE;
        }
        void* pBuffer = m_pRS->GetBlobBuffer();
        while (ret == SQL_SUCCESS_WITH_INFO)
        {
            ret = SQLGetData(m_pRS->m_hStmt, n, SQL_C_BINARY, pBuffer, BLOB_BUFFER_LEN, &m_nLength);

            if (ret == SQL_ERROR)
                break;

            if (m_nLength == SQL_NULL_DATA)
                break;
        
            long len = ((m_nLength > BLOB_BUFFER_LEN) || (m_nLength == SQL_NO_TOTAL)) ? BLOB_BUFFER_LEN : m_nLength;
            if (m_pstream->Write(pBuffer, len, NULL) != S_OK)
                return FALSE;
        }
    }
    else //SQL_LONGVARCHAR || SQL_WLONGVARCHAR
    {
        SQLINTEGER PartLength = BLOB_BUFFER_LEN;
        m_nLength = 0;
        while (ret == SQL_SUCCESS_WITH_INFO)
        {
            char* pBuffer = m_pstring->GetBufferSetLength(m_nLength + PartLength);
        
            SQLINTEGER Length;
            ret = SQLGetData(m_pRS->m_hStmt, n, SQL_C_CHAR, pBuffer + m_nLength, PartLength + 1, &Length);

            if (ret == SQL_ERROR)
                break;

            if (Length == SQL_NULL_DATA)
            {
                m_nLength = Length;
                m_pstring->ReleaseBuffer(0);
                return TRUE;
            }
        
            if (Length == SQL_NO_TOTAL)
            {
                m_nLength += PartLength;
                Length = BLOB_BUFFER_LEN;
            }
            else if (Length > PartLength)
            {
                m_nLength += PartLength;
                PartLength = Length - PartLength;
            }
            else
                m_nLength += Length;
        }

        m_pstring->ReleaseBuffer(m_nLength);
    }
    return Check(ret);
}

BOOL CQueryParam::PutLongData(int n, void* pValuePtr)
{
    SQLRETURN ret = SQL_SUCCESS_WITH_INFO;
    if (m_nSQLType == SQL_LONGVARBINARY)
    {
        IStream* pIStream;
        if (pValuePtr)
            pIStream = *(IStream**)pValuePtr;
        else
            pIStream = m_pstream;

        void* pBuffer = m_pRS->GetBlobBuffer();

        if (pIStream)
        {
            LARGE_INTEGER dlibMove;
            dlibMove.QuadPart = 0;
            ULARGE_INTEGER libNewPos;
            if (pIStream->Seek(dlibMove, STREAM_SEEK_SET, &libNewPos) == S_OK)
            {
                ULONG cbRead;
                while (pIStream->Read(pBuffer, BLOB_BUFFER_LEN, &cbRead) == S_OK)
                {
                    if (cbRead == 0)
                        break;

                    ret = SQLPutData(m_pRS->m_hStmt, pBuffer, cbRead);
                    if (!Check(ret))
                        break;
                }
            }
            else
                return FALSE;
        }
        else
            ret = SQLPutData(m_pRS->m_hStmt, pBuffer, SQL_NULL_DATA);
    }
    else //SQL_LONGVARCHAR || SQL_WLONGVARCHAR
    {
        CString* pString;
        if (pValuePtr)
            pString = *(CString**)pValuePtr;
        else
            pString = m_pstring;
        
        if (pString)
            ret = SQLPutData(m_pRS->m_hStmt, (void*)pString->operator LPCTSTR(), pString->GetLength());
        else
            ret = SQLPutData(m_pRS->m_hStmt, NULL, SQL_NULL_DATA);
    }
    return Check(ret);
}

CType CDBField::Get1CType()
{
    if (m_BLCTypeName.GetLength() > 0)
        return CType(0);

    int TypeCode = m_rt.GetTypeCode();
    if (TypeCode < 0 || TypeCode > 17)
    {

        switch(m_nSQLType)
        {
        case SQL_BIT:
        case SQL_TINYINT:
        case SQL_SMALLINT:
        case SQL_INTEGER:
        case SQL_REAL:
        case SQL_FLOAT:
        case SQL_DOUBLE:
        case SQL_NUMERIC:
        case SQL_DECIMAL:
        case SQL_BIGINT:
            return CType(1);
            break;

        case SQL_DATE:
        case SQL_TIMESTAMP:
        case SQL_TYPE_DATE:
        case SQL_TYPE_TIMESTAMP:
            return CType(3);
            break;

        case SQL_TIME:
        case SQL_TYPE_TIME:
        case SQL_CHAR:
        case SQL_VARCHAR:
        case SQL_LONGVARCHAR:
        case SQL_WCHAR:
        case SQL_WVARCHAR:
        case SQL_WLONGVARCHAR:
            return CType(2);
            break;

        case SQL_BINARY:
        case SQL_VARBINARY:
        case SQL_LONGVARBINARY:
        default:
            return CType(0);
        };
    };
    return m_rt;
}

SQLRETURN CODBCRecordset::ExecuteStatement(const char* szSqlStr, BOOL bScroll, BOOL bPrep)
{
	if (!bPrep)
	{
		m_sLastError.Empty();
		if (m_bIsOpen)
            Close();
		if (!AllocStmt())
            return SQL_ERROR;
	}
	else
	{
		if (m_bIsOpen && m_hStmt)
			if (!Check(SQLCancel(m_hStmt)))
                return  SQL_ERROR;
	};

	if (!BindParams())
        return SQL_ERROR;

	if (bScroll)
		if (!Check(SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_SCROLLABLE,SQL_NTS)))
			return SQL_ERROR;

	SQLRETURN ret;
	if (!bPrep)
		ret = SQLExecDirect(m_hStmt, (SQLCHAR*)szSqlStr, SQL_NTS);
	else
		ret = SQLExecute(m_hStmt);

    return ProcessDataAtExec(ret);
}

SQLRETURN CODBCRecordset::ExecuteXMLStatement(const char* szSqlStr, BOOL bPrep)
{
    if (!Check(ExecuteStatement(szSqlStr, FALSE, bPrep)))
        return FALSE;

    SQLSMALLINT nResultCols;
	SQLRETURN ret;

    ret = SQLNumResultCols(m_hStmt, &nResultCols);
    if (!Check(ret) && nResultCols != 1)
    {
        m_sLastError = "Запрос не XML.";
        return SQL_ERROR;
    }

    SQLSMALLINT nSQLType, nScale, nNullability;
    SQLUINTEGER nPrecision;

	ret = SQLDescribeCol(m_hStmt, 1,
		NULL, 0, NULL,
		&nSQLType,
		&nPrecision,
		&nScale,
		&nNullability);

    if (nSQLType != SQL_WLONGVARCHAR)
    {
        m_sLastError = "Запрос не XML.";
        return SQL_ERROR;
    };
    return ret;
}

BOOL CODBCRecordset::ExecuteXML(const char* szSqlStr, BOOL bPrep, CString& sResult)
{
    SQLRETURN ret = ExecuteXMLStatement(szSqlStr, bPrep);
    if (!Check(ret))
        return FALSE;

    SQLINTEGER nLength;
    long Len = 0;
    while (Check(ret))
    {
        ret = SQLFetch(m_hStmt);

        while (Check(ret))
        {
            LPSTR pStr = sResult.GetBufferSetLength(Len + BLOB_BUFFER_LEN);
            ret = SQLGetData(m_hStmt, 1, SQL_C_CHAR, pStr + Len, BLOB_BUFFER_LEN, &nLength);

            if (ret == SQL_ERROR)
                break;

            if (ret == SQL_NO_DATA)
            {
                ret = SQL_SUCCESS;
                break;
            };
            Len += ((nLength > BLOB_BUFFER_LEN) || (nLength == SQL_NO_TOTAL)) ? BLOB_BUFFER_LEN : nLength;
        }
    };
    sResult.ReleaseBuffer(Len);

    return ret == SQL_NO_DATA;
}

BOOL CODBCRecordset::ExecuteXML(const char* szSqlStr, BOOL bPrep, CBinaryData& Result)
{
    SQLRETURN ret = ExecuteXMLStatement(szSqlStr, bPrep);
    if (!Check(ret))
        return FALSE;

    IStream* pStream = Result.GetIStream();
    SQLPOINTER pBuffer = GetBlobBuffer();
    SQLINTEGER nLength;
    long Len;

    SQLSMALLINT type;
    switch(Result.m_Encoding)
    {
    case CBinaryData::ANSI:
        type = SQL_C_CHAR;
        break;
    case CBinaryData::OEM:
        type = SQL_C_CHAR;
        break;
    case CBinaryData::UTF_16:
        type = SQL_C_WCHAR;
        break;
    case CBinaryData::UTF_8:
        type = SQL_C_WCHAR;
        break;
    default:
        m_sLastError = "Недопустимая кодировка.";
        return FALSE;
    };

    CString sUtf8;

    while (Check(ret))
    {
        ret = SQLFetch(m_hStmt);

        while (Check(ret))
        {
            ret = SQLGetData(m_hStmt, 1, type, pBuffer, BLOB_BUFFER_LEN, &nLength);

            if (ret == SQL_ERROR)
                break;

            if (ret == SQL_NO_DATA)
            {
                ret = SQL_SUCCESS;
                break;
            };

            Len = ((nLength > BLOB_BUFFER_LEN) || (nLength == SQL_NO_TOTAL)) ? BLOB_BUFFER_LEN : nLength;
            
            void* pData;
            long DataLen;

            switch(Result.m_Encoding)
            {
            case CBinaryData::ANSI:
                pData = pBuffer;
                DataLen = Len;
                break;
            case CBinaryData::OEM:
                CharToOem((LPSTR)pBuffer, (LPSTR)pBuffer);
                pData = pBuffer;
                DataLen = Len;
                break;
            case CBinaryData::UTF_16:
                pData = pBuffer;
                DataLen = Len;
                break;
            case CBinaryData::UTF_8:
                DataLen = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pBuffer, Len / 2, NULL, 0, NULL, NULL);
                pData = sUtf8.GetBufferSetLength(DataLen);
                WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pBuffer, Len / 2, (LPSTR)pData, DataLen, NULL, NULL);
                break;
            };

            if (pStream->Write(pData, DataLen, NULL) != S_OK)
            {
                m_sLastError = "Ошибка записи в файл.";
                ret = SQL_ERROR;
                break;
            };

        }
    };

    return ret == SQL_NO_DATA;
}

BOOL CODBCRecordset::ParseV7FieldType(CString& strName, CType& Type, e1CAddType& addType, CString& strBLCTypeName)
{
    int nPos = strName.Find('$');
	if (nPos >= 0)
	{
        CString strType1C(strName.operator LPCSTR() + nPos + 1);
		if (!CMetaDataWork::SetTypeAndKind(Type, strType1C))
            return FALSE;

		strName.GetBufferSetLength(nPos);
		strName.TrimRight();

        switch (Type.GetTypeCode())
        {
        case UNDEFINE_TYPE_1C:
            if (!strType1C.CompareNoCase("Субконто")|| !strType1C.CompareNoCase("Subconto"))
                addType = eSbKind;
            else if (!strType1C.CompareNoCase("ВидДокумента")|| !strType1C.CompareNoCase("DocumentKind"))
			{
                addType = eDocKindAsStr;
				Type.SetTypeCode(STRING_TYPE_1C);
			}
            else if (!strType1C.CompareNoCase("ВидДокументаПредставление")|| !strType1C.CompareNoCase("DocumentKindPresent"))
			{
                addType = eDocKindPresentAsStr;
				Type.SetTypeCode(STRING_TYPE_1C);
			}
            break;
            /*
        case DOCUMENT_TYPE_1C:
            if (Type.GetTypeID() == 0 && nPrecision != 13)
            {
                if (nPrecision == 9)
                    addType = eDocKind;
                else
                    return FALSE;
            }
            break;
            */
        case AGREGATE_TYPE_1C:
            {
                char const * szAggType = strchr(strType1C, '.');
                if (szAggType)
                {
                    strBLCTypeName = szAggType + 1;
                    Type.SetTypeCode(-1);
                }
                else
                    return FALSE;
            }
            break;
        }
    }
    return TRUE;
}

BOOL CODBCRecordset::TypifyField(int nIndex, CDBField& field)
{

    if (!CODBCRecordset::ParseV7FieldType(field.m_strName, field.m_rt, field.m_AddType, field.m_BLCTypeName))
    {
        m_sLastError.Format("Ошибка типизации поля %s.", (LPCSTR)field.m_strName);
        return FALSE;
    }

    if (field.IsLongSQLType() && m_nRowsetSize > 1)
    {
        m_sLastError.Format("При использовании массива строк получение длинных полей невозможно (%s)", (LPCSTR)field.m_strName);
        return FALSE;
    }
    return TRUE;
}

BOOL CODBCRecordset::PrepareFields()
{
    for (int i = 0; i < m_nResultCols; i++)
    {
        CDBField& dbf = m_rgFields[i];
        if (dbf.m_rt.GetTypeCode() == DOCUMENT_TYPE_1C && dbf.m_rt.GetTypeID() == 0 && dbf.m_nPrecision == 9)
            dbf.m_AddType = eDocKind;

        if (dbf.m_AddType == eDocKind || dbf.m_AddType == eSbKind)
        {
            int AddFieldIndex = GetFieldIndex(dbf.m_strName + KindSuffixEng);
            if (AddFieldIndex >= 0)
                dbf.m_AddFieldIndex = AddFieldIndex;
            else
                dbf.m_AddFieldIndex = GetFieldIndex(dbf.m_strName + KindSuffixRus);

            if (dbf.m_AddFieldIndex < 0)
            {
                m_sLastError.Format("Для типизации поля \"%s\", не найдено дополнительное типизирующее поле.", (LPCSTR)dbf.m_strName);
                return FALSE;
            };
            if (dbf.m_AddType == eSbKind)
                m_rgFields[dbf.m_AddFieldIndex].m_rt.SetTypeCode(SUBCONTOKIND_TYPE_1C);
        }
    };

    return TRUE;
}
