// stdafx.cpp : source file that includes just the standard includes

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


#include "registry.h"
BOOL _cdecl PluginInfo(PLUGIN_INFO* pinfo){
	pinfo->VerNeed=PLUG_VER;
	pinfo->csTitle="������ � ��������";
	return TRUE;
}

BOOL _cdecl InitPlugin(){
	RegisterObject(RUNTIME_CLASS(CValueRegistry),"Registry");
	RegisterObject(RUNTIME_CLASS(CValueRegistry),"������");
	//AfxMessageBox("LOAD!");
	return TRUE;
}

BOOL _cdecl DonePlugin(){
	//AfxMessageBox("DONE!");
	UnRegisterObject("������");
	UnRegisterObject("Registry");
	return TRUE;
}

