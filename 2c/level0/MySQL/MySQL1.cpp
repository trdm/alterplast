// MySQL.cpp: implementation of the MySQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef __1C__
#include "..\MyDB.h" 
#endif
#include "MySQL1.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int bDebug=0;

#include <odbcinst.h>
#include "..\OutToDebug.h"
#ifdef __1C__
	#include "..\RainBow\\_CBLModule7.h"
#endif

UINT GetProcessorTimeMySQL(UINT nDiv);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Новый класс!!!
//Новый класс!!!
//Новый класс!!!
//Новый класс!!!
CDatabase CMySQL1::cODBC;
bool	CMySQL1::bWasODBCConnected=false;
CMySQL1::CMySQL1()
{
	pODBCQuery=NULL;
	cRecord.m_pDatabase=&cODBC;
}
CMySQL1::~CMySQL1()
{
	//delete pODBCRecordArray;
}
int CMySQL1::Connect(CString csHost,CString csBase,CString UserName,CString Password,int nType)
{
	nConnectionType=nType;
	if(nConnectionType==MSSQL_ODBC)
	{
		CString tmpStr; 
		RETCODE nRetCode; 
		
		//create DSN on this computer 
		char *szDriver = "SQL Server"; 
		tmpStr="DSN=";
		tmpStr+=csBase;
		tmpStr+="\\DESCRIPTION=Подключение к базе ";
		tmpStr+=csBase;
		tmpStr+=" (MS SQL)\\SERVER=";
		tmpStr+=csHost; 
		tmpStr+="\\ADDRESS=1433\\NETWORK=DBMSSOCN,"; 
		tmpStr+=csHost; 
		tmpStr+="\\DATABASE="; 
		tmpStr+=csBase; 
		tmpStr+="\\"; 
		//change double slashes to terminate symbol 
		char *szAttributes=tmpStr.GetBuffer(tmpStr.GetLength()); 
		for(int i=0;i<tmpStr.GetLength();i++) 
		{ 
			if(*(szAttributes+i)==(int) '\\') 
				*(szAttributes+i)=NULL; 
		} 
		nRetCode = SQLConfigDataSource(NULL, ODBC_ADD_DSN, szDriver, szAttributes); 
		if(!nRetCode) 
		{ 
			csError.Format("Error create DSN in registry! Строка инициализации:%s;%s",csHost,csBase);
			bError=true;
			OutToDebug("%s\n",csError);
			return 0;
		} 
		
		csHost=csBase;
		nConnectionType=ODBCSQL_CONNECTION;
	}
	else
		if(nConnectionType==ORACLE_ODBC)
		{
			if(0)
			{
				RETCODE nRetCode; 
				nRetCode = SQLConfigDataSource( NULL,
					ODBC_ADD_SYS_DSN, /* OR  ODBC_ADD_DSN*/ 
					"Microsoft ODBC for Oracle",
					"BufferSize=40000\0"
					"CatalogCap=1\0"
					"ConnectString="+csBase+"\0"
					"Description=Подключение к базе "+csBase+" (ORACLE)\0"
					"DSN=MSORCL32_DSN\0"
					"NumberFloat=0\0"
					"PWD="+Password+"\0"
					"RDOSupport=0\0"
					"Remarks=0\0"
					"RowLimit=0\0"
					"Server="+csHost+"\0"
					"SynonymColumns=0\0"
					"SystemTable=0\0"
					"TranslationDLL=\0"
					"TranslationName=\0"
					"TranslationOption=\0"
					"TxnCap=0\0"
					"UID="+UserName+"\0"
					);
				
				if(!nRetCode) 
				{ 
					csError.Format("Error create DSN in registry! Строка инициализации:%s;%s",csHost,csBase);
					bError=true;
					OutToDebug("%s\n",csError);
					return 0;
				} 
			}
			CString strConnect = _T("Driver={Microsoft ODBC for Oracle};Server=OracleServer.world;"
				"Uid="+UserName+";Pwd="+Password+";");
			
			
			
			

		csHost=csBase=strConnect;
		nConnectionType=ODBCSQL_CONNECTION;
	}


	//OutToDebug("nConnectionType=%d\n",nConnectionType);
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::Connect(csHost,csBase,UserName,Password);
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			if (!pODBCQuery) 
				pODBCQuery	= new _CODBCQuery();
			return 1;
		}


	CString csODBC2;//="ODBC;";
	if(!UserName.IsEmpty()||!Password.IsEmpty())
		csODBC2.Format("ODBC;UID=%s;PWD=%s;",UserName,Password);

	try
	{
		if(cODBC.IsOpen())
			cODBC.Close();
		cODBC.SetQueryTimeout(0);
		if(csODBC2.IsEmpty())
			cODBC.OpenEx(csHost);
		else
			cODBC.Open(csHost,FALSE,TRUE,csODBC2);
	}
	catch(CDBException *e)
	{
		csError.Format("%s Строка инициализации:%s;%s",e->m_strError,csHost,csODBC2);
		bError=true;
		OutToDebug("%s\n",csError);
		e->Delete();
		return 0;
	};


	if(cODBC.IsOpen())
	{
		cRecord.m_pDatabase=&cODBC;
		bWasODBCConnected=true;
		cODBC.SetQueryTimeout(0);
		return 1;
	}
	else
	{
		csError.Format("База данных не открывается! Ошибка при подключении по строке:%s;%s",csHost,csODBC2);
		bError=true;
		return 0;
	}

	return 0;

}
int CMySQL1::Query(CString SQLQuery,CString csFNames)
{
	if(bDebug==0)
		return _Query(SQLQuery,csFNames);

	UINT nStart=GetProcessorTimeMySQL(10);
 
    COleDateTime timeNow=COleDateTime::GetCurrentTime();
	OutToDebug("SQLQuery:\n%s\n",SQLQuery);
	OutToDebug("Begin:%s...",timeNow.Format("%H:%M:%S"));
	int nRes=_Query(SQLQuery,csFNames);
	timeNow=COleDateTime::GetCurrentTime();
	OutToDebug("End:%s",timeNow.Format("%H:%M:%S"));

	UINT nEnd=GetProcessorTimeMySQL(10);
	OutToDebug(" (CPU: %d)\n",(int)(nEnd-nStart));

	return nRes;
}
int CMySQL1::_Query(CString SQLQuery,CString csFNames)
{
	nCurRow=-1;
	nCurNField=-1;
	nCurLine=-1;
	nCountFields=0;
	nCountRows=0;

/*
	CDataBase7 * pDataBase7;
	pDataBase7	= CDataBase7::GetDefault();
	long l=long(pDataBase7->GetRealDB());
	OutToDebug("1. GetRealDB=%d\n",l);

	if(pODBCQuery)
	{
		l=long(((CDataBase7*)pODBCQuery->GetDB7())->GetRealDB());
		OutToDebug("2. GetRealDB=%d\n",l);
	}
*/
	//OutToDebug("SQLQuery:\n%s\n",SQLQuery);

	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::Query(SQLQuery);
	else
	if(nConnectionType==RAINBOW_CONNECTION)
	{
		//для соединения 1С мы заранее должны передать используемые имена полей в запросе
		
		if(csFNames!="")
		{
			nCountFields=csFNames.Replace(",",",")+1;
			if(pFieldName)
				delete []pFieldName;
			pFieldName=new CString[nCountFields];
			int n1=0;
			for(int i=0;i<nCountFields;i++)
			{
				int n2=csFNames.Find(",",n1);
				if(n2>=0)
					pFieldName[i]=csFNames.Mid(n1,n2-n1);
				else
					pFieldName[i]=csFNames.Mid(n1);
				pFieldName[i].TrimLeft();
				pFieldName[i].TrimRight();
				n1=n2+1;
			}
		}

		if (!pODBCQuery) 
			pODBCQuery	= new _CODBCQuery();

		int nRes;
		bError=false;
		try
		{
			//nRes =  pODBCQuery->Prepare(SQLQuery,0,1);
			nRes =  pODBCQuery->Prepare(SQLQuery,1,1);
			nRes =  pODBCQuery->Open();
		}
		catch(...)
		{
			bError=true;
			csError=CString("Ошибка при выполнении запроса:")+SQLQuery;
/*
			CDataBase7 * pDataBase7;
			pDataBase7			= CDataBase7::GetDefault();

			long l=long(pDataBase7->GetRealDB());
			OutToDebug("GetRealDB=%d\n",l);
/*			cODBC.m_hdbc=(void*)(long(pDataBase7->GetRealDB())+0x4c);
			try
			{
				cODBC.ExecuteSQL(SQLQuery);
			}
			catch(CDBException *e)
			{
				csError=e->m_strError;
				OutToDebug("%s\n",csError);
				bError=true;
				e->Delete();
				return 0;
			}
*/
			nRes=0;
		}
		return nRes;
	}

/*
//ODBC
	odbcDynamic.ExecuteSQL(SQLQuery,pODBCRecordArray);
	if(odbcDynamic.bSuccess)
		return 1;

	csError=CString("Ошибка при обработке запроса ")+SQLQuery;
	OutToDebug("%s\n",csError);
	bError=true;
	return 0;

*/


	if(!bWasODBCConnected)
	{
		bError=true;
		csError="Не было подключения к источнику данных ODBC!";
		return 0;
	}

	cRecord.m_pDatabase=&cODBC;
	SQLQuery.TrimLeft();
	if(SQLQuery.IsEmpty())
		return 0;
	CString str=SQLQuery.Left(6);
	str.MakeUpper();
	bError=false;
	nCountFields=0;
	if(str=="SELECT")//||str.Left(4)=="SHOW")
	{
		int nRet=0;
		try
		{
			if(cRecord.IsOpen())
				cRecord.Close();
			nRet=cRecord.Open(SQLQuery,CRecordset::snapshot,CRecordset::readOnly);

			nCountFields=cRecord.GetODBCFieldCount();
		}
		catch(CDBException *e)
		{
			csError=e->m_strError;
			OutToDebug("%s\n",csError);
			bError=true;
			e->Delete();
			return 0;
		};
		if(nRet==0)
			return 0;

		return 1;
	}
	else
	{
		try
		{
			cODBC.ExecuteSQL(SQLQuery);
		}
		catch(CDBException *e)
		{
			csError=e->m_strError;
			OutToDebug("%s\n",csError);
			bError=true;
			e->Delete();
			return 0;
		}
	}
	return 1; 

}

