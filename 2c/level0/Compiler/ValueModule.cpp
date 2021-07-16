// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueObject.cpp: implementation of the CValueModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueModule.h"
#include "ValueContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CValueModule::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueModule,CValue);

CValueModule::CValueModule()
{
	nType=100;
	pRun=0;
}

CValueModule::~CValueModule()
{
	Clear();
}
void CValueModule::Clear()
{
	vContext.Detach();
	if(pRun)
		delete pRun;
	pRun=0;
}

//******************************
//Работа как агрегатного объекта
//******************************
enum
{
	enLoad=0,
	enGetContext,
	enCheckFunction,
	enCheckAttribut,
	enOpen,
	enRun,
	enCheckFunction2,
};
void CValueModule::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"Load","Загрузить"},
		{"GetContext","ПолучитьКонтекст"},
		{"CheckFunction","ПроверитьФункцию"},
		{"CheckAttribut","ПроверитьАтрибут"},
		{"Open","Открыть"},
		{"Run","Запустить"},
		{"FindFunction","НайтиФункцию"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}



CValue CValueModule::Method(int iName,CValue **p)
{
	if(!AfxGetModuleManager())
		Error("Текущая конфигурация еще не загружена!");
	if(!pRun&&iName!=enLoad)
		Error("Модуль еще не загружен");
	CValue Ret;
	switch(iName)
	{
		case enLoad:
			{
				Clear();
				CString csPath=p[0]->GetString();
				int nType=p[1]->GetNumber()+2;

				pRun=AfxGetModuleManager()->GetNewRunModule(csPath,0,0);
				if(!pRun)
					Error("Ошибка загрузки модуля");
				Ret=1;
				/*
				CMetadata *pMetadata=AfxGetModuleManager()->pMetadata;
				CMetaObject *pObject=pMetadata->GetMetaObject(csPath,nType,0,1);
				*/

				break;
			}
		case enGetContext:
			{
				CValueContext *pValContext=new CValueContext();
				pValContext->Attach(pRun);
				vContext=ValueByRef(pValContext);
				Ret=vContext;
				break;
			}
		case enCheckFunction:
		case enCheckFunction2:
			{
				CString csName=p[0]->GetString();
				int n=(int)pRun->FindFunction(csName);
				Ret=(n>-1);
				break;
			}
		case enCheckAttribut:
			{
				CString csName=p[0]->GetString();
				int n=(int)pRun->pByteCode->VarList[mUpper(csName)]-1;
				Ret=(n!=-1);
				break;
			}
	}
	return Ret;
}
	

