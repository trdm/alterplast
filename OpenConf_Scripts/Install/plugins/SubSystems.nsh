;===========================================================================
; Сценарий установки плагина Подсистемы (Сергей Ушаков aka Phoenix)

Section "Подсистемы 1.3.3.0"

    SectionIn 1 2

	!insertmacro OC_STATUS "Установка плагинов | Подсистемы..."

    SetOutPath "$INSTDIR\config"
    File "${OC_ConfigDir}\SubSystems.dll"
    File "${OC_ConfigDir}\SubSystems.tlb"
    File "${OC_ConfigDir}\ClipboardHistory.exe"

    ;; Возможно это переустановка, или установка "поверх", поэтому
	;; если файл с настройками уже существует, не будем его затирать
	IfFileExists "$INSTDIR\config\SubSystemsData.mdb" +2 0
		File "${OC_ConfigDir}\SubSystemsData.mdb"

    SetOutPath "$INSTDIR\config\scripts"
    File "${OC_ConfigDir}\scripts\SubSystemsManage.vbs"

	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\SubSystems.readme.doc"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "SubSystems.readme.doc" "Подсистемы - Справка"
	File "${OC_ConfigDir}\docs\ClipboardHistory.readme.txt"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "ClipboardHistory.readme.txt" "ClipboardHistory - Описание"

SectionEnd