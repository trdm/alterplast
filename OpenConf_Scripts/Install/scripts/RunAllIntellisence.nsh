;===========================================================================
; Сценарий установки скрипта RunAllIntellisence.vbs

Section "Интеграция Intellisence и Dots"

	SectionIn 1 2

	!insertmacro OC_STATUS "Установка скриптов | Интеграция Intellisence и Dots..."

	SetOutPath "$INSTDIR\config\scripts\Intellisence"
	File "${OC_ConfigDir}\scripts\Intellisence\RunAllIntellisence.vbs"

SectionEnd