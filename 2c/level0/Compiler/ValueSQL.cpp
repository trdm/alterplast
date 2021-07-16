// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueSQL.cpp: implementation of the CValueSQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueSQL.h"

#include "OutToDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CValueSQL::Methods;
int nCurrentConnectionType=MYSQL_CONNECTION;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueSQL,CValue);
BOOL bWasConnect=0;

CValueSQL::CValueSQL()
{
	nType=100;

}

CValueSQL::~CValueSQL()
{

}


//******************************
//Работа как агрегатного объекта
//******************************
void CValueSQL::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"Connect","Соединение"},
		{"ConnectODBC","СоединениеODBC"},
		{"ConnectODBC","СоединениеОДБС"},
		{"Disconnect","Отсоединиться"},
		{"Query","Запрос"},
		{"GetFieldsName","ПолучитьИменаПолей"},
		{"CountFields","КоличествоПолей"},
		{"GetLine","ПолучитьСтроку"},
		{"GetField","ПолучитьПоле"},
		{"GetFieldsValue","ПолучитьЗначенияПолей"},
		{"IsTableExist","СуществуетТаблица"},
		{"GetInsertID","ПолучитьИдентификатор"},
		{"GetAllValues","ПолучитьВсеЗначения"},
		{"BeginTrans","НачатьТранзакцию"},
		{"CommitTrans","ЗафиксироватьТранзакцию"},
		{"Rollback","ОтменитьТранзакцию"},
		{"GetConnect","СтрокаСоединения"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

CValue CValueSQL::Method(int iName,CValue **p)
{
	CValue Ret;
	switch(iName)
	{
		case enConnect:
			{
				nCurrentConnectionType=MYSQL_CONNECTION;
                CString HostName=p[0]->GetString();
                CString BaseName=p[1]->GetString();
                CString UserName=p[2]->GetString();
                CString Password=p[3]->GetString();
                Ret=DB.Connect(HostName,BaseName,UserName,Password,nCurrentConnectionType);
				bWasConnect=Ret;
				break;
			}
		case enConnectODBC:
		case enConnectODBC2:
			{
				nCurrentConnectionType=ODBCSQL_CONNECTION;
                CString HostName=p[0]->GetString();
                CString BaseName=p[1]->GetString();
                CString UserName=p[2]->GetString();
                CString Password=p[3]->GetString();
                int n=DB.Connect(HostName,BaseName,UserName,Password,nCurrentConnectionType);
				if(n==0)
				{
					Ret=String(DB.csError);
					bWasConnect=0;
				}
				else
				{
					Ret=1;
					bWasConnect=1;
				}
				break;
			}
		case enDisconnect:
			{
				if(bWasConnect)
					DB.Reset();
				bWasConnect=0;
				break;
			}
		case enQuery:
			{
				if(!bWasConnect)
					Error("Не было соединения с базой данных!");
				Ret=DB.Query(p[0]->GetString());
				if(DB.bError)
				{
					CString Str;
					Str.Format("%s\nОшибка в запросе:\n%s\n",DB.csError,p[0]->GetString());
					OutToDebug("%s",Str);
					Error(Str.GetBuffer(0));
				}
				break;
			}
		case enGetInsertID:
			{
				if(!bWasConnect)
					Error("Не было соединения с базой данных!");
				__int64 nRes=DB.GetInsertID();
				Ret=nRes;
				break;
			}

		//__________________________________________________________________
		//Выборка структуры:
		case enGetFieldsName://ПолучитьИменаПолей в массив
			{
				int nCount=DB.CountFields();
				if(nCount>0)
				{
					Ret.CreateObject("Массив");
					for(int i=0;i<nCount;i++)
						Ret.SetAt(i+1,String(DB.GetFieldName(i)));
				}
				break;
			}
			
		case enCountFields://КоличествоПолей
			{
				Ret=DB.CountFields();
				break;
			}
			
		//__________________________________________________________________
		//Выборка данных:
		case enGetLine:
			{
				Ret=DB.GetLine();
				break;
			}
		case enGetField:
			{
				int n=p[0]->GetNumber();
				if(n<1)
					Error("Недопустимый индекс поля");
				Ret=String(DB.GetString(n-1));
				break;
			}

		case enGetFieldsValue://ПолучитьЗначенияПолей в массив
			{
				int nCount=DB.CountFields();
				Ret.CreateObject("Массив");
				if(nCount>0)
				{
					Ret.SetSizeArray(nCount);
					for(int i=0;i<nCount;i++)
					{
						Ret.SetAt(i+1,String(DB.GetString(i)));
					}
				}
				break;
			}
		case enIsTableExist://СущестуетТаблица 
			{
				if(!bWasConnect)
					Error("Не было соединения с базой данных!");
				Ret=DB.IsTableExist(p[0]->GetString());
				break;
			}
		case enGetAllValues://ПолучитьВсеЗначения в массив структур
			{
				int nCount=DB.CountFields();
				Ret.CreateObject("Массив");
				if(nCount>0)
				{
					int n=0;
					while(DB.GetLine())
					{
						n++;
						CValue Row;
						Row.CreateObject("Структура");
						for(int i=0;i<nCount;i++)
						{
							Row.SetAt(DB.GetFieldName(i),String(DB.GetString(i)));
						}
						Ret.SetAt(n,Row);
					}
				}
				break;
			}
		case enBeginTrans:
			Ret=DB.BeginTrans();
			break;
		case enCommitTrans:
			Ret=DB.CommitTrans();
			break;
		case enRollback:
			Ret=DB.Rollback();
			break;
		case enGetConnect:
			Ret=String(DB.GetConnect());
			break;
	}
	return Ret;
}
