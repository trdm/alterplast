# Microsoft Developer Studio Project File - Name="config" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CONFIG - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "config.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "config.mak" CFG="CONFIG - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "config - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "config - Win32 SemiDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "config - Win32 Debug_Home" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "config"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "config - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "MY_CONFIG_BUILD" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /machine:I386 /out:"C:\Program Files\1cV77_25\BIN\config.dll" /filealign:512
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "config - Win32 SemiDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SemiDebug"
# PROP BASE Intermediate_Dir "SemiDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SemiDebug"
# PROP Intermediate_Dir "SemiDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "MY_CONFIG_BUILD" /D "_SEMI_DEBUG" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32 /no_warn
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /out:"C:\Program Files\1cV77_25\BIN\config.dll"
# SUBTRACT LINK32 /force
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=chcp 1251 > nul	copy config.tlb "c:\Program Files\1CV77_25\BIN\config.tlb"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "config - Win32 Debug_Home"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "config___Win32_Debug_Home"
# PROP BASE Intermediate_Dir "config___Win32_Debug_Home"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug_Home"
# PROP Intermediate_Dir "Debug_Home"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "MY_CONFIG_BUILD" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "AT_HOME" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32 /no_warn
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /out:"C:\Program Files\1cV77_25\BIN\config.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy config.tlb "c:\Program Files\1Cv77\BIN\config.tlb" /y
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "config - Win32 Release"
# Name "config - Win32 SemiDebug"
# Name "config - Win32 Debug_Home"
# Begin Group "AllFiles"

# PROP Default_Filter ""
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Group "ActiveDoc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ActiveDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveDoc.h
# End Source File
# Begin Source File

SOURCE=.\OleFormsView.cpp
# End Source File
# Begin Source File

SOURCE=.\OleFormsView.h
# End Source File
# End Group
# Begin Group "About Box"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FireDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FireDlg.h
# End Source File
# Begin Source File

SOURCE=.\FIREWND.CPP
# End Source File
# Begin Source File

SOURCE=.\FIREWND.H
# End Source File
# End Group
# Begin Group "Hook Trap Wrap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HackVirtual.cpp
# End Source File
# Begin Source File

SOURCE=.\HackVirtual.h
# End Source File
# Begin Source File

SOURCE=.\IdleHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\IdleHandler.h
# End Source File
# Begin Source File

SOURCE=.\OCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\OCMenu.h
# End Source File
# Begin Source File

SOURCE=.\oleclientiemhook.h
# End Source File
# Begin Source File

SOURCE=.\oleclientitemhook.cpp
# End Source File
# Begin Source File

SOURCE=.\savewrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\savewrapper.h
# End Source File
# Begin Source File

SOURCE=.\Silenser.cpp
# End Source File
# Begin Source File

SOURCE=.\Silenser.h
# End Source File
# End Group
# Begin Group "Toolbars"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomCmdDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomCmdDlg1.h
# End Source File
# Begin Source File

SOURCE=.\SECToolBarMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SECToolBarMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\configsvcimpl.cpp

!IF  "$(CFG)" == "config - Win32 Release"

# ADD CPP /FAs

!ELSEIF  "$(CFG)" == "config - Win32 SemiDebug"

!ELSEIF  "$(CFG)" == "config - Win32 Debug_Home"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\configsvcimpl.h
# End Source File
# Begin Source File

SOURCE=.\Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Helper.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HotKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Initializer.cpp
# End Source File
# Begin Source File

SOURCE=.\Initializer.h
# End Source File
# Begin Source File

SOURCE=.\MacrosDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MacrosDlg.h
# End Source File
# Begin Source File

SOURCE=.\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\Option.h
# End Source File
# End Group
# Begin Group "Scripting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Scripting.cpp
# End Source File
# Begin Source File

SOURCE=.\Scripting.h
# End Source File
# Begin Source File

SOURCE=.\ScriptManeger.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptManeger.h
# End Source File
# End Group
# Begin Group "Plugins"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager.h
# End Source File
# Begin Source File

SOURCE=.\PluginPageDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginPageDoc.h
# End Source File
# Begin Source File

SOURCE=.\PluginPageView.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginPageView.h
# End Source File
# End Group
# Begin Group "OCPage"

