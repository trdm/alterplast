;===========================================================================
; —ценарий деинсталл€ции плагина FDSubst (MetaEditor)	  

Section un."FDSubst"

    SectionIn 1

	!insertmacro OC_STATUS "”даление плагинов | FDSubst..."

    Delete "$INSTDIR\config\fdsubst.dll"

	Delete "$INSTDIR\config\docs\FDSubst.readme.txt"
	!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "FDSubst - ќписание"

SectionEnd