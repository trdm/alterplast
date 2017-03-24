#ifndef __CLIPBOARD_H__
	#define __CLIPBOARD_H__

#pragma once

#include "../mycontextbase.h"


/////////////////////////////////////////////////////////////////////////////
// CClipboard command target

class CClipboard : public CMyContextBase
{
	DECLARE_DYNCREATE(CClipboard);

public:
	BOOL funcGet(CValue &RetVal, CValue **ppValue);
	BOOL funcSet(CValue **ppValue);

	CString Get();
	void Set(const CString& str);

public:
	
	DECLARE_MY_CONTEXT();
	
private:
};

#endif