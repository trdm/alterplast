# Microsoft Developer Studio Project File - Name="profuislib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=profuislib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "profuislib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "profuislib.mak" CFG="profuislib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "profuislib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "profuislib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "profuislib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "profuislib___Win32_Release"
# PROP BASE Intermediate_Dir "profuislib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../ReleasePRofUI"
# PROP Intermediate_Dir "../../../ReleasePRofUI"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\Include" /D "NDEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /I "..\Include" /D "NDEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\GUI-DLL\ProfUIS221ym.lib"
# ADD LIB32 /nologo /out:"..\..\..\GUI-DLL\ProfUIS221ym.lib"

!ELSEIF  "$(CFG)" == "profuislib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "profuislib___Win32_Debug"
# PROP BASE Intermediate_Dir "profuislib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../DebugPRofUI"
# PROP Intermediate_Dir "../../../DebugPRofUI"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\GUI-DLL\ProfUIS221ymd.lib"
# ADD LIB32 /nologo /out:"..\..\..\GUI-DLL\ProfUIS221ymd.lib"

!ENDIF 

# Begin Target

# Name "profuislib - Win32 Release"
# Name "profuislib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Src\exdlgbase.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtBtnOnFlat.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtButton.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtCmdIcon.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtCmdManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtColorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\extcontrolbar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtDockBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtHook.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtHook.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtMenuControlBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtMiniDockFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtMouseCaptureSink.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtPaintManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtPopupMenuWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtStatusControlBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\ExtToolControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\ExtBtnOnFlat.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtButton.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtCmdIcon.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtCmdManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtColorCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtColorDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtComboBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtDockBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtMenuControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtMfcDef.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtMiniDockFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtMouseCaptureSink.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtPaintManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtPopupMenuWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtResizableDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtStatusControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtTempl.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtToolControlBar.h
# End Source File
# Begin Source File

SOURCE="..\Include\Prof-UIS.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
