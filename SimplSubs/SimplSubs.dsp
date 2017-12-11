# Microsoft Developer Studio Project File - Name="SimplSubs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SIMPLSUBS - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimplSubs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimplSubs.mak" CFG="SIMPLSUBS - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimplSubs - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SimplSubs - Win32 MyDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimplSubs - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"C:\Program Files\1Cv77\BIN\config\SimplSubs.dll"

!ELSEIF  "$(CFG)" == "SimplSubs - Win32 MyDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SimplSubs___Win32_MyDebug"
# PROP BASE Intermediate_Dir "SimplSubs___Win32_MyDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MyDebug"
# PROP Intermediate_Dir "MyDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"C:\Program Files\1Cv77\BIN\config\SimplSubs.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\Program Files\1Cv77\BIN\config\SimplSubs.dll"

!ENDIF 

# Begin Target

# Name "SimplSubs - Win32 Release"
# Name "SimplSubs - Win32 MyDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseView.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\InputStrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MDSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\MltiTree.cpp
# End Source File
# Begin Source File

SOURCE=.\MyADO.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyView.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplSubs.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplSubs.def
# End Source File
# Begin Source File

SOURCE=.\SimplSubs.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubsView.cpp
# End Source File
# Begin Source File

SOURCE=.\SubsViewMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseView.h
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeView.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\InputStrDlg.h
# End Source File
# Begin Source File

SOURCE=.\MDSplitter.h
# End Source File
# Begin Source File

SOURCE=.\MltiTree.h
# End Source File
# Begin Source File

SOURCE=.\MyADO.h
# End Source File
# Begin Source File

SOURCE=.\MyDoc.h
# End Source File
# Begin Source File

SOURCE=.\MyListBox.h
# End Source File
# Begin Source File

SOURCE=.\MyView.h
# End Source File
# Begin Source File

SOURCE=.\plugin.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubsView.h
# End Source File
# Begin Source File

SOURCE=.\SubsViewMain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FRAME.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\SimplSubs.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "1С"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=.\LIBS\userdef.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\Account.lib
# End Source File
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

SOURCE=.\LIBS\Salary.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\seven.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\Tracer.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\Trade.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\txtedt.lib
# End Source File
# Begin Source File

SOURCE=.\LIBS\type32.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\TO DO.txt"
# End Source File
# Begin Source File

SOURCE=".\Задачи.txt"
# End Source File
# Begin Source File

SOURCE=".\Наброски.txt"
# End Source File
# Begin Source File

SOURCE=".\описание.txt"
# End Source File
# Begin Source File

SOURCE=".\Docs\От Орефкова (отредактировано).txt"
# End Source File
# Begin Source File

SOURCE=".\Docs\От Орефкова.txt"
# End Source File
# End Target
# End Project
