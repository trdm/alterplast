@echo off

REM для добавления произвольных html-файлов к сборке 1С++ нужно не забыть добавить имена файлов в раздел "Учебник" в файле 1cpp.rst.
REM для примера можно посмотреть подключение файла TSQLInV7FirstSteps

for %%I in (kb\*.html) do call kb2chm.bat %%~nI %%~dpI %%~xI