# PROP Default_Filter ""
# Begin Group "TreeViewItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemMacros.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMacros.h
# End Source File
# Begin Source File

SOURCE=.\ItemScript.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemScript.h
# End Source File
# Begin Source File

SOURCE=.\ItemScripts.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemScripts.h
# End Source File
# Begin Source File

SOURCE=.\PluginItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginItem.h
# End Source File
# Begin Source File

SOURCE=.\PluginMacros.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginMacros.h
# End Source File
# Begin Source File

SOURCE=.\PluginsItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginsItem.h
# End Source File
# Begin Source File

SOURCE=.\TreeItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeItemInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\LeftTree.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftTree.h
# End Source File
# Begin Source File

SOURCE=.\OCDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OCDoc.h
# End Source File
# Begin Source File

SOURCE=.\OCView.cpp
# End Source File
# Begin Source File

SOURCE=.\OCView.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\IConfig.h
# End Source File
# Begin Source File

SOURCE=.\ITimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ITimer.h
# End Source File
# End Group
# Begin Group "MetaData"

# PROP Default_Filter ""
# Begin Group "Type1C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IType1C.cpp
# End Source File
# Begin Source File

SOURCE=.\IType1C.h
# End Source File
# End Group
# Begin Group "MetaDataObjects"

# PROP Default_Filter ""
# Begin Group "TaskDef"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ITaskDef.cpp
# End Source File
# Begin Source File

SOURCE=.\ITaskDef.h
# End Source File
# End Group
# Begin Group "Constants"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\IConstants.h
# End Source File
# Begin Source File

SOURCE=.\IConstDef.cpp
# End Source File
# Begin Source File

SOURCE=.\IConstDef.h
# End Source File
# End Group
# Begin Group "Enums"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IMetaEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\IMetaEnum.h
# End Source File
# Begin Source File

SOURCE=.\IMetaEnums.cpp
# End Source File
# Begin Source File

SOURCE=.\IMetaEnums.h
# End Source File
# Begin Source File

SOURCE=.\IMetaEnumVal.cpp
# End Source File
# Begin Source File

SOURCE=.\IMetaEnumVal.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\IMetaDataObjChilds.cpp
# End Source File
# Begin Source File

SOURCE=.\IMetaDataObjChilds.h
# End Source File
# Begin Source File

SOURCE=.\IMetaDataObjProps.cpp
# End Source File
# Begin Source File

SOURCE=.\IMetaDataObjProps.h
# End Source File
# Begin Source File

SOURCE=.\metadataobj.cpp
# End Source File
# Begin Source File

SOURCE=.\metadataobj.h
# End Source File
# Begin Source File

SOURCE=.\MetaDataObjDerived.cpp
# End Source File
# Begin Source File

SOURCE=.\MetaDataObjDerived.h
# End Source File
# End Group
# Begin Group "Arrays"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IRefers.cpp
# End Source File
# Begin Source File

SOURCE=.\IRefers.h
# End Source File
# Begin Source File

SOURCE=.\metadataobjarray.cpp
# End Source File
# Begin Source File

SOURCE=.\metadataobjarray.h
# End Source File
# End Group
# Begin Group "Migration"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IMigration.cpp
# End Source File
# Begin Source File

SOURCE=.\IMigration.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\metadata.cpp

!IF  "$(CFG)" == "config - Win32 Release"

!ELSEIF  "$(CFG)" == "config - Win32 SemiDebug"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "config - Win32 Debug_Home"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\metadata.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfgwindows.cpp
# End Source File
# Begin Source File

SOURCE=.\cfgwindows.h
# End Source File
# Begin Source File

SOURCE=.\ICfgWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ICfgWindow.h
# End Source File
# Begin Source File

SOURCE=.\IMDWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\IMDWindow.h
# End Source File
# End Group
# Begin Group "Modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IAllTypedObj.cpp
# End Source File
# Begin Source File

SOURCE=.\IAllTypedObj.h
# End Source File
# End Group
# Begin Group "IPlugins"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IPlugins.cpp
# End Source File
# Begin Source File

SOURCE=.\IPlugins.h
# End Source File
# End Group
# Begin Group "IScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IScripts.cpp
# End Source File
# Begin Source File

