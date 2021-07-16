# Microsoft Developer Studio Project File - Name="Enterprise" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Enterprise - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Enterprise.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Enterprise.mak" CFG="Enterprise - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Enterprise - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Enterprise - Win32 TestSpeed" (based on "Win32 (x86) Application")
!MESSAGE "Enterprise - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release"
# PROP Intermediate_Dir "../Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "_WINDLL" /D "_USRDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /version:7.6 /stack:0x200000 /subsystem:windows /machine:I386 /out:"C:\2C\Enterprise.exe"
# SUBTRACT LINK32 /profile /incremental:yes /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=copy CREDITS\add.txt+CREDITS\CREDITS.CPP CREDITS\CREDITS2.CPP	copy CREDITS\CREDITS2.CPP CREDITS\CREDITS.CPP
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Enterprise___Win32_TestSpeed"
# PROP BASE Intermediate_Dir "Enterprise___Win32_TestSpeed"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../TestSpeed"
# PROP Intermediate_Dir "../TestSpeed"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Zp4 /MD /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "__STATPROFUIS_WITH_DLLMFC__" /D "TEST_SPEED" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /version:7.5 /stack:0x100000 /subsystem:windows /machine:I386 /out:"C:\2C\Enterprise.exe"
# ADD LINK32 /nologo /version:7.6 /stack:0x200000 /subsystem:windows /profile /debug /machine:I386 /out:"C:\2C\Enterprise.exe" /libpath:"../GUI-DLL"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Enterprise___Win32_Debug"
# PROP BASE Intermediate_Dir "Enterprise___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Debug"
# PROP Intermediate_Dir "../Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /Zi /Od /D "MYDEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "__STATPROFUIS_WITH_DLLMFC__" /Fr /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /ZI /Od /D "MYDEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "__STATPROFUIS_WITH_DLLMFC__" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 MSVCRTD.LIB /nologo /version:7.5 /stack:0x300000 /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"C:\2C\Enterprise.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 MSVCRTD.LIB /nologo /version:7.6 /stack:0x200000 /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"MSVCRT" /out:"C:\2C\Enterprise.exe"

!ENDIF 

# Begin Target

# Name "Enterprise - Win32 Release"
# Name "Enterprise - Win32 TestSpeed"
# Name "Enterprise - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Форма"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonUnit.cpp

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DialogUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\DocUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\FormFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\FormUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\MicroForm.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenForm.cpp
# End Source File
# End Group
# Begin Group "Метаданные"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DocList.cpp

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileMeta.cpp
# End Source File
# Begin Source File

SOURCE=.\Metadata.cpp
# End Source File
# Begin Source File

SOURCE=.\MetadataTree.cpp
# End Source File
# Begin Source File

SOURCE=.\MetaObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MetedataDocument.cpp

!IF  "$(CFG)" == "Enterprise - Win32 Release"

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

# SUBTRACT BASE CPP /O<none> /Fr
# SUBTRACT CPP /O<none> /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ModuleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleView.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjInfo.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\EditForm\ChangeOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugMessageBar.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Enterprise.cpp
# End Source File
# Begin Source File

SOURCE=.\Enterprise.rc
# End Source File
# Begin Source File

SOURCE=.\FindTextDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionList.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDate.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\InputNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\InputPeriod.cpp
# End Source File
# Begin Source File

