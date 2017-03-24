@echo off
rem vim:enc=cp866

if "-%1" == "-" goto usage
if "-%2" == "-" goto need2
if "-%3" == "-" goto need3
if "-%4" == "-" goto need4
	goto run

:need2
	call "%0" %1 .*
	goto exit

:need3
	call "%0" %1 %2 File
	goto exit

:need4
	call "%0" %1 %2 %3 %1
	goto exit

:run
	dir /b /oN %1 | perl -n -e "chomp;/%2/i&&{$p=q(%4)}&&print qq(\t%3 \"$p\\$_\"\n)" 
	goto exit

:usage
	echo Использование: flst.bat dir1 [regexp [cmd [dir2]]]
	echo Параметры:
	echo   dir1   - директория, из которой отбираются файлы (абсолютный или относительный путь)
	echo   regexp - регулярное выражение, используемое для отбора файлов по маске (по умолчанию .*)
	echo   cmd    - команда NSIS, оперирующая с файлами (File или Delete, по умолчанию - File)
	echo   dir2   - путь к файлу на целевой машине (имеет смысл для генерации команды Delete)

:exit

