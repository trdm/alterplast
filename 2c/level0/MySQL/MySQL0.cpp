// MySQL.cpp: implementation of the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef __1C__
#include "..\MyDB.h" 
#endif
#include "MySQL0.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <odbcinst.h>


#include "..\OutToDebug.h"

MYSQL	*myData=NULL, mysql ;
CString lastHost;
CString lastBase;
CString lastUserName;
CString lastPassword;
int lastMySQLPort;

UINT GetProcessorTimeMySQL(UINT nDiv)
{
        UINT TikCount;
        UINT TikCountLo;
        UINT TikCountHi;
        __asm                                   
        {                                               
                RDTSC
                MOV     TikCountLo,EAX          
                MOV     TikCountHi,EDX          
        }       
        TikCount=(TikCountHi<<(32-nDiv))+(TikCountLo>>nDiv);
        return TikCount;

}



//#include "..\LIBMYSQL\LIBMYSQL.C"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CMySQL0::nConnectionType=MYSQL_CONNECTION;
CMySQL0::CMySQL0()
{
	bError=false;
	MySQLres=NULL;
	//myData=NULL;
	bCheckError=false;

	nCountFields=0;
	pFieldName=NULL;
	pResultValue=NULL;
	nCurNField=-1;
	//nConnectionType=MYSQL_CONNECTION;
	nCountRows=0;
}

CMySQL0::~CMySQL0()
{
	Reset();
}

void CMySQL0::Reset()
{
	if(pFieldName)
		delete []pFieldName;
	pFieldName=NULL;
	if(pResultValue)
		delete[]pResultValue;
	pResultValue=NULL;
	nCountFields=0;
	nCurNField=-1;

	if(MySQLres!=NULL)
		mysql_free_result(MySQLres);
	MySQLres=NULL;
	//if(myData) mysql_close(myData); 
	//myData=NULL;
}

CString ReadKey(CString Key1,CString Key2)
{
DWORD dw;
HKEY hKey;/* Указатель на ключ реестра */

/* Открываем нужный нам ключ реестра с правами только чтения */

if ( ERROR_SUCCESS != 
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,/* Корневой ключ */
Key1,
NULL,
NULL, 
0, 
KEY_READ, /* доступ к ключу только на чтение */
NULL,
&hKey,/* Сюда функция запишет указатель на ключ */
&dw))
    return _T("");

DWORD d;

/* сначала мы получаем размер нужных нам данных */
if ( ERROR_SUCCESS != 
	RegQueryValueEx(hKey,/* открытый ключ */
Key2,/* подключ */
NULL,
NULL,
NULL,
&d/* Размер значения указанного ключа */
))
{
	RegCloseKey(hKey);/* не забываем закрывать ключи реестра по окончании работы с ними */ 
	return _T("");
}


/* получаем сами данные */
CString csRes;
RegQueryValueEx(hKey,
Key2,
NULL,
NULL,
(unsigned char*)csRes.GetBuffer(d),/* заводим указатель на строку символов (длина определяется предыдущей функцией)*/
&d);

RegCloseKey(hKey);/* не забываем закрывать ключи реестра по окончании работы с ними */ 
csRes.ReleaseBuffer();
return csRes;
}

//имитация коннекта
int CMySQL0::Connect(CString csHost,CString csBase,CString UserName,CString Password,int MySQLPort)
{
	if(myData)
	if(csHost==lastHost&&lastBase==csBase&&lastUserName==UserName&&lastPassword==Password&&lastMySQLPort==MySQLPort)
		return 1;

	lastHost=csHost;
	lastBase=csBase;
	lastUserName=UserName;
	lastPassword=Password;
	lastMySQLPort=MySQLPort;


	if(csHost.IsEmpty())
		csHost=_T("localhost");
	bError=false;
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_OPT_COMPRESS,NULL);
	myData=mysql_real_connect( &mysql, csHost.GetBuffer(0), UserName.GetBuffer(0), Password.GetBuffer(0), NULL, MySQLPort,
			NULL, MYSQL_OPT_COMPRESS );


	if(myData!=NULL)
	{
		if ( mysql_select_db( myData,csBase.GetBuffer(0)) < 0 )
		{
			CString SQLQuery;
			SQLQuery.Format("CREATE DATABASE IF NOT EXISTS %s",csBase);
			if ( ! mysql_query( myData, SQLQuery.GetBuffer(0)) )
			{
				if ( mysql_select_db( myData,csBase.GetBuffer(0)) < 0 )
				{
					csError.Format("Can't select the database %s",csBase);
					bError=true;
					if(bCheckError)
						AfxMessageBox(csError);
					return 0;
					
				}
			}
			else
			{
					csError.Format("Can't create the database %s",csBase);
					bError=true;
					if(bCheckError)
						AfxMessageBox(csError);
					return 0;
			}
		}

		return 1;
	}
	else//попытка запустить сервер, если он находится на локальной машине и была запись в реестре
	{
		CString csFindError=_T("Can't connect to MySQL server on");
		csError=mysql_error(&mysql);
		//OutToDebug("%s\n",csError);
		if(csError.Find(csFindError)>=0)
		{
			csHost.MakeLower();
			csHost.TrimLeft();
			csHost.TrimRight();
			static bool boolWasShelExecute=false;
			//OutToDebug("csHost=%s\n",csHost);
			if(!boolWasShelExecute&&csHost==_T("localhost"))//запускаем MySQL вручную и только один раз
			{
				boolWasShelExecute=true;
				//OutToDebug("OK\n");

				CString csPath;
					csPath=ReadKey("Software\\MySQL\\SERVER","PathMySQL");
				if(csPath.IsEmpty())
					csPath=ReadKey("Software\\PB\\OLAP77_SERVER","PathMySQL");

				//OutToDebug("csPath=%s\n",csPath);

				if(!csPath.IsEmpty())
				if(csPath[0]!='{')
				if(csPath[0]!='[')
				{  
					CString csFileName;
					csFileName.Format("%s\\mysqld.exe",csPath);
					//ShellExecute(NULL,NULL,csFileName,NULL,NULL,SW_HIDE);
					ShellExecute(NULL,NULL,csFileName,"--standalone",NULL,SW_HIDE);
					Sleep(500);
					return Connect(csHost,csBase,UserName,Password,MySQLPort);
				}
			}
			if(boolWasShelExecute)
			{
				static int nCountRun=0;
				nCountRun++;
				Sleep(500);
				if(nCountRun<10)
					return Connect(csHost,csBase,UserName,Password,MySQLPort);
			}

		}

		bError=true;
		if(bCheckError)
			AfxMessageBox(csError);
		return 0;
	}
	return 0;
}


