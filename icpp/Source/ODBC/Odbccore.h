//  MODULE:   odbccore.h
//
//	AUTHOR: Carlos Antollini 
//
//  mailto: cantollini@hotmail.com
//
//	Date: 08/21/2001
//
//	Version 1.11
// 

#ifndef _odbc_code_def
#define _odbc_code_def

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <afxdb.h>
#include <afxdb_.h>
#include <time.h>
#include "SQLNumeric.h"
#include "..\BinaryData.h"

//#include "sqlint.h"

class CIBSet;
class CIBSetSQL;
class CIBSetDBF;
class CODBCDatabase;
class CDBField;
class CQueryParam;
class CODBCRecordset;

class CODBCDatabase
{
public:
	enum drvCompletion
	{
		sqlNoPrompt = SQL_DRIVER_NOPROMPT,
		sqlPrompt = SQL_DRIVER_PROMPT
	};

protected:
	void SQLFree();
	void SQLAlloc();

public:
	CODBCDatabase();
	CODBCDatabase(SQLHDBC hDbc);
	BOOL Attach(SQLHDBC hDbc);
	~CODBCDatabase();

	SQLHDBC m_hDbc;
	BOOL m_bIsAttached;
	CString m_sLastError;

protected:
	LONG m_lLoginTimeout;
	LONG m_lConnectionTimeout;
	BOOL m_bIsConnected;
	SQLHENV m_hEnv;
	//CODBCRecordset* m_pCurrent;

public:
//trad
    CMetaDataCont* m_pMetaDataCont;
    CIBSet* m_pCurrentIBSet;
    void SetIBSet(CIBSet* pIBSet);
//

public:
	static BOOL ConfigDataSource(WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes, HWND hwndParent = NULL);
	BOOL Open(const char* lpstrDSN, CHAR* lpstrUser = NULL, CHAR* lpstrPass = NULL);
	BOOL DriverConnect(const char* szConnStr, CHAR* szConnStrOut = NULL, HWND hWnd = NULL, enum drvCompletion drvConn = sqlNoPrompt);
	void SetReadOnly(BOOL bReadOnly = TRUE);
	void SetConnectionTimeout(LONG nSeconds);
	LONG GetConnectionTimeout();
	void SetLoginTimeout(LONG nSeconds)	{m_lLoginTimeout = nSeconds;};
	BOOL IsConnected() {return m_bIsConnected;};
	void Close();
	const char* GetLastError();
	//void SetCurrent(CODBCRecordset* pCurrent);
	//void ResetCurrent();
	//CODBCRecordset* GetCurrent(){ return m_pCurrent;}
//DmitrO
private:
    char m_DriverODBCVer[6];
    void GetDriverInfo();
    BOOL m_NeedLongDataLen;
public:
    const char* GetDriverODBCVer() { return m_DriverODBCVer;};
    CString GetDriverVer();
    BOOL GetNeedLongDataLen() { return m_NeedLongDataLen;};
//
};

//DmitrO
#define BLOB_BUFFER_LEN (8*1024)
enum e1CAddType { eStandart, eDocKind, eSbKind, eDocKindAsStr, eDocKindPresentAsStr, eRefKindAsStr, eRefKindPresentAsStr };
const char KindSuffixEng[] = "_kind";
const char KindSuffixRus[] = "_вид";
//

class	CDBField
{
    friend class CODBCRecordset;
public:
	CDBField();
	~CDBField();

	void Fixup();
	void SetCType(bool NumericEnabled);
	//void SetValue(CValue& rVal,BOOL bAgg);
	//CDBField& operator =( const CValue& bVal );
    BOOL BindToColumn(int n, BYTE* pRowset);

	CDBField& operator =( const bool bVal );
	CDBField& operator =( const char chVal );
	CDBField& operator =( const unsigned char chVal );
	CDBField& operator =( const short sVal );
	CDBField& operator =( const int iVal );
	CDBField& operator =( const long lVal );
	CDBField& operator =( const float fVal );
	CDBField& operator =( const double fVal );
	CDBField& operator =( const COleDateTime& dtVal );
	CDBField& operator =( const CString& cVal );
	CDBField& operator =( const LPCTSTR szVal );
	CDBField& operator =( IStream* pIStream );
	CDBField& operator =( const GUID* pGUID );

