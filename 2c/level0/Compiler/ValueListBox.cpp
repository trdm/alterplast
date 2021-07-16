// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListBox.cpp: implementation of the CValueListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Dialog.h"
#include "ValueListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueListBox,CValueList);
CValueListBox::CValueListBox()
{
	nType=100;

}

CValueListBox::~CValueListBox()
{

}
/*
//различие кода1:нет метода GetText
//различие кода2:есть Check
int CValueListBox::
Check(int nIndex,int bValue)
{
	if(nIndex<1||nIndex>GetListSize())
		SetError("Индекс выходит за границы списка значений!");
	int nRes=GetSel(nIndex-1);
	if(bValue!=-1)
		SetSel(nIndex-1,bValue);
	return nRes;
}
*/

