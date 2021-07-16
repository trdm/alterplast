//Авторские права - VTOOLS.RU (info@vtools.ru)
// Methods.cpp: implementation of the CMethods class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Methods.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMethods::CMethods()
{
}

CMethods::~CMethods()
{
}

//Подготавливаем дерево быстрого доступа к названиям методов и свойств
void CMethods::Prepare(SEngRus *M,int nCountM,SEngRus *A,int nCountA)
{
	treeMethods.RemoveAll();
	treeAttributes.RemoveAll();
	CString csName;
	if(M) 
	{
		aMethods.SetSize(nCountM);
		for(int i=0;i<nCountM;i++)
		{
			aMethods[i].Eng=M[i].Eng;
			aMethods[i].Rus=M[i].Rus;
			aMethods[i].sShortDescription=M[i].sShortDescription;

			csName=M[i].Eng;
			csName.MakeUpper();
			treeMethods[csName]=(void*)(UINT)(i+1);

			csName=M[i].Rus;
			csName.MakeUpper();
			treeMethods[csName]=(void*)(UINT)(i+1);
		}
	}
	if(A)
	{
		aAttributes.SetSize(nCountA);
		for(int i=0;i<nCountA;i++)
		{
			aAttributes[i].Eng=A[i].Eng;
			aAttributes[i].Rus=A[i].Rus;

			csName=A[i].Eng;
			csName.MakeUpper();
			treeAttributes[csName]=(void*)(UINT)(i+1);

			csName=A[i].Rus;
			csName.MakeUpper();
			treeAttributes[csName]=(void*)(UINT)(i+1);
		}
	}
}

int CMethods::FindMethod(CString csName)
{
	csName.MakeUpper();
	return (int)treeMethods[csName]-1;
}
int CMethods::FindAttribute(CString csName)
{
	csName.MakeUpper();
	return (int)treeAttributes[csName]-1;
}


CString CMethods::GetMethodName(int nName,int nAlias)
{
	if(nName<GetNMethods())
	{
		if(nAlias==0)
			return aMethods[nName].Eng;
		else
			return aMethods[nName].Rus;
	}
	return CString("");
}
char *CMethods::GetMethodDescription(int nName,int nAlias)
{
	if(nName<GetNMethods())
	{
			return aMethods[nName].sShortDescription;
	}
	return 0;
}
CString CMethods::GetAttributeName(int nName,int nAlias)
{
	if(nName<GetNAttributes())
	{
		if(nAlias==0)
			return aAttributes[nName].Eng;
		else
			return aAttributes[nName].Rus;
	}
	return CString("");
}

int CMethods::GetNMethods(void)
{
	return aMethods.GetSize();
}
int CMethods::GetNAttributes(void)
{
	return aAttributes.GetSize();
}
