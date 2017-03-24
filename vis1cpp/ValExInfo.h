// ValExInfo.h: interface for the CExportedVaInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALEXINFO_H__7D860C24_6258_11D7_8D5E_D07126378404__INCLUDED_)
#define AFX_VALEXINFO_H__7D860C24_6258_11D7_8D5E_D07126378404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CExportedVaInfo : public CClassMemberInfo //CInfo  
{
public:
// 	CExportedVaInfo(const CString& name):CInfo(name, TYPE_MEMBER_EXPORT){}
// 	virtual ~CExportedVaInfo();
// 	
// 	virtual void OnSelect();

	CExportedVaInfo(const CString& name) : CClassMemberInfo(name, TYPE_MEMBER_EXPORT) {}
};

#endif // !defined(AFX_VALEXINFO_H__7D860C24_6258_11D7_8D5E_D07126378404__INCLUDED_)