int CMySQL1::CountFields()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::CountFields();
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			//OutToDebug("CountFields=%d\n",nCountFields);
			return nCountFields;
			//OutToDebug("Вызов метода CountFields запрещен!");
		}

	if(!bWasODBCConnected)
		return 0;

	return nCountFields;
}
CString CMySQL1::FieldName()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::FieldName();
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			if(pFieldName&&nCurNField<nCountFields)
			{
				//OutToDebug("FieldName=%s\n",pFieldName[nCurNField]);
				return pFieldName[nCurNField];
			}
			else
				OutToDebug("Error FieldName !! nCurNField=%d\n",nCurNField);
			return _T("");
		}



	if(!bWasODBCConnected)
		return "";

	if(nCurNField>=nCountFields)
		return "";
	CODBCFieldInfo info;
	cRecord.GetODBCFieldInfo(nCurNField,info);
	return info.m_strName;

}
int CMySQL1::GetFieldName()
{
	//OutToDebug("GetFieldName 1\n");
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::GetFieldName();
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			nCurNField++;
			//OutToDebug("GetFieldName 2\n");
			if(pFieldName&&nCurNField<nCountFields)
				return 1;
			else
			{
				nCurNField=-1;
				return 0;
			}
//			OutToDebug("Вызов метода GetFieldName запрещен!");
		}


	if(!bWasODBCConnected)
		return 0;

	 nCurNField++;
	 if(nCurNField<nCountFields)
		return 1;
	 else
		return 0;
}
CString CMySQL1::GetFieldName(int n)
{
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::GetFieldName(n);
	if(n>=nCountFields||nConnectionType!=ODBCSQL_CONNECTION)
		return "";
	CODBCFieldInfo info;
	cRecord.GetODBCFieldInfo(n,info);
	return info.m_strName;
}
int CMySQL1::Move(int nRow)
{
	int n=CountRows();
	if(nConnectionType==MYSQL_CONNECTION)
	{
		CMySQL0::Seek(nRow);
		return 1;
	}
	try
	{
		cRecord.Move(nRow,SQL_FETCH_ABSOLUTE);
		return 1;
	}
	catch(...)
	{
		return 0;
	};
}
int CMySQL1::CountRows()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return nCountRows;
	if(!bWasODBCConnected)
		return 0;
	nCountRows=cRecord.GetRecordCount();
	return nCountRows;
}

