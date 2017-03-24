// ProcInfo.h: interface for the CPrivatedMethodInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCINFO_H__7D860C21_6258_11D7_8D5E_D07126378404__INCLUDED_)
#define AFX_PROCINFO_H__7D860C21_6258_11D7_8D5E_D07126378404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CPrivatedMethodInfo : public CClassMemberInfo //CInfo  
{
public:
// 	CPrivatedMethodInfo(const CString& name) : CInfo(name, TYPE_MODULE_PROC_PRIVATE){}
// 	virtual ~CPrivatedMethodInfo();
// 	
// 	virtual void OnSelect();
	
	CPrivatedMethodInfo(const CString& name) : CClassMemberInfo(name, TYPE_MODULE_PROC_PRIVATE) {}
};

#endif // !defined(AFX_PROCINFO_H__7D860C21_6258_11D7_8D5E_D07126378404__INCLUDED_)
