// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueMeta.cpp: implementation of the CValueMeta class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueMeta.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "../ModuleManager.h"


CMethods CValueMeta::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueMeta,CValue);
CValueMeta::CValueMeta()
{
	nType=100;
	pObj=0;
}

CValueMeta::~CValueMeta()
{
}

//******************************
//Работа как агрегатного объекта
//******************************
void CValueMeta::PrepareNames(void)
{
	SEngRus aMethods[]={
/*		{"Name","Наименование"},
		{"Identifier","Идентификатор"},
		{"Alias","Синоним"},
		{"Comment","Комментарий"},
		{"Description","Описание",} 
		{"CommonForms","ОбщиеФормы"},
		{"Objects","Объекты"},
*/
		{"Array","Массив"},

	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM,aMethods,nCountM);
}

CValue CValueMeta::Method(int iName,CValue **p)
{
	if(!AfxGetModuleManager())
		Error("Не задан обработчик модулей");
	CValue Ret;
/*	switch(iName)
	{
		case enCommonForms:
			{ 
				int nSize=AfxGetModuleManager()->pMetadata->ListFormName.GetSize();
				if(p[0]->nType==0)
					return nSize;
				int n=p[0]->GetNumber()-1;
				if(n<0||n>=nSize)
					Error("Номер не входит в границы допустимых значений");
				CString csFormName=AfxGetModuleManager()->pMetadata->ListFormName[n];
				Ret.CreateObject("Метаданные");
				CValueMeta  *p=(CValueMeta *)Ret.pRef;
				p->pObj=AfxGetModuleManager()->pMetadata->GetMetaObject(FORMSNAME+"\\"+csFormName,csFormName,1);
				break;
			}
	}
*/	return Ret;
}

CValue CValueMeta::GetAttribute(int iName)//значение атрибута
{
	if(iName==enArray)
	{
		if(!AfxGetModuleManager())
			Error("Не задан обработчик модулей");
		return AfxGetModuleManager()->pMetadata->vMeta;
	}

	if(!pObj)
		Error("Объект не выбран");
	CValue Ret;
/*	switch(iName)
	{
		case enName:
		case enName2:
			{ 
				Ret=String(pObj->csName);
				break;
			}
		case enAlias:
			{ 
				Ret=String(pObj->csAlias);
				break;
			}
		case enComment:
			{ 
				Ret=String(pObj->csComment);
				break;
			}
	}
*/
	return Ret;
}
