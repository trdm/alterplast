// ValInfo.h: interface for the CPrivatedValInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALINFO_H__7D860C23_6258_11D7_8D5E_D07126378404__INCLUDED_)
#define AFX_VALINFO_H__7D860C23_6258_11D7_8D5E_D07126378404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CPrivatedValInfo : public CClassMemberInfo //CInfo  
{
public:
// 	CPrivatedValInfo(const CString& name):CInfo(name, TYPE_MEMBER_PRIVATE) {}
// 	virtual ~CPrivatedValInfo();
// 	
// 	virtual void OnSelect();
	
	CPrivatedValInfo(const CString& name) : CClassMemberInfo(name, TYPE_MEMBER_PRIVATE) {}
};

#endif // !defined(AFX_VALINFO_H__7D860C23_6258_11D7_8D5E_D07126378404__INCLUDED_)
