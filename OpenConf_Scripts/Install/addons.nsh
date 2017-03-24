;===========================================================================
;; Утилита OpenIn1C (Алексей Диркс) и сопутствующие скрипты

Section "Утилита OpenIn1C"
	SectionIn 1 2
	!insertmacro OC_STATUS "Установка OpenConf | Утилита OpenIn1C..."
	SetOutPath "$INSTDIR\config\system"
	File "${OC_ConfigDir}\system\OpenIn1C.exe"
	File "${OC_ConfigDir}\system\Register_OpenIn1C.vbs"
	SetOutPath "$INSTDIR\config\scripts\Разное"
	File "${OC_ConfigDir}\scripts\Разное\ОткрытьВПредприятии.vbs"
	ExecWait 'wscript.exe	"$INSTDIR\config\system\Register_OpenIn1C.vbs" \
							"$INSTDIR\config\system\OpenIn1C.exe"'
SectionEnd

Section "un.Утилита OpenIn1C"
	SectionIn 1 2
	!insertmacro OC_STATUS "Удаление OpenConf | Утилита OpenIn1C..."
	Delete "$INSTDIR\config\system\OpenIn1C.exe"
	Delete "$INSTDIR\config\system\Register_OpenIn1C.vbs"
	Delete "$INSTDIR\config\scripts\Разное\ОткрытьВПредприятии.vbs"
	RMDir "$INSTDIR\config\scripts\Разное"
	RMDir "$INSTDIR\config\scripts"
SectionEnd

;===========================================================================
;; Клавиатурные сокращения по умолчанию

Section "Клавиатурные сокращения"
	SectionIn 1
	SetOutPath "$INSTDIR\config\system"
	File "resources\HotKeys.reg"
	ExecWait 'regedit /s "$INSTDIR\config\system\HotKeys.reg"'
	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\HotKeysList.htm"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "HotKeysList.htm" "Список клавиатурных сокращений"
SectionEnd

Section "un.Клавиатурные сокращения"
	Delete "$INSTDIR\config\system\HotKeys.reg"	
	Delete "$INSTDIR\config\docs\HotKeysList.htm"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Список клавиатурных сокращений"
SectionEnd