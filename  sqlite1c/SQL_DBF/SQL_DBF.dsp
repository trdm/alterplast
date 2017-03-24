# Microsoft Developer Studio Project File - Name="SQL_DBF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SQL_DBF - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SQL_DBF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SQL_DBF.mak" CFG="SQL_DBF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SQL_DBF - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SQL_DBF - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SQL_DBF - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"bin/1sqlite.dll"

!ELSEIF  "$(CFG)" == "SQL_DBF - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"bin/1sqlite.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SQL_DBF - Win32 Release"
# Name "SQL_DBF - Win32 Debug"
# Begin Group "Основные"

# PROP Default_Filter ""
# Begin Group "Движок выборок"

# PROP Default_Filter ""
# Begin Group "Классы информации о таблицах"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\calcjournal.cpp
# End Source File
# Begin Source File

SOURCE=.\calcjournal.h
# End Source File
# Begin Source File

SOURCE=.\docheaders.cpp
# End Source File
# Begin Source File

SOURCE=.\docheaders.h
# End Source File
# Begin Source File

SOURCE=.\doctables.cpp
# End Source File
# Begin Source File

SOURCE=.\doctables.h
# End Source File
# Begin Source File

SOURCE=.\journal.cpp
# End Source File
# Begin Source File

SOURCE=.\journal.h
# End Source File
# Begin Source File

SOURCE=.\phisicalinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\phisicalinfo.h
# End Source File
# Begin Source File

SOURCE=.\referencetabinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\referencetabinfo.h
# End Source File
# Begin Source File

SOURCE=.\register.cpp
# End Source File
# Begin Source File

SOURCE=.\register.h
# End Source File
# Begin Source File

SOURCE=.\systabs.h
# End Source File
# Begin Source File

SOURCE=.\vtab_info.cpp
# End Source File
# Begin Source File

SOURCE=.\vtab_info.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\filtermachine.cpp
# End Source File
# Begin Source File

SOURCE=.\filtermachine.h
# End Source File
# Begin Source File

SOURCE=.\index_selector.h
# End Source File
# Begin Source File

SOURCE=.\longstrreader.cpp
# End Source File
# Begin Source File

SOURCE=.\longstrreader.h
# End Source File
# Begin Source File

SOURCE=.\strategycash.cpp
# End Source File
# Begin Source File

SOURCE=.\strategycash.h
# End Source File
# Begin Source File

SOURCE=.\utex.cpp
# End Source File
# Begin Source File

SOURCE=.\utex.h
# End Source File
# End Group
# Begin Group "1С часть"

# PROP Default_Filter ""
# Begin Group "Провайдер ТП"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dataprovider.cpp

!IF  "$(CFG)" == "SQL_DBF - Win32 Release"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "SQL_DBF - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dataprovider.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\database.cpp
# End Source File
# Begin Source File

SOURCE=.\database.h
# End Source File
# Begin Source File

SOURCE=.\metaparser.cpp

!IF  "$(CFG)" == "SQL_DBF - Win32 Release"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "SQL_DBF - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\metaparser.h
# End Source File
# Begin Source File

SOURCE=.\resultloader.h
# End Source File
# Begin Source File

SOURCE=.\SQL_DBF.cpp
# End Source File
# Begin Source File

SOURCE=.\valuework.hpp
# End Source File
# End Group
# End Group
# Begin Group "Ресурсы"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SQL_DBF.rc
# End Source File
# Begin Source File

SOURCE=.\res\SQL_DBF.rc2
# End Source File
# End Group
# Begin Group "Прочее"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SQL_DBF.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "SQLite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SQLite\sqlite3.c

!IF  "$(CFG)" == "SQL_DBF - Win32 Release"

# ADD CPP /w /W0 /GR- /GX- /Ox /Ot /Oa /Ow /Og /Oi /Op /Oy /Ob2 /D "VDBE_PROFILE" /D SQLITE_THREADSAFE=0 /D SQLITE_OMIT_SHARED_CACHE=1 /D SQLITE_OMIT_AUTHORIZATION=1 /D SQLITE_OMIT_TCL_VARIABLE=1 /D TEMP_STORE=2
# SUBTRACT CPP /Os /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "SQL_DBF - Win32 Debug"

# ADD CPP /w /W0 /GR- /GX- /D "VDBE_PROFILE" /D "NDEBUG_WINDOWS" /D SQLITE_THREADSAFE=0 /D SQLITE_OMIT_SHARED_CACHE=1 /D SQLITE_OMIT_AUTHORIZATION=1 /D SQLITE_OMIT_TCL_VARIABLE=1 /D TEMP_STORE=2
# SUBTRACT CPP /D "_WINDOWS" /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
