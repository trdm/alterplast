# Microsoft Developer Studio Project File - Name="Vis1cpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Vis1cpp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Vis1cpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Vis1cpp.mak" CFG="Vis1cpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Vis1cpp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Vis1cpp - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Vis1cpp - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "_NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_DEBUG_INTERFACES" /D "USE_LOG_FILE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"T:\1cv77\Bin25\BIN\Config\Vis1cpp.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\1cv77\Bin25\BIN\Config\Vis1cpp.dll
InputPath=\1cv77\Bin25\BIN\Config\Vis1cpp.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Vis1cpp - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseMinDependency/Vis1cpp_ole.dll "
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\Vis1cpp_ole.dll
InputPath=.\ReleaseMinDependency\Vis1cpp_ole.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Vis1cpp - Win32 Debug"
# Name "Vis1cpp - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClassInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassProp.cpp
# End Source File
# Begin Source File

SOURCE=.\CPPDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CPPView.cpp
# End Source File
# Begin Source File

SOURCE=.\DeclProcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DefFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Info.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoWrcSpace.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MyModule.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTextDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcExInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RootProp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\TXTGET.CPP
# End Source File
# Begin Source File

SOURCE=.\ValExInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ValInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Vis1cpp.def
# End Source File
# Begin Source File

SOURCE=.\WrapperForSyntaxCheck.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClassInfo.h
# End Source File
# Begin Source File

SOURCE=.\ClassProp.h
# End Source File
# Begin Source File

SOURCE=.\configsvcimpl.h
# End Source File
# Begin Source File

SOURCE=.\CPPDoc.h
# End Source File
# Begin Source File

SOURCE=.\CPPView.h
# End Source File
# Begin Source File

SOURCE=.\DeclProcInfo.h
# End Source File
# Begin Source File

SOURCE=.\DefFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Info.h
# End Source File
# Begin Source File

SOURCE=.\InfoWrcSpace.h
# End Source File
# Begin Source File

SOURCE=.\ModuleParser.h
# End Source File
# Begin Source File

SOURCE=.\MyModule.h
# End Source File
# Begin Source File

SOURCE=.\MyTextDoc.h
# End Source File
# Begin Source File

SOURCE=.\ProcExInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProcInfo.h
# End Source File
# Begin Source File

SOURCE=.\RootProp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Syntax.h
# End Source File
# Begin Source File

SOURCE=.\TXTGET.H
# End Source File
# Begin Source File

SOURCE=.\ValExInfo.h
# End Source File
# Begin Source File

SOURCE=.\ValInfo.h
# End Source File
# Begin Source File

SOURCE=.\VIS1CPP_my.H
# End Source File
# Begin Source File

SOURCE=.\WrapperForSyntaxCheck.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\CLASS.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\class1.ico
# End Source File
# Begin Source File

SOURCE=.\CoClasses.rgs
# End Source File
# Begin Source File

SOURCE=.\CoClassInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\CoDefFile.rgs
# End Source File
# Begin Source File

SOURCE=.\CoDefFiles.rgs
# End Source File
# Begin Source File

SOURCE=.\RES\DEFFILE.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\ICO00001.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\ICO00002.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\ICON1.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\OPENMEMB.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\OPENMET.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\PRIVMEMB.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\PRIVMET.ICO
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RES\TOOLBAR1.BMP
# End Source File
# Begin Source File

SOURCE=.\Vis1cpp.rc
# End Source File
# Begin Source File

SOURCE=.\RES\VIS1CPP.RC2
# End Source File
# Begin Source File

SOURCE=.\Vis1cppPlugin.rgs
# End Source File
# End Group
# Begin Group "OpenConf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=config.h
# End Source File
# Begin Source File

SOURCE=config.tlb
# End Source File
# End Group
# Begin Group "1C"

# PROP Default_Filter ""
# Begin Group "Libs"

# PROP Default_Filter ""
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
# Begin Source File

SOURCE=.\LIBS\userdef.lib
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1CHEADERS\1cheaders.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\basic.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\bkend.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\blang.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\br32.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\dbeng32.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\editr.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\moxel.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\rgproc.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\seven.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\txtedt.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\type32.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\userdef.h
# End Source File
# End Group
# End Group
# Begin Group "COM-Plugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Vis1cpp.cpp
# End Source File
# Begin Source File

SOURCE=.\Vis1cpp.idl
# ADD MTL /tlb ".\Vis1cpp.tlb" /h "Vis1cpp.h" /iid "Vis1cpp_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\Vis1cppCP.h
# End Source File
# Begin Source File

SOURCE=.\Vis1cppPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Vis1cppPlugin.h
# End Source File
# End Group
# Begin Group "Texts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HISTORY.TXT
# End Source File
# Begin Source File

SOURCE=.\README.DOC
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Group
# Begin Group "1C++ Files"

# PROP Default_Filter ""
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1CPP\ComputeExpression.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\GlobalMethods.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\istrmap.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\MetaDataOfClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\ModuleString.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\Preprocessor.cpp
# End Source File
# End Group
# Begin Group "Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1CPP\ComputeExpression.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\GlobalMethods.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\istrmap.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\MetaDataOfClasses.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\ModuleString.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\Preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\1CPP\SetOfHierarchy.h
# End Source File
# End Group
# Begin Group "Assert"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1CPP\MyAssert.cpp
# End Source File
# Begin Source File

SOURCE=.\1CPP\MyAssert.h
# End Source File
# End Group
# End Group
# Begin Group "COM-Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CInterfacesCollection.h
# End Source File
# Begin Source File

SOURCE=.\CoClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\CoClasses.h
# End Source File
# Begin Source File

SOURCE=.\CoClassInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CoClassInfo.h
# End Source File
# Begin Source File

SOURCE=.\CoDefFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CoDefFile.h
# End Source File
# Begin Source File

SOURCE=.\CoDefFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\CoDefFiles.h
# End Source File
# End Group
# End Target
# End Project
