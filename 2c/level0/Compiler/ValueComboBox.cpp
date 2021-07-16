//Авторские права - VTOOLS.RU (info@vtools.ru)
// ValueComboBox.cpp: implementation of the CValueComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueComboBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueComboBox,CValueList);
CValueComboBox::CValueComboBox()
{
	nType=100;
}

CValueComboBox::~CValueComboBox()
{

}
