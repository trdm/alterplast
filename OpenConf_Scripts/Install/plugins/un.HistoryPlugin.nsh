;===========================================================================
; Сценарий деинсталляции плагина История (Артур Аюханов aka artbear)	  

Section un."История"
	
	!insertmacro OC_STATUS "Удаление плагинов | История..."

    Delete "$INSTDIR\config\historyPlugin.dll"
	Delete "$INSTDIR\config\docs\HistoryPlugin.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Плагин История - Справка"

SectionEnd