	//	Get the bool value. Do appropriate conversion.
	bool			AsBool()	const;
	//	Get the unsigned char value. Do appropriate conversion.
	unsigned char	AsChar()	const;
	//	Get the short value. Do appropriate conversion.
	short			AsShort()	const;
	//***	Get the int value which is equal to long value. Do appropriate conversion.
	int				AsInt()		const;
	//	Get the long value. Do appropriate conversion.
	long			AsLong()	const;
	//	Get the float value. Do appropriate conversion.
	float			AsFloat()	const;
	//	Get the double value. Do appropriate conversion.
	double			AsDouble()	const;
	//	Get the date and time value. Do appropriate conversion.
	COleDateTime	AsDate()	const;
	//	Get the CString value. Do appropriate conversion.
	CString			AsString()	const;
    //	Get the IStream value. Only for SQL_C_TIMESTAMP
	const TIMESTAMP_STRUCT*	AsTimeStamp()	const;
	//	Get the IStream value. Only for SQL_C_BINARY
	IStream*		AsIStream()	const;
	//	Get the IStream value. Only for SQL_C_BINARY
	const GUID*		AsGUID()	const;

//DmitrO
	//	Get the 1C numeric value. Do appropriate conversion.
	CNumeric AsNumeric() const;
    void CDBField::GetValue(CValue& value, BYTE* pData = NULL) const;
	CDBField& operator =(const CValue& value);
//
	//	Data type conversion operators
	operator	bool() const;
	operator	unsigned char() const;
	operator	short() const;
	operator	int() const;
	operator	long() const;
	operator	float() const;
	operator	double() const;
	operator	COleDateTime() const;
	operator	CString() const;

	//	Field type query methods
	BOOL	IsNull()	const	{ return m_bIsNull; };
	bool	IsBool()	const	{ return m_nCType == SQL_C_BIT; };
	bool	IsChar()	const	{ return m_nCType == SQL_C_UTINYINT; };
	bool	IsShort()	const	{ return m_nCType == SQL_C_SSHORT; };
	bool	IsInt()		const	{ return m_nCType == SQL_C_SLONG; };
	bool	IsLong()	const	{ return m_nCType == SQL_C_SLONG; };
	bool	IsFloat()	const	{ return m_nCType == SQL_C_FLOAT; };
	bool	IsDouble()	const	{ return m_nCType == SQL_C_DOUBLE; };
	bool	IsNumber()	const	{ return IsShort() || IsLong() || IsFloat() || IsDouble(); };
	bool	IsDate()	const	{ return m_nCType == SQL_C_TIMESTAMP; };
	bool	IsString()	const	{ return m_nCType == SQL_C_CHAR; };
	bool	IsBinary()	const	{ return m_nCType == SQL_C_BINARY; };

    BOOL IsLongSQLType() const
    {
        return m_nSQLType == SQL_LONGVARCHAR || m_nSQLType == SQL_WLONGVARCHAR
            || m_nSQLType == SQL_LONGVARBINARY;
    };
protected:
    CODBCRecordset* m_pRS;
	SQLPOINTER GetBuffer();

protected:
	union
	{
	  BOOL              m_boolVal;
	  unsigned char     m_chVal;
	  short             m_iVal;
	  long              m_lVal;
	  float             m_fltVal;
	  double            m_dblVal;
	  TIMESTAMP_STRUCT* m_pdate;
	  CString*          m_pstring;
//DmitrO
	  SQL_NUMERIC_STRUCT* m_pnumeric;
	  SQL_TIME_STRUCT* m_ptime;
	  IStream*          m_pstream;
	  GUID*             m_pguid;
//
	};

public:	
	// ODBC info
	CString m_strName;
	SWORD m_nSQLType;
	UDWORD m_nPrecision;
	SWORD m_nScale;
	SWORD m_nNullability;
	// Ext info
	BOOL m_bIsNull;
	long m_nLength;
	short m_nCType;
	SQLINTEGER m_nBufSize;
	//
	CType m_rt;
    e1CAddType m_AddType;
    int m_AddFieldIndex;
    CString m_BLCTypeName;

    BOOL GetLongData(int n);
    CType Get1CType();
};

