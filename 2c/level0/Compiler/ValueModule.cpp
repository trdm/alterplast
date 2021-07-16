// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
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
//������ ��� ����������� �������
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
		{"Load","���������"},
		{"GetContext","����������������"},
		{"CheckFunction","����������������"},
		{"CheckAttribut","����������������"},
		{"Open","�������"},
		{"Run","���������"},
		{"FindFunction","������������"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}



CValue CValueModule::Method(int iName,CValue **p)
{
	if(!AfxGetModuleManager())
		Error("������� ������������ ��� �� ���������!");
	if(!pRun&&iName!=enLoad)
		Error("������ ��� �� ��������");
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
					Error("������ �������� ������");
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
	

