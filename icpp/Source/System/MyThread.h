//////////////////////////////////////////////////////////////////////
//	MyThread.h
//	Interface for CMyThread class
//	Refactoring of vtools project class
//	Initial code written by artbear (јртур јюханов)
//	Contacts: EMAIL artbear@inbox.ru
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../mycontextbase.h"

class CMyThread : public CMyContextBase
{
//	DECLARE_DYNCREATE(CMyThread);

public:

	//атрибуты
	HANDLE hThread;

	BOOL procCreateThread(CValue **params);
    BOOL procDeleteThread(CValue **params);
    BOOL procSleep(CValue **params);

	CMyThread();
	CMyThread(int);
	virtual ~CMyThread();

//	DECLARE_MY_CONTEXT();
};
