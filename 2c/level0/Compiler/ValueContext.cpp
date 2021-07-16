// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueContext.cpp: implementation of the CValueContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueContext.h"
#include "../MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueContext,CValueObject);

CValueContext::CValueContext()
{
	nType=100;
	pRun=0;
	aContVarList.CreateObject("Массив");
	aContValueList.CreateObject("Массив");

}

CValueContext::~CValueContext()
{
	pRun=0;
	nType=0;
}
void CValueContext::Attach(void *pObj)
{
	pRun=(CProcUnit*)pObj;
}
void CValueContext::Detach()
{
	pRun=0;
	nType=0;
}

#define EXT_ATTRIBUTE_CALL 2000000000
#define EXT_FUNCTION_CALL  3000000000
#define LOCAL_ATTRIBUTE    1000000

CValue CValueContext::Method(int iName,CValue **p)
{
	if(iName==EXT_ATTRIBUTE_CALL)
	{
		int n=FindAttribute(p[0]->GetString());
		//if(n<0)
		//	Error(CString("Атрибут ")+p[0]->GetString()+" не найден");
		if(p[1]->nType==0)
		{
			//Message(CValue(n));
			return GetAttribute(n);
		}
		else
			{
				SetAttribute(n,*p[1]);
			}
		return CValue();
	}
	if(iName>=0&&pRun)
		return pRun->CallFunction(iName,p);
	return CValue();
}
int  CValueContext::FindMethod(CString csName)
{
	if(!pRun)
		return -1;

	CString csMethName=mUpper(csName);
	if(csMethName=="ПЕРЕМ"||csMethName=="VAR")//доступ к массиву переменных
		return EXT_ATTRIBUTE_CALL;
 
	int n=pRun->FindFunction(csMethName);
	return n;
}
int  CValueContext::FindAttribute(CString csName)
{
	if(pRun)
	{
		int iName=(int)pRun->pByteCode->VarList[mUpper(csName)]-1;
		//Message(CValue(csName+" "+CValue(iName)));
		if(iName==-1)
		{
			int iSize=aContVarList.GetSizeArray();
			for (int iIndex=1;iIndex<=iSize;iIndex++)
				if(mUpper(aContVarList.GetAt(iIndex).GetString())==csName)
				{ 
					return (int)LOCAL_ATTRIBUTE+iIndex;
				}
			aContVarList.SetAt(iSize+1,CValue(csName));
			aContValueList.SetAt(iSize+1,CValue());
			return (int)LOCAL_ATTRIBUTE+iSize+1;
		}
		return iName;
	}
	else
		return -1;
}
CValue CValueContext::GetAttribute(int nVar)//получение значения атрибута
{
	if(nVar>=0&&pRun)
	{
		int iContLocalCount=aContVarList.GetSizeArray();
		
		if(nVar<LOCAL_ATTRIBUTE)
			return pRun->GetAttribute(nVar);
		else
			if(nVar>LOCAL_ATTRIBUTE)
				return aContValueList.GetAt(nVar-LOCAL_ATTRIBUTE);
			else
				return CValue();

	}
	else
		return CValue();
}
void CValueContext::SetAttribute(int nVar,CValue &Val)//установка атрибута
{
	if(nVar>=0&&pRun)
	{
		if(nVar<LOCAL_ATTRIBUTE)
			pRun->SetAttribute(nVar,Val);
		else
			if(nVar>LOCAL_ATTRIBUTE)
				aContValueList.SetAt(nVar-LOCAL_ATTRIBUTE,Val);
			//Message(CValue(GetAttribute(nVar)+" "+CValue(aContVarList.GetAt(nVar-LOCAL_ATTRIBUTE))));
	}
}

void CValueContext::SaveToString(CString &Str)
{
	Str.Format("{%s/}",csObjectType);
}
void CValueContext::LoadFromString(CString &Str)
{
}

CString CValueContext::GetTypeString()const
{
	if(pRun)
		return "Групповой контекст";
	else
		return NOT_DEFINED;

}
CString CValueContext::GetString(void)const
{
	return GetTypeString();
}

int CValueContext::GetType(void)const
{
	return nType;
}

NUMBER CValueContext::GetNumber(void)const
{
	return (int)pRun;
}
