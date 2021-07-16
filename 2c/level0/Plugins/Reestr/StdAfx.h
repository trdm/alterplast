// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A8EB2999_A077_4C71_BB41_9D48E28EBBB2__INCLUDED_)
#define AFX_STDAFX_H__A8EB2999_A077_4C71_BB41_9D48E28EBBB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h> 



// TODO: reference additional headers your program requires here
#define IS_PLUGIN
#include"../../Compiler/plugdetect.h"
#include"../../Compiler/Methods.h"
#include"../../Compiler/value.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A8EB2999_A077_4C71_BB41_9D48E28EBBB2__INCLUDED_)
