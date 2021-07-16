// Test.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "../../COMPILER/Value.h"

  
#include <math.h>

CValue Sin(CValue **p)
{
	return sin(p[0]->GetNumber());
}
CValue Cos(CValue **p)
{
	return cos(p[0]->GetNumber());
}


//������
struct SCallFunction aSysFunctions[]=
{
	//{"���_�������_�_2�",�����_����������,"��������� (�������������)"}
	{"�����",	1,Sin,"�����(�������������)"},
	{"�������",	1,Cos,"�������(�������������)"},



	{"END",0,NULL},
};



BOOL _cdecl PluginInfo(PLUGIN_INFO* pinfo)
{
	pinfo->VerNeed=PLUG_VER;
	pinfo->csTitle="�������� ������";
	return TRUE;
}

BOOL _cdecl InitPlugin()
{
	AddSystemFunctions(aSysFunctions);

	return TRUE;
}

BOOL _cdecl DonePlugin()
{



	return TRUE;
}
