// StringArrayEx.cpp: implementation of the CStringArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "StringArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStringArrayEx, CStringArray, VERSIONABLE_SCHEMA | 1)

CStringArrayEx::CStringArrayEx()
{

}

CStringArrayEx::~CStringArrayEx()
{

}

void CStringArrayEx::FillSeparateString(char const* str, const char *sep, bool bErase)
{
	char *token = NULL;
	char *dest = (char*)malloc(strlen(str)+1);

	if( dest )
	{
		strcpy(dest, str);

		token = strtok(dest, sep);
		while( token != NULL )
		{         
			CString s(token);
			s.TrimRight();
			s.TrimLeft();
			Add(s);
			token = strtok(NULL, sep);
		}

		free(dest);
	}
}

void CStringArrayEx::FillSeparateString(CString &str, const char *sep, bool bErase)
{
	FillSeparateString((LPCSTR)str, sep, bErase);
}
