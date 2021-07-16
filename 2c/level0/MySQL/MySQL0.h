// MySQL.h: interface for the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_00)
#define AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_00

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#pragma comment(lib, "mysql\\lib\\libmysql.lib")


#include "winsock2.h"
#include "mysql-h\mysql.h"
#include "afxdb.h"

#define MYSQL_CONNECTION	0
#define ODBCSQL_CONNECTION	1
#define RAINBOW_CONNECTION	2

#define MSSQL_ODBC			5
#define ORACLE_ODBC			6
#define ORACLE				7

class CMySQL0  
{
public:
	bool bCheckError;
	bool bError;
	CString csHost2;
	CString csBase2;
	CString UserName2;
	CString Password2;
	int MySQLPort2;
	CString csError;
	static int nConnectionType;
	
	CString *pFieldName;
	CString *pResultValue;
	int		nCountFields;
	int		nCurNField;
	int		nCurRow;
	int		nCountRows;

	int Seek(int offset);
	bool GetError(void){	return 	bError;	};
	int GetCountRows(){return nCountRows;};
	CString GetFieldName(int n);


	bool IsPresent(CString csHost,CString UserName,CString Password);

	//используемые поля

	//настоящий коннект:
	int Connect(CString csHost,CString csBase,CString UserName=_T("root"),CString Password=_T(""),int nType=0);
	int Query(CString SQLQuery);

	int CountFields();
	CString FieldName();
	int GetFieldName();

	CString _GetValue(int Number);
	long GetLong(int Number);
	double GetDouble(int Number);
	CString GetString(int Number);

	int GetLine();
	void Reset();
	
	my_ulonglong GetInsertID();

	CMySQL0();
	virtual ~CMySQL0();


	MYSQL_RES	* MySQLres ;
	MYSQL_FIELD	* MySQLfd ;
	MYSQL_ROW	MySQLrow ;

};

#endif // !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_00)
