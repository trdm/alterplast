;===========================================================================
; Сценарий установки скрипта author.js

Section "author.js (Авторские комментарии)"

	SectionIn 1 2

	!insertmacro OC_STATUS "Установка скриптов | author.js..."

	SetOutPath "$INSTDIR\config\scripts\Редактирование"
	File "${OC_ConfigDir}\scripts\Редактирование\author.js"

	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\authorJS.readme.htm"
	!insertmacro OC_ADD_FILE_TO_STARTMENU "authorJS.readme.htm" "author.js - Справка"

SectionEnd