;===========================================================================
; Сценарий установки плагина FDSubst (MetaEditor)	  

Section "FDSubst (Расш. диалоги Открыть/Сохранить)"

    SectionIn 1 2

	!insertmacro OC_STATUS "Установка плагинов | FDSubst..."

    SetOutPath "$INSTDIR\config"
    File "${OC_ConfigDir}\fdsubst.dll"

	SetOutPath "$INSTDIR\config\docs"
	File "${OC_ConfigDir}\docs\FDSubst.readme.txt"
	!insertmacro OC_ADD_DOCFILE_TO_STARTMENU "FDSubst.readme.txt" "FDSubst - Описание"

SectionEnd