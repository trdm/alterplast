# Microsoft Developer Studio Project File - Name="RWidjets" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RWidjets - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RWidjets.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RWidjets.mak" CFG="RWidjets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RWidjets - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RWidjets - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RWidjets - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "_NDEBUG" /D "_ANSI" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /D "_AFXDLL" /D "DMSLG" /D "DelayLoad:Basic.dll" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Rpcrt4.lib WINMM.LIB version.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\RWidjets.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(TargetPath) "C:\Program Files\1Cv77\BIN\"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RWidjets - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RTools___Win32_Debug0"
# PROP BASE Intermediate_Dir "RTools___Win32_Debug0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RTools___Win32_Debug0"
# PROP Intermediate_Dir "RTools___Win32_Debug0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RWidjets - Win32 Release"
# Name "RWidjets - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MyContext\blmap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MyContext\mycontextbase.cpp
# End Source File
# Begin Source File

SOURCE=.\RaiseEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\RHook.cpp
# End Source File
# Begin Source File

SOURCE=.\RMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\RMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\RMsgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\RWidjets.cpp
# End Source File
# Begin Source File

SOURCE=.\RWidjets.def
# End Source File
# Begin Source File

SOURCE=.\RWidjets.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DlgMsg.h
# End Source File
# Begin Source File

SOURCE=.\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RHook.h
# End Source File
# Begin Source File

SOURCE=.\RMenu.h
# End Source File
# Begin Source File

SOURCE=.\RMenuItem.h
# End Source File
# Begin Source File

SOURCE=.\RMsgWindow.h
# End Source File
# Begin Source File

SOURCE=.\RWidjets.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\RWidjets.rc2
# End Source File
# End Group
# Begin Group "LIB"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=.\LIBS\BASIC.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\bkend.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\blang.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\br32.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\dbeng32.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\editr.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\FRAME.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\moxel.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\mxl2xl.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\rgproc.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\seven.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\txtedt.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\type32.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\userdef.lib
# End Source File
# End Group
# End Target
# End Project
# Section RWidjets : {00808000-3A94-0035-8000-800080800000}
# 	1:10:IDR_RTOOLS:6000
# End Section