//________________________________________________________________________________
//________________________________________________________________________________
CString CMySQL1::_GetValue(int Number)
{
	//OutToDebug("_GetValue 1\n");
	CString str;
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::_GetValue(Number);
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			//OutToDebug("_GetValue 2\n");
			pODBCQuery->GetString(Number,str);
			return str;
		}

	if(Number>=nCountFields)
		return "";
	CString Str=cRecord.GetString(Number);
	Str.TrimRight();
	return Str;
}

//________________________________________________________________________________
CString CMySQL1::GetString(int Number)
{
	CString str;
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::_GetValue(Number);
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			try
			{
				pODBCQuery->GetString(Number,str);
				str.TrimRight();
			}
			catch(...)
			{
				str="";
			}
			return str;
		}

	if(!bWasODBCConnected)
		return "";

	CString Str=cRecord.GetString(Number);
	Str.TrimRight();
	if(Number>=nCountFields)
		return "";
	return Str;
}

//________________________________________________________________________________
long CMySQL1::GetLong(int Number)
{
	CString str;
	if(nConnectionType==MYSQL_CONNECTION)
		return atoi(CMySQL0::_GetValue(Number));
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			long lParam=0;
			pODBCQuery->GetLong(Number,lParam);
			//OutToDebug("GetLong=%d\n",lParam);
			return lParam;
		}

	if(!bWasODBCConnected)
		return 0;


	if(Number>=nCountFields)
		return 0;
	return cRecord.GetLong(Number);
}

