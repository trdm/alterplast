;===========================================================================
; Сценарий установки плагина Inspector 2

Section "Inspector 2"

    SectionIn 1

	!insertmacro OC_STATUS "Установка плагинов | Inspector 2..."

    SetOutPath "$INSTDIR\config"
    File "${OC_ConfigDir}\inspector2.dll"

    SetOutPath "$INSTDIR\config\scripts"
    File "${OC_ConfigDir}\scripts\ManageInspector2.vbs"

	;;TODO
	;SetOutPath "$INSTDIR\config\docs"
	;File "${OC_ConfigDir}\docs\Inspector2.readme.txt"
	;!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "Inspector2.readme.txt" "Inspector 2 - Описание"

SectionEnd