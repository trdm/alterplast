// MetaWork.h: interface for the CMetaWork class.
#if !defined(AFX_METAWORK_H__BA3ACAE2_9B32_4BB3_923B_68006E37DC7F__INCLUDED_)
#define AFX_METAWORK_H__BA3ACAE2_9B32_4BB3_923B_68006E37DC7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "blmap.h"

class CTypeWrap
{
public:
	CTypeWrap():m_type(0),m_id(0),next(NULL){}
	CTypeWrap(const CTypeWrap& t):m_type(t.m_type),m_id(t.m_id),next(NULL){}
	~CTypeWrap(){if(next)delete next;}
	void AddType(CTypeWrap* t)
	{
		t->next=next;
		next=t;
	}

	static CTypeWrap* FromCreateObject(CString name);
	int m_type;
	int m_id;
	CTypeWrap* next;

	static CStringArray	m_strTypes;
	static CBLMap		m_mapTypes;
};

class CMetaWork  
{
public:
	CMetaWork();
	~CMetaWork();
	static CTypeWrap* GetTypesFromProps(CString prop, CTypeWrap* pCurrentType);

};

#endif // !defined(AFX_METAWORK_H__BA3ACAE2_9B32_4BB3_923B_68006E37DC7F__INCLUDED_)
