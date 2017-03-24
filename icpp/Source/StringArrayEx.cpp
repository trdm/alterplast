// StringArrayEx.cpp: implementation of the CStringArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringArrayEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CStringArrayEx::FillSeparateString(LPCTSTR str, LPCTSTR sep, bool bErase)
{
	char *token = NULL;
	std::vector<char> vect(strlen(str)+1);
	char *dest = &vect[0];

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
}