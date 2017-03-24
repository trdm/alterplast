# Microsoft Developer Studio Project File - Name="parser_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=parser_test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "parser_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "parser_test.mak" CFG="parser_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "parser_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "parser_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "parser_test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ParserPlugin_Release"
# PROP Intermediate_Dir "ParserPlugin_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "parser_test - Win32 Debug"

# PROP BASE Use_MFC 0
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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "parser_test - Win32 Release"
# Name "parser_test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Analyser.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\istrmap.cpp
# End Source File
# Begin Source File

SOURCE=lang1c.tab.cpp
# End Source File
# Begin Source File

SOURCE=lang1c.y

!IF  "$(CFG)" == "parser_test - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=lang1c.y
InputName=lang1c

"$(InputName).tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\cygwin\bin\bison $(InputDir)\$(InputName).y -o  $(InputDir)\$(InputName).tab.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "parser_test - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputDir=.
InputPath=lang1c.y
InputName=lang1c

"$(InputName).tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison $(InputDir)\$(InputName).y -o  $(InputDir)\$(InputName).tab.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\lexer_1c.cpp
# End Source File
# Begin Source File

SOURCE=parser.cpp
# End Source File
# Begin Source File

SOURCE=parser_test.cpp

!IF  "$(CFG)" == "parser_test - Win32 Release"

!ELSEIF  "$(CFG)" == "parser_test - Win32 Debug"

# ADD CPP /ZI /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Types1C.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Analyser.h
# End Source File
# Begin Source File

SOURCE=.\istrmap.h
# End Source File
# Begin Source File

SOURCE=.\lang1c.tab.h
# End Source File
# Begin Source File

SOURCE=.\lexer_1c.h
# End Source File
# Begin Source File

SOURCE=parser.h
# End Source File
# Begin Source File

SOURCE=.\Types1C.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
