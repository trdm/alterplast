// MyAssert.cpp: implementation of MyAssert
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//#include "MyAssert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// namespace Utils1s
// {
// 	
// void assertion_failed(char const * expr, char const * function, char const * file, long line) // user defined
// {
// 	//LPCSTR szError = "assertion_failed\r\n \t file - <%s>,\r\n \t line - <%d>,\r\n \t function - <%s>,\r\n \t expr - <%s>";
// 	//
// 	// 	LogErr(			szError, file, line, function, expr);
// 	// 	//MessageBox(		szError, file, line, function, expr);
// 	// 
// 	// 	RuntimeError(		szError, file, line, function, expr);
// 
// 	LPCSTR szError = "assertion_failed\r\n \t file - <%s>,\r\n \t line - <%d>,\r\n \t expr - <%s>";
// 	
// 	LogErr(			szError, file, line, expr);
// 	//MessageBox(		szError, file, line, expr);
// 	
// 	RuntimeError(		szError, file, line, expr);
// }
// 	
// } // namespace Utils1s
