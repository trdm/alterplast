// MyModule.cpp: implementation of the CMyModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "MyModule.h"
#include "classinfo.h"
#include "syntax.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMyModule::~CMyModule()
{

}

int CMyModule::OnSyntaxError(void)
{
	new CSyntax(this);
	return 1;
}