# Microsoft Developer Studio Project File - Name="telepat" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=telepat - Win32 Release MinDependency
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "telepat.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "telepat.mak" CFG="telepat - Win32 Release MinDependency"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "telepat - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "telepat - Win32 MyDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "telepat - Win32 BuildResource" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "telepat"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "NOT_BUILD_RESOURCE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /map /machine:I386 /out:"C:\Program Files\1Cv77_25\BIN\config\telepat.dll" /filealign:512
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MyDebug"
# PROP BASE Intermediate_Dir "MyDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MyDebug"
# PROP Intermediate_Dir "MyDebug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "NOT_BUILD_RESOURCE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /out:"C:\Program Files\1Cv77_25\BIN\config\telepat.dll"

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "telepat___Win32_BuildResource"
# PROP BASE Intermediate_Dir "telepat___Win32_BuildResource"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BuildResource"
# PROP Intermediate_Dir "BuildResource"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "BUILD_RESOURCE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /profile /map /debug /machine:I386 /out:"C:\Program Files\1Cv77_25\BIN\config\telepat.dll"

!ENDIF 

# Begin Target

# Name "telepat - Win32 Release MinDependency"
# Name "telepat - Win32 MyDebug"
# Name "telepat - Win32 BuildResource"
# Begin Group "AllFiles"

# PROP Default_Filter ""
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Group "TxtTrap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyCmdTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCmdTarget.h
# End Source File
# Begin Source File

SOURCE=.\txttrap.cpp
# End Source File
# Begin Source File

SOURCE=.\txttrap.h
# End Source File
# End Group
# Begin Group "Lexical Analizer"

# PROP Default_Filter ""
# Begin Group "Lexer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lexRes1000

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# Begin Custom Build - Make Lex Resource on $(InputPath)
InputPath=.\lexRes1000

"$(InputPath).bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zipper $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StatesAndSymbols.inl
# End Source File
# Begin Source File

SOURCE=.\SwictTable.inl
# End Source File
# Begin Source File

SOURCE=.\syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\syntax.h
# End Source File
# End Group
# Begin Group "Parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BTYACC\CMethodParser

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# Begin Custom Build - Make YACC table on $(InputPath)
InputDir=.\BTYACC
InputPath=.\BTYACC\CMethodParser
InputName=CMethodParser

"$(InputDir)\CMethodParser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(InputDir) 
	do $(InputName) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

# Begin Custom Build - Make YACC table on $(InputPath)
InputDir=.\BTYACC
InputPath=.\BTYACC\CMethodParser
InputName=CMethodParser

"$(InputDir)\CMethodParser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(InputDir) 
	do $(InputName) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

# Begin Custom Build - Make YACC table on $(InputPath)
InputDir=.\BTYACC
InputPath=.\BTYACC\CMethodParser
InputName=CMethodParser

"$(InputDir)\CMethodParser.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(InputDir) 
	do $(InputName) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BTYACC\CMethodParser.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BTYACC\CMethodParser.h
# End Source File
# Begin Source File

SOURCE=.\BTYACC\CMethodParser.output
# End Source File
# Begin Source File

SOURCE=.\lexem2tokenMet.inl
# End Source File
# Begin Source File

SOURCE=.\lexRes1002

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# Begin Custom Build - Make Lex Resource on $(InputPath)
InputPath=.\lexRes1002

"$(InputPath).bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zipper $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

# Begin Custom Build - Make Lex Resource on $(InputPath)
InputPath=.\lexRes1002

"$(InputPath).bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zipper $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MethodParser.cpp

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# ADD CPP /FAcs

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MethodParser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BTYACC\DO.CMD
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# End Group
# Begin Group "List"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HintWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\HintWnd.h
# End Source File
# Begin Source File

SOURCE=.\TeleList.cpp
# End Source File
# Begin Source File

SOURCE=.\TeleList.h
# End Source File
# Begin Source File

SOURCE=.\TelePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\TelePanel.h
# End Source File
# Begin Source File

SOURCE=.\tentrydrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\tentrydrawing.h
# End Source File
# Begin Source File

SOURCE=.\ToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTip.h
# End Source File
# End Group
# Begin Group "Containers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KeyWords.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyWords.h
# End Source File
# Begin Source File

SOURCE=.\localvarcont.cpp
# End Source File
# Begin Source File

SOURCE=.\localvarcont.h
# End Source File
# Begin Source File

SOURCE=.\MetaGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\MetaGroup.h
# End Source File
# Begin Source File

SOURCE=.\proccont.cpp
# End Source File
# Begin Source File