/////////////////////////////////////////////////////////////////////////////
inline
CDBField::operator	bool() const {
	return	AsBool();
}
inline
CDBField::operator	unsigned char() const {
	return	AsChar();
}
inline
CDBField::operator	short() const {
	return	AsShort();
}
inline
CDBField::operator	int() const {
	return	AsInt();
}
inline
CDBField::operator	long() const {
	return	AsLong();
}
inline
CDBField::operator	float() const {
	return	AsFloat();
}
inline
CDBField::operator	double() const {
	return	AsDouble();
}
inline
CDBField::operator	COleDateTime() const {
	return	AsDate();
}

inline
CDBField::operator	CString() const {
	return	AsString();
}

class CQueryParam : public CDBField
{  
public:  
	int m_nIOtype;
public:
    int m_Modificator;
    CQueryParam():m_Modificator(0) {};
//
	BOOL GetBufferAndSize(void **pointer, int &size);
	BOOL BindAsParameter(SQLHSTMT hStmt,int n, BOOL NeedLongDataLen);
  BOOL BindAsParameter(SQLHSTMT hStmt,int n, void *pBuffer, void *pInd);
	BOOL BindAsNamedParameter(SQLHSTMT hStmt,int n);  
    BOOL PutLongData(int n, void* pValuePtr);
    void SetDataAtExec(BOOL NeedLongDataLen, long DataLength);
};

typedef CArray<CQueryParam,CQueryParam&> QueryParamArray;

/*struct SPQualif
{
	const char* spName;
	const char* spDb;
	const char* spOwner;
};*/

class CODBCRecordset
{
    friend class CDBField;
    friend class CQueryParam;
public:
	void SetDatabase(CODBCDatabase* pDb);

	CODBCRecordset();
	CODBCRecordset(CODBCDatabase* pDb);
	~CODBCRecordset() { Close(TRUE);};

	SQLHSTMT AttachRecordset(CODBCDatabase* pDb);
	BOOL BindColumns(int nBindCount,int aMap[]);
	BOOL Open(const char* szSqlStr,BOOL bScroll = FALSE,BOOL bPrep=FALSE);
	SQLINTEGER Execute(const char* szSqlStr,BOOL bPrep=FALSE);
	BOOL Prepare(const char* szSqlStr);
	void Close(BOOL bDetachBase = FALSE);
	const char* GetLastError();

	inline CDBField& operator[](int nField) const
	{ 
		ASSERT(nField>=0 && nField<m_nResultCols);
		return m_rgFields[nField];
	}
	inline CDBField& operator[](const char* szFieldName) const
	{
		int nField = GetFieldIndex(szFieldName);
		ASSERT(nField>=0 && nField<m_nResultCols);
		return m_rgFields[nField];
	}

  inline int CODBCRecordset::GetFieldCount()const {	return m_nResultCols;}
  inline int GetRowsAffected() {return m_nRowsAffected;};

//	BOOL BuildParamsSP(SPQualif& spq,int nParams,param_list* params);
	BOOL BuildParams();
  //	void BindParamData(param_list* params);
  inline int ParamCount() const{ return m_aParams.GetSize();}
  BOOL AddParam(const char* pname,int IOType,SWORD nSQLType,CType const& type,int nLen,SWORD nDec);

	void DeleteParam(int nParam){ m_aParams.RemoveAt(nParam);}
	void ClearParams(){ m_aParams.SetSize(0);}
	CQueryParam& GetParam(int nParam) { return m_aParams[nParam];}

	BOOL MoveFirst();
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveLast();

	inline BOOL IsEof() const {return m_bIsEOF;};
	inline BOOL IsBof() const  {return m_bIsBOF;};
	inline BOOL IsOpen() const  {return m_bIsOpen;};

	void SetRowsetSize(int nRows);
	inline int GetRowsetSize() const {	return m_nRowsetSize;}
	inline SQLUSMALLINT GetRowStatus(int nRow) const {	return m_pRowstatus[nRow];}
	inline int GetRowSize() const { return m_nRowSize;}
	inline BYTE* GetRowset() const { return m_pRowset;}
	inline SQLUINTEGER GetRowFetched()const {	return m_nRowFetched;}
	inline int GetOffset(int i) {	return m_pOffsets[i];}
	inline CDBField* GetFields() {	return m_rgFields;}
	inline const int* GetOffsets()	{ return m_pOffsets;}
	
