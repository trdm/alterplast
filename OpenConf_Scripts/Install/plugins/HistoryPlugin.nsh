;===========================================================================
; Сценарий установки плагина История (Артур Аюханов aka artbear)	  

Section "История"

    SectionIn 1 2

	!insertmacro OC_STATUS "Установка плагинов | История..."

    SetOutPath "$INSTDIR\config"
    File "${OC_ConfigDir}\historyPlugin.dll"

	;;TODO скачать/спросить у Артура последнюю версию плагина и ридми
	;SetOutPath "$INSTDIR\config\docs"
	;File "${OC_ConfigDir}\docs\HistoryPlugin.readme.txt"
	;!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "HistoryPlugin.readme.txt" "Плагин История - Справка"

SectionEnd