// SQLLite.cpp: implementation of the CSQLLite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLLite.h"
#include "sqlite_source/sqlite.h"
//#include "sqlite-source-3_0_2/sqlite3.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

sqlite *dbSQL=0;

BOOL _cdecl PluginInfo(PLUGIN_INFO* pinfo)
{
	pinfo->VerNeed=PLUG_VER;
	pinfo->csTitle="���������� ������ SQLLite";
	return TRUE;
}

BOOL _cdecl InitPlugin()
{
	RegisterObject(RUNTIME_CLASS(CSQLLite),"SQLLite");
	return TRUE;
}

BOOL _cdecl DonePlugin()
{
	UnRegisterObject("SQLLite");

	if(dbSQL)
		sqlite_close(dbSQL);
	dbSQL=0;

	return TRUE;
}


CMethods CSQLLite::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSQLLite,CValue);

CSQLLite::CSQLLite()
{
	nType=100;
	pResultSQL=0;
	Clear();
}

CSQLLite::~CSQLLite()
{
	Clear();
}


//******************************
//������ ��� ����������� �������
//******************************

static struct 
{
	CSQLLite::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CSQLLite::Connect,"Connect","����������"},
	{CSQLLite::Disconnect,"Disconnect","�������������"},
	{CSQLLite::Query,"Query","������"},
	{CSQLLite::GetFieldsName,"GetFieldsName","������������������"},
	{CSQLLite::CountFields,"CountFields","���������������"},
	{CSQLLite::CountRows,"CountRows","���������������"},
	{CSQLLite::GetLine,"GetLine","��������������"},
	{CSQLLite::GetField,"GetField","������������"},
	{CSQLLite::GetFieldsValue,"GetFieldsValue","���������������������"},
	{CSQLLite::IsTableExist,"IsTableExist","�����������������"},
	{CSQLLite::GetInsertID,"GetInsertID","���������������������"},
	{CSQLLite::GetAllValues,"GetAllValues","�������������������"},
	{CSQLLite::BeginTrans,"BeginTrans","����������������"},
	{CSQLLite::CommitTrans,"CommitTrans","�����������������������"},
	{CSQLLite::Rollback,"Rollback","������������������"},
};


#define NMETHODS (sizeof(ArrayMethods)/sizeof(*ArrayMethods) )

void CSQLLite::PrepareNames(void)
{

	SEngRus aMethods[NMETHODS];
	for(int i=0;i<NMETHODS;i++){
		aMethods[i] = ArrayMethods[i].meths;
	}
	int nCountM=NMETHODS;
	Methods.Prepare(aMethods,nCountM,0,0);
}


CValue CSQLLite::Method(int iName,CValue **p)
{

	CValue Ret;
	if(iName<NMETHODS)
		(this->* ArrayMethods[iName].caller)(Ret,p);
	return Ret;
}

CString CSQLLite::GetString()
{
	return "SQLLite";
}
CString CSQLLite::GetTypeString(void)const
{
	return "SQLLite";
}

/*
//////////////////////////////////////////////////////////////////////
//callback
//////////////////////////////////////////////////////////////////////
static int callback(void *pParam, int argc, char **argv, char **azColName)
{
	//OutToDebug("pParam=%d\n",pParam);
	int i;
	if(!pParam)
		return 0;
	CValue  *pArray=(CValue  *)pParam;
	CValue Row;
	Row.CreateObject("���������");
	for(i=0; i<argc; i++)
	{
		Row.SetAt(azColName[i],String(argv[i]));
		//OutToDebug("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	pArray->SetAt(pArray->GetSizeArray()+1,Row);
	//OutToDebug("\n");
	return 0;
}
*/
void CSQLLite::Clear()
{
	if(pResultSQL)
	{
		sqlite_free_table(pResultSQL);
	}
	pResultSQL=0;
	nRowCount=0;
	nColCount=0;
	nCurLine=-1;
}
//////////////////////////////////////////////////////////////////////
//���������������� ������
//////////////////////////////////////////////////////////////////////

BOOL CSQLLite::Connect(CValue &rez,CValue**ppV)
{
	Clear();
	if(dbSQL)
		sqlite_close(dbSQL);
	dbSQL=0;

	char *zErrMsg = 0;
	dbSQL = sqlite_open(ppV[0]->GetString(), 0, &zErrMsg);
	if( dbSQL==0 )
	{
		OutToDebug("SQLLite error: %s\n",zErrMsg);
		rez=0;
		Error(zErrMsg);
	}
	rez=1;
	return TRUE;
};
/*
int CSQLLite::Query(CString Str,CValue *pArray=0)
{
	char *zErrMsg = 0;
	int rc = sqlite_exec(dbSQL, Str, callback, (void*)pArray, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		OutToDebug("SQLLite error: %s\n", zErrMsg);
		Error(zErrMsg);
	}
	return rc;
}
*/
int CSQLLite::Query(CString Str)
{
	if(!dbSQL)
		Error("�� ���� ���������� � ����� ������!");

	Clear();
	char *zErrMsg = 0;

	//������ �������� ���� "\'" �� "''"
	Str.Replace("\\'","''");
	int rc = sqlite_get_table(dbSQL, Str, &pResultSQL, &nRowCount, &nColCount,&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		OutToDebug("SQLLite error: %s\n", zErrMsg);
		Error(zErrMsg);
	}
	return 1;
}

