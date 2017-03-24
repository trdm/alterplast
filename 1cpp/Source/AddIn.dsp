# Microsoft Developer Studio Project File - Name="AddIn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AddIn - Win32 Release MinDependency
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AddIn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AddIn.mak" CFG="AddIn - Win32 Release MinDependency"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AddIn - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AddIn - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""$/1C/AddIn/rainbow_ex/AddIn", QWEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AddIn - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /Od /D "WIN32" /D "_NDEBUG" /D "_ANSI" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /D "_AFXDLL" /D "DMSLG" /D "DelayLoad:Basic.dll" /D "_DEBUG_PURE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /o "NUL" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Netapi32.lib Rpcrt4.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WINMM.LIB version.lib /nologo /base:"0x24000000" /version:1.2 /subsystem:windows /dll /debug /machine:I386 /out:"Debug/1CPP.dll" /pdbtype:sept
# SUBTRACT LINK32 /profile /pdb:none

!ELSEIF  "$(CFG)" == "AddIn - Win32 Release MinDependency"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "_NDEBUG" /D "_ANSI" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_WINDLL" /D "_AFXDLL" /D "DMSLG" /D "DelayLoad:Basic.dll" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /o "NUL" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Rpcrt4.lib WINMM.LIB version.lib /base:"0x24000000" /dll /machine:IX86 /out:"ReleaseMinDependency/1CPP.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AddIn - Win32 Debug"
# Name "AddIn - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Components (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\blmap.cpp
# End Source File
# Begin Source File

SOURCE=.\BLModuleWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentClass.cpp
# End Source File
# Begin Source File

SOURCE=.\ComponentClassAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\ComputeExpression.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Factory.cpp
# End Source File
# Begin Source File

SOURCE=.\istrmap.cpp
# End Source File
# Begin Source File

SOURCE=.\System\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MetaDataOfClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleString.cpp
# End Source File
# Begin Source File

SOURCE=.\mycontextbase.cpp
# End Source File
# Begin Source File

SOURCE=.\PicService.cpp
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.cpp
# End Source File
# Begin Source File

SOURCE=.\SafeContext.cpp
# End Source File
# Begin Source File

SOURCE=.\SetOfHierarchy.cpp
# End Source File
# Begin Source File

SOURCE=.\System\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\V7Control.cpp
# End Source File
# Begin Source File

SOURCE=.\ValueCollection.cpp
# End Source File
# End Group
# Begin Group "Lib+ (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Delegate.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecuteModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\HashMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Struct.cpp
# End Source File
# Begin Source File

SOURCE=.\TableEx.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkAsRegister.cpp
# End Source File
# End Group
# Begin Group "Addon (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AddIn.cpp
# End Source File
# Begin Source File

SOURCE=.\AddIn.def
# End Source File
# Begin Source File

SOURCE=.\AddIn.idl
# PROP Exclude_From_Scan -1

!IF  "$(CFG)" == "AddIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\AddIn.idl

BuildCmds= \
	midl /Oicf /h "AddInTlb.h" /iid "AddIn_i.c" "AddIn.idl"

"AddInTlb.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"AddIn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"AddIn.Tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "AddIn - Win32 Release MinDependency"

# PROP BASE Exclude_From_Build 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\AddIn.idl

BuildCmds= \
	midl /Oicf /h "AddInTlb.h" /iid "AddIn_i.c" "AddIn.idl"

