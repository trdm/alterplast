# Microsoft Developer Studio Project File - Name="ParserPlugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ParserPlugin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ParserPlugin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ParserPlugin.mak" CFG="ParserPlugin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ParserPlugin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ParserPlugin - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ParserPlugin - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ParserPlugin - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ParserPlugin_Debug"
# PROP Intermediate_Dir "ParserPlugin_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "1CHEADERS" /D "_DEBUG" /D "PARSER_PLUGIN" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\ParserPlugin_Debug
TargetPath=C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll
InputPath=C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ParserPlugin - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ParserPlugin_ReleaseMinSize"
# PROP Intermediate_Dir "ParserPlugin_ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I ".." /I "1CHEADERS" /D "NDEBUG" /D "_ATL_DLL" /D "PARSER_PLUGIN" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ParserPlugin_ReleaseMinSize
TargetPath=.\ParserPlugin_ReleaseMinSize\ParserPlugin.dll
InputPath=.\ParserPlugin_ReleaseMinSize\ParserPlugin.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ParserPlugin - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ParserPlugin_ReleaseMinDependency"
# PROP Intermediate_Dir "ParserPlugin_ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /I ".." /I "1CHEADERS" /D "NDEBUG" /D "_ATL_STATIC_REGISTRY" /D "PARSER_PLUGIN" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll"
# Begin Custom Build - Performing registration
OutDir=.\ParserPlugin_ReleaseMinDependency
TargetPath=C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll
InputPath=C:\Program Files\1Cv77\BIN\config\ParserPlugin.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ParserPlugin - Win32 Debug"
# Name "ParserPlugin - Win32 Release MinSize"
# Name "ParserPlugin - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Analyser.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeAnalyser.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonFunctions.cpp
# End Source File
# Begin Source File

SOURCE=..\istrmap.cpp
# End Source File
# Begin Source File

SOURCE=..\lang1c.tab.cpp
# End Source File
# Begin Source File

SOURCE=..\lang1c.y

!IF  "$(CFG)" == "ParserPlugin - Win32 Debug"

# Begin Custom Build
InputDir=\OpenConf_Scripts\—истемные‘айлы\1CLangParser
InputPath=..\lang1c.y
InputName=lang1c

"$(InputDir)\$(InputName).tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison $(InputDir)\$(InputName).y -o  $(InputDir)\$(InputName).tab.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ParserPlugin - Win32 Release MinSize"

# Begin Custom Build
InputDir=\OpenConf_Scripts\—истемные‘айлы\1CLangParser
InputPath=..\lang1c.y
InputName=lang1c

"$(InputDir)\$(InputName).tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison $(InputDir)\$(InputName).y -o  $(InputDir)\$(InputName).tab.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ParserPlugin - Win32 Release MinDependency"

# Begin Custom Build
InputDir=\OpenConf_Scripts\—истемные‘айлы\1CLangParser
InputPath=..\lang1c.y
InputName=lang1c

"$(InputDir)\$(InputName).tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison $(InputDir)\$(InputName).y -o  $(InputDir)\$(InputName).tab.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\lexer_1c.cpp
# End Source File
# Begin Source File

SOURCE=..\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserPlugin.def
# End Source File
# Begin Source File

SOURCE=.\ParserPlugin.idl
# ADD MTL /tlb ".\ParserPlugin.tlb" /h "ParserPlugin.h" /iid "ParserPlugin_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\ParserPlugin.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Types1C.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Analyser.h
# End Source File
# Begin Source File

SOURCE=.\CodeAnalyser.h
# End Source File
# Begin Source File

SOURCE=..\CommonFunctions.h
# End Source File
# Begin Source File

SOURCE=..\istrmap.h
# End Source File
# Begin Source File

SOURCE=..\lang1c.tab.h
# End Source File
# Begin Source File

SOURCE=..\lexer_1c.h
# End Source File
# Begin Source File

SOURCE=..\parser.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Types1C.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CodeAnalyser.rgs
# End Source File
# End Group
# End Target
# End Project