SOURCE=.\IScripts.h
# End Source File
# Begin Source File

SOURCE=.\ISelfScript.cpp
# End Source File
# Begin Source File

SOURCE=.\ISelfScript.h
# End Source File
# End Group
# Begin Group "CdgDocs"

# PROP Default_Filter ""
# Begin Group "Table"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ITableDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ITableDoc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ICfgDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ICfgDoc.h
# End Source File
# Begin Source File

SOURCE=.\ICfgDocuments.cpp
# End Source File
# Begin Source File

SOURCE=.\ICfgDocuments.h
# End Source File
# Begin Source File

SOURCE=.\IDialogDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\IDialogDoc.h
# End Source File
# Begin Source File

SOURCE=.\ITextDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ITextDoc.h
# End Source File
# Begin Source File

SOURCE=.\IWorkBook.cpp
# End Source File
# Begin Source File

SOURCE=.\IWorkBook.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DispImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\DispImpl.h
# End Source File
# Begin Source File

SOURCE=.\interfaces.idl
# ADD MTL /tlb "config.tlb" /h "interfaces.h"
# End Source File
# Begin Source File

SOURCE=.\tlibversion.h
# End Source File
# End Group
# Begin Group "other"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\config.def
# End Source File
# Begin Source File

SOURCE=.\config.rc
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "OCForms"

# PROP Default_Filter ""
# Begin Group "Design"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\FormCnt.cpp
# End Source File
# Begin Source File

SOURCE=.\FormCnt.h
# End Source File
# Begin Source File

SOURCE=.\FormDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDlg.h
# End Source File
# Begin Source File

SOURCE=.\FormDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDoc.h
# End Source File
# Begin Source File

SOURCE=.\FormHolder.cpp
# End Source File
# Begin Source File

SOURCE=.\FormHolder.h
# End Source File
# Begin Source File

SOURCE=.\FormScript.cpp
# End Source File
# Begin Source File

SOURCE=.\FormScript.h
# End Source File
# End Group
# Begin Group "Dockable Panels"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PanelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelManager.h
# End Source File
# Begin Source File

SOURCE=.\PanelSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\PanelView.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelView.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\177.ico
# End Source File
# Begin Source File

SOURCE=.\22.ico
# End Source File
# Begin Source File

SOURCE=.\RES\bmp128.bmp
# End Source File
# Begin Source File

SOURCE=.\vis1cpp\res\class.ico
# End Source File
# Begin Source File

SOURCE=.\res\config.rc2
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\vis1cpp\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\vis1cpp\res\openmet.ico
# End Source File
# Begin Source File

SOURCE=.\RES\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\vis1cpp\RES\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Texts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\history.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Group
# Begin Group "Docs"

# PROP Default_Filter ""
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Docs\Scripts\scripts.htm
# End Source File
# End Group
# Begin Group "_plugins"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Docs\plugins\icfgplugin.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\plugins\plugins.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\plugins\Samples\samplesvb.htm
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Docs\objects\objects.htm
# End Source File
# End Group
# Begin Source File

SOURCE=.\Docs\Contents.hhc
# End Source File
# Begin Source File

SOURCE=.\Docs\history.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\Index.hhk
# End Source File
# Begin Source File

SOURCE=.\Docs\macros.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\main.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\MENU.JS
# End Source File
# Begin Source File

SOURCE=.\Docs\OpenConf.hhp

!IF  "$(CFG)" == "config - Win32 Release"

!ELSEIF  "$(CFG)" == "config - Win32 SemiDebug"

# Begin Custom Build - Generate Help File
InputDir=.\Docs
InputPath=.\Docs\OpenConf.hhp

"$(InputDir)\openconf.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputDir)\hhc.exe $(InputPath) 
	set errorlevel=0 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "config - Win32 Debug_Home"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Docs\respect.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\setup.htm
# End Source File
# Begin Source File

SOURCE=.\Docs\STYLES.CSS
# End Source File
# Begin Source File

SOURCE=.\Docs\working.html
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LIBS\userdef.lib
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

SOURCE=.\LIBS\BASIC.lib
# End Source File
# End Group
# End Group
# End Target
# End Project
