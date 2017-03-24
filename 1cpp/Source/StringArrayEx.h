// StringArrayEx.h: interface for the CStringArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_)
#define AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxcoll.h>
#include <string.h>

class CStringArrayEx : public CStringArray  
{
  DECLARE_SERIAL(CStringArrayEx)
public:
	void FillSeparateString(char const* str, const char *sep, bool bErase = true);
	void FillSeparateString(CString& str, const char* sep = ",", bool bErase = true);
	CStringArrayEx();
	virtual ~CStringArrayEx();

};

#endif // !defined(AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_)
