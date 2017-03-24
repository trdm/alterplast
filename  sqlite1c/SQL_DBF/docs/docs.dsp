# Microsoft Developer Studio Project File - Name="docs" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=docs - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "docs.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "docs.mak" CFG="docs - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "docs - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "htmlroot"
# PROP Target_Dir ""
# Begin Target

# Name "docs - Win32 Release"
# Begin Source File

SOURCE=.\doc.txt
# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\htmlroot
OutDir=.\..\bin
ProjDir=.
InputPath=.\doc.txt
InputName=doc

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set path=%path%;c:\Program Files\HTML Help Workshop 
	set inpname=$(InputName) 
	set intname="$(IntDir)\%inpname%.html" 
	set hhpname="$(IntDir)\%inpname%.hhp" 
	set outdir=$(OutDir) 
	set chmname="%outdir%\%inpname%.chm" 
	if exist %intname% del %intname% 
	c:\Python24\Scripts\rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "%outdir%" 
	if not exist %hhpname% ( 
	echo Error create %hhpname% and project files 
	exit 1 
	) 
	if exist %chmname% del %chmname% 
	if exist %chmname% ( 
	echo Error delete old %chmname% 
	exit 1 
	) 
	hhc %hhpname% > nul 
	if not exist %chmname% ( 
	echo Error create %chmname% 
	exit 1 
	) 
	call "$(IntDir)\%inpname%del.bat" 
	del "$(IntDir)\%inpname%del.bat" 
	exit 0 
	
# End Custom Build
# End Source File
# End Target
# End Project
