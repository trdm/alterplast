;===========================================================================
; Сценарий установки скрипта navigator.js

Section "navigator.js (Навигатор)"

	SectionIn 1 2

	!insertmacro OC_STATUS "Установка скриптов | navigator.js..."

	SetOutPath "$INSTDIR\config\scripts\Навигация"
	File "${OC_ConfigDir}\scripts\Навигация\navigator.js"

	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\navigatorJS.readme.txt"
	!insertmacro OC_ADD_FILE_TO_STARTMENU "navigatorJS.readme.txt" "navigator.js - Справка"

SectionEnd
