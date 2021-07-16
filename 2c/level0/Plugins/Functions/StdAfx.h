// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_)
#define AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_

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
#include "Enterprise/plugdetect.h"
#include "Enterprise/value.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__A9FD83D6_FABE_47A0_814A_8F3C890E7720__INCLUDED_)
