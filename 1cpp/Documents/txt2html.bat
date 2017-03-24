@echo off

rem Для работы скрипта необходимо установить пакет DocUtils (http://docutils.sourceforge.net/index.html)
rem и прописать путь к нему в переменную среды PATH
rem
rem set PATH=%PATH%;S:\docutils\tools

if "%1"=="" goto EXIT

set NAME=%~n1
set INTFLDR=%~dp1html
set intNAME=%INTFLDR%\%NAME%.html

if not exist "%INTFLDR%" md "%INTFLDR%"

echo Building %NAME% : html
rst2html.py --output-encoding CP1251 --date --time --generator "%1" "%intNAME%" || goto EXIT

rem echo Building %NAME% : xml
rem rst2xml.py  --output-encoding CP1251 "%1" "%INTFLDR%\%NAME%.xml" || goto EXIT

:EXIT
