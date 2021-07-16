// MySQL.cpp: implementation of the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef __1C__
//#include "MyDB.h" 
#endif
#include "MySQL2.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <odbcinst.h>
#include "..\OutToDebug.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Новый класс!!!
//Новый класс!!!
//Новый класс!!!
//Новый класс!!!
CMySQL2::CMySQL2()
{
}
CMySQL2::~CMySQL2()
{
	//delete pODBCRecordArray;
}
int CMySQL2::Connect(CString csHost,CString csBase,CString UserName,CString Password,int nType)
{
	if(nType!=ORACLE)
		return CMySQL1::Connect(csHost,csBase,UserName,Password,nType);

//	OServer srv;
//	srv.Open(csHost);

	OStartup();


	nConnectionType=ORACLE;
	//OSUCCESS
	return m_odb.Open(csBase, UserName, Password,ODATABASE_DEFAULT);//ODATABASE_NONBLOCK
}

int CMySQL2::Query(CString SQLQuery,CString csFNames)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::Query(SQLQuery,csFNames);

	m_dynaset.Open(m_odb,SQLQuery);
	return m_dynaset.IsOpen();
}

int CMySQL2::CountFields()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::CountFields();

	return 0; 
}
CString CMySQL2::FieldName()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::FieldName();
	else

	return "";
}
int CMySQL2::GetFieldName()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetFieldName();

	return 0;
}
CString CMySQL2::GetFieldName(int n)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetFieldName(n);

	return "";
}
int CMySQL2::Move(int nRow)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::Move(nRow);

	return 0;
}
int CMySQL2::CountRows()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::CountRows();

	return 0;
}


//________________________________________________________________________________
CString CMySQL2::GetString(int Number)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetString(Number);

	return "";
}

//________________________________________________________________________________
long CMySQL2::GetLong(int Number)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetLong(Number);

	return 0;
}

//________________________________________________________________________________
double CMySQL2::GetDouble(int Number)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetDouble(Number);

	return 0;
}

//________________________________________________________________________________
int CMySQL2::GetLine()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::GetLine();

	return 0;
}
int CMySQL2::MoveFirst()
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::MoveFirst();

	return 0;
}
void CMySQL2::Reset()
{
	if(nConnectionType!=ORACLE)
		CMySQL1::Reset();
}

int CMySQL2::IsTableExist(CString csTableName)
{
	if(nConnectionType!=ORACLE)
		return CMySQL1::IsTableExist(csTableName);

	return 0;
}
