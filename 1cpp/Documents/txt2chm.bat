@echo off

rem Для работы скрипта необходимо установить пакет DocUtils
rem (http://docutils.sourceforge.net/index.html)
rem и прописать путь к нему в переменную среды PATH
rem
rem Также должен быть установлен путь к HTML Help Workshop
rem 
rem set PATH=%PATH%;S:\docutils\tools

if "%1"=="" echo USAGE: txt2chm document.txt
if "%1"=="" goto EXIT

set NAME=%~n1
set INTFLDR=%~dp1html
set CHMFLDR=%~dp1chm
set intNAME=%INTFLDR%\%NAME%.html
set hhpNAME=%INTFLDR%\%NAME%.hhp

if not exist "%INTFLDR%" md "%INTFLDR%"
if not exist "%CHMFLDR%" md "%CHMFLDR%"

echo Building %NAME% : html
if exist "%intNAME%" del "%intNAME%"
rst2html.py --output-encoding CP1251 --date --time --generator "%1" "%intNAME%" || goto EXIT

rem echo Building %NAME% : xml
rem rst2xml.py  --output-encoding CP1251 "%1" "%INTFLDR%\%NAME%.xml" || goto EXIT

echo Building %~nx1 : hhp
cscript html2chm.js "%intNAME%" "%CHMFLDR%" || goto EXIT

echo Building %NAME% : chm
hhc.exe "%hhpNAME%"

call "%INTFLDR%\%NAME%del.bat"
del  "%INTFLDR%\%NAME%del.bat"

:EXIT
