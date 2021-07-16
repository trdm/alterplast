// SQLLite.cpp: implementation of the CSQLLite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyObject.h"
#include "../../OutToDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CSQLLite::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSQLLite,CValue);

CSQLLite::CSQLLite()
{
	pResultSQL=0;
}

CSQLLite::~CSQLLite()
{
}


//******************************
//Работа как агрегатного объекта
//******************************

static struct 
{
	CSQLLite::MethFunc caller;
	SEngRus meths;
}  ArrayMethods[]=
{
	{CSQLLite::Connect,"Connect","Соединение"},
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

//////////////////////////////////////////////////////////////////////
//Пользовательские методы
//////////////////////////////////////////////////////////////////////

BOOL CSQLLite::Connect(CValue &rez,CValue**ppV)
{
	return TRUE;
};