BOOL CSQLLite::Query(CValue &rez,CValue**ppV)
{
	rez=Query(ppV[0]->GetString());
	return TRUE;
};

BOOL CSQLLite::Disconnect(CValue &rez,CValue**ppV)
{
	if(dbSQL)
	{
		sqlite_close(dbSQL);
		rez=1;
	}
	else
	{
		rez=0;
	}
	dbSQL=0;
	return TRUE;
};

//������� ���������:

BOOL CSQLLite::GetFieldsName(CValue &rez,CValue**ppV)//������������������ � ������
{
	rez.CreateObject("������");
	if(nColCount>0)
	{
		for(int i=0;i<nColCount;i++)
			rez.SetAt(i+1,String(pResultSQL[i]));
	}
	return TRUE;
};
BOOL CSQLLite::CountFields(CValue &rez,CValue**ppV)
{
	rez=nColCount;
	return TRUE;
};
BOOL CSQLLite::CountRows(CValue &rez,CValue**ppV)
{
	rez=nRowCount;
	return TRUE;
};
BOOL CSQLLite::GetLine(CValue &rez,CValue**ppV)
{
	nCurLine++;
	if(nCurLine>=nRowCount)
	{
		rez=0;
	}
	else
	{
		rez=1;
	}
	return TRUE;
};
CString CSQLLite::GetField(int n)
{
	if(n<0||n>=nColCount)
		Error("������������ ������ ����");
	if(nCurLine<0)
		Error("�� ���� ������� �����");
	if(nCurLine>=nRowCount)
		Error("������� ����� ��� ���������");

	return pResultSQL[(nCurLine+1)*nColCount+n];
};
BOOL CSQLLite::GetField(CValue &rez,CValue**ppV)
{
	int n=int(ppV[0]->GetNumber())-1;
	rez=String(GetField(n));
	return TRUE;
};

BOOL CSQLLite::GetFieldsValue(CValue &rez,CValue**ppV)//��������������������� � ������
{
	rez.CreateObject("������");
	if(nColCount>0)
	{
		rez.SetSizeArray(nColCount);
		for(int i=0;i<nColCount;i++)
		{
			rez.SetAt(i+1,String(GetField(i)));
		}
	}
	return TRUE;
};
BOOL CSQLLite::GetAllValues(CValue &rez,CValue**ppV)//������������������� � ������ ��������
{
	rez.CreateObject("������");
	if(nColCount>0)
	{
		rez.SetSizeArray(nRowCount);
		for(nCurLine=0;nCurLine<nRowCount;nCurLine++)
		{
			CValue Row;
			Row.CreateObject("���������");
			for(int i=0;i<nColCount;i++)
			{
				Row.SetAt(pResultSQL[i],String(GetField(i)));
			}
			rez.SetAt(nCurLine+1,Row);
		}
	}
	
	return TRUE;
};

BOOL CSQLLite::IsTableExist(CValue &rez,CValue**ppV)
{
	if(!dbSQL)
		Error("�� ���� ���������� � ����� ������!");

	CString csQuery;
	csQuery.Format("SELECT * FROM %s LIMIT 1",ppV[0]->GetString());
	char *zErrMsg = 0;
	int rc = sqlite_exec(dbSQL, csQuery, 0, 0, &zErrMsg);
	if( rc==SQLITE_OK )
	{
		rez=1;
	}
	else
	{
		rez=0;
	}
	return TRUE;
};
BOOL CSQLLite::GetInsertID(CValue &rez,CValue**ppV)
{
	//__int64 
	rez=sqlite_last_insert_rowid(dbSQL);
	return TRUE;
};

BOOL CSQLLite::BeginTrans(CValue &rez,CValue**ppV)
{
	Query("BEGIN");
	return TRUE;
};
BOOL CSQLLite::CommitTrans(CValue &rez,CValue**ppV)
{
	Query("COMMIT");
	return TRUE;
};
BOOL CSQLLite::Rollback(CValue &rez,CValue**ppV)
{
	Query("ROLLBACK");
	return TRUE;
};

