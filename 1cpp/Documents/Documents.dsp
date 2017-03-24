# Microsoft Developer Studio Project File - Name="Documents" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=Documents - Win32 CHM
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Documents.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Documents.mak" CFG="Documents - Win32 CHM"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Documents - Win32 HTML" (based on "Win32 (x86) Generic Project")
!MESSAGE "Documents - Win32 ALS" (based on "Win32 (x86) Generic Project")
!MESSAGE "Documents - Win32 CHM" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Documents"
# PROP Scc_LocalPath "."
MTL=midl.exe

!IF  "$(CFG)" == "Documents - Win32 HTML"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HTML"
# PROP BASE Intermediate_Dir "HTML"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "HTML"
# PROP Intermediate_Dir "HTML"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ALS"
# PROP BASE Intermediate_Dir "ALS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ALS"
# PROP Intermediate_Dir "ALS"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Documents___Win32_CHM"
# PROP BASE Intermediate_Dir "Documents___Win32_CHM"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "chm"
# PROP Intermediate_Dir "html"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Documents - Win32 HTML"
# Name "Documents - Win32 ALS"
# Name "Documents - Win32 CHM"
# Begin Group "Users Docs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\1C++.als"
# End Source File
# Begin Source File

SOURCE=.\1cpp.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\1cpp.txt
InputName=1cpp

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\1cpp.txt
InputName=1cpp

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ActiveX.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\ActiveX.txt
InputName=ActiveX

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\ActiveX.txt
InputName=ActiveX

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 --language=ru "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BinaryData.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\BinaryData.txt
InputName=BinaryData

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\BinaryData.txt
InputName=BinaryData

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\classes.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\classes.txt
InputName=classes

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\classes.txt
InputName=classes

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 --language=ru "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DataProviderSDK.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция SDK по поставщикам данных
IntDir=.\HTML
InputPath=.\DataProviderSDK.txt
InputName=DataProviderSDK

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\DataProviderSDK.txt
InputName=DataProviderSDK

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\extobjs.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\extobjs.txt
InputName=extobjs

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\extobjs.txt
InputName=extobjs

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FormEx.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\HTML
InputPath=.\FormEx.txt
InputName=FormEx

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\FormEx.txt
InputName=FormEx

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\guid.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\guid.txt
InputName=guid

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\guid.txt
InputName=guid

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\history.txt
# End Source File
# Begin Source File

SOURCE=.\Hooker.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\Hooker.txt
InputName=Hooker

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IndexedTable.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция доки по Индексированной таблице
IntDir=.\HTML
InputPath=.\IndexedTable.txt
InputName=IndexedTable

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\IndexedTable.txt
InputName=IndexedTable

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Informer.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\Informer.txt
InputName=Informer

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 --tab-width 4 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LogDataProvider.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция документации по Log Provider'у
IntDir=.\HTML
InputPath=.\LogDataProvider.txt
InputName=LogDataProvider

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\LogDataProvider.txt
InputName=LogDataProvider

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build
IntDir=.\HTML
InputPath=.\Math.txt
InputName=Math

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\Math.txt
InputName=Math

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ODBC.TXT

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция доки по ODBC
IntDir=.\HTML
InputPath=.\ODBC.TXT
InputName=ODBC

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\ODBC.TXT
InputName=ODBC

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\oledb.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция доки по OLEDB
IntDir=.\HTML
InputPath=.\oledb.txt
InputName=oledb

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\oledb.txt
InputName=oledb

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SettingsManager.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\SettingsManager.txt
InputName=SettingsManager

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TableField.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция доки по Табличному полю
IntDir=.\HTML
InputPath=.\TableField.txt
InputName=TableField

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\TableField.txt
InputName=TableField

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TurboBL.txt
# End Source File
# End Group
# Begin Group "Developers Docs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\chmcreate.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\chmcreate.txt
InputName=chmcreate

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DevelopersReadme.txt
# End Source File
# Begin Source File

SOURCE=.\DocumentsRules.txt

!IF  "$(CFG)" == "Documents - Win32 HTML"

# Begin Custom Build - Компиляция правил оформления документации
IntDir=.\HTML
InputPath=.\DocumentsRules.txt
InputName=DocumentsRules

"$(IntDir)\$(InputName).html" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	rst2html.py --output-encoding CP1251 $(InputPath) $(IntDir)\$(InputName).html

# End Custom Build

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
IntDir=.\html
OutDir=.\chm
ProjDir=.
InputPath=.\DocumentsRules.txt
InputName=DocumentsRules

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(IntDir)\$(InputName).html" 
	set hhpname="$(IntDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	if exist %intname% del %intname% 
	rst2html.py --output-encoding=CP1251 "$(InputPath)" %intname% 
	if not exist %intname% ( 
	echo Error create %intname% 
	exit 1 
	) 
	cscript "$(ProjDir)\html2chm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(IntDir)\$(InputName)del.bat" 
	del "$(IntDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Release.txt
# End Source File
# End Group
# Begin Group "Resource files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\commonchm.txt
# End Source File
# Begin Source File

SOURCE=.\html\default.css
# End Source File
# Begin Source File

SOURCE=.\html\img\seealso.PNG
# End Source File
# End Group
# Begin Group "KB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\KB\TSQLInV7FirstSteps.html

!IF  "$(CFG)" == "Documents - Win32 HTML"

!ELSEIF  "$(CFG)" == "Documents - Win32 ALS"

!ELSEIF  "$(CFG)" == "Documents - Win32 CHM"

# Begin Custom Build - Компиляция доки из $(InputName)
InputDir=.\KB
OutDir=.\chm
ProjDir=.
InputPath=.\KB\TSQLInV7FirstSteps.html
InputName=TSQLInV7FirstSteps

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set intname="$(InputPath)" 
	set hhpname="$(InputDir)\$(InputName).hhp" 
	set chmname="$(OutDir)\$(InputName).chm" 
	cscript "$(ProjDir)\simplechm.js" //nologo //d %intname% "$(OutDir)" 
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
	call "$(InputDir)\$(InputName)del.bat" 
	del "$(InputDir)\$(InputName)del.bat" 
	exit 0 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
