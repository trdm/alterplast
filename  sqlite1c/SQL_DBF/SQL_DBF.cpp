// SQL_DBF.cpp : Defines the initialization routines for the DLL.
#include "stdafx.h"
#include <afxdllx.h>
#include "utex.h"
#include "vtab_info.h"

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
	 	DisableThreadLibraryCalls(hInstance);
		Init1CGlobal(hInstance);
		
		u8text::init();
		
		if(pDataBase7->IsKindOf(RUNTIME_CLASS(CDBEngDB7)))
		{
			pDataDict = *(CDataDictionary**)((long)pDataBase7 + 0x20);
			dbMode = pDataBase7->IsOpenExclusive() ? dbDbfMono : dbDbfShare;
		}
		
		context_obj::CContextBase::InitAllContextClasses();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		context_obj::CContextBase::DoneAllContextClasses();
		CVtabInfo::doneWork();
	}
	return 1;   // ok
}
