// MySQL.h: interface for the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_02)
#define AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_02

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MySQL1.h"


#pragma comment(lib, "mysql\\lib\\oraclm32.lib")
#include "oracl-h\oracl.h"


class CMySQL2 :public CMySQL1  
{
public:
	CMySQL2();
	~CMySQL2();

 	//Новые методы и атрибуты
	ODatabase m_odb;
    ODynaset m_dynaset;


	//Старые - переропределенные
	int Connect(CString csHost,CString csBase,CString UserName=_T("root"),CString Password=_T(""),int nType=0);
	int Query(CString SQLQuery,CString csFNames="");
	int CountFields();
	CString FieldName();
	int GetFieldName();
	CString GetFieldName(int n);
	int Move(int nRow);
	int CountRows();
	CString GetString(int Number);
	long GetLong(int Number);
	double GetDouble(int Number);
	int GetLine();
	int MoveFirst();

	void Reset();
	int IsTableExist(CString csTableName);

/*	int nCurLine;
	void SetTypeConnection(int nType)	{	nConnectionType=nType;};
	int _Query(CString SQLQuery,CString csFNames);
	CString _GetValue(int Number);
*/
};


#endif // !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_02)
