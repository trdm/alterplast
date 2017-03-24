
@echo off

rem Для работы скрипта необходимо установить пакет DocUtils (http://docutils.sourceforge.net/index.html)
rem и прописать путь к нему в переменную среды PATH
rem
rem set PATH=%PATH%;S:\docutils\tools

if "%1"=="" goto EXIT

set NAME=%~n1
set INTFLDR=%~dp1site
set intNAME=%INTFLDR%\%NAME%.html
set newNAME=%NAME%.new

if not exist "%INTFLDR%" md "%INTFLDR%"

echo Building %NAME% : html
sed "/.. include:: commonchm.txt/d" "%1" > "%newNAME%"
rst2html.py --output-encoding CP1251 --date --time --generator "%newNAME%" "%intNAME%" || goto EXIT
del /F /Q %newNAME%

:EXIT