SOURCE=.\proccont.h
# End Source File
# Begin Source File

SOURCE=.\TeleCont.cpp
# End Source File
# Begin Source File

SOURCE=.\TeleCont.h
# End Source File
# Begin Source File

SOURCE=.\TeleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TeleGroup.h
# End Source File
# Begin Source File

SOURCE=.\TForm.cpp
# End Source File
# Begin Source File

SOURCE=.\TForm.h
# End Source File
# Begin Source File

SOURCE=.\varcont.cpp
# End Source File
# Begin Source File

SOURCE=.\varcont.h
# End Source File
# End Group
# Begin Group "Plugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\telepat.cpp
# End Source File
# Begin Source File

SOURCE=.\telepat.idl
# ADD MTL /tlb ".\telepat.tlb" /h "telepat.h" /iid "telepat_i.c" /mktyplib203 /Oicf
# End Source File
# Begin Source File

SOURCE=.\telepatCP.h
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\blmap.cpp
# End Source File
# Begin Source File

SOURCE=.\blmap.h
# End Source File
# Begin Source File

SOURCE=.\HackVirtual.cpp
# End Source File
# Begin Source File

SOURCE=.\HackVirtual.h
# End Source File
# Begin Source File

SOURCE=.\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\Option.h
# End Source File
# End Group
# Begin Group "Types Containers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1cv7Lang.xml

!IF  "$(CFG)" == "telepat - Win32 Release MinDependency"

# Begin Custom Build - Make tls file
InputPath=.\1cv7Lang.xml

"1cv7lang.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	xml2tls\release\xml2tls 1cv7lang.xml 1cv7lang.bin a

# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 MyDebug"

# Begin Custom Build - Make tls file
InputPath=.\1cv7Lang.xml

"1cv7lang.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	xml2tls\release\xml2tls 1cv7lang.xml 1cv7lang.bin a

# End Custom Build

!ELSEIF  "$(CFG)" == "telepat - Win32 BuildResource"

# Begin Custom Build - Make tls file
InputPath=.\1cv7Lang.xml

"1cv7lang.bin" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	xml2tls\release\xml2tls 1cv7lang.xml 1cv7lang.bin a

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\typegroup.cpp
# End Source File
# Begin Source File

SOURCE=.\typegroup.h
# End Source File
# Begin Source File

SOURCE=.\TypesCont.cpp
# End Source File
# Begin Source File

SOURCE=.\TypesCont.h
# End Source File
# End Group
# Begin Group "CodeJump"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CJStack.cpp
# End Source File
# Begin Source File

SOURCE=.\CJStack.h
# End Source File
# Begin Source File

SOURCE=.\CodeJump.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeJump.h
# End Source File
# Begin Source File

SOURCE=.\EnumProcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumProcDlg.h
# End Source File
# End Group
# Begin Group "Param Wnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ParamWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamWnd.h
# End Source File
# End Group
# Begin Group "String Parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StringParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StringParser.h
# End Source File
# End Group
# Begin Group "Templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TemplateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TemplateInfo.h
# End Source File
# End Group
# Begin Group "ToolBar Buttons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MethComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MethComboBox.h
# End Source File
# End Group
# End Group
# Begin Group "Other"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\telepat.def
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\1cv7lang.bin
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\cmatrexp.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\cmatrpriv.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\cmmetexp.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\cmmetpriv.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\dlgfield.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\gmatrexp.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\gmatrpriv.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\gmmetexp.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\gmmetpriv.ico
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\keyatr.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\keyfunc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\keyword.ico
# End Source File
# Begin Source File

SOURCE=.\lexRes1000.bin
# End Source File
# Begin Source File

SOURCE=.\lexRes1001.bin
# End Source File
# Begin Source File

SOURCE=.\lexRes1002.bin
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\localvar.ico
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\metaatr.ico
# End Source File
# Begin Source File

SOURCE=.\plugin.bmp
# End Source File
# Begin Source File

SOURCE=.\Plugin.rgs
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\strings.ico
# End Source File
# Begin Source File

SOURCE=.\telepat.rc
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\templates.ico
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icons\LoColor\toolbar2.bmp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\MetaWork.cpp
# End Source File
# Begin Source File

SOURCE=.\MetaWork.h
# End Source File
# End Target
# End Project
# Section telepat : {5104473C-EE74-01E0-5FEE-000040B4D801}
# 	1:10:IDD_PLUGIN:104
# 	1:10:IDR_PLUGIN:103
# 	1:10:IDB_PLUGIN:102
# End Section
