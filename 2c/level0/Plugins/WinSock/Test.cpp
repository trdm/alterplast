// Test.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "../../COMPILER/Value.h"

#include "MyClient.h"
#include "MyClient3.h"
#include "MyServer.h"

#include "MyClientSimple.h"
#include "MyServerSimple.h"

//#include <atlbase.h>
//#include "Release/ServerCOM.tlh"

//******************************************************************
#include <math.h>
CValue Sin(CValue **p)
{
	return sin(p[0]->GetNumber());
}
//������
struct SCallFunction aSysFunctions[]=
{
	//{"���_�������_�_2�",�����_����������,"��������� (�������������)"}
	{"�����",	1,Sin},

	{"END",0,NULL},
};
//******************************************************************



BOOL _cdecl PluginInfo(PLUGIN_INFO* pinfo)
{
	if(pinfo->VerNeed<3)//��� ����� ������ ������� � 3
		return FALSE;

	pinfo->VerNeed=PLUG_VER;
	pinfo->csTitle="������ � �������� Windows";

	return TRUE;
}

BOOL _cdecl InitPlugin()
{
	WSADATA wsaData;
	int wsaret=WSAStartup(0x101,&wsaData);
	if(wsaret)	
		return FALSE;
	RegisterObject(RUNTIME_CLASS(CMyClientSimple),"������");
	RegisterObject(RUNTIME_CLASS(CMyClientSimple),"Client");

	RegisterObject(RUNTIME_CLASS(CMyServerSimple),"������");
	RegisterObject(RUNTIME_CLASS(CMyServerSimple),"Server");
	return TRUE;
}

BOOL _cdecl DonePlugin()
{

	UnRegisterObject("������");
	UnRegisterObject("Client");
	UnRegisterObject("������");
	UnRegisterObject("Server");

	WSACleanup();

	return TRUE;
}
