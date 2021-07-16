// MySQL.h: interface for the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_)
#define AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySQL0.h"
#include "MySQL1.h"
//#include "MySQL2.h"//ORACLE SUPPORT
//class CMySQL :public CMySQL2//ORACLE SUPPORT
class CMySQL :public CMySQL1
{
public:
	static 	BOOL MySQLLoaded();
};


#endif // !defined(AFX_MYSQL_H__5EEE3195_2A86_4FD2_B99E_A61DB5BC5129__INCLUDED_)
