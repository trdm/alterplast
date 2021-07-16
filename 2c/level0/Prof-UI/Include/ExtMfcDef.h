// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2003 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __EXT_MFC_DEF_H)
#define __EXT_MFC_DEF_H

#define __EXTMFC_ALL_FORM_MOVABLE_WND_STYLES (0x0000FFFFL)

#pragma warning ( disable : 4201 )
	#include <mmsystem.h>
#pragma warning ( default : 4201 )

#if (!defined __EXT_MFC_WINMMDLL_ALREADY_DYN_LIKNED)
	#define __EXT_MFC_WINMMDLL_ALREADY_DYN_LIKNED
	#pragma message("   Automatically linking with WinMM library")
	#pragma message("      (Windows Multimedia System)")
	#pragma comment( lib, "winmm.lib" ) 
#endif

// __PROF_UIS_VERSION_DWORD is required for static library builds
#define __PROF_UIS_VERSION_DWORD DWORD( 0x02020101 )

// the multiply monitor stub can be included into the library
// builds (used in ExtPaintManager.cpp to define
// COMPILE_MULTIMON_STUBS symbol before include multimon.h)
#if (defined _AFXDLL)
	#define __PROF_UIS_BUILTIN_MULTIMON_STUB
#endif // _AFXDLL
#if (defined __PROF_UIS_BUILTIN_MULTIMON_STUB)
	#pragma message("   Multiply monitor support: built-in")
#else
	#pragma message("   Multiply monitor support: external")
#endif // __PROF_UIS_BUILTIN_MULTIMON_STUB

// Prof-UIS basic library
#if (defined _AFXDLL)
	#if (defined __STATPROFUIS_WITH_DLLMFC__)
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221yud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221ymd.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221ynd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221yu.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221ym.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221yn.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS221ud.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221ud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_MODULE_NAME	_T("guid.dll")
				#define __PROF_UIS_LIB_NAME		   "guid.lib"
			#else
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS221nd.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221nd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS221u.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221u.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_MODULE_NAME	_T("gui.dll")
				#define __PROF_UIS_LIB_NAME		   "gui.lib"
			#else
				#define __PROF_UIS_MODULE_NAME	_T("ProfUIS221n.dll")
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221n.lib"
			#endif // _UNICODE
		#endif // _DEBUG
	#endif // __STATPROFUIS_WITH_DLLMFC__
#else
		#if (defined _DEBUG)
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221sud.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221smd.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221snd.lib"
			#endif // _UNICODE
		#else
			#if (defined _UNICODE)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221su.lib"
			#elif (defined _MBCS)
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221sm.lib"
			#else
				#define __PROF_UIS_LIB_NAME		   "ProfUIS221sn.lib"
			#endif // _UNICODE
		#endif // _DEBUG
#endif // _AFXDLL

#if (defined __PROF_UIS_IMPL__)
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		#define __PROF_UIS_API _declspec(dllexport)
	#else
		#define __PROF_UIS_API
	#endif // _AFXDLL
#else 
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
		#define __PROF_UIS_API _declspec(dllimport)
	#else
		#define __PROF_UIS_API
	#endif // _AFXDLL

	#pragma message("   Automatically linking with ProfUIS221 library")
	#pragma message("      (Professional User Interface Suite)")
	#pragma comment( lib, __PROF_UIS_LIB_NAME ) 
#endif

#if (defined __PROF_UIS_ADV_IMPL__)
	#if (!defined __ENABLED_PROF_UIS_ADV__)
		#define __ENABLED_PROF_UIS_ADV__
	#endif
#endif // #if (defined __PROF_UIS_ADV_IMPL__)

#if (defined __ENABLED_PROF_UIS_ADV__)

	// __PROF_UIS_VERSION_DWORD is required for static library builds
	#define __PROF_UIS_ADV_VERSION_DWORD DWORD( 0x02010501 )

	// Prof-UIS advanced library
	// (now for internal use only)

	#if (defined _AFXDLL)
		#if (defined __STATPROFUIS_WITH_DLLMFC__)
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221yud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221ymd.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221ynd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221yu.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221ym.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221yn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#else
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221ud.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221ud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221md.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221md.lib"
				#else
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221nd.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221nd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221u.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221u.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221m.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221m.lib"
				#else
					#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv221n.dll")
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221n.lib"
				#endif // _UNICODE
			#endif // _DEBUG
		#endif // __STATPROFUIS_WITH_DLLMFC__
	#else
			#if (defined _DEBUG)
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221sud.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221smd.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221snd.lib"
				#endif // _UNICODE
			#else
				#if (defined _UNICODE)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221su.lib"
				#elif (defined _MBCS)
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221sm.lib"
				#else
					#define __PROF_UIS_ADV_LIB_NAME		   "ProfUISAdv221sn.lib"
				#endif // _UNICODE
			#endif // _DEBUG
	#endif // _AFXDLL

	#if defined __PROF_UIS_ADV_IMPL__
		#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			#define __PROF_UIS_ADV_API _declspec(dllexport)
		#else
			#define __PROF_UIS_ADV_API
		#endif // _AFXDLL
	#else 
		#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
			#define __PROF_UIS_ADV_API _declspec(dllimport)
		#else
			#define __PROF_UIS_ADV_API
		#endif // _AFXDLL

		#pragma message("   Automatically linking with ProfUISAdv221 library")
		#pragma message("      (Professional User Interface Suite / Advanced Stuff)")
		#pragma comment( lib, __PROF_UIS_ADV_LIB_NAME ) 
	#endif
#endif // __ENABLED_PROF_UIS_ADV__

// Prof-UIS registry names
#define __PROF_UIS_REG_SECTION			_T("ProfUIS221")
#define __PROF_UIS_REG_PROFILES			_T("Profiles")
#define __PROF_UIS_REG_COMMAND_MANAGER	_T("CommandManager")
#define __PROF_UIS_REG_CONTROL_BAR		_T("ControlBar")
#define __PROF_UIS_REG_CUSTOMIZATION	_T("Customization")

#endif // __EXT_MFC_DEF_H
