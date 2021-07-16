// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// Functions.h: interface for the Functions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONS_H__E3B394F5_5BCE_47D2_95E6_A3C5B74001FE__INCLUDED_)
#define AFX_FUNCTIONS_H__E3B394F5_5BCE_47D2_95E6_A3C5B74001FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OutToDebug.h"

class CTranslateError //: public CException  
{
public:
	static CString GetErrorMessage();
};

bool FileExist(CString sFileName);
char *LoadFromFileBin(CString sFileName,int &size);
char *strmid(char *s,int nStart,int nLen=0);
char *strupper(char *s);
void setLayeredWindow( HWND hWnd, BYTE ab = 200 );


#endif // !defined(AFX_FUNCTIONS_H__E3B394F5_5BCE_47D2_95E6_A3C5B74001FE__INCLUDED_)