//________________________________________________________________________________
double CMySQL1::GetDouble(int Number)
{
	if(nConnectionType==MYSQL_CONNECTION)
		return atof(CMySQL0::_GetValue(Number));
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			CString str;
			pODBCQuery->GetString(Number,str);
			return atof(str);
		}

	if(!bWasODBCConnected)
		return 0;


	if(Number>=nCountFields)
		return 0;
	return cRecord.GetDouble(Number);
}

//________________________________________________________________________________
//________________________________________________________________________________
int CMySQL1::GetLine()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::GetLine();
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			if(!pODBCQuery)
			{
				OutToDebug("Ошибка вызова GetLine, объект pODBCQuery не существует!");
				return 0;
			}
			int nRes = pODBCQuery->GotoNext();
			return nRes;
		}

	if(!bWasODBCConnected)
		return 0;


	if( cRecord.IsBOF( ) )
		return 0;// The recordset is empty


	nCurLine++;
	if(nCurLine==0)
	{
		if(!cRecord.IsEOF())
			return 1;
		else
			return 0;
	}


	cRecord.MoveNext();
	if(!cRecord.IsEOF())
	{
		return 1;
	}
	else
	{
		cRecord.MoveFirst();//новый цикл выборки
		return 0;
	}
	
}
int CMySQL1::MoveFirst()
{
	nCurLine=-1;
	if(nConnectionType==MYSQL_CONNECTION)
	{

		mysql_row_seek(MySQLres,0);
		return 1;
	}
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			if(!pODBCQuery)
			{
				OutToDebug("Ошибка вызова MoveFirst, объект pODBCQuery не существует!");
				return 0;
			}
			return 1;
		}

	if(!bWasODBCConnected)
		return 0;

	cRecord.MoveFirst();
	return 1;
}
void CMySQL1::Reset()
{
	nCurNField=-1;
	nCurLine=-1;
	nCountFields=0;

	if(nConnectionType==MYSQL_CONNECTION)
	{
		CMySQL0::Reset();
		return;
	}
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			if (pODBCQuery) delete pODBCQuery;
			pODBCQuery = NULL;
			return;
		}



	if(cRecord.IsOpen())
		cRecord.Close();
	if(cODBC.IsOpen())
		cODBC.Close();
	return;
}

