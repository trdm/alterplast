// ProcExInfo.h: interface for the CExportedMethodInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCEXINFO_H__7D860C22_6258_11D7_8D5E_D07126378404__INCLUDED_)
#define AFX_PROCEXINFO_H__7D860C22_6258_11D7_8D5E_D07126378404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CExportedMethodInfo : public CClassMemberInfo //CInfo  
{
public:
// 	CExportedMethodInfo(const CString& name) : CInfo(name,TYPE_MODULE_PROC_EXPORT) {}
// 	virtual ~CExportedMethodInfo();
// 
// 	virtual void OnSelect();
	CExportedMethodInfo(const CString& name) : CClassMemberInfo(name, TYPE_MODULE_PROC_EXPORT) {}
};

#endif // !defined(AFX_PROCEXINFO_H__7D860C22_6258_11D7_8D5E_D07126378404__INCLUDED_)
