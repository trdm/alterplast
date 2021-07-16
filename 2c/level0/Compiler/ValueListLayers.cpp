// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListLayers.cpp: implementation of the CValueListLayers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Dialog.h"
#include "ValueListLayers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueListLayers,CValueList);
CValueListLayers::CValueListLayers()
{
	nType=100;

}

CValueListLayers::~CValueListLayers()
{

}

void CValueListLayers::AddValue(CValue Val,CString Str)
{
	CValueList::AddValue(Val,Str);
	CopyValues();
}

void CValueListLayers::SetSize(int nSize)
{
	CValueList::SetSize(nSize);
	CopyValues();
}

void CValueListLayers::MoveValue(int nCount,int nIndex)
{
	CValueList::MoveValue(nCount,nIndex);
	CopyValues();
}

void CValueListLayers::RemoveValue(int nIndex, int nCount)
{
	CValueList::RemoveValue(nIndex,nCount);
	CopyValues();
}

void CValueListLayers::FromSeparatedString(CString Str)
{
	CValueList::FromSeparatedString(Str);
	CopyValues();
}
void CValueListLayers::SortByPresent(int nDirect)
{
	CValueList::SortByPresent(nDirect);
	CopyValues();
}
void CValueListLayers::Sort(int nDirect)
{
	CValueList::Sort(nDirect);
	CopyValues();
}
void CValueListLayers::RemoveAll(void)
{
	CValueList::RemoveAll();
	CopyValues();
}
int CValueListLayers::Set(CString Str,CValue Val)
{
	int nRes=CValueList::Set(Str,Val);
	CopyValues();
	return nRes;
}
void CValueListLayers::SetValue(int nIndex, CValue Val, CString Str, int nCount)
{
	CValueList::SetValue(nIndex,Val,Str,nCount);
	CopyValues();
}
void CValueListLayers::InsertValue(int nIndex,CValue Val,CString Str,int nCount)
{
	CValueList::InsertValue(nIndex,Val,Str,nCount);
	CopyValues();
}



CString CValueListLayers::GetTypeString(void)const
{
	return "Закладки";
}


void CValueListLayers::CopyValues()
{
	if(!pWnd)
		Error("Не было привязки окна CWnd!");

	CMyTabCtrl *pTab=(CMyTabCtrl*)pWnd;
	pTab->RemoveAll();
	for(int i=0;i<GetListSize();i++)
	{
		CElementList data=aValue.GetAt(i);
		pTab->AddValue(data.Value.GetString(),data.Present);
	}
}
int CValueListLayers::UseLayer(CString csName,int nMode)
{
	if(!pWnd)
		Error("Не было привязки окна CWnd!");

	CMyTabCtrl *pTab=(CMyTabCtrl*)pWnd;
	int nIndex=pTab->GetCurSel();//выбранная закладка
	pTab->UseLayer(csName,nMode);

	return nIndex+1;
}
