// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "resource.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>
#include <atlctl.cpp>
#include <atlwin.cpp>

// все глобальные методы должны быть в файле system\\GlobalMethods.cpp
//
// надоело при изменении stdafx.cpp ждать компил€ции всего проекта, а не одного файла, 
// как при изменении и компил€ции других файлов :)
// файлы stdafx.h и stdafx.cpp лучше вообще не мен€ть

//_____________________________________________________________________________
//
