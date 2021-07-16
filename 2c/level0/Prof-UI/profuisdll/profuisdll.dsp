# Microsoft Developer Studio Project File - Name="profuisdll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=profuisdll - Win32 MBCS Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "profuisdll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "profuisdll.mak" CFG="profuisdll - Win32 MBCS Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "profuisdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 MBCS Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 MBCS Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "profuisdll - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\GUI-DLL"
# PROP Intermediate_Dir "..\..\..\GUI-DLL\Release\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_WINDLL" /D "_AFXEXT" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\GUI-DLL/ProfUIS221n.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "profuisdll - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\GUI-DLL"
# PROP Intermediate_Dir "..\..\..\GUI-DLL\Debug\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_AFXDLL" /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\GUI-DLL/ProfUIS221nd.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "profuisdll - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "profuisdll___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "profuisdll___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin6"
# PROP Intermediate_Dir "..\Bin6\UnicodeDebug\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "_UNICODE" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin6/ProfUIS21d.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin6/ProfUIS221ud.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "profuisdll - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "profuisdll___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "profuisdll___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin6"
# PROP Intermediate_Dir "..\Bin6\UnicodeRelease\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_WINDLL" /D "_AFXEXT" /D "_UNICODE" /D "_AFXDLL" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin6/ProfUIS21.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin6/ProfUIS221u.dll"

!ELSEIF  "$(CFG)" == "profuisdll - Win32 MBCS Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "profuisdll___Win32_MBCS_Debug"
# PROP BASE Intermediate_Dir "profuisdll___Win32_MBCS_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\GUI-DLL"
# PROP Intermediate_Dir "..\..\..\MbcsDebug\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "_WINDLL" /D "_AFXEXT" /D "_AFXDLL" /D "_MBCS" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin6/ProfUIS221d.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\GUI-DLL\guid.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "profuisdll - Win32 MBCS Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "profuisdll___Win32_MBCS_Release"
# PROP BASE Intermediate_Dir "profuisdll___Win32_MBCS_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\GUI-DLL\"
# PROP Intermediate_Dir "..\..\..\MbcsRelease\profuisdll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_WINDLL" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "_WINDLL" /D "_AFXEXT" /D "_AFXDLL" /D "_MBCS" /D "__PROF_UIS_IMPL__" /D _WIN32_WINNT=0x0400 /D _WIN32_IE=0x0400 /D "WIN32" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin6/ProfUIS221.dll"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\GUI-DLL\gui.dll"

!ENDIF 

# Begin Target

# Name "profuisdll - Win32 Release"
# Name "profuisdll - Win32 Debug"
# Name "profuisdll - Win32 Unicode Debug"
# Name "profuisdll - Win32 Unicode Release"
# Name "profuisdll - Win32 MBCS Debug"
# Name "profuisdll - Win32 MBCS Release"
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

SOURCE=..\Src\ExtControlBar.cpp
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
# ADD CPP /I "..\profuisdll"
# End Source File
# Begin Source File

SOURCE=..\Src\ExtToolControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\profuisdll.cpp
# End Source File
# Begin Source File

SOURCE=.\profuisdll.def
# End Source File
# Begin Source File

SOURCE=.\profuisdll.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
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

SOURCE=..\Include\ExtCustomize.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtDockBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\ExtEdit.h
# End Source File
# Begin Source File

SOURCE=..\Src\ExtHook.h
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

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cmd00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cmd00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cmd00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\id_view_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idd_ext_.ico
# End Source File
# Begin Source File

SOURCE=.\idd_ext_color_dlg.ico
# End Source File
# Begin Source File

SOURCE=.\res\idd_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\ied_cp.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_el.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_fill.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_line.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_pen.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_rect.cur
# End Source File
# Begin Source File

SOURCE=.\res\ied_tool.cur
# End Source File
# Begin Source File

SOURCE=.\res\profuisdll.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
