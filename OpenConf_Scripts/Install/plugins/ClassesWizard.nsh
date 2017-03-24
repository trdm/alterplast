;===========================================================================
; Сценарий установки плагина ClassWizard (Артур Аюханов aka artbear)	  

Section "Classes Wizard 1.3.6"

    SectionIn 1

	!insertmacro OC_STATUS "Установка плагинов | Classes Wizard..."

    SetOutPath "$INSTDIR\config"
    File "${OC_ConfigDir}\ClassesWizard.dll"

    SetOutPath "$INSTDIR\config\scripts"
    File "${OC_ConfigDir}\scripts\wizard1cpp.vbs"

	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\ClassesWizard.readme.txt"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "ClassesWizard.readme.txt" "Classes Wizard - Описание"

SectionEnd