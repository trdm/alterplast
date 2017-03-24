;===========================================================================
; —ценарий деинсталл€ции плагина Inspector2

Section un."Inspector 2"
	
	!insertmacro OC_STATUS "”даление плагинов | Inspector 2..."

    Delete "$INSTDIR\config\inspector2.dll"
    Delete "$INSTDIR\config\scripts\ManageInspector2.vbs"

	;Delete "$INSTDIR\config\docs\Inspector2.readme.txt"
	;!insertmacro OC_DEL_DOCFILE_FROM_STARTMENU "Inspector 2 - ќписание"

SectionEnd