SOURCE=.\LineInput.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StackBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Форма (h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonUnit.h

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DialogUnit.h
# End Source File
# Begin Source File

SOURCE=.\DocUnit.h
# End Source File
# Begin Source File

SOURCE=.\FormFrame.h
# End Source File
# Begin Source File

SOURCE=.\FormUnit.h
# End Source File
# Begin Source File

SOURCE=.\MicroForm.h
# End Source File
# Begin Source File

SOURCE=.\OpenForm.h
# End Source File
# End Group
# Begin Group "Метаданные (h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DocList.h
# End Source File
# Begin Source File

SOURCE=.\FileMeta.h
# End Source File
# Begin Source File

SOURCE=.\Metadata.h
# End Source File
# Begin Source File

SOURCE=.\MetadataTree.h
# End Source File
# Begin Source File

SOURCE=.\MetaObject.h
# End Source File
# Begin Source File

SOURCE=.\MetedataDocument.h
# End Source File
# Begin Source File

SOURCE=.\ModuleDoc.h
# End Source File
# Begin Source File

SOURCE=.\ModuleManager.h
# End Source File
# Begin Source File

SOURCE=.\ModuleView.h
# End Source File
# Begin Source File

SOURCE=.\ObjInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EditForm\ChangeOrder.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DebugMessageBar.h
# End Source File
# Begin Source File

SOURCE=.\EditWnd.h
# End Source File
# Begin Source File

SOURCE=.\Enterprise.h
# End Source File
# Begin Source File

SOURCE=.\FindTextDialog.h
# End Source File
# Begin Source File

SOURCE=.\FunctionList.h
# End Source File
# Begin Source File

SOURCE=.\InputDate.h
# End Source File
# Begin Source File

SOURCE=.\InputDialog.h
# End Source File
# Begin Source File

SOURCE=.\InputNumeric.h
# End Source File
# Begin Source File

SOURCE=.\InputPeriod.h
# End Source File
# Begin Source File

SOURCE=.\LineInput.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MessageBar.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\plugdetect.h
# End Source File
# Begin Source File

SOURCE=.\ReportManager.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StackBar.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\_empty25.cur
# End Source File
# Begin Source File

SOURCE=.\res\_empty26.cur
# End Source File
# Begin Source File

SOURCE=.\ActvHost\res\arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calcbtn1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\calceql.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checkcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\combocur.cur
# End Source File
# Begin Source File

SOURCE=.\res\combocurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\configto.bmp
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

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00008.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00009.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00010.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00011.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\DARROW.BMP
# End Source File
# Begin Source File

SOURCE=.\res\datecurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\Dialog.ico
# End Source File
# Begin Source File

SOURCE=.\res\dragadd.cur
# End Source File
# Begin Source File

SOURCE=.\res\dropcbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dyntoolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\ActvHost\res\edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_cop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_cut.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_for.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_mak.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_pas.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_sec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\elb_add.bmp
# End Source File
# Begin Source File

SOURCE=.\res\elb_del.bmp
# End Source File
# Begin Source File

SOURCE=.\res\elb_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\elb_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\empty256.cur
# End Source File
# Begin Source File

SOURCE=.\res\Enterprise.ico
# End Source File
# Begin Source File

SOURCE=.\res\Enterprise.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EnterpriseDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\groupcur.cur
# End Source File
# Begin Source File

SOURCE=.\res\groupcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\horzdrag.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_ente.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_form.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_help.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_laye.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_meta.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_myfo.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_samp.ico
# End Source File
# Begin Source File

SOURCE=.\res\jurnaldo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\labelcurs.cur
# End Source File
# Begin Source File

SOURCE=.\ActvHost\res\layout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\listcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\mdi.ico
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\MenuButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\module.bmp
# End Source File
# Begin Source File

SOURCE=.\res\module1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\multieditcurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\NEXTMONT.BMP
# End Source File
# Begin Source File

SOURCE=.\res\NEXTYR.BMP
# End Source File
# Begin Source File

SOURCE=.\ActvHost\res\notfound.bmp
# End Source File
# Begin Source File

SOURCE=.\res\paddoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\PAN_HAND.CUR
# End Source File
# Begin Source File

SOURCE=.\res\pointer_.cur
# End Source File
# Begin Source File

SOURCE=.\res\PREVMONT.BMP
# End Source File
# Begin Source File

SOURCE=.\res\PREVYR.BMP
# End Source File
# Begin Source File

SOURCE=.\res\radiocurs.cur
# End Source File
# Begin Source File

SOURCE=.\res\RARROW.BMP
# End Source File
# Begin Source File

SOURCE=.\res\splash1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=".\Пиктограмки.bmp"
# End Source File
# End Group
# Begin Group "Интерпретатор!!"

# PROP Default_Filter ""
# Begin Group "CValue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Compiler\ChooseValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ChooseValue.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ExtOleDispatchDriver.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\Methods.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\Methods.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\Value.cpp

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compiler\Value.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueArray.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueComboBox.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueContext.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueContext.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueControl.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueFileSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueFileSystem.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueForm.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueForm.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueGrid.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueGridExt.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueGridExt.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueList.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueList.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueListBox.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueListControl.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueListLayers.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueListLayers.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMenu.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMeta.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMeta.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMetaFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueMetaFile.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueModule.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueObject.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueOLE.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueOLE.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueRange.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueRange.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueSQL.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueText.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueText.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\ValueTreeCtrl.h
# End Source File
# End Group
# Begin Group "Spell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Compiler\SPELL.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\SPELL.h
# End Source File
# End Group
# Begin Group "CStringExt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Compiler\StringExt.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\StringExt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Compiler\CompileModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\CompileModule.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\Definition.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\Functions.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\Functions.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\OutToDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\OutToDebug.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ProcUnit.cpp

!IF  "$(CFG)" == "Enterprise - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "Enterprise - Win32 TestSpeed"

!ELSEIF  "$(CFG)" == "Enterprise - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compiler\ProcUnit.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\ProcUnitContext.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Compiler\ProcUnitInline.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Compiler\SystemFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\SystemFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Compiler\TranslateModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Compiler\TranslateModule.h
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Group "Edit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\1CCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\1CCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Edit\amsEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\amsEdit.h
# End Source File
# Begin Source File

SOURCE=.\Edit\CalcBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\DropEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\DropEdit.h
# End Source File
# Begin Source File

SOURCE=.\Edit\PromptEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\PromptEdit.h
# End Source File
# Begin Source File

SOURCE=.\Edit\secaux.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\secaux.h
# End Source File
# Begin Source File

SOURCE=.\Edit\SecCalc.h
# End Source File
# Begin Source File

SOURCE=.\Edit\SecCalc2.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\sloadbmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\sloadbmp.h
# End Source File
# Begin Source File

SOURCE=.\Edit\xrnumericedit.cpp
# End Source File
# Begin Source File

SOURCE=.\Edit\xrnumericedit.h
# End Source File
# End Group
# Begin Group "EditLog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EditLog\_editlog_stream.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\debug.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\EditLog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLog\EditLog.h
# End Source File
# Begin Source File

SOURCE=.\EditLog\SubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLog\SubclassWnd.h
# End Source File
# End Group
# Begin Group "MsgBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgBox\timedmsgbox.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBox\timedmsgbox.h
# End Source File
# End Group
# Begin Group "Menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Menu\DynMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu\DynMenu.h
# End Source File
# Begin Source File

SOURCE=.\Menu\MenuListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu\MenuListBox.h
# End Source File
# End Group
# Begin Group "DirDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirDialog\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog\DirDialog.h
# End Source File
# End Group
# Begin Group "GridCtrl"

# PROP Default_Filter ""
# Begin Group "Cells"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridCtrl_src\Cells\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\Cells\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\Cells\GridURLCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\Cells\GridURLCell.h
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridCtrlExt.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridCtrlExt.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridDoc.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridView.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\GridView.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\Moxcel.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\Moxcel.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\Sections.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\View\undo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GridCtrl_src\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.h
# End Source File
# End Group
# Begin Group "DocumentBar_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DocumentBar_src\DocSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\DocSelect.h
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\MenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\MenuButton.h
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\SwitcherButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentBar_src\SwitcherButton.h
# End Source File
# End Group
# Begin Group "CREDITS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CREDITS\CREDITS.CPP
# End Source File
# Begin Source File

SOURCE=.\CREDITS\CREDITS.H
# End Source File
# End Group
# Begin Group "DragDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DragDialog\DragFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\DragDialog\DragFormView.h
# End Source File
# End Group
# Begin Group "IconListBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IconListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IconListBox.h
# End Source File
# End Group
# Begin Group "FListCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FListCtrl\FListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FListCtrl\FListCtrl.h
# End Source File
# End Group
# Begin Group "RichEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RichEdit\RichEditCtrlGS.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEdit\RichEditCtrlGS.h
# End Source File
# End Group
# Begin Group "SyntaxColor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutocomplectionCtrl\AutocomplectionCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\AutocomplectionCtrl\AutocomplectionCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalEditView.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalEditView.inl
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextBuffer.inl
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextView.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextView.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextView.inl
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CCrystalTextView2.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\cedefs.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CEditReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CEditReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CFindTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\CFindTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\editcmd.h
# End Source File
# Begin Source File

SOURCE=.\SyntaxColor\editreg.h
# End Source File
# End Group
# Begin Group "ImageButton"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageButton\ImageButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageButton\ImageButton.h
# End Source File
# End Group
# Begin Group "WindowManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WindowManager\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowManager\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\WindowManager\ViewManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowManager\ViewManager.h
# End Source File
# Begin Source File

SOURCE=.\WindowManager\WindowManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowManager\WindowManager.h
# End Source File
# Begin Source File

SOURCE=.\WindowManager\WindowTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowManager\WindowTabCtrl.h
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\help_view\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\help_view\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\help_view\CntrItemEg.cpp
# End Source File
# Begin Source File

SOURCE=.\help_view\CntrItemEg.h
# End Source File
# Begin Source File

SOURCE=.\help_view\MyHelpDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\help_view\MyHelpDoc.h
# End Source File
# Begin Source File

SOURCE=.\help_view\MyHelpView.cpp
# End Source File
# Begin Source File

SOURCE=.\help_view\MyHelpView.h
# End Source File
# End Group
# Begin Group "EditForm"

# PROP Default_Filter ""
# Begin Group "ObjectInspector"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObjectInspector\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\ColourPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\ColourPopup.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\GfxTools.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\ObjectInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\ObjectInspector.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\PropertyBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectInspector\PropertyBar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EditForm\DblSubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\DblSubclassWnd.h
# End Source File
# Begin Source File

SOURCE=.\EditForm\DynControl.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\DynControl.h
# End Source File
# Begin Source File

SOURCE=.\EditForm\FormDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\FormDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditForm\FormEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\FormEditor.h
# End Source File
# Begin Source File

SOURCE=.\EditForm\LayerSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\layerselect.h
# End Source File
# Begin Source File

SOURCE=.\EditForm\MultiRectTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\EditForm\MultiRectTracker.h
# End Source File
# End Group
# Begin Group "MySQL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MySQL\MySQL.cpp
# End Source File
# Begin Source File

SOURCE=.\MySQL\MySQL.h
# End Source File
# Begin Source File

SOURCE=.\MySQL\MySQL0.cpp
# End Source File
# Begin Source File

SOURCE=.\MySQL\MySQL0.h
# End Source File
# Begin Source File

SOURCE=.\MySQL\MySQL1.cpp
# End Source File
# Begin Source File

SOURCE=.\MySQL\MySQL1.h
# End Source File
# Begin Source File

SOURCE=.\MySQL\ODBCRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\MySQL\ODBCRecordset.h
# End Source File
# End Group
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SourceUT\OXDllExt.h
# End Source File
# Begin Source File

SOURCE=.\SourceUT\OXTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceUT\OXTabView.h
# End Source File
# Begin Source File

SOURCE=.\GUI\XColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI\XColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\GUI\XHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI\XHyperLink.h
# End Source File
# End Group
# End Group
# Begin Group "MyDDE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyDataExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDataExchange.h
# End Source File
# End Group
# Begin Group "DropTarget"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DropTarget\MyDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\DropTarget\MyDropTarget.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Enterprise.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