int CMySQL0::Query(CString SQLQuery)
{
	nCountRows=0;
	bError=false;
	int Res=0;


	//OutToDebug("..0..%s..\n",SQLQuery);
	if ( ! mysql_query( myData, SQLQuery.GetBuffer(0)) )
		{
			if(MySQLres!=NULL)
				mysql_free_result(MySQLres);
			MySQLres=NULL;

			//OutToDebug("..1..");
			MySQLres = mysql_store_result( myData ) ;
			//MySQLres = mysql_use_result( myData ) ;
			//OutToDebug("..2..");
			if(MySQLres!=NULL)
			{
				Res= (int)mysql_num_rows( MySQLres);

				//заполняем массив названий колонок
				nCountFields=mysql_num_fields(MySQLres);
				if(pFieldName)
					delete []pFieldName;
				pFieldName=new CString[nCountFields];
				for(int i=0;i<nCountFields;i++)
				{
					MySQLfd = mysql_fetch_field(MySQLres);
					pFieldName[i]=MySQLfd->name;
				}
				//nCountFields--;

			}
		}
	else
	{
		//OutToDebug("..error..");
		csError=mysql_error(myData);
		bError=true;
		if(bCheckError)
			AfxMessageBox(csError);
		if(csError=="Lost connection to MySQL server during query")
		{
			OutToDebug("%s\n",csError);
			OutToDebug("Попытка повторного соединения...\n");

			CString csHost=lastHost;
			CString csBase=lastBase;
			CString UserName=lastUserName;
			CString Password=lastPassword;
			int MySQLPort=lastMySQLPort;
			lastHost="";
			lastBase="";
			int nRes=Connect(csHost,csBase,UserName,Password,MySQLPort);
			if(nRes==1)
			{
				return Query(SQLQuery);
			}
			OutToDebug("%s\n",csError);
		}
	}

	nCountRows=Res;

	if(Res>=1)
		Res=1;
	return Res;
}

my_ulonglong CMySQL0::GetInsertID()
{
	return mysql_insert_id(myData);
}


CString CMySQL0::_GetValue(int Number)
{
	if(nCountRows==0)
		return "";

	if(MySQLrow!=NULL)
		return MySQLrow[Number];
	return _T("");
}
long CMySQL0::GetLong(int Number)
{
	return atoi(_GetValue(Number));
}
double CMySQL0::GetDouble(int Number)
{
	return atof(_GetValue(Number));
}
CString CMySQL0::GetString(int Number)
{
	return _GetValue(Number);
}

int CMySQL0::GetLine()
{
	if(nCountRows==0)
		return 0;

	MySQLrow = mysql_fetch_row( MySQLres );
	if(MySQLrow!=NULL)
		return 1;
	else
	{
		mysql_data_seek(MySQLres,0);//новый цикл выборки
		return 0;
	}
}
int CMySQL0::Seek(int offset)
{
	mysql_data_seek(MySQLres,offset-1);
	return GetLine();
}
/*
CString CMySQL0::Value(int i,int j)
{
	if(MySQLrow!=NULL)
		return MySQLrow[Number];
	return _T("");
}
*/

int CMySQL0::GetFieldName()
{
	nCurNField++;
	if(nCurNField<nCountFields)
		return 1;
	else
		return 0;
}

CString CMySQL0::GetFieldName(int n)
{
	if(pFieldName)
	{
		if(n<nCountFields)
		{
			return pFieldName[n];
		}
	}
	return _T("");
}

CString CMySQL0::FieldName()
{
	if(nCurNField<nCountFields)
		return pFieldName[nCurNField];
	else
		return _T("");
}

int CMySQL0::CountFields()
{
   return nCountFields;
}


bool CMySQL0::IsPresent(CString csHost,CString UserName,CString Password)
{

	bError=false;
	csError="";

	mysql_init(&mysql);
	myData=mysql_real_connect( &mysql, csHost.GetBuffer(0), UserName.GetBuffer(0), Password.GetBuffer(0), NULL, 3306,
			NULL, 0 );
	if(myData!=NULL)
	{
		Reset();
		return true;
	}
	csError=mysql_error(&mysql);
	return false;

}




