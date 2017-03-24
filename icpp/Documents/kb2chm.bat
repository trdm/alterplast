@echo off

REM set InputName=TSQLInV7FirstSteps
REM set InputDir=.\kb\
REM set InputExt=html

set InputName=%1
set InputDir=%2
set InputExt=%3

set InputPath=%InputDir%%InputName%%InputExt%
set OutDir=.\chm
set ProjectDir=.\

set intname="%InputPath%"
set hhpname="%InputDir%%InputName%.hhp"
set chmname="%OutDir%\%InputName%.chm"
cscript "%ProjectDir%simplechm.js" //nologo //d %intname% "%OutDir%"
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
call "%InputDir%%InputName%del.bat"
del "%InputDir%%InputName%del.bat"
rem exit 0
