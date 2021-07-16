// MySQL.h: interface for the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_01)
#define AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_01

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "winsock2.h"
#include "afxdb.h"
#include "ODBCRecordset.h"
#include "MySQL0.h"


#ifndef __1C__
//заглушка для не 1С компиляции
	class CODBCQuery
	{
	public:
		CODBCQuery()
		{
			ASSERT(FALSE);
		};
		int Prepare(CString Str,int a,int b)
		{
			ASSERT(FALSE);
			return 0;
		};
		int Open()
		{
			ASSERT(FALSE);
			return 0;
		};
		void GetString(int n,CString &Val)
		{
			ASSERT(FALSE);
		};
		void GetLong(int n,long &Val)
		{
			ASSERT(FALSE);
		};
		int GotoNext()
		{
			ASSERT(FALSE);
			return 0;
		};
	};
	class _CODBCQuery:public CODBCQuery
	{
	};
#endif


//extern CDatabase	cODBC;
class CMySQL1 :public CMySQL0  
{
public:
	CMySQL1();
	~CMySQL1();

 	//Новые методы и атрибуты
	CODBCQuery * pODBCQuery;
	static CDatabase	cODBC;
	CODBCRecordset	cRecord;
	static bool	bWasODBCConnected;
	int FetchData();

	int nCurLine;
	int IsTableExist(CString csTableName);


	void SetTypeConnection(int nType)	{	nConnectionType=nType;};
	//Старые - переропределенные
	int Connect(CString csHost,CString csBase,CString UserName=_T("root"),CString Password=_T(""),int nType=0);
	int Query(CString SQLQuery,CString csFNames="");
	int _Query(CString SQLQuery,CString csFNames);
	int CountFields();
	CString FieldName();
	int GetFieldName();
	CString GetFieldName(int n);
	int Move(int nRow);
	int CountRows();

	CString _GetValue(int Number);
	double GetDouble(int Number);
	CString GetString(int Number);
	long GetLong(int Number);
	int GetLine();
	void Reset();
	int MoveFirst();

	my_ulonglong GetInsertID();

	CString GetConnect();
	BOOL BeginTrans();
	BOOL CommitTrans();
	BOOL Rollback();

};


#endif // !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_01)