"AddInTlb.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"AddIn_i.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"AddIn.Tlb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\App.cpp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\System\GlobalMethods.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TemplateClass.cpp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "XTK (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\xt_list.cpp
# End Source File
# End Group
# Begin Group "DynaClasses (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DML.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaCrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaValue.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaValueList.cpp
# End Source File
# Begin Source File

SOURCE=.\RC6.cpp
# End Source File
# End Group
# Begin Group "SQL (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ODBC\1CPP_ODBCDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBC\1CPP_ODBCRecordSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=.\GUID.cpp
# End Source File
# Begin Source File

SOURCE=.\SQL\MetaDataWork.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBC\MetaNameParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MySQL_Query.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBC\Odbccore.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBC\SQLLock.cpp
# End Source File
# Begin Source File

SOURCE=.\ODBC\SQLNumeric.cpp
# End Source File
# End Group
# Begin Group "Metadata (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\CreateMetaInfoClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\StringArrayEx.cpp
# End Source File
# End Group
# Begin Group "TurboBL (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\TurboBL\allboost.cpp
# End Source File
# Begin Source File

SOURCE=.\TurboBL\boostgc.cpp
# End Source File
# Begin Source File

SOURCE=.\TurboBL\docrefboost.cpp
# End Source File
# Begin Source File

SOURCE=.\TurboBL\gcboost.cpp
# End Source File
# End Group
# Begin Group "CursorGrid (cpp)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CursorGrid\CursorGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\SQLProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7CursorGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7ODBCProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\VTProvider.cpp
# End Source File
# End Group
# Begin Group "OLEDB (cpp)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OLEDB\DBPropSet.cpp
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_cooledb.cpp
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledb.cpp
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledbparam.cpp
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledbparamhlpr.cpp
# End Source File
# Begin Source File

SOURCE=.\OLEDB\ValueTo.cpp
# End Source File
# End Group
# Begin Group "IndexedVT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VTExt\SerialStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTExtended.cpp
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTExtGrouping.cpp
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTExtRow.cpp
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTIterator.cpp
# End Source File
# End Group
# Begin Group "System (cpp)"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\System\Clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\System\compare.cpp
# End Source File
# Begin Source File

SOURCE=.\System\DllHooker.cpp
# End Source File
# Begin Source File

SOURCE=.\System\fix_alloc.cpp
# End Source File
# Begin Source File

SOURCE=.\System\GroupContextWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\System\Hooker.cpp
# End Source File
# Begin Source File

SOURCE=.\System\Informer.cpp
# End Source File
# Begin Source File

SOURCE=.\System\Math.cpp
# End Source File
# Begin Source File

SOURCE=.\System\ModuleEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\System\MyAddinContext.cpp
# End Source File
# Begin Source File

SOURCE=.\System\MyThread.cpp
# End Source File
# Begin Source File

SOURCE=.\System\pure_api.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\System\System.cpp
# End Source File
# End Group
# Begin Group "AXControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AXControl.cpp
# End Source File
# Begin Source File

SOURCE=.\AXControl.h
# End Source File
# End Group
# Begin Group "Form (cpp)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Form\V7Form.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Components (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\BLCollection.h
# End Source File
# Begin Source File

SOURCE=.\blmap.h
# End Source File
# Begin Source File

SOURCE=.\BLModuleWrapper.h
# End Source File
# Begin Source File

SOURCE=.\blptr.h
# End Source File
# Begin Source File

SOURCE=.\CastContext.h
# End Source File
# Begin Source File

SOURCE=.\ComponentClass.h
# End Source File
# Begin Source File

SOURCE=.\ComputeExpression.h
# End Source File
# Begin Source File

SOURCE=.\CtrlEvents.h
# End Source File
# Begin Source File

SOURCE=.\Factory.h
# End Source File
# Begin Source File

SOURCE=.\InitFormEx.h
# End Source File
# Begin Source File

SOURCE=.\istrmap.h
# End Source File
# Begin Source File

SOURCE=.\LOADER.H
# End Source File
# Begin Source File

SOURCE=.\System\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MapDefMethod.h
# End Source File
# Begin Source File

SOURCE=.\MetaDataOfClasses.h
# End Source File
# Begin Source File

SOURCE=.\MethodsWrapper.h
# End Source File
# Begin Source File

SOURCE=.\ModuleString.h
# End Source File
# Begin Source File

SOURCE=.\System\MyAddinContext.h
# End Source File
# Begin Source File

SOURCE=.\mycontextbase.h
# End Source File
# Begin Source File

SOURCE=.\ParamDefs.h
# End Source File
# Begin Source File

SOURCE=.\PicService.h
# End Source File
# Begin Source File

SOURCE=.\Preprocessor.h
# End Source File
# Begin Source File

SOURCE=.\PropDefs.h
# End Source File
# Begin Source File

SOURCE=.\SafeContext.h
# End Source File
# Begin Source File

SOURCE=.\SetOfHierarchy.h
# End Source File
# Begin Source File

SOURCE=.\System\Settings.h
# End Source File
# Begin Source File

SOURCE=.\TableEx.h
# End Source File
# Begin Source File

SOURCE=.\V7Control.h
# End Source File
# Begin Source File

SOURCE=.\ValueCollection.h
# End Source File
# End Group
# Begin Group "Lib+(h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CodeDecodeFile.h
# End Source File
# Begin Source File

SOURCE=.\Delegate.h
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# Begin Source File

SOURCE=.\ExecuteModule.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\HashMap.h
# End Source File
# Begin Source File

SOURCE=.\Struct.h
# End Source File
# End Group
# Begin Group "Addon (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AddIn.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\ext_header.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TemplateClass.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\WorkAsRegister.h
# End Source File
# End Group
# Begin Group "XTK (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\xt_list.h
# End Source File
# End Group
# Begin Group "DynaClasses (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DML.h
# End Source File
# Begin Source File

SOURCE=.\DynaCrypt.h
# End Source File
# Begin Source File

SOURCE=.\DynaValue.h
# End Source File
# Begin Source File

SOURCE=.\DynaValueList.h
# End Source File
# Begin Source File

SOURCE=.\RC6.h
# End Source File
# End Group
# Begin Group "SQL (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\ODBC\1CPP_ODBCDatabase.h
# End Source File
# Begin Source File

SOURCE=.\ODBC\1CPP_ODBCRecordSet.h
# End Source File
# Begin Source File

SOURCE=.\BinaryData.h
# End Source File
# Begin Source File

SOURCE=.\GUID.h
# End Source File
# Begin Source File

SOURCE=.\SQL\MetaDataWork.h
# End Source File
# Begin Source File

SOURCE=.\ODBC\MetaNameParser.h
# End Source File
# Begin Source File

SOURCE=.\MySQL_Query.h
# End Source File
# Begin Source File

SOURCE=.\ODBC\Odbccore.h
# End Source File
# Begin Source File

SOURCE=.\ODBC\SQLLock.h
# End Source File
# Begin Source File

SOURCE=.\ODBC\SQLNumeric.h
# End Source File
# End Group
# Begin Group "Metadata (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CreateMetaInfoClasses.h
# End Source File
# Begin Source File

SOURCE=.\StringArrayEx.h
# End Source File
# End Group
# Begin Group "TurboBL (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\TurboBL\allboost.h
# End Source File
# Begin Source File

SOURCE=.\TurboBL\boostgc.h
# End Source File
# Begin Source File

SOURCE=.\TurboBL\docrefboost.h
# End Source File
# Begin Source File

SOURCE=.\TurboBL\gcboost.h
# End Source File
# End Group
# Begin Group "CursorGrid (h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CursorGrid\commons.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\CursorGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\DataProvider.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\SQLProvider.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7CursorGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7DataProvider.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogEvent.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogFilter.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7LogProvider.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\V7ODBCProvider.h
# End Source File
# Begin Source File

SOURCE=.\CursorGrid\VTProvider.h
# End Source File
# End Group
# Begin Group "OLEDB (h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OLEDB\DBPropSet.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_ConvHlpr.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_cooledb.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledb.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledbparam.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledbparamhlpr.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\sp_oledbptr.h
# End Source File
# Begin Source File

SOURCE=.\OLEDB\ValueTo.h
# End Source File
# End Group
# Begin Group "IndexedVT (h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VTExt\BinaryTree.h
# End Source File
# Begin Source File

SOURCE=.\VTExt\RBTree.h
# End Source File
# Begin Source File

SOURCE=.\VTExt\SerialStorage.h
# End Source File
# Begin Source File

SOURCE=.\VTExt\VTExtended.h
# End Source File
# End Group
# Begin Group "System (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\System\Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\System\compare.h
# End Source File
# Begin Source File

SOURCE=.\System\DllHooker.h
# End Source File
# Begin Source File

SOURCE=.\System\fix_alloc.hpp
# End Source File
# Begin Source File

SOURCE=.\System\GlobalMethods.h
# End Source File
# Begin Source File

SOURCE=.\System\GroupContextWrap.h
# End Source File
# Begin Source File

SOURCE=.\System\Math.h
# End Source File
# Begin Source File

SOURCE=.\System\MyThread.h
# End Source File
# Begin Source File

SOURCE=.\System\sh_ptr.hpp
# End Source File
# Begin Source File

SOURCE=.\System\System.h
# End Source File
# End Group
# Begin Group "Form (h)"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Form\V7Form.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Addin.rc

!IF  "$(CFG)" == "AddIn - Win32 Debug"

!ELSEIF  "$(CFG)" == "AddIn - Win32 Release MinDependency"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ADDIN.RGS
# End Source File
# Begin Source File

SOURCE=.\bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\checks.bmp
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\logo.bmp
# End Source File
# End Group
# Begin Group "LIBS"

# PROP Default_Filter "LIB"
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

SOURCE=.\LIBS\SEVEN.lib
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
# Begin Group "1CHEADERS"

# PROP Default_Filter "h"
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

SOURCE=.\1CHEADERS\frame.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\moxel.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\mxl2xl.h
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

SOURCE=.\1CHEADERS\types.h
# End Source File
# Begin Source File

SOURCE=.\1CHEADERS\userdef.h
# End Source File
# End Group
# Begin Group "AboutDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\System\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\System\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\System\MyAboutWindowManager.cpp
# End Source File
# Begin Source File

SOURCE=.\System\MyAboutWindowManager.h
# End Source File
# Begin Source File

SOURCE=.\System\OwnAboutWindowManager.h
# End Source File
# Begin Source File

SOURCE=.\System\WrapAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\System\WrapAboutDlg.h
# End Source File
# End Group
# End Target
# End Project
# Section AddIn : {35053A22-8589-11D1-B16A-00C0F0283628}
# 	2:21:DefaultSinkHeaderFile:progressbar.h
# 	2:16:DefaultSinkClass:CProgressBar
# End Section
# Section AddIn : {35053A20-8589-11D1-B16A-00C0F0283628}
# 	2:5:Class:CProgressBar
# 	2:10:HeaderFile:progressbar.h
# 	2:8:ImplFile:progressbar.cpp
# End Section
