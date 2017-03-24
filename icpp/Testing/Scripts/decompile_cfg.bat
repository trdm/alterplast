@echo off
rem ***** Decompile Config *****
REM copy /y ..\..\NEW_STRU\1cv7.md .
copy /y ..\..\1cv7.md .
gcomp.exe -d -v -D ..\Config --no-profiles