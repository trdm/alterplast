@echo off
rem vim:enc=cp866
 
if "-%1"=="-" goto usage
if "%2"=="-b" goto build_installer
	goto generate_nsh

:usage
	echo Использование: build.bat [-g \ Версия_сборки [-b]]
	echo Параметры:
	echo   Версия_сборки -- номер версии сборки строкой, будет добавлен к имени файла инсталлера
	echo   -b            -- если флаг задан, то будет пропущена генерация сценариев установки
	echo   -g            -- если флаг задан, то будет произведена только генерация сценариев
	goto exit

:generate_nsh
	echo Генерация сценариев установки скриптов...
	perl tools\gen_nsh.pl scripts\un.intell.nsh.pl ../Intell > scripts/un.Intell.auto.nsh
	perl tools\scripts.nsh.pl -d ..\scripts -f scripts\filters.txt -i scripts > scripts\scripts.auto.nsh
	perl tools\scripts.nsh.pl -u -d ..\scripts -f scripts\filters.txt -i scripts > scripts\un.scripts.auto.nsh	

	echo Генерация сценариев установки компонентов...
	perl tools\gen_nsh.pl system.nsh.pl ..\system > system.auto.nsh

	rem требуется только генерация кода, выходим
	if "%1"=="-g" goto exit

:build_installer
	echo Сборка инсталлятора...
	C:\progra~1\NSIS\makensis.exe /DOC_VerFile=%1 OpenConf.nsi | ..\system\fecho.exe > build.log

:exit
	echo Готово
