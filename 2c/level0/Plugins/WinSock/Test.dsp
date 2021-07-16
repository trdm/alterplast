# Microsoft Developer Studio Project File - Name="Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Test.mak" CFG="Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Test - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Test - Win32 ReleaseD" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TEST_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUG2C_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib /nologo /dll /machine:I386 /out:"c:\2c\WinSock.dll"
# SUBTRACT LINK32 /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TEST_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUG2C_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib /nologo /dll /debug /machine:I386 /out:"c:\2c\WinSockD.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Test - Win32 ReleaseD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Test___Win32_ReleaseD"
# PROP BASE Intermediate_Dir "Test___Win32_ReleaseD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Test___Win32_ReleaseD"
# PROP Intermediate_Dir "Test___Win32_ReleaseD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUG2C_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLUG2C_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib /nologo /dll /machine:I386 /out:"c:\2c\WinSock.dll"
# SUBTRACT BASE LINK32 /incremental:yes /map /debug
# ADD LINK32 ws2_32.lib /nologo /dll /debug /machine:I386 /out:"c:\2c\WinSock.dll"
# SUBTRACT LINK32 /incremental:yes /map

!ENDIF 

# Begin Target

# Name "Test - Win32 Release"
# Name "Test - Win32 Debug"
# Name "Test - Win32 ReleaseD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Buffer2.cpp
# End Source File
# Begin Source File

SOURCE=.\CpuUsage.cpp
# End Source File
# Begin Source File

SOURCE=.\IOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Mapping.cpp
# End Source File
# Begin Source File

SOURCE=.\MyClientSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\MyServerSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\OutToDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketClass.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Test.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Buffer2.h
# End Source File
# Begin Source File

SOURCE=.\CpuUsage.h
# End Source File
# Begin Source File

SOURCE=.\IOCPServer.h
# End Source File
# Begin Source File

SOURCE=.\Mapping.h
# End Source File
# Begin Source File

SOURCE=.\MyClientSimple.h
# End Source File
# Begin Source File

SOURCE=.\MyServerSimple.h
# End Source File
# Begin Source File

SOURCE=.\OutToDebug.h
# End Source File
# Begin Source File

SOURCE=.\SocketClass.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Test.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "COM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Release\ServerCOM.tlh
# End Source File
# Begin Source File

SOURCE=.\Release\ServerCOM.tli
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Enterprise.lib
# End Source File
# End Target
# End Project