	//void SetValue(CValue& rVal,CDBField& dbf,BYTE* pRow,BOOL bAgg);
public:	
  //CursorScrollable:
    //SQL_NONSCROLLABLE 
    //SQL_SCROLLABLE
  //CursorType:
    //SQL_CURSOR_FORWARD_ONLY (default)
    //SQL_CURSOR_STATIC 
    //SQL_CURSOR_KEYSET_DRIVEN 
    //SQL_CURSOR_DYNAMIC     
  //Sensitivity
    //SQL_UNSPECIFIED 
    //SQL_INSENSITIVE 
    //SQL_SENSITIVE     
  //SQL_ATTR_CURSOR_SCROLLABLE

	void SetCursorsTypes(SQLUINTEGER CursorType = SQL_CURSOR_FORWARD_ONLY);
  void SetCursorsScrollable(SQLUINTEGER CursorScrollable = SQL_NONSCROLLABLE);
  void SetCursorsSensitivity(SQLUINTEGER CursorSensitivity = SQL_UNSPECIFIED); 
  void SetCursorConcurrency(SQLUINTEGER CursorConcurrency = SQL_CONCUR_READ_ONLY);

//DmitrO - return value
  BOOL SetRowsetSizeForParams(int nRows);
//
  void ResizeRowsetSizeForParams(int nRows);
  void SetParamOfRow(int nRow, int nNumParam, int nSize, void *pValue);

  CString m_sLastError;		
protected:
  SQLHSTMT m_hStmt;
	int m_nRowsAffected;
	SWORD m_nResultCols;
	CDBField* m_rgFields;
    int m_nDriverODBCMajorVer;
	
	int m_nRowsetSize;
	int m_nRowSize;
	SQLUINTEGER m_nRowFetched;
	BYTE* m_pRowset;
	BYTE* m_pRowstatus;
	int* m_pOffsets;

	BOOL m_bIsOpen;
	BOOL m_bIsEOF;
	BOOL m_bIsBOF;
	CODBCDatabase* m_pDb;

  QueryParamArray m_aParams;
	int m_nBindType;

  int m_nRowsetSizeForParams;
  SQLUSMALLINT m_nParamsProcessed;

protected:
	BOOL AllocStmt();
	BOOL CreateFields();
	BOOL CreateFieldsMapped(short nBindCount,int aMap[]);
	BOOL BindParams();
	void Fixup();
	void FixupParams();
private:
	BOOL BuildRowsetForParams();
//DmitrO
	BOOL AllocateRowsetForParams();
//
  static int pm_named;
  static int pm_pos;
  CMapStringToPtr mapName_IndexFields;
  bool m_isSetType;
  bool m_isSetCursorsScrollable;
  bool m_isSetCursorsSensitivity;
  bool m_isSetCursorConcurrency;

  SQLUINTEGER m_CursorType;
  SQLUINTEGER m_CursorScrollable;
  SQLUINTEGER m_CursorSensitivity;
  SQLUINTEGER m_CursorConcurrency;
  int m_nAllocRowSizeForParams;
//DmitrO
  int m_QueryTimeout;
  bool m_IsQueryTimeoutExpired;
public:
  void SetQueryTimeout(int nSeconds)
  { m_QueryTimeout = nSeconds; };
  bool IsQueryTimeoutExpired()
  { return m_IsQueryTimeoutExpired; };
private:
    void* m_pBlobBuffer;
    void* GetBlobBuffer();
    SQLRETURN ProcessDataAtExec(SQLRETURN ret);
    BOOL PrepareFields();
private:
    SQLRETURN ExecuteStatement(const char* szSqlStr, BOOL bScroll = FALSE, BOOL bPrep=FALSE);
    SQLRETURN ExecuteXMLStatement(const char* szSqlStr, BOOL bPrep=FALSE);
public:
    BOOL ExecuteXML(const char* szSqlStr, BOOL bPrep, CString& sResult);
    BOOL ExecuteXML(const char* szSqlStr, BOOL bPrep, CBinaryData& Result);
    static BOOL ParseV7FieldType(CString& strName, CType& Type, e1CAddType& addType, CString& strBLCTypeName);
protected:
    virtual BOOL TypifyField(int nIndex, CDBField& field);
    virtual BOOL InternalMapFields() const {return TRUE;};
    virtual int GetFieldIndex(const char* szFieldName) const;
    virtual CBLContext* GetVCtorParam() {return NULL;};
//
};

#endif
