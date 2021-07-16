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


//Массив
struct SCallFunction aSysFunctions[]=
{
	//{"имя_функции_в_2С",Число_параметров,"Подсказка (необязательно)"}
	{"синус",	1,Sin,"Синус(УголВРадианах)"},
	{"косинус",	1,Cos,"Косинус(УголВРадианах)"},



	{"END",0,NULL},
};



BOOL _cdecl PluginInfo(PLUGIN_INFO* pinfo)
{
	pinfo->VerNeed=PLUG_VER;
	pinfo->csTitle="Тестовый плагин";
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