my_ulonglong CMySQL1::GetInsertID()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return CMySQL0::GetInsertID();

	bError=1;
	csError="Данное соединение не поддерживает матод GetInsertID";

	return 0;
}

int CMySQL1::IsTableExist(CString csTableName)
{
/*
	CString csQuery;
	csQuery.Format("SELECT COUNT(*) FROM %s",csTableName);
	bool _bCheckError=bCheckError;
	bCheckError=false;
	Query(csQuery);
	bCheckError=_bCheckError;
	int nRes=!bError;
	return nRes;
*/
///*

	if(nConnectionType==MYSQL_CONNECTION)
	{
		CString csQuery;
		csQuery.Format("SELECT * FROM %s LIMIT 1",csTableName);
		bool _bCheckError=bCheckError;
		bCheckError=false;
		Query(csQuery);
		bCheckError=_bCheckError;
		int nRes=!bError;
		return nRes;
	}
	else
		if(nConnectionType==RAINBOW_CONNECTION)
		{
			CString csQuery;
			csQuery.Format("SELECT TOP 1 * FROM %s",csTableName);
			bool _bCheckError=bCheckError;
			bCheckError=false;
			Query(csQuery);
			bCheckError=_bCheckError;
			int nRes=!bError;
			return nRes;
		}


		if(cRecord.IsOpen())
			cRecord.Close();

		//CRecordset	cRecord2(&cODBC);
		//cRecord2.m_pDatabase=&cODBC;

		if(!bWasODBCConnected)
		{
			OutToDebug("Не было соединения через ODBC (2)!\n");
			return 0;
		}

		try
		{
			cRecord.Open(CString("SELECT * FROM ")+csTableName,CRecordset::forwardOnly,CRecordset::readOnly);
		}
		catch(CDBException *e)
		{
			bError=false;
			e->Delete();
			return 0;
		}
		if(cRecord.IsOpen())
			cRecord.Close();
		return 1;
//*/
}



int CMySQL1::FetchData()
{

	return 1;
}


CString CMySQL1::GetConnect()
{
	if(nConnectionType==MYSQL_CONNECTION)
		return "MySQL";
	else
		return cODBC.GetConnect();
}

//Поддержка транзакций
BOOL CMySQL1::BeginTrans()
{
	if(nConnectionType==MYSQL_CONNECTION)
	{
		return Query("BEGIN");
	}
	else
	{
		if(cODBC.CanTransact())
			return cODBC.BeginTrans();
		else
			Error(CString("Источник данных \"")+cODBC.GetConnect()+"\" транзакции не поддерживает");
	}
	return 0;
}
BOOL CMySQL1::CommitTrans()
{
	if(nConnectionType==MYSQL_CONNECTION)
	{
		return Query("COMMIT");
	}
	else
	{
#ifdef _DEBUG
		if(!cODBC.m_bTransactionPending)
			Error("Транзакция не активна");
#endif

		if(cODBC.CanTransact())
			return cODBC.CommitTrans();
		else
			Error(CString("Источник данных \"")+cODBC.GetConnect()+"\" транзакции не поддерживает");
	}
	return 0;
}
BOOL CMySQL1::Rollback()
{
	if(nConnectionType==MYSQL_CONNECTION)
	{
		return Query("ROLLBACK");
	}
	else
	{
#ifdef _DEBUG
		if(!cODBC.m_bTransactionPending)
			Error("Транзакция не активна");
#endif

		if(cODBC.CanTransact())
			return cODBC.Rollback();
		else
			Error(CString("Источник данных \"")+cODBC.GetConnect()+"\" транзакции не поддерживает");
	}
	return 0;
